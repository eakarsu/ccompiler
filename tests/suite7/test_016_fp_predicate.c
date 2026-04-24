int printf(const char *fmt, ...);

typedef int (*Predicate)(int);

int is_positive(int x) { return x > 0; }
int is_negative(int x) { return x < 0; }
int is_even(int x) { return (x % 2) == 0; }
int is_odd(int x) { return (x % 2) != 0; }
int is_gt_five(int x) { return x > 5; }
int is_zero(int x) { return x == 0; }
int is_between_1_10(int x) { return x >= 1 && x <= 10; }

int count_matching(int arr[], int n, Predicate pred) {
    int count;
    int i;
    count = 0;
    for (i = 0; i < n; i++) {
        if (pred(arr[i])) count = count + 1;
    }
    return count;
}

int find_first(int arr[], int n, Predicate pred) {
    int i;
    for (i = 0; i < n; i++) {
        if (pred(arr[i])) return i;
    }
    return -1;
}

int find_last(int arr[], int n, Predicate pred) {
    int i;
    for (i = n - 1; i >= 0; i--) {
        if (pred(arr[i])) return i;
    }
    return -1;
}

int filter(int arr[], int n, int out[], Predicate pred) {
    int j;
    int i;
    j = 0;
    for (i = 0; i < n; i++) {
        if (pred(arr[i])) {
            out[j] = arr[i];
            j = j + 1;
        }
    }
    return j;
}

int all_match(int arr[], int n, Predicate pred) {
    int i;
    for (i = 0; i < n; i++) {
        if (!pred(arr[i])) return 0;
    }
    return 1;
}

int any_match(int arr[], int n, Predicate pred) {
    int i;
    for (i = 0; i < n; i++) {
        if (pred(arr[i])) return 1;
    }
    return 0;
}

int none_match(int arr[], int n, Predicate pred) {
    int i;
    for (i = 0; i < n; i++) {
        if (pred(arr[i])) return 0;
    }
    return 1;
}

int partition(int arr[], int n, Predicate pred) {
    int left;
    int right;
    int tmp;
    left = 0;
    right = n - 1;
    while (left < right) {
        while (left < right && pred(arr[left])) left = left + 1;
        while (left < right && !pred(arr[right])) right = right - 1;
        if (left < right) {
            tmp = arr[left];
            arr[left] = arr[right];
            arr[right] = tmp;
            left = left + 1;
            right = right - 1;
        }
    }
    return count_matching(arr, n, pred);
}

void print_arr(int arr[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

int main(void) {
    int data[10];
    int out[10];
    int n;
    int cnt;

    data[0] = -5; data[1] = 3; data[2] = 0; data[3] = 8;
    data[4] = -2; data[5] = 7; data[6] = 1; data[7] = -4;
    data[8] = 6; data[9] = 0;
    n = 10;

    cnt = count_matching(data, n, is_positive);
    // EXPECT: positive: 5
    printf("positive: %d\n", cnt);

    cnt = count_matching(data, n, is_negative);
    // EXPECT: negative: 3
    printf("negative: %d\n", cnt);

    cnt = count_matching(data, n, is_even);
    // EXPECT: even: 6
    printf("even: %d\n", cnt);

    cnt = count_matching(data, n, is_zero);
    // EXPECT: zeros: 2
    printf("zeros: %d\n", cnt);

    // EXPECT: first positive: 1
    printf("first positive: %d\n", find_first(data, n, is_positive));

    // EXPECT: last positive: 8
    printf("last positive: %d\n", find_last(data, n, is_positive));

    // EXPECT: first negative: 0
    printf("first negative: %d\n", find_first(data, n, is_negative));

    // EXPECT: last negative: 7
    printf("last negative: %d\n", find_last(data, n, is_negative));

    cnt = filter(data, n, out, is_positive);
    // EXPECT: filtered positive(5): 3 8 7 1 6
    printf("filtered positive(%d): ", cnt);
    print_arr(out, cnt);

    cnt = filter(data, n, out, is_gt_five);
    // EXPECT: filtered >5(3): 8 7 6
    printf("filtered >5(%d): ", cnt);
    print_arr(out, cnt);

    // EXPECT: all positive: 0
    printf("all positive: %d\n", all_match(data, n, is_positive));

    // EXPECT: any negative: 1
    printf("any negative: %d\n", any_match(data, n, is_negative));

    // EXPECT: none zero: 0
    printf("none zero: %d\n", none_match(data, n, is_zero));

    // EXPECT: none gt_five for negs: 1
    printf("none gt_five for negs: %d\n", none_match(data, 1, is_gt_five));

    cnt = count_matching(data, n, is_between_1_10);
    // EXPECT: between 1-10: 5
    printf("between 1-10: %d\n", cnt);

    return 0;
}
