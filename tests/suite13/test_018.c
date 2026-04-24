int printf(const char *fmt, ...);

// Radix sort (LSD - Least Significant Digit first, base 10)
// Sorts non-negative integers by processing digits from right to left

int get_max(int *arr, int n) {
    int max = arr[0];
    int i;
    for (i = 1; i < n; i = i + 1) {
        if (arr[i] > max) max = arr[i];
    }
    return max;
}

int count_digits(int num) {
    if (num == 0) return 1;
    int count = 0;
    while (num > 0) {
        count = count + 1;
        num = num / 10;
    }
    return count;
}

// Counting sort by a specific digit position
// exp = 1 for ones, 10 for tens, 100 for hundreds, etc.
void counting_sort_by_digit(int *arr, int n, int exp, int *output, int *count) {
    int i;
    // Clear count array (0-9 digits)
    for (i = 0; i < 10; i = i + 1) {
        count[i] = 0;
    }
    // Count occurrences of each digit at position
    for (i = 0; i < n; i = i + 1) {
        int digit = (arr[i] / exp) % 10;
        count[digit] = count[digit] + 1;
    }
    // Prefix sums
    for (i = 1; i < 10; i = i + 1) {
        count[i] = count[i] + count[i - 1];
    }
    // Build output (stable, backward)
    for (i = n - 1; i >= 0; i = i - 1) {
        int digit = (arr[i] / exp) % 10;
        count[digit] = count[digit] - 1;
        output[count[digit]] = arr[i];
    }
    // Copy back
    for (i = 0; i < n; i = i + 1) {
        arr[i] = output[i];
    }
}

int radix_sort(int *arr, int n, int *output, int *count) {
    int max = get_max(arr, n);
    int passes = 0;
    int exp = 1;
    while (max / exp > 0) {
        counting_sort_by_digit(arr, n, exp, output, count);
        exp = exp * 10;
        passes = passes + 1;
    }
    return passes;
}

int is_sorted(int *arr, int n) {
    int i;
    for (i = 0; i < n - 1; i = i + 1) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

// Extract specific digit
int get_digit(int num, int pos) {
    int i;
    for (i = 0; i < pos; i = i + 1) {
        num = num / 10;
    }
    return num % 10;
}

// Sum of digits
int digit_sum(int num) {
    int sum = 0;
    while (num > 0) {
        sum = sum + num % 10;
        num = num / 10;
    }
    return sum;
}

int main() {
    int output[20];
    int count[10];

    // Test 1: Basic radix sort
    int a1[8];
    a1[0] = 170; a1[1] = 45; a1[2] = 75; a1[3] = 90;
    a1[4] = 802; a1[5] = 24; a1[6] = 2; a1[7] = 66;
    int passes = radix_sort(a1, 8, output, count);
    printf("sorted: %d %d %d %d %d %d %d %d\n",
           a1[0], a1[1], a1[2], a1[3], a1[4], a1[5], a1[6], a1[7]);
    // EXPECT: sorted: 2 24 45 66 75 90 170 802
    printf("passes: %d\n", passes);
    // EXPECT: passes: 3

    // Test 2: All single digit
    int a2[5];
    a2[0] = 9; a2[1] = 1; a2[2] = 5; a2[3] = 3; a2[4] = 7;
    passes = radix_sort(a2, 5, output, count);
    printf("single digit: %d %d %d %d %d passes: %d\n",
           a2[0], a2[1], a2[2], a2[3], a2[4], passes);
    // EXPECT: single digit: 1 3 5 7 9 passes: 1

    // Test 3: Digit extraction
    printf("digit 170 pos0: %d\n", get_digit(170, 0));
    // EXPECT: digit 170 pos0: 0
    printf("digit 170 pos1: %d\n", get_digit(170, 1));
    // EXPECT: digit 170 pos1: 7
    printf("digit 170 pos2: %d\n", get_digit(170, 2));
    // EXPECT: digit 170 pos2: 1
    printf("digit 802 pos0: %d\n", get_digit(802, 0));
    // EXPECT: digit 802 pos0: 2

    // Test 4: Count digits
    printf("digits in 0: %d\n", count_digits(0));
    // EXPECT: digits in 0: 1
    printf("digits in 9: %d\n", count_digits(9));
    // EXPECT: digits in 9: 1
    printf("digits in 100: %d\n", count_digits(100));
    // EXPECT: digits in 100: 3
    printf("digits in 9999: %d\n", count_digits(9999));
    // EXPECT: digits in 9999: 4

    // Test 5: Larger numbers
    int a5[6];
    a5[0] = 5432; a5[1] = 1234; a5[2] = 9876; a5[3] = 4321;
    a5[4] = 6789; a5[5] = 1111;
    passes = radix_sort(a5, 6, output, count);
    printf("big: %d %d %d %d %d %d\n",
           a5[0], a5[1], a5[2], a5[3], a5[4], a5[5]);
    // EXPECT: big: 1111 1234 4321 5432 6789 9876
    printf("big passes: %d\n", passes);
    // EXPECT: big passes: 4

    // Test 6: Digit sums
    printf("dsum 123: %d\n", digit_sum(123));
    // EXPECT: dsum 123: 6
    printf("dsum 9999: %d\n", digit_sum(9999));
    // EXPECT: dsum 9999: 36
    printf("dsum 100: %d\n", digit_sum(100));
    // EXPECT: dsum 100: 1

    // Test 7: With zeros
    int a7[6];
    a7[0] = 10; a7[1] = 0; a7[2] = 100; a7[3] = 1; a7[4] = 1000; a7[5] = 0;
    passes = radix_sort(a7, 6, output, count);
    printf("zeros: %d %d %d %d %d %d\n",
           a7[0], a7[1], a7[2], a7[3], a7[4], a7[5]);
    // EXPECT: zeros: 0 0 1 10 100 1000
    printf("zeros passes: %d\n", passes);
    // EXPECT: zeros passes: 4

    // Test 8: Sorted check
    printf("is_sorted: %d\n", is_sorted(a7, 6));
    // EXPECT: is_sorted: 1

    // Test 9: Duplicates
    int a9[6];
    a9[0] = 55; a9[1] = 33; a9[2] = 55; a9[3] = 11; a9[4] = 33; a9[5] = 11;
    radix_sort(a9, 6, output, count);
    printf("dupes: %d %d %d %d %d %d\n",
           a9[0], a9[1], a9[2], a9[3], a9[4], a9[5]);
    // EXPECT: dupes: 11 11 33 33 55 55

    printf("done\n");
    // EXPECT: done

    return 0;
}
