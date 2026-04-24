int printf(const char *fmt, ...);

/* Test 019: Image Rotation 90 Degrees
   Rotate a 5x5 integer image grid by 90, 180, and 270 degrees.
   Also test that 4 rotations of 90 degrees returns to original.
*/

int img[5][5];
int rot[5][5];
int tmp[5][5];

void init_image(void) {
    int i;
    int j;
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            img[i][j] = i * 10 + j;
        }
    }
}

void copy_to_tmp(int src[5][5]) {
    int i;
    int j;
    for (i = 0; i < 5; i = i + 1)
        for (j = 0; j < 5; j = j + 1)
            tmp[i][j] = src[i][j];
}

void rotate_90_cw(int src[5][5], int dst[5][5]) {
    int i;
    int j;
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            dst[j][4 - i] = src[i][j];
        }
    }
}

void rotate_180(int src[5][5], int dst[5][5]) {
    int i;
    int j;
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            dst[4 - i][4 - j] = src[i][j];
        }
    }
}

void rotate_270_cw(int src[5][5], int dst[5][5]) {
    int i;
    int j;
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            dst[4 - j][i] = src[i][j];
        }
    }
}

int images_equal(int a[5][5], int b[5][5]) {
    int i;
    int j;
    for (i = 0; i < 5; i = i + 1)
        for (j = 0; j < 5; j = j + 1)
            if (a[i][j] != b[i][j]) return 0;
    return 1;
}

void print_corners(int arr[5][5], const char *label) {
    printf("%s tl=%d tr=%d bl=%d br=%d\n",
        label, arr[0][0], arr[0][4], arr[4][0], arr[4][4]);
}

int main(void) {
    int eq;

    init_image();
    printf("orig_00=%d\n", img[0][0]);  // EXPECT: orig_00=0
    printf("orig_04=%d\n", img[0][4]);  // EXPECT: orig_04=4
    printf("orig_40=%d\n", img[4][0]);  // EXPECT: orig_40=40
    printf("orig_44=%d\n", img[4][4]);  // EXPECT: orig_44=44

    rotate_90_cw(img, rot);
    printf("rot90_00=%d\n", rot[0][0]);  // EXPECT: rot90_00=40
    printf("rot90_04=%d\n", rot[0][4]);  // EXPECT: rot90_04=0
    printf("rot90_40=%d\n", rot[4][0]);  // EXPECT: rot90_40=44
    printf("rot90_44=%d\n", rot[4][4]);  // EXPECT: rot90_44=4
    printf("rot90_22=%d\n", rot[2][2]);  // EXPECT: rot90_22=22

    rotate_180(img, rot);
    printf("rot180_00=%d\n", rot[0][0]);  // EXPECT: rot180_00=44
    printf("rot180_04=%d\n", rot[0][4]);  // EXPECT: rot180_04=40
    printf("rot180_40=%d\n", rot[4][0]);  // EXPECT: rot180_40=4
    printf("rot180_44=%d\n", rot[4][4]);  // EXPECT: rot180_44=0

    rotate_270_cw(img, rot);
    printf("rot270_00=%d\n", rot[0][0]);  // EXPECT: rot270_00=4
    printf("rot270_04=%d\n", rot[0][4]);  // EXPECT: rot270_04=44
    printf("rot270_40=%d\n", rot[4][0]);  // EXPECT: rot270_40=0
    printf("rot270_44=%d\n", rot[4][4]);  // EXPECT: rot270_44=40

    rotate_90_cw(img, rot);
    copy_to_tmp(rot);
    rotate_90_cw(tmp, rot);
    copy_to_tmp(rot);
    rotate_90_cw(tmp, rot);
    copy_to_tmp(rot);
    rotate_90_cw(tmp, rot);
    eq = images_equal(img, rot);
    printf("four_rotations_identity=%d\n", eq);  // EXPECT: four_rotations_identity=1

    rotate_90_cw(img, rot);
    copy_to_tmp(rot);
    rotate_270_cw(tmp, rot);
    eq = images_equal(img, rot);
    printf("rot90_then_rot270_identity=%d\n", eq);  // EXPECT: rot90_then_rot270_identity=1

    printf("test_019 passed\n");  // EXPECT: test_019 passed
    return 0;
}
