int printf(const char *fmt, ...);
// EXPECT: Stack 0 size: 3\nStack 1 size: 2\nStack 2 size: 1\nStack 3 size: 4\nStack 0 top: 30\nStack 1 top: 200\nPop stack 0: 30\nPop stack 0: 20\nStack 0 size: 1\nStack 3: 35 25 15 5\nStack 3 empty: 1\nStack 2 empty: 0\nDone

// Multiple stacks in one array

struct MultiStack {
    int data[64];
    int tops[4];
    int bases[4];
    int num_stacks;
};

void ms_init(struct MultiStack *ms) {
    ms->num_stacks = 4;
    int i = 0;
    while (i < 4) {
        ms->bases[i] = i * 16;
        ms->tops[i] = i * 16 - 1;
        i = i + 1;
    }
    i = 0;
    while (i < 64) {
        ms->data[i] = 0;
        i = i + 1;
    }
}

int ms_is_empty(struct MultiStack *ms, int stack_id) {
    return ms->tops[stack_id] < ms->bases[stack_id];
}

int ms_is_full(struct MultiStack *ms, int stack_id) {
    return ms->tops[stack_id] >= ms->bases[stack_id] + 15;
}

int ms_push(struct MultiStack *ms, int stack_id, int val) {
    if (ms_is_full(ms, stack_id)) {
        return -1;
    }
    ms->tops[stack_id] = ms->tops[stack_id] + 1;
    ms->data[ms->tops[stack_id]] = val;
    return 0;
}

int ms_pop(struct MultiStack *ms, int stack_id) {
    if (ms_is_empty(ms, stack_id)) {
        return -1;
    }
    int val = ms->data[ms->tops[stack_id]];
    ms->tops[stack_id] = ms->tops[stack_id] - 1;
    return val;
}

int ms_peek(struct MultiStack *ms, int stack_id) {
    return ms->data[ms->tops[stack_id]];
}

int ms_size(struct MultiStack *ms, int stack_id) {
    return ms->tops[stack_id] - ms->bases[stack_id] + 1;
}

int main(void) {
    struct MultiStack ms;
    ms_init(&ms);

    // Push to different stacks
    ms_push(&ms, 0, 10);
    ms_push(&ms, 0, 20);
    ms_push(&ms, 0, 30);

    ms_push(&ms, 1, 100);
    ms_push(&ms, 1, 200);

    ms_push(&ms, 2, 1000);

    ms_push(&ms, 3, 5);
    ms_push(&ms, 3, 15);
    ms_push(&ms, 3, 25);
    ms_push(&ms, 3, 35);

    // Check sizes
    printf("Stack 0 size: %d\n", ms_size(&ms, 0));
    printf("Stack 1 size: %d\n", ms_size(&ms, 1));
    printf("Stack 2 size: %d\n", ms_size(&ms, 2));
    printf("Stack 3 size: %d\n", ms_size(&ms, 3));

    // Peek
    printf("Stack 0 top: %d\n", ms_peek(&ms, 0));
    printf("Stack 1 top: %d\n", ms_peek(&ms, 1));

    // Pop from stacks
    printf("Pop stack 0: %d\n", ms_pop(&ms, 0));
    printf("Pop stack 0: %d\n", ms_pop(&ms, 0));
    printf("Stack 0 size: %d\n", ms_size(&ms, 0));

    // Pop all from stack 3
    printf("Stack 3:");
    while (!ms_is_empty(&ms, 3)) {
        printf(" %d", ms_pop(&ms, 3));
    }
    printf("\n");

    // Empty check
    printf("Stack 3 empty: %d\n", ms_is_empty(&ms, 3));
    printf("Stack 2 empty: %d\n", ms_is_empty(&ms, 2));

    printf("Done\n");
    return 0;
}
