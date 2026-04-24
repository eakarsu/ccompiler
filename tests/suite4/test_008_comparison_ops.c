int printf(const char *fmt, ...);

// Test comparison operators: ==, !=, <, >, <=, >=
// with ints, chars, negative values, zero, and boundary cases.

int eq(int a, int b) { return a == b; }
int ne(int a, int b) { return a != b; }
int lt(int a, int b) { return a < b; }
int gt(int a, int b) { return a > b; }
int le(int a, int b) { return a <= b; }
int ge(int a, int b) { return a >= b; }

int clamp(int val, int lo, int hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

int in_range(int val, int lo, int hi) {
    return val >= lo && val <= hi;
}

int median_of_three(int a, int b, int c) {
    if ((a >= b && a <= c) || (a <= b && a >= c)) return a;
    if ((b >= a && b <= c) || (b <= a && b >= c)) return b;
    return c;
}

int compare(int a, int b) {
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

int sorted3(int a, int b, int c) {
    return a <= b && b <= c;
}

int all_equal(int a, int b, int c) {
    return a == b && b == c;
}

int max3(int a, int b, int c) {
    int m = a;
    if (b > m) m = b;
    if (c > m) m = c;
    return m;
}

int min3(int a, int b, int c) {
    int m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

int count_positives(int *arr, int len) {
    int count = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (arr[i] > 0) count = count + 1;
    }
    return count;
}

int count_negatives(int *arr, int len) {
    int count = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (arr[i] < 0) count = count + 1;
    }
    return count;
}

int count_zeros(int *arr, int len) {
    int count = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        if (arr[i] == 0) count = count + 1;
    }
    return count;
}

int main(void) {
    // Equality
    // EXPECT: 5 == 5 = 1
    printf("%d == %d = %d\n", 5, 5, eq(5, 5));

    // EXPECT: 5 == 6 = 0
    printf("%d == %d = %d\n", 5, 6, eq(5, 6));

    // EXPECT: 0 == 0 = 1
    printf("%d == %d = %d\n", 0, 0, eq(0, 0));

    // EXPECT: -1 == -1 = 1
    printf("%d == %d = %d\n", -1, -1, eq(-1, -1));

    // Inequality
    // EXPECT: 5 != 6 = 1
    printf("%d != %d = %d\n", 5, 6, ne(5, 6));

    // EXPECT: 5 != 5 = 0
    printf("%d != %d = %d\n", 5, 5, ne(5, 5));

    // Less than
    // EXPECT: 3 < 5 = 1
    printf("%d < %d = %d\n", 3, 5, lt(3, 5));

    // EXPECT: 5 < 3 = 0
    printf("%d < %d = %d\n", 5, 3, lt(5, 3));

    // EXPECT: 5 < 5 = 0
    printf("%d < %d = %d\n", 5, 5, lt(5, 5));

    // EXPECT: -5 < 0 = 1
    printf("%d < %d = %d\n", -5, 0, lt(-5, 0));

    // EXPECT: -10 < -5 = 1
    printf("%d < %d = %d\n", -10, -5, lt(-10, -5));

    // Greater than
    // EXPECT: 10 > 5 = 1
    printf("%d > %d = %d\n", 10, 5, gt(10, 5));

    // EXPECT: -1 > -10 = 1
    printf("%d > %d = %d\n", -1, -10, gt(-1, -10));

    // Less than or equal
    // EXPECT: 5 <= 5 = 1
    printf("%d <= %d = %d\n", 5, 5, le(5, 5));

    // EXPECT: 4 <= 5 = 1
    printf("%d <= %d = %d\n", 4, 5, le(4, 5));

    // EXPECT: 6 <= 5 = 0
    printf("%d <= %d = %d\n", 6, 5, le(6, 5));

    // Greater than or equal
    // EXPECT: 5 >= 5 = 1
    printf("%d >= %d = %d\n", 5, 5, ge(5, 5));

    // EXPECT: 6 >= 5 = 1
    printf("%d >= %d = %d\n", 6, 5, ge(6, 5));

    // EXPECT: 4 >= 5 = 0
    printf("%d >= %d = %d\n", 4, 5, ge(4, 5));

    // Clamp
    // EXPECT: clamp(15, 0, 10) = 10
    printf("clamp(%d, %d, %d) = %d\n", 15, 0, 10, clamp(15, 0, 10));

    // EXPECT: clamp(-5, 0, 10) = 0
    printf("clamp(%d, %d, %d) = %d\n", -5, 0, 10, clamp(-5, 0, 10));

    // EXPECT: clamp(5, 0, 10) = 5
    printf("clamp(%d, %d, %d) = %d\n", 5, 0, 10, clamp(5, 0, 10));

    // In range
    // EXPECT: in_range(5, 1, 10) = 1
    printf("in_range(%d, %d, %d) = %d\n", 5, 1, 10, in_range(5, 1, 10));

    // EXPECT: in_range(0, 1, 10) = 0
    printf("in_range(%d, %d, %d) = %d\n", 0, 1, 10, in_range(0, 1, 10));

    // Median
    // EXPECT: median(3, 1, 2) = 2
    printf("median(%d, %d, %d) = %d\n", 3, 1, 2, median_of_three(3, 1, 2));

    // EXPECT: median(5, 5, 5) = 5
    printf("median(%d, %d, %d) = %d\n", 5, 5, 5, median_of_three(5, 5, 5));

    // Compare
    // EXPECT: compare(1, 2) = -1
    printf("compare(%d, %d) = %d\n", 1, 2, compare(1, 2));

    // EXPECT: compare(2, 1) = 1
    printf("compare(%d, %d) = %d\n", 2, 1, compare(2, 1));

    // EXPECT: compare(3, 3) = 0
    printf("compare(%d, %d) = %d\n", 3, 3, compare(3, 3));

    // Sorted check
    // EXPECT: sorted(1,2,3) = 1
    printf("sorted(%d,%d,%d) = %d\n", 1, 2, 3, sorted3(1, 2, 3));

    // EXPECT: sorted(3,2,1) = 0
    printf("sorted(%d,%d,%d) = %d\n", 3, 2, 1, sorted3(3, 2, 1));

    // All equal
    // EXPECT: all_eq(7,7,7) = 1
    printf("all_eq(%d,%d,%d) = %d\n", 7, 7, 7, all_equal(7, 7, 7));

    // EXPECT: all_eq(7,7,8) = 0
    printf("all_eq(%d,%d,%d) = %d\n", 7, 7, 8, all_equal(7, 7, 8));

    // Max/min of three
    // EXPECT: max3(3,-1,7) = 7
    printf("max3(%d,%d,%d) = %d\n", 3, -1, 7, max3(3, -1, 7));

    // EXPECT: min3(3,-1,7) = -1
    printf("min3(%d,%d,%d) = %d\n", 3, -1, 7, min3(3, -1, 7));

    // Array counting
    int data[7];
    data[0] = -3;
    data[1] = 0;
    data[2] = 5;
    data[3] = -1;
    data[4] = 0;
    data[5] = 8;
    data[6] = -2;

    // EXPECT: positives = 2
    printf("positives = %d\n", count_positives(data, 7));

    // EXPECT: negatives = 3
    printf("negatives = %d\n", count_negatives(data, 7));

    // EXPECT: zeros = 2
    printf("zeros = %d\n", count_zeros(data, 7));

    // Char comparisons
    char ca = 65;  // 'A'
    char cz = 90;  // 'Z'
    // EXPECT: A < Z = 1
    printf("A < Z = %d\n", ca < cz);

    // EXPECT: Z > A = 1
    printf("Z > A = %d\n", cz > ca);

    return 0;
}
