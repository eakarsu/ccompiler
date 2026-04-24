int printf(const char *fmt, ...);
// EXPECT: Stack0[10,20,30]\nStack1[100,200]\nStack2[1000]\nPop stack0: 30\nPop stack1: 200\nPeek stack2: 1000\nSize stack0: 2\nSize stack1: 1\nSize stack2: 1\nStack0 full push: 0\nStack0[10,20,40,50,60,70,80,90,100,110]\nEmpty stack2: 0\nEmpty stack2: 1
// Multi-stack in a single array

struct MultiStack {
    int data[30];
    int tops[3];
    int starts[3];
    int limits[3];
};

void ms_init(struct MultiStack *ms) {
    int i;
    for (i = 0; i < 30; i++) ms->data[i] = 0;
    ms->starts[0] = 0;  ms->limits[0] = 10; ms->tops[0] = -1;
    ms->starts[1] = 10; ms->limits[1] = 20; ms->tops[1] = 9;
    ms->starts[2] = 20; ms->limits[2] = 30; ms->tops[2] = 19;
}

int ms_push(struct MultiStack *ms, int stack_id, int val) {
    int next;
    if (stack_id < 0 || stack_id >= 3) return 0;
    next = ms->tops[stack_id] + 1;
    if (next >= ms->limits[stack_id]) return 0;
    ms->tops[stack_id] = next;
    ms->data[next] = val;
    return 1;
}

int ms_pop(struct MultiStack *ms, int stack_id) {
    int val;
    if (stack_id < 0 || stack_id >= 3) return -1;
    if (ms->tops[stack_id] < ms->starts[stack_id]) return -1;
    val = ms->data[ms->tops[stack_id]];
    ms->tops[stack_id]--;
    return val;
}

int ms_peek(struct MultiStack *ms, int stack_id) {
    if (stack_id < 0 || stack_id >= 3) return -1;
    if (ms->tops[stack_id] < ms->starts[stack_id]) return -1;
    return ms->data[ms->tops[stack_id]];
}

int ms_size(struct MultiStack *ms, int stack_id) {
    if (stack_id < 0 || stack_id >= 3) return 0;
    return ms->tops[stack_id] - ms->starts[stack_id] + 1;
}

int ms_is_empty(struct MultiStack *ms, int stack_id) {
    return ms->tops[stack_id] < ms->starts[stack_id];
}

void ms_print_stack(struct MultiStack *ms, int stack_id) {
    int i;
    printf("Stack%d[", stack_id);
    for (i = ms->starts[stack_id]; i <= ms->tops[stack_id]; i++) {
        printf("%d", ms->data[i]);
        if (i < ms->tops[stack_id]) printf(",");
    }
    printf("]\n");
}

int main(void) {
    struct MultiStack ms;
    int i;
    ms_init(&ms);

    ms_push(&ms, 0, 10);
    ms_push(&ms, 0, 20);
    ms_push(&ms, 0, 30);
    ms_push(&ms, 1, 100);
    ms_push(&ms, 1, 200);
    ms_push(&ms, 2, 1000);

    for (i = 0; i < 3; i++) {
        ms_print_stack(&ms, i);
    }

    printf("Pop stack0: %d\n", ms_pop(&ms, 0));
    printf("Pop stack1: %d\n", ms_pop(&ms, 1));
    printf("Peek stack2: %d\n", ms_peek(&ms, 2));

    printf("Size stack0: %d\n", ms_size(&ms, 0));
    printf("Size stack1: %d\n", ms_size(&ms, 1));
    printf("Size stack2: %d\n", ms_size(&ms, 2));

    for (i = 0; i < 8; i++) {
        ms_push(&ms, 0, 40 + i * 10);
    }
    printf("Stack0 full push: %d\n", ms_push(&ms, 0, 999));
    ms_print_stack(&ms, 0);

    printf("Empty stack2: %d\n", ms_is_empty(&ms, 2));
    ms_pop(&ms, 2);
    printf("Empty stack2: %d\n", ms_is_empty(&ms, 2));

    return 0;
}
