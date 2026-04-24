int printf(const char *fmt, ...);

int global_val = 100;

int get_ten(void) {
    return 10;
}

int main(void) {
    // Basic += operator
    int a = 10;
    a += 5;
    // EXPECT: 15
    printf("%d\n", a);

    // Basic -= operator
    int b = 20;
    b -= 7;
    // EXPECT: 13
    printf("%d\n", b);

    // Basic *= operator
    int c = 6;
    c *= 8;
    // EXPECT: 48
    printf("%d\n", c);

    // Basic /= operator
    int d = 100;
    d /= 4;
    // EXPECT: 25
    printf("%d\n", d);

    // Basic %= operator
    int e = 17;
    e %= 5;
    // EXPECT: 2
    printf("%d\n", e);

    // Basic &= operator
    int f = 0xFF;
    f &= 0x0F;
    // EXPECT: 15
    printf("%d\n", f);

    // Basic |= operator
    int g = 0x0F;
    g |= 0xF0;
    // EXPECT: 255
    printf("%d\n", g);

    // Basic ^= operator
    int h = 0xFF;
    h ^= 0x0F;
    // EXPECT: 240
    printf("%d\n", h);

    // Basic <<= operator
    int i = 1;
    i <<= 4;
    // EXPECT: 16
    printf("%d\n", i);

    // Basic >>= operator
    int j = 256;
    j >>= 3;
    // EXPECT: 32
    printf("%d\n", j);

    // Chained compound assignment
    int x = 5;
    x += 3;
    x *= 2;
    x -= 4;
    // (5+3)*2 - 4 = 12
    // EXPECT: 12
    printf("%d\n", x);

    // Compound assignment with function call
    int y = 50;
    y -= get_ten();
    // EXPECT: 40
    printf("%d\n", y);

    // Compound assignment on global
    global_val += 50;
    // EXPECT: 150
    printf("%d\n", global_val);

    global_val *= 2;
    // EXPECT: 300
    printf("%d\n", global_val);

    // Compound assignment on array element
    int arr[5];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;
    arr[2] += 15;
    // EXPECT: 45
    printf("%d\n", arr[2]);

    arr[4] >>= 1;
    // EXPECT: 25
    printf("%d\n", arr[4]);

    // Compound assignment on pointer dereference
    int val = 100;
    int *p = &val;
    *p += 23;
    // EXPECT: 123
    printf("%d\n", val);

    *p -= 23;
    *p *= 3;
    // EXPECT: 300
    printf("%d\n", val);

    // Compound assignment in loop
    int sum = 0;
    for (int k = 1; k <= 10; k++) {
        sum += k;
    }
    // 1+2+...+10 = 55
    // EXPECT: 55
    printf("%d\n", sum);

    // Compound assignment with bitwise operations
    int mask = 0;
    mask |= 1;
    mask |= 4;
    mask |= 16;
    // 1 + 4 + 16 = 21
    // EXPECT: 21
    printf("%d\n", mask);

    mask &= 5;
    // 21 & 5: 10101 & 00101 = 00101 = 5
    // EXPECT: 5
    printf("%d\n", mask);

    // Compound shift in loop
    int shifted = 1;
    for (int k = 0; k < 8; k++) {
        shifted <<= 1;
    }
    // 1 << 8 = 256
    // EXPECT: 256
    printf("%d\n", shifted);

    // XOR compound to swap values
    int s1 = 42;
    int s2 = 99;
    s1 ^= s2;
    s2 ^= s1;
    s1 ^= s2;
    // EXPECT: 99
    printf("%d\n", s1);
    // EXPECT: 42
    printf("%d\n", s2);

    // Compound assignment on struct member
    struct { int x; int y; } pt;
    pt.x = 10;
    pt.y = 20;
    pt.x += pt.y;
    // EXPECT: 30
    printf("%d\n", pt.x);

    pt.y *= 3;
    // EXPECT: 60
    printf("%d\n", pt.y);

    // %= with negative dividend
    int neg = -17;
    neg %= 5;
    // C11: result has sign of dividend, -17 % 5 = -2
    // EXPECT: -2
    printf("%d\n", neg);

    // /= with negative
    int neg2 = -100;
    neg2 /= 3;
    // -100 / 3 = -33 (truncation toward zero)
    // EXPECT: -33
    printf("%d\n", neg2);

    return 0;
}
