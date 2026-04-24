int printf(const char *fmt, ...);

typedef int (*TransformFunc)(int);

struct Decorator {
    TransformFunc func;
    int applied;
};

struct DecoratorChain {
    struct Decorator decorators[8];
    int count;
};

int identity(int x) { return x; }
int double_it(int x) { return x * 2; }
int add_ten(int x) { return x + 10; }
int square(int x) { return x * x; }
int negate(int x) { return -x; }
int abs_val(int x) { if (x < 0) return -x; return x; }
int clamp_100(int x) { if (x > 100) return 100; if (x < 0) return 0; return x; }
int add_one(int x) { return x + 1; }

void chain_init(struct DecoratorChain *c) {
    c->count = 0;
}

void chain_add(struct DecoratorChain *c, TransformFunc f) {
    c->decorators[c->count].func = f;
    c->decorators[c->count].applied = 0;
    c->count = c->count + 1;
}

int chain_execute(struct DecoratorChain *c, int input) {
    int i;
    int val;
    val = input;
    for (i = 0; i < c->count; i++) {
        val = c->decorators[i].func(val);
        c->decorators[i].applied = 1;
    }
    return val;
}

int chain_execute_reverse(struct DecoratorChain *c, int input) {
    int i;
    int val;
    val = input;
    for (i = c->count - 1; i >= 0; i--) {
        val = c->decorators[i].func(val);
    }
    return val;
}

int chain_count_applied(struct DecoratorChain *c) {
    int i;
    int count;
    count = 0;
    for (i = 0; i < c->count; i++) {
        if (c->decorators[i].applied) {
            count = count + 1;
        }
    }
    return count;
}

int chain_execute_partial(struct DecoratorChain *c, int input, int n) {
    int i;
    int val;
    int limit;
    val = input;
    limit = n;
    if (limit > c->count) limit = c->count;
    for (i = 0; i < limit; i++) {
        val = c->decorators[i].func(val);
    }
    return val;
}

int chain_execute_skip(struct DecoratorChain *c, int input, int skip_idx) {
    int i;
    int val;
    val = input;
    for (i = 0; i < c->count; i++) {
        if (i != skip_idx) {
            val = c->decorators[i].func(val);
        }
    }
    return val;
}

int compose2(TransformFunc f, TransformFunc g, int x) {
    return f(g(x));
}

int main(void) {
    struct DecoratorChain chain;
    int result;

    chain_init(&chain);
    chain_add(&chain, double_it);
    chain_add(&chain, add_ten);

    result = chain_execute(&chain, 5);
    // EXPECT: double+add10(5) = 20
    printf("double+add10(5) = %d\n", result);

    // EXPECT: applied: 2
    printf("applied: %d\n", chain_count_applied(&chain));

    chain_init(&chain);
    chain_add(&chain, add_ten);
    chain_add(&chain, double_it);

    result = chain_execute(&chain, 5);
    // EXPECT: add10+double(5) = 30
    printf("add10+double(5) = %d\n", result);

    chain_init(&chain);
    chain_add(&chain, double_it);
    chain_add(&chain, add_ten);
    chain_add(&chain, square);

    result = chain_execute(&chain, 3);
    // EXPECT: dbl+add10+sq(3) = 256
    printf("dbl+add10+sq(3) = %d\n", result);

    result = chain_execute_reverse(&chain, 3);
    // EXPECT: reverse sq+add10+dbl(3) = 38
    printf("reverse sq+add10+dbl(3) = %d\n", result);

    chain_init(&chain);
    chain_add(&chain, negate);
    chain_add(&chain, abs_val);
    chain_add(&chain, double_it);

    result = chain_execute(&chain, 7);
    // EXPECT: neg+abs+dbl(7) = 14
    printf("neg+abs+dbl(7) = %d\n", result);

    result = chain_execute(&chain, -3);
    // EXPECT: neg+abs+dbl(-3) = 6
    printf("neg+abs+dbl(-3) = %d\n", result);

    chain_init(&chain);
    chain_add(&chain, double_it);
    chain_add(&chain, add_ten);
    chain_add(&chain, square);
    chain_add(&chain, clamp_100);

    result = chain_execute(&chain, 5);
    // EXPECT: clamped(5) = 100
    printf("clamped(5) = %d\n", result);

    result = chain_execute(&chain, 1);
    // EXPECT: clamped(1) = 100
    printf("clamped(1) = %d\n", result);

    result = chain_execute(&chain, 0);
    // EXPECT: clamped(0) = 100
    printf("clamped(0) = %d\n", result);

    chain_init(&chain);
    chain_add(&chain, add_one);
    chain_add(&chain, double_it);
    chain_add(&chain, add_ten);

    result = chain_execute_partial(&chain, 5, 1);
    // EXPECT: partial(1): 6
    printf("partial(1): %d\n", result);

    result = chain_execute_partial(&chain, 5, 2);
    // EXPECT: partial(2): 12
    printf("partial(2): %d\n", result);

    result = chain_execute_partial(&chain, 5, 3);
    // EXPECT: partial(3): 22
    printf("partial(3): %d\n", result);

    result = chain_execute_skip(&chain, 5, 1);
    // EXPECT: skip(1): 16
    printf("skip(1): %d\n", result);

    // EXPECT: compose dbl(add10(3)): 26
    printf("compose dbl(add10(3)): %d\n", compose2(double_it, add_ten, 3));

    // EXPECT: compose add10(dbl(3)): 16
    printf("compose add10(dbl(3)): %d\n", compose2(add_ten, double_it, 3));

    // EXPECT: chain size: 3
    printf("chain size: %d\n", chain.count);

    return 0;
}
