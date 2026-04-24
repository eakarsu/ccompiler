int printf(const char *fmt, ...);

// Simple FFT butterfly (radix-2, integer)
// We work with complex numbers as pairs (real, imag) stored in separate arrays
// Twiddle factors approximated as integers scaled by 1000

// Butterfly: given (a_re, a_im) and (b_re, b_im) and twiddle (w_re, w_im)
//   t_re = (w_re * b_re - w_im * b_im) / 1000
//   t_im = (w_re * b_im + w_im * b_re) / 1000
//   out_top = a + t
//   out_bot = a - t

typedef struct {
    int re;
    int im;
} Complex;

Complex make_complex(int re, int im) {
    Complex c;
    c.re = re;
    c.im = im;
    return c;
}

Complex complex_add(Complex a, Complex b) {
    Complex c;
    c.re = a.re + b.re;
    c.im = a.im + b.im;
    return c;
}

Complex complex_sub(Complex a, Complex b) {
    Complex c;
    c.re = a.re - b.re;
    c.im = a.im - b.im;
    return c;
}

Complex complex_mul_scaled(Complex a, Complex b) {
    Complex c;
    c.re = (a.re * b.re - a.im * b.im) / 1000;
    c.im = (a.re * b.im + a.im * b.re) / 1000;
    return c;
}

void butterfly(Complex *top, Complex *bot, Complex twiddle) {
    Complex t = complex_mul_scaled(twiddle, *bot);
    Complex new_top = complex_add(*top, t);
    Complex new_bot = complex_sub(*top, t);
    *top = new_top;
    *bot = new_bot;
}

// Bit-reverse for N=4 (2 bits): 0->0, 1->2, 2->1, 3->3
int bit_reverse_2(int x) {
    return ((x & 1) << 1) | ((x >> 1) & 1);
}

// Bit-reverse for N=8 (3 bits): swap bits
int bit_reverse_3(int x) {
    int b0 = x & 1;
    int b1 = (x >> 1) & 1;
    int b2 = (x >> 2) & 1;
    return (b0 << 2) | (b1 << 1) | b2;
}

// 4-point FFT using butterflies
// Twiddle factors for N=4:
// W^0 = 1+0j = (1000, 0)
// W^1 = 0-1j = (0, -1000)
void fft4(Complex *x) {
    // Bit-reverse permutation
    Complex tmp[4];
    int i;
    for (i = 0; i < 4; i++) {
        tmp[bit_reverse_2(i)] = x[i];
    }
    for (i = 0; i < 4; i++) {
        x[i] = tmp[i];
    }

    // Stage 1: butterflies with stride 1, twiddle W^0 = (1000,0)
    Complex w0 = make_complex(1000, 0);
    butterfly(&x[0], &x[1], w0);
    butterfly(&x[2], &x[3], w0);

    // Stage 2: butterflies with stride 2
    // Twiddle for k=0: W^0 = (1000, 0)
    // Twiddle for k=1: W^1 = (0, -1000)
    Complex w1 = make_complex(0, -1000);
    butterfly(&x[0], &x[2], w0);
    butterfly(&x[1], &x[3], w1);
}

int magnitude_sq(Complex c) {
    return c.re * c.re + c.im * c.im;
}

int main() {
    // Test 1: Basic butterfly operation
    Complex a = make_complex(1000, 0);
    Complex b = make_complex(500, 0);
    Complex w = make_complex(1000, 0);  // W^0 = 1
    butterfly(&a, &b, w);
    // t = (1000*500)/1000 + 0 = 500 + 0i
    // top = 1000+500 = 1500, bot = 1000-500 = 500
    printf("Bfly1: top=(%d,%d) bot=(%d,%d)\n", a.re, a.im, b.re, b.im);
    // EXPECT: Bfly1: top=(1500,0) bot=(500,0)

    // Test 2: Butterfly with W = -j = (0, -1000)
    Complex a2 = make_complex(1000, 0);
    Complex b2 = make_complex(1000, 0);
    Complex wj = make_complex(0, -1000);
    butterfly(&a2, &b2, wj);
    // t_re = (0*1000 - (-1000)*0)/1000 = 0
    // t_im = (0*0 + (-1000)*1000)/1000 = -1000
    // top = (1000+0, 0+(-1000)) = (1000, -1000)
    // bot = (1000-0, 0-(-1000)) = (1000, 1000)
    printf("Bfly2: top=(%d,%d) bot=(%d,%d)\n", a2.re, a2.im, b2.re, b2.im);
    // EXPECT: Bfly2: top=(1000,-1000) bot=(1000,1000)

    // Test 3: 4-point FFT of [1000, 0, 0, 0] (impulse)
    Complex fft_data[4];
    fft_data[0] = make_complex(1000, 0);
    fft_data[1] = make_complex(0, 0);
    fft_data[2] = make_complex(0, 0);
    fft_data[3] = make_complex(0, 0);
    fft4(fft_data);
    // FFT of impulse = all 1s (scaled by 1000)
    printf("FFT imp: (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n",
           fft_data[0].re, fft_data[0].im,
           fft_data[1].re, fft_data[1].im,
           fft_data[2].re, fft_data[2].im,
           fft_data[3].re, fft_data[3].im);
    // EXPECT: FFT imp: (1000,0) (1000,0) (1000,0) (1000,0)

    // Test 4: 4-point FFT of DC signal [1000, 1000, 1000, 1000]
    Complex dc[4];
    dc[0] = make_complex(1000, 0);
    dc[1] = make_complex(1000, 0);
    dc[2] = make_complex(1000, 0);
    dc[3] = make_complex(1000, 0);
    fft4(dc);
    // FFT of DC = [4000, 0, 0, 0]
    printf("FFT dc: (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n",
           dc[0].re, dc[0].im,
           dc[1].re, dc[1].im,
           dc[2].re, dc[2].im,
           dc[3].re, dc[3].im);
    // EXPECT: FFT dc: (4000,0) (0,0) (0,0) (0,0)

    // Test 5: 4-point FFT of alternating [1000, -1000, 1000, -1000]
    Complex alt[4];
    alt[0] = make_complex(1000, 0);
    alt[1] = make_complex(-1000, 0);
    alt[2] = make_complex(1000, 0);
    alt[3] = make_complex(-1000, 0);
    fft4(alt);
    // This is Nyquist frequency, should be [0, 0, 4000, 0]
    printf("FFT alt: (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n",
           alt[0].re, alt[0].im,
           alt[1].re, alt[1].im,
           alt[2].re, alt[2].im,
           alt[3].re, alt[3].im);
    // EXPECT: FFT alt: (0,0) (0,0) (4000,0) (0,0)

    // Test 6: Bit-reverse tests
    printf("BR2: %d %d %d %d\n",
           bit_reverse_2(0), bit_reverse_2(1), bit_reverse_2(2), bit_reverse_2(3));
    // EXPECT: BR2: 0 2 1 3

    printf("BR3: %d %d %d %d %d %d %d %d\n",
           bit_reverse_3(0), bit_reverse_3(1), bit_reverse_3(2), bit_reverse_3(3),
           bit_reverse_3(4), bit_reverse_3(5), bit_reverse_3(6), bit_reverse_3(7));
    // EXPECT: BR3: 0 4 2 6 1 5 3 7

    // Test 7: Complex arithmetic
    Complex p = make_complex(3, 4);
    Complex q = make_complex(1, -2);
    Complex s = complex_add(p, q);
    Complex d = complex_sub(p, q);
    printf("Add: (%d,%d)\n", s.re, s.im);
    // EXPECT: Add: (4,2)
    printf("Sub: (%d,%d)\n", d.re, d.im);
    // EXPECT: Sub: (2,6)

    // Test 8: Magnitude squared
    int m1 = magnitude_sq(make_complex(3, 4));
    printf("Mag sq (3,4): %d\n", m1);
    // EXPECT: Mag sq (3,4): 25
    int m2 = magnitude_sq(make_complex(0, 1000));
    printf("Mag sq (0,1000): %d\n", m2);
    // EXPECT: Mag sq (0,1000): 1000000

    printf("All FFT tests passed\n");
    // EXPECT: All FFT tests passed

    return 0;
}
