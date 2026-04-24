#include "cc.h"

/* ARM64 instruction selection helpers */

const char *arm64_isel_reg64(int id) {
    static const char *names[] = {
        "x0","x1","x2","x3","x4","x5","x6","x7",
        "x8","x9","x10","x11","x12","x13","x14","x15",
        "x16","x17","x18","x19","x20","x21","x22","x23",
        "x24","x25","x26","x27","x28","x29","x30","sp"
    };
    if (id >= 0 && id < 32) return names[id];
    return "x??";
}

const char *arm64_isel_reg32(int id) {
    static const char *names[] = {
        "w0","w1","w2","w3","w4","w5","w6","w7",
        "w8","w9","w10","w11","w12","w13","w14","w15",
        "w16","w17","w18","w19","w20","w21","w22","w23",
        "w24","w25","w26","w27","w28","w29","w30","wsp"
    };
    if (id >= 0 && id < 32) return names[id];
    return "w??";
}

const char *arm64_isel_regname(int id, IRType ty) {
    if (ty.size <= 4) return arm64_isel_reg32(id);
    return arm64_isel_reg64(id);
}

const char *arm64_isel_vreg(int id, IRType ty) {
    static char buf[8];
    char prefix = 'd';
    if (ty.kind == IR_TYPE_F32) prefix = 's';
    snprintf(buf, sizeof(buf), "%c%d", prefix, id);
    return buf;
}

const char *arm64_isel_cc(IROpcode op) {
    switch (op) {
    case IR_ICMP_EQ: return "eq";
    case IR_ICMP_NE: return "ne";
    case IR_ICMP_SLT: return "lt";
    case IR_ICMP_SLE: return "le";
    case IR_ICMP_SGT: return "gt";
    case IR_ICMP_SGE: return "ge";
    case IR_ICMP_ULT: return "lo";
    case IR_ICMP_ULE: return "ls";
    case IR_ICMP_UGT: return "hi";
    case IR_ICMP_UGE: return "hs";
    case IR_FCMP_OEQ: return "eq";
    case IR_FCMP_ONE: return "ne";
    case IR_FCMP_OLT: return "mi";
    case IR_FCMP_OLE: return "ls";
    case IR_FCMP_OGT: return "gt";
    case IR_FCMP_OGE: return "ge";
    default: return "al";
    }
}

bool arm64_isel_is_float(IRType ty) {
    return ty.kind == IR_TYPE_F32 || ty.kind == IR_TYPE_F64;
}
