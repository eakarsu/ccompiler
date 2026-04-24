int printf(const char *fmt, ...);

// Queue using circular buffer

struct Queue {
    int data[32];
    int head;
    int tail;
    int count;
    int capacity;
};

void queue_init(struct Queue *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    q->capacity = 32;
}

int queue_empty(struct Queue *q) {
    return q->count == 0;
}

int queue_full(struct Queue *q) {
    return q->count == q->capacity;
}

int queue_size(struct Queue *q) {
    return q->count;
}

int queue_enqueue(struct Queue *q, int val) {
    if (queue_full(q)) return -1;
    q->data[q->tail] = val;
    q->tail = (q->tail + 1) % q->capacity;
    q->count = q->count + 1;
    return 0;
}

int queue_dequeue(struct Queue *q) {
    if (queue_empty(q)) return -999;
    int val = q->data[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->count = q->count - 1;
    return val;
}

int queue_front(struct Queue *q) {
    if (queue_empty(q)) return -999;
    return q->data[q->head];
}

int queue_back(struct Queue *q) {
    if (queue_empty(q)) return -999;
    int idx = (q->tail - 1 + q->capacity) % q->capacity;
    return q->data[idx];
}

// Generate first n numbers in a hot-potato elimination game
// n people in circle, every k-th person eliminated
int josephus(int n, int k) {
    struct Queue q;
    queue_init(&q);
    int i = 1;
    while (i <= n) {
        queue_enqueue(&q, i);
        i = i + 1;
    }
    while (queue_size(&q) > 1) {
        int j = 0;
        while (j < k - 1) {
            int person = queue_dequeue(&q);
            queue_enqueue(&q, person);
            j = j + 1;
        }
        queue_dequeue(&q); // eliminate
    }
    return queue_front(&q);
}

// Interleave first half with second half of queue
// e.g. [1,2,3,4,5,6] -> [1,4,2,5,3,6]
void interleave_queue(struct Queue *q) {
    int n = queue_size(q);
    int half = n / 2;
    int first[16];
    int second[16];
    int i = 0;
    while (i < half) {
        first[i] = queue_dequeue(q);
        i = i + 1;
    }
    i = 0;
    while (i < n - half) {
        second[i] = queue_dequeue(q);
        i = i + 1;
    }
    i = 0;
    int j = 0;
    int k = 0;
    while (i < half || j < n - half) {
        if (i < half) {
            queue_enqueue(q, first[i]);
            i = i + 1;
        }
        if (j < n - half) {
            queue_enqueue(q, second[j]);
            j = j + 1;
        }
    }
}

int main() {
    struct Queue q;
    queue_init(&q);

    printf("%d\n", queue_empty(&q));
    // EXPECT: 1
    printf("%d\n", queue_size(&q));
    // EXPECT: 0

    queue_enqueue(&q, 10);
    queue_enqueue(&q, 20);
    queue_enqueue(&q, 30);

    printf("%d\n", queue_front(&q));
    // EXPECT: 10
    printf("%d\n", queue_back(&q));
    // EXPECT: 30
    printf("%d\n", queue_size(&q));
    // EXPECT: 3

    printf("%d\n", queue_dequeue(&q));
    // EXPECT: 10
    printf("%d\n", queue_dequeue(&q));
    // EXPECT: 20
    printf("%d\n", queue_front(&q));
    // EXPECT: 30
    printf("%d\n", queue_dequeue(&q));
    // EXPECT: 30
    printf("%d\n", queue_empty(&q));
    // EXPECT: 1
    printf("%d\n", queue_dequeue(&q));
    // EXPECT: -999

    // Wrap-around test: enqueue and dequeue many times to force circular behavior
    struct Queue q2;
    queue_init(&q2);
    int i = 0;
    while (i < 25) {
        queue_enqueue(&q2, i);
        i = i + 1;
    }
    i = 0;
    while (i < 20) {
        queue_dequeue(&q2);
        i = i + 1;
    }
    // Now head is at index 20, 5 elements remain (20..24)
    // Add 10 more: 100..109, these will wrap around
    i = 0;
    while (i < 10) {
        queue_enqueue(&q2, 100 + i);
        i = i + 1;
    }
    printf("%d\n", queue_size(&q2));
    // EXPECT: 15
    printf("%d\n", queue_front(&q2));
    // EXPECT: 20
    printf("%d\n", queue_back(&q2));
    // EXPECT: 109

    // Drain all and sum
    int sum = 0;
    while (!queue_empty(&q2)) {
        sum = sum + queue_dequeue(&q2);
        // 20+21+22+23+24 + 100+101+102+103+104+105+106+107+108+109
    }
    // first part: 20+21+22+23+24 = 110
    // second part: 100+101+102+103+104+105+106+107+108+109 = 1045
    // total = 1155
    printf("%d\n", sum);
    // EXPECT: 1155

    // Josephus problem: 7 people, every 3rd eliminated
    // Elimination order: 3,6,2,7,5,1 -> survivor = 4
    printf("%d\n", josephus(7, 3));
    // EXPECT: 4

    // Josephus: 5 people, every 2nd
    // Elimination: 2,4,1,5 -> survivor = 3
    printf("%d\n", josephus(5, 2));
    // EXPECT: 3

    // Interleave test
    struct Queue q3;
    queue_init(&q3);
    queue_enqueue(&q3, 1);
    queue_enqueue(&q3, 2);
    queue_enqueue(&q3, 3);
    queue_enqueue(&q3, 4);
    queue_enqueue(&q3, 5);
    queue_enqueue(&q3, 6);
    interleave_queue(&q3);
    // Result: 1,4,2,5,3,6
    printf("%d %d %d %d %d %d\n",
        queue_dequeue(&q3), queue_dequeue(&q3), queue_dequeue(&q3),
        queue_dequeue(&q3), queue_dequeue(&q3), queue_dequeue(&q3));
    // EXPECT: 1 4 2 5 3 6

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
