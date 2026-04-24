int printf(const char *fmt, ...);

struct Generator {
    int state;
    int value;
    int limit;
    int current;
};

void gen_init(struct Generator *g, int start, int lim) {
    g->state = 0;
    g->value = 0;
    g->limit = lim;
    g->current = start;
}

int gen_next(struct Generator *g) {
    if (g->current >= g->limit) {
        g->state = -1;
        return 0;
    }
    g->value = g->current;
    g->current = g->current + 1;
    g->state = 1;
    return 1;
}

struct FibGen {
    int state;
    int a;
    int b;
    int value;
    int count;
    int max_count;
};

void fib_init(struct FibGen *f, int max) {
    f->state = 0;
    f->a = 0;
    f->b = 1;
    f->value = 0;
    f->count = 0;
    f->max_count = max;
}

int fib_next(struct FibGen *f) {
    if (f->count >= f->max_count) {
        f->state = -1;
        return 0;
    }
    f->value = f->a;
    int temp = f->a + f->b;
    f->a = f->b;
    f->b = temp;
    f->count = f->count + 1;
    f->state = 1;
    return 1;
}

struct FilterGen {
    struct Generator base;
    int divisor;
};

void filter_init(struct FilterGen *fg, int start, int lim, int div) {
    gen_init(&fg->base, start, lim);
    fg->divisor = div;
}

int filter_next(struct FilterGen *fg) {
    while (gen_next(&fg->base)) {
        if (fg->base.value % fg->divisor == 0) {
            return 1;
        }
    }
    return 0;
}

struct StateMachine {
    int state;
    int output;
    int transitions[16];
};

void sm_init(struct StateMachine *sm) {
    sm->state = 0;
    sm->output = 0;
    sm->transitions[0] = 1;
    sm->transitions[1] = 0;
    sm->transitions[2] = 2;
    sm->transitions[3] = 0;
    sm->transitions[4] = 2;
    sm->transitions[5] = 3;
    sm->transitions[6] = 0;
    sm->transitions[7] = 1;
}

int sm_step(struct StateMachine *sm, int input) {
    if (input < 0 || input > 1) return -1;
    int idx = sm->state * 2 + input;
    sm->state = sm->transitions[idx];
    if (sm->state == 3) {
        sm->output = sm->output + 1;
        sm->state = 0;
    }
    return sm->state;
}

struct Accumulator {
    int state;
    int total;
    int count;
};

void acc_init(struct Accumulator *a) {
    a->state = 0;
    a->total = 0;
    a->count = 0;
}

void acc_add(struct Accumulator *a, int val) {
    a->total = a->total + val;
    a->count = a->count + 1;
    a->state = 1;
}

int acc_average(struct Accumulator *a) {
    if (a->count == 0) return 0;
    return a->total / a->count;
}

int main(void) {
    struct Generator g;
    gen_init(&g, 3, 7);
    int sum = 0;
    while (gen_next(&g)) {
        sum = sum + g.value;
    }
    // EXPECT: gensum=18
    printf("gensum=%d\n", sum);
    // EXPECT: genstate=-1
    printf("genstate=%d\n", g.state);

    struct FibGen fg;
    fib_init(&fg, 8);
    int fibsum = 0;
    int last_fib = 0;
    while (fib_next(&fg)) {
        fibsum = fibsum + fg.value;
        last_fib = fg.value;
    }
    // EXPECT: fibsum=33
    printf("fibsum=%d\n", fibsum);
    // EXPECT: lastfib=13
    printf("lastfib=%d\n", last_fib);

    struct FilterGen ffg;
    filter_init(&ffg, 1, 20, 3);
    int fsum = 0;
    int fcount = 0;
    while (filter_next(&ffg)) {
        fsum = fsum + ffg.base.value;
        fcount = fcount + 1;
    }
    // EXPECT: filtsum=63
    printf("filtsum=%d\n", fsum);
    // EXPECT: filtcnt=6
    printf("filtcnt=%d\n", fcount);

    struct StateMachine sm;
    sm_init(&sm);
    int inputs[10] = {0, 0, 1, 0, 0, 1, 1, 0, 0, 1};
    int i = 0;
    while (i < 10) {
        sm_step(&sm, inputs[i]);
        i = i + 1;
    }
    // EXPECT: sm_output=3
    printf("sm_output=%d\n", sm.output);
    // EXPECT: sm_state=0
    printf("sm_state=%d\n", sm.state);

    struct Accumulator acc;
    acc_init(&acc);
    acc_add(&acc, 10);
    acc_add(&acc, 20);
    acc_add(&acc, 30);
    acc_add(&acc, 40);
    // EXPECT: acc_total=100
    printf("acc_total=%d\n", acc.total);
    // EXPECT: acc_avg=25
    printf("acc_avg=%d\n", acc_average(&acc));
    // EXPECT: acc_count=4
    printf("acc_count=%d\n", acc.count);

    struct Generator g2;
    gen_init(&g2, 0, 0);
    // EXPECT: empty=0
    printf("empty=%d\n", gen_next(&g2));

    struct FibGen fg2;
    fib_init(&fg2, 1);
    fib_next(&fg2);
    // EXPECT: fib1=0
    printf("fib1=%d\n", fg2.value);

    struct Generator g3;
    gen_init(&g3, 10, 12);
    gen_next(&g3);
    // EXPECT: g3val=10
    printf("g3val=%d\n", g3.value);
    gen_next(&g3);
    // EXPECT: g3val2=11
    printf("g3val2=%d\n", g3.value);

    struct Accumulator acc2;
    acc_init(&acc2);
    // EXPECT: acc2_avg=0
    printf("acc2_avg=%d\n", acc_average(&acc2));

    return 0;
}
