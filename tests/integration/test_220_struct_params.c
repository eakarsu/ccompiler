int printf(const char *fmt, ...);
// EXPECT: dist_sq=25\ntranslated: (13,24)\nscaled: (26,48)\narea=50\nperimeter=30\ncontains(5,3)=1\ncontains(15,3)=0\noverlap=1\nno_overlap=0\ndot=32\nlen_sq=14
// Test 220: Functions with struct parameters (passed by pointer)

struct Point {
    int x;
    int y;
};

struct Rect {
    struct Point top_left;
    struct Point bottom_right;
};

int point_distance_sq(struct Point *a, struct Point *b) {
    int dx = a->x - b->x;
    int dy = a->y - b->y;
    return dx * dx + dy * dy;
}

void point_translate(struct Point *p, int dx, int dy) {
    p->x = p->x + dx;
    p->y = p->y + dy;
}

void point_scale(struct Point *p, int factor) {
    p->x = p->x * factor;
    p->y = p->y * factor;
}

int rect_area(struct Rect *r) {
    int w = r->bottom_right.x - r->top_left.x;
    int h = r->bottom_right.y - r->top_left.y;
    if (w < 0) w = -w;
    if (h < 0) h = -h;
    return w * h;
}

int rect_perimeter(struct Rect *r) {
    int w = r->bottom_right.x - r->top_left.x;
    int h = r->bottom_right.y - r->top_left.y;
    if (w < 0) w = -w;
    if (h < 0) h = -h;
    return 2 * (w + h);
}

int rect_contains_point(struct Rect *r, struct Point *p) {
    return p->x >= r->top_left.x && p->x <= r->bottom_right.x &&
           p->y >= r->top_left.y && p->y <= r->bottom_right.y;
}

int rect_overlap(struct Rect *a, struct Rect *b) {
    if (a->top_left.x > b->bottom_right.x) return 0;
    if (b->top_left.x > a->bottom_right.x) return 0;
    if (a->top_left.y > b->bottom_right.y) return 0;
    if (b->top_left.y > a->bottom_right.y) return 0;
    return 1;
}

struct Vec3 {
    int x;
    int y;
    int z;
};

int vec3_dot(struct Vec3 *a, struct Vec3 *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

int vec3_length_sq(struct Vec3 *v) {
    return v->x * v->x + v->y * v->y + v->z * v->z;
}

int main(void) {
    struct Point p1;
    p1.x = 3; p1.y = 4;
    struct Point p2;
    p2.x = 6; p2.y = 8;
    printf("dist_sq=%d\n", point_distance_sq(&p1, &p2));

    point_translate(&p1, 10, 20);
    printf("translated: (%d,%d)\n", p1.x, p1.y);
    point_scale(&p1, 2);
    printf("scaled: (%d,%d)\n", p1.x, p1.y);

    struct Rect r;
    r.top_left.x = 0; r.top_left.y = 0;
    r.bottom_right.x = 10; r.bottom_right.y = 5;
    printf("area=%d\n", rect_area(&r));
    printf("perimeter=%d\n", rect_perimeter(&r));

    struct Point test;
    test.x = 5; test.y = 3;
    printf("contains(5,3)=%d\n", rect_contains_point(&r, &test));
    test.x = 15; test.y = 3;
    printf("contains(15,3)=%d\n", rect_contains_point(&r, &test));

    struct Rect r2;
    r2.top_left.x = 5; r2.top_left.y = 2;
    r2.bottom_right.x = 15; r2.bottom_right.y = 8;
    printf("overlap=%d\n", rect_overlap(&r, &r2));

    struct Rect r3;
    r3.top_left.x = 20; r3.top_left.y = 20;
    r3.bottom_right.x = 30; r3.bottom_right.y = 30;
    printf("no_overlap=%d\n", rect_overlap(&r, &r3));

    struct Vec3 v1;
    v1.x = 1; v1.y = 2; v1.z = 3;
    struct Vec3 v2;
    v2.x = 4; v2.y = 5; v2.z = 6;
    printf("dot=%d\n", vec3_dot(&v1, &v2));
    printf("len_sq=%d\n", vec3_length_sq(&v1));

    return 0;
}
