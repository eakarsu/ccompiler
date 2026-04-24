int printf(const char *fmt, ...);
// EXPECT: popcount: 0 1 1 2 1 2 2 3 1 2 2 3 2 3 3 4 \nhighbit: 0 1 1 2 2 2 2 3 3 3 3 3 3 3 3 4 \nlowbit: 0 1 0 2 0 1 0 3 0 1 0 2 0 1 0 4 \nreverse: 1->128 128->1 170->85 85->170 \npow2: 1 2 4 8 16 \nnextpow2: 1->1 2->2 3->4 4->4 5->8 6->8 7->8 8->8 9->16 10->16 \ntrailing0: 0 1 0 2 0 1 0 3 0 1 0 2 0 1 0 4 \npopcount(n!): 1 1 2 2 4 4 6 6 
// Test: bit counting and manipulation puzzles

int popcount(int n) {
    int count;
    count = 0;
    while (n > 0) {
        count += n & 1;
        n = n >> 1;
    }
    return count;
}

int highest_bit(int n) {
    int pos;
    pos = -1;
    while (n > 0) {
        pos++;
        n = n >> 1;
    }
    return pos;
}

int lowest_bit(int n) {
    int pos;
    if (n == 0) return -1;
    pos = 0;
    while ((n & 1) == 0) {
        pos++;
        n = n >> 1;
    }
    return pos;
}

int reverse_bits_8(int n) {
    int result, i;
    result = 0;
    for (i = 0; i < 8; i++) {
        result = (result << 1) | (n & 1);
        n = n >> 1;
    }
    return result;
}

int is_power_of_2(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

int next_power_of_2(int n) {
    int p;
    p = 1;
    while (p < n) {
        p = p << 1;
    }
    return p;
}

int main(void) {
    int i;

    /* Population count */
    printf("popcount: ");
    for (i = 0; i <= 15; i++) {
        printf("%d ", popcount(i));
    }
    printf("\n");

    /* Highest bit position */
    printf("highbit: ");
    for (i = 1; i <= 16; i++) {
        printf("%d ", highest_bit(i));
    }
    printf("\n");

    /* Lowest bit position */
    printf("lowbit: ");
    for (i = 1; i <= 16; i++) {
        printf("%d ", lowest_bit(i));
    }
    printf("\n");

    /* Reverse 8 bits */
    printf("reverse: ");
    printf("%d->%d ", 1, reverse_bits_8(1));
    printf("%d->%d ", 128, reverse_bits_8(128));
    printf("%d->%d ", 0xAA, reverse_bits_8(0xAA));
    printf("%d->%d ", 0x55, reverse_bits_8(0x55));
    printf("\n");

    /* Power of 2 check */
    printf("pow2: ");
    for (i = 1; i <= 20; i++) {
        if (is_power_of_2(i)) {
            printf("%d ", i);
        }
    }
    printf("\n");

    /* Next power of 2 */
    printf("nextpow2: ");
    for (i = 1; i <= 10; i++) {
        printf("%d->%d ", i, next_power_of_2(i));
    }
    printf("\n");

    /* Count trailing zeros */
    printf("trailing0: ");
    for (i = 1; i <= 16; i++) {
        printf("%d ", lowest_bit(i));
    }
    printf("\n");

    /* Popcount of n! is interesting */
    {
        int fact, n;
        fact = 1;
        printf("popcount(n!): ");
        for (n = 1; n <= 8; n++) {
            fact *= n;
            printf("%d ", popcount(fact));
        }
        printf("\n");
    }

    return 0;
}
