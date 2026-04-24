int printf(const char *fmt, ...);
// EXPECT: 1+2=3
// 10-7=3
// 6*7=42
// 100/3=33
// 100%3=1
// neg5=-5
// doubleneg5=5
// 2+3*4=14
// (2+3)*4=20
// 10/3*3=9
// 10%3+1=2
// 1+2+3+4+5=15
// 10-3-2-1=4
// 2*3*4=24
// 100/2/5=10
// 17%5%2=0
// neg_neg10=10
// neg_sum=-7
// 3+4*5-2=21
// (3+4)*(5-2)=21
// 10/(2+3)=2
// (10+20)/(3+2)=6
// 1*2+3*4+5*6=44
// (1+2)*(3+4)*1=21
// 100-50-25-10=15
// 1000/10/10=10
// 7*7*7=343
// 123+456=579
// 999-123=876
// 50*20=1000
// 1000000/1000=1000
// 13%7=6
// 25%10=5
// neg3mul4=-12
// 4mulneg3=-12
// neg3mulneg4=12
// neg10div3=-3
// neg10mod3=-1
// 10divneg3=-3
// 0+0=0
// 0*100=0
// 0/5=0
// 0%5=0
// 1*1=1
// 5+neg3=2
// chain=210
// pow10_4=10000
// tri10=55
// sumcubes10=3025
// sumsq10=385
// gcd(12,8)=4
// lcm(4,6)=12
// fact5=120
// fib10=55
// isprime7=1
// isprime10=0
// digitsum(1234)=10
// reverse(1234)=4321
// collatz(27)=111
// poly(3)=34
// isqrt(26)=5
// perfect_sq(25)=1
// perfect_sq(26)=0
// mulbyadd(7,8)=56
// divbysub(100,7)=14
// sign(-5)=-1
// sign(0)=0
// sign(5)=1
// clamp(15,0,10)=10
// clamp(-3,0,10)=0
// abs(-42)=42
// max(3,7)=7
// min(3,7)=3
// comb(10,3)=120
// digroot(9999)=9
// popcount(255)=8

int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return a - b;
}

int mul(int a, int b) {
    return a * b;
}

int divide(int a, int b) {
    return a / b;
}

int mod(int a, int b) {
    return a % b;
}

int negate(int x) {
    return -x;
}

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int max(int a, int b) {
    if (a > b) return a;
    return b;
}

int min(int a, int b) {
    if (a < b) return a;
    return b;
}

int square(int x) {
    return x * x;
}

int cube(int x) {
    return x * x * x;
}

int power(int base, int exp) {
    int result;
    int i;
    result = 1;
    i = 0;
    while (i < exp) {
        result = result * base;
        i = i + 1;
    }
    return result;
}

int factorial(int n) {
    int result;
    int i;
    result = 1;
    i = 1;
    while (i <= n) {
        result = result * i;
        i = i + 1;
    }
    return result;
}

int sum_range(int a, int b) {
    int s;
    int i;
    s = 0;
    i = a;
    while (i <= b) {
        s = s + i;
        i = i + 1;
    }
    return s;
}

int gcd(int a, int b) {
    int t;
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int lcm(int a, int b) {
    return a / gcd(a, b) * b;
}

int is_even(int x) {
    return (x % 2) == 0;
}

int is_odd(int x) {
    return (x % 2) != 0;
}

int sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

int clamp(int x, int lo, int hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

int sum_of_digits(int n) {
    int s;
    if (n < 0) n = -n;
    s = 0;
    while (n > 0) {
        s = s + (n % 10);
        n = n / 10;
    }
    return s;
}

int reverse_number(int n) {
    int rev;
    int neg;
    neg = 0;
    if (n < 0) {
        neg = 1;
        n = -n;
    }
    rev = 0;
    while (n > 0) {
        rev = rev * 10 + (n % 10);
        n = n / 10;
    }
    if (neg) return -rev;
    return rev;
}

int count_digits(int n) {
    int c;
    if (n == 0) return 1;
    if (n < 0) n = -n;
    c = 0;
    while (n > 0) {
        c = c + 1;
        n = n / 10;
    }
    return c;
}

int is_prime(int n) {
    int i;
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    i = 3;
    while (i * i <= n) {
        if (n % i == 0) return 0;
        i = i + 2;
    }
    return 1;
}

int fibonacci(int n) {
    int a;
    int b;
    int t;
    int i;
    if (n <= 0) return 0;
    if (n == 1) return 1;
    a = 0;
    b = 1;
    i = 2;
    while (i <= n) {
        t = a + b;
        a = b;
        b = t;
        i = i + 1;
    }
    return b;
}

int combination(int n, int r) {
    int num;
    int den;
    int i;
    if (r > n) return 0;
    if (r == 0) return 1;
    if (r > n - r) r = n - r;
    num = 1;
    den = 1;
    i = 0;
    while (i < r) {
        num = num * (n - i);
        den = den * (i + 1);
        i = i + 1;
    }
    return num / den;
}

int triangle_number(int n) {
    return n * (n + 1) / 2;
}

int sum_of_squares(int n) {
    int s;
    int i;
    s = 0;
    i = 1;
    while (i <= n) {
        s = s + i * i;
        i = i + 1;
    }
    return s;
}

int sum_of_cubes(int n) {
    int s;
    int i;
    s = 0;
    i = 1;
    while (i <= n) {
        s = s + i * i * i;
        i = i + 1;
    }
    return s;
}

int chain_multiply(int a, int b, int c, int d, int e) {
    return a * b * c * d * e;
}

int polynomial(int x) {
    return 3 * x * x + 2 * x + 1;
}

int collatz_steps(int n) {
    int steps;
    steps = 0;
    while (n != 1) {
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
        steps = steps + 1;
    }
    return steps;
}

int digital_root(int n) {
    if (n < 0) n = -n;
    while (n >= 10) {
        n = sum_of_digits(n);
    }
    return n;
}

int int_sqrt(int n) {
    int i;
    i = 0;
    while ((i + 1) * (i + 1) <= n) {
        i = i + 1;
    }
    return i;
}

int is_perfect_square(int n) {
    int s;
    s = int_sqrt(n);
    return s * s == n;
}

int popcount(int n) {
    int c;
    c = 0;
    while (n > 0) {
        c = c + (n % 2);
        n = n / 2;
    }
    return c;
}

int multiply_by_add(int a, int b) {
    int result;
    int neg;
    int i;
    neg = 0;
    if (a < 0) {
        a = -a;
        neg = 1 - neg;
    }
    if (b < 0) {
        b = -b;
        neg = 1 - neg;
    }
    result = 0;
    i = 0;
    while (i < b) {
        result = result + a;
        i = i + 1;
    }
    if (neg) return -result;
    return result;
}

int divide_by_sub(int a, int b) {
    int count;
    int neg;
    neg = 0;
    if (a < 0) {
        a = -a;
        neg = 1 - neg;
    }
    if (b < 0) {
        b = -b;
        neg = 1 - neg;
    }
    count = 0;
    while (a >= b) {
        a = a - b;
        count = count + 1;
    }
    if (neg) return -count;
    return count;
}

int main(void) {
    // Basic arithmetic
    printf("%d+%d=%d\n", 1, 2, 1 + 2);
    printf("%d-%d=%d\n", 10, 7, 10 - 7);
    printf("%d*%d=%d\n", 6, 7, 6 * 7);
    printf("%d/%d=%d\n", 100, 3, 100 / 3);
    printf("%d%%%d=%d\n", 100, 3, 100 % 3);

    // Negation
    printf("neg%d=%d\n", 5, -5);
    printf("doubleneg%d=%d\n", 5, -(-5));

    // Precedence
    printf("2+3*4=%d\n", 2 + 3 * 4);
    printf("(2+3)*4=%d\n", (2 + 3) * 4);
    printf("10/3*3=%d\n", 10 / 3 * 3);
    printf("10%%3+1=%d\n", 10 % 3 + 1);

    // Chained operations
    printf("1+2+3+4+5=%d\n", 1 + 2 + 3 + 4 + 5);
    printf("10-3-2-1=%d\n", 10 - 3 - 2 - 1);
    printf("2*3*4=%d\n", 2 * 3 * 4);
    printf("100/2/5=%d\n", 100 / 2 / 5);
    printf("17%%5%%2=%d\n", 17 % 5 % 2);

    // Negation combos
    printf("neg_neg10=%d\n", -(-10));
    printf("neg_sum=%d\n", -(3 + 4));

    // Mixed precedence
    printf("3+4*5-2=%d\n", 3 + 4 * 5 - 2);
    printf("(3+4)*(5-2)=%d\n", (3 + 4) * (5 - 2));
    printf("10/(2+3)=%d\n", 10 / (2 + 3));
    printf("(10+20)/(3+2)=%d\n", (10 + 20) / (3 + 2));
    printf("1*2+3*4+5*6=%d\n", 1 * 2 + 3 * 4 + 5 * 6);
    printf("(1+2)*(3+4)*1=%d\n", (1 + 2) * (3 + 4) * 1);

    // Subtraction chains
    printf("100-50-25-10=%d\n", 100 - 50 - 25 - 10);
    printf("1000/10/10=%d\n", 1000 / 10 / 10);
    printf("7*7*7=%d\n", 7 * 7 * 7);

    // Larger numbers
    printf("123+456=%d\n", 123 + 456);
    printf("999-123=%d\n", 999 - 123);
    printf("50*20=%d\n", 50 * 20);
    printf("1000000/1000=%d\n", 1000000 / 1000);
    printf("13%%7=%d\n", 13 % 7);
    printf("25%%10=%d\n", 25 % 10);

    // Negative multiplication
    printf("neg3mul4=%d\n", -3 * 4);
    printf("4mulneg3=%d\n", 4 * -3);
    printf("neg3mulneg4=%d\n", (-3) * (-4));

    // Negative division and mod
    printf("neg10div3=%d\n", -10 / 3);
    printf("neg10mod3=%d\n", -10 % 3);
    printf("10divneg3=%d\n", 10 / -3);

    // Zero operations
    printf("0+0=%d\n", 0 + 0);
    printf("0*100=%d\n", 0 * 100);
    printf("0/5=%d\n", 0 / 5);
    printf("0%%5=%d\n", 0 % 5);

    // Identity
    printf("1*1=%d\n", 1 * 1);
    printf("5+neg3=%d\n", 5 + (-3));

    // Using helper functions
    printf("chain=%d\n", chain_multiply(1, 2, 3, 5, 7));
    printf("pow10_4=%d\n", power(10, 4));
    printf("tri10=%d\n", triangle_number(10));
    printf("sumcubes10=%d\n", sum_of_cubes(10));
    printf("sumsq10=%d\n", sum_of_squares(10));
    printf("gcd(12,8)=%d\n", gcd(12, 8));
    printf("lcm(4,6)=%d\n", lcm(4, 6));
    printf("fact5=%d\n", factorial(5));
    printf("fib10=%d\n", fibonacci(10));
    printf("isprime7=%d\n", is_prime(7));
    printf("isprime10=%d\n", is_prime(10));
    printf("digitsum(1234)=%d\n", sum_of_digits(1234));
    printf("reverse(1234)=%d\n", reverse_number(1234));
    printf("collatz(27)=%d\n", collatz_steps(27));
    printf("poly(3)=%d\n", polynomial(3));
    printf("isqrt(26)=%d\n", int_sqrt(26));
    printf("perfect_sq(25)=%d\n", is_perfect_square(25));
    printf("perfect_sq(26)=%d\n", is_perfect_square(26));
    printf("mulbyadd(7,8)=%d\n", multiply_by_add(7, 8));
    printf("divbysub(100,7)=%d\n", divide_by_sub(100, 7));
    printf("sign(-5)=%d\n", sign(-5));
    printf("sign(0)=%d\n", sign(0));
    printf("sign(5)=%d\n", sign(5));
    printf("clamp(15,0,10)=%d\n", clamp(15, 0, 10));
    printf("clamp(-3,0,10)=%d\n", clamp(-3, 0, 10));
    printf("abs(-42)=%d\n", abs_val(-42));
    printf("max(3,7)=%d\n", max(3, 7));
    printf("min(3,7)=%d\n", min(3, 7));
    printf("comb(10,3)=%d\n", combination(10, 3));
    printf("digroot(9999)=%d\n", digital_root(9999));
    printf("popcount(255)=%d\n", popcount(255));

    return 0;
}
