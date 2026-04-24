// EXPECT: vla_basic: 0 1 2 3 4
// EXPECT: vla_sum=10
// EXPECT: vla_size5=1
// EXPECT: vla_size10=1
// EXPECT: vla_loop_sum=20
// EXPECT: vla_computed: n=7 sum=21
// EXPECT: vla_fill=55
// EXPECT: vla_double: 0 2 4 6
// EXPECT: vla_reverse: 4 3 2 1 0
// EXPECT: vla_large_ok=1
// EXPECT: vla_param_sum=15
// EXPECT: vla_squares=30
// EXPECT: vla_copy_ok=1
// EXPECT: vla_fib=34
// EXPECT: vla_accum=150
// EXPECT: all_passed=1

int printf(const char *fmt, ...);

// Helper: sum an array
int sum_array(int n, int arr[]) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) {
        s = s + arr[i];
    }
    return s;
}

// Function that uses VLA parameter
int vla_param_func(int n) {
    int arr[n];
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
    return sum_array(n, arr);
}

// Compute squares into VLA
int sum_of_squares(int n) {
    int sq[n];
    int i;
    for (i = 0; i < n; i++) {
        sq[i] = i * i;
    }
    return sum_array(n, sq);
}

// Fibonacci via VLA
int fib_vla(int n) {
    int f[n];
    f[0] = 0;
    f[1] = 1;
    int i;
    for (i = 2; i < n; i++) {
        f[i] = f[i - 1] + f[i - 2];
    }
    return f[n - 1];
}

int main(void) {
    int passed = 1;

    // Basic VLA
    int n = 5;
    int vla1[n];
    int i;
    for (i = 0; i < n; i++) {
        vla1[i] = i;
    }
    printf("vla_basic:");
    for (i = 0; i < n; i++) {
        printf(" %d", vla1[i]);
    }
    printf("\n");
    if (vla1[0] != 0 || vla1[4] != 4) passed = 0;

    // VLA sum
    int vla_sum = 0;
    for (i = 0; i < n; i++) {
        vla_sum = vla_sum + vla1[i];
    }
    printf("vla_sum=%d\n", vla_sum);
    if (vla_sum != 10) passed = 0;

    // VLA with different sizes
    int s5 = 5;
    int a5[s5];
    for (i = 0; i < s5; i++) a5[i] = i;
    printf("vla_size5=%d\n", (a5[4] == 4) ? 1 : 0);
    if (a5[4] != 4) passed = 0;

    int s10 = 10;
    int a10[s10];
    for (i = 0; i < s10; i++) a10[i] = i * 10;
    printf("vla_size10=%d\n", (a10[9] == 90) ? 1 : 0);
    if (a10[9] != 90) passed = 0;

    // VLA in loop: each iteration creates a different-sized VLA
    int loop_sum = 0;
    int iter;
    for (iter = 1; iter <= 5; iter++) {
        int larr[iter];
        int j;
        for (j = 0; j < iter; j++) {
            larr[j] = j;
        }
        // Sum: 0, 0+1, 0+1+2, 0+1+2+3, 0+1+2+3+4
        //     = 0, 1, 3, 6, 10 => total 20
        for (j = 0; j < iter; j++) {
            loop_sum = loop_sum + larr[j];
        }
    }
    // Actually: 0 + 1 + 3 + 6 + 10 = 20
    printf("vla_loop_sum=%d\n", loop_sum);

    // VLA with computed size
    int base = 3;
    int extra = 4;
    int cn = base + extra; // 7
    int cvla[cn];
    for (i = 0; i < cn; i++) {
        cvla[i] = i;
    }
    int csum = sum_array(cn, cvla);
    printf("vla_computed: n=%d sum=%d\n", cn, csum);
    if (cn != 7 || csum != 21) passed = 0;

    // VLA fill with triangular number
    int tn = 10;
    int tri[tn];
    for (i = 0; i < tn; i++) {
        tri[i] = i + 1;
    }
    int tri_sum = sum_array(tn, tri);
    printf("vla_fill=%d\n", tri_sum);
    if (tri_sum != 55) passed = 0;

    // VLA with doubling values
    int dn = 4;
    int dvla[dn];
    for (i = 0; i < dn; i++) {
        dvla[i] = i * 2;
    }
    printf("vla_double:");
    for (i = 0; i < dn; i++) {
        printf(" %d", dvla[i]);
    }
    printf("\n");

    // VLA reverse fill
    int rn = 5;
    int rvla[rn];
    for (i = 0; i < rn; i++) {
        rvla[i] = rn - 1 - i;
    }
    printf("vla_reverse:");
    for (i = 0; i < rn; i++) {
        printf(" %d", rvla[i]);
    }
    printf("\n");
    if (rvla[0] != 4 || rvla[4] != 0) passed = 0;

    // Larger VLA
    int ln = 100;
    int lvla[ln];
    for (i = 0; i < ln; i++) {
        lvla[i] = i;
    }
    printf("vla_large_ok=%d\n", (lvla[99] == 99) ? 1 : 0);
    if (lvla[99] != 99) passed = 0;

    // VLA in called function
    int ps = vla_param_func(5); // 1+2+3+4+5 = 15
    printf("vla_param_sum=%d\n", ps);
    if (ps != 15) passed = 0;

    // Sum of squares via VLA
    int ssq = sum_of_squares(5); // 0+1+4+9+16 = 30
    printf("vla_squares=%d\n", ssq);
    if (ssq != 30) passed = 0;

    // Copy between VLAs
    int cn2 = 6;
    int src[cn2];
    int dst[cn2];
    for (i = 0; i < cn2; i++) src[i] = i * 11;
    for (i = 0; i < cn2; i++) dst[i] = src[i];
    int copy_ok = 1;
    for (i = 0; i < cn2; i++) {
        if (dst[i] != i * 11) copy_ok = 0;
    }
    printf("vla_copy_ok=%d\n", copy_ok);
    if (!copy_ok) passed = 0;

    // Fibonacci via VLA
    int fv = fib_vla(10); // fib(9) = 55
    printf("vla_fib=%d\n", fv);
    if (fv != 34) passed = 0;

    // Accumulator pattern with VLA
    int an = 5;
    int acc[an];
    acc[0] = 10;
    for (i = 1; i < an; i++) {
        acc[i] = acc[i - 1] + 10 * (i + 1);
    }
    // acc[0]=10, acc[1]=10+20=30, acc[2]=30+30=60, acc[3]=60+40=100, acc[4]=100+50=150
    printf("vla_accum=%d\n", acc[an - 1]);
    if (acc[an - 1] != 150) passed = 0;

    printf("all_passed=%d\n", passed);
    return passed ? 0 : 1;
}
