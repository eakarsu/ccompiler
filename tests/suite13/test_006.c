int printf(const char *fmt, ...);

// Priority queue using sorted array (min-priority queue)
// Elements are kept sorted so that dequeue always returns the minimum

struct PriorityQueue {
    int keys[64];
    int vals[64];
    int count;
};

void pq_init(struct PriorityQueue *pq) {
    pq->count = 0;
}

int pq_empty(struct PriorityQueue *pq) {
    return pq->count == 0;
}

int pq_size(struct PriorityQueue *pq) {
    return pq->count;
}

// Insert maintaining sorted order (ascending by key)
void pq_insert(struct PriorityQueue *pq, int key, int val) {
    int pos = pq->count;
    // Find insertion point
    while (pos > 0 && pq->keys[pos - 1] > key) {
        pq->keys[pos] = pq->keys[pos - 1];
        pq->vals[pos] = pq->vals[pos - 1];
        pos = pos - 1;
    }
    pq->keys[pos] = key;
    pq->vals[pos] = val;
    pq->count = pq->count + 1;
}

// Remove and return minimum (front of sorted array)
int pq_dequeue_key(struct PriorityQueue *pq) {
    if (pq->count == 0) return -999;
    int key = pq->keys[0];
    int i = 1;
    while (i < pq->count) {
        pq->keys[i - 1] = pq->keys[i];
        pq->vals[i - 1] = pq->vals[i];
        i = i + 1;
    }
    pq->count = pq->count - 1;
    return key;
}

int pq_dequeue_val(struct PriorityQueue *pq) {
    if (pq->count == 0) return -999;
    int val = pq->vals[0];
    int i = 1;
    while (i < pq->count) {
        pq->keys[i - 1] = pq->keys[i];
        pq->vals[i - 1] = pq->vals[i];
        i = i + 1;
    }
    pq->count = pq->count - 1;
    return val;
}

int pq_peek_key(struct PriorityQueue *pq) {
    if (pq->count == 0) return -999;
    return pq->keys[0];
}

int pq_peek_val(struct PriorityQueue *pq) {
    if (pq->count == 0) return -999;
    return pq->vals[0];
}

// Merge two sorted arrays using priority queue
void merge_sorted(int *a, int na, int *b, int nb, int *out) {
    struct PriorityQueue pq;
    pq_init(&pq);
    int ai = 0;
    int bi = 0;
    // 0 = from a, 1 = from b, encoded in val
    while (ai < na) {
        pq_insert(&pq, a[ai], 0);
        ai = ai + 1;
    }
    while (bi < nb) {
        pq_insert(&pq, b[bi], 1);
        bi = bi + 1;
    }
    int oi = 0;
    while (!pq_empty(&pq)) {
        out[oi] = pq_dequeue_key(&pq);
        oi = oi + 1;
    }
}

// Find k-th smallest element in an unsorted array
int kth_smallest(int *arr, int n, int k) {
    struct PriorityQueue pq;
    pq_init(&pq);
    int i = 0;
    while (i < n) {
        pq_insert(&pq, arr[i], i);
        i = i + 1;
    }
    int result = 0;
    i = 0;
    while (i < k) {
        result = pq_dequeue_key(&pq);
        i = i + 1;
    }
    return result;
}

// Task scheduling: process tasks with deadlines, return count done on time
// Each task has (deadline, processing_time). Process in order of deadline.
int schedule_tasks(int *deadlines, int *proc_times, int n) {
    struct PriorityQueue pq;
    pq_init(&pq);
    int i = 0;
    while (i < n) {
        pq_insert(&pq, deadlines[i], proc_times[i]);
        i = i + 1;
    }
    int time = 0;
    int done = 0;
    while (!pq_empty(&pq)) {
        int dl = pq_peek_key(&pq);
        int pt = pq_peek_val(&pq);
        pq_dequeue_key(&pq);
        time = time + pt;
        if (time <= dl) {
            done = done + 1;
        }
    }
    return done;
}

int main() {
    struct PriorityQueue pq;
    pq_init(&pq);

    printf("%d\n", pq_empty(&pq));
    // EXPECT: 1

    pq_insert(&pq, 30, 300);
    pq_insert(&pq, 10, 100);
    pq_insert(&pq, 50, 500);
    pq_insert(&pq, 20, 200);
    pq_insert(&pq, 40, 400);

    printf("%d\n", pq_size(&pq));
    // EXPECT: 5
    printf("%d\n", pq_peek_key(&pq));
    // EXPECT: 10
    printf("%d\n", pq_peek_val(&pq));
    // EXPECT: 100

    printf("%d\n", pq_dequeue_key(&pq));
    // EXPECT: 10
    printf("%d\n", pq_dequeue_key(&pq));
    // EXPECT: 20
    printf("%d\n", pq_dequeue_key(&pq));
    // EXPECT: 30
    printf("%d\n", pq_dequeue_key(&pq));
    // EXPECT: 40
    printf("%d\n", pq_dequeue_key(&pq));
    // EXPECT: 50
    printf("%d\n", pq_empty(&pq));
    // EXPECT: 1

    // Merge sorted arrays
    int a[4];
    a[0] = 1; a[1] = 5; a[2] = 9; a[3] = 13;
    int b[3];
    b[0] = 2; b[1] = 6; b[2] = 10;
    int out[7];
    merge_sorted(a, 4, b, 3, out);
    printf("%d %d %d %d %d %d %d\n", out[0], out[1], out[2], out[3], out[4], out[5], out[6]);
    // EXPECT: 1 2 5 6 9 10 13

    // K-th smallest
    int arr[8];
    arr[0] = 7; arr[1] = 2; arr[2] = 9; arr[3] = 1;
    arr[4] = 5; arr[5] = 8; arr[6] = 3; arr[7] = 6;
    printf("%d\n", kth_smallest(arr, 8, 1));
    // EXPECT: 1
    printf("%d\n", kth_smallest(arr, 8, 3));
    // EXPECT: 3
    printf("%d\n", kth_smallest(arr, 8, 5));
    // EXPECT: 6
    printf("%d\n", kth_smallest(arr, 8, 8));
    // EXPECT: 9

    // Task scheduling
    int dl[5];
    int pt[5];
    dl[0] = 4; pt[0] = 2;   // done at t=2, deadline=4, OK
    dl[1] = 6; pt[1] = 3;   // done at t=5, deadline=6, OK
    dl[2] = 8; pt[2] = 1;   // done at t=6, deadline=8, OK
    dl[3] = 9; pt[3] = 4;   // done at t=10, deadline=9, LATE
    dl[4] = 15; pt[4] = 2;  // done at t=12, deadline=15, OK
    printf("%d\n", schedule_tasks(dl, pt, 5));
    // EXPECT: 4

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
