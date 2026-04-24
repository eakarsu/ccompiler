int printf(const char *fmt, ...);

// Deque (double-ended queue) with circular buffer

struct Deque {
    int data[16];
    int front;
    int back;
    int size;
    int capacity;
};

void deque_init(struct Deque *d) {
    d->front = 0;
    d->back = 0;
    d->size = 0;
    d->capacity = 16;
}

int deque_empty(struct Deque *d) {
    return d->size == 0;
}

int deque_full(struct Deque *d) {
    return d->size == d->capacity;
}

int deque_push_front(struct Deque *d, int val) {
    if (deque_full(d)) return 0;
    d->front = (d->front - 1 + d->capacity) % d->capacity;
    d->data[d->front] = val;
    d->size = d->size + 1;
    return 1;
}

int deque_push_back(struct Deque *d, int val) {
    if (deque_full(d)) return 0;
    d->data[d->back] = val;
    d->back = (d->back + 1) % d->capacity;
    d->size = d->size + 1;
    return 1;
}

int deque_pop_front(struct Deque *d) {
    int val;
    if (deque_empty(d)) return -999;
    val = d->data[d->front];
    d->front = (d->front + 1) % d->capacity;
    d->size = d->size - 1;
    return val;
}

int deque_pop_back(struct Deque *d) {
    int val;
    if (deque_empty(d)) return -999;
    d->back = (d->back - 1 + d->capacity) % d->capacity;
    val = d->data[d->back];
    d->size = d->size - 1;
    return val;
}

int deque_peek_front(struct Deque *d) {
    if (deque_empty(d)) return -999;
    return d->data[d->front];
}

int deque_peek_back(struct Deque *d) {
    int idx;
    if (deque_empty(d)) return -999;
    idx = (d->back - 1 + d->capacity) % d->capacity;
    return d->data[idx];
}

// Sliding window maximum using deque of indices
void sliding_window_max(int *arr, int n, int k, int *result) {
    struct Deque dq;
    int i;
    int ri = 0;
    deque_init(&dq);

    for (i = 0; i < n; i++) {
        // Remove elements outside window
        while (!deque_empty(&dq) && deque_peek_front(&dq) <= i - k) {
            deque_pop_front(&dq);
        }
        // Remove smaller elements from back
        while (!deque_empty(&dq) && arr[deque_peek_back(&dq)] <= arr[i]) {
            deque_pop_back(&dq);
        }
        deque_push_back(&dq, i);
        if (i >= k - 1) {
            result[ri] = arr[deque_peek_front(&dq)];
            ri = ri + 1;
        }
    }
}

// Use deque as both stack and queue
void deque_print_all(struct Deque *d) {
    int i;
    int idx;
    for (i = 0; i < d->size; i++) {
        idx = (d->front + i) % d->capacity;
        if (i > 0) printf(" ");
        printf("%d", d->data[idx]);
    }
}

int main() {
    struct Deque dq;
    int val;
    int arr[8];
    int result[8];
    int i;

    deque_init(&dq);
    printf("empty: %d\n", deque_empty(&dq));           // EXPECT: empty: 1

    deque_push_back(&dq, 10);
    deque_push_back(&dq, 20);
    deque_push_back(&dq, 30);
    printf("front: %d\n", deque_peek_front(&dq));       // EXPECT: front: 10
    printf("back: %d\n", deque_peek_back(&dq));         // EXPECT: back: 30
    printf("size: %d\n", dq.size);                      // EXPECT: size: 3

    deque_push_front(&dq, 5);
    printf("after push_front 5: ");
    deque_print_all(&dq);
    printf("\n");                                        // EXPECT: after push_front 5: 5 10 20 30

    val = deque_pop_front(&dq);
    printf("pop_front: %d\n", val);                     // EXPECT: pop_front: 5

    val = deque_pop_back(&dq);
    printf("pop_back: %d\n", val);                      // EXPECT: pop_back: 30

    printf("remaining: ");
    deque_print_all(&dq);
    printf("\n");                                        // EXPECT: remaining: 10 20

    // Test wrap-around
    deque_init(&dq);
    for (i = 0; i < 10; i++) {
        deque_push_back(&dq, i * 10);
    }
    for (i = 0; i < 8; i++) {
        deque_pop_front(&dq);
    }
    deque_push_back(&dq, 100);
    deque_push_back(&dq, 110);
    deque_push_back(&dq, 120);
    printf("wrap: ");
    deque_print_all(&dq);
    printf("\n");                                        // EXPECT: wrap: 80 90 100 110 120
    printf("wrap size: %d\n", dq.size);                 // EXPECT: wrap size: 5

    // Sliding window maximum
    arr[0] = 1; arr[1] = 3; arr[2] = -1; arr[3] = -3;
    arr[4] = 5; arr[5] = 3; arr[6] = 6; arr[7] = 7;
    sliding_window_max(arr, 8, 3, result);
    printf("sliding max k=3: ");
    for (i = 0; i < 6; i++) {
        if (i > 0) printf(" ");
        printf("%d", result[i]);
    }
    printf("\n");                                        // EXPECT: sliding max k=3: 3 3 5 5 6 7

    // Palindrome check using deque
    deque_init(&dq);
    deque_push_back(&dq, 1);
    deque_push_back(&dq, 2);
    deque_push_back(&dq, 3);
    deque_push_back(&dq, 2);
    deque_push_back(&dq, 1);
    {
        int is_pal = 1;
        while (dq.size > 1) {
            if (deque_pop_front(&dq) != deque_pop_back(&dq)) {
                is_pal = 0;
            }
        }
        printf("palindrome: %d\n", is_pal);            // EXPECT: palindrome: 1
    }

    return 0;
}
