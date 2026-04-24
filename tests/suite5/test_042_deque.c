int printf(const char *fmt, ...);

int dq[32];
int dq_front;
int dq_back;
int dq_count;

void deque_init(void) {
    dq_front = 0;
    dq_back = 0;
    dq_count = 0;
}

void push_back(int val) {
    dq[dq_back] = val;
    dq_back = (dq_back + 1) % 32;
    dq_count = dq_count + 1;
}

void push_front(int val) {
    dq_front = (dq_front - 1 + 32) % 32;
    dq[dq_front] = val;
    dq_count = dq_count + 1;
}

int pop_front(void) {
    int val;
    val = dq[dq_front];
    dq_front = (dq_front + 1) % 32;
    dq_count = dq_count - 1;
    return val;
}

int pop_back(void) {
    int val;
    dq_back = (dq_back - 1 + 32) % 32;
    val = dq[dq_back];
    dq_count = dq_count - 1;
    return val;
}

int peek_front(void) {
    return dq[dq_front];
}

int peek_back(void) {
    int idx;
    idx = (dq_back - 1 + 32) % 32;
    return dq[idx];
}

int deque_empty(void) {
    if (dq_count == 0) return 1;
    return 0;
}

int deque_size(void) {
    return dq_count;
}

int main(void) {
    int val;
    int i;

    deque_init();

    // EXPECT: empty: 1
    printf("empty: %d\n", deque_empty());

    push_back(10);
    push_back(20);
    push_back(30);

    // EXPECT: size: 3
    printf("size: %d\n", deque_size());

    // EXPECT: front: 10
    printf("front: %d\n", peek_front());

    // EXPECT: back: 30
    printf("back: %d\n", peek_back());

    push_front(5);
    // EXPECT: new front: 5
    printf("new front: %d\n", peek_front());

    // EXPECT: size: 4
    printf("size: %d\n", deque_size());

    val = pop_front();
    // EXPECT: popped front: 5
    printf("popped front: %d\n", val);

    val = pop_back();
    // EXPECT: popped back: 30
    printf("popped back: %d\n", val);

    // EXPECT: size: 2
    printf("size: %d\n", deque_size());

    // EXPECT: front: 10
    printf("front: %d\n", peek_front());

    // EXPECT: back: 20
    printf("back: %d\n", peek_back());

    val = pop_front();
    val = pop_front();
    // EXPECT: empty after pops: 1
    printf("empty after pops: %d\n", deque_empty());

    push_front(100);
    push_front(200);
    push_back(300);
    push_back(400);

    // EXPECT: front: 200
    printf("front: %d\n", peek_front());

    // EXPECT: back: 400
    printf("back: %d\n", peek_back());

    // EXPECT: size: 4
    printf("size: %d\n", deque_size());

    val = pop_front();
    // EXPECT: f: 200
    printf("f: %d\n", val);
    val = pop_front();
    // EXPECT: f: 100
    printf("f: %d\n", val);
    val = pop_back();
    // EXPECT: b: 400
    printf("b: %d\n", val);
    val = pop_back();
    // EXPECT: b: 300
    printf("b: %d\n", val);

    // EXPECT: final empty: 1
    printf("final empty: %d\n", deque_empty());

    deque_init();
    i = 0;
    while (i < 10) {
        push_back(i * 3);
        i = i + 1;
    }
    // EXPECT: bulk size: 10
    printf("bulk size: %d\n", deque_size());
    // EXPECT: bulk front: 0
    printf("bulk front: %d\n", peek_front());
    // EXPECT: bulk back: 27
    printf("bulk back: %d\n", peek_back());

    return 0;
}
