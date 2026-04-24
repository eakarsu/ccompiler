int printf(const char *fmt, ...);

// Simple bytecode compiler and executor
// Compiles simple arithmetic expressions to bytecode, then executes

enum BCOp {
    BC_PUSH,
    BC_ADD,
    BC_SUB,
    BC_MUL,
    BC_DIV,
    BC_NEG,
    BC_STORE,
    BC_LOAD,
    BC_RET
};

typedef struct {
    int code[128];
    int len;
} Bytecode;

typedef struct {
    int stack[32];
    int sp;
    int locals[8];
} Executor;

void bc_init(Bytecode *bc) {
    bc->len = 0;
    int i = 0;
    while (i < 128) { bc->code[i] = 0; i = i + 1; }
}

void bc_emit(Bytecode *bc, int op) {
    bc->code[bc->len] = op;
    bc->len = bc->len + 1;
}

void bc_emit2(Bytecode *bc, int op, int arg) {
    bc->code[bc->len] = op;
    bc->len = bc->len + 1;
    bc->code[bc->len] = arg;
    bc->len = bc->len + 1;
}

void exec_init(Executor *ex) {
    ex->sp = 0;
    int i = 0;
    while (i < 32) { ex->stack[i] = 0; i = i + 1; }
    i = 0;
    while (i < 8) { ex->locals[i] = 0; i = i + 1; }
}

void exec_push(Executor *ex, int val) {
    ex->stack[ex->sp] = val;
    ex->sp = ex->sp + 1;
}

int exec_pop(Executor *ex) {
    ex->sp = ex->sp - 1;
    return ex->stack[ex->sp];
}

int exec_run(Executor *ex, Bytecode *bc) {
    int pc = 0;
    int a;
    int b;

    while (pc < bc->len) {
        int op = bc->code[pc];
        pc = pc + 1;

        if (op == BC_PUSH) {
            a = bc->code[pc];
            pc = pc + 1;
            exec_push(ex, a);
        } else if (op == BC_ADD) {
            b = exec_pop(ex);
            a = exec_pop(ex);
            exec_push(ex, a + b);
        } else if (op == BC_SUB) {
            b = exec_pop(ex);
            a = exec_pop(ex);
            exec_push(ex, a - b);
        } else if (op == BC_MUL) {
            b = exec_pop(ex);
            a = exec_pop(ex);
            exec_push(ex, a * b);
        } else if (op == BC_DIV) {
            b = exec_pop(ex);
            a = exec_pop(ex);
            exec_push(ex, a / b);
        } else if (op == BC_NEG) {
            a = exec_pop(ex);
            exec_push(ex, -a);
        } else if (op == BC_STORE) {
            a = bc->code[pc]; pc = pc + 1;
            b = exec_pop(ex);
            ex->locals[a] = b;
        } else if (op == BC_LOAD) {
            a = bc->code[pc]; pc = pc + 1;
            exec_push(ex, ex->locals[a]);
        } else if (op == BC_RET) {
            return exec_pop(ex);
        }
    }
    if (ex->sp > 0) return exec_pop(ex);
    return 0;
}

// Simple compiler for expressions like "3+4", "2*5+1"
// Only handles single digits and +, -, *, /
// Compiles to bytecode using simple left-to-right evaluation
void compile_expr(Bytecode *bc, char *expr) {
    int i = 0;
    int pending_op = -1;

    while (expr[i] != 0) {
        char c = expr[i];

        if (c >= '0' && c <= '9') {
            // Parse number
            int num = 0;
            while (expr[i] != 0 && expr[i] >= '0' && expr[i] <= '9') {
                num = num * 10 + (expr[i] - '0');
                i = i + 1;
            }
            bc_emit2(bc, BC_PUSH, num);

            if (pending_op >= 0) {
                bc_emit(bc, pending_op);
                pending_op = -1;
            }
            continue;
        } else if (c == '+') {
            pending_op = BC_ADD;
        } else if (c == '-') {
            pending_op = BC_SUB;
        } else if (c == '*') {
            pending_op = BC_MUL;
        } else if (c == '/') {
            pending_op = BC_DIV;
        }
        i = i + 1;
    }
}

int compile_and_run(char *expr) {
    Bytecode bc;
    Executor ex;
    bc_init(&bc);
    exec_init(&ex);
    compile_expr(&bc, expr);
    bc_emit(&bc, BC_RET);
    return exec_run(&ex, &bc);
}

int main(void) {
    Bytecode bc;
    Executor ex;
    int result;

    // Test 1: Direct bytecode: push 3, push 4, add
    bc_init(&bc);
    exec_init(&ex);
    bc_emit2(&bc, BC_PUSH, 3);
    bc_emit2(&bc, BC_PUSH, 4);
    bc_emit(&bc, BC_ADD);
    bc_emit(&bc, BC_RET);
    result = exec_run(&ex, &bc);
    printf("t1: %d\n", result);
    // EXPECT: t1: 7

    // Test 2: push 10, push 3, sub
    bc_init(&bc);
    exec_init(&ex);
    bc_emit2(&bc, BC_PUSH, 10);
    bc_emit2(&bc, BC_PUSH, 3);
    bc_emit(&bc, BC_SUB);
    bc_emit(&bc, BC_RET);
    result = exec_run(&ex, &bc);
    printf("t2: %d\n", result);
    // EXPECT: t2: 7

    // Test 3: push 6, push 7, mul
    bc_init(&bc);
    exec_init(&ex);
    bc_emit2(&bc, BC_PUSH, 6);
    bc_emit2(&bc, BC_PUSH, 7);
    bc_emit(&bc, BC_MUL);
    bc_emit(&bc, BC_RET);
    result = exec_run(&ex, &bc);
    printf("t3: %d\n", result);
    // EXPECT: t3: 42

    // Test 4: store and load
    bc_init(&bc);
    exec_init(&ex);
    bc_emit2(&bc, BC_PUSH, 99);
    bc_emit2(&bc, BC_STORE, 0);
    bc_emit2(&bc, BC_LOAD, 0);
    bc_emit(&bc, BC_RET);
    result = exec_run(&ex, &bc);
    printf("t4: %d\n", result);
    // EXPECT: t4: 99

    // Test 5: neg
    bc_init(&bc);
    exec_init(&ex);
    bc_emit2(&bc, BC_PUSH, 7);
    bc_emit(&bc, BC_NEG);
    bc_emit(&bc, BC_RET);
    result = exec_run(&ex, &bc);
    printf("t5: %d\n", result);
    // EXPECT: t5: -7

    // Test 6: Compile "3+4" => left-to-right, no precedence
    result = compile_and_run("3+4");
    printf("t6: %d\n", result);
    // EXPECT: t6: 7

    // Test 7: Compile "10-3"
    result = compile_and_run("10-3");
    printf("t7: %d\n", result);
    // EXPECT: t7: 7

    // Test 8: Compile "6*7"
    result = compile_and_run("6*7");
    printf("t8: %d\n", result);
    // EXPECT: t8: 42

    // Test 9: Compile "100/4"
    result = compile_and_run("100/4");
    printf("t9: %d\n", result);
    // EXPECT: t9: 25

    // Test 10: Chain "2+3+4" => left-to-right: 2, push 3, add(=5), push 4, add(=9)
    result = compile_and_run("2+3+4");
    printf("t10: %d\n", result);
    // EXPECT: t10: 9

    // Test 11: Local variables in bytecode: a=5, b=3, a*b+a
    bc_init(&bc);
    exec_init(&ex);
    bc_emit2(&bc, BC_PUSH, 5);
    bc_emit2(&bc, BC_STORE, 0);  // a=5
    bc_emit2(&bc, BC_PUSH, 3);
    bc_emit2(&bc, BC_STORE, 1);  // b=3
    bc_emit2(&bc, BC_LOAD, 0);   // push a
    bc_emit2(&bc, BC_LOAD, 1);   // push b
    bc_emit(&bc, BC_MUL);        // a*b = 15
    bc_emit2(&bc, BC_LOAD, 0);   // push a
    bc_emit(&bc, BC_ADD);        // 15+5 = 20
    bc_emit(&bc, BC_RET);
    result = exec_run(&ex, &bc);
    printf("t11: %d\n", result);
    // EXPECT: t11: 20

    // Test 12: Bytecode length tracking
    bc_init(&bc);
    bc_emit2(&bc, BC_PUSH, 1);
    bc_emit2(&bc, BC_PUSH, 2);
    bc_emit(&bc, BC_ADD);
    printf("t12: len=%d\n", bc.len);
    // EXPECT: t12: len=5

    printf("bytecode compiler done\n");
    // EXPECT: bytecode compiler done

    return 0;
}
