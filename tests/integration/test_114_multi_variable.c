int printf(const char *fmt, ...);
// EXPECT: sum 1..8 = 36\nprod 1..4 = 24\nweighted = 30\npoly(3) = 103\nchain: x=10 y=15 z=30 w=20\nrotated: x=2 y=3 z=1\nreversed: 8 7 6 5\nsum of squares: 204\nmin=1 max=8\ncomplex expr = 27\ndot product = 70
// Test: multi-variable expressions

int main(void) {
    int a, b, c, d, e, f, g, h;
    int r1, r2, r3;
    int arr[8];
    int i;
    int x, y, z, w;

    /* Initialize many variables */
    a = 1; b = 2; c = 3; d = 4;
    e = 5; f = 6; g = 7; h = 8;

    /* Sum of all */
    r1 = a + b + c + d + e + f + g + h;
    printf("sum 1..8 = %d\n", r1);

    /* Product of first four */
    r2 = a * b * c * d;
    printf("prod 1..4 = %d\n", r2);

    /* Weighted sum */
    r3 = a*1 + b*2 + c*3 + d*4;
    printf("weighted = %d\n", r3);

    /* Polynomial: 2x^3 + 3x^2 + 5x + 7 for x=3 */
    x = 3;
    r1 = 2*x*x*x + 3*x*x + 5*x + 7;
    printf("poly(3) = %d\n", r1);

    /* Multiple assignments in sequence */
    x = 10;
    y = x + 5;
    z = y * 2;
    w = z - x;
    printf("chain: x=%d y=%d z=%d w=%d\n", x, y, z, w);

    /* Swap three variables circularly */
    x = 1; y = 2; z = 3;
    w = x; x = y; y = z; z = w;
    printf("rotated: x=%d y=%d z=%d\n", x, y, z);

    /* Fill array from variables */
    arr[0] = a; arr[1] = b; arr[2] = c; arr[3] = d;
    arr[4] = e; arr[5] = f; arr[6] = g; arr[7] = h;

    /* Reverse into different vars */
    x = arr[7]; y = arr[6]; z = arr[5]; w = arr[4];
    printf("reversed: %d %d %d %d\n", x, y, z, w);

    /* Accumulate from multiple sources */
    r1 = 0;
    for (i = 0; i < 8; i++) {
        r1 += arr[i] * arr[i];
    }
    printf("sum of squares: %d\n", r1);

    /* Min and max of many values */
    {
        int min_val, max_val;
        min_val = arr[0];
        max_val = arr[0];
        for (i = 1; i < 8; i++) {
            if (arr[i] < min_val) min_val = arr[i];
            if (arr[i] > max_val) max_val = arr[i];
        }
        printf("min=%d max=%d\n", min_val, max_val);
    }

    /* Expression with all variables */
    r1 = (a + b) * (c - d) + (e * f) - (g / h);
    printf("complex expr = %d\n", r1);

    /* Dot product of two "vectors" */
    r1 = a*e + b*f + c*g + d*h;
    printf("dot product = %d\n", r1);

    return 0;
}
