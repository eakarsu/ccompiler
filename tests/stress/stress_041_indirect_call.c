// stress_041_indirect_call.c - Indirect function call torture test
// EXPECT: add_result=30
// EXPECT: sub_result=10
// EXPECT: fptr_array=60
// EXPECT: double_ptr=100
// EXPECT: returned_fptr=25
// EXPECT: vtable_add=15
// EXPECT: vtable_sub=5
// EXPECT: vtable_mul=50
// EXPECT: vtable_shl=40
// EXPECT: vtable_neg=-10
// EXPECT: dispatch_sum=380
// EXPECT: chain_result=42
// EXPECT: conditional_fp=70
// EXPECT: struct_array_fp=210

int printf(const char *fmt, ...);

// --- Basic function pointer in struct ---
typedef int (*binop_fn)(int, int);

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int shl(int a, int b) { return a << b; }
int neg_first(int a, int b) { return -a; }

struct Callable {
    binop_fn func;
    int x;
    int y;
};

// --- Function pointer returned from function ---
typedef int (*unary_fn)(int);

int square(int x) { return x * x; }
int triple(int x) { return x * 3; }

unary_fn get_op(int which) {
    if (which == 0) return square;
    return triple;
}

// --- Vtable pattern ---
struct VTable {
    binop_fn methods[5];
};

struct Object {
    struct VTable *vt;
    int data;
};

int dispatch(struct Object *obj, int method_idx, int arg) {
    return obj->vt->methods[method_idx](obj->data, arg);
}

// --- Double pointer to function ---
int apply_double_ptr(binop_fn *fp, int a, int b) {
    return (*fp)(a, b);
}

// --- Chain: function pointer in struct, struct in array, call through index ---
struct NamedOp {
    binop_fn op;
    int id;
};

int chain_call(struct NamedOp *ops, int idx, int a, int b) {
    return ops[idx].op(a, b);
}

// --- Function pointer selected by condition ---
int conditional_dispatch(int cond, int a, int b) {
    binop_fn f;
    if (cond > 0)
        f = add;
    else
        f = mul;
    return f(a, b);
}

// --- Array of structs with function pointers, sum all results ---
int sum_struct_array(struct Callable *arr, int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i++) {
        total = total + arr[i].func(arr[i].x, arr[i].y);
    }
    return total;
}

int main(void) {
    // Basic struct function pointer
    struct Callable c1;
    c1.func = add;
    c1.x = 10;
    c1.y = 20;
    printf("add_result=%d\n", c1.func(c1.x, c1.y));

    struct Callable c2;
    c2.func = sub;
    c2.x = 20;
    c2.y = 10;
    printf("sub_result=%d\n", c2.func(c2.x, c2.y));

    // Array of function pointers
    binop_fn ops[3];
    ops[0] = add;
    ops[1] = sub;
    ops[2] = mul;
    int r = ops[0](10, 20) + ops[1](30, 10) + ops[2](2, 5);
    printf("fptr_array=%d\n", r);

    // Double pointer to function
    binop_fn fp = mul;
    binop_fn *fpp = &fp;
    printf("double_ptr=%d\n", apply_double_ptr(fpp, 10, 10));

    // Function pointer returned from function then called
    unary_fn sq = get_op(0);
    printf("returned_fptr=%d\n", sq(5));

    // Vtable dispatch
    struct VTable vt;
    vt.methods[0] = add;
    vt.methods[1] = sub;
    vt.methods[2] = mul;
    vt.methods[3] = shl;
    vt.methods[4] = neg_first;

    struct Object obj;
    obj.vt = &vt;
    obj.data = 10;

    printf("vtable_add=%d\n", dispatch(&obj, 0, 5));
    printf("vtable_sub=%d\n", dispatch(&obj, 1, 5));
    printf("vtable_mul=%d\n", dispatch(&obj, 2, 5));
    printf("vtable_shl=%d\n", dispatch(&obj, 3, 2));
    printf("vtable_neg=%d\n", dispatch(&obj, 4, 0));

    // Dispatch sum: call all 5 methods and sum
    int dsum = 0;
    int i;
    for (i = 0; i < 5; i++) {
        dsum = dsum + dispatch(&obj, i, 5);
    }
    printf("dispatch_sum=%d\n", dsum);

    // Chain through struct array
    struct NamedOp named_ops[3];
    named_ops[0].op = add;
    named_ops[0].id = 0;
    named_ops[1].op = mul;
    named_ops[1].id = 1;
    named_ops[2].op = sub;
    named_ops[2].id = 2;
    int cr = chain_call(named_ops, 1, 6, 7);
    printf("chain_result=%d\n", cr);

    // Conditional function pointer
    printf("conditional_fp=%d\n", conditional_dispatch(1, 30, 40));

    // Array of structs with function pointers
    struct Callable arr[3];
    arr[0].func = add;  arr[0].x = 10; arr[0].y = 20;
    arr[1].func = mul;  arr[1].x = 5;  arr[1].y = 6;
    arr[2].func = add;  arr[2].x = 75; arr[2].y = 75;
    printf("struct_array_fp=%d\n", sum_struct_array(arr, 3));

    return 0;
}
