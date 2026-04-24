int printf(const char *fmt, ...);
// EXPECT: empty? 1\nafter enqueue: [10 20 30]\nsize = 3\nfront = 10\ndequeue = 10\ndequeue = 20\nafter dequeue: [30]\ncircular: [30 40 50 60 70]\nsize = 5\ndrain: 30 40 50 60 70 \nempty? 1\nBFS: 1 2 3 4 5 6 7 \nFIFO: 0 10 20 30 40 
// Test: pointer-based queue using circular array pool

int queue[10];
int q_front = 0;
int q_back = 0;
int q_count = 0;
int q_cap = 10;

void enqueue(int val) {
    queue[q_back] = val;
    q_back = (q_back + 1) % q_cap;
    q_count = q_count + 1;
}

int dequeue(void) {
    int val = queue[q_front];
    q_front = (q_front + 1) % q_cap;
    q_count = q_count - 1;
    return val;
}

int front(void) {
    return queue[q_front];
}

int q_empty(void) {
    return q_count == 0;
}

int q_size(void) {
    return q_count;
}

void print_queue(void) {
    printf("[");
    int i;
    int idx = q_front;
    for (i = 0; i < q_count; i++) {
        if (i > 0) printf(" ");
        printf("%d", queue[idx]);
        idx = (idx + 1) % q_cap;
    }
    printf("]\n");
}

int main(void) {
    // Basic queue
    printf("empty? %d\n", q_empty());

    enqueue(10);
    enqueue(20);
    enqueue(30);
    printf("after enqueue: ");
    print_queue();
    printf("size = %d\n", q_size());
    printf("front = %d\n", front());

    printf("dequeue = %d\n", dequeue());
    printf("dequeue = %d\n", dequeue());
    printf("after dequeue: ");
    print_queue();

    // Circular behavior - add more after dequeue
    enqueue(40);
    enqueue(50);
    enqueue(60);
    enqueue(70);
    printf("circular: ");
    print_queue();
    printf("size = %d\n", q_size());

    // Drain queue
    printf("drain: ");
    while (!q_empty()) {
        printf("%d ", dequeue());
    }
    printf("\n");
    printf("empty? %d\n", q_empty());

    // BFS-like simulation: process and generate
    enqueue(1);
    printf("BFS: ");
    int steps = 0;
    while (!q_empty() && steps < 7) {
        int val = dequeue();
        printf("%d ", val);
        if (val * 2 <= 8) enqueue(val * 2);
        if (val * 2 + 1 <= 8) enqueue(val * 2 + 1);
        steps = steps + 1;
    }
    printf("\n");

    // Clear remaining
    while (!q_empty()) dequeue();

    // FIFO order test
    int i;
    for (i = 0; i < 5; i++) enqueue(i * 10);
    printf("FIFO: ");
    for (i = 0; i < 5; i++) printf("%d ", dequeue());
    printf("\n");

    return 0;
}
