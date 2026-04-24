int printf(const char *fmt, ...);

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point a;
    Point b;
} Segment;

int sign(int v) {
    if (v > 0) return 1;
    if (v < 0) return -1;
    return 0;
}

int cross_val(Point o, Point a, Point b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

int on_segment(Point p, Point a, Point b) {
    int minx = a.x;
    int maxx = b.x;
    if (minx > maxx) { int t = minx; minx = maxx; maxx = t; }
    int miny = a.y;
    int maxy = b.y;
    if (miny > maxy) { int t = miny; miny = maxy; maxy = t; }
    return p.x >= minx && p.x <= maxx && p.y >= miny && p.y <= maxy;
}

int segments_intersect(Segment s1, Segment s2) {
    int d1 = cross_val(s2.a, s2.b, s1.a);
    int d2 = cross_val(s2.a, s2.b, s1.b);
    int d3 = cross_val(s1.a, s1.b, s2.a);
    int d4 = cross_val(s1.a, s1.b, s2.b);

    int sd1 = sign(d1);
    int sd2 = sign(d2);
    int sd3 = sign(d3);
    int sd4 = sign(d4);

    if (sd1 * sd2 < 0 && sd3 * sd4 < 0) return 1;

    if (sd1 == 0 && on_segment(s1.a, s2.a, s2.b)) return 1;
    if (sd2 == 0 && on_segment(s1.b, s2.a, s2.b)) return 1;
    if (sd3 == 0 && on_segment(s2.a, s1.a, s1.b)) return 1;
    if (sd4 == 0 && on_segment(s2.b, s1.a, s1.b)) return 1;

    return 0;
}

int lines_parallel(Point a1, Point a2, Point b1, Point b2) {
    int dax = a2.x - a1.x;
    int day = a2.y - a1.y;
    int dbx = b2.x - b1.x;
    int dby = b2.y - b1.y;
    return dax * dby - day * dbx == 0;
}

Segment make_seg(int x1, int y1, int x2, int y2) {
    Segment s;
    s.a.x = x1; s.a.y = y1;
    s.b.x = x2; s.b.y = y2;
    return s;
}

Point make_point(int x, int y) {
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

int main(void) {
    Segment s1 = make_seg(0, 0, 10, 10);
    Segment s2 = make_seg(0, 10, 10, 0);
    int r1 = segments_intersect(s1, s2);
    printf("%d\n", r1);
    // EXPECT: 1

    Segment s3 = make_seg(0, 0, 5, 5);
    Segment s4 = make_seg(6, 6, 10, 10);
    int r2 = segments_intersect(s3, s4);
    printf("%d\n", r2);
    // EXPECT: 0

    Segment s5 = make_seg(0, 0, 5, 5);
    Segment s6 = make_seg(5, 5, 10, 10);
    int r3 = segments_intersect(s5, s6);
    printf("%d\n", r3);
    // EXPECT: 1

    Segment s7 = make_seg(0, 0, 10, 0);
    Segment s8 = make_seg(5, -5, 5, 5);
    int r4 = segments_intersect(s7, s8);
    printf("%d\n", r4);
    // EXPECT: 1

    Segment s9 = make_seg(0, 0, 10, 0);
    Segment s10 = make_seg(0, 1, 10, 1);
    int r5 = segments_intersect(s9, s10);
    printf("%d\n", r5);
    // EXPECT: 0

    int p1 = lines_parallel(make_point(0,0), make_point(1,1),
                             make_point(2,2), make_point(3,3));
    printf("%d\n", p1);
    // EXPECT: 1

    int p2 = lines_parallel(make_point(0,0), make_point(1,0),
                             make_point(0,1), make_point(1,1));
    printf("%d\n", p2);
    // EXPECT: 1

    int p3 = lines_parallel(make_point(0,0), make_point(1,1),
                             make_point(0,0), make_point(1,0));
    printf("%d\n", p3);
    // EXPECT: 0

    Segment h1 = make_seg(0, 5, 10, 5);
    Segment v1 = make_seg(5, 0, 5, 10);
    int r6 = segments_intersect(h1, v1);
    printf("%d\n", r6);
    // EXPECT: 1

    Segment h2 = make_seg(0, 5, 4, 5);
    int r7 = segments_intersect(h2, v1);
    printf("%d\n", r7);
    // EXPECT: 0

    Segment d1 = make_seg(0, 0, 4, 4);
    Segment d2 = make_seg(1, 3, 3, 1);
    int r8 = segments_intersect(d1, d2);
    printf("%d\n", r8);
    // EXPECT: 1

    Segment d3 = make_seg(0, 0, 1, 1);
    Segment d4 = make_seg(3, 0, 0, 3);
    int r9 = segments_intersect(d3, d4);
    printf("%d\n", r9);
    // EXPECT: 0

    Segment col1 = make_seg(0, 0, 5, 0);
    Segment col2 = make_seg(3, 0, 8, 0);
    int r10 = segments_intersect(col1, col2);
    printf("%d\n", r10);
    // EXPECT: 1

    Segment col3 = make_seg(0, 0, 2, 0);
    Segment col4 = make_seg(3, 0, 5, 0);
    int r11 = segments_intersect(col3, col4);
    printf("%d\n", r11);
    // EXPECT: 0

    int count = 0;
    Segment box[4];
    box[0] = make_seg(0, 0, 10, 0);
    box[1] = make_seg(10, 0, 10, 10);
    box[2] = make_seg(10, 10, 0, 10);
    box[3] = make_seg(0, 10, 0, 0);
    Segment diag = make_seg(-1, 5, 11, 5);
    int i;
    for (i = 0; i < 4; i++) {
        if (segments_intersect(box[i], diag)) count++;
    }
    printf("%d\n", count);
    // EXPECT: 2

    printf("done\n");
    // EXPECT: done

    return 0;
}
