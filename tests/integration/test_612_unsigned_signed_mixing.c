int printf(const char *fmt, ...);
// EXPECT: === 1. Signed vs Unsigned Comparison ===\n-1 < 1u: 0\n0 < 1u: 1\n-1 == UINT_MAX: 1\n1 < 0u+0u: 0\n=== 2. Unsigned Wrap-Around ===\nwrap_sub: 4294967295\nwrap_add: 0\nwrap_add2: 100\nwrap_mul: 4294967286\n=== 3. Unsigned Subtraction Wrapping ===\n5 - 10 unsigned: 4294967291\n0 - 1 unsigned: 4294967295\n1 - 2 unsigned: 4294967295\n10 - 10 unsigned: 0\n=== 4. Unsigned Addition Wrapping ===\nmax + 1: 0\nmax + 2: 1\nmax + max: 4294967294\nhalf + half: 4294967294\nhalf + half + 2: 0\n=== 5. Signed vs Unsigned Division ===\nsigned -10 / 3: -3\nsigned -10 % 3: -1\nunsigned big / 3: 1431655762\nunsigned big % 3: 0\n=== 6. Bitwise on Signed vs Unsigned ===\nsigned -1 & 0xFF: 255\nsigned -1 | 0: -1\nsigned -1 ^ 0: -1\nunsigned max & 0xFF: 255\nunsigned max | 0: 4294967295\nunsigned max ^ 0: 4294967295\n~0 signed: -1\n~0u unsigned: 4294967295\n=== 7. Right Shift: Arithmetic vs Logical ===\n(-1) >> 1 signed: -1\n(-1) >> 16 signed: -1\n(-1) >> 31 signed: -1\nUINT_MAX >> 1 unsigned: 2147483647\nUINT_MAX >> 16 unsigned: 65535\nUINT_MAX >> 31 unsigned: 1\n(-128) >> 1 signed: -64\n(-128) >> 7 signed: -1\n128u >> 1: 64\n=== 8. Casting Between Signed and Unsigned ===\n(unsigned)(-1): 4294967295\n(unsigned)(-2): 4294967294\n(unsigned)(-128): 4294967168\n(int)(4294967295u): -1\n(int)(4294967294u): -2\n(int)(2147483648u): -2147483648\n(int)(2147483647u): 2147483647\n=== 9. Mixed Arithmetic Result Type ===\n-1 + 1u: 0\n-2 + 1u: 4294967295\n1 + UINT_MAX: 0\n-1 * 2u: 4294967294\n=== 10. Comparison Chains Mixed ===\nchain1: 0\nchain2: 1\nchain3: 0\nchain4: 1\n=== 11. Overflow Detection Patterns ===\nadd_overflowed(max,1): 1\nadd_overflowed(max-1,1): 0\nadd_overflowed(100,200): 0\nadd_overflowed(max,max): 1\nsub_underflowed(0,1): 1\nsub_underflowed(1,1): 0\nsub_underflowed(5,3): 0\nsub_underflowed(0,max): 1\n=== 12. Unsigned Loop Patterns ===\ncountdown: 3 2 1 0 done\nupcount: 0 1 2 4294967295 done\n=== 13. Bit Pattern Reinterpretation ===\nneg1_as_unsigned: 4294967295\nmax_unsigned_as_signed: -1\nhex_80000000_as_signed: -2147483648\nhex_7FFFFFFF_as_signed: 2147483647\n=== 14. Division Edge Cases ===\nunsigned 0/1: 0\nunsigned 0%1: 0\nunsigned max/1: 4294967295\nunsigned max/max: 1\nunsigned max%max: 0\n=== 15. Shift Edge Cases ===\n1u << 31: 2147483648\n1 << 31 as signed: -2147483648\n1u << 0: 1\n0u << 5: 0\nAll tests passed.

// Test 612: Signed/unsigned integer mixing edge cases

int main() {
    // ==========================================
    // 1. Signed vs Unsigned Comparison
    // ==========================================
    printf("=== 1. Signed vs Unsigned Comparison ===\n");
    {
        unsigned u = 1;
        int s = -1;
        // In C, when comparing signed and unsigned int, signed is converted to unsigned.
        // -1 becomes 4294967295u, which is NOT < 1u => result is 0
        int result = (s < u);
        printf("-1 < 1u: %d\n", result);

        int s2 = 0;
        int result2 = (s2 < u);
        printf("0 < 1u: %d\n", result2);

        // -1 as unsigned == UINT_MAX
        unsigned umax = 4294967295u;
        int result3 = ((unsigned)s == umax);
        printf("-1 == UINT_MAX: %d\n", result3);

        // 1 < 0u+0u => 1 < 0u => 0 (1 is not less than 0)
        unsigned zero = 0;
        int result4 = (1 < zero + zero);
        printf("1 < 0u+0u: %d\n", result4);
    }

    // ==========================================
    // 2. Unsigned Wrap-Around
    // ==========================================
    printf("=== 2. Unsigned Wrap-Around ===\n");
    {
        unsigned x = 0;
        x = x - 1;
        printf("wrap_sub: %u\n", x);

        unsigned y = 4294967295u;
        y = y + 1;
        printf("wrap_add: %u\n", y);

        unsigned z = 4294967295u;
        z = z + 101;
        printf("wrap_add2: %u\n", z);

        unsigned w = 4294967295u;
        w = w * 10;
        printf("wrap_mul: %u\n", w);
    }

    // ==========================================
    // 3. Unsigned Subtraction Wrapping
    // ==========================================
    printf("=== 3. Unsigned Subtraction Wrapping ===\n");
    {
        unsigned a = 5;
        unsigned b = 10;
        unsigned r = a - b;
        printf("5 - 10 unsigned: %u\n", r);

        unsigned c = 0;
        unsigned d = 1;
        unsigned r2 = c - d;
        printf("0 - 1 unsigned: %u\n", r2);

        unsigned e = 1;
        unsigned f = 2;
        unsigned r3 = e - f;
        printf("1 - 2 unsigned: %u\n", r3);

        unsigned g = 10;
        unsigned h = 10;
        unsigned r4 = g - h;
        printf("10 - 10 unsigned: %u\n", r4);
    }

    // ==========================================
    // 4. Unsigned Addition Wrapping
    // ==========================================
    printf("=== 4. Unsigned Addition Wrapping ===\n");
    {
        unsigned max = 4294967295u;
        unsigned r1 = max + 1;
        printf("max + 1: %u\n", r1);

        unsigned r2 = max + 2;
        printf("max + 2: %u\n", r2);

        unsigned r3 = max + max;
        printf("max + max: %u\n", r3);

        unsigned half = 2147483647u;
        unsigned r4 = half + half;
        printf("half + half: %u\n", r4);

        unsigned r5 = half + half + 2;
        printf("half + half + 2: %u\n", r5);
    }

    // ==========================================
    // 5. Signed vs Unsigned Division and Modulo
    // ==========================================
    printf("=== 5. Signed vs Unsigned Division ===\n");
    {
        int sa = -10;
        int sb = 3;
        // signed division: -10 / 3 = -3 (truncated toward zero)
        printf("signed -10 / 3: %d\n", sa / sb);
        // signed modulo: -10 % 3 = -1
        printf("signed -10 %% 3: %d\n", sa % sb);

        // (unsigned)(-10) = 4294967286
        // 4294967286 / 3 = 1431655762, remainder 0
        unsigned ua = (unsigned)sa;
        unsigned ub = (unsigned)sb;
        printf("unsigned big / 3: %u\n", ua / ub);
        printf("unsigned big %% 3: %u\n", ua % ub);
    }

    // ==========================================
    // 6. Bitwise Operations on Signed vs Unsigned
    // ==========================================
    printf("=== 6. Bitwise on Signed vs Unsigned ===\n");
    {
        int sneg = -1;
        printf("signed -1 & 0xFF: %d\n", sneg & 0xFF);
        printf("signed -1 | 0: %d\n", sneg | 0);
        printf("signed -1 ^ 0: %d\n", sneg ^ 0);

        unsigned umax = 4294967295u;
        printf("unsigned max & 0xFF: %u\n", umax & 0xFF);
        printf("unsigned max | 0: %u\n", umax | 0);
        printf("unsigned max ^ 0: %u\n", umax ^ 0);

        int zero = 0;
        printf("~0 signed: %d\n", ~zero);
        unsigned uzero = 0;
        printf("~0u unsigned: %u\n", ~uzero);
    }

    // ==========================================
    // 7. Right Shift: Arithmetic vs Logical
    // ==========================================
    printf("=== 7. Right Shift: Arithmetic vs Logical ===\n");
    {
        int sneg = -1;
        printf("(-1) >> 1 signed: %d\n", sneg >> 1);
        printf("(-1) >> 16 signed: %d\n", sneg >> 16);
        printf("(-1) >> 31 signed: %d\n", sneg >> 31);

        unsigned umax = 4294967295u;
        printf("UINT_MAX >> 1 unsigned: %u\n", umax >> 1);
        printf("UINT_MAX >> 16 unsigned: %u\n", umax >> 16);
        printf("UINT_MAX >> 31 unsigned: %u\n", umax >> 31);

        int neg128 = -128;
        printf("(-128) >> 1 signed: %d\n", neg128 >> 1);
        printf("(-128) >> 7 signed: %d\n", neg128 >> 7);

        unsigned u128 = 128;
        printf("128u >> 1: %u\n", u128 >> 1);
    }

    // ==========================================
    // 8. Casting Between Signed and Unsigned
    // ==========================================
    printf("=== 8. Casting Between Signed and Unsigned ===\n");
    {
        int neg1 = -1;
        int neg2 = -2;
        int neg128 = -128;
        printf("(unsigned)(-1): %u\n", (unsigned)neg1);
        printf("(unsigned)(-2): %u\n", (unsigned)neg2);
        printf("(unsigned)(-128): %u\n", (unsigned)neg128);

        unsigned uval1 = 4294967295u;
        unsigned uval2 = 4294967294u;
        unsigned uval3 = 2147483648u;
        unsigned uval4 = 2147483647u;
        printf("(int)(4294967295u): %d\n", (int)uval1);
        printf("(int)(4294967294u): %d\n", (int)uval2);
        printf("(int)(2147483648u): %d\n", (int)uval3);
        printf("(int)(2147483647u): %d\n", (int)uval4);
    }

    // ==========================================
    // 9. Mixed Arithmetic Result Type
    // ==========================================
    printf("=== 9. Mixed Arithmetic Result Type ===\n");
    {
        int s = -1;
        unsigned u = 1;
        unsigned r1 = s + u;
        printf("-1 + 1u: %u\n", r1);

        int s2 = -2;
        unsigned r2 = s2 + u;
        printf("-2 + 1u: %u\n", r2);

        unsigned umax = 4294967295u;
        unsigned r3 = 1 + umax;
        printf("1 + UINT_MAX: %u\n", r3);

        int s3 = -1;
        unsigned u2 = 2;
        unsigned r4 = s3 * u2;
        printf("-1 * 2u: %u\n", r4);
    }

    // ==========================================
    // 10. Comparison Chains with Mixed Types
    // ==========================================
    printf("=== 10. Comparison Chains Mixed ===\n");
    {
        int a = -1;
        unsigned b = 0;
        unsigned c = 1;
        int d = 2;

        int chain1 = (a < b);
        printf("chain1: %d\n", chain1);

        int chain2 = (b < c);
        printf("chain2: %d\n", chain2);

        int chain3 = (a > d);
        printf("chain3: %d\n", chain3);

        int chain4 = (c <= (unsigned)d);
        printf("chain4: %d\n", chain4);
    }

    // ==========================================
    // 11. Overflow Detection Patterns
    // ==========================================
    printf("=== 11. Overflow Detection Patterns ===\n");
    {
        unsigned max = 4294967295u;

        {
            unsigned a = max;
            unsigned b = 1;
            unsigned sum = a + b;
            int overflowed = (sum < a);
            printf("add_overflowed(max,1): %d\n", overflowed);
        }

        {
            unsigned a = max - 1;
            unsigned b = 1;
            unsigned sum = a + b;
            int overflowed = (sum < a);
            printf("add_overflowed(max-1,1): %d\n", overflowed);
        }

        {
            unsigned a = 100;
            unsigned b = 200;
            unsigned sum = a + b;
            int overflowed = (sum < a);
            printf("add_overflowed(100,200): %d\n", overflowed);
        }

        {
            unsigned a = max;
            unsigned b = max;
            unsigned sum = a + b;
            int overflowed = (sum < a);
            printf("add_overflowed(max,max): %d\n", overflowed);
        }

        {
            unsigned a = 0;
            unsigned b = 1;
            int underflowed = (b > a);
            printf("sub_underflowed(0,1): %d\n", underflowed);
        }

        {
            unsigned a = 1;
            unsigned b = 1;
            int underflowed = (b > a);
            printf("sub_underflowed(1,1): %d\n", underflowed);
        }

        {
            unsigned a = 5;
            unsigned b = 3;
            int underflowed = (b > a);
            printf("sub_underflowed(5,3): %d\n", underflowed);
        }

        {
            unsigned a = 0;
            unsigned b = max;
            int underflowed = (b > a);
            printf("sub_underflowed(0,max): %d\n", underflowed);
        }
    }

    // ==========================================
    // 12. Unsigned Loop Patterns
    // ==========================================
    printf("=== 12. Unsigned Loop Patterns ===\n");
    {
        printf("countdown:");
        unsigned i = 3;
        while (1) {
            printf(" %u", i);
            if (i == 0) break;
            i = i - 1;
        }
        printf(" done\n");

        printf("upcount:");
        unsigned start = 0;
        unsigned count = 0;
        unsigned val = start;
        while (count < 4) {
            printf(" %u", val);
            if (count == 2) {
                val = 4294967295u;  // jump to max
            } else {
                val = val + 1;
            }
            count = count + 1;
        }
        printf(" done\n");
    }

    // ==========================================
    // 13. Bit Pattern Reinterpretation
    // ==========================================
    printf("=== 13. Bit Pattern Reinterpretation ===\n");
    {
        int neg1 = -1;
        unsigned u_neg1 = (unsigned)neg1;
        printf("neg1_as_unsigned: %u\n", u_neg1);  // 4294967295

        unsigned umax = 4294967295u;
        int s_umax = (int)umax;
        printf("max_unsigned_as_signed: %d\n", s_umax);  // -1

        unsigned hex80 = 2147483648u;
        int s_hex80 = (int)hex80;
        printf("hex_80000000_as_signed: %d\n", s_hex80);

        unsigned hex7f = 2147483647u;
        int s_hex7f = (int)hex7f;
        printf("hex_7FFFFFFF_as_signed: %d\n", s_hex7f);
    }

    // ==========================================
    // 14. Division Edge Cases
    // ==========================================
    printf("=== 14. Division Edge Cases ===\n");
    {
        unsigned zero = 0;
        unsigned one = 1;
        unsigned max = 4294967295u;

        printf("unsigned 0/1: %u\n", zero / one);
        printf("unsigned 0%%1: %u\n", zero % one);
        printf("unsigned max/1: %u\n", max / one);
        printf("unsigned max/max: %u\n", max / max);
        printf("unsigned max%%max: %u\n", max % max);
    }

    // ==========================================
    // 15. Shift Edge Cases
    // ==========================================
    printf("=== 15. Shift Edge Cases ===\n");
    {
        unsigned u1 = 1;
        printf("1u << 31: %u\n", u1 << 31);

        int s1 = 1;
        int r_shift = s1 << 31;
        printf("1 << 31 as signed: %d\n", r_shift);

        printf("1u << 0: %u\n", u1 << 0);

        unsigned u0 = 0;
        printf("0u << 5: %u\n", u0 << 5);
    }

    printf("All tests passed.\n");
    return 0;
}
