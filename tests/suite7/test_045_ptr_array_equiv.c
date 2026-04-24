int printf(const char *fmt, ...);

int main(void) {
    int arr[5];
    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 50;

    // arr[i] == *(arr+i)
    // EXPECT: arr[0] = 10
    printf("arr[0] = %d\n", arr[0]);

    // EXPECT: *(arr+0) = 10
    printf("*(arr+0) = %d\n", *(arr + 0));

    // EXPECT: arr[2] = 30
    printf("arr[2] = %d\n", arr[2]);

    // EXPECT: *(arr+2) = 30
    printf("*(arr+2) = %d\n", *(arr + 2));

    // *(i+arr) commutative form
    // EXPECT: *(0+arr) = 10
    printf("*(0+arr) = %d\n", *(0 + arr));

    // EXPECT: *(3+arr) = 40
    printf("*(3+arr) = %d\n", *(3 + arr));

    // i[arr] form (commutative subscript)
    // EXPECT: 0[arr] = 10
    printf("0[arr] = %d\n", 0[arr]);

    // EXPECT: 1[arr] = 20
    printf("1[arr] = %d\n", 1[arr]);

    // EXPECT: 4[arr] = 50
    printf("4[arr] = %d\n", 4[arr]);

    // All four forms for same index
    int idx = 3;
    int v1 = arr[idx];
    int v2 = *(arr + idx);
    int v3 = *(idx + arr);
    int v4 = idx[arr];
    // EXPECT: all_equal_40 = 1
    printf("all_equal_40 = %d\n", (v1 == 40 && v2 == 40 && v3 == 40 && v4 == 40));

    // Pointer variable equivalence
    int *p = arr;
    // EXPECT: p[0] = 10
    printf("p[0] = %d\n", p[0]);

    // EXPECT: *(p+1) = 20
    printf("*(p+1) = %d\n", *(p + 1));

    // EXPECT: 2[p] = 30
    printf("2[p] = %d\n", 2[p]);

    // Character arrays
    char str[6];
    str[0] = 'H'; str[1] = 'e'; str[2] = 'l'; str[3] = 'l'; str[4] = 'o'; str[5] = 0;

    // EXPECT: str[0] = 72
    printf("str[0] = %d\n", (int)str[0]);

    // EXPECT: *(str+4) = 111
    printf("*(str+4) = %d\n", (int)*(str + 4));

    // EXPECT: 1[str] = 101
    printf("1[str] = %d\n", (int)(1[str]));

    // Sum via different access methods
    int sum1 = 0;
    int sum2 = 0;
    int sum3 = 0;
    int sum4 = 0;
    int i;
    for (i = 0; i < 5; i++) {
        sum1 = sum1 + arr[i];
        sum2 = sum2 + *(arr + i);
        sum3 = sum3 + *(i + arr);
        sum4 = sum4 + i[arr];
    }
    // EXPECT: sum1 = 150
    printf("sum1 = %d\n", sum1);

    // EXPECT: sum2 = 150
    printf("sum2 = %d\n", sum2);

    // EXPECT: sum3 = 150
    printf("sum3 = %d\n", sum3);

    // EXPECT: sum4 = 150
    printf("sum4 = %d\n", sum4);

    // Nested array access
    int mat[3][3];
    int r, c2;
    for (r = 0; r < 3; r++) {
        for (c2 = 0; c2 < 3; c2++) {
            mat[r][c2] = r * 3 + c2 + 1;
        }
    }

    // EXPECT: mat[1][2] = 6
    printf("mat[1][2] = %d\n", mat[1][2]);

    // EXPECT: *(*(mat+1)+2) = 6
    printf("*(*(mat+1)+2) = %d\n", *(*(mat + 1) + 2));

    // EXPECT: *(mat[2]+0) = 7
    printf("*(mat[2]+0) = %d\n", *(mat[2] + 0));

    // Equivalence across pointer arithmetic
    int data[4];
    data[0] = 5; data[1] = 15; data[2] = 25; data[3] = 35;
    int *dp = data + 1;
    // EXPECT: dp[-1] = 5
    printf("dp[-1] = %d\n", dp[-1]);

    // EXPECT: dp[0] = 15
    printf("dp[0] = %d\n", dp[0]);

    // EXPECT: dp[2] = 35
    printf("dp[2] = %d\n", dp[2]);

    return 0;
}
