int printf(const char *fmt, ...);
// EXPECT: p1 = (3, 4)\np2 = (7, 1)\np1+p2 = (10, 5)\nmanhattan = 7\ndist^2 = 25\narea = 50\nperimeter = 30\noverlap = 1\nno overlap = 0\ncentroid = (5, 5)\ntotal manhattan = 120\nscaled area = 300
// Test: arithmetic with struct members

struct point {
    int x;
    int y;
};

struct rect {
    int x, y, w, h;
};

int manhattan_dist(struct point a, struct point b) {
    int dx, dy;
    dx = a.x - b.x;
    if (dx < 0) dx = -dx;
    dy = a.y - b.y;
    if (dy < 0) dy = -dy;
    return dx + dy;
}

int rect_area(struct rect r) {
    return r.w * r.h;
}

int rect_perimeter(struct rect r) {
    return 2 * (r.w + r.h);
}

int rects_overlap(struct rect a, struct rect b) {
    if (a.x + a.w <= b.x || b.x + b.w <= a.x) return 0;
    if (a.y + a.h <= b.y || b.y + b.h <= a.y) return 0;
    return 1;
}

int main(void) {
    struct point p1, p2, p3;
    struct rect r1, r2;
    struct point pts[5];
    int i;
    int total;

    /* Point arithmetic */
    p1.x = 3; p1.y = 4;
    p2.x = 7; p2.y = 1;
    printf("p1 = (%d, %d)\n", p1.x, p1.y);
    printf("p2 = (%d, %d)\n", p2.x, p2.y);

    /* Point addition */
    p3.x = p1.x + p2.x;
    p3.y = p1.y + p2.y;
    printf("p1+p2 = (%d, %d)\n", p3.x, p3.y);

    /* Manhattan distance */
    printf("manhattan = %d\n", manhattan_dist(p1, p2));

    /* Distance squared (avoid sqrt) */
    {
        int dx, dy;
        dx = p2.x - p1.x;
        dy = p2.y - p1.y;
        printf("dist^2 = %d\n", dx * dx + dy * dy);
    }

    /* Rectangle operations */
    r1.x = 0; r1.y = 0; r1.w = 10; r1.h = 5;
    printf("area = %d\n", rect_area(r1));
    printf("perimeter = %d\n", rect_perimeter(r1));

    /* Rectangle overlap */
    r2.x = 5; r2.y = 3; r2.w = 8; r2.h = 6;
    printf("overlap = %d\n", rects_overlap(r1, r2));

    r2.x = 20; r2.y = 20; r2.w = 5; r2.h = 5;
    printf("no overlap = %d\n", rects_overlap(r1, r2));

    /* Array of points - compute centroid */
    pts[0].x = 0; pts[0].y = 0;
    pts[1].x = 10; pts[1].y = 0;
    pts[2].x = 10; pts[2].y = 10;
    pts[3].x = 0; pts[3].y = 10;
    pts[4].x = 5; pts[4].y = 5;

    {
        int cx, cy;
        cx = 0; cy = 0;
        for (i = 0; i < 5; i++) {
            cx += pts[i].x;
            cy += pts[i].y;
        }
        cx /= 5;
        cy /= 5;
        printf("centroid = (%d, %d)\n", cx, cy);
    }

    /* Total pairwise manhattan distance */
    total = 0;
    for (i = 0; i < 5; i++) {
        int j;
        for (j = i + 1; j < 5; j++) {
            total += manhattan_dist(pts[i], pts[j]);
        }
    }
    printf("total manhattan = %d\n", total);

    /* Scale rectangle */
    r1.w *= 2;
    r1.h *= 3;
    printf("scaled area = %d\n", rect_area(r1));

    return 0;
}
