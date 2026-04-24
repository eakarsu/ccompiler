int printf(const char *fmt, ...);

// Triangle classification by side lengths (using squared lengths to avoid sqrt)
// Also: area via Heron's formula approximation, and special triangle checks

struct Point {
    int x;
    int y;
};

int dist_sq(struct Point a, struct Point b) {
    int dx;
    int dy;
    dx = a.x - b.x;
    dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// Integer square root (floor)
int isqrt(int n) {
    int x;
    int x1;
    if (n <= 0) return 0;
    x = n;
    x1 = (x + 1) / 2;
    while (x1 < x) {
        x = x1;
        x1 = (x + n / x) / 2;
    }
    return x;
}

// Classify triangle by angles using squared side lengths
// a2, b2, c2 are the squared lengths of sides opposite to vertices A, B, C
// Returns: 0=right, 1=acute, 2=obtuse
int classify_angle(int a2, int b2, int c2) {
    // c is the longest side squared
    int max_sq;
    int sum_sq;
    max_sq = a2;
    sum_sq = b2 + c2;
    if (b2 > max_sq) {
        max_sq = b2;
        sum_sq = a2 + c2;
    }
    if (c2 > max_sq) {
        max_sq = c2;
        sum_sq = a2 + b2;
    }
    if (max_sq == sum_sq) return 0; // right
    if (max_sq < sum_sq) return 1;  // acute
    return 2; // obtuse
}

// Classify triangle by sides
// Returns: 0=equilateral, 1=isosceles, 2=scalene
int classify_sides(int a2, int b2, int c2) {
    int eq_count;
    eq_count = 0;
    if (a2 == b2) eq_count = eq_count + 1;
    if (b2 == c2) eq_count = eq_count + 1;
    if (a2 == c2) eq_count = eq_count + 1;
    if (eq_count >= 3) return 0; // equilateral
    if (eq_count >= 1) return 1; // isosceles
    return 2; // scalene
}

// Check if three points form a valid triangle (non-degenerate)
// Uses cross product: if cross product is 0, points are collinear
int is_valid_triangle(struct Point a, struct Point b, struct Point c) {
    int cp;
    cp = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
    if (cp == 0) return 0;
    return 1;
}

// 16 * area^2 using the formula: 16A^2 = 2a2*b2 + 2b2*c2 + 2c2*a2 - a2^2 - b2^2 - c2^2
// where a2,b2,c2 are squared side lengths
int area_16sq(int a2, int b2, int c2) {
    int val;
    val = 2*a2*b2 + 2*b2*c2 + 2*c2*a2 - a2*a2 - b2*b2 - c2*c2;
    return val;
}

void print_angle_class(int cls) {
    if (cls == 0) printf("right");
    else if (cls == 1) printf("acute");
    else printf("obtuse");
}

void print_side_class(int cls) {
    if (cls == 0) printf("equilateral");
    else if (cls == 1) printf("isosceles");
    else printf("scalene");
}

int main() {
    struct Point a;
    struct Point b;
    struct Point c;
    int ab2;
    int bc2;
    int ca2;
    int ac;
    int sc;
    int valid;
    int a16;

    // Right triangle (3-4-5): (0,0),(3,0),(0,4)
    a.x = 0; a.y = 0;
    b.x = 3; b.y = 0;
    c.x = 0; c.y = 4;
    ab2 = dist_sq(a, b); // 9
    bc2 = dist_sq(b, c); // 25
    ca2 = dist_sq(c, a); // 16
    printf("3-4-5 sides sq: %d %d %d\n", ab2, bc2, ca2); // EXPECT: 3-4-5 sides sq: 9 25 16
    ac = classify_angle(ab2, bc2, ca2);
    printf("3-4-5 angle: "); // EXPECT: 3-4-5 angle: right
    print_angle_class(ac);
    printf("\n");
    sc = classify_sides(ab2, bc2, ca2);
    printf("3-4-5 sides: "); // EXPECT: 3-4-5 sides: scalene
    print_side_class(sc);
    printf("\n");

    // Isosceles right triangle: (0,0),(4,0),(0,4)
    a.x = 0; a.y = 0;
    b.x = 4; b.y = 0;
    c.x = 0; c.y = 4;
    ab2 = dist_sq(a, b);
    bc2 = dist_sq(b, c);
    ca2 = dist_sq(c, a);
    printf("iso-right sides sq: %d %d %d\n", ab2, bc2, ca2); // EXPECT: iso-right sides sq: 16 32 16
    ac = classify_angle(ab2, bc2, ca2);
    printf("iso-right angle: "); // EXPECT: iso-right angle: right
    print_angle_class(ac);
    printf("\n");
    sc = classify_sides(ab2, bc2, ca2);
    printf("iso-right sides: "); // EXPECT: iso-right sides: isosceles
    print_side_class(sc);
    printf("\n");

    // Equilateral-ish: not possible exactly on int grid, but test (0,0),(2,0),(1,2)
    a.x = 0; a.y = 0;
    b.x = 2; b.y = 0;
    c.x = 1; c.y = 2;
    ab2 = dist_sq(a, b); // 4
    bc2 = dist_sq(b, c); // 5
    ca2 = dist_sq(c, a); // 5
    printf("near-equi sides sq: %d %d %d\n", ab2, bc2, ca2); // EXPECT: near-equi sides sq: 4 5 5
    ac = classify_angle(ab2, bc2, ca2);
    printf("near-equi angle: "); // EXPECT: near-equi angle: acute
    print_angle_class(ac);
    printf("\n");
    sc = classify_sides(ab2, bc2, ca2);
    printf("near-equi sides: "); // EXPECT: near-equi sides: isosceles
    print_side_class(sc);
    printf("\n");

    // Obtuse triangle: (0,0),(10,0),(1,1)
    a.x = 0; a.y = 0;
    b.x = 10; b.y = 0;
    c.x = 1; c.y = 1;
    ab2 = dist_sq(a, b);
    bc2 = dist_sq(b, c);
    ca2 = dist_sq(c, a);
    printf("obtuse sides sq: %d %d %d\n", ab2, bc2, ca2); // EXPECT: obtuse sides sq: 100 82 2
    ac = classify_angle(ab2, bc2, ca2);
    printf("obtuse angle: "); // EXPECT: obtuse angle: obtuse
    print_angle_class(ac);
    printf("\n");

    // Valid triangle check
    valid = is_valid_triangle(a, b, c);
    printf("obtuse valid: %d\n", valid); // EXPECT: obtuse valid: 1

    // Collinear points
    a.x = 0; a.y = 0;
    b.x = 5; b.y = 5;
    c.x = 10; c.y = 10;
    valid = is_valid_triangle(a, b, c);
    printf("collinear valid: %d\n", valid); // EXPECT: collinear valid: 0

    // Integer sqrt tests
    printf("isqrt(0) = %d\n", isqrt(0));   // EXPECT: isqrt(0) = 0
    printf("isqrt(1) = %d\n", isqrt(1));   // EXPECT: isqrt(1) = 1
    printf("isqrt(25) = %d\n", isqrt(25)); // EXPECT: isqrt(25) = 5
    printf("isqrt(26) = %d\n", isqrt(26)); // EXPECT: isqrt(26) = 5
    printf("isqrt(100) = %d\n", isqrt(100)); // EXPECT: isqrt(100) = 10

    // Area test: 3-4-5 triangle, area = 6, 16*area^2 = 576
    a.x = 0; a.y = 0;
    b.x = 3; b.y = 0;
    c.x = 0; c.y = 4;
    ab2 = dist_sq(a, b);
    bc2 = dist_sq(b, c);
    ca2 = dist_sq(c, a);
    a16 = area_16sq(ab2, bc2, ca2);
    printf("3-4-5 16*area^2 = %d\n", a16); // EXPECT: 3-4-5 16*area^2 = 576

    return 0;
}
