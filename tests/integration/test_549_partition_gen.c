int printf(const char *fmt, ...);
// EXPECT: Partitions of 5:\n  5\n  4+1\n  3+2\n  3+1+1\n  2+2+1\n  2+1+1+1\n  1+1+1+1+1\nCount: 7\n\nPartitions of 6:\n  6\n  5+1\n  4+2\n  4+1+1\n  3+3\n  3+2+1\n  3+1+1+1\n  2+2+2\n  2+2+1+1\n  2+1+1+1+1\n  1+1+1+1+1+1\nCount: 11\n\nPartitions of 7: 15\n\nPartitions of 7 into distinct parts:\n  7\n  6+1\n  5+2\n  4+3\n  4+2+1\n\nPartitions of 7 into odd parts:\n  7\n  5+1+1\n  3+3+1\n  3+1+1+1+1\n  1+1+1+1+1+1+1\n\nPartition counts:\np(1) = 1\np(2) = 2\np(3) = 3\np(4) = 5\np(5) = 7\np(6) = 11\np(7) = 15\np(8) = 22\np(9) = 30\np(10) = 42
// Test: Integer partition generation

int partitions[200][20]; // store partitions
int part_lens[200];
int num_partitions;

void gen_partitions(int n, int max_part, int parts[], int nparts) {
    if (n == 0) {
        int i;
        for (i = 0; i < nparts; i++) {
            partitions[num_partitions][i] = parts[i];
        }
        part_lens[num_partitions] = nparts;
        num_partitions++;
        return;
    }
    int i;
    int limit = max_part;
    if (limit > n) limit = n;
    for (i = limit; i >= 1; i--) {
        parts[nparts] = i;
        gen_partitions(n - i, i, parts, nparts + 1);
    }
}

int main(void) {
    int i, j;

    // Generate partitions of 5
    int parts[20];
    num_partitions = 0;
    gen_partitions(5, 5, parts, 0);

    printf("Partitions of 5:\n");
    for (i = 0; i < num_partitions; i++) {
        printf("  ");
        for (j = 0; j < part_lens[i]; j++) {
            if (j > 0) printf("+");
            printf("%d", partitions[i][j]);
        }
        printf("\n");
    }
    printf("Count: %d\n\n", num_partitions);

    // Generate partitions of 6
    num_partitions = 0;
    gen_partitions(6, 6, parts, 0);

    printf("Partitions of 6:\n");
    for (i = 0; i < num_partitions; i++) {
        printf("  ");
        for (j = 0; j < part_lens[i]; j++) {
            if (j > 0) printf("+");
            printf("%d", partitions[i][j]);
        }
        printf("\n");
    }
    printf("Count: %d\n\n", num_partitions);

    // Generate partitions of 7
    num_partitions = 0;
    gen_partitions(7, 7, parts, 0);
    printf("Partitions of 7: %d\n\n", num_partitions);

    // Partitions into distinct parts of 7
    printf("Partitions of 7 into distinct parts:\n");
    for (i = 0; i < num_partitions; i++) {
        int distinct = 1;
        for (j = 0; j < part_lens[i] - 1; j++) {
            if (partitions[i][j] == partitions[i][j + 1]) {
                distinct = 0;
                break;
            }
        }
        if (distinct) {
            printf("  ");
            for (j = 0; j < part_lens[i]; j++) {
                if (j > 0) printf("+");
                printf("%d", partitions[i][j]);
            }
            printf("\n");
        }
    }

    // Partitions into odd parts of 7
    printf("\nPartitions of 7 into odd parts:\n");
    for (i = 0; i < num_partitions; i++) {
        int all_odd = 1;
        for (j = 0; j < part_lens[i]; j++) {
            if (partitions[i][j] % 2 == 0) {
                all_odd = 0;
                break;
            }
        }
        if (all_odd) {
            printf("  ");
            for (j = 0; j < part_lens[i]; j++) {
                if (j > 0) printf("+");
                printf("%d", partitions[i][j]);
            }
            printf("\n");
        }
    }

    // Partition counts for n = 1..10
    printf("\nPartition counts:\n");
    for (i = 1; i <= 10; i++) {
        num_partitions = 0;
        gen_partitions(i, i, parts, 0);
        printf("p(%d) = %d\n", i, num_partitions);
    }

    return 0;
}
