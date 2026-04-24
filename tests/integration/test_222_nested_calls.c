int printf(const char *fmt, ...);
// EXPECT: double(inc(sq(3)))=20\ndouble(inc(sq(5)))=52\nabs(neg(double(inc(4))))=10\nabs(neg(double(inc(7))))=16\ninc^5(10)=15\ninc^5(0)=5\na*b+b*c(2,3,4)=18\na*b+b*c(5,6,7)=72\ncomplex(1,2,3,4)=1\ncomplex(5,3,8,2)=58\nrec_chain(10)=55\nclamp(15,0,10)=10\nclamp(-5,0,10)=0\n2*3+(10-2*2)=12\nmax(3+4,2*3)=7\nmin(sq(3),dbl(5))=9\nmax(1,2)+min(3,4)=5\nsq(1+dbl(2))=25
// Test 222: Nested function calls (f(g(h(x))))

int double_val(int x) { return x * 2; }
int inc(int x) { return x + 1; }
int square(int x) { return x * x; }
int negate(int x) { return -x; }
int abs_val(int x) { if (x < 0) return -x; return x; }

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }

int max(int a, int b) { if (a > b) return a; return b; }
int min(int a, int b) { if (a < b) return a; return b; }

int clamp(int val, int lo, int hi) {
    return min(max(val, lo), hi);
}

int compose_3(int x) {
    return double_val(inc(square(x)));
}

int compose_4(int x) {
    return abs_val(negate(double_val(inc(x))));
}

int deeply_nested(int x) {
    return inc(inc(inc(inc(inc(x)))));
}

int chain_arithmetic(int a, int b, int c) {
    return add(mul(a, b), mul(b, c));
}

int complex_expr(int a, int b, int c, int d) {
    return add(mul(add(a, b), sub(c, d)), mul(a, d));
}

int recursive_chain(int n) {
    if (n <= 0) return 0;
    return add(n, recursive_chain(sub(n, 1)));
}

int main(void) {
    printf("double(inc(sq(3)))=%d\n", compose_3(3));
    printf("double(inc(sq(5)))=%d\n", compose_3(5));

    printf("abs(neg(double(inc(4))))=%d\n", compose_4(4));
    printf("abs(neg(double(inc(7))))=%d\n", compose_4(7));

    printf("inc^5(10)=%d\n", deeply_nested(10));
    printf("inc^5(0)=%d\n", deeply_nested(0));

    printf("a*b+b*c(2,3,4)=%d\n", chain_arithmetic(2, 3, 4));
    printf("a*b+b*c(5,6,7)=%d\n", chain_arithmetic(5, 6, 7));

    printf("complex(1,2,3,4)=%d\n", complex_expr(1, 2, 3, 4));
    printf("complex(5,3,8,2)=%d\n", complex_expr(5, 3, 8, 2));

    printf("rec_chain(10)=%d\n", recursive_chain(10));

    printf("clamp(15,0,10)=%d\n", clamp(15, 0, 10));
    printf("clamp(-5,0,10)=%d\n", clamp(-5, 0, 10));

    int a = add(mul(2, 3), sub(10, mul(2, 2)));
    printf("2*3+(10-2*2)=%d\n", a);

    int b = max(add(3, 4), mul(2, 3));
    printf("max(3+4,2*3)=%d\n", b);

    int c = min(square(3), double_val(5));
    printf("min(sq(3),dbl(5))=%d\n", c);

    int d = add(max(1, 2), min(3, 4));
    printf("max(1,2)+min(3,4)=%d\n", d);

    int e = square(add(1, double_val(2)));
    printf("sq(1+dbl(2))=%d\n", e);

    return 0;
}
