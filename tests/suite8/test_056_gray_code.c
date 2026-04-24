int printf(const char *fmt, ...);

int to_gray(int n) {
    return n ^ (n >> 1);
}

int from_gray(int gray) {
    int n;
    n = gray;
    gray = gray >> 1;
    while (gray > 0) {
        n = n ^ gray;
        gray = gray >> 1;
    }
    return n;
}

int gray_distance(int a, int b) {
    int ga;
    int gb;
    int diff;
    int count;
    ga = to_gray(a);
    gb = to_gray(b);
    diff = ga ^ gb;
    count = 0;
    while (diff) {
        count = count + (diff & 1);
        diff = diff >> 1;
    }
    return count;
}

void print_gray_sequence(int bits) {
    int n;
    int i;
    int g;
    n = 1 << bits;
    for (i = 0; i < n; i = i + 1) {
        g = to_gray(i);
        if (i > 0) printf(" ");
        printf("%d", g);
    }
    printf("\n");
}

int verify_single_bit_change(int bits) {
    int n;
    int i;
    int g1;
    int g2;
    int diff;
    int count;
    n = 1 << bits;
    for (i = 0; i < n - 1; i = i + 1) {
        g1 = to_gray(i);
        g2 = to_gray(i + 1);
        diff = g1 ^ g2;
        count = 0;
        while (diff) {
            count = count + (diff & 1);
            diff = diff >> 1;
        }
        if (count != 1) return 0;
    }
    return 1;
}

int verify_roundtrip(int max_val) {
    int i;
    int gray;
    int back;
    for (i = 0; i < max_val; i = i + 1) {
        gray = to_gray(i);
        back = from_gray(gray);
        if (back != i) return 0;
    }
    return 1;
}

int is_valid_gray_sequence(int *seq, int n) {
    int i;
    int diff;
    int count;
    for (i = 0; i < n - 1; i = i + 1) {
        diff = seq[i] ^ seq[i + 1];
        count = 0;
        while (diff) {
            count = count + (diff & 1);
            diff = diff >> 1;
        }
        if (count != 1) return 0;
    }
    return 1;
}

int main() {
    int i;
    int g;
    int seq[16];

    printf("=== Gray Code Conversion ===\n");
    // EXPECT: === Gray Code Conversion ===

    for (i = 0; i < 16; i = i + 1) {
        printf("binary %d -> gray %d\n", i, to_gray(i));
    }
    // EXPECT: binary 0 -> gray 0
    // EXPECT: binary 1 -> gray 1
    // EXPECT: binary 2 -> gray 3
    // EXPECT: binary 3 -> gray 2
    // EXPECT: binary 4 -> gray 6
    // EXPECT: binary 5 -> gray 7
    // EXPECT: binary 6 -> gray 5
    // EXPECT: binary 7 -> gray 4
    // EXPECT: binary 8 -> gray 12
    // EXPECT: binary 9 -> gray 13
    // EXPECT: binary 10 -> gray 15
    // EXPECT: binary 11 -> gray 14
    // EXPECT: binary 12 -> gray 10
    // EXPECT: binary 13 -> gray 11
    // EXPECT: binary 14 -> gray 9
    // EXPECT: binary 15 -> gray 8

    printf("=== Gray to Binary ===\n");
    // EXPECT: === Gray to Binary ===

    for (i = 0; i < 16; i = i + 1) {
        g = to_gray(i);
        printf("gray %d -> binary %d\n", g, from_gray(g));
    }
    // EXPECT: gray 0 -> binary 0
    // EXPECT: gray 1 -> binary 1
    // EXPECT: gray 3 -> binary 2
    // EXPECT: gray 2 -> binary 3
    // EXPECT: gray 6 -> binary 4
    // EXPECT: gray 7 -> binary 5
    // EXPECT: gray 5 -> binary 6
    // EXPECT: gray 4 -> binary 7
    // EXPECT: gray 12 -> binary 8
    // EXPECT: gray 13 -> binary 9
    // EXPECT: gray 15 -> binary 10
    // EXPECT: gray 14 -> binary 11
    // EXPECT: gray 10 -> binary 12
    // EXPECT: gray 11 -> binary 13
    // EXPECT: gray 9 -> binary 14
    // EXPECT: gray 8 -> binary 15

    printf("=== 3-bit Gray Sequence ===\n");
    // EXPECT: === 3-bit Gray Sequence ===
    print_gray_sequence(3);
    // EXPECT: 0 1 3 2 6 7 5 4

    printf("=== 4-bit Gray Sequence ===\n");
    // EXPECT: === 4-bit Gray Sequence ===
    print_gray_sequence(4);
    // EXPECT: 0 1 3 2 6 7 5 4 12 13 15 14 10 11 9 8

    printf("=== Verify Single-Bit Change ===\n");
    // EXPECT: === Verify Single-Bit Change ===

    printf("3-bit valid: %d\n", verify_single_bit_change(3));
    // EXPECT: 3-bit valid: 1
    printf("4-bit valid: %d\n", verify_single_bit_change(4));
    // EXPECT: 4-bit valid: 1
    printf("8-bit valid: %d\n", verify_single_bit_change(8));
    // EXPECT: 8-bit valid: 1

    printf("=== Roundtrip Verify ===\n");
    // EXPECT: === Roundtrip Verify ===

    printf("roundtrip 256: %d\n", verify_roundtrip(256));
    // EXPECT: roundtrip 256: 1
    printf("roundtrip 1024: %d\n", verify_roundtrip(1024));
    // EXPECT: roundtrip 1024: 1

    printf("=== Gray Distance ===\n");
    // EXPECT: === Gray Distance ===

    printf("gdist(0,1) = %d\n", gray_distance(0, 1));
    // EXPECT: gdist(0,1) = 1
    printf("gdist(0,3) = %d\n", gray_distance(0, 3));
    // EXPECT: gdist(0,3) = 1
    printf("gdist(7,8) = %d\n", gray_distance(7, 8));
    // EXPECT: gdist(7,8) = 1

    printf("=== Validate Sequence Array ===\n");
    // EXPECT: === Validate Sequence Array ===

    for (i = 0; i < 16; i = i + 1) {
        seq[i] = to_gray(i);
    }
    printf("seq valid: %d\n", is_valid_gray_sequence(seq, 16));
    // EXPECT: seq valid: 1

    seq[5] = 100;
    printf("broken seq valid: %d\n", is_valid_gray_sequence(seq, 16));
    // EXPECT: broken seq valid: 0

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
