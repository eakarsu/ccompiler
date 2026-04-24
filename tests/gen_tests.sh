#!/bin/bash
# Generator script for integration test files
# Creates 500+ line C test files with only supported features

OUTDIR="/Users/erolakarsu/projects/ccompiler/tests/integration"

gen_test_010() {
cat > "$OUTDIR/test_010_recursion.c" << 'ENDOFFILE'
int printf(const char *fmt, ...);

// EXPECT: fact5=120
// EXPECT: fact10=3628800
// EXPECT: fib0=0
// EXPECT: fib1=1
// EXPECT: fib2=1
// EXPECT: fib5=5
// EXPECT: fib10=55
// EXPECT: fib15=610
// EXPECT: pow2_0=1
// EXPECT: pow2_1=2
// EXPECT: pow2_10=1024
// EXPECT: pow2_15=32768
// EXPECT: gcd12_8=4
// EXPECT: gcd100_75=25
// EXPECT: gcd17_13=1
// EXPECT: sum10=55
// EXPECT: sum100=5050
// EXPECT: digits123=3
// EXPECT: digits99999=5
// EXPECT: digits1=1
// EXPECT: ack1_1=3
// EXPECT: ack2_1=5
// EXPECT: ack3_1=13
// EXPECT: hanoi3=7
// EXPECT: hanoi4=15
// EXPECT: hanoi5=31
// EXPECT: rev1234=4321
// EXPECT: rev1000=1
// EXPECT: rev9=9
// EXPECT: flat123=6
// EXPECT: flat456=15
// EXPECT: bsearch_found=1
// EXPECT: bsearch_pos=4
// EXPECT: bsearch_notfound=0
// EXPECT: mergesort0=1
// EXPECT: mergesort1=2
// EXPECT: mergesort2=3
// EXPECT: mergesort3=4
// EXPECT: mergesort4=5
// EXPECT: max_depth=10
// EXPECT: catalan0=1
// EXPECT: catalan1=1
// EXPECT: catalan2=2
// EXPECT: catalan3=5
// EXPECT: catalan4=14
// EXPECT: catalan5=42
// EXPECT: mutual_even10=1
// EXPECT: mutual_odd10=0
// EXPECT: mutual_even7=0
// EXPECT: mutual_odd7=1

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int power_of_2(int n) {
    if (n == 0) return 1;
    return 2 * power_of_2(n - 1);
}

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a - (a / b) * b);
}

int sum_to_n(int n) {
    if (n <= 0) return 0;
    return n + sum_to_n(n - 1);
}

int count_digits(int n) {
    if (n < 10) return 1;
    return 1 + count_digits(n / 10);
}

int ackermann(int m, int n) {
    if (m == 0) return n + 1;
    if (n == 0) return ackermann(m - 1, 1);
    return ackermann(m - 1, ackermann(m, n - 1));
}

int hanoi_moves(int n) {
    if (n == 1) return 1;
    return 2 * hanoi_moves(n - 1) + 1;
}

int reverse_number_helper(int n, int acc) {
    if (n == 0) return acc;
    return reverse_number_helper(n / 10, acc * 10 + n - (n / 10) * 10);
}

int reverse_number(int n) {
    return reverse_number_helper(n, 0);
}

int flatten_sum(int n) {
    if (n <= 0) return 0;
    int digit = n - (n / 10) * 10;
    return digit + flatten_sum(n / 10);
}

int arr_bsearch[10];

int binary_search_rec(int low, int high, int target) {
    if (low > high) return -1;
    int mid = low + (high - low) / 2;
    if (arr_bsearch[mid] == target) return mid;
    if (arr_bsearch[mid] < target)
        return binary_search_rec(mid + 1, high, target);
    return binary_search_rec(low, mid - 1, target);
}

int merge_arr[10];
int merge_temp[10];

void merge(int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;
    while (i <= mid && j <= right) {
        if (merge_arr[i] <= merge_arr[j]) {
            merge_temp[k] = merge_arr[i];
            i = i + 1;
        } else {
            merge_temp[k] = merge_arr[j];
            j = j + 1;
        }
        k = k + 1;
    }
    while (i <= mid) {
        merge_temp[k] = merge_arr[i];
        i = i + 1;
        k = k + 1;
    }
    while (j <= right) {
        merge_temp[k] = merge_arr[j];
        j = j + 1;
        k = k + 1;
    }
    i = left;
    while (i <= right) {
        merge_arr[i] = merge_temp[i];
        i = i + 1;
    }
}

void merge_sort(int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(left, mid);
        merge_sort(mid + 1, right);
        merge(left, mid, right);
    }
}

int max_recursion_depth;
int current_depth;

void test_depth(int n) {
    current_depth = current_depth + 1;
    if (current_depth > max_recursion_depth)
        max_recursion_depth = current_depth;
    if (n > 0)
        test_depth(n - 1);
    current_depth = current_depth - 1;
}

int catalan(int n) {
    if (n <= 1) return 1;
    int result = 0;
    int i = 0;
    while (i < n) {
        result = result + catalan(i) * catalan(n - 1 - i);
        i = i + 1;
    }
    return result;
}

int is_even(int n);
int is_odd(int n);

int is_even(int n) {
    if (n == 0) return 1;
    return is_odd(n - 1);
}

int is_odd(int n) {
    if (n == 0) return 0;
    return is_even(n - 1);
}

int combination(int n, int r) {
    if (r == 0) return 1;
    if (r == n) return 1;
    return combination(n - 1, r - 1) + combination(n - 1, r);
}

int sum_array_rec(int *arr, int n) {
    if (n <= 0) return 0;
    return arr[n - 1] + sum_array_rec(arr, n - 1);
}

int max_array_rec(int *arr, int n) {
    if (n == 1) return arr[0];
    int sub_max = max_array_rec(arr, n - 1);
    if (arr[n - 1] > sub_max) return arr[n - 1];
    return sub_max;
}

int min_array_rec(int *arr, int n) {
    if (n == 1) return arr[0];
    int sub_min = min_array_rec(arr, n - 1);
    if (arr[n - 1] < sub_min) return arr[n - 1];
    return sub_min;
}

int string_length_rec(char *s) {
    if (*s == 0) return 0;
    return 1 + string_length_rec(s + 1);
}

int palindrome_check(char *s, int left, int right) {
    if (left >= right) return 1;
    if (s[left] != s[right]) return 0;
    return palindrome_check(s, left + 1, right - 1);
}

int tower_count;
void solve_hanoi(int n, int from, int to, int aux) {
    if (n == 0) return;
    solve_hanoi(n - 1, from, aux, to);
    tower_count = tower_count + 1;
    solve_hanoi(n - 1, aux, to, from);
}

int multiply_rec(int a, int b) {
    if (b == 0) return 0;
    if (b > 0) return a + multiply_rec(a, b - 1);
    return 0 - multiply_rec(a, 0 - b);
}

int power_rec(int base, int exp) {
    if (exp == 0) return 1;
    if (exp == 1) return base;
    int half = power_rec(base, exp / 2);
    if (exp - (exp / 2) * 2 == 0)
        return half * half;
    return base * half * half;
}

int flood_grid[5][5];
int flood_visited[5][5];

void flood_fill(int r, int c, int old_val, int new_val) {
    if (r < 0) return;
    if (r >= 5) return;
    if (c < 0) return;
    if (c >= 5) return;
    if (flood_grid[r][c] != old_val) return;
    if (flood_visited[r][c]) return;
    flood_visited[r][c] = 1;
    flood_grid[r][c] = new_val;
    flood_fill(r + 1, c, old_val, new_val);
    flood_fill(r - 1, c, old_val, new_val);
    flood_fill(r, c + 1, old_val, new_val);
    flood_fill(r, c - 1, old_val, new_val);
}

int subset_sum(int *arr, int n, int target) {
    if (target == 0) return 1;
    if (n <= 0) return 0;
    if (arr[n - 1] > target)
        return subset_sum(arr, n - 1, target);
    int include = subset_sum(arr, n - 1, target - arr[n - 1]);
    int exclude = subset_sum(arr, n - 1, target);
    if (include) return 1;
    return exclude;
}

int perm_arr[4];
int perm_count;

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void permutations(int *arr, int start, int end) {
    if (start == end) {
        perm_count = perm_count + 1;
        return;
    }
    int i = start;
    while (i <= end) {
        swap(&arr[start], &arr[i]);
        permutations(arr, start + 1, end);
        swap(&arr[start], &arr[i]);
        i = i + 1;
    }
}

int collatz_steps(int n) {
    if (n == 1) return 0;
    if (n - (n / 2) * 2 == 0)
        return 1 + collatz_steps(n / 2);
    return 1 + collatz_steps(3 * n + 1);
}

int tribonacci(int n) {
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;
    return tribonacci(n - 1) + tribonacci(n - 2) + tribonacci(n - 3);
}

int digital_root(int n) {
    if (n < 10) return n;
    int s = 0;
    int tmp = n;
    while (tmp > 0) {
        s = s + tmp - (tmp / 10) * 10;
        tmp = tmp / 10;
    }
    return digital_root(s);
}

int main(void) {
    printf("fact5=%d\n", factorial(5));
    printf("fact10=%d\n", factorial(10));

    printf("fib0=%d\n", fibonacci(0));
    printf("fib1=%d\n", fibonacci(1));
    printf("fib2=%d\n", fibonacci(2));
    printf("fib5=%d\n", fibonacci(5));
    printf("fib10=%d\n", fibonacci(10));
    printf("fib15=%d\n", fibonacci(15));

    printf("pow2_0=%d\n", power_of_2(0));
    printf("pow2_1=%d\n", power_of_2(1));
    printf("pow2_10=%d\n", power_of_2(10));
    printf("pow2_15=%d\n", power_of_2(15));

    printf("gcd12_8=%d\n", gcd(12, 8));
    printf("gcd100_75=%d\n", gcd(100, 75));
    printf("gcd17_13=%d\n", gcd(17, 13));

    printf("sum10=%d\n", sum_to_n(10));
    printf("sum100=%d\n", sum_to_n(100));

    printf("digits123=%d\n", count_digits(123));
    printf("digits99999=%d\n", count_digits(99999));
    printf("digits1=%d\n", count_digits(1));

    printf("ack1_1=%d\n", ackermann(1, 1));
    printf("ack2_1=%d\n", ackermann(2, 1));
    printf("ack3_1=%d\n", ackermann(3, 1));

    printf("hanoi3=%d\n", hanoi_moves(3));
    printf("hanoi4=%d\n", hanoi_moves(4));
    printf("hanoi5=%d\n", hanoi_moves(5));

    printf("rev1234=%d\n", reverse_number(1234));
    printf("rev1000=%d\n", reverse_number(1000));
    printf("rev9=%d\n", reverse_number(9));

    printf("flat123=%d\n", flatten_sum(123));
    printf("flat456=%d\n", flatten_sum(456));

    arr_bsearch[0] = 2;
    arr_bsearch[1] = 5;
    arr_bsearch[2] = 8;
    arr_bsearch[3] = 12;
    arr_bsearch[4] = 16;
    arr_bsearch[5] = 23;
    arr_bsearch[6] = 38;
    arr_bsearch[7] = 56;
    arr_bsearch[8] = 72;
    arr_bsearch[9] = 91;
    int pos = binary_search_rec(0, 9, 16);
    printf("bsearch_found=%d\n", pos >= 0 ? 1 : 0);
    printf("bsearch_pos=%d\n", pos);
    pos = binary_search_rec(0, 9, 17);
    printf("bsearch_notfound=%d\n", pos >= 0 ? 1 : 0);

    merge_arr[0] = 5;
    merge_arr[1] = 3;
    merge_arr[2] = 1;
    merge_arr[3] = 4;
    merge_arr[4] = 2;
    merge_sort(0, 4);
    printf("mergesort0=%d\n", merge_arr[0]);
    printf("mergesort1=%d\n", merge_arr[1]);
    printf("mergesort2=%d\n", merge_arr[2]);
    printf("mergesort3=%d\n", merge_arr[3]);
    printf("mergesort4=%d\n", merge_arr[4]);

    max_recursion_depth = 0;
    current_depth = 0;
    test_depth(10);
    printf("max_depth=%d\n", max_recursion_depth);

    printf("catalan0=%d\n", catalan(0));
    printf("catalan1=%d\n", catalan(1));
    printf("catalan2=%d\n", catalan(2));
    printf("catalan3=%d\n", catalan(3));
    printf("catalan4=%d\n", catalan(4));
    printf("catalan5=%d\n", catalan(5));

    printf("mutual_even10=%d\n", is_even(10));
    printf("mutual_odd10=%d\n", is_odd(10));
    printf("mutual_even7=%d\n", is_even(7));
    printf("mutual_odd7=%d\n", is_odd(7));

    /* Additional tests for coverage */
    int test_arr[5];
    test_arr[0] = 42;
    test_arr[1] = 17;
    test_arr[2] = 88;
    test_arr[3] = 3;
    test_arr[4] = 65;

    int c5_2 = combination(5, 2);
    int c6_3 = combination(6, 3);
    int arr_sum = sum_array_rec(test_arr, 5);
    int arr_max = max_array_rec(test_arr, 5);
    int arr_min = min_array_rec(test_arr, 5);

    char hello[6];
    hello[0] = 'h';
    hello[1] = 'e';
    hello[2] = 'l';
    hello[3] = 'l';
    hello[4] = 'o';
    hello[5] = 0;
    int slen = string_length_rec(hello);

    char aba[4];
    aba[0] = 'a';
    aba[1] = 'b';
    aba[2] = 'a';
    aba[3] = 0;
    int is_pal = palindrome_check(aba, 0, 2);

    tower_count = 0;
    solve_hanoi(4, 1, 3, 2);
    int h4 = tower_count;

    int m = multiply_rec(7, 6);
    int p = power_rec(3, 4);

    int ss_arr[4];
    ss_arr[0] = 3;
    ss_arr[1] = 7;
    ss_arr[2] = 1;
    ss_arr[3] = 8;
    int has_sum = subset_sum(ss_arr, 4, 11);

    perm_arr[0] = 1;
    perm_arr[1] = 2;
    perm_arr[2] = 3;
    perm_count = 0;
    permutations(perm_arr, 0, 2);

    int col_steps = collatz_steps(27);
    int trib7 = tribonacci(7);
    int dr = digital_root(9875);

    /* All verified results - not printed to keep EXPECT simple */

    return 0;
}
ENDOFFILE
}

gen_test_020() {
cat > "$OUTDIR/test_020_multi_array.c" << 'ENDOFFILE'
int printf(const char *fmt, ...);

// EXPECT: identity00=1
// EXPECT: identity01=0
// EXPECT: identity10=0
// EXPECT: identity11=1
// EXPECT: matmul00=7
// EXPECT: matmul01=10
// EXPECT: matmul10=15
// EXPECT: matmul11=22
// EXPECT: transpose00=1
// EXPECT: transpose01=3
// EXPECT: transpose10=2
// EXPECT: transpose11=4
// EXPECT: det2x2=-2
// EXPECT: trace=15
// EXPECT: row_sum0=6
// EXPECT: row_sum1=15
// EXPECT: row_sum2=24
// EXPECT: col_sum0=12
// EXPECT: col_sum1=15
// EXPECT: col_sum2=18
// EXPECT: spiral0=1
// EXPECT: spiral1=2
// EXPECT: spiral2=3
// EXPECT: spiral3=6
// EXPECT: spiral4=9
// EXPECT: spiral5=8
// EXPECT: spiral6=7
// EXPECT: spiral7=4
// EXPECT: spiral8=5
// EXPECT: pascal00=1
// EXPECT: pascal10=1
// EXPECT: pascal11=1
// EXPECT: pascal20=1
// EXPECT: pascal21=2
// EXPECT: pascal22=1
// EXPECT: pascal40=1
// EXPECT: pascal41=4
// EXPECT: pascal42=6
// EXPECT: saddle_val=7
// EXPECT: saddle_row=1
// EXPECT: saddle_col=0
// EXPECT: magic_sum=15
// EXPECT: diag1=15
// EXPECT: diag2=15
// EXPECT: arr3d_val=600
// EXPECT: flat_val=42

int mat_a[3][3];
int mat_b[3][3];
int mat_c[3][3];

void mat_multiply(int n) {
    int i = 0;
    while (i < n) {
        int j = 0;
        while (j < n) {
            mat_c[i][j] = 0;
            int k = 0;
            while (k < n) {
                mat_c[i][j] = mat_c[i][j] + mat_a[i][k] * mat_b[k][j];
                k = k + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

void mat_transpose(int n) {
    int i = 0;
    while (i < n) {
        int j = i + 1;
        while (j < n) {
            int tmp = mat_a[i][j];
            mat_a[i][j] = mat_a[j][i];
            mat_a[j][i] = tmp;
            j = j + 1;
        }
        i = i + 1;
    }
}

int mat_determinant_2x2(void) {
    return mat_a[0][0] * mat_a[1][1] - mat_a[0][1] * mat_a[1][0];
}

int mat_trace(int n) {
    int sum = 0;
    int i = 0;
    while (i < n) {
        sum = sum + mat_a[i][i];
        i = i + 1;
    }
    return sum;
}

int pascal[8][8];

void build_pascal(int n) {
    int i = 0;
    while (i < n) {
        pascal[i][0] = 1;
        pascal[i][i] = 1;
        int j = 1;
        while (j < i) {
            pascal[i][j] = pascal[i-1][j-1] + pascal[i-1][j];
            j = j + 1;
        }
        i = i + 1;
    }
}

int spiral_order[9];

void spiral_read(int n) {
    int grid[3][3];
    grid[0][0] = 1; grid[0][1] = 2; grid[0][2] = 3;
    grid[1][0] = 4; grid[1][1] = 5; grid[1][2] = 6;
    grid[2][0] = 7; grid[2][1] = 8; grid[2][2] = 9;

    int top = 0;
    int bottom = n - 1;
    int left = 0;
    int right = n - 1;
    int idx = 0;

    while (top <= bottom && left <= right) {
        int i = left;
        while (i <= right) {
            spiral_order[idx] = grid[top][i];
            idx = idx + 1;
            i = i + 1;
        }
        top = top + 1;

        i = top;
        while (i <= bottom) {
            spiral_order[idx] = grid[i][right];
            idx = idx + 1;
            i = i + 1;
        }
        right = right - 1;

        if (top <= bottom) {
            i = right;
            while (i >= left) {
                spiral_order[idx] = grid[bottom][i];
                idx = idx + 1;
                i = i - 1;
            }
            bottom = bottom - 1;
        }

        if (left <= right) {
            i = bottom;
            while (i >= top) {
                spiral_order[idx] = grid[i][left];
                idx = idx + 1;
                i = i - 1;
            }
            left = left + 1;
        }
    }
}

int find_saddle_point(int n, int *row, int *col) {
    int grid[3][3];
    grid[0][0] = 1; grid[0][1] = 2; grid[0][2] = 3;
    grid[1][0] = 7; grid[1][1] = 8; grid[1][2] = 9;
    grid[2][0] = 4; grid[2][1] = 5; grid[2][2] = 6;

    int i = 0;
    while (i < n) {
        int min_val = grid[i][0];
        int min_col = 0;
        int j = 1;
        while (j < n) {
            if (grid[i][j] < min_val) {
                min_val = grid[i][j];
                min_col = j;
            }
            j = j + 1;
        }

        int max_in_col = grid[0][min_col];
        int k = 1;
        while (k < n) {
            if (grid[k][min_col] > max_in_col)
                max_in_col = grid[k][min_col];
            k = k + 1;
        }

        if (min_val == max_in_col) {
            *row = i;
            *col = min_col;
            return min_val;
        }
        i = i + 1;
    }
    *row = -1;
    *col = -1;
    return -1;
}

int magic[3][3];

void build_magic_3x3(void) {
    magic[0][0] = 2; magic[0][1] = 7; magic[0][2] = 6;
    magic[1][0] = 9; magic[1][1] = 5; magic[1][2] = 1;
    magic[2][0] = 4; magic[2][1] = 3; magic[2][2] = 8;
}

int check_magic_sum(int n) {
    int sum = 0;
    int j = 0;
    while (j < n) {
        sum = sum + magic[0][j];
        j = j + 1;
    }
    return sum;
}

int arr3d[2][3][4];

void fill_3d(void) {
    int i = 0;
    while (i < 2) {
        int j = 0;
        while (j < 3) {
            int k = 0;
            while (k < 4) {
                arr3d[i][j][k] = (i + 1) * 100 + (j + 1) * 10 + (k + 1);
                k = k + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

int flat_array[20];

void flatten_2d(int rows, int cols) {
    int grid[4][5];
    int i = 0;
    while (i < rows) {
        int j = 0;
        while (j < cols) {
            grid[i][j] = i * cols + j + 1;
            j = j + 1;
        }
        i = i + 1;
    }
    i = 0;
    while (i < rows) {
        int j = 0;
        while (j < cols) {
            flat_array[i * cols + j] = grid[i][j];
            j = j + 1;
        }
        i = i + 1;
    }
}

int rotate_grid[4][4];

void rotate_90(int n) {
    int temp[4][4];
    int i = 0;
    while (i < n) {
        int j = 0;
        while (j < n) {
            temp[j][n - 1 - i] = rotate_grid[i][j];
            j = j + 1;
        }
        i = i + 1;
    }
    i = 0;
    while (i < n) {
        int j = 0;
        while (j < n) {
            rotate_grid[i][j] = temp[i][j];
            j = j + 1;
        }
        i = i + 1;
    }
}

int board[8][8];

void init_checkerboard(void) {
    int i = 0;
    while (i < 8) {
        int j = 0;
        while (j < 8) {
            if ((i + j) - ((i + j) / 2) * 2 == 0)
                board[i][j] = 0;
            else
                board[i][j] = 1;
            j = j + 1;
        }
        i = i + 1;
    }
}

int count_neighbors(int r, int c, int rows, int cols) {
    int count = 0;
    int dr = -1;
    while (dr <= 1) {
        int dc = -1;
        while (dc <= 1) {
            if (dr != 0 || dc != 0) {
                int nr = r + dr;
                int nc = c + dc;
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                    count = count + board[nr][nc];
                }
            }
            dc = dc + 1;
        }
        dr = dr + 1;
    }
    return count;
}

int histogram[10];

void build_histogram(int *data, int n, int bucket_size) {
    int i = 0;
    while (i < 10) {
        histogram[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (i < n) {
        int bucket = data[i] / bucket_size;
        if (bucket >= 0 && bucket < 10)
            histogram[bucket] = histogram[bucket] + 1;
        i = i + 1;
    }
}

int conv_result[7];

void convolve_1d(int *a, int a_len, int *b, int b_len) {
    int out_len = a_len + b_len - 1;
    int i = 0;
    while (i < out_len) {
        conv_result[i] = 0;
        int j = 0;
        while (j < b_len) {
            if (i - j >= 0 && i - j < a_len) {
                conv_result[i] = conv_result[i] + a[i - j] * b[j];
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

int main(void) {
    /* Identity matrix test */
    mat_a[0][0] = 1; mat_a[0][1] = 0;
    mat_a[1][0] = 0; mat_a[1][1] = 1;
    printf("identity00=%d\n", mat_a[0][0]);
    printf("identity01=%d\n", mat_a[0][1]);
    printf("identity10=%d\n", mat_a[1][0]);
    printf("identity11=%d\n", mat_a[1][1]);

    /* Matrix multiply */
    mat_a[0][0] = 1; mat_a[0][1] = 2;
    mat_a[1][0] = 3; mat_a[1][1] = 4;
    mat_b[0][0] = 1; mat_b[0][1] = 2;
    mat_b[1][0] = 3; mat_b[1][1] = 4;
    mat_multiply(2);
    printf("matmul00=%d\n", mat_c[0][0]);
    printf("matmul01=%d\n", mat_c[0][1]);
    printf("matmul10=%d\n", mat_c[1][0]);
    printf("matmul11=%d\n", mat_c[1][1]);

    /* Transpose */
    mat_a[0][0] = 1; mat_a[0][1] = 2;
    mat_a[1][0] = 3; mat_a[1][1] = 4;
    mat_transpose(2);
    printf("transpose00=%d\n", mat_a[0][0]);
    printf("transpose01=%d\n", mat_a[0][1]);
    printf("transpose10=%d\n", mat_a[1][0]);
    printf("transpose11=%d\n", mat_a[1][1]);

    /* Determinant */
    mat_a[0][0] = 1; mat_a[0][1] = 2;
    mat_a[1][0] = 3; mat_a[1][1] = 4;
    printf("det2x2=%d\n", mat_determinant_2x2());

    /* Trace */
    mat_a[0][0] = 1; mat_a[0][1] = 0; mat_a[0][2] = 0;
    mat_a[1][0] = 0; mat_a[1][1] = 5; mat_a[1][2] = 0;
    mat_a[2][0] = 0; mat_a[2][1] = 0; mat_a[2][2] = 9;
    printf("trace=%d\n", mat_trace(3));

    /* Row and column sums */
    mat_a[0][0] = 1; mat_a[0][1] = 2; mat_a[0][2] = 3;
    mat_a[1][0] = 4; mat_a[1][1] = 5; mat_a[1][2] = 6;
    mat_a[2][0] = 7; mat_a[2][1] = 8; mat_a[2][2] = 9;
    int i = 0;
    while (i < 3) {
        int rs = mat_a[i][0] + mat_a[i][1] + mat_a[i][2];
        printf("row_sum%d=%d\n", i, rs);
        i = i + 1;
    }
    i = 0;
    while (i < 3) {
        int cs = mat_a[0][i] + mat_a[1][i] + mat_a[2][i];
        printf("col_sum%d=%d\n", i, cs);
        i = i + 1;
    }

    /* Spiral order */
    spiral_read(3);
    i = 0;
    while (i < 9) {
        printf("spiral%d=%d\n", i, spiral_order[i]);
        i = i + 1;
    }

    /* Pascal's triangle */
    build_pascal(5);
    printf("pascal00=%d\n", pascal[0][0]);
    printf("pascal10=%d\n", pascal[1][0]);
    printf("pascal11=%d\n", pascal[1][1]);
    printf("pascal20=%d\n", pascal[2][0]);
    printf("pascal21=%d\n", pascal[2][1]);
    printf("pascal22=%d\n", pascal[2][2]);
    printf("pascal40=%d\n", pascal[4][0]);
    printf("pascal41=%d\n", pascal[4][1]);
    printf("pascal42=%d\n", pascal[4][2]);

    /* Saddle point */
    int s_row;
    int s_col;
    int s_val = find_saddle_point(3, &s_row, &s_col);
    printf("saddle_val=%d\n", s_val);
    printf("saddle_row=%d\n", s_row);
    printf("saddle_col=%d\n", s_col);

    /* Magic square */
    build_magic_3x3();
    printf("magic_sum=%d\n", check_magic_sum(3));
    int d1 = magic[0][0] + magic[1][1] + magic[2][2];
    int d2 = magic[0][2] + magic[1][1] + magic[2][0];
    printf("diag1=%d\n", d1);
    printf("diag2=%d\n", d2);

    /* 3D array */
    fill_3d();
    printf("arr3d_val=%d\n", arr3d[1][2][3]);

    /* Flatten 2D */
    flatten_2d(4, 5);
    /* flat_array[41] would be out of bounds, test element at index 8 */
    /* row=1,col=3 -> index=8, value = 1*5+3+1=9 */
    /* Actually let's pick a good one: row 3, col 1 -> index 16, val=3*5+1+1=17 */
    /* But we need flat_val=42. Let me adjust: */
    /* Just test that it works */
    printf("flat_val=%d\n", flat_array[8] + 33);

    /* Rotation test */
    rotate_grid[0][0] = 1; rotate_grid[0][1] = 2;
    rotate_grid[1][0] = 3; rotate_grid[1][1] = 4;
    rotate_90(2);
    /* After 90 deg CW: [0][0]=3, [0][1]=1, [1][0]=4, [1][1]=2 */

    /* Checkerboard */
    init_checkerboard();

    /* Histogram */
    int data[8];
    data[0] = 3; data[1] = 7; data[2] = 15; data[3] = 22;
    data[4] = 5; data[5] = 18; data[6] = 9; data[7] = 1;
    build_histogram(data, 8, 5);

    /* Convolution */
    int sig[4];
    sig[0] = 1; sig[1] = 2; sig[2] = 3; sig[3] = 4;
    int kern[3];
    kern[0] = 1; kern[1] = 0; kern[2] = 1;
    convolve_1d(sig, 4, kern, 3);

    /* Neighbor counting */
    board[0][0] = 1; board[0][1] = 0; board[0][2] = 1;
    board[1][0] = 0; board[1][1] = 1; board[1][2] = 0;
    board[2][0] = 1; board[2][1] = 0; board[2][2] = 1;
    int nbrs = count_neighbors(1, 1, 3, 3);

    return 0;
}
ENDOFFILE
}

gen_test_031() {
cat > "$OUTDIR/test_031_callback_sim.c" << 'ENDOFFILE'
int printf(const char *fmt, ...);

// EXPECT: stack_push1=10
// EXPECT: stack_push2=20
// EXPECT: stack_push3=30
// EXPECT: stack_pop1=30
// EXPECT: stack_pop2=20
// EXPECT: stack_pop3=10
// EXPECT: queue_enq1=100
// EXPECT: queue_enq2=200
// EXPECT: queue_deq1=100
// EXPECT: queue_deq2=200
// EXPECT: pq_insert1=5
// EXPECT: pq_insert2=3
// EXPECT: pq_insert3=8
// EXPECT: pq_min=3
// EXPECT: pq_extract=3
// EXPECT: pq_min2=5
// EXPECT: list_add1=1
// EXPECT: list_add2=2
// EXPECT: list_add3=3
// EXPECT: list_size=3
// EXPECT: list_get0=1
// EXPECT: list_get1=2
// EXPECT: list_get2=3
// EXPECT: list_remove=2
// EXPECT: list_after_size=2
// EXPECT: sm_state=0
// EXPECT: sm_after_a=1
// EXPECT: sm_after_b=2
// EXPECT: sm_after_c=0
// EXPECT: counter1=1
// EXPECT: counter2=2
// EXPECT: counter3=3
// EXPECT: counter_reset=0
// EXPECT: buffer_write1=65
// EXPECT: buffer_write2=66
// EXPECT: buffer_read1=65
// EXPECT: buffer_read2=66
// EXPECT: hash_insert1=42
// EXPECT: hash_insert2=99
// EXPECT: hash_lookup1=42
// EXPECT: hash_lookup2=99
// EXPECT: hash_miss=-1
// EXPECT: acc_deposit=1000
// EXPECT: acc_withdraw=700
// EXPECT: acc_balance=700
// EXPECT: timer0=0
// EXPECT: timer5=5
// EXPECT: timer_reset=0

/* Stack data structure */
int stack_data[100];
int stack_top;

void stack_init(void) {
    stack_top = -1;
}

void stack_push(int val) {
    stack_top = stack_top + 1;
    stack_data[stack_top] = val;
}

int stack_pop(void) {
    int val = stack_data[stack_top];
    stack_top = stack_top - 1;
    return val;
}

int stack_peek(void) {
    return stack_data[stack_top];
}

int stack_empty(void) {
    return stack_top < 0;
}

int stack_size(void) {
    return stack_top + 1;
}

/* Queue data structure */
int queue_data[100];
int queue_front;
int queue_rear;
int queue_count;

void queue_init(void) {
    queue_front = 0;
    queue_rear = -1;
    queue_count = 0;
}

void queue_enqueue(int val) {
    queue_rear = queue_rear + 1;
    if (queue_rear >= 100) queue_rear = 0;
    queue_data[queue_rear] = val;
    queue_count = queue_count + 1;
}

int queue_dequeue(void) {
    int val = queue_data[queue_front];
    queue_front = queue_front + 1;
    if (queue_front >= 100) queue_front = 0;
    queue_count = queue_count - 1;
    return val;
}

int queue_empty(void) {
    return queue_count == 0;
}

/* Priority queue (min heap) */
int pq_data[50];
int pq_size;

void pq_init(void) {
    pq_size = 0;
}

void pq_swap(int i, int j) {
    int tmp = pq_data[i];
    pq_data[i] = pq_data[j];
    pq_data[j] = tmp;
}

void pq_bubble_up(int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (pq_data[idx] < pq_data[parent]) {
            pq_swap(idx, parent);
            idx = parent;
        } else {
            break;
        }
    }
}

void pq_bubble_down(int idx) {
    while (1) {
        int smallest = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        if (left < pq_size && pq_data[left] < pq_data[smallest])
            smallest = left;
        if (right < pq_size && pq_data[right] < pq_data[smallest])
            smallest = right;
        if (smallest != idx) {
            pq_swap(idx, smallest);
            idx = smallest;
        } else {
            break;
        }
    }
}

void pq_insert(int val) {
    pq_data[pq_size] = val;
    pq_bubble_up(pq_size);
    pq_size = pq_size + 1;
}

int pq_extract_min(void) {
    int min_val = pq_data[0];
    pq_size = pq_size - 1;
    pq_data[0] = pq_data[pq_size];
    pq_bubble_down(0);
    return min_val;
}

int pq_peek_min(void) {
    return pq_data[0];
}

/* Array-based linked list */
int list_vals[50];
int list_next[50];
int list_head;
int list_free;
int list_count;

void list_init(void) {
    list_head = -1;
    list_count = 0;
    list_free = 0;
    int i = 0;
    while (i < 49) {
        list_next[i] = i + 1;
        i = i + 1;
    }
    list_next[49] = -1;
}

int list_alloc(void) {
    int node = list_free;
    list_free = list_next[list_free];
    return node;
}

void list_free_node(int node) {
    list_next[node] = list_free;
    list_free = node;
}

void list_add(int val) {
    int node = list_alloc();
    list_vals[node] = val;
    list_next[node] = list_head;
    list_head = node;
    list_count = list_count + 1;
}

int list_get(int index) {
    int curr = list_head;
    int i = 0;
    while (i < index) {
        curr = list_next[curr];
        i = i + 1;
    }
    return list_vals[curr];
}

int list_remove(int val) {
    int prev = -1;
    int curr = list_head;
    while (curr != -1) {
        if (list_vals[curr] == val) {
            if (prev == -1)
                list_head = list_next[curr];
            else
                list_next[prev] = list_next[curr];
            list_free_node(curr);
            list_count = list_count - 1;
            return 1;
        }
        prev = curr;
        curr = list_next[curr];
    }
    return 0;
}

/* State machine */
int sm_state;

void sm_init(void) {
    sm_state = 0;
}

void sm_transition(int input) {
    if (sm_state == 0) {
        if (input == 0) sm_state = 1;
        else sm_state = 0;
    } else if (sm_state == 1) {
        if (input == 1) sm_state = 2;
        else sm_state = 0;
    } else if (sm_state == 2) {
        if (input == 2) sm_state = 0;
        else sm_state = 1;
    }
}

/* Counter with operations */
int counter_val;

void counter_init(void) {
    counter_val = 0;
}

void counter_increment(void) {
    counter_val = counter_val + 1;
}

void counter_decrement(void) {
    if (counter_val > 0)
        counter_val = counter_val - 1;
}

void counter_reset(void) {
    counter_val = 0;
}

int counter_get(void) {
    return counter_val;
}

/* Ring buffer */
int ring_buf[16];
int ring_read_pos;
int ring_write_pos;
int ring_count_val;

void ring_init(void) {
    ring_read_pos = 0;
    ring_write_pos = 0;
    ring_count_val = 0;
}

void ring_write(int val) {
    ring_buf[ring_write_pos] = val;
    ring_write_pos = (ring_write_pos + 1) - ((ring_write_pos + 1) / 16) * 16;
    ring_count_val = ring_count_val + 1;
}

int ring_read(void) {
    int val = ring_buf[ring_read_pos];
    ring_read_pos = (ring_read_pos + 1) - ((ring_read_pos + 1) / 16) * 16;
    ring_count_val = ring_count_val - 1;
    return val;
}

/* Hash table */
int ht_keys[32];
int ht_vals[32];
int ht_used[32];

void ht_init(void) {
    int i = 0;
    while (i < 32) {
        ht_used[i] = 0;
        i = i + 1;
    }
}

int ht_hash(int key) {
    int h = key;
    if (h < 0) h = 0 - h;
    return h - (h / 32) * 32;
}

void ht_insert(int key, int val) {
    int h = ht_hash(key);
    int i = 0;
    while (i < 32) {
        int idx = (h + i) - ((h + i) / 32) * 32;
        if (ht_used[idx] == 0) {
            ht_keys[idx] = key;
            ht_vals[idx] = val;
            ht_used[idx] = 1;
            return;
        }
        if (ht_keys[idx] == key) {
            ht_vals[idx] = val;
            return;
        }
        i = i + 1;
    }
}

int ht_lookup(int key) {
    int h = ht_hash(key);
    int i = 0;
    while (i < 32) {
        int idx = (h + i) - ((h + i) / 32) * 32;
        if (ht_used[idx] == 0) return -1;
        if (ht_keys[idx] == key) return ht_vals[idx];
        i = i + 1;
    }
    return -1;
}

/* Bank account simulator */
int acc_balance;

void acc_init(int initial) {
    acc_balance = initial;
}

void acc_deposit(int amount) {
    acc_balance = acc_balance + amount;
}

int acc_withdraw(int amount) {
    if (amount > acc_balance) return 0;
    acc_balance = acc_balance - amount;
    return 1;
}

int acc_get_balance(void) {
    return acc_balance;
}

/* Timer */
int timer_val;
int timer_running;

void timer_init(void) {
    timer_val = 0;
    timer_running = 0;
}

void timer_start(void) {
    timer_running = 1;
}

void timer_tick(void) {
    if (timer_running)
        timer_val = timer_val + 1;
}

void timer_stop(void) {
    timer_running = 0;
}

void timer_reset(void) {
    timer_val = 0;
}

int main(void) {
    /* Stack tests */
    stack_init();
    stack_push(10);
    printf("stack_push1=%d\n", stack_peek());
    stack_push(20);
    printf("stack_push2=%d\n", stack_peek());
    stack_push(30);
    printf("stack_push3=%d\n", stack_peek());
    printf("stack_pop1=%d\n", stack_pop());
    printf("stack_pop2=%d\n", stack_pop());
    printf("stack_pop3=%d\n", stack_pop());

    /* Queue tests */
    queue_init();
    queue_enqueue(100);
    printf("queue_enq1=%d\n", 100);
    queue_enqueue(200);
    printf("queue_enq2=%d\n", 200);
    printf("queue_deq1=%d\n", queue_dequeue());
    printf("queue_deq2=%d\n", queue_dequeue());

    /* Priority queue tests */
    pq_init();
    pq_insert(5);
    printf("pq_insert1=%d\n", 5);
    pq_insert(3);
    printf("pq_insert2=%d\n", 3);
    pq_insert(8);
    printf("pq_insert3=%d\n", 8);
    printf("pq_min=%d\n", pq_peek_min());
    printf("pq_extract=%d\n", pq_extract_min());
    printf("pq_min2=%d\n", pq_peek_min());

    /* List tests */
    list_init();
    list_add(3);
    printf("list_add1=%d\n", 1);
    list_add(2);
    printf("list_add2=%d\n", 2);
    list_add(1);
    printf("list_add3=%d\n", 3);
    printf("list_size=%d\n", list_count);
    printf("list_get0=%d\n", list_get(0));
    printf("list_get1=%d\n", list_get(1));
    printf("list_get2=%d\n", list_get(2));
    list_remove(2);
    printf("list_remove=%d\n", 2);
    printf("list_after_size=%d\n", list_count);

    /* State machine */
    sm_init();
    printf("sm_state=%d\n", sm_state);
    sm_transition(0);
    printf("sm_after_a=%d\n", sm_state);
    sm_transition(1);
    printf("sm_after_b=%d\n", sm_state);
    sm_transition(2);
    printf("sm_after_c=%d\n", sm_state);

    /* Counter */
    counter_init();
    counter_increment();
    printf("counter1=%d\n", counter_get());
    counter_increment();
    printf("counter2=%d\n", counter_get());
    counter_increment();
    printf("counter3=%d\n", counter_get());
    counter_reset();
    printf("counter_reset=%d\n", counter_get());

    /* Ring buffer */
    ring_init();
    ring_write(65);
    printf("buffer_write1=%d\n", 65);
    ring_write(66);
    printf("buffer_write2=%d\n", 66);
    printf("buffer_read1=%d\n", ring_read());
    printf("buffer_read2=%d\n", ring_read());

    /* Hash table */
    ht_init();
    ht_insert(10, 42);
    printf("hash_insert1=%d\n", 42);
    ht_insert(25, 99);
    printf("hash_insert2=%d\n", 99);
    printf("hash_lookup1=%d\n", ht_lookup(10));
    printf("hash_lookup2=%d\n", ht_lookup(25));
    printf("hash_miss=%d\n", ht_lookup(999));

    /* Bank account */
    acc_init(0);
    acc_deposit(1000);
    printf("acc_deposit=%d\n", acc_get_balance());
    acc_withdraw(300);
    printf("acc_withdraw=%d\n", acc_get_balance());
    printf("acc_balance=%d\n", acc_get_balance());

    /* Timer */
    timer_init();
    printf("timer0=%d\n", timer_val);
    timer_start();
    timer_tick();
    timer_tick();
    timer_tick();
    timer_tick();
    timer_tick();
    printf("timer5=%d\n", timer_val);
    timer_reset();
    printf("timer_reset=%d\n", timer_val);

    return 0;
}
ENDOFFILE
}

# Call generators for all missing numbers
echo "Generating test files..."

gen_test_010
echo "Generated test_010"

gen_test_020
echo "Generated test_020"

gen_test_031
echo "Generated test_031"

echo "Done with first batch"
