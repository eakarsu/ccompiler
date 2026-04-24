int printf(const char *fmt, ...);

/* Test 017: Erosion and Dilation (Morphological Operations)
   Binary image morphology on a 7x7 grid.
   Erosion: pixel is 1 only if all 3x3 neighbors are 1.
   Dilation: pixel is 1 if any 3x3 neighbor is 1.
*/

int img[7][7];
int out[7][7];

void clear_out(void) {
    int i;
    int j;
    for (i = 0; i < 7; i = i + 1)
        for (j = 0; j < 7; j = j + 1)
            out[i][j] = 0;
}

void clear_img(void) {
    int i;
    int j;
    for (i = 0; i < 7; i = i + 1)
        for (j = 0; j < 7; j = j + 1)
            img[i][j] = 0;
}

void init_image(void) {
    clear_img();
    img[2][2] = 1; img[2][3] = 1; img[2][4] = 1;
    img[3][2] = 1; img[3][3] = 1; img[3][4] = 1;
    img[4][2] = 1; img[4][3] = 1; img[4][4] = 1;
}

void copy_out_to_img(void) {
    int i;
    int j;
    for (i = 0; i < 7; i = i + 1)
        for (j = 0; j < 7; j = j + 1)
            img[i][j] = out[i][j];
}

int count_ones_img(void) {
    int i;
    int j;
    int c;
    c = 0;
    for (i = 0; i < 7; i = i + 1)
        for (j = 0; j < 7; j = j + 1)
            if (img[i][j] == 1) c = c + 1;
    return c;
}

int count_ones_out(void) {
    int i;
    int j;
    int c;
    c = 0;
    for (i = 0; i < 7; i = i + 1)
        for (j = 0; j < 7; j = j + 1)
            if (out[i][j] == 1) c = c + 1;
    return c;
}

void erode(void) {
    int i;
    int j;
    int ki;
    int kj;
    int all;
    int ni;
    int nj;

    clear_out();
    for (i = 1; i < 6; i = i + 1) {
        for (j = 1; j < 6; j = j + 1) {
            all = 1;
            for (ki = -1; ki <= 1; ki = ki + 1) {
                for (kj = -1; kj <= 1; kj = kj + 1) {
                    ni = i + ki;
                    nj = j + kj;
                    if (img[ni][nj] == 0) {
                        all = 0;
                    }
                }
            }
            out[i][j] = all;
        }
    }
}

void dilate(void) {
    int i;
    int j;
    int ki;
    int kj;
    int any;
    int ni;
    int nj;

    clear_out();
    for (i = 1; i < 6; i = i + 1) {
        for (j = 1; j < 6; j = j + 1) {
            any = 0;
            for (ki = -1; ki <= 1; ki = ki + 1) {
                for (kj = -1; kj <= 1; kj = kj + 1) {
                    ni = i + ki;
                    nj = j + kj;
                    if (img[ni][nj] == 1) {
                        any = 1;
                    }
                }
            }
            out[i][j] = any;
        }
    }
}

int main(void) {
    int orig;
    int eroded;
    int dilated;

    init_image();
    orig = count_ones_img();
    printf("original=%d\n", orig);  // EXPECT: original=9

    erode();
    eroded = count_ones_out();
    printf("eroded=%d\n", eroded);  // EXPECT: eroded=1
    printf("eroded_center=%d\n", out[3][3]);  // EXPECT: eroded_center=1
    printf("eroded_22=%d\n", out[2][2]);  // EXPECT: eroded_22=0

    init_image();
    dilate();
    dilated = count_ones_out();
    printf("dilated=%d\n", dilated);  // EXPECT: dilated=25
    printf("dilated_11=%d\n", out[1][1]);  // EXPECT: dilated_11=1
    printf("dilated_55=%d\n", out[5][5]);  // EXPECT: dilated_55=1
    printf("dilated_00=%d\n", out[0][0]);  // EXPECT: dilated_00=0

    copy_out_to_img();
    erode();
    printf("dilate_then_erode=%d\n", count_ones_out());  // EXPECT: dilate_then_erode=9
    printf("close_center=%d\n", out[3][3]);  // EXPECT: close_center=1

    init_image();
    erode();
    copy_out_to_img();
    dilate();
    printf("erode_then_dilate=%d\n", count_ones_out());  // EXPECT: erode_then_dilate=9
    printf("open_center=%d\n", out[3][3]);  // EXPECT: open_center=1

    printf("test_017 passed\n");  // EXPECT: test_017 passed
    return 0;
}
