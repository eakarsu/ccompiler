int printf(const char *fmt, ...);
// EXPECT: Factorials:\n0! = 1\n1! = 1\n2! = 2\n3! = 6\n4! = 24\n5! = 120\n6! = 720\n7! = 5040\n8! = 40320\n9! = 362880\n10! = 3628800\n11! = 39916800\n12! = 479001600\n\nPermutations P(n,r):\nP(5,2) = 20\nP(5,3) = 60\nP(8,3) = 336\nP(10,4) = 5040\nP(7,7) = 5040\nP(6,1) = 6\n\nCircular permutations:\nCircular(3) = 2\nCircular(4) = 6\nCircular(5) = 24\nCircular(6) = 120\nCircular(7) = 720\nCircular(8) = 5040\n\nPermutations with repetition:\nAABB arrangements: 6\nAAABBC arrangements: 60\nABCABC arrangements: 90\n\nPermutation [3,1,4,5,2,6] inversions: 4\nParity: even
// Test: Permutation counting

int factorial(int n) {
    int result = 1;
    int i;
    for (i = 2; i <= n; i++) {
        result = result * i;
    }
    return result;
}

int perm(int n, int r) {
    // P(n,r) = n! / (n-r)!
    int result = 1;
    int i;
    for (i = n - r + 1; i <= n; i++) {
        result = result * i;
    }
    return result;
}

int main(void) {
    int i, j;

    printf("Factorials:\n");
    for (i = 0; i <= 12; i++) {
        printf("%d! = %d\n", i, factorial(i));
    }

    printf("\nPermutations P(n,r):\n");
    int ns[6];
    int rs[6];
    ns[0] = 5;  rs[0] = 2;
    ns[1] = 5;  rs[1] = 3;
    ns[2] = 8;  rs[2] = 3;
    ns[3] = 10; rs[3] = 4;
    ns[4] = 7;  rs[4] = 7;
    ns[5] = 6;  rs[5] = 1;

    for (i = 0; i < 6; i++) {
        printf("P(%d,%d) = %d\n", ns[i], rs[i], perm(ns[i], rs[i]));
    }

    // Circular permutations: (n-1)!
    printf("\nCircular permutations:\n");
    for (i = 3; i <= 8; i++) {
        printf("Circular(%d) = %d\n", i, factorial(i - 1));
    }

    // Permutations with repetition
    // "MISSISSIPPI" style: n! / (n1! * n2! * ... * nk!)
    // For "AABB": 4! / (2! * 2!) = 6
    printf("\nPermutations with repetition:\n");
    int n1 = factorial(4) / (factorial(2) * factorial(2));
    printf("AABB arrangements: %d\n", n1);

    // "AAABBC": 6! / (3! * 2! * 1!) = 60
    int n2 = factorial(6) / (factorial(3) * factorial(2) * factorial(1));
    printf("AAABBC arrangements: %d\n", n2);

    // "ABCABC": 6! / (2! * 2! * 2!) = 90
    int n3 = factorial(6) / (factorial(2) * factorial(2) * factorial(2));
    printf("ABCABC arrangements: %d\n", n3);

    // Count inversions in a permutation
    int perm_arr[6];
    perm_arr[0] = 3;
    perm_arr[1] = 1;
    perm_arr[2] = 4;
    perm_arr[3] = 5;
    perm_arr[4] = 2;
    perm_arr[5] = 6;

    int inversions = 0;
    for (i = 0; i < 6; i++) {
        for (j = i + 1; j < 6; j++) {
            if (perm_arr[i] > perm_arr[j]) {
                inversions++;
            }
        }
    }
    printf("\nPermutation [3,1,4,5,2,6] inversions: %d\n", inversions);
    printf("Parity: %s\n", inversions % 2 == 0 ? "even" : "odd");

    return 0;
}
