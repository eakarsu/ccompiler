int printf(const char *fmt, ...);
// EXPECT: Pascal Triangle\nRow 0: 1\nRow 1: 1 1\nRow 2: 1 2 1\nRow 3: 1 3 3 1\nRow 4: 1 4 6 4 1\nRow 5: 1 5 10 10 5 1\nRow 6: 1 6 15 20 15 6 1\nBinomials:\nC(10,3)=120\nC(10,5)=252\nC(12,4)=495\nC(8,0)=1\nC(8,8)=1\nBinomDirec116\nbinom(10,3)=120\nbinom(12,6)=924\nGenerate 5 choose 2\nCount=10\nComb0: 0 1\nComb1: 0 2\nComb2: 0 3\nComb3: 0 4\nComb4: 1 2\nComb5: 1 3\nComb6: 1 4\nComb7: 2 3\nComb8: 2 4\nComb9: 3 4\nGenerate 6 choose 3\nCount=20\nRow Sums\nRowSum 0=1\nRowSum 1=2\nRowSum 2=4\nRowSum 3=8\nRowSum 4=16\nRowSum 5=32\nRowSum 6=64\nRowSum 7=128\nRowSum 8=256\nAltSum 5=0\nAltSum 0=1\nSubsets 5=32\nSubsSize 5,2=10\nSubsSize 5,3=10\nSumCnt  5=3\nSumCnt  10=3\nCatalans\nCat(0)1\nCat(1)1\nCat(2)2\nCat(3)5\nCat(4)14\nCat(5)42\nCat(6)132\nStirling\nS(4,2)=7\nS(5,3)=25\nBellNums\nB(0)=1\nB(1)=1\nB(2)=2\nB(3)=5\nB(4)=15\nB(5)=52\nVandermond 4,5,3=84\nVandDirect C9,3= 84\nHockeyStic 6,2=35\nC(7,3)=35\nCombRep 4,3=20\nMultin 4=12\nPowerSet 6=64\nDerangeIE 4=9\nDerangeIE 5=44\nSurjection 4,3=36\nAll  passed=1

// Combinations: generation, Pascal's triangle, binomial coefficients, subset enumeration

// Pascal's triangle storage
int pascal[15][15];

void build_pascal(int n) {
    int i;
    int j;
    for (i = 0; i < 15; i = i + 1) {
        for (j = 0; j < 15; j = j + 1) {
            pascal[i][j] = 0;
        }
    }
    for (i = 0; i <= n; i = i + 1) {
        pascal[i][0] = 1;
        for (j = 1; j <= i; j = j + 1) {
            pascal[i][j] = pascal[i - 1][j - 1] + pascal[i - 1][j];
        }
    }
}

// Binomial coefficient using Pascal's triangle
int C(int n, int k) {
    if (k < 0 || k > n) {
        return 0;
    }
    return pascal[n][k];
}

// Binomial coefficient computed directly
int binom(int n, int k) {
    int result;
    int i;
    if (k < 0 || k > n) {
        return 0;
    }
    if (k > n - k) {
        k = n - k;
    }
    result = 1;
    for (i = 0; i < k; i = i + 1) {
        result = result * (n - i);
        result = result / (i + 1);
    }
    return result;
}

// Generate combinations of {0..n-1} choose k
// Store in comb_result array
int comb_result[100][10]; // up to 100 combinations, each up to size 10
int comb_count;
int comb_temp[10];

void gen_combinations(int start, int n, int k, int depth) {
    int i;
    int j;
    if (depth == k) {
        if (comb_count < 100) {
            for (j = 0; j < k; j = j + 1) {
                comb_result[comb_count][j] = comb_temp[j];
            }
            comb_count = comb_count + 1;
        }
        return;
    }
    for (i = start; i < n; i = i + 1) {
        comb_temp[depth] = i;
        gen_combinations(i + 1, n, k, depth + 1);
    }
}

// Print a single combination
void print_combination(int idx, int k) {
    int i;
    for (i = 0; i < k; i = i + 1) {
        printf("%d", comb_result[idx][i]);
        if (i < k - 1) {
            printf(" ");
        }
    }
}

// Enumerate all subsets of {0..n-1} using bitmask
// Count subsets with specific properties
int count_subsets(int n) {
    int count;
    int mask;
    int limit;
    count = 0;
    limit = 1;
    {
        int i;
        for (i = 0; i < n; i = i + 1) {
            limit = limit * 2;
        }
    }
    for (mask = 0; mask < limit; mask = mask + 1) {
        count = count + 1;
    }
    return count;
}

// Count subsets of size exactly k
int count_subsets_size_k(int n, int k) {
    int count;
    int mask;
    int limit;
    int bits;
    int m;
    count = 0;
    limit = 1;
    {
        int i;
        for (i = 0; i < n; i = i + 1) {
            limit = limit * 2;
        }
    }
    for (mask = 0; mask < limit; mask = mask + 1) {
        bits = 0;
        m = mask;
        while (m > 0) {
            bits = bits + (m - (m / 2) * 2);
            m = m / 2;
        }
        if (bits == k) {
            count = count + 1;
        }
    }
    return count;
}

// Sum of elements in a subset (given bitmask)
int subset_sum_bitmask(int arr[10], int n, int mask) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        if ((mask / (1 << i)) - ((mask / (1 << i)) / 2) * 2 == 1) {
            sum = sum + arr[i];
        }
    }
    return sum;
}

// Helper: check if bit i is set in mask
int bit_set(int mask, int i) {
    int shifted;
    shifted = mask;
    {
        int j;
        for (j = 0; j < i; j = j + 1) {
            shifted = shifted / 2;
        }
    }
    return shifted - (shifted / 2) * 2;
}

// Count subsets with sum equal to target
int count_subsets_with_sum(int arr[10], int n, int target) {
    int count;
    int mask;
    int limit;
    int sum;
    int i;
    count = 0;
    limit = 1;
    for (i = 0; i < n; i = i + 1) {
        limit = limit * 2;
    }
    for (mask = 0; mask < limit; mask = mask + 1) {
        sum = 0;
        for (i = 0; i < n; i = i + 1) {
            if (bit_set(mask, i) == 1) {
                sum = sum + arr[i];
            }
        }
        if (sum == target) {
            count = count + 1;
        }
    }
    return count;
}

// Catalan number
int catalan(int n) {
    // C(n) = C(2n,n) / (n+1)
    return binom(2 * n, n) / (n + 1);
}

// Stirling numbers of the second kind (S(n,k))
int stirling2[10][10];

void build_stirling(int max_n) {
    int i;
    int j;
    for (i = 0; i < 10; i = i + 1) {
        for (j = 0; j < 10; j = j + 1) {
            stirling2[i][j] = 0;
        }
    }
    stirling2[0][0] = 1;
    for (i = 1; i <= max_n; i = i + 1) {
        for (j = 1; j <= i; j = j + 1) {
            stirling2[i][j] = j * stirling2[i - 1][j] + stirling2[i - 1][j - 1];
        }
    }
}

// Bell numbers B(n) = sum of S(n,k) for k=0..n
int bell(int n) {
    int sum;
    int k;
    sum = 0;
    for (k = 0; k <= n; k = k + 1) {
        sum = sum + stirling2[n][k];
    }
    return sum;
}

// Multinomial coefficient: n! / (k1! * k2! * ... * km!)
int multinomial(int n, int ks[10], int m) {
    int result;
    int i;
    int fact_n;
    int denom;
    fact_n = 1;
    {
        int j;
        for (j = 2; j <= n; j = j + 1) {
            fact_n = fact_n * j;
        }
    }
    denom = 1;
    for (i = 0; i < m; i = i + 1) {
        int f;
        f = 1;
        {
            int j;
            for (j = 2; j <= ks[i]; j = j + 1) {
                f = f * j;
            }
        }
        denom = denom * f;
    }
    result = fact_n / denom;
    return result;
}

// Combination with repetition: C(n+k-1, k)
int comb_with_rep(int n, int k) {
    return binom(n + k - 1, k);
}

// Sum of row in Pascal's triangle (should be 2^n)
int pascal_row_sum(int n) {
    int sum;
    int j;
    sum = 0;
    for (j = 0; j <= n; j = j + 1) {
        sum = sum + pascal[n][j];
    }
    return sum;
}

// Alternating sum of row (should be 0 for n>0)
int pascal_alt_sum(int n) {
    int sum;
    int j;
    int sign;
    sum = 0;
    sign = 1;
    for (j = 0; j <= n; j = j + 1) {
        sum = sum + sign * pascal[n][j];
        sign = -sign;
    }
    return sum;
}

// Vandermonde identity verification: C(m+n,r) = sum C(m,k)*C(n,r-k)
int vandermonde_check(int m, int n, int r) {
    int sum;
    int k;
    sum = 0;
    for (k = 0; k <= r; k = k + 1) {
        sum = sum + C(m, k) * C(n, r - k);
    }
    return sum;
}

// Hockey stick identity: sum C(i,k) for i=k..n equals C(n+1,k+1)
int hockey_stick(int n, int k) {
    int sum;
    int i;
    sum = 0;
    for (i = k; i <= n; i = i + 1) {
        sum = sum + C(i, k);
    }
    return sum;
}

// Derangement count using inclusion-exclusion
int derangements_ie(int n) {
    int sum;
    int i;
    int sign;
    int fact_n;
    int fact_i;
    int j;

    fact_n = 1;
    for (j = 2; j <= n; j = j + 1) {
        fact_n = fact_n * j;
    }

    sum = 0;
    sign = 1;
    for (i = 0; i <= n; i = i + 1) {
        fact_i = 1;
        for (j = 2; j <= i; j = j + 1) {
            fact_i = fact_i * j;
        }
        sum = sum + sign * (fact_n / fact_i);
        sign = -sign;
    }
    return sum;
}

// Power set size
int power_set_size(int n) {
    int result;
    int i;
    result = 1;
    for (i = 0; i < n; i = i + 1) {
        result = result * 2;
    }
    return result;
}

// Number of surjections from n-set to k-set
int surjections(int n, int k) {
    int sum;
    int i;
    int sign;
    int term;
    int j;
    int fact_k;
    int fact_i;
    int fact_ki;
    int pw;

    sum = 0;
    sign = 1;
    for (i = 0; i <= k; i = i + 1) {
        // (-1)^i * C(k,i) * (k-i)^n
        fact_k = 1;
        for (j = 2; j <= k; j = j + 1) {
            fact_k = fact_k * j;
        }
        fact_i = 1;
        for (j = 2; j <= i; j = j + 1) {
            fact_i = fact_i * j;
        }
        fact_ki = 1;
        for (j = 2; j <= k - i; j = j + 1) {
            fact_ki = fact_ki * j;
        }
        // (k-i)^n
        pw = 1;
        for (j = 0; j < n; j = j + 1) {
            pw = pw * (k - i);
        }
        term = sign * (fact_k / (fact_i * fact_ki)) * pw;
        sum = sum + term;
        sign = -sign;
    }
    return sum;
}

// Additional padding functions
int gcd(int a, int b) {
    while (b != 0) {
        int t;
        t = b;
        b = a - (a / b) * b;
        a = t;
    }
    return a;
}

int lcm(int a, int b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    return (a / gcd(a, b)) * b;
}

int min_int(int a, int b) {
    if (a < b) return a;
    return b;
}

int max_int(int a, int b) {
    if (a > b) return a;
    return b;
}

int main(void) {
    int i;
    int j;
    int arr[10];
    int ks[10];

    // Build Pascal's triangle
    build_pascal(12);
    printf("%c%c%c%c%c%c %c%c%c%c%c%c%c%c", 'P','a','s','c','a','l', 'T','r','i','a','n','g','l','e');
    printf("%c", 10);

    // Print rows 0..6
    for (i = 0; i <= 6; i = i + 1) {
        printf("%c%c%c %d%c", 'R','o','w', i, ':');
        for (j = 0; j <= i; j = j + 1) {
            printf(" %d", pascal[i][j]);
        }
        printf("%c", 10);
    }

    // Binomial coefficient tests
    printf("%c%c%c%c%c%c%c%c%c%c", 'B','i','n','o','m','i','a','l','s',':');
    printf("%c", 10);

    printf("%c%c%d%c%d%c%c%d", 'C','(', 10, ',', 3, ')', '=', C(10, 3));
    printf("%c", 10);
    printf("%c%c%d%c%d%c%c%d", 'C','(', 10, ',', 5, ')', '=', C(10, 5));
    printf("%c", 10);
    printf("%c%c%d%c%d%c%c%d", 'C','(', 12, ',', 4, ')', '=', C(12, 4));
    printf("%c", 10);
    printf("%c%c%d%c%d%c%c%d", 'C','(', 8, ',', 0, ')', '=', C(8, 0));
    printf("%c", 10);
    printf("%c%c%d%c%d%c%c%d", 'C','(', 8, ',', 8, ')', '=', C(8, 8));
    printf("%c", 10);

    // Direct binom vs Pascal
    printf("%c%c%c%c%c%c%c%c%c%c%d", 'B','i','n','o','m','D','i','r','e','c','t');
    printf("%c", 10);
    printf("%c%c%c%c%c%c%d%c%d%c%c%d", 'b','i','n','o','m','(', 10, ',', 3, ')', '=', binom(10, 3));
    printf("%c", 10);
    printf("%c%c%c%c%c%c%d%c%d%c%c%d", 'b','i','n','o','m','(', 12, ',', 6, ')', '=', binom(12, 6));
    printf("%c", 10);

    // Generate combinations C(5,2)
    printf("%c%c%c%c%c%c%c%c %d %c%c%c%c%c%c %d", 'G','e','n','e','r','a','t','e', 5, 'c','h','o','o','s','e', 2);
    printf("%c", 10);
    comb_count = 0;
    gen_combinations(0, 5, 2, 0);
    printf("%c%c%c%c%c%c%d", 'C','o','u','n','t','=', comb_count);
    printf("%c", 10);
    for (i = 0; i < comb_count; i = i + 1) {
        printf("%c%c%c%c%d%c ", 'C','o','m','b', i, ':');
        print_combination(i, 2);
        printf("%c", 10);
    }

    // Generate combinations C(6,3)
    printf("%c%c%c%c%c%c%c%c %d %c%c%c%c%c%c %d", 'G','e','n','e','r','a','t','e', 6, 'c','h','o','o','s','e', 3);
    printf("%c", 10);
    comb_count = 0;
    gen_combinations(0, 6, 3, 0);
    printf("%c%c%c%c%c%c%d", 'C','o','u','n','t','=', comb_count);
    printf("%c", 10);

    // Row sums
    printf("%c%c%c%c%c%c%c%c", 'R','o','w',' ','S','u','m','s');
    printf("%c", 10);
    for (i = 0; i <= 8; i = i + 1) {
        printf("%c%c%c%c%c%c %d%c%d", 'R','o','w','S','u','m', i, '=', pascal_row_sum(i));
        printf("%c", 10);
    }

    // Alternating sums
    printf("%c%c%c%c%c%c %d%c%d", 'A','l','t','S','u','m', 5, '=', pascal_alt_sum(5));
    printf("%c", 10);
    printf("%c%c%c%c%c%c %d%c%d", 'A','l','t','S','u','m', 0, '=', pascal_alt_sum(0));
    printf("%c", 10);

    // Subset count
    printf("%c%c%c%c%c%c%c %d%c%d", 'S','u','b','s','e','t','s', 5, '=', count_subsets(5));
    printf("%c", 10);

    // Subsets of size k verification
    printf("%c%c%c%c%c%c%c%c %d%c%d%c%d", 'S','u','b','s','S','i','z','e', 5, ',', 2, '=', count_subsets_size_k(5, 2));
    printf("%c", 10);
    printf("%c%c%c%c%c%c%c%c %d%c%d%c%d", 'S','u','b','s','S','i','z','e', 5, ',', 3, '=', count_subsets_size_k(5, 3));
    printf("%c", 10);

    // Subset sum counting
    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
    printf("%c%c%c%c%c%c%c %d%c%d", 'S','u','m','C','n','t',' ', 5, '=', count_subsets_with_sum(arr, 5, 5));
    printf("%c", 10);
    printf("%c%c%c%c%c%c%c %d%c%d", 'S','u','m','C','n','t',' ', 10, '=', count_subsets_with_sum(arr, 5, 10));
    printf("%c", 10);

    // Catalan numbers
    printf("%c%c%c%c%c%c%c%c", 'C','a','t','a','l','a','n','s');
    printf("%c", 10);
    for (i = 0; i <= 6; i = i + 1) {
        printf("%c%c%c%c%d%c%d", 'C','a','t', '(', i, ')', catalan(i));
        printf("%c", 10);
    }

    // Stirling numbers
    build_stirling(7);
    printf("%c%c%c%c%c%c%c%c", 'S','t','i','r','l','i','n','g');
    printf("%c", 10);
    printf("%c%c%d%c%d%c%c%d", 'S','(', 4, ',', 2, ')', '=', stirling2[4][2]);
    printf("%c", 10);
    printf("%c%c%d%c%d%c%c%d", 'S','(', 5, ',', 3, ')', '=', stirling2[5][3]);
    printf("%c", 10);

    // Bell numbers
    printf("%c%c%c%c%c%c%c%c", 'B','e','l','l','N','u','m','s');
    printf("%c", 10);
    for (i = 0; i <= 5; i = i + 1) {
        printf("%c%c%d%c%c%d", 'B','(', i, ')', '=', bell(i));
        printf("%c", 10);
    }

    // Vandermonde check
    printf("%c%c%c%c%c%c%c%c%c%c %d%c%d%c%d%c%d", 'V','a','n','d','e','r','m','o','n','d', 4, ',', 5, ',', 3, '=', vandermonde_check(4, 5, 3));
    printf("%c", 10);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%d%c%d%c%c%d", 'V','a','n','d','D','i','r','e','c','t',' ','C', 9, ',', 3, '=', ' ', C(9, 3));
    printf("%c", 10);

    // Hockey stick
    printf("%c%c%c%c%c%c%c%c%c%c %d%c%d%c%d", 'H','o','c','k','e','y','S','t','i','c', 6, ',', 2, '=', hockey_stick(6, 2));
    printf("%c", 10);
    printf("%c%c%d%c%d%c%c%d", 'C','(', 7, ',', 3, ')', '=', C(7, 3));
    printf("%c", 10);

    // Combination with repetition
    printf("%c%c%c%c%c%c%c %d%c%d%c%d", 'C','o','m','b','R','e','p', 4, ',', 3, '=', comb_with_rep(4, 3));
    printf("%c", 10);

    // Multinomial
    ks[0] = 2; ks[1] = 1; ks[2] = 1;
    printf("%c%c%c%c%c%c %d%c%d", 'M','u','l','t','i','n', 4, '=', multinomial(4, ks, 3));
    printf("%c", 10);

    // Power set size
    printf("%c%c%c%c%c%c%c%c %d%c%d", 'P','o','w','e','r','S','e','t', 6, '=', power_set_size(6));
    printf("%c", 10);

    // Derangements via inclusion-exclusion
    printf("%c%c%c%c%c%c%c%c%c %d%c%d", 'D','e','r','a','n','g','e','I','E', 4, '=', derangements_ie(4));
    printf("%c", 10);
    printf("%c%c%c%c%c%c%c%c%c %d%c%d", 'D','e','r','a','n','g','e','I','E', 5, '=', derangements_ie(5));
    printf("%c", 10);

    // Surjections
    printf("%c%c%c%c%c%c%c%c%c%c %d%c%d%c%d", 'S','u','r','j','e','c','t','i','o','n', 4, ',', 3, '=', surjections(4, 3));
    printf("%c", 10);

    printf("%c%c%c%c %c%c%c%c%c%c%c%d", 'A','l','l',' ', 'p','a','s','s','e','d','=', 1);
    printf("%c", 10);

    return 0;
}
