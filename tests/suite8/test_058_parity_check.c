int printf(const char *fmt, ...);

int parity_naive(int x) {
    int p;
    p = 0;
    while (x) {
        p = p ^ (x & 1);
        x = x >> 1;
    }
    return p;
}

int parity_kernighan(int x) {
    int p;
    p = 0;
    while (x) {
        p = p ^ 1;
        x = x & (x - 1);
    }
    return p;
}

int parity_fold(int x) {
    x = x ^ (x >> 16);
    x = x ^ (x >> 8);
    x = x ^ (x >> 4);
    x = x ^ (x >> 2);
    x = x ^ (x >> 1);
    return x & 1;
}

int parity_lookup(int x) {
    int table[16];
    int p;
    table[0] = 0; table[1] = 1; table[2] = 1; table[3] = 0;
    table[4] = 1; table[5] = 0; table[6] = 0; table[7] = 1;
    table[8] = 1; table[9] = 0; table[10] = 0; table[11] = 1;
    table[12] = 0; table[13] = 1; table[14] = 1; table[15] = 0;
    p = 0;
    while (x) {
        p = p ^ table[x & 15];
        x = x >> 4;
    }
    return p;
}

int add_even_parity(int data, int bits) {
    int parity;
    parity = parity_naive(data);
    return (data << 1) | parity;
}

int add_odd_parity(int data, int bits) {
    int parity;
    parity = parity_naive(data) ^ 1;
    return (data << 1) | parity;
}

int check_even_parity(int encoded) {
    return parity_naive(encoded) == 0;
}

int check_odd_parity(int encoded) {
    return parity_naive(encoded) == 1;
}

int column_parity(int *data, int n, int bits) {
    int result;
    int i;
    result = 0;
    for (i = 0; i < n; i = i + 1) {
        result = result ^ data[i];
    }
    return result;
}

int detect_single_error(int *data, int n, int parity_row) {
    int col_parity;
    int i;
    int row;
    int bit;

    col_parity = column_parity(data, n, 8);

    for (i = 0; i < n; i = i + 1) {
        if (parity_naive(data[i]) != ((parity_row >> i) & 1)) {
            row = i;
            bit = -1;
            if (col_parity != 0) {
                bit = 0;
                while (bit < 8) {
                    if ((col_parity >> bit) & 1) {
                        return row * 8 + bit;
                    }
                    bit = bit + 1;
                }
            }
            return row * 8;
        }
    }
    return -1;
}

int popcount(int x) {
    int c;
    c = 0;
    while (x) { c = c + 1; x = x & (x - 1); }
    return c;
}

int main() {
    int i;
    int p1;
    int p2;
    int p3;
    int p4;
    int enc;
    int data[4];
    int col_par;

    printf("=== Parity Naive ===\n");
    // EXPECT: === Parity Naive ===

    for (i = 0; i < 16; i = i + 1) {
        if (i > 0) printf(" ");
        printf("par(%d)=%d", i, parity_naive(i));
    }
    printf("\n");
    // EXPECT: par(0)=0 par(1)=1 par(2)=1 par(3)=0 par(4)=1 par(5)=0 par(6)=0 par(7)=1 par(8)=1 par(9)=0 par(10)=0 par(11)=1 par(12)=0 par(13)=1 par(14)=1 par(15)=0

    printf("=== All Methods Agree ===\n");
    // EXPECT: === All Methods Agree ===

    for (i = 0; i < 256; i = i + 1) {
        p1 = parity_naive(i);
        p2 = parity_kernighan(i);
        p3 = parity_fold(i);
        p4 = parity_lookup(i);
        if (p1 != p2 || p1 != p3 || p1 != p4) {
            printf("MISMATCH at %d\n", i);
        }
    }
    printf("all 256 values agree: 1\n");
    // EXPECT: all 256 values agree: 1

    printf("=== Parity Properties ===\n");
    // EXPECT: === Parity Properties ===

    printf("par(0) = %d\n", parity_naive(0));
    // EXPECT: par(0) = 0
    printf("par(1) = %d\n", parity_naive(1));
    // EXPECT: par(1) = 1
    printf("par(255) = %d\n", parity_naive(255));
    // EXPECT: par(255) = 0
    printf("par(127) = %d\n", parity_naive(127));
    // EXPECT: par(127) = 1

    printf("parity(a^b) == parity(a)^parity(b): ");
    // EXPECT: parity(a^b) == parity(a)^parity(b): 1
    p1 = parity_naive(170 ^ 85);
    p2 = parity_naive(170) ^ parity_naive(85);
    printf("%d\n", p1 == p2);

    printf("=== Even Parity Encoding ===\n");
    // EXPECT: === Even Parity Encoding ===

    for (i = 0; i < 8; i = i + 1) {
        enc = add_even_parity(i, 3);
        printf("even_par(%d) = %d, check = %d\n", i, enc, check_even_parity(enc));
    }
    // EXPECT: even_par(0) = 0, check = 1
    // EXPECT: even_par(1) = 3, check = 1
    // EXPECT: even_par(2) = 5, check = 1
    // EXPECT: even_par(3) = 6, check = 1
    // EXPECT: even_par(4) = 9, check = 1
    // EXPECT: even_par(5) = 10, check = 1
    // EXPECT: even_par(6) = 12, check = 1
    // EXPECT: even_par(7) = 15, check = 1

    printf("=== Odd Parity Encoding ===\n");
    // EXPECT: === Odd Parity Encoding ===

    for (i = 0; i < 8; i = i + 1) {
        enc = add_odd_parity(i, 3);
        printf("odd_par(%d) = %d, check = %d\n", i, enc, check_odd_parity(enc));
    }
    // EXPECT: odd_par(0) = 1, check = 1
    // EXPECT: odd_par(1) = 2, check = 1
    // EXPECT: odd_par(2) = 4, check = 1
    // EXPECT: odd_par(3) = 7, check = 1
    // EXPECT: odd_par(4) = 8, check = 1
    // EXPECT: odd_par(5) = 11, check = 1
    // EXPECT: odd_par(6) = 13, check = 1
    // EXPECT: odd_par(7) = 14, check = 1

    printf("=== Column Parity ===\n");
    // EXPECT: === Column Parity ===

    data[0] = 170;
    data[1] = 85;
    data[2] = 51;
    data[3] = 204;
    col_par = column_parity(data, 4, 8);
    printf("col_parity = %d\n", col_par);
    // EXPECT: col_parity = 0

    data[3] = 205;
    col_par = column_parity(data, 4, 8);
    printf("col_parity after error = %d\n", col_par);
    // EXPECT: col_parity after error = 1

    printf("=== Popcount Parity Relation ===\n");
    // EXPECT: === Popcount Parity Relation ===

    for (i = 0; i < 16; i = i + 1) {
        p1 = parity_naive(i);
        p2 = popcount(i) & 1;
        if (p1 != p2) {
            printf("FAIL at %d\n", i);
        }
    }
    printf("parity == popcount mod 2: verified\n");
    // EXPECT: parity == popcount mod 2: verified

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
