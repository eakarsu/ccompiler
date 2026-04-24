int printf(const char *fmt, ...);

/* Min-heap priority queue using array, plus heap sort */

#define HEAP_CAP 32

typedef struct {
    int data[HEAP_CAP];
    int size;
} MinHeap;

void heap_init(MinHeap *h) {
    h->size = 0;
}

/* Swap two elements */
static void swap(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}

/* Sift up: fix heap property from index i upward */
static void sift_up(MinHeap *h, int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (h->data[parent] > h->data[i]) {
            swap(&h->data[parent], &h->data[i]);
            i = parent;
        } else {
            break;
        }
    }
}

/* Sift down: fix heap property from index i downward */
static void sift_down(MinHeap *h, int i, int n) {
    while (1) {
        int left  = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        if (left  < n && h->data[left]  < h->data[smallest]) smallest = left;
        if (right < n && h->data[right] < h->data[smallest]) smallest = right;
        if (smallest == i) break;
        swap(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
}

/* Insert into heap */
int heap_insert(MinHeap *h, int val) {
    if (h->size >= HEAP_CAP) return 0;
    h->data[h->size] = val;
    sift_up(h, h->size);
    h->size++;
    return 1;
}

/* Peek min */
int heap_peek_min(MinHeap *h, int *out) {
    if (h->size == 0) return 0;
    *out = h->data[0];
    return 1;
}

/* Extract min */
int heap_extract_min(MinHeap *h, int *out) {
    if (h->size == 0) return 0;
    *out = h->data[0];
    h->size--;
    if (h->size > 0) {
        h->data[0] = h->data[h->size];
        sift_down(h, 0, h->size);
    }
    return 1;
}

/* Check heap property: for all i, data[i] <= data[children] */
int heap_is_valid(MinHeap *h) {
    int i;
    for (i = 0; i < h->size; i++) {
        int left  = 2 * i + 1;
        int right = 2 * i + 2;
        if (left  < h->size && h->data[i] > h->data[left])  return 0;
        if (right < h->size && h->data[i] > h->data[right]) return 0;
    }
    return 1;
}

/* Build heap from array (heapify) */
void heap_build(MinHeap *h, int *arr, int n) {
    int i;
    h->size = n;
    for (i = 0; i < n; i++) h->data[i] = arr[i];
    /* sift down from last internal node upward */
    for (i = n / 2 - 1; i >= 0; i--) {
        sift_down(h, i, n);
    }
}

/* Heap sort (ascending) using a temporary max-heap approach:
   We sort in-place using the same sift_down but treat the array as
   a max-heap. We rebuild as max-heap first. */
static void sift_down_max(int *arr, int i, int n) {
    while (1) {
        int left  = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;
        if (left  < n && arr[left]  > arr[largest]) largest = left;
        if (right < n && arr[right] > arr[largest]) largest = right;
        if (largest == i) break;
        int t = arr[i]; arr[i] = arr[largest]; arr[largest] = t;
        i = largest;
    }
}

void heap_sort(int *arr, int n) {
    int i;
    /* Build max-heap */
    for (i = n / 2 - 1; i >= 0; i--) {
        sift_down_max(arr, i, n);
    }
    /* Extract elements */
    for (i = n - 1; i > 0; i--) {
        int t = arr[0]; arr[0] = arr[i]; arr[i] = t;
        sift_down_max(arr, 0, i);
    }
}

/* Print array */
void print_arr(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

int main(void) {
    MinHeap h;
    heap_init(&h);

    /* Insert values */
    heap_insert(&h, 15);
    heap_insert(&h, 3);
    heap_insert(&h, 9);
    heap_insert(&h, 1);
    heap_insert(&h, 7);

    printf("%d\n", heap_is_valid(&h));  /* 1 */
    // EXPECT: 1
    printf("%d\n", h.size);             /* 5 */
    // EXPECT: 5

    int v;
    heap_peek_min(&h, &v);
    printf("%d\n", v);                  /* 1 */
    // EXPECT: 1

    /* Extract sorted order */
    heap_extract_min(&h, &v);
    printf("%d\n", v);                  /* 1 */
    // EXPECT: 1
    printf("%d\n", heap_is_valid(&h));  /* 1 */
    // EXPECT: 1

    heap_extract_min(&h, &v);
    printf("%d\n", v);                  /* 3 */
    // EXPECT: 3

    heap_extract_min(&h, &v);
    printf("%d\n", v);                  /* 7 */
    // EXPECT: 7

    heap_extract_min(&h, &v);
    printf("%d\n", v);                  /* 9 */
    // EXPECT: 9

    heap_extract_min(&h, &v);
    printf("%d\n", v);                  /* 15 */
    // EXPECT: 15

    printf("%d\n", h.size);             /* 0 */
    // EXPECT: 0
    printf("%d\n", heap_extract_min(&h, &v)); /* 0 (empty) */
    // EXPECT: 0

    /* Larger insertion test */
    int vals[10] = {50, 30, 70, 10, 40, 60, 80, 20, 90, 5};
    int i;
    for (i = 0; i < 10; i++) heap_insert(&h, vals[i]);

    printf("%d\n", heap_is_valid(&h));  /* 1 */
    // EXPECT: 1

    heap_peek_min(&h, &v);
    printf("%d\n", v);                  /* 5 */
    // EXPECT: 5

    /* Extract all: should come out sorted */
    int prev = -1, sorted_ok = 1;
    int extracted[10];
    for (i = 0; i < 10; i++) {
        heap_extract_min(&h, &extracted[i]);
        if (extracted[i] < prev) sorted_ok = 0;
        prev = extracted[i];
    }
    print_arr(extracted, 10);
    // EXPECT: 5 10 20 30 40 50 60 70 80 90
    printf("%d\n", sorted_ok);          /* 1 */
    // EXPECT: 1

    /* Heapify (build from array) */
    int arr2[7] = {4, 8, 1, 6, 3, 9, 2};
    heap_build(&h, arr2, 7);
    printf("%d\n", heap_is_valid(&h));  /* 1 */
    // EXPECT: 1
    heap_peek_min(&h, &v);
    printf("%d\n", v);                  /* 1 */
    // EXPECT: 1

    /* Heap sort */
    int arr3[8] = {5, 2, 8, 1, 9, 3, 7, 4};
    heap_sort(arr3, 8);
    print_arr(arr3, 8);
    // EXPECT: 1 2 3 4 5 7 8 9

    int arr4[5] = {3, 1, 4, 1, 5};
    heap_sort(arr4, 5);
    print_arr(arr4, 5);
    // EXPECT: 1 1 3 4 5

    int arr5[1] = {42};
    heap_sort(arr5, 1);
    printf("%d\n", arr5[0]);            /* 42 */
    // EXPECT: 42

    return 0;
}
