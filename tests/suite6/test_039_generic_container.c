int printf(const char *fmt, ...);

typedef struct {
    int data[16];
    int size;
    int capacity;
} IntStack;

typedef struct {
    int data[16];
    int head;
    int tail;
    int count;
} IntQueue;

typedef struct {
    int keys[8];
    int values[8];
    int count;
} IntMap;

void stack_init(IntStack *s) {
    s->size = 0;
    s->capacity = 16;
}

int stack_push(IntStack *s, int val) {
    if (s->size >= s->capacity) return 0;
    s->data[s->size] = val;
    s->size = s->size + 1;
    return 1;
}

int stack_pop(IntStack *s) {
    if (s->size <= 0) return -1;
    s->size = s->size - 1;
    return s->data[s->size];
}

int stack_peek(IntStack *s) {
    if (s->size <= 0) return -1;
    return s->data[s->size - 1];
}

int stack_is_empty(IntStack *s) {
    return s->size == 0;
}

int stack_contains(IntStack *s, int val) {
    int i;
    for (i = 0; i < s->size; i = i + 1) {
        if (s->data[i] == val) return 1;
    }
    return 0;
}

void queue_init(IntQueue *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

int queue_enqueue(IntQueue *q, int val) {
    if (q->count >= 16) return 0;
    q->data[q->tail] = val;
    q->tail = (q->tail + 1) % 16;
    q->count = q->count + 1;
    return 1;
}

int queue_dequeue(IntQueue *q) {
    if (q->count <= 0) return -1;
    int val = q->data[q->head];
    q->head = (q->head + 1) % 16;
    q->count = q->count - 1;
    return val;
}

int queue_front(IntQueue *q) {
    if (q->count <= 0) return -1;
    return q->data[q->head];
}

int queue_is_empty(IntQueue *q) {
    return q->count == 0;
}

void map_init(IntMap *m) {
    m->count = 0;
}

int map_put(IntMap *m, int key, int value) {
    int i;
    for (i = 0; i < m->count; i = i + 1) {
        if (m->keys[i] == key) {
            m->values[i] = value;
            return 1;
        }
    }
    if (m->count >= 8) return 0;
    m->keys[m->count] = key;
    m->values[m->count] = value;
    m->count = m->count + 1;
    return 1;
}

int map_get(IntMap *m, int key, int default_val) {
    int i;
    for (i = 0; i < m->count; i = i + 1) {
        if (m->keys[i] == key) return m->values[i];
    }
    return default_val;
}

int map_contains(IntMap *m, int key) {
    int i;
    for (i = 0; i < m->count; i = i + 1) {
        if (m->keys[i] == key) return 1;
    }
    return 0;
}

int map_remove(IntMap *m, int key) {
    int i;
    for (i = 0; i < m->count; i = i + 1) {
        if (m->keys[i] == key) {
            int j;
            for (j = i; j < m->count - 1; j = j + 1) {
                m->keys[j] = m->keys[j + 1];
                m->values[j] = m->values[j + 1];
            }
            m->count = m->count - 1;
            return 1;
        }
    }
    return 0;
}

int main(void) {
    IntStack s;
    stack_init(&s);
    // EXPECT: stack empty: 1
    printf("stack empty: %d\n", stack_is_empty(&s));

    stack_push(&s, 10);
    stack_push(&s, 20);
    stack_push(&s, 30);
    // EXPECT: stack size: 3
    printf("stack size: %d\n", s.size);

    // EXPECT: peek: 30
    printf("peek: %d\n", stack_peek(&s));

    // EXPECT: pop: 30
    printf("pop: %d\n", stack_pop(&s));

    // EXPECT: pop: 20
    printf("pop: %d\n", stack_pop(&s));

    // EXPECT: peek after pops: 10
    printf("peek after pops: %d\n", stack_peek(&s));

    // EXPECT: contains 10: 1
    printf("contains 10: %d\n", stack_contains(&s, 10));

    // EXPECT: contains 99: 0
    printf("contains 99: %d\n", stack_contains(&s, 99));

    IntQueue q;
    queue_init(&q);
    // EXPECT: queue empty: 1
    printf("queue empty: %d\n", queue_is_empty(&q));

    queue_enqueue(&q, 100);
    queue_enqueue(&q, 200);
    queue_enqueue(&q, 300);
    // EXPECT: queue count: 3
    printf("queue count: %d\n", q.count);

    // EXPECT: front: 100
    printf("front: %d\n", queue_front(&q));

    // EXPECT: dequeue: 100
    printf("dequeue: %d\n", queue_dequeue(&q));

    // EXPECT: dequeue: 200
    printf("dequeue: %d\n", queue_dequeue(&q));

    // EXPECT: front after dequeue: 300
    printf("front after dequeue: %d\n", queue_front(&q));

    queue_enqueue(&q, 400);
    // EXPECT: queue count: 2
    printf("queue count: %d\n", q.count);

    IntMap m;
    map_init(&m);
    map_put(&m, 1, 100);
    map_put(&m, 2, 200);
    map_put(&m, 3, 300);
    // EXPECT: map count: 3
    printf("map count: %d\n", m.count);

    // EXPECT: get 1: 100
    printf("get 1: %d\n", map_get(&m, 1, -1));

    // EXPECT: get 2: 200
    printf("get 2: %d\n", map_get(&m, 2, -1));

    // EXPECT: get 99: -1
    printf("get 99: %d\n", map_get(&m, 99, -1));

    // EXPECT: contains 3: 1
    printf("contains 3: %d\n", map_contains(&m, 3));

    // EXPECT: contains 5: 0
    printf("contains 5: %d\n", map_contains(&m, 5));

    map_put(&m, 2, 999);
    // EXPECT: updated 2: 999
    printf("updated 2: %d\n", map_get(&m, 2, -1));

    map_remove(&m, 2);
    // EXPECT: after remove count: 2
    printf("after remove count: %d\n", m.count);

    // EXPECT: get removed 2: -1
    printf("get removed 2: %d\n", map_get(&m, 2, -1));

    IntStack s2;
    stack_init(&s2);
    stack_push(&s2, 5);
    stack_push(&s2, 3);
    stack_push(&s2, 8);
    stack_push(&s2, 1);
    int sum = 0;
    while (!stack_is_empty(&s2)) {
        sum = sum + stack_pop(&s2);
    }
    // EXPECT: stack sum: 17
    printf("stack sum: %d\n", sum);

    return 0;
}
