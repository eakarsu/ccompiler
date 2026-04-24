// stress_001_deep_nesting.c - Deeply nested constructs stress test
// EXPECT: nested_if=255
// EXPECT: nested_for=720
// EXPECT: nested_switch=42
// EXPECT: deep_call=120
// EXPECT: nested_ternary=64
// EXPECT: nested_struct=999
// EXPECT: combined=2200

int printf(const char *fmt, ...);

// --- 8-level nested if/else computing a bitmask ---
int nested_if(int a, int b, int c, int d, int e, int f, int g, int h) {
    int result = 0;
    if (a > 0) {
        result |= 1;
        if (b > 0) {
            result |= 2;
            if (c > 0) {
                result |= 4;
                if (d > 0) {
                    result |= 8;
                    if (e > 0) {
                        result |= 16;
                        if (f > 0) {
                            result |= 32;
                            if (g > 0) {
                                result |= 64;
                                if (h > 0) {
                                    result |= 128;
                                } else {
                                    result |= 0;
                                }
                            } else {
                                result &= ~64;
                            }
                        } else {
                            result &= ~32;
                        }
                    } else {
                        result &= ~16;
                    }
                } else {
                    result &= ~8;
                }
            } else {
                result &= ~4;
            }
        } else {
            result &= ~2;
        }
    } else {
        result = 0;
    }
    return result;
}

// --- 6-level nested for loops computing 6! = 720 ---
int nested_for(void) {
    int count = 0;
    int i, j, k, l, m, n;
    for (i = 0; i < 1; i++) {
        for (j = 0; j < 2; j++) {
            for (k = 0; k < 3; k++) {
                for (l = 0; l < 4; l++) {
                    for (m = 0; m < 5; m++) {
                        for (n = 0; n < 6; n++) {
                            count++;
                        }
                    }
                }
            }
        }
    }
    return count;
}

// --- 5-level nested switch ---
int nested_switch(int a, int b, int c, int d, int e) {
    int r = 0;
    switch (a) {
        case 1:
            switch (b) {
                case 2:
                    switch (c) {
                        case 3:
                            switch (d) {
                                case 4:
                                    switch (e) {
                                        case 5: r = 42; break;
                                        case 6: r = 43; break;
                                        default: r = -1; break;
                                    }
                                    break;
                                case 7: r = 50; break;
                                default: r = -2; break;
                            }
                            break;
                        case 8: r = 60; break;
                        default: r = -3; break;
                    }
                    break;
                case 9: r = 70; break;
                default: r = -4; break;
            }
            break;
        case 10: r = 80; break;
        default: r = -5; break;
    }
    return r;
}

// --- Deeply nested function calls (15+ deep) computing factorial ---
int f1(int n);
int f2(int n);
int f3(int n);
int f4(int n);
int f5(int n);
int f6(int n);
int f7(int n);
int f8(int n);
int f9(int n);
int f10(int n);
int f11(int n);
int f12(int n);
int f13(int n);
int f14(int n);
int f15(int n);

int f15(int n) { return n <= 1 ? 1 : n * f14(n - 1); }
int f14(int n) { return f15(n); }
int f13(int n) { return f14(n); }
int f12(int n) { return f13(n); }
int f11(int n) { return f12(n); }
int f10(int n) { return f11(n); }
int f9(int n)  { return f10(n); }
int f8(int n)  { return f9(n); }
int f7(int n)  { return f8(n); }
int f6(int n)  { return f7(n); }
int f5(int n)  { return f6(n); }
int f4(int n)  { return f5(n); }
int f3(int n)  { return f4(n); }
int f2(int n)  { return f3(n); }
int f1(int n)  { return f2(n); }

// --- 6-deep nested ternary computing power of 2 ---
int nested_ternary(int n) {
    return n == 0 ? 1
         : n == 1 ? 2
         : n == 2 ? 4
         : n == 3 ? 8
         : n == 4 ? 16
         : n == 5 ? 32
         : n == 6 ? 64
         : -1;
}

// --- 5-level nested struct access ---
struct L5 { int val; };
struct L4 { struct L5 inner; int pad; };
struct L3 { struct L4 inner; int pad; };
struct L2 { struct L3 inner; int pad; };
struct L1 { struct L2 inner; int pad; };

int nested_struct_access(void) {
    struct L1 s;
    s.inner.inner.inner.inner.val = 999;
    s.inner.inner.inner.pad = 10;
    s.inner.inner.pad = 20;
    s.inner.pad = 30;
    s.pad = 40;
    return s.inner.inner.inner.inner.val;
}

int main(void) {
    int r1 = nested_if(1, 1, 1, 1, 1, 1, 1, 1);
    printf("nested_if=%d\n", r1);

    int r2 = nested_for();
    printf("nested_for=%d\n", r2);

    int r3 = nested_switch(1, 2, 3, 4, 5);
    printf("nested_switch=%d\n", r3);

    int r4 = f1(5);
    printf("deep_call=%d\n", r4);

    int r5 = nested_ternary(6);
    printf("nested_ternary=%d\n", r5);

    int r6 = nested_struct_access();
    printf("nested_struct=%d\n", r6);

    int combined = r1 + r2 + r3 + r4 + r5 + r6;
    printf("combined=%d\n", combined);

    return 0;
}
