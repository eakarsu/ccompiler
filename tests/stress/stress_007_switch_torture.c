// stress_007_switch_torture.c - Switch statement torture test
// EXPECT: big_switch_sum=6678
// EXPECT: nested_switch=1111
// EXPECT: fallthrough=15
// EXPECT: neg_case=2
// EXPECT: large_case=9
// EXPECT: duffs_device=1
// EXPECT: expr_switch=500
// EXPECT: block_switch=70
// EXPECT: dispatch=54
int printf(const char *fmt, ...);

// --- Switch with 50+ cases ---
int big_switch(int n) {
    switch (n) {
        case 0: return 100;
        case 1: return 101;
        case 2: return 102;
        case 3: return 103;
        case 4: return 104;
        case 5: return 105;
        case 6: return 106;
        case 7: return 107;
        case 8: return 108;
        case 9: return 109;
        case 10: return 110;
        case 11: return 111;
        case 12: return 112;
        case 13: return 113;
        case 14: return 114;
        case 15: return 115;
        case 16: return 116;
        case 17: return 117;
        case 18: return 118;
        case 19: return 119;
        case 20: return 120;
        case 21: return 121;
        case 22: return 122;
        case 23: return 123;
        case 24: return 124;
        case 25: return 125;
        case 26: return 126;
        case 27: return 127;
        case 28: return 128;
        case 29: return 129;
        case 30: return 130;
        case 31: return 131;
        case 32: return 132;
        case 33: return 133;
        case 34: return 134;
        case 35: return 135;
        case 36: return 136;
        case 37: return 137;
        case 38: return 138;
        case 39: return 139;
        case 40: return 140;
        case 41: return 141;
        case 42: return 142;
        case 43: return 143;
        case 44: return 144;
        case 45: return 145;
        case 46: return 146;
        case 47: return 147;
        case 48: return 148;
        case 49: return 149;
        case 50: return 150;
        case 51: return 151;
        case 52: return 152;
        default: return -1;
    }
}

// --- Nested switch 4 levels ---
int nested_switch(int a, int b, int c, int d) {
    int r = 0;
    switch (a) {
        case 1:
            switch (b) {
                case 10:
                    switch (c) {
                        case 100:
                            switch (d) {
                                case 1000: r = 1111; break;
                                case 2000: r = 1112; break;
                                default: r = 1110; break;
                            }
                            break;
                        case 200:
                            r = 1200;
                            break;
                        default:
                            r = 1000;
                            break;
                    }
                    break;
                case 20:
                    r = 120;
                    break;
                default:
                    r = 100;
                    break;
            }
            break;
        case 2:
            r = 2;
            break;
        default:
            r = 0;
            break;
    }
    return r;
}

// --- Switch with fallthrough ---
int fallthrough_switch(int n) {
    int sum = 0;
    switch (n) {
        case 5: sum += 5;
        case 4: sum += 4;
        case 3: sum += 3;
        case 2: sum += 2;
        case 1: sum += 1;
        case 0: sum += 0;
            break;
        default: sum = -1;
    }
    return sum;
}

// --- Switch with negative and large case values ---
int signed_switch(int n) {
    switch (n) {
        case -100: return 1;
        case -50: return 2;
        case -1: return 3;
        case 0: return 4;
        case 1: return 5;
        case 50: return 6;
        case 100: return 7;
        case 1000: return 8;
        case 10000: return 9;
        default: return -1;
    }
}

// --- Duff's device pattern ---
void duffs_device(int *to, int *from, int count) {
    int n = (count + 7) / 8;
    switch (count % 8) {
        case 0: do { *to++ = *from++;
        case 7:      *to++ = *from++;
        case 6:      *to++ = *from++;
        case 5:      *to++ = *from++;
        case 4:      *to++ = *from++;
        case 3:      *to++ = *from++;
        case 2:      *to++ = *from++;
        case 1:      *to++ = *from++;
                } while (--n > 0);
    }
}

// --- Switch on expressions ---
int expr_switch(int a, int b) {
    switch (a * 10 + b) {
        case 11: return 100;  // a=1, b=1
        case 12: return 200;
        case 21: return 300;  // a=2, b=1
        case 22: return 400;
        case 35: return 500;  // a=3, b=5
        default: return 0;
    }
}

// --- Switch with block scoped variables ---
int block_switch(int n) {
    int result = 0;
    switch (n) {
        case 1: {
            int x = 10;
            result = x * 2;
            break;
        }
        case 2: {
            int x = 20;
            result = x * 3;
            break;
        }
        case 3: {
            int x = 30;
            int y = 40;
            result = x + y;
            break;
        }
        default: {
            int x = -1;
            result = x;
            break;
        }
    }
    return result;
}

// --- Switch with computed goto-like dispatch ---
int dispatch(int op, int a, int b) {
    switch (op) {
        case 0: return a + b;
        case 1: return a - b;
        case 2: return a * b;
        case 3: return a / b;
        case 4: return a % b;
        case 5: return a & b;
        case 6: return a | b;
        case 7: return a ^ b;
        case 8: return a << b;
        case 9: return a >> b;
        default: return 0;
    }
}

int main(void) {
    // Big switch
    int sum = 0;
    int i;
    for (i = 0; i <= 52; i++) {
        sum += big_switch(i);
    }
    // sum = 100+101+...+152 = 53 terms, avg 126, = 53*126 = 6678
    printf("big_switch_sum=%d\n", sum);

    // Nested switch
    printf("nested_switch=%d\n", nested_switch(1, 10, 100, 1000));

    // Fallthrough: fallthrough_switch(5) = 5+4+3+2+1+0 = 15
    printf("fallthrough=%d\n", fallthrough_switch(5));

    // Signed switch
    printf("neg_case=%d\n", signed_switch(-50));
    printf("large_case=%d\n", signed_switch(10000));

    // Duff's device
    int src[20], dst[20];
    for (i = 0; i < 20; i++) src[i] = i * 3;
    duffs_device(dst, src, 20);
    int duff_ok = 1;
    for (i = 0; i < 20; i++) {
        if (dst[i] != i * 3) duff_ok = 0;
    }
    printf("duffs_device=%d\n", duff_ok);

    // Expression switch
    printf("expr_switch=%d\n", expr_switch(3, 5));

    // Block switch
    printf("block_switch=%d\n", block_switch(3));

    // Dispatch
    int dsum = 0;
    dsum += dispatch(0, 10, 3); // 13
    dsum += dispatch(1, 10, 3); // 7
    dsum += dispatch(2, 10, 3); // 30
    dsum += dispatch(3, 10, 3); // 3
    dsum += dispatch(4, 10, 3); // 1
    printf("dispatch=%d\n", dsum); // 54

    return 0;
}
