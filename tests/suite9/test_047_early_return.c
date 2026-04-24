int printf(const char *fmt, ...);

int early_guard(int x) {
    if (x < 0) return -1;
    if (x == 0) return 0;
    if (x > 100) return 100;
    return x;
}

int nested_early_return(int a, int b, int c) {
    if (a <= 0) {
        if (b <= 0) {
            if (c <= 0) {
                return -3;
            }
            return -2;
        }
        return -1;
    }
    if (a > b) {
        if (a > c) {
            return a;
        }
        return c;
    }
    if (b > c) {
        return b;
    }
    return c;
}

int find_in_sorted(int *arr, int len, int target) {
    int lo = 0;
    int hi = len - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (arr[mid] == target) {
            return mid;
        }
        if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

int validate_input(int *data, int len) {
    if (!data) return -1;
    if (len <= 0) return -2;
    if (len > 100) return -3;

    int i;
    for (i = 0; i < len; i = i + 1) {
        if (data[i] < 0) return -4;
        if (data[i] > 1000) return -5;
    }

    int sum = 0;
    for (i = 0; i < len; i = i + 1) {
        sum = sum + data[i];
        if (sum > 5000) return -6;
    }

    return sum;
}

int recursive_early(int n, int depth) {
    if (depth > 10) return -1;
    if (n <= 1) return n;
    if (n % 2 == 0) return recursive_early(n / 2, depth + 1);
    return recursive_early(3 * n + 1, depth + 1);
}

int chain_return(int x) {
    if (x == 1) return 10;
    if (x == 2) return 20;
    if (x == 3) return 30;
    if (x == 4) return 40;
    if (x == 5) return 50;
    if (x == 6) return 60;
    if (x == 7) return 70;
    if (x == 8) return 80;
    if (x == 9) return 90;
    if (x == 10) return 100;
    return 0;
}

int loop_early_return(int n) {
    int i;
    int j;
    for (i = 2; i < n; i = i + 1) {
        int is_prime = 1;
        for (j = 2; j * j <= i; j = j + 1) {
            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime && i > n / 2) {
            return i;
        }
    }
    return -1;
}

int main() {
    // EXPECT: guard -5: -1
    printf("guard -5: %d\n", early_guard(-5));
    // EXPECT: guard 0: 0
    printf("guard 0: %d\n", early_guard(0));
    // EXPECT: guard 50: 50
    printf("guard 50: %d\n", early_guard(50));
    // EXPECT: guard 200: 100
    printf("guard 200: %d\n", early_guard(200));

    // EXPECT: nested -1 -1 -1: -3
    printf("nested -1 -1 -1: %d\n", nested_early_return(-1, -1, -1));
    // EXPECT: nested -1 -1 5: -2
    printf("nested -1 -1 5: %d\n", nested_early_return(-1, -1, 5));
    // EXPECT: nested -1 5 3: -1
    printf("nested -1 5 3: %d\n", nested_early_return(-1, 5, 3));
    // EXPECT: nested 10 5 3: 10
    printf("nested 10 5 3: %d\n", nested_early_return(10, 5, 3));
    // EXPECT: nested 5 10 7: 10
    printf("nested 5 10 7: %d\n", nested_early_return(5, 10, 7));
    // EXPECT: nested 5 7 10: 10
    printf("nested 5 7 10: %d\n", nested_early_return(5, 7, 10));

    int sorted[] = {2, 5, 8, 12, 16, 23, 38, 42, 56, 72};
    // EXPECT: find 23: 5
    printf("find 23: %d\n", find_in_sorted(sorted, 10, 23));
    // EXPECT: find 99: -1
    printf("find 99: %d\n", find_in_sorted(sorted, 10, 99));
    // EXPECT: find 2: 0
    printf("find 2: %d\n", find_in_sorted(sorted, 10, 2));

    int data1[] = {10, 20, 30, 40};
    // EXPECT: validate ok: 100
    printf("validate ok: %d\n", validate_input(data1, 4));

    int data2[] = {10, -5, 30};
    // EXPECT: validate neg: -4
    printf("validate neg: %d\n", validate_input(data2, 3));

    // EXPECT: recursive 7: -1
    printf("recursive 7: %d\n", recursive_early(7, 0));
    // EXPECT: recursive 16: 1
    printf("recursive 16: %d\n", recursive_early(16, 0));

    int sum = 0;
    int i;
    for (i = 1; i <= 10; i = i + 1) {
        sum = sum + chain_return(i);
    }
    // EXPECT: chain sum: 550
    printf("chain sum: %d\n", sum);

    // EXPECT: first prime > 25 in 50: 29
    printf("first prime > 25 in 50: %d\n", loop_early_return(50));

    return 0;
}
