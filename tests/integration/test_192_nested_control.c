int printf(const char *fmt, ...);
// EXPECT: For-while-if pyramid:\n* \n* * \n* . * \n* . . * \n* . . . * \nMatrix conditional processing:\nupper triangle mod sum=45\nSwitch in for in while:\nsteps=8 state=3\nNested break/continue:\nresult=33\nDo-while in for:\n10 has 2 digits\n20 has 2 digits\n30 has 2 digits\n40 has 2 digits\nTriple nested condition:\npermutations summing to 6: 18
int main(void) {
    int i;
    int j;
    int k;

    printf("For-while-if pyramid:\n");
    for (i = 1; i <= 5; i++) {
        j = 0;
        while (j < i) {
            if (j == 0 || j == i - 1) {
                printf("* ");
            } else {
                printf(". ");
            }
            j++;
        }
        printf("\n");
    }

    printf("Matrix conditional processing:\n");
    int mat[4][4];
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            mat[i][j] = (i + 1) * (j + 1);
        }
    }
    int total = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i <= j) {
                k = mat[i][j];
                while (k > 10) {
                    k = k - 10;
                }
                total = total + k;
            }
        }
    }
    printf("upper triangle mod sum=%d\n", total);

    printf("Switch in for in while:\n");
    int state = 0;
    int steps = 0;
    while (state < 3) {
        for (i = 0; i < 4; i++) {
            switch (state) {
                case 0:
                    if (i == 2) state = 1;
                    break;
                case 1:
                    if (i == 1) state = 2;
                    break;
                case 2:
                    if (i == 3) state = 3;
                    break;
            }
            steps++;
        }
    }
    printf("steps=%d state=%d\n", steps, state);

    printf("Nested break/continue:\n");
    int result = 0;
    for (i = 0; i < 10; i++) {
        if (i % 3 == 0) continue;
        for (j = 0; j < 10; j++) {
            if (j % 4 == 0) continue;
            if (i + j > 12) break;
            result++;
        }
    }
    printf("result=%d\n", result);

    printf("Do-while in for:\n");
    for (i = 1; i <= 4; i++) {
        int n = i * 10;
        int digits = 0;
        do {
            digits++;
            n = n / 10;
        } while (n > 0);
        printf("%d has %d digits\n", i * 10, digits);
    }

    printf("Triple nested condition:\n");
    int count = 0;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            for (k = 0; k < 6; k++) {
                if (i != j && j != k && i != k) {
                    if (i + j + k == 6) {
                        count++;
                    }
                }
            }
        }
    }
    printf("permutations summing to 6: %d\n", count);

    return 0;
}
