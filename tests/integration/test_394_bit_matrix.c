int printf(const char *fmt, ...);
// EXPECT: identity matrix:\n10000000\n01000000\n00100000\n00010000\n00001000\n00000100\n00000010\n00000001\npopcount: 8\ntranspose same: 1\nL-shape:\n10000000\n10000000\n10000000\n10000000\n10000000\n10000000\n10000000\n11111111\nL popcount: 15\nL transposed:\n11111111\n00000001\n00000001\n00000001\n00000001\n00000001\n00000001\n00000001\nidentity AND L popcount: 2
// Test: bit matrix

// 8x8 bit matrix stored in 2 ints (32 bits each, 64 total)
void bm_set(int *mat, int row, int col) {
    int bit = row * 8 + col;
    mat[bit / 32] = mat[bit / 32] | (1 << (bit % 32));
}

int bm_get(int *mat, int row, int col) {
    int bit = row * 8 + col;
    return (mat[bit / 32] >> (bit % 32)) & 1;
}

void bm_clear_all(int *mat) {
    mat[0] = 0; mat[1] = 0;
}

void bm_print(int *mat) {
    int r;
    int c;
    for (r = 0; r < 8; r++) {
        for (c = 0; c < 8; c++) {
            printf("%d", bm_get(mat, r, c));
        }
        printf("\n");
    }
}

// Transpose 8x8 bit matrix
void bm_transpose(int *dst, int *src) {
    bm_clear_all(dst);
    int r;
    int c;
    for (r = 0; r < 8; r++) {
        for (c = 0; c < 8; c++) {
            if (bm_get(src, r, c)) {
                bm_set(dst, c, r);
            }
        }
    }
}

int bm_popcount(int *mat) {
    int count = 0;
    int i;
    int v;
    for (i = 0; i < 2; i++) {
        v = mat[i];
        while (v) { v = v & (v - 1); count++; }
    }
    return count;
}

int main(void) {
    int mat[2];
    bm_clear_all(mat);

    // Set diagonal
    int i;
    for (i = 0; i < 8; i++) {
        bm_set(mat, i, i);
    }
    printf("identity matrix:\n");
    bm_print(mat);
    printf("popcount: %d\n", bm_popcount(mat));

    // Transpose of identity is identity
    int trans[2];
    bm_transpose(trans, mat);
    printf("transpose same: %d\n", mat[0] == trans[0] && mat[1] == trans[1]);

    // L-shaped matrix
    int lmat[2];
    bm_clear_all(lmat);
    for (i = 0; i < 8; i++) {
        bm_set(lmat, i, 0);     // left column
        bm_set(lmat, 7, i);     // bottom row
    }
    printf("L-shape:\n");
    bm_print(lmat);
    printf("L popcount: %d\n", bm_popcount(lmat));

    // Transpose L
    int ltrans[2];
    bm_transpose(ltrans, lmat);
    printf("L transposed:\n");
    bm_print(ltrans);

    // AND of two matrices
    int mat_and[2];
    mat_and[0] = mat[0] & lmat[0];
    mat_and[1] = mat[1] & lmat[1];
    printf("identity AND L popcount: %d\n", bm_popcount(mat_and));

    return 0;
}
