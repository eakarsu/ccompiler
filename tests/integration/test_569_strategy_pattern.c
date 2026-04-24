int printf(const char *fmt, ...);
// EXPECT: Original: 64 25 12 22 11 90 45 37\nBubble sort (14 ops): 11 12 22 25 37 45 64 90\nSelection sort (6 ops): 11 12 22 25 37 45 64 90\nInsertion sort (14 ops): 11 12 22 25 37 45 64 90\nLinear search 45: idx=5\nBinary search 45: idx=5\nBinary search 99: idx=-1

typedef int (*SortStrategy)(int *, int);
typedef int (*SearchStrategy)(int *, int, int);

int bubble_sort(int *arr, int n) {
    int swaps = 0;
    int i;
    int j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
                swaps++;
            }
        }
    }
    return swaps;
}

int selection_sort(int *arr, int n) {
    int swaps = 0;
    int i;
    int j;
    for (i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) min_idx = j;
        }
        if (min_idx != i) {
            int tmp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = tmp;
            swaps++;
        }
    }
    return swaps;
}

int insertion_sort(int *arr, int n) {
    int moves = 0;
    int i;
    int j;
    for (i = 1; i < n; i++) {
        int key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
            moves++;
        }
        arr[j + 1] = key;
    }
    return moves;
}

int linear_search(int *arr, int n, int target) {
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] == target) return i;
    }
    return -1;
}

int binary_search(int *arr, int n, int target) {
    int lo = 0;
    int hi = n - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

struct Sorter {
    char *name;
    SortStrategy sort;
};

void copy_arr(int *dst, int *src, int n) {
    int i;
    for (i = 0; i < n; i++) dst[i] = src[i];
}

void print_arr(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

int main(void) {
    int original[8];
    original[0] = 64; original[1] = 25; original[2] = 12; original[3] = 22;
    original[4] = 11; original[5] = 90; original[6] = 45; original[7] = 37;
    int n = 8;

    struct Sorter sorters[3];
    sorters[0].name = "Bubble";     sorters[0].sort = bubble_sort;
    sorters[1].name = "Selection";  sorters[1].sort = selection_sort;
    sorters[2].name = "Insertion";  sorters[2].sort = insertion_sort;

    printf("Original: ");
    print_arr(original, n);

    int work[8];
    int i;
    for (i = 0; i < 3; i++) {
        copy_arr(work, original, n);
        int ops = sorters[i].sort(work, n);
        printf("%s sort (%d ops): ", sorters[i].name, ops);
        print_arr(work, n);
    }

    copy_arr(work, original, n);
    bubble_sort(work, n);
    printf("Linear search 45: idx=%d\n", linear_search(work, n, 45));
    printf("Binary search 45: idx=%d\n", binary_search(work, n, 45));
    printf("Binary search 99: idx=%d\n", binary_search(work, n, 99));

    return 0;
}
