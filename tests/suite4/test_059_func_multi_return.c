int printf(const char *fmt, ...);

// Early return for error checking
int safe_divide(int a, int b) {
    if (b == 0) return -1;
    return a / b;
}

// Multiple return points based on conditions
int classify(int x) {
    if (x < 0) return -1;
    if (x == 0) return 0;
    return 1;
}

// Return based on range
int grade(int score) {
    if (score >= 90) return 4;
    if (score >= 80) return 3;
    if (score >= 70) return 2;
    if (score >= 60) return 1;
    return 0;
}

// Return string based on value
char *day_name(int day) {
    if (day == 0) return "Sunday";
    if (day == 1) return "Monday";
    if (day == 2) return "Tuesday";
    if (day == 3) return "Wednesday";
    if (day == 4) return "Thursday";
    if (day == 5) return "Friday";
    if (day == 6) return "Saturday";
    return "Invalid";
}

// Guard clauses with multiple early returns
int find_in_array(int *arr, int n, int target) {
    if (arr == 0) return -1;
    if (n <= 0) return -1;
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (arr[i] == target) return i;
    }
    return -1;
}

// Return from nested loops
int find_pair_sum(int *arr, int n, int target) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            if (arr[i] + arr[j] == target) {
                return i * 100 + j;
            }
        }
    }
    return -1;
}

// Return from switch
int month_days(int month) {
    switch (month) {
        case 1: return 31;
        case 2: return 28;
        case 3: return 31;
        case 4: return 30;
        case 5: return 31;
        case 6: return 30;
        case 7: return 31;
        case 8: return 31;
        case 9: return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31;
    }
    return -1;
}

// Multiple returns in recursive function
int binary_search(int *arr, int lo, int hi, int target) {
    if (lo > hi) return -1;
    int mid = lo + (hi - lo) / 2;
    if (arr[mid] == target) return mid;
    if (arr[mid] < target) return binary_search(arr, mid + 1, hi, target);
    return binary_search(arr, lo, mid - 1, target);
}

// Return different values based on struct content
struct Result {
    int value;
    int error;
};

int get_result(struct Result *r) {
    if (r->error != 0) return -999;
    return r->value;
}

// Complex conditional returns
int fizzbuzz_type(int n) {
    if (n % 15 == 0) return 3;
    if (n % 3 == 0) return 1;
    if (n % 5 == 0) return 2;
    return 0;
}

// Early return in validation
int validate_age(int age) {
    if (age < 0) return -1;
    if (age > 150) return -1;
    if (age < 18) return 0;
    return 1;
}

// Return from while loop
int count_trailing_zeros(int n) {
    if (n == 0) return -1;
    int count = 0;
    while (n % 10 == 0) {
        count = count + 1;
        n = n / 10;
    }
    return count;
}

// Return pointer based on condition
int g_positive = 1;
int g_negative = -1;
int g_zero_val = 0;

int *classify_ptr(int x) {
    if (x > 0) return &g_positive;
    if (x < 0) return &g_negative;
    return &g_zero_val;
}

// Multiple returns from do-while
int first_digit(int n) {
    if (n < 0) n = -n;
    if (n == 0) return 0;
    do {
        if (n < 10) return n;
        n = n / 10;
    } while (1);
    return 0;
}

int main(void) {
    // Safe divide
    // EXPECT: 5
    printf("%d\n", safe_divide(10, 2));
    // EXPECT: -1
    printf("%d\n", safe_divide(10, 0));

    // Classify
    // EXPECT: -1
    printf("%d\n", classify(-5));
    // EXPECT: 0
    printf("%d\n", classify(0));
    // EXPECT: 1
    printf("%d\n", classify(42));

    // Grade
    // EXPECT: 4
    printf("%d\n", grade(95));
    // EXPECT: 3
    printf("%d\n", grade(85));
    // EXPECT: 2
    printf("%d\n", grade(75));
    // EXPECT: 1
    printf("%d\n", grade(65));
    // EXPECT: 0
    printf("%d\n", grade(50));

    // Day name
    // EXPECT: Sunday
    printf("%s\n", day_name(0));
    // EXPECT: Wednesday
    printf("%s\n", day_name(3));
    // EXPECT: Saturday
    printf("%s\n", day_name(6));
    // EXPECT: Invalid
    printf("%s\n", day_name(9));

    // Find in array
    int arr[] = {5, 3, 7, 1, 9};
    // EXPECT: 2
    printf("%d\n", find_in_array(arr, 5, 7));
    // EXPECT: -1
    printf("%d\n", find_in_array(arr, 5, 6));

    // Find pair sum: arr[0]=5, arr[2]=7, 5+7=12, result=0*100+2=2
    // EXPECT: 2
    printf("%d\n", find_pair_sum(arr, 5, 12));
    // EXPECT: -1
    printf("%d\n", find_pair_sum(arr, 5, 99));

    // Month days
    // EXPECT: 31
    printf("%d\n", month_days(1));
    // EXPECT: 28
    printf("%d\n", month_days(2));
    // EXPECT: 30
    printf("%d\n", month_days(4));

    // Binary search
    int sorted[] = {1, 3, 5, 7, 9, 11, 13};
    // EXPECT: 3
    printf("%d\n", binary_search(sorted, 0, 6, 7));
    // EXPECT: -1
    printf("%d\n", binary_search(sorted, 0, 6, 8));

    // Struct result
    struct Result r1;
    r1.value = 42;
    r1.error = 0;
    // EXPECT: 42
    printf("%d\n", get_result(&r1));
    struct Result r2;
    r2.value = 42;
    r2.error = 1;
    // EXPECT: -999
    printf("%d\n", get_result(&r2));

    // Fizzbuzz type
    // EXPECT: 3
    printf("%d\n", fizzbuzz_type(15));
    // EXPECT: 1
    printf("%d\n", fizzbuzz_type(9));
    // EXPECT: 2
    printf("%d\n", fizzbuzz_type(10));
    // EXPECT: 0
    printf("%d\n", fizzbuzz_type(7));

    // Validate age
    // EXPECT: -1
    printf("%d\n", validate_age(-5));
    // EXPECT: 0
    printf("%d\n", validate_age(10));
    // EXPECT: 1
    printf("%d\n", validate_age(25));

    // Count trailing zeros
    // EXPECT: 2
    printf("%d\n", count_trailing_zeros(1200));
    // EXPECT: 0
    printf("%d\n", count_trailing_zeros(123));
    // EXPECT: -1
    printf("%d\n", count_trailing_zeros(0));

    // Classify pointer
    // EXPECT: 1
    printf("%d\n", *classify_ptr(99));
    // EXPECT: -1
    printf("%d\n", *classify_ptr(-5));
    // EXPECT: 0
    printf("%d\n", *classify_ptr(0));

    // First digit
    // EXPECT: 5
    printf("%d\n", first_digit(5432));
    // EXPECT: 9
    printf("%d\n", first_digit(987));
    // EXPECT: 1
    printf("%d\n", first_digit(-123));

    return 0;
}
