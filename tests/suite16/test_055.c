int printf(const char *fmt, ...);

// Determinant computation using cofactor expansion and Sarrus' rule
// All matrix values scaled by 1000

// 2x2 determinant: ad - bc (values x1000, result x1000)
int det2(int a, int b, int c, int d) {
    return (int)(((long long)a * d - (long long)b * c) / 1000);
}

// 3x3 determinant using Sarrus' rule
// Matrix stored as flat array row-major, values x1000
int det3(int m[]) {
    long long d;
    d = (long long)m[0] * m[4] / 1000 * m[8] / 1000
      + (long long)m[1] * m[5] / 1000 * m[6] / 1000
      + (long long)m[2] * m[3] / 1000 * m[7] / 1000
      - (long long)m[2] * m[4] / 1000 * m[6] / 1000
      - (long long)m[1] * m[3] / 1000 * m[8] / 1000
      - (long long)m[0] * m[5] / 1000 * m[7] / 1000;
    return (int)d;
}

// 3x3 determinant using cofactor expansion along first row
int det3_cofactor(int m[]) {
    int c0;
    int c1;
    int c2;

    // cofactor along row 0
    c0 = det2(m[4], m[5], m[7], m[8]);
    c1 = det2(m[3], m[5], m[6], m[8]);
    c2 = det2(m[3], m[4], m[6], m[7]);

    return (int)(((long long)m[0] * c0 - (long long)m[1] * c1 + (long long)m[2] * c2) / 1000);
}

// 4x4 determinant using cofactor expansion
// Matrix stored as flat 16-element array, values x1000
int det4(int m[]) {
    int sub[9];
    int cofactors[4];
    long long result;
    int i;
    int j;
    int k;
    int col;
    int sign;

    result = 0;
    sign = 1;
    for (col = 0; col < 4; col++) {
        // Build 3x3 submatrix by removing row 0 and column col
        k = 0;
        for (i = 1; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (j != col) {
                    sub[k] = m[i * 4 + j];
                    k++;
                }
            }
        }
        cofactors[col] = det3(sub);
        result = result + (long long)sign * m[col] * cofactors[col] / 1000;
        sign = -sign;
    }
    return (int)result;
}

// Check if a matrix is singular (det ~= 0)
int is_singular(int det_val) {
    if (det_val < 0) det_val = -det_val;
    return det_val < 10; // tolerance
}

void test_det2(void) {
    int d;

    printf("=== 2x2 Determinant ===\n"); // EXPECT: === 2x2 Determinant ===

    // [[1,2],[3,4]] -> 1*4 - 2*3 = -2
    d = det2(1000, 2000, 3000, 4000);
    printf("det[[1,2],[3,4]] = %d\n", d); // EXPECT: det[[1,2],[3,4]] = -2000

    // Identity: det = 1
    d = det2(1000, 0, 0, 1000);
    printf("det(I2) = %d\n", d); // EXPECT: det(I2) = 1000

    // [[2,3],[1,4]] -> 8-3 = 5
    d = det2(2000, 3000, 1000, 4000);
    printf("det[[2,3],[1,4]] = %d\n", d); // EXPECT: det[[2,3],[1,4]] = 5000

    // Singular: [[1,2],[2,4]] -> 4-4 = 0
    d = det2(1000, 2000, 2000, 4000);
    printf("det[[1,2],[2,4]] = %d\n", d); // EXPECT: det[[1,2],[2,4]] = 0
    printf("singular: %d\n", is_singular(d)); // EXPECT: singular: 1
}

void test_det3(void) {
    int m[9];
    int d;

    printf("=== 3x3 Determinant ===\n"); // EXPECT: === 3x3 Determinant ===

    // Identity
    m[0] = 1000; m[1] = 0;    m[2] = 0;
    m[3] = 0;    m[4] = 1000; m[5] = 0;
    m[6] = 0;    m[7] = 0;    m[8] = 1000;
    d = det3(m);
    printf("det(I3) = %d\n", d); // EXPECT: det(I3) = 1000

    d = det3_cofactor(m);
    printf("det(I3) cofactor = %d\n", d); // EXPECT: det(I3) cofactor = 1000

    // [[1,2,3],[4,5,6],[7,8,9]] -> det = 0 (singular)
    m[0] = 1000; m[1] = 2000; m[2] = 3000;
    m[3] = 4000; m[4] = 5000; m[5] = 6000;
    m[6] = 7000; m[7] = 8000; m[8] = 9000;
    d = det3(m);
    printf("det[[1..9]] = %d\n", d); // EXPECT: det[[1..9]] = 0
    printf("singular: %d\n", is_singular(d)); // EXPECT: singular: 1

    // [[2,1,0],[1,3,1],[0,1,2]] -> det = 8
    m[0] = 2000; m[1] = 1000; m[2] = 0;
    m[3] = 1000; m[4] = 3000; m[5] = 1000;
    m[6] = 0;    m[7] = 1000; m[8] = 2000;
    d = det3(m);
    printf("det sym = %d\n", d); // EXPECT: det sym = 8000

    d = det3_cofactor(m);
    printf("det sym cofactor = %d\n", d); // EXPECT: det sym cofactor = 8000
}

void test_det4(void) {
    int m[16];
    int d;

    printf("=== 4x4 Determinant ===\n"); // EXPECT: === 4x4 Determinant ===

    // 4x4 Identity
    m[0]  = 1000; m[1]  = 0;    m[2]  = 0;    m[3]  = 0;
    m[4]  = 0;    m[5]  = 1000; m[6]  = 0;    m[7]  = 0;
    m[8]  = 0;    m[9]  = 0;    m[10] = 1000; m[11] = 0;
    m[12] = 0;    m[13] = 0;    m[14] = 0;    m[15] = 1000;
    d = det4(m);
    printf("det(I4) = %d\n", d); // EXPECT: det(I4) = 1000

    // [[2,0,0,0],[0,3,0,0],[0,0,4,0],[0,0,0,5]] -> det = 120
    m[0]  = 2000; m[1]  = 0;    m[2]  = 0;    m[3]  = 0;
    m[4]  = 0;    m[5]  = 3000; m[6]  = 0;    m[7]  = 0;
    m[8]  = 0;    m[9]  = 0;    m[10] = 4000; m[11] = 0;
    m[12] = 0;    m[13] = 0;    m[14] = 0;    m[15] = 5000;
    d = det4(m);
    printf("det(diag 2345) = %d\n", d); // EXPECT: det(diag 2345) = 120000

    // [[1,1,1,1],[1,2,3,4],[1,3,6,10],[1,4,10,20]] (Pascal matrix) -> det = 1
    m[0]  = 1000; m[1]  = 1000; m[2]  = 1000; m[3]  = 1000;
    m[4]  = 1000; m[5]  = 2000; m[6]  = 3000; m[7]  = 4000;
    m[8]  = 1000; m[9]  = 3000; m[10] = 6000; m[11] = 10000;
    m[12] = 1000; m[13] = 4000; m[14] = 10000; m[15] = 20000;
    d = det4(m);
    printf("det(Pascal4) = %d\n", d); // EXPECT: det(Pascal4) = 1000
}

int main(void) {
    test_det2();
    test_det3();
    test_det4();
    printf("DONE\n"); // EXPECT: DONE
    return 0;
}
