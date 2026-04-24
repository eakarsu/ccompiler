int printf(const char *fmt, ...);

int arr_global[8];

void fill_array(int *a, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        *(a + i) = (i + 1) * 10;
    }
}

int count_elements(int *start, int *end) {
    return end - start;
}

void reverse_via_ptrs(int *start, int *end) {
    end = end - 1;
    while (start < end) {
        int tmp = *start;
        *start = *end;
        *end = tmp;
        start = start + 1;
        end = end - 1;
    }
}

int find_via_ptr(int *arr, int n, int target) {
    int *p = arr;
    int *end = arr + n;
    while (p < end) {
        if (*p == target) {
            return p - arr;
        }
        p = p + 1;
    }
    return -1;
}

int main(void) {
    int arr[6];
    arr[0] = 100;
    arr[1] = 200;
    arr[2] = 300;
    arr[3] = 400;
    arr[4] = 500;
    arr[5] = 600;

    int *p = arr;

    // EXPECT: 100
    printf("%d\n", *p);

    p = p + 1;
    // EXPECT: 200
    printf("%d\n", *p);

    p = p + 2;
    // EXPECT: 400
    printf("%d\n", *p);

    p = p - 1;
    // EXPECT: 300
    printf("%d\n", *p);

    int *q = arr + 5;
    // EXPECT: 600
    printf("%d\n", *q);

    int diff = q - arr;
    // EXPECT: 5
    printf("%d\n", diff);

    int *start = arr;
    int *end = arr + 6;
    // EXPECT: 6
    printf("%d\n", count_elements(start, end));

    int sum = 0;
    int *iter = arr;
    while (iter < arr + 6) {
        sum = sum + *iter;
        iter = iter + 1;
    }
    // EXPECT: 2100
    printf("%d\n", sum);

    fill_array(arr_global, 8);
    // EXPECT: 10
    printf("%d\n", arr_global[0]);
    // EXPECT: 50
    printf("%d\n", arr_global[4]);
    // EXPECT: 80
    printf("%d\n", arr_global[7]);

    int data[5];
    data[0] = 1;
    data[1] = 2;
    data[2] = 3;
    data[3] = 4;
    data[4] = 5;

    reverse_via_ptrs(data, data + 5);
    // EXPECT: 5
    printf("%d\n", data[0]);
    // EXPECT: 3
    printf("%d\n", data[2]);
    // EXPECT: 1
    printf("%d\n", data[4]);

    int idx = find_via_ptr(data, 5, 3);
    // EXPECT: 2
    printf("%d\n", idx);

    idx = find_via_ptr(data, 5, 99);
    // EXPECT: -1
    printf("%d\n", idx);

    char str[4];
    str[0] = 'A';
    str[1] = 'B';
    str[2] = 'C';
    str[3] = 0;
    char *cp = str;
    // EXPECT: 65
    printf("%d\n", *cp);

    cp = cp + 1;
    // EXPECT: 66
    printf("%d\n", *cp);

    cp = cp + 1;
    // EXPECT: 67
    printf("%d\n", *cp);

    int nums[4];
    nums[0] = 10;
    nums[1] = 20;
    nums[2] = 30;
    nums[3] = 40;
    int *mid = nums + 2;
    // EXPECT: 2
    printf("%d\n", mid - nums);

    int before = (nums < mid);
    // EXPECT: 1
    printf("%d\n", before);

    int after = (mid < nums);
    // EXPECT: 0
    printf("%d\n", after);

    return 0;
}
