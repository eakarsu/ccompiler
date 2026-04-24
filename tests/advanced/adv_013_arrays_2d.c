int printf(const char *fmt, ...);
// EXPECT: === 2D Array Tests ===
// EXPECT: basic_2d: m[0][0]=1 m[0][1]=2 m[1][0]=3 m[1][1]=4
// EXPECT: loop_fill:
// EXPECT:  0 1 2
// EXPECT:  3 4 5
// EXPECT:  6 7 8
// EXPECT: row_sum: row0=3 row1=12 row2=21
// EXPECT: col_sum: col0=9 col1=12 col2=15
// EXPECT: total_sum: 36
// EXPECT: identity:
// EXPECT:  1 0 0
// EXPECT:  0 1 0
// EXPECT:  0 0 1
// EXPECT: matrix_add:
// EXPECT:  2 4 6
// EXPECT:  8 10 12
// EXPECT: transpose:
// EXPECT:  1 4
// EXPECT:  2 5
// EXPECT:  3 6
// EXPECT: flat_access: m[1][2]=5
// EXPECT: diagonal_sum: 15
// EXPECT: max_element: 20
// EXPECT: scale_matrix:
// EXPECT:  3 6 9
// EXPECT:  12 15 18
// EXPECT: row_major_order: 1 2 3 4 5 6 7 8 9
// EXPECT: func_param_sum: 45
// EXPECT: matrix_equal: yes
// EXPECT: border_sum: 12
// EXPECT: === All 2D Array Tests Passed ===

int matrix_sum_3x3(int m[3][3]) {
    int s = 0;
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            s = s + m[i][j];
        }
    }
    return s;
}

void print_matrix_3x3(int m[3][3]) {
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        printf(" ");
        for (j = 0; j < 3; j++) {
            if (j > 0) printf(" ");
            printf("%d", m[i][j]);
        }
        printf("\n");
    }
}

void print_matrix_2x3(int m[2][3]) {
    int i;
    int j;
    for (i = 0; i < 2; i++) {
        printf(" ");
        for (j = 0; j < 3; j++) {
            if (j > 0) printf(" ");
            printf("%d", m[i][j]);
        }
        printf("\n");
    }
}

void print_matrix_3x2(int m[3][2]) {
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        printf(" ");
        for (j = 0; j < 2; j++) {
            if (j > 0) printf(" ");
            printf("%d", m[i][j]);
        }
        printf("\n");
    }
}

int main() {
    printf("=== 2D Array Tests ===\n");

    // basic 2D array declaration and access
    int m1[2][2];
    m1[0][0] = 1; m1[0][1] = 2;
    m1[1][0] = 3; m1[1][1] = 4;
    printf("basic_2d: m[0][0]=%d m[0][1]=%d m[1][0]=%d m[1][1]=%d\n",
           m1[0][0], m1[0][1], m1[1][0], m1[1][1]);

    // fill with nested loops
    int m2[3][3];
    int i;
    int j;
    int val = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            m2[i][j] = val;
            val = val + 1;
        }
    }
    printf("loop_fill:\n");
    print_matrix_3x3(m2);

    // row sums
    int rs0 = 0;
    int rs1 = 0;
    int rs2 = 0;
    for (j = 0; j < 3; j++) {
        rs0 = rs0 + m2[0][j];
        rs1 = rs1 + m2[1][j];
        rs2 = rs2 + m2[2][j];
    }
    printf("row_sum: row0=%d row1=%d row2=%d\n", rs0, rs1, rs2);

    // column sums
    int cs0 = 0;
    int cs1 = 0;
    int cs2 = 0;
    for (i = 0; i < 3; i++) {
        cs0 = cs0 + m2[i][0];
        cs1 = cs1 + m2[i][1];
        cs2 = cs2 + m2[i][2];
    }
    printf("col_sum: col0=%d col1=%d col2=%d\n", cs0, cs1, cs2);

    // total sum
    int total = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            total = total + m2[i][j];
        }
    }
    printf("total_sum: %d\n", total);

    // identity matrix
    int id[3][3];
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (i == j)
                id[i][j] = 1;
            else
                id[i][j] = 0;
        }
    }
    printf("identity:\n");
    print_matrix_3x3(id);

    // matrix addition (2x3)
    int a[2][3];
    int b[2][3];
    int c[2][3];
    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
    a[1][0] = 4; a[1][1] = 5; a[1][2] = 6;
    b[0][0] = 1; b[0][1] = 2; b[0][2] = 3;
    b[1][0] = 4; b[1][1] = 5; b[1][2] = 6;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
    printf("matrix_add:\n");
    print_matrix_2x3(c);

    // transpose (2x3 -> 3x2)
    int t[3][2];
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            t[j][i] = a[i][j];
        }
    }
    printf("transpose:\n");
    print_matrix_3x2(t);

    // flat access via row-major index calculation
    // m2[1][2] = 1*3 + 2 = 5
    int *flat = (int *)m2;
    int flat_val = *(flat + 1 * 3 + 2);
    printf("flat_access: m[1][2]=%d\n", flat_val);

    // diagonal sum of 5x5
    int d5[5][5];
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            d5[i][j] = 0;
        }
    }
    for (i = 0; i < 5; i++) {
        d5[i][i] = i + 1;
    }
    int diag_sum = 0;
    for (i = 0; i < 5; i++) {
        diag_sum = diag_sum + d5[i][i];
    }
    printf("diagonal_sum: %d\n", diag_sum);

    // find max element in 4x4
    int mx[4][4];
    val = 1;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            mx[i][j] = val;
            val = val + 1;
        }
    }
    // override one element to be max
    mx[2][3] = 20;
    int max_el = mx[0][0];
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (mx[i][j] > max_el) max_el = mx[i][j];
        }
    }
    printf("max_element: %d\n", max_el);

    // scale matrix by constant
    int sc[2][3];
    sc[0][0] = 1; sc[0][1] = 2; sc[0][2] = 3;
    sc[1][0] = 4; sc[1][1] = 5; sc[1][2] = 6;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            sc[i][j] = sc[i][j] * 3;
        }
    }
    printf("scale_matrix:\n");
    print_matrix_2x3(sc);

    // row-major order traversal
    int rm[3][3];
    val = 1;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            rm[i][j] = val;
            val = val + 1;
        }
    }
    printf("row_major_order:");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            printf(" %d", rm[i][j]);
        }
    }
    printf("\n");

    // pass 2D array to function for sum
    printf("func_param_sum: %d\n", matrix_sum_3x3(rm));

    // matrix equality check
    int eq1[3][3];
    int eq2[3][3];
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            eq1[i][j] = i * 3 + j;
            eq2[i][j] = i * 3 + j;
        }
    }
    int equal = 1;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (eq1[i][j] != eq2[i][j]) equal = 0;
        }
    }
    printf("matrix_equal: %s\n", equal ? "yes" : "no");

    // border sum of 4x4 matrix (all border elements)
    int br[4][4];
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            br[i][j] = 1;
        }
    }
    // interior elements = 0
    br[1][1] = 0; br[1][2] = 0;
    br[2][1] = 0; br[2][2] = 0;
    int border_sum = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i == 0 || i == 3 || j == 0 || j == 3) {
                border_sum = border_sum + br[i][j];
            }
        }
    }
    printf("border_sum: %d\n", border_sum);

    printf("=== All 2D Array Tests Passed ===\n");
    return 0;
}
