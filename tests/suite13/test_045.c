int printf(const char *fmt, ...);

// Bitmask operations: set, clear, toggle, check bits

struct BitSet {
    int bits[4];  // 128 bits total (4 * 32 bits)
};

void bs_init(struct BitSet *bs) {
    bs->bits[0] = 0;
    bs->bits[1] = 0;
    bs->bits[2] = 0;
    bs->bits[3] = 0;
}

void bs_set(struct BitSet *bs, int pos) {
    int word = pos / 31;
    int bit = pos % 31;
    bs->bits[word] = bs->bits[word] | (1 << bit);
}

void bs_clear(struct BitSet *bs, int pos) {
    int word = pos / 31;
    int bit = pos % 31;
    bs->bits[word] = bs->bits[word] & (~(1 << bit));
}

void bs_toggle(struct BitSet *bs, int pos) {
    int word = pos / 31;
    int bit = pos % 31;
    bs->bits[word] = bs->bits[word] ^ (1 << bit);
}

int bs_test(struct BitSet *bs, int pos) {
    int word = pos / 31;
    int bit = pos % 31;
    return (bs->bits[word] >> bit) & 1;
}

int bs_count(struct BitSet *bs) {
    int total = 0;
    int w = 0;
    while (w < 4) {
        int val = bs->bits[w];
        while (val != 0) {
            total = total + (val & 1);
            val = (val >> 1) & 0x7FFFFFFF;
        }
        w = w + 1;
    }
    return total;
}

// Set a range of bits [lo, hi] inclusive
void bs_set_range(struct BitSet *bs, int lo, int hi) {
    int i = lo;
    while (i <= hi) {
        bs_set(bs, i);
        i = i + 1;
    }
}

// Clear a range of bits
void bs_clear_range(struct BitSet *bs, int lo, int hi) {
    int i = lo;
    while (i <= hi) {
        bs_clear(bs, i);
        i = i + 1;
    }
}

// AND two bitsets, store result in dst
void bs_and(struct BitSet *dst, struct BitSet *a, struct BitSet *b) {
    dst->bits[0] = a->bits[0] & b->bits[0];
    dst->bits[1] = a->bits[1] & b->bits[1];
    dst->bits[2] = a->bits[2] & b->bits[2];
    dst->bits[3] = a->bits[3] & b->bits[3];
}

// OR two bitsets
void bs_or(struct BitSet *dst, struct BitSet *a, struct BitSet *b) {
    dst->bits[0] = a->bits[0] | b->bits[0];
    dst->bits[1] = a->bits[1] | b->bits[1];
    dst->bits[2] = a->bits[2] | b->bits[2];
    dst->bits[3] = a->bits[3] | b->bits[3];
}

// XOR two bitsets
void bs_xor(struct BitSet *dst, struct BitSet *a, struct BitSet *b) {
    dst->bits[0] = a->bits[0] ^ b->bits[0];
    dst->bits[1] = a->bits[1] ^ b->bits[1];
    dst->bits[2] = a->bits[2] ^ b->bits[2];
    dst->bits[3] = a->bits[3] ^ b->bits[3];
}

// Simple permission system using bitmasks
int PERM_READ  = 1;
int PERM_WRITE = 2;
int PERM_EXEC  = 4;
int PERM_DEL   = 8;

int has_perm(int perms, int perm) {
    return (perms & perm) != 0;
}

int add_perm(int perms, int perm) {
    return perms | perm;
}

int remove_perm(int perms, int perm) {
    return perms & (~perm);
}

int main() {
    // Basic bitset operations
    struct BitSet bs;
    bs_init(&bs);

    bs_set(&bs, 0);
    bs_set(&bs, 5);
    bs_set(&bs, 10);
    printf("t0=%d\n", bs_test(&bs, 0));
    // EXPECT: t0=1
    printf("t1=%d\n", bs_test(&bs, 1));
    // EXPECT: t1=0
    printf("t5=%d\n", bs_test(&bs, 5));
    // EXPECT: t5=1
    printf("t10=%d\n", bs_test(&bs, 10));
    // EXPECT: t10=1
    printf("cnt=%d\n", bs_count(&bs));
    // EXPECT: cnt=3

    // Clear
    bs_clear(&bs, 5);
    printf("t5c=%d\n", bs_test(&bs, 5));
    // EXPECT: t5c=0
    printf("cnt2=%d\n", bs_count(&bs));
    // EXPECT: cnt2=2

    // Toggle
    bs_toggle(&bs, 10);  // was 1, now 0
    bs_toggle(&bs, 3);   // was 0, now 1
    printf("t10t=%d\n", bs_test(&bs, 10));
    // EXPECT: t10t=0
    printf("t3t=%d\n", bs_test(&bs, 3));
    // EXPECT: t3t=1
    printf("cnt3=%d\n", bs_count(&bs));
    // EXPECT: cnt3=2

    // Range set
    struct BitSet bs2;
    bs_init(&bs2);
    bs_set_range(&bs2, 3, 7);
    printf("r3=%d\n", bs_test(&bs2, 3));
    // EXPECT: r3=1
    printf("r5=%d\n", bs_test(&bs2, 5));
    // EXPECT: r5=1
    printf("r7=%d\n", bs_test(&bs2, 7));
    // EXPECT: r7=1
    printf("r2=%d\n", bs_test(&bs2, 2));
    // EXPECT: r2=0
    printf("r8=%d\n", bs_test(&bs2, 8));
    // EXPECT: r8=0
    printf("rcnt=%d\n", bs_count(&bs2));
    // EXPECT: rcnt=5

    // Range clear
    bs_clear_range(&bs2, 4, 6);
    printf("r4c=%d\n", bs_test(&bs2, 4));
    // EXPECT: r4c=0
    printf("r5c=%d\n", bs_test(&bs2, 5));
    // EXPECT: r5c=0
    printf("rcnt2=%d\n", bs_count(&bs2));
    // EXPECT: rcnt2=2

    // AND, OR, XOR
    struct BitSet a;
    struct BitSet b;
    struct BitSet result;
    bs_init(&a);
    bs_init(&b);
    bs_set(&a, 1); bs_set(&a, 2); bs_set(&a, 3);
    bs_set(&b, 2); bs_set(&b, 3); bs_set(&b, 4);

    bs_and(&result, &a, &b);
    printf("and_cnt=%d\n", bs_count(&result));
    // EXPECT: and_cnt=2
    printf("and2=%d\n", bs_test(&result, 2));
    // EXPECT: and2=1
    printf("and1=%d\n", bs_test(&result, 1));
    // EXPECT: and1=0

    bs_or(&result, &a, &b);
    printf("or_cnt=%d\n", bs_count(&result));
    // EXPECT: or_cnt=4

    bs_xor(&result, &a, &b);
    printf("xor_cnt=%d\n", bs_count(&result));
    // EXPECT: xor_cnt=2
    printf("xor1=%d\n", bs_test(&result, 1));
    // EXPECT: xor1=1
    printf("xor4=%d\n", bs_test(&result, 4));
    // EXPECT: xor4=1
    printf("xor2=%d\n", bs_test(&result, 2));
    // EXPECT: xor2=0

    // Permission system
    int perms = 0;
    perms = add_perm(perms, PERM_READ);
    perms = add_perm(perms, PERM_WRITE);
    printf("hr=%d\n", has_perm(perms, PERM_READ));
    // EXPECT: hr=1
    printf("hw=%d\n", has_perm(perms, PERM_WRITE));
    // EXPECT: hw=1
    printf("hx=%d\n", has_perm(perms, PERM_EXEC));
    // EXPECT: hx=0

    perms = add_perm(perms, PERM_EXEC);
    printf("hx2=%d\n", has_perm(perms, PERM_EXEC));
    // EXPECT: hx2=1

    perms = remove_perm(perms, PERM_WRITE);
    printf("hw2=%d\n", has_perm(perms, PERM_WRITE));
    // EXPECT: hw2=0
    printf("pval=%d\n", perms);
    // EXPECT: pval=5

    printf("done\n");
    // EXPECT: done
    return 0;
}
