int printf(const char *fmt, ...);

// Forth-like interpreter with dictionary
// Words: numbers, +, -, *, /, dup, drop, swap, over, rot, .
// User-defined words stored in dictionary

typedef struct {
    int stack[64];
    int sp;
} ForthStack;

// Dictionary entry: a word name and its definition (sequence of tokens)
// Tokens encoded as integers:
// 0-999: literal numbers
// 1000+: operations
// 2000+: call word at dictionary index (2000+idx)

enum ForthTok {
    F_ADD = 1000,
    F_SUB = 1001,
    F_MUL = 1002,
    F_DIV = 1003,
    F_DUP = 1004,
    F_DROP = 1005,
    F_SWAP = 1006,
    F_OVER = 1007,
    F_ROT = 1008,
    F_DOT = 1009,
    F_MOD = 1010,
    F_NEG = 1011,
    F_ABS = 1012
};

typedef struct {
    char name[16];
    int body[32];
    int body_len;
} DictEntry;

typedef struct {
    ForthStack ds;
    DictEntry dict[32];
    int dict_count;
    int output[32];
    int out_count;
} Forth;

void forth_init(Forth *f) {
    f->ds.sp = 0;
    f->dict_count = 0;
    f->out_count = 0;
    int i = 0;
    while (i < 64) { f->ds.stack[i] = 0; i = i + 1; }
    i = 0;
    while (i < 32) { f->output[i] = 0; i = i + 1; }
}

void forth_push(Forth *f, int val) {
    f->ds.stack[f->ds.sp] = val;
    f->ds.sp = f->ds.sp + 1;
}

int forth_pop(Forth *f) {
    f->ds.sp = f->ds.sp - 1;
    return f->ds.stack[f->ds.sp];
}

int forth_peek(Forth *f) {
    return f->ds.stack[f->ds.sp - 1];
}

void forth_exec_token(Forth *f, int tok) {
    int a;
    int b;
    int c;

    if (tok >= 0 && tok < 1000) {
        forth_push(f, tok);
    } else if (tok >= 2000) {
        int idx = tok - 2000;
        int i = 0;
        while (i < f->dict[idx].body_len) {
            forth_exec_token(f, f->dict[idx].body[i]);
            i = i + 1;
        }
    } else if (tok == F_ADD) {
        b = forth_pop(f); a = forth_pop(f);
        forth_push(f, a + b);
    } else if (tok == F_SUB) {
        b = forth_pop(f); a = forth_pop(f);
        forth_push(f, a - b);
    } else if (tok == F_MUL) {
        b = forth_pop(f); a = forth_pop(f);
        forth_push(f, a * b);
    } else if (tok == F_DIV) {
        b = forth_pop(f); a = forth_pop(f);
        forth_push(f, a / b);
    } else if (tok == F_MOD) {
        b = forth_pop(f); a = forth_pop(f);
        forth_push(f, a % b);
    } else if (tok == F_DUP) {
        a = forth_peek(f);
        forth_push(f, a);
    } else if (tok == F_DROP) {
        forth_pop(f);
    } else if (tok == F_SWAP) {
        b = forth_pop(f); a = forth_pop(f);
        forth_push(f, b);
        forth_push(f, a);
    } else if (tok == F_OVER) {
        a = f->ds.stack[f->ds.sp - 2];
        forth_push(f, a);
    } else if (tok == F_ROT) {
        c = forth_pop(f); b = forth_pop(f); a = forth_pop(f);
        forth_push(f, b);
        forth_push(f, c);
        forth_push(f, a);
    } else if (tok == F_DOT) {
        a = forth_pop(f);
        f->output[f->out_count] = a;
        f->out_count = f->out_count + 1;
    } else if (tok == F_NEG) {
        a = forth_pop(f);
        forth_push(f, -a);
    } else if (tok == F_ABS) {
        a = forth_pop(f);
        if (a < 0) a = -a;
        forth_push(f, a);
    }
}

void forth_exec(Forth *f, int *tokens, int len) {
    int i = 0;
    while (i < len) {
        forth_exec_token(f, tokens[i]);
        i = i + 1;
    }
}

int forth_define(Forth *f, char *name, int *body, int blen) {
    int idx = f->dict_count;
    int i = 0;
    while (name[i] != 0 && i < 15) {
        f->dict[idx].name[i] = name[i];
        i = i + 1;
    }
    f->dict[idx].name[i] = 0;
    i = 0;
    while (i < blen) {
        f->dict[idx].body[i] = body[i];
        i = i + 1;
    }
    f->dict[idx].body_len = blen;
    f->dict_count = f->dict_count + 1;
    return 2000 + idx;
}

int main(void) {
    Forth f;
    int tokens[32];
    int body[32];

    // Test 1: 3 4 + . => 7
    forth_init(&f);
    tokens[0] = 3; tokens[1] = 4; tokens[2] = F_ADD; tokens[3] = F_DOT;
    forth_exec(&f, tokens, 4);
    printf("t1: %d\n", f.output[0]);
    // EXPECT: t1: 7

    // Test 2: 10 3 - . => 7
    forth_init(&f);
    tokens[0] = 10; tokens[1] = 3; tokens[2] = F_SUB; tokens[3] = F_DOT;
    forth_exec(&f, tokens, 4);
    printf("t2: %d\n", f.output[0]);
    // EXPECT: t2: 7

    // Test 3: 6 7 * . => 42
    forth_init(&f);
    tokens[0] = 6; tokens[1] = 7; tokens[2] = F_MUL; tokens[3] = F_DOT;
    forth_exec(&f, tokens, 4);
    printf("t3: %d\n", f.output[0]);
    // EXPECT: t3: 42

    // Test 4: 5 dup * . => 25
    forth_init(&f);
    tokens[0] = 5; tokens[1] = F_DUP; tokens[2] = F_MUL; tokens[3] = F_DOT;
    forth_exec(&f, tokens, 4);
    printf("t4: %d\n", f.output[0]);
    // EXPECT: t4: 25

    // Test 5: 3 4 swap - . => 4-3 = 1
    forth_init(&f);
    tokens[0] = 3; tokens[1] = 4; tokens[2] = F_SWAP; tokens[3] = F_SUB; tokens[4] = F_DOT;
    forth_exec(&f, tokens, 5);
    printf("t5: %d\n", f.output[0]);
    // EXPECT: t5: 1

    // Test 6: 3 4 over . . . => pops top first: 3, then 4, then 3
    forth_init(&f);
    tokens[0] = 3; tokens[1] = 4; tokens[2] = F_OVER;
    tokens[3] = F_DOT; tokens[4] = F_DOT; tokens[5] = F_DOT;
    forth_exec(&f, tokens, 6);
    printf("t6: %d %d %d\n", f.output[0], f.output[1], f.output[2]);
    // EXPECT: t6: 3 4 3

    // Test 7: rot: 1 2 3 rot . . . => stack becomes (2 3 1), dot pops: 1, 3, 2
    forth_init(&f);
    tokens[0] = 1; tokens[1] = 2; tokens[2] = 3; tokens[3] = F_ROT;
    tokens[4] = F_DOT; tokens[5] = F_DOT; tokens[6] = F_DOT;
    forth_exec(&f, tokens, 7);
    printf("t7: %d %d %d\n", f.output[0], f.output[1], f.output[2]);
    // EXPECT: t7: 1 3 2

    // Test 8: Define SQUARE = dup *
    forth_init(&f);
    body[0] = F_DUP; body[1] = F_MUL;
    int sq_tok = forth_define(&f, "square", body, 2);
    tokens[0] = 6; tokens[1] = sq_tok; tokens[2] = F_DOT;
    forth_exec(&f, tokens, 3);
    printf("t8: %d\n", f.output[0]);
    // EXPECT: t8: 36

    // Test 9: Define CUBE = dup dup * *
    f.out_count = 0; f.ds.sp = 0;
    body[0] = F_DUP; body[1] = F_DUP; body[2] = F_MUL; body[3] = F_MUL;
    int cube_tok = forth_define(&f, "cube", body, 4);
    tokens[0] = 3; tokens[1] = cube_tok; tokens[2] = F_DOT;
    forth_exec(&f, tokens, 3);
    printf("t9: %d\n", f.output[0]);
    // EXPECT: t9: 27

    // Test 10: Define DOUBLE = dup +
    f.out_count = 0; f.ds.sp = 0;
    body[0] = F_DUP; body[1] = F_ADD;
    int dbl_tok = forth_define(&f, "double", body, 2);
    tokens[0] = 7; tokens[1] = dbl_tok; tokens[2] = F_DOT;
    forth_exec(&f, tokens, 3);
    printf("t10: %d\n", f.output[0]);
    // EXPECT: t10: 14

    // Test 11: Compose: QUAD = double double
    f.out_count = 0; f.ds.sp = 0;
    body[0] = dbl_tok; body[1] = dbl_tok;
    int quad_tok = forth_define(&f, "quad", body, 2);
    tokens[0] = 5; tokens[1] = quad_tok; tokens[2] = F_DOT;
    forth_exec(&f, tokens, 3);
    printf("t11: %d\n", f.output[0]);
    // EXPECT: t11: 20

    // Test 12: Modulo
    forth_init(&f);
    tokens[0] = 17; tokens[1] = 5; tokens[2] = F_MOD; tokens[3] = F_DOT;
    forth_exec(&f, tokens, 4);
    printf("t12: %d\n", f.output[0]);
    // EXPECT: t12: 2

    // Test 13: Negate
    forth_init(&f);
    tokens[0] = 7; tokens[1] = F_NEG; tokens[2] = F_DOT;
    forth_exec(&f, tokens, 3);
    printf("t13: %d\n", f.output[0]);
    // EXPECT: t13: -7

    // Test 14: Abs of negative
    forth_init(&f);
    tokens[0] = 7; tokens[1] = F_NEG; tokens[2] = F_ABS; tokens[3] = F_DOT;
    forth_exec(&f, tokens, 4);
    printf("t14: %d\n", f.output[0]);
    // EXPECT: t14: 7

    // Test 15: Multiple dots track output count
    forth_init(&f);
    tokens[0] = 10; tokens[1] = 20; tokens[2] = 30;
    tokens[3] = F_DOT; tokens[4] = F_DOT; tokens[5] = F_DOT;
    forth_exec(&f, tokens, 6);
    printf("t15: count=%d vals=%d,%d,%d\n", f.out_count, f.output[0], f.output[1], f.output[2]);
    // EXPECT: t15: count=3 vals=30,20,10

    // Test 16: Define SQUARE then apply twice: sq(sq(3)) = sq(9) = 81
    forth_init(&f);
    body[0] = F_DUP; body[1] = F_MUL;
    sq_tok = forth_define(&f, "square", body, 2);
    body[0] = sq_tok; body[1] = sq_tok;
    int sq2_tok = forth_define(&f, "sq2", body, 2);
    tokens[0] = 3; tokens[1] = sq2_tok; tokens[2] = F_DOT;
    forth_exec(&f, tokens, 3);
    printf("t16: %d\n", f.output[0]);
    // EXPECT: t16: 81

    // Test 17: Dictionary count
    printf("t17: dict=%d\n", f.dict_count);
    // EXPECT: t17: dict=2

    // Test 18: Complex expression (2+3)*(4+1) = 5*5 = 25
    forth_init(&f);
    tokens[0] = 2; tokens[1] = 3; tokens[2] = F_ADD;
    tokens[3] = 4; tokens[4] = 1; tokens[5] = F_ADD;
    tokens[6] = F_MUL; tokens[7] = F_DOT;
    forth_exec(&f, tokens, 8);
    printf("t18: %d\n", f.output[0]);
    // EXPECT: t18: 25

    printf("forth done\n");
    // EXPECT: forth done

    return 0;
}
