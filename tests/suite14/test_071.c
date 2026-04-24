int printf(const char *fmt, ...);

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point a;
    Point b;
} Line;

int dist_sq(Point p1, Point p2) {
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    return dx * dx + dy * dy;
}

int manhattan(Point p1, Point p2) {
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    return dx + dy;
}

int chebyshev(Point p1, Point p2) {
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    if (dx > dy) return dx;
    return dy;
}

int line_length_sq(Line l) {
    return dist_sq(l.a, l.b);
}

Point midpoint(Point p1, Point p2) {
    Point m;
    m.x = (p1.x + p2.x) / 2;
    m.y = (p1.y + p2.y) / 2;
    return m;
}

int collinear(Point p1, Point p2, Point p3) {
    int cross = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
    return cross == 0;
}

int point_on_segment(Point p, Line seg) {
    int cross = (seg.b.x - seg.a.x) * (p.y - seg.a.y) - (seg.b.y - seg.a.y) * (p.x - seg.a.x);
    if (cross != 0) return 0;
    int minx = seg.a.x;
    int maxx = seg.b.x;
    if (minx > maxx) { int t = minx; minx = maxx; maxx = t; }
    int miny = seg.a.y;
    int maxy = seg.b.y;
    if (miny > maxy) { int t = miny; miny = maxy; maxy = t; }
    return p.x >= minx && p.x <= maxx && p.y >= miny && p.y <= maxy;
}

Point make_point(int x, int y) {
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

Line make_line(int x1, int y1, int x2, int y2) {
    Line l;
    l.a.x = x1;
    l.a.y = y1;
    l.b.x = x2;
    l.b.y = y2;
    return l;
}

int main(void) {
    Point origin;
    origin.x = 0;
    origin.y = 0;

    Point p1 = make_point(3, 4);
    Point p2 = make_point(6, 8);
    int d1 = dist_sq(origin, p1);
    printf("%d\n", d1);
    // EXPECT: 25

    int d2 = dist_sq(origin, p2);
    printf("%d\n", d2);
    // EXPECT: 100

    int d3 = dist_sq(p1, p2);
    printf("%d\n", d3);
    // EXPECT: 25

    int m1 = manhattan(origin, p1);
    printf("%d\n", m1);
    // EXPECT: 7

    int m2 = manhattan(p1, p2);
    printf("%d\n", m2);
    // EXPECT: 7

    Point neg = make_point(-3, -4);
    int m3 = manhattan(origin, neg);
    printf("%d\n", m3);
    // EXPECT: 7

    int c1 = chebyshev(origin, p1);
    printf("%d\n", c1);
    // EXPECT: 4

    int c2 = chebyshev(origin, make_point(10, 3));
    printf("%d\n", c2);
    // EXPECT: 10

    int c3 = chebyshev(origin, make_point(3, 10));
    printf("%d\n", c3);
    // EXPECT: 10

    Line l1 = make_line(0, 0, 6, 8);
    int lsq = line_length_sq(l1);
    printf("%d\n", lsq);
    // EXPECT: 100

    Point mid = midpoint(origin, p2);
    printf("%d %d\n", mid.x, mid.y);
    // EXPECT: 3 4

    Point mid2 = midpoint(p1, p2);
    printf("%d %d\n", mid2.x, mid2.y);
    // EXPECT: 4 6

    int col1 = collinear(origin, p1, p2);
    printf("%d\n", col1);
    // EXPECT: 1

    int col2 = collinear(origin, make_point(2, 4), make_point(3, 6));
    printf("%d\n", col2);
    // EXPECT: 1

    int col3 = collinear(make_point(1, 1), make_point(2, 2), make_point(3, 3));
    printf("%d\n", col3);
    // EXPECT: 1

    Line seg = make_line(0, 0, 10, 10);
    int on1 = point_on_segment(make_point(5, 5), seg);
    printf("%d\n", on1);
    // EXPECT: 1

    int on2 = point_on_segment(make_point(5, 6), seg);
    printf("%d\n", on2);
    // EXPECT: 0

    int on3 = point_on_segment(make_point(0, 0), seg);
    printf("%d\n", on3);
    // EXPECT: 1

    int on4 = point_on_segment(make_point(11, 11), seg);
    printf("%d\n", on4);
    // EXPECT: 0

    Point pts[4];
    pts[0] = make_point(0, 0);
    pts[1] = make_point(10, 0);
    pts[2] = make_point(10, 10);
    pts[3] = make_point(0, 10);
    int total_perimeter_sq = 0;
    int i;
    for (i = 0; i < 4; i++) {
        int next = (i + 1) % 4;
        total_perimeter_sq = total_perimeter_sq + dist_sq(pts[i], pts[next]);
    }
    printf("%d\n", total_perimeter_sq);
    // EXPECT: 400

    Point centroid;
    int sx = 0;
    int sy = 0;
    for (i = 0; i < 4; i++) {
        sx = sx + pts[i].x;
        sy = sy + pts[i].y;
    }
    centroid.x = sx / 4;
    centroid.y = sy / 4;
    printf("%d %d\n", centroid.x, centroid.y);
    // EXPECT: 5 5

    printf("done\n");
    // EXPECT: done

    return 0;
}
