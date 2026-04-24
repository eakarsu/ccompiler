int printf(const char *fmt, ...);

int main(void) {
    // Multiplication before addition: a + b * c
    int r1 = 2 + 3 * 4;
    // 2 + 12 = 14
    // EXPECT: 14
    printf("%d\n", r1);

    // Division before subtraction: a - b / c
    int r2 = 20 - 12 / 4;
    // 20 - 3 = 17
    // EXPECT: 17
    printf("%d\n", r2);

    // Modulo same precedence as multiplication
    int r3 = 2 + 10 % 3;
    // 2 + 1 = 3
    // EXPECT: 3
    printf("%d\n", r3);

    // Left-to-right associativity for arithmetic
    int r4 = 100 / 10 / 2;
    // (100/10)/2 = 10/2 = 5
    // EXPECT: 5
    printf("%d\n", r4);

    // Shift lower precedence than addition
    int r5 = 1 << 2 + 3;
    // 1 << (2+3) = 1 << 5 = 32
    // EXPECT: 32
    printf("%d\n", r5);

    // Addition before shift
    int r6 = 3 + 1 << 2;
    // (3+1) << 2 = 4 << 2 = 16
    // EXPECT: 16
    printf("%d\n", r6);

    // Comparison lower than shift
    int r7 = 1 << 3 > 4;
    // (1 << 3) > 4 => 8 > 4 => 1
    // EXPECT: 1
    printf("%d\n", r7);

    // Comparison lower than addition
    int r8 = 3 + 2 > 4;
    // (3+2) > 4 => 5 > 4 => 1
    // EXPECT: 1
    printf("%d\n", r8);

    // Equality lower than comparison
    int r9 = 1 < 2 == 1;
    // (1 < 2) == 1 => 1 == 1 => 1
    // EXPECT: 1
    printf("%d\n", r9);

    // Bitwise AND lower than equality
    int r10 = 7 & 3 == 3;
    // 7 & (3 == 3) => 7 & 1 => 1
    // EXPECT: 1
    printf("%d\n", r10);

    // Bitwise XOR lower than AND
    int r11 = 6 ^ 3 & 5;
    // 6 ^ (3 & 5) => 6 ^ 1 => 7
    // EXPECT: 7
    printf("%d\n", r11);

    // Bitwise OR lower than XOR
    int r12 = 1 | 2 ^ 3;
    // 1 | (2 ^ 3) => 1 | 1 => 1
    // EXPECT: 1
    printf("%d\n", r12);

    // Logical AND lower than bitwise OR
    int r13 = 1 | 0 && 0 | 1;
    // (1 | 0) && (0 | 1) => 1 && 1 => 1
    // EXPECT: 1
    printf("%d\n", r13);

    // Logical OR lower than logical AND
    int r14 = 0 || 1 && 1;
    // 0 || (1 && 1) => 0 || 1 => 1
    // EXPECT: 1
    printf("%d\n", r14);

    int r15 = 1 || 0 && 0;
    // 1 || (0 && 0) => 1 || 0 => 1
    // EXPECT: 1
    printf("%d\n", r15);

    // Ternary lower than logical OR
    int r16 = 1 ? 10 : 20;
    // EXPECT: 10
    printf("%d\n", r16);

    int r17 = 0 || 1 ? 10 : 20;
    // (0 || 1) ? 10 : 20 => 1 ? 10 : 20 => 10
    // EXPECT: 10
    printf("%d\n", r17);

    // Unary higher than binary
    int r18 = -3 * 2;
    // (-3) * 2 = -6
    // EXPECT: -6
    printf("%d\n", r18);

    // Logical NOT higher than comparison
    int r19 = !0 > 0;
    // (!0) > 0 => 1 > 0 => 1
    // EXPECT: 1
    printf("%d\n", r19);

    // Bitwise NOT higher than arithmetic
    int r20 = ~0 + 2;
    // (~0) + 2 => -1 + 2 => 1
    // EXPECT: 1
    printf("%d\n", r20);

    // Complex precedence chain: * before + before << before < before & before ^ before | before && before ||
    // 2 * 3 + 1 << 1 < 20 => ((2*3)+1) << 1 < 20 => 7 << (1 < 20) => 7 << 1 => 14
    // Wait, let me be more careful:
    // 2 * 3 + 1 << 1 < 20
    // Step 1: 2*3 = 6
    // Step 2: 6+1 = 7
    // Step 3: 7 << 1 = 14
    // Step 4: 14 < 20 = 1
    int r21 = 2 * 3 + 1 << 1;
    // (2*3+1) << 1 = 7 << 1 = 14
    // EXPECT: 14
    printf("%d\n", r21);

    // Parentheses override precedence
    int r22 = (2 + 3) * 4;
    // EXPECT: 20
    printf("%d\n", r22);

    int r23 = 2 * (3 + 4);
    // EXPECT: 14
    printf("%d\n", r23);

    // Nested comparison operators
    int r24 = (5 > 3) + (3 < 5) + (4 >= 4) + (4 <= 4) + (5 != 3) + (3 == 3);
    // 1 + 1 + 1 + 1 + 1 + 1 = 6
    // EXPECT: 6
    printf("%d\n", r24);

    // Right-to-left associativity of assignment
    int ra;
    int rb;
    int rc;
    ra = rb = rc = 7;
    // rc=7, rb=7, ra=7
    // EXPECT: 7
    printf("%d\n", ra);
    // EXPECT: 7
    printf("%d\n", rb);

    // Short-circuit evaluation: && stops at first false
    int sc_a = 0;
    int sc_b = 0;
    int sc_r = (sc_a = 1) && (sc_b = 0) && (sc_a = 99);
    // sc_a=1, then 1 && (sc_b=0) => sc_b=0, 0 is false, short circuit
    // sc_a should still be 1 (99 never assigned)
    // EXPECT: 0
    printf("%d\n", sc_r);
    // EXPECT: 1
    printf("%d\n", sc_a);
    // EXPECT: 0
    printf("%d\n", sc_b);

    // Short-circuit evaluation: || stops at first true
    int sc2_a = 0;
    int sc2_b = 0;
    int sc2_r = (sc2_a = 5) || (sc2_b = 10);
    // sc2_a=5, 5 is true, short circuit, sc2_b not assigned
    // EXPECT: 1
    printf("%d\n", sc2_r);
    // EXPECT: 5
    printf("%d\n", sc2_a);
    // EXPECT: 0
    printf("%d\n", sc2_b);

    // Verify: bitwise ops don't short-circuit
    int bw_a = 0;
    int bw_b = 0;
    int bw_r = (bw_a = 3) & (bw_b = 5);
    // Both sides evaluated: 3 & 5 = 1
    // EXPECT: 1
    printf("%d\n", bw_r);
    // EXPECT: 3
    printf("%d\n", bw_a);
    // EXPECT: 5
    printf("%d\n", bw_b);

    // Precedence: unary minus vs multiplication
    int r25 = 2 * -3;
    // 2 * (-3) = -6
    // EXPECT: -6
    printf("%d\n", r25);

    // sizeof higher than arithmetic
    int r26 = sizeof(int) + 1;
    // 4 + 1 = 5
    // EXPECT: 5
    printf("%d\n", r26);

    return 0;
}
