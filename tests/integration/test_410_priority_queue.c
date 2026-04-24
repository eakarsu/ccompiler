int printf(const char *fmt, ...);
// EXPECT: Min: 1\nSize: 7\nExtract order: 1 5 10 15 20 25 30\nHeap sorted: 3 5 11 17 28 42 55 61 77 93
// Test: Priority queue with array (min-heap)

int heap[64];
int heap_size;

void pq_init(void) {
    heap_size = 0;
}

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void sift_up(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap[parent] > heap[i]) {
            swap(&heap[parent], &heap[i]);
            i = parent;
        } else {
            break;
        }
    }
}

void sift_down(int i) {
    while (2 * i + 1 < heap_size) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        if (left < heap_size && heap[left] < heap[smallest])
            smallest = left;
        if (right < heap_size && heap[right] < heap[smallest])
            smallest = right;
        if (smallest != i) {
            swap(&heap[i], &heap[smallest]);
            i = smallest;
        } else {
            break;
        }
    }
}

void pq_insert(int val) {
    heap[heap_size] = val;
    heap_size++;
    sift_up(heap_size - 1);
}

int pq_extract_min(void) {
    int min = heap[0];
    heap_size--;
    heap[0] = heap[heap_size];
    sift_down(0);
    return min;
}

int pq_peek(void) {
    return heap[0];
}

int main(void) {
    pq_init();

    pq_insert(15);
    pq_insert(10);
    pq_insert(20);
    pq_insert(5);
    pq_insert(25);
    pq_insert(1);
    pq_insert(30);

    printf("Min: %d\n", pq_peek());
    printf("Size: %d\n", heap_size);

    printf("Extract order:");
    while (heap_size > 0) {
        printf(" %d", pq_extract_min());
    }
    printf("\n");

    int data[] = {42, 17, 93, 5, 28, 61, 3, 77, 11, 55};
    int n = 10;
    int i;
    pq_init();
    for (i = 0; i < n; i++) {
        pq_insert(data[i]);
    }
    printf("Heap sorted:");
    for (i = 0; i < n; i++) {
        printf(" %d", pq_extract_min());
    }
    printf("\n");

    return 0;
}
