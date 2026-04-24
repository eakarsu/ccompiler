int printf(const char *fmt, ...);
// EXPECT: 1 3 7 9 7 1 9 3 9 7 3 1 3 9 1 7 1 3 9 7 5 15 15 15 15 4 2 0 1 1 16 1 136 36 66 3 7 7 3 40 102 1 15 10 13 1 1 5 7 46 34

// Matrix rotation (90/180/270), spiral traversal, diagonal operations

int mat[10][10];
int tmp[10][10];
int out[100];

// Initialize matrix with sequential values
void mat_init_seq(int n) {
    int i;
    int j;
    int val;
    val = 1;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            mat[i][j] = val;
            val = val + 1;
        }
    }
}

// Copy mat to tmp
void mat_copy_to_tmp(int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            tmp[i][j] = mat[i][j];
        }
    }
}

// Copy tmp to mat
void mat_copy_from_tmp(int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            mat[i][j] = tmp[i][j];
        }
    }
}

// Rotate matrix 90 degrees clockwise
void rotate_90(int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            tmp[j][n - 1 - i] = mat[i][j];
        }
    }
    mat_copy_from_tmp(n);
}

// Rotate matrix 180 degrees
void rotate_180(int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            tmp[n - 1 - i][n - 1 - j] = mat[i][j];
        }
    }
    mat_copy_from_tmp(n);
}

// Rotate matrix 270 degrees clockwise (= 90 counter-clockwise)
void rotate_270(int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            tmp[n - 1 - j][i] = mat[i][j];
        }
    }
    mat_copy_from_tmp(n);
}

// Spiral traversal - store in out array, return count
int spiral_traverse(int n) {
    int top;
    int bottom;
    int left;
    int right;
    int idx;
    int i;
    top = 0;
    bottom = n - 1;
    left = 0;
    right = n - 1;
    idx = 0;
    while (top <= bottom && left <= right) {
        for (i = left; i <= right; i = i + 1) {
            out[idx] = mat[top][i];
            idx = idx + 1;
        }
        top = top + 1;
        for (i = top; i <= bottom; i = i + 1) {
            out[idx] = mat[i][right];
            idx = idx + 1;
        }
        right = right - 1;
        if (top <= bottom) {
            for (i = right; i >= left; i = i - 1) {
                out[idx] = mat[bottom][i];
                idx = idx + 1;
            }
            bottom = bottom - 1;
        }
        if (left <= right) {
            for (i = bottom; i >= top; i = i - 1) {
                out[idx] = mat[i][left];
                idx = idx + 1;
            }
            left = left + 1;
        }
    }
    return idx;
}

// Transpose matrix in place
void transpose(int n) {
    int i;
    int j;
    int t;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            t = mat[i][j];
            mat[i][j] = mat[j][i];
            mat[j][i] = t;
        }
    }
}

// Sum of main diagonal
int diag_main_sum(int n) {
    int i;
    int sum;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        sum = sum + mat[i][i];
    }
    return sum;
}

// Sum of anti-diagonal
int diag_anti_sum(int n) {
    int i;
    int sum;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        sum = sum + mat[i][n - 1 - i];
    }
    return sum;
}

// Sum of row
int row_sum(int row, int n) {
    int j;
    int sum;
    sum = 0;
    for (j = 0; j < n; j = j + 1) {
        sum = sum + mat[row][j];
    }
    return sum;
}

// Sum of column
int col_sum(int col, int n) {
    int i;
    int sum;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        sum = sum + mat[i][col];
    }
    return sum;
}

// Matrix multiply: result in tmp, square matrices
void mat_multiply(int a[10][10], int b[10][10], int n) {
    int i;
    int j;
    int k;
    int sum;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            sum = 0;
            for (k = 0; k < n; k = k + 1) {
                sum = sum + a[i][k] * b[k][j];
            }
            tmp[i][j] = sum;
        }
    }
}

// Check if matrix is symmetric
int is_symmetric(int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            if (mat[i][j] != mat[j][i]) return 0;
        }
    }
    return 1;
}

// Check if matrix is identity
int is_identity(int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            if (i == j) {
                if (mat[i][j] != 1) return 0;
            } else {
                if (mat[i][j] != 0) return 0;
            }
        }
    }
    return 1;
}

// Max element in matrix
int mat_max(int n) {
    int i;
    int j;
    int mx;
    mx = mat[0][0];
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            if (mat[i][j] > mx) mx = mat[i][j];
        }
    }
    return mx;
}

// Min element in matrix
int mat_min(int n) {
    int i;
    int j;
    int mn;
    mn = mat[0][0];
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            if (mat[i][j] < mn) mn = mat[i][j];
        }
    }
    return mn;
}

// Sum of all elements
int mat_total(int n) {
    int i;
    int j;
    int sum;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            sum = sum + mat[i][j];
        }
    }
    return sum;
}

// Trace of upper triangle (above main diag)
int upper_tri_sum(int n) {
    int i;
    int j;
    int sum;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            sum = sum + mat[i][j];
        }
    }
    return sum;
}

// Trace of lower triangle (below main diag)
int lower_tri_sum(int n) {
    int i;
    int j;
    int sum;
    sum = 0;
    for (i = 1; i < n; i = i + 1) {
        for (j = 0; j < i; j = j + 1) {
            sum = sum + mat[i][j];
        }
    }
    return sum;
}

// Flip matrix horizontally (mirror columns)
void flip_horizontal(int n) {
    int i;
    int j;
    int t;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n / 2; j = j + 1) {
            t = mat[i][j];
            mat[i][j] = mat[i][n - 1 - j];
            mat[i][n - 1 - j] = t;
        }
    }
}

// Flip matrix vertically (mirror rows)
void flip_vertical(int n) {
    int i;
    int j;
    int t;
    for (i = 0; i < n / 2; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            t = mat[i][j];
            mat[i][j] = mat[n - 1 - i][j];
            mat[n - 1 - i][j] = t;
        }
    }
}

// Set matrix value
void mat_set(int r, int c, int v) {
    mat[r][c] = v;
}

// Get matrix value
int mat_get(int r, int c) {
    return mat[r][c];
}

// Diagonal traversal: print diagonals from top-right to bottom-left
int diag_traverse(int n) {
    int d;
    int i;
    int j;
    int idx;
    idx = 0;
    for (d = 0; d < 2 * n - 1; d = d + 1) {
        if (d < n) {
            i = 0;
            j = d;
        } else {
            i = d - n + 1;
            j = n - 1;
        }
        while (i < n && j >= 0) {
            out[idx] = mat[i][j];
            idx = idx + 1;
            i = i + 1;
            j = j - 1;
        }
    }
    return idx;
}

// Border sum
int border_sum(int n) {
    int i;
    int sum;
    sum = 0;
    if (n == 1) return mat[0][0];
    for (i = 0; i < n; i = i + 1) {
        sum = sum + mat[0][i];
        sum = sum + mat[n - 1][i];
    }
    for (i = 1; i < n - 1; i = i + 1) {
        sum = sum + mat[i][0];
        sum = sum + mat[i][n - 1];
    }
    return sum;
}

int main(void) {
    int i;
    int cnt;
    int first;

    first = 1;

    // Test 1-4: 3x3 matrix init and corners
    mat_init_seq(3);
    printf("%d", mat[0][0]); // 1
    printf(" %d", mat[0][2]); // 3
    printf(" %d", mat[2][0]); // 7
    printf(" %d", mat[2][2]); // 9

    // Test 5-8: Rotate 90 degrees clockwise
    // Original 3x3: 1 2 3 / 4 5 6 / 7 8 9
    // After 90 CW:  7 4 1 / 8 5 2 / 9 6 3
    mat_init_seq(3);
    rotate_90(3);
    printf(" %d", mat[0][0]); // 7
    printf(" %d", mat[0][2]); // 1
    printf(" %d", mat[2][0]); // 9
    printf(" %d", mat[2][2]); // 3

    // Test 9-12: Rotate 180
    // 9 8 7 / 6 5 4 / 3 2 1
    mat_init_seq(3);
    rotate_180(3);
    printf(" %d", mat[0][0]); // 9
    printf(" %d", mat[0][2]); // 7
    printf(" %d", mat[2][0]); // 3
    printf(" %d", mat[2][2]); // 1

    // Test 13-16: Rotate 270
    // 3 6 9 / 2 5 8 / 1 4 7
    mat_init_seq(3);
    rotate_270(3);
    printf(" %d", mat[0][0]); // 3
    printf(" %d", mat[0][2]); // 9
    printf(" %d", mat[2][0]); // 1
    printf(" %d", mat[2][2]); // 7

    // Test 17-21: Spiral traversal of 3x3
    // 1 2 3 / 4 5 6 / 7 8 9 => spiral: 1 2 3 6 9 8 7 4 5
    mat_init_seq(3);
    cnt = spiral_traverse(3);
    printf(" %d", out[0]); // 1
    printf(" %d", out[2]); // 3
    printf(" %d", out[4]); // 9
    printf(" %d", out[6]); // 7
    printf(" %d", out[8]); // 5

    // Test 22-23: Diagonal sums of 3x3
    mat_init_seq(3);
    printf(" %d", diag_main_sum(3)); // 1+5+9 = 15
    printf(" %d", diag_anti_sum(3)); // 3+5+7 = 15

    // Test 24-25: Row and column sums
    printf(" %d", row_sum(1, 3)); // 4+5+6 = 15
    printf(" %d", col_sum(1, 3)); // 2+5+8 = 15

    // Test 26-27: Transpose
    mat_init_seq(3);
    transpose(3);
    printf(" %d", mat[0][1]); // was 2, now mat[0][1] = 4
    printf(" %d", mat[1][0]); // was 4, now mat[1][0] = 2

    // Test 28: Symmetric check (not symmetric after seq init)
    mat_init_seq(3);
    printf(" %d", is_symmetric(3)); // 0

    // Test 29: Make symmetric and check
    mat[0][0]=1; mat[0][1]=2; mat[0][2]=3;
    mat[1][0]=2; mat[1][1]=5; mat[1][2]=6;
    mat[2][0]=3; mat[2][1]=6; mat[2][2]=9;
    printf(" %d", is_symmetric(3)); // 1

    // Test 30: Identity check
    mat[0][0]=1; mat[0][1]=0; mat[0][2]=0;
    mat[1][0]=0; mat[1][1]=1; mat[1][2]=0;
    mat[2][0]=0; mat[2][1]=0; mat[2][2]=1;
    printf(" %d", is_identity(3)); // 1

    // Test 31-33: Max, min, total of 4x4
    mat_init_seq(4);
    printf(" %d", mat_max(4));   // 16
    printf(" %d", mat_min(4));   // 1
    printf(" %d", mat_total(4)); // 136

    // Test 34-35: Upper and lower triangle sums (4x4 sequential)
    // upper: 2+3+4+7+8+12 = 36
    // lower: 5+9+10+13+14+15 = 66
    printf(" %d", upper_tri_sum(4)); // 36
    printf(" %d", lower_tri_sum(4)); // 66

    // Test 36-37: Flip horizontal on 3x3
    mat_init_seq(3);
    flip_horizontal(3);
    // 3 2 1 / 6 5 4 / 9 8 7
    printf(" %d", mat[0][0]); // 3
    printf(" %d", mat[2][2]); // 7

    // Test 38-39: Flip vertical on 3x3
    mat_init_seq(3);
    flip_vertical(3);
    // 7 8 9 / 4 5 6 / 1 2 3
    printf(" %d", mat[0][0]); // 7
    printf(" %d", mat[2][2]); // 3

    // Test 40-41: Border sum
    mat_init_seq(3);
    printf(" %d", border_sum(3)); // 1+2+3+7+8+9+4+6 = 40
    mat_init_seq(4);
    printf(" %d", border_sum(4)); // top:1+2+3+4=10, bot:13+14+15+16=58, left:5+9=14, right:8+12=20 => 10+58+14+20=102

    // Test 42-44: 4x4 spiral first, middle, last
    mat_init_seq(4);
    cnt = spiral_traverse(4);
    printf(" %d", out[0]);  // 1
    printf(" %d", out[7]);  // 8
    printf(" %d", out[15]); // 11 (center)

    // Test 45-46: 4x4 rotate 90, check corners
    mat_init_seq(4);
    rotate_90(4);
    printf(" %d", mat[0][0]); // 13
    printf(" %d", mat[0][3]); // 1

    // Test 47-48: Diagonal traverse of 3x3
    mat_init_seq(3);
    cnt = diag_traverse(3);
    // diags: (0,0)=1, (0,1)(1,0)=2,4, (0,2)(1,1)(2,0)=3,5,7, (1,2)(2,1)=6,8, (2,2)=9
    printf(" %d", out[0]); // 1
    printf(" %d", out[4]); // 5 (which is mat[1][1])

    // Test 49: Matrix multiply 2x2
    mat[0][0]=1; mat[0][1]=2;
    mat[1][0]=3; mat[1][1]=4;
    mat_copy_to_tmp(2);
    mat_multiply(mat, tmp, 2);
    // result in tmp: [1*1+2*3, 1*2+2*4] = [7, 10]
    //                [3*1+4*3, 3*2+4*4] = [15, 22]
    printf(" %d", tmp[0][0]); // 7
    printf(" %d", tmp[1][1]); // 22

    // Test 50: 4x4 diagonal sums
    mat_init_seq(4);
    printf(" %d", diag_main_sum(4)); // 1+6+11+16 = 34

    printf("\n");
    return 0;
}
