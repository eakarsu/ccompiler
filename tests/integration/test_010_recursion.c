int printf(const char *fmt, ...);
// EXPECT: fact0=1
// fact1=1
// fact5=120
// fact10=3628800
// fib0=0
// fib1=1
// fib5=5
// fib10=55
// fib12=144
// sum10=55
// sum100=5050
// pow2_10=1024
// pow3_5=243
// powfast2_10=1024
// powfast3_5=243
// gcd(12,8)=4
// gcd(100,75)=25
// lcm(4,6)=12
// bsearch_6=3
// bsearch_14=7
// bsearch_5=-1
// rec_max=9
// rec_min=1
// rec_sum=25
// rec_count2=1
// rec_contains9=1
// rec_contains7=0
// palindrome=1
// not_palindrome=0
// reversed=5 4 3 2 1
// all_pos=1
// all_pos_neg=0
// any_neg=1
// move 1: 1->3
// move 2: 1->2
// move 1: 3->2
// move 3: 1->3
// move 1: 2->1
// move 2: 2->3
// move 1: 1->3
// hanoi3_moves=7
// digitsum(12345)=15
// digits(12345)=5
// reverse(12345)=54321
// is_palin(12321)=1
// is_palin(12345)=0
// mul_rec(7,8)=56
// div_rec(17,3)=5
// mod_rec(17,3)=2
// qsort=0 1 2 3 4 5 6 7 8 9
// msort=0 1 2 3 4 5 6 7 8 9
// even0=1
// even1=0
// even10=1
// odd7=1
// C(5,2)=10
// C(10,3)=120
// catalan0=1
// catalan1=1
// catalan4=14
// catalan5=42
// trib5=4
// trib7=13
// pell0=0
// pell5=29
// lucas0=2
// lucas5=11
// ack(0,0)=1
// ack(1,1)=3
// ack(2,2)=7
// ack(3,1)=13
// sum_tail100=5050
// fact_tail10=3628800
// fib_tail10=55
// fib10_depth=9
// done=1

// ==========================================================================
// Global counter for move counting
// ==========================================================================

int move_count;

// ==========================================================================
// Basic recursive functions
// ==========================================================================

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int sum_recursive(int n) {
    if (n <= 0) return 0;
    return n + sum_recursive(n - 1);
}

int power_recursive(int base, int exp) {
    if (exp == 0) return 1;
    return base * power_recursive(base, exp - 1);
}

int power_fast(int base, int exp) {
    int half;
    if (exp == 0) return 1;
    half = power_fast(base, exp / 2);
    if (exp % 2 == 0) {
        return half * half;
    }
    return base * half * half;
}

int gcd_recursive(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    if (b == 0) return a;
    return gcd_recursive(b, a % b);
}

int lcm(int a, int b) {
    return a / gcd_recursive(a, b) * b;
}

// ==========================================================================
// Recursive search functions
// ==========================================================================

int binary_search_rec(int *arr, int lo, int hi, int target) {
    int mid;
    if (lo > hi) return -1;
    mid = (lo + hi) / 2;
    if (arr[mid] == target) return mid;
    if (arr[mid] < target) return binary_search_rec(arr, mid + 1, hi, target);
    return binary_search_rec(arr, lo, mid - 1, target);
}

int recursive_max(int *arr, int n) {
    int sub_max;
    if (n == 1) return arr[0];
    sub_max = recursive_max(arr, n - 1);
    if (arr[n - 1] > sub_max) return arr[n - 1];
    return sub_max;
}

int recursive_min(int *arr, int n) {
    int sub_min;
    if (n == 1) return arr[0];
    sub_min = recursive_min(arr, n - 1);
    if (arr[n - 1] < sub_min) return arr[n - 1];
    return sub_min;
}

int recursive_sum_arr(int *arr, int n) {
    if (n == 0) return 0;
    return arr[n - 1] + recursive_sum_arr(arr, n - 1);
}

int recursive_count(int *arr, int n, int val) {
    if (n == 0) return 0;
    if (arr[n - 1] == val) {
        return 1 + recursive_count(arr, n - 1, val);
    }
    return recursive_count(arr, n - 1, val);
}

int recursive_contains(int *arr, int n, int val) {
    if (n == 0) return 0;
    if (arr[n - 1] == val) return 1;
    return recursive_contains(arr, n - 1, val);
}

// ==========================================================================
// Recursive string/array operations
// ==========================================================================

int is_palindrome_rec(int *arr, int lo, int hi) {
    if (lo >= hi) return 1;
    if (arr[lo] != arr[hi]) return 0;
    return is_palindrome_rec(arr, lo + 1, hi - 1);
}

void reverse_arr_rec(int *arr, int lo, int hi) {
    int t;
    if (lo >= hi) return;
    t = arr[lo];
    arr[lo] = arr[hi];
    arr[hi] = t;
    reverse_arr_rec(arr, lo + 1, hi - 1);
}

int recursive_all_positive(int *arr, int n) {
    if (n == 0) return 1;
    if (arr[n - 1] <= 0) return 0;
    return recursive_all_positive(arr, n - 1);
}

int recursive_any_negative(int *arr, int n) {
    if (n == 0) return 0;
    if (arr[n - 1] < 0) return 1;
    return recursive_any_negative(arr, n - 1);
}

// ==========================================================================
// Tower of Hanoi
// ==========================================================================

void hanoi(int n, int from, int to, int aux) {
    if (n == 0) return;
    hanoi(n - 1, from, aux, to);
    printf("move %d: %d->%d\n", n, from, to);
    move_count = move_count + 1;
    hanoi(n - 1, aux, to, from);
}

// ==========================================================================
// Number theory recursive functions
// ==========================================================================

int digit_sum_rec(int n) {
    if (n < 0) n = -n;
    if (n < 10) return n;
    return (n % 10) + digit_sum_rec(n / 10);
}

int count_digits_rec(int n) {
    if (n < 0) n = -n;
    if (n < 10) return 1;
    return 1 + count_digits_rec(n / 10);
}

int reverse_number_helper(int n, int acc) {
    if (n == 0) return acc;
    return reverse_number_helper(n / 10, acc * 10 + n % 10);
}

int reverse_number(int n) {
    return reverse_number_helper(n, 0);
}

int is_palindrome_number(int n) {
    if (n < 0) return 0;
    return n == reverse_number(n);
}

int multiply_rec(int a, int b) {
    if (b == 0) return 0;
    if (b < 0) return -multiply_rec(a, -b);
    return a + multiply_rec(a, b - 1);
}

int divide_rec(int a, int b) {
    if (a < b) return 0;
    return 1 + divide_rec(a - b, b);
}

int modulo_rec(int a, int b) {
    if (a < b) return a;
    return modulo_rec(a - b, b);
}

// ==========================================================================
// Recursive sorting
// ==========================================================================

void swap(int *a, int *b) {
    int t;
    t = *a;
    *a = *b;
    *b = t;
}

int partition(int *arr, int lo, int hi) {
    int pivot;
    int i;
    int j;
    pivot = arr[hi];
    i = lo;
    for (j = lo; j < hi; j = j + 1) {
        if (arr[j] < pivot) {
            swap(&arr[i], &arr[j]);
            i = i + 1;
        }
    }
    swap(&arr[i], &arr[hi]);
    return i;
}

void quicksort(int *arr, int lo, int hi) {
    int p;
    if (lo < hi) {
        p = partition(arr, lo, hi);
        quicksort(arr, lo, p - 1);
        quicksort(arr, p + 1, hi);
    }
}

// Recursive merge sort
void merge_rec(int *arr, int *temp, int lo, int mid, int hi) {
    int i;
    int j;
    int k;
    for (i = lo; i <= hi; i = i + 1) {
        temp[i] = arr[i];
    }
    i = lo;
    j = mid + 1;
    k = lo;
    while (i <= mid && j <= hi) {
        if (temp[i] <= temp[j]) {
            arr[k] = temp[i];
            i = i + 1;
        } else {
            arr[k] = temp[j];
            j = j + 1;
        }
        k = k + 1;
    }
    while (i <= mid) {
        arr[k] = temp[i];
        i = i + 1;
        k = k + 1;
    }
}

void mergesort_rec(int *arr, int *temp, int lo, int hi) {
    int mid;
    if (lo < hi) {
        mid = (lo + hi) / 2;
        mergesort_rec(arr, temp, lo, mid);
        mergesort_rec(arr, temp, mid + 1, hi);
        merge_rec(arr, temp, lo, mid, hi);
    }
}

// ==========================================================================
// Mutual recursion
// ==========================================================================

int is_odd_rec(int n);

int is_even_rec(int n) {
    if (n == 0) return 1;
    return is_odd_rec(n - 1);
}

int is_odd_rec(int n) {
    if (n == 0) return 0;
    return is_even_rec(n - 1);
}

// ==========================================================================
// Combinatorial recursive functions
// ==========================================================================

int combination(int n, int k) {
    if (k == 0 || k == n) return 1;
    return combination(n - 1, k - 1) + combination(n - 1, k);
}

int catalan(int n) {
    int i;
    int s;
    if (n <= 1) return 1;
    s = 0;
    for (i = 0; i < n; i = i + 1) {
        s = s + catalan(i) * catalan(n - 1 - i);
    }
    return s;
}

// ==========================================================================
// Recursive number sequences
// ==========================================================================

int tribonacci(int n) {
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;
    return tribonacci(n - 1) + tribonacci(n - 2) + tribonacci(n - 3);
}

int pell(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    return 2 * pell(n - 1) + pell(n - 2);
}

int lucas(int n) {
    if (n == 0) return 2;
    if (n == 1) return 1;
    return lucas(n - 1) + lucas(n - 2);
}

// ==========================================================================
// Recursive depth tracking
// ==========================================================================

int max_recursion_depth;

int fibonacci_depth(int n, int depth) {
    if (depth > max_recursion_depth) {
        max_recursion_depth = depth;
    }
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fibonacci_depth(n - 1, depth + 1) + fibonacci_depth(n - 2, depth + 1);
}

// ==========================================================================
// Ackermann function (small values only)
// ==========================================================================

int ackermann(int m, int n) {
    if (m == 0) return n + 1;
    if (n == 0) return ackermann(m - 1, 1);
    return ackermann(m - 1, ackermann(m, n - 1));
}

// ==========================================================================
// Tail-recursive helpers
// ==========================================================================

int sum_tail(int n, int acc) {
    if (n <= 0) return acc;
    return sum_tail(n - 1, acc + n);
}

int factorial_tail(int n, int acc) {
    if (n <= 1) return acc;
    return factorial_tail(n - 1, n * acc);
}

int fib_tail(int n, int a, int b) {
    if (n == 0) return a;
    return fib_tail(n - 1, b, a + b);
}

// ==========================================================================
// Print array helper
// ==========================================================================

void print_arr(int *arr, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(" ");
    }
    printf("\n");
}

// ==========================================================================
// Main function
// ==========================================================================

int main(void) {
    int arr[20];
    int temp[20];
    int i;

    // Factorial
    printf("fact0=%d\n", factorial(0));
    printf("fact1=%d\n", factorial(1));
    printf("fact5=%d\n", factorial(5));
    printf("fact10=%d\n", factorial(10));

    // Fibonacci
    printf("fib0=%d\n", fibonacci(0));
    printf("fib1=%d\n", fibonacci(1));
    printf("fib5=%d\n", fibonacci(5));
    printf("fib10=%d\n", fibonacci(10));
    printf("fib12=%d\n", fibonacci(12));

    // Sum
    printf("sum10=%d\n", sum_recursive(10));
    printf("sum100=%d\n", sum_recursive(100));

    // Power
    printf("pow2_10=%d\n", power_recursive(2, 10));
    printf("pow3_5=%d\n", power_recursive(3, 5));
    printf("powfast2_10=%d\n", power_fast(2, 10));
    printf("powfast3_5=%d\n", power_fast(3, 5));

    // GCD/LCM
    printf("gcd(12,8)=%d\n", gcd_recursive(12, 8));
    printf("gcd(100,75)=%d\n", gcd_recursive(100, 75));
    printf("lcm(4,6)=%d\n", lcm(4, 6));

    // Binary search
    for (i = 0; i < 10; i = i + 1) arr[i] = i * 2;
    printf("bsearch_6=%d\n", binary_search_rec(arr, 0, 9, 6));
    printf("bsearch_14=%d\n", binary_search_rec(arr, 0, 9, 14));
    printf("bsearch_5=%d\n", binary_search_rec(arr, 0, 9, 5));

    // Recursive array operations
    arr[0]=5; arr[1]=2; arr[2]=8; arr[3]=1; arr[4]=9;
    printf("rec_max=%d\n", recursive_max(arr, 5));
    printf("rec_min=%d\n", recursive_min(arr, 5));
    printf("rec_sum=%d\n", recursive_sum_arr(arr, 5));
    printf("rec_count2=%d\n", recursive_count(arr, 5, 2));
    printf("rec_contains9=%d\n", recursive_contains(arr, 5, 9));
    printf("rec_contains7=%d\n", recursive_contains(arr, 5, 7));

    // Palindrome check
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=2; arr[4]=1;
    printf("palindrome=%d\n", is_palindrome_rec(arr, 0, 4));
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    printf("not_palindrome=%d\n", is_palindrome_rec(arr, 0, 4));

    // Reverse array
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    reverse_arr_rec(arr, 0, 4);
    printf("reversed=");
    print_arr(arr, 5);

    // All positive / any negative
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    printf("all_pos=%d\n", recursive_all_positive(arr, 5));
    arr[2] = -1;
    printf("all_pos_neg=%d\n", recursive_all_positive(arr, 5));
    printf("any_neg=%d\n", recursive_any_negative(arr, 5));

    // Tower of Hanoi (3 disks)
    move_count = 0;
    hanoi(3, 1, 3, 2);
    printf("hanoi3_moves=%d\n", move_count);

    // Digit operations
    printf("digitsum(12345)=%d\n", digit_sum_rec(12345));
    printf("digits(12345)=%d\n", count_digits_rec(12345));
    printf("reverse(12345)=%d\n", reverse_number(12345));
    printf("is_palin(12321)=%d\n", is_palindrome_number(12321));
    printf("is_palin(12345)=%d\n", is_palindrome_number(12345));

    // Recursive arithmetic
    printf("mul_rec(7,8)=%d\n", multiply_rec(7, 8));
    printf("div_rec(17,3)=%d\n", divide_rec(17, 3));
    printf("mod_rec(17,3)=%d\n", modulo_rec(17, 3));

    // Quicksort
    arr[0]=7; arr[1]=3; arr[2]=9; arr[3]=1; arr[4]=5;
    arr[5]=0; arr[6]=8; arr[7]=2; arr[8]=6; arr[9]=4;
    quicksort(arr, 0, 9);
    printf("qsort=");
    print_arr(arr, 10);

    // Recursive merge sort
    arr[0]=4; arr[1]=6; arr[2]=2; arr[3]=8; arr[4]=0;
    arr[5]=1; arr[6]=9; arr[7]=3; arr[8]=7; arr[9]=5;
    mergesort_rec(arr, temp, 0, 9);
    printf("msort=");
    print_arr(arr, 10);

    // Mutual recursion
    printf("even0=%d\n", is_even_rec(0));
    printf("even1=%d\n", is_even_rec(1));
    printf("even10=%d\n", is_even_rec(10));
    printf("odd7=%d\n", is_odd_rec(7));

    // Combinations
    printf("C(5,2)=%d\n", combination(5, 2));
    printf("C(10,3)=%d\n", combination(10, 3));

    // Catalan numbers
    printf("catalan0=%d\n", catalan(0));
    printf("catalan1=%d\n", catalan(1));
    printf("catalan4=%d\n", catalan(4));
    printf("catalan5=%d\n", catalan(5));

    // Tribonacci
    printf("trib5=%d\n", tribonacci(5));
    printf("trib7=%d\n", tribonacci(7));

    // Pell numbers
    printf("pell0=%d\n", pell(0));
    printf("pell5=%d\n", pell(5));

    // Lucas numbers
    printf("lucas0=%d\n", lucas(0));
    printf("lucas5=%d\n", lucas(5));

    // Ackermann
    printf("ack(0,0)=%d\n", ackermann(0, 0));
    printf("ack(1,1)=%d\n", ackermann(1, 1));
    printf("ack(2,2)=%d\n", ackermann(2, 2));
    printf("ack(3,1)=%d\n", ackermann(3, 1));

    // Tail-recursive versions
    printf("sum_tail100=%d\n", sum_tail(100, 0));
    printf("fact_tail10=%d\n", factorial_tail(10, 1));
    printf("fib_tail10=%d\n", fib_tail(10, 0, 1));

    // Fibonacci with depth tracking
    max_recursion_depth = 0;
    fibonacci_depth(10, 0);
    printf("fib10_depth=%d\n", max_recursion_depth);

    printf("done=1\n");

    return 0;
}
