int printf(const char *fmt, ...);
// EXPECT: 4-bit Gray code:\n   0: 0000 -> 0000 (gray=0)\n   1: 0001 -> 0001 (gray=1)\n   2: 0010 -> 0011 (gray=3)\n   3: 0011 -> 0010 (gray=2)\n   4: 0100 -> 0110 (gray=6)\n   5: 0101 -> 0111 (gray=7)\n   6: 0110 -> 0101 (gray=5)\n   7: 0111 -> 0100 (gray=4)\n   8: 1000 -> 1100 (gray=12)\n   9: 1001 -> 1101 (gray=13)\n  10: 1010 -> 1111 (gray=15)\n  11: 1011 -> 1110 (gray=14)\n  12: 1100 -> 1010 (gray=10)\n  13: 1101 -> 1011 (gray=11)\n  14: 1110 -> 1001 (gray=9)\n  15: 1111 -> 1000 (gray=8)\nadjacent differ by 1 bit: 1\nround-trip match: 1\nbijection: 1\nfrom_gray tests:\n  from_gray(0) = 0\n  from_gray(1) = 1\n  from_gray(3) = 2\n  from_gray(2) = 3\n  from_gray(7) = 5\n  from_gray(15) = 10\nwrap hamming: 1
// Test: gray code

int to_gray(int n) {
    return n ^ (n >> 1);
}

int from_gray(int gray) {
    int n = gray;
    int mask = n >> 1;
    while (mask != 0) {
        n = n ^ mask;
        mask = mask >> 1;
    }
    return n;
}

void print_binary(int val, int bits) {
    int i;
    for (i = bits - 1; i >= 0; i--) {
        printf("%d", (val >> i) & 1);
    }
}

int hamming_distance(int a, int b) {
    int xor_val = a ^ b;
    int count = 0;
    while (xor_val) {
        xor_val = xor_val & (xor_val - 1);
        count++;
    }
    return count;
}

int main(void) {
    // Generate Gray code for 4 bits
    printf("4-bit Gray code:\n");
    int i;
    for (i = 0; i < 16; i++) {
        int gray = to_gray(i);
        printf("  %2d: ", i);
        print_binary(i, 4);
        printf(" -> ");
        print_binary(gray, 4);
        printf(" (gray=%d)\n", gray);
    }

    // Verify adjacent Gray codes differ by 1 bit
    int all_one_bit = 1;
    for (i = 0; i < 15; i++) {
        int g1 = to_gray(i);
        int g2 = to_gray(i + 1);
        if (hamming_distance(g1, g2) != 1) {
            all_one_bit = 0;
        }
    }
    printf("adjacent differ by 1 bit: %d\n", all_one_bit);

    // Verify round-trip conversion
    int all_match = 1;
    for (i = 0; i < 256; i++) {
        if (from_gray(to_gray(i)) != i) {
            all_match = 0;
        }
    }
    printf("round-trip match: %d\n", all_match);

    // Gray code is bijection (all values unique)
    int seen[16];
    for (i = 0; i < 16; i++) seen[i] = 0;
    for (i = 0; i < 16; i++) {
        int g = to_gray(i);
        seen[g] = 1;
    }
    int all_seen = 1;
    for (i = 0; i < 16; i++) {
        if (!seen[i]) all_seen = 0;
    }
    printf("bijection: %d\n", all_seen);

    // Specific conversions
    printf("from_gray tests:\n");
    printf("  from_gray(0) = %d\n", from_gray(0));
    printf("  from_gray(1) = %d\n", from_gray(1));
    printf("  from_gray(3) = %d\n", from_gray(3));
    printf("  from_gray(2) = %d\n", from_gray(2));
    printf("  from_gray(7) = %d\n", from_gray(7));
    printf("  from_gray(15) = %d\n", from_gray(15));

    // Gray code wraps around with 1-bit difference
    int first_gray = to_gray(0);
    int last_gray = to_gray(15);
    printf("wrap hamming: %d\n", hamming_distance(first_gray, last_gray));

    return 0;
}
