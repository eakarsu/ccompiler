int printf(const char *fmt, ...);
// EXPECT: A*I:\n1 2 3 \n4 5 6 \n7 8 9 \nA*B:\n5 2 7 \n14 5 16 \n23 8 25 \ntrace(A*B) = 35\ndet(B) = 3\nA+B:\n3 2 4 \n4 6 6 \n8 8 11 \n2*A:\n2 4 6 \n8 10 12 \n14 16 18 
// Test: matrix arithmetic and multiplication

int main(void) {
    int a[3][3];
    int b[3][3];
    int c[3][3];
    int i, j, k;
    int sum;
    int det;

    /* Initialize matrix A = [[1,2,3],[4,5,6],[7,8,9]] */
    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
    a[1][0] = 4; a[1][1] = 5; a[1][2] = 6;
    a[2][0] = 7; a[2][1] = 8; a[2][2] = 9;

    /* Initialize matrix B = identity */
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            b[i][j] = (i == j) ? 1 : 0;
        }
    }

    /* C = A * B (should equal A) */
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            c[i][j] = 0;
            for (k = 0; k < 3; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    printf("A*I:\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            printf("%d ", c[i][j]);
        }
        printf("\n");
    }

    /* B = [[2,0,1],[0,1,0],[1,0,2]] */
    b[0][0] = 2; b[0][1] = 0; b[0][2] = 1;
    b[1][0] = 0; b[1][1] = 1; b[1][2] = 0;
    b[2][0] = 1; b[2][1] = 0; b[2][2] = 2;

    /* C = A * B */
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            c[i][j] = 0;
            for (k = 0; k < 3; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    printf("A*B:\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            printf("%d ", c[i][j]);
        }
        printf("\n");
    }

    /* Trace of result */
    sum = 0;
    for (i = 0; i < 3; i++) {
        sum += c[i][i];
    }
    printf("trace(A*B) = %d\n", sum);

    /* Determinant of B using expansion */
    det = b[0][0] * (b[1][1] * b[2][2] - b[1][2] * b[2][1])
        - b[0][1] * (b[1][0] * b[2][2] - b[1][2] * b[2][0])
        + b[0][2] * (b[1][0] * b[2][1] - b[1][1] * b[2][0]);
    printf("det(B) = %d\n", det);

    /* Matrix addition A + B */
    printf("A+B:\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            printf("%d ", a[i][j] + b[i][j]);
        }
        printf("\n");
    }

    /* Scalar multiply 2*A */
    printf("2*A:\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            printf("%d ", 2 * a[i][j]);
        }
        printf("\n");
    }

    return 0;
}
