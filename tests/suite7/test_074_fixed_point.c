int printf(const char *fmt, ...);

int fp_from_int(int x) {
    return x * 1000;
}

int fp_to_int(int fp) {
    if (fp >= 0) return fp / 1000;
    return -((-fp) / 1000);
}

int fp_frac_part(int fp) {
    if (fp >= 0) return fp % 1000;
    return -((-fp) % 1000);
}

int fp_add(int a, int b) {
    return a + b;
}

int fp_sub(int a, int b) {
    return a - b;
}

int fp_mul(int a, int b) {
    int a_hi = a / 1000;
    int a_lo = a % 1000;
    int b_hi = b / 1000;
    int b_lo = b % 1000;
    return a_hi * b + a_lo * b_hi + a_lo * b_lo / 1000;
}

int fp_div(int a, int b) {
    if (b == 0) return 0;
    return (a * 100) / (b / 10);
}

int fp_abs(int a) {
    if (a < 0) return -a;
    return a;
}

int fp_neg(int a) {
    return -a;
}

int fp_compare(int a, int b) {
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

int fp_min(int a, int b) {
    if (a < b) return a;
    return b;
}

int fp_max(int a, int b) {
    if (a > b) return a;
    return b;
}

int fp_lerp(int a, int b, int t) {
    int diff = fp_sub(b, a);
    int scaled = fp_mul(diff, t);
    return fp_add(a, scaled);
}

int fp_avg(int a, int b) {
    return (a + b) / 2;
}

int fp_weighted_avg(int a, int wa, int b, int wb) {
    int sum_w = wa + wb;
    if (sum_w == 0) return 0;
    return (fp_mul(a, wa) + fp_mul(b, wb)) / (sum_w / 1000);
}

void fp_print(int fp) {
    int whole = fp / 1000;
    int frac = fp % 1000;
    if (fp < 0 && whole == 0) {
        frac = -frac;
        printf("-%d.%d", whole, frac);
    } else if (fp < 0) {
        whole = -whole;
        frac = -frac;
        printf("-%d.%d", whole, frac);
    } else {
        printf("%d.%d", whole, frac);
    }
}

int main(void) {
    int a = fp_from_int(3);
    int b = fp_from_int(7);

    // EXPECT: from_int(3)=3000
    printf("from_int(3)=%d\n", a);

    // EXPECT: from_int(7)=7000
    printf("from_int(7)=%d\n", b);

    // EXPECT: to_int(3500)=3
    printf("to_int(3500)=%d\n", fp_to_int(3500));

    // EXPECT: to_int(-3500)=-3
    printf("to_int(-3500)=%d\n", fp_to_int(-3500));

    // EXPECT: frac(3500)=500
    printf("frac(3500)=%d\n", fp_frac_part(3500));

    // EXPECT: frac(-3500)=-500
    printf("frac(-3500)=%d\n", fp_frac_part(-3500));

    // EXPECT: add=10000
    printf("add=%d\n", fp_add(a, b));

    // EXPECT: sub=-4000
    printf("sub=%d\n", fp_sub(a, b));

    int c = 2500;
    int d = 4000;

    // EXPECT: mul(2500,4000)=10000
    printf("mul(2500,4000)=%d\n", fp_mul(c, d));

    // EXPECT: div(3000,2000)=1500
    printf("div(3000,2000)=%d\n", fp_div(3000, 2000));

    // EXPECT: abs(-5000)=5000
    printf("abs(-5000)=%d\n", fp_abs(-5000));

    // EXPECT: neg(3000)=-3000
    printf("neg(3000)=%d\n", fp_neg(3000));

    // EXPECT: compare(3000,5000)=-1
    printf("compare(3000,5000)=%d\n", fp_compare(3000, 5000));

    // EXPECT: compare(5000,5000)=0
    printf("compare(5000,5000)=%d\n", fp_compare(5000, 5000));

    // EXPECT: compare(7000,2000)=1
    printf("compare(7000,2000)=%d\n", fp_compare(7000, 2000));

    // EXPECT: min(3000,7000)=3000
    printf("min(3000,7000)=%d\n", fp_min(3000, 7000));

    // EXPECT: max(3000,7000)=7000
    printf("max(3000,7000)=%d\n", fp_max(3000, 7000));

    // EXPECT: avg(3000,7000)=5000
    printf("avg(3000,7000)=%d\n", fp_avg(3000, 7000));

    int half = 500;
    // EXPECT: lerp(2000,8000,500)=5000
    printf("lerp(2000,8000,500)=%d\n", fp_lerp(2000, 8000, half));

    // EXPECT: mul(1000,1000)=1000
    printf("mul(1000,1000)=%d\n", fp_mul(1000, 1000));

    // EXPECT: div(6000,3000)=2000
    printf("div(6000,3000)=%d\n", fp_div(6000, 3000));

    return 0;
}
