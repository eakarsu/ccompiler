int printf(const char *fmt, ...);
// EXPECT: A (multiples of 3): count=10\nB (multiples of 5): count=6\nA AND B: count=2\n  bits: 0 15 \nA OR B: count=14\nA XOR B: count=12\nbit 0 in A: 1\nbit 1 in A: 0\nbit 3 in A: 1\nbit 5 in B: 1\nbit 7 in B: 0\nA == C: 1\nA == B: 0\nA after unset 0: bit0=0 count=9
// Test: bitset data structure

typedef struct {
    int data[4];  // 128 bits
} Bitset;

void bs_clear(Bitset *bs) {
    bs->data[0] = 0; bs->data[1] = 0;
    bs->data[2] = 0; bs->data[3] = 0;
}

void bs_set(Bitset *bs, int bit) {
    bs->data[bit / 32] = bs->data[bit / 32] | (1 << (bit % 32));
}

void bs_unset(Bitset *bs, int bit) {
    bs->data[bit / 32] = bs->data[bit / 32] & ~(1 << (bit % 32));
}

int bs_test(Bitset *bs, int bit) {
    return (bs->data[bit / 32] >> (bit % 32)) & 1;
}

int bs_count(Bitset *bs) {
    int count = 0;
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        int v = bs->data[i];
        for (j = 0; j < 32; j++) {
            count = count + (v & 1);
            v = v >> 1;
        }
    }
    return count;
}

void bs_and(Bitset *result, Bitset *a, Bitset *b) {
    int i;
    for (i = 0; i < 4; i++) {
        result->data[i] = a->data[i] & b->data[i];
    }
}

void bs_or(Bitset *result, Bitset *a, Bitset *b) {
    int i;
    for (i = 0; i < 4; i++) {
        result->data[i] = a->data[i] | b->data[i];
    }
}

void bs_xor(Bitset *result, Bitset *a, Bitset *b) {
    int i;
    for (i = 0; i < 4; i++) {
        result->data[i] = a->data[i] ^ b->data[i];
    }
}

int bs_equal(Bitset *a, Bitset *b) {
    int i;
    for (i = 0; i < 4; i++) {
        if (a->data[i] != b->data[i]) return 0;
    }
    return 1;
}

int main(void) {
    Bitset a;
    Bitset b;
    Bitset result;

    // Initialize
    bs_clear(&a);
    bs_clear(&b);

    // Set bits in A: multiples of 3 < 30
    int i;
    for (i = 0; i < 30; i += 3) bs_set(&a, i);
    printf("A (multiples of 3): count=%d\n", bs_count(&a));

    // Set bits in B: multiples of 5 < 30
    for (i = 0; i < 30; i += 5) bs_set(&b, i);
    printf("B (multiples of 5): count=%d\n", bs_count(&b));

    // AND = intersection
    bs_and(&result, &a, &b);
    printf("A AND B: count=%d\n", bs_count(&result));
    printf("  bits: ");
    for (i = 0; i < 30; i++) {
        if (bs_test(&result, i)) printf("%d ", i);
    }
    printf("\n");

    // OR = union
    bs_or(&result, &a, &b);
    printf("A OR B: count=%d\n", bs_count(&result));

    // XOR = symmetric difference
    bs_xor(&result, &a, &b);
    printf("A XOR B: count=%d\n", bs_count(&result));

    // Test individual bits
    printf("bit 0 in A: %d\n", bs_test(&a, 0));
    printf("bit 1 in A: %d\n", bs_test(&a, 1));
    printf("bit 3 in A: %d\n", bs_test(&a, 3));
    printf("bit 5 in B: %d\n", bs_test(&b, 5));
    printf("bit 7 in B: %d\n", bs_test(&b, 7));

    // Equality
    Bitset c;
    bs_clear(&c);
    for (i = 0; i < 30; i += 3) bs_set(&c, i);
    printf("A == C: %d\n", bs_equal(&a, &c));
    printf("A == B: %d\n", bs_equal(&a, &b));

    // Unset
    bs_unset(&a, 0);
    printf("A after unset 0: bit0=%d count=%d\n", bs_test(&a, 0), bs_count(&a));

    return 0;
}
