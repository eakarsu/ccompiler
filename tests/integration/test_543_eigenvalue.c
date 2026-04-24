int printf(const char *fmt, ...);
// EXPECT: 2x2 eigenvalue by characteristic polynomial:\nA = [[4,1],[2,3]]\ndet(A-LI) = L^2 - 7L + 10 = (L-5)(L-2)\nEigenvalues: 5, 2\n\nEigenvector check:\nlambda=5, v=[1,1]: A*v = [5, 5]\nlambda=2, v=[1,-2]: A*v = [2, -4]\n5*[1,1] = [5, 5]\n2*[1,-2] = [2, -4]\n\n3x3 Matrix:\n| 4 1 0 |\n| 1 3 1 |\n| 0 1 2 |\n\nPower iteration (scaled by 100):\nIter 0: v=[100,100,100], eigenvalue~5.00\nIter 1: v=[100,100,60], eigenvalue~5.00\nIter 2: v=[100,92,44], eigenvalue~4.92\nIter 3: v=[100,85,36], eigenvalue~4.85\nIter 4: v=[100,80,32], eigenvalue~4.80\nIter 5: v=[100,77,30], eigenvalue~4.77\nIter 6: v=[100,75,28], eigenvalue~4.75\nIter 7: v=[100,74,27], eigenvalue~4.74\nIter 8: v=[100,73,27], eigenvalue~4.73\nIter 9: v=[100,73,26], eigenvalue~4.73\n\n2x2 eigenvalue from trace and det:\nA = [[4,1],[2,3]]\ntrace = 7, det = 10\ndiscriminant = trace^2 - 4*det = 9\neigenvalues = (7 +- sqrt(9)) / 2\n= (7 +- 3) / 2 = 5, 2
// Test: Eigenvalue estimation via power iteration (integer-scaled)

void mat_vec(int m[3][3], int v[3], int result[3]) {
    int i, j;
    for (i = 0; i < 3; i++) {
        result[i] = 0;
        for (j = 0; j < 3; j++) {
            result[i] = result[i] + m[i][j] * v[j];
        }
    }
}

int main(void) {
    int i, j;

    // 2x2 characteristic polynomial: det(A - lambda*I) = 0
    // For A = [[4, 1], [2, 3]]
    // det = (4-L)(3-L) - 2 = L^2 - 7L + 10 = (L-5)(L-2)
    // Eigenvalues: 5 and 2

    printf("2x2 eigenvalue by characteristic polynomial:\n");
    printf("A = [[4,1],[2,3]]\n");
    printf("det(A-LI) = L^2 - 7L + 10 = (L-5)(L-2)\n");
    printf("Eigenvalues: 5, 2\n\n");

    // Verify: A*v = lambda*v
    // For lambda=5: A*[1,2]^T should be [5,10]^T... let's check
    // [4,1]*[1,2] = 4+2 = 6, [2,3]*[1,2] = 2+6 = 8. Not right.
    // Eigenvector for lambda=5: (A-5I)v=0 => [-1,1;2,-2]v=0 => v=[1,1]
    printf("Eigenvector check:\n");
    printf("lambda=5, v=[1,1]: A*v = [%d, %d]\n", 4 * 1 + 1 * 1, 2 * 1 + 3 * 1);
    printf("lambda=2, v=[1,-2]: A*v = [%d, %d]\n", 4 * 1 + 1 * (-2), 2 * 1 + 3 * (-2));
    printf("5*[1,1] = [5, 5]\n");
    printf("2*[1,-2] = [2, -4]\n\n");

    // Power iteration on 3x3 matrix (scaled by 100)
    int M[3][3];
    M[0][0] = 4; M[0][1] = 1; M[0][2] = 0;
    M[1][0] = 1; M[1][1] = 3; M[1][2] = 1;
    M[2][0] = 0; M[2][1] = 1; M[2][2] = 2;

    printf("3x3 Matrix:\n");
    for (i = 0; i < 3; i++) {
        printf("| %d %d %d |\n", M[i][0], M[i][1], M[i][2]);
    }

    // Power iteration with integer scaling
    int v[3];
    v[0] = 100; v[1] = 100; v[2] = 100;

    printf("\nPower iteration (scaled by 100):\n");
    for (i = 0; i < 10; i++) {
        int w[3];
        mat_vec(M, v, w);

        // Find max component for normalization
        int max_val = 0;
        for (j = 0; j < 3; j++) {
            int abs_val = w[j];
            if (abs_val < 0) abs_val = -abs_val;
            if (abs_val > max_val) max_val = abs_val;
        }

        // Eigenvalue estimate = max_val / 100
        printf("Iter %d: v=[%d,%d,%d], eigenvalue~%d.%02d\n",
               i, v[0], v[1], v[2], max_val / 100, max_val % 100);

        // Normalize
        if (max_val > 0) {
            for (j = 0; j < 3; j++) {
                v[j] = w[j] * 100 / max_val;
            }
        }
    }

    // Characteristic polynomial of 2x2: trace and determinant give eigenvalues
    // For [[a,b],[c,d]]: eigenvalues = (trace +- sqrt(trace^2 - 4*det)) / 2
    printf("\n2x2 eigenvalue from trace and det:\n");
    int a = 4, b = 1, c = 2, d = 3;
    int tr = a + d;
    int det = a * d - b * c;
    printf("A = [[%d,%d],[%d,%d]]\n", a, b, c, d);
    printf("trace = %d, det = %d\n", tr, det);
    printf("discriminant = trace^2 - 4*det = %d\n", tr * tr - 4 * det);
    printf("eigenvalues = (%d +- sqrt(%d)) / 2\n", tr, tr * tr - 4 * det);
    // sqrt(49-40) = sqrt(9) = 3
    printf("= (%d +- %d) / 2 = %d, %d\n", tr, 3, (tr + 3) / 2, (tr - 3) / 2);

    return 0;
}
