int printf(const char *fmt, ...);

int binary_search(int arr[], int low, int high, int target) {
    if (low > high) return -1;
    int mid = low + (high - low) / 2;
    if (arr[mid] == target) return mid;
    if (arr[mid] > target) return binary_search(arr, low, mid - 1, target);
    return binary_search(arr, mid + 1, high, target);
}

int array_sum(int arr[], int n) {
    if (n <= 0) return 0;
    return arr[n - 1] + array_sum(arr, n - 1);
}

int array_max(int arr[], int n) {
    if (n == 1) return arr[0];
    int sub_max = array_max(arr, n - 1);
    if (arr[n - 1] > sub_max) return arr[n - 1];
    return sub_max;
}

int array_min(int arr[], int n) {
    if (n == 1) return arr[0];
    int sub_min = array_min(arr, n - 1);
    if (arr[n - 1] < sub_min) return arr[n - 1];
    return sub_min;
}

void array_reverse(int arr[], int left, int right) {
    if (left >= right) return;
    int tmp = arr[left];
    arr[left] = arr[right];
    arr[right] = tmp;
    array_reverse(arr, left + 1, right - 1);
}

int array_contains(int arr[], int n, int val) {
    if (n <= 0) return 0;
    if (arr[n - 1] == val) return 1;
    return array_contains(arr, n - 1, val);
}

int array_count(int arr[], int n, int val) {
    if (n <= 0) return 0;
    int c = array_count(arr, n - 1, val);
    if (arr[n - 1] == val) c = c + 1;
    return c;
}

int array_index_of(int arr[], int n, int val) {
    if (n <= 0) return -1;
    if (arr[0] == val) return 0;
    int sub = array_index_of(arr + 1, n - 1, val);
    if (sub == -1) return -1;
    return sub + 1;
}

int is_sorted(int arr[], int n) {
    if (n <= 1) return 1;
    if (arr[n - 2] > arr[n - 1]) return 0;
    return is_sorted(arr, n - 1);
}

int array_product(int arr[], int n) {
    if (n <= 0) return 1;
    return arr[n - 1] * array_product(arr, n - 1);
}

int count_positive(int arr[], int n) {
    if (n <= 0) return 0;
    int c = count_positive(arr, n - 1);
    if (arr[n - 1] > 0) c = c + 1;
    return c;
}

void print_array(int arr[], int n) {
    int i = 0;
    while (i < n) {
        if (i > 0) printf(",");
        printf("%d", arr[i]);
        i = i + 1;
    }
}

int main(void) {
    int sorted[10];
    sorted[0] = 2; sorted[1] = 5; sorted[2] = 8; sorted[3] = 12;
    sorted[4] = 16; sorted[5] = 23; sorted[6] = 38; sorted[7] = 56;
    sorted[8] = 72; sorted[9] = 91;

    // EXPECT: bsearch(23)=5
    printf("bsearch(23)=%d\n", binary_search(sorted, 0, 9, 23));
    // EXPECT: bsearch(2)=0
    printf("bsearch(2)=%d\n", binary_search(sorted, 0, 9, 2));
    // EXPECT: bsearch(91)=9
    printf("bsearch(91)=%d\n", binary_search(sorted, 0, 9, 91));
    // EXPECT: bsearch(50)=-1
    printf("bsearch(50)=%d\n", binary_search(sorted, 0, 9, 50));

    int arr[8];
    arr[0] = 3; arr[1] = 7; arr[2] = 1; arr[3] = 9;
    arr[4] = 2; arr[5] = 8; arr[6] = 4; arr[7] = 6;

    // EXPECT: sum=40
    printf("sum=%d\n", array_sum(arr, 8));
    // EXPECT: max=9
    printf("max=%d\n", array_max(arr, 8));
    // EXPECT: min=1
    printf("min=%d\n", array_min(arr, 8));
    // EXPECT: contains(7)=1
    printf("contains(7)=%d\n", array_contains(arr, 8, 7));
    // EXPECT: contains(5)=0
    printf("contains(5)=%d\n", array_contains(arr, 8, 5));

    int arr2[10];
    arr2[0] = 1; arr2[1] = 2; arr2[2] = 3; arr2[3] = 4; arr2[4] = 5;
    arr2[5] = 6; arr2[6] = 7; arr2[7] = 8; arr2[8] = 9; arr2[9] = 10;

    array_reverse(arr2, 0, 9);
    // EXPECT: reversed=10,9,8,7,6,5,4,3,2,1
    printf("reversed=");
    print_array(arr2, 10);
    printf("\n");

    int arr3[8];
    arr3[0] = 1; arr3[1] = 3; arr3[2] = 3; arr3[3] = 5;
    arr3[4] = 3; arr3[5] = 7; arr3[6] = 3; arr3[7] = 9;

    // EXPECT: count(3)=4
    printf("count(3)=%d\n", array_count(arr3, 8, 3));
    // EXPECT: count(1)=1
    printf("count(1)=%d\n", array_count(arr3, 8, 1));
    // EXPECT: indexOf(5)=3
    printf("indexOf(5)=%d\n", array_index_of(arr3, 8, 5));
    // EXPECT: indexOf(99)=-1
    printf("indexOf(99)=%d\n", array_index_of(arr3, 8, 99));
    // EXPECT: isSorted=0
    printf("isSorted=%d\n", is_sorted(arr3, 8));
    // EXPECT: isSortedS=1
    printf("isSortedS=%d\n", is_sorted(sorted, 10));

    int small[4];
    small[0] = 2; small[1] = 3; small[2] = 4; small[3] = 5;
    // EXPECT: product=120
    printf("product=%d\n", array_product(small, 4));

    int mixed[6];
    mixed[0] = -3; mixed[1] = 5; mixed[2] = -1; mixed[3] = 8;
    mixed[4] = 0; mixed[5] = 2;
    // EXPECT: positive=3
    printf("positive=%d\n", count_positive(mixed, 6));

    return 0;
}
