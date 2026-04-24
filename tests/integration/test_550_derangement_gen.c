int printf(const char *fmt, ...);
// EXPECT: Derangements of {1,2,3}:\n  [2,3,1]\n  [3,1,2]\nD(3) = 2\n\nDerangements of {1,2,3,4}:\n  [2,1,4,3]\n  [2,3,4,1]\n  [2,4,1,3]\n  [3,1,4,2]\n  [3,4,1,2]\n  [3,4,2,1]\n  [4,3,2,1]\n  [4,3,1,2]\n  [4,1,2,3]\nD(4) = 9\n\nD(5) = 44\nFirst 10 derangements of {1,2,3,4,5}:\n  [2,1,4,5,3]\n  [2,1,5,3,4]\n  [2,3,1,5,4]\n  [2,3,4,5,1]\n  [2,3,5,1,4]\n  [2,4,1,5,3]\n  [2,4,5,1,3]\n  [2,4,5,3,1]\n  [2,5,4,3,1]\n  [2,5,4,1,3]\n\nDerangement counts and formula:\nD(0) = 1\nD(1) = 0\nD(2) = 1\nD(3) = 2\nD(4) = 9\nD(5) = 44\nD(6) = 265\nD(7) = 1854\n\nCycle structure of derangements of {1,2,3,4}:\n[2,1,4,3] -> 2 cycles\n[2,3,4,1] -> 1 cycles\n[2,4,1,3] -> 1 cycles\n[3,1,4,2] -> 1 cycles\n[3,4,1,2] -> 2 cycles\n[3,4,2,1] -> 1 cycles\n[4,3,2,1] -> 2 cycles\n[4,3,1,2] -> 1 cycles\n[4,1,2,3] -> 1 cycles

int derangements[100][8];
int num_derangements;

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void gen_derangements(int arr[], int n, int pos) {
    if (pos == n) {
        // Check if it's a derangement
        int i;
        for (i = 0; i < n; i++) {
            if (arr[i] == i + 1) return;
        }
        for (i = 0; i < n; i++) {
            derangements[num_derangements][i] = arr[i];
        }
        num_derangements++;
        return;
    }
    int i;
    for (i = pos; i < n; i++) {
        swap(&arr[pos], &arr[i]);
        gen_derangements(arr, n, pos + 1);
        swap(&arr[pos], &arr[i]);
    }
}

void print_perm(int arr[], int n) {
    int i;
    printf("[");
    for (i = 0; i < n; i++) {
        if (i > 0) printf(",");
        printf("%d", arr[i]);
    }
    printf("]");
}

int main(void) {
    int i, j;

    // Generate derangements of {1,2,3}
    int arr3[3];
    arr3[0] = 1; arr3[1] = 2; arr3[2] = 3;
    num_derangements = 0;
    gen_derangements(arr3, 3, 0);

    printf("Derangements of {1,2,3}:\n");
    for (i = 0; i < num_derangements; i++) {
        printf("  ");
        print_perm(derangements[i], 3);
        printf("\n");
    }
    printf("D(3) = %d\n\n", num_derangements);

    // Generate derangements of {1,2,3,4}
    int arr4[4];
    arr4[0] = 1; arr4[1] = 2; arr4[2] = 3; arr4[3] = 4;
    num_derangements = 0;
    gen_derangements(arr4, 4, 0);

    printf("Derangements of {1,2,3,4}:\n");
    for (i = 0; i < num_derangements; i++) {
        printf("  ");
        print_perm(derangements[i], 4);
        printf("\n");
    }
    printf("D(4) = %d\n\n", num_derangements);

    // Generate derangements of {1,2,3,4,5}
    int arr5[5];
    arr5[0] = 1; arr5[1] = 2; arr5[2] = 3; arr5[3] = 4; arr5[4] = 5;
    num_derangements = 0;
    gen_derangements(arr5, 5, 0);
    printf("D(5) = %d\n", num_derangements);

    // Print first 10 derangements of {1,2,3,4,5}
    printf("First 10 derangements of {1,2,3,4,5}:\n");
    int limit = num_derangements;
    if (limit > 10) limit = 10;
    for (i = 0; i < limit; i++) {
        printf("  ");
        print_perm(derangements[i], 5);
        printf("\n");
    }

    // Verify D(n) = (n-1)*(D(n-1) + D(n-2))
    printf("\nDerangement counts and formula:\n");
    int D[8];
    D[0] = 1;
    D[1] = 0;
    D[2] = 1;
    for (i = 3; i <= 7; i++) {
        D[i] = (i - 1) * (D[i - 1] + D[i - 2]);
    }
    for (i = 0; i <= 7; i++) {
        printf("D(%d) = %d\n", i, D[i]);
    }

    // Fixed point analysis of derangements of {1,2,3,4}
    printf("\nCycle structure of derangements of {1,2,3,4}:\n");
    arr4[0] = 1; arr4[1] = 2; arr4[2] = 3; arr4[3] = 4;
    num_derangements = 0;
    gen_derangements(arr4, 4, 0);

    for (i = 0; i < num_derangements; i++) {
        print_perm(derangements[i], 4);
        // Count cycles
        int visited[4];
        for (j = 0; j < 4; j++) visited[j] = 0;
        int cycles = 0;
        for (j = 0; j < 4; j++) {
            if (!visited[j]) {
                cycles++;
                int k = j;
                while (!visited[k]) {
                    visited[k] = 1;
                    k = derangements[i][k] - 1;
                }
            }
        }
        printf(" -> %d cycles\n", cycles);
    }

    return 0;
}
