int printf(const char *fmt, ...);

/* Test 016: Sharpen Filter
   Apply sharpening kernel: center=5, neighbors=-1
   to enhance edges in a 6x6 integer image.
*/

int img[6][6];
int out[6][6];

void init_image(void) {
    int i;
    int j;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            img[i][j] = 50;
        }
    }
    img[2][2] = 100;
    img[2][3] = 100;
    img[3][2] = 100;
    img[3][3] = 100;
}

int clamp(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

void sharpen(void) {
    int sk[3][3];
    int i;
    int j;
    int ki;
    int kj;
    int sum;
    int ni;
    int nj;

    sk[0][0] = 0;  sk[0][1] = -1; sk[0][2] = 0;
    sk[1][0] = -1; sk[1][1] = 5;  sk[1][2] = -1;
    sk[2][0] = 0;  sk[2][1] = -1; sk[2][2] = 0;

    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            if (i == 0 || i == 5 || j == 0 || j == 5) {
                out[i][j] = img[i][j];
            } else {
                sum = 0;
                for (ki = -1; ki <= 1; ki = ki + 1) {
                    for (kj = -1; kj <= 1; kj = kj + 1) {
                        ni = i + ki;
                        nj = j + kj;
                        sum = sum + img[ni][nj] * sk[ki + 1][kj + 1];
                    }
                }
                out[i][j] = clamp(sum, 0, 255);
            }
        }
    }
}

int max_pixel(void) {
    int i;
    int j;
    int mx;
    mx = 0;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            if (out[i][j] > mx) mx = out[i][j];
        }
    }
    return mx;
}

int min_pixel(void) {
    int i;
    int j;
    int mn;
    mn = 999;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            if (out[i][j] < mn) mn = out[i][j];
        }
    }
    return mn;
}

// EXPECT: sharpened
// EXPECT: max=200
// EXPECT: min=0
void check_flat_region(void) {
    int val;
    val = out[1][1];
    printf("flat_pixel=%d\n", val);  // EXPECT: flat_pixel=50
}

void check_edge_pixels(void) {
    printf("sharp_22=%d\n", out[2][2]);  // EXPECT: sharp_22=200
    printf("sharp_33=%d\n", out[3][3]);  // EXPECT: sharp_33=200
    printf("sharp_23=%d\n", out[2][3]);  // EXPECT: sharp_23=200
    printf("sharp_32=%d\n", out[3][2]);  // EXPECT: sharp_32=200
}

void check_boundary_of_bright(void) {
    printf("near_12=%d\n", out[1][2]);  // EXPECT: near_12=0
    printf("near_21=%d\n", out[2][1]);  // EXPECT: near_21=0
    printf("near_42=%d\n", out[4][2]);  // EXPECT: near_42=0
    printf("near_34=%d\n", out[3][4]);  // EXPECT: near_34=0
}

int sum_image(void) {
    int i;
    int j;
    int s;
    s = 0;
    for (i = 0; i < 6; i = i + 1) {
        for (j = 0; j < 6; j = j + 1) {
            s = s + out[i][j];
        }
    }
    return s;
}

int main(void) {
    init_image();
    sharpen();
    printf("sharpened\n");
    printf("max=%d\n", max_pixel());
    printf("min=%d\n", min_pixel());
    check_flat_region();
    check_edge_pixels();
    check_boundary_of_bright();
    printf("sum=%d\n", sum_image());  // EXPECT: sum=2000
    printf("test_016 passed\n");  // EXPECT: test_016 passed
    return 0;
}
