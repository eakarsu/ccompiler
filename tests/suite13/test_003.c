int printf(const char *fmt, ...);

// Deque (double-ended queue) operations

struct Deque {
    int data[64];
    int head;
    int tail;
    int count;
    int capacity;
};

void deque_init(struct Deque *d) {
    d->head = 32;
    d->tail = 32;
    d->count = 0;
    d->capacity = 64;
}

int deque_empty(struct Deque *d) {
    return d->count == 0;
}

int deque_size(struct Deque *d) {
    return d->count;
}

void deque_push_back(struct Deque *d, int val) {
    d->data[d->tail] = val;
    d->tail = (d->tail + 1) % d->capacity;
    d->count = d->count + 1;
}

void deque_push_front(struct Deque *d, int val) {
    d->head = (d->head - 1 + d->capacity) % d->capacity;
    d->data[d->head] = val;
    d->count = d->count + 1;
}

int deque_pop_back(struct Deque *d) {
    if (deque_empty(d)) return -999;
    d->tail = (d->tail - 1 + d->capacity) % d->capacity;
    d->count = d->count - 1;
    return d->data[d->tail];
}

int deque_pop_front(struct Deque *d) {
    if (deque_empty(d)) return -999;
    int val = d->data[d->head];
    d->head = (d->head + 1) % d->capacity;
    d->count = d->count - 1;
    return val;
}

int deque_front(struct Deque *d) {
    if (deque_empty(d)) return -999;
    return d->data[d->head];
}

int deque_back(struct Deque *d) {
    if (deque_empty(d)) return -999;
    int idx = (d->tail - 1 + d->capacity) % d->capacity;
    return d->data[idx];
}

int deque_at(struct Deque *d, int i) {
    int idx = (d->head + i) % d->capacity;
    return d->data[idx];
}

// Check if deque is a palindrome
int is_palindrome(struct Deque *d) {
    struct Deque tmp;
    deque_init(&tmp);
    // Copy elements
    int i = 0;
    while (i < deque_size(d)) {
        deque_push_back(&tmp, deque_at(d, i));
        i = i + 1;
    }
    while (deque_size(&tmp) > 1) {
        int front = deque_pop_front(&tmp);
        int back = deque_pop_back(&tmp);
        if (front != back) return 0;
    }
    return 1;
}

// Sliding window maximum using deque of indices
void sliding_window_max(int *arr, int n, int k, int *result) {
    struct Deque dq;
    deque_init(&dq);
    int i = 0;
    int ri = 0;
    while (i < n) {
        // Remove elements outside window
        while (!deque_empty(&dq) && deque_front(&dq) <= i - k) {
            deque_pop_front(&dq);
        }
        // Remove smaller elements from back
        while (!deque_empty(&dq) && arr[deque_back(&dq)] <= arr[i]) {
            deque_pop_back(&dq);
        }
        deque_push_back(&dq, i);
        if (i >= k - 1) {
            result[ri] = arr[deque_front(&dq)];
            ri = ri + 1;
        }
        i = i + 1;
    }
}

int main() {
    struct Deque d;
    deque_init(&d);

    printf("%d\n", deque_empty(&d));
    // EXPECT: 1

    deque_push_back(&d, 10);
    deque_push_back(&d, 20);
    deque_push_front(&d, 5);
    // Deque: [5, 10, 20]

    printf("%d\n", deque_size(&d));
    // EXPECT: 3
    printf("%d\n", deque_front(&d));
    // EXPECT: 5
    printf("%d\n", deque_back(&d));
    // EXPECT: 20

    printf("%d\n", deque_pop_front(&d));
    // EXPECT: 5
    printf("%d\n", deque_pop_back(&d));
    // EXPECT: 20
    printf("%d\n", deque_front(&d));
    // EXPECT: 10
    printf("%d\n", deque_size(&d));
    // EXPECT: 1

    deque_push_front(&d, 1);
    deque_push_front(&d, 2);
    deque_push_back(&d, 3);
    deque_push_back(&d, 4);
    // Deque: [2, 1, 10, 3, 4]

    printf("%d\n", deque_size(&d));
    // EXPECT: 5
    printf("%d %d %d %d %d\n",
        deque_at(&d, 0), deque_at(&d, 1), deque_at(&d, 2),
        deque_at(&d, 3), deque_at(&d, 4));
    // EXPECT: 2 1 10 3 4

    // Palindrome check
    struct Deque p1;
    deque_init(&p1);
    deque_push_back(&p1, 1);
    deque_push_back(&p1, 2);
    deque_push_back(&p1, 3);
    deque_push_back(&p1, 2);
    deque_push_back(&p1, 1);
    printf("%d\n", is_palindrome(&p1));
    // EXPECT: 1

    struct Deque p2;
    deque_init(&p2);
    deque_push_back(&p2, 1);
    deque_push_back(&p2, 2);
    deque_push_back(&p2, 3);
    deque_push_back(&p2, 4);
    printf("%d\n", is_palindrome(&p2));
    // EXPECT: 0

    // Sliding window maximum
    int arr[8];
    arr[0] = 1; arr[1] = 3; arr[2] = -1; arr[3] = -3;
    arr[4] = 5; arr[5] = 3; arr[6] = 6; arr[7] = 7;
    int result[6];
    sliding_window_max(arr, 8, 3, result);
    // Windows: [1,3,-1]->3, [3,-1,-3]->3, [-1,-3,5]->5, [-3,5,3]->5, [5,3,6]->6, [3,6,7]->7
    printf("%d %d %d %d %d %d\n",
        result[0], result[1], result[2], result[3], result[4], result[5]);
    // EXPECT: 3 3 5 5 6 7

    // Use deque as both stack and queue
    struct Deque d2;
    deque_init(&d2);
    int j = 0;
    while (j < 10) {
        if (j % 2 == 0) {
            deque_push_front(&d2, j);
        } else {
            deque_push_back(&d2, j);
        }
        j = j + 1;
    }
    // Push order: front(0), back(1), front(2), back(3), front(4), back(5), front(6), back(7), front(8), back(9)
    // Deque: [8, 6, 4, 2, 0, 1, 3, 5, 7, 9]
    int sum = 0;
    while (!deque_empty(&d2)) {
        sum = sum + deque_pop_front(&d2);
    }
    // sum = 0+1+2+3+4+5+6+7+8+9 = 45
    printf("%d\n", sum);
    // EXPECT: 45

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
