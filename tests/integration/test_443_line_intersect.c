int printf(const char *fmt, ...);
// EXPECT: Cross intersect: 1\nNo intersect: 0\nT-intersect: 1\nTouching: 1\nParallel: 0\nDist^2 (0,0)-(10,10): 200\nDist^2 (0,0)-(3,4): 25\n2x dist point(5,5) to line(0,0)-(10,0): 50\nCollinear (0,0)(5,5)(10,10): 1\nCollinear (0,0)(5,5)(10,11): 0
// Test: Line segment intersection

typedef struct {
    int x;
    int y;
} Point;

int sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

int cross_product(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

int on_segment(Point p, Point a, Point b) {
    int minx = a.x < b.x ? a.x : b.x;
    int maxx = a.x > b.x ? a.x : b.x;
    int miny = a.y < b.y ? a.y : b.y;
    int maxy = a.y > b.y ? a.y : b.y;
    return p.x >= minx && p.x <= maxx && p.y >= miny && p.y <= maxy;
}

int segments_intersect(Point a, Point b, Point c, Point d) {
    int d1 = cross_product(c, d, a);
    int d2 = cross_product(c, d, b);
    int d3 = cross_product(a, b, c);
    int d4 = cross_product(a, b, d);

    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
        ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
        return 1;
    }

    if (d1 == 0 && on_segment(a, c, d)) return 1;
    if (d2 == 0 && on_segment(b, c, d)) return 1;
    if (d3 == 0 && on_segment(c, a, b)) return 1;
    if (d4 == 0 && on_segment(d, a, b)) return 1;

    return 0;
}

int distance_sq(Point a, Point b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return dx * dx + dy * dy;
}

int point_to_line_dist_2x(Point p, Point a, Point b) {
    int area2 = cross_product(a, b, p);
    if (area2 < 0) area2 = -area2;
    return area2;
}

int collinear(Point a, Point b, Point c) {
    return cross_product(a, b, c) == 0;
}

int main(void) {
    Point a1 = {0, 0}, b1 = {10, 10};
    Point c1 = {0, 10}, d1 = {10, 0};
    printf("Cross intersect: %d\n", segments_intersect(a1, b1, c1, d1));

    Point a2 = {0, 0}, b2 = {5, 5};
    Point c2 = {6, 6}, d2 = {10, 10};
    printf("No intersect: %d\n", segments_intersect(a2, b2, c2, d2));

    Point a3 = {0, 0}, b3 = {10, 0};
    Point c3 = {5, 0}, d3 = {5, 10};
    printf("T-intersect: %d\n", segments_intersect(a3, b3, c3, d3));

    Point a4 = {0, 0}, b4 = {5, 5};
    Point c4 = {5, 5}, d4 = {10, 0};
    printf("Touching: %d\n", segments_intersect(a4, b4, c4, d4));

    Point a5 = {0, 0}, b5 = {10, 0};
    Point c5 = {0, 5}, d5 = {10, 5};
    printf("Parallel: %d\n", segments_intersect(a5, b5, c5, d5));

    printf("Dist^2 (0,0)-(10,10): %d\n", distance_sq(a1, b1));
    printf("Dist^2 (0,0)-(3,4): %d\n", distance_sq((Point){0,0}, (Point){3,4}));

    printf("2x dist point(5,5) to line(0,0)-(10,0): %d\n",
           point_to_line_dist_2x((Point){5,5}, (Point){0,0}, (Point){10,0}));

    printf("Collinear (0,0)(5,5)(10,10): %d\n",
           collinear((Point){0,0}, (Point){5,5}, (Point){10,10}));
    printf("Collinear (0,0)(5,5)(10,11): %d\n",
           collinear((Point){0,0}, (Point){5,5}, (Point){10,11}));

    return 0;
}
