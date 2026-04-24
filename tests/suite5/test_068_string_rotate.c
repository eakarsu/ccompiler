int printf(const char *fmt, ...);

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) {
        n = n + 1;
    }
    return n;
}

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void reverse_range(char *s, int lo, int hi) {
    while (lo < hi) {
        char tmp = s[lo];
        s[lo] = s[hi];
        s[hi] = tmp;
        lo = lo + 1;
        hi = hi - 1;
    }
}

void rotate_left(char *s, int k) {
    int len = str_len(s);
    if (len == 0) return;
    k = k % len;
    if (k < 0) k = k + len;
    if (k == 0) return;
    reverse_range(s, 0, k - 1);
    reverse_range(s, k, len - 1);
    reverse_range(s, 0, len - 1);
}

void rotate_right(char *s, int k) {
    int len = str_len(s);
    if (len == 0) return;
    k = k % len;
    if (k < 0) k = k + len;
    if (k == 0) return;
    rotate_left(s, len - k);
}

int is_rotation(char *a, char *b) {
    int la = str_len(a);
    int lb = str_len(b);
    if (la != lb) return 0;
    if (la == 0) return 1;

    char doubled[80];
    int i = 0;
    while (i < la) {
        doubled[i] = a[i];
        i = i + 1;
    }
    int j = 0;
    while (j < la) {
        doubled[i] = a[j];
        i = i + 1;
        j = j + 1;
    }
    doubled[i] = 0;

    int dlen = str_len(doubled);
    int blen = lb;
    int k = 0;
    while (k <= dlen - blen) {
        int match = 1;
        int m = 0;
        while (m < blen) {
            if (doubled[k + m] != b[m]) {
                match = 0;
                break;
            }
            m = m + 1;
        }
        if (match) return 1;
        k = k + 1;
    }
    return 0;
}

int rotation_distance(char *a, char *b) {
    int la = str_len(a);
    int lb = str_len(b);
    if (la != lb) return -1;
    if (la == 0) return 0;

    char tmp[40];
    int i = 0;
    while (i < la) {
        str_copy(tmp, a);
        rotate_left(tmp, i);
        if (str_eq(tmp, b)) return i;
        i = i + 1;
    }
    return -1;
}

int main(void) {
    char buf[40];

    buf[0]='a'; buf[1]='b'; buf[2]='c';
    buf[3]='d'; buf[4]='e'; buf[5]=0;

    rotate_left(buf, 2);
    // EXPECT: rotate_left 2: cdeab
    printf("rotate_left 2: %s\n", buf);

    buf[0]='a'; buf[1]='b'; buf[2]='c';
    buf[3]='d'; buf[4]='e'; buf[5]=0;

    rotate_right(buf, 2);
    // EXPECT: rotate_right 2: deabc
    printf("rotate_right 2: %s\n", buf);

    buf[0]='a'; buf[1]='b'; buf[2]='c';
    buf[3]='d'; buf[4]='e'; buf[5]=0;

    rotate_left(buf, 5);
    // EXPECT: rotate_left 5: abcde
    printf("rotate_left 5: %s\n", buf);

    rotate_left(buf, 0);
    // EXPECT: rotate_left 0: abcde
    printf("rotate_left 0: %s\n", buf);

    rotate_left(buf, 1);
    // EXPECT: rotate_left 1: bcdea
    printf("rotate_left 1: %s\n", buf);

    char a1[10];
    a1[0]='a'; a1[1]='b'; a1[2]='c';
    a1[3]='d'; a1[4]='e'; a1[5]=0;

    char b1[10];
    b1[0]='c'; b1[1]='d'; b1[2]='e';
    b1[3]='a'; b1[4]='b'; b1[5]=0;

    // EXPECT: is_rotation abcde/cdeab: 1
    printf("is_rotation abcde/cdeab: %d\n",
           is_rotation(a1, b1));

    char b2[10];
    b2[0]='c'; b2[1]='d'; b2[2]='a';
    b2[3]='b'; b2[4]='e'; b2[5]=0;

    // EXPECT: is_rotation abcde/cdabe: 0
    printf("is_rotation abcde/cdabe: %d\n",
           is_rotation(a1, b2));

    char b3[6];
    b3[0]='a'; b3[1]='b'; b3[2]='c'; b3[3]=0;
    // EXPECT: is_rotation diff len: 0
    printf("is_rotation diff len: %d\n",
           is_rotation(a1, b3));

    // EXPECT: rot_dist abcde/cdeab: 2
    printf("rot_dist abcde/cdeab: %d\n",
           rotation_distance(a1, b1));

    // EXPECT: rot_dist same: 0
    printf("rot_dist same: %d\n",
           rotation_distance(a1, a1));

    // EXPECT: rot_dist not_rot: -1
    printf("rot_dist not_rot: %d\n",
           rotation_distance(a1, b2));

    char x1[10];
    x1[0]='x'; x1[1]='y'; x1[2]=0;
    char x2[10];
    x2[0]='y'; x2[1]='x'; x2[2]=0;
    // EXPECT: is_rotation xy/yx: 1
    printf("is_rotation xy/yx: %d\n",
           is_rotation(x1, x2));

    // EXPECT: rot_dist xy/yx: 1
    printf("rot_dist xy/yx: %d\n",
           rotation_distance(x1, x2));

    char single[5];
    single[0]='z'; single[1]=0;
    // EXPECT: is_rotation z/z: 1
    printf("is_rotation z/z: %d\n",
           is_rotation(single, single));

    char e1[5];
    e1[0] = 0;
    char e2[5];
    e2[0] = 0;
    // EXPECT: is_rotation empty: 1
    printf("is_rotation empty: %d\n",
           is_rotation(e1, e2));

    return 0;
}
