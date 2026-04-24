// stress_002_huge_expressions.c - Very long and complex expressions
// EXPECT: long_expr=116
// EXPECT: deep_parens=42
// EXPECT: ternary_chain=80
// EXPECT: comma_expr=45
// EXPECT: precedence=37
// EXPECT: bitwise_mix=255
// EXPECT: shift_chain=4096
// EXPECT: logical_chain=1
// EXPECT: sizeof_expr=13
// EXPECT: nested_assign=150
int printf(const char *fmt, ...);

// --- Single expression with 30+ operators ---
int long_expression(int a, int b, int c) {
    // a=5, b=3, c=7
    // 5+3*7 - 5/1 + 3%3 + 7*2 - 5 + 3 + 7 + 5*1 + 3 - 7 + 5 + 3*2 + 7 - 5 + 3 + 7*3 + 5 - 3 + 7 + 5 + 3 - 7 + 5*4 + 3 + 7 - 5 + 3
    // = 5+21 -5 +0 +14 -5 +3 +7 +5 +3 -7 +5 +6 +7 -5 +3 +21 +5 -3 +7 +5 +3 -7 +20 +3 +7 -5 +3
    // Let me compute step by step:
    // 5 + 21 = 26; 26 - 5 = 21; 21 + 0 = 21; 21 + 14 = 35;
    // 35 - 5 = 30; 30 + 3 = 33; 33 + 7 = 40;
    // 40 + 5 = 45; 45 + 3 = 48; 48 - 7 = 41; 41 + 5 = 46;
    // 46 + 6 = 52; 52 + 7 = 59; 59 - 5 = 54; 54 + 3 = 57;
    // 57 + 21 = 78; 78 + 5 = 83; 83 - 3 = 80; 80 + 7 = 87;
    // 87 + 5 = 92; 92 + 3 = 95; 95 - 7 = 88; 88 + 20 = 108;
    // 108 + 3 = 111; 111 + 7 = 118; 118 - 5 = 113; 113 + 3 = 116
    return a + b * c - a / 1 + b % 3 + c * 2 - a + b + c
         + a * 1 + b - c + a + b * 2 + c - a + b + c * 3
         + a - b + c + a + b - c + a * 4 + b + c - a + b;
}

// --- Deeply parenthesized expression (15 levels) ---
int deep_parens(int x) {
    // x=27, adds 15 -> 42
    return (((((((((((((((x + 1) + 1) + 1) + 1) + 1) + 1) + 1)
            + 1) + 1) + 1) + 1) + 1) + 1) + 1) + 1);
}

// --- Long ternary chain acting as arbitrary lookup table ---
int ternary_chain(int n) {
    return n == 0 ? 10
         : n == 1 ? 20
         : n == 2 ? 30
         : n == 3 ? 40
         : n == 4 ? 50
         : n == 5 ? 60
         : n == 6 ? 70
         : n == 7 ? 80
         : n == 8 ? 90
         : n == 9 ? 100
         : n == 10 ? 110
         : n == 11 ? 120
         : n == 12 ? 130
         : -1;
}

// --- Huge comma expression computing sum 1..9 = 45 ---
int comma_expr(void) {
    int sum = 0;
    int i = 0;
    return (i=1, sum+=i, i=2, sum+=i, i=3, sum+=i, i=4, sum+=i,
            i=5, sum+=i, i=6, sum+=i, i=7, sum+=i, i=8, sum+=i,
            i=9, sum+=i, sum);
}

// --- Complex precedence test mixing many operators ---
int precedence_test(int a, int b, int c) {
    // a=2, b=3, c=4
    // Evaluation by C precedence:
    // a + b*c = 2 + 12 = 14
    // - c/a = - 4/2 = -2 -> 12
    // + b%a = + 3%2 = +1 -> 13
    // + (b<<2) = + 12 -> 25
    // - (c>>1) = - 2 -> 23
    // + (a&b) = + (2&3) = +2 -> 25
    // + (b|c) = + (3|4) = +7 -> 32
    // + (a^b) = + (2^3) = +1 -> 33
    // + (a<b) = +1 -> 34
    // + (c>a) = +1 -> 35
    // + (b<=b) = +1 -> 36
    // + (a>=a) = +1 -> 37
    return a + b * c - c / a + b % a + (b << 2) - (c >> 1)
         + (a & b) + (b | c) + (a ^ b)
         + (a < b) + (c > a) + (b <= b) + (a >= a);
}

// --- Bitwise operations building 0xFF ---
int bitwise_mix(void) {
    return (1 | 2 | 4 | 8 | 16 | 32 | 64 | 128);
}

// --- Chain of left shifts: 1 << 12 = 4096 ---
int shift_chain(int x) {
    return x << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1;
}

// --- Long logical chain returning 1 if all conditions true ---
int logical_chain(void) {
    int a = 1, b = 2, c = 3, d = 4, e = 5;
    int f = 6, g = 7, h = 8, i = 9, j = 10;
    return (a > 0 && b > 0 && c > 0 && d > 0 && e > 0
         && f > 0 && g > 0 && h > 0 && i > 0 && j > 0
         && a < b && b < c && c < d && d < e && e < f
         && f < g && g < h && h < i && i < j);
}

// --- Complex cast and sizeof expression ---
int sizeof_expr(void) {
    // sizeof(int) is typically 4 on most platforms
    int a = (int)(sizeof(int) + sizeof(char) + sizeof(long));
    // 4 + 1 + 8 = 13 on LP64
    return a;
}

// --- Nested assignment expressions ---
int nested_assign(void) {
    int a, b, c, d, e;
    a = b = c = d = e = 10;
    a += b += c += d += e;
    // e=10, d=10+10=20, c=10+20=30, b=10+30=40, a=10+40=50
    return a + b + c + d + e;
}

int main(void) {
    int r1 = long_expression(5, 3, 7);
    printf("long_expr=%d\n", r1);

    int r2 = deep_parens(27);
    printf("deep_parens=%d\n", r2);

    int r3 = ternary_chain(7);
    printf("ternary_chain=%d\n", r3);

    int r4 = comma_expr();
    printf("comma_expr=%d\n", r4);

    int r5 = precedence_test(2, 3, 4);
    printf("precedence=%d\n", r5);

    int r6 = bitwise_mix();
    printf("bitwise_mix=%d\n", r6);

    int r7 = shift_chain(1);
    printf("shift_chain=%d\n", r7);

    int r8 = logical_chain();
    printf("logical_chain=%d\n", r8);

    int r9 = sizeof_expr();
    printf("sizeof_expr=%d\n", r9);

    int r10 = nested_assign();
    printf("nested_assign=%d\n", r10);

    return 0;
}
