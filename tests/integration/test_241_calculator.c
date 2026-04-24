int printf(const char *fmt, ...);
// EXPECT: 3+4=7\n(2+3)*(4+1)=25\n2^10=1024\n17%5=2\n((10-3)*2+5)/3=6\n=== factorials ===\n0!=1\n1!=1\n2!=2\n3!=6\n4!=24\n5!=120\n6!=720\n7!=5040\n8!=40320\n9!=362880\n10!=3628800\n=== gcd ===\ngcd(48,18)=6\ngcd(100,75)=25\ngcd(13,7)=1\n=== isqrt ===\nisqrt(0)=0\nisqrt(1)=1\nisqrt(4)=2\nisqrt(16)=4\nisqrt(100)=10\nisqrt(99)=9
// Test 241: Mini calculator using function dispatch

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int div_fn(int a, int b) { if (b == 0) return 0; return a / b; }
int mod_fn(int a, int b) { if (b == 0) return 0; return a - (a / b) * b; }
int pow_fn(int base, int exp) {
    int result = 1;
    int i = 0;
    while (i < exp) { result = result * base; i = i + 1; }
    return result;
}

typedef int (*op_fn)(int, int);

int evaluate(int *program, int len) {
    // Simple stack machine: positive = push, negative = operation
    // -1=add, -2=sub, -3=mul, -4=div, -5=mod, -6=pow
    int stack[50];
    int sp = 0;
    int i = 0;
    while (i < len) {
        if (program[i] >= 0) {
            stack[sp] = program[i];
            sp = sp + 1;
        } else {
            int b = stack[sp - 1]; sp = sp - 1;
            int a = stack[sp - 1]; sp = sp - 1;
            int result = 0;
            if (program[i] == -1) result = add(a, b);
            else if (program[i] == -2) result = sub(a, b);
            else if (program[i] == -3) result = mul(a, b);
            else if (program[i] == -4) result = div_fn(a, b);
            else if (program[i] == -5) result = mod_fn(a, b);
            else if (program[i] == -6) result = pow_fn(a, b);
            stack[sp] = result;
            sp = sp + 1;
        }
        i = i + 1;
    }
    return stack[0];
}

int factorial(int n) {
    int result = 1;
    int i = 2;
    while (i <= n) {
        result = result * i;
        i = i + 1;
    }
    return result;
}

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a - (a / b) * b;
        a = t;
    }
    return a;
}

int abs_val(int x) { if (x < 0) return -x; return x; }

int isqrt(int n) {
    int x = n;
    int y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

int main(void) {
    // 3 + 4
    int p1[3]; p1[0] = 3; p1[1] = 4; p1[2] = -1;
    printf("3+4=%d\n", evaluate(p1, 3));

    // (2+3) * (4+1)
    int p2[7]; p2[0]=2; p2[1]=3; p2[2]=-1; p2[3]=4; p2[4]=1; p2[5]=-1; p2[6]=-3;
    printf("(2+3)*(4+1)=%d\n", evaluate(p2, 7));

    // 2^10
    int p3[3]; p3[0] = 2; p3[1] = 10; p3[2] = -6;
    printf("2^10=%d\n", evaluate(p3, 3));

    // 17 % 5
    int p4[3]; p4[0] = 17; p4[1] = 5; p4[2] = -5;
    printf("17%%5=%d\n", evaluate(p4, 3));

    // ((10 - 3) * 2 + 5) / 3
    int p5[9]; p5[0]=10; p5[1]=3; p5[2]=-2; p5[3]=2; p5[4]=-3; p5[5]=5; p5[6]=-1; p5[7]=3; p5[8]=-4;
    printf("((10-3)*2+5)/3=%d\n", evaluate(p5, 9));

    printf("=== factorials ===\n");
    int i = 0;
    while (i <= 10) {
        printf("%d!=%d\n", i, factorial(i));
        i = i + 1;
    }

    printf("=== gcd ===\n");
    printf("gcd(48,18)=%d\n", gcd(48, 18));
    printf("gcd(100,75)=%d\n", gcd(100, 75));
    printf("gcd(13,7)=%d\n", gcd(13, 7));

    printf("=== isqrt ===\n");
    printf("isqrt(0)=%d\n", isqrt(0));
    printf("isqrt(1)=%d\n", isqrt(1));
    printf("isqrt(4)=%d\n", isqrt(4));
    printf("isqrt(16)=%d\n", isqrt(16));
    printf("isqrt(100)=%d\n", isqrt(100));
    printf("isqrt(99)=%d\n", isqrt(99));

    return 0;
}
