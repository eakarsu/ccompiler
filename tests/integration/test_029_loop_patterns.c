int printf(const char *fmt, ...);
// EXPECT: 1:36\n2:225\n3:1\n4:5\n5:2\n6:0\n7:55\n8:5050\n9:1\n10:120\n11:1\n12:5040\n13:1\n14:3\n15:5\n16:609\n17:102\n18:55\n19:21\n20:5\n21:0\n22:3\n23:25\n24:1\n25:100\n26:14\n27:28\n28:7\n29:1\n30:4\n31:-1\n32:10\n33:15\n34:4\n35:7\n36:0\n37:3\n38:0\n39:5\n40:5\n41:60\n42:0\n43:3000\n44:200\n45:3\n46:1\n47:7\n48:1\n49:11\n50:35
// =============================================================================
// TEST 029: Loop Patterns
// =============================================================================
// Tests nested loops, loop unrolling simulation, multiple loop variables,
// complex loop conditions, loops with multiple exits, do-while patterns,
// and various control flow within loops.
// =============================================================================

// -----------------------------------------------------------------------------
// Sum of multiplication table up to n x n
// -----------------------------------------------------------------------------
int mult_table_sum(int n) {
    int i;
    int j;
    int sum;
    sum = 0;
    for (i = 1; i <= n; i = i + 1) {
        for (j = 1; j <= n; j = j + 1) {
            sum = sum + i * j;
        }
    }
    return sum;
}

// -----------------------------------------------------------------------------
// Count pairs (i,j) where i+j == target, 0 <= i < j < n
// -----------------------------------------------------------------------------
int count_pairs_sum(int n, int target) {
    int i;
    int j;
    int count;
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            if (i + j == target) count = count + 1;
        }
    }
    return count;
}

// -----------------------------------------------------------------------------
// Triangular number: 1+2+3+...+n using while loop
// -----------------------------------------------------------------------------
int triangular(int n) {
    int sum;
    int i;
    sum = 0;
    i = 1;
    while (i <= n) {
        sum = sum + i;
        i = i + 1;
    }
    return sum;
}

// -----------------------------------------------------------------------------
// Factorial using do-while
// -----------------------------------------------------------------------------
int factorial_do_while(int n) {
    int result;
    if (n <= 1) return 1;
    result = 1;
    do {
        result = result * n;
        n = n - 1;
    } while (n > 1);
    return result;
}

// -----------------------------------------------------------------------------
// Count digits using do-while
// -----------------------------------------------------------------------------
int count_digits(int n) {
    int count;
    if (n < 0) n = -n;
    count = 0;
    do {
        count = count + 1;
        n = n / 10;
    } while (n > 0);
    return count;
}

// -----------------------------------------------------------------------------
// Nested loop: find first pair where product > target
// Returns i*100+j encoding the pair, or -1
// -----------------------------------------------------------------------------
int find_product_pair(int n, int target) {
    int i;
    int j;
    for (i = 1; i <= n; i = i + 1) {
        for (j = 1; j <= n; j = j + 1) {
            if (i * j > target) {
                return i * 100 + j;
            }
        }
    }
    return -1;
}

// -----------------------------------------------------------------------------
// Loop unrolling simulation: sum array elements 4 at a time
// -----------------------------------------------------------------------------
int arr_unroll[40];

int sum_unrolled(int n) {
    int sum;
    int i;
    sum = 0;
    i = 0;
    // Process 4 elements at a time
    while (i + 3 < n) {
        sum = sum + arr_unroll[i] + arr_unroll[i + 1] + arr_unroll[i + 2] + arr_unroll[i + 3];
        i = i + 4;
    }
    // Handle remainder
    while (i < n) {
        sum = sum + arr_unroll[i];
        i = i + 1;
    }
    return sum;
}

// -----------------------------------------------------------------------------
// Multiple loop variables: converging indices
// Returns number of iterations until they meet
// -----------------------------------------------------------------------------
int converging_indices(int n) {
    int left;
    int right;
    int count;
    left = 0;
    right = n - 1;
    count = 0;
    while (left < right) {
        left = left + 1;
        right = right - 1;
        count = count + 1;
    }
    return count;
}

// -----------------------------------------------------------------------------
// Loop with continue: sum only odd numbers
// -----------------------------------------------------------------------------
int sum_odd(int n) {
    int i;
    int sum;
    sum = 0;
    for (i = 1; i <= n; i = i + 1) {
        if (i % 2 == 0) continue;
        sum = sum + i;
    }
    return sum;
}

// -----------------------------------------------------------------------------
// Loop with break: find first multiple of 7 after start
// -----------------------------------------------------------------------------
int first_multiple_7(int start) {
    int i;
    i = start;
    while (i < 1000) {
        if (i > start && i % 7 == 0) break;
        i = i + 1;
    }
    return i;
}

// -----------------------------------------------------------------------------
// Nested loop with inner break: find row with sum > target
// Returns row index or -1
// -----------------------------------------------------------------------------
int matrix[5][5];

int find_row_sum_gt(int rows, int cols, int target) {
    int i;
    int j;
    int row_sum;
    for (i = 0; i < rows; i = i + 1) {
        row_sum = 0;
        for (j = 0; j < cols; j = j + 1) {
            row_sum = row_sum + matrix[i][j];
        }
        if (row_sum > target) return i;
    }
    return -1;
}

// -----------------------------------------------------------------------------
// Sieve-like pattern: mark multiples
// Count non-marked entries
// -----------------------------------------------------------------------------
int sieve[100];

int count_unmarked(int n) {
    int i;
    int j;
    int count;
    for (i = 0; i < n; i = i + 1) sieve[i] = 0;
    for (i = 2; i < n; i = i + 1) {
        if (sieve[i] == 0) {
            for (j = i + i; j < n; j = j + i) {
                sieve[j] = 1;
            }
        }
    }
    count = 0;
    for (i = 2; i < n; i = i + 1) {
        if (sieve[i] == 0) count = count + 1;
    }
    return count;
}

// -----------------------------------------------------------------------------
// Bubble sort iteration count
// Returns number of swaps performed
// -----------------------------------------------------------------------------
int sort_arr[20];

int bubble_sort_count(int n) {
    int i;
    int j;
    int tmp;
    int swaps;
    swaps = 0;
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (sort_arr[j] > sort_arr[j + 1]) {
                tmp = sort_arr[j];
                sort_arr[j] = sort_arr[j + 1];
                sort_arr[j + 1] = tmp;
                swaps = swaps + 1;
            }
        }
    }
    return swaps;
}

// -----------------------------------------------------------------------------
// For loop with complex condition: Collatz-like
// Returns iterations until condition met
// -----------------------------------------------------------------------------
int complex_loop(int n) {
    int count;
    count = 0;
    while (n != 1 && count < 50) {
        if (n % 3 == 0) {
            n = n / 3;
        } else if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = n + 1;
        }
        count = count + 1;
    }
    return count;
}

// -----------------------------------------------------------------------------
// Zigzag pattern: increment then decrement
// Returns final value after n zigzags
// -----------------------------------------------------------------------------
int zigzag(int n, int step) {
    int val;
    int i;
    int going_up;
    val = 0;
    going_up = 1;
    for (i = 0; i < n; i = i + 1) {
        if (going_up) {
            val = val + step;
            if (val >= 10 * step) going_up = 0;
        } else {
            val = val - step;
            if (val <= 0) going_up = 1;
        }
    }
    return val;
}

// -----------------------------------------------------------------------------
// While loop with sentinel: sum until -1
// -----------------------------------------------------------------------------
int sentinel_data[30];

int sum_until_sentinel(void) {
    int i;
    int sum;
    sum = 0;
    i = 0;
    while (sentinel_data[i] != -1 && i < 30) {
        sum = sum + sentinel_data[i];
        i = i + 1;
    }
    return sum;
}

// -----------------------------------------------------------------------------
// Loop accumulation pattern: running average * 100 (integer only)
// Returns last average * 100
// -----------------------------------------------------------------------------
int running_avg(int *data, int n) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        sum = sum + data[i];
    }
    return (sum * 100) / n;
}

// -----------------------------------------------------------------------------
// Nested loop: count all triples (i,j,k) with i<j<k<n that sum to target
// -----------------------------------------------------------------------------
int count_triples(int n, int target) {
    int i;
    int j;
    int k;
    int count;
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            for (k = j + 1; k < n; k = k + 1) {
                if (i + j + k == target) count = count + 1;
            }
        }
    }
    return count;
}

// -----------------------------------------------------------------------------
// Power of 2 loop: keep doubling until exceeding target
// Returns the count of doublings
// -----------------------------------------------------------------------------
int doublings_to_exceed(int target) {
    int val;
    int count;
    val = 1;
    count = 0;
    while (val <= target) {
        val = val * 2;
        count = count + 1;
    }
    return count;
}

// -----------------------------------------------------------------------------
// Scan pattern: two nested loops with different bounds
// Sum of grid positions where row + col is even
// -----------------------------------------------------------------------------
int grid_sum_even_pos(int rows, int cols) {
    int i;
    int j;
    int sum;
    sum = 0;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            if ((i + j) % 2 == 0) {
                sum = sum + matrix[i][j];
            }
        }
    }
    return sum;
}

// =============================================================================
// Main test function
// =============================================================================
int main(void) {
    int test_num;
    int v;
    int i;
    int data[10];

    test_num = 0;

    // ---- Multiplication table sum ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, mult_table_sum(3)); // (1+2+3)^2 = 36

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, mult_table_sum(5)); // (15)^2 = 225

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, mult_table_sum(1)); // 1

    // ---- Count pairs ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_pairs_sum(10, 9)); // pairs: (0,9),(1,8),(2,7),(3,6),(4,5) = 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_pairs_sum(5, 3)); // (0,3),(1,2) = 2

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_pairs_sum(5, 10)); // 0

    // ---- Triangular numbers ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, triangular(10)); // 55

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, triangular(100)); // 5050

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, triangular(1)); // 1

    // ---- Do-while factorial ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, factorial_do_while(5)); // 120

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, factorial_do_while(1)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, factorial_do_while(7)); // 5040

    // ---- Count digits ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_digits(0)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_digits(123)); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_digits(99999)); // 5

    // ---- Find product pair ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, find_product_pair(10, 50)); // first i,j where i*j>50: (6,9) => 609

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, find_product_pair(5, 1)); // (1,2) => 102

    // ---- Loop unrolling ----
    for (i = 0; i < 10; i = i + 1) arr_unroll[i] = i + 1;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, sum_unrolled(10)); // 55

    for (i = 0; i < 7; i = i + 1) arr_unroll[i] = 3;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, sum_unrolled(7)); // 21

    // ---- Converging indices ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, converging_indices(10)); // 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, converging_indices(1)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, converging_indices(7)); // 3

    // ---- Sum odd ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, sum_odd(10)); // 1+3+5+7+9 = 25

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, sum_odd(1)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, sum_odd(20)); // 100

    // ---- First multiple of 7 ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, first_multiple_7(10)); // 14

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, first_multiple_7(21)); // 28

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, first_multiple_7(1)); // 7

    // ---- Matrix row sum ----
    // Fill matrix
    for (i = 0; i < 5; i = i + 1) {
        int j;
        for (j = 0; j < 5; j = j + 1) {
            matrix[i][j] = i * 5 + j + 1;
        }
    }
    // Row 0: 1+2+3+4+5=15, Row 1: 6+7+8+9+10=40, etc.
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, find_row_sum_gt(5, 5, 20)); // 1 (row 1 sum=40>20)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, find_row_sum_gt(5, 5, 100)); // 4 (row 4: 21+22+23+24+25=115)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, find_row_sum_gt(5, 5, 200)); // -1

    // ---- Sieve (count primes) ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_unmarked(30)); // 10

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_unmarked(50)); // 15

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_unmarked(10)); // 4

    // ---- Bubble sort swaps ----
    sort_arr[0] = 5; sort_arr[1] = 3; sort_arr[2] = 1; sort_arr[3] = 4; sort_arr[4] = 2;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, bubble_sort_count(5)); // number of swaps

    // Already sorted
    sort_arr[0] = 1; sort_arr[1] = 2; sort_arr[2] = 3;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, bubble_sort_count(3)); // 0

    // ---- Complex loop ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, complex_loop(12)); // 12->4->2->1 = 3 steps

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, complex_loop(1)); // 0

    // ---- Zigzag ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, zigzag(5, 1)); // 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, zigzag(15, 1)); // goes up to 10, down to 5

    // ---- Sentinel sum ----
    sentinel_data[0] = 10; sentinel_data[1] = 20; sentinel_data[2] = 30;
    sentinel_data[3] = -1;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, sum_until_sentinel()); // 60

    sentinel_data[0] = -1;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, sum_until_sentinel()); // 0

    // ---- Running average ----
    data[0] = 10; data[1] = 20; data[2] = 30; data[3] = 40; data[4] = 50;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, running_avg(data, 5)); // 3000

    data[0] = 1; data[1] = 2; data[2] = 3;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, running_avg(data, 3)); // 200

    // ---- Count triples ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_triples(10, 6)); // (0,1,5),(0,2,4),(1,2,3) = 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_triples(5, 3)); // (0,1,2) = 1

    // ---- Doublings ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, doublings_to_exceed(100)); // 7 (128>100)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, doublings_to_exceed(1)); // 1 (2>1)

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, doublings_to_exceed(1024)); // 11

    // ---- Grid sum even positions ----
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, grid_sum_even_pos(3, 3));

    return 0;
}
