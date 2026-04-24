int printf(const char *fmt, ...);

// Integer partition function

int partition_table[120];

int partition_dp(int n) {
    int p[120];
    int i;
    int j;
    p[0] = 1;
    for (i = 1; i <= n; i++) {
        p[i] = 0;
    }
    for (i = 1; i <= n; i++) {
        for (j = i; j <= n; j++) {
            p[j] = p[j] + p[j - i];
        }
    }
    return p[n];
}

int partition_into_distinct(int n) {
    int p[120];
    int i;
    int j;
    p[0] = 1;
    for (i = 1; i <= n; i++) {
        p[i] = 0;
    }
    for (i = 1; i <= n; i++) {
        for (j = n; j >= i; j--) {
            p[j] = p[j] + p[j - i];
        }
    }
    return p[n];
}

int partition_into_odd(int n) {
    int p[120];
    int i;
    int j;
    p[0] = 1;
    for (i = 1; i <= n; i++) {
        p[i] = 0;
    }
    for (i = 1; i <= n; i = i + 2) {
        for (j = i; j <= n; j++) {
            p[j] = p[j] + p[j - i];
        }
    }
    return p[n];
}

int partition_at_most_k_parts(int n, int k) {
    int p[60][60];
    int i;
    int j;
    for (i = 0; i <= n; i++) {
        for (j = 0; j <= k; j++) {
            p[i][j] = 0;
        }
    }
    for (j = 0; j <= k; j++) {
        p[0][j] = 1;
    }
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= k; j++) {
            p[i][j] = p[i][j - 1];
            if (i >= j) {
                p[i][j] = p[i][j] + p[i - j][j];
            }
        }
    }
    return p[n][k];
}

int partition_exactly_k_parts(int n, int k) {
    int p[60][60];
    int i;
    int j;
    if (k > n) return 0;
    for (i = 0; i <= n; i++) {
        for (j = 0; j <= k; j++) {
            p[i][j] = 0;
        }
    }
    p[0][0] = 1;
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= k; j++) {
            if (i >= j) {
                p[i][j] = p[i - 1][j - 1] + p[i - j][j];
            }
        }
    }
    return p[n][k];
}

int largest_part_at_most(int n, int maxpart) {
    int p[120];
    int i;
    int j;
    p[0] = 1;
    for (i = 1; i <= n; i++) {
        p[i] = 0;
    }
    for (i = 1; i <= maxpart; i++) {
        for (j = i; j <= n; j++) {
            p[j] = p[j] + p[j - i];
        }
    }
    return p[n];
}

void print_partition_sequence(int count) {
    int i;
    int first;
    first = 1;
    for (i = 0; i < count; i++) {
        if (first) {
            printf("%d", partition_dp(i));
            first = 0;
        } else {
            printf(" %d", partition_dp(i));
        }
    }
    printf("\n");
}

int sum_partitions(int n) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i <= n; i++) {
        sum = sum + partition_dp(i);
    }
    return sum;
}

int main() {
    int p;
    int pd;
    int po;
    int pk;
    int pek;
    int lp;
    int sp;

    // EXPECT: Partition numbers p(0)..p(15):
    printf("Partition numbers p(0)..p(15):\n");
    // EXPECT: 1 1 2 3 5 7 11 15 22 30 42 56 77 101 135 176
    print_partition_sequence(16);

    p = partition_dp(20);
    // EXPECT: p(20) = 627
    printf("p(20) = %d\n", p);

    p = partition_dp(30);
    // EXPECT: p(30) = 5604
    printf("p(30) = %d\n", p);

    p = partition_dp(50);
    // EXPECT: p(50) = 204226
    printf("p(50) = %d\n", p);

    pd = partition_into_distinct(10);
    // EXPECT: Distinct partitions of 10: 10
    printf("Distinct partitions of 10: %d\n", pd);

    pd = partition_into_distinct(15);
    // EXPECT: Distinct partitions of 15: 27
    printf("Distinct partitions of 15: %d\n", pd);

    po = partition_into_odd(10);
    // EXPECT: Odd partitions of 10: 10
    printf("Odd partitions of 10: %d\n", po);

    po = partition_into_odd(15);
    // EXPECT: Odd partitions of 15: 27
    printf("Odd partitions of 15: %d\n", po);

    // EXPECT: Distinct == Odd (Euler's theorem) for 10: 1
    printf("Distinct == Odd (Euler's theorem) for 10: %d\n",
           partition_into_distinct(10) == partition_into_odd(10) ? 1 : 0);

    pk = partition_at_most_k_parts(10, 3);
    // EXPECT: p(10) with at most 3 parts: 14
    printf("p(10) with at most 3 parts: %d\n", pk);

    pk = partition_at_most_k_parts(10, 5);
    // EXPECT: p(10) with at most 5 parts: 30
    printf("p(10) with at most 5 parts: %d\n", pk);

    pek = partition_exactly_k_parts(10, 3);
    // EXPECT: p(10) with exactly 3 parts: 8
    printf("p(10) with exactly 3 parts: %d\n", pek);

    pek = partition_exactly_k_parts(10, 4);
    // EXPECT: p(10) with exactly 4 parts: 9
    printf("p(10) with exactly 4 parts: %d\n", pek);

    lp = largest_part_at_most(10, 4);
    // EXPECT: p(10) largest part at most 4: 23
    printf("p(10) largest part at most 4: %d\n", lp);

    lp = largest_part_at_most(10, 3);
    // EXPECT: p(10) largest part at most 3: 14
    printf("p(10) largest part at most 3: %d\n", lp);

    // EXPECT: Conjugate duality: at most k parts == largest at most k
    printf("Conjugate duality: at most k parts == largest at most k\n");
    // EXPECT: p(10,<=3) parts=14, largest<=3: 14, equal: 1
    printf("p(10,<=3) parts=%d, largest<=3: %d, equal: %d\n",
           partition_at_most_k_parts(10, 3),
           largest_part_at_most(10, 3),
           partition_at_most_k_parts(10, 3) == largest_part_at_most(10, 3) ? 1 : 0);

    sp = sum_partitions(10);
    // EXPECT: Sum p(0)..p(10): 139
    printf("Sum p(0)..p(10): %d\n", sp);

    return 0;
}
