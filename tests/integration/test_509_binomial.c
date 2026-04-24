int printf(const char *fmt, ...);
// EXPECT: Binomial coefficients C(n,k):\nn= 0: 1 \nn= 1: 1 1 \nn= 2: 1 2 1 \nn= 3: 1 3 3 1 \nn= 4: 1 4 6 4 1 \nn= 5: 1 5 10 10 5 1 \nn= 6: 1 6 15 20 15 6 1 \nn= 7: 1 7 21 35 35 21 7 1 \nn= 8: 1 8 28 56 70 56 28 8 1 \nn= 9: 1 9 36 84 126 126 84 36 9 1 \nn=10: 1 10 45 120 210 252 210 120 45 10 1 \n\nVandermonde: C(9,3) = 84, sum = 84\n\nBinomial theorem check (sum of row = 2^n):\nn=0: sum=1, 2^n=1, match=1\nn=1: sum=2, 2^n=2, match=1\nn=2: sum=4, 2^n=4, match=1\nn=3: sum=8, 2^n=8, match=1\nn=4: sum=16, 2^n=16, match=1\nn=5: sum=32, 2^n=32, match=1\nn=6: sum=64, 2^n=64, match=1\nn=7: sum=128, 2^n=128, match=1\nn=8: sum=256, 2^n=256, match=1\nn=9: sum=512, 2^n=512, match=1\nn=10: sum=1024, 2^n=1024, match=1\nn=11: sum=2048, 2^n=2048, match=1\nn=12: sum=4096, 2^n=4096, match=1\n\n(3+2)^4 expansion:\nC(4,0)*3^4*2^0 = 1*81*1 = 81\nC(4,1)*3^3*2^1 = 4*27*2 = 216\nC(4,2)*3^2*2^2 = 6*9*4 = 216\nC(4,3)*3^1*2^3 = 4*3*8 = 96\nC(4,4)*3^0*2^4 = 1*1*16 = 16\nTotal: 625 (should be 625)
// Test: Binomial coefficients

int binom(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    // Use symmetry
    if (k > n - k) k = n - k;
    int result = 1;
    int i;
    for (i = 0; i < k; i++) {
        result = result * (n - i);
        result = result / (i + 1);
    }
    return result;
}

int main(void) {
    int i, j, n, k;

    printf("Binomial coefficients C(n,k):\n");
    for (n = 0; n <= 10; n++) {
        printf("n=%2d: ", n);
        for (k = 0; k <= n; k++) {
            printf("%d ", binom(n, k));
        }
        printf("\n");
    }

    // Vandermonde's identity: C(m+n,r) = sum C(m,k)*C(n,r-k)
    int m_val = 5, n_val = 4, r_val = 3;
    int lhs = binom(m_val + n_val, r_val);
    int rhs = 0;
    for (k = 0; k <= r_val; k++) {
        rhs = rhs + binom(m_val, k) * binom(n_val, r_val - k);
    }
    printf("\nVandermonde: C(%d,%d) = %d, sum = %d\n",
           m_val + n_val, r_val, lhs, rhs);

    // Binomial theorem: (1+1)^n = sum C(n,k)
    printf("\nBinomial theorem check (sum of row = 2^n):\n");
    for (n = 0; n <= 12; n++) {
        int sum = 0;
        for (k = 0; k <= n; k++) {
            sum = sum + binom(n, k);
        }
        int pow2 = 1;
        for (i = 0; i < n; i++) pow2 = pow2 * 2;
        printf("n=%d: sum=%d, 2^n=%d, match=%d\n", n, sum, pow2, sum == pow2);
    }

    // Compute (x+y)^4 coefficients with x=3, y=2
    printf("\n(3+2)^4 expansion:\n");
    int total = 0;
    int x = 3, y = 2;
    for (k = 0; k <= 4; k++) {
        int xp = 1, yp = 1;
        for (i = 0; i < 4 - k; i++) xp = xp * x;
        for (i = 0; i < k; i++) yp = yp * y;
        int term = binom(4, k) * xp * yp;
        printf("C(4,%d)*3^%d*2^%d = %d*%d*%d = %d\n",
               k, 4 - k, k, binom(4, k), xp, yp, term);
        total = total + term;
    }
    printf("Total: %d (should be %d)\n", total, 5 * 5 * 5 * 5);

    return 0;
}
