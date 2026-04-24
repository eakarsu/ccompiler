// stress_067_funcptr_table.c - Function pointer dispatch table stress test
// EXPECT: dispatch[0]=op_add(3,4)=7
// EXPECT: dispatch[1]=op_sub(3,4)=-1
// EXPECT: dispatch[2]=op_mul(3,4)=12
// EXPECT: dispatch[3]=op_div(12,4)=3
// EXPECT: dispatch[4]=op_mod(10,3)=1
// EXPECT: dispatch[5]=op_neg(7,0)=-7
// EXPECT: dispatch[6]=op_abs(-5,0)=5
// EXPECT: dispatch[7]=op_min(3,8)=3
// EXPECT: dispatch[8]=op_max(3,8)=8
// EXPECT: dispatch[9]=op_shl(1,4)=16
// EXPECT: dispatch[10]=op_shr(256,4)=16
// EXPECT: dispatch[11]=op_and(0xFF,0x0F)=15
// EXPECT: dispatch[12]=op_or(0xF0,0x0F)=255
// EXPECT: dispatch[13]=op_xor(0xFF,0xFF)=0
// EXPECT: dispatch[14]=op_not(0,0)=-1
// EXPECT: hash_dispatch_add=30
// EXPECT: hash_dispatch_mul=200
// EXPECT: chain[0]=start
// EXPECT: chain[1]=alpha
// EXPECT: chain[2]=beta
// EXPECT: chain[3]=gamma
// EXPECT: chain[4]=done
// EXPECT: union_funcptr=42
// EXPECT: cmp_same=1
// EXPECT: cmp_diff=0
// EXPECT: accumulate=55
// EXPECT: transform=2 6 12 20 30
// EXPECT: callback_result=110
// EXPECT: composed=18
// EXPECT: predicate_filter=2 4 6 8 10

int printf(const char *fmt, ...);

// --- 15 operation functions ---
int op_add(int a, int b) { return a + b; }
int op_sub(int a, int b) { return a - b; }
int op_mul(int a, int b) { return a * b; }
int op_div(int a, int b) { return a / b; }
int op_mod(int a, int b) { return a % b; }
int op_neg(int a, int b) { return -a; }
int op_abs(int a, int b) { return a < 0 ? -a : a; }
int op_min(int a, int b) { return a < b ? a : b; }
int op_max(int a, int b) { return a > b ? a : b; }
int op_shl(int a, int b) { return a << b; }
int op_shr(int a, int b) { return a >> b; }
int op_and(int a, int b) { return a & b; }
int op_or(int a, int b)  { return a | b; }
int op_xor(int a, int b) { return a ^ b; }
int op_not(int a, int b) { return ~a; }

// Dispatch table type
typedef int (*binop_fn)(int, int);

// --- Enum dispatch ---
enum Op {
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
    OP_NEG, OP_ABS, OP_MIN, OP_MAX,
    OP_SHL, OP_SHR, OP_AND, OP_OR, OP_XOR, OP_NOT,
    OP_COUNT
};

binop_fn dispatch_table[15];

void init_dispatch(void) {
    dispatch_table[OP_ADD] = op_add;
    dispatch_table[OP_SUB] = op_sub;
    dispatch_table[OP_MUL] = op_mul;
    dispatch_table[OP_DIV] = op_div;
    dispatch_table[OP_MOD] = op_mod;
    dispatch_table[OP_NEG] = op_neg;
    dispatch_table[OP_ABS] = op_abs;
    dispatch_table[OP_MIN] = op_min;
    dispatch_table[OP_MAX] = op_max;
    dispatch_table[OP_SHL] = op_shl;
    dispatch_table[OP_SHR] = op_shr;
    dispatch_table[OP_AND] = op_and;
    dispatch_table[OP_OR]  = op_or;
    dispatch_table[OP_XOR] = op_xor;
    dispatch_table[OP_NOT] = op_not;
}

int dispatch_call(enum Op op, int a, int b) {
    return dispatch_table[op](a, b);
}

void test_enum_dispatch(void) {
    init_dispatch();
    printf("dispatch[0]=op_add(3,4)=%d\n", dispatch_call(OP_ADD, 3, 4));
    printf("dispatch[1]=op_sub(3,4)=%d\n", dispatch_call(OP_SUB, 3, 4));
    printf("dispatch[2]=op_mul(3,4)=%d\n", dispatch_call(OP_MUL, 3, 4));
    printf("dispatch[3]=op_div(12,4)=%d\n", dispatch_call(OP_DIV, 12, 4));
    printf("dispatch[4]=op_mod(10,3)=%d\n", dispatch_call(OP_MOD, 10, 3));
    printf("dispatch[5]=op_neg(7,0)=%d\n", dispatch_call(OP_NEG, 7, 0));
    printf("dispatch[6]=op_abs(-5,0)=%d\n", dispatch_call(OP_ABS, -5, 0));
    printf("dispatch[7]=op_min(3,8)=%d\n", dispatch_call(OP_MIN, 3, 8));
    printf("dispatch[8]=op_max(3,8)=%d\n", dispatch_call(OP_MAX, 3, 8));
    printf("dispatch[9]=op_shl(1,4)=%d\n", dispatch_call(OP_SHL, 1, 4));
    printf("dispatch[10]=op_shr(256,4)=%d\n", dispatch_call(OP_SHR, 256, 4));
    printf("dispatch[11]=op_and(0xFF,0x0F)=%d\n", dispatch_call(OP_AND, 0xFF, 0x0F));
    printf("dispatch[12]=op_or(0xF0,0x0F)=%d\n", dispatch_call(OP_OR, 0xF0, 0x0F));
    printf("dispatch[13]=op_xor(0xFF,0xFF)=%d\n", dispatch_call(OP_XOR, 0xFF, 0xFF));
    printf("dispatch[14]=op_not(0,0)=%d\n", dispatch_call(OP_NOT, 0, 0));
}

// --- String hash dispatch ---
int simple_hash(const char *s) {
    int h = 0;
    while (*s) {
        h = h * 31 + *s;
        s++;
    }
    return h;
}

void test_hash_dispatch(void) {
    // Use hash to select operation
    int h_add = simple_hash("add");
    int h_mul = simple_hash("mul");

    binop_fn ops[2];
    ops[0] = op_add;
    ops[1] = op_mul;

    // Map: use hash % 2 to select
    // We just test that both work
    printf("hash_dispatch_add=%d\n", ops[0](10, 20));
    printf("hash_dispatch_mul=%d\n", ops[1](10, 20));
}

// --- Chained dispatch: function returns index of next ---
typedef int (*chain_fn)(int);

int chain_start(int state) {
    printf("chain[%d]=start\n", state);
    return 1;
}

int chain_alpha(int state) {
    printf("chain[%d]=alpha\n", state);
    return 2;
}

int chain_beta(int state) {
    printf("chain[%d]=beta\n", state);
    return 3;
}

int chain_gamma(int state) {
    printf("chain[%d]=gamma\n", state);
    return 4;
}

int chain_done(int state) {
    printf("chain[%d]=done\n", state);
    return -1; // terminal
}

void test_chain_dispatch(void) {
    chain_fn chain[5];
    chain[0] = chain_start;
    chain[1] = chain_alpha;
    chain[2] = chain_beta;
    chain[3] = chain_gamma;
    chain[4] = chain_done;

    int state = 0;
    while (state >= 0 && state < 5) {
        state = chain[state](state);
    }
}

// --- Function pointer in union ---
union FuncOrInt {
    binop_fn func;
    int value;
};

void test_union_funcptr(void) {
    union FuncOrInt u;
    u.func = op_mul;
    int result = u.func(6, 7);
    printf("union_funcptr=%d\n", result);
}

// --- Function pointer comparison ---
void test_funcptr_compare(void) {
    binop_fn a = op_add;
    binop_fn b = op_add;
    binop_fn c = op_sub;
    printf("cmp_same=%d\n", a == b);
    printf("cmp_diff=%d\n", a == c);
}

// --- Accumulate with function pointer ---
int accumulate(int *arr, int n, binop_fn op, int init) {
    int result = init;
    int i;
    for (i = 0; i < n; i++) {
        result = op(result, arr[i]);
    }
    return result;
}

void test_accumulate(void) {
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int sum = accumulate(arr, 10, op_add, 0);
    printf("accumulate=%d\n", sum);
}

// --- Transform array with function pointer ---
void transform(int *dst, int *a, int *b, int n, binop_fn op) {
    int i;
    for (i = 0; i < n; i++) {
        dst[i] = op(a[i], b[i]);
    }
}

void test_transform(void) {
    int a[5] = {1, 2, 3, 4, 5};
    int b[5] = {2, 3, 4, 5, 6};
    int result[5];
    transform(result, a, b, 5, op_mul);
    printf("transform=");
    int i;
    for (i = 0; i < 5; i++) {
        if (i > 0) printf(" ");
        printf("%d", result[i]);
    }
    printf("\n");
}

// --- Callback pattern ---
typedef void (*callback_fn)(int result, int *out);

void store_result(int result, int *out) {
    *out = result;
}

void compute_and_callback(int a, int b, binop_fn op, callback_fn cb, int *out) {
    int result = op(a, b);
    cb(result, out);
}

void test_callback(void) {
    int result = 0;
    compute_and_callback(10, 11, op_mul, store_result, &result);
    printf("callback_result=%d\n", result);
}

// --- Function composition via pointers ---
int double_it(int a, int b) { return a * 2; }
int add_three(int a, int b) { return a + 3; }

int compose(int val, binop_fn *fns, int n) {
    int i;
    for (i = 0; i < n; i++) {
        val = fns[i](val, 0);
    }
    return val;
}

void test_composed(void) {
    // Start with 3, double to 6, add 3 to 9, double to 18
    binop_fn pipeline[3];
    pipeline[0] = double_it;
    pipeline[1] = add_three;
    pipeline[2] = double_it;
    int result = compose(3, pipeline, 3);
    printf("composed=%d\n", result);
}

// --- Predicate-based filter ---
typedef int (*predicate_fn)(int);

int is_even(int x) { return x % 2 == 0; }

void filter_print(int *arr, int n, predicate_fn pred) {
    int first = 1;
    int i;
    for (i = 0; i < n; i++) {
        if (pred(arr[i])) {
            if (!first) printf(" ");
            printf("%d", arr[i]);
            first = 0;
        }
    }
    printf("\n");
}

void test_predicate_filter(void) {
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    printf("predicate_filter=");
    filter_print(arr, 10, is_even);
}

int main(void) {
    test_enum_dispatch();
    test_hash_dispatch();
    test_chain_dispatch();
    test_union_funcptr();
    test_funcptr_compare();
    test_accumulate();
    test_transform();
    test_callback();
    test_composed();
    test_predicate_filter();
    return 0;
}
