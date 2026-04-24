int printf(const char *fmt, ...);
// EXPECT: === Fractal Calculations ===\nMandelbrot escape times (scaled coords):\n       \n       \n   ##  \n       \n       \nSierpinski carpet 9x9:\n#########\n# ## ## #\n#########\n###   ###\n# #   # #\n###   ###\n#########\n# ## ## #\n#########\nKoch curve properties:\n  Iteration 0: segments=1 length_factor=3\n  Iteration 1: segments=4 length_factor=12\n  Iteration 2: segments=16 length_factor=48\n  Iteration 3: segments=64 length_factor=192\n  Iteration 4: segments=256 length_factor=768\n  Iteration 5: segments=1024 length_factor=3072\nMandelbrot area estimate:\n  Points inside: 5 / 651

struct Complex {
    int real;
    int imag;
};

struct Complex cmul(struct Complex a, struct Complex b) {
    struct Complex r;
    r.real = a.real * b.real - a.imag * b.imag;
    r.imag = a.real * b.imag + a.imag * b.real;
    return r;
}

struct Complex cadd(struct Complex a, struct Complex b) {
    struct Complex r;
    r.real = a.real + b.real;
    r.imag = a.imag + b.imag;
    return r;
}

int cmag_sq(struct Complex c) {
    return c.real * c.real + c.imag * c.imag;
}

int mandelbrot_escape(int cr, int ci, int max_iter) {
    struct Complex z;
    z.real = 0;
    z.imag = 0;
    struct Complex c;
    c.real = cr;
    c.imag = ci;

    int i;
    for (i = 0; i < max_iter; i++) {
        struct Complex z2 = cmul(z, z);
        z = cadd(z2, c);
        if (cmag_sq(z) > 400) return i;
    }
    return max_iter;
}

int sierpinski_check(int x, int y) {
    while (x > 0 || y > 0) {
        if ((x % 3 == 1) && (y % 3 == 1)) return 0;
        x = x / 3;
        y = y / 3;
    }
    return 1;
}

int koch_length(int iterations) {
    int length = 3;
    int i;
    for (i = 0; i < iterations; i++) {
        length = length * 4;
    }
    return length;
}

int koch_segments(int iterations) {
    int seg = 1;
    int i;
    for (i = 0; i < iterations; i++) {
        seg = seg * 4;
    }
    return seg;
}

int main(void) {
    printf("=== Fractal Calculations ===\n");

    printf("Mandelbrot escape times (scaled coords):\n");
    int row, col;
    for (row = -4; row <= 4; row = row + 2) {
        for (col = -8; col <= 4; col = col + 2) {
            int iters = mandelbrot_escape(col, row, 20);
            if (iters >= 20) {
                printf("#");
            } else if (iters > 10) {
                printf("+");
            } else if (iters > 5) {
                printf(".");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    printf("Sierpinski carpet 9x9:\n");
    int x, y;
    for (y = 0; y < 9; y++) {
        for (x = 0; x < 9; x++) {
            printf("%c", sierpinski_check(x, y) ? '#' : ' ');
        }
        printf("\n");
    }

    printf("Koch curve properties:\n");
    int i;
    for (i = 0; i <= 5; i++) {
        printf("  Iteration %d: segments=%d length_factor=%d\n",
               i, koch_segments(i), koch_length(i));
    }

    printf("Mandelbrot area estimate:\n");
    int inside = 0;
    int total = 0;
    for (row = -10; row <= 10; row++) {
        for (col = -20; col <= 10; col++) {
            total++;
            if (mandelbrot_escape(col, row, 30) >= 30) {
                inside++;
            }
        }
    }
    printf("  Points inside: %d / %d\n", inside, total);

    return 0;
}
