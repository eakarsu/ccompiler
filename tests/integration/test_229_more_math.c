int printf(const char *fmt, ...);
// EXPECT: === fibonacci ===\nfib(0)=0\nfib(1)=1\nfib(2)=1\nfib(3)=2\nfib(4)=3\nfib(5)=5\nfib(6)=8\nfib(7)=13\nfib(8)=21\nfib(9)=34\nfib(10)=55\nfib(11)=89\nfib(12)=144\nfib(13)=233\nfib(14)=377\nfib(15)=610\n=== tribonacci ===\ntrib(0)=0\ntrib(1)=0\ntrib(2)=1\ntrib(3)=1\ntrib(4)=2\ntrib(5)=4\ntrib(6)=7\ntrib(7)=13\ntrib(8)=24\ntrib(9)=44\ntrib(10)=81\n=== lucas ===\nlucas(0)=2\nlucas(1)=1\nlucas(2)=3\nlucas(3)=4\nlucas(4)=7\nlucas(5)=11\nlucas(6)=18\nlucas(7)=29\nlucas(8)=47\n=== binomial ===\nC(10,3)=120\nC(10,5)=252\nC(7,2)=21\n=== derangements ===\nD(0)=1\nD(1)=0\nD(2)=1\nD(3)=2\nD(4)=9\nD(5)=44\nD(6)=265\nD(7)=1854\n=== stirling ===\nS(4,2)=7\nS(5,3)=25\n=== bell ===\nB(0)=1\nB(1)=1\nB(2)=2\nB(3)=5\nB(4)=15\nB(5)=52\nB(6)=203\nB(7)=877
// Test 229: More math utilities - Fibonacci variants, combinatorics

int fib_bottom_up(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    int prev2 = 0;
    int prev1 = 1;
    int i = 2;
    while (i <= n) {
        int curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
        i = i + 1;
    }
    return prev1;
}

int tribonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;
    int a = 0;
    int b = 0;
    int c = 1;
    int i = 3;
    while (i <= n) {
        int next = a + b + c;
        a = b;
        b = c;
        c = next;
        i = i + 1;
    }
    return c;
}

int lucas(int n) {
    if (n == 0) return 2;
    if (n == 1) return 1;
    int a = 2;
    int b = 1;
    int i = 2;
    while (i <= n) {
        int c = a + b;
        a = b;
        b = c;
        i = i + 1;
    }
    return b;
}

int binomial(int n, int k) {
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    int result = 1;
    int i = 0;
    while (i < k) {
        result = result * (n - i);
        result = result / (i + 1);
        i = i + 1;
    }
    return result;
}

int derangements(int n) {
    if (n == 0) return 1;
    if (n == 1) return 0;
    int d0 = 1;
    int d1 = 0;
    int i = 2;
    while (i <= n) {
        int d2 = (i - 1) * (d1 + d0);
        d0 = d1;
        d1 = d2;
        i = i + 1;
    }
    return d1;
}

int stirling2(int n, int k) {
    if (n == 0 && k == 0) return 1;
    if (n == 0 || k == 0) return 0;
    if (k == 1 || k == n) return 1;
    return k * stirling2(n - 1, k) + stirling2(n - 1, k - 1);
}

int bell_number(int n) {
    int triangle[15][15];
    triangle[0][0] = 1;
    int i = 1;
    while (i <= n) {
        triangle[i][0] = triangle[i - 1][i - 1];
        int j = 1;
        while (j <= i) {
            triangle[i][j] = triangle[i][j - 1] + triangle[i - 1][j - 1];
            j = j + 1;
        }
        i = i + 1;
    }
    return triangle[n][0];
}

int main(void) {
    printf("=== fibonacci ===\n");
    int i = 0;
    while (i <= 15) {
        printf("fib(%d)=%d\n", i, fib_bottom_up(i));
        i = i + 1;
    }

    printf("=== tribonacci ===\n");
    i = 0;
    while (i <= 10) {
        printf("trib(%d)=%d\n", i, tribonacci(i));
        i = i + 1;
    }

    printf("=== lucas ===\n");
    i = 0;
    while (i <= 8) {
        printf("lucas(%d)=%d\n", i, lucas(i));
        i = i + 1;
    }

    printf("=== binomial ===\n");
    printf("C(10,3)=%d\n", binomial(10, 3));
    printf("C(10,5)=%d\n", binomial(10, 5));
    printf("C(7,2)=%d\n", binomial(7, 2));

    printf("=== derangements ===\n");
    i = 0;
    while (i <= 7) {
        printf("D(%d)=%d\n", i, derangements(i));
        i = i + 1;
    }

    printf("=== stirling ===\n");
    printf("S(4,2)=%d\n", stirling2(4, 2));
    printf("S(5,3)=%d\n", stirling2(5, 3));

    printf("=== bell ===\n");
    i = 0;
    while (i <= 7) {
        printf("B(%d)=%d\n", i, bell_number(i));
        i = i + 1;
    }

    return 0;
}
