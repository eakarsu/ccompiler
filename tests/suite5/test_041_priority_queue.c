int printf(const char *fmt, ...);

int heap[64];
int heap_size;

void swap(int *a, int *b) {
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void heap_init(void) {
    heap_size = 0;
}

void sift_up(int i) {
    int parent;
    while (i > 0) {
        parent = (i - 1) / 2;
        if (heap[parent] > heap[i]) {
            swap(&heap[parent], &heap[i]);
            i = parent;
        } else {
            break;
        }
    }
}

void sift_down(int i) {
    int left;
    int right;
    int smallest;
    while (1) {
        left = 2 * i + 1;
        right = 2 * i + 2;
        smallest = i;
        if (left < heap_size && heap[left] < heap[smallest]) {
            smallest = left;
        }
        if (right < heap_size && heap[right] < heap[smallest]) {
            smallest = right;
        }
        if (smallest != i) {
            swap(&heap[i], &heap[smallest]);
            i = smallest;
        } else {
            break;
        }
    }
}

void heap_insert(int val) {
    heap[heap_size] = val;
    sift_up(heap_size);
    heap_size = heap_size + 1;
}

int heap_extract_min(void) {
    int min_val;
    min_val = heap[0];
    heap_size = heap_size - 1;
    heap[0] = heap[heap_size];
    sift_down(0);
    return min_val;
}

int heap_peek(void) {
    return heap[0];
}

void heapify(int arr[], int n) {
    int i;
    heap_size = n;
    i = 0;
    while (i < n) {
        heap[i] = arr[i];
        i = i + 1;
    }
    i = (n / 2) - 1;
    while (i >= 0) {
        sift_down(i);
        i = i - 1;
    }
}

int main(void) {
    int arr[8];
    int val;
    int i;
    int sorted[10];

    heap_init();

    heap_insert(5);
    heap_insert(3);
    heap_insert(8);
    heap_insert(1);
    heap_insert(2);

    // EXPECT: size: 5
    printf("size: %d\n", heap_size);

    // EXPECT: min: 1
    printf("min: %d\n", heap_peek());

    val = heap_extract_min();
    // EXPECT: extracted: 1
    printf("extracted: %d\n", val);

    val = heap_extract_min();
    // EXPECT: extracted: 2
    printf("extracted: %d\n", val);

    // EXPECT: size after: 3
    printf("size after: %d\n", heap_size);

    val = heap_extract_min();
    // EXPECT: extracted: 3
    printf("extracted: %d\n", val);

    heap_insert(0);
    heap_insert(4);
    // EXPECT: new min: 0
    printf("new min: %d\n", heap_peek());

    val = heap_extract_min();
    // EXPECT: got: 0
    printf("got: %d\n", val);
    val = heap_extract_min();
    // EXPECT: got: 4
    printf("got: %d\n", val);
    val = heap_extract_min();
    // EXPECT: got: 5
    printf("got: %d\n", val);
    val = heap_extract_min();
    // EXPECT: got: 8
    printf("got: %d\n", val);

    // EXPECT: empty: 0
    printf("empty: %d\n", heap_size);

    arr[0] = 9;
    arr[1] = 4;
    arr[2] = 7;
    arr[3] = 1;
    arr[4] = 3;
    arr[5] = 6;
    arr[6] = 2;
    arr[7] = 8;
    heapify(arr, 8);

    // EXPECT: heapified min: 1
    printf("heapified min: %d\n", heap_peek());

    i = 0;
    while (i < 8) {
        sorted[i] = heap_extract_min();
        i = i + 1;
    }
    // EXPECT: sorted: 1 2 3 4 6 7 8 9
    printf("sorted: %d %d %d %d %d %d %d %d\n",
        sorted[0], sorted[1], sorted[2], sorted[3],
        sorted[4], sorted[5], sorted[6], sorted[7]);

    heap_init();
    heap_insert(10);
    // EXPECT: single: 10
    printf("single: %d\n", heap_peek());

    heap_insert(5);
    // EXPECT: after add 5: 5
    printf("after add 5: %d\n", heap_peek());

    heap_insert(15);
    // EXPECT: after add 15: 5
    printf("after add 15: %d\n", heap_peek());

    return 0;
}
