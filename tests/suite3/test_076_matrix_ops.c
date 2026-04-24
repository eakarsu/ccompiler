int printf(const char *fmt, ...);

/* 3x3 and 4x4 matrix operations (integer arithmetic) */

#define N3 3
#define N4 4

/* Print NxN matrix */
void mat_print3(int m[N3][N3]) {
    int i, j;
    for (i = 0; i < N3; i++) {
        for (j = 0; j < N3; j++) {
            if (j > 0) printf(" ");
            printf("%d", m[i][j]);
        }
        printf("\n");
    }
}

void mat_print4(int m[N4][N4]) {
    int i, j;
    for (i = 0; i < N4; i++) {
        for (j = 0; j < N4; j++) {
            if (j > 0) printf(" ");
            printf("%d", m[i][j]);
        }
        printf("\n");
    }
}

/* Add two 3x3 matrices */
void mat_add3(int a[N3][N3], int b[N3][N3], int c[N3][N3]) {
    int i, j;
    for (i = 0; i < N3; i++)
        for (j = 0; j < N3; j++)
            c[i][j] = a[i][j] + b[i][j];
}

/* Multiply two 3x3 matrices */
void mat_mul3(int a[N3][N3], int b[N3][N3], int c[N3][N3]) {
    int i, j, k;
    for (i = 0; i < N3; i++)
        for (j = 0; j < N3; j++) {
            c[i][j] = 0;
            for (k = 0; k < N3; k++)
                c[i][j] += a[i][k] * b[k][j];
        }
}

/* Transpose 3x3 matrix */
void mat_transpose3(int a[N3][N3], int b[N3][N3]) {
    int i, j;
    for (i = 0; i < N3; i++)
        for (j = 0; j < N3; j++)
            b[j][i] = a[i][j];
}

/* Trace of 3x3 matrix */
int mat_trace3(int a[N3][N3]) {
    return a[0][0] + a[1][1] + a[2][2];
}

/* Is identity 3x3? */
int mat_is_identity3(int a[N3][N3]) {
    int i, j;
    for (i = 0; i < N3; i++)
        for (j = 0; j < N3; j++) {
            int expected = (i == j) ? 1 : 0;
            if (a[i][j] != expected) return 0;
        }
    return 1;
}

/* Matrix-vector multiply 3x3 * vec3 */
void mat_vec_mul3(int m[N3][N3], int v[N3], int out[N3]) {
    int i, j;
    for (i = 0; i < N3; i++) {
        out[i] = 0;
        for (j = 0; j < N3; j++)
            out[i] += m[i][j] * v[j];
    }
}

/* Scalar multiply 4x4 */
void mat_scalar_mul4(int a[N4][N4], int s, int c[N4][N4]) {
    int i, j;
    for (i = 0; i < N4; i++)
        for (j = 0; j < N4; j++)
            c[i][j] = a[i][j] * s;
}

int main(void) {
    /* 3x3 identity */
    int I3[N3][N3] = {{1,0,0},{0,1,0},{0,0,1}};
    printf("%d\n", mat_is_identity3(I3));  /* 1 */
    // EXPECT: 1
    printf("%d\n", mat_trace3(I3));        /* 3 */
    // EXPECT: 3

    /* Matrix A */
    int A[N3][N3] = {{1,2,3},{4,5,6},{7,8,9}};
    printf("%d\n", mat_trace3(A));         /* 15 */
    // EXPECT: 15

    /* A + I3 */
    int S[N3][N3];
    mat_add3(A, I3, S);
    mat_print3(S);
    // EXPECT: 2 2 3
    // EXPECT: 4 6 6
    // EXPECT: 7 8 10

    /* Transpose of A */
    int AT[N3][N3];
    mat_transpose3(A, AT);
    mat_print3(AT);
    // EXPECT: 1 4 7
    // EXPECT: 2 5 8
    // EXPECT: 3 6 9

    /* A * I3 = A */
    int AI[N3][N3];
    mat_mul3(A, I3, AI);
    mat_print3(AI);
    // EXPECT: 1 2 3
    // EXPECT: 4 5 6
    // EXPECT: 7 8 9

    /* B = {{2,0,1},{1,3,0},{0,1,2}} */
    int B[N3][N3] = {{2,0,1},{1,3,0},{0,1,2}};

    /* A * B */
    int AB[N3][N3];
    mat_mul3(A, B, AB);
    /* Row0: 1*2+2*1+3*0=4, 1*0+2*3+3*1=9, 1*1+2*0+3*2=7 */
    /* Row1: 4*2+5*1+6*0=13, 4*0+5*3+6*1=21, 4*1+5*0+6*2=16 */
    /* Row2: 7*2+8*1+9*0=22, 7*0+8*3+9*1=33, 7*1+8*0+9*2=25 */
    mat_print3(AB);
    // EXPECT: 4 9 7
    // EXPECT: 13 21 16
    // EXPECT: 22 33 25

    /* Matrix-vector multiply: A * [1,0,0] = first column of A */
    int v1[N3] = {1, 0, 0};
    int r1[N3];
    mat_vec_mul3(A, v1, r1);
    /* [1*1+2*0+3*0, 4*1+5*0+6*0, 7*1+8*0+9*0] = [1,4,7] */
    printf("%d %d %d\n", r1[0], r1[1], r1[2]);
    // EXPECT: 1 4 7

    /* A * [1,1,1] = row sums */
    int v2[N3] = {1, 1, 1};
    int r2[N3];
    mat_vec_mul3(A, v2, r2);
    /* [6, 15, 24] */
    printf("%d %d %d\n", r2[0], r2[1], r2[2]);
    // EXPECT: 6 15 24

    /* 4x4 matrix scalar multiply */
    int M4[N4][N4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9,10,11,12},
        {13,14,15,16}
    };
    int M4s[N4][N4];
    mat_scalar_mul4(M4, 3, M4s);
    mat_print4(M4s);
    // EXPECT: 3 6 9 12
    // EXPECT: 15 18 21 24
    // EXPECT: 27 30 33 36
    // EXPECT: 39 42 45 48

    /* Identity check after A*AT is NOT identity (just check trace) */
    int AAt[N3][N3];
    mat_mul3(A, AT, AAt);
    /* A*AT:
       Row0: [1,2,3]*[1,2,3]=14, [1,2,3]*[4,5,6]=32, [1,2,3]*[7,8,9]=50
       Row1: [4,5,6]*[1,2,3]=32, [4,5,6]*[4,5,6]=77, [4,5,6]*[7,8,9]=122  -- wait
       Actually A*AT where AT[col] = A[row]:
       c[i][j] = sum_k A[i][k]*AT[k][j] = sum_k A[i][k]*A[j][k]
       c[0][0]=1+4+9=14, c[0][1]=4+10+18=32, c[0][2]=7+16+27=50
       c[1][0]=4+10+18=32, c[1][1]=16+25+36=77, c[1][2]=28+40+54=122
       c[2][0]=7+16+27=50, c[2][1]=28+40+54=122, c[2][2]=49+64+81=194
    */
    printf("%d\n", mat_trace3(AAt));       /* 14+77+194=285 */
    // EXPECT: 285
    printf("%d\n", mat_is_identity3(AAt)); /* 0 */
    // EXPECT: 0

    return 0;
}
