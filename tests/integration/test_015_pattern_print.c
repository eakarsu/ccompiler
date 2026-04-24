int printf(const char *fmt, ...);
// EXPECT: === Right Triangle ===\n*\n**\n***\n****\n*****\n=== Inverted Triangle ===\n*****\n****\n***\n**\n*\n=== Number Triangle ===\n1\n12\n123\n1234\n12345\n=== Pyramid ===\n    *\n   ***\n  *****\n *******\n*********\n=== Diamond ===\n    *\n   ***\n  *****\n *******\n*********\n *******\n  *****\n   ***\n    *\n=== Pascal Triangle Row 0 ===\n1\n=== Pascal Triangle Row 1 ===\n1 1\n=== Pascal Triangle Row 2 ===\n1 2 1\n=== Pascal Triangle Row 3 ===\n1 3 3 1\n=== Pascal Triangle Row 4 ===\n1 4 6 4 1\n=== Pascal Triangle Row 5 ===\n1 5 10 10 5 1\n=== Pascal Triangle Row 6 ===\n1 6 15 20 15 6 1\n=== Pascal Triangle Row 7 ===\n1 7 21 35 35 21 7 1\n=== Multiplication Table 1-5 ===\n  1  2  3  4  5\n  2  4  6  8 10\n  3  6  9 12 15\n  4  8 12 16 20\n  5 10 15 20 25\n=== Number Square ===\n1 2 3 4 5\n2 3 4 5 6\n3 4 5 6 7\n4 5 6 7 8\n5 6 7 8 9\n=== Checkerboard 5x5 ===\n1 0 1 0 1\n0 1 0 1 0\n1 0 1 0 1\n0 1 0 1 0\n1 0 1 0 1\n=== Floyd Triangle ===\n1\n2 3\n4 5 6\n7 8 9 10\n11 12 13 14 15\n=== Zigzag Row Pattern ===\nrow0: 1 2 3 4 5\nrow1: 10 9 8 7 6\nrow2: 11 12 13 14 15\nrow3: 20 19 18 17 16\n=== Spiral Fill 4x4 ===\n1 2 3 4\n12 13 14 5\n11 16 15 6\n10 9 8 7\n=== Pattern Validation ===\npascal_sum_row0=1\npascal_sum_row1=2\npascal_sum_row2=4\npascal_sum_row3=8\npascal_sum_row4=16\npascal_sum_row5=32\nmult_table_diag: 1 4 9 16 25\nfloyd_last_row_sum=65\nspiral_sum=136\ncheckerboard_ones=13\n=== Hourglass ===\n*****\n ***\n  *\n ***\n*****\n=== Right Aligned Numbers ===\n    1\n   12\n  123\n 1234\n12345\n=== Hollow Square 5x5 ===\n*****\n*   *\n*   *\n*   *\n*****\n=== Border Number Grid 5x5 ===\n1 1 1 1 1\n1 0 0 0 1\n1 0 0 0 1\n1 0 0 0 1\n1 1 1 1 1\n=== All patterns done ===

// ---- Helper functions ----

void print_char_n(char c, int n) {
    int i;
    i = 0;
    while (i < n) {
        printf("%c", c);
        i = i + 1;
    }
}

void print_spaces(int n) {
    int i;
    i = 0;
    while (i < n) {
        printf(" ");
        i = i + 1;
    }
}

// Right triangle of stars, height h
void right_triangle(int h) {
    int row;
    int col;
    row = 1;
    while (row <= h) {
        col = 0;
        while (col < row) {
            printf("%c", '*');
            col = col + 1;
        }
        printf("\n");
        row = row + 1;
    }
}

// Inverted triangle of stars, height h
void inverted_triangle(int h) {
    int row;
    int col;
    row = h;
    while (row >= 1) {
        col = 0;
        while (col < row) {
            printf("%c", '*');
            col = col + 1;
        }
        printf("\n");
        row = row - 1;
    }
}

// Number triangle: row i prints 1..i
void number_triangle(int h) {
    int row;
    int col;
    row = 1;
    while (row <= h) {
        col = 1;
        while (col <= row) {
            printf("%d", col);
            col = col + 1;
        }
        printf("\n");
        row = row + 1;
    }
}

// Pyramid of stars with height h
void pyramid(int h) {
    int row;
    int col;
    int stars;
    row = 0;
    while (row < h) {
        // print leading spaces
        col = 0;
        while (col < h - 1 - row) {
            printf(" ");
            col = col + 1;
        }
        // print stars
        stars = 2 * row + 1;
        col = 0;
        while (col < stars) {
            printf("%c", '*');
            col = col + 1;
        }
        printf("\n");
        row = row + 1;
    }
}

// Diamond pattern: top half is pyramid, bottom half is inverted
void diamond(int h) {
    int row;
    int col;
    int stars;
    // top half (including middle)
    row = 0;
    while (row < h) {
        col = 0;
        while (col < h - 1 - row) {
            printf(" ");
            col = col + 1;
        }
        stars = 2 * row + 1;
        col = 0;
        while (col < stars) {
            printf("%c", '*');
            col = col + 1;
        }
        printf("\n");
        row = row + 1;
    }
    // bottom half
    row = h - 2;
    while (row >= 0) {
        col = 0;
        while (col < h - 1 - row) {
            printf(" ");
            col = col + 1;
        }
        stars = 2 * row + 1;
        col = 0;
        while (col < stars) {
            printf("%c", '*');
            col = col + 1;
        }
        printf("\n");
        row = row - 1;
    }
}

// Pascal triangle: compute row into array, return row
int g_pascal_prev[20];
int g_pascal_curr[20];

void pascal_row(int n) {
    int i;
    int j;
    // row 0
    g_pascal_prev[0] = 1;
    i = 1;
    while (i < 20) {
        g_pascal_prev[i] = 0;
        i = i + 1;
    }

    if (n == 0) {
        g_pascal_curr[0] = 1;
        i = 1;
        while (i < 20) {
            g_pascal_curr[i] = 0;
            i = i + 1;
        }
        return;
    }

    i = 1;
    while (i <= n) {
        g_pascal_curr[0] = 1;
        j = 1;
        while (j <= i) {
            g_pascal_curr[j] = g_pascal_prev[j - 1] + g_pascal_prev[j];
            j = j + 1;
        }
        j = i + 1;
        while (j < 20) {
            g_pascal_curr[j] = 0;
            j = j + 1;
        }
        // copy curr to prev
        j = 0;
        while (j < 20) {
            g_pascal_prev[j] = g_pascal_curr[j];
            j = j + 1;
        }
        i = i + 1;
    }
}

void print_pascal_row(int n) {
    int j;
    pascal_row(n);
    printf("=== Pascal Triangle Row %d ===\n", n);
    j = 0;
    while (j <= n) {
        if (j > 0) {
            printf(" ");
        }
        printf("%d", g_pascal_curr[j]);
        j = j + 1;
    }
    printf("\n");
}

// Multiplication table from 1..n
void mult_table(int n) {
    int r;
    int c;
    int val;
    r = 1;
    while (r <= n) {
        c = 1;
        while (c <= n) {
            val = r * c;
            if (val < 10) {
                printf(" ");
            }
            printf(" %d", val);
            c = c + 1;
        }
        printf("\n");
        r = r + 1;
    }
}

// Number square pattern
void number_square(int n) {
    int r;
    int c;
    r = 1;
    while (r <= n) {
        c = 0;
        while (c < n) {
            if (c > 0) {
                printf(" ");
            }
            printf("%d", r + c);
            c = c + 1;
        }
        printf("\n");
        r = r + 1;
    }
}

// Checkerboard pattern
void checkerboard(int n) {
    int r;
    int c;
    r = 0;
    while (r < n) {
        c = 0;
        while (c < n) {
            if (c > 0) {
                printf(" ");
            }
            if ((r + c) % 2 == 0) {
                printf("%d", 1);
            } else {
                printf("%d", 0);
            }
            c = c + 1;
        }
        printf("\n");
        r = r + 1;
    }
}

int count_checkerboard_ones(int n) {
    int r;
    int c;
    int count;
    count = 0;
    r = 0;
    while (r < n) {
        c = 0;
        while (c < n) {
            if ((r + c) % 2 == 0) {
                count = count + 1;
            }
            c = c + 1;
        }
        r = r + 1;
    }
    return count;
}

// Floyd's triangle
void floyd_triangle(int rows) {
    int num;
    int r;
    int c;
    num = 1;
    r = 1;
    while (r <= rows) {
        c = 1;
        while (c <= r) {
            if (c > 1) {
                printf(" ");
            }
            printf("%d", num);
            num = num + 1;
            c = c + 1;
        }
        printf("\n");
        r = r + 1;
    }
}

int floyd_last_row_sum(int rows) {
    int num;
    int r;
    int c;
    int sum;
    num = 1;
    r = 1;
    sum = 0;
    while (r <= rows) {
        c = 1;
        while (c <= r) {
            if (r == rows) {
                sum = sum + num;
            }
            num = num + 1;
            c = c + 1;
        }
        r = r + 1;
    }
    return sum;
}

// Zigzag row pattern: even rows left-to-right, odd rows right-to-left
void zigzag_rows(int rows, int cols) {
    int r;
    int c;
    int num;
    r = 0;
    while (r < rows) {
        printf("row%d:", r);
        c = 0;
        while (c < cols) {
            if (r % 2 == 0) {
                num = r * cols + c + 1;
            } else {
                num = (r + 1) * cols - c;
            }
            printf(" %d", num);
            c = c + 1;
        }
        printf("\n");
        r = r + 1;
    }
}

// Spiral fill of NxN matrix
int g_spiral[16]; // 4x4

void spiral_fill(int n) {
    int top;
    int bottom;
    int left;
    int right;
    int num;
    int i;

    // Clear
    i = 0;
    while (i < n * n) {
        g_spiral[i] = 0;
        i = i + 1;
    }

    top = 0;
    bottom = n - 1;
    left = 0;
    right = n - 1;
    num = 1;

    while (num <= n * n) {
        // Fill top row
        i = left;
        while (i <= right) {
            g_spiral[top * n + i] = num;
            num = num + 1;
            i = i + 1;
        }
        top = top + 1;

        // Fill right column
        i = top;
        while (i <= bottom) {
            g_spiral[i * n + right] = num;
            num = num + 1;
            i = i + 1;
        }
        right = right - 1;

        // Fill bottom row
        i = right;
        while (i >= left) {
            g_spiral[bottom * n + i] = num;
            num = num + 1;
            i = i - 1;
        }
        bottom = bottom - 1;

        // Fill left column
        i = bottom;
        while (i >= top) {
            g_spiral[i * n + left] = num;
            num = num + 1;
            i = i - 1;
        }
        left = left + 1;
    }
}

void print_spiral(int n) {
    int r;
    int c;
    r = 0;
    while (r < n) {
        c = 0;
        while (c < n) {
            if (c > 0) {
                printf(" ");
            }
            printf("%d", g_spiral[r * n + c]);
            c = c + 1;
        }
        printf("\n");
        r = r + 1;
    }
}

int spiral_sum(int n) {
    int s;
    int i;
    s = 0;
    i = 0;
    while (i < n * n) {
        s = s + g_spiral[i];
        i = i + 1;
    }
    return s;
}

// Hourglass pattern
void hourglass(int h) {
    int row;
    int col;
    int stars;
    // top half (wide to narrow)
    row = 0;
    while (row < h) {
        col = 0;
        while (col < row) {
            printf(" ");
            col = col + 1;
        }
        stars = 2 * (h - 1 - row) + 1;
        col = 0;
        while (col < stars) {
            printf("%c", '*');
            col = col + 1;
        }
        printf("\n");
        row = row + 1;
    }
    // bottom half (narrow to wide)
    row = h - 2;
    while (row >= 0) {
        col = 0;
        while (col < row) {
            printf(" ");
            col = col + 1;
        }
        stars = 2 * (h - 1 - row) + 1;
        col = 0;
        while (col < stars) {
            printf("%c", '*');
            col = col + 1;
        }
        printf("\n");
        row = row - 1;
    }
}

// Right aligned numbers
void right_aligned_numbers(int h) {
    int row;
    int col;
    row = 1;
    while (row <= h) {
        // leading spaces
        col = 0;
        while (col < h - row) {
            printf(" ");
            col = col + 1;
        }
        col = 1;
        while (col <= row) {
            printf("%d", col);
            col = col + 1;
        }
        printf("\n");
        row = row + 1;
    }
}

// Hollow square
void hollow_square(int n) {
    int r;
    int c;
    r = 0;
    while (r < n) {
        c = 0;
        while (c < n) {
            if (r == 0 || r == n - 1 || c == 0 || c == n - 1) {
                printf("%c", '*');
            } else {
                printf(" ");
            }
            c = c + 1;
        }
        printf("\n");
        r = r + 1;
    }
}

// Border number grid: 1 on border, 0 inside
void border_grid(int n) {
    int r;
    int c;
    r = 0;
    while (r < n) {
        c = 0;
        while (c < n) {
            if (c > 0) {
                printf(" ");
            }
            if (r == 0 || r == n - 1 || c == 0 || c == n - 1) {
                printf("%d", 1);
            } else {
                printf("%d", 0);
            }
            c = c + 1;
        }
        printf("\n");
        r = r + 1;
    }
}

int main() {
    int row_sum;
    int j;
    int n;

    printf("=== Right Triangle ===\n");
    right_triangle(5);

    printf("=== Inverted Triangle ===\n");
    inverted_triangle(5);

    printf("=== Number Triangle ===\n");
    number_triangle(5);

    printf("=== Pyramid ===\n");
    pyramid(5);

    printf("=== Diamond ===\n");
    diamond(5);

    // Pascal's triangle rows 0 through 7
    n = 0;
    while (n <= 7) {
        print_pascal_row(n);
        n = n + 1;
    }

    printf("=== Multiplication Table 1-5 ===\n");
    mult_table(5);

    printf("=== Number Square ===\n");
    number_square(5);

    printf("=== Checkerboard 5x5 ===\n");
    checkerboard(5);

    printf("=== Floyd Triangle ===\n");
    floyd_triangle(5);

    printf("=== Zigzag Row Pattern ===\n");
    zigzag_rows(4, 5);

    printf("=== Spiral Fill 4x4 ===\n");
    spiral_fill(4);
    print_spiral(4);

    printf("=== Pattern Validation ===\n");
    // Pascal row sums should be powers of 2
    n = 0;
    while (n <= 5) {
        pascal_row(n);
        row_sum = 0;
        j = 0;
        while (j <= n) {
            row_sum = row_sum + g_pascal_curr[j];
            j = j + 1;
        }
        printf("pascal_sum_row%d=%d\n", n, row_sum);
        n = n + 1;
    }

    // Multiplication table diagonal (perfect squares)
    printf("mult_table_diag:");
    n = 1;
    while (n <= 5) {
        printf(" %d", n * n);
        n = n + 1;
    }
    printf("\n");

    // Floyd last row sum: row 5 has 11+12+13+14+15=65
    printf("floyd_last_row_sum=%d\n", floyd_last_row_sum(5));

    // Spiral sum for 4x4: 1+2+...+16 = 136
    printf("spiral_sum=%d\n", spiral_sum(4));

    // Checkerboard ones count in 5x5: 13
    printf("checkerboard_ones=%d\n", count_checkerboard_ones(5));

    printf("=== Hourglass ===\n");
    hourglass(3);

    printf("=== Right Aligned Numbers ===\n");
    right_aligned_numbers(5);

    printf("=== Hollow Square 5x5 ===\n");
    hollow_square(5);

    printf("=== Border Number Grid 5x5 ===\n");
    border_grid(5);

    printf("=== All patterns done ===\n");

    return 0;
}
