int printf(const char *fmt, ...);
// EXPECT: All subsets of {1,2,3}:\n{}\n{1}\n{2}\n{1,2}\n{3}\n{1,3}\n{2,3}\n{1,2,3}\nTotal: 8\n\nAll subsets of {1,2,3,4}:\n{}\n{1}\n{2}\n{1,2}\n{3}\n{1,3}\n{2,3}\n{1,2,3}\n{4}\n{1,4}\n{2,4}\n{1,2,4}\n{3,4}\n{1,3,4}\n{2,3,4}\n{1,2,3,4}\nTotal: 16\n\nSubsets of {1,2,3,4} of size 2:\n{1,2}\n{1,3}\n{2,3}\n{1,4}\n{2,4}\n{3,4}\nCount: 6\n\nSubsets of {3,7,1,8,5} summing to 11:\n{3,7,1} = 11\n{3,8} = 11\nCount: 2\n\nPower set sizes:\n|P({1..0})| = 2^0 = 1\n|P({1..1})| = 2^1 = 2\n|P({1..2})| = 2^2 = 4\n|P({1..3})| = 2^3 = 8\n|P({1..4})| = 2^4 = 16\n|P({1..5})| = 2^5 = 32\n|P({1..6})| = 2^6 = 64\n|P({1..7})| = 2^7 = 128\n|P({1..8})| = 2^8 = 256
// Test: Subset generation

void print_subset(int set[], int n, int mask) {
    printf("{");
    int first = 1;
    int i;
    int bit = 1;
    for (i = 0; i < n; i++) {
        if (mask / bit % 2 == 1) {
            if (!first) printf(",");
            printf("%d", set[i]);
            first = 0;
        }
        bit = bit * 2;
    }
    printf("}");
}

int main(void) {
    int i, j;

    // Generate all subsets of {1,2,3}
    int set3[3];
    set3[0] = 1; set3[1] = 2; set3[2] = 3;
    int n = 3;
    int total = 1;
    for (i = 0; i < n; i++) total = total * 2;

    printf("All subsets of {1,2,3}:\n");
    for (i = 0; i < total; i++) {
        print_subset(set3, n, i);
        printf("\n");
    }
    printf("Total: %d\n\n", total);

    // Generate all subsets of {1,2,3,4}
    int set4[4];
    set4[0] = 1; set4[1] = 2; set4[2] = 3; set4[3] = 4;
    n = 4;
    total = 16;

    printf("All subsets of {1,2,3,4}:\n");
    for (i = 0; i < total; i++) {
        print_subset(set4, n, i);
        printf("\n");
    }
    printf("Total: %d\n\n", total);

    // Subsets of size k
    printf("Subsets of {1,2,3,4} of size 2:\n");
    int count = 0;
    for (i = 0; i < 16; i++) {
        // Count bits
        int bits = 0;
        int temp = i;
        while (temp > 0) {
            bits = bits + (temp % 2);
            temp = temp / 2;
        }
        if (bits == 2) {
            print_subset(set4, 4, i);
            printf("\n");
            count++;
        }
    }
    printf("Count: %d\n\n", count);

    // Subset sum problem: find subsets of {3,7,1,8,5} that sum to 11
    int set5[5];
    set5[0] = 3; set5[1] = 7; set5[2] = 1; set5[3] = 8; set5[4] = 5;
    int target = 11;
    total = 32; // 2^5

    printf("Subsets of {3,7,1,8,5} summing to %d:\n", target);
    count = 0;
    for (i = 0; i < total; i++) {
        int sum = 0;
        int bit = 1;
        for (j = 0; j < 5; j++) {
            if (i / bit % 2 == 1) {
                sum = sum + set5[j];
            }
            bit = bit * 2;
        }
        if (sum == target) {
            print_subset(set5, 5, i);
            printf(" = %d\n", sum);
            count++;
        }
    }
    printf("Count: %d\n\n", count);

    // Power set cardinality for different n
    printf("Power set sizes:\n");
    for (i = 0; i <= 8; i++) {
        int p = 1;
        for (j = 0; j < i; j++) p = p * 2;
        printf("|P({1..%d})| = 2^%d = %d\n", i, i, p);
    }

    return 0;
}
