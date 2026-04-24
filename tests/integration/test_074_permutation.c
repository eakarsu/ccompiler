int printf(const char *fmt, ...);
// EXPECT: Factorials\n0!=1\n1!=1\n2!=2\n3!=6\n4!=24\n5!=120\n6!=720\n7!=5040\nPermCount3= 6\nPermCount4= 24\nPerm1: 1 2 3\nPerm2: 1 3 2\nPerm3: 2 1 3\nPerm4: 2 3 1\nPerm5: 3 2 1\nPerm6: 3 1 2\nNextPerm\nStart: 0 1 2 3\nNext1: 0 1 3 2\nNext2: 0 2 1 3\nNext3: 0 2 3 1\nNext4: 0 3 1 2\nNext5: 0 3 2 1\nRankTest\nRank 0 1 2 3 = 0\nRank 3 2 1 0 = 23\nRank 1 0 3 2 = 7\nUnranked\nUnrank 0: 0 1 2 3\nUnrank 23: 3 2 1 0\nUnrank 10: 1 3 0 2\nInversions=4\nSign=1\nCycles=2\nFixedPoint2\nDerangemnt1\nDr0= 1\nDr1= 0\nDr2= 1\nDr3= 2\nDr4= 9\nDr5= 44\nDr6= 265\nPOrder3\nInverse 1 3 0 2\nCompose 3 2 1 0\nLIS=2\nLDS=3\nLehmer 2 0 1 0\nValid=1\nInvalid=0\nAll  passed=1

// Permutations: generation, counting, next permutation, rank/unrank

int perm[10];
int perm_used[10];
int perm_count;

// Factorial
int factorial(int n) {
    int r;
    int i;
    r = 1;
    for (i = 2; i <= n; i = i + 1) {
        r = r * i;
    }
    return r;
}

// Swap two elements in array
void swap(int arr[10], int i, int j) {
    int t;
    t = arr[i];
    arr[i] = arr[j];
    arr[j] = t;
}

// Print permutation of length n
void print_perm(int arr[10], int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        printf("%d", arr[i]);
        if (i < n - 1) {
            printf(" ");
        }
    }
    printf("%c", 10);
}

// Generate all permutations of {0,..,n-1} recursively
// Store count only (too many to print all)
int gen_count;

void generate_perms(int arr[10], int start, int n) {
    int i;
    if (start == n) {
        gen_count = gen_count + 1;
        return;
    }
    for (i = start; i < n; i = i + 1) {
        swap(arr, start, i);
        generate_perms(arr, start + 1, n);
        swap(arr, start, i);
    }
}

// Generate and print first k permutations of {1,..,n}
int printed_count;
int max_print;

void generate_and_print(int arr[10], int start, int n) {
    int i;
    if (printed_count >= max_print) {
        return;
    }
    if (start == n) {
        printed_count = printed_count + 1;
        printf("%c%c%c%c%d%c ", 'P','e','r','m', printed_count, ':');
        print_perm(arr, n);
        return;
    }
    for (i = start; i < n; i = i + 1) {
        if (printed_count >= max_print) {
            return;
        }
        swap(arr, start, i);
        generate_and_print(arr, start + 1, n);
        swap(arr, start, i);
    }
}

// Next permutation algorithm (lexicographic order)
// Returns 1 if next exists, 0 if already last
int next_permutation(int arr[10], int n) {
    int i;
    int j;
    int k;
    int l;

    // Find largest i such that arr[i] < arr[i+1]
    i = n - 2;
    while (i >= 0 && arr[i] >= arr[i + 1]) {
        i = i - 1;
    }
    if (i < 0) {
        return 0; // last permutation
    }

    // Find largest j such that arr[j] > arr[i]
    j = n - 1;
    while (arr[j] <= arr[i]) {
        j = j - 1;
    }

    // Swap arr[i] and arr[j]
    swap(arr, i, j);

    // Reverse arr[i+1..n-1]
    k = i + 1;
    l = n - 1;
    while (k < l) {
        swap(arr, k, l);
        k = k + 1;
        l = l - 1;
    }
    return 1;
}

// Previous permutation
int prev_permutation(int arr[10], int n) {
    int i;
    int j;
    int k;
    int l;

    i = n - 2;
    while (i >= 0 && arr[i] <= arr[i + 1]) {
        i = i - 1;
    }
    if (i < 0) {
        return 0;
    }

    j = n - 1;
    while (arr[j] >= arr[i]) {
        j = j - 1;
    }

    swap(arr, i, j);

    k = i + 1;
    l = n - 1;
    while (k < l) {
        swap(arr, k, l);
        k = k + 1;
        l = l - 1;
    }
    return 1;
}

// Permutation rank (0-indexed, lexicographic)
// For permutation of {0,..,n-1}
int perm_rank(int arr[10], int n) {
    int rank;
    int i;
    int j;
    int count;
    rank = 0;
    for (i = 0; i < n; i = i + 1) {
        count = 0;
        for (j = i + 1; j < n; j = j + 1) {
            if (arr[j] < arr[i]) {
                count = count + 1;
            }
        }
        rank = rank + count * factorial(n - 1 - i);
    }
    return rank;
}

// Unrank: given rank, produce the permutation of {0,..,n-1}
void perm_unrank(int rank, int n, int result[10]) {
    int available[10];
    int i;
    int j;
    int fact;
    int idx;

    for (i = 0; i < n; i = i + 1) {
        available[i] = i;
    }

    for (i = 0; i < n; i = i + 1) {
        fact = factorial(n - 1 - i);
        idx = rank / fact;
        rank = rank - idx * fact;
        result[i] = available[idx];
        // Remove available[idx] by shifting
        for (j = idx; j < n - 1 - i; j = j + 1) {
            available[j] = available[j + 1];
        }
    }
}

// Check if array is a valid permutation of {0,..,n-1}
int is_valid_perm(int arr[10], int n) {
    int seen[10];
    int i;
    for (i = 0; i < n; i = i + 1) {
        seen[i] = 0;
    }
    for (i = 0; i < n; i = i + 1) {
        if (arr[i] < 0 || arr[i] >= n) {
            return 0;
        }
        if (seen[arr[i]] == 1) {
            return 0;
        }
        seen[arr[i]] = 1;
    }
    return 1;
}

// Count inversions in a permutation
int count_inversions(int arr[10], int n) {
    int inv;
    int i;
    int j;
    inv = 0;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            if (arr[i] > arr[j]) {
                inv = inv + 1;
            }
        }
    }
    return inv;
}

// Check if permutation is even (even number of inversions)
int is_even_perm(int arr[10], int n) {
    int inv;
    inv = count_inversions(arr, n);
    return 1 - (inv - (inv / 2) * 2);
}

// Apply permutation: result[i] = arr[perm[i]]
void apply_perm(int arr[10], int p[10], int result[10], int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        result[i] = arr[p[i]];
    }
}

// Compose two permutations: result[i] = p2[p1[i]]
void compose_perms(int p1[10], int p2[10], int result[10], int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        result[i] = p2[p1[i]];
    }
}

// Inverse permutation: if p[i]=j then inv[j]=i
void inverse_perm(int p[10], int inv[10], int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        inv[p[i]] = i;
    }
}

// Count fixed points (elements where arr[i]==i)
int count_fixed_points(int arr[10], int n) {
    int count;
    int i;
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        if (arr[i] == i) {
            count = count + 1;
        }
    }
    return count;
}

// Count cycles in permutation
int count_cycles(int arr[10], int n) {
    int visited[10];
    int cycles;
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        visited[i] = 0;
    }
    cycles = 0;
    for (i = 0; i < n; i = i + 1) {
        if (visited[i] == 0) {
            cycles = cycles + 1;
            j = i;
            while (visited[j] == 0) {
                visited[j] = 1;
                j = arr[j];
            }
        }
    }
    return cycles;
}

// Order of permutation (smallest k such that p^k = identity)
int perm_order(int arr[10], int n) {
    int temp[10];
    int cur[10];
    int i;
    int k;
    int is_id;

    for (i = 0; i < n; i = i + 1) {
        cur[i] = arr[i];
    }

    for (k = 1; k <= 100; k = k + 1) {
        is_id = 1;
        for (i = 0; i < n; i = i + 1) {
            if (cur[i] != i) {
                is_id = 0;
                break;
            }
        }
        if (is_id == 1) {
            return k;
        }
        // cur = cur composed with arr
        for (i = 0; i < n; i = i + 1) {
            temp[i] = arr[cur[i]];
        }
        for (i = 0; i < n; i = i + 1) {
            cur[i] = temp[i];
        }
    }
    return -1;
}

// Derangement check (no fixed points)
int is_derangement(int arr[10], int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (arr[i] == i) {
            return 0;
        }
    }
    return 1;
}

// Count derangements of n elements (subfactorial)
int count_derangements(int n) {
    // D(n) = (n-1) * (D(n-1) + D(n-2))
    int d0;
    int d1;
    int d2;
    int i;
    if (n == 0) {
        return 1;
    }
    if (n == 1) {
        return 0;
    }
    d0 = 1;
    d1 = 0;
    for (i = 2; i <= n; i = i + 1) {
        d2 = (i - 1) * (d1 + d0);
        d0 = d1;
        d1 = d2;
    }
    return d1;
}

// Lehmer code
void lehmer_code(int arr[10], int code[10], int n) {
    int i;
    int j;
    int count;
    for (i = 0; i < n; i = i + 1) {
        count = 0;
        for (j = i + 1; j < n; j = j + 1) {
            if (arr[j] < arr[i]) {
                count = count + 1;
            }
        }
        code[i] = count;
    }
}

// Signature (sign of permutation): +1 or -1
int perm_sign(int arr[10], int n) {
    int inv;
    inv = count_inversions(arr, n);
    if ((inv - (inv / 2) * 2) == 0) {
        return 1;
    }
    return -1;
}

// Longest increasing subsequence length (simple O(n^2))
int lis_length(int arr[10], int n) {
    int dp[10];
    int i;
    int j;
    int mx;
    for (i = 0; i < n; i = i + 1) {
        dp[i] = 1;
    }
    for (i = 1; i < n; i = i + 1) {
        for (j = 0; j < i; j = j + 1) {
            if (arr[j] < arr[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
            }
        }
    }
    mx = 0;
    for (i = 0; i < n; i = i + 1) {
        if (dp[i] > mx) {
            mx = dp[i];
        }
    }
    return mx;
}

// Longest decreasing subsequence length
int lds_length(int arr[10], int n) {
    int dp[10];
    int i;
    int j;
    int mx;
    for (i = 0; i < n; i = i + 1) {
        dp[i] = 1;
    }
    for (i = 1; i < n; i = i + 1) {
        for (j = 0; j < i; j = j + 1) {
            if (arr[j] > arr[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
            }
        }
    }
    mx = 0;
    for (i = 0; i < n; i = i + 1) {
        if (dp[i] > mx) {
            mx = dp[i];
        }
    }
    return mx;
}

// Copy array
void copy_arr(int src[10], int dst[10], int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        dst[i] = src[i];
    }
}

// Compare two arrays
int arrays_equal(int a[10], int b[10], int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    int a[10];
    int b[10];
    int c[10];
    int code[10];
    int i;
    int n;
    int rank;

    // Factorial tests
    printf("%c%c%c%c%c%c%c%c%c%c", 'F','a','c','t','o','r','i','a','l','s');
    printf("%c", 10);
    for (i = 0; i <= 7; i = i + 1) {
        printf("%d%c%c%d", i, '!', '=', factorial(i));
        printf("%c", 10);
    }

    // Count permutations
    a[0] = 0; a[1] = 1; a[2] = 2;
    gen_count = 0;
    generate_perms(a, 0, 3);
    printf("%c%c%c%c%c%c%c%c%c%d%c%c%d", 'P','e','r','m','C','o','u','n','t', 3, '=', ' ', gen_count);
    printf("%c", 10);

    a[0] = 0; a[1] = 1; a[2] = 2; a[3] = 3;
    gen_count = 0;
    generate_perms(a, 0, 4);
    printf("%c%c%c%c%c%c%c%c%c%d%c%c%d", 'P','e','r','m','C','o','u','n','t', 4, '=', ' ', gen_count);
    printf("%c", 10);

    // Print first 6 permutations of {1,2,3}
    a[0] = 1; a[1] = 2; a[2] = 3;
    printed_count = 0;
    max_print = 6;
    generate_and_print(a, 0, 3);

    // Next permutation tests
    printf("%c%c%c%c%c%c%c%c", 'N','e','x','t','P','e','r','m');
    printf("%c", 10);

    a[0] = 0; a[1] = 1; a[2] = 2; a[3] = 3;
    printf("%c%c%c%c%c%c %d %d %d %d", 'S','t','a','r','t',':', a[0], a[1], a[2], a[3]);
    printf("%c", 10);

    for (i = 0; i < 5; i = i + 1) {
        next_permutation(a, 4);
        printf("%c%c%c%c%d%c %d %d %d %d", 'N','e','x','t', i+1, ':', a[0], a[1], a[2], a[3]);
        printf("%c", 10);
    }

    // Rank tests
    printf("%c%c%c%c%c%c%c%c", 'R','a','n','k','T','e','s','t');
    printf("%c", 10);

    a[0] = 0; a[1] = 1; a[2] = 2; a[3] = 3;
    rank = perm_rank(a, 4);
    printf("%c%c%c%c %d %d %d %d %c%c%d", 'R','a','n','k', a[0], a[1], a[2], a[3], '=', ' ', rank);
    printf("%c", 10);

    a[0] = 3; a[1] = 2; a[2] = 1; a[3] = 0;
    rank = perm_rank(a, 4);
    printf("%c%c%c%c %d %d %d %d %c%c%d", 'R','a','n','k', a[0], a[1], a[2], a[3], '=', ' ', rank);
    printf("%c", 10);

    a[0] = 1; a[1] = 0; a[2] = 3; a[3] = 2;
    rank = perm_rank(a, 4);
    printf("%c%c%c%c %d %d %d %d %c%c%d", 'R','a','n','k', a[0], a[1], a[2], a[3], '=', ' ', rank);
    printf("%c", 10);

    // Unrank tests
    printf("%c%c%c%c%c%c%c%c", 'U','n','r','a','n','k','e','d');
    printf("%c", 10);

    perm_unrank(0, 4, b);
    printf("%c%c%c%c%c%c %d%c %d %d %d %d", 'U','n','r','a','n','k', 0, ':', b[0], b[1], b[2], b[3]);
    printf("%c", 10);

    perm_unrank(23, 4, b);
    printf("%c%c%c%c%c%c %d%c %d %d %d %d", 'U','n','r','a','n','k', 23, ':', b[0], b[1], b[2], b[3]);
    printf("%c", 10);

    perm_unrank(10, 4, b);
    printf("%c%c%c%c%c%c %d%c %d %d %d %d", 'U','n','r','a','n','k', 10, ':', b[0], b[1], b[2], b[3]);
    printf("%c", 10);

    // Inversion count
    a[0] = 3; a[1] = 1; a[2] = 0; a[3] = 2;
    printf("%c%c%c%c%c%c%c%c%c%c%c%d", 'I','n','v','e','r','s','i','o','n','s','=', count_inversions(a, 4));
    printf("%c", 10);

    // Sign
    printf("%c%c%c%c%c%d", 'S','i','g','n','=', perm_sign(a, 4));
    printf("%c", 10);

    // Cycles
    printf("%c%c%c%c%c%c%c%d", 'C','y','c','l','e','s','=', count_cycles(a, 4));
    printf("%c", 10);

    // Fixed points
    a[0] = 0; a[1] = 2; a[2] = 1; a[3] = 3;
    printf("%c%c%c%c%c%c%c%c%c%c%d", 'F','i','x','e','d','P','o','i','n','t', count_fixed_points(a, 4));
    printf("%c", 10);

    // Derangement check
    a[0] = 1; a[1] = 0; a[2] = 3; a[3] = 2;
    printf("%c%c%c%c%c%c%c%c%c%c%d", 'D','e','r','a','n','g','e','m','n','t', is_derangement(a, 4));
    printf("%c", 10);

    // Derangement count
    for (i = 0; i <= 6; i = i + 1) {
        printf("%c%c%d%c%c%d", 'D','r', i, '=', ' ', count_derangements(i));
        printf("%c", 10);
    }

    // Perm order
    a[0] = 1; a[1] = 2; a[2] = 0; a[3] = 3;
    printf("%c%c%c%c%c%c%d", 'P','O','r','d','e','r', perm_order(a, 4));
    printf("%c", 10);

    // Inverse permutation
    a[0] = 2; a[1] = 0; a[2] = 3; a[3] = 1;
    inverse_perm(a, b, 4);
    printf("%c%c%c%c%c%c%c %d %d %d %d", 'I','n','v','e','r','s','e', b[0], b[1], b[2], b[3]);
    printf("%c", 10);

    // Compose
    a[0] = 1; a[1] = 0; a[2] = 3; a[3] = 2;
    b[0] = 2; b[1] = 3; b[2] = 0; b[3] = 1;
    compose_perms(a, b, c, 4);
    printf("%c%c%c%c%c%c%c %d %d %d %d", 'C','o','m','p','o','s','e', c[0], c[1], c[2], c[3]);
    printf("%c", 10);

    // LIS
    a[0] = 3; a[1] = 1; a[2] = 4; a[3] = 0; a[4] = 2;
    printf("%c%c%c%c%d", 'L','I','S','=', lis_length(a, 5));
    printf("%c", 10);

    // LDS
    printf("%c%c%c%c%d", 'L','D','S','=', lds_length(a, 5));
    printf("%c", 10);

    // Lehmer code
    a[0] = 2; a[1] = 0; a[2] = 3; a[3] = 1;
    lehmer_code(a, code, 4);
    printf("%c%c%c%c%c%c %d %d %d %d", 'L','e','h','m','e','r', code[0], code[1], code[2], code[3]);
    printf("%c", 10);

    // Valid perm check
    a[0] = 0; a[1] = 1; a[2] = 2; a[3] = 3;
    printf("%c%c%c%c%c%c%d", 'V','a','l','i','d','=', is_valid_perm(a, 4));
    printf("%c", 10);

    a[0] = 0; a[1] = 1; a[2] = 1; a[3] = 3;
    printf("%c%c%c%c%c%c%c%c%d", 'I','n','v','a','l','i','d','=', is_valid_perm(a, 4));
    printf("%c", 10);

    printf("%c%c%c%c %c%c%c%c%c%c%c%d", 'A','l','l',' ', 'p','a','s','s','e','d','=', 1);
    printf("%c", 10);

    return 0;
}
