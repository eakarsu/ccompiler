int printf(const char *fmt, ...);

int sum6(int a, int b, int c, int d, int e, int f) {
    return a + b + c + d + e + f;
}

int sum7(int a, int b, int c, int d, int e, int f, int g) {
    return a + b + c + d + e + f + g;
}

int sum8(int a, int b, int c, int d, int e, int f, int g, int h) {
    return a + b + c + d + e + f + g + h;
}

int weighted6(int a, int b, int c, int d, int e, int f) {
    return a * 1 + b * 2 + c * 3 + d * 4 + e * 5 + f * 6;
}

int max6(int a, int b, int c, int d, int e, int f) {
    int m = a;
    if (b > m) m = b;
    if (c > m) m = c;
    if (d > m) m = d;
    if (e > m) m = e;
    if (f > m) m = f;
    return m;
}

int min6(int a, int b, int c, int d, int e, int f) {
    int m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    if (d < m) m = d;
    if (e < m) m = e;
    if (f < m) m = f;
    return m;
}

int count_positive6(int a, int b, int c, int d, int e, int f) {
    int cnt = 0;
    if (a > 0) cnt = cnt + 1;
    if (b > 0) cnt = cnt + 1;
    if (c > 0) cnt = cnt + 1;
    if (d > 0) cnt = cnt + 1;
    if (e > 0) cnt = cnt + 1;
    if (f > 0) cnt = cnt + 1;
    return cnt;
}

int product_first_last(int a, int b, int c, int d, int e, int f, int g, int h) {
    return a * h;
}

int alternating_sum8(int a, int b, int c, int d, int e, int f, int g, int h) {
    return a - b + c - d + e - f + g - h;
}

int check_sorted6(int a, int b, int c, int d, int e, int f) {
    if (a > b) return 0;
    if (b > c) return 0;
    if (c > d) return 0;
    if (d > e) return 0;
    if (e > f) return 0;
    return 1;
}

int distance6(int x1, int y1, int z1, int x2, int y2, int z2) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    int dz = z1 - z2;
    return dx * dx + dy * dy + dz * dz;
}

int blend_rgb(int r1, int g1, int b1, int r2, int g2, int b2) {
    int r = (r1 + r2) / 2;
    int g = (g1 + g2) / 2;
    int b = (b1 + b2) / 2;
    return r * 10000 + g * 100 + b;
}

int clamp_range(int val, int min1, int max1, int min2, int max2, int use_first) {
    int lo;
    int hi;
    if (use_first) {
        lo = min1;
        hi = max1;
    } else {
        lo = min2;
        hi = max2;
    }
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

int nested_call_7(int a, int b, int c, int d, int e, int f, int g) {
    return sum6(a, b, c, d, e, f) + g;
}

int chain_8(int a, int b, int c, int d, int e, int f, int g, int h) {
    int s1 = sum6(a, b, c, d, e, f);
    int s2 = sum6(c, d, e, f, g, h);
    return s1 + s2;
}

int main(void) {
    // EXPECT: sum6: 21
    printf("sum6: %d\n", sum6(1, 2, 3, 4, 5, 6));

    // EXPECT: sum7: 28
    printf("sum7: %d\n", sum7(1, 2, 3, 4, 5, 6, 7));

    // EXPECT: sum8: 36
    printf("sum8: %d\n", sum8(1, 2, 3, 4, 5, 6, 7, 8));

    // EXPECT: weighted6: 91
    printf("weighted6: %d\n", weighted6(1, 2, 3, 4, 5, 6));

    // EXPECT: max6: 42
    printf("max6: %d\n", max6(3, 42, 17, -5, 28, 9));

    // EXPECT: min6: -5
    printf("min6: %d\n", min6(3, 42, 17, -5, 28, 9));

    // EXPECT: count_pos: 4
    printf("count_pos: %d\n", count_positive6(3, -1, 7, 0, 5, 2));

    // EXPECT: prod_fl: 80
    printf("prod_fl: %d\n", product_first_last(10, 2, 3, 4, 5, 6, 7, 8));

    // EXPECT: alt_sum: -4
    printf("alt_sum: %d\n", alternating_sum8(1, 2, 3, 4, 5, 6, 7, 8));

    // EXPECT: sorted_yes: 1
    printf("sorted_yes: %d\n", check_sorted6(1, 2, 3, 4, 5, 6));

    // EXPECT: sorted_no: 0
    printf("sorted_no: %d\n", check_sorted6(1, 5, 3, 4, 2, 6));

    // EXPECT: dist: 14
    printf("dist: %d\n", distance6(1, 2, 3, 2, 4, 6));

    // EXPECT: blend: 1270127
    printf("blend: %d\n", blend_rgb(255, 0, 0, 0, 0, 255));

    // EXPECT: clamp_first: 5
    printf("clamp_first: %d\n", clamp_range(3, 5, 10, 20, 30, 1));

    // EXPECT: clamp_second: 20
    printf("clamp_second: %d\n", clamp_range(3, 5, 10, 20, 30, 0));

    // EXPECT: nested7: 28
    printf("nested7: %d\n", nested_call_7(1, 2, 3, 4, 5, 6, 7));

    // EXPECT: chain8: 54
    printf("chain8: %d\n", chain_8(1, 2, 3, 4, 5, 6, 7, 8));

    // EXPECT: sum8_neg: -4
    printf("sum8_neg: %d\n", sum8(-1, -2, -3, -4, 1, 2, 3, 0));

    // EXPECT: sum6_zero: 0
    printf("sum6_zero: %d\n", sum6(0, 0, 0, 0, 0, 0));

    return 0;
}
