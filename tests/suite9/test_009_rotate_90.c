int printf(const char *fmt, ...);

/* Rotate NxN matrix 90 degrees clockwise: dst[j][n-1-i] = src[i][j] */
void rotate_cw(int *src, int *dst, int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            *(dst + j * n + (n - 1 - i)) = *(src + i * n + j);
        }
    }
}

/* Rotate NxN matrix 90 degrees counter-clockwise: dst[n-1-j][i] = src[i][j] */
void rotate_ccw(int *src, int *dst, int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            *(dst + (n - 1 - j) * n + i) = *(src + i * n + j);
        }
    }
}

/* Rotate 180 degrees: dst[n-1-i][n-1-j] = src[i][j] */
void rotate_180(int *src, int *dst, int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            *(dst + (n - 1 - i) * n + (n - 1 - j)) = *(src + i * n + j);
        }
    }
}

/* In-place 90 degree clockwise rotation using transpose + reverse rows */
void rotate_inplace_cw(int *m, int n) {
    int i;
    int j;
    int tmp;

    /* Transpose */
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            tmp = *(m + i * n + j);
            *(m + i * n + j) = *(m + j * n + i);
            *(m + j * n + i) = tmp;
        }
    }
    /* Reverse each row */
    for (i = 0; i < n; i = i + 1) {
        int left;
        int right;
        left = 0;
        right = n - 1;
        while (left < right) {
            tmp = *(m + i * n + left);
            *(m + i * n + left) = *(m + i * n + right);
            *(m + i * n + right) = tmp;
            left = left + 1;
            right = right - 1;
        }
    }
}

void copy_matrix(int *src, int *dst, int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            *(dst + i * n + j) = *(src + i * n + j);
        }
    }
}

int matrices_equal(int *a, int *b, int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            if (*(a + i * n + j) != *(b + i * n + j)) return 0;
        }
    }
    return 1;
}

void print_mat(int *m, int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            if (j > 0) printf(" ");
            printf("%d", *(m + i * n + j));
        }
        printf("\n");
    }
}

int main() {
    int a[3][3];
    int r[3][3];

    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
    a[1][0] = 4; a[1][1] = 5; a[1][2] = 6;
    a[2][0] = 7; a[2][1] = 8; a[2][2] = 9;

    rotate_cw((int *)a, (int *)r, 3);

    // EXPECT: CW 90:
    printf("CW 90:\n");
    // EXPECT: 7 4 1
    // EXPECT: 8 5 2
    // EXPECT: 9 6 3
    print_mat((int *)r, 3);

    rotate_ccw((int *)a, (int *)r, 3);

    // EXPECT: CCW 90:
    printf("CCW 90:\n");
    // EXPECT: 3 6 9
    // EXPECT: 2 5 8
    // EXPECT: 1 4 7
    print_mat((int *)r, 3);

    rotate_180((int *)a, (int *)r, 3);

    // EXPECT: 180:
    printf("180:\n");
    // EXPECT: 9 8 7
    // EXPECT: 6 5 4
    // EXPECT: 3 2 1
    print_mat((int *)r, 3);

    /* In-place rotation */
    int b[3][3];
    copy_matrix((int *)a, (int *)b, 3);
    rotate_inplace_cw((int *)b, 3);

    // EXPECT: In-place CW:
    printf("In-place CW:\n");
    // EXPECT: 7 4 1
    // EXPECT: 8 5 2
    // EXPECT: 9 6 3
    print_mat((int *)b, 3);

    /* Verify 4 rotations = original */
    int c[3][3];
    int t1[3][3];
    int t2[3][3];
    int t3[3][3];
    rotate_cw((int *)a, (int *)t1, 3);
    rotate_cw((int *)t1, (int *)t2, 3);
    rotate_cw((int *)t2, (int *)t3, 3);
    rotate_cw((int *)t3, (int *)c, 3);

    // EXPECT: 4 rotations = original: 1
    printf("4 rotations = original: %d\n", matrices_equal((int *)a, (int *)c, 3));

    /* CW then CCW = identity */
    int f[3][3];
    int g[3][3];
    rotate_cw((int *)a, (int *)f, 3);
    rotate_ccw((int *)f, (int *)g, 3);

    // EXPECT: CW then CCW = original: 1
    printf("CW then CCW = original: %d\n", matrices_equal((int *)a, (int *)g, 3));

    /* 4x4 rotation */
    int m4[4][4];
    int r4[4][4];
    int v;
    int i;
    int j;
    v = 1;
    for (i = 0; i < 4; i = i + 1)
        for (j = 0; j < 4; j = j + 1) {
            m4[i][j] = v;
            v = v + 1;
        }

    rotate_cw((int *)m4, (int *)r4, 4);

    // EXPECT: 4x4 CW:
    printf("4x4 CW:\n");
    // EXPECT: 13 9 5 1
    // EXPECT: 14 10 6 2
    // EXPECT: 15 11 7 3
    // EXPECT: 16 12 8 4
    print_mat((int *)r4, 4);

    // EXPECT: Done
    printf("Done\n");

    return 0;
}
