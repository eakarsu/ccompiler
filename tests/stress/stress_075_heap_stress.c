// stress_075_heap_stress.c - Binary heap stress test
// EXPECT: insert_30_size=30
// EXPECT: heap_valid_after_insert=1
// EXPECT: min_after_insert=1
// EXPECT: extract_sorted=1
// EXPECT: first_extracted=1
// EXPECT: last_extracted=30
// EXPECT: empty_after_extract=0
// EXPECT: interleave_e1=30
// EXPECT: interleave_e2=20
// EXPECT: interleave_e3=10
// EXPECT: interleave_e4=50
// EXPECT: interleave_valid=1
// EXPECT: heapsort_ok=1
// EXPECT: heapsort_sum=210
// EXPECT: heapify_valid=1
// EXPECT: heapify_min=1
// EXPECT: heapify_size=15
// EXPECT: dup_valid=1
// EXPECT: dup_first=0
// EXPECT: dup_second=0

int printf(const char *fmt, ...);

typedef struct {
    int data[64];
    int size;
} MinHeap;

void heap_init(MinHeap *h) {
    h->size = 0;
}

void heap_swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void heap_sift_up(MinHeap *h, int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (h->data[i] < h->data[parent]) {
            heap_swap(&h->data[i], &h->data[parent]);
            i = parent;
        } else {
            break;
        }
    }
}

void heap_sift_down(MinHeap *h, int i) {
    while (1) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < h->size && h->data[left] < h->data[smallest])
            smallest = left;
        if (right < h->size && h->data[right] < h->data[smallest])
            smallest = right;
        if (smallest != i) {
            heap_swap(&h->data[i], &h->data[smallest]);
            i = smallest;
        } else {
            break;
        }
    }
}

void heap_insert(MinHeap *h, int val) {
    h->data[h->size] = val;
    h->size++;
    heap_sift_up(h, h->size - 1);
}

int heap_extract_min(MinHeap *h) {
    int min = h->data[0];
    h->size--;
    h->data[0] = h->data[h->size];
    heap_sift_down(h, 0);
    return min;
}

int heap_peek(MinHeap *h) {
    return h->data[0];
}

int verify_heap_property(MinHeap *h) {
    int i;
    for (i = 0; i < h->size; i++) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < h->size && h->data[left] < h->data[i]) return 0;
        if (right < h->size && h->data[right] < h->data[i]) return 0;
    }
    return 1;
}

// Build heap from array using heapify (bottom-up)
void heapify(MinHeap *h, int *arr, int n) {
    int i;
    h->size = n;
    for (i = 0; i < n; i++) h->data[i] = arr[i];
    for (i = n / 2 - 1; i >= 0; i--) {
        heap_sift_down(h, i);
    }
}

int main(void) {
    MinHeap h;
    heap_init(&h);

    // Insert 30 elements in scrambled order
    int vals[30] = {25, 17, 3, 28, 11, 6, 30, 1, 19, 14,
                    22, 8, 27, 5, 16, 10, 23, 2, 29, 12,
                    20, 7, 26, 4, 18, 9, 24, 13, 21, 15};
    int i;
    for (i = 0; i < 30; i++) {
        heap_insert(&h, vals[i]);
    }
    printf("insert_30_size=%d\n", h.size);
    printf("heap_valid_after_insert=%d\n", verify_heap_property(&h));
    printf("min_after_insert=%d\n", heap_peek(&h));

    // Extract all 30 in sorted order
    int sorted[30];
    int sorted_ok = 1;
    for (i = 0; i < 30; i++) {
        sorted[i] = heap_extract_min(&h);
        if (i > 0 && sorted[i] < sorted[i - 1]) sorted_ok = 0;
    }
    printf("extract_sorted=%d\n", sorted_ok);
    printf("first_extracted=%d\n", sorted[0]);
    printf("last_extracted=%d\n", sorted[29]);
    printf("empty_after_extract=%d\n", h.size);

    // Interleaved insert and extract
    heap_init(&h);
    heap_insert(&h, 50);
    heap_insert(&h, 30);
    heap_insert(&h, 70);
    int e1 = heap_extract_min(&h); // 30
    heap_insert(&h, 20);
    heap_insert(&h, 60);
    int e2 = heap_extract_min(&h); // 20
    heap_insert(&h, 10);
    int e3 = heap_extract_min(&h); // 10
    int e4 = heap_extract_min(&h); // 50
    printf("interleave_e1=%d\n", e1);
    printf("interleave_e2=%d\n", e2);
    printf("interleave_e3=%d\n", e3);
    printf("interleave_e4=%d\n", e4);
    printf("interleave_valid=%d\n", verify_heap_property(&h));

    // Heap sort: insert all, extract all
    MinHeap h2;
    heap_init(&h2);
    int unsorted[20] = {19, 3, 17, 1, 8, 14, 20, 6, 11, 15,
                        2, 9, 18, 5, 12, 7, 16, 4, 13, 10};
    for (i = 0; i < 20; i++) heap_insert(&h2, unsorted[i]);
    int heap_sorted[20];
    for (i = 0; i < 20; i++) heap_sorted[i] = heap_extract_min(&h2);
    int hs_ok = 1;
    for (i = 0; i < 19; i++) {
        if (heap_sorted[i] > heap_sorted[i + 1]) hs_ok = 0;
    }
    printf("heapsort_ok=%d\n", hs_ok);
    int hs_sum = 0;
    for (i = 0; i < 20; i++) hs_sum += heap_sorted[i];
    printf("heapsort_sum=%d\n", hs_sum);

    // Build heap from array (heapify)
    MinHeap h3;
    int raw[15] = {15, 10, 14, 9, 8, 13, 12, 7, 6, 5, 4, 3, 11, 2, 1};
    heapify(&h3, raw, 15);
    printf("heapify_valid=%d\n", verify_heap_property(&h3));
    printf("heapify_min=%d\n", heap_peek(&h3));
    printf("heapify_size=%d\n", h3.size);

    // Stress: insert duplicates
    MinHeap h4;
    heap_init(&h4);
    for (i = 0; i < 20; i++) heap_insert(&h4, i % 5); // lots of dups: 0,1,2,3,4,0,1,2,3,4,...
    printf("dup_valid=%d\n", verify_heap_property(&h4));
    int dup_first = heap_extract_min(&h4);
    int dup_second = heap_extract_min(&h4);
    printf("dup_first=%d\n", dup_first);
    printf("dup_second=%d\n", dup_second);

    return 0;
}
