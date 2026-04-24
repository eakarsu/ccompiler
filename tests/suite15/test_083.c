int printf(const char *fmt, ...);

/* Gradient descent optimization (integer-scaled, 1D)
 * Minimize f(x) = (x - target)^2 using gradient descent.
 * All values scaled by 1000.
 * gradient of f = 2*(x - target)
 * learning_rate scaled by 1000
 */

/* Compute f(x) = (x - target)^2 / 1000  (scaled to avoid overflow) */
int cost_func(int x, int target) {
    int diff;
    diff = x - target;
    return diff * diff / 1000;
}

/* Compute gradient: 2*(x - target) */
int gradient(int x, int target) {
    return 2 * (x - target);
}

/* One step of gradient descent: x_new = x - lr * grad / 1000 */
int gd_step(int x, int target, int lr) {
    int grad;
    grad = gradient(x, target);
    return x - lr * grad / 1000;
}

/* Run gradient descent for n iterations */
int gradient_descent(int x0, int target, int lr, int n) {
    int x;
    int i;
    x = x0;
    i = 0;
    while (i < n) {
        x = gd_step(x, target, lr);
        i = i + 1;
    }
    return x;
}

/* Minimize f(x) = x^2 - 6000*x + 9000000 = (x-3000)^2
 * Gradient = 2*x - 6000
 */
int quad_gradient(int x) {
    return 2 * x - 6000;
}

int quad_gd(int x0, int lr, int n) {
    int x;
    int i;
    int grad;
    x = x0;
    i = 0;
    while (i < n) {
        grad = quad_gradient(x);
        x = x - lr * grad / 1000;
        i = i + 1;
    }
    return x;
}

/* Minimize f(x) = a*x^2 + b*x + c
 * Gradient = 2*a*x + b
 * Minimum at x = -b / (2*a)
 */
int poly_gd(int x0, int a, int b, int lr, int n) {
    int x;
    int i;
    int grad;
    x = x0;
    i = 0;
    while (i < n) {
        grad = 2 * a * x / 1000 + b;
        x = x - lr * grad / 1000;
        i = i + 1;
    }
    return x;
}

/* Absolute value */
int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int main(void) {
    int result;
    int cost;
    int i;
    int x;
    int history[10];

    printf("Gradient Descent Tests\n");
    // EXPECT: Gradient Descent Tests

    /* Test 1: Simple target tracking, target=5000 */
    result = gradient_descent(0, 5000, 100, 50);
    printf("GD target 5000: %d\n", result);
    // EXPECT: GD target 5000: 4996

    /* Test 2: Starting above target */
    result = gradient_descent(10000, 3000, 200, 30);
    printf("GD from above: %d\n", result);
    // EXPECT: GD from above: 3002

    /* Test 3: Track convergence history */
    x = 0;
    i = 0;
    while (i < 10) {
        x = gd_step(x, 8000, 300);
        history[i] = x;
        i = i + 1;
    }

    printf("Step 1: %d\n", history[0]);
    // EXPECT: Step 1: 4800
    printf("Step 2: %d\n", history[1]);
    // EXPECT: Step 2: 6720
    printf("Step 3: %d\n", history[2]);
    // EXPECT: Step 3: 7488
    printf("Step 5: %d\n", history[4]);
    // EXPECT: Step 5: 7918
    printf("Step 10: %d\n", history[9]);
    // EXPECT: Step 10: 7998

    /* Test 4: Quadratic f(x) = (x-3000)^2, min at x=3000 */
    result = quad_gd(0, 100, 100);
    printf("Quad min: %d\n", result);
    // EXPECT: Quad min: 2996

    /* Test 5: Starting from negative */
    result = gradient_descent(-10000, 2000, 150, 80);
    printf("From negative: %d\n", result);
    // EXPECT: From negative: 1997

    /* Test 6: Very small learning rate - slow convergence */
    result = gradient_descent(0, 5000, 10, 20);
    printf("Small LR: %d\n", result);
    // EXPECT: Small LR: 1655

    /* Test 7: Polynomial a=2, b=-8000, min at x = 8000/(2*2) = 2000 */
    result = poly_gd(0, 2000, -8000, 50, 200);
    printf("Poly min: %d\n", result);
    // EXPECT: Poly min: 1996

    /* Test 8: Cost at optimum vs start */
    cost = cost_func(0, 5000);
    printf("Cost at 0: %d\n", cost);
    // EXPECT: Cost at 0: 25000

    cost = cost_func(4999, 5000);
    printf("Cost at 4999: %d\n", cost);
    // EXPECT: Cost at 4999: 0

    cost = cost_func(5000, 5000);
    printf("Cost at opt: %d\n", cost);
    // EXPECT: Cost at opt: 0

    /* Test 9: Negative target */
    result = gradient_descent(0, -4000, 200, 50);
    printf("Neg target: %d\n", result);
    // EXPECT: Neg target: -3998

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
