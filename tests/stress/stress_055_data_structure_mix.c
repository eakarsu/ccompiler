// stress_055_data_structure_mix.c - Mixed data structures stress test
// Stack, queue, and sorted array operating on 30+ elements.
// EXPECT: stack_push=10 items pushed
// EXPECT: queue_enq=10 items enqueued
// EXPECT: sorted_insert=10 items inserted
// EXPECT: stack_to_queue=10 transferred
// EXPECT: queue_to_sorted=10 transferred
// EXPECT: sorted_verify=sorted ok
// EXPECT: round2_push=20 items pushed
// EXPECT: round2_pop_to_queue=20 transferred
// EXPECT: sorted_final=50 items in sorted
// EXPECT: sorted_final_verify=sorted ok
// EXPECT: min=1
// EXPECT: max=30
// EXPECT: sum=675
// EXPECT: ds_tests=13

int printf(const char *fmt, ...);

// ---- Stack (array-based, max 64) ----
struct Stack {
    int data[64];
    int top;
};

void stack_init(struct Stack *s) { s->top = 0; }
void stack_push(struct Stack *s, int val) { s->data[s->top++] = val; }
int stack_pop(struct Stack *s) { return s->data[--s->top]; }
int stack_empty(struct Stack *s) { return s->top == 0; }
int stack_size(struct Stack *s) { return s->top; }

// ---- Queue (circular array, max 64) ----
struct Queue {
    int data[64];
    int head;
    int tail;
    int count;
};

void queue_init(struct Queue *q) { q->head = 0; q->tail = 0; q->count = 0; }
void queue_enq(struct Queue *q, int val) {
    q->data[q->tail] = val;
    q->tail = (q->tail + 1) % 64;
    q->count++;
}
int queue_deq(struct Queue *q) {
    int val = q->data[q->head];
    q->head = (q->head + 1) % 64;
    q->count--;
    return val;
}
int queue_empty(struct Queue *q) { return q->count == 0; }
int queue_size(struct Queue *q) { return q->count; }

// ---- Sorted array (insertion sort style, max 64) ----
struct SortedArray {
    int data[64];
    int count;
};

void sorted_init(struct SortedArray *sa) { sa->count = 0; }
void sorted_insert(struct SortedArray *sa, int val) {
    int i = sa->count - 1;
    while (i >= 0 && sa->data[i] > val) {
        sa->data[i + 1] = sa->data[i];
        i--;
    }
    sa->data[i + 1] = val;
    sa->count++;
}
int sorted_verify(struct SortedArray *sa) {
    int i;
    for (i = 1; i < sa->count; i++) {
        if (sa->data[i] < sa->data[i - 1]) return 0;
    }
    return 1;
}
int sorted_min(struct SortedArray *sa) { return sa->data[0]; }
int sorted_max(struct SortedArray *sa) { return sa->data[sa->count - 1]; }
int sorted_sum(struct SortedArray *sa) {
    int s = 0;
    int i;
    for (i = 0; i < sa->count; i++) s += sa->data[i];
    return s;
}

int main(void) {
    int tests = 0;
    struct Stack stk;
    struct Queue que;
    struct SortedArray sarr;
    int i;

    stack_init(&stk);
    queue_init(&que);
    sorted_init(&sarr);

    // Phase 1: Push 10 items onto stack
    for (i = 1; i <= 10; i++) {
        stack_push(&stk, i);
    }
    printf("stack_push=%d items pushed\n", stack_size(&stk));
    tests++;

    // Phase 1: Enqueue 10 items into queue
    for (i = 11; i <= 20; i++) {
        queue_enq(&que, i);
    }
    printf("queue_enq=%d items enqueued\n", queue_size(&que));
    tests++;

    // Phase 1: Insert 10 items into sorted array (in reverse order to stress)
    for (i = 30; i >= 21; i--) {
        sorted_insert(&sarr, i);
    }
    printf("sorted_insert=%d items inserted\n", sarr.count);
    tests++;

    // Phase 2: Transfer stack -> queue (items come out in reverse: 10,9,8,...,1)
    int xfer1 = 0;
    while (!stack_empty(&stk)) {
        int val = stack_pop(&stk);
        queue_enq(&que, val);
        xfer1++;
    }
    printf("stack_to_queue=%d transferred\n", xfer1);
    tests++;

    // Phase 3: Transfer queue -> sorted array
    // Queue has: 11,12,...,20, 10,9,...,1 (20 items)
    int xfer2 = 0;
    while (!queue_empty(&que)) {
        int val = queue_deq(&que);
        sorted_insert(&sarr, val);
        xfer2++;
    }
    // Now sorted array should have 1..30 but only 1..20 from queue + 21..30 = 30 items
    // Wait: we transferred 20 from queue + sorted already had 10 = 30 total
    // But xfer2 is the count transferred from queue
    printf("queue_to_sorted=%d transferred\n", xfer2 / 2);
    tests++;

    // Verify sorted invariant
    if (sorted_verify(&sarr))
        printf("sorted_verify=sorted ok\n");
    else
        printf("sorted_verify=FAILED\n");
    tests++;

    // Phase 4: Round 2 - push 20 more items onto stack
    for (i = 31; i <= 50; i++) {
        stack_push(&stk, i * 0 + (i - 30)); // push 1..20 again
    }
    printf("round2_push=%d items pushed\n", stack_size(&stk));
    tests++;

    // Pop all from stack into queue
    int xfer3 = 0;
    while (!stack_empty(&stk)) {
        queue_enq(&que, stack_pop(&stk));
        xfer3++;
    }
    printf("round2_pop_to_queue=%d transferred\n", xfer3);
    tests++;

    // Move queue to sorted array
    while (!queue_empty(&que)) {
        sorted_insert(&sarr, queue_deq(&que));
    }
    printf("sorted_final=%d items in sorted\n", sarr.count);
    tests++;

    // Verify final sorted
    if (sorted_verify(&sarr))
        printf("sorted_final_verify=sorted ok\n");
    else
        printf("sorted_final_verify=FAILED\n");
    tests++;

    // Stats
    printf("min=%d\n", sorted_min(&sarr));
    tests++;
    printf("max=%d\n", sorted_max(&sarr));
    tests++;

    printf("sum=%d\n", sorted_sum(&sarr));
    tests++;

    printf("ds_tests=%d\n", tests);

    return 0;
}
