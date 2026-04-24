int printf(const char *fmt, ...);

int data[128];
int tops[4];
int bases[4];
int capacities[4];

void multi_stack_init(void) {
    bases[0] = 0;
    bases[1] = 32;
    bases[2] = 64;
    bases[3] = 96;
    capacities[0] = 32;
    capacities[1] = 32;
    capacities[2] = 32;
    capacities[3] = 32;
    tops[0] = 0;
    tops[1] = 0;
    tops[2] = 0;
    tops[3] = 0;
}

int stack_empty(int s) {
    return tops[s] == 0;
}

int stack_full(int s) {
    return tops[s] == capacities[s];
}

int stack_size(int s) {
    return tops[s];
}

void stack_push(int s, int val) {
    if (stack_full(s)) return;
    data[bases[s] + tops[s]] = val;
    tops[s] = tops[s] + 1;
}

int stack_pop(int s) {
    int val;
    if (stack_empty(s)) return -1;
    tops[s] = tops[s] - 1;
    val = data[bases[s] + tops[s]];
    return val;
}

int stack_peek(int s) {
    if (stack_empty(s)) return -1;
    return data[bases[s] + tops[s] - 1];
}

int main(void) {
    int val;
    int i;
    int total;

    multi_stack_init();

    // EXPECT: s0 empty: 1
    printf("s0 empty: %d\n", stack_empty(0));

    // EXPECT: s1 empty: 1
    printf("s1 empty: %d\n", stack_empty(1));

    stack_push(0, 10);
    stack_push(0, 20);
    stack_push(0, 30);

    stack_push(1, 100);
    stack_push(1, 200);

    stack_push(2, 1000);

    // EXPECT: s0 size: 3
    printf("s0 size: %d\n", stack_size(0));

    // EXPECT: s1 size: 2
    printf("s1 size: %d\n", stack_size(1));

    // EXPECT: s2 size: 1
    printf("s2 size: %d\n", stack_size(2));

    // EXPECT: s3 size: 0
    printf("s3 size: %d\n", stack_size(3));

    // EXPECT: s0 peek: 30
    printf("s0 peek: %d\n", stack_peek(0));

    // EXPECT: s1 peek: 200
    printf("s1 peek: %d\n", stack_peek(1));

    val = stack_pop(0);
    // EXPECT: s0 pop: 30
    printf("s0 pop: %d\n", val);

    val = stack_pop(0);
    // EXPECT: s0 pop: 20
    printf("s0 pop: %d\n", val);

    // EXPECT: s0 size: 1
    printf("s0 size: %d\n", stack_size(0));

    stack_push(3, 42);
    stack_push(3, 43);
    stack_push(3, 44);

    // EXPECT: s3 peek: 44
    printf("s3 peek: %d\n", stack_peek(3));

    val = stack_pop(1);
    // EXPECT: s1 pop: 200
    printf("s1 pop: %d\n", val);

    val = stack_pop(1);
    // EXPECT: s1 pop: 100
    printf("s1 pop: %d\n", val);

    // EXPECT: s1 empty: 1
    printf("s1 empty: %d\n", stack_empty(1));

    stack_push(2, 2000);
    stack_push(2, 3000);
    // EXPECT: s2 peek: 3000
    printf("s2 peek: %d\n", stack_peek(2));

    // EXPECT: s2 size: 3
    printf("s2 size: %d\n", stack_size(2));

    total = 0;
    i = 0;
    while (i < 4) {
        total = total + stack_size(i);
        i = i + 1;
    }
    // EXPECT: total items: 7
    printf("total items: %d\n", total);

    val = stack_pop(2);
    val = stack_pop(2);
    val = stack_pop(2);
    // EXPECT: s2 last: 1000
    printf("s2 last: %d\n", val);

    // EXPECT: s2 empty: 1
    printf("s2 empty: %d\n", stack_empty(2));

    return 0;
}
