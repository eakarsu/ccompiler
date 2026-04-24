// EXPECT: === Compound Assignment Operators ===
// EXPECT: plus_eq: 15
// EXPECT: minus_eq: 7
// EXPECT: mul_eq: 30
// EXPECT: div_eq: 5
// EXPECT: mod_eq: 1
// EXPECT: and_eq: 0x0A
// EXPECT: or_eq: 0xFF
// EXPECT: xor_eq: 0x05
// EXPECT: shl_eq: 40
// EXPECT: shr_eq: 2
// EXPECT: chain_plus: 60
// EXPECT: chain_mixed: 25
// EXPECT: array_compound: 2 4 6 8 10
// EXPECT: ptr_compound: 30
// EXPECT: struct_compound: x=15 y=8
// EXPECT: char_compound: 70
// EXPECT: unsigned_compound: 4294967290
// EXPECT: accumulate: sum=55
// EXPECT: bitfield_toggle: 0xA5 -> 0x5A -> 0xFF -> 0x0F
// EXPECT: compound_in_condition: 3
// EXPECT: nested_array_compound: 110 220 330
// EXPECT: running_product: 120
// EXPECT: compound_with_ternary: 15
// EXPECT: multi_var_compound: a=4 b=6 c=8

int printf(const char *fmt, ...);

struct Point {
    int x;
    int y;
};

int main(void) {
    printf("=== Compound Assignment Operators ===\n");

    // Basic +=
    {
        int a = 10;
        a += 5;
        printf("plus_eq: %d\n", a);
    }

    // Basic -=
    {
        int a = 10;
        a -= 3;
        printf("minus_eq: %d\n", a);
    }

    // Basic *=
    {
        int a = 6;
        a *= 5;
        printf("mul_eq: %d\n", a);
    }

    // Basic /=
    {
        int a = 20;
        a /= 4;
        printf("div_eq: %d\n", a);
    }

    // Basic %=
    {
        int a = 10;
        a %= 3;
        printf("mod_eq: %d\n", a);
    }

    // &=
    {
        int a = 0x0F;
        a &= 0x0A;
        printf("and_eq: 0x%02X\n", a);
    }

    // |=
    {
        int a = 0xF0;
        a |= 0x0F;
        printf("or_eq: 0x%02X\n", a);
    }

    // ^=
    {
        int a = 0x0F;
        a ^= 0x0A;
        printf("xor_eq: 0x%02X\n", a);
    }

    // <<=
    {
        int a = 5;
        a <<= 3;
        printf("shl_eq: %d\n", a);
    }

    // >>=
    {
        int a = 10;
        a >>= 2;
        printf("shr_eq: %d\n", a);
    }

    // Chained +=
    {
        int a = 0;
        a += 10;
        a += 20;
        a += 30;
        printf("chain_plus: %d\n", a);
    }

    // Chained mixed
    {
        int a = 10;
        a *= 3;    // 30
        a -= 5;    // 25
        printf("chain_mixed: %d\n", a);
    }

    // Compound assignment on array elements
    {
        int arr[5];
        int i;
        for (i = 0; i < 5; i++) {
            arr[i] = i + 1;
        }
        for (i = 0; i < 5; i++) {
            arr[i] *= 2;
        }
        printf("array_compound: %d %d %d %d %d\n",
               arr[0], arr[1], arr[2], arr[3], arr[4]);
    }

    // Compound assignment through pointer
    {
        int x = 10;
        int *p = &x;
        *p += 20;
        printf("ptr_compound: %d\n", x);
    }

    // Compound assignment on struct members
    {
        struct Point pt;
        pt.x = 10;
        pt.y = 3;
        pt.x += 5;
        pt.y *= 2;
        pt.y += 2;
        printf("struct_compound: x=%d y=%d\n", pt.x, pt.y);
    }

    // Compound assignment on char (with promotion)
    {
        char c = 'A';  // 65
        c += 5;        // 70
        printf("char_compound: %d\n", (int)c);
    }

    // Compound assignment with unsigned
    {
        unsigned int u = 0;
        u -= 6;  // wraps to UINT_MAX - 5
        printf("unsigned_compound: %u\n", u);
    }

    // Accumulate in loop
    {
        int sum = 0;
        int i;
        for (i = 1; i <= 10; i++) {
            sum += i;
        }
        printf("accumulate: sum=%d\n", sum);
    }

    // Bitwise compound for toggling
    {
        int flags = 0xA5;
        printf("bitfield_toggle: 0x%02X", flags);
        flags ^= 0xFF;  // invert all bits (in lower byte)
        printf(" -> 0x%02X", flags & 0xFF);
        flags |= 0xA5;  // set some bits
        printf(" -> 0x%02X", flags & 0xFF);
        flags &= 0x0F;  // clear upper nibble
        printf(" -> 0x%02X\n", flags);
    }

    // Compound in condition check
    {
        int x = 0;
        x += 3;
        if (x == 3) {
            printf("compound_in_condition: %d\n", x);
        }
    }

    // Nested array compound
    {
        int matrix[3];
        matrix[0] = 100;
        matrix[1] = 200;
        matrix[2] = 300;
        int i;
        for (i = 0; i < 3; i++) {
            matrix[i] += (i + 1) * 10;
        }
        printf("nested_array_compound: %d %d %d\n",
               matrix[0], matrix[1], matrix[2]);
    }

    // Running product
    {
        int prod = 1;
        int i;
        for (i = 1; i <= 5; i++) {
            prod *= i;
        }
        printf("running_product: %d\n", prod);
    }

    // Compound with ternary
    {
        int x = 10;
        int flag = 1;
        x += flag ? 5 : 0;
        printf("compound_with_ternary: %d\n", x);
    }

    // Multiple variables with compound
    {
        int a = 1;
        int b = 2;
        int c = 3;
        a += 3;
        b += 4;
        c += 5;
        printf("multi_var_compound: a=%d b=%d c=%d\n", a, b, c);
    }

    return 0;
}
