int printf(const char *fmt, ...);

// Levenshtein distance (edit distance) computation
// Uses dynamic programming with a 2-row optimization

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) {
        n = n + 1;
    }
    return n;
}

int min2(int a, int b) {
    if (a < b) return a;
    return b;
}

int min3(int a, int b, int c) {
    return min2(min2(a, b), c);
}

// Levenshtein distance using full matrix (for small strings)
int levenshtein(char *s1, char *s2) {
    int len1 = str_len(s1);
    int len2 = str_len(s2);
    int dp[32][32];
    int i;
    int j;

    for (i = 0; i <= len1; i = i + 1) {
        dp[i][0] = i;
    }
    for (j = 0; j <= len2; j = j + 1) {
        dp[0][j] = j;
    }

    for (i = 1; i <= len1; i = i + 1) {
        for (j = 1; j <= len2; j = j + 1) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + min3(
                    dp[i - 1][j],     // deletion
                    dp[i][j - 1],     // insertion
                    dp[i - 1][j - 1]  // substitution
                );
            }
        }
    }
    return dp[len1][len2];
}

// Check if distance is within threshold (early termination)
int within_distance(char *s1, char *s2, int threshold) {
    int dist = levenshtein(s1, s2);
    if (dist <= threshold) return 1;
    return 0;
}

// Find closest match in a word list
struct WordMatch {
    int index;
    int distance;
};

void find_closest(char *target, char *words[8], int nwords, struct WordMatch *result) {
    int best_dist = 999;
    int best_idx = -1;
    int i;
    int d;

    for (i = 0; i < nwords; i = i + 1) {
        d = levenshtein(target, words[i]);
        if (d < best_dist) {
            best_dist = d;
            best_idx = i;
        }
    }
    result->index = best_idx;
    result->distance = best_dist;
}

void test_basic() {
    int d;

    d = levenshtein("", "");
    printf("empty-empty: %d\n", d);
    // EXPECT: empty-empty: 0

    d = levenshtein("abc", "");
    printf("abc-empty: %d\n", d);
    // EXPECT: abc-empty: 3

    d = levenshtein("", "xyz");
    printf("empty-xyz: %d\n", d);
    // EXPECT: empty-xyz: 3

    d = levenshtein("abc", "abc");
    printf("same: %d\n", d);
    // EXPECT: same: 0

    d = levenshtein("a", "b");
    printf("single sub: %d\n", d);
    // EXPECT: single sub: 1
}

void test_operations() {
    int d;

    // Substitution
    d = levenshtein("kitten", "sitten");
    printf("kitten-sitten: %d\n", d);
    // EXPECT: kitten-sitten: 1

    // Full classic example
    d = levenshtein("kitten", "sitting");
    printf("kitten-sitting: %d\n", d);
    // EXPECT: kitten-sitting: 3

    // Insertion
    d = levenshtein("abc", "abcd");
    printf("insert: %d\n", d);
    // EXPECT: insert: 1

    // Deletion
    d = levenshtein("abcd", "abc");
    printf("delete: %d\n", d);
    // EXPECT: delete: 1

    // Transposition (counts as 2 in Levenshtein)
    d = levenshtein("ab", "ba");
    printf("transpose: %d\n", d);
    // EXPECT: transpose: 2

    d = levenshtein("sunday", "saturday");
    printf("sunday-saturday: %d\n", d);
    // EXPECT: sunday-saturday: 3
}

void test_threshold() {
    int r;

    r = within_distance("hello", "helo", 1);
    printf("hello-helo d1: %d\n", r);
    // EXPECT: hello-helo d1: 1

    r = within_distance("hello", "world", 2);
    printf("hello-world d2: %d\n", r);
    // EXPECT: hello-world d2: 0

    r = within_distance("cat", "car", 1);
    printf("cat-car d1: %d\n", r);
    // EXPECT: cat-car d1: 1

    r = within_distance("test", "tent", 1);
    printf("test-tent d1: %d\n", r);
    // EXPECT: test-tent d1: 1

    r = within_distance("abc", "xyz", 2);
    printf("abc-xyz d2: %d\n", r);
    // EXPECT: abc-xyz d2: 0
}

void test_closest() {
    char *words[8];
    struct WordMatch result;

    words[0] = "apple";
    words[1] = "orange";
    words[2] = "banana";
    words[3] = "grape";

    find_closest("aple", words, 4, &result);
    printf("closest to aple: idx=%d dist=%d\n", result.index, result.distance);
    // EXPECT: closest to aple: idx=0 dist=1

    find_closest("ornge", words, 4, &result);
    printf("closest to ornge: idx=%d dist=%d\n", result.index, result.distance);
    // EXPECT: closest to ornge: idx=1 dist=1

    find_closest("banan", words, 4, &result);
    printf("closest to banan: idx=%d dist=%d\n", result.index, result.distance);
    // EXPECT: closest to banan: idx=2 dist=1

    find_closest("grpe", words, 4, &result);
    printf("closest to grpe: idx=%d dist=%d\n", result.index, result.distance);
    // EXPECT: closest to grpe: idx=3 dist=1

    find_closest("xyz", words, 4, &result);
    printf("closest to xyz: idx=%d dist=%d\n", result.index, result.distance);
    // EXPECT: closest to xyz: idx=0 dist=5
}

int main() {
    test_basic();
    test_operations();
    test_threshold();
    test_closest();

    printf("levenshtein tests done\n");
    // EXPECT: levenshtein tests done
    return 0;
}
