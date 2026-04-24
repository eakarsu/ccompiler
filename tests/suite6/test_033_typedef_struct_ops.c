int printf(const char *fmt, ...);

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int width;
    int height;
} Size;

typedef struct {
    Point origin;
    Size size;
} Rect;

Point point_new(int x, int y) {
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

Point point_add(Point a, Point b) {
    Point r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

Point point_sub(Point a, Point b) {
    Point r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    return r;
}

Point point_scale(Point p, int factor) {
    Point r;
    r.x = p.x * factor;
    r.y = p.y * factor;
    return r;
}

Point point_negate(Point p) {
    Point r;
    r.x = -p.x;
    r.y = -p.y;
    return r;
}

int point_dot(Point a, Point b) {
    return a.x * b.x + a.y * b.y;
}

int point_equal(Point a, Point b) {
    return (a.x == b.x) && (a.y == b.y);
}

int point_manhattan(Point a, Point b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    return dx + dy;
}

int point_dist_sq(Point a, Point b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return dx * dx + dy * dy;
}

int point_compare(Point a, Point b) {
    if (a.x != b.x) return a.x - b.x;
    return a.y - b.y;
}

Size size_new(int w, int h) {
    Size s;
    s.width = w;
    s.height = h;
    return s;
}

int size_area(Size s) {
    return s.width * s.height;
}

int size_perimeter(Size s) {
    return 2 * (s.width + s.height);
}

Rect rect_new(int x, int y, int w, int h) {
    Rect r;
    r.origin = point_new(x, y);
    r.size = size_new(w, h);
    return r;
}

int rect_area(Rect r) {
    return size_area(r.size);
}

int rect_right(Rect r) {
    return r.origin.x + r.size.width;
}

int rect_bottom(Rect r) {
    return r.origin.y + r.size.height;
}

int rect_contains_point(Rect r, Point p) {
    return (p.x >= r.origin.x) && (p.x < rect_right(r)) &&
           (p.y >= r.origin.y) && (p.y < rect_bottom(r));
}

Rect rect_translate(Rect r, Point offset) {
    Rect result;
    result.origin = point_add(r.origin, offset);
    result.size = r.size;
    return result;
}

int main(void) {
    Point a = point_new(3, 4);
    Point b = point_new(1, 2);

    // EXPECT: a: 3 4
    printf("a: %d %d\n", a.x, a.y);

    // EXPECT: b: 1 2
    printf("b: %d %d\n", b.x, b.y);

    Point sum = point_add(a, b);
    // EXPECT: add: 4 6
    printf("add: %d %d\n", sum.x, sum.y);

    Point diff = point_sub(a, b);
    // EXPECT: sub: 2 2
    printf("sub: %d %d\n", diff.x, diff.y);

    Point scaled = point_scale(a, 3);
    // EXPECT: scale: 9 12
    printf("scale: %d %d\n", scaled.x, scaled.y);

    Point neg = point_negate(a);
    // EXPECT: negate: -3 -4
    printf("negate: %d %d\n", neg.x, neg.y);

    // EXPECT: dot: 11
    printf("dot: %d\n", point_dot(a, b));

    // EXPECT: equal ab: 0
    printf("equal ab: %d\n", point_equal(a, b));

    Point a2 = point_new(3, 4);
    // EXPECT: equal aa2: 1
    printf("equal aa2: %d\n", point_equal(a, a2));

    // EXPECT: manhattan: 4
    printf("manhattan: %d\n", point_manhattan(a, b));

    // EXPECT: dist_sq: 8
    printf("dist_sq: %d\n", point_dist_sq(a, b));

    // EXPECT: compare ab: 1
    printf("compare ab: %d\n", point_compare(a, b) > 0 ? 1 : 0);

    Size s = size_new(10, 5);
    // EXPECT: area: 50
    printf("area: %d\n", size_area(s));

    // EXPECT: perimeter: 30
    printf("perimeter: %d\n", size_perimeter(s));

    Rect r = rect_new(2, 3, 10, 5);
    // EXPECT: rect area: 50
    printf("rect area: %d\n", rect_area(r));

    // EXPECT: rect right: 12
    printf("rect right: %d\n", rect_right(r));

    // EXPECT: rect bottom: 8
    printf("rect bottom: %d\n", rect_bottom(r));

    Point inside = point_new(5, 5);
    Point outside = point_new(15, 15);
    // EXPECT: contains inside: 1
    printf("contains inside: %d\n", rect_contains_point(r, inside));

    // EXPECT: contains outside: 0
    printf("contains outside: %d\n", rect_contains_point(r, outside));

    Point offset = point_new(10, 20);
    Rect moved = rect_translate(r, offset);
    // EXPECT: moved origin: 12 23
    printf("moved origin: %d %d\n", moved.origin.x, moved.origin.y);

    // EXPECT: moved area: 50
    printf("moved area: %d\n", rect_area(moved));

    Point zero = point_new(0, 0);
    // EXPECT: dist_sq from origin: 25
    printf("dist_sq from origin: %d\n", point_dist_sq(a, zero));

    return 0;
}
