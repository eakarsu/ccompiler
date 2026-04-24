int printf(const char *fmt, ...);
// EXPECT: === Deep Recursion Tests ===
// EXPECT: hanoi_1: A->C
// EXPECT: hanoi_2: A->B A->C B->C
// EXPECT: hanoi_3: A->C A->B C->B A->C B->A B->C A->C
// EXPECT: hanoi_count_3: 7
// EXPECT: hanoi_count_4: 15
// EXPECT: bsearch_found: index=4
// EXPECT: bsearch_first: index=0
// EXPECT: bsearch_last: index=9
// EXPECT: bsearch_notfound: index=-1
// EXPECT: bsearch_single: index=0
// EXPECT: power_2_0: 1
// EXPECT: power_2_10: 1024
// EXPECT: power_3_5: 243
// EXPECT: power_fast_2_10: 1024
// EXPECT: power_fast_3_7: 2187
// EXPECT: power_fast_5_4: 625
// EXPECT: is_even_0: yes
// EXPECT: is_odd_1: yes
// EXPECT: is_even_10: yes
// EXPECT: is_odd_7: yes
// EXPECT: is_even_3: no
// EXPECT: sum_digits_12345: 15
// EXPECT: sum_digits_9999: 36
// EXPECT: sum_digits_0: 0
// EXPECT: count_digits_12345: 5
// EXPECT: count_digits_1: 1
// EXPECT: count_digits_1000000: 7
// EXPECT: reverse_num_12345: 54321
// EXPECT: reverse_num_1000: 1
// EXPECT: ackermann_0_0: 1
// EXPECT: ackermann_1_1: 3
// EXPECT: ackermann_2_2: 7
// EXPECT: ackermann_3_2: 29
// EXPECT: flatten_sum_depth3: 55
// EXPECT: rec_gcd_48_18: 6
// EXPECT: rec_gcd_100_0: 100
// EXPECT: rec_max: 50
// EXPECT: rec_min: 5
// EXPECT: === All Deep Recursion Tests Passed ===

// Hanoi - print moves
void hanoi(int n, char from, char to, char aux) {
    if (n == 0) return;
    hanoi(n - 1, from, aux, to);
    printf("%c->%c", from, to);
    if (n > 0) {
        // We need a way to know if more moves follow - caller handles spacing
    }
    hanoi(n - 1, aux, to, from);
}

// Count hanoi moves
int hanoi_count(int n) {
    if (n == 0) return 0;
    return 1 + 2 * hanoi_count(n - 1);
}

// Recursive binary search
int bsearch_rec(int *arr, int lo, int hi, int target) {
    if (lo > hi) return -1;
    int mid = lo + (hi - lo) / 2;
    if (arr[mid] == target) return mid;
    if (arr[mid] < target) return bsearch_rec(arr, mid + 1, hi, target);
    return bsearch_rec(arr, lo, mid - 1, target);
}

// Simple recursive power
int rec_power(int base, int exp) {
    if (exp == 0) return 1;
    return base * rec_power(base, exp - 1);
}

// Fast recursive power (exponentiation by squaring)
int fast_power(int base, int exp) {
    if (exp == 0) return 1;
    if (exp % 2 == 0) {
        int half = fast_power(base, exp / 2);
        return half * half;
    }
    return base * fast_power(base, exp - 1);
}

// Mutual recursion: is_even and is_odd
int mutual_is_odd(int n);

int mutual_is_even(int n) {
    if (n == 0) return 1;
    return mutual_is_odd(n - 1);
}

int mutual_is_odd(int n) {
    if (n == 0) return 0;
    return mutual_is_even(n - 1);
}

// Recursive digit sum
int sum_digits(int n) {
    if (n < 0) n = -n;
    if (n < 10) return n;
    return (n % 10) + sum_digits(n / 10);
}

// Recursive digit count
int count_digits(int n) {
    if (n < 0) n = -n;
    if (n < 10) return 1;
    return 1 + count_digits(n / 10);
}

// Recursive number reversal
int reverse_num_helper(int n, int acc) {
    if (n == 0) return acc;
    return reverse_num_helper(n / 10, acc * 10 + n % 10);
}

int reverse_num(int n) {
    return reverse_num_helper(n, 0);
}

// Ackermann function (small values only!)
int ackermann(int m, int n) {
    if (m == 0) return n + 1;
    if (n == 0) return ackermann(m - 1, 1);
    return ackermann(m - 1, ackermann(m, n - 1));
}

// Recursive sum of 1..n (deep recursion)
int rec_sum(int n) {
    if (n <= 0) return 0;
    return n + rec_sum(n - 1);
}

// Recursive GCD
int rec_gcd(int a, int b) {
    if (b == 0) return a;
    return rec_gcd(b, a % b);
}

// Recursive max of array
int rec_max(int *arr, int n) {
    if (n == 1) return arr[0];
    int sub_max = rec_max(arr, n - 1);
    if (arr[n - 1] > sub_max) return arr[n - 1];
    return sub_max;
}

// Recursive min of array
int rec_min(int *arr, int n) {
    if (n == 1) return arr[0];
    int sub_min = rec_min(arr, n - 1);
    if (arr[n - 1] < sub_min) return arr[n - 1];
    return sub_min;
}

// Helper to print hanoi moves with spaces
void hanoi_print(int n, char from, char to, char aux, int *first) {
    if (n == 0) return;
    hanoi_print(n - 1, from, aux, to, first);
    if (*first == 0) printf(" ");
    printf("%c->%c", from, to);
    *first = 0;
    hanoi_print(n - 1, aux, to, from, first);
}

int main() {
    printf("=== Deep Recursion Tests ===\n");

    // Tower of Hanoi - small cases
    int first;

    printf("hanoi_1: ");
    first = 1;
    hanoi_print(1, 'A', 'C', 'B', &first);
    printf("\n");

    printf("hanoi_2: ");
    first = 1;
    hanoi_print(2, 'A', 'C', 'B', &first);
    printf("\n");

    printf("hanoi_3: ");
    first = 1;
    hanoi_print(3, 'A', 'C', 'B', &first);
    printf("\n");

    // Hanoi move counts
    printf("hanoi_count_3: %d\n", hanoi_count(3));
    printf("hanoi_count_4: %d\n", hanoi_count(4));

    // Binary search - sorted array
    int sorted[10];
    sorted[0] = 2; sorted[1] = 5; sorted[2] = 8; sorted[3] = 12; sorted[4] = 15;
    sorted[5] = 22; sorted[6] = 30; sorted[7] = 35; sorted[8] = 40; sorted[9] = 50;

    printf("bsearch_found: index=%d\n", bsearch_rec(sorted, 0, 9, 15));
    printf("bsearch_first: index=%d\n", bsearch_rec(sorted, 0, 9, 2));
    printf("bsearch_last: index=%d\n", bsearch_rec(sorted, 0, 9, 50));
    printf("bsearch_notfound: index=%d\n", bsearch_rec(sorted, 0, 9, 99));

    // Binary search - single element
    int single[1];
    single[0] = 42;
    printf("bsearch_single: index=%d\n", bsearch_rec(single, 0, 0, 42));

    // Recursive power
    printf("power_2_0: %d\n", rec_power(2, 0));
    printf("power_2_10: %d\n", rec_power(2, 10));
    printf("power_3_5: %d\n", rec_power(3, 5));

    // Fast power (exponentiation by squaring)
    printf("power_fast_2_10: %d\n", fast_power(2, 10));
    printf("power_fast_3_7: %d\n", fast_power(3, 7));
    printf("power_fast_5_4: %d\n", fast_power(5, 4));

    // Mutual recursion
    printf("is_even_0: %s\n", mutual_is_even(0) ? "yes" : "no");
    printf("is_odd_1: %s\n", mutual_is_odd(1) ? "yes" : "no");
    printf("is_even_10: %s\n", mutual_is_even(10) ? "yes" : "no");
    printf("is_odd_7: %s\n", mutual_is_odd(7) ? "yes" : "no");
    printf("is_even_3: %s\n", mutual_is_even(3) ? "yes" : "no");

    // Digit sum
    printf("sum_digits_12345: %d\n", sum_digits(12345));
    printf("sum_digits_9999: %d\n", sum_digits(9999));
    printf("sum_digits_0: %d\n", sum_digits(0));

    // Digit count
    printf("count_digits_12345: %d\n", count_digits(12345));
    printf("count_digits_1: %d\n", count_digits(1));
    printf("count_digits_1000000: %d\n", count_digits(1000000));

    // Number reversal
    printf("reverse_num_12345: %d\n", reverse_num(12345));
    printf("reverse_num_1000: %d\n", reverse_num(1000));

    // Ackermann function (small values)
    printf("ackermann_0_0: %d\n", ackermann(0, 0));
    printf("ackermann_1_1: %d\n", ackermann(1, 1));
    printf("ackermann_2_2: %d\n", ackermann(2, 2));
    printf("ackermann_3_2: %d\n", ackermann(3, 2));

    // Deep recursion sum 1..10 = 55
    printf("flatten_sum_depth3: %d\n", rec_sum(10));

    // Recursive GCD
    printf("rec_gcd_48_18: %d\n", rec_gcd(48, 18));
    printf("rec_gcd_100_0: %d\n", rec_gcd(100, 0));

    // Recursive max/min
    int data[6];
    data[0] = 10; data[1] = 50; data[2] = 30; data[3] = 5; data[4] = 40; data[5] = 20;
    printf("rec_max: %d\n", rec_max(data, 6));
    printf("rec_min: %d\n", rec_min(data, 6));

    printf("=== All Deep Recursion Tests Passed ===\n");
    return 0;
}
