int printf(const char *fmt, ...);

struct Point {
    int x;
    int y;
};

struct Rect {
    int x;
    int y;
    int w;
    int h;
};

struct Color {
    int r;
    int g;
    int b;
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

int point_distance_sq(struct Point a, struct Point b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return dx * dx + dy * dy;
}

int point_equal(struct Point a, struct Point b) {
    return a.x == b.x && a.y == b.y;
}

struct Rect make_rect(int x, int y, int w, int h) {
    struct Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    return r;
}

int rect_area(struct Rect r) {
    return r.w * r.h;
}

int rect_perimeter(struct Rect r) {
    return 2 * (r.w + r.h);
}

int rect_contains_point(struct Rect r, struct Point p) {
    return p.x >= r.x && p.x < r.x + r.w &&
           p.y >= r.y && p.y < r.y + r.h;
}

int rect_overlaps(struct Rect a, struct Rect b) {
    if (a.x + a.w <= b.x) return 0;
    if (b.x + b.w <= a.x) return 0;
    if (a.y + a.h <= b.y) return 0;
    if (b.y + b.h <= a.y) return 0;
    return 1;
}

void move_rect(struct Rect *r, int dx, int dy) {
    r->x = r->x + dx;
    r->y = r->y + dy;
}

struct Color make_color(int r, int g, int b) {
    struct Color c;
    c.r = r;
    c.g = g;
    c.b = b;
    return c;
}

struct Color blend_colors(struct Color a, struct Color b) {
    struct Color result;
    result.r = (a.r + b.r) / 2;
    result.g = (a.g + b.g) / 2;
    result.b = (a.b + b.b) / 2;
    return result;
}

int color_brightness(struct Color c) {
    return (c.r + c.g + c.b) / 3;
}

int color_equal(struct Color a, struct Color b) {
    return a.r == b.r && a.g == b.g && a.b == b.b;
}

int main(void) {
    struct Point p1 = make_point(3, 4);
    // EXPECT: p1: 3 4
    printf("p1: %d %d\n", p1.x, p1.y);

    struct Point p2 = make_point(1, 2);
    struct Point p3 = add_points(p1, p2);
    // EXPECT: add: 4 6
    printf("add: %d %d\n", p3.x, p3.y);

    struct Point p4 = scale_point(p1, 3);
    // EXPECT: scale: 9 12
    printf("scale: %d %d\n", p4.x, p4.y);

    // EXPECT: dist_sq: 8
    printf("dist_sq: %d\n", point_distance_sq(p1, p2));

    // EXPECT: equal_same: 1
    printf("equal_same: %d\n", point_equal(p1, p1));

    // EXPECT: equal_diff: 0
    printf("equal_diff: %d\n", point_equal(p1, p2));

    struct Rect r1 = make_rect(0, 0, 10, 5);
    // EXPECT: area: 50
    printf("area: %d\n", rect_area(r1));

    // EXPECT: perim: 30
    printf("perim: %d\n", rect_perimeter(r1));

    struct Point inside = make_point(5, 3);
    struct Point outside = make_point(15, 3);
    // EXPECT: contains_in: 1
    printf("contains_in: %d\n", rect_contains_point(r1, inside));

    // EXPECT: contains_out: 0
    printf("contains_out: %d\n", rect_contains_point(r1, outside));

    struct Rect r2 = make_rect(5, 3, 10, 10);
    // EXPECT: overlap: 1
    printf("overlap: %d\n", rect_overlaps(r1, r2));

    struct Rect r3 = make_rect(20, 20, 5, 5);
    // EXPECT: no_overlap: 0
    printf("no_overlap: %d\n", rect_overlaps(r1, r3));

    move_rect(&r1, 10, 10);
    // EXPECT: moved: 10 10
    printf("moved: %d %d\n", r1.x, r1.y);

    struct Color red = make_color(255, 0, 0);
    struct Color blue = make_color(0, 0, 255);
    struct Color mix = blend_colors(red, blue);
    // EXPECT: blend: 127 0 127
    printf("blend: %d %d %d\n", mix.r, mix.g, mix.b);

    // EXPECT: bright_red: 85
    printf("bright_red: %d\n", color_brightness(red));

    // EXPECT: color_eq: 0
    printf("color_eq: %d\n", color_equal(red, blue));

    struct Color red2 = make_color(255, 0, 0);
    // EXPECT: color_eq_same: 1
    printf("color_eq_same: %d\n", color_equal(red, red2));

    return 0;
}
