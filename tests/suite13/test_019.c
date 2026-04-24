int printf(const char *fmt, ...);

// Heap sort with heapify
// Builds a max-heap, then repeatedly extracts the maximum

int heapify_calls;

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// Sift down to maintain max-heap property
void sift_down(int *arr, int n, int i) {
    heapify_calls = heapify_calls + 1;
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }
    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        sift_down(arr, n, largest);
    }
}

void build_max_heap(int *arr, int n) {
    int i = n / 2 - 1;
    while (i >= 0) {
        sift_down(arr, n, i);
        i = i - 1;
    }
}

void heap_sort(int *arr, int n) {
    heapify_calls = 0;
    build_max_heap(arr, n);
    int i = n - 1;
    while (i > 0) {
        swap(&arr[0], &arr[i]);
        sift_down(arr, i, 0);
        i = i - 1;
    }
}

// Check if array is a valid max-heap
int is_max_heap(int *arr, int n) {
    int i;
    for (i = 0; i < n / 2; i = i + 1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && arr[i] < arr[left]) return 0;
        if (right < n && arr[i] < arr[right]) return 0;
    }
    return 1;
}

// Get heap height (floor of log2(n))
int heap_height(int n) {
    int h = 0;
    int v = 1;
    while (v <= n) {
        h = h + 1;
        v = v * 2;
    }
    return h - 1;
}

// Extract max from heap (reduce heap size)
int extract_max(int *arr, int *n) {
    int max = arr[0];
    *n = *n - 1;
    arr[0] = arr[*n];
    heapify_calls = 0;
    sift_down(arr, *n, 0);
    return max;
}

// Insert into heap (sift up)
void heap_insert(int *arr, int *n, int val) {
    arr[*n] = val;
    int i = *n;
    *n = *n + 1;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (arr[parent] < arr[i]) {
            swap(&arr[parent], &arr[i]);
            i = parent;
        } else {
            break;
        }
    }
}

int is_sorted(int *arr, int n) {
    int i;
    for (i = 0; i < n - 1; i = i + 1) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

int main() {
    // Test 1: Basic heap sort
    int a1[8];
    a1[0] = 12; a1[1] = 11; a1[2] = 13; a1[3] = 5;
    a1[4] = 6; a1[5] = 7; a1[6] = 3; a1[7] = 1;
    heap_sort(a1, 8);
    printf("sorted: %d %d %d %d %d %d %d %d\n",
           a1[0], a1[1], a1[2], a1[3], a1[4], a1[5], a1[6], a1[7]);
    // EXPECT: sorted: 1 3 5 6 7 11 12 13
    printf("heapify calls: %d\n", heapify_calls);
    // EXPECT: heapify calls: 21

    // Test 2: Build heap and verify
    int a2[7];
    a2[0] = 4; a2[1] = 10; a2[2] = 3; a2[3] = 5;
    a2[4] = 1; a2[5] = 8; a2[6] = 9;
    heapify_calls = 0;
    build_max_heap(a2, 7);
    printf("is max heap: %d\n", is_max_heap(a2, 7));
    // EXPECT: is max heap: 1
    printf("heap root: %d\n", a2[0]);
    // EXPECT: heap root: 10
    printf("build calls: %d\n", heapify_calls);
    // EXPECT: build calls: 6

    // Test 3: Heap height
    printf("height 1: %d\n", heap_height(1));
    // EXPECT: height 1: 0
    printf("height 3: %d\n", heap_height(3));
    // EXPECT: height 3: 1
    printf("height 7: %d\n", heap_height(7));
    // EXPECT: height 7: 2
    printf("height 8: %d\n", heap_height(8));
    // EXPECT: height 8: 3
    printf("height 15: %d\n", heap_height(15));
    // EXPECT: height 15: 3

    // Test 4: Extract max operations
    int a4[6];
    a4[0] = 20; a4[1] = 15; a4[2] = 10; a4[3] = 5; a4[4] = 8; a4[5] = 3;
    // This is already a valid max-heap
    printf("valid heap: %d\n", is_max_heap(a4, 6));
    // EXPECT: valid heap: 1
    int sz = 6;
    int m1 = extract_max(a4, &sz);
    int m2 = extract_max(a4, &sz);
    int m3 = extract_max(a4, &sz);
    printf("extracted: %d %d %d remaining: %d\n", m1, m2, m3, sz);
    // EXPECT: extracted: 20 15 10 remaining: 3

    // Test 5: Insert into heap
    int a5[10];
    int sz5 = 0;
    heap_insert(a5, &sz5, 5);
    heap_insert(a5, &sz5, 10);
    heap_insert(a5, &sz5, 3);
    heap_insert(a5, &sz5, 15);
    heap_insert(a5, &sz5, 8);
    printf("heap after inserts: root=%d size=%d\n", a5[0], sz5);
    // EXPECT: heap after inserts: root=15 size=5
    printf("is heap: %d\n", is_max_heap(a5, sz5));
    // EXPECT: is heap: 1

    // Test 6: Already sorted input
    int a6[5];
    a6[0] = 1; a6[1] = 2; a6[2] = 3; a6[3] = 4; a6[4] = 5;
    heap_sort(a6, 5);
    printf("presorted: %d %d %d %d %d\n", a6[0], a6[1], a6[2], a6[3], a6[4]);
    // EXPECT: presorted: 1 2 3 4 5

    // Test 7: Reverse sorted
    int a7[5];
    a7[0] = 5; a7[1] = 4; a7[2] = 3; a7[3] = 2; a7[4] = 1;
    heap_sort(a7, 5);
    printf("reverse: %d %d %d %d %d\n", a7[0], a7[1], a7[2], a7[3], a7[4]);
    // EXPECT: reverse: 1 2 3 4 5
    printf("is_sorted: %d\n", is_sorted(a7, 5));
    // EXPECT: is_sorted: 1

    printf("done\n");
    // EXPECT: done

    return 0;
}
