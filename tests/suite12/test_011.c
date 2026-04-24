int printf(const char *fmt, ...);

/* Test 011: 2D Convolution Kernel
   Apply a 3x3 convolution kernel to a 5x5 integer image grid.
   Kernel: simple averaging (all 1s, divide by 9)
*/

int img[5][5];
int out[5][5];
int kernel[3][3];

void init_image(void) {
    int i;
    int j;
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            img[i][j] = (i + 1) * (j + 1);
        }
    }
}

void init_kernel(void) {
    int i;
    int j;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 3; j = j + 1) {
            kernel[i][j] = 1;
        }
    }
}

void convolve(void) {
    int i;
    int j;
    int ki;
    int kj;
    int sum;
    int ni;
    int nj;

    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            if (i == 0 || i == 4 || j == 0 || j == 4) {
                out[i][j] = img[i][j];
            } else {
                sum = 0;
                for (ki = -1; ki <= 1; ki = ki + 1) {
                    for (kj = -1; kj <= 1; kj = kj + 1) {
                        ni = i + ki;
                        nj = j + kj;
                        sum = sum + img[ni][nj] * kernel[ki + 1][kj + 1];
                    }
                }
                out[i][j] = sum / 9;
            }
        }
    }
}

void print_image(int arr[5][5], const char *label) {
    int i;
    int j;
    printf("%s\n", label);
    for (i = 0; i < 5; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            if (j > 0) printf(" ");
            printf("%d", arr[i][j]);
        }
        printf("\n");
    }
}

// EXPECT: convolution done
void verify_convolution(void) {
    int center;
    center = out[2][2];
    printf("center=%d\n", center);  // EXPECT: center=9
    printf("corner=%d\n", out[0][0]);  // EXPECT: corner=1
    printf("edge=%d\n", out[0][2]);  // EXPECT: edge=3
}

void test_identity_kernel(void) {
    int id[3][3];
    int i;
    int j;
    int sum;
    for (i = 0; i < 3; i = i + 1)
        for (j = 0; j < 3; j = j + 1)
            id[i][j] = 0;
    id[1][1] = 1;

    sum = 0;
    for (i = -1; i <= 1; i = i + 1) {
        for (j = -1; j <= 1; j = j + 1) {
            sum = sum + img[2 + i][2 + j] * id[i + 1][j + 1];
        }
    }
    printf("identity=%d\n", sum);  // EXPECT: identity=9
}

void test_weighted_kernel(void) {
    int wk[3][3];
    int i;
    int j;
    int sum;
    for (i = 0; i < 3; i = i + 1)
        for (j = 0; j < 3; j = j + 1)
            wk[i][j] = 1;
    wk[1][1] = 4;

    sum = 0;
    for (i = -1; i <= 1; i = i + 1) {
        for (j = -1; j <= 1; j = j + 1) {
            sum = sum + img[2 + i][2 + j] * wk[i + 1][j + 1];
        }
    }
    printf("weighted_sum=%d\n", sum);  // EXPECT: weighted_sum=108
    printf("weighted_avg=%d\n", sum / 12);  // EXPECT: weighted_avg=9
}

int main(void) {
    init_image();
    init_kernel();
    convolve();
    printf("convolution done\n");
    verify_convolution();
    test_identity_kernel();
    test_weighted_kernel();
    printf("test_011 passed\n");  // EXPECT: test_011 passed
    return 0;
}
