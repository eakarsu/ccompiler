int printf(const char *fmt, ...);

struct Stack {
    int data[20];
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
    s->top++;
    s->data[s->top] = val;
}

int stack_pop(struct Stack *s) {
    int val = s->data[s->top];
    s->top--;
    return val;
}

int stack_peek(struct Stack *s) {
    return s->data[s->top];
}

struct QueueTwoStacks {
    struct Stack s1;
    struct Stack s2;
};

void q2s_init(struct QueueTwoStacks *q) {
    stack_init(&q->s1);
    stack_init(&q->s2);
}

void q2s_enqueue(struct QueueTwoStacks *q, int val) {
    stack_push(&q->s1, val);
}

int q2s_dequeue(struct QueueTwoStacks *q) {
    if (stack_empty(&q->s2)) {
        while (!stack_empty(&q->s1)) {
            stack_push(&q->s2, stack_pop(&q->s1));
        }
    }
    return stack_pop(&q->s2);
}

int q2s_peek(struct QueueTwoStacks *q) {
    if (stack_empty(&q->s2)) {
        while (!stack_empty(&q->s1)) {
            stack_push(&q->s2, stack_pop(&q->s1));
        }
    }
    return stack_peek(&q->s2);
}

int q2s_empty(struct QueueTwoStacks *q) {
    return stack_empty(&q->s1) && stack_empty(&q->s2);
}

struct Queue {
    int data[20];
    int front;
    int rear;
    int count;
};

void queue_init(struct Queue *q) {
    q->front = 0;
    q->rear = 0;
    q->count = 0;
}

int queue_empty(struct Queue *q) {
    return q->count == 0;
}

int queue_size(struct Queue *q) {
    return q->count;
}

void queue_enqueue(struct Queue *q, int val) {
    q->data[q->rear] = val;
    q->rear++;
    if (q->rear >= 20) q->rear = 0;
    q->count++;
}

int queue_dequeue(struct Queue *q) {
    int val = q->data[q->front];
    q->front++;
    if (q->front >= 20) q->front = 0;
    q->count--;
    return val;
}

int queue_peek(struct Queue *q) {
    return q->data[q->front];
}

struct StackTwoQueues {
    struct Queue q1;
    struct Queue q2;
};

void s2q_init(struct StackTwoQueues *s) {
    queue_init(&s->q1);
    queue_init(&s->q2);
}

void s2q_push(struct StackTwoQueues *s, int val) {
    queue_enqueue(&s->q2, val);
    while (!queue_empty(&s->q1)) {
        queue_enqueue(&s->q2, queue_dequeue(&s->q1));
    }
    struct Queue tmp;
    tmp = s->q1;
    s->q1 = s->q2;
    s->q2 = tmp;
}

int s2q_pop(struct StackTwoQueues *s) {
    return queue_dequeue(&s->q1);
}

int s2q_peek(struct StackTwoQueues *s) {
    return queue_peek(&s->q1);
}

int s2q_empty(struct StackTwoQueues *s) {
    return queue_empty(&s->q1);
}

int main(void) {
    struct QueueTwoStacks q;
    q2s_init(&q);

    // EXPECT: q_empty=1
    printf("q_empty=%d\n", q2s_empty(&q));

    q2s_enqueue(&q, 10);
    q2s_enqueue(&q, 20);
    q2s_enqueue(&q, 30);

    // EXPECT: q_peek=10
    printf("q_peek=%d\n", q2s_peek(&q));

    // EXPECT: q_deq=10
    printf("q_deq=%d\n", q2s_dequeue(&q));
    // EXPECT: q_deq=20
    printf("q_deq=%d\n", q2s_dequeue(&q));

    q2s_enqueue(&q, 40);
    q2s_enqueue(&q, 50);

    // EXPECT: q_deq=30
    printf("q_deq=%d\n", q2s_dequeue(&q));
    // EXPECT: q_deq=40
    printf("q_deq=%d\n", q2s_dequeue(&q));
    // EXPECT: q_deq=50
    printf("q_deq=%d\n", q2s_dequeue(&q));

    // EXPECT: q_empty_after=1
    printf("q_empty_after=%d\n", q2s_empty(&q));

    q2s_enqueue(&q, 1);
    q2s_enqueue(&q, 2);
    q2s_enqueue(&q, 3);
    q2s_enqueue(&q, 4);
    q2s_enqueue(&q, 5);
    int i;
    // EXPECT: fifo: 1 2 3 4 5
    printf("fifo:");
    for (i = 0; i < 5; i++) {
        printf(" %d", q2s_dequeue(&q));
    }
    printf("\n");

    struct StackTwoQueues s;
    s2q_init(&s);

    // EXPECT: s_empty=1
    printf("s_empty=%d\n", s2q_empty(&s));

    s2q_push(&s, 10);
    s2q_push(&s, 20);
    s2q_push(&s, 30);

    // EXPECT: s_peek=30
    printf("s_peek=%d\n", s2q_peek(&s));

    // EXPECT: s_pop=30
    printf("s_pop=%d\n", s2q_pop(&s));
    // EXPECT: s_pop=20
    printf("s_pop=%d\n", s2q_pop(&s));

    s2q_push(&s, 40);
    s2q_push(&s, 50);

    // EXPECT: s_pop=50
    printf("s_pop=%d\n", s2q_pop(&s));
    // EXPECT: s_pop=40
    printf("s_pop=%d\n", s2q_pop(&s));
    // EXPECT: s_pop=10
    printf("s_pop=%d\n", s2q_pop(&s));

    // EXPECT: s_empty_after=1
    printf("s_empty_after=%d\n", s2q_empty(&s));

    s2q_push(&s, 1);
    s2q_push(&s, 2);
    s2q_push(&s, 3);
    s2q_push(&s, 4);
    s2q_push(&s, 5);
    // EXPECT: lifo: 5 4 3 2 1
    printf("lifo:");
    for (i = 0; i < 5; i++) {
        printf(" %d", s2q_pop(&s));
    }
    printf("\n");

    // EXPECT: done
    printf("done\n");

    return 0;
}
