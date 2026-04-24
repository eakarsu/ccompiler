int printf(const char *fmt, ...);

/* Edge cases: single-element arrays, zero-length ops, deep nesting, long chains */

/* --- Single-element array --- */
int single_sum(int arr[1]) { return arr[0]; }
void single_set(int arr[1], int v) { arr[0] = v; }

/* --- Deeply nested function calls --- */
int add1(int x) { return x + 1; }
int double_it(int x) { return x * 2; }
int sub3(int x) { return x - 3; }
int square(int x) { return x * x; }
int half(int x) { return x / 2; }

/* --- Maximum nesting of conditionals --- */
int nested_cond(int x) {
    if (x > 100) {
        if (x > 200) {
            if (x > 300) {
                if (x > 400) {
                    return 5;
                } else {
                    return 4;
                }
            } else {
                return 3;
            }
        } else {
            return 2;
        }
    } else {
        return 1;
    }
}

/* --- Long expression chain --- */
int long_chain(int x) {
    return x + 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10
             + 11 + 12 + 13 + 14 + 15 + 16 + 17 + 18 + 19 + 20;
    /* sum 1..20 = 210, then +x */
}

/* --- Deeply nested struct access --- */
typedef struct { int v; } L0;
typedef struct { L0 a; L0 b; } L1;
typedef struct { L1 x; L1 y; } L2;
typedef struct { L2 p; L2 q; } L3;

/* --- Zero-valued operations --- */
int zero_ops(int x) {
    int r = x * 0;      /* 0 */
    r = r + 0;          /* 0 */
    r = r - 0;          /* 0 */
    r = r / 1;          /* 0 */
    r = r % 1;          /* 0 */
    return r;
}

/* --- Identity operations --- */
int identity_ops(int x) {
    x = x + 0;
    x = x - 0;
    x = x * 1;
    x = x / 1;
    x = x | 0;
    x = x & ~0;
    x = x ^ 0;
    x = x << 0;
    x = x >> 0;
    return x;
}

/* --- Recursive sum with deep call stack --- */
int recsum(int n) {
    if (n <= 0) return 0;
    return n + recsum(n - 1);
}

/* --- Long boolean chain --- */
int long_bool(int x) {
    return (x > 0) && (x < 1000) && (x != 5) && (x != 10)
        && (x % 2 == 0) && (x % 3 != 0);
}

/* --- Struct member access chain --- */
int deep_access(void) {
    L3 l3;
    l3.p.x.a.v = 1;
    l3.p.x.b.v = 2;
    l3.p.y.a.v = 3;
    l3.p.y.b.v = 4;
    l3.q.x.a.v = 5;
    l3.q.x.b.v = 6;
    l3.q.y.a.v = 7;
    l3.q.y.b.v = 8;
    return l3.p.x.a.v + l3.p.x.b.v + l3.p.y.a.v + l3.p.y.b.v
         + l3.q.x.a.v + l3.q.x.b.v + l3.q.y.a.v + l3.q.y.b.v;
    /* 1+2+3+4+5+6+7+8 = 36 */
}

int main(void) {
    /* --- Single-element array --- */
    int s1[1];
    s1[0] = 99;
    printf("%d\n", s1[0]);             // EXPECT: 99
    printf("%d\n", single_sum(s1));    // EXPECT: 99
    single_set(s1, 42);
    printf("%d\n", s1[0]);             // EXPECT: 42

    int s2[1] = {7};
    printf("%d\n", s2[0]);             // EXPECT: 7

    /* --- Zero-length operations --- */
    printf("%d\n", zero_ops(12345));   // EXPECT: 0
    printf("%d\n", zero_ops(-99));     // EXPECT: 0

    /* --- Identity operations --- */
    printf("%d\n", identity_ops(17));  // EXPECT: 17
    printf("%d\n", identity_ops(-5));  // EXPECT: -5
    printf("%d\n", identity_ops(0));   // EXPECT: 0

    /* --- Nested conditional --- */
    printf("%d\n", nested_cond(50));   // EXPECT: 1
    printf("%d\n", nested_cond(150));  // EXPECT: 2
    printf("%d\n", nested_cond(250));  // EXPECT: 3
    printf("%d\n", nested_cond(350));  // EXPECT: 4
    printf("%d\n", nested_cond(450));  // EXPECT: 5

    /* --- Long expression chain (x + 1+2+...+20 = x+210) --- */
    printf("%d\n", long_chain(0));     // EXPECT: 210
    printf("%d\n", long_chain(5));     // EXPECT: 215
    printf("%d\n", long_chain(-10));   // EXPECT: 200

    /* --- Deeply nested function calls --- */
    /* add1(double_it(sub3(square(half(10))))) */
    /* half(10)=5, square(5)=25, sub3(25)=22, double_it(22)=44, add1(44)=45 */
    printf("%d\n", add1(double_it(sub3(square(half(10)))))); // EXPECT: 45

    /* add1(add1(add1(add1(add1(0))))) = 5 */
    printf("%d\n", add1(add1(add1(add1(add1(0)))))); // EXPECT: 5

    /* double_it(double_it(double_it(1))) = 8 */
    printf("%d\n", double_it(double_it(double_it(1)))); // EXPECT: 8

    /* --- Deep recursive call --- */
    printf("%d\n", recsum(10));        // EXPECT: 55
    printf("%d\n", recsum(20));        // EXPECT: 210
    printf("%d\n", recsum(0));         // EXPECT: 0

    /* --- Long boolean chain --- */
    printf("%d\n", long_bool(4));      // EXPECT: 1
    printf("%d\n", long_bool(6));      // EXPECT: 0
    printf("%d\n", long_bool(5));      // EXPECT: 0
    printf("%d\n", long_bool(-1));     // EXPECT: 0
    printf("%d\n", long_bool(8));      // EXPECT: 1

    /* --- Deeply nested struct access --- */
    printf("%d\n", deep_access());     // EXPECT: 36

    /* --- Single-element array in loop --- */
    int sa[1];
    int i;
    int running = 0;
    for (i = 0; i < 5; i++) {
        sa[0] = i * i;
        running += sa[0];
        /* i=0:0; i=1:1; i=2:4; i=3:9; i=4:16 => 30 */
    }
    printf("%d\n", running);           // EXPECT: 30

    /* --- Long chain of assignments --- */
    int a, b, c, d, e, f, g, h;
    a = 1; b = a + 1; c = b + 1; d = c + 1;
    e = d + 1; f = e + 1; g = f + 1; h = g + 1;
    printf("%d\n", h);                 // EXPECT: 8
    printf("%d\n", a+b+c+d+e+f+g+h);  // EXPECT: 36

    /* --- Ternary chains --- */
    int x = 15;
    int res = (x > 20) ? 3 : (x > 10) ? 2 : (x > 5) ? 1 : 0;
    printf("%d\n", res);               // EXPECT: 2

    int y = 3;
    int r2 = (y > 20) ? 4 : (y > 10) ? 3 : (y > 5) ? 2 : (y > 0) ? 1 : 0;
    printf("%d\n", r2);                // EXPECT: 1

    /* --- Bitwise long chain --- */
    int bits = 0;
    bits = bits | (1 << 0);
    bits = bits | (1 << 2);
    bits = bits | (1 << 4);
    bits = bits | (1 << 6);
    /* bit 0,2,4,6: 1+4+16+64 = 85 */
    printf("%d\n", bits);              // EXPECT: 85

    bits = bits & ~(1 << 2);          /* clear bit 2: 85-4=81 */
    printf("%d\n", bits);              // EXPECT: 81

    return 0;
}
