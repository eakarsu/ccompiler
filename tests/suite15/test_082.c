int printf(const char *fmt, ...);

/* Linear regression (integer least squares)
 * Computes y = a*x + b using integer arithmetic.
 * All values scaled by 1000 for precision.
 */

typedef struct {
    int slope;     /* a * 1000 */
    int intercept; /* b * 1000 */
} LinReg;

/* Compute linear regression: y = slope*x + intercept
 * x and y arrays contain raw (unscaled) values.
 * slope and intercept are returned scaled by 1000.
 */
void linear_fit(int *x, int *y, int n, LinReg *result) {
    int sum_x;
    int sum_y;
    int sum_xy;
    int sum_xx;
    int i;
    int denom;

    sum_x = 0;
    sum_y = 0;
    sum_xy = 0;
    sum_xx = 0;

    i = 0;
    while (i < n) {
        sum_x = sum_x + x[i];
        sum_y = sum_y + y[i];
        sum_xy = sum_xy + x[i] * y[i];
        sum_xx = sum_xx + x[i] * x[i];
        i = i + 1;
    }

    denom = n * sum_xx - sum_x * sum_x;

    if (denom == 0) {
        result->slope = 0;
        result->intercept = 0;
        return;
    }

    /* slope = (n * sum_xy - sum_x * sum_y) / denom, scaled by 1000 */
    result->slope = (n * sum_xy - sum_x * sum_y) * 1000 / denom;
    /* intercept = (sum_y * sum_xx - sum_x * sum_xy) / denom, scaled by 1000 */
    result->intercept = (sum_y * sum_xx - sum_x * sum_xy) * 1000 / denom;
}

/* Predict y*1000 given x (unscaled) and regression result */
int predict(LinReg *reg, int x) {
    return reg->slope * x + reg->intercept;
}

/* Compute R^2 * 1000 (coefficient of determination) */
int r_squared(int *x, int *y, int n, LinReg *reg) {
    int mean_y;
    int ss_tot;
    int ss_res;
    int i;
    int pred;
    int diff;
    int sum_y;

    sum_y = 0;
    i = 0;
    while (i < n) {
        sum_y = sum_y + y[i];
        i = i + 1;
    }
    /* mean_y scaled by 1000 */
    mean_y = sum_y * 1000 / n;

    ss_tot = 0;
    ss_res = 0;
    i = 0;
    while (i < n) {
        pred = predict(reg, x[i]);
        diff = y[i] * 1000 - pred;
        ss_res = ss_res + diff * diff / 1000;

        diff = y[i] * 1000 - mean_y;
        ss_tot = ss_tot + diff * diff / 1000;
        i = i + 1;
    }

    if (ss_tot == 0) return 1000;
    return 1000 - (ss_res * 1000 / ss_tot);
}

int main(void) {
    int x1[5];
    int y1[5];
    int x2[6];
    int y2[6];
    int x3[4];
    int y3[4];
    LinReg reg;
    int pred;
    int r2;
    int i;

    printf("Linear Regression Tests\n");
    // EXPECT: Linear Regression Tests

    /* Test 1: Perfect linear relationship y = 2x + 1 */
    x1[0] = 1; y1[0] = 3;
    x1[1] = 2; y1[1] = 5;
    x1[2] = 3; y1[2] = 7;
    x1[3] = 4; y1[3] = 9;
    x1[4] = 5; y1[4] = 11;

    linear_fit(x1, y1, 5, &reg);
    printf("Test1 slope: %d\n", reg.slope);
    // EXPECT: Test1 slope: 2000
    printf("Test1 intercept: %d\n", reg.intercept);
    // EXPECT: Test1 intercept: 1000

    pred = predict(&reg, 10);
    printf("Predict x=10: %d\n", pred);
    // EXPECT: Predict x=10: 21000

    r2 = r_squared(x1, y1, 5, &reg);
    printf("R2 perfect: %d\n", r2);
    // EXPECT: R2 perfect: 1000

    /* Test 2: Noisy data */
    x2[0] = 0;  y2[0] = 1;
    x2[1] = 1;  y2[1] = 4;
    x2[2] = 2;  y2[2] = 5;
    x2[3] = 3;  y2[3] = 8;
    x2[4] = 4;  y2[4] = 9;
    x2[5] = 5;  y2[5] = 13;

    linear_fit(x2, y2, 6, &reg);
    printf("Test2 slope: %d\n", reg.slope);
    // EXPECT: Test2 slope: 2228
    printf("Test2 intercept: %d\n", reg.intercept);
    // EXPECT: Test2 intercept: 1095

    pred = predict(&reg, 6);
    printf("Predict x=6: %d\n", pred);
    // EXPECT: Predict x=6: 14463

    r2 = r_squared(x2, y2, 6, &reg);
    printf("R2 noisy: %d\n", r2);
    // EXPECT: R2 noisy: 973

    /* Test 3: Negative slope y = -3x + 20 */
    x3[0] = 1; y3[0] = 17;
    x3[1] = 2; y3[1] = 14;
    x3[2] = 3; y3[2] = 11;
    x3[3] = 4; y3[3] = 8;

    linear_fit(x3, y3, 4, &reg);
    printf("Test3 slope: %d\n", reg.slope);
    // EXPECT: Test3 slope: -3000
    printf("Test3 intercept: %d\n", reg.intercept);
    // EXPECT: Test3 intercept: 20000

    pred = predict(&reg, 0);
    printf("Predict x=0: %d\n", pred);
    // EXPECT: Predict x=0: 20000

    pred = predict(&reg, 7);
    printf("Predict x=7: %d\n", pred);
    // EXPECT: Predict x=7: -1000

    /* Test 4: Horizontal line y = 5 */
    i = 0;
    while (i < 5) {
        x1[i] = i;
        y1[i] = 5;
        i = i + 1;
    }

    linear_fit(x1, y1, 5, &reg);
    printf("Horizontal slope: %d\n", reg.slope);
    // EXPECT: Horizontal slope: 0
    printf("Horizontal intercept: %d\n", reg.intercept);
    // EXPECT: Horizontal intercept: 5000

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
