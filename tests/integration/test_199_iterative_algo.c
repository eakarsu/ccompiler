int printf(const char *fmt, ...);
// EXPECT: GCD table:\ngcd(6,6)=6 gcd(6,8)=2 gcd(6,10)=2 gcd(6,12)=6 \ngcd(8,6)=2 gcd(8,8)=8 gcd(8,10)=2 gcd(8,12)=4 \ngcd(10,6)=2 gcd(10,8)=2 gcd(10,10)=10 gcd(10,12)=2 \ngcd(12,6)=6 gcd(12,8)=4 gcd(12,10)=2 gcd(12,12)=12 \nPower mod:\n2^10 mod 1000=24\n3^7 mod 100=87\n5^13 mod 97=29\n7^20 mod 1000=1\nFibonacci: 0 1 1 2 3 5 8 13 21 34 55 89 144 233 377 610 \nDigital roots:\ndr(1)=1\ndr(9)=9\ndr(15)=6\ndr(99)=9\ndr(123)=6\ndr(9999)=9\ndr(12345)=6\ndr(493)=7\nExtended Euclidean (iterative):\ngcd(35,15)=5 x=1 y=-2 check=5\ngcd(48,18)=6 x=-1 y=3 check=6\ngcd(100,35)=5 x=-1 y=3 check=5\ngcd(252,198)=18 x=4 y=-5 check=18
int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int power_mod(int base, int exp, int mod) {
    int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp / 2;
        base = (base * base) % mod;
    }
    return result;
}

int fib_iter(int n) {
    if (n <= 1) return n;
    int a = 0;
    int b = 1;
    int i;
    for (i = 2; i <= n; i++) {
        int c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int digit_sum_repeat(int n) {
    while (n >= 10) {
        int sum = 0;
        while (n > 0) {
            sum = sum + n % 10;
            n = n / 10;
        }
        n = sum;
    }
    return n;
}

int main(void) {
    int i;
    int j;

    printf("GCD table:\n");
    for (i = 6; i <= 12; i = i + 2) {
        for (j = 6; j <= 12; j = j + 2) {
            printf("gcd(%d,%d)=%d ", i, j, gcd(i, j));
        }
        printf("\n");
    }

    printf("Power mod:\n");
    printf("2^10 mod 1000=%d\n", power_mod(2, 10, 1000));
    printf("3^7 mod 100=%d\n", power_mod(3, 7, 100));
    printf("5^13 mod 97=%d\n", power_mod(5, 13, 97));
    printf("7^20 mod 1000=%d\n", power_mod(7, 20, 1000));

    printf("Fibonacci: ");
    for (i = 0; i <= 15; i++) {
        printf("%d ", fib_iter(i));
    }
    printf("\n");

    printf("Digital roots:\n");
    int drvals[8];
    drvals[0] = 1; drvals[1] = 9; drvals[2] = 15; drvals[3] = 99;
    drvals[4] = 123; drvals[5] = 9999; drvals[6] = 12345; drvals[7] = 493;
    for (i = 0; i < 8; i++) {
        printf("dr(%d)=%d\n", drvals[i], digit_sum_repeat(drvals[i]));
    }

    printf("Extended Euclidean (iterative):\n");
    int pairs[4][2];
    pairs[0][0] = 35; pairs[0][1] = 15;
    pairs[1][0] = 48; pairs[1][1] = 18;
    pairs[2][0] = 100; pairs[2][1] = 35;
    pairs[3][0] = 252; pairs[3][1] = 198;
    for (i = 0; i < 4; i++) {
        int a = pairs[i][0];
        int b = pairs[i][1];
        int x0 = 1; int x1 = 0;
        int y0 = 0; int y1 = 1;
        int ta = a;
        int tb = b;
        while (tb != 0) {
            int q = ta / tb;
            int temp = tb; tb = ta - q * tb; ta = temp;
            temp = x1; x1 = x0 - q * x1; x0 = temp;
            temp = y1; y1 = y0 - q * y1; y0 = temp;
        }
        printf("gcd(%d,%d)=%d x=%d y=%d check=%d\n",
               a, b, ta, x0, y0, a * x0 + b * y0);
    }

    return 0;
}
