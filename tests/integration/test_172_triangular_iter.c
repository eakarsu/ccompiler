int printf(const char *fmt, ...);
// EXPECT: Upper triangle:\n0 1 2 3 4 \n2 3 4 5 \n4 5 6 \n6 7 \n8 \nLower triangle:\n0 \n0 1 \n0 2 4 \n0 3 6 9 \n0 4 8 12 16 \nPascal's triangle:\n1 \n1 1 \n1 2 1 \n1 3 3 1 \n1 4 6 4 1 \n1 5 10 10 5 1 \n1 6 15 20 15 6 1 \n1 7 21 35 35 21 7 1 \nTriangular sums:\nT(1)=1\nT(2)=3\nT(3)=6\nT(4)=10\nT(5)=15\nT(6)=21\nT(7)=28\nT(8)=36\nFloyd's triangle:\n1 \n2 3 \n4 5 6 \n7 8 9 10 \n11 12 13 14 15 \nTriangular pairs sum:\ntotal=75
int main(void) {
    int i;
    int j;

    printf("Upper triangle:\n");
    for (i = 0; i < 5; i++) {
        for (j = i; j < 5; j++) {
            printf("%d ", i + j);
        }
        printf("\n");
    }

    printf("Lower triangle:\n");
    for (i = 0; i < 5; i++) {
        for (j = 0; j <= i; j++) {
            printf("%d ", i * j);
        }
        printf("\n");
    }

    printf("Pascal's triangle:\n");
    int pascal[8][8];
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            pascal[i][j] = 0;
        }
    }
    for (i = 0; i < 8; i++) {
        pascal[i][0] = 1;
        for (j = 1; j <= i; j++) {
            pascal[i][j] = pascal[i - 1][j - 1] + pascal[i - 1][j];
        }
        for (j = 0; j <= i; j++) {
            printf("%d ", pascal[i][j]);
        }
        printf("\n");
    }

    printf("Triangular sums:\n");
    for (i = 1; i <= 8; i++) {
        int sum = 0;
        for (j = 1; j <= i; j++) {
            sum = sum + j;
        }
        printf("T(%d)=%d\n", i, sum);
    }

    printf("Floyd's triangle:\n");
    int num = 1;
    for (i = 1; i <= 5; i++) {
        for (j = 1; j <= i; j++) {
            printf("%d ", num);
            num++;
        }
        printf("\n");
    }

    printf("Triangular pairs sum:\n");
    int total = 0;
    for (i = 0; i < 6; i++) {
        for (j = i + 1; j < 6; j++) {
            total = total + i + j;
        }
    }
    printf("total=%d\n", total);

    return 0;
}
