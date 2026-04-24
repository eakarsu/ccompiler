int printf(const char *fmt, ...);

void print_arr(int *a, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");
        printf("%d", a[i]);
    }
    printf("\n");
}

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void move_zeros_to_end(int *a, int n) {
    int pos = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (a[i] != 0) {
            a[pos] = a[i];
            pos++;
        }
    }
    while (pos < n) {
        a[pos] = 0;
        pos++;
    }
}

void alternate_pos_neg(int *a, int n) {
    int pos[20];
    int neg[20];
    int pc = 0;
    int nc = 0;
    int i;
    int pi;
    int ni;

    for (i = 0; i < n; i++) {
        if (a[i] >= 0) {
            pos[pc] = a[i];
            pc++;
        } else {
            neg[nc] = a[i];
            nc++;
        }
    }

    pi = 0;
    ni = 0;
    i = 0;
    while (pi < pc && ni < nc) {
        a[i] = pos[pi];
        pi++;
        i++;
        a[i] = neg[ni];
        ni++;
        i++;
    }
    while (pi < pc) {
        a[i] = pos[pi];
        pi++;
        i++;
    }
    while (ni < nc) {
        a[i] = neg[ni];
        ni++;
        i++;
    }
}

void segregate_even_odd(int *a, int n) {
    int left = 0;
    int right = n - 1;
    while (left < right) {
        while (left < right && (a[left] % 2) == 0) left++;
        while (left < right && (a[right] % 2) != 0) right--;
        if (left < right) {
            swap(a + left, a + right);
            left++;
            right--;
        }
    }
}

void stable_segregate_even_odd(int *a, int n) {
    int temp[20];
    int i;
    int idx = 0;
    for (i = 0; i < n; i++) {
        if ((a[i] % 2) == 0) {
            temp[idx] = a[i];
            idx++;
        }
    }
    for (i = 0; i < n; i++) {
        if ((a[i] % 2) != 0) {
            temp[idx] = a[i];
            idx++;
        }
    }
    for (i = 0; i < n; i++) {
        a[i] = temp[i];
    }
}

int main(void) {
    int a[8];

    a[0] = 0; a[1] = 1; a[2] = 0; a[3] = 3;
    a[4] = 12; a[5] = 0; a[6] = 4; a[7] = 0;
    move_zeros_to_end(a, 8);
    // EXPECT: 1 3 12 4 0 0 0 0
    print_arr(a, 8);

    a[0] = 1; a[1] = 2; a[2] = 3; a[3] = 4;
    a[4] = 5; a[5] = 6; a[6] = 7; a[7] = 8;
    move_zeros_to_end(a, 8);
    // EXPECT: 1 2 3 4 5 6 7 8
    print_arr(a, 8);

    a[0] = 0; a[1] = 0; a[2] = 0; a[3] = 0;
    a[4] = 0; a[5] = 0; a[6] = 0; a[7] = 0;
    move_zeros_to_end(a, 8);
    // EXPECT: 0 0 0 0 0 0 0 0
    print_arr(a, 8);

    int b[6];
    b[0] = 1; b[1] = -2; b[2] = 3; b[3] = -4; b[4] = 5; b[5] = -6;
    alternate_pos_neg(b, 6);
    // EXPECT: 1 -2 3 -4 5 -6
    print_arr(b, 6);

    b[0] = -1; b[1] = -2; b[2] = 3; b[3] = 4; b[4] = 5; b[5] = -6;
    alternate_pos_neg(b, 6);
    // EXPECT: 3 -1 4 -2 5 -6
    print_arr(b, 6);

    int c[5];
    c[0] = -1; c[1] = -2; c[2] = -3; c[3] = -4; c[4] = -5;
    alternate_pos_neg(c, 5);
    // EXPECT: -1 -2 -3 -4 -5
    print_arr(c, 5);

    c[0] = 1; c[1] = 2; c[2] = 3; c[3] = 4; c[4] = 5;
    alternate_pos_neg(c, 5);
    // EXPECT: 1 2 3 4 5
    print_arr(c, 5);

    int d[8];
    d[0] = 12; d[1] = 34; d[2] = 45; d[3] = 9;
    d[4] = 8; d[5] = 90; d[6] = 3; d[7] = 7;
    stable_segregate_even_odd(d, 8);
    // EXPECT: 12 34 8 90 45 9 3 7
    print_arr(d, 8);

    d[0] = 1; d[1] = 3; d[2] = 5; d[3] = 7;
    d[4] = 2; d[5] = 4; d[6] = 6; d[7] = 8;
    stable_segregate_even_odd(d, 8);
    // EXPECT: 2 4 6 8 1 3 5 7
    print_arr(d, 8);

    d[0] = 2; d[1] = 4; d[2] = 6; d[3] = 8;
    d[4] = 10; d[5] = 12; d[6] = 14; d[7] = 16;
    stable_segregate_even_odd(d, 8);
    // EXPECT: 2 4 6 8 10 12 14 16
    print_arr(d, 8);

    int e[6];
    e[0] = 3; e[1] = 6; e[2] = 9; e[3] = 2; e[4] = 8; e[5] = 1;
    segregate_even_odd(e, 6);
    int i;
    int even_end = -1;
    int valid = 1;
    for (i = 0; i < 6; i++) {
        if ((e[i] % 2) == 0) {
            if (even_end >= 0 && i > even_end + 1) {
                int j;
                for (j = even_end + 1; j < i; j++) {
                    if ((e[j] % 2) != 0) valid = 0;
                }
            }
            even_end = i;
        }
    }
    // EXPECT: segregated=1
    printf("segregated=%d\n", valid);

    int f[4];
    f[0] = 0; f[1] = 5; f[2] = 0; f[3] = 3;
    move_zeros_to_end(f, 4);
    // EXPECT: 5 3 0 0
    print_arr(f, 4);

    int g[3];
    g[0] = 7; g[1] = 0; g[2] = 0;
    move_zeros_to_end(g, 3);
    // EXPECT: 7 0 0
    print_arr(g, 3);

    int h[4];
    h[0] = 2; h[1] = -3; h[2] = 4; h[3] = -1;
    alternate_pos_neg(h, 4);
    // EXPECT: 2 -3 4 -1
    print_arr(h, 4);

    // EXPECT: done
    printf("done\n");

    return 0;
}
