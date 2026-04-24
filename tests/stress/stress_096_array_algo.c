// stress_096_array_algo.c - Array algorithm torture test
// Exercises: rotate left/right, Dutch national flag, merge sorted,
// quickselect, remove duplicates, reverse, binary search.
// EXPECT: rotate_left=3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2
// EXPECT: rotate_right=19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18
// EXPECT: dnf=0 0 0 0 0 0 0 1 1 1 1 1 1 1 2 2 2 2 2 2
// EXPECT: merged=1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
// EXPECT: kth_1=1
// EXPECT: kth_5=5
// EXPECT: kth_10=10
// EXPECT: kth_15=15
// EXPECT: dedup_count=10
// EXPECT: dedup=1 2 3 4 5 6 7 8 9 10
// EXPECT: reversed=20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1
// EXPECT: bsearch_found=7
// EXPECT: bsearch_notfound=-1
// EXPECT: insertion_sort=1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
// EXPECT: all_pass=1

int printf(const char *fmt, ...);

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// Rotate array left by k positions
void rotate_left(int *arr, int n, int k) {
    k = k % n;
    // reverse first k
    int i = 0;
    int j = k - 1;
    while (i < j) { swap(&arr[i], &arr[j]); i++; j--; }
    // reverse rest
    i = k; j = n - 1;
    while (i < j) { swap(&arr[i], &arr[j]); i++; j--; }
    // reverse all
    i = 0; j = n - 1;
    while (i < j) { swap(&arr[i], &arr[j]); i++; j--; }
}

// Rotate array right by k positions
void rotate_right(int *arr, int n, int k) {
    k = k % n;
    rotate_left(arr, n, n - k);
}

// Dutch national flag partition: 0s, 1s, 2s
void dutch_flag(int *arr, int n) {
    int lo = 0;
    int mid = 0;
    int hi = n - 1;
    while (mid <= hi) {
        if (arr[mid] == 0) {
            swap(&arr[lo], &arr[mid]);
            lo++;
            mid++;
        } else if (arr[mid] == 1) {
            mid++;
        } else {
            swap(&arr[mid], &arr[hi]);
            hi--;
        }
    }
}

// Merge two sorted arrays into result
void merge_sorted(int *a, int an, int *b, int bn, int *out) {
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < an && j < bn) {
        if (a[i] <= b[j]) {
            out[k] = a[i]; i++;
        } else {
            out[k] = b[j]; j++;
        }
        k++;
    }
    while (i < an) { out[k] = a[i]; i++; k++; }
    while (j < bn) { out[k] = b[j]; j++; k++; }
}

// Quickselect: find kth smallest (1-indexed)
int partition(int *arr, int lo, int hi) {
    int pivot = arr[hi];
    int i = lo;
    int j;
    for (j = lo; j < hi; j++) {
        if (arr[j] <= pivot) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[hi]);
    return i;
}

int quickselect(int *arr, int lo, int hi, int k) {
    if (lo == hi) return arr[lo];
    int p = partition(arr, lo, hi);
    if (k == p) return arr[p];
    if (k < p) return quickselect(arr, lo, p - 1, k);
    return quickselect(arr, p + 1, hi, k);
}

// Remove duplicates from sorted array, return new length
int remove_duplicates(int *arr, int n) {
    if (n <= 1) return n;
    int w = 1;
    int i;
    for (i = 1; i < n; i++) {
        if (arr[i] != arr[i - 1]) {
            arr[w] = arr[i];
            w++;
        }
    }
    return w;
}

// Reverse array in place
void reverse(int *arr, int n) {
    int i = 0;
    int j = n - 1;
    while (i < j) {
        swap(&arr[i], &arr[j]);
        i++;
        j--;
    }
}

// Binary search: returns index or -1
int binary_search(int *arr, int n, int target) {
    int lo = 0;
    int hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

// Insertion sort
void insertion_sort(int *arr, int n) {
    int i;
    for (i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void print_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

int arrays_equal(int *a, int *b, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int main(void) {
    int pass = 1;
    int i;

    // Rotate left by 2
    int rl[20];
    for (i = 0; i < 20; i++) rl[i] = i + 1;
    rotate_left(rl, 20, 2);
    printf("rotate_left=");
    print_array(rl, 20);
    if (rl[0] != 3 || rl[18] != 1 || rl[19] != 2) pass = 0;

    // Rotate right by 2
    int rr[20];
    for (i = 0; i < 20; i++) rr[i] = i + 1;
    rotate_right(rr, 20, 2);
    printf("rotate_right=");
    print_array(rr, 20);
    if (rr[0] != 19 || rr[1] != 20 || rr[2] != 1) pass = 0;

    // Dutch national flag
    int dnf[20];
    dnf[0]=2; dnf[1]=0; dnf[2]=1; dnf[3]=2; dnf[4]=0;
    dnf[5]=1; dnf[6]=0; dnf[7]=2; dnf[8]=1; dnf[9]=0;
    dnf[10]=2; dnf[11]=1; dnf[12]=0; dnf[13]=2; dnf[14]=0;
    dnf[15]=1; dnf[16]=2; dnf[17]=0; dnf[18]=1; dnf[19]=1;
    dutch_flag(dnf, 20);
    printf("dnf=");
    print_array(dnf, 20);
    // verify sorted: all 0s then 1s then 2s
    int prev_val = 0;
    for (i = 0; i < 20; i++) {
        if (dnf[i] < prev_val) { pass = 0; break; }
        prev_val = dnf[i];
    }

    // Merge sorted
    int sa[10];
    int sb[10];
    for (i = 0; i < 10; i++) { sa[i] = 2 * i + 1; sb[i] = 2 * i + 2; }
    // sa = 1,3,5,7,9,11,13,15,17,19  sb = 2,4,6,8,10,12,14,16,18,20
    int merged[20];
    merge_sorted(sa, 10, sb, 10, merged);
    printf("merged=");
    print_array(merged, 20);
    for (i = 0; i < 20; i++) {
        if (merged[i] != i + 1) { pass = 0; break; }
    }

    // Quickselect
    int qs[20];
    qs[0]=15; qs[1]=3; qs[2]=9; qs[3]=1; qs[4]=14;
    qs[5]=7; qs[6]=20; qs[7]=2; qs[8]=18; qs[9]=5;
    qs[10]=12; qs[11]=8; qs[12]=17; qs[13]=4; qs[14]=11;
    qs[15]=6; qs[16]=19; qs[17]=10; qs[18]=16; qs[19]=13;

    // Copy for each test since quickselect modifies the array
    int qc[20];
    for (i = 0; i < 20; i++) qc[i] = qs[i];
    printf("kth_1=%d\n", quickselect(qc, 0, 19, 0));
    if (quickselect(qs, 0, 19, 0) != 1) pass = 0;

    for (i = 0; i < 20; i++) qc[i] = qs[i];
    // qs may be modified, re-init
    qs[0]=15; qs[1]=3; qs[2]=9; qs[3]=1; qs[4]=14;
    qs[5]=7; qs[6]=20; qs[7]=2; qs[8]=18; qs[9]=5;
    qs[10]=12; qs[11]=8; qs[12]=17; qs[13]=4; qs[14]=11;
    qs[15]=6; qs[16]=19; qs[17]=10; qs[18]=16; qs[19]=13;
    for (i = 0; i < 20; i++) qc[i] = qs[i];
    printf("kth_5=%d\n", quickselect(qc, 0, 19, 4));
    if (quickselect(qc, 0, 19, 4) != 5) pass = 0;

    for (i = 0; i < 20; i++) qs[i] = i + 1; // reset
    // Shuffle deterministically
    qs[0]=15; qs[1]=3; qs[2]=9; qs[3]=1; qs[4]=14;
    qs[5]=7; qs[6]=20; qs[7]=2; qs[8]=18; qs[9]=5;
    qs[10]=12; qs[11]=8; qs[12]=17; qs[13]=4; qs[14]=11;
    qs[15]=6; qs[16]=19; qs[17]=10; qs[18]=16; qs[19]=13;
    for (i = 0; i < 20; i++) qc[i] = qs[i];
    printf("kth_10=%d\n", quickselect(qc, 0, 19, 9));

    for (i = 0; i < 20; i++) qc[i] = qs[i];
    printf("kth_15=%d\n", quickselect(qc, 0, 19, 14));

    // Remove duplicates
    int dup[20];
    dup[0]=1; dup[1]=1; dup[2]=2; dup[3]=2; dup[4]=3;
    dup[5]=3; dup[6]=4; dup[7]=5; dup[8]=5; dup[9]=5;
    dup[10]=6; dup[11]=7; dup[12]=7; dup[13]=8; dup[14]=8;
    dup[15]=8; dup[16]=9; dup[17]=9; dup[18]=10; dup[19]=10;
    int newlen = remove_duplicates(dup, 20);
    printf("dedup_count=%d\n", newlen);
    if (newlen != 10) pass = 0;
    printf("dedup=");
    print_array(dup, newlen);
    for (i = 0; i < newlen; i++) {
        if (dup[i] != i + 1) { pass = 0; break; }
    }

    // Reverse
    int rev[20];
    for (i = 0; i < 20; i++) rev[i] = i + 1;
    reverse(rev, 20);
    printf("reversed=");
    print_array(rev, 20);
    if (rev[0] != 20 || rev[19] != 1) pass = 0;

    // Binary search
    int sorted[20];
    for (i = 0; i < 20; i++) sorted[i] = (i + 1) * 3;
    // sorted = 3,6,9,...,60
    int idx = binary_search(sorted, 20, 24);  // 24 = 8*3, index 7
    printf("bsearch_found=%d\n", idx);
    if (idx != 7) pass = 0;
    idx = binary_search(sorted, 20, 25);
    printf("bsearch_notfound=%d\n", idx);
    if (idx != -1) pass = 0;

    // Insertion sort
    int isort[20];
    isort[0]=20; isort[1]=19; isort[2]=18; isort[3]=17; isort[4]=16;
    isort[5]=15; isort[6]=14; isort[7]=13; isort[8]=12; isort[9]=11;
    isort[10]=10; isort[11]=9; isort[12]=8; isort[13]=7; isort[14]=6;
    isort[15]=5; isort[16]=4; isort[17]=3; isort[18]=2; isort[19]=1;
    insertion_sort(isort, 20);
    printf("insertion_sort=");
    print_array(isort, 20);
    for (i = 0; i < 20; i++) {
        if (isort[i] != i + 1) { pass = 0; break; }
    }

    printf("all_pass=%d\n", pass);
    return 0;
}
