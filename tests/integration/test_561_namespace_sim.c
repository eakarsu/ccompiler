int printf(const char *fmt, ...);
// EXPECT: === math module ===\nabs(-42) = 42\nmax(10,20) = 20\nmin(10,20) = 10\nclamp(15,0,10) = 10\ngcd(48,36) = 12\n=== str module ===\nlen(hello) = 5\nequal(hello,hello) = 1\nequal(hello,world) = 0\nreverse(abcde) = edcba\n=== arr module ===\nsum = 23\nfind(8) = 2\nfind(99) = -1\ncount(3) = 3

/* Module: math_ */
int math_abs(int x) { return x < 0 ? -x : x; }
int math_max(int a, int b) { return a > b ? a : b; }
int math_min(int a, int b) { return a < b ? a : b; }
int math_clamp(int x, int lo, int hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}
int math_gcd(int a, int b) {
    a = math_abs(a);
    b = math_abs(b);
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Module: str_ */
int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) len++;
    return len;
}
int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}
void str_reverse(char *s, char *out) {
    int len = str_len(s);
    int i;
    for (i = 0; i < len; i++) {
        out[i] = s[len - 1 - i];
    }
    out[len] = 0;
}

/* Module: arr_ */
int arr_sum(int *a, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) s = s + a[i];
    return s;
}
int arr_find(int *a, int n, int val) {
    int i;
    for (i = 0; i < n; i++) {
        if (a[i] == val) return i;
    }
    return -1;
}
int arr_count(int *a, int n, int val) {
    int c = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (a[i] == val) c++;
    }
    return c;
}

int main(void) {
    printf("=== math module ===\n");
    printf("abs(-42) = %d\n", math_abs(-42));
    printf("max(10,20) = %d\n", math_max(10, 20));
    printf("min(10,20) = %d\n", math_min(10, 20));
    printf("clamp(15,0,10) = %d\n", math_clamp(15, 0, 10));
    printf("gcd(48,36) = %d\n", math_gcd(48, 36));

    printf("=== str module ===\n");
    char *hello = "hello";
    printf("len(hello) = %d\n", str_len(hello));
    printf("equal(hello,hello) = %d\n", str_equal(hello, "hello"));
    printf("equal(hello,world) = %d\n", str_equal(hello, "world"));
    char buf[10];
    str_reverse("abcde", buf);
    printf("reverse(abcde) = %s\n", buf);

    printf("=== arr module ===\n");
    int data[6];
    data[0] = 5; data[1] = 3; data[2] = 8; data[3] = 3; data[4] = 1; data[5] = 3;
    printf("sum = %d\n", arr_sum(data, 6));
    printf("find(8) = %d\n", arr_find(data, 6, 8));
    printf("find(99) = %d\n", arr_find(data, 6, 99));
    printf("count(3) = %d\n", arr_count(data, 6, 3));

    return 0;
}
