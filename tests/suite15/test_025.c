int printf(const char *fmt, ...);

// Type checker for simple expressions
// Supports int, bool, char types and checks binary operation compatibility

enum Type {
    TYPE_INT,
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_ERROR,
    TYPE_VOID
};

enum ExprKind {
    EXPR_LIT_INT,
    EXPR_LIT_BOOL,
    EXPR_LIT_CHAR,
    EXPR_ADD,
    EXPR_SUB,
    EXPR_MUL,
    EXPR_DIV,
    EXPR_EQ,
    EXPR_LT,
    EXPR_AND,
    EXPR_OR,
    EXPR_NOT
};

struct TypedExpr {
    int kind;
    int type;
    int value;
    int left;   // index into expr pool
    int right;  // index into expr pool
};

struct TypeChecker {
    struct TypedExpr exprs[64];
    int count;
    int errors;
    char error_msgs[8][64];
    int error_count;
};

void tc_init(struct TypeChecker *tc) {
    tc->count = 0;
    tc->errors = 0;
    tc->error_count = 0;
}

void add_error(struct TypeChecker *tc, char *msg) {
    if (tc->error_count < 8) {
        int i = 0;
        while (msg[i] != 0 && i < 63) {
            tc->error_msgs[tc->error_count][i] = msg[i];
            i = i + 1;
        }
        tc->error_msgs[tc->error_count][i] = 0;
        tc->error_count = tc->error_count + 1;
    }
    tc->errors = tc->errors + 1;
}

char *type_name(int t) {
    if (t == TYPE_INT) return "int";
    if (t == TYPE_BOOL) return "bool";
    if (t == TYPE_CHAR) return "char";
    if (t == TYPE_ERROR) return "error";
    if (t == TYPE_VOID) return "void";
    return "unknown";
}

int make_int_lit(struct TypeChecker *tc, int val) {
    int idx = tc->count;
    tc->exprs[idx].kind = EXPR_LIT_INT;
    tc->exprs[idx].type = TYPE_INT;
    tc->exprs[idx].value = val;
    tc->exprs[idx].left = -1;
    tc->exprs[idx].right = -1;
    tc->count = tc->count + 1;
    return idx;
}

int make_bool_lit(struct TypeChecker *tc, int val) {
    int idx = tc->count;
    tc->exprs[idx].kind = EXPR_LIT_BOOL;
    tc->exprs[idx].type = TYPE_BOOL;
    tc->exprs[idx].value = val;
    tc->exprs[idx].left = -1;
    tc->exprs[idx].right = -1;
    tc->count = tc->count + 1;
    return idx;
}

int make_char_lit(struct TypeChecker *tc, int val) {
    int idx = tc->count;
    tc->exprs[idx].kind = EXPR_LIT_CHAR;
    tc->exprs[idx].type = TYPE_CHAR;
    tc->exprs[idx].value = val;
    tc->exprs[idx].left = -1;
    tc->exprs[idx].right = -1;
    tc->count = tc->count + 1;
    return idx;
}

int check_arithmetic(struct TypeChecker *tc, int kind, int left, int right) {
    int lt = tc->exprs[left].type;
    int rt = tc->exprs[right].type;
    int idx = tc->count;

    tc->exprs[idx].kind = kind;
    tc->exprs[idx].left = left;
    tc->exprs[idx].right = right;

    // Both must be int or char (char promotes to int)
    if ((lt == TYPE_INT || lt == TYPE_CHAR) && (rt == TYPE_INT || rt == TYPE_CHAR)) {
        tc->exprs[idx].type = TYPE_INT;
    } else {
        tc->exprs[idx].type = TYPE_ERROR;
        add_error(tc, "arith requires int");
    }

    tc->count = tc->count + 1;
    return idx;
}

int check_comparison(struct TypeChecker *tc, int kind, int left, int right) {
    int lt = tc->exprs[left].type;
    int rt = tc->exprs[right].type;
    int idx = tc->count;

    tc->exprs[idx].kind = kind;
    tc->exprs[idx].left = left;
    tc->exprs[idx].right = right;

    if ((lt == TYPE_INT || lt == TYPE_CHAR) && (rt == TYPE_INT || rt == TYPE_CHAR)) {
        tc->exprs[idx].type = TYPE_BOOL;
    } else if (lt == TYPE_BOOL && rt == TYPE_BOOL && kind == EXPR_EQ) {
        tc->exprs[idx].type = TYPE_BOOL;
    } else {
        tc->exprs[idx].type = TYPE_ERROR;
        add_error(tc, "bad comparison");
    }

    tc->count = tc->count + 1;
    return idx;
}

int check_logical(struct TypeChecker *tc, int kind, int left, int right) {
    int lt = tc->exprs[left].type;
    int rt = tc->exprs[right].type;
    int idx = tc->count;

    tc->exprs[idx].kind = kind;
    tc->exprs[idx].left = left;
    tc->exprs[idx].right = right;

    if (lt == TYPE_BOOL && rt == TYPE_BOOL) {
        tc->exprs[idx].type = TYPE_BOOL;
    } else {
        tc->exprs[idx].type = TYPE_ERROR;
        add_error(tc, "logical requires bool");
    }

    tc->count = tc->count + 1;
    return idx;
}

int check_not(struct TypeChecker *tc, int child) {
    int ct = tc->exprs[child].type;
    int idx = tc->count;

    tc->exprs[idx].kind = EXPR_NOT;
    tc->exprs[idx].left = child;
    tc->exprs[idx].right = -1;

    if (ct == TYPE_BOOL) {
        tc->exprs[idx].type = TYPE_BOOL;
    } else {
        tc->exprs[idx].type = TYPE_ERROR;
        add_error(tc, "not requires bool");
    }

    tc->count = tc->count + 1;
    return idx;
}

int main() {
    struct TypeChecker tc;

    printf("=== TypeCheck Test 1: Valid arithmetic ===\n");
    // EXPECT: === TypeCheck Test 1: Valid arithmetic ===
    tc_init(&tc);
    int a = make_int_lit(&tc, 5);
    int b = make_int_lit(&tc, 3);
    int c = check_arithmetic(&tc, EXPR_ADD, a, b);
    printf("5+3 type: %s\n", type_name(tc.exprs[c].type));
    // EXPECT: 5+3 type: int
    printf("errors: %d\n", tc.errors);
    // EXPECT: errors: 0

    printf("=== TypeCheck Test 2: Char + Int ===\n");
    // EXPECT: === TypeCheck Test 2: Char + Int ===
    tc_init(&tc);
    int ch = make_char_lit(&tc, 65);
    int num = make_int_lit(&tc, 1);
    int add = check_arithmetic(&tc, EXPR_ADD, ch, num);
    printf("char+int type: %s\n", type_name(tc.exprs[add].type));
    // EXPECT: char+int type: int
    printf("errors: %d\n", tc.errors);
    // EXPECT: errors: 0

    printf("=== TypeCheck Test 3: Bool arithmetic error ===\n");
    // EXPECT: === TypeCheck Test 3: Bool arithmetic error ===
    tc_init(&tc);
    int t = make_bool_lit(&tc, 1);
    int f = make_bool_lit(&tc, 0);
    int bad = check_arithmetic(&tc, EXPR_ADD, t, f);
    printf("bool+bool type: %s\n", type_name(tc.exprs[bad].type));
    // EXPECT: bool+bool type: error
    printf("errors: %d\n", tc.errors);
    // EXPECT: errors: 1
    printf("msg: %s\n", tc.error_msgs[0]);
    // EXPECT: msg: arith requires int

    printf("=== TypeCheck Test 4: Comparison ===\n");
    // EXPECT: === TypeCheck Test 4: Comparison ===
    tc_init(&tc);
    int x = make_int_lit(&tc, 10);
    int y = make_int_lit(&tc, 20);
    int lt = check_comparison(&tc, EXPR_LT, x, y);
    printf("10<20 type: %s\n", type_name(tc.exprs[lt].type));
    // EXPECT: 10<20 type: bool
    printf("errors: %d\n", tc.errors);
    // EXPECT: errors: 0

    printf("=== TypeCheck Test 5: Bool equality ===\n");
    // EXPECT: === TypeCheck Test 5: Bool equality ===
    tc_init(&tc);
    int b1 = make_bool_lit(&tc, 1);
    int b2 = make_bool_lit(&tc, 0);
    int eq = check_comparison(&tc, EXPR_EQ, b1, b2);
    printf("bool==bool type: %s\n", type_name(tc.exprs[eq].type));
    // EXPECT: bool==bool type: bool
    printf("errors: %d\n", tc.errors);
    // EXPECT: errors: 0

    printf("=== TypeCheck Test 6: Logical AND ===\n");
    // EXPECT: === TypeCheck Test 6: Logical AND ===
    tc_init(&tc);
    int p = make_bool_lit(&tc, 1);
    int q = make_bool_lit(&tc, 1);
    int and_e = check_logical(&tc, EXPR_AND, p, q);
    printf("bool&&bool type: %s\n", type_name(tc.exprs[and_e].type));
    // EXPECT: bool&&bool type: bool
    printf("errors: %d\n", tc.errors);
    // EXPECT: errors: 0

    printf("=== TypeCheck Test 7: Logical on int error ===\n");
    // EXPECT: === TypeCheck Test 7: Logical on int error ===
    tc_init(&tc);
    int i1 = make_int_lit(&tc, 1);
    int i2 = make_int_lit(&tc, 2);
    int bad2 = check_logical(&tc, EXPR_OR, i1, i2);
    printf("int||int type: %s\n", type_name(tc.exprs[bad2].type));
    // EXPECT: int||int type: error
    printf("errors: %d\n", tc.errors);
    // EXPECT: errors: 1

    printf("=== TypeCheck Test 8: NOT ===\n");
    // EXPECT: === TypeCheck Test 8: NOT ===
    tc_init(&tc);
    int bv = make_bool_lit(&tc, 1);
    int notb = check_not(&tc, bv);
    printf("!bool type: %s\n", type_name(tc.exprs[notb].type));
    // EXPECT: !bool type: bool

    int iv = make_int_lit(&tc, 5);
    int noti = check_not(&tc, iv);
    printf("!int type: %s\n", type_name(tc.exprs[noti].type));
    // EXPECT: !int type: error
    printf("errors: %d\n", tc.errors);
    // EXPECT: errors: 1

    printf("=== TypeCheck Test 9: Chained ===\n");
    // EXPECT: === TypeCheck Test 9: Chained ===
    tc_init(&tc);
    int v1 = make_int_lit(&tc, 5);
    int v2 = make_int_lit(&tc, 10);
    int v3 = make_int_lit(&tc, 3);
    int s1 = check_arithmetic(&tc, EXPR_ADD, v1, v2);  // int
    int s2 = check_arithmetic(&tc, EXPR_MUL, s1, v3);  // int
    int cmp = check_comparison(&tc, EXPR_LT, s2, make_int_lit(&tc, 100)); // bool
    printf("(5+10)*3 < 100 type: %s\n", type_name(tc.exprs[cmp].type));
    // EXPECT: (5+10)*3 < 100 type: bool
    printf("errors: %d\n", tc.errors);
    // EXPECT: errors: 0

    printf("done\n");
    // EXPECT: done
    return 0;
}
