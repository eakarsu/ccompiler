int printf(const char *fmt, ...);

// Gray code generation and conversion

// Convert binary to Gray code
int to_gray(int n) {
    return n ^ ((n >> 1) & 0x7FFFFFFF);
}

// Convert Gray code to binary
int from_gray(int gray) {
    int n = gray;
    int mask = (n >> 1) & 0x7FFFFFFF;
    while (mask != 0) {
        n = n ^ mask;
        mask = (mask >> 1) & 0x7FFFFFFF;
    }
    return n;
}

// Count bits that differ between consecutive Gray codes
int gray_diff_bits(int a, int b) {
    int diff = a ^ b;
    int count = 0;
    while (diff != 0) {
        count = count + (diff & 1);
        diff = (diff >> 1) & 0x7FFFFFFF;
    }
    return count;
}

// Find which bit position changes between consecutive Gray codes
int gray_change_bit(int n) {
    int g1 = to_gray(n);
    int g2 = to_gray(n + 1);
    int diff = g1 ^ g2;
    int pos = 0;
    while (diff > 1) {
        diff = (diff >> 1) & 0x7FFFFFFF;
        pos = pos + 1;
    }
    return pos;
}

// Generate n-bit Gray code sequence, store in array
void generate_gray_sequence(int *out, int nbits) {
    int count = 1 << nbits;
    int i = 0;
    while (i < count) {
        out[i] = to_gray(i);
        i = i + 1;
    }
}

// Verify Gray code property: consecutive codes differ by 1 bit
int verify_gray_property(int *seq, int len) {
    int i = 0;
    while (i < len - 1) {
        int diff = seq[i] ^ seq[i + 1];
        // Check exactly one bit set
        if (diff == 0) return 0;
        if ((diff & (diff - 1)) != 0) return 0;
        i = i + 1;
    }
    return 1;
}

// Check if sequence is a valid Gray code (all unique + adjacent differ by 1)
int is_valid_gray(int *seq, int len) {
    // Check adjacency property
    if (!verify_gray_property(seq, len)) return 0;
    // Check uniqueness using O(n^2)
    int i = 0;
    while (i < len) {
        int j = i + 1;
        while (j < len) {
            if (seq[i] == seq[j]) return 0;
            j = j + 1;
        }
        i = i + 1;
    }
    return 1;
}

// Count how many times bit position p changes in n-bit Gray code
int count_bit_changes(int nbits, int p) {
    int count = 0;
    int total = 1 << nbits;
    int i = 0;
    while (i < total - 1) {
        int g1 = to_gray(i);
        int g2 = to_gray(i + 1);
        int diff = g1 ^ g2;
        if (diff == (1 << p)) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

// Balanced ternary-like encoding using two Gray codes
// encode value into two fields
void dual_gray_encode(int val, int *g1, int *g2) {
    int lo = val & 0xF;
    int hi = (val >> 4) & 0xF;
    *g1 = to_gray(lo);
    *g2 = to_gray(hi);
}

int dual_gray_decode(int g1, int g2) {
    int lo = from_gray(g1);
    int hi = from_gray(g2);
    return (hi << 4) | lo;
}

int main() {
    // to_gray tests
    printf("g(0)=%d\n", to_gray(0));
    // EXPECT: g(0)=0
    printf("g(1)=%d\n", to_gray(1));
    // EXPECT: g(1)=1
    printf("g(2)=%d\n", to_gray(2));
    // EXPECT: g(2)=3
    printf("g(3)=%d\n", to_gray(3));
    // EXPECT: g(3)=2
    printf("g(4)=%d\n", to_gray(4));
    // EXPECT: g(4)=6
    printf("g(5)=%d\n", to_gray(5));
    // EXPECT: g(5)=7
    printf("g(6)=%d\n", to_gray(6));
    // EXPECT: g(6)=5
    printf("g(7)=%d\n", to_gray(7));
    // EXPECT: g(7)=4

    // from_gray roundtrip
    int i = 0;
    int roundtrip_ok = 1;
    while (i < 16) {
        if (from_gray(to_gray(i)) != i) {
            roundtrip_ok = 0;
        }
        i = i + 1;
    }
    printf("roundtrip=%d\n", roundtrip_ok);
    // EXPECT: roundtrip=1

    // from_gray specific
    printf("fg(0)=%d\n", from_gray(0));
    // EXPECT: fg(0)=0
    printf("fg(1)=%d\n", from_gray(1));
    // EXPECT: fg(1)=1
    printf("fg(3)=%d\n", from_gray(3));
    // EXPECT: fg(3)=2
    printf("fg(2)=%d\n", from_gray(2));
    // EXPECT: fg(2)=3

    // Gray code property: consecutive differ by exactly 1 bit
    printf("diff01=%d\n", gray_diff_bits(to_gray(0), to_gray(1)));
    // EXPECT: diff01=1
    printf("diff12=%d\n", gray_diff_bits(to_gray(1), to_gray(2)));
    // EXPECT: diff12=1
    printf("diff67=%d\n", gray_diff_bits(to_gray(6), to_gray(7)));
    // EXPECT: diff67=1

    // Which bit changes
    printf("cb0=%d\n", gray_change_bit(0));
    // EXPECT: cb0=0
    printf("cb1=%d\n", gray_change_bit(1));
    // EXPECT: cb1=1
    printf("cb2=%d\n", gray_change_bit(2));
    // EXPECT: cb2=0
    printf("cb3=%d\n", gray_change_bit(3));
    // EXPECT: cb3=2

    // Generate and validate 3-bit Gray code sequence
    int seq[8];
    generate_gray_sequence(seq, 3);
    printf("valid3=%d\n", is_valid_gray(seq, 8));
    // EXPECT: valid3=1

    // Generate and validate 4-bit Gray code
    int seq4[16];
    generate_gray_sequence(seq4, 4);
    printf("valid4=%d\n", is_valid_gray(seq4, 16));
    // EXPECT: valid4=1

    // Count bit changes in 3-bit Gray code
    // bit 0 changes: 0->1, 2->3, 4->5, 6->7 = 4 times
    printf("ch0=%d\n", count_bit_changes(3, 0));
    // EXPECT: ch0=4
    // bit 1 changes: 1->2, 5->6 = 2 times
    printf("ch1=%d\n", count_bit_changes(3, 1));
    // EXPECT: ch1=2
    // bit 2 changes: 3->4 = 1 time
    printf("ch2=%d\n", count_bit_changes(3, 2));
    // EXPECT: ch2=1

    // Dual Gray encode/decode
    int g1 = 0;
    int g2 = 0;
    dual_gray_encode(0, &g1, &g2);
    printf("dg0=%d\n", dual_gray_decode(g1, g2));
    // EXPECT: dg0=0
    dual_gray_encode(42, &g1, &g2);
    printf("dg42=%d\n", dual_gray_decode(g1, g2));
    // EXPECT: dg42=42
    dual_gray_encode(255, &g1, &g2);
    printf("dg255=%d\n", dual_gray_decode(g1, g2));
    // EXPECT: dg255=255
    dual_gray_encode(100, &g1, &g2);
    printf("dg100=%d\n", dual_gray_decode(g1, g2));
    // EXPECT: dg100=100

    printf("done\n");
    // EXPECT: done
    return 0;
}
