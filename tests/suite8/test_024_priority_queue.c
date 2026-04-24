int printf(const char *fmt, ...);

/* Priority queue (min-heap) using array */

struct PriorityQueue {
    int data[32];
    int size;
};

void pq_init(struct PriorityQueue *pq) {
    pq->size = 0;
    int i;
    for (i = 0; i < 32; i++) {
        pq->data[i] = 0;
    }
}

void pq_swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int pq_parent(int i) {
    return (i - 1) / 2;
}

int pq_left(int i) {
    return 2 * i + 1;
}

int pq_right(int i) {
    return 2 * i + 2;
}

void pq_sift_up(struct PriorityQueue *pq, int i) {
    while (i > 0 && pq->data[pq_parent(i)] > pq->data[i]) {
        pq_swap(&pq->data[pq_parent(i)], &pq->data[i]);
        i = pq_parent(i);
    }
}

void pq_sift_down(struct PriorityQueue *pq, int i) {
    int smallest;
    int l;
    int r;
    while (1) {
        smallest = i;
        l = pq_left(i);
        r = pq_right(i);
        if (l < pq->size && pq->data[l] < pq->data[smallest]) {
            smallest = l;
        }
        if (r < pq->size && pq->data[r] < pq->data[smallest]) {
            smallest = r;
        }
        if (smallest == i) break;
        pq_swap(&pq->data[i], &pq->data[smallest]);
        i = smallest;
    }
}

int pq_insert(struct PriorityQueue *pq, int val) {
    if (pq->size >= 32) return 0;
    pq->data[pq->size] = val;
    pq_sift_up(pq, pq->size);
    pq->size = pq->size + 1;
    return 1;
}

int pq_extract_min(struct PriorityQueue *pq) {
    int min_val;
    if (pq->size == 0) return -999;
    min_val = pq->data[0];
    pq->size = pq->size - 1;
    pq->data[0] = pq->data[pq->size];
    pq_sift_down(pq, 0);
    return min_val;
}

int pq_peek(struct PriorityQueue *pq) {
    if (pq->size == 0) return -999;
    return pq->data[0];
}

void pq_print(struct PriorityQueue *pq) {
    int i;
    printf("pq[%d]:", pq->size);
    for (i = 0; i < pq->size; i++) {
        printf(" %d", pq->data[i]);
    }
    printf("\n");
}

/* Verify heap property: parent <= children */
int pq_is_valid(struct PriorityQueue *pq) {
    int i;
    int l;
    int r;
    for (i = 0; i < pq->size; i++) {
        l = pq_left(i);
        r = pq_right(i);
        if (l < pq->size && pq->data[i] > pq->data[l]) return 0;
        if (r < pq->size && pq->data[i] > pq->data[r]) return 0;
    }
    return 1;
}

/* Sort array using priority queue (heapsort-like) */
void pq_sort(int *arr, int n) {
    struct PriorityQueue pq;
    int i;
    pq_init(&pq);
    for (i = 0; i < n; i++) {
        pq_insert(&pq, arr[i]);
    }
    for (i = 0; i < n; i++) {
        arr[i] = pq_extract_min(&pq);
    }
}

int main() {
    struct PriorityQueue pq;
    int val;
    int arr[8];
    int i;

    pq_init(&pq);
    pq_insert(&pq, 50);
    pq_insert(&pq, 30);
    pq_insert(&pq, 40);
    pq_insert(&pq, 10);
    pq_insert(&pq, 20);

    printf("min=%d\n", pq_peek(&pq));
    // EXPECT: min=10

    printf("valid=%d\n", pq_is_valid(&pq));
    // EXPECT: valid=1

    printf("size=%d\n", pq.size);
    // EXPECT: size=5

    val = pq_extract_min(&pq);
    printf("extracted=%d\n", val);
    // EXPECT: extracted=10

    val = pq_extract_min(&pq);
    printf("extracted=%d\n", val);
    // EXPECT: extracted=20

    val = pq_extract_min(&pq);
    printf("extracted=%d\n", val);
    // EXPECT: extracted=30

    printf("remaining size=%d\n", pq.size);
    // EXPECT: remaining size=2

    val = pq_extract_min(&pq);
    printf("extracted=%d\n", val);
    // EXPECT: extracted=40

    val = pq_extract_min(&pq);
    printf("extracted=%d\n", val);
    // EXPECT: extracted=50

    printf("empty=%d\n", pq.size);
    // EXPECT: empty=0

    /* Sort test */
    arr[0] = 38; arr[1] = 27; arr[2] = 43;
    arr[3] = 3;  arr[4] = 9;  arr[5] = 82;
    arr[6] = 10; arr[7] = 1;

    pq_sort(arr, 8);
    printf("sorted:");
    for (i = 0; i < 8; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");
    // EXPECT: sorted: 1 3 9 10 27 38 43 82

    return 0;
}
