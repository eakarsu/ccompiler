int printf(const char *fmt, ...);

// Binary min-heap operations using array
// Parent of i = i/2, Left child = 2*i, Right child = 2*i+1
// Index 0 unused; heap starts at index 1

struct MinHeap {
    int data[64];
    int size;
};

void heap_init(struct MinHeap *h) {
    int i;
    for (i = 0; i < 64; i++) {
        h->data[i] = 0;
    }
    h->size = 0;
}

void heap_swap(struct MinHeap *h, int a, int b) {
    int tmp = h->data[a];
    h->data[a] = h->data[b];
    h->data[b] = tmp;
}

void heap_sift_up(struct MinHeap *h, int idx) {
    int parent;
    while (idx > 1) {
        parent = idx / 2;
        if (h->data[idx] < h->data[parent]) {
            heap_swap(h, idx, parent);
            idx = parent;
        } else {
            return;
        }
    }
}

void heap_sift_down(struct MinHeap *h, int idx) {
    int left;
    int right;
    int smallest;
    while (1) {
        left = 2 * idx;
        right = 2 * idx + 1;
        smallest = idx;
        if (left <= h->size && h->data[left] < h->data[smallest]) {
            smallest = left;
        }
        if (right <= h->size && h->data[right] < h->data[smallest]) {
            smallest = right;
        }
        if (smallest != idx) {
            heap_swap(h, idx, smallest);
            idx = smallest;
        } else {
            return;
        }
    }
}

void heap_insert(struct MinHeap *h, int val) {
    h->size = h->size + 1;
    h->data[h->size] = val;
    heap_sift_up(h, h->size);
}

int heap_extract_min(struct MinHeap *h) {
    int min_val;
    if (h->size == 0) return -1;
    min_val = h->data[1];
    h->data[1] = h->data[h->size];
    h->size = h->size - 1;
    if (h->size > 0) {
        heap_sift_down(h, 1);
    }
    return min_val;
}

int heap_peek(struct MinHeap *h) {
    if (h->size == 0) return -1;
    return h->data[1];
}

// Build heap from array (heapify)
void heap_build(struct MinHeap *h, int *arr, int n) {
    int i;
    h->size = n;
    for (i = 1; i <= n; i++) {
        h->data[i] = arr[i - 1];
    }
    // Sift down from last non-leaf to root
    for (i = n / 2; i >= 1; i--) {
        heap_sift_down(h, i);
    }
}

// Heap sort: extract all elements into output array
void heap_sort(int *arr, int n) {
    struct MinHeap h;
    int i;
    heap_init(&h);
    heap_build(&h, arr, n);
    for (i = 0; i < n; i++) {
        arr[i] = heap_extract_min(&h);
    }
}

void print_labeled_arr(char *label, int *arr, int n) {
    int i;
    printf("%s", label);
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

int main() {
    struct MinHeap h;
    int val;
    int i;

    heap_init(&h);

    // Insert elements
    heap_insert(&h, 42);
    heap_insert(&h, 15);
    heap_insert(&h, 88);
    heap_insert(&h, 7);
    heap_insert(&h, 23);
    heap_insert(&h, 3);
    heap_insert(&h, 56);

    printf("Size: %d\n", h.size);
    // EXPECT: Size: 7
    printf("Min: %d\n", heap_peek(&h));
    // EXPECT: Min: 3

    // Extract elements in sorted order
    val = heap_extract_min(&h);
    printf("Extract: %d\n", val);
    // EXPECT: Extract: 3
    val = heap_extract_min(&h);
    printf("Extract: %d\n", val);
    // EXPECT: Extract: 7
    val = heap_extract_min(&h);
    printf("Extract: %d\n", val);
    // EXPECT: Extract: 15

    printf("Size after 3 extracts: %d\n", h.size);
    // EXPECT: Size after 3 extracts: 4
    printf("New min: %d\n", heap_peek(&h));
    // EXPECT: New min: 23

    // Extract remaining
    val = heap_extract_min(&h);
    printf("Extract: %d\n", val);
    // EXPECT: Extract: 23
    val = heap_extract_min(&h);
    printf("Extract: %d\n", val);
    // EXPECT: Extract: 42
    val = heap_extract_min(&h);
    printf("Extract: %d\n", val);
    // EXPECT: Extract: 56
    val = heap_extract_min(&h);
    printf("Extract: %d\n", val);
    // EXPECT: Extract: 88

    printf("Empty size: %d\n", h.size);
    // EXPECT: Empty size: 0

    // Heap sort test
    int arr[8];
    arr[0] = 64;
    arr[1] = 25;
    arr[2] = 12;
    arr[3] = 99;
    arr[4] = 1;
    arr[5] = 47;
    arr[6] = 38;
    arr[7] = 73;

    heap_sort(arr, 8);
    print_labeled_arr("Sorted: ", arr, 8);
    // EXPECT: Sorted: 1 12 25 38 47 64 73 99

    // Build heap directly and extract
    int arr2[5];
    arr2[0] = 50;
    arr2[1] = 10;
    arr2[2] = 30;
    arr2[3] = 5;
    arr2[4] = 20;

    struct MinHeap h2;
    heap_init(&h2);
    heap_build(&h2, arr2, 5);

    printf("Built heap min: %d\n", heap_peek(&h2));
    // EXPECT: Built heap min: 5
    printf("Built heap size: %d\n", h2.size);
    // EXPECT: Built heap size: 5

    // Extract all from built heap
    int extracted[5];
    for (i = 0; i < 5; i++) {
        extracted[i] = heap_extract_min(&h2);
    }
    print_labeled_arr("Built extract: ", extracted, 5);
    // EXPECT: Built extract: 5 10 20 30 50

    // Test with already sorted input
    int arr3[4];
    arr3[0] = 1;
    arr3[1] = 2;
    arr3[2] = 3;
    arr3[3] = 4;
    heap_sort(arr3, 4);
    print_labeled_arr("Already sorted: ", arr3, 4);
    // EXPECT: Already sorted: 1 2 3 4

    // Test with reverse sorted input
    int arr4[4];
    arr4[0] = 4;
    arr4[1] = 3;
    arr4[2] = 2;
    arr4[3] = 1;
    heap_sort(arr4, 4);
    print_labeled_arr("Reverse sorted: ", arr4, 4);
    // EXPECT: Reverse sorted: 1 2 3 4

    return 0;
}
