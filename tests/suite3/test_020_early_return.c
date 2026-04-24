int printf(const char *fmt, ...);

/* test_020_early_return.c
   Tests: functions with multiple return points, return in if/else,
   return in switch, return in loops, return based on complex conditions.
*/

/* Return in if/else: every branch returns */
int classify_triangle(int a, int b, int c) {
    if (a <= 0 || b <= 0 || c <= 0) return -1; /* invalid */
    if (a + b <= c || a + c <= b || b + c <= a) return 0; /* not a triangle */
    if (a == b && b == c) return 3; /* equilateral */
    if (a == b || b == c || a == c) return 2; /* isosceles */
    return 1; /* scalene */
}

/* Return in switch */
const char *season_name_idx(int month) {
    switch (month) {
        case 12: case 1: case 2: return (const char *)"winter";
        case 3: case 4: case 5: return (const char *)"spring";
        case 6: case 7: case 8: return (const char *)"summer";
        case 9: case 10: case 11: return (const char *)"autumn";
        default: return (const char *)"unknown";
    }
}

/* Return in loop: linear search */
int linear_search(int arr[], int n, int target) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) return i;
    }
    return -1;
}

/* Return based on complex condition */
int is_valid_date(int y, int m, int d) {
    if (y < 1 || m < 1 || m > 12 || d < 1) return 0;
    int days_in_month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    /* leap year check */
    if (m == 2) {
        int leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
        if (leap && d <= 29) return 1;
        if (!leap && d <= 28) return 1;
        return 0;
    }
    if (d > days_in_month[m]) return 0;
    return 1;
}

/* Early return in nested loops */
int matrix_has_zero(int m[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (m[i][j] == 0) return 1;
        }
    }
    return 0;
}

/* Multiple returns based on recursive-like iterative logic */
int nth_triangular(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    int s = 0;
    for (int i = 1; i <= n; i++) s += i;
    return s;
}
/* nth_triangular(1)=1, nth_triangular(5)=15 */

/* Return inside do-while */
int find_factor(int n) {
    if (n <= 1) return n;
    int i = 2;
    do {
        if (n % i == 0) return i;
        i++;
    } while (i * i <= n);
    return n; /* prime */
}
/* find_factor(12) = 2 */
/* find_factor(7) = 7 */
/* find_factor(9) = 3 */

/* Return in while loop */
int string_length(const char *s) {
    int len = 0;
    while (1) {
        if (s[len] == '\0') return len;
        len++;
    }
}

/* Return based on multiple conditions */
int classify_number(int n) {
    if (n < 0) return -2;
    if (n == 0) return 0;
    if (n % 2 == 0 && n % 3 == 0) return 6; /* div by 6 */
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    return 1; /* odd, not div by 3 */
}

/* Multiple return in switch + loops combined */
int process(int arr[], int n, int mode) {
    switch (mode) {
        case 0: { /* sum */
            int s = 0;
            for (int i = 0; i < n; i++) s += arr[i];
            return s;
        }
        case 1: { /* max */
            if (n == 0) return -1;
            int m = arr[0];
            for (int i = 1; i < n; i++) if (arr[i] > m) m = arr[i];
            return m;
        }
        case 2: { /* count positive */
            int c = 0;
            for (int i = 0; i < n; i++) if (arr[i] > 0) c++;
            return c;
        }
        default: return -999;
    }
}

/* Return after complex guard clauses (guard pattern) */
int compute(int x, int y, int z) {
    if (x == 0) return 0;
    if (y == 0) return x;
    if (z == 0) return x + y;
    if (x < 0 && y < 0 && z < 0) return -1;
    return x * y + z;
}

int main(void) {
    printf("%d\n", classify_triangle(3, 4, 5));   // EXPECT: 1
    printf("%d\n", classify_triangle(5, 5, 5));   // EXPECT: 3
    printf("%d\n", classify_triangle(5, 5, 3));   // EXPECT: 2
    printf("%d\n", classify_triangle(1, 2, 10));  // EXPECT: 0
    printf("%d\n", classify_triangle(-1, 2, 3));  // EXPECT: -1

    /* season names returned as pointers, just test lengths or first char */
    const char *s = season_name_idx(1);
    printf("%d\n", string_length(s));   // EXPECT: 6
    s = season_name_idx(7);
    printf("%d\n", string_length(s));   // EXPECT: 6
    s = season_name_idx(10);
    printf("%d\n", string_length(s));   // EXPECT: 6
    s = season_name_idx(4);
    printf("%d\n", string_length(s));   // EXPECT: 6
    s = season_name_idx(13);
    printf("%d\n", string_length(s));   // EXPECT: 7

    int arr[6] = {10, 3, 7, 5, 8, 1};
    printf("%d\n", linear_search(arr, 6, 7));   // EXPECT: 2
    printf("%d\n", linear_search(arr, 6, 99));  // EXPECT: -1
    printf("%d\n", linear_search(arr, 6, 10));  // EXPECT: 0

    printf("%d\n", is_valid_date(2024, 2, 29)); // EXPECT: 1
    printf("%d\n", is_valid_date(2023, 2, 29)); // EXPECT: 0
    printf("%d\n", is_valid_date(2023, 4, 31)); // EXPECT: 0
    printf("%d\n", is_valid_date(2023, 12, 31)); // EXPECT: 1

    int mat1[3][3] = {{1,2,3},{4,0,6},{7,8,9}};
    printf("%d\n", matrix_has_zero(mat1)); // EXPECT: 1
    int mat2[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
    printf("%d\n", matrix_has_zero(mat2)); // EXPECT: 0

    printf("%d\n", nth_triangular(0)); // EXPECT: 0
    printf("%d\n", nth_triangular(1)); // EXPECT: 1
    printf("%d\n", nth_triangular(5)); // EXPECT: 15

    printf("%d\n", find_factor(12)); // EXPECT: 2
    printf("%d\n", find_factor(7));  // EXPECT: 7
    printf("%d\n", find_factor(9));  // EXPECT: 3

    printf("%d\n", classify_number(-5)); // EXPECT: -2
    printf("%d\n", classify_number(0));  // EXPECT: 0
    printf("%d\n", classify_number(12)); // EXPECT: 6
    printf("%d\n", classify_number(4));  // EXPECT: 2
    printf("%d\n", classify_number(9));  // EXPECT: 3
    printf("%d\n", classify_number(7));  // EXPECT: 1

    int data[5] = {3, -1, 4, -2, 5};
    printf("%d\n", process(data, 5, 0)); // EXPECT: 9
    printf("%d\n", process(data, 5, 1)); // EXPECT: 5
    printf("%d\n", process(data, 5, 2)); // EXPECT: 3
    printf("%d\n", process(data, 5, 9)); // EXPECT: -999

    printf("%d\n", compute(0, 5, 5));    // EXPECT: 0
    printf("%d\n", compute(3, 0, 5));    // EXPECT: 3
    printf("%d\n", compute(3, 4, 0));    // EXPECT: 7
    printf("%d\n", compute(-1,-2,-3));   // EXPECT: -1
    printf("%d\n", compute(3, 4, 5));    // EXPECT: 17

    return 0;
}
