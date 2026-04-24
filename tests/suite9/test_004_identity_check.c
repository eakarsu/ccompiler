int printf(const char *fmt, ...);

void make_identity(int *m, int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            if (i == j) {
                *(m + i * n + j) = 1;
            } else {
                *(m + i * n + j) = 0;
            }
        }
    }
}

int is_identity(int *m, int n) {
    int i;
    int j;
    int val;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            val = *(m + i * n + j);
            if (i == j) {
                if (val != 1) return 0;
            } else {
                if (val != 0) return 0;
            }
        }
    }
    return 1;
}

int is_zero_matrix(int *m, int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            if (*(m + i * n + j) != 0) return 0;
        }
    }
    return 1;
}

int is_diagonal(int *m, int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            if (i != j && *(m + i * n + j) != 0) return 0;
        }
    }
    return 1;
}

int is_upper_triangular(int *m, int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < i; j = j + 1) {
            if (*(m + i * n + j) != 0) return 0;
        }
    }
    return 1;
}

int is_lower_triangular(int *m, int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            if (*(m + i * n + j) != 0) return 0;
        }
    }
    return 1;
}

void print_matrix(int *m, int n) {
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
    int id3[3][3];
    make_identity((int *)id3, 3);

    // EXPECT: 3x3 identity:
    printf("3x3 identity:\n");
    // EXPECT: 1 0 0
    // EXPECT: 0 1 0
    // EXPECT: 0 0 1
    print_matrix((int *)id3, 3);

    // EXPECT: is_identity: 1
    printf("is_identity: %d\n", is_identity((int *)id3, 3));

    // EXPECT: is_diagonal: 1
    printf("is_diagonal: %d\n", is_diagonal((int *)id3, 3));

    // EXPECT: is_upper: 1
    printf("is_upper: %d\n", is_upper_triangular((int *)id3, 3));

    // EXPECT: is_lower: 1
    printf("is_lower: %d\n", is_lower_triangular((int *)id3, 3));

    /* Modify to non-identity */
    id3[0][1] = 5;
    // EXPECT: after mod is_identity: 0
    printf("after mod is_identity: %d\n", is_identity((int *)id3, 3));

    /* Zero matrix */
    int z[2][2];
    z[0][0] = 0; z[0][1] = 0;
    z[1][0] = 0; z[1][1] = 0;

    // EXPECT: is_zero: 1
    printf("is_zero: %d\n", is_zero_matrix((int *)z, 2));

    // EXPECT: is_identity_zero: 0
    printf("is_identity_zero: %d\n", is_identity((int *)z, 2));

    /* Upper triangular */
    int u[3][3];
    u[0][0] = 1; u[0][1] = 2; u[0][2] = 3;
    u[1][0] = 0; u[1][1] = 4; u[1][2] = 5;
    u[2][0] = 0; u[2][1] = 0; u[2][2] = 6;

    // EXPECT: upper_tri: 1
    printf("upper_tri: %d\n", is_upper_triangular((int *)u, 3));

    // EXPECT: lower_tri: 0
    printf("lower_tri: %d\n", is_lower_triangular((int *)u, 3));

    /* 4x4 identity */
    int id4[4][4];
    make_identity((int *)id4, 4);
    // EXPECT: 4x4 is_identity: 1
    printf("4x4 is_identity: %d\n", is_identity((int *)id4, 4));

    // EXPECT: Done
    printf("Done\n");

    return 0;
}
