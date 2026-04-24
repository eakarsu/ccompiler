int printf(const char *fmt, ...);
// EXPECT: Original:\n1 2 3\n4 5 6\n7 8 9\nRotated CW 90:\n7 4 1\n8 5 2\n9 6 3\nRotated CCW 90:\n3 6 9\n2 5 8\n1 4 7\nRotated 180:\n9 8 7\n6 5 4\n3 2 1\nCW then CCW = identity: 1\n4x CW = identity: 1\n4x4 original:\n1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16\n4x4 CW 90:\n13 9 5 1\n14 10 6 2\n15 11 7 3\n16 12 8 4
// Test: 2D array rotation (90 degrees clockwise/counter-clockwise)

void print_mat(int *m, int n) {
    int i; int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (j > 0) printf(" ");
            printf("%d", *(m + i * n + j));
        }
        printf("\n");
    }
}

void copy_mat(int *dst, int *src, int n) {
    int i;
    for (i = 0; i < n * n; i++) dst[i] = src[i];
}

// Rotate 90 degrees clockwise: new[j][n-1-i] = old[i][j]
void rotate_cw(int *src, int *dst, int n) {
    int i; int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            *(dst + j * n + (n - 1 - i)) = *(src + i * n + j);
        }
    }
}

// Rotate 90 degrees counter-clockwise: new[n-1-j][i] = old[i][j]
void rotate_ccw(int *src, int *dst, int n) {
    int i; int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            *(dst + (n - 1 - j) * n + i) = *(src + i * n + j);
        }
    }
}

// Rotate 180: new[n-1-i][n-1-j] = old[i][j]
void rotate_180(int *src, int *dst, int n) {
    int i; int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            *(dst + (n-1-i) * n + (n-1-j)) = *(src + i * n + j);
        }
    }
}

int main(void) {
    // 3x3 matrix
    int orig[9];
    int i;
    for (i = 0; i < 9; i++) orig[i] = i + 1;
    printf("Original:\n");
    print_mat(orig, 3);

    int rotated[9];
    rotate_cw(orig, rotated, 3);
    printf("Rotated CW 90:\n");
    print_mat(rotated, 3);

    int rotated2[9];
    rotate_ccw(orig, rotated2, 3);
    printf("Rotated CCW 90:\n");
    print_mat(rotated2, 3);

    int rotated3[9];
    rotate_180(orig, rotated3, 3);
    printf("Rotated 180:\n");
    print_mat(rotated3, 3);

    // Verify: CW + CCW = identity
    int temp[9];
    rotate_cw(orig, temp, 3);
    int back[9];
    rotate_ccw(temp, back, 3);
    int same = 1;
    for (i = 0; i < 9; i++) {
        if (back[i] != orig[i]) same = 0;
    }
    printf("CW then CCW = identity: %d\n", same);

    // 4 CW rotations = identity
    int r1[9]; int r2[9]; int r3[9]; int r4[9];
    rotate_cw(orig, r1, 3);
    rotate_cw(r1, r2, 3);
    rotate_cw(r2, r3, 3);
    rotate_cw(r3, r4, 3);
    same = 1;
    for (i = 0; i < 9; i++) {
        if (r4[i] != orig[i]) same = 0;
    }
    printf("4x CW = identity: %d\n", same);

    // 4x4
    int big[16];
    for (i = 0; i < 16; i++) big[i] = i + 1;
    printf("4x4 original:\n");
    print_mat(big, 4);
    int big_r[16];
    rotate_cw(big, big_r, 4);
    printf("4x4 CW 90:\n");
    print_mat(big_r, 4);

    return 0;
}
