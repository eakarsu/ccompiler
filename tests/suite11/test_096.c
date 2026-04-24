int printf(const char *fmt, ...);

// Test: Assignment in conditionals

int count_positive(int *arr, int n) {
    int count;
    int i;
    int val;
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        if ((val = arr[i]) > 0) {
            count = count + 1;
        }
    }
    return count;
}

int find_first_negative(int *arr, int n) {
    int i;
    int val;
    for (i = 0; i < n; i = i + 1) {
        if ((val = arr[i]) < 0) {
            return val;
        }
    }
    return 0;
}

int main() {
    int a;
    int b;
    int result;

    // Simple assignment in if
    if ((a = 5) > 3) {
        printf("%d\n", a); // EXPECT: 5
    }

    // Assignment in if with else
    if ((a = 0)) {
        printf("%d\n", 1);
    } else {
        printf("%d\n", 0); // EXPECT: 0
    }

    // Non-zero assignment evaluates true
    if ((a = 42)) {
        printf("%d\n", a); // EXPECT: 42
    }

    // Assignment in while
    int data[6];
    data[0] = 3;
    data[1] = 7;
    data[2] = 1;
    data[3] = 9;
    data[4] = 0;
    data[5] = 5;

    int idx;
    idx = 0;
    int val;
    int sum;
    sum = 0;
    while ((val = data[idx]) != 0) {
        sum = sum + val;
        idx = idx + 1;
    }
    printf("%d\n", sum); // EXPECT: 20
    printf("%d\n", idx); // EXPECT: 4

    // Nested assignment in conditionals
    if ((a = 10) > 5) {
        if ((b = a * 2) == 20) {
            printf("%d\n", b); // EXPECT: 20
        }
    }

    // Assignment in for condition
    int arr[8];
    arr[0] = 2;
    arr[1] = 4;
    arr[2] = 6;
    arr[3] = 8;
    arr[4] = 0;
    arr[5] = 10;
    arr[6] = 12;
    arr[7] = 14;

    sum = 0;
    int i;
    for (i = 0; (val = arr[i]) != 0; i = i + 1) {
        sum = sum + val;
    }
    printf("%d\n", sum); // EXPECT: 20

    // Assignment in ternary condition
    result = ((a = 7) > 5) ? a * 10 : 0;
    printf("%d\n", result); // EXPECT: 70

    // Chain of assignments
    int c;
    a = b = c = 99;
    printf("%d\n", a); // EXPECT: 99
    printf("%d\n", b); // EXPECT: 99
    printf("%d\n", c); // EXPECT: 99

    // Assignment chain in if
    if ((a = b = 15) > 10) {
        printf("%d\n", a); // EXPECT: 15
        printf("%d\n", b); // EXPECT: 15
    }

    // count_positive with assignment in condition
    int nums[7];
    nums[0] = 5;
    nums[1] = -3;
    nums[2] = 8;
    nums[3] = -1;
    nums[4] = 0;
    nums[5] = 7;
    nums[6] = -2;
    printf("%d\n", count_positive(nums, 7)); // EXPECT: 3

    // find_first_negative
    printf("%d\n", find_first_negative(nums, 7)); // EXPECT: -3

    // Assignment in do-while
    a = 10;
    sum = 0;
    do {
        sum = sum + a;
    } while ((a = a - 3) > 0);
    printf("%d\n", sum); // EXPECT: 22

    // Assignment comparison pattern
    a = 100;
    b = 200;
    if ((result = a - b) < 0) {
        printf("%d\n", result); // EXPECT: -100
    }

    // Assignment in switch
    switch ((a = 3)) {
    case 3:
        printf("%d\n", a); // EXPECT: 3
        break;
    default:
        printf("%d\n", 0);
        break;
    }

    // Multiple assignments in condition
    a = 0;
    b = 0;
    if ((a = 5, b = 10, a + b) > 12) {
        printf("%d\n", a + b); // EXPECT: 15
    }

    // Boolean-like assignment
    int flag;
    flag = 0;
    if ((flag = (5 > 3))) {
        printf("%d\n", flag); // EXPECT: 1
    }

    // Negative assignment in condition
    if ((a = -5) < 0) {
        printf("%d\n", a); // EXPECT: -5
    }

    return 0;
}
