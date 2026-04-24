int printf(const char *fmt, ...);
// EXPECT: a=3+4i\nb=1-2i\na+b=4+2i\na-b=2+6i\na*b=11-2i\na*2=6+8i\nconj(a)=3-4i\n|a|^2=25\n|b|^2=5\na*conj(a)=25+0i\na==a: 1\na==b: 0\ni^2=-1+0i
// Test: struct for complex number arithmetic

struct Complex {
    int real;
    int imag;
};

struct Complex complex_make(int r, int i) {
    struct Complex c;
    c.real = r;
    c.imag = i;
    return c;
}

struct Complex complex_add(struct Complex a, struct Complex b) {
    return complex_make(a.real + b.real, a.imag + b.imag);
}

struct Complex complex_sub(struct Complex a, struct Complex b) {
    return complex_make(a.real - b.real, a.imag - b.imag);
}

struct Complex complex_mul(struct Complex a, struct Complex b) {
    int r = a.real * b.real - a.imag * b.imag;
    int i = a.real * b.imag + a.imag * b.real;
    return complex_make(r, i);
}

struct Complex complex_scale(struct Complex c, int s) {
    return complex_make(c.real * s, c.imag * s);
}

struct Complex complex_conjugate(struct Complex c) {
    return complex_make(c.real, -c.imag);
}

int complex_magnitude_sq(struct Complex c) {
    return c.real * c.real + c.imag * c.imag;
}

int complex_equal(struct Complex a, struct Complex b) {
    return a.real == b.real && a.imag == b.imag;
}

void complex_print(struct Complex c) {
    if (c.imag >= 0) {
        printf("%d+%di", c.real, c.imag);
    } else {
        printf("%d%di", c.real, c.imag);
    }
}

int main(void) {
    struct Complex a = complex_make(3, 4);
    struct Complex b = complex_make(1, -2);

    printf("a="); complex_print(a); printf("\n");
    printf("b="); complex_print(b); printf("\n");

    struct Complex sum = complex_add(a, b);
    printf("a+b="); complex_print(sum); printf("\n");

    struct Complex diff = complex_sub(a, b);
    printf("a-b="); complex_print(diff); printf("\n");

    struct Complex prod = complex_mul(a, b);
    printf("a*b="); complex_print(prod); printf("\n");

    struct Complex scaled = complex_scale(a, 2);
    printf("a*2="); complex_print(scaled); printf("\n");

    struct Complex conj = complex_conjugate(a);
    printf("conj(a)="); complex_print(conj); printf("\n");

    printf("|a|^2=%d\n", complex_magnitude_sq(a));
    printf("|b|^2=%d\n", complex_magnitude_sq(b));

    struct Complex aa = complex_mul(a, complex_conjugate(a));
    printf("a*conj(a)="); complex_print(aa); printf("\n");

    printf("a==a: %d\n", complex_equal(a, a));
    printf("a==b: %d\n", complex_equal(a, b));

    struct Complex i_unit = complex_make(0, 1);
    struct Complex i_sq = complex_mul(i_unit, i_unit);
    printf("i^2="); complex_print(i_sq); printf("\n");

    return 0;
}
