int printf(const char *fmt, ...);

/* Simplified Simplex method for 2-variable linear programming
 * Maximize c1*x1 + c2*x2
 * Subject to constraints: a[i]*x1 + b[i]*x2 <= d[i]
 * All values scaled by 100 to avoid floating point.
 *
 * Uses a tabular simplex approach with integer arithmetic.
 */

typedef struct {
    int a[6];    /* coefficients for x1 */
    int b[6];    /* coefficients for x2 */
    int rhs[6]; /* right hand side */
    int c1;      /* objective x1 coefficient * 100 */
    int c2;      /* objective x2 coefficient * 100 */
    int ncon;    /* number of constraints */
} LP;

/* Find optimal x1, x2 by enumerating vertices of feasible region.
 * For 2 variables, the optimum is at a vertex of the polygon.
 * Check all intersections of constraint boundaries + axes.
 * Returns optimal objective value * 100.
 */
int solve_lp(LP *lp, int *opt_x1, int *opt_x2) {
    int best;
    int bx1;
    int bx2;
    int i;
    int j;
    int x1;
    int x2;
    int obj;
    int feasible;
    int k;
    int det;

    best = -1;
    bx1 = 0;
    bx2 = 0;

    /* Check origin */
    feasible = 1;
    k = 0;
    while (k < lp->ncon) {
        if (0 > lp->rhs[k] * 100) feasible = 0;
        k = k + 1;
    }
    if (feasible) {
        best = 0;
        bx1 = 0;
        bx2 = 0;
    }

    /* Check intersections of each pair of constraints */
    i = 0;
    while (i < lp->ncon) {
        j = i + 1;
        while (j < lp->ncon) {
            /* Solve: a[i]*x1 + b[i]*x2 = rhs[i]*100
             *        a[j]*x1 + b[j]*x2 = rhs[j]*100
             */
            det = lp->a[i] * lp->b[j] - lp->a[j] * lp->b[i];
            if (det != 0) {
                x1 = (lp->rhs[i] * 100 * lp->b[j] - lp->rhs[j] * 100 * lp->b[i]) / det;
                x2 = (lp->a[i] * lp->rhs[j] * 100 - lp->a[j] * lp->rhs[i] * 100) / det;

                if (x1 >= 0 && x2 >= 0) {
                    /* Check feasibility against all constraints */
                    feasible = 1;
                    k = 0;
                    while (k < lp->ncon) {
                        if (lp->a[k] * x1 + lp->b[k] * x2 > lp->rhs[k] * 100 + 1) {
                            feasible = 0;
                        }
                        k = k + 1;
                    }
                    if (feasible) {
                        obj = lp->c1 * x1 + lp->c2 * x2;
                        if (obj > best) {
                            best = obj;
                            bx1 = x1;
                            bx2 = x2;
                        }
                    }
                }
            }
            j = j + 1;
        }

        /* Also check intersection with x1=0 axis */
        if (lp->b[i] != 0) {
            x1 = 0;
            x2 = lp->rhs[i] * 100 / lp->b[i];
            if (x2 >= 0) {
                feasible = 1;
                k = 0;
                while (k < lp->ncon) {
                    if (lp->b[k] * x2 > lp->rhs[k] * 100 + 1) feasible = 0;
                    k = k + 1;
                }
                if (feasible) {
                    obj = lp->c2 * x2;
                    if (obj > best) {
                        best = obj;
                        bx1 = 0;
                        bx2 = x2;
                    }
                }
            }
        }

        /* Check intersection with x2=0 axis */
        if (lp->a[i] != 0) {
            x1 = lp->rhs[i] * 100 / lp->a[i];
            x2 = 0;
            if (x1 >= 0) {
                feasible = 1;
                k = 0;
                while (k < lp->ncon) {
                    if (lp->a[k] * x1 > lp->rhs[k] * 100 + 1) feasible = 0;
                    k = k + 1;
                }
                if (feasible) {
                    obj = lp->c1 * x1;
                    if (obj > best) {
                        best = obj;
                        bx1 = x1;
                        bx2 = 0;
                    }
                }
            }
        }

        i = i + 1;
    }

    *opt_x1 = bx1;
    *opt_x2 = bx2;
    return best / 100;
}

int main(void) {
    LP lp;
    int x1;
    int x2;
    int obj;

    printf("Simplex LP Tests\n");
    // EXPECT: Simplex LP Tests

    /* Test 1: Max 3*x1 + 2*x2
     * s.t. x1 + x2 <= 4
     *      x1 + 3*x2 <= 6
     * Optimal: x1=3, x2=1, obj=11 */
    lp.c1 = 3;
    lp.c2 = 2;
    lp.ncon = 2;
    lp.a[0] = 1; lp.b[0] = 1; lp.rhs[0] = 4;
    lp.a[1] = 1; lp.b[1] = 3; lp.rhs[1] = 6;

    obj = solve_lp(&lp, &x1, &x2);
    printf("T1 x1: %d\n", x1);
    // EXPECT: T1 x1: 400
    printf("T1 x2: %d\n", x2);
    // EXPECT: T1 x2: 0
    printf("T1 obj: %d\n", obj);
    // EXPECT: T1 obj: 12

    /* Test 2: Max 5*x1 + 4*x2
     * s.t. 6*x1 + 4*x2 <= 24
     *      x1 + 2*x2 <= 6
     * Optimal: x1=3, x2=1.5 -> x1=300, x2=150, obj=21 */
    lp.c1 = 5;
    lp.c2 = 4;
    lp.ncon = 2;
    lp.a[0] = 6; lp.b[0] = 4; lp.rhs[0] = 24;
    lp.a[1] = 1; lp.b[1] = 2; lp.rhs[1] = 6;

    obj = solve_lp(&lp, &x1, &x2);
    printf("T2 x1: %d\n", x1);
    // EXPECT: T2 x1: 300
    printf("T2 x2: %d\n", x2);
    // EXPECT: T2 x2: 150
    printf("T2 obj: %d\n", obj);
    // EXPECT: T2 obj: 21

    /* Test 3: Max x1 + x2
     * s.t. x1 <= 5
     *      x2 <= 4
     *      x1 + x2 <= 8
     * Optimal: x1=4, x2=4 or x1=5, x2=3, obj=8 */
    lp.c1 = 1;
    lp.c2 = 1;
    lp.ncon = 3;
    lp.a[0] = 1; lp.b[0] = 0; lp.rhs[0] = 5;
    lp.a[1] = 0; lp.b[1] = 1; lp.rhs[1] = 4;
    lp.a[2] = 1; lp.b[2] = 1; lp.rhs[2] = 8;

    obj = solve_lp(&lp, &x1, &x2);
    printf("T3 obj: %d\n", obj);
    // EXPECT: T3 obj: 8

    /* Test 4: Degenerate - objective along edge
     * Max 2*x1 + x2
     * s.t. 2*x1 + x2 <= 10
     *      x1 <= 4
     * Optimal: x1=4, x2=2, obj=10 */
    lp.c1 = 2;
    lp.c2 = 1;
    lp.ncon = 2;
    lp.a[0] = 2; lp.b[0] = 1; lp.rhs[0] = 10;
    lp.a[1] = 1; lp.b[1] = 0; lp.rhs[1] = 4;

    obj = solve_lp(&lp, &x1, &x2);
    printf("T4 x1: %d\n", x1);
    // EXPECT: T4 x1: 400
    printf("T4 x2: %d\n", x2);
    // EXPECT: T4 x2: 200
    printf("T4 obj: %d\n", obj);
    // EXPECT: T4 obj: 10

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
