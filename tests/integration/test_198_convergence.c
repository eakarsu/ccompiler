int printf(const char *fmt, ...);
// EXPECT: Harmonic partial sums (x1000):\nH(1)=1000 H(2)=1500 H(3)=1833 H(4)=2083 H(5)=2283 H(6)=2449 H(7)=2591 H(8)=2716 H(9)=2827 H(10)=2927 H(11)=3017 H(12)=3100 H(13)=3176 H(14)=3247 H(15)=3313 H(16)=3375 H(17)=3433 H(18)=3488 H(19)=3540 H(20)=3590 \nAlternating series (x10000):\npi/4 approx (x10000) = 7732\nBasel series (x10000):\npi^2/6 approx (x10000) = 16230\nFixed-point iteration x=cos(x)*1000:\nfixed point=739 steps=18\nGeometric series (x1000):\n1000 1500 1750 1875 1937 1968 1983 1990 1993 1994 1994 1994 1994 1994 1994 \nlimit should approach 2000\nPower series e^1 (x1000):\nterm 0: e=1000\nterm 1: e=2000\nterm 2: e=2500\nterm 3: e=2666\nterm 4: e=2707\nterm 5: e=2715\nterm 6: e=2716\nterm 7: e=2716\nterm 8: e=2716\nterm 9: e=2716\nterm 10: e=2716\nIterated averaging:\na=150 b=150 a=150 b=150 a=150 b=150 a=150 b=150 a=150 b=150 a=150 b=150 a=150 b=150 a=150 b=150 a=150 b=150 a=150 b=150 
int main(void) {
    int i;

    printf("Harmonic partial sums (x1000):\n");
    int harm = 0;
    for (i = 1; i <= 20; i++) {
        harm = harm + 1000 / i;
        printf("H(%d)=%d ", i, harm);
    }
    printf("\n");

    printf("Alternating series (x10000):\n");
    int alt = 0;
    for (i = 1; i <= 20; i++) {
        if (i % 2 == 1) {
            alt = alt + 10000 / (2 * i - 1);
        } else {
            alt = alt - 10000 / (2 * i - 1);
        }
    }
    printf("pi/4 approx (x10000) = %d\n", alt);

    printf("Basel series (x10000):\n");
    int basel = 0;
    for (i = 1; i <= 50; i++) {
        basel = basel + 10000 / (i * i);
    }
    printf("pi^2/6 approx (x10000) = %d\n", basel);

    printf("Fixed-point iteration x=cos(x)*1000:\n");
    int x = 1000;
    int prev;
    int steps = 0;
    for (i = 0; i < 20; i++) {
        int x2 = (x * x) / 1000;
        int x4 = (x2 * x2) / 1000;
        int cos_approx = 1000 - x2 / 2 + x4 / 24;
        prev = x;
        x = cos_approx;
        steps++;
        if (x == prev) break;
    }
    printf("fixed point=%d steps=%d\n", x, steps);

    printf("Geometric series (x1000):\n");
    int ratio_num = 1;
    int ratio_den = 2;
    int geo = 0;
    int term = 1000;
    for (i = 0; i < 15; i++) {
        geo = geo + term;
        term = term * ratio_num / ratio_den;
        printf("%d ", geo);
    }
    printf("\nlimit should approach 2000\n");

    printf("Power series e^1 (x1000):\n");
    int e_approx = 0;
    int factorial = 1;
    for (i = 0; i <= 10; i++) {
        if (i > 0) factorial = factorial * i;
        e_approx = e_approx + 1000 / factorial;
        printf("term %d: e=%d\n", i, e_approx);
    }

    printf("Iterated averaging:\n");
    int a = 100;
    int b = 200;
    for (i = 0; i < 10; i++) {
        int new_a = (a + b) / 2;
        int new_b = (a + b + 1) / 2;
        a = new_a;
        b = new_b;
        printf("a=%d b=%d ", a, b);
    }
    printf("\n");

    return 0;
}
