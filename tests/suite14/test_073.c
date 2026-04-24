int printf(const char *fmt, ...);

typedef struct {
    int x;
    int y;
} Point;

int cross(Point o, Point a, Point b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

int dist_sq(Point a, Point b) {
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    return dx * dx + dy * dy;
}

void swap_points(Point *pts, int i, int j) {
    Point tmp;
    tmp.x = pts[i].x;
    tmp.y = pts[i].y;
    pts[i].x = pts[j].x;
    pts[i].y = pts[j].y;
    pts[j].x = tmp.x;
    pts[j].y = tmp.y;
}

void sort_by_angle(Point *pts, int n, Point pivot) {
    int i;
    int j;
    for (i = 1; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            int c = cross(pivot, pts[i], pts[j]);
            if (c < 0) {
                swap_points(pts, i, j);
            } else if (c == 0) {
                int di = dist_sq(pivot, pts[i]);
                int dj = dist_sq(pivot, pts[j]);
                if (di > dj) {
                    swap_points(pts, i, j);
                }
            }
        }
    }
}

int convex_hull(Point *pts, int n, Point *hull) {
    if (n < 3) {
        int i;
        for (i = 0; i < n; i++) {
            hull[i].x = pts[i].x;
            hull[i].y = pts[i].y;
        }
        return n;
    }

    int lowest = 0;
    int i;
    for (i = 1; i < n; i++) {
        if (pts[i].y < pts[lowest].y ||
            (pts[i].y == pts[lowest].y && pts[i].x < pts[lowest].x)) {
            lowest = i;
        }
    }
    swap_points(pts, 0, lowest);

    Point pivot;
    pivot.x = pts[0].x;
    pivot.y = pts[0].y;
    sort_by_angle(pts, n, pivot);

    int top = 0;
    hull[0].x = pts[0].x;
    hull[0].y = pts[0].y;
    top = 1;
    hull[1].x = pts[1].x;
    hull[1].y = pts[1].y;
    top = 2;

    for (i = 2; i < n; i++) {
        while (top > 1 && cross(hull[top - 2], hull[top - 1], pts[i]) <= 0) {
            top--;
        }
        hull[top].x = pts[i].x;
        hull[top].y = pts[i].y;
        top++;
    }
    return top;
}

int shoelace2(Point *pts, int n) {
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

int main(void) {
    Point pts1[4];
    Point hull1[4];
    pts1[0].x = 0; pts1[0].y = 0;
    pts1[1].x = 10; pts1[1].y = 0;
    pts1[2].x = 10; pts1[2].y = 10;
    pts1[3].x = 0; pts1[3].y = 10;

    int h1 = convex_hull(pts1, 4, hull1);
    printf("%d\n", h1);
    // EXPECT: 4

    int a1 = shoelace2(hull1, h1);
    printf("%d\n", a1);
    // EXPECT: 200

    Point pts2[5];
    Point hull2[5];
    pts2[0].x = 0; pts2[0].y = 0;
    pts2[1].x = 5; pts2[1].y = 0;
    pts2[2].x = 10; pts2[2].y = 0;
    pts2[3].x = 10; pts2[3].y = 10;
    pts2[4].x = 0; pts2[4].y = 10;

    int h2 = convex_hull(pts2, 5, hull2);
    printf("%d\n", h2);
    // EXPECT: 4

    Point pts3[5];
    Point hull3[5];
    pts3[0].x = 0; pts3[0].y = 0;
    pts3[1].x = 4; pts3[1].y = 0;
    pts3[2].x = 2; pts3[2].y = 1;
    pts3[3].x = 4; pts3[3].y = 4;
    pts3[4].x = 0; pts3[4].y = 4;

    int h3 = convex_hull(pts3, 5, hull3);
    printf("%d\n", h3);
    // EXPECT: 4

    int a3 = shoelace2(hull3, h3);
    printf("%d\n", a3);
    // EXPECT: 32

    Point pts4[3];
    Point hull4[3];
    pts4[0].x = 0; pts4[0].y = 0;
    pts4[1].x = 5; pts4[1].y = 0;
    pts4[2].x = 0; pts4[2].y = 5;
    int h4 = convex_hull(pts4, 3, hull4);
    printf("%d\n", h4);
    // EXPECT: 3

    int a4 = shoelace2(hull4, h4);
    printf("%d\n", a4);
    // EXPECT: 25

    Point pts5[6];
    Point hull5[6];
    pts5[0].x = 1; pts5[0].y = 1;
    pts5[1].x = 3; pts5[1].y = 1;
    pts5[2].x = 5; pts5[2].y = 1;
    pts5[3].x = 5; pts5[3].y = 5;
    pts5[4].x = 1; pts5[4].y = 5;
    pts5[5].x = 3; pts5[5].y = 3;
    int h5 = convex_hull(pts5, 6, hull5);
    printf("%d\n", h5);
    // EXPECT: 4

    int a5 = shoelace2(hull5, h5);
    printf("%d\n", a5);
    // EXPECT: 32

    printf("done\n");
    // EXPECT: done

    return 0;
}
