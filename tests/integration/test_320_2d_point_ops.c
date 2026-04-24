int printf(const char *fmt, ...);
// EXPECT: a=(3,4)\nb=(6,8)\na+b=(9,12)\nb-a=(3,4)\na*3=(9,12)\ndot=50\ncross=0\ndist_sq=25\nmanhattan=7\nmidpoint=(5,5)\nline_len_sq=200\nperimeter=14
// Test: struct for 2D point operations

struct Point2D {
    int x;
    int y;
};

struct Line {
    struct Point2D start;
    struct Point2D end;
};

struct Point2D point_make(int x, int y) {
    struct Point2D p;
    p.x = x;
    p.y = y;
    return p;
}

struct Point2D point_add(struct Point2D a, struct Point2D b) {
    return point_make(a.x + b.x, a.y + b.y);
}

struct Point2D point_sub(struct Point2D a, struct Point2D b) {
    return point_make(a.x - b.x, a.y - b.y);
}

struct Point2D point_scale(struct Point2D p, int s) {
    return point_make(p.x * s, p.y * s);
}

int point_dot(struct Point2D a, struct Point2D b) {
    return a.x * b.x + a.y * b.y;
}

int point_cross(struct Point2D a, struct Point2D b) {
    return a.x * b.y - a.y * b.x;
}

int point_dist_sq(struct Point2D a, struct Point2D b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return dx * dx + dy * dy;
}

int point_manhattan(struct Point2D a, struct Point2D b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    return dx + dy;
}

struct Point2D line_midpoint(struct Line *l) {
    return point_make((l->start.x + l->end.x) / 2,
                      (l->start.y + l->end.y) / 2);
}

int line_length_sq(struct Line *l) {
    return point_dist_sq(l->start, l->end);
}

int main(void) {
    struct Point2D a = point_make(3, 4);
    struct Point2D b = point_make(6, 8);
    printf("a=(%d,%d)\n", a.x, a.y);
    printf("b=(%d,%d)\n", b.x, b.y);

    struct Point2D sum = point_add(a, b);
    printf("a+b=(%d,%d)\n", sum.x, sum.y);

    struct Point2D diff = point_sub(b, a);
    printf("b-a=(%d,%d)\n", diff.x, diff.y);

    struct Point2D scaled = point_scale(a, 3);
    printf("a*3=(%d,%d)\n", scaled.x, scaled.y);

    printf("dot=%d\n", point_dot(a, b));
    printf("cross=%d\n", point_cross(a, b));
    printf("dist_sq=%d\n", point_dist_sq(a, b));
    printf("manhattan=%d\n", point_manhattan(a, b));

    struct Line line;
    line.start = point_make(0, 0);
    line.end = point_make(10, 10);
    struct Point2D mid = line_midpoint(&line);
    printf("midpoint=(%d,%d)\n", mid.x, mid.y);
    printf("line_len_sq=%d\n", line_length_sq(&line));

    struct Point2D pts[4];
    pts[0] = point_make(0, 0);
    pts[1] = point_make(4, 0);
    pts[2] = point_make(4, 3);
    pts[3] = point_make(0, 3);
    int perimeter = 0;
    int i;
    for (i = 0; i < 4; i++) {
        int next = (i + 1) % 4;
        perimeter = perimeter + point_manhattan(pts[i], pts[next]);
    }
    printf("perimeter=%d\n", perimeter);

    return 0;
}
