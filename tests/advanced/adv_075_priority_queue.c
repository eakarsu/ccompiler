// EXPECT: === Priority Queue ===
// EXPECT: Insert: 5 3 8 1 4 7 2 6
// EXPECT: Size: 8
// EXPECT: Peek min: 1
// EXPECT: Extract all: 1 2 3 4 5 6 7 8
// EXPECT: Empty: yes
// EXPECT: Insert desc: 10 9 8 7 6 5 4 3 2 1
// EXPECT: Extract: 1 2 3 4 5 6 7 8 9 10
// EXPECT: Heap sort: 3 5 7 11 13 17 19 23
// EXPECT: Heap sort desc: 23 19 17 13 11 7 5 3
// EXPECT: Single element: 42
// EXPECT: Two elements: 1 2
// EXPECT: Duplicates: 3 3 3 5 5 7
// EXPECT: All priority queue tests passed

int printf(const char *fmt, ...);

typedef struct {
    int data[32];
    int size;
} MinHeap;

void heap_init(MinHeap *h) {
    h->size = 0;
}

void heap_swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void heap_bubble_up(MinHeap *h, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (h->data[parent] > h->data[idx]) {
            heap_swap(&h->data[parent], &h->data[idx]);
            idx = parent;
        } else {
            break;
        }
    }
}

void heap_bubble_down(MinHeap *h, int idx) {
    while (1) {
        int smallest = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;

        if (left < h->size && h->data[left] < h->data[smallest]) {
            smallest = left;
        }
        if (right < h->size && h->data[right] < h->data[smallest]) {
            smallest = right;
        }
        if (smallest != idx) {
            heap_swap(&h->data[idx], &h->data[smallest]);
            idx = smallest;
        } else {
            break;
        }
    }
}

int heap_insert(MinHeap *h, int val) {
    if (h->size >= 32) return -1;
    h->data[h->size] = val;
    heap_bubble_up(h, h->size);
    h->size++;
    return 0;
}

int heap_peek(MinHeap *h, int *val) {
    if (h->size == 0) return -1;
    *val = h->data[0];
    return 0;
}

int heap_extract_min(MinHeap *h, int *val) {
    if (h->size == 0) return -1;
    *val = h->data[0];
    h->size--;
    h->data[0] = h->data[h->size];
    heap_bubble_down(h, 0);
    return 0;
}

int heap_empty(MinHeap *h) {
    return h->size == 0;
}

void test_basic(void) {
    MinHeap h;
    heap_init(&h);

    int vals[8];
    vals[0] = 5; vals[1] = 3; vals[2] = 8; vals[3] = 1;
    vals[4] = 4; vals[5] = 7; vals[6] = 2; vals[7] = 6;

    printf("Insert:");
    int i;
    for (i = 0; i < 8; i++) {
        heap_insert(&h, vals[i]);
        printf(" %d", vals[i]);
    }
    printf("\n");

    printf("Size: %d\n", h.size);

    int v;
    heap_peek(&h, &v);
    printf("Peek min: %d\n", v);

    printf("Extract all:");
    while (!heap_empty(&h)) {
        heap_extract_min(&h, &v);
        printf(" %d", v);
    }
    printf("\n");
    printf("Empty: %s\n", heap_empty(&h) ? "yes" : "no");
}

void test_descending(void) {
    MinHeap h;
    heap_init(&h);

    printf("Insert desc:");
    int i;
    for (i = 10; i >= 1; i--) {
        heap_insert(&h, i);
        printf(" %d", i);
    }
    printf("\n");

    int v;
    printf("Extract:");
    while (!heap_empty(&h)) {
        heap_extract_min(&h, &v);
        printf(" %d", v);
    }
    printf("\n");
}

void heap_sort_asc(int *arr, int n) {
    MinHeap h;
    heap_init(&h);
    int i;
    for (i = 0; i < n; i++) {
        heap_insert(&h, arr[i]);
    }
    int v;
    for (i = 0; i < n; i++) {
        heap_extract_min(&h, &v);
        arr[i] = v;
    }
}

typedef struct {
    int data[32];
    int size;
} MaxHeap;

void maxheap_init(MaxHeap *h) { h->size = 0; }

void maxheap_bubble_up(MaxHeap *h, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (h->data[parent] < h->data[idx]) {
            heap_swap(&h->data[parent], &h->data[idx]);
            idx = parent;
        } else {
            break;
        }
    }
}

void maxheap_bubble_down(MaxHeap *h, int idx) {
    while (1) {
        int largest = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        if (left < h->size && h->data[left] > h->data[largest]) largest = left;
        if (right < h->size && h->data[right] > h->data[largest]) largest = right;
        if (largest != idx) {
            heap_swap(&h->data[idx], &h->data[largest]);
            idx = largest;
        } else {
            break;
        }
    }
}

void maxheap_insert(MaxHeap *h, int val) {
    h->data[h->size] = val;
    maxheap_bubble_up(h, h->size);
    h->size++;
}

int maxheap_extract(MaxHeap *h) {
    int val = h->data[0];
    h->size--;
    h->data[0] = h->data[h->size];
    maxheap_bubble_down(h, 0);
    return val;
}

void heap_sort_desc(int *arr, int n) {
    MaxHeap h;
    maxheap_init(&h);
    int i;
    for (i = 0; i < n; i++) maxheap_insert(&h, arr[i]);
    for (i = 0; i < n; i++) arr[i] = maxheap_extract(&h);
}

void test_heap_sort(void) {
    int arr[8];
    arr[0] = 13; arr[1] = 7; arr[2] = 23; arr[3] = 3;
    arr[4] = 19; arr[5] = 5; arr[6] = 11; arr[7] = 17;

    heap_sort_asc(arr, 8);
    printf("Heap sort:");
    int i;
    for (i = 0; i < 8; i++) printf(" %d", arr[i]);
    printf("\n");

    heap_sort_desc(arr, 8);
    printf("Heap sort desc:");
    for (i = 0; i < 8; i++) printf(" %d", arr[i]);
    printf("\n");
}

void test_edge_cases(void) {
    MinHeap h;
    heap_init(&h);
    int v;

    heap_insert(&h, 42);
    heap_extract_min(&h, &v);
    printf("Single element: %d\n", v);

    heap_insert(&h, 2);
    heap_insert(&h, 1);
    printf("Two elements:");
    heap_extract_min(&h, &v); printf(" %d", v);
    heap_extract_min(&h, &v); printf(" %d", v);
    printf("\n");

    heap_insert(&h, 5);
    heap_insert(&h, 3);
    heap_insert(&h, 5);
    heap_insert(&h, 3);
    heap_insert(&h, 7);
    heap_insert(&h, 3);
    printf("Duplicates:");
    while (!heap_empty(&h)) {
        heap_extract_min(&h, &v);
        printf(" %d", v);
    }
    printf("\n");
}

int main(void) {
    printf("=== Priority Queue ===\n");
    test_basic();
    test_descending();
    test_heap_sort();
    test_edge_cases();
    printf("All priority queue tests passed\n");
    return 0;
}
