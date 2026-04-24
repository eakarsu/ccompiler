int printf(const char *fmt, ...);

int safe_divide(int a, int b) {
    if (b == 0) return -1;
    return a / b;
}

int classify_number(int n) {
    if (n < 0) return -1;
    if (n == 0) return 0;
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    return 4;
}

int find_first_negative(int *arr, int n) {
    if (n <= 0) return -1;
    int i = 0;
    while (i < n) {
        if (arr[i] < 0) return i;
        i = i + 1;
    }
    return -1;
}

int validate_range(int val, int lo, int hi) {
    if (lo > hi) return -1;
    if (val < lo) return 0;
    if (val > hi) return 0;
    return 1;
}

int str_to_int(char *s) {
    if (s[0] == 0) return 0;
    int sign = 1;
    int i = 0;
    if (s[0] == '-') {
        sign = -1;
        i = 1;
    }
    int result = 0;
    while (s[i] != 0) {
        if (s[i] < '0' || s[i] > '9') return -99999;
        result = result * 10 + (s[i] - '0');
        i = i + 1;
    }
    return result * sign;
}

int is_prime(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n - (n / 2) * 2 == 0) return 0;
    int i = 3;
    while (i * i <= n) {
        if (n - (n / i) * i == 0) return 0;
        i = i + 2;
    }
    return 1;
}

int binary_search(int *arr, int n, int target) {
    if (n <= 0) return -1;
    int lo = 0;
    int hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

int check_password(char *pw) {
    int len = 0;
    int has_upper = 0;
    int has_lower = 0;
    int has_digit = 0;
    int i = 0;
    while (pw[i] != 0) {
        if (pw[i] >= 'A' && pw[i] <= 'Z') has_upper = 1;
        if (pw[i] >= 'a' && pw[i] <= 'z') has_lower = 1;
        if (pw[i] >= '0' && pw[i] <= '9') has_digit = 1;
        len = len + 1;
        i = i + 1;
    }
    if (len < 4) return 0;
    if (has_upper == 0) return 1;
    if (has_lower == 0) return 2;
    if (has_digit == 0) return 3;
    return 4;
}

int array_guard(int *arr, int n, int idx) {
    if (arr == 0) return -1;
    if (n <= 0) return -2;
    if (idx < 0 || idx >= n) return -3;
    return arr[idx];
}

int nested_guards(int a, int b, int c) {
    if (a < 0) return -1;
    if (b < 0) return -2;
    if (c < 0) return -3;
    if (a == 0 && b == 0 && c == 0) return 0;
    if (a > 100 || b > 100 || c > 100) return -4;
    return a + b + c;
}

int grade(int score) {
    if (score < 0) return -1;
    if (score > 100) return -1;
    if (score >= 90) return 4;
    if (score >= 80) return 3;
    if (score >= 70) return 2;
    if (score >= 60) return 1;
    return 0;
}

int collatz_steps(int n) {
    if (n <= 0) return -1;
    if (n == 1) return 0;
    int steps = 0;
    while (n != 1) {
        if (n - (n / 2) * 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
        steps = steps + 1;
        if (steps > 1000) return -2;
    }
    return steps;
}

int main(void) {
    // EXPECT: div_ok: 5
    printf("div_ok: %d\n", safe_divide(10, 2));

    // EXPECT: div_zero: -1
    printf("div_zero: %d\n", safe_divide(10, 0));

    // EXPECT: class_neg: -1
    printf("class_neg: %d\n", classify_number(-5));

    // EXPECT: class_0: 0
    printf("class_0: %d\n", classify_number(0));

    // EXPECT: class_small: 1
    printf("class_small: %d\n", classify_number(5));

    // EXPECT: class_big: 4
    printf("class_big: %d\n", classify_number(5000));

    int arr1[] = {5, 3, -2, 8, -1};
    // EXPECT: first_neg: 2
    printf("first_neg: %d\n", find_first_negative(arr1, 5));

    int arr2[] = {5, 3, 2, 8, 1};
    // EXPECT: no_neg: -1
    printf("no_neg: %d\n", find_first_negative(arr2, 5));

    // EXPECT: valid_in: 1
    printf("valid_in: %d\n", validate_range(5, 0, 10));

    // EXPECT: valid_out: 0
    printf("valid_out: %d\n", validate_range(15, 0, 10));

    // EXPECT: valid_bad: -1
    printf("valid_bad: %d\n", validate_range(5, 10, 0));

    // EXPECT: stoi_123: 123
    printf("stoi_123: %d\n", str_to_int("123"));

    // EXPECT: stoi_neg: -42
    printf("stoi_neg: %d\n", str_to_int("-42"));

    // EXPECT: prime7: 1
    printf("prime7: %d\n", is_prime(7));

    // EXPECT: prime4: 0
    printf("prime4: %d\n", is_prime(4));

    // EXPECT: prime1: 0
    printf("prime1: %d\n", is_prime(1));

    int sorted[] = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};
    // EXPECT: bsearch_found: 5
    printf("bsearch_found: %d\n", binary_search(sorted, 10, 23));

    // EXPECT: bsearch_miss: -1
    printf("bsearch_miss: %d\n", binary_search(sorted, 10, 24));

    // EXPECT: pw_short: 0
    printf("pw_short: %d\n", check_password("Ab1"));

    // EXPECT: pw_good: 4
    printf("pw_good: %d\n", check_password("Ab1x"));

    // EXPECT: guard_ok: 30
    int garr[] = {10, 20, 30, 40, 50};
    printf("guard_ok: %d\n", array_guard(garr, 5, 2));

    // EXPECT: guard_oob: -3
    printf("guard_oob: %d\n", array_guard(garr, 5, 10));

    // EXPECT: nested_ok: 6
    printf("nested_ok: %d\n", nested_guards(1, 2, 3));

    // EXPECT: nested_neg: -2
    printf("nested_neg: %d\n", nested_guards(1, -1, 3));

    // EXPECT: grade90: 4
    printf("grade90: %d\n", grade(95));

    // EXPECT: grade70: 2
    printf("grade70: %d\n", grade(75));

    // EXPECT: collatz6: 8
    printf("collatz6: %d\n", collatz_steps(6));

    // EXPECT: collatz1: 0
    printf("collatz1: %d\n", collatz_steps(1));

    return 0;
}
