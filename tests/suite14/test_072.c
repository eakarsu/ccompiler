int printf(const char *fmt, ...);

typedef struct {
    int x;
    int y;
} Point;

int shoelace_doubled(Point *pts, int n) {
    int area2 = 0;
    int i;
    for (i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area2 = area2 + pts[i].x * pts[j].y;
        area2 = area2 - pts[j].x * pts[i].y;
    }
    if (area2 < 0) area2 = -area2;
    return area2;
}

int triangle_area2(Point a, Point b, Point c) {
    int area2 = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
    if (area2 < 0) area2 = -area2;
    return area2;
}

int polygon_perimeter_sq(Point *pts, int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i++) {
        int j = (i + 1) % n;
        int dx = pts[j].x - pts[i].x;
        int dy = pts[j].y - pts[i].y;
        total = total + dx * dx + dy * dy;
    }
    return total;
}

int is_convex(Point *pts, int n) {
    int sign = 0;
    int i;
    for (i = 0; i < n; i++) {
        int j = (i + 1) % n;
        int k = (i + 2) % n;
        int cross = (pts[j].x - pts[i].x) * (pts[k].y - pts[j].y)
                  - (pts[j].y - pts[i].y) * (pts[k].x - pts[j].x);
        if (cross != 0) {
            if (sign == 0) {
                sign = (cross > 0) ? 1 : -1;
            } else {
                int csign = (cross > 0) ? 1 : -1;
                if (csign != sign) return 0;
            }
        }
    }
    return 1;
}

Point make_point(int x, int y) {
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

int main(void) {
    Point square[4];
    square[0] = make_point(0, 0);
    square[1] = make_point(10, 0);
    square[2] = make_point(10, 10);
    square[3] = make_point(0, 10);

    int a1 = shoelace_doubled(square, 4);
    printf("%d\n", a1);
    // EXPECT: 200

    Point triangle[3];
    triangle[0] = make_point(0, 0);
    triangle[1] = make_point(10, 0);
    triangle[2] = make_point(0, 10);

    int a2 = shoelace_doubled(triangle, 3);
    printf("%d\n", a2);
    // EXPECT: 100

    int a3 = triangle_area2(make_point(0,0), make_point(10,0), make_point(0,10));
    printf("%d\n", a3);
    // EXPECT: 100

    int a4 = triangle_area2(make_point(0,0), make_point(6,0), make_point(3,4));
    printf("%d\n", a4);
    // EXPECT: 24

    Point rect[4];
    rect[0] = make_point(0, 0);
    rect[1] = make_point(5, 0);
    rect[2] = make_point(5, 3);
    rect[3] = make_point(0, 3);
    int a5 = shoelace_doubled(rect, 4);
    printf("%d\n", a5);
    // EXPECT: 30

    Point pentagon[5];
    pentagon[0] = make_point(2, 0);
    pentagon[1] = make_point(4, 0);
    pentagon[2] = make_point(5, 3);
    pentagon[3] = make_point(3, 5);
    pentagon[4] = make_point(1, 3);
    int a6 = shoelace_doubled(pentagon, 5);
    printf("%d\n", a6);
    // EXPECT: 26

    int psq = polygon_perimeter_sq(square, 4);
    printf("%d\n", psq);
    // EXPECT: 400

    int tsq = polygon_perimeter_sq(triangle, 3);
    printf("%d\n", tsq);
    // EXPECT: 400

    int cv1 = is_convex(square, 4);
    printf("%d\n", cv1);
    // EXPECT: 1

    int cv2 = is_convex(triangle, 3);
    printf("%d\n", cv2);
    // EXPECT: 1

    Point concave[4];
    concave[0] = make_point(0, 0);
    concave[1] = make_point(4, 0);
    concave[2] = make_point(2, 1);
    concave[3] = make_point(0, 4);
    int cv3 = is_convex(concave, 4);
    printf("%d\n", cv3);
    // EXPECT: 0

    int a7 = shoelace_doubled(concave, 4);
    printf("%d\n", a7);
    // EXPECT: 12

    Point big_tri[3];
    big_tri[0] = make_point(0, 0);
    big_tri[1] = make_point(100, 0);
    big_tri[2] = make_point(50, 80);
    int a8 = shoelace_doubled(big_tri, 3);
    printf("%d\n", a8);
    // EXPECT: 8000

    Point hex[6];
    hex[0] = make_point(2, 0);
    hex[1] = make_point(4, 0);
    hex[2] = make_point(6, 2);
    hex[3] = make_point(4, 4);
    hex[4] = make_point(2, 4);
    hex[5] = make_point(0, 2);
    int a9 = shoelace_doubled(hex, 6);
    printf("%d\n", a9);
    // EXPECT: 32

    int cv4 = is_convex(hex, 6);
    printf("%d\n", cv4);
    // EXPECT: 1

    Point degenerate[3];
    degenerate[0] = make_point(0, 0);
    degenerate[1] = make_point(5, 5);
    degenerate[2] = make_point(10, 10);
    int a10 = shoelace_doubled(degenerate, 3);
    printf("%d\n", a10);
    // EXPECT: 0

    printf("done\n");
    // EXPECT: done

    return 0;
}
