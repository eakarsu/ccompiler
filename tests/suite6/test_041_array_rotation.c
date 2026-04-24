int printf(const char *fmt, ...);

void print_array(int *a, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", a[i]);
    }
    printf("\n");
}

void reverse(int *a, int start, int end) {
    int temp;
    while (start < end) {
        temp = a[start];
        a[start] = a[end];
        a[end] = temp;
        start++;
        end--;
    }
}

void rotate_left(int *a, int n, int k) {
    k = k % n;
    if (k == 0) return;
    reverse(a, 0, k - 1);
    reverse(a, k, n - 1);
    reverse(a, 0, n - 1);
}

void rotate_right(int *a, int n, int k) {
    k = k % n;
    if (k == 0) return;
    reverse(a, 0, n - 1);
    reverse(a, 0, k - 1);
    reverse(a, k, n - 1);
}

void block_swap(int *a, int *b, int n) {
    int i;
    int temp;
    for (i = 0; i < n; i++) {
        temp = a[i];
        a[i] = b[i];
        b[i] = temp;
    }
}

void block_swap_rotate(int *a, int n, int d) {
    int i;
    int j;
    if (d == 0 || d == n) return;
    i = d;
    j = n - d;
    while (i != j) {
        if (i < j) {
            block_swap(a + d - i, a + d + j - i, i);
            j = j - i;
        } else {
            block_swap(a + d - i, a + d, j);
            i = i - j;
        }
    }
    block_swap(a + d - i, a + d, i);
}

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

void juggling_rotate(int *a, int n, int d) {
    int i;
    int j;
    int k;
    int temp;
    int g;
    d = d % n;
    if (d == 0) return;
    g = gcd(d, n);
    for (i = 0; i < g; i++) {
        temp = a[i];
        j = i;
        while (1) {
            k = j + d;
            if (k >= n) k = k - n;
            if (k == i) break;
            a[j] = a[k];
            j = k;
        }
        a[j] = temp;
    }
}

void copy_array(int *dst, int *src, int n) {
    int i;
    for (i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}

int main(void) {
    int a[7];
    int orig[7];
    int b[8];
    int c[5];

    orig[0] = 1; orig[1] = 2; orig[2] = 3; orig[3] = 4;
    orig[4] = 5; orig[5] = 6; orig[6] = 7;

    copy_array(a, orig, 7);
    rotate_left(a, 7, 2);
    // EXPECT: 3 4 5 6 7 1 2
    printf("%d %d %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4], a[5], a[6]);

    copy_array(a, orig, 7);
    rotate_left(a, 7, 0);
    // EXPECT: 1 2 3 4 5 6 7
    printf("%d %d %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4], a[5], a[6]);

    copy_array(a, orig, 7);
    rotate_left(a, 7, 7);
    // EXPECT: 1 2 3 4 5 6 7
    printf("%d %d %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4], a[5], a[6]);

    copy_array(a, orig, 7);
    rotate_right(a, 7, 2);
    // EXPECT: 6 7 1 2 3 4 5
    printf("%d %d %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4], a[5], a[6]);

    copy_array(a, orig, 7);
    rotate_right(a, 7, 5);
    // EXPECT: 3 4 5 6 7 1 2
    printf("%d %d %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4], a[5], a[6]);

    copy_array(a, orig, 7);
    rotate_left(a, 7, 9);
    // EXPECT: 3 4 5 6 7 1 2
    printf("%d %d %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4], a[5], a[6]);

    copy_array(a, orig, 7);
    block_swap_rotate(a, 7, 2);
    // EXPECT: 3 4 5 6 7 1 2
    printf("%d %d %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4], a[5], a[6]);

    copy_array(a, orig, 7);
    block_swap_rotate(a, 7, 3);
    // EXPECT: 4 5 6 7 1 2 3
    printf("%d %d %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4], a[5], a[6]);

    copy_array(a, orig, 7);
    juggling_rotate(a, 7, 2);
    // EXPECT: 3 4 5 6 7 1 2
    printf("%d %d %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4], a[5], a[6]);

    copy_array(a, orig, 7);
    juggling_rotate(a, 7, 4);
    // EXPECT: 5 6 7 1 2 3 4
    printf("%d %d %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4], a[5], a[6]);

    b[0] = 10; b[1] = 20; b[2] = 30; b[3] = 40;
    b[4] = 50; b[5] = 60; b[6] = 70; b[7] = 80;
    rotate_left(b, 8, 3);
    // EXPECT: 40 50 60 70 80 10 20 30
    printf("%d %d %d %d %d %d %d %d\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]);

    b[0] = 10; b[1] = 20; b[2] = 30; b[3] = 40;
    b[4] = 50; b[5] = 60; b[6] = 70; b[7] = 80;
    juggling_rotate(b, 8, 4);
    // EXPECT: 50 60 70 80 10 20 30 40
    printf("%d %d %d %d %d %d %d %d\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]);

    c[0] = 5; c[1] = 4; c[2] = 3; c[3] = 2; c[4] = 1;
    rotate_right(c, 5, 3);
    // EXPECT: 3 2 1 5 4
    printf("%d %d %d %d %d\n", c[0], c[1], c[2], c[3], c[4]);

    // EXPECT: gcd=6
    printf("gcd=%d\n", gcd(12, 18));

    // EXPECT: gcd=1
    printf("gcd=%d\n", gcd(7, 13));

    return 0;
}
