int printf(const char *fmt, ...);
// EXPECT: p1: (3, 4)\np2: (7, 1)\nsum: (10, 5)\nscaled: (15, 20)\nmid: (5, 2)\nneg: (-3, -4)\ndist^2: 25\nchain: (20, 10)
// Test: struct returned from function (small, <=16 bytes)

struct Point {
    int x;
    int y;
};

struct Point make_point(int x, int y) {
    struct Point p;
    p.x = x;
    p.y = y;
    return p;
}

struct Point add_points(struct Point a, struct Point b) {
    struct Point result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

struct Point scale_point(struct Point p, int factor) {
    struct Point result;
    result.x = p.x * factor;
    result.y = p.y * factor;
    return result;
}

struct Point midpoint(struct Point a, struct Point b) {
    struct Point result;
    result.x = (a.x + b.x) / 2;
    result.y = (a.y + b.y) / 2;
    return result;
}

struct Point negate_point(struct Point p) {
    struct Point result;
    result.x = -p.x;
    result.y = -p.y;
    return result;
}

int distance_sq(struct Point a, struct Point b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return dx * dx + dy * dy;
}

int main(void) {
    struct Point p1 = make_point(3, 4);
    struct Point p2 = make_point(7, 1);
    printf("p1: (%d, %d)\n", p1.x, p1.y);
    printf("p2: (%d, %d)\n", p2.x, p2.y);

    struct Point sum = add_points(p1, p2);
    printf("sum: (%d, %d)\n", sum.x, sum.y);

    struct Point scaled = scale_point(p1, 5);
    printf("scaled: (%d, %d)\n", scaled.x, scaled.y);

    struct Point mid = midpoint(p1, p2);
    printf("mid: (%d, %d)\n", mid.x, mid.y);

    struct Point neg = negate_point(p1);
    printf("neg: (%d, %d)\n", neg.x, neg.y);

    printf("dist^2: %d\n", distance_sq(p1, p2));

    struct Point chain = scale_point(add_points(p1, p2), 2);
    printf("chain: (%d, %d)\n", chain.x, chain.y);

    return 0;
}
