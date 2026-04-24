int printf(const char *fmt, ...);
// EXPECT: All C(5,3) = 10 combinations:\nRank 0: {0,1,2}\nRank 1: {0,1,3}\nRank 2: {0,1,4}\nRank 3: {0,2,3}\nRank 4: {0,2,4}\nRank 5: {0,3,4}\nRank 6: {1,2,3}\nRank 7: {1,2,4}\nRank 8: {1,3,4}\nRank 9: {2,3,4}\n\nRank verification:\n{0,1,2} -> rank 0 (expected 0) OK\n{0,1,3} -> rank 1 (expected 1) OK\n{0,1,4} -> rank 2 (expected 2) OK\n{0,2,3} -> rank 3 (expected 3) OK\n{0,2,4} -> rank 4 (expected 4) OK\n{0,3,4} -> rank 5 (expected 5) OK\n{1,2,3} -> rank 6 (expected 6) OK\n{1,2,4} -> rank 7 (expected 7) OK\n{1,3,4} -> rank 8 (expected 8) OK\n{2,3,4} -> rank 9 (expected 9) OK\n\nAll C(6,2) = 15 combinations:\nRank 0: {0,1}\nRank 1: {0,2}\nRank 2: {0,3}\nRank 3: {0,4}\nRank 4: {0,5}\nRank 5: {1,2}\nRank 6: {1,3}\nRank 7: {1,4}\nRank 8: {1,5}\nRank 9: {2,3}\nRank 10: {2,4}\nRank 11: {2,5}\nRank 12: {3,4}\nRank 13: {3,5}\nRank 14: {4,5}\n\nC(7,4) = 35, specific combinations:\nRank 0: {0,1,2,3}\nRank 10: {0,2,3,4}\nRank 20: {1,2,3,4}\nRank 30: {2,3,4,5}\nRank 34: {3,4,5,6}\n\nPascal's rule verification:\nC(3,1)=3 = C(2,0)+C(2,1)=1+2=3\nC(3,2)=3 = C(2,1)+C(2,2)=2+1=3\nC(4,1)=4 = C(3,0)+C(3,1)=1+3=4\nC(4,2)=6 = C(3,1)+C(3,2)=3+3=6\nC(4,3)=4 = C(3,2)+C(3,3)=3+1=4\nC(5,1)=5 = C(4,0)+C(4,1)=1+4=5\nC(5,2)=10 = C(4,1)+C(4,2)=4+6=10\nC(5,3)=10 = C(4,2)+C(4,3)=6+4=10\nC(5,4)=5 = C(4,3)+C(4,4)=4+1=5\nC(6,1)=6 = C(5,0)+C(5,1)=1+5=6\nC(6,2)=15 = C(5,1)+C(5,2)=5+10=15\nC(6,3)=20 = C(5,2)+C(5,3)=10+10=20\nC(6,4)=15 = C(5,3)+C(5,4)=10+5=15\nC(6,5)=6 = C(5,4)+C(5,5)=5+1=6\nC(7,1)=7 = C(6,0)+C(6,1)=1+6=7\nC(7,2)=21 = C(6,1)+C(6,2)=6+15=21\nC(7,3)=35 = C(6,2)+C(6,3)=15+20=35\nC(7,4)=35 = C(6,3)+C(6,4)=20+15=35\nC(7,5)=21 = C(6,4)+C(6,5)=15+6=21\nC(7,6)=7 = C(6,5)+C(6,6)=6+1=7\nC(8,1)=8 = C(7,0)+C(7,1)=1+7=8\nC(8,2)=28 = C(7,1)+C(7,2)=7+21=28\nC(8,3)=56 = C(7,2)+C(7,3)=21+35=56\nC(8,4)=70 = C(7,3)+C(7,4)=35+35=70\nC(8,5)=56 = C(7,4)+C(7,5)=35+21=56\nC(8,6)=28 = C(7,5)+C(7,6)=21+7=28\nC(8,7)=8 = C(7,6)+C(7,7)=7+1=8
// Test: Nth combination (combinatorial number system)

int binom(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    int result = 1;
    int i;
    for (i = 0; i < k; i++) {
        result = result * (n - i);
        result = result / (i + 1);
    }
    return result;
}

// Unrank: find the nth combination of k elements from {0,1,...,n-1}
void nth_combination(int n, int k, int rank, int result[]) {
    int i;
    int r = rank;
    int pos = 0;
    int start = 0;

    for (i = k; i >= 1; i--) {
        int j;
        for (j = start; j <= n - i; j++) {
            int c = binom(n - j - 1, i - 1);
            if (r < c) {
                result[pos] = j;
                pos++;
                start = j + 1;
                break;
            }
            r = r - c;
        }
    }
}

// Rank: find the rank of a given combination
int rank_combination(int n, int k, int comb[]) {
    int rank = 0;
    int i, j;
    int prev = -1;
    for (i = 0; i < k; i++) {
        for (j = prev + 1; j < comb[i]; j++) {
            rank = rank + binom(n - j - 1, k - i - 1);
        }
        prev = comb[i];
    }
    return rank;
}

int main(void) {
    int i;

    // List all C(5,3) = 10 combinations
    int n = 5, k = 3;
    int total = binom(n, k);
    printf("All C(%d,%d) = %d combinations:\n", n, k, total);

    int comb[5];
    for (i = 0; i < total; i++) {
        nth_combination(n, k, i, comb);
        printf("Rank %d: {%d,%d,%d}\n", i, comb[0], comb[1], comb[2]);
    }

    // Verify ranking
    printf("\nRank verification:\n");
    for (i = 0; i < total; i++) {
        nth_combination(n, k, i, comb);
        int r = rank_combination(n, k, comb);
        printf("{%d,%d,%d} -> rank %d (expected %d) %s\n",
               comb[0], comb[1], comb[2], r, i,
               r == i ? "OK" : "FAIL");
    }

    // C(6,2) = 15 combinations
    printf("\nAll C(6,2) = %d combinations:\n", binom(6, 2));
    for (i = 0; i < binom(6, 2); i++) {
        nth_combination(6, 2, i, comb);
        printf("Rank %d: {%d,%d}\n", i, comb[0], comb[1]);
    }

    // C(7,4) specific ranks
    printf("\nC(7,4) = %d, specific combinations:\n", binom(7, 4));
    int ranks[5];
    ranks[0] = 0;
    ranks[1] = 10;
    ranks[2] = 20;
    ranks[3] = 30;
    ranks[4] = 34;
    for (i = 0; i < 5; i++) {
        nth_combination(7, 4, ranks[i], comb);
        printf("Rank %d: {%d,%d,%d,%d}\n", ranks[i],
               comb[0], comb[1], comb[2], comb[3]);
    }

    // Pascal's rule: C(n,k) = C(n-1,k-1) + C(n-1,k)
    printf("\nPascal's rule verification:\n");
    for (n = 3; n <= 8; n++) {
        for (k = 1; k < n; k++) {
            int lhs = binom(n, k);
            int rhs = binom(n - 1, k - 1) + binom(n - 1, k);
            printf("C(%d,%d)=%d = C(%d,%d)+C(%d,%d)=%d+%d=%d\n",
                   n, k, lhs, n - 1, k - 1, n - 1, k,
                   binom(n - 1, k - 1), binom(n - 1, k), rhs);
        }
    }

    return 0;
}
