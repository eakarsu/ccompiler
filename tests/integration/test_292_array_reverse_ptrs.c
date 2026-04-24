int printf(const char *fmt, ...);
// EXPECT: original: 1 2 3 4 5 6\nreversed: 6 5 4 3 2 1\nodd reversed: 50 40 30 20 10\nsingle: 99\nrange [2,5] reversed: 1 2 6 5 4 3 7 8\ndouble reverse: 11 22 33 44\nbefore: hello world foo\nwords reversed: foo world hello\nwords reversed: fox brown quick the
// Test: array reversal via pointers

void print_arr(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

void reverse_ptrs(int *arr, int n) {
    int *lo = arr;
    int *hi = arr + n - 1;
    while (lo < hi) {
        int tmp = *lo;
        *lo = *hi;
        *hi = tmp;
        lo = lo + 1;
        hi = hi - 1;
    }
}

// Reverse a substring of array
void reverse_range(int *arr, int start, int end) {
    int *lo = arr + start;
    int *hi = arr + end;
    while (lo < hi) {
        int tmp = *lo;
        *lo = *hi;
        *hi = tmp;
        lo = lo + 1;
        hi = hi - 1;
    }
}

// Reverse words in a sentence (char array)
void reverse_chars(char *s, int lo, int hi) {
    while (lo < hi) {
        char tmp = s[lo];
        s[lo] = s[hi];
        s[hi] = tmp;
        lo = lo + 1;
        hi = hi - 1;
    }
}

int my_strlen(char *s) {
    int i = 0;
    while (s[i] != '\0') i = i + 1;
    return i;
}

void reverse_words(char *s) {
    int len = my_strlen(s);
    // Reverse entire string
    reverse_chars(s, 0, len - 1);
    // Reverse each word
    int start = 0;
    int i;
    for (i = 0; i <= len; i++) {
        if (i == len || s[i] == ' ') {
            reverse_chars(s, start, i - 1);
            start = i + 1;
        }
    }
}

int main(void) {
    // Basic reverse
    int a[6];
    a[0]=1; a[1]=2; a[2]=3; a[3]=4; a[4]=5; a[5]=6;
    printf("original: ");
    print_arr(a, 6);
    reverse_ptrs(a, 6);
    printf("reversed: ");
    print_arr(a, 6);

    // Odd length
    int b[5];
    b[0]=10; b[1]=20; b[2]=30; b[3]=40; b[4]=50;
    reverse_ptrs(b, 5);
    printf("odd reversed: ");
    print_arr(b, 5);

    // Single element
    int c[1];
    c[0] = 99;
    reverse_ptrs(c, 1);
    printf("single: ");
    print_arr(c, 1);

    // Reverse range
    int d[8];
    d[0]=1; d[1]=2; d[2]=3; d[3]=4; d[4]=5; d[5]=6; d[6]=7; d[7]=8;
    reverse_range(d, 2, 5);
    printf("range [2,5] reversed: ");
    print_arr(d, 8);

    // Double reverse = original
    int e[4];
    e[0]=11; e[1]=22; e[2]=33; e[3]=44;
    reverse_ptrs(e, 4);
    reverse_ptrs(e, 4);
    printf("double reverse: ");
    print_arr(e, 4);

    // Reverse words in sentence
    char sent[30];
    char *src = "hello world foo";
    int i = 0;
    while (src[i] != '\0') { sent[i] = src[i]; i = i + 1; }
    sent[i] = '\0';
    printf("before: %s\n", sent);
    reverse_words(sent);
    printf("words reversed: %s\n", sent);

    // Another sentence
    char s2[40];
    char *src2 = "the quick brown fox";
    i = 0;
    while (src2[i] != '\0') { s2[i] = src2[i]; i = i + 1; }
    s2[i] = '\0';
    reverse_words(s2);
    printf("words reversed: %s\n", s2);

    return 0;
}
