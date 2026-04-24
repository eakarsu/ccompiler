int printf(const char *fmt, ...);

// Test 093: Closure simulation with structs (captured environment)

typedef int (*IntOp)(int);

struct Adder {
    int amount;
};

struct Multiplier {
    int factor;
};

struct LinearTransform {
    int scale;
    int offset;
};

struct Clamper {
    int lo;
    int hi;
};

struct Accumulator {
    int total;
};

int adder_apply(struct Adder *a, int x) {
    return x + a->amount;
}

int multiplier_apply(struct Multiplier *m, int x) {
    return x * m->factor;
}

int linear_apply(struct LinearTransform *lt, int x) {
    return x * lt->scale + lt->offset;
}

int clamper_apply(struct Clamper *c, int x) {
    if (x < c->lo) return c->lo;
    if (x > c->hi) return c->hi;
    return x;
}

void accumulator_add(struct Accumulator *acc, int x) {
    acc->total = acc->total + x;
}

int accumulator_get(struct Accumulator *acc) {
    return acc->total;
}

void accumulator_reset(struct Accumulator *acc) {
    acc->total = 0;
}

// Compose two "closures" by chaining struct-based function calls
int compose_add_mul(struct Adder *a, struct Multiplier *m, int x) {
    return multiplier_apply(m, adder_apply(a, x));
}

int compose_mul_add(struct Multiplier *m, struct Adder *a, int x) {
    return adder_apply(a, multiplier_apply(m, x));
}

// Apply a closure over an array
void apply_adder_array(struct Adder *a, int *arr, int len) {
    int i;
    for (i = 0; i < len; i++) {
        arr[i] = adder_apply(a, arr[i]);
    }
}

void apply_linear_array(struct LinearTransform *lt, int *arr, int len) {
    int i;
    for (i = 0; i < len; i++) {
        arr[i] = linear_apply(lt, arr[i]);
    }
}

void apply_clamper_array(struct Clamper *c, int *arr, int len) {
    int i;
    for (i = 0; i < len; i++) {
        arr[i] = clamper_apply(c, arr[i]);
    }
}

void print_array(const char *label, int *arr, int len) {
    int i;
    printf("%s:", label);
    for (i = 0; i < len; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");
}

int main(void) {
    struct Adder a5;
    struct Adder a_neg3;
    struct Multiplier m2;
    struct Multiplier m10;
    struct LinearTransform lt;
    struct Clamper cl;
    struct Accumulator acc;
    int arr[5];
    int i;

    a5.amount = 5;
    a_neg3.amount = -3;
    m2.factor = 2;
    m10.factor = 10;
    lt.scale = 3;
    lt.offset = 1;
    cl.lo = 0;
    cl.hi = 10;

    printf("add5(7): %d\n", adder_apply(&a5, 7)); // EXPECT: add5(7): 12
    printf("add-3(7): %d\n", adder_apply(&a_neg3, 7)); // EXPECT: add-3(7): 4
    printf("mul2(7): %d\n", multiplier_apply(&m2, 7)); // EXPECT: mul2(7): 14
    printf("mul10(7): %d\n", multiplier_apply(&m10, 7)); // EXPECT: mul10(7): 70
    printf("lin(7): %d\n", linear_apply(&lt, 7)); // EXPECT: lin(7): 22
    printf("clamp(-5): %d\n", clamper_apply(&cl, -5)); // EXPECT: clamp(-5): 0
    printf("clamp(5): %d\n", clamper_apply(&cl, 5)); // EXPECT: clamp(5): 5
    printf("clamp(15): %d\n", clamper_apply(&cl, 15)); // EXPECT: clamp(15): 10

    printf("add5_mul2(3): %d\n", compose_add_mul(&a5, &m2, 3)); // EXPECT: add5_mul2(3): 16
    printf("mul2_add5(3): %d\n", compose_mul_add(&m2, &a5, 3)); // EXPECT: mul2_add5(3): 11

    acc.total = 0;
    for (i = 1; i <= 5; i++) {
        accumulator_add(&acc, i);
    }
    printf("acc_sum: %d\n", accumulator_get(&acc)); // EXPECT: acc_sum: 15

    accumulator_reset(&acc);
    for (i = 1; i <= 5; i++) {
        accumulator_add(&acc, i * i);
    }
    printf("acc_sq_sum: %d\n", accumulator_get(&acc)); // EXPECT: acc_sq_sum: 55

    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
    apply_adder_array(&a5, arr, 5);
    print_array("arr+5", arr, 5); // EXPECT: arr+5: 6 7 8 9 10

    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
    apply_linear_array(&lt, arr, 5);
    print_array("arr_lin", arr, 5); // EXPECT: arr_lin: 4 7 10 13 16

    arr[0] = -5; arr[1] = 0; arr[2] = 5; arr[3] = 10; arr[4] = 15;
    apply_clamper_array(&cl, arr, 5);
    print_array("clamped", arr, 5); // EXPECT: clamped: 0 0 5 10 10

    printf("done\n"); // EXPECT: done
    return 0;
}
