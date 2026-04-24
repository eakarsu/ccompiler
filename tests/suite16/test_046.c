int printf(const char *fmt, ...);

// String Rotation detection and generation

int str_len(char *s) {
    int i = 0;
    while (s[i] != 0) {
        i = i + 1;
    }
    return i;
}

int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

// Rotate string left by k positions, result goes into out
void rotate_left(char *s, int n, int k, char *out) {
    k = k % n;
    if (k < 0) k = k + n;
    int i = 0;
    while (i < n) {
        out[i] = s[(i + k) % n];
        i = i + 1;
    }
    out[n] = 0;
}

// Rotate string right by k positions
void rotate_right(char *s, int n, int k, char *out) {
    k = k % n;
    if (k < 0) k = k + n;
    rotate_left(s, n, n - k, out);
}

// Check if b is a rotation of a using concatenation method
int is_rotation(char *a, int na, char *b, int nb) {
    if (na != nb) return 0;
    if (na == 0) return 1;

    // Build a+a in concat buffer (max 40 chars)
    char concat[42];
    int i = 0;
    while (i < na) {
        concat[i] = a[i];
        i = i + 1;
    }
    int j = 0;
    while (j < na) {
        concat[na + j] = a[j];
        j = j + 1;
    }
    int total = na + na;

    // Search for b in concat
    i = 0;
    while (i <= total - nb) {
        int match = 1;
        j = 0;
        while (j < nb) {
            if (concat[i + j] != b[j]) {
                match = 0;
                j = nb; // break
            }
            j = j + 1;
        }
        if (match) return 1;
        i = i + 1;
    }
    return 0;
}

// Find rotation amount: how many left rotations to turn a into b
int find_rotation_amount(char *a, int na, char *b, int nb) {
    if (na != nb) return -1;
    char rotated[22];
    int k = 0;
    while (k < na) {
        rotate_left(a, na, k, rotated);
        if (str_equal(rotated, b)) {
            return k;
        }
        k = k + 1;
    }
    return -1;
}

void test_rotate_left() {
    char s[8];
    char out[8];
    s[0]='A'; s[1]='B'; s[2]='C'; s[3]='D'; s[4]='E'; s[5]=0;
    rotate_left(s, 5, 2, out);
    printf("Left 2: %s\n", out); // EXPECT: Left 2: CDEAB

    rotate_left(s, 5, 0, out);
    printf("Left 0: %s\n", out); // EXPECT: Left 0: ABCDE

    rotate_left(s, 5, 5, out);
    printf("Left 5: %s\n", out); // EXPECT: Left 5: ABCDE
}

void test_rotate_right() {
    char s[8];
    char out[8];
    s[0]='A'; s[1]='B'; s[2]='C'; s[3]='D'; s[4]='E'; s[5]=0;
    rotate_right(s, 5, 2, out);
    printf("Right 2: %s\n", out); // EXPECT: Right 2: DEABC

    rotate_right(s, 5, 1, out);
    printf("Right 1: %s\n", out); // EXPECT: Right 1: EABCD
}

void test_is_rotation() {
    char a[8]; char b[8];
    a[0]='A'; a[1]='B'; a[2]='C'; a[3]='D'; a[4]=0;
    b[0]='C'; b[1]='D'; b[2]='A'; b[3]='B'; b[4]=0;
    printf("ABCD rot CDAB: %d\n", is_rotation(a, 4, b, 4)); // EXPECT: ABCD rot CDAB: 1

    char c[8];
    c[0]='A'; c[1]='C'; c[2]='B'; c[3]='D'; c[4]=0;
    printf("ABCD rot ACBD: %d\n", is_rotation(a, 4, c, 4)); // EXPECT: ABCD rot ACBD: 0

    char d[6];
    d[0]='A'; d[1]='B'; d[2]='C'; d[3]='D'; d[4]='E'; d[5]=0;
    printf("Diff len: %d\n", is_rotation(a, 4, d, 5)); // EXPECT: Diff len: 0
}

void test_find_rotation() {
    char a[8]; char b[8];
    a[0]='A'; a[1]='B'; a[2]='C'; a[3]='D'; a[4]=0;
    b[0]='C'; b[1]='D'; b[2]='A'; b[3]='B'; b[4]=0;
    int r = find_rotation_amount(a, 4, b, 4);
    printf("Rotation amount: %d\n", r); // EXPECT: Rotation amount: 2

    b[0]='A'; b[1]='B'; b[2]='C'; b[3]='D'; b[4]=0;
    r = find_rotation_amount(a, 4, b, 4);
    printf("Same rotation: %d\n", r); // EXPECT: Same rotation: 0

    b[0]='D'; b[1]='A'; b[2]='B'; b[3]='C'; b[4]=0;
    r = find_rotation_amount(a, 4, b, 4);
    printf("Rotation by 3: %d\n", r); // EXPECT: Rotation by 3: 3
}

void test_all_rotations() {
    char s[4];
    char out[4];
    s[0]='X'; s[1]='Y'; s[2]='Z'; s[3]=0;
    rotate_left(s, 3, 0, out);
    printf("Rot0: %s\n", out); // EXPECT: Rot0: XYZ
    rotate_left(s, 3, 1, out);
    printf("Rot1: %s\n", out); // EXPECT: Rot1: YZX
    rotate_left(s, 3, 2, out);
    printf("Rot2: %s\n", out); // EXPECT: Rot2: ZXY
}

void test_self_rotation() {
    char s[4];
    s[0]='A'; s[1]='A'; s[2]='A'; s[3]=0;
    printf("All same is rot: %d\n", is_rotation(s, 3, s, 3)); // EXPECT: All same is rot: 1
    int r = find_rotation_amount(s, 3, s, 3);
    printf("All same rot amt: %d\n", r); // EXPECT: All same rot amt: 0
}

int main() {
    test_rotate_left();
    test_rotate_right();
    test_is_rotation();
    test_find_rotation();
    test_all_rotations();
    test_self_rotation();
    printf("String rotation done\n"); // EXPECT: String rotation done
    return 0;
}
