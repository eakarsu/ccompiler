int printf(const char *fmt, ...);
// EXPECT: 3D coordinates:\n(0,1,2) (0,2,1) (1,0,2) (1,1,1) (1,2,0) (2,0,1) (2,1,0) \ncount=7\nMultiplication cube:\n1 2 3 4 | 2 4 6 8 | 3 6 9 12 | \n2 4 6 8 | 4 8 12 16 | 6 12 18 24 | \n3D array fill:\nsum=300\nDiagonal elements: 1 18 \nLayer sums: 78 222 
int main(void) {
    int i;
    int j;
    int k;
    int count;

    printf("3D coordinates:\n");
    count = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 3; k++) {
                if (i + j + k == 3) {
                    printf("(%d,%d,%d) ", i, j, k);
                    count++;
                }
            }
        }
    }
    printf("\ncount=%d\n", count);

    printf("Multiplication cube:\n");
    for (i = 1; i <= 2; i++) {
        for (j = 1; j <= 3; j++) {
            for (k = 1; k <= 4; k++) {
                printf("%d ", i * j * k);
            }
            printf("| ");
        }
        printf("\n");
    }

    printf("3D array fill:\n");
    int arr[2][3][4];
    int val = 1;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 4; k++) {
                arr[i][j][k] = val;
                val++;
            }
        }
    }
    int sum = 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 4; k++) {
                sum = sum + arr[i][j][k];
            }
        }
    }
    printf("sum=%d\n", sum);

    printf("Diagonal elements: ");
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 4; k++) {
                if (i == j && j == k) {
                    printf("%d ", arr[i][j][k]);
                }
            }
        }
    }
    printf("\n");

    printf("Layer sums: ");
    for (i = 0; i < 2; i++) {
        int lsum = 0;
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 4; k++) {
                lsum = lsum + arr[i][j][k];
            }
        }
        printf("%d ", lsum);
    }
    printf("\n");

    return 0;
}
