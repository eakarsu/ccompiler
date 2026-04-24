int printf(const char *fmt, ...);
// EXPECT: System 1:\n2x + y - z = 8\n-3x - y + 2z = -11\n-2x + y + 2z = -3\n\nSolution: x=2000, y=3000, z=-1000\n\nVerification:\n2(2000) + (3000) - (-1000) = 8000 (expected 8)\n-3(2000) - (3000) + 2(-1000) = -11000 (expected -11)\n-2(2000) + (3000) + 2(-1000) = -3000 (expected -3)\n\nSystem 2 (Cramer's rule):\n3x + 2y = 12\nx + 4y = 10\ndet = 10\nx = 28/10 = 2\ny = 18/10 = 1\nCheck: 3*2 + 2*1 = 8\nCheck: 2 + 4*1 = 6\n\nTrivial system: x=5, y=3, z=7\nSolution: x=5, y=3, z=7
// Test: Gaussian elimination on small systems (integer-scaled)

int main(void) {
    int i, j, k;

    // Solve 3x3 system using scaled integer Gaussian elimination
    // 2x + y - z = 8
    // -3x - y + 2z = -11
    // -2x + y + 2z = -3
    // Solution: x=2, y=3, z=-1

    // Store as augmented matrix * 1000 for precision
    int a[3][4];
    a[0][0] = 2000; a[0][1] = 1000; a[0][2] = -1000; a[0][3] = 8000;
    a[1][0] = -3000; a[1][1] = -1000; a[1][2] = 2000; a[1][3] = -11000;
    a[2][0] = -2000; a[2][1] = 1000; a[2][2] = 2000; a[2][3] = -3000;

    printf("System 1:\n");
    printf("2x + y - z = 8\n");
    printf("-3x - y + 2z = -11\n");
    printf("-2x + y + 2z = -3\n\n");

    // Forward elimination
    for (i = 0; i < 3; i++) {
        // Find pivot
        int pivot = a[i][i];
        if (pivot == 0) {
            printf("Zero pivot at %d\n", i);
            continue;
        }
        // Eliminate below
        for (j = i + 1; j < 3; j++) {
            int factor = a[j][i];
            for (k = i; k < 4; k++) {
                // a[j][k] = a[j][k] - factor/pivot * a[i][k]
                a[j][k] = a[j][k] * pivot - factor * a[i][k];
                // Rescale to avoid overflow
                a[j][k] = a[j][k] / 1000;
            }
        }
    }

    // Back substitution
    int sol[3];
    for (i = 2; i >= 0; i--) {
        int sum = a[i][3];
        for (j = i + 1; j < 3; j++) {
            sum = sum - a[i][j] * sol[j] / 1000;
        }
        sol[i] = sum * 1000 / a[i][i];
    }

    printf("Solution: x=%d, y=%d, z=%d\n\n", sol[0], sol[1], sol[2]);

    // Verify
    printf("Verification:\n");
    printf("2(%d) + (%d) - (%d) = %d (expected 8)\n",
           sol[0], sol[1], sol[2], 2 * sol[0] + sol[1] - sol[2]);
    printf("-3(%d) - (%d) + 2(%d) = %d (expected -11)\n",
           sol[0], sol[1], sol[2], -3 * sol[0] - sol[1] + 2 * sol[2]);
    printf("-2(%d) + (%d) + 2(%d) = %d (expected -3)\n\n",
           sol[0], sol[1], sol[2], -2 * sol[0] + sol[1] + 2 * sol[2]);

    // Solve 2x2 system by Cramer's rule
    // 3x + 2y = 12
    // x + 4y = 10
    printf("System 2 (Cramer's rule):\n");
    printf("3x + 2y = 12\n");
    printf("x + 4y = 10\n");

    int det_main = 3 * 4 - 2 * 1;
    int det_x = 12 * 4 - 2 * 10;
    int det_y = 3 * 10 - 12 * 1;

    printf("det = %d\n", det_main);
    printf("x = %d/%d = %d\n", det_x, det_main, det_x / det_main);
    printf("y = %d/%d = %d\n", det_y, det_main, det_y / det_main);

    // Verify
    int cx = det_x / det_main;
    int cy = det_y / det_main;
    printf("Check: 3*%d + 2*%d = %d\n", cx, cy, 3 * cx + 2 * cy);
    printf("Check: %d + 4*%d = %d\n\n", cx, cy, cx + 4 * cy);

    // Another system: identity-like
    // x = 5
    // y = 3
    // z = 7
    printf("Trivial system: x=5, y=3, z=7\n");
    printf("Solution: x=%d, y=%d, z=%d\n", 5, 3, 7);

    return 0;
}
