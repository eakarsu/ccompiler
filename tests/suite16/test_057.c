int printf(const char *fmt, ...);

// Gaussian elimination with x1000 scaling
// Solve Ax = b using row reduction

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

// Gaussian elimination with partial pivoting
// Augmented matrix: n x (n+1) stored as flat array
// Values x1000. Solves in place, solution in last column.
// Returns 1 on success, 0 if singular.
int gauss_solve(int aug[], int n) {
    int i;
    int j;
    int k;
    int pivot_row;
    int max_val;
    int temp;
    long long factor;
    int cols;

    cols = n + 1;

    for (i = 0; i < n; i++) {
        // Partial pivoting: find row with largest absolute value in column i
        max_val = abs_val(aug[i * cols + i]);
        pivot_row = i;
        for (k = i + 1; k < n; k++) {
            if (abs_val(aug[k * cols + i]) > max_val) {
                max_val = abs_val(aug[k * cols + i]);
                pivot_row = k;
            }
        }

        // Swap rows if necessary
        if (pivot_row != i) {
            for (j = 0; j < cols; j++) {
                temp = aug[i * cols + j];
                aug[i * cols + j] = aug[pivot_row * cols + j];
                aug[pivot_row * cols + j] = temp;
            }
        }

        // Check for zero pivot
        if (abs_val(aug[i * cols + i]) < 1) {
            return 0; // singular
        }

        // Eliminate below
        for (k = i + 1; k < n; k++) {
            factor = (long long)aug[k * cols + i] * 1000 / aug[i * cols + i];
            for (j = i; j < cols; j++) {
                aug[k * cols + j] = aug[k * cols + j] - (int)(factor * aug[i * cols + j] / 1000);
            }
        }
    }

    // Back substitution
    for (i = n - 1; i >= 0; i--) {
        for (j = i + 1; j < n; j++) {
            aug[i * cols + n] = aug[i * cols + n] - (int)((long long)aug[i * cols + j] * aug[j * cols + n] / 1000);
        }
        aug[i * cols + n] = (int)((long long)aug[i * cols + n] * 1000 / aug[i * cols + i]);
    }

    return 1;
}

void test_2x2_system(void) {
    int aug[6]; // 2x3
    int ok;

    printf("=== Gauss 2x2 ===\n"); // EXPECT: === Gauss 2x2 ===

    // 2x + 3y = 8, x + y = 3 -> x=1, y=2
    aug[0] = 2000; aug[1] = 3000; aug[2] = 8000;
    aug[3] = 1000; aug[4] = 1000; aug[5] = 3000;

    ok = gauss_solve(aug, 2);
    printf("ok=%d x=%d y=%d\n", ok, aug[2], aug[5]); // EXPECT: ok=1 x=1000 y=2000

    // x + y = 5, 2x - y = 1 -> x=2, y=3
    aug[0] = 1000; aug[1] = 1000; aug[2] = 5000;
    aug[3] = 2000; aug[4] = -1000; aug[5] = 1000;

    ok = gauss_solve(aug, 2);
    printf("ok=%d x=%d y=%d\n", ok, aug[2], aug[5]); // EXPECT: ok=1 x=2000 y=3000
}

void test_3x3_system(void) {
    int aug[12]; // 3x4
    int ok;

    printf("=== Gauss 3x3 ===\n"); // EXPECT: === Gauss 3x3 ===

    // x + y + z = 6
    // 2x + y - z = 1
    // x - y + z = 2
    // Solution: x=1, y=2, z=3
    aug[0]  = 1000; aug[1]  = 1000;  aug[2]  = 1000;  aug[3]  = 6000;
    aug[4]  = 2000; aug[5]  = 1000;  aug[6]  = -1000; aug[7]  = 1000;
    aug[8]  = 1000; aug[9]  = -1000; aug[10] = 1000;  aug[11] = 2000;

    ok = gauss_solve(aug, 3);
    printf("ok=%d\n", ok); // EXPECT: ok=1
    printf("x=%d y=%d z=%d\n", aug[3], aug[7], aug[11]); // EXPECT: x=1000 y=2000 z=3001
}

void test_3x3_system2(void) {
    int aug[12]; // 3x4
    int ok;

    printf("=== Gauss 3x3 system 2 ===\n"); // EXPECT: === Gauss 3x3 system 2 ===

    // 3x + 2y - z = 1
    // 2x - 2y + 4z = -2
    // -x + y/2 - z = 0 -> scaled: -1000x + 500y - 1000z = 0
    aug[0]  = 3000;  aug[1]  = 2000;  aug[2]  = -1000; aug[3]  = 1000;
    aug[4]  = 2000;  aug[5]  = -2000; aug[6]  = 4000;  aug[7]  = -2000;
    aug[8]  = -1000; aug[9]  = 500;   aug[10] = -1000; aug[11] = 0;

    ok = gauss_solve(aug, 3);
    printf("ok=%d\n", ok); // EXPECT: ok=1
    printf("x=%d y=%d z=%d\n", aug[3], aug[7], aug[11]); // EXPECT: x=1013 y=-2032 z=-2023
}

void test_singular(void) {
    int aug[6]; // 2x3
    int ok;

    printf("=== Gauss Singular ===\n"); // EXPECT: === Gauss Singular ===

    // x + 2y = 3, 2x + 4y = 6 (dependent)
    aug[0] = 1000; aug[1] = 2000; aug[2] = 3000;
    aug[3] = 2000; aug[4] = 4000; aug[5] = 6000;

    ok = gauss_solve(aug, 2);
    printf("singular: ok=%d\n", ok); // EXPECT: singular: ok=0
}

void test_pivoting(void) {
    int aug[12]; // 3x4
    int ok;

    printf("=== Gauss with Pivoting ===\n"); // EXPECT: === Gauss with Pivoting ===

    // Small pivot in (0,0), needs pivoting
    // 0*x + y + z = 3  -> pivoting will swap
    // 2x + y - z = 1
    // x - y + z = 2
    // Solution should be x=1, y=1, z=2
    aug[0]  = 0;    aug[1]  = 1000;  aug[2]  = 1000;  aug[3]  = 3000;
    aug[4]  = 2000; aug[5]  = 1000;  aug[6]  = -1000; aug[7]  = 1000;
    aug[8]  = 1000; aug[9]  = -1000; aug[10] = 1000;  aug[11] = 2000;

    ok = gauss_solve(aug, 3);
    printf("ok=%d\n", ok); // EXPECT: ok=1
    printf("x=%d y=%d z=%d\n", aug[3], aug[7], aug[11]); // EXPECT: x=1000 y=1000 z=2000
}

void test_diagonal(void) {
    int aug[12];
    int ok;

    printf("=== Gauss Diagonal ===\n"); // EXPECT: === Gauss Diagonal ===

    // 2x = 6, 3y = 9, 4z = 20
    // x=3, y=3, z=5
    aug[0]  = 2000; aug[1]  = 0;    aug[2]  = 0;    aug[3]  = 6000;
    aug[4]  = 0;    aug[5]  = 3000; aug[6]  = 0;    aug[7]  = 9000;
    aug[8]  = 0;    aug[9]  = 0;    aug[10] = 4000; aug[11] = 20000;

    ok = gauss_solve(aug, 3);
    printf("ok=%d\n", ok); // EXPECT: ok=1
    printf("x=%d y=%d z=%d\n", aug[3], aug[7], aug[11]); // EXPECT: x=3000 y=3000 z=5000
}

int main(void) {
    test_2x2_system();
    test_3x3_system();
    test_3x3_system2();
    test_singular();
    test_pivoting();
    test_diagonal();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
