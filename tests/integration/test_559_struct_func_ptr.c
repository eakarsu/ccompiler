int printf(const char *fmt, ...);
// EXPECT: Input: 5\n  double(5) = 10\n  square(5) = 25\n  negate(5) = -5\n  inc(5) = 6\n  dec(5) = 4\nPipeline (inc -> double -> square):\n  2 -> 36\n  3 -> 64\n  5 -> 144\n  10 -> 484\nCall counts:\nProcessor 'double': 5 calls\nProcessor 'square': 5 calls\nProcessor 'negate': 1 calls\nProcessor 'inc': 5 calls\nProcessor 'dec': 1 calls

typedef int (*Transform)(int);

struct Processor {
    char *name;
    Transform fn;
    int calls;
};

int double_it(int x) { return x * 2; }
int square_it(int x) { return x * x; }
int negate_it(int x) { return -x; }
int increment(int x) { return x + 1; }
int decrement(int x) { return x - 1; }

int run_processor(struct Processor *p, int val) {
    p->calls = p->calls + 1;
    return p->fn(val);
}

void print_processor(struct Processor *p) {
    printf("Processor '%s': %d calls\n", p->name, p->calls);
}

struct Pipeline {
    struct Processor *stages[4];
    int num_stages;
};

int run_pipeline(struct Pipeline *pipe, int val) {
    int result = val;
    int i;
    for (i = 0; i < pipe->num_stages; i++) {
        result = run_processor(pipe->stages[i], result);
    }
    return result;
}

int main(void) {
    struct Processor procs[5];
    procs[0].name = "double";   procs[0].fn = double_it;   procs[0].calls = 0;
    procs[1].name = "square";   procs[1].fn = square_it;   procs[1].calls = 0;
    procs[2].name = "negate";   procs[2].fn = negate_it;   procs[2].calls = 0;
    procs[3].name = "inc";      procs[3].fn = increment;   procs[3].calls = 0;
    procs[4].name = "dec";      procs[4].fn = decrement;   procs[4].calls = 0;

    int i;
    int val = 5;
    printf("Input: %d\n", val);
    for (i = 0; i < 5; i++) {
        printf("  %s(%d) = %d\n", procs[i].name, val, run_processor(&procs[i], val));
    }

    struct Pipeline pipe;
    pipe.stages[0] = &procs[3];
    pipe.stages[1] = &procs[0];
    pipe.stages[2] = &procs[1];
    pipe.num_stages = 3;

    printf("Pipeline (inc -> double -> square):\n");
    int inputs[4];
    inputs[0] = 2; inputs[1] = 3; inputs[2] = 5; inputs[3] = 10;
    for (i = 0; i < 4; i++) {
        printf("  %d -> %d\n", inputs[i], run_pipeline(&pipe, inputs[i]));
    }

    printf("Call counts:\n");
    for (i = 0; i < 5; i++) {
        print_processor(&procs[i]);
    }

    return 0;
}
