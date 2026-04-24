// EXPECT: === Complex Number Tests ===
// EXPECT: c1 = 3 + 4i
// EXPECT: c2 = 1 + 2i
// EXPECT: c3 = 0 + 0i
// EXPECT: c4 = -2 + 5i
// EXPECT: c5 = 7 + 0i
// EXPECT: c6 = 0 + -3i
// EXPECT: Addition:
// EXPECT:   (3+4i) + (1+2i) = 4 + 6i
// EXPECT:   (3+4i) + (0+0i) = 3 + 4i
// EXPECT:   (-2+5i) + (2+-5i) = 0 + 0i
// EXPECT:   (7+0i) + (0+-3i) = 7 + -3i
// EXPECT: Subtraction:
// EXPECT:   (3+4i) - (1+2i) = 2 + 2i
// EXPECT:   (1+2i) - (3+4i) = -2 + -2i
// EXPECT:   (3+4i) - (3+4i) = 0 + 0i
// EXPECT: Multiplication:
// EXPECT:   (3+4i) * (1+2i) = -5 + 10i
// EXPECT:   (3+4i) * (0+0i) = 0 + 0i
// EXPECT:   (3+4i) * (7+0i) = 21 + 28i
// EXPECT:   (0+1i) * (0+1i) = -1 + 0i
// EXPECT:   (1+1i) * (1+-1i) = 2 + 0i
// EXPECT: Division:
// EXPECT:   (3+4i) / (1+2i) = 2 + 0i (approx)
// EXPECT:   (10+0i) / (2+0i) = 5 + 0i (approx)
// EXPECT:   (-5+10i) / (1+2i) = 3 + 4i (approx)
// EXPECT: Magnitude squared:
// EXPECT:   |3+4i|^2 = 25
// EXPECT:   |1+2i|^2 = 5
// EXPECT:   |0+0i|^2 = 0
// EXPECT:   |7+0i|^2 = 49
// EXPECT: Integer sqrt approximation:
// EXPECT:   isqrt(25) = 5
// EXPECT:   isqrt(5) = 2
// EXPECT:   isqrt(0) = 0
// EXPECT:   isqrt(49) = 7
// EXPECT: Conjugate:
// EXPECT:   conj(3+4i) = 3 + -4i
// EXPECT:   (3+4i) * conj(3+4i) = 25 + 0i
// EXPECT: All complex number tests passed!

int printf(const char *fmt, ...);

struct Complex {
    int real;
    int imag;
};

struct Complex complex_new(int r, int i) {
    struct Complex c;
    c.real = r;
    c.imag = i;
    return c;
}

struct Complex complex_add(struct Complex a, struct Complex b) {
    struct Complex r;
    r.real = a.real + b.real;
    r.imag = a.imag + b.imag;
    return r;
}

struct Complex complex_sub(struct Complex a, struct Complex b) {
    struct Complex r;
    r.real = a.real - b.real;
    r.imag = a.imag - b.imag;
    return r;
}

// (a+bi)(c+di) = (ac-bd) + (ad+bc)i
struct Complex complex_mul(struct Complex a, struct Complex b) {
    struct Complex r;
    r.real = a.real * b.real - a.imag * b.imag;
    r.imag = a.real * b.imag + a.imag * b.real;
    return r;
}

// (a+bi)/(c+di) = ((ac+bd) + (bc-ad)i) / (c^2+d^2)
// Integer division (truncated)
struct Complex complex_div(struct Complex a, struct Complex b) {
    struct Complex r;
    int denom = b.real * b.real + b.imag * b.imag;
    if (denom == 0) {
        r.real = 0;
        r.imag = 0;
        return r;
    }
    r.real = (a.real * b.real + a.imag * b.imag) / denom;
    r.imag = (a.imag * b.real - a.real * b.imag) / denom;
    return r;
}

int complex_mag_sq(struct Complex c) {
    return c.real * c.real + c.imag * c.imag;
}

struct Complex complex_conj(struct Complex c) {
    struct Complex r;
    r.real = c.real;
    r.imag = -c.imag;
    return r;
}

// Integer square root (floor)
int isqrt(int n) {
    if (n <= 0) return 0;
    int x = n;
    int y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

void print_complex(struct Complex c) {
    printf("%d + %di", c.real, c.imag);
}

int main() {
    printf("=== Complex Number Tests ===\n");

    struct Complex c1 = complex_new(3, 4);
    struct Complex c2 = complex_new(1, 2);
    struct Complex c3 = complex_new(0, 0);
    struct Complex c4 = complex_new(-2, 5);
    struct Complex c5 = complex_new(7, 0);
    struct Complex c6 = complex_new(0, -3);

    printf("c1 = %d + %di\n", c1.real, c1.imag);
    printf("c2 = %d + %di\n", c2.real, c2.imag);
    printf("c3 = %d + %di\n", c3.real, c3.imag);
    printf("c4 = %d + %di\n", c4.real, c4.imag);
    printf("c5 = %d + %di\n", c5.real, c5.imag);
    printf("c6 = %d + %di\n", c6.real, c6.imag);

    // Addition
    printf("Addition:\n");
    struct Complex r;

    r = complex_add(c1, c2);
    printf("  (3+4i) + (1+2i) = %d + %di\n", r.real, r.imag);

    r = complex_add(c1, c3);
    printf("  (3+4i) + (0+0i) = %d + %di\n", r.real, r.imag);

    struct Complex neg_c4 = complex_new(2, -5);
    r = complex_add(c4, neg_c4);
    printf("  (-2+5i) + (2+-5i) = %d + %di\n", r.real, r.imag);

    r = complex_add(c5, c6);
    printf("  (7+0i) + (0+-3i) = %d + %di\n", r.real, r.imag);

    // Subtraction
    printf("Subtraction:\n");

    r = complex_sub(c1, c2);
    printf("  (3+4i) - (1+2i) = %d + %di\n", r.real, r.imag);

    r = complex_sub(c2, c1);
    printf("  (1+2i) - (3+4i) = %d + %di\n", r.real, r.imag);

    r = complex_sub(c1, c1);
    printf("  (3+4i) - (3+4i) = %d + %di\n", r.real, r.imag);

    // Multiplication
    printf("Multiplication:\n");

    r = complex_mul(c1, c2);
    printf("  (3+4i) * (1+2i) = %d + %di\n", r.real, r.imag);

    r = complex_mul(c1, c3);
    printf("  (3+4i) * (0+0i) = %d + %di\n", r.real, r.imag);

    r = complex_mul(c1, c5);
    printf("  (3+4i) * (7+0i) = %d + %di\n", r.real, r.imag);

    // i * i = -1
    struct Complex ci = complex_new(0, 1);
    r = complex_mul(ci, ci);
    printf("  (0+1i) * (0+1i) = %d + %di\n", r.real, r.imag);

    // (1+i)(1-i) = 2
    struct Complex a = complex_new(1, 1);
    struct Complex b = complex_new(1, -1);
    r = complex_mul(a, b);
    printf("  (1+1i) * (1+-1i) = %d + %di\n", r.real, r.imag);

    // Division
    printf("Division:\n");

    // (3+4i)/(1+2i) = (3+8+i(4-6))/(1+4) = 11/5 + (-2/5)i ~ 2+0i
    r = complex_div(c1, c2);
    printf("  (3+4i) / (1+2i) = %d + %di (approx)\n", r.real, r.imag);

    r = complex_div(complex_new(10, 0), complex_new(2, 0));
    printf("  (10+0i) / (2+0i) = %d + %di (approx)\n", r.real, r.imag);

    // (-5+10i)/(1+2i) = (-5+20+i(10+5))/(5) = 15/5+15i/5 = 3+3i? No:
    // (-5*1+10*2)/(1+4) + (10*1-(-5)*2)/(1+4)i = 15/5 + 20/5i = 3+4i
    r = complex_div(complex_new(-5, 10), c2);
    printf("  (-5+10i) / (1+2i) = %d + %di (approx)\n", r.real, r.imag);

    // Magnitude squared
    printf("Magnitude squared:\n");
    printf("  |3+4i|^2 = %d\n", complex_mag_sq(c1));
    printf("  |1+2i|^2 = %d\n", complex_mag_sq(c2));
    printf("  |0+0i|^2 = %d\n", complex_mag_sq(c3));
    printf("  |7+0i|^2 = %d\n", complex_mag_sq(c5));

    // Integer sqrt approximation
    printf("Integer sqrt approximation:\n");
    printf("  isqrt(25) = %d\n", isqrt(25));
    printf("  isqrt(5) = %d\n", isqrt(5));
    printf("  isqrt(0) = %d\n", isqrt(0));
    printf("  isqrt(49) = %d\n", isqrt(49));

    // Conjugate
    printf("Conjugate:\n");
    struct Complex conj = complex_conj(c1);
    printf("  conj(3+4i) = %d + %di\n", conj.real, conj.imag);

    // z * conj(z) = |z|^2 (real number)
    r = complex_mul(c1, conj);
    printf("  (3+4i) * conj(3+4i) = %d + %di\n", r.real, r.imag);

    printf("All complex number tests passed!\n");
    return 0;
}
