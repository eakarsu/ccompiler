int printf(const char *fmt, ...);

// Matrix Multiplication (3x3)
// Multiplies two 3x3 matrices and verifies results

struct Mat3 {
    int m[3][3];
};

void mat3_zero(struct Mat3 *out) {
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            out->m[i][j] = 0;
        }
    }
}

void mat3_identity(struct Mat3 *out) {
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (i == j) {
                out->m[i][j] = 1;
            } else {
                out->m[i][j] = 0;
            }
        }
    }
}

void mat3_mul(struct Mat3 *a, struct Mat3 *b, struct Mat3 *out) {
    int i;
    int j;
    int k;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            int sum = 0;
            for (k = 0; k < 3; k++) {
                sum = sum + a->m[i][k] * b->m[k][j];
            }
            out->m[i][j] = sum;
        }
    }
}

void mat3_print(struct Mat3 *m) {
    int i;
    for (i = 0; i < 3; i++) {
        printf("%d %d %d\n", m->m[i][0], m->m[i][1], m->m[i][2]);
    }
}

int mat3_equal(struct Mat3 *a, struct Mat3 *b) {
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (a->m[i][j] != b->m[i][j]) return 0;
        }
    }
    return 1;
}

void mat3_scale(struct Mat3 *m, int s, struct Mat3 *out) {
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            out->m[i][j] = m->m[i][j] * s;
        }
    }
}

int main() {
    struct Mat3 a;
    struct Mat3 b;
    struct Mat3 c;
    struct Mat3 ident;

    // Test 1: Identity * A = A
    mat3_identity(&ident);
    a.m[0][0] = 1; a.m[0][1] = 2; a.m[0][2] = 3;
    a.m[1][0] = 4; a.m[1][1] = 5; a.m[1][2] = 6;
    a.m[2][0] = 7; a.m[2][1] = 8; a.m[2][2] = 9;

    mat3_mul(&ident, &a, &c);
    printf("Identity * A == A: %d\n", mat3_equal(&a, &c));
    // EXPECT: Identity * A == A: 1

    // Test 2: A * Identity = A
    mat3_mul(&a, &ident, &c);
    printf("A * Identity == A: %d\n", mat3_equal(&a, &c));
    // EXPECT: A * Identity == A: 1

    // Test 3: Specific multiplication
    // A = [[1,2,3],[4,5,6],[7,8,9]]
    // B = [[9,8,7],[6,5,4],[3,2,1]]
    b.m[0][0] = 9; b.m[0][1] = 8; b.m[0][2] = 7;
    b.m[1][0] = 6; b.m[1][1] = 5; b.m[1][2] = 4;
    b.m[2][0] = 3; b.m[2][1] = 2; b.m[2][2] = 1;

    mat3_mul(&a, &b, &c);
    // Row 0: 1*9+2*6+3*3=9+12+9=30, 1*8+2*5+3*2=8+10+6=24, 1*7+2*4+3*1=7+8+3=18
    // Row 1: 4*9+5*6+6*3=36+30+18=84, 4*8+5*5+6*2=32+25+12=69, 4*7+5*4+6*1=28+20+6=54
    // Row 2: 7*9+8*6+9*3=63+48+27=138, 7*8+8*5+9*2=56+40+18=114, 7*7+8*4+9*1=49+32+9=90
    printf("A*B row 0: %d %d %d\n", c.m[0][0], c.m[0][1], c.m[0][2]);
    // EXPECT: A*B row 0: 30 24 18
    printf("A*B row 1: %d %d %d\n", c.m[1][0], c.m[1][1], c.m[1][2]);
    // EXPECT: A*B row 1: 84 69 54
    printf("A*B row 2: %d %d %d\n", c.m[2][0], c.m[2][1], c.m[2][2]);
    // EXPECT: A*B row 2: 138 114 90

    // Test 4: Multiply by zero matrix
    struct Mat3 z;
    mat3_zero(&z);
    mat3_mul(&a, &z, &c);
    int all_zero = 1;
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (c.m[i][j] != 0) all_zero = 0;
        }
    }
    printf("A * Zero == Zero: %d\n", all_zero);
    // EXPECT: A * Zero == Zero: 1

    // Test 5: (A*B) != (B*A) in general
    struct Mat3 ba;
    mat3_mul(&b, &a, &ba);
    printf("A*B != B*A: %d\n", 1 - mat3_equal(&c, &ba));
    // EXPECT: A*B != B*A: 1

    // Test 6: B*A result
    // B*A: row0: 9*1+8*4+7*7=9+32+49=90, 9*2+8*5+7*8=18+40+56=114, 9*3+8*6+7*9=27+48+63=138
    // row1: 6*1+5*4+4*7=6+20+28=54, 6*2+5*5+4*8=12+25+32=69, 6*3+5*6+4*9=18+30+36=84
    // row2: 3*1+2*4+1*7=3+8+7=18, 3*2+2*5+1*8=6+10+8=24, 3*3+2*6+1*9=9+12+9=30
    printf("B*A row 0: %d %d %d\n", ba.m[0][0], ba.m[0][1], ba.m[0][2]);
    // EXPECT: B*A row 0: 90 114 138
    printf("B*A row 1: %d %d %d\n", ba.m[1][0], ba.m[1][1], ba.m[1][2]);
    // EXPECT: B*A row 1: 54 69 84
    printf("B*A row 2: %d %d %d\n", ba.m[2][0], ba.m[2][1], ba.m[2][2]);
    // EXPECT: B*A row 2: 18 24 30

    // Test 7: Scale and multiply
    struct Mat3 scaled;
    mat3_scale(&ident, 3, &scaled);
    mat3_mul(&scaled, &a, &c);
    // 3I * A = 3A
    printf("3I*A [0][0]: %d\n", c.m[0][0]);
    // EXPECT: 3I*A [0][0]: 3
    printf("3I*A [1][1]: %d\n", c.m[1][1]);
    // EXPECT: 3I*A [1][1]: 15
    printf("3I*A [2][2]: %d\n", c.m[2][2]);
    // EXPECT: 3I*A [2][2]: 27

    return 0;
}
