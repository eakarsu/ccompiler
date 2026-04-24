int printf(const char *fmt, ...);
// EXPECT: Size: 7\nMin: 3\nSorted extraction: 3 5 8 10 15 20 25\nEmpty size: 0\nDuplicates: 3 3 7 7\nSingle: 42\nDone

// Array-based min-heap priority queue

struct PriorityQueue {
    int data[64];
    int size;
};

void pq_init(struct PriorityQueue *pq) {
    pq->size = 0;
}

void pq_swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void pq_push(struct PriorityQueue *pq, int val) {
    int i = pq->size;
    pq->data[i] = val;
    pq->size = pq->size + 1;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pq->data[i] < pq->data[parent]) {
            pq_swap(&pq->data[i], &pq->data[parent]);
            i = parent;
        } else {
            break;
        }
    }
}

int pq_pop(struct PriorityQueue *pq) {
    int min_val = pq->data[0];
    pq->size = pq->size - 1;
    pq->data[0] = pq->data[pq->size];
    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        if (left < pq->size && pq->data[left] < pq->data[smallest]) {
            smallest = left;
        }
        if (right < pq->size && pq->data[right] < pq->data[smallest]) {
            smallest = right;
        }
        if (smallest != i) {
            pq_swap(&pq->data[i], &pq->data[smallest]);
            i = smallest;
        } else {
            break;
        }
    }
    return min_val;
}

int pq_peek(struct PriorityQueue *pq) {
    return pq->data[0];
}

int main(void) {
    struct PriorityQueue pq;
    pq_init(&pq);

    pq_push(&pq, 15);
    pq_push(&pq, 10);
    pq_push(&pq, 20);
    pq_push(&pq, 5);
    pq_push(&pq, 8);
    pq_push(&pq, 25);
    pq_push(&pq, 3);

    printf("Size: %d\n", pq.size);
    printf("Min: %d\n", pq_peek(&pq));

    printf("Sorted extraction:");
    int i = 0;
    while (pq.size > 0) {
        printf(" %d", pq_pop(&pq));
        i = i + 1;
    }
    printf("\n");

    printf("Empty size: %d\n", pq.size);

    // Test with duplicates
    pq_push(&pq, 7);
    pq_push(&pq, 7);
    pq_push(&pq, 3);
    pq_push(&pq, 3);
    printf("Duplicates:");
    while (pq.size > 0) {
        printf(" %d", pq_pop(&pq));
    }
    printf("\n");

    // Test single element
    pq_push(&pq, 42);
    printf("Single: %d\n", pq_pop(&pq));

    printf("Done\n");
    return 0;
}
