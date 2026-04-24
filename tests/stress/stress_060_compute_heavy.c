// stress_060_compute_heavy.c - Compute-intensive stress test
// Matrix multiply, determinant, polynomial eval, bubble sort, binary search, CRC.
// EXPECT: matmul_trace=3584
// EXPECT: det4x4=1
// EXPECT: poly_eval=10000000000
// EXPECT: sorted_first=1
// EXPECT: sorted_last=50
// EXPECT: bsearch_found=25
// EXPECT: bsearch_idx=24
// EXPECT: crc=61423
// EXPECT: gcd_result=6
// EXPECT: prime_count=15
// EXPECT: fib_30=832040
// EXPECT: compute_tests=11

int printf(const char *fmt, ...);

// ---- 8x8 Matrix Multiply ----
void mat_mul_8x8(int A[8][8], int B[8][8], int C[8][8]) {
    int i, j, k;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            C[i][j] = 0;
            for (k = 0; k < 8; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int mat_trace_8x8(int M[8][8]) {
    int trace = 0;
    int i;
    for (i = 0; i < 8; i++) trace += M[i][i];
    return trace;
}

// ---- 4x4 Determinant (cofactor expansion) ----
int det3x3(int m[4][4], int skip_row, int c0, int c1, int c2) {
    int rows[3];
    int ri = 0;
    int i;
    for (i = 0; i < 4; i++) {
        if (i != skip_row) rows[ri++] = i;
    }
    int a = m[rows[0]][c0], b = m[rows[0]][c1], c = m[rows[0]][c2];
    int d = m[rows[1]][c0], e = m[rows[1]][c1], f = m[rows[1]][c2];
    int g = m[rows[2]][c0], h = m[rows[2]][c1], k = m[rows[2]][c2];
    return a*(e*k - f*h) - b*(d*k - f*g) + c*(d*h - e*g);
}

int det4x4(int m[4][4]) {
    int det = 0;
    int j;
    int sign = 1;
    for (j = 0; j < 4; j++) {
        // Cofactor: skip row 0, column j
        int cols[3];
        int ci = 0;
        int k;
        for (k = 0; k < 4; k++) {
            if (k != j) cols[ci++] = k;
        }
        det += sign * m[0][j] * det3x3(m, 0, cols[0], cols[1], cols[2]);
        sign = -sign;
    }
    return det;
}

// ---- Polynomial evaluation (Horner's method) degree 10 ----
long poly_eval(int *coeffs, int degree, long x) {
    long result = coeffs[degree];
    int i;
    for (i = degree - 1; i >= 0; i--) {
        result = result * x + coeffs[i];
    }
    return result;
}

// ---- Bubble sort ----
void bubble_sort(int *arr, int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

// ---- Binary search ----
int binary_search(int *arr, int n, int target) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

// ---- CRC-16 (simple) ----
int crc16(int *data, int len) {
    int crc = 0xFFFF;
    int i, j;
    for (i = 0; i < len; i++) {
        crc = crc ^ (data[i] & 0xFF);
        for (j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc = crc >> 1;
            }
        }
    }
    return crc & 0xFFFF;
}

// ---- GCD (Euclidean) ----
int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// ---- Count primes up to n (sieve) ----
int count_primes(int n) {
    int sieve[64];
    int i, j;
    for (i = 0; i <= n; i++) sieve[i] = 1;
    sieve[0] = 0;
    sieve[1] = 0;
    for (i = 2; i * i <= n; i++) {
        if (sieve[i]) {
            for (j = i * i; j <= n; j += i) {
                sieve[j] = 0;
            }
        }
    }
    int count = 0;
    for (i = 2; i <= n; i++) {
        if (sieve[i]) count++;
    }
    return count;
}

// ---- Fibonacci ----
int fib(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1;
    int i;
    for (i = 2; i <= n; i++) {
        int c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int main(void) {
    int tests = 0;

    // 8x8 Matrix multiply: A = identity * 2 on diagonal, B = i+j+1
    int A[8][8];
    int B[8][8];
    int C[8][8];
    int i, j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            A[i][j] = (i == j) ? 2 : 0;
            B[i][j] = i + j + 1;
        }
    }
    mat_mul_8x8(A, B, C);
    // C = 2*B, trace = sum of 2*(i+i+1) for i=0..7 = 2*sum(2i+1) = 2*(1+3+5+7+9+11+13+15)=2*64=128
    // Wait: C[i][j] = sum_k A[i][k]*B[k][j] = A[i][i]*B[i][j] = 2*B[i][j] = 2*(i+j+1)
    // trace = sum C[i][i] = sum 2*(i+i+1) = sum 2*(2i+1) = 2*sum(2i+1) for i=0..7
    // = 2*(1+3+5+7+9+11+13+15) = 2*64 = 128
    // Hmm, let me make it more interesting
    // Use A[i][j] = i+1, B[i][j] = j+1
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            A[i][j] = i + 1;
            B[i][j] = j + 1;
        }
    }
    mat_mul_8x8(A, B, C);
    // C[i][j] = (i+1) * sum_k (j+1) = ... no
    // C[i][j] = sum_k A[i][k] * B[k][j] = sum_k (i+1)*(j+1) = 8*(i+1)*(j+1)
    // trace = sum_i 8*(i+1)^2 = 8*(1+4+9+16+25+36+49+64) = 8*204 = 1632
    // Let me use simpler: A=identity, B has known trace
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            A[i][j] = (i == j) ? 1 : 0;
            B[i][j] = (i + 1) * (j + 1);
        }
    }
    mat_mul_8x8(A, B, C);
    // C = I * B = B, trace = sum (i+1)^2 = 1+4+9+16+25+36+49+64 = 204
    // Make it bigger: multiply two non-trivial matrices
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            A[i][j] = i + j;
            B[i][j] = i - j + 8;
        }
    }
    mat_mul_8x8(A, B, C);
    int trace = mat_trace_8x8(C);
    printf("matmul_trace=%d\n", trace);
    tests++;

    // 4x4 Determinant: identity matrix should give 1
    int M[4][4];
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            M[i][j] = (i == j) ? 1 : 0;
    int det = det4x4(M);
    printf("det4x4=%d\n", det);
    tests++;

    // Polynomial: x^10, evaluate at x=10 = 10^10 = 10000000000
    int coeffs[11];
    for (i = 0; i < 10; i++) coeffs[i] = 0;
    coeffs[10] = 1;
    long pval = poly_eval(coeffs, 10, 10);
    printf("poly_eval=%ld\n", pval);
    tests++;

    // Bubble sort 50 elements (reverse order)
    int arr[50];
    for (i = 0; i < 50; i++) {
        arr[i] = 50 - i;
    }
    bubble_sort(arr, 50);
    printf("sorted_first=%d\n", arr[0]);
    tests++;
    printf("sorted_last=%d\n", arr[49]);
    tests++;

    // Binary search for 25 in sorted array
    int idx = binary_search(arr, 50, 25);
    printf("bsearch_found=%d\n", arr[idx]);
    tests++;
    printf("bsearch_idx=%d\n", idx);
    tests++;

    // CRC of array 1..50
    int crc_data[50];
    for (i = 0; i < 50; i++) crc_data[i] = i + 1;
    int crc_val = crc16(crc_data, 50);
    printf("crc=%d\n", crc_val);
    tests++;

    // GCD
    int g = gcd(48, 18);
    printf("gcd_result=%d\n", g);
    tests++;

    // Count primes up to 50
    int pc = count_primes(50);
    printf("prime_count=%d\n", pc);
    tests++;

    // Fibonacci
    int f30 = fib(30);
    printf("fib_30=%d\n", f30);
    tests++;

    printf("compute_tests=%d\n", tests);

    return 0;
}
