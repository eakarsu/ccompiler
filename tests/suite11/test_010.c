int printf(const char *fmt, ...);

// Combined interpreter: a small language with variables, arithmetic, conditionals, loops
// Bytecode format: [opcode, operand1, operand2] (3 words per instruction)
// Variables stored in slots 0..15

enum Op {
    OP_SET    = 1,   // SET var, imm
    OP_COPY   = 2,   // COPY dst, src
    OP_ADD    = 3,   // ADD dst, src  (dst += src)
    OP_SUB    = 4,   // SUB dst, src  (dst -= src)
    OP_MUL    = 5,   // MUL dst, src  (dst *= src)
    OP_DIV    = 6,   // DIV dst, src  (dst /= src)
    OP_MOD    = 7,   // MOD dst, src  (dst %= src)
    OP_CMP    = 8,   // CMP a, b => sets flag (-1, 0, 1)
    OP_JEQ    = 9,   // JEQ addr, _ => jump if flag==0
    OP_JNE    = 10,  // JNE addr, _ => jump if flag!=0
    OP_JLT    = 11,  // JLT addr, _ => jump if flag<0
    OP_JMP    = 12,  // JMP addr, _
    OP_PRINT  = 13,  // PRINT var, _
    OP_CALL   = 14,  // CALL addr, _ => push return address
    OP_RET    = 15,  // RET _, _ => pop return address
    OP_HALT   = 16   // HALT
};

struct Interp {
    int vars[16];
    int code[256];
    int pc;
    int flag;
    int call_stack[16];
    int csp;
    int halted;
    int inst_count;
};

void interp_init(struct Interp *ip) {
    int i;
    for (i = 0; i < 16; i++) ip->vars[i] = 0;
    for (i = 0; i < 256; i++) ip->code[i] = 0;
    for (i = 0; i < 16; i++) ip->call_stack[i] = 0;
    ip->pc = 0;
    ip->flag = 0;
    ip->csp = 0;
    ip->halted = 0;
    ip->inst_count = 0;
}

void interp_emit(struct Interp *ip, int *pos, int op, int a, int b) {
    ip->code[*pos] = op;
    ip->code[*pos + 1] = a;
    ip->code[*pos + 2] = b;
    *pos = *pos + 3;
}

void interp_run(struct Interp *ip) {
    while (!ip->halted) {
        int op = ip->code[ip->pc];
        int a  = ip->code[ip->pc + 1];
        int b  = ip->code[ip->pc + 2];
        ip->pc = ip->pc + 3;
        ip->inst_count = ip->inst_count + 1;

        if (op == OP_SET) {
            ip->vars[a] = b;
        } else if (op == OP_COPY) {
            ip->vars[a] = ip->vars[b];
        } else if (op == OP_ADD) {
            ip->vars[a] = ip->vars[a] + ip->vars[b];
        } else if (op == OP_SUB) {
            ip->vars[a] = ip->vars[a] - ip->vars[b];
        } else if (op == OP_MUL) {
            ip->vars[a] = ip->vars[a] * ip->vars[b];
        } else if (op == OP_DIV) {
            ip->vars[a] = ip->vars[a] / ip->vars[b];
        } else if (op == OP_MOD) {
            ip->vars[a] = ip->vars[a] % ip->vars[b];
        } else if (op == OP_CMP) {
            int va = ip->vars[a];
            int vb = ip->vars[b];
            if (va < vb) ip->flag = -1;
            else if (va > vb) ip->flag = 1;
            else ip->flag = 0;
        } else if (op == OP_JEQ) {
            if (ip->flag == 0) ip->pc = a;
        } else if (op == OP_JNE) {
            if (ip->flag != 0) ip->pc = a;
        } else if (op == OP_JLT) {
            if (ip->flag < 0) ip->pc = a;
        } else if (op == OP_JMP) {
            ip->pc = a;
        } else if (op == OP_PRINT) {
            printf("v%d = %d\n", a, ip->vars[a]);
        } else if (op == OP_CALL) {
            ip->call_stack[ip->csp] = ip->pc;
            ip->csp = ip->csp + 1;
            ip->pc = a;
        } else if (op == OP_RET) {
            ip->csp = ip->csp - 1;
            ip->pc = ip->call_stack[ip->csp];
        } else if (op == OP_HALT) {
            ip->halted = 1;
        }
    }
}

int main() {
    struct Interp ip;
    interp_init(&ip);

    int pos = 0;

    // Program: compute sum of 1..10 = 55
    // v0 = counter (1), v1 = limit (10), v2 = sum (0), v3 = step (1)
    interp_emit(&ip, &pos, OP_SET, 0, 1);    // v0 = 1
    interp_emit(&ip, &pos, OP_SET, 1, 10);   // v1 = 10
    interp_emit(&ip, &pos, OP_SET, 2, 0);    // v2 = 0
    interp_emit(&ip, &pos, OP_SET, 3, 1);    // v3 = 1

    // loop at pos=12: v2 += v0; v0 += v3; cmp v0, v1; jle loop
    int loop_top = pos;
    interp_emit(&ip, &pos, OP_ADD, 2, 0);    // v2 += v0
    interp_emit(&ip, &pos, OP_ADD, 0, 3);    // v0 += v3
    interp_emit(&ip, &pos, OP_CMP, 0, 1);    // cmp v0, v1
    // JLT means v0 < v1, so jump back. Also need JEQ for v0 == v1
    interp_emit(&ip, &pos, OP_JLT, loop_top, 0);
    interp_emit(&ip, &pos, OP_JEQ, loop_top, 0);

    interp_emit(&ip, &pos, OP_PRINT, 2, 0);  // print sum
    // EXPECT: v2 = 55

    // Compute fibonacci(10) = 55
    // v4=fib(n-2)=0, v5=fib(n-1)=1, v6=temp, v7=counter=2, v8=limit=10, v9=step=1
    interp_emit(&ip, &pos, OP_SET, 4, 0);    // v4 = 0
    interp_emit(&ip, &pos, OP_SET, 5, 1);    // v5 = 1
    interp_emit(&ip, &pos, OP_SET, 7, 2);    // v7 = 2
    interp_emit(&ip, &pos, OP_SET, 8, 10);   // v8 = 10
    interp_emit(&ip, &pos, OP_SET, 9, 1);    // v9 = 1

    int fib_loop = pos;
    interp_emit(&ip, &pos, OP_COPY, 6, 5);   // v6 = v5
    interp_emit(&ip, &pos, OP_ADD, 5, 4);    // v5 = v5 + v4
    interp_emit(&ip, &pos, OP_COPY, 4, 6);   // v4 = v6 (old v5)
    interp_emit(&ip, &pos, OP_ADD, 7, 9);    // v7++
    interp_emit(&ip, &pos, OP_CMP, 7, 8);    // cmp v7, v8
    interp_emit(&ip, &pos, OP_JLT, fib_loop, 0);
    interp_emit(&ip, &pos, OP_JEQ, fib_loop, 0);

    interp_emit(&ip, &pos, OP_PRINT, 5, 0);  // print fib(10)
    // EXPECT: v5 = 55

    // Test CALL/RET: call a "subroutine" that sets v10 = v0 * 2
    // Subroutine will be placed after HALT
    int sub_addr = pos + 3 * 4;  // skip SET, CALL, PRINT, HALT below

    interp_emit(&ip, &pos, OP_SET, 0, 21);   // v0 = 21
    interp_emit(&ip, &pos, OP_CALL, sub_addr, 0);
    // After return, v10 should be 42
    interp_emit(&ip, &pos, OP_PRINT, 10, 0);
    // EXPECT: v10 = 42

    interp_emit(&ip, &pos, OP_HALT, 0, 0);

    // Subroutine: v10 = v0; v10 += v0; ret
    interp_emit(&ip, &pos, OP_COPY, 10, 0);
    interp_emit(&ip, &pos, OP_ADD, 10, 0);
    interp_emit(&ip, &pos, OP_RET, 0, 0);

    interp_run(&ip);

    printf("instructions executed: %d\n", ip.inst_count);
    // EXPECT: instructions executed: 116

    printf("final v0=%d v2=%d v5=%d v10=%d\n",
           ip.vars[0], ip.vars[2], ip.vars[5], ip.vars[10]);
    // EXPECT: final v0=21 v2=55 v5=55 v10=42

    printf("interp done\n");
    // EXPECT: interp done

    return 0;
}
