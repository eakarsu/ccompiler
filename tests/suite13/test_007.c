int printf(const char *fmt, ...);

// Two-stack queue implementation
// Uses two stacks: inbox for enqueue, outbox for dequeue
// Amortized O(1) per operation

struct Stack {
    int data[64];
    int top;
};

void stack_init(struct Stack *s) {
    s->top = -1;
}

int stack_empty(struct Stack *s) {
    return s->top == -1;
}

int stack_size(struct Stack *s) {
    return s->top + 1;
}

void stack_push(struct Stack *s, int val) {
    s->top = s->top + 1;
    s->data[s->top] = val;
}

int stack_pop(struct Stack *s) {
    int val = s->data[s->top];
    s->top = s->top - 1;
    return val;
}

int stack_peek(struct Stack *s) {
    return s->data[s->top];
}

struct TwoStackQueue {
    struct Stack inbox;
    struct Stack outbox;
};

void tsq_init(struct TwoStackQueue *q) {
    stack_init(&q->inbox);
    stack_init(&q->outbox);
}

int tsq_empty(struct TwoStackQueue *q) {
    return stack_empty(&q->inbox) && stack_empty(&q->outbox);
}

int tsq_size(struct TwoStackQueue *q) {
    return stack_size(&q->inbox) + stack_size(&q->outbox);
}

void tsq_enqueue(struct TwoStackQueue *q, int val) {
    stack_push(&q->inbox, val);
}

void tsq_transfer(struct TwoStackQueue *q) {
    if (stack_empty(&q->outbox)) {
        while (!stack_empty(&q->inbox)) {
            stack_push(&q->outbox, stack_pop(&q->inbox));
        }
    }
}

int tsq_dequeue(struct TwoStackQueue *q) {
    tsq_transfer(q);
    if (stack_empty(&q->outbox)) return -999;
    return stack_pop(&q->outbox);
}

int tsq_front(struct TwoStackQueue *q) {
    tsq_transfer(q);
    if (stack_empty(&q->outbox)) return -999;
    return stack_peek(&q->outbox);
}

// Generate numbers 1..n in order using two-stack queue
int test_fifo_order(int n) {
    struct TwoStackQueue q;
    tsq_init(&q);
    int i = 1;
    while (i <= n) {
        tsq_enqueue(&q, i);
        i = i + 1;
    }
    int ok = 1;
    i = 1;
    while (i <= n) {
        int val = tsq_dequeue(&q);
        if (val != i) ok = 0;
        i = i + 1;
    }
    return ok;
}

// Simulate a task scheduler with round-robin using two-stack queue
// Each task has remaining_time. Process 1 unit per round.
// Return total rounds until all done.
int round_robin(int *tasks, int n) {
    struct TwoStackQueue q;
    tsq_init(&q);
    int i = 0;
    while (i < n) {
        tsq_enqueue(&q, tasks[i]);
        i = i + 1;
    }
    int rounds = 0;
    while (!tsq_empty(&q)) {
        int t = tsq_dequeue(&q);
        rounds = rounds + 1;
        t = t - 1;
        if (t > 0) {
            tsq_enqueue(&q, t);
        }
    }
    return rounds;
}

// Interleave enqueue and dequeue operations, verify ordering
int interleave_test() {
    struct TwoStackQueue q;
    tsq_init(&q);
    int result = 0;

    // Enqueue 1,2,3
    tsq_enqueue(&q, 1);
    tsq_enqueue(&q, 2);
    tsq_enqueue(&q, 3);

    // Dequeue 1,2
    result = result + tsq_dequeue(&q); // 1
    result = result + tsq_dequeue(&q); // 2

    // Enqueue 4,5
    tsq_enqueue(&q, 4);
    tsq_enqueue(&q, 5);

    // Dequeue 3,4,5
    result = result + tsq_dequeue(&q); // 3
    result = result + tsq_dequeue(&q); // 4
    result = result + tsq_dequeue(&q); // 5

    // result = 1+2+3+4+5 = 15
    return result;
}

int main() {
    struct TwoStackQueue q;
    tsq_init(&q);

    printf("%d\n", tsq_empty(&q));
    // EXPECT: 1
    printf("%d\n", tsq_size(&q));
    // EXPECT: 0

    tsq_enqueue(&q, 10);
    tsq_enqueue(&q, 20);
    tsq_enqueue(&q, 30);

    printf("%d\n", tsq_size(&q));
    // EXPECT: 3
    printf("%d\n", tsq_front(&q));
    // EXPECT: 10

    printf("%d\n", tsq_dequeue(&q));
    // EXPECT: 10

    // Add more while outbox still has items
    tsq_enqueue(&q, 40);
    tsq_enqueue(&q, 50);

    printf("%d\n", tsq_dequeue(&q));
    // EXPECT: 20
    printf("%d\n", tsq_dequeue(&q));
    // EXPECT: 30
    printf("%d\n", tsq_dequeue(&q));
    // EXPECT: 40
    printf("%d\n", tsq_dequeue(&q));
    // EXPECT: 50
    printf("%d\n", tsq_empty(&q));
    // EXPECT: 1

    // Empty dequeue
    printf("%d\n", tsq_dequeue(&q));
    // EXPECT: -999

    // FIFO order test
    printf("%d\n", test_fifo_order(20));
    // EXPECT: 1

    // Interleave test
    printf("%d\n", interleave_test());
    // EXPECT: 15

    // Round-robin scheduler: tasks with times 3, 1, 4, 2
    // Round 1: process 3->2, 1->0(done), 4->3, 2->1 = 4 rounds
    // Round 2: 2->1, 3->2, 1->0(done) = 3 rounds
    // Round 3: 1->0(done), 2->1 = 2 rounds
    // Round 4: 1->0(done) = 1 round
    // Total = 3+1+4+2 = 10
    int tasks[4];
    tasks[0] = 3; tasks[1] = 1; tasks[2] = 4; tasks[3] = 2;
    printf("%d\n", round_robin(tasks, 4));
    // EXPECT: 10

    // Round-robin with all equal
    int tasks2[5];
    tasks2[0] = 3; tasks2[1] = 3; tasks2[2] = 3; tasks2[3] = 3; tasks2[4] = 3;
    // Total = 5*3 = 15
    printf("%d\n", round_robin(tasks2, 5));
    // EXPECT: 15

    // Large interleaved test
    struct TwoStackQueue q2;
    tsq_init(&q2);
    int sum = 0;
    int j = 0;
    while (j < 30) {
        tsq_enqueue(&q2, j);
        if (j % 3 == 2) {
            // Dequeue one every 3 enqueues
            sum = sum + tsq_dequeue(&q2);
        }
        j = j + 1;
    }
    // Dequeues happened at j=2,5,8,11,14,17,20,23,26,29
    // Values dequeued: 0,1,2,3,4,5,6,7,8,9
    // sum = 0+1+2+3+4+5+6+7+8+9 = 45
    // Remaining in queue: 10,11,...,29 minus already dequeued
    // Actually remaining: 10..29 = 20 elements
    while (!tsq_empty(&q2)) {
        sum = sum + tsq_dequeue(&q2);
    }
    // Total sum = 0+1+2+...+29 = 435
    printf("%d\n", sum);
    // EXPECT: 435

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
