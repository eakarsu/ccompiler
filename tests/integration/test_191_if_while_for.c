int printf(const char *fmt, ...);
// EXPECT: Conditional loops:\nArray: 3 10 4 11 5 12 6 0 7 1 \nEven indices sum: 25\nConditional accumulation per row:\nrow 0: sum of multiples of 3 = 3\nrow 1: sum of multiples of 3 = 15\nrow 2: sum of multiples of 3 = 27\nrow 3: sum of multiples of 3 = 18\nWhile with if-break-continue:\n7 14 21 28 35 \nfound=5\nNested search with conditions:\n2 3 5 7 \nPattern with all three:\nO \nE O \nO E O \nE O E O \nO E O E O 
int main(void) {
    int i;
    int j;

    printf("Conditional loops:\n");
    int arr[10];
    for (i = 0; i < 10; i++) {
        arr[i] = (i * 7 + 3) % 13;
    }
    printf("Array: ");
    for (i = 0; i < 10; i++) printf("%d ", arr[i]);
    printf("\n");

    printf("Even indices sum: ");
    int sum = 0;
    for (i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            j = 0;
            while (j < arr[i]) {
                sum = sum + 1;
                j++;
            }
        }
    }
    printf("%d\n", sum);

    printf("Conditional accumulation per row:\n");
    int grid[4][5];
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 5; j++) {
            grid[i][j] = i * 5 + j + 1;
        }
    }
    for (i = 0; i < 4; i++) {
        int row_sum = 0;
        for (j = 0; j < 5; j++) {
            if (grid[i][j] % 3 == 0) {
                row_sum = row_sum + grid[i][j];
            }
        }
        if (row_sum > 0) {
            printf("row %d: sum of multiples of 3 = %d\n", i, row_sum);
        }
    }

    printf("While with if-break-continue:\n");
    i = 0;
    int found_count = 0;
    while (i < 50) {
        i++;
        if (i % 7 != 0) continue;
        if (i > 40) break;
        printf("%d ", i);
        found_count++;
    }
    printf("\nfound=%d\n", found_count);

    printf("Nested search with conditions:\n");
    for (i = 2; i <= 10; i++) {
        int is_prime = 1;
        if (i > 2 && i % 2 == 0) {
            is_prime = 0;
        } else {
            j = 3;
            while (j * j <= i) {
                if (i % j == 0) {
                    is_prime = 0;
                    break;
                }
                j = j + 2;
            }
        }
        if (is_prime) {
            printf("%d ", i);
        }
    }
    printf("\n");

    printf("Pattern with all three:\n");
    for (i = 1; i <= 5; i++) {
        j = i;
        while (j > 0) {
            if (j % 2 == 0) {
                printf("E ");
            } else {
                printf("O ");
            }
            j--;
        }
        printf("\n");
    }

    return 0;
}
