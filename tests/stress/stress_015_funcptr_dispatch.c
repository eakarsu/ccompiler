// Function pointer hell: dispatch tables, returning fptrs, struct members, callbacks
int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

// 10 simple functions for dispatch table
int fn0(int x) { return x + 0; }
int fn1(int x) { return x + 1; }
int fn2(int x) { return x + 2; }
int fn3(int x) { return x + 3; }
int fn4(int x) { return x + 4; }
int fn5(int x) { return x + 5; }
int fn6(int x) { return x + 6; }
int fn7(int x) { return x + 7; }
int fn8(int x) { return x + 8; }
int fn9(int x) { return x + 9; }

typedef int (*int_fn)(int);

void test_dispatch_table(void) {
    int_fn table[10];
    table[0] = fn0;
    table[1] = fn1;
    table[2] = fn2;
    table[3] = fn3;
    table[4] = fn4;
    table[5] = fn5;
    table[6] = fn6;
    table[7] = fn7;
    table[8] = fn8;
    table[9] = fn9;
    int i;
    for (i = 0; i < 10; i++) {
        int result = table[i](100);
        printf("fn%d(100)=%d\n", i, result);
    }
    // EXPECT: fn0(100)=100
    // EXPECT: fn1(100)=101
    // EXPECT: fn2(100)=102
    // EXPECT: fn3(100)=103
    // EXPECT: fn4(100)=104
    // EXPECT: fn5(100)=105
    // EXPECT: fn6(100)=106
    // EXPECT: fn7(100)=107
    // EXPECT: fn8(100)=108
    // EXPECT: fn9(100)=109
}

// Function returning a function pointer
int add_op(int a, int b) { return a + b; }
int sub_op(int a, int b) { return a - b; }
int mul_op(int a, int b) { return a * b; }

typedef int (*binop_fn)(int, int);

binop_fn get_op(int choice) {
    if (choice == 0) return add_op;
    if (choice == 1) return sub_op;
    return mul_op;
}

void test_return_funcptr(void) {
    binop_fn f;
    f = get_op(0);
    printf("add=%d\n", f(10, 3));
    // EXPECT: add=13
    f = get_op(1);
    printf("sub=%d\n", f(10, 3));
    // EXPECT: sub=7
    f = get_op(2);
    printf("mul=%d\n", f(10, 3));
    // EXPECT: mul=30
}

// Function pointer as struct member
struct handler {
    int id;
    int_fn process;
};

void test_struct_funcptr(void) {
    struct handler h1;
    h1.id = 1;
    h1.process = fn5;
    struct handler h2;
    h2.id = 2;
    h2.process = fn9;
    printf("h1: id=%d result=%d\n", h1.id, h1.process(10));
    printf("h2: id=%d result=%d\n", h2.id, h2.process(10));
    // EXPECT: h1: id=1 result=15
    // EXPECT: h2: id=2 result=19
}

// Callback with void* context
struct context {
    int multiplier;
    int offset;
};

int callback(void *ctx, int value) {
    struct context *c = (struct context *)ctx;
    return value * c->multiplier + c->offset;
}

void apply_callback(int (*cb)(void *, int), void *ctx, int val) {
    int result = cb(ctx, val);
    printf("callback result=%d\n", result);
}

void test_callback(void) {
    struct context c1;
    c1.multiplier = 3;
    c1.offset = 7;
    apply_callback(callback, (void *)&c1, 10);
    // EXPECT: callback result=37

    struct context c2;
    c2.multiplier = 5;
    c2.offset = -2;
    apply_callback(callback, (void *)&c2, 8);
    // EXPECT: callback result=38
}

// Typedef chain
typedef int (*base_fn)(int);
typedef base_fn (*factory_fn)(int);

base_fn my_factory(int which) {
    if (which == 0) return fn0;
    if (which == 1) return fn3;
    return fn7;
}

void test_typedef_chain(void) {
    factory_fn factory = my_factory;
    base_fn f1 = factory(0);
    base_fn f2 = factory(1);
    base_fn f3 = factory(2);
    printf("chain: %d %d %d\n", f1(10), f2(10), f3(10));
    // EXPECT: chain: 10 13 17
}

// Array of structs with function pointers
void test_handler_array(void) {
    struct handler handlers[5];
    int_fn fns[5];
    fns[0] = fn1;
    fns[1] = fn3;
    fns[2] = fn5;
    fns[3] = fn7;
    fns[4] = fn9;
    int i;
    for (i = 0; i < 5; i++) {
        handlers[i].id = i;
        handlers[i].process = fns[i];
    }
    for (i = 0; i < 5; i++) {
        printf("handler[%d]=%d\n", i, handlers[i].process(0));
    }
    // EXPECT: handler[0]=1
    // EXPECT: handler[1]=3
    // EXPECT: handler[2]=5
    // EXPECT: handler[3]=7
    // EXPECT: handler[4]=9
}

// Funcptr in conditional
void test_conditional_funcptr(void) {
    int i;
    for (i = 0; i < 6; i++) {
        int_fn f = (i % 2 == 0) ? fn0 : fn9;
        printf("cond[%d]=%d\n", i, f(100));
    }
    // EXPECT: cond[0]=100
    // EXPECT: cond[1]=109
    // EXPECT: cond[2]=100
    // EXPECT: cond[3]=109
    // EXPECT: cond[4]=100
    // EXPECT: cond[5]=109
}

// Nested function pointer call: get_op returns a binop, call it immediately
void test_immediate_call(void) {
    int r = get_op(2)(6, 7);
    printf("immediate=%d\n", r);
    // EXPECT: immediate=42
}

// Function pointer comparison
void test_funcptr_compare(void) {
    int_fn a = fn5;
    int_fn b = fn5;
    int_fn c = fn3;
    printf("eq=%d neq=%d\n", a == b, a == c);
    // EXPECT: eq=1 neq=0
}

// Funcptr stored on heap
void test_heap_funcptr(void) {
    int_fn *fp = (int_fn *)malloc(sizeof(int_fn));
    *fp = fn8;
    int r = (*fp)(10);
    printf("heap=%d\n", r);
    // EXPECT: heap=18
    free(fp);
}

int main(void) {
    test_dispatch_table();
    test_return_funcptr();
    test_struct_funcptr();
    test_callback();
    test_typedef_chain();
    test_handler_array();
    test_conditional_funcptr();
    test_immediate_call();
    test_funcptr_compare();
    test_heap_funcptr();
    return 0;
}
