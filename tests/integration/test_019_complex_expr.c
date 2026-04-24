int printf(const char *fmt, ...);
// EXPECT: === Basic Precedence ===\n2+3*4=14\n(2+3)*4=20\n10-2*3=4\n10/2+3=8\n10%3+1=2\n2*3+4*5=26\n(2+3)*(4+5)=45\n100/10/2=5\n100-30-20-10=40\n=== Unary Minus ===\n-5=-5\n-(-5)=5\n-3*4=-12\n-(3+4)=-7\n-3+4=1\n-3*-4=12\n=== Compound Assignments ===\nx=10\nx+=5: 15\nx-=3: 12\nx*=2: 24\nx/=4: 6\nx%=4: 2\n=== Increment Patterns ===\na=1, a+=1: 2\na+=a: 4\na*=a: 16\na-=10: 6\na+=100: 106\na/=2: 53\na%=10: 3\n=== Bitwise Operators ===\n5&3=1\n5|3=7\n5^3=6\n~0=-1\n~1=-2\n1<<0=1\n1<<1=2\n1<<2=4\n1<<3=8\n1<<10=1024\n8>>1=4\n8>>2=2\n8>>3=1\n16>>4=1\n=== Bitwise Precedence ===\n2|4&6=6\n(2|4)&6=6\n3^5&6=7\n1|2|4=7\n7&5&3=1\n1<<2|1<<0=5\n1<<3|1<<1=10\n=== Comparison Operators ===\n3<5=1\n5<3=0\n3<=3=1\n3<=4=1\n5>3=1\n3>5=0\n3>=3=1\n4>=3=1\n3==3=1\n3==4=0\n3!=4=1\n3!=3=0\n=== Logical Operators ===\n1&&1=1\n1&&0=0\n0&&1=0\n0&&0=0\n1||1=1\n1||0=1\n0||1=1\n0||0=0\n!0=1\n!1=0\n!5=0\n=== Short Circuit ===\n0&&side_effect: result=0, called=0\n1||side_effect: result=1, called=0\n1&&side_effect: result=1, called=1\n0||side_effect: result=1, called=1\n=== Complex Expressions ===\n(3+4)*(5-2)/(7-4)=7\n10*10-5*5-3*3=66\n(1+2+3+4+5)*2=30\n100-(10+20+30)=40\n2*3+4*5+6*7=68\n=== Nested Function Calls ===\nmax(3,5)=5\nmin(3,5)=3\nabs(-7)=7\nabs(7)=7\nmax(min(5,9),min(3,7))=5\nmin(max(1,4),max(2,3))=3\nmax(abs(-3),abs(-5))=5\n=== Conditional Expression ===\nbigger(3,5)=5\nbigger(7,2)=7\nsign(-5)=-1\nsign(0)=0\nsign(5)=1\nclamp(-5,0,10)=0\nclamp(5,0,10)=5\nclamp(15,0,10)=10\n=== Polynomial Evaluation ===\npoly(2,3,1, x=0)=1\npoly(2,3,1, x=1)=6\npoly(2,3,1, x=2)=15\npoly(2,3,1, x=3)=28\npoly(2,3,1, x=-1)=0\n=== Horner's Method ===\nhorner([1,2,3,4], x=2)=26\nhorner([1,2,3,4], x=0)=4\nhorner([1,2,3,4], x=1)=10\nhorner([1,0,0,0], x=5)=125\n=== Power Function ===\npow(2,0)=1\npow(2,1)=2\npow(2,10)=1024\npow(3,5)=243\npow(5,3)=125\n=== Fibonacci ===\nfib(0)=0\nfib(1)=1\nfib(2)=1\nfib(5)=5\nfib(10)=55\n=== GCD ===\ngcd(12,8)=4\ngcd(100,75)=25\ngcd(17,13)=1\ngcd(0,5)=5\n=== Complex Compound ===\nchain: a=10\na+=5 -> 15\na*=2 -> 30\na-=10 -> 20\na/=4 -> 5\na%=3 -> 2\na<<=3 -> 16\na>>=1 -> 8\na|=3 -> 11\na&=14 -> 10\na^=5 -> 15\n=== Expression in Array Index ===\narr[1+1]=30\narr[2*2-1]=40\narr[10/2-3]=30\narr[7%5]=30\n=== Chained Comparisons ===\n(1<2)+(2<3)+(3<4)=3\n(5>4)+(4>3)+(3>2)=3\n(1==1)+(2==2)+(3==3)=3\n(1!=2)+(2!=3)+(3!=4)=3\n=== Mixed Bit Operations ===\nset bit 0 of 0: 1\nset bit 3 of 0: 8\nclear bit 0 of 7: 6\ntoggle bit 1 of 5: 7\ntest bit 2 of 5: 1\ntest bit 1 of 5: 0\n=== Multi-operator Stress ===\n1+2-3+4-5+6-7+8-9+10=7\n1*2*3*4*5=120\n((((1+2)*3)+4)*5)=65\n10-9+8-7+6-5+4-3+2-1=5\n=== Collatz Steps ===\ncollatz(1)=0\ncollatz(2)=1\ncollatz(6)=8\ncollatz(27)=111\n=== Digital Root ===\ndroot(0)=0\ndroot(5)=5\ndroot(493)=7\ndroot(99)=9\n=== Boolean Expression Tree ===\n(1&&1)||(0&&1)=1\n(0||0)&&(1||1)=0\n!(0||0)=1\n!(1&&1)=0\n(1||0)&&(0||1)=1\n=== All expression tests done ===

int g_side_effect_called;

int side_effect_fn() {
    g_side_effect_called = 1;
    return 1;
}

int my_abs(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

int my_max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int my_min(int a, int b) {
    if (a < b) {
        return a;
    }
    return b;
}

int bigger(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int sign(int x) {
    if (x > 0) {
        return 1;
    }
    if (x < 0) {
        return -1;
    }
    return 0;
}

int clamp(int x, int lo, int hi) {
    if (x < lo) {
        return lo;
    }
    if (x > hi) {
        return hi;
    }
    return x;
}

int poly_eval(int a, int b, int c, int x) {
    return a * x * x + b * x + c;
}

int horner(int *coeffs, int n, int x) {
    int result;
    int i;
    result = coeffs[0];
    i = 1;
    while (i < n) {
        result = result * x + coeffs[i];
        i = i + 1;
    }
    return result;
}

int my_pow(int base, int exp) {
    int result;
    result = 1;
    while (exp > 0) {
        result = result * base;
        exp = exp - 1;
    }
    return result;
}

int fib(int n) {
    int a;
    int b;
    int tmp;
    int i;
    if (n == 0) return 0;
    if (n == 1) return 1;
    a = 0;
    b = 1;
    i = 2;
    while (i <= n) {
        tmp = a + b;
        a = b;
        b = tmp;
        i = i + 1;
    }
    return b;
}

int gcd(int a, int b) {
    int tmp;
    while (b != 0) {
        tmp = b;
        b = a % b;
        a = tmp;
    }
    return a;
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
    int sum;
    if (n == 0) return 0;
    while (n >= 10) {
        sum = 0;
        while (n > 0) {
            sum = sum + n % 10;
            n = n / 10;
        }
        n = sum;
    }
    return n;
}

int set_bit(int val, int bit) {
    return val | (1 << bit);
}

int clear_bit(int val, int bit) {
    return val & ~(1 << bit);
}

int toggle_bit(int val, int bit) {
    return val ^ (1 << bit);
}

int test_bit(int val, int bit) {
    if ((val & (1 << bit)) != 0) {
        return 1;
    }
    return 0;
}

int main() {
    int x;
    int a;
    int result;
    int coeffs[4];
    int arr[5];
    int i;

    printf("=== Basic Precedence ===\n");
    printf("2+3*4=%d\n", 2 + 3 * 4);
    printf("(2+3)*4=%d\n", (2 + 3) * 4);
    printf("10-2*3=%d\n", 10 - 2 * 3);
    printf("10/2+3=%d\n", 10 / 2 + 3);
    printf("10%%3+1=%d\n", 10 % 3 + 1);
    printf("2*3+4*5=%d\n", 2 * 3 + 4 * 5);
    printf("(2+3)*(4+5)=%d\n", (2 + 3) * (4 + 5));
    printf("100/10/2=%d\n", 100 / 10 / 2);
    printf("100-30-20-10=%d\n", 100 - 30 - 20 - 10);

    printf("=== Unary Minus ===\n");
    printf("-5=%d\n", -5);
    printf("-(-5)=%d\n", -(-5));
    printf("-3*4=%d\n", -3 * 4);
    printf("-(3+4)=%d\n", -(3 + 4));
    printf("-3+4=%d\n", -3 + 4);
    printf("-3*-4=%d\n", (-3) * (-4));

    printf("=== Compound Assignments ===\n");
    x = 10;
    printf("x=%d\n", x);
    x += 5;
    printf("x+=5: %d\n", x);
    x -= 3;
    printf("x-=3: %d\n", x);
    x *= 2;
    printf("x*=2: %d\n", x);
    x /= 4;
    printf("x/=4: %d\n", x);
    x %= 4;
    printf("x%%=4: %d\n", x);

    printf("=== Increment Patterns ===\n");
    a = 1;
    a += 1;
    printf("a=1, a+=1: %d\n", a);
    a += a;
    printf("a+=a: %d\n", a);
    a *= a;
    printf("a*=a: %d\n", a);
    a -= 10;
    printf("a-=10: %d\n", a);
    a += 100;
    printf("a+=100: %d\n", a);
    a /= 2;
    printf("a/=2: %d\n", a);
    a %= 10;
    printf("a%%=10: %d\n", a);

    printf("=== Bitwise Operators ===\n");
    printf("5&3=%d\n", 5 & 3);
    printf("5|3=%d\n", 5 | 3);
    printf("5^3=%d\n", 5 ^ 3);
    printf("~0=%d\n", ~0);
    printf("~1=%d\n", ~1);
    printf("1<<0=%d\n", 1 << 0);
    printf("1<<1=%d\n", 1 << 1);
    printf("1<<2=%d\n", 1 << 2);
    printf("1<<3=%d\n", 1 << 3);
    printf("1<<10=%d\n", 1 << 10);
    printf("8>>1=%d\n", 8 >> 1);
    printf("8>>2=%d\n", 8 >> 2);
    printf("8>>3=%d\n", 8 >> 3);
    printf("16>>4=%d\n", 16 >> 4);

    printf("=== Bitwise Precedence ===\n");
    printf("2|4&6=%d\n", 2 | (4 & 6));
    printf("(2|4)&6=%d\n", (2 | 4) & 6);
    printf("3^5&6=%d\n", 3 ^ (5 & 6));
    printf("1|2|4=%d\n", 1 | 2 | 4);
    printf("7&5&3=%d\n", 7 & 5 & 3);
    printf("1<<2|1<<0=%d\n", (1 << 2) | (1 << 0));
    printf("1<<3|1<<1=%d\n", (1 << 3) | (1 << 1));

    printf("=== Comparison Operators ===\n");
    printf("3<5=%d\n", 3 < 5);
    printf("5<3=%d\n", 5 < 3);
    printf("3<=3=%d\n", 3 <= 3);
    printf("3<=4=%d\n", 3 <= 4);
    printf("5>3=%d\n", 5 > 3);
    printf("3>5=%d\n", 3 > 5);
    printf("3>=3=%d\n", 3 >= 3);
    printf("4>=3=%d\n", 4 >= 3);
    printf("3==3=%d\n", 3 == 3);
    printf("3==4=%d\n", 3 == 4);
    printf("3!=4=%d\n", 3 != 4);
    printf("3!=3=%d\n", 3 != 3);

    printf("=== Logical Operators ===\n");
    printf("1&&1=%d\n", 1 && 1);
    printf("1&&0=%d\n", 1 && 0);
    printf("0&&1=%d\n", 0 && 1);
    printf("0&&0=%d\n", 0 && 0);
    printf("1||1=%d\n", 1 || 1);
    printf("1||0=%d\n", 1 || 0);
    printf("0||1=%d\n", 0 || 1);
    printf("0||0=%d\n", 0 || 0);
    printf("!0=%d\n", !0);
    printf("!1=%d\n", !1);
    printf("!5=%d\n", !5);

    printf("=== Short Circuit ===\n");
    g_side_effect_called = 0;
    result = 0 && side_effect_fn();
    printf("0&&side_effect: result=%d, called=%d\n", result, g_side_effect_called);

    g_side_effect_called = 0;
    result = 1 || side_effect_fn();
    printf("1||side_effect: result=%d, called=%d\n", result, g_side_effect_called);

    g_side_effect_called = 0;
    result = 1 && side_effect_fn();
    printf("1&&side_effect: result=%d, called=%d\n", result, g_side_effect_called);

    g_side_effect_called = 0;
    result = 0 || side_effect_fn();
    printf("0||side_effect: result=%d, called=%d\n", result, g_side_effect_called);

    printf("=== Complex Expressions ===\n");
    printf("(3+4)*(5-2)/(7-4)=%d\n", (3 + 4) * (5 - 2) / (7 - 4));
    printf("10*10-5*5-3*3=%d\n", 10 * 10 - 5 * 5 - 3 * 3);
    printf("(1+2+3+4+5)*2=%d\n", (1 + 2 + 3 + 4 + 5) * 2);
    printf("100-(10+20+30)=%d\n", 100 - (10 + 20 + 30));
    printf("2*3+4*5+6*7=%d\n", 2 * 3 + 4 * 5 + 6 * 7);

    printf("=== Nested Function Calls ===\n");
    printf("max(3,5)=%d\n", my_max(3, 5));
    printf("min(3,5)=%d\n", my_min(3, 5));
    printf("abs(-7)=%d\n", my_abs(-7));
    printf("abs(7)=%d\n", my_abs(7));
    printf("max(min(5,9),min(3,7))=%d\n", my_max(my_min(5, 9), my_min(3, 7)));
    printf("min(max(1,4),max(2,3))=%d\n", my_min(my_max(1, 4), my_max(2, 3)));
    printf("max(abs(-3),abs(-5))=%d\n", my_max(my_abs(-3), my_abs(-5)));

    printf("=== Conditional Expression ===\n");
    printf("bigger(3,5)=%d\n", bigger(3, 5));
    printf("bigger(7,2)=%d\n", bigger(7, 2));
    printf("sign(-5)=%d\n", sign(-5));
    printf("sign(0)=%d\n", sign(0));
    printf("sign(5)=%d\n", sign(5));
    printf("clamp(-5,0,10)=%d\n", clamp(-5, 0, 10));
    printf("clamp(5,0,10)=%d\n", clamp(5, 0, 10));
    printf("clamp(15,0,10)=%d\n", clamp(15, 0, 10));

    printf("=== Polynomial Evaluation ===\n");
    printf("poly(2,3,1, x=0)=%d\n", poly_eval(2, 3, 1, 0));
    printf("poly(2,3,1, x=1)=%d\n", poly_eval(2, 3, 1, 1));
    printf("poly(2,3,1, x=2)=%d\n", poly_eval(2, 3, 1, 2));
    printf("poly(2,3,1, x=3)=%d\n", poly_eval(2, 3, 1, 3));
    printf("poly(2,3,1, x=-1)=%d\n", poly_eval(2, 3, 1, -1));

    printf("=== Horner's Method ===\n");
    coeffs[0] = 1; coeffs[1] = 2; coeffs[2] = 3; coeffs[3] = 4;
    printf("horner([1,2,3,4], x=2)=%d\n", horner(coeffs, 4, 2));
    printf("horner([1,2,3,4], x=0)=%d\n", horner(coeffs, 4, 0));
    printf("horner([1,2,3,4], x=1)=%d\n", horner(coeffs, 4, 1));
    coeffs[0] = 1; coeffs[1] = 0; coeffs[2] = 0; coeffs[3] = 0;
    printf("horner([1,0,0,0], x=5)=%d\n", horner(coeffs, 4, 5));

    printf("=== Power Function ===\n");
    printf("pow(2,0)=%d\n", my_pow(2, 0));
    printf("pow(2,1)=%d\n", my_pow(2, 1));
    printf("pow(2,10)=%d\n", my_pow(2, 10));
    printf("pow(3,5)=%d\n", my_pow(3, 5));
    printf("pow(5,3)=%d\n", my_pow(5, 3));

    printf("=== Fibonacci ===\n");
    printf("fib(0)=%d\n", fib(0));
    printf("fib(1)=%d\n", fib(1));
    printf("fib(2)=%d\n", fib(2));
    printf("fib(5)=%d\n", fib(5));
    printf("fib(10)=%d\n", fib(10));

    printf("=== GCD ===\n");
    printf("gcd(12,8)=%d\n", gcd(12, 8));
    printf("gcd(100,75)=%d\n", gcd(100, 75));
    printf("gcd(17,13)=%d\n", gcd(17, 13));
    printf("gcd(0,5)=%d\n", gcd(0, 5));

    printf("=== Complex Compound ===\n");
    a = 10;
    printf("chain: a=%d\n", a);
    a += 5;
    printf("a+=5 -> %d\n", a);
    a *= 2;
    printf("a*=2 -> %d\n", a);
    a -= 10;
    printf("a-=10 -> %d\n", a);
    a /= 4;
    printf("a/=4 -> %d\n", a);
    a %= 3;
    printf("a%%=3 -> %d\n", a);
    a <<= 3;
    printf("a<<=3 -> %d\n", a);
    a >>= 1;
    printf("a>>=1 -> %d\n", a);
    a |= 3;
    printf("a|=3 -> %d\n", a);
    a &= 14;
    printf("a&=14 -> %d\n", a);
    a ^= 5;
    printf("a^=5 -> %d\n", a);

    printf("=== Expression in Array Index ===\n");
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;
    printf("arr[1+1]=%d\n", arr[1 + 1]);
    printf("arr[2*2-1]=%d\n", arr[2 * 2 - 1]);
    printf("arr[10/2-3]=%d\n", arr[10 / 2 - 3]);
    printf("arr[7%%5]=%d\n", arr[7 % 5]);

    printf("=== Chained Comparisons ===\n");
    printf("(1<2)+(2<3)+(3<4)=%d\n", (1 < 2) + (2 < 3) + (3 < 4));
    printf("(5>4)+(4>3)+(3>2)=%d\n", (5 > 4) + (4 > 3) + (3 > 2));
    printf("(1==1)+(2==2)+(3==3)=%d\n", (1 == 1) + (2 == 2) + (3 == 3));
    printf("(1!=2)+(2!=3)+(3!=4)=%d\n", (1 != 2) + (2 != 3) + (3 != 4));

    printf("=== Mixed Bit Operations ===\n");
    printf("set bit 0 of 0: %d\n", set_bit(0, 0));
    printf("set bit 3 of 0: %d\n", set_bit(0, 3));
    printf("clear bit 0 of 7: %d\n", clear_bit(7, 0));
    printf("toggle bit 1 of 5: %d\n", toggle_bit(5, 1));
    printf("test bit 2 of 5: %d\n", test_bit(5, 2));
    printf("test bit 1 of 5: %d\n", test_bit(5, 1));

    printf("=== Multi-operator Stress ===\n");
    printf("1+2-3+4-5+6-7+8-9+10=%d\n", 1 + 2 - 3 + 4 - 5 + 6 - 7 + 8 - 9 + 10);
    printf("1*2*3*4*5=%d\n", 1 * 2 * 3 * 4 * 5);
    printf("((((1+2)*3)+4)*5)=%d\n", ((((1 + 2) * 3) + 4) * 5));
    printf("10-9+8-7+6-5+4-3+2-1=%d\n", 10 - 9 + 8 - 7 + 6 - 5 + 4 - 3 + 2 - 1);

    printf("=== Collatz Steps ===\n");
    printf("collatz(1)=%d\n", collatz_steps(1));
    printf("collatz(2)=%d\n", collatz_steps(2));
    printf("collatz(6)=%d\n", collatz_steps(6));
    printf("collatz(27)=%d\n", collatz_steps(27));

    printf("=== Digital Root ===\n");
    printf("droot(0)=%d\n", digital_root(0));
    printf("droot(5)=%d\n", digital_root(5));
    printf("droot(493)=%d\n", digital_root(493));
    printf("droot(99)=%d\n", digital_root(99));

    printf("=== Boolean Expression Tree ===\n");
    printf("(1&&1)||(0&&1)=%d\n", (1 && 1) || (0 && 1));
    printf("(0||0)&&(1||1)=%d\n", (0 || 0) && (1 || 1));
    printf("!(0||0)=%d\n", !(0 || 0));
    printf("!(1&&1)=%d\n", !(1 && 1));
    printf("(1||0)&&(0||1)=%d\n", (1 || 0) && (0 || 1));

    printf("=== All expression tests done ===\n");

    return 0;
}
