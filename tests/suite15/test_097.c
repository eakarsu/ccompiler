int printf(const char *fmt, ...);

/* Convex Hull Trick
   Maintain a set of lines y = m*x + b.
   For a query x, find the line with minimum y value.
   Lines are added with decreasing slopes for efficiency. */

struct Line {
    int m;  /* slope */
    int b;  /* intercept */
};

struct Line hull[32];
int hull_size;

void hull_init(void) {
    hull_size = 0;
}

/* Check if line at index mid is unnecessary given prev and next */
int bad_line(int l1, int l2, int l3) {
    /* Line l2 is unnecessary if intersection of l1 and l3
       is below or at l2 at the intersection x.
       l1 and l3 intersect at x = (b3-b1)/(m1-m3)
       l2 at that x: m2*(b3-b1)/(m1-m3) + b2
       l1 at that x: m1*(b3-b1)/(m1-m3) + b1
       l2 is bad if (b3-b1)*(m1-m2) <= (b2-b1)*(m1-m3)
       Using cross-multiplication to avoid division */
    int m1 = hull[l1].m;
    int b1 = hull[l1].b;
    int m2 = hull[l2].m;
    int b2 = hull[l2].b;
    int m3 = hull[l3].m;
    int b3 = hull[l3].b;

    /* For minimum queries with decreasing slopes:
       l2 is unnecessary if
       (b3 - b1) * (m1 - m2) <= (b2 - b1) * (m1 - m3) */
    return (b3 - b1) * (m1 - m2) <= (b2 - b1) * (m1 - m3);
}

void add_line(int m, int b) {
    hull[hull_size].m = m;
    hull[hull_size].b = b;
    hull_size = hull_size + 1;

    /* Remove unnecessary lines */
    while (hull_size >= 3 && bad_line(hull_size - 3, hull_size - 2, hull_size - 1)) {
        /* Remove the second-to-last line */
        hull[hull_size - 2] = hull[hull_size - 1];
        hull_size = hull_size - 1;
    }
}

int eval_line(int idx, int x) {
    return hull[idx].m * x + hull[idx].b;
}

/* Query minimum y at point x. Brute force search over hull. */
int query_min(int x) {
    int best;
    int val;
    int i;
    if (hull_size == 0) return 999999;

    best = eval_line(0, x);
    for (i = 1; i < hull_size; i++) {
        val = eval_line(i, x);
        if (val < best) best = val;
    }
    return best;
}

/* Brute force check: evaluate all original lines */
struct Line all_lines[32];
int all_count;

void all_init(void) {
    all_count = 0;
}

void all_add(int m, int b) {
    all_lines[all_count].m = m;
    all_lines[all_count].b = b;
    all_count = all_count + 1;
}

int all_query_min(int x) {
    int best;
    int val;
    int i;
    if (all_count == 0) return 999999;

    best = all_lines[0].m * x + all_lines[0].b;
    for (i = 1; i < all_count; i++) {
        val = all_lines[i].m * x + all_lines[i].b;
        if (val < best) best = val;
    }
    return best;
}

void test_basic(void) {
    printf("=== Basic Convex Hull Trick ===\n");
    // EXPECT: === Basic Convex Hull Trick ===

    hull_init();
    all_init();

    /* Lines (decreasing slope): y=3x+0, y=2x+1, y=x+3, y=0x+10 */
    add_line(3, 0);  all_add(3, 0);
    add_line(2, 1);  all_add(2, 1);
    add_line(1, 3);  all_add(1, 3);
    add_line(0, 10); all_add(0, 10);

    /* At x=0: min(0,1,3,10) = 0 (line y=3x+0) */
    printf("Query x=0: %d\n", query_min(0));
    // EXPECT: Query x=0: 0

    /* At x=1: min(3,3,4,10) = 3 (lines y=3x+0 and y=2x+1 tie) */
    printf("Query x=1: %d\n", query_min(1));
    // EXPECT: Query x=1: 3

    /* At x=2: min(6,5,5,10) = 5 (lines y=2x+1 and y=x+3 tie) */
    printf("Query x=2: %d\n", query_min(2));
    // EXPECT: Query x=2: 5

    /* At x=5: min(15,11,8,10) = 8 (line y=x+3) */
    printf("Query x=5: %d\n", query_min(5));
    // EXPECT: Query x=5: 8

    /* At x=10: min(30,21,13,10) = 10 (line y=0x+10) */
    printf("Query x=10: %d\n", query_min(10));
    // EXPECT: Query x=10: 10
}

void test_verification(void) {
    int x;
    int hull_val;
    int brute_val;
    int all_match;

    printf("=== Verification ===\n");
    // EXPECT: === Verification ===

    /* Verify hull queries match brute force for all test points */
    all_match = 1;
    for (x = 0; x <= 20; x++) {
        hull_val = query_min(x);
        brute_val = all_query_min(x);
        if (hull_val != brute_val) {
            all_match = 0;
        }
    }
    printf("All match brute force: %d\n", all_match);
    // EXPECT: All match brute force: 1

    printf("Hull size: %d\n", hull_size);
    // EXPECT: Hull size: 4
    /* All 4 lines are on the lower envelope:
       y=3x+0 best for x<=1, y=2x+1 best around x=2,
       y=x+3 best around x=3-7, y=0x+10 best for x>=8 */
}

void test_redundant_lines(void) {
    printf("=== Redundant Lines ===\n");
    // EXPECT: === Redundant Lines ===

    hull_init();
    /* Lines: y=5x+0, y=3x+100, y=x+0
       At check for middle line (3x+100):
       bad_line(0,1,2): (0-0)*(5-3) <= (100-0)*(5-1)
       0 <= 400 -> true. So 3x+100 is removed.
       Hull has 2 lines: y=5x+0 and y=x+0 */
    add_line(5, 0);
    add_line(3, 100);
    add_line(1, 0);

    printf("Hull after redundant: %d\n", hull_size);
    // EXPECT: Hull after redundant: 2

    /* x=0: min(0, 0) = 0 */
    printf("Query x=0: %d\n", query_min(0));
    // EXPECT: Query x=0: 0
    /* x=10: min(50, 10) = 10 */
    printf("Query x=10: %d\n", query_min(10));
    // EXPECT: Query x=10: 10
}

void test_dp_application(void) {
    /* Classic DP with CHT:
       dp[i] = min(dp[j] + a[i]*b[j]) for j < i
       Simplified: a = {1,2,3,4,5}, b = {5,4,3,2,1}, dp[0] = 0
       dp[1] = dp[0] + a[1]*b[0] = 0 + 2*5 = 10
       dp[2] = min(dp[0]+a[2]*b[0], dp[1]+a[2]*b[1]) = min(0+15, 10+12) = 15
       dp[3] = min(dp[0]+a[3]*b[0], dp[1]+a[3]*b[1], dp[2]+a[3]*b[2])
             = min(0+20, 10+16, 15+12) = 20
       dp[4] = min(0+25, 10+20, 15+15, 20+10) = 25
    */
    int a[5];
    int b[5];
    int dp[5];
    int i;
    int j;
    int best;
    int val;

    a[0]=1; a[1]=2; a[2]=3; a[3]=4; a[4]=5;
    b[0]=5; b[1]=4; b[2]=3; b[3]=2; b[4]=1;

    dp[0] = 0;
    for (i = 1; i < 5; i++) {
        best = 999999;
        for (j = 0; j < i; j++) {
            val = dp[j] + a[i] * b[j];
            if (val < best) best = val;
        }
        dp[i] = best;
    }

    printf("=== DP Application ===\n");
    // EXPECT: === DP Application ===
    printf("dp[1]: %d\n", dp[1]);
    // EXPECT: dp[1]: 10
    printf("dp[2]: %d\n", dp[2]);
    // EXPECT: dp[2]: 15
    printf("dp[3]: %d\n", dp[3]);
    // EXPECT: dp[3]: 20
    printf("dp[4]: %d\n", dp[4]);
    // EXPECT: dp[4]: 25
}

int main(void) {
    test_basic();
    test_verification();
    test_redundant_lines();
    test_dp_application();

    printf("All convex hull trick tests passed!\n");
    // EXPECT: All convex hull trick tests passed!
    return 0;
}
