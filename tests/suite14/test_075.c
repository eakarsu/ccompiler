int printf(const char *fmt, ...);

typedef struct {
    int x;
    int y;
} Point;

int point_in_polygon(Point test, Point *poly, int n) {
    int inside = 0;
    int i;
    int j = n - 1;
    for (i = 0; i < n; i++) {
        int yi = poly[i].y;
        int yj = poly[j].y;
        int xi = poly[i].x;
        int xj = poly[j].x;

        if ((yi > test.y) != (yj > test.y)) {
            int cross_x_num = (xj - xi) * (test.y - yi);
            int cross_x_den = yj - yi;
            int edge_x = xi * cross_x_den + cross_x_num;
            int test_x_scaled = test.x * cross_x_den;
            if (cross_x_den > 0) {
                if (test_x_scaled < edge_x) inside = 1 - inside;
            } else {
                if (test_x_scaled > edge_x) inside = 1 - inside;
            }
        }
        j = i;
    }
    return inside;
}

int point_on_edge(Point p, Point a, Point b) {
    int cross = (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
    if (cross != 0) return 0;
    int minx = a.x;
    int maxx = b.x;
    if (minx > maxx) { int t = minx; minx = maxx; maxx = t; }
    int miny = a.y;
    int maxy = b.y;
    if (miny > maxy) { int t = miny; miny = maxy; maxy = t; }
    return p.x >= minx && p.x <= maxx && p.y >= miny && p.y <= maxy;
}

int point_on_polygon_boundary(Point p, Point *poly, int n) {
    int i;
    for (i = 0; i < n; i++) {
        int j = (i + 1) % n;
        if (point_on_edge(p, poly[i], poly[j])) return 1;
    }
    return 0;
}

Point make_pt(int x, int y) {
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

int main(void) {
    Point square[4];
    square[0] = make_pt(0, 0);
    square[1] = make_pt(10, 0);
    square[2] = make_pt(10, 10);
    square[3] = make_pt(0, 10);

    int r1 = point_in_polygon(make_pt(5, 5), square, 4);
    printf("%d\n", r1);
    // EXPECT: 1

    int r2 = point_in_polygon(make_pt(15, 5), square, 4);
    printf("%d\n", r2);
    // EXPECT: 0

    int r3 = point_in_polygon(make_pt(-1, 5), square, 4);
    printf("%d\n", r3);
    // EXPECT: 0

    int r4 = point_in_polygon(make_pt(5, -1), square, 4);
    printf("%d\n", r4);
    // EXPECT: 0

    int r5 = point_in_polygon(make_pt(5, 11), square, 4);
    printf("%d\n", r5);
    // EXPECT: 0

    int r6 = point_in_polygon(make_pt(1, 1), square, 4);
    printf("%d\n", r6);
    // EXPECT: 1

    int r7 = point_in_polygon(make_pt(9, 9), square, 4);
    printf("%d\n", r7);
    // EXPECT: 1

    Point triangle[3];
    triangle[0] = make_pt(0, 0);
    triangle[1] = make_pt(20, 0);
    triangle[2] = make_pt(10, 20);

    int r8 = point_in_polygon(make_pt(10, 5), triangle, 3);
    printf("%d\n", r8);
    // EXPECT: 1

    int r9 = point_in_polygon(make_pt(0, 20), triangle, 3);
    printf("%d\n", r9);
    // EXPECT: 0

    int r10 = point_in_polygon(make_pt(1, 1), triangle, 3);
    printf("%d\n", r10);
    // EXPECT: 1

    int r11 = point_in_polygon(make_pt(19, 1), triangle, 3);
    printf("%d\n", r11);
    // EXPECT: 1

    int b1 = point_on_polygon_boundary(make_pt(5, 0), square, 4);
    printf("%d\n", b1);
    // EXPECT: 1

    int b2 = point_on_polygon_boundary(make_pt(0, 5), square, 4);
    printf("%d\n", b2);
    // EXPECT: 1

    int b3 = point_on_polygon_boundary(make_pt(5, 5), square, 4);
    printf("%d\n", b3);
    // EXPECT: 0

    int b4 = point_on_polygon_boundary(make_pt(0, 0), square, 4);
    printf("%d\n", b4);
    // EXPECT: 1

    Point lshape[6];
    lshape[0] = make_pt(0, 0);
    lshape[1] = make_pt(10, 0);
    lshape[2] = make_pt(10, 5);
    lshape[3] = make_pt(5, 5);
    lshape[4] = make_pt(5, 10);
    lshape[5] = make_pt(0, 10);

    int l1 = point_in_polygon(make_pt(2, 2), lshape, 6);
    printf("%d\n", l1);
    // EXPECT: 1

    int l2 = point_in_polygon(make_pt(8, 2), lshape, 6);
    printf("%d\n", l2);
    // EXPECT: 1

    int l3 = point_in_polygon(make_pt(8, 8), lshape, 6);
    printf("%d\n", l3);
    // EXPECT: 0

    int l4 = point_in_polygon(make_pt(2, 8), lshape, 6);
    printf("%d\n", l4);
    // EXPECT: 1

    int count_inside = 0;
    int x;
    int y;
    for (y = 1; y <= 9; y = y + 2) {
        for (x = 1; x <= 9; x = x + 2) {
            if (point_in_polygon(make_pt(x, y), square, 4)) {
                count_inside++;
            }
        }
    }
    printf("%d\n", count_inside);
    // EXPECT: 25

    printf("done\n");
    // EXPECT: done

    return 0;
}
