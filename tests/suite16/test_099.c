int printf(const char *fmt, ...);

// Test 099: Higher-order function patterns: compose, curry, partial application

typedef int (*UnaryFn)(int);
typedef int (*BinaryFn)(int, int);

// Function composition table: store composed functions as chains
struct Composed {
    UnaryFn fns[8];
    int count;
};

void composed_init(struct Composed *c) {
    c->count = 0;
}

void composed_add(struct Composed *c, UnaryFn fn) {
    c->fns[c->count] = fn;
    c->count = c->count + 1;
}

int composed_apply(struct Composed *c, int x) {
    int i;
    int result;
    result = x;
    for (i = 0; i < c->count; i++) {
        result = c->fns[i](result);
    }
    return result;
}

// Partial application simulation with structs
struct PartialAdd {
    int bound_arg;
};

struct PartialMul {
    int bound_arg;
};

struct PartialMod {
    int bound_arg;
};

int partial_add_apply(struct PartialAdd *p, int x) {
    return x + p->bound_arg;
}

int partial_mul_apply(struct PartialMul *p, int x) {
    return x * p->bound_arg;
}

int partial_mod_apply(struct PartialMod *p, int x) {
    return x % p->bound_arg;
}

// Simple unary functions to compose
int inc(int x) { return x + 1; }
int dbl(int x) { return x * 2; }
int sqr(int x) { return x * x; }
int neg(int x) { return -x; }
int abs_val(int x) { if (x < 0) return -x; return x; }
int half(int x) { return x / 2; }
int add3(int x) { return x + 3; }
int sub7(int x) { return x - 7; }

// Apply a unary function to each element
void map_unary(int *src, int *dst, int len, UnaryFn fn) {
    int i;
    for (i = 0; i < len; i++) {
        dst[i] = fn(src[i]);
    }
}

// Apply a composed function to each element
void map_composed(int *src, int *dst, int len, struct Composed *c) {
    int i;
    for (i = 0; i < len; i++) {
        dst[i] = composed_apply(c, src[i]);
    }
}

// fold_left with binary function
int fold_left(int *arr, int len, int init, BinaryFn fn) {
    int i;
    int acc;
    acc = init;
    for (i = 0; i < len; i++) {
        acc = fn(acc, arr[i]);
    }
    return acc;
}

// fold_right with binary function
int fold_right(int *arr, int len, int init, BinaryFn fn) {
    int i;
    int acc;
    acc = init;
    for (i = len - 1; i >= 0; i--) {
        acc = fn(arr[i], acc);
    }
    return acc;
}

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }
int sub(int a, int b) { return a - b; }
int max_fn(int a, int b) { if (a > b) return a; return b; }

void print_array(const char *label, int *arr, int len) {
    int i;
    printf("%s:", label);
    for (i = 0; i < len; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");
}

int main(void) {
    struct Composed comp;
    struct PartialAdd pa;
    struct PartialMul pm;
    struct PartialMod pmod;
    int src[5];
    int dst[5];
    int i;

    // Single composition: inc then dbl
    composed_init(&comp);
    composed_add(&comp, inc);
    composed_add(&comp, dbl);
    printf("inc_dbl(3): %d\n", composed_apply(&comp, 3)); // EXPECT: inc_dbl(3): 8
    printf("inc_dbl(0): %d\n", composed_apply(&comp, 0)); // EXPECT: inc_dbl(0): 2

    // Triple composition: inc, dbl, sqr => sqr(dbl(inc(x)))
    composed_init(&comp);
    composed_add(&comp, inc);
    composed_add(&comp, dbl);
    composed_add(&comp, sqr);
    printf("ids(2): %d\n", composed_apply(&comp, 2)); // EXPECT: ids(2): 36
    printf("ids(4): %d\n", composed_apply(&comp, 4)); // EXPECT: ids(4): 100

    // Reverse composition: sqr, neg, abs
    composed_init(&comp);
    composed_add(&comp, sqr);
    composed_add(&comp, neg);
    composed_add(&comp, abs_val);
    printf("sna(3): %d\n", composed_apply(&comp, 3)); // EXPECT: sna(3): 9
    printf("sna(-5): %d\n", composed_apply(&comp, -5)); // EXPECT: sna(-5): 25

    // Partial application: add 10
    pa.bound_arg = 10;
    printf("pa10(5): %d\n", partial_add_apply(&pa, 5)); // EXPECT: pa10(5): 15
    printf("pa10(-3): %d\n", partial_add_apply(&pa, -3)); // EXPECT: pa10(-3): 7

    // Partial application: mul 3
    pm.bound_arg = 3;
    printf("pm3(7): %d\n", partial_mul_apply(&pm, 7)); // EXPECT: pm3(7): 21
    printf("pm3(0): %d\n", partial_mul_apply(&pm, 0)); // EXPECT: pm3(0): 0

    // Partial application: mod 5
    pmod.bound_arg = 5;
    printf("pmod5(17): %d\n", partial_mod_apply(&pmod, 17)); // EXPECT: pmod5(17): 2
    printf("pmod5(10): %d\n", partial_mod_apply(&pmod, 10)); // EXPECT: pmod5(10): 0

    // Map with composition
    src[0] = 1; src[1] = 2; src[2] = 3; src[3] = 4; src[4] = 5;

    composed_init(&comp);
    composed_add(&comp, dbl);
    composed_add(&comp, add3);
    map_composed(src, dst, 5, &comp);
    print_array("dbl_a3", dst, 5); // EXPECT: dbl_a3: 5 7 9 11 13

    composed_init(&comp);
    composed_add(&comp, sqr);
    composed_add(&comp, half);
    map_composed(src, dst, 5, &comp);
    print_array("sq_hlf", dst, 5); // EXPECT: sq_hlf: 0 2 4 8 12

    // Fold left: sum
    printf("fold_sum: %d\n", fold_left(src, 5, 0, add)); // EXPECT: fold_sum: 15

    // Fold left: product
    printf("fold_prod: %d\n", fold_left(src, 5, 1, mul)); // EXPECT: fold_prod: 120

    // Fold left: max
    printf("fold_max: %d\n", fold_left(src, 5, src[0], max_fn)); // EXPECT: fold_max: 5

    // Fold left: subtract (((((0-1)-2)-3)-4)-5) = -15
    printf("fold_sub_l: %d\n", fold_left(src, 5, 0, sub)); // EXPECT: fold_sub_l: -15

    // Fold right: subtract (1-(2-(3-(4-(5-0))))) = 3
    printf("fold_sub_r: %d\n", fold_right(src, 5, 0, sub)); // EXPECT: fold_sub_r: 3

    // Multiple partial applications on array
    pa.bound_arg = 100;
    for (i = 0; i < 5; i++) {
        dst[i] = partial_add_apply(&pa, src[i]);
    }
    print_array("pa100", dst, 5); // EXPECT: pa100: 101 102 103 104 105

    printf("done\n"); // EXPECT: done
    return 0;
}
