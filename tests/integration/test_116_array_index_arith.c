int printf(const char *fmt, ...);
// EXPECT: arr[3] = 9\narr[3+2] = 25\narr[3*2] = 36\narr[20-1] = 361\narr[i+j] = 36\narr[i*j] = 64\narr[i-j] = 4\narr[i/j] = 4\narr[1] = 1\narr[5] = 25\narr[10] = 100\narr[15] = 225\narr[19] = 361\n 1  2  3  4 \n 5  6  7  8 \n 9 10 11 12 \n13 14 15 16 \ndiagonal sum = 34\nanti-diagonal = 34\nrow 2 sum = 42
// Test: arithmetic in array indices

int main(void) {
    int arr[20];
    int matrix[4][4];
    int i, j;
    int idx;
    int sum;

    /* Fill array with computed indices */
    for (i = 0; i < 20; i++) {
        arr[i] = i * i;
    }

    /* Access with computed index */
    idx = 3;
    printf("arr[3] = %d\n", arr[idx]);
    printf("arr[3+2] = %d\n", arr[idx + 2]);
    printf("arr[3*2] = %d\n", arr[idx * 2]);
    printf("arr[20-1] = %d\n", arr[20 - 1]);

    /* Expression as index */
    i = 4;
    j = 2;
    printf("arr[i+j] = %d\n", arr[i + j]);
    printf("arr[i*j] = %d\n", arr[i * j]);
    printf("arr[i-j] = %d\n", arr[i - j]);
    printf("arr[i/j] = %d\n", arr[i / j]);

    /* Index from another array */
    {
        int indices[5];
        indices[0] = 1;
        indices[1] = 5;
        indices[2] = 10;
        indices[3] = 15;
        indices[4] = 19;
        for (i = 0; i < 5; i++) {
            printf("arr[%d] = %d\n", indices[i], arr[indices[i]]);
        }
    }

    /* Fill 2D array with arithmetic index */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            matrix[i][j] = i * 4 + j + 1;
        }
    }

    /* Print matrix */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }

    /* Diagonal sum using index arithmetic */
    sum = 0;
    for (i = 0; i < 4; i++) {
        sum += matrix[i][i];
    }
    printf("diagonal sum = %d\n", sum);

    /* Anti-diagonal */
    sum = 0;
    for (i = 0; i < 4; i++) {
        sum += matrix[i][3 - i];
    }
    printf("anti-diagonal = %d\n", sum);

    /* Row sum using index */
    sum = 0;
    for (j = 0; j < 4; j++) {
        sum += matrix[2][j];
    }
    printf("row 2 sum = %d\n", sum);

    return 0;
}
