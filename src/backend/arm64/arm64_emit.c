#include "cc.h"

/* ARM64 assembly emission - naive stack-based approach */

/* Forward declarations from arm64_isel.c */
extern const char *arm64_isel_reg64(int id);
extern const char *arm64_isel_reg32(int id);
extern const char *arm64_isel_regname(int id, IRType ty);
extern const char *arm64_isel_vreg(int id, IRType ty);
extern const char *arm64_isel_cc(IROpcode op);
extern bool arm64_isel_is_float(IRType ty);

/* The current function's total frame size (set before emitting instructions).
 * Used to convert negative rbp-relative offsets to positive sp-relative offsets.
 * Frame layout:
 *   [sp]            = saved x29 (fp)
 *   [sp+8]          = saved x30 (lr)
 *   [sp+16 ... sp+total-1] = local storage
 *
 *   regalloc offset -N maps to sp offset: total + offset_from_regalloc
 *   i.e. total + (-N) = total - N
 */
static int arm64_frame_total;
static bool arm64_dynamic_align; /* true when max_alloca_align > 16 */

static const char *sym_prefix(void) {
#ifdef __APPLE__
    return "_";
#else
    return "";
#endif
}

/* Local label prefix: on macOS use "L" (not ".L"), on Linux use ".L" */
static const char *local_prefix(void) {
#ifdef __APPLE__
    return "L";
#else
    return ".L";
#endif
}

/* Convert regalloc's negative-rbp offset to positive sp offset.
 * Frame layout (growing downward, sp at bottom):
 *   sp + 0                    : local vars start
 *   sp + frame_size - 1       : local vars end
 *   sp + frame_size           : saved x29 (fp)
 *   sp + frame_size + 8       : saved x30 (lr)
 *   sp + frame_size + 16      : previous sp
 *
 * regalloc gives negative offsets: -8 is the first slot below rbp.
 * The actual position: frame_size + slot  (since frame_size maps to rbp).
 * But we want it within the local vars area (below saved fp/lr).
 * So: sp_offset = frame_size + slot
 * where frame_size = func->stack_size (from regalloc, aligned to 16).
 */
static int arm64_frame_size; /* = func->stack_size (local vars only) */
static int arm64_x8_save_offset; /* offset where x8 (return pointer) is saved */

static int sp_offset(int slot) {
    /* slot is negative (e.g. -8, -16, -24...).
     * Map to: frame_size + slot = positive offset from sp.
     * frame_size = arm64_frame_size (which equals func->stack_size). */
    return arm64_frame_size + slot;
}

/* ARM64 immediate offset limits for load/store:
 * ldrsb/ldrb/strb:  [0, 4095]    (1-byte granularity)
 * ldrsh/ldrh/strh:  [0, 8190]    (2-byte granularity)
 * ldr/str w:        [0, 16380]   (4-byte granularity)
 * ldr/str x:        [0, 32760]   (8-byte granularity)
 * add immediate:    [0, 4095]
 * For larger offsets, use x16 (scratch) to hold the offset and use
 * register-offset addressing: [sp, x16] */

static int imm_offset_max(int size) {
    switch (size) {
    case 1: return 4095;
    case 2: return 8190;
    case 4: return 16380;
    case 8: return 32760;
    default: return 4095;
    }
}

/* Emit: add dst, sp, #imm (handling large immediates) */
static void emit_add_sp_imm(FILE *out, const char *dst, int imm) {
    if (imm >= 0 && imm <= 4095) {
        fprintf(out, "\tadd\t%s, sp, #%d\n", dst, imm);
    } else {
        fprintf(out, "\tmov\tx16, #%d\n", imm);
        fprintf(out, "\tadd\t%s, sp, x16\n", dst);
    }
}

static void emit_load_slot(FILE *out, int slot, const char *reg, int size) {
    int off = sp_offset(slot);
    const char *op;
    switch (size) {
    case 1: op = "ldrsb"; break;
    case 2: op = "ldrsh"; break;
    case 4: op = "ldr"; break;
    case 8: op = "ldr"; break;
    default: op = "ldr"; break;
    }
    if (off > imm_offset_max(size)) {
        fprintf(out, "\tmov\tx16, #%d\n", off);
        fprintf(out, "\t%s\t%s, [sp, x16]\n", op, reg);
    } else {
        fprintf(out, "\t%s\t%s, [sp, #%d]\n", op, reg, off);
    }
}

static void emit_store_slot(FILE *out, int slot, const char *reg, int size) {
    int off = sp_offset(slot);
    const char *op;
    switch (size) {
    case 1: op = "strb"; break;
    case 2: op = "strh"; break;
    case 4: op = "str"; break;
    case 8: op = "str"; break;
    default: op = "str"; break;
    }
    if (off > imm_offset_max(size)) {
        fprintf(out, "\tmov\tx16, #%d\n", off);
        fprintf(out, "\t%s\t%s, [sp, x16]\n", op, reg);
    } else {
        fprintf(out, "\t%s\t%s, [sp, #%d]\n", op, reg, off);
    }
}

static void emit_load_val(FILE *out, IRInst *inst, const char *reg) {
    if (inst->op == IR_CONST_INT && inst->call_target) {
        /* Global symbol reference (string literal, global variable) */
#ifdef __APPLE__
        fprintf(out, "\tadrp\t%s, _%s@PAGE\n", reg, inst->call_target);
        fprintf(out, "\tadd\t%s, %s, _%s@PAGEOFF\n", reg, reg, inst->call_target);
#else
        fprintf(out, "\tadrp\t%s, %s\n", reg, inst->call_target);
        fprintf(out, "\tadd\t%s, %s, :lo12:%s\n", reg, reg, inst->call_target);
#endif
    } else if (inst->op == IR_CONST_INT) {
        if (inst->ival == 0) {
            fprintf(out, "\tmov\t%s, xzr\n", reg);
        } else if (inst->ival > 0 && (uint64_t)inst->ival < 65536) {
            fprintf(out, "\tmov\t%s, #%lld\n", reg, (long long)inst->ival);
        } else if (inst->ival < 0 && inst->ival >= -65536) {
            fprintf(out, "\tmov\t%s, #%lld\n", reg, (long long)inst->ival);
        } else {
            fprintf(out, "\tmov\t%s, #%lld\n", reg, (long long)(inst->ival & 0xFFFF));
            if ((uint64_t)inst->ival > 0xFFFF) {
                fprintf(out, "\tmovk\t%s, #%lld, lsl #16\n", reg,
                        (long long)((inst->ival >> 16) & 0xFFFF));
            }
            if ((uint64_t)inst->ival > 0xFFFFFFFFULL) {
                fprintf(out, "\tmovk\t%s, #%lld, lsl #32\n", reg,
                        (long long)((inst->ival >> 32) & 0xFFFF));
            }
            if ((uint64_t)inst->ival > 0xFFFFFFFFFFFFULL) {
                fprintf(out, "\tmovk\t%s, #%lld, lsl #48\n", reg,
                        (long long)((inst->ival >> 48) & 0xFFFF));
            }
        }
    } else if (inst->op == IR_ALLOCA) {
        /* Alloca: value is the address of the stack slot */
        int off = sp_offset(inst->stack_slot);
        emit_add_sp_imm(out, reg, off);
    } else if (inst->stack_slot != 0) {
        emit_load_slot(out, inst->stack_slot, reg, 8);
    }
}

static void emit_store_val(FILE *out, IRInst *inst, const char *reg) {
    if (inst->stack_slot != 0) {
        emit_store_slot(out, inst->stack_slot, reg, 8);
    }
}

/* Float-point helpers using d0/d1 registers */
static void emit_load_fval(FILE *out, IRInst *inst, const char *freg) {
    if (inst->op == IR_CONST_FLOAT) {
        /* Load float constant via integer register, then fmov */
        union { double d; uint64_t u; } cvt;
        cvt.d = inst->fval;
        if (cvt.u == 0) {
            fprintf(out, "\tfmov\t%s, xzr\n", freg);
        } else {
            fprintf(out, "\tmov\tx16, #%llu\n", (unsigned long long)(cvt.u & 0xFFFF));
            if (cvt.u > 0xFFFF)
                fprintf(out, "\tmovk\tx16, #%llu, lsl #16\n", (unsigned long long)((cvt.u >> 16) & 0xFFFF));
            if (cvt.u > 0xFFFFFFFFULL)
                fprintf(out, "\tmovk\tx16, #%llu, lsl #32\n", (unsigned long long)((cvt.u >> 32) & 0xFFFF));
            if (cvt.u > 0xFFFFFFFFFFFFULL)
                fprintf(out, "\tmovk\tx16, #%llu, lsl #48\n", (unsigned long long)((cvt.u >> 48) & 0xFFFF));
            fprintf(out, "\tfmov\t%s, x16\n", freg);
        }
    } else if (inst->stack_slot != 0) {
        int off = sp_offset(inst->stack_slot);
        if (off >= 0 && off <= 32760 && (off % 8 == 0)) {
            fprintf(out, "\tldr\t%s, [sp, #%d]\n", freg, off);
        } else {
            fprintf(out, "\tmov\tx16, #%d\n", off);
            fprintf(out, "\tadd\tx16, sp, x16\n");
            fprintf(out, "\tldr\t%s, [x16]\n", freg);
        }
    }
}

static void emit_store_fval(FILE *out, IRInst *inst, const char *freg) {
    if (inst->stack_slot != 0) {
        int off = sp_offset(inst->stack_slot);
        if (off >= 0 && off <= 32760 && (off % 8 == 0)) {
            fprintf(out, "\tstr\t%s, [sp, #%d]\n", freg, off);
        } else {
            fprintf(out, "\tmov\tx16, #%d\n", off);
            fprintf(out, "\tadd\tx16, sp, x16\n");
            fprintf(out, "\tstr\t%s, [x16]\n", freg);
        }
    }
}

/* Load an aggregate argument for passing to a function call.
 * Aggregates in our ABI are always passed by pointer:
 * - IR_ALLOCA/IR_GEP: already produce addresses, use emit_load_val
 * - IR_CALL/IR_LOAD/IR_COPY with aggregate data in stack slot:
 *   compute the address of the stack slot containing the data */
static void emit_load_agg_arg(FILE *out, IRInst *arg, const char *reg) {
    if (arg->op == IR_ALLOCA || arg->op == IR_GEP ||
        (arg->op == IR_CONST_INT && arg->call_target != NULL)) {
        /* These already produce an address */
        emit_load_val(out, arg, reg);
    } else if (arg->stack_slot != 0) {
        /* Data is inline in the stack slot; pass its address */
        emit_add_sp_imm(out, reg, sp_offset(arg->stack_slot));
    } else {
        /* Fallback */
        emit_load_val(out, arg, reg);
    }
}

/* Emit PHI copies: for each phi in target block that has src_block as
 * a predecessor, store the incoming value into the phi's stack slot.
 * To avoid the "lost copy" problem (where a PHI source is another PHI
 * that was already overwritten), we load ALL incoming values first into
 * temp registers, then store them all. */
static void emit_phi_copies(FILE *out, IRBlock *src_block, IRBlock *target) {
    /* Count phis that need copies */
    int nphi = 0;
    for (IRInst *phi = target->first; phi && phi->op == IR_PHI; phi = phi->next) {
        for (int i = 0; i < phi->phi_count; i++) {
            if (phi->phi_blocks[i] == src_block) {
                nphi++;
                break;
            }
        }
    }
    if (nphi == 0) return;

    /* Phase 1: load all incoming values into temp registers x9..x15 */
    int idx = 0;
    for (IRInst *phi = target->first; phi && phi->op == IR_PHI; phi = phi->next) {
        for (int i = 0; i < phi->phi_count; i++) {
            if (phi->phi_blocks[i] == src_block) {
                char reg[8];
                snprintf(reg, sizeof(reg), "x%d", 9 + idx);
                emit_load_val(out, phi->phi_vals[i], reg);
                idx++;
                break;
            }
        }
    }

    /* Phase 2: store from temp registers into phi stack slots */
    idx = 0;
    for (IRInst *phi = target->first; phi && phi->op == IR_PHI; phi = phi->next) {
        for (int i = 0; i < phi->phi_count; i++) {
            if (phi->phi_blocks[i] == src_block) {
                char reg[8];
                snprintf(reg, sizeof(reg), "x%d", 9 + idx);
                emit_store_val(out, phi, reg);
                idx++;
                break;
            }
        }
    }
}

static void emit_inst(IRInst *inst, FILE *out, Target *tgt) {
    (void)tgt;

    switch (inst->op) {
    case IR_CONST_INT:
    case IR_CONST_FLOAT:
        /* Constants are loaded when used */
        break;

    case IR_ALLOCA:
        if (inst->alloca_size == 0 && inst->num_operands > 0) {
            /* Dynamic alloca: runtime stack allocation */
            emit_load_val(out, inst->operands[0], "x0");
            /* Round up to 16-byte alignment: (size + 15) & ~15 */
            fprintf(out, "\tadd\tx0, x0, #15\n");
            fprintf(out, "\tand\tx0, x0, #-16\n");
            /* Subtract from SP */
            fprintf(out, "\tsub\tsp, sp, x0\n");
            /* Result is the new SP value */
            fprintf(out, "\tmov\tx0, sp\n");
            emit_store_val(out, inst, "x0");
        }
        /* Static alloca is just a stack slot. Nothing to emit here.
         * When used as an operand, emit_load_val computes sp+offset. */
        break;

    case IR_LOAD: {
        IRInst *ptr = inst->operands[0];
        int sz = inst->ty.size;
        if (sz <= 0) sz = 8;
        bool is_float = (inst->ty.kind == IR_TYPE_F32 || inst->ty.kind == IR_TYPE_F64);

        /* Aggregate load: copy entire struct from source to dest slot */
        if (inst->ty.kind == IR_TYPE_AGGREGATE && sz > 8 && inst->stack_slot != 0) {
            int dst_off = sp_offset(inst->stack_slot);
            if (ptr->op == IR_ALLOCA) {
                int src_off = sp_offset(ptr->stack_slot);
                int off = 0;
                while (off + 8 <= sz) {
                    fprintf(out, "\tldr\tx9, [sp, #%d]\n", src_off + off);
                    fprintf(out, "\tstr\tx9, [sp, #%d]\n", dst_off + off);
                    off += 8;
                }
                if (off + 4 <= sz) {
                    fprintf(out, "\tldr\tw9, [sp, #%d]\n", src_off + off);
                    fprintf(out, "\tstr\tw9, [sp, #%d]\n", dst_off + off);
                    off += 4;
                }
                if (off < sz) {
                    fprintf(out, "\tldrb\tw9, [sp, #%d]\n", src_off + off);
                    fprintf(out, "\tstrb\tw9, [sp, #%d]\n", dst_off + off);
                }
            } else {
                emit_load_val(out, ptr, "x1");
                int off = 0;
                while (off + 8 <= sz) {
                    fprintf(out, "\tldr\tx9, [x1, #%d]\n", off);
                    fprintf(out, "\tstr\tx9, [sp, #%d]\n", dst_off + off);
                    off += 8;
                }
                if (off + 4 <= sz) {
                    fprintf(out, "\tldr\tw9, [x1, #%d]\n", off);
                    fprintf(out, "\tstr\tw9, [sp, #%d]\n", dst_off + off);
                    off += 4;
                }
                if (off < sz) {
                    fprintf(out, "\tldrb\tw9, [x1, #%d]\n", off);
                    fprintf(out, "\tstrb\tw9, [sp, #%d]\n", dst_off + off);
                }
            }
            break;
        }

        /* Float load: use float registers */
        if (is_float) {
            const char *freg = (sz <= 4) ? "s0" : "d0";
            if (ptr->op == IR_ALLOCA) {
                int off = sp_offset(ptr->stack_slot);
                int maxoff = (sz <= 4) ? 16380 : 32760;
                if (off >= 0 && off <= maxoff) {
                    fprintf(out, "\tldr\t%s, [sp, #%d]\n", freg, off);
                } else {
                    fprintf(out, "\tmov\tx16, #%d\n", off);
                    fprintf(out, "\tadd\tx16, sp, x16\n");
                    fprintf(out, "\tldr\t%s, [x16]\n", freg);
                }
            } else {
                emit_load_val(out, ptr, "x0");
                fprintf(out, "\tldr\t%s, [x0]\n", freg);
            }
            /* Widen float to double for consistent storage in 8-byte slot */
            if (sz <= 4)
                fprintf(out, "\tfcvt\td0, s0\n");
            emit_store_fval(out, inst, "d0");
            break;
        }

        if (ptr->op == IR_ALLOCA) {
            /* Load directly from alloca's stack memory */
            int off = sp_offset(ptr->stack_slot);
            const char *dreg = sz <= 4 ? "w0" : "x0";
            const char *lop;
            int maxoff;
            bool is_unsigned = inst->ty.is_unsigned;
            if (sz == 1) { lop = is_unsigned ? "ldrb" : "ldrsb"; maxoff = 4095; }
            else if (sz == 2) { lop = is_unsigned ? "ldrh" : "ldrsh"; maxoff = 8190; }
            else { lop = "ldr"; maxoff = sz <= 4 ? 16380 : 32760; }
            if (off > maxoff) {
                fprintf(out, "\tmov\tx16, #%d\n", off);
                fprintf(out, "\t%s\t%s, [sp, x16]\n", lop, dreg);
            } else {
                fprintf(out, "\t%s\t%s, [sp, #%d]\n", lop, dreg, off);
            }
            if (sz <= 4) {
                if (is_unsigned)
                    fprintf(out, "\tand\tx0, x0, #0xFFFFFFFF\n");
                else
                    fprintf(out, "\tsxtw\tx0, w0\n");
            }
        } else {
            emit_load_val(out, ptr, "x0");
            bool is_unsigned = inst->ty.is_unsigned;
            if (sz == 1)
                fprintf(out, "\t%s\tw0, [x0]\n", is_unsigned ? "ldrb" : "ldrsb");
            else if (sz == 2)
                fprintf(out, "\t%s\tw0, [x0]\n", is_unsigned ? "ldrh" : "ldrsh");
            else if (sz <= 4)
                fprintf(out, "\tldr\tw0, [x0]\n");
            else
                fprintf(out, "\tldr\tx0, [x0]\n");
            if (sz <= 4) {
                if (is_unsigned)
                    fprintf(out, "\tand\tx0, x0, #0xFFFFFFFF\n");
                else
                    fprintf(out, "\tsxtw\tx0, w0\n");
            }
        }
        emit_store_val(out, inst, "x0");
        break;
    }

    case IR_STORE: {
        IRInst *val = inst->operands[0];
        IRInst *ptr = inst->operands[1];
        /* Determine store size from the value's type */
        int sz = val->ty.size;
        if (sz <= 0) sz = 8;
        bool is_float = (val->ty.kind == IR_TYPE_F32 || val->ty.kind == IR_TYPE_F64);

        if (is_float) {
            /* Float store: use float registers */
            emit_load_fval(out, val, "d0");
            /* Narrow to single if storing a float */
            if (sz <= 4)
                fprintf(out, "\tfcvt\ts0, d0\n");
            const char *freg = (sz <= 4) ? "s0" : "d0";
            if (ptr->op == IR_ALLOCA) {
                int off = sp_offset(ptr->stack_slot);
                int maxoff = (sz <= 4) ? 16380 : 32760;
                if (off >= 0 && off <= maxoff) {
                    fprintf(out, "\tstr\t%s, [sp, #%d]\n", freg, off);
                } else {
                    fprintf(out, "\tmov\tx16, #%d\n", off);
                    fprintf(out, "\tadd\tx16, sp, x16\n");
                    fprintf(out, "\tstr\t%s, [x16]\n", freg);
                }
            } else {
                emit_load_val(out, ptr, "x1");
                fprintf(out, "\tstr\t%s, [x1]\n", freg);
            }
            break;
        }

        emit_load_val(out, val, "x0");
        if (ptr->op == IR_ALLOCA) {
            /* Store directly to alloca's stack memory */
            int off = sp_offset(ptr->stack_slot);
            const char *sop;
            const char *sreg;
            int maxoff;
            if (sz == 1) { sop = "strb"; sreg = "w0"; maxoff = 4095; }
            else if (sz == 2) { sop = "strh"; sreg = "w0"; maxoff = 8190; }
            else if (sz <= 4) { sop = "str"; sreg = "w0"; maxoff = 16380; }
            else { sop = "str"; sreg = "x0"; maxoff = 32760; }
            if (off > maxoff) {
                fprintf(out, "\tmov\tx16, #%d\n", off);
                fprintf(out, "\t%s\t%s, [sp, x16]\n", sop, sreg);
            } else {
                fprintf(out, "\t%s\t%s, [sp, #%d]\n", sop, sreg, off);
            }
        } else {
            emit_load_val(out, ptr, "x1");
            if (sz == 1)
                fprintf(out, "\tstrb\tw0, [x1]\n");
            else if (sz == 2)
                fprintf(out, "\tstrh\tw0, [x1]\n");
            else if (sz <= 4)
                fprintf(out, "\tstr\tw0, [x1]\n");
            else
                fprintf(out, "\tstr\tx0, [x1]\n");
        }
        break;
    }

    case IR_GEP:
        emit_load_val(out, inst->operands[0], "x0");
        if (inst->gep_offset != 0) {
            if (inst->gep_offset > 0 && inst->gep_offset <= 4095) {
                fprintf(out, "\tadd\tx0, x0, #%d\n", inst->gep_offset);
            } else {
                fprintf(out, "\tmov\tx16, #%d\n", inst->gep_offset);
                fprintf(out, "\tadd\tx0, x0, x16\n");
            }
        }
        emit_store_val(out, inst, "x0");
        break;

    case IR_ADD: case IR_SUB: case IR_MUL: {
        emit_load_val(out, inst->operands[0], "x0");
        emit_load_val(out, inst->operands[1], "x1");
        const char *op = "add";
        if (inst->op == IR_SUB) op = "sub";
        if (inst->op == IR_MUL) op = "mul";
        /* Use 32-bit registers for 32-bit operations (correct wrapping) */
        if (inst->ty.size <= 4)
            fprintf(out, "\t%s\tw0, w0, w1\n", op);
        else
            fprintf(out, "\t%s\tx0, x0, x1\n", op);
        emit_store_val(out, inst, "x0");
        break;
    }

    case IR_SDIV:
        emit_load_val(out, inst->operands[0], "x0");
        emit_load_val(out, inst->operands[1], "x1");
        if (inst->ty.size <= 4)
            fprintf(out, "\tsdiv\tw0, w0, w1\n");
        else
            fprintf(out, "\tsdiv\tx0, x0, x1\n");
        emit_store_val(out, inst, "x0");
        break;

    case IR_UDIV:
        emit_load_val(out, inst->operands[0], "x0");
        emit_load_val(out, inst->operands[1], "x1");
        /* Use 32-bit registers for 32-bit unsigned division */
        if (inst->ty.size <= 4)
            fprintf(out, "\tudiv\tw0, w0, w1\n");
        else
            fprintf(out, "\tudiv\tx0, x0, x1\n");
        emit_store_val(out, inst, "x0");
        break;

    case IR_SREM:
        emit_load_val(out, inst->operands[0], "x0");
        emit_load_val(out, inst->operands[1], "x1");
        if (inst->ty.size <= 4) {
            fprintf(out, "\tsdiv\tw2, w0, w1\n");
            fprintf(out, "\tmsub\tw0, w2, w1, w0\n");
        } else {
            fprintf(out, "\tsdiv\tx2, x0, x1\n");
            fprintf(out, "\tmsub\tx0, x2, x1, x0\n");
        }
        emit_store_val(out, inst, "x0");
        break;

    case IR_UREM:
        emit_load_val(out, inst->operands[0], "x0");
        emit_load_val(out, inst->operands[1], "x1");
        if (inst->ty.size <= 4) {
            fprintf(out, "\tudiv\tw2, w0, w1\n");
            fprintf(out, "\tmsub\tw0, w2, w1, w0\n");
        } else {
            fprintf(out, "\tudiv\tx2, x0, x1\n");
            fprintf(out, "\tmsub\tx0, x2, x1, x0\n");
        }
        emit_store_val(out, inst, "x0");
        break;

    case IR_AND:
        emit_load_val(out, inst->operands[0], "x0");
        emit_load_val(out, inst->operands[1], "x1");
        if (inst->ty.size <= 4)
            fprintf(out, "\tand\tw0, w0, w1\n");
        else
            fprintf(out, "\tand\tx0, x0, x1\n");
        emit_store_val(out, inst, "x0");
        break;

    case IR_OR:
        emit_load_val(out, inst->operands[0], "x0");
        emit_load_val(out, inst->operands[1], "x1");
        if (inst->ty.size <= 4)
            fprintf(out, "\torr\tw0, w0, w1\n");
        else
            fprintf(out, "\torr\tx0, x0, x1\n");
        emit_store_val(out, inst, "x0");
        break;

    case IR_XOR:
        emit_load_val(out, inst->operands[0], "x0");
        emit_load_val(out, inst->operands[1], "x1");
        if (inst->ty.size <= 4)
            fprintf(out, "\teor\tw0, w0, w1\n");
        else
            fprintf(out, "\teor\tx0, x0, x1\n");
        emit_store_val(out, inst, "x0");
        break;

    case IR_SHL:
        emit_load_val(out, inst->operands[0], "x0");
        emit_load_val(out, inst->operands[1], "x1");
        if (inst->ty.size <= 4)
            fprintf(out, "\tlsl\tw0, w0, w1\n");
        else
            fprintf(out, "\tlsl\tx0, x0, x1\n");
        emit_store_val(out, inst, "x0");
        break;

    case IR_LSHR:
        emit_load_val(out, inst->operands[0], "x0");
        emit_load_val(out, inst->operands[1], "x1");
        /* Use 32-bit registers for types <= 4 bytes to get correct
         * logical shift behavior (no sign-extension artifacts) */
        if (inst->ty.size <= 4)
            fprintf(out, "\tlsr\tw0, w0, w1\n");
        else
            fprintf(out, "\tlsr\tx0, x0, x1\n");
        emit_store_val(out, inst, "x0");
        break;

    case IR_ASHR:
        emit_load_val(out, inst->operands[0], "x0");
        emit_load_val(out, inst->operands[1], "x1");
        if (inst->ty.size <= 4)
            fprintf(out, "\tasr\tw0, w0, w1\n");
        else
            fprintf(out, "\tasr\tx0, x0, x1\n");
        emit_store_val(out, inst, "x0");
        break;

    case IR_NEG:
        emit_load_val(out, inst->operands[0], "x0");
        if (inst->ty.size <= 4)
            fprintf(out, "\tneg\tw0, w0\n");
        else
            fprintf(out, "\tneg\tx0, x0\n");
        emit_store_val(out, inst, "x0");
        break;

    case IR_NOT:
        emit_load_val(out, inst->operands[0], "x0");
        if (inst->ty.size <= 4)
            fprintf(out, "\tmvn\tw0, w0\n");
        else
            fprintf(out, "\tmvn\tx0, x0\n");
        emit_store_val(out, inst, "x0");
        break;

    case IR_ICMP_EQ: case IR_ICMP_NE:
    case IR_ICMP_SLT: case IR_ICMP_SLE:
    case IR_ICMP_SGT: case IR_ICMP_SGE:
    case IR_ICMP_ULT: case IR_ICMP_ULE:
    case IR_ICMP_UGT: case IR_ICMP_UGE:
        emit_load_val(out, inst->operands[0], "x0");
        emit_load_val(out, inst->operands[1], "x1");
        /* Use 32-bit compare for 32-bit operands to preserve sign semantics */
        if (inst->operands[0]->ty.size <= 4)
            fprintf(out, "\tcmp\tw0, w1\n");
        else
            fprintf(out, "\tcmp\tx0, x1\n");
        fprintf(out, "\tcset\tx0, %s\n", arm64_isel_cc(inst->op));
        emit_store_val(out, inst, "x0");
        break;

    case IR_SEXT: {
        emit_load_val(out, inst->operands[0], "x0");
        int src_sz = inst->operands[0]->ty.size;
        if (src_sz == 1)
            fprintf(out, "\tsxtb\tx0, w0\n");
        else if (src_sz == 2)
            fprintf(out, "\tsxth\tx0, w0\n");
        else
            fprintf(out, "\tsxtw\tx0, w0\n");
        emit_store_val(out, inst, "x0");
        break;
    }

    case IR_ZEXT: {
        emit_load_val(out, inst->operands[0], "x0");
        /* Zero extend from source size to destination size */
        int src_sz = inst->operands[0]->ty.size;
        if (src_sz == 1)
            fprintf(out, "\tand\tx0, x0, #0xFF\n");
        else if (src_sz == 2)
            fprintf(out, "\tand\tx0, x0, #0xFFFF\n");
        else
            fprintf(out, "\tand\tx0, x0, #0xFFFFFFFF\n");
        emit_store_val(out, inst, "x0");
        break;
    }

    case IR_TRUNC:
        emit_load_val(out, inst->operands[0], "x0");
        if (inst->ty.size == 1) {
            fprintf(out, "\tand\tx0, x0, #0xFF\n");
        } else if (inst->ty.size == 2) {
            fprintf(out, "\tand\tx0, x0, #0xFFFF\n");
        } else if (inst->ty.size == 4) {
            fprintf(out, "\tand\tx0, x0, #0xFFFFFFFF\n");
        }
        emit_store_val(out, inst, "x0");
        break;

    case IR_INTTOPTR:
    case IR_PTRTOINT:
    case IR_BITCAST:
        /* These are just reinterpretations of the same bits */
        emit_load_val(out, inst->operands[0], "x0");
        emit_store_val(out, inst, "x0");
        break;

    /* ===== Floating-point operations ===== */
    case IR_FADD:
        emit_load_fval(out, inst->operands[0], "d0");
        emit_load_fval(out, inst->operands[1], "d1");
        fprintf(out, "\tfadd\td0, d0, d1\n");
        emit_store_fval(out, inst, "d0");
        break;

    case IR_FSUB:
        emit_load_fval(out, inst->operands[0], "d0");
        emit_load_fval(out, inst->operands[1], "d1");
        fprintf(out, "\tfsub\td0, d0, d1\n");
        emit_store_fval(out, inst, "d0");
        break;

    case IR_FMUL:
        emit_load_fval(out, inst->operands[0], "d0");
        emit_load_fval(out, inst->operands[1], "d1");
        fprintf(out, "\tfmul\td0, d0, d1\n");
        emit_store_fval(out, inst, "d0");
        break;

    case IR_FDIV:
        emit_load_fval(out, inst->operands[0], "d0");
        emit_load_fval(out, inst->operands[1], "d1");
        fprintf(out, "\tfdiv\td0, d0, d1\n");
        emit_store_fval(out, inst, "d0");
        break;

    case IR_FNEG:
        emit_load_fval(out, inst->operands[0], "d0");
        fprintf(out, "\tfneg\td0, d0\n");
        emit_store_fval(out, inst, "d0");
        break;

    case IR_FPTOSI:
        /* Float/double -> signed integer */
        emit_load_fval(out, inst->operands[0], "d0");
        fprintf(out, "\tfcvtzs\tx0, d0\n");
        emit_store_val(out, inst, "x0");
        break;

    case IR_FPTOUI:
        /* Float/double -> unsigned integer */
        emit_load_fval(out, inst->operands[0], "d0");
        fprintf(out, "\tfcvtzu\tx0, d0\n");
        emit_store_val(out, inst, "x0");
        break;

    case IR_SITOFP:
        /* Signed integer -> float/double */
        emit_load_val(out, inst->operands[0], "x0");
        /* Sign-extend smaller types to 64-bit */
        if (inst->operands[0]->ty.size <= 4)
            fprintf(out, "\tsxtw\tx0, w0\n");
        fprintf(out, "\tscvtf\td0, x0\n");
        emit_store_fval(out, inst, "d0");
        break;

    case IR_UITOFP:
        /* Unsigned integer -> float/double */
        emit_load_val(out, inst->operands[0], "x0");
        if (inst->operands[0]->ty.size <= 4)
            fprintf(out, "\tand\tx0, x0, #0xFFFFFFFF\n");
        fprintf(out, "\tucvtf\td0, x0\n");
        emit_store_fval(out, inst, "d0");
        break;

    case IR_FPEXT:
        /* float -> double: since all float values are stored as doubles
         * in 8-byte stack slots, this is just a copy. */
        emit_load_fval(out, inst->operands[0], "d0");
        emit_store_fval(out, inst, "d0");
        break;

    case IR_FPTRUNC:
        /* double -> float: since all values stored as doubles in 8-byte
         * slots, just copy. Precision loss happens when IR_STORE writes
         * to a 4-byte float alloca via fcvt+str s0. */
        emit_load_fval(out, inst->operands[0], "d0");
        emit_store_fval(out, inst, "d0");
        break;

    case IR_FCMP_OEQ:
    case IR_FCMP_ONE:
    case IR_FCMP_OLT:
    case IR_FCMP_OLE:
    case IR_FCMP_OGT:
    case IR_FCMP_OGE: {
        emit_load_fval(out, inst->operands[0], "d0");
        emit_load_fval(out, inst->operands[1], "d1");
        fprintf(out, "\tfcmp\td0, d1\n");
        const char *cc = "eq";
        switch (inst->op) {
        case IR_FCMP_OEQ: cc = "eq"; break;
        case IR_FCMP_ONE: cc = "ne"; break;
        case IR_FCMP_OLT: cc = "lt"; break;
        case IR_FCMP_OLE: cc = "le"; break;
        case IR_FCMP_OGT: cc = "gt"; break;
        case IR_FCMP_OGE: cc = "ge"; break;
        default: break;
        }
        fprintf(out, "\tcset\tx0, %s\n", cc);
        emit_store_val(out, inst, "x0");
        break;
    }

    case IR_BR:
        emit_phi_copies(out, inst->parent, inst->br_target);
        fprintf(out, "\tb\t%s%s_%d\n", local_prefix(),
                inst->parent->parent->name, inst->br_target->id);
        break;

    case IR_CONDBR: {
        emit_load_val(out, inst->operands[0], "x0");
        /* For conditional branches, we need to emit phi copies for both targets.
         * Use cbnz to jump to true_target, fall through to false_target.
         * But phi copies must happen before the branch for each target.
         * Solution: emit phi copies + branch for true path after cbnz falls through. */
        IRBlock *src = inst->parent;
        IRBlock *btrue = inst->br_true;
        IRBlock *bfalse = inst->br_false;
        const char *fname = inst->parent->parent->name;

        /* If neither target has phis, emit the simple version */
        bool true_has_phi = (btrue->first && btrue->first->op == IR_PHI);
        bool false_has_phi = (bfalse->first && bfalse->first->op == IR_PHI);

        if (!true_has_phi && !false_has_phi) {
            fprintf(out, "\tcbnz\tx0, %s%s_%d\n", local_prefix(), fname, btrue->id);
            fprintf(out, "\tb\t%s%s_%d\n", local_prefix(), fname, bfalse->id);
        } else {
            /* cbnz x0, .Ltrue_phi_copy; fall through to false path */
            fprintf(out, "\tcbnz\tx0, %s%s_%d_phi\n", local_prefix(), fname, btrue->id);
            /* False path: emit phi copies then branch */
            emit_phi_copies(out, src, bfalse);
            fprintf(out, "\tb\t%s%s_%d\n", local_prefix(), fname, bfalse->id);
            /* True path phi copy trampoline */
            fprintf(out, "%s%s_%d_phi:\n", local_prefix(), fname, btrue->id);
            emit_phi_copies(out, src, btrue);
            fprintf(out, "\tb\t%s%s_%d\n", local_prefix(), fname, btrue->id);
        }
        break;
    }

    case IR_RET:
        if (inst->num_operands > 0) {
            IRInst *retop = inst->operands[0];
            /* Aggregate return: copy struct data into registers or via x8 */
            if (retop->ty.kind == IR_TYPE_AGGREGATE && retop->ty.size > 0) {
                int retsize = retop->ty.size;
                int src_off = sp_offset(retop->stack_slot);
                if (retsize > 16) {
                    /* Large aggregate: copy to x8 pointer (saved in prologue) */
                    if (arm64_x8_save_offset == -1) {
                        /* Dynamic-alignment prologue: x8 was saved at [x29, #-8] */
                        fprintf(out, "\tldr\tx8, [x29, #-8]\n");
                    } else {
                        fprintf(out, "\tldr\tx8, [sp, #%d]\n", arm64_x8_save_offset);
                    }
                    int off = 0;
                    while (off + 8 <= retsize) {
                        fprintf(out, "\tldr\tx9, [sp, #%d]\n", src_off + off);
                        fprintf(out, "\tstr\tx9, [x8, #%d]\n", off);
                        off += 8;
                    }
                    if (off + 4 <= retsize) {
                        fprintf(out, "\tldr\tw9, [sp, #%d]\n", src_off + off);
                        fprintf(out, "\tstr\tw9, [x8, #%d]\n", off);
                        off += 4;
                    }
                    if (off < retsize) {
                        fprintf(out, "\tldrb\tw9, [sp, #%d]\n", src_off + off);
                        fprintf(out, "\tstrb\tw9, [x8, #%d]\n", off);
                    }
                } else {
                    /* Small aggregate: return in x0/x1 */
                    fprintf(out, "\tldr\tx0, [sp, #%d]\n", src_off);
                    if (retsize > 8)
                        fprintf(out, "\tldr\tx1, [sp, #%d]\n", src_off + 8);
                }
            } else if (retop->ty.kind == IR_TYPE_F32 || retop->ty.kind == IR_TYPE_F64) {
                /* Float/double return: value in d0 */
                emit_load_fval(out, retop, "d0");
            } else {
                emit_load_val(out, retop, "x0");
            }
        }
        /* Epilogue */
        if (arm64_dynamic_align) {
            /* Dynamic-alignment prologue used x29 as stable frame pointer.
             * Restore SP from x29 (which points to saved fp/lr area),
             * then pop fp/lr. */
            fprintf(out, "\tmov\tsp, x29\n");
            fprintf(out, "\tldp\tx29, x30, [sp], #16\n");
        } else {
            if (arm64_frame_total - 16 <= 504) {
                fprintf(out, "\tldp\tx29, x30, [sp, #%d]\n", arm64_frame_total - 16);
            } else if (arm64_frame_total - 16 <= 32760) {
                fprintf(out, "\tldr\tx29, [sp, #%d]\n", arm64_frame_total - 16);
                fprintf(out, "\tldr\tx30, [sp, #%d]\n", arm64_frame_total - 8);
            } else {
                fprintf(out, "\tmov\tx16, #%d\n", arm64_frame_total - 16);
                fprintf(out, "\tldr\tx29, [sp, x16]\n");
                fprintf(out, "\tadd\tx16, x16, #8\n");
                fprintf(out, "\tldr\tx30, [sp, x16]\n");
            }
            if (arm64_frame_total <= 4096) {
                fprintf(out, "\tadd\tsp, sp, #%d\n", arm64_frame_total);
            } else {
                fprintf(out, "\tmov\tx9, #%d\n", arm64_frame_total);
                fprintf(out, "\tadd\tsp, sp, x9\n");
            }
        }
        fprintf(out, "\tret\n");
        break;

    case IR_RETV:
        if (arm64_dynamic_align) {
            fprintf(out, "\tmov\tsp, x29\n");
            fprintf(out, "\tldp\tx29, x30, [sp], #16\n");
        } else {
            if (arm64_frame_total - 16 <= 504) {
                fprintf(out, "\tldp\tx29, x30, [sp, #%d]\n", arm64_frame_total - 16);
            } else if (arm64_frame_total - 16 <= 32760) {
                fprintf(out, "\tldr\tx29, [sp, #%d]\n", arm64_frame_total - 16);
                fprintf(out, "\tldr\tx30, [sp, #%d]\n", arm64_frame_total - 8);
            } else {
                fprintf(out, "\tmov\tx16, #%d\n", arm64_frame_total - 16);
                fprintf(out, "\tldr\tx29, [sp, x16]\n");
                fprintf(out, "\tadd\tx16, x16, #8\n");
                fprintf(out, "\tldr\tx30, [sp, x16]\n");
            }
            if (arm64_frame_total <= 4096) {
                fprintf(out, "\tadd\tsp, sp, #%d\n", arm64_frame_total);
            } else {
                fprintf(out, "\tmov\tx9, #%d\n", arm64_frame_total);
                fprintf(out, "\tadd\tsp, sp, x9\n");
            }
        }
        fprintf(out, "\tret\n");
        break;

    case IR_CALL: {
        /* Determine if this is an indirect call (function pointer) */
        bool indirect = (inst->call_target == NULL);
        int arg_start = indirect ? 1 : 0;  /* skip operands[0] = fptr for indirect */
        int nargs_total = inst->num_operands - arg_start;
        bool large_ret = (inst->ty.kind == IR_TYPE_AGGREGATE &&
                          inst->ty.size > 16 && inst->stack_slot != 0);

        /* For large aggregate returns, set x8 = address of destination slot.
         * Must be done BEFORE loading args (which might clobber x8). */
        if (large_ret) {
            emit_add_sp_imm(out, "x8", sp_offset(inst->stack_slot));
        }

#ifdef __APPLE__
        if (inst->is_variadic_call) {
            /* Apple ARM64 ABI: fixed args in registers, variadic args on stack. */
            int nfixed = inst->num_fixed_args;
            int nvar = nargs_total - nfixed;

            /* Load fixed args into registers */
            for (int i = 0; i < nfixed && i < nargs_total; i++) {
                char reg[8];
                snprintf(reg, sizeof(reg), "x%d", i);
                IRInst *arg = inst->operands[arg_start + i];
                if (arg->ty.kind == IR_TYPE_AGGREGATE) {
                    emit_load_agg_arg(out, arg, reg);
                } else {
                    emit_load_val(out, arg, reg);
                }
            }

            /* Load ALL variadic args before sub sp (since emit_load_val
             * uses sp-relative offsets that change after sub sp).
             * First 7 go in x9-x15, overflow stored to stack after sub. */
            int nvar_regs = nvar < 7 ? nvar : 7;
            for (int i = 0; i < nvar_regs; i++) {
                char reg[8];
                snprintf(reg, sizeof(reg), "x%d", 9 + i);
                emit_load_val(out, inst->operands[arg_start + nfixed + i], reg);
            }

            /* For overflow args (>7), we use a two-pass approach:
             * Pre-store them at negative offsets using stur before sub sp. */
            int var_size = nvar * 8;
            var_size = (var_size + 15) & ~15;

            /* Pre-store overflow variadic args BEFORE sub sp.
             * After sub sp, these will be at [new_sp, #(i*8)].
             * Before sub sp, that's [sp, #(i*8 - var_size)] = negative offset.
             * Use stur for negative offsets. */
            for (int i = 7; i < nvar; i++) {
                emit_load_val(out, inst->operands[arg_start + nfixed + i], "x17");
                int neg_off = i * 8 - var_size;
                fprintf(out, "\tstur\tx17, [sp, #%d]\n", neg_off);
            }

            if (indirect)
                emit_load_val(out, inst->operands[0], "x16");

            /* Now allocate stack space */
            if (var_size > 0)
                fprintf(out, "\tsub\tsp, sp, #%d\n", var_size);

            /* Store first 7 variadic args from temp regs */
            for (int i = 0; i < nvar_regs; i++) {
                fprintf(out, "\tstr\tx%d, [sp, #%d]\n", 9 + i, i * 8);
            }

            if (indirect)
                fprintf(out, "\tblr\tx16\n");
            else
                fprintf(out, "\tbl\t%s%s\n", sym_prefix(), inst->call_target);

            if (var_size > 0) {
                if (var_size <= 4095)
                    fprintf(out, "\tadd\tsp, sp, #%d\n", var_size);
                else {
                    fprintf(out, "\tmov\tx16, #%d\n", var_size);
                    fprintf(out, "\tadd\tsp, sp, x16\n");
                }
            }

            /* Store return value */
            if (large_ret) {
                /* Data already written to x8 destination by callee */
            } else if (inst->ty.kind == IR_TYPE_AGGREGATE && inst->ty.size > 0 && inst->stack_slot != 0) {
                int off = sp_offset(inst->stack_slot);
                fprintf(out, "\tstr\tx0, [sp, #%d]\n", off);
                if (inst->ty.size > 8)
                    fprintf(out, "\tstr\tx1, [sp, #%d]\n", off + 8);
            } else {
                emit_store_val(out, inst, "x0");
            }
        } else
#endif
        {
            /* Non-variadic: integer/pointer args in x0..x7, float/double in d0..d7.
             * Args beyond the 8th integer or 8th float go on the stack. */
            int int_arg = 0;
            int flt_arg = 0;

            /* First pass: count stack overflow args */
            int n_stack_args = 0;
            {
                int ia = 0, fa = 0;
                for (int i = arg_start; i < inst->num_operands; i++) {
                    IRInst *arg = inst->operands[i];
                    if (arg->ty.kind == IR_TYPE_F32 || arg->ty.kind == IR_TYPE_F64) {
                        if (fa >= 8) n_stack_args++;
                        fa++;
                    } else {
                        if (ia >= 8) n_stack_args++;
                        ia++;
                    }
                }
            }
            int overflow_size = n_stack_args * 8;
            overflow_size = (overflow_size + 15) & ~15; /* 16-byte align */

            /* Load register-passed args (first 8 int in x0-x7, first 8 float in d0-d7) */
            for (int i = arg_start; i < inst->num_operands; i++) {
                IRInst *arg = inst->operands[i];
                if (arg->ty.kind == IR_TYPE_F32 || arg->ty.kind == IR_TYPE_F64) {
                    if (flt_arg < 8) {
                        char freg[8];
                        snprintf(freg, sizeof(freg), "d%d", flt_arg);
                        emit_load_fval(out, arg, freg);
                    }
                    flt_arg++;
                } else if (arg->ty.kind == IR_TYPE_AGGREGATE) {
                    if (int_arg < 8) {
                        char reg[8];
                        snprintf(reg, sizeof(reg), "x%d", int_arg);
                        emit_load_agg_arg(out, arg, reg);
                    }
                    int_arg++;
                } else {
                    if (int_arg < 8) {
                        char reg[8];
                        snprintf(reg, sizeof(reg), "x%d", int_arg);
                        emit_load_val(out, arg, reg);
                    }
                    int_arg++;
                }
            }

            /* Pre-store overflow args at negative offsets before sub sp.
             * After sub sp, [sp+0] = first overflow arg, [sp+8] = second, etc. */
            if (overflow_size > 0) {
                int_arg = 0;
                flt_arg = 0;
                int stack_idx = 0;
                for (int i = arg_start; i < inst->num_operands; i++) {
                    IRInst *arg = inst->operands[i];
                    if (arg->ty.kind == IR_TYPE_F32 || arg->ty.kind == IR_TYPE_F64) {
                        if (flt_arg >= 8) {
                            emit_load_fval(out, arg, "d16");
                            int neg_off = stack_idx * 8 - overflow_size;
                            fprintf(out, "\tstur\td16, [sp, #%d]\n", neg_off);
                            stack_idx++;
                        }
                        flt_arg++;
                    } else {
                        if (int_arg >= 8) {
                            emit_load_val(out, arg, "x17");
                            int neg_off = stack_idx * 8 - overflow_size;
                            fprintf(out, "\tstur\tx17, [sp, #%d]\n", neg_off);
                            stack_idx++;
                        }
                        int_arg++;
                    }
                }
                fprintf(out, "\tsub\tsp, sp, #%d\n", overflow_size);
            }

            if (indirect) {
                emit_load_val(out, inst->operands[0], "x16");
                fprintf(out, "\tblr\tx16\n");
            } else {
                fprintf(out, "\tbl\t%s%s\n", sym_prefix(), inst->call_target);
            }

            /* Deallocate stack overflow space */
            if (overflow_size > 0) {
                if (overflow_size <= 4095)
                    fprintf(out, "\tadd\tsp, sp, #%d\n", overflow_size);
                else {
                    fprintf(out, "\tmov\tx16, #%d\n", overflow_size);
                    fprintf(out, "\tadd\tsp, sp, x16\n");
                }
            }
            /* Store return value */
            if (large_ret) {
                /* Data already written to x8 destination by callee */
            } else if (inst->ty.kind == IR_TYPE_AGGREGATE && inst->ty.size > 0 && inst->stack_slot != 0) {
                int off = sp_offset(inst->stack_slot);
                fprintf(out, "\tstr\tx0, [sp, #%d]\n", off);
                if (inst->ty.size > 8)
                    fprintf(out, "\tstr\tx1, [sp, #%d]\n", off + 8);
            } else if (inst->ty.kind == IR_TYPE_F32 || inst->ty.kind == IR_TYPE_F64) {
                /* Float/double return value comes back in d0 */
                emit_store_fval(out, inst, "d0");
            } else {
                emit_store_val(out, inst, "x0");
            }
        }
        break;
    }

    case IR_COPY:
        /* Parameter copies have no operands - handled by prologue spill */
        if (inst->num_operands > 0 && inst->operands[0]) {
            emit_load_val(out, inst->operands[0], "x0");
            emit_store_val(out, inst, "x0");
        }
        break;

    case IR_PHI:
        /* PHI nodes resolved by copies during SSA destruction */
        break;

    case IR_MEMCPY: {
        /* memcpy dest, src, size (size in inst->ival)
         * Both operands should resolve to memory addresses.
         * - Allocas/GEPs: emit_load_val gives the address directly.
         * - CALL with aggregate return: data is inline in stack slot,
         *   so use the slot address.
         * - Other (COPY/params, LOAD, etc.): value IS a pointer,
         *   so emit_load_val loads the pointer from the slot. */
        if (inst->num_operands >= 2) {
            int size = (int)inst->ival;
            IRInst *dst = inst->operands[0];
            IRInst *src = inst->operands[1];

            /* Get destination address */
            if (dst->op == IR_CALL && dst->ty.kind == IR_TYPE_AGGREGATE && dst->stack_slot != 0) {
                emit_add_sp_imm(out, "x0", sp_offset(dst->stack_slot));
            } else {
                emit_load_val(out, dst, "x0");
            }

            /* Get source address */
            if (src->op == IR_CALL && src->ty.kind == IR_TYPE_AGGREGATE && src->stack_slot != 0) {
                emit_add_sp_imm(out, "x1", sp_offset(src->stack_slot));
            } else {
                emit_load_val(out, src, "x1");
            }

            /* Emit inline copy using 8-byte, 4-byte, 2-byte, 1-byte chunks */
            int off = 0;
            while (off + 8 <= size) {
                fprintf(out, "\tldr\tx9, [x1, #%d]\n", off);
                fprintf(out, "\tstr\tx9, [x0, #%d]\n", off);
                off += 8;
            }
            if (off + 4 <= size) {
                fprintf(out, "\tldr\tw9, [x1, #%d]\n", off);
                fprintf(out, "\tstr\tw9, [x0, #%d]\n", off);
                off += 4;
            }
            if (off + 2 <= size) {
                fprintf(out, "\tldrh\tw9, [x1, #%d]\n", off);
                fprintf(out, "\tstrh\tw9, [x0, #%d]\n", off);
                off += 2;
            }
            if (off < size) {
                fprintf(out, "\tldrb\tw9, [x1, #%d]\n", off);
                fprintf(out, "\tstrb\tw9, [x0, #%d]\n", off);
            }
        }
        break;
    }

    default:
        fprintf(out, "\t// unhandled IR op %d\n", inst->op);
        break;
    }
}

void arm64_emit_func(IRFunc *func, FILE *out, Target *tgt, Arena *a) {
    (void)a;
    int frame = func->stack_size;
    /* Check if this function returns a large aggregate via x8 pointer */
    bool large_agg_ret = (func->ret_type.kind == IR_TYPE_AGGREGATE &&
                          func->ret_type.size > 16);
    int extra = large_agg_ret ? 8 : 0;  /* space to save x8 */
    int total = frame + 16 + extra;  /* +16 for saved fp/lr, +8 for x8 if needed */
    /* Determine whether we need dynamic stack alignment.
     * The ARM64 ABI only guarantees SP is 16-byte aligned on function entry.
     * If any local variable requires alignment > 16 bytes, a simple static
     * "sub sp, sp, #N" cannot guarantee alignment because original_sp % 32
     * (or higher) is unpredictable.  In that case we use a dynamic-alignment
     * prologue modelled after what Clang generates:
     *   stp x29, x30, [sp, #-16]!   ; push fp/lr, sp now 16-aligned
     *   mov x29, sp                  ; stable frame pointer for epilogue
     *   sub x9, sp, #<frame_size+extra_padding>
     *   and sp, x9, #~(align-1)     ; dynamically align sp
     * Locals are accessed via SP (which is now at the correct alignment).
     * The epilogue uses "mov sp, x29; ldp x29, x30, [sp], #16" to restore. */
    int max_align = func->max_alloca_align > 16 ? func->max_alloca_align : 16;
    bool dyn_align = (max_align > 16);

    /* Align total to at least 16. */
    total = (total + 15) & ~15;

    /* Set global frame size for sp_offset() */
    arm64_frame_total = total;
    arm64_frame_size = frame;
    arm64_dynamic_align = dyn_align;
    /* x8 is saved just below fp/lr save area (only for static layout) */
    arm64_x8_save_offset = (large_agg_ret && !dyn_align) ? (total - 16 - 8) : 0;

    fprintf(out, "\t.globl\t%s%s\n", sym_prefix(), func->name);
#ifdef __APPLE__
    fprintf(out, "\t.p2align\t2\n");
#else
    fprintf(out, "\t.type\t%s, %%function\n", func->name);
#endif
    fprintf(out, "%s%s:\n", sym_prefix(), func->name);

    if (dyn_align) {
        /* Dynamic-alignment prologue:
         * 1. Push fp/lr with pre-decrement (SP stays 16-aligned).
         * 2. Set x29 = sp (stable frame pointer for epilogue restoration).
         * 3. Compute candidate SP by subtracting the local-var area plus
         *    enough padding for the alignment mask to work.
         * 4. AND sp with ~(align-1) to align down.
         *
         * We add (max_align - 16) bytes of padding to ensure there is
         * always enough room after alignment.  frame already accounts
         * for the slot sizes; the AND can only decrease SP, so we need
         * the candidate to be at most (max_align - 1) bytes above the
         * true minimum.  Adding max_align is conservative and correct. */
        fprintf(out, "\tstp\tx29, x30, [sp, #-16]!\n");
        fprintf(out, "\tmov\tx29, sp\n");
        /* sub x9, sp, #(frame + max_align) — allocate enough for alignment */
        int dyn_sub = frame + max_align;
        if (dyn_sub <= 4095) {
            fprintf(out, "\tsub\tx9, sp, #%d\n", dyn_sub);
        } else {
            fprintf(out, "\tmov\tx16, #%d\n", dyn_sub);
            fprintf(out, "\tsub\tx9, sp, x16\n");
        }
        /* and sp, x9, #~(max_align-1) */
        long mask = ~((long)(max_align - 1));
        fprintf(out, "\tand\tsp, x9, #0x%lx\n", mask & 0xFFFFFFFFFFFFFFFL);

        /* arm64_frame_size is used by sp_offset() for local var access.
         * After the AND, sp is the new aligned base.  The regalloc slots
         * were computed assuming the base is at 0, so sp_offset(slot) =
         * frame + slot gives the correct positive offset from the aligned SP.
         * This is unchanged from the static layout. */

        /* Save x8 for large aggregate returns */
        if (large_agg_ret) {
            /* Save x8 at a fixed offset below x29 (safe, above aligned sp) */
            fprintf(out, "\tstr\tx8, [x29, #-8]\n");
            arm64_x8_save_offset = -1; /* sentinel: stored relative to x29 */
        }
    } else {
        /* Static prologue: allocate fixed frame, save fp/lr at top */
        if (total <= 4096) {
            fprintf(out, "\tsub\tsp, sp, #%d\n", total);
        } else {
            fprintf(out, "\tmov\tx9, #%d\n", total);
            fprintf(out, "\tsub\tsp, sp, x9\n");
        }
        if (total - 16 <= 504) {
            fprintf(out, "\tstp\tx29, x30, [sp, #%d]\n", total - 16);
        } else if (total - 16 <= 32760) {
            fprintf(out, "\tstr\tx29, [sp, #%d]\n", total - 16);
            fprintf(out, "\tstr\tx30, [sp, #%d]\n", total - 8);
        } else {
            fprintf(out, "\tmov\tx16, #%d\n", total - 16);
            fprintf(out, "\tstr\tx29, [sp, x16]\n");
            fprintf(out, "\tadd\tx16, x16, #8\n");
            fprintf(out, "\tstr\tx30, [sp, x16]\n");
        }
        emit_add_sp_imm(out, "x29", total - 16);

        /* Save x8 (indirect return pointer) for large aggregate returns */
        if (large_agg_ret) {
            fprintf(out, "\tstr\tx8, [sp, #%d]\n", arm64_x8_save_offset);
        }
    }

    /* Spill parameters: integer/pointer params come in x0..x7,
     * float/double params come in d0..d7 per the ARM64 ABI.
     * Parameters beyond the 8th integer or 8th float arrive on the
     * caller's stack at [x29, #16], [x29, #24], etc. */
    {
        int int_arg = 0;
        int flt_arg = 0;
        int stack_param_idx = 0;
        for (int i = 0; i < func->num_params; i++) {
            IRInst *param = func->params[i];
            bool is_float = (param->ty.kind == IR_TYPE_F32 || param->ty.kind == IR_TYPE_F64);
            bool on_stack = is_float ? (flt_arg >= 8) : (int_arg >= 8);

            if (param->stack_slot != 0) {
                if (on_stack) {
                    /* Load from caller's stack frame into x17, then store to local slot */
                    fprintf(out, "\tldr\tx17, [x29, #%d]\n", 16 + stack_param_idx * 8);
                    emit_store_slot(out, param->stack_slot, "x17", 8);
                } else if (is_float) {
                    /* Float/double param: spill from dN to stack slot */
                    int off = sp_offset(param->stack_slot);
                    if (off >= 0 && off <= 32760 && (off % 8 == 0)) {
                        fprintf(out, "\tstr\td%d, [sp, #%d]\n", flt_arg, off);
                    } else {
                        fprintf(out, "\tmov\tx16, #%d\n", off);
                        fprintf(out, "\tadd\tx16, sp, x16\n");
                        fprintf(out, "\tstr\td%d, [x16]\n", flt_arg);
                    }
                } else {
                    char reg[8];
                    snprintf(reg, sizeof(reg), "x%d", int_arg);
                    emit_store_slot(out, param->stack_slot, reg, 8);
                }
            }

            if (on_stack) stack_param_idx++;
            if (is_float) flt_arg++;
            else int_arg++;
        }
    }

    /* Emit basic blocks */
    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        fprintf(out, "%s%s_%d:\n", local_prefix(), func->name, bb->id);
        for (IRInst *inst = bb->first; inst; inst = inst->next) {
            emit_inst(inst, out, tgt);
        }
    }

    fprintf(out, "\n");
}
