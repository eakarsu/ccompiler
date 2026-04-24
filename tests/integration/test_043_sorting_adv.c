int printf(const char *fmt, ...);
// EXPECT: === Iterative Merge Sort ===
// EXPECT: Input: 38 27 43 3 9 82 10 1
// EXPECT: After merge sort: 1 3 9 10 27 38 43 82
// EXPECT: Input: 5 4 3 2 1
// EXPECT: After merge sort: 1 2 3 4 5
// EXPECT: Input: 1 2 3 4 5
// EXPECT: After merge sort: 1 2 3 4 5
// EXPECT: === Quicksort ===
// EXPECT: Input: 10 7 8 9 1 5
// EXPECT: After quicksort: 1 5 7 8 9 10
// EXPECT: Input: 64 25 12 22 11
// EXPECT: After quicksort: 11 12 22 25 64
// EXPECT: Input: 3 3 3 3 3
// EXPECT: After quicksort: 3 3 3 3 3
// EXPECT: === Counting Sort ===
// EXPECT: Input: 4 2 2 8 3 3 1 7 5 4
// EXPECT: After counting sort: 1 2 2 3 3 4 4 5 7 8
// EXPECT: Count array: [0]=0 [1]=1 [2]=2 [3]=2 [4]=2 [5]=1 [6]=0 [7]=1 [8]=1
// EXPECT: === Radix Sort (Base 10) ===
// EXPECT: Input: 170 45 75 90 802 24 2 66
// EXPECT: After digit 1: 170 90 802 2 24 45 75 66
// EXPECT: After digit 2: 802 2 24 45 66 170 75 90
// EXPECT: After digit 3: 2 24 45 66 75 90 170 802
// EXPECT: Final: 2 24 45 66 75 90 170 802
// EXPECT: === Sort Stability Test ===
// EXPECT: Input pairs (key,id): (3,0) (1,1) (4,2) (1,3) (5,4) (3,5) (1,6)
// EXPECT: After stable sort: (1,1) (1,3) (1,6) (3,0) (3,5) (4,2) (5,4)
// EXPECT: Stability check: STABLE
// EXPECT: === Merge Sort on Large Array ===
// EXPECT: Sorted 20 elements: 1 2 5 7 8 11 13 14 17 19 22 23 27 31 33 37 41 43 47 50
// EXPECT: === Quicksort with Duplicates ===
// EXPECT: Input: 5 3 8 3 5 2 8 1 5 3
// EXPECT: After quicksort: 1 2 3 3 3 5 5 5 8 8
// EXPECT: === Counting Sort Range ===
// EXPECT: Input: 10 13 11 15 12 14 10 13
// EXPECT: After counting sort (range 10-15): 10 10 11 12 13 13 14 15
// EXPECT: === Radix Sort Larger ===
// EXPECT: Input: 329 457 657 839 436 720 355
// EXPECT: Sorted: 329 355 436 457 657 720 839
// EXPECT: === Insertion Sort Comparison ===
// EXPECT: Input: 12 11 13 5 6
// EXPECT: After insertion sort: 5 6 11 12 13
// EXPECT: === Selection Sort Comparison ===
// EXPECT: Input: 29 10 14 37 13
// EXPECT: After selection sort: 10 13 14 29 37
// EXPECT: === All sorting tests passed ===

int temp_arr[100];

void print_array(int *arr, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

void copy_array(int *dest, int *src, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        dest[i] = src[i];
    }
}

void merge(int *arr, int l, int m, int r) {
    int i;
    int j;
    int k;
    int n1;
    int n2;
    int left[50];
    int right[50];

    n1 = m - l + 1;
    n2 = r - m;

    for (i = 0; i < n1; i = i + 1) left[i] = arr[l + i];
    for (j = 0; j < n2; j = j + 1) right[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i = i + 1;
        } else {
            arr[k] = right[j];
            j = j + 1;
        }
        k = k + 1;
    }
    while (i < n1) {
        arr[k] = left[i];
        i = i + 1;
        k = k + 1;
    }
    while (j < n2) {
        arr[k] = right[j];
        j = j + 1;
        k = k + 1;
    }
}

void merge_sort(int *arr, int n) {
    int curr_size;
    int left_start;

    for (curr_size = 1; curr_size < n; curr_size = curr_size * 2) {
        for (left_start = 0; left_start < n - 1; left_start = left_start + 2 * curr_size) {
            int mid;
            int right_end;
            mid = left_start + curr_size - 1;
            if (mid >= n - 1) mid = n - 1;
            right_end = left_start + 2 * curr_size - 1;
            if (right_end >= n) right_end = n - 1;
            if (mid < right_end) {
                merge(arr, left_start, mid, right_end);
            }
        }
    }
}

void swap(int *a, int *b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int *arr, int low, int high) {
    int pivot;
    int i;
    int j;
    pivot = arr[high];
    i = low - 1;
    for (j = low; j < high; j = j + 1) {
        if (arr[j] <= pivot) {
            i = i + 1;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quicksort(int *arr, int n) {
    int stack[100];
    int top;
    int low;
    int high;
    int p;

    top = -1;
    stack[top + 1] = 0;
    stack[top + 2] = n - 1;
    top = top + 2;

    while (top >= 0) {
        high = stack[top];
        top = top - 1;
        low = stack[top];
        top = top - 1;

        if (low < high) {
            p = partition(arr, low, high);

            if (p - 1 > low) {
                top = top + 1;
                stack[top] = low;
                top = top + 1;
                stack[top] = p - 1;
            }
            if (p + 1 < high) {
                top = top + 1;
                stack[top] = p + 1;
                top = top + 1;
                stack[top] = high;
            }
        }
    }
}

void counting_sort(int *arr, int n, int max_val, int *count_out) {
    int count[100];
    int output[100];
    int i;

    for (i = 0; i <= max_val; i = i + 1) {
        count[i] = 0;
    }
    for (i = 0; i < n; i = i + 1) {
        count[arr[i]] = count[arr[i]] + 1;
    }

    if (count_out != 0) {
        for (i = 0; i <= max_val; i = i + 1) {
            count_out[i] = count[i];
        }
    }

    for (i = 1; i <= max_val; i = i + 1) {
        count[i] = count[i] + count[i - 1];
    }

    for (i = n - 1; i >= 0; i = i - 1) {
        output[count[arr[i]] - 1] = arr[i];
        count[arr[i]] = count[arr[i]] - 1;
    }

    for (i = 0; i < n; i = i + 1) {
        arr[i] = output[i];
    }
}

int get_digit(int num, int pos) {
    int i;
    int result;
    result = num;
    for (i = 0; i < pos; i = i + 1) {
        result = result / 10;
    }
    return result - (result / 10) * 10;
}

int find_max(int *arr, int n) {
    int max;
    int i;
    max = arr[0];
    for (i = 1; i < n; i = i + 1) {
        if (arr[i] > max) max = arr[i];
    }
    return max;
}

int num_digits(int num) {
    int count;
    if (num == 0) return 1;
    count = 0;
    while (num > 0) {
        count = count + 1;
        num = num / 10;
    }
    return count;
}

void radix_sort(int *arr, int n, int print_steps) {
    int max;
    int max_d;
    int d;
    int i;
    int count[10];
    int output[100];

    max = find_max(arr, n);
    max_d = num_digits(max);

    for (d = 0; d < max_d; d = d + 1) {
        for (i = 0; i < 10; i = i + 1) {
            count[i] = 0;
        }
        for (i = 0; i < n; i = i + 1) {
            count[get_digit(arr[i], d)] = count[get_digit(arr[i], d)] + 1;
        }
        for (i = 1; i < 10; i = i + 1) {
            count[i] = count[i] + count[i - 1];
        }
        for (i = n - 1; i >= 0; i = i - 1) {
            int dig;
            dig = get_digit(arr[i], d);
            output[count[dig] - 1] = arr[i];
            count[dig] = count[dig] - 1;
        }
        for (i = 0; i < n; i = i + 1) {
            arr[i] = output[i];
        }

        if (print_steps) {
            printf("After digit %d: ", d + 1);
            print_array(arr, n);
        }
    }
}

struct pair {
    int key;
    int id;
};

void merge_pairs(struct pair *arr, int l, int m, int r) {
    int n1;
    int n2;
    int i;
    int j;
    int k;
    struct pair left[50];
    struct pair right2[50];

    n1 = m - l + 1;
    n2 = r - m;

    for (i = 0; i < n1; i = i + 1) {
        left[i].key = arr[l + i].key;
        left[i].id = arr[l + i].id;
    }
    for (j = 0; j < n2; j = j + 1) {
        right2[j].key = arr[m + 1 + j].key;
        right2[j].id = arr[m + 1 + j].id;
    }

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (left[i].key <= right2[j].key) {
            arr[k].key = left[i].key;
            arr[k].id = left[i].id;
            i = i + 1;
        } else {
            arr[k].key = right2[j].key;
            arr[k].id = right2[j].id;
            j = j + 1;
        }
        k = k + 1;
    }
    while (i < n1) {
        arr[k].key = left[i].key;
        arr[k].id = left[i].id;
        i = i + 1;
        k = k + 1;
    }
    while (j < n2) {
        arr[k].key = right2[j].key;
        arr[k].id = right2[j].id;
        j = j + 1;
        k = k + 1;
    }
}

void stable_merge_sort(struct pair *arr, int n) {
    int curr_size;
    int left_start;

    for (curr_size = 1; curr_size < n; curr_size = curr_size * 2) {
        for (left_start = 0; left_start < n - 1; left_start = left_start + 2 * curr_size) {
            int mid;
            int right_end;
            mid = left_start + curr_size - 1;
            if (mid >= n - 1) mid = n - 1;
            right_end = left_start + 2 * curr_size - 1;
            if (right_end >= n) right_end = n - 1;
            if (mid < right_end) {
                merge_pairs(arr, left_start, mid, right_end);
            }
        }
    }
}

int check_stability(struct pair *arr, int n) {
    int i;
    for (i = 0; i < n - 1; i = i + 1) {
        if (arr[i].key == arr[i + 1].key) {
            if (arr[i].id > arr[i + 1].id) {
                return 0;
            }
        }
    }
    return 1;
}

void insertion_sort(int *arr, int n) {
    int i;
    int j;
    int key;
    for (i = 1; i < n; i = i + 1) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void selection_sort(int *arr, int n) {
    int i;
    int j;
    int min_idx;
    for (i = 0; i < n - 1; i = i + 1) {
        min_idx = i;
        for (j = i + 1; j < n; j = j + 1) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap(&arr[i], &arr[min_idx]);
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

void counting_sort_range(int *arr, int n, int lo, int hi) {
    int range;
    int count[100];
    int output[100];
    int i;

    range = hi - lo + 1;
    for (i = 0; i < range; i = i + 1) {
        count[i] = 0;
    }
    for (i = 0; i < n; i = i + 1) {
        count[arr[i] - lo] = count[arr[i] - lo] + 1;
    }
    for (i = 1; i < range; i = i + 1) {
        count[i] = count[i] + count[i - 1];
    }
    for (i = n - 1; i >= 0; i = i - 1) {
        output[count[arr[i] - lo] - 1] = arr[i];
        count[arr[i] - lo] = count[arr[i] - lo] - 1;
    }
    for (i = 0; i < n; i = i + 1) {
        arr[i] = output[i];
    }
}

int abs_val(int x) {
    if (x < 0) return 0 - x;
    return x;
}

int min_of(int a, int b) {
    if (a < b) return a;
    return b;
}

int max_of(int a, int b) {
    if (a > b) return a;
    return b;
}

int main(void) {
    int arr[100];
    int i;

    printf("=== Iterative Merge Sort ===\n");

    {
        int a[8];
        a[0] = 38; a[1] = 27; a[2] = 43; a[3] = 3;
        a[4] = 9; a[5] = 82; a[6] = 10; a[7] = 1;
        printf("Input: ");
        print_array(a, 8);
        merge_sort(a, 8);
        printf("After merge sort: ");
        print_array(a, 8);
    }

    {
        int a[5];
        a[0] = 5; a[1] = 4; a[2] = 3; a[3] = 2; a[4] = 1;
        printf("Input: ");
        print_array(a, 5);
        merge_sort(a, 5);
        printf("After merge sort: ");
        print_array(a, 5);
    }

    {
        int a[5];
        a[0] = 1; a[1] = 2; a[2] = 3; a[3] = 4; a[4] = 5;
        printf("Input: ");
        print_array(a, 5);
        merge_sort(a, 5);
        printf("After merge sort: ");
        print_array(a, 5);
    }

    printf("=== Quicksort ===\n");

    {
        int a[6];
        a[0] = 10; a[1] = 7; a[2] = 8; a[3] = 9; a[4] = 1; a[5] = 5;
        printf("Input: ");
        print_array(a, 6);
        quicksort(a, 6);
        printf("After quicksort: ");
        print_array(a, 6);
    }

    {
        int a[5];
        a[0] = 64; a[1] = 25; a[2] = 12; a[3] = 22; a[4] = 11;
        printf("Input: ");
        print_array(a, 5);
        quicksort(a, 5);
        printf("After quicksort: ");
        print_array(a, 5);
    }

    {
        int a[5];
        a[0] = 3; a[1] = 3; a[2] = 3; a[3] = 3; a[4] = 3;
        printf("Input: ");
        print_array(a, 5);
        quicksort(a, 5);
        printf("After quicksort: ");
        print_array(a, 5);
    }

    printf("=== Counting Sort ===\n");
    {
        int a[10];
        int counts[9];
        a[0] = 4; a[1] = 2; a[2] = 2; a[3] = 8; a[4] = 3;
        a[5] = 3; a[6] = 1; a[7] = 7; a[8] = 5; a[9] = 4;
        printf("Input: ");
        print_array(a, 10);
        counting_sort(a, 10, 8, counts);
        printf("After counting sort: ");
        print_array(a, 10);
        printf("Count array:");
        for (i = 0; i <= 8; i = i + 1) {
            printf(" [%d]=%d", i, counts[i]);
        }
        printf("\n");
    }

    printf("=== Radix Sort (Base 10) ===\n");
    {
        int a[8];
        a[0] = 170; a[1] = 45; a[2] = 75; a[3] = 90;
        a[4] = 802; a[5] = 24; a[6] = 2; a[7] = 66;
        printf("Input: ");
        print_array(a, 8);
        radix_sort(a, 8, 1);
        printf("Final: ");
        print_array(a, 8);
    }

    printf("=== Sort Stability Test ===\n");
    {
        struct pair pairs[7];
        pairs[0].key = 3; pairs[0].id = 0;
        pairs[1].key = 1; pairs[1].id = 1;
        pairs[2].key = 4; pairs[2].id = 2;
        pairs[3].key = 1; pairs[3].id = 3;
        pairs[4].key = 5; pairs[4].id = 4;
        pairs[5].key = 3; pairs[5].id = 5;
        pairs[6].key = 1; pairs[6].id = 6;

        printf("Input pairs (key,id):");
        for (i = 0; i < 7; i = i + 1) {
            printf(" (%d,%d)", pairs[i].key, pairs[i].id);
        }
        printf("\n");

        stable_merge_sort(pairs, 7);

        printf("After stable sort:");
        for (i = 0; i < 7; i = i + 1) {
            printf(" (%d,%d)", pairs[i].key, pairs[i].id);
        }
        printf("\n");

        if (check_stability(pairs, 7)) {
            printf("Stability check: STABLE\n");
        } else {
            printf("Stability check: UNSTABLE\n");
        }
    }

    printf("=== Merge Sort on Large Array ===\n");
    {
        int a[20];
        a[0] = 50; a[1] = 23; a[2] = 37; a[3] = 1; a[4] = 19;
        a[5] = 43; a[6] = 8; a[7] = 31; a[8] = 14; a[9] = 47;
        a[10] = 2; a[11] = 27; a[12] = 41; a[13] = 5; a[14] = 33;
        a[15] = 17; a[16] = 22; a[17] = 11; a[18] = 7; a[19] = 13;
        merge_sort(a, 20);
        printf("Sorted 20 elements: ");
        print_array(a, 20);
    }

    printf("=== Quicksort with Duplicates ===\n");
    {
        int a[10];
        a[0] = 5; a[1] = 3; a[2] = 8; a[3] = 3; a[4] = 5;
        a[5] = 2; a[6] = 8; a[7] = 1; a[8] = 5; a[9] = 3;
        printf("Input: ");
        print_array(a, 10);
        quicksort(a, 10);
        printf("After quicksort: ");
        print_array(a, 10);
    }

    printf("=== Counting Sort Range ===\n");
    {
        int a[8];
        a[0] = 10; a[1] = 13; a[2] = 11; a[3] = 15;
        a[4] = 12; a[5] = 14; a[6] = 10; a[7] = 13;
        printf("Input: ");
        print_array(a, 8);
        counting_sort_range(a, 8, 10, 15);
        printf("After counting sort (range 10-15): ");
        print_array(a, 8);
    }

    printf("=== Radix Sort Larger ===\n");
    {
        int a[7];
        a[0] = 329; a[1] = 457; a[2] = 657; a[3] = 839;
        a[4] = 436; a[5] = 720; a[6] = 355;
        printf("Input: ");
        print_array(a, 7);
        radix_sort(a, 7, 0);
        printf("Sorted: ");
        print_array(a, 7);
    }

    printf("=== Insertion Sort Comparison ===\n");
    {
        int a[5];
        a[0] = 12; a[1] = 11; a[2] = 13; a[3] = 5; a[4] = 6;
        printf("Input: ");
        print_array(a, 5);
        insertion_sort(a, 5);
        printf("After insertion sort: ");
        print_array(a, 5);
    }

    printf("=== Selection Sort Comparison ===\n");
    {
        int a[5];
        a[0] = 29; a[1] = 10; a[2] = 14; a[3] = 37; a[4] = 13;
        printf("Input: ");
        print_array(a, 5);
        selection_sort(a, 5);
        printf("After selection sort: ");
        print_array(a, 5);
    }

    printf("=== All sorting tests passed ===\n");
    return 0;
}
