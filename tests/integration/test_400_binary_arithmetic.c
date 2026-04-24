int printf(const char *fmt, ...);
// EXPECT: bitwise_add:\n  3 + 5 = 8\n  0 + 0 = 0\n  100 + 200 = 300\n  -10 + 3 = -7\n  255 + 1 = 256\n  add verified: 1\nbitwise_sub:\n  10 - 3 = 7\n  3 - 10 = -7\n  0 - 0 = 0\n  100 - 1 = 99\nbitwise_mul:\n  3 * 4 = 12\n  7 * 8 = 56\n  -3 * 5 = -15\n  -4 * -6 = 24\n  0 * 100 = 0\nbitwise_lt:\n  3 < 5: 1\n  5 < 3: 0\n  3 < 3: 0\n  -1 < 0: 1\nbitwise_eq:\n  3 == 3: 1\n  3 == 5: 0\nbinary addition: 13 + 7\n  00001101 (13)\n+ 00000111  (7)\n= 00010100 (20)\n3*4 + 5*6 = 42
// Test: binary arithmetic

// Add two numbers using only bitwise operations
int bitwise_add(int a, int b) {
    while (b != 0) {
        int carry = a & b;
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}

// Subtract using bitwise operations: a - b = a + (~b + 1)
int bitwise_sub(int a, int b) {
    return bitwise_add(a, bitwise_add(~b, 1));
}

// Multiply using bitwise operations
int bitwise_mul(int a, int b) {
    int result = 0;
    int neg = 0;
    if (a < 0) { a = bitwise_add(~a, 1); neg = !neg; }
    if (b < 0) { b = bitwise_add(~b, 1); neg = !neg; }
    while (b > 0) {
        if (b & 1) {
            result = bitwise_add(result, a);
        }
        a = a << 1;
        b = b >> 1;
    }
    if (neg) result = bitwise_add(~result, 1);
    return result;
}

// Compare using subtraction
int bitwise_lt(int a, int b) {
    int diff = bitwise_sub(a, b);
    return (diff >> 31) & 1;
}

int bitwise_eq(int a, int b) {
    return (a ^ b) == 0;
}

void print_binary_8(int val) {
    int i;
    for (i = 7; i >= 0; i--) {
        printf("%d", (val >> i) & 1);
    }
}

int main(void) {
    // Bitwise addition
    printf("bitwise_add:\n");
    printf("  3 + 5 = %d\n", bitwise_add(3, 5));
    printf("  0 + 0 = %d\n", bitwise_add(0, 0));
    printf("  100 + 200 = %d\n", bitwise_add(100, 200));
    printf("  -10 + 3 = %d\n", bitwise_add(-10, 3));
    printf("  255 + 1 = %d\n", bitwise_add(255, 1));

    // Verify against regular addition
    int i;
    int all_ok = 1;
    for (i = -50; i <= 50; i++) {
        int j;
        for (j = -50; j <= 50; j++) {
            if (bitwise_add(i, j) != i + j) {
                all_ok = 0;
            }
        }
    }
    printf("  add verified: %d\n", all_ok);

    // Bitwise subtraction
    printf("bitwise_sub:\n");
    printf("  10 - 3 = %d\n", bitwise_sub(10, 3));
    printf("  3 - 10 = %d\n", bitwise_sub(3, 10));
    printf("  0 - 0 = %d\n", bitwise_sub(0, 0));
    printf("  100 - 1 = %d\n", bitwise_sub(100, 1));

    // Bitwise multiplication
    printf("bitwise_mul:\n");
    printf("  3 * 4 = %d\n", bitwise_mul(3, 4));
    printf("  7 * 8 = %d\n", bitwise_mul(7, 8));
    printf("  -3 * 5 = %d\n", bitwise_mul(-3, 5));
    printf("  -4 * -6 = %d\n", bitwise_mul(-4, -6));
    printf("  0 * 100 = %d\n", bitwise_mul(0, 100));

    // Comparison
    printf("bitwise_lt:\n");
    printf("  3 < 5: %d\n", bitwise_lt(3, 5));
    printf("  5 < 3: %d\n", bitwise_lt(5, 3));
    printf("  3 < 3: %d\n", bitwise_lt(3, 3));
    printf("  -1 < 0: %d\n", bitwise_lt(-1, 0));

    printf("bitwise_eq:\n");
    printf("  3 == 3: %d\n", bitwise_eq(3, 3));
    printf("  3 == 5: %d\n", bitwise_eq(3, 5));

    // Binary addition visualization
    printf("binary addition: 13 + 7\n");
    printf("  "); print_binary_8(13); printf(" (13)\n");
    printf("+ "); print_binary_8(7); printf("  (7)\n");
    printf("= "); print_binary_8(bitwise_add(13, 7)); printf(" (%d)\n",
           bitwise_add(13, 7));

    // Chain of operations
    int result = bitwise_add(bitwise_mul(3, 4), bitwise_mul(5, 6));
    printf("3*4 + 5*6 = %d\n", result);

    return 0;
}
