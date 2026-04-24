int printf(const char *fmt, ...);

// Test 097: Lazy evaluation simulation using thunks (deferred computation)

struct Thunk {
    int evaluated;
    int cached_value;
    int arg1;
    int arg2;
    int kind;  // 0=const, 1=add, 2=mul, 3=sub, 4=square, 5=fact
};

void thunk_const(struct Thunk *t, int val) {
    t->evaluated = 0;
    t->cached_value = 0;
    t->arg1 = val;
    t->arg2 = 0;
    t->kind = 0;
}

void thunk_add(struct Thunk *t, int a, int b) {
    t->evaluated = 0;
    t->cached_value = 0;
    t->arg1 = a;
    t->arg2 = b;
    t->kind = 1;
}

void thunk_mul(struct Thunk *t, int a, int b) {
    t->evaluated = 0;
    t->cached_value = 0;
    t->arg1 = a;
    t->arg2 = b;
    t->kind = 2;
}

void thunk_sub(struct Thunk *t, int a, int b) {
    t->evaluated = 0;
    t->cached_value = 0;
    t->arg1 = a;
    t->arg2 = b;
    t->kind = 3;
}

void thunk_square(struct Thunk *t, int a) {
    t->evaluated = 0;
    t->cached_value = 0;
    t->arg1 = a;
    t->arg2 = 0;
    t->kind = 4;
}

void thunk_fact(struct Thunk *t, int n) {
    t->evaluated = 0;
    t->cached_value = 0;
    t->arg1 = n;
    t->arg2 = 0;
    t->kind = 5;
}

int compute_factorial(int n) {
    int result;
    int i;
    result = 1;
    for (i = 2; i <= n; i++) {
        result = result * i;
    }
    return result;
}

int thunk_force(struct Thunk *t) {
    if (t->evaluated) {
        return t->cached_value;
    }
    if (t->kind == 0) {
        t->cached_value = t->arg1;
    } else if (t->kind == 1) {
        t->cached_value = t->arg1 + t->arg2;
    } else if (t->kind == 2) {
        t->cached_value = t->arg1 * t->arg2;
    } else if (t->kind == 3) {
        t->cached_value = t->arg1 - t->arg2;
    } else if (t->kind == 4) {
        t->cached_value = t->arg1 * t->arg1;
    } else if (t->kind == 5) {
        t->cached_value = compute_factorial(t->arg1);
    }
    t->evaluated = 1;
    return t->cached_value;
}

int thunk_is_evaluated(struct Thunk *t) {
    return t->evaluated;
}

// Lazy list: array of thunks
struct LazyList {
    struct Thunk items[10];
    int len;
};

void lazy_list_init(struct LazyList *ll) {
    ll->len = 0;
}

void lazy_list_push(struct LazyList *ll, struct Thunk t) {
    ll->items[ll->len] = t;
    ll->len = ll->len + 1;
}

int lazy_list_get(struct LazyList *ll, int idx) {
    return thunk_force(&ll->items[idx]);
}

int lazy_list_sum(struct LazyList *ll) {
    int total;
    int i;
    total = 0;
    for (i = 0; i < ll->len; i++) {
        total = total + thunk_force(&ll->items[i]);
    }
    return total;
}

int lazy_list_count_evaluated(struct LazyList *ll) {
    int count;
    int i;
    count = 0;
    for (i = 0; i < ll->len; i++) {
        if (thunk_is_evaluated(&ll->items[i])) {
            count = count + 1;
        }
    }
    return count;
}

int main(void) {
    struct Thunk t;
    struct LazyList ll;
    int val;
    struct Thunk tmp;

    // Basic thunk evaluation
    thunk_const(&t, 42);
    printf("is_eval: %d\n", thunk_is_evaluated(&t)); // EXPECT: is_eval: 0
    val = thunk_force(&t);
    printf("const: %d\n", val); // EXPECT: const: 42
    printf("is_eval: %d\n", thunk_is_evaluated(&t)); // EXPECT: is_eval: 1
    val = thunk_force(&t);
    printf("cached: %d\n", val); // EXPECT: cached: 42

    thunk_add(&t, 10, 20);
    printf("add: %d\n", thunk_force(&t)); // EXPECT: add: 30

    thunk_mul(&t, 6, 7);
    printf("mul: %d\n", thunk_force(&t)); // EXPECT: mul: 42

    thunk_sub(&t, 100, 37);
    printf("sub: %d\n", thunk_force(&t)); // EXPECT: sub: 63

    thunk_square(&t, 9);
    printf("square: %d\n", thunk_force(&t)); // EXPECT: square: 81

    thunk_fact(&t, 6);
    printf("fact6: %d\n", thunk_force(&t)); // EXPECT: fact6: 720

    thunk_fact(&t, 10);
    printf("fact10: %d\n", thunk_force(&t)); // EXPECT: fact10: 3628800

    // Lazy list
    lazy_list_init(&ll);
    thunk_add(&tmp, 1, 2);
    lazy_list_push(&ll, tmp);
    thunk_mul(&tmp, 3, 4);
    lazy_list_push(&ll, tmp);
    thunk_sub(&tmp, 10, 5);
    lazy_list_push(&ll, tmp);
    thunk_square(&tmp, 6);
    lazy_list_push(&ll, tmp);
    thunk_fact(&tmp, 5);
    lazy_list_push(&ll, tmp);

    printf("ll_len: %d\n", ll.len); // EXPECT: ll_len: 5
    printf("ll_eval_before: %d\n", lazy_list_count_evaluated(&ll)); // EXPECT: ll_eval_before: 0

    printf("ll[0]: %d\n", lazy_list_get(&ll, 0)); // EXPECT: ll[0]: 3
    printf("ll[1]: %d\n", lazy_list_get(&ll, 1)); // EXPECT: ll[1]: 12
    printf("ll_eval_after2: %d\n", lazy_list_count_evaluated(&ll)); // EXPECT: ll_eval_after2: 2

    printf("ll[2]: %d\n", lazy_list_get(&ll, 2)); // EXPECT: ll[2]: 5
    printf("ll[3]: %d\n", lazy_list_get(&ll, 3)); // EXPECT: ll[3]: 36
    printf("ll[4]: %d\n", lazy_list_get(&ll, 4)); // EXPECT: ll[4]: 120

    printf("ll_sum: %d\n", lazy_list_sum(&ll)); // EXPECT: ll_sum: 176
    printf("ll_eval_all: %d\n", lazy_list_count_evaluated(&ll)); // EXPECT: ll_eval_all: 5

    printf("done\n"); // EXPECT: done
    return 0;
}
