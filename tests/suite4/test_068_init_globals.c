int printf(const char *fmt, ...);

// Test various global initializations: zero, positive, negative, char arrays with strings.

// Zero-initialized (BSS)
int g_zero1;
int g_zero2;
int g_zero_arr[5];

// Positive initializations
int g_one = 1;
int g_hundred = 100;
int g_large = 999999;
int g_max = 2147483647;

// Negative initializations
int g_neg1 = -1;
int g_neg100 = -100;
int g_neg_large = -999999;
int g_min = -2147483647;

// Char arrays with string initialization
char g_hello[] = "hello";
char g_empty[] = "";
char g_abc[] = "abc";
char g_long[] = "the quick brown fox";

// Integer arrays with initialization
int g_seq[5] = {1, 2, 3, 4, 5};
int g_neg_arr[3] = {-10, -20, -30};
int g_mixed[4] = {100, -200, 300, -400};

// Struct initialization
struct Pair {
    int first;
    int second;
};

struct Pair g_pair = {10, 20};
struct Pair g_neg_pair = {-5, -15};
struct Pair g_pairs[3];

// Char variables
char g_ch_a = 65;
char g_ch_z = 122;

int sum_arr(int *arr, int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        total = total + arr[i];
    }
    return total;
}

int strlen_manual(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

int check_zero_arr(void) {
    int i;
    for (i = 0; i < 5; i = i + 1) {
        if (g_zero_arr[i] != 0) return 0;
    }
    return 1;
}

int main(void) {
    // Zero-initialized globals
    // EXPECT: 0
    printf("%d\n", g_zero1);
    // EXPECT: 0
    printf("%d\n", g_zero2);
    // EXPECT: 1
    printf("%d\n", check_zero_arr());

    // Positive initializations
    // EXPECT: 1
    printf("%d\n", g_one);
    // EXPECT: 100
    printf("%d\n", g_hundred);
    // EXPECT: 999999
    printf("%d\n", g_large);
    // EXPECT: 2147483647
    printf("%d\n", g_max);

    // Negative initializations
    // EXPECT: -1
    printf("%d\n", g_neg1);
    // EXPECT: -100
    printf("%d\n", g_neg100);
    // EXPECT: -999999
    printf("%d\n", g_neg_large);
    // EXPECT: -2147483647
    printf("%d\n", g_min);

    // Char arrays
    // EXPECT: hello
    printf("%s\n", g_hello);
    // EXPECT: 5
    printf("%d\n", strlen_manual(g_hello));

    // EXPECT: 0
    printf("%d\n", strlen_manual(g_empty));

    // EXPECT: abc
    printf("%s\n", g_abc);
    // EXPECT: 3
    printf("%d\n", strlen_manual(g_abc));

    // EXPECT: the quick brown fox
    printf("%s\n", g_long);
    // EXPECT: 19
    printf("%d\n", strlen_manual(g_long));

    // Integer array initialization
    // sum = 1+2+3+4+5 = 15
    // EXPECT: 15
    printf("%d\n", sum_arr(g_seq, 5));

    // EXPECT: 1
    printf("%d\n", g_seq[0]);
    // EXPECT: 5
    printf("%d\n", g_seq[4]);

    // Negative array
    // sum = -10+-20+-30 = -60
    // EXPECT: -60
    printf("%d\n", sum_arr(g_neg_arr, 3));

    // Mixed array
    // sum = 100-200+300-400 = -200
    // EXPECT: -200
    printf("%d\n", sum_arr(g_mixed, 4));
    // EXPECT: 100
    printf("%d\n", g_mixed[0]);
    // EXPECT: -400
    printf("%d\n", g_mixed[3]);

    // Struct initialization
    // EXPECT: 10
    printf("%d\n", g_pair.first);
    // EXPECT: 20
    printf("%d\n", g_pair.second);
    // EXPECT: -5
    printf("%d\n", g_neg_pair.first);
    // EXPECT: -15
    printf("%d\n", g_neg_pair.second);

    // Uninitialized struct array should be zero
    // EXPECT: 0
    printf("%d\n", g_pairs[0].first);
    // EXPECT: 0
    printf("%d\n", g_pairs[2].second);

    // Char variables (ASCII)
    // 'A' = 65, 'z' = 122
    // EXPECT: 65
    printf("%d\n", g_ch_a);
    // EXPECT: 122
    printf("%d\n", g_ch_z);

    // Verify modifying initialized globals works
    g_one = 1000;
    // EXPECT: 1000
    printf("%d\n", g_one);

    g_neg1 = 1;
    // EXPECT: 1
    printf("%d\n", g_neg1);

    return 0;
}
