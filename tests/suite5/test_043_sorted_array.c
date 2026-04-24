int printf(const char *fmt, ...);

int arr[64];
int arr_len;

void sorted_init(void) {
    arr_len = 0;
}

int binary_search_pos(int val) {
    int lo;
    int hi;
    int mid;
    lo = 0;
    hi = arr_len;
    while (lo < hi) {
        mid = (lo + hi) / 2;
        if (arr[mid] < val) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

void sorted_insert(int val) {
    int pos;
    int i;
    pos = binary_search_pos(val);
    i = arr_len;
    while (i > pos) {
        arr[i] = arr[i - 1];
        i = i - 1;
    }
    arr[pos] = val;
    arr_len = arr_len + 1;
}

int sorted_find(int val) {
    int pos;
    pos = binary_search_pos(val);
    if (pos < arr_len && arr[pos] == val) {
        return pos;
    }
    return -1;
}

void sorted_remove_at(int idx) {
    int i;
    i = idx;
    while (i < arr_len - 1) {
        arr[i] = arr[i + 1];
        i = i + 1;
    }
    arr_len = arr_len - 1;
}

int sorted_remove(int val) {
    int pos;
    pos = sorted_find(val);
    if (pos >= 0) {
        sorted_remove_at(pos);
        return 1;
    }
    return 0;
}

int merged[128];

int merge_sorted(int a[], int alen, int b[], int blen) {
    int i;
    int j;
    int k;
    i = 0;
    j = 0;
    k = 0;
    while (i < alen && j < blen) {
        if (a[i] <= b[j]) {
            merged[k] = a[i];
            i = i + 1;
        } else {
            merged[k] = b[j];
            j = j + 1;
        }
        k = k + 1;
    }
    while (i < alen) {
        merged[k] = a[i];
        i = i + 1;
        k = k + 1;
    }
    while (j < blen) {
        merged[k] = b[j];
        j = j + 1;
        k = k + 1;
    }
    return k;
}

int main(void) {
    int idx;
    int ret;
    int b[5];
    int mlen;
    int i;

    sorted_init();

    sorted_insert(30);
    sorted_insert(10);
    sorted_insert(20);
    sorted_insert(50);
    sorted_insert(40);

    // EXPECT: len: 5
    printf("len: %d\n", arr_len);

    // EXPECT: sorted: 10 20 30 40 50
    printf("sorted: %d %d %d %d %d\n",
        arr[0], arr[1], arr[2], arr[3], arr[4]);

    idx = sorted_find(30);
    // EXPECT: find 30: 2
    printf("find 30: %d\n", idx);

    idx = sorted_find(99);
    // EXPECT: find 99: -1
    printf("find 99: %d\n", idx);

    idx = sorted_find(10);
    // EXPECT: find 10: 0
    printf("find 10: %d\n", idx);

    ret = sorted_remove(30);
    // EXPECT: remove 30: 1
    printf("remove 30: %d\n", ret);

    // EXPECT: after remove: 10 20 40 50
    printf("after remove: %d %d %d %d\n",
        arr[0], arr[1], arr[2], arr[3]);

    // EXPECT: len: 4
    printf("len: %d\n", arr_len);

    ret = sorted_remove(99);
    // EXPECT: remove 99: 0
    printf("remove 99: %d\n", ret);

    sorted_insert(25);
    sorted_insert(5);
    // EXPECT: inserted: 5 10 20 25 40 50
    printf("inserted: %d %d %d %d %d %d\n",
        arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);

    b[0] = 1;
    b[1] = 15;
    b[2] = 22;
    b[3] = 35;
    b[4] = 60;

    mlen = merge_sorted(arr, arr_len, b, 5);
    // EXPECT: merged len: 11
    printf("merged len: %d\n", mlen);

    // EXPECT: merged: 1 5 10 15 20 22 25 35 40 50 60
    printf("merged: %d %d %d %d %d %d %d %d %d %d %d\n",
        merged[0], merged[1], merged[2], merged[3],
        merged[4], merged[5], merged[6], merged[7],
        merged[8], merged[9], merged[10]);

    sorted_init();
    sorted_insert(42);
    // EXPECT: single: 42
    printf("single: %d\n", arr[0]);

    sorted_insert(41);
    // EXPECT: two: 41 42
    printf("two: %d %d\n", arr[0], arr[1]);

    sorted_insert(43);
    // EXPECT: three: 41 42 43
    printf("three: %d %d %d\n", arr[0], arr[1], arr[2]);

    return 0;
}
