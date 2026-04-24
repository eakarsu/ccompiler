int printf(const char *fmt, ...);
// EXPECT: A:\n1 2\n3 4\nB:\n5 6\n7 8\nA*B:\n19 22\n43 50\nD (2x3):\n1 2 3\n4 5 6\nE (3x2):\n7 8\n9 10\n11 12\nD*E (2x2):\n58 64\n139 154\nA*I:\n1 2\n3 4\nG:\n1 2 3\n4 5 6\n7 8 9\nH:\n9 8 7\n6 5 4\n3 2 1\nG*H:\n30 24 18\n84 69 54\n138 114 90\neye is identity: 1\na is identity: 0
// Test: matrix multiplication

void print_mat(int *m, int rows, int cols) {
    int i; int j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (j > 0) printf(" ");
            printf("%d", *(m + i * cols + j));
        }
        printf("\n");
    }
}

// C = A * B where A is (ra x ca), B is (ca x cb), C is (ra x cb)
void mat_mul(int *a, int ra, int ca, int *b, int cb, int *c) {
    int i; int j; int k;
    for (i = 0; i < ra; i++) {
        for (j = 0; j < cb; j++) {
            int sum = 0;
            for (k = 0; k < ca; k++) {
                sum = sum + *(a + i * ca + k) * *(b + k * cb + j);
            }
            *(c + i * cb + j) = sum;
        }
    }
}

// Check if identity
int is_identity(int *m, int n) {
    int i; int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            int expected = (i == j) ? 1 : 0;
            if (*(m + i * n + j) != expected) return 0;
        }
    }
    return 1;
}

int main(void) {
    // 2x2 * 2x2
    int a[4]; a[0]=1; a[1]=2; a[2]=3; a[3]=4;
    int b[4]; b[0]=5; b[1]=6; b[2]=7; b[3]=8;
    int c[4];
    mat_mul(a, 2, 2, b, 2, c);
    printf("A:\n"); print_mat(a, 2, 2);
    printf("B:\n"); print_mat(b, 2, 2);
    printf("A*B:\n"); print_mat(c, 2, 2);

    // 2x3 * 3x2
    int d[6]; d[0]=1; d[1]=2; d[2]=3; d[3]=4; d[4]=5; d[5]=6;
    int e[6]; e[0]=7; e[1]=8; e[2]=9; e[3]=10; e[4]=11; e[5]=12;
    int f[4];
    mat_mul(d, 2, 3, e, 2, f);
    printf("D (2x3):\n"); print_mat(d, 2, 3);
    printf("E (3x2):\n"); print_mat(e, 3, 2);
    printf("D*E (2x2):\n"); print_mat(f, 2, 2);

    // Identity multiplication
    int eye[4]; eye[0]=1; eye[1]=0; eye[2]=0; eye[3]=1;
    int r[4];
    mat_mul(a, 2, 2, eye, 2, r);
    printf("A*I:\n"); print_mat(r, 2, 2);

    // 3x3 multiplication
    int g[9]; int i;
    for (i = 0; i < 9; i++) g[i] = i + 1;
    int h[9];
    for (i = 0; i < 9; i++) h[i] = 9 - i;
    int gh[9];
    mat_mul(g, 3, 3, h, 3, gh);
    printf("G:\n"); print_mat(g, 3, 3);
    printf("H:\n"); print_mat(h, 3, 3);
    printf("G*H:\n"); print_mat(gh, 3, 3);

    // Check identity
    printf("eye is identity: %d\n", is_identity(eye, 2));
    printf("a is identity: %d\n", is_identity(a, 2));

    return 0;
}
