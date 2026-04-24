int printf(const char *fmt, ...);

// Call stack simulation: function call/return with frames
// Simulates nested function calls with local variables and return values

struct Frame {
    int return_addr;
    int locals[8];
    int num_locals;
};

struct CallStack {
    struct Frame frames[16];
    int fp;
    int retval;
};

void cs_init(struct CallStack *cs) {
    cs->fp = 0;
    cs->retval = 0;
    int i;
    int j;
    for (i = 0; i < 16; i++) {
        cs->frames[i].return_addr = 0;
        cs->frames[i].num_locals = 0;
        for (j = 0; j < 8; j++) {
            cs->frames[i].locals[j] = 0;
        }
    }
}

void cs_call(struct CallStack *cs, int ret_addr, int nlocals) {
    cs->fp = cs->fp + 1;
    cs->frames[cs->fp].return_addr = ret_addr;
    cs->frames[cs->fp].num_locals = nlocals;
    int i;
    for (i = 0; i < 8; i++) {
        cs->frames[cs->fp].locals[i] = 0;
    }
}

int cs_return(struct CallStack *cs, int val) {
    int ret = cs->frames[cs->fp].return_addr;
    cs->fp = cs->fp - 1;
    cs->retval = val;
    return ret;
}

void cs_set_local(struct CallStack *cs, int idx, int val) {
    cs->frames[cs->fp].locals[idx] = val;
}

int cs_get_local(struct CallStack *cs, int idx) {
    return cs->frames[cs->fp].locals[idx];
}

int main() {
    struct CallStack cs;
    cs_init(&cs);

    printf("initial fp: %d\n", cs.fp);
    // EXPECT: initial fp: 0

    // Simulate: main calls func_a(5), which calls func_b(10)
    // main frame
    cs_set_local(&cs, 0, 100);  // main's local

    // call func_a at pc=10, with 2 locals
    cs_call(&cs, 10, 2);
    printf("after call_a fp: %d\n", cs.fp);
    // EXPECT: after call_a fp: 1

    cs_set_local(&cs, 0, 5);   // func_a param
    cs_set_local(&cs, 1, 0);   // func_a local

    // func_a calls func_b at pc=20, with 3 locals
    cs_call(&cs, 20, 3);
    printf("after call_b fp: %d\n", cs.fp);
    // EXPECT: after call_b fp: 2

    cs_set_local(&cs, 0, 10);  // func_b param
    cs_set_local(&cs, 1, 3);   // func_b local
    cs_set_local(&cs, 2, cs_get_local(&cs, 0) * cs_get_local(&cs, 1));
    printf("func_b result: %d\n", cs_get_local(&cs, 2));
    // EXPECT: func_b result: 30

    // func_b returns 30
    int ret_pc = cs_return(&cs, 30);
    printf("returned to pc: %d retval: %d fp: %d\n", ret_pc, cs.retval, cs.fp);
    // EXPECT: returned to pc: 20 retval: 30 fp: 1

    // back in func_a: local[1] = retval + param
    cs_set_local(&cs, 1, cs.retval + cs_get_local(&cs, 0));
    printf("func_a result: %d\n", cs_get_local(&cs, 1));
    // EXPECT: func_a result: 35

    // func_a returns 35
    ret_pc = cs_return(&cs, 35);
    printf("returned to pc: %d retval: %d fp: %d\n", ret_pc, cs.retval, cs.fp);
    // EXPECT: returned to pc: 10 retval: 35 fp: 0

    // back in main
    printf("main local: %d\n", cs_get_local(&cs, 0));
    // EXPECT: main local: 100

    // Deep recursion test: simulate factorial(5)
    cs_init(&cs);
    cs_set_local(&cs, 0, 1);  // accumulator in main frame

    // Push frames for factorial calls
    int n;
    for (n = 5; n >= 1; n = n - 1) {
        cs_call(&cs, n * 100, 1);
        cs_set_local(&cs, 0, n);
    }
    printf("depth at bottom: %d\n", cs.fp);
    // EXPECT: depth at bottom: 5

    // Unwind, multiplying as we go
    int acc = 1;
    while (cs.fp > 0) {
        int local_n = cs_get_local(&cs, 0);
        acc = acc * local_n;
        cs_return(&cs, acc);
    }
    printf("factorial(5) = %d\n", acc);
    // EXPECT: factorial(5) = 120

    // Test frame isolation
    cs_init(&cs);
    cs_set_local(&cs, 0, 111);
    cs_call(&cs, 0, 1);
    cs_set_local(&cs, 0, 222);
    cs_call(&cs, 0, 1);
    cs_set_local(&cs, 0, 333);

    printf("inner: %d\n", cs_get_local(&cs, 0));
    // EXPECT: inner: 333

    cs_return(&cs, 0);
    printf("middle: %d\n", cs_get_local(&cs, 0));
    // EXPECT: middle: 222

    cs_return(&cs, 0);
    printf("outer: %d\n", cs_get_local(&cs, 0));
    // EXPECT: outer: 111

    return 0;
}
