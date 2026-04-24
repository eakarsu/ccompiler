int printf(const char *fmt, ...);

typedef int (*IntFn)(int);

int double_it(int x) { return x * 2; }
int add_one(int x) { return x + 1; }
int add_three(int x) { return x + 3; }
int square(int x) { return x * x; }
int negate(int x) { return -x; }
int add_ten(int x) { return x + 10; }
int triple(int x) { return x * 3; }
int sub_five(int x) { return x - 5; }

struct Composed {
    IntFn f;
    IntFn g;
};

int call_composed(struct Composed *c, int x) {
    return c->f(c->g(x));
}

struct Pipeline {
    IntFn stages[8];
    int count;
};

void pipe_init(struct Pipeline *p) {
    p->count = 0;
}

void pipe_add(struct Pipeline *p, IntFn f) {
    if (p->count < 8) {
        p->stages[p->count] = f;
        p->count = p->count + 1;
    }
}

int pipe_run(struct Pipeline *p, int x) {
    int i;
    for (i = 0; i < p->count; i++) {
        x = p->stages[i](x);
    }
    return x;
}

int pipe_run_n(struct Pipeline *p, int x, int n) {
    int i;
    for (i = 0; i < n; i++) {
        x = pipe_run(p, x);
    }
    return x;
}

void pipe_map(struct Pipeline *p, int arr[], int out[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        out[i] = pipe_run(p, arr[i]);
    }
}

int pipe_fold(struct Pipeline *p, int arr[], int n, int init) {
    int result;
    int i;
    result = init;
    for (i = 0; i < n; i++) {
        result = result + pipe_run(p, arr[i]);
    }
    return result;
}

int main(void) {
    struct Composed c;
    struct Pipeline pipe;
    int arr[5];
    int out[5];
    int val;

    c.f = double_it;
    c.g = add_one;
    val = call_composed(&c, 5);
    // EXPECT: double(add1(5)) = 12
    printf("double(add1(5)) = %d\n", val);

    c.f = add_one;
    c.g = double_it;
    val = call_composed(&c, 5);
    // EXPECT: add1(double(5)) = 11
    printf("add1(double(5)) = %d\n", val);

    c.f = square;
    c.g = add_one;
    val = call_composed(&c, 3);
    // EXPECT: sq(add1(3)) = 16
    printf("sq(add1(3)) = %d\n", val);

    c.f = negate;
    c.g = square;
    val = call_composed(&c, 4);
    // EXPECT: neg(sq(4)) = -16
    printf("neg(sq(4)) = %d\n", val);

    pipe_init(&pipe);
    pipe_add(&pipe, add_one);
    pipe_add(&pipe, double_it);
    pipe_add(&pipe, add_three);
    val = pipe_run(&pipe, 5);
    // EXPECT: pipe(5): 15
    printf("pipe(5): %d\n", val);

    val = pipe_run(&pipe, 0);
    // EXPECT: pipe(0): 5
    printf("pipe(0): %d\n", val);

    val = pipe_run(&pipe, 10);
    // EXPECT: pipe(10): 25
    printf("pipe(10): %d\n", val);

    val = pipe_run_n(&pipe, 1, 3);
    // EXPECT: pipe x3(1): 43
    printf("pipe x3(1): %d\n", val);

    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
    pipe_map(&pipe, arr, out, 5);
    // EXPECT: mapped: 7 9 11 13 15
    printf("mapped: %d %d %d %d %d\n", out[0], out[1], out[2], out[3], out[4]);

    val = pipe_fold(&pipe, arr, 5, 0);
    // EXPECT: fold: 55
    printf("fold: %d\n", val);

    pipe_init(&pipe);
    pipe_add(&pipe, triple);
    pipe_add(&pipe, sub_five);
    val = pipe_run(&pipe, 4);
    // EXPECT: triple-5(4): 7
    printf("triple-5(4): %d\n", val);

    val = pipe_run(&pipe, 10);
    // EXPECT: triple-5(10): 25
    printf("triple-5(10): %d\n", val);

    pipe_add(&pipe, double_it);
    val = pipe_run(&pipe, 3);
    // EXPECT: t-5*2(3): 8
    printf("t-5*2(3): %d\n", val);

    // EXPECT: stages: 3
    printf("stages: %d\n", pipe.count);

    pipe_map(&pipe, arr, out, 5);
    // EXPECT: mapped2: -4 2 8 14 20
    printf("mapped2: %d %d %d %d %d\n", out[0], out[1], out[2], out[3], out[4]);

    return 0;
}
