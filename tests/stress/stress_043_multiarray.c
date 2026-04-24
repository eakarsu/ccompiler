// stress_043_multiarray.c - Multi-dimensional array stress test
// EXPECT: 3d_corner=63
// EXPECT: 3d_sum=2016
// EXPECT: 4d_val=15
// EXPECT: 4d_sum=120
// EXPECT: arr_of_ptr=42
// EXPECT: ptr_to_2d=99
// EXPECT: pass_3d=1000
// EXPECT: flatten_sum=2016
// EXPECT: row_col_sum=268
// EXPECT: diagonal=40
// EXPECT: transpose_ok=1
// EXPECT: spiral_first=0
// EXPECT: spiral_last=9

int printf(const char *fmt, ...);

int sum_3d(int a[][4][4], int d0) {
    int s = 0;
    int i, j, k;
    for (i = 0; i < d0; i++)
        for (j = 0; j < 4; j++)
            for (k = 0; k < 4; k++)
                s = s + a[i][j][k];
    return s;
}

int sum_row(int a[][4], int row) {
    int s = 0;
    int j;
    for (j = 0; j < 4; j++)
        s = s + a[row][j];
    return s;
}

int sum_col(int a[][4], int col) {
    int s = 0;
    int i;
    for (i = 0; i < 4; i++)
        s = s + a[i][col];
    return s;
}

int sum_flat(int *p, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++)
        s = s + p[i];
    return s;
}

int main(void) {
    int i, j, k, l;

    // --- 3D array int a[4][4][4], values = i*16+j*4+k (0..63) ---
    int a3[4][4][4];
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            for (k = 0; k < 4; k++)
                a3[i][j][k] = i * 16 + j * 4 + k;

    printf("3d_corner=%d\n", a3[3][3][3]);   // 48+12+3=63
    printf("3d_sum=%d\n", sum_3d(a3, 4));     // sum(0..63) = 63*64/2 = 2016

    // --- 4D array int b[2][2][2][2], values = i*8+j*4+k*2+l (0..15) ---
    int b4[2][2][2][2];
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++)
            for (k = 0; k < 2; k++)
                for (l = 0; l < 2; l++)
                    b4[i][j][k][l] = i * 8 + j * 4 + k * 2 + l;

    printf("4d_val=%d\n", b4[1][1][1][1]);   // 8+4+2+1=15

    int s4 = 0;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++)
            for (k = 0; k < 2; k++)
                for (l = 0; l < 2; l++)
                    s4 = s4 + b4[i][j][k][l];
    printf("4d_sum=%d\n", s4);               // sum(0..15) = 15*16/2 = 120

    // --- Array of pointers to arrays ---
    int row0[4]; row0[0]=10; row0[1]=11; row0[2]=12; row0[3]=13;
    int row1[4]; row1[0]=20; row1[1]=21; row1[2]=22; row1[3]=23;
    int row2[4]; row2[0]=30; row2[1]=31; row2[2]=42; row2[3]=33;
    int row3[4]; row3[0]=40; row3[1]=41; row3[2]=42; row3[3]=43;

    int *rows[4];
    rows[0] = row0; rows[1] = row1; rows[2] = row2; rows[3] = row3;
    printf("arr_of_ptr=%d\n", rows[2][2]);    // row2[2] = 42

    // --- Pointer to 2D array, modify element via pointer arithmetic ---
    int m2[4][4];
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            m2[i][j] = i * 10 + j;

    int *pm2 = &m2[0][0];
    pm2[2 * 4 + 3] = 99;  // m2[2][3] = 99
    printf("ptr_to_2d=%d\n", m2[2][3]);

    // --- Pass 3D array to function ---
    // a3[i][j][k] = (i+1)*(j+1)*(k+1)
    // sum = [sum_{i=0}^3 (i+1)] * [sum_{j=0}^3 (j+1)] * [sum_{k=0}^3 (k+1)]
    //     = 10 * 10 * 10 = 1000
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            for (k = 0; k < 4; k++)
                a3[i][j][k] = (i + 1) * (j + 1) * (k + 1);
    printf("pass_3d=%d\n", sum_3d(a3, 4));

    // --- Flattened access via pointer arithmetic ---
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            for (k = 0; k < 4; k++)
                a3[i][j][k] = i * 16 + j * 4 + k;
    int *flat = &a3[0][0][0];
    printf("flatten_sum=%d\n", sum_flat(flat, 64)); // sum(0..63)=2016

    // --- Row and column sums ---
    // m2[i][j] = (i+1)*(10+j)
    // Row 2: 3*10=30, 3*11=33, 3*12=36, 3*13=39, sum=138
    // Col 3: 1*13=13, 2*13=26, 3*13=39, 4*13=52, sum=130
    // row_col_sum = 138+130 = 268
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            m2[i][j] = (i + 1) * (10 + j);
    printf("row_col_sum=%d\n", sum_row(m2, 2) + sum_col(m2, 3));

    // --- Diagonal sum ---
    // m2[i][j] = i*(i+1) + j*(j+1)
    // diag: m2[0][0]=0, m2[1][1]=2+2=4, m2[2][2]=6+6=12, m2[3][3]=12+12=24
    // sum = 0+4+12+24 = 40
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            m2[i][j] = i * (i + 1) + j * (j + 1);
    {
        int dsum = 0;
        for (i = 0; i < 4; i++)
            dsum = dsum + m2[i][i];
        printf("diagonal=%d\n", dsum);
    }

    // --- Transpose and verify ---
    int orig[4][4];
    int trans[4][4];
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            orig[i][j] = i * 4 + j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            trans[j][i] = orig[i][j];
    int ok = 1;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (trans[i][j] != orig[j][i])
                ok = 0;
    printf("transpose_ok=%d\n", ok);

    // --- Spiral traversal of 4x4 ---
    // orig[i][j] = i*4+j (0..15)
    // Spiral order: (0,0),(0,1),(0,2),(0,3),(1,3),(2,3),(3,3),(3,2),(3,1),(3,0),
    //               (2,0),(1,0),(1,1),(1,2),(2,2),(2,1)
    int si[16]; int sj[16];
    si[0]=0; sj[0]=0;   si[1]=0; sj[1]=1;   si[2]=0; sj[2]=2;   si[3]=0; sj[3]=3;
    si[4]=1; sj[4]=3;   si[5]=2; sj[5]=3;   si[6]=3; sj[6]=3;   si[7]=3; sj[7]=2;
    si[8]=3; sj[8]=1;   si[9]=3; sj[9]=0;   si[10]=2; sj[10]=0; si[11]=1; sj[11]=0;
    si[12]=1; sj[12]=1; si[13]=1; sj[13]=2; si[14]=2; sj[14]=2; si[15]=2; sj[15]=1;

    printf("spiral_first=%d\n", orig[si[0]][sj[0]]);     // orig[0][0]=0
    printf("spiral_last=%d\n", orig[si[15]][sj[15]]);     // orig[2][1]=9

    return 0;
}
