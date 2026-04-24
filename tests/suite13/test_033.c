int printf(const char *fmt, ...);

// Determinant Calculation (3x3)
// Computes determinants using cofactor expansion and Sarrus' rule

struct Mat3 {
    int m[3][3];
};

// Determinant via cofactor expansion along first row
int det3_cofactor(struct Mat3 *a) {
    int d = 0;
    // cofactor along row 0
    // a00 * (a11*a22 - a12*a21)
    d = d + a->m[0][0] * (a->m[1][1] * a->m[2][2] - a->m[1][2] * a->m[2][1]);
    // -a01 * (a10*a22 - a12*a20)
    d = d - a->m[0][1] * (a->m[1][0] * a->m[2][2] - a->m[1][2] * a->m[2][0]);
    // +a02 * (a10*a21 - a11*a20)
    d = d + a->m[0][2] * (a->m[1][0] * a->m[2][1] - a->m[1][1] * a->m[2][0]);
    return d;
}

// Determinant via Sarrus' rule
int det3_sarrus(struct Mat3 *a) {
    int pos = 0;
    int neg = 0;
    // positive diagonals
    pos = pos + a->m[0][0] * a->m[1][1] * a->m[2][2];
    pos = pos + a->m[0][1] * a->m[1][2] * a->m[2][0];
    pos = pos + a->m[0][2] * a->m[1][0] * a->m[2][1];
    // negative diagonals
    neg = neg + a->m[0][2] * a->m[1][1] * a->m[2][0];
    neg = neg + a->m[0][1] * a->m[1][0] * a->m[2][2];
    neg = neg + a->m[0][0] * a->m[1][2] * a->m[2][1];
    return pos - neg;
}

// Compute 2x2 minor determinant at (row, col)
int minor2x2(struct Mat3 *a, int row, int col) {
    int r[2];
    int c[2];
    int ri = 0;
    int ci;
    int i;
    for (i = 0; i < 3; i++) {
        if (i != row) {
            r[ri] = i;
            ri = ri + 1;
        }
    }
    ci = 0;
    for (i = 0; i < 3; i++) {
        if (i != col) {
            c[ci] = i;
            ci = ci + 1;
        }
    }
    return a->m[r[0]][c[0]] * a->m[r[1]][c[1]] - a->m[r[0]][c[1]] * a->m[r[1]][c[0]];
}

void mat3_set(struct Mat3 *m, int a, int b, int c, int d, int e, int f, int g, int h, int i) {
    m->m[0][0] = a; m->m[0][1] = b; m->m[0][2] = c;
    m->m[1][0] = d; m->m[1][1] = e; m->m[1][2] = f;
    m->m[2][0] = g; m->m[2][1] = h; m->m[2][2] = i;
}

int main() {
    struct Mat3 a;

    // Test 1: Identity matrix determinant = 1
    mat3_set(&a, 1, 0, 0, 0, 1, 0, 0, 0, 1);
    printf("det(I) cofactor: %d\n", det3_cofactor(&a));
    // EXPECT: det(I) cofactor: 1
    printf("det(I) sarrus: %d\n", det3_sarrus(&a));
    // EXPECT: det(I) sarrus: 1

    // Test 2: Matrix with known determinant
    // [[1,2,3],[4,5,6],[7,8,9]] -> det = 0
    mat3_set(&a, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    int d1 = det3_cofactor(&a);
    int d2 = det3_sarrus(&a);
    printf("det(A) cofactor: %d\n", d1);
    // EXPECT: det(A) cofactor: 0
    printf("det(A) sarrus: %d\n", d2);
    // EXPECT: det(A) sarrus: 0
    printf("methods agree: %d\n", d1 == d2);
    // EXPECT: methods agree: 1

    // Test 3: Non-singular matrix
    // [[2,1,1],[1,3,2],[1,0,0]]
    // det = 2*(3*0-2*0) - 1*(1*0-2*1) + 1*(1*0-3*1) = 0 - 1*(0-2) + 1*(0-3) = 0+2-3 = -1
    mat3_set(&a, 2, 1, 1, 1, 3, 2, 1, 0, 0);
    printf("det(B): %d\n", det3_cofactor(&a));
    // EXPECT: det(B): -1
    printf("det(B) sarrus: %d\n", det3_sarrus(&a));
    // EXPECT: det(B) sarrus: -1

    // Test 4: Diagonal matrix determinant = product of diagonals
    // [[3,0,0],[0,4,0],[0,0,5]] -> det = 60
    mat3_set(&a, 3, 0, 0, 0, 4, 0, 0, 0, 5);
    printf("det(diag): %d\n", det3_cofactor(&a));
    // EXPECT: det(diag): 60

    // Test 5: Minors
    mat3_set(&a, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    // Minor(0,0) = |5,6; 8,9| = 45-48 = -3
    printf("minor(0,0): %d\n", minor2x2(&a, 0, 0));
    // EXPECT: minor(0,0): -3
    // Minor(0,1) = |4,6; 7,9| = 36-42 = -6
    printf("minor(0,1): %d\n", minor2x2(&a, 0, 1));
    // EXPECT: minor(0,1): -6
    // Minor(1,1) = |1,3; 7,9| = 9-21 = -12
    printf("minor(1,1): %d\n", minor2x2(&a, 1, 1));
    // EXPECT: minor(1,1): -12

    // Test 6: Swapping two rows negates determinant
    // Original: [[2,1,1],[1,3,2],[1,0,0]] det = -1
    // Swap row 0 and row 1: [[1,3,2],[2,1,1],[1,0,0]]
    // det = 1*(1*0-1*0) - 3*(2*0-1*1) + 2*(2*0-1*1) = 0 - 3*(-1) + 2*(-1) = 0+3-2 = 1
    mat3_set(&a, 1, 3, 2, 2, 1, 1, 1, 0, 0);
    printf("det(row swap): %d\n", det3_cofactor(&a));
    // EXPECT: det(row swap): 1

    // Test 7: Upper triangular matrix
    // [[2,3,4],[0,5,6],[0,0,7]] -> det = 2*5*7 = 70
    mat3_set(&a, 2, 3, 4, 0, 5, 6, 0, 0, 7);
    printf("det(upper tri): %d\n", det3_cofactor(&a));
    // EXPECT: det(upper tri): 70

    // Test 8: Matrix with negative entries
    // [[-1,2,-3],[4,-5,6],[-7,8,-9]]
    // det = -1*(-5*-9 - 6*8) - 2*(4*-9 - 6*-7) + (-3)*(4*8 - (-5)*(-7))
    //     = -1*(45-48) - 2*(-36+42) + (-3)*(32-35)
    //     = -1*(-3) - 2*(6) + (-3)*(-3)
    //     = 3 - 12 + 9 = 0
    mat3_set(&a, -1, 2, -3, 4, -5, 6, -7, 8, -9);
    printf("det(neg): %d\n", det3_cofactor(&a));
    // EXPECT: det(neg): 0
    printf("det(neg) sarrus: %d\n", det3_sarrus(&a));
    // EXPECT: det(neg) sarrus: 0

    // Test 9: Permutation matrix det = -1
    // [[0,1,0],[0,0,1],[1,0,0]] (cyclic permutation, even? 0->1->2->0, two transpositions = even, det=+1)
    // Actually: det = 0*(0-0) - 1*(0-1) + 0*(0-0) = 0+1+0 = 1
    mat3_set(&a, 0, 1, 0, 0, 0, 1, 1, 0, 0);
    printf("det(perm): %d\n", det3_cofactor(&a));
    // EXPECT: det(perm): 1

    return 0;
}
