// EXPECT: === Math Functions ===
// EXPECT: --- Absolute Value ---
// EXPECT: abs(5)=5
// EXPECT: abs(-5)=5
// EXPECT: abs(0)=0
// EXPECT: abs(-100)=100
// EXPECT: --- Min / Max ---
// EXPECT: min(3,7)=3
// EXPECT: min(7,3)=3
// EXPECT: min(-2,5)=-2
// EXPECT: max(3,7)=7
// EXPECT: max(7,3)=7
// EXPECT: max(-2,5)=5
// EXPECT: min3(4,2,6)=2
// EXPECT: max3(4,2,6)=6
// EXPECT: --- GCD ---
// EXPECT: gcd(12,8)=4
// EXPECT: gcd(17,13)=1
// EXPECT: gcd(100,75)=25
// EXPECT: gcd(0,5)=5
// EXPECT: gcd(7,0)=7
// EXPECT: gcd(48,36)=12
// EXPECT: --- LCM ---
// EXPECT: lcm(4,6)=12
// EXPECT: lcm(3,5)=15
// EXPECT: lcm(12,8)=24
// EXPECT: lcm(7,1)=7
// EXPECT: lcm(9,6)=18
// EXPECT: --- Power ---
// EXPECT: pow(2,0)=1
// EXPECT: pow(2,1)=2
// EXPECT: pow(2,10)=1024
// EXPECT: pow(3,5)=243
// EXPECT: pow(5,3)=125
// EXPECT: pow(7,2)=49
// EXPECT: pow(1,100)=1
// EXPECT: --- Integer Sqrt (Newton) ---
// EXPECT: isqrt(0)=0
// EXPECT: isqrt(1)=1
// EXPECT: isqrt(4)=2
// EXPECT: isqrt(9)=3
// EXPECT: isqrt(16)=4
// EXPECT: isqrt(25)=5
// EXPECT: isqrt(26)=5
// EXPECT: isqrt(99)=9
// EXPECT: isqrt(100)=10
// EXPECT: isqrt(144)=12
// EXPECT: isqrt(10000)=100
// EXPECT: --- Factorial ---
// EXPECT: fact(0)=1
// EXPECT: fact(1)=1
// EXPECT: fact(5)=120
// EXPECT: fact(7)=5040
// EXPECT: fact(10)=3628800
// EXPECT: fact(12)=479001600
// EXPECT: --- Clamp ---
// EXPECT: clamp(5,0,10)=5
// EXPECT: clamp(-3,0,10)=0
// EXPECT: clamp(15,0,10)=10
// EXPECT: --- Sign ---
// EXPECT: sign(-42)=-1
// EXPECT: sign(0)=0
// EXPECT: sign(42)=1
// EXPECT: === Done ===

int printf(const char *fmt, ...);

int my_abs(int x) {
    if (x < 0) return -x;
    return x;
}

int my_min(int a, int b) {
    if (a < b) return a;
    return b;
}

int my_max(int a, int b) {
    if (a > b) return a;
    return b;
}

int min3(int a, int b, int c) {
    return my_min(a, my_min(b, c));
}

int max3(int a, int b, int c) {
    return my_max(a, my_max(b, c));
}

int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int lcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    int g = gcd(a, b);
    return (a / g) * b;
}

int ipow(int base, int exp) {
    int result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result = result * base;
        }
        base = base * base;
        exp = exp >> 1;
    }
    return result;
}

int isqrt(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    int x = n;
    int y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

int factorial(int n) {
    if (n <= 1) return 1;
    int result = 1;
    int i = 2;
    while (i <= n) {
        result = result * i;
        i = i + 1;
    }
    return result;
}

int clamp(int x, int lo, int hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

int sign(int x) {
    if (x < 0) return -1;
    if (x > 0) return 1;
    return 0;
}

int main(void) {
    printf("=== Math Functions ===\n");

    printf("--- Absolute Value ---\n");
    printf("abs(5)=%d\n", my_abs(5));
    printf("abs(-5)=%d\n", my_abs(-5));
    printf("abs(0)=%d\n", my_abs(0));
    printf("abs(-100)=%d\n", my_abs(-100));

    printf("--- Min / Max ---\n");
    printf("min(3,7)=%d\n", my_min(3, 7));
    printf("min(7,3)=%d\n", my_min(7, 3));
    printf("min(-2,5)=%d\n", my_min(-2, 5));
    printf("max(3,7)=%d\n", my_max(3, 7));
    printf("max(7,3)=%d\n", my_max(7, 3));
    printf("max(-2,5)=%d\n", my_max(-2, 5));
    printf("min3(4,2,6)=%d\n", min3(4, 2, 6));
    printf("max3(4,2,6)=%d\n", max3(4, 2, 6));

    printf("--- GCD ---\n");
    printf("gcd(12,8)=%d\n", gcd(12, 8));
    printf("gcd(17,13)=%d\n", gcd(17, 13));
    printf("gcd(100,75)=%d\n", gcd(100, 75));
    printf("gcd(0,5)=%d\n", gcd(0, 5));
    printf("gcd(7,0)=%d\n", gcd(7, 0));
    printf("gcd(48,36)=%d\n", gcd(48, 36));

    printf("--- LCM ---\n");
    printf("lcm(4,6)=%d\n", lcm(4, 6));
    printf("lcm(3,5)=%d\n", lcm(3, 5));
    printf("lcm(12,8)=%d\n", lcm(12, 8));
    printf("lcm(7,1)=%d\n", lcm(7, 1));
    printf("lcm(9,6)=%d\n", lcm(9, 6));

    printf("--- Power ---\n");
    printf("pow(2,0)=%d\n", ipow(2, 0));
    printf("pow(2,1)=%d\n", ipow(2, 1));
    printf("pow(2,10)=%d\n", ipow(2, 10));
    printf("pow(3,5)=%d\n", ipow(3, 5));
    printf("pow(5,3)=%d\n", ipow(5, 3));
    printf("pow(7,2)=%d\n", ipow(7, 2));
    printf("pow(1,100)=%d\n", ipow(1, 100));

    printf("--- Integer Sqrt (Newton) ---\n");
    printf("isqrt(0)=%d\n", isqrt(0));
    printf("isqrt(1)=%d\n", isqrt(1));
    printf("isqrt(4)=%d\n", isqrt(4));
    printf("isqrt(9)=%d\n", isqrt(9));
    printf("isqrt(16)=%d\n", isqrt(16));
    printf("isqrt(25)=%d\n", isqrt(25));
    printf("isqrt(26)=%d\n", isqrt(26));
    printf("isqrt(99)=%d\n", isqrt(99));
    printf("isqrt(100)=%d\n", isqrt(100));
    printf("isqrt(144)=%d\n", isqrt(144));
    printf("isqrt(10000)=%d\n", isqrt(10000));

    printf("--- Factorial ---\n");
    printf("fact(0)=%d\n", factorial(0));
    printf("fact(1)=%d\n", factorial(1));
    printf("fact(5)=%d\n", factorial(5));
    printf("fact(7)=%d\n", factorial(7));
    printf("fact(10)=%d\n", factorial(10));
    printf("fact(12)=%d\n", factorial(12));

    printf("--- Clamp ---\n");
    printf("clamp(5,0,10)=%d\n", clamp(5, 0, 10));
    printf("clamp(-3,0,10)=%d\n", clamp(-3, 0, 10));
    printf("clamp(15,0,10)=%d\n", clamp(15, 0, 10));

    printf("--- Sign ---\n");
    printf("sign(-42)=%d\n", sign(-42));
    printf("sign(0)=%d\n", sign(0));
    printf("sign(42)=%d\n", sign(42));

    printf("=== Done ===\n");
    return 0;
}
