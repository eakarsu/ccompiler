int printf(const char *fmt, ...);
// EXPECT: === Char Overflow ===\nc+=1000: -14\nc*=3: 44\nc<<=4: 80\nc<<=6: 64\n=== All Compound Ops (char) ===\n+=: -106\n-=: -50\n*=: -56\n/=: 33\n%=: 2\n&=: 10\n|=: 90\n^=: -16\n<<=: 96\n>>=: 30\n=== Int No Truncation ===\nint+=: 300\n=== Loop Accumulation ===\nloop: 44\n=== Chained Compound ===\nchain: a=60 b=50 c=30\n=== Pointer Compound ===\nptr: 45\n=== Result in Expression ===\nexpr: x=10 y=10\n=== Negative and Boundary ===\nneg+=: 106\nnegmul: 1\nnegwrap: 127\nmaxwrap: -128\nmax2: -127\n=== Sequential Compound ===\nseq: 18\n=== Array Element Compound ===\narr: 30 20 60 -60\n=== Condition and Struct ===\ncond: yes\nstruct: -106 -56 6000\n=== Int Bitwise and Shift ===\nbw&=: 43776\nbw|=: 44015\nbw^=: 21520\nsh<<=: 1024\nsh>>=: 128\n=== While Compound ===\nwhile: wh=0 cnt=9\n=== Nested and Ternary ===\nnested: 12 7\nternary: 110\n=== Extra Char Ops ===\nbig: -56\nmod: 10\nrsneg: -4\nchain3: 10 9 7

// Test: Compound assignment operators with type promotion
// Focuses on char truncation behavior and all compound operators

void test_char_overflow(void) {
    printf("=== Char Overflow ===\n");
    // 10 + 1000 = 1010, truncated to char: -14
    char c1 = 10;
    c1 += 1000;
    printf("c+=1000: %d\n", c1);
    // 100 * 3 = 300, truncated to char: 44
    char c2 = 100;
    c2 *= 3;
    printf("c*=3: %d\n", c2);
    // 5 << 4 = 80, fits in char
    char c3 = 5;
    c3 <<= 4;
    printf("c<<=4: %d\n", c3);
    // 5 << 6 = 320, truncated to char: 64
    char c4 = 5;
    c4 <<= 6;
    printf("c<<=6: %d\n", c4);
}

void test_all_compound_ops(void) {
    printf("=== All Compound Ops (char) ===\n");
    // += : 50 + 100 = 150 -> -106
    char ca = 50; ca += 100;
    printf("+=: %d\n", ca);
    // -= : 50 - 100 = -50
    char cb = 50; cb -= 100;
    printf("-=: %d\n", cb);
    // *= : 20 * 10 = 200 -> -56
    char cc = 20; cc *= 10;
    printf("*=: %d\n", cc);
    // /= : 100 / 3 = 33
    char cd = 100; cd /= 3;
    printf("/=: %d\n", cd);
    // %= : 100 % 7 = 2
    char ce = 100; ce %= 7;
    printf("%%=: %d\n", ce);
    // &= : 90 & 15 = 10
    char cf = 90; cf &= 15;
    printf("&=: %d\n", cf);
    // |= : 80 | 10 = 90
    char cg = 80; cg |= 10;
    printf("|=: %d\n", cg);
    // ^= : -1 ^ 15 = -16
    char ch = -1; ch ^= 15;
    printf("^=: %d\n", ch);
    // <<= : 3 << 5 = 96
    char ci = 3; ci <<= 5;
    printf("<<=: %d\n", ci);
    // >>= : 120 >> 2 = 30
    char cj = 120; cj >>= 2;
    printf(">>=: %d\n", cj);
}

void test_int_no_truncation(void) {
    printf("=== Int No Truncation ===\n");
    int x = 100;
    x += 200;
    printf("int+=: %d\n", x);
}

void test_loop_accumulation(void) {
    printf("=== Loop Accumulation ===\n");
    // 10 iterations of c += 30: sum = 300 -> truncated to 44
    char c = 0;
    int i;
    for (i = 0; i < 10; i++) c += 30;
    printf("loop: %d\n", c);
}

void test_chained_compound(void) {
    printf("=== Chained Compound ===\n");
    // Right-to-left: b += c -> 50, then a += b -> 60
    char a = 10;
    char b = 20;
    char c = 30;
    a += b += c;
    printf("chain: a=%d b=%d c=%d\n", a, b, c);
}

void test_pointer_compound(void) {
    printf("=== Pointer Compound ===\n");
    char val = 40;
    char *p = &val;
    *p += 5;
    printf("ptr: %d\n", val);
}

void test_expr_result(void) {
    printf("=== Result in Expression ===\n");
    int x = 7;
    int y = (x += 3);
    printf("expr: x=%d y=%d\n", x, y);
}

void test_negative_and_boundary(void) {
    printf("=== Negative and Boundary ===\n");
    // -100 + -50 = -150 -> truncated: 106
    char cn1 = -100; cn1 += -50;
    printf("neg+=: %d\n", cn1);
    // -1 * -1 = 1
    char cn2 = -1; cn2 *= -1;
    printf("negmul: %d\n", cn2);
    // -128 - 1 = -129 -> truncated: 127
    char cn3 = -128; cn3 -= 1;
    printf("negwrap: %d\n", cn3);
    // 127 + 1 = 128 -> truncated: -128
    char cu1 = 127; cu1 += 1;
    printf("maxwrap: %d\n", cu1);
    // 127 + 2 = 129 -> truncated: -127
    char cu2 = 127; cu2 += 2;
    printf("max2: %d\n", cu2);
}

void test_sequential(void) {
    printf("=== Sequential Compound ===\n");
    // 10 +20=30, *2=60, -5=55, /3=18
    char cs = 10;
    cs += 20;
    cs *= 2;
    cs -= 5;
    cs /= 3;
    printf("seq: %d\n", cs);
}

void test_array_compound(void) {
    printf("=== Array Element Compound ===\n");
    char arr[4];
    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40;
    arr[0] += arr[1];   // 30
    arr[2] *= 2;        // 60
    arr[3] -= 100;      // -60
    printf("arr: %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3]);
}

void test_condition_and_struct(void) {
    printf("=== Condition and Struct ===\n");
    // Compound assignment result used in if condition
    int cond = 5;
    if ((cond += 3) == 8) printf("cond: yes\n");
    else printf("cond: no\n");
    // Struct member compound assignments
    struct { char x; char y; int z; } s;
    s.x = 50; s.y = 100; s.z = 1000;
    s.x += 100;   // 150 -> -106
    s.y *= 2;     // 200 -> -56
    s.z += 5000;  // 6000
    printf("struct: %d %d %d\n", s.x, s.y, s.z);
}

void test_int_bitwise_shift(void) {
    printf("=== Int Bitwise and Shift ===\n");
    int bw = 43981;   // 0xABCD
    bw &= 65280;      // 0xAB00 = 43776
    printf("bw&=: %d\n", bw);
    bw |= 239;        // 0xABEF = 44015
    printf("bw|=: %d\n", bw);
    bw ^= 65535;      // 0x5410 = 21520
    printf("bw^=: %d\n", bw);
    int sh = 1;
    sh <<= 10;        // 1024
    printf("sh<<=: %d\n", sh);
    sh >>= 3;         // 128
    printf("sh>>=: %d\n", sh);
}

void test_while_compound(void) {
    printf("=== While Compound ===\n");
    // Subtract 100 each iteration until <= 0
    // 900,800,...,100 -> 9 iterations, final wh=0
    int wh = 1000;
    int cnt = 0;
    while ((wh -= 100) > 0) cnt++;
    printf("while: wh=%d cnt=%d\n", wh, cnt);
}

void test_nested_and_ternary(void) {
    printf("=== Nested and Ternary ===\n");
    // na += (nb -= 3) -> nb=7, na=12
    char na = 5; char nb = 10;
    na += (nb -= 3);
    printf("nested: %d %d\n", na, nb);
    // tv += ternary: tv > 5 is true, so tv += 100 -> 110
    int tv = 10;
    tv += (tv > 5) ? 100 : 1;
    printf("ternary: %d\n", tv);
}

void test_extra_char_ops(void) {
    printf("=== Extra Char Ops ===\n");
    // 2 * 100 = 200 -> truncated: -56
    char big = 2; big *= 100;
    printf("big: %d\n", big);
    // 100 % 30 = 10
    char md = 100; md %= 30;
    printf("mod: %d\n", md);
    // -16 >> 2 = -4 (arithmetic right shift)
    char rs = -16; rs >>= 2;
    printf("rsneg: %d\n", rs);
    // Triple chain: mc=3+4=7, mb=2+7=9, ma=1+9=10
    int ma = 1; int mb = 2; int mc = 3;
    ma += mb += mc += 4;
    printf("chain3: %d %d %d\n", ma, mb, mc);
}

int main(void) {
    test_char_overflow();
    test_all_compound_ops();
    test_int_no_truncation();
    test_loop_accumulation();
    test_chained_compound();
    test_pointer_compound();
    test_expr_result();
    test_negative_and_boundary();
    test_sequential();
    test_array_compound();
    test_condition_and_struct();
    test_int_bitwise_shift();
    test_while_compound();
    test_nested_and_ternary();
    test_extra_char_ops();
    return 0;
}
