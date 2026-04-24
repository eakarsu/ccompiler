int printf(const char *fmt, ...);
// EXPECT: sum1k=  500500|sumsq100= 338350|mod7:  sum=2997,cnt0= 143,cnt6=142|minmax:min=-500,max=499|rev: 10,9,8,7,6,5,4,3,2,1|bsort:srt=1,first5=  0,0,0,1,1|isort:srt=1|bsrch:50,250,-1,999|fib:55,6765,832040,102334155|rsum:5050,125250|pow:1024,2187,3125|gcd:6,25,1|dot=338350|mat:      80,214,316,386|chain: 3719,54698|primes=25,last=   97|p200:cnt=46|prefix=1,3,6,10,15,21,28,36,45,55|inv=13|large:a[999]=2997,a[500]=  1500|heavy=2629059|end

/* Stress test: large arrays, many function calls, deep recursion, extensive computation */

int big_array[1000];
int big_array2[1000];
int scratch[500];

/* Fill array with pattern */
void fill_pattern(int *arr, int size, int start, int step) {
    int i;
    i = 0;
    while (i < size) {
        arr[i] = start + i * step;
        i = i + 1;
    }
}

/* Fill with modular values */
void fill_mod(int *arr, int size, int mod) {
    int i;
    i = 0;
    while (i < size) {
        arr[i] = i % mod;
        i = i + 1;
    }
}

/* Sum of array */
int array_sum(int *arr, int size) {
    int i;
    int s;
    s = 0;
    i = 0;
    while (i < size) {
        s = s + arr[i];
        i = i + 1;
    }
    return s;
}

/* Sum of squares */
int sum_squares(int *arr, int size) {
    int i;
    int s;
    s = 0;
    i = 0;
    while (i < size) {
        s = s + arr[i] * arr[i];
        i = i + 1;
    }
    return s;
}

/* Dot product of two arrays */
int dot_product(int *a, int *b, int size) {
    int i;
    int s;
    s = 0;
    i = 0;
    while (i < size) {
        s = s + a[i] * b[i];
        i = i + 1;
    }
    return s;
}

/* Count values equal to target */
int count_val(int *arr, int size, int target) {
    int i;
    int cnt;
    cnt = 0;
    i = 0;
    while (i < size) {
        if (arr[i] == target) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

/* Find max value */
int find_max(int *arr, int size) {
    int i;
    int m;
    m = arr[0];
    i = 1;
    while (i < size) {
        if (arr[i] > m) m = arr[i];
        i = i + 1;
    }
    return m;
}

/* Find min value */
int find_min(int *arr, int size) {
    int i;
    int m;
    m = arr[0];
    i = 1;
    while (i < size) {
        if (arr[i] < m) m = arr[i];
        i = i + 1;
    }
    return m;
}

/* Reverse array in-place */
void reverse(int *arr, int size) {
    int i;
    int j;
    int tmp;
    i = 0;
    j = size - 1;
    while (i < j) {
        tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
        i = i + 1;
        j = j - 1;
    }
}

/* Binary search (array must be sorted) */
int binary_search(int *arr, int size, int target) {
    int lo;
    int hi;
    int mid;
    lo = 0;
    hi = size - 1;
    while (lo <= hi) {
        mid = (lo + hi) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

/* Bubble sort a portion */
void bubble_sort(int *arr, int size) {
    int i;
    int j;
    int tmp;
    i = 0;
    while (i < size - 1) {
        j = 0;
        while (j < size - 1 - i) {
            if (arr[j] > arr[j + 1]) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

/* Insertion sort */
void insertion_sort(int *arr, int size) {
    int i;
    int j;
    int key;
    i = 1;
    while (i < size) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
        i = i + 1;
    }
}

/* Check if sorted */
int is_sorted(int *arr, int size) {
    int i;
    i = 0;
    while (i < size - 1) {
        if (arr[i] > arr[i + 1]) return 0;
        i = i + 1;
    }
    return 1;
}

/* Deep recursion: fibonacci with memoization */
int fib_memo[100];
int fib_inited;

void init_fib(void) {
    int i;
    i = 0;
    while (i < 100) {
        fib_memo[i] = -1;
        i = i + 1;
    }
    fib_inited = 1;
}

int fib(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    if (fib_memo[n] >= 0) return fib_memo[n];
    fib_memo[n] = fib(n - 1) + fib(n - 2);
    return fib_memo[n];
}

/* Deep recursion: sum 1..n */
int rec_sum(int n) {
    if (n <= 0) return 0;
    return n + rec_sum(n - 1);
}

/* Deep recursion: power */
int rec_power(int base, int exp) {
    if (exp <= 0) return 1;
    return base * rec_power(base, exp - 1);
}

/* Deep recursion: GCD */
int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

/* Matrix multiply 4x4 stored as flat array */
void mat_mul_4x4(int *a, int *b, int *c) {
    int i;
    int j;
    int k;
    int sum;
    i = 0;
    while (i < 4) {
        j = 0;
        while (j < 4) {
            sum = 0;
            k = 0;
            while (k < 4) {
                sum = sum + a[i * 4 + k] * b[k * 4 + j];
                k = k + 1;
            }
            c[i * 4 + j] = sum;
            j = j + 1;
        }
        i = i + 1;
    }
}

/* Count inversions in array (how unsorted it is) */
int count_inversions(int *arr, int size) {
    int i;
    int j;
    int cnt;
    cnt = 0;
    i = 0;
    while (i < size) {
        j = i + 1;
        while (j < size) {
            if (arr[i] > arr[j]) cnt = cnt + 1;
            j = j + 1;
        }
        i = i + 1;
    }
    return cnt;
}

/* Prefix sum */
void prefix_sum(int *arr, int *out, int size) {
    int i;
    out[0] = arr[0];
    i = 1;
    while (i < size) {
        out[i] = out[i - 1] + arr[i];
        i = i + 1;
    }
}

/* Many function calls: compute hash-like value */
int compute_heavy(int x) {
    int result;
    result = x * 31 + 7;
    result = result ^ (result >> 3);
    result = result * 17 + 13;
    result = result ^ (result >> 5);
    result = result & 0xFFFF;
    return result;
}

int chain_compute(int x, int iterations) {
    int i;
    i = 0;
    while (i < iterations) {
        x = compute_heavy(x);
        i = i + 1;
    }
    return x;
}

/* Sieve of primes up to n, store in array, return count */
int primes[200];

int sieve(int n) {
    int is_prime[500];
    int i;
    int j;
    int cnt;
    i = 0;
    while (i < n + 1 && i < 500) {
        is_prime[i] = 1;
        i = i + 1;
    }
    is_prime[0] = 0;
    if (n >= 1) is_prime[1] = 0;
    i = 2;
    while (i * i <= n) {
        if (is_prime[i]) {
            j = i * i;
            while (j <= n) {
                is_prime[j] = 0;
                j = j + i;
            }
        }
        i = i + 1;
    }
    cnt = 0;
    i = 2;
    while (i <= n) {
        if (is_prime[i]) {
            primes[cnt] = i;
            cnt = cnt + 1;
        }
        i = i + 1;
    }
    return cnt;
}

int main(void) {
    int i;
    int s;
    int v;
    int cnt;
    int mat_a[16];
    int mat_b[16];
    int mat_c[16];

    /* Test 1: Fill and sum large array */
    fill_pattern(big_array, 1000, 1, 1); /* 1, 2, ..., 1000 */
    s = array_sum(big_array, 1000);
    printf("%c%c%c%c%c%c%c%c", 's', 'u', 'm', '1', 'k', '=', ' ', ' ');
    printf("%d", s);

    /* Test 2: Sum of squares (first 100) */
    s = sum_squares(big_array, 100);
    printf("%c%c%c%c%c%c%c%c%c%c%c", '|', 's', 'u', 'm', 's', 'q', '1', '0', '0', '=', ' ');
    printf("%d", s);

    /* Test 3: Fill with modular pattern */
    fill_mod(big_array, 1000, 7);
    printf("%c%c%c%c%c%c%c%c", '|', 'm', 'o', 'd', '7', ':', ' ', ' ');
    printf("%c%c%c%c", 's', 'u', 'm', '=');
    printf("%d", array_sum(big_array, 1000));
    printf("%c%c%c%c%c%c%c", ',', 'c', 'n', 't', '0', '=', ' ');
    printf("%d", count_val(big_array, 1000, 0));
    printf("%c%c%c%c%c%c", ',', 'c', 'n', 't', '6', '=');
    printf("%d", count_val(big_array, 1000, 6));

    /* Test 4: Max and min */
    fill_pattern(big_array, 1000, -500, 1); /* -500, -499, ..., 499 */
    printf("%c%c%c%c%c%c%c", '|', 'm', 'i', 'n', 'm', 'a', 'x');
    printf("%c%c%c%c", ':', 'm', 'i', 'n');
    printf("%c", '=');
    printf("%d", find_min(big_array, 1000));
    printf("%c%c%c%c", ',', 'm', 'a', 'x');
    printf("%c", '=');
    printf("%d", find_max(big_array, 1000));

    /* Test 5: Reverse */
    fill_pattern(big_array, 10, 1, 1);
    reverse(big_array, 10);
    printf("%c%c%c%c%c%c", '|', 'r', 'e', 'v', ':', ' ');
    i = 0;
    while (i < 10) {
        if (i > 0) printf("%c", ',');
        printf("%d", big_array[i]);
        i = i + 1;
    }

    /* Test 6: Sort (small subset for bubble sort) */
    fill_mod(scratch, 50, 17);
    reverse(scratch, 50);
    bubble_sort(scratch, 50);
    printf("%c%c%c%c%c%c%c%c%c%c%c", '|', 'b', 's', 'o', 'r', 't', ':', 's', 'r', 't', '=');
    printf("%d", is_sorted(scratch, 50));
    printf("%c%c%c%c%c%c%c%c%c%c", ',', 'f', 'i', 'r', 's', 't', '5', '=', ' ', ' ');
    i = 0;
    while (i < 5) {
        if (i > 0) printf("%c", ',');
        printf("%d", scratch[i]);
        i = i + 1;
    }

    /* Test 7: Insertion sort */
    fill_mod(scratch, 50, 13);
    reverse(scratch, 50);
    insertion_sort(scratch, 50);
    printf("%c%c%c%c%c%c%c%c%c%c%c", '|', 'i', 's', 'o', 'r', 't', ':', 's', 'r', 't', '=');
    printf("%d", is_sorted(scratch, 50));

    /* Test 8: Binary search */
    fill_pattern(big_array, 1000, 0, 2); /* 0, 2, 4, ..., 1998 */
    printf("%c%c%c%c%c%c%c", '|', 'b', 's', 'r', 'c', 'h', ':');
    printf("%d", binary_search(big_array, 1000, 100));
    printf("%c", ',');
    printf("%d", binary_search(big_array, 1000, 500));
    printf("%c", ',');
    printf("%d", binary_search(big_array, 1000, 999));
    printf("%c", ',');
    printf("%d", binary_search(big_array, 1000, 1998));

    /* Test 9: Fibonacci */
    init_fib();
    printf("%c%c%c%c", '|', 'f', 'i', 'b');
    printf("%c", ':');
    printf("%d", fib(10));
    printf("%c", ',');
    printf("%d", fib(20));
    printf("%c", ',');
    printf("%d", fib(30));
    printf("%c", ',');
    printf("%d", fib(40));

    /* Test 10: Recursive sum */
    printf("%c%c%c%c%c%c", '|', 'r', 's', 'u', 'm', ':');
    printf("%d", rec_sum(100));
    printf("%c", ',');
    printf("%d", rec_sum(500));

    /* Test 11: Power */
    printf("%c%c%c%c%c", '|', 'p', 'o', 'w', ':');
    printf("%d", rec_power(2, 10));
    printf("%c", ',');
    printf("%d", rec_power(3, 7));
    printf("%c", ',');
    printf("%d", rec_power(5, 5));

    /* Test 12: GCD */
    printf("%c%c%c%c%c", '|', 'g', 'c', 'd', ':');
    printf("%d", gcd(48, 18));
    printf("%c", ',');
    printf("%d", gcd(100, 75));
    printf("%c", ',');
    printf("%d", gcd(17, 13));

    /* Test 13: Dot product */
    fill_pattern(big_array, 100, 1, 1);
    fill_pattern(big_array2, 100, 1, 1);
    printf("%c%c%c%c%c", '|', 'd', 'o', 't', '=');
    printf("%d", dot_product(big_array, big_array2, 100));

    /* Test 14: Matrix multiply */
    i = 0;
    while (i < 16) {
        mat_a[i] = i + 1;
        mat_b[i] = 16 - i;
        i = i + 1;
    }
    mat_mul_4x4(mat_a, mat_b, mat_c);
    printf("%c%c%c%c%c%c%c%c%c%c%c", '|', 'm', 'a', 't', ':', ' ', ' ', ' ', ' ', ' ', ' ');
    printf("%d", mat_c[0]);
    printf("%c", ',');
    printf("%d", mat_c[5]);
    printf("%c", ',');
    printf("%d", mat_c[10]);
    printf("%c", ',');
    printf("%d", mat_c[15]);

    /* Test 15: Chain computation */
    printf("%c%c%c%c%c%c%c%c", '|', 'c', 'h', 'a', 'i', 'n', ':', ' ');
    printf("%d", chain_compute(42, 100));
    printf("%c", ',');
    printf("%d", chain_compute(1, 50));

    /* Test 16: Sieve of primes */
    cnt = sieve(100);
    printf("%c%c%c%c%c%c%c%c", '|', 'p', 'r', 'i', 'm', 'e', 's', '=');
    printf("%d", cnt);
    printf("%c%c%c%c%c%c%c%c%c", ',', 'l', 'a', 's', 't', '=', ' ', ' ', ' ');
    printf("%d", primes[cnt - 1]);

    /* Primes up to 200 */
    cnt = sieve(200);
    printf("%c%c%c%c%c%c%c%c%c%c", '|', 'p', '2', '0', '0', ':', 'c', 'n', 't', '=');
    printf("%d", cnt);

    /* Test 17: Prefix sum */
    fill_pattern(big_array, 10, 1, 1);
    prefix_sum(big_array, big_array2, 10);
    printf("%c%c%c%c%c%c%c%c", '|', 'p', 'r', 'e', 'f', 'i', 'x', '=');
    i = 0;
    while (i < 10) {
        if (i > 0) printf("%c", ',');
        printf("%d", big_array2[i]);
        i = i + 1;
    }

    /* Test 18: Inversions */
    {
        int inv_arr[8];
        inv_arr[0] = 5; inv_arr[1] = 3; inv_arr[2] = 8; inv_arr[3] = 1;
        inv_arr[4] = 2; inv_arr[5] = 7; inv_arr[6] = 4; inv_arr[7] = 6;
        printf("%c%c%c%c%c", '|', 'i', 'n', 'v', '=');
        printf("%d", count_inversions(inv_arr, 8));
    }

    /* Test 19: Large fill and verify */
    fill_pattern(big_array, 1000, 0, 3);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c", '|', 'l', 'a', 'r', 'g', 'e', ':', 'a', '[', '9', '9', '9', ']', '=');
    printf("%d", big_array[999]);
    printf("%c%c%c%c%c%c%c%c%c%c", ',', 'a', '[', '5', '0', '0', ']', '=', ' ', ' ');
    printf("%d", big_array[500]);

    /* Test 20: Stress many function calls */
    s = 0;
    i = 0;
    while (i < 100) {
        s = s + compute_heavy(i);
        i = i + 1;
    }
    printf("%c%c%c%c%c%c%c", '|', 'h', 'e', 'a', 'v', 'y', '=');
    printf("%d", s);

    printf("%c%c%c%c", '|', 'e', 'n', 'd');
    printf("%c", '\n');

    return 0;
}
