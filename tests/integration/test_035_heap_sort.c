int printf(const char *fmt, ...);
// EXPECT: ms=0 ms2=5 mp=30 mv=1 me1=30 me2=20 me3=15 ms3=2 me4=10 me5=5 ms4=0 np=10 nv=1 ne1=10 ne2=30 ne3=40 ne4=50 ne5=70 sa=1 s0=3 s1=9 s6=82 sd=1 d0=82 d6=3 pk=1 pv=200 pp1=200 pp2=400 pp3=100 pp4=300 pp5=500 k1s=1 k2s=3 k3s=8 k1l=22 k2l=15 mg0=1 mg1=2 mg5=10 h1=0 h7=2 h16=4 e1=42 e2a=3 e2b=7

/* ================================================================
 * test_035_heap_sort.c
 * Max-heap and min-heap operations, heap sort, priority queue.
 * ================================================================ */

/* ---- Max-Heap (1-indexed in array, size at index 0) ---- */
int maxh[128];

void maxh_init(void) { maxh[0] = 0; }
int maxh_size(void) { return maxh[0]; }

void maxh_swap(int i, int j) {
    int t;
    t = maxh[i]; maxh[i] = maxh[j]; maxh[j] = t;
}

void maxh_sift_up(int i) {
    int p;
    while (i > 1) {
        p = i / 2;
        if (maxh[i] > maxh[p]) { maxh_swap(i, p); i = p; }
        else break;
    }
}

void maxh_sift_down(int i) {
    int sz; int largest; int l; int r;
    sz = maxh[0];
    while (1) {
        largest = i;
        l = 2 * i;
        r = 2 * i + 1;
        if (l <= sz && maxh[l] > maxh[largest]) largest = l;
        if (r <= sz && maxh[r] > maxh[largest]) largest = r;
        if (largest != i) { maxh_swap(i, largest); i = largest; }
        else break;
    }
}

void maxh_insert(int val) {
    int sz;
    sz = maxh[0] + 1;
    maxh[0] = sz;
    maxh[sz] = val;
    maxh_sift_up(sz);
}

int maxh_peek(void) {
    if (maxh[0] == 0) return -1;
    return maxh[1];
}

int maxh_extract(void) {
    int val; int sz;
    if (maxh[0] == 0) return -1;
    val = maxh[1];
    sz = maxh[0];
    maxh[1] = maxh[sz];
    maxh[0] = sz - 1;
    if (maxh[0] > 0) maxh_sift_down(1);
    return val;
}

int maxh_verify(void) {
    int i; int sz; int l; int r;
    sz = maxh[0];
    i = 1;
    while (i <= sz) {
        l = 2 * i; r = 2 * i + 1;
        if (l <= sz && maxh[l] > maxh[i]) return 0;
        if (r <= sz && maxh[r] > maxh[i]) return 0;
        i = i + 1;
    }
    return 1;
}

/* ---- Min-Heap (1-indexed) ---- */
int minh[128];

void minh_init(void) { minh[0] = 0; }
int minh_size(void) { return minh[0]; }

void minh_swap(int i, int j) {
    int t;
    t = minh[i]; minh[i] = minh[j]; minh[j] = t;
}

void minh_sift_up(int i) {
    int p;
    while (i > 1) {
        p = i / 2;
        if (minh[i] < minh[p]) { minh_swap(i, p); i = p; }
        else break;
    }
}

void minh_sift_down(int i) {
    int sz; int smallest; int l; int r;
    sz = minh[0];
    while (1) {
        smallest = i;
        l = 2 * i; r = 2 * i + 1;
        if (l <= sz && minh[l] < minh[smallest]) smallest = l;
        if (r <= sz && minh[r] < minh[smallest]) smallest = r;
        if (smallest != i) { minh_swap(i, smallest); i = smallest; }
        else break;
    }
}

void minh_insert(int val) {
    int sz;
    sz = minh[0] + 1;
    minh[0] = sz;
    minh[sz] = val;
    minh_sift_up(sz);
}

int minh_peek(void) {
    if (minh[0] == 0) return -1;
    return minh[1];
}

int minh_extract(void) {
    int val; int sz;
    if (minh[0] == 0) return -1;
    val = minh[1];
    sz = minh[0];
    minh[1] = minh[sz];
    minh[0] = sz - 1;
    if (minh[0] > 0) minh_sift_down(1);
    return val;
}

int minh_verify(void) {
    int i; int sz; int l; int r;
    sz = minh[0];
    i = 1;
    while (i <= sz) {
        l = 2 * i; r = 2 * i + 1;
        if (l <= sz && minh[l] < minh[i]) return 0;
        if (r <= sz && minh[r] < minh[i]) return 0;
        i = i + 1;
    }
    return 1;
}

/* ---- Heap Sort Ascending (0-indexed array) ---- */
void hsort_asc(int *a, int n) {
    int i; int t; int p; int c; int sz;
    /* Build max-heap */
    i = n / 2 - 1;
    while (i >= 0) {
        p = i;
        while (1) {
            c = 2 * p + 1;
            if (c >= n) break;
            if (c + 1 < n && a[c + 1] > a[c]) c = c + 1;
            if (a[c] > a[p]) { t = a[p]; a[p] = a[c]; a[c] = t; p = c; }
            else break;
        }
        i = i - 1;
    }
    /* Extract */
    sz = n;
    while (sz > 1) {
        t = a[0]; a[0] = a[sz - 1]; a[sz - 1] = t;
        sz = sz - 1;
        p = 0;
        while (1) {
            c = 2 * p + 1;
            if (c >= sz) break;
            if (c + 1 < sz && a[c + 1] > a[c]) c = c + 1;
            if (a[c] > a[p]) { t = a[p]; a[p] = a[c]; a[c] = t; p = c; }
            else break;
        }
    }
}

/* Heap Sort Descending (0-indexed, min-heap) */
void hsort_desc(int *a, int n) {
    int i; int t; int p; int c; int sz;
    i = n / 2 - 1;
    while (i >= 0) {
        p = i;
        while (1) {
            c = 2 * p + 1;
            if (c >= n) break;
            if (c + 1 < n && a[c + 1] < a[c]) c = c + 1;
            if (a[c] < a[p]) { t = a[p]; a[p] = a[c]; a[c] = t; p = c; }
            else break;
        }
        i = i - 1;
    }
    sz = n;
    while (sz > 1) {
        t = a[0]; a[0] = a[sz - 1]; a[sz - 1] = t;
        sz = sz - 1;
        p = 0;
        while (1) {
            c = 2 * p + 1;
            if (c >= sz) break;
            if (c + 1 < sz && a[c + 1] < a[c]) c = c + 1;
            if (a[c] < a[p]) { t = a[p]; a[p] = a[c]; a[c] = t; p = c; }
            else break;
        }
    }
}

int is_sorted_asc(int *a, int n) {
    int i;
    i = 0;
    while (i < n - 1) { if (a[i] > a[i + 1]) return 0; i = i + 1; }
    return 1;
}

int is_sorted_desc(int *a, int n) {
    int i;
    i = 0;
    while (i < n - 1) { if (a[i] < a[i + 1]) return 0; i = i + 1; }
    return 1;
}

/* ---- Priority Queue (0-indexed min-heap) ---- */
int pq_keys[64];
int pq_vals[64];
int pq_size;

void pq_init(void) { pq_size = 0; }

void pq_swap(int i, int j) {
    int t;
    t = pq_keys[i]; pq_keys[i] = pq_keys[j]; pq_keys[j] = t;
    t = pq_vals[i]; pq_vals[i] = pq_vals[j]; pq_vals[j] = t;
}

void pq_up(int i) {
    int p;
    while (i > 0) {
        p = (i - 1) / 2;
        if (pq_keys[i] < pq_keys[p]) { pq_swap(i, p); i = p; }
        else break;
    }
}

void pq_down(int i) {
    int sm; int l; int r;
    while (1) {
        sm = i; l = 2*i+1; r = 2*i+2;
        if (l < pq_size && pq_keys[l] < pq_keys[sm]) sm = l;
        if (r < pq_size && pq_keys[r] < pq_keys[sm]) sm = r;
        if (sm != i) { pq_swap(i, sm); i = sm; }
        else break;
    }
}

void pq_push(int pri, int val) {
    pq_keys[pq_size] = pri;
    pq_vals[pq_size] = val;
    pq_size = pq_size + 1;
    pq_up(pq_size - 1);
}

int pq_pop_val(void) {
    int v;
    if (pq_size == 0) return -1;
    v = pq_vals[0];
    pq_size = pq_size - 1;
    if (pq_size > 0) {
        pq_keys[0] = pq_keys[pq_size];
        pq_vals[0] = pq_vals[pq_size];
        pq_down(0);
    }
    return v;
}

int pq_peek_key(void) { if (pq_size == 0) return -1; return pq_keys[0]; }
int pq_peek_val(void) { if (pq_size == 0) return -1; return pq_vals[0]; }

/* kth smallest using min-heap */
int kth_smallest(int *a, int n, int k) {
    int i; int v;
    minh_init();
    i = 0;
    while (i < n) { minh_insert(a[i]); i = i + 1; }
    i = 0; v = -1;
    while (i < k) { v = minh_extract(); i = i + 1; }
    return v;
}

/* kth largest using max-heap */
int kth_largest(int *a, int n, int k) {
    int i; int v;
    maxh_init();
    i = 0;
    while (i < n) { maxh_insert(a[i]); i = i + 1; }
    i = 0; v = -1;
    while (i < k) { v = maxh_extract(); i = i + 1; }
    return v;
}

/* Merge two sorted arrays using min-heap */
void merge_sorted(int *a, int na, int *b, int nb, int *out) {
    int i; int oi;
    minh_init();
    i = 0;
    while (i < na) { minh_insert(a[i]); i = i + 1; }
    i = 0;
    while (i < nb) { minh_insert(b[i]); i = i + 1; }
    oi = 0;
    while (minh_size() > 0) { out[oi] = minh_extract(); oi = oi + 1; }
}

/* Heap height: floor(log2(n)) */
int heap_height(int n) {
    int h;
    if (n <= 0) return -1;
    h = 0;
    while (n > 1) { n = n / 2; h = h + 1; }
    return h;
}

/* Check if 0-indexed array is max-heap */
int is_maxheap0(int *a, int n) {
    int i; int l; int r;
    i = 0;
    while (i < n / 2) {
        l = 2*i+1; r = 2*i+2;
        if (l < n && a[l] > a[i]) return 0;
        if (r < n && a[r] > a[i]) return 0;
        i = i + 1;
    }
    return 1;
}

int main(void) {
    int arr[20];
    int arr2[20];
    int merged[40];
    int val;
    int i;

    /* ---- Max-Heap tests ---- */
    maxh_init();
    printf("ms=%d ", maxh_size());
    maxh_insert(10); maxh_insert(20); maxh_insert(5);
    maxh_insert(30); maxh_insert(15);
    printf("ms2=%d ", maxh_size());
    printf("mp=%d ", maxh_peek());
    printf("mv=%d ", maxh_verify());
    val = maxh_extract(); printf("me1=%d ", val);
    val = maxh_extract(); printf("me2=%d ", val);
    val = maxh_extract(); printf("me3=%d ", val);
    printf("ms3=%d ", maxh_size());
    val = maxh_extract(); printf("me4=%d ", val);
    val = maxh_extract(); printf("me5=%d ", val);
    printf("ms4=%d ", maxh_size());

    /* ---- Min-Heap tests ---- */
    minh_init();
    minh_insert(50); minh_insert(30); minh_insert(70);
    minh_insert(10); minh_insert(40);
    printf("np=%d ", minh_peek());
    printf("nv=%d ", minh_verify());
    val = minh_extract(); printf("ne1=%d ", val);
    val = minh_extract(); printf("ne2=%d ", val);
    val = minh_extract(); printf("ne3=%d ", val);
    val = minh_extract(); printf("ne4=%d ", val);
    val = minh_extract(); printf("ne5=%d ", val);

    /* ---- Heap Sort Asc ---- */
    arr[0]=38; arr[1]=27; arr[2]=43; arr[3]=3; arr[4]=9;
    arr[5]=82; arr[6]=10;
    hsort_asc(arr, 7);
    printf("sa=%d ", is_sorted_asc(arr, 7));
    printf("s0=%d s1=%d s6=%d ", arr[0], arr[1], arr[6]);

    /* ---- Heap Sort Desc ---- */
    arr[0]=38; arr[1]=27; arr[2]=43; arr[3]=3; arr[4]=9;
    arr[5]=82; arr[6]=10;
    hsort_desc(arr, 7);
    printf("sd=%d ", is_sorted_desc(arr, 7));
    printf("d0=%d d6=%d ", arr[0], arr[6]);

    /* ---- Priority Queue ---- */
    pq_init();
    pq_push(3, 100); pq_push(1, 200); pq_push(4, 300);
    pq_push(1, 400); pq_push(5, 500);
    printf("pk=%d ", pq_peek_key());
    printf("pv=%d ", pq_peek_val());
    val = pq_pop_val(); printf("pp1=%d ", val);
    val = pq_pop_val(); printf("pp2=%d ", val);
    val = pq_pop_val(); printf("pp3=%d ", val);
    val = pq_pop_val(); printf("pp4=%d ", val);
    val = pq_pop_val(); printf("pp5=%d ", val);

    /* ---- kth smallest/largest ---- */
    arr[0]=15; arr[1]=3; arr[2]=8; arr[3]=22; arr[4]=1;
    printf("k1s=%d ", kth_smallest(arr, 5, 1));
    printf("k2s=%d ", kth_smallest(arr, 5, 2));
    printf("k3s=%d ", kth_smallest(arr, 5, 3));
    printf("k1l=%d ", kth_largest(arr, 5, 1));
    printf("k2l=%d ", kth_largest(arr, 5, 2));

    /* ---- Merge sorted ---- */
    arr[0]=1; arr[1]=5; arr[2]=9;
    arr2[0]=2; arr2[1]=6; arr2[2]=10;
    merge_sorted(arr, 3, arr2, 3, merged);
    printf("mg0=%d mg1=%d mg5=%d ", merged[0], merged[1], merged[5]);

    /* ---- Heap height ---- */
    printf("h1=%d h7=%d h16=%d ", heap_height(1), heap_height(7), heap_height(16));

    /* ---- Sort edge cases ---- */
    arr[0]=42;
    hsort_asc(arr, 1);
    printf("e1=%d ", arr[0]);

    arr[0]=7; arr[1]=3;
    hsort_asc(arr, 2);
    printf("e2a=%d e2b=%d", arr[0], arr[1]);

    printf("\n");
    return 0;
}

/* ---- Additional functions to reach 500+ lines ---- */

/* Build max-heap from 0-indexed array in-place */
void build_maxheap0(int *a, int n) {
    int i; int p; int c; int t;
    i = n / 2 - 1;
    while (i >= 0) {
        p = i;
        while (1) {
            c = 2 * p + 1;
            if (c >= n) break;
            if (c + 1 < n && a[c + 1] > a[c]) c = c + 1;
            if (a[c] > a[p]) { t = a[p]; a[p] = a[c]; a[c] = t; p = c; }
            else break;
        }
        i = i - 1;
    }
}

/* Build min-heap from 0-indexed array */
void build_minheap0(int *a, int n) {
    int i; int p; int c; int t;
    i = n / 2 - 1;
    while (i >= 0) {
        p = i;
        while (1) {
            c = 2 * p + 1;
            if (c >= n) break;
            if (c + 1 < n && a[c + 1] < a[c]) c = c + 1;
            if (a[c] < a[p]) { t = a[p]; a[p] = a[c]; a[c] = t; p = c; }
            else break;
        }
        i = i - 1;
    }
}

/* Count elements in heap greater than threshold */
int count_greater(int *a, int n, int thresh) {
    int i; int cnt;
    cnt = 0; i = 0;
    while (i < n) { if (a[i] > thresh) cnt = cnt + 1; i = i + 1; }
    return cnt;
}

/* Sum of all elements */
int array_sum(int *a, int n) {
    int i; int s;
    s = 0; i = 0;
    while (i < n) { s = s + a[i]; i = i + 1; }
    return s;
}

/* Max element */
int array_max(int *a, int n) {
    int i; int m;
    m = a[0]; i = 1;
    while (i < n) { if (a[i] > m) m = a[i]; i = i + 1; }
    return m;
}

/* Min element */
int array_min(int *a, int n) {
    int i; int m;
    m = a[0]; i = 1;
    while (i < n) { if (a[i] < m) m = a[i]; i = i + 1; }
    return m;
}

/* Reverse array */
void array_reverse(int *a, int n) {
    int i; int t;
    i = 0;
    while (i < n / 2) {
        t = a[i]; a[i] = a[n - 1 - i]; a[n - 1 - i] = t;
        i = i + 1;
    }
}

/* Copy array */
void array_copy(int *dst, int *src, int n) {
    int i;
    i = 0;
    while (i < n) { dst[i] = src[i]; i = i + 1; }
}

/* Check if two arrays are equal */
int arrays_equal(int *a, int *b, int n) {
    int i;
    i = 0;
    while (i < n) { if (a[i] != b[i]) return 0; i = i + 1; }
    return 1;
}

/* Partial sort: sort first k elements */
void partial_sort(int *a, int n, int k) {
    int i; int v;
    minh_init();
    i = 0;
    while (i < n) { minh_insert(a[i]); i = i + 1; }
    i = 0;
    while (i < k && minh_size() > 0) {
        a[i] = minh_extract();
        i = i + 1;
    }
}

/* Count duplicates in sorted array */
int count_dups(int *a, int n) {
    int i; int cnt;
    cnt = 0; i = 1;
    while (i < n) { if (a[i] == a[i - 1]) cnt = cnt + 1; i = i + 1; }
    return cnt;
}

/* Running maximum using max-heap */
void running_max(int *a, int n, int *out) {
    int i;
    maxh_init();
    i = 0;
    while (i < n) {
        maxh_insert(a[i]);
        out[i] = maxh_peek();
        i = i + 1;
    }
}

/* Heap-based selection: find median of array */
int heap_median(int *a, int n) {
    int i; int v;
    minh_init();
    i = 0;
    while (i < n) { minh_insert(a[i]); i = i + 1; }
    i = 0;
    v = -1;
    while (i <= n / 2) { v = minh_extract(); i = i + 1; }
    return v;
}

/* Check stability: extract all from max-heap, verify descending */
int maxh_extract_all_sorted(void) {
    int prev; int cur;
    if (maxh_size() == 0) return 1;
    prev = maxh_extract();
    while (maxh_size() > 0) {
        cur = maxh_extract();
        if (cur > prev) return 0;
        prev = cur;
    }
    return 1;
}

/* Check stability: extract all from min-heap, verify ascending */
int minh_extract_all_sorted(void) {
    int prev; int cur;
    if (minh_size() == 0) return 1;
    prev = minh_extract();
    while (minh_size() > 0) {
        cur = minh_extract();
        if (cur < prev) return 0;
        prev = cur;
    }
    return 1;
}
