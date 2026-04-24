int printf(const char *fmt, ...);

// Permutation generation using backtracking

int arr[6];
int used[6];
int perm[6];
int perm_count;

void swap(int *a, int *b) {
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void generate_perms(int n, int depth) {
    int i;
    if (depth == n) {
        perm_count = perm_count + 1;
        return;
    }
    for (i = 0; i < n; i++) {
        if (used[i] == 0) {
            perm[depth] = arr[i];
            used[i] = 1;
            generate_perms(n, depth + 1);
            used[i] = 0;
        }
    }
}

void print_perms(int n, int depth) {
    int i;
    int j;
    if (depth == n) {
        for (j = 0; j < n; j++) {
            if (j > 0) printf(" ");
            printf("%d", perm[j]);
        }
        printf("\n");
        return;
    }
    for (i = 0; i < n; i++) {
        if (used[i] == 0) {
            perm[depth] = arr[i];
            used[i] = 1;
            print_perms(n, depth + 1);
            used[i] = 0;
        }
    }
}

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int is_sorted(int *a, int n) {
    int i;
    for (i = 0; i < n - 1; i++) {
        if (a[i] > a[i + 1]) return 0;
    }
    return 1;
}

int sorted_count;

void count_sorted_perms(int n, int depth) {
    int i;
    if (depth == n) {
        if (is_sorted(perm, n)) {
            sorted_count = sorted_count + 1;
        }
        return;
    }
    for (i = 0; i < n; i++) {
        if (used[i] == 0) {
            perm[depth] = arr[i];
            used[i] = 1;
            count_sorted_perms(n, depth + 1);
            used[i] = 0;
        }
    }
}

int main() {
    int i;
    int n;
    int f;

    // Test 1: count permutations of 3 elements
    n = 3;
    for (i = 0; i < n; i++) {
        arr[i] = i + 1;
        used[i] = 0;
    }
    perm_count = 0;
    generate_perms(n, 0);
    // EXPECT: Perms of 3: 6
    printf("Perms of 3: %d\n", perm_count);

    // Test 2: count permutations of 4 elements
    n = 4;
    for (i = 0; i < n; i++) {
        arr[i] = i + 1;
        used[i] = 0;
    }
    perm_count = 0;
    generate_perms(n, 0);
    // EXPECT: Perms of 4: 24
    printf("Perms of 4: %d\n", perm_count);

    // Test 3: print all permutations of {1,2,3}
    n = 3;
    for (i = 0; i < n; i++) {
        arr[i] = i + 1;
        used[i] = 0;
    }
    // EXPECT: All perms of 1 2 3:
    printf("All perms of 1 2 3:\n");
    // EXPECT: 1 2 3
    // EXPECT: 1 3 2
    // EXPECT: 2 1 3
    // EXPECT: 2 3 1
    // EXPECT: 3 1 2
    // EXPECT: 3 2 1
    print_perms(n, 0);

    // Test 4: factorial check
    f = factorial(5);
    // EXPECT: 5! = 120
    printf("5! = %d\n", f);

    // Test 5: count sorted permutations (should be exactly 1)
    n = 4;
    for (i = 0; i < n; i++) {
        arr[i] = i + 1;
        used[i] = 0;
    }
    sorted_count = 0;
    count_sorted_perms(n, 0);
    // EXPECT: Sorted perms of 4: 1
    printf("Sorted perms of 4: %d\n", sorted_count);

    // EXPECT: Permutations done
    printf("Permutations done\n");

    return 0;
}
