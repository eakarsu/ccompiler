int printf(const char *fmt, ...);

// Test global struct variables, modifying from functions, global struct arrays.

struct Point {
    int x;
    int y;
};

struct Rect {
    struct Point origin;
    int width;
    int height;
};

struct Point g_origin;
struct Point g_p1 = {10, 20};
struct Point g_p2 = {30, 40};

struct Rect g_rect;

struct Point g_points[4];

int point_sum(struct Point *p) {
    return p->x + p->y;
}

void set_point(struct Point *p, int x, int y) {
    p->x = x;
    p->y = y;
}

void move_point(struct Point *p, int dx, int dy) {
    p->x = p->x + dx;
    p->y = p->y + dy;
}

int rect_area(struct Rect *r) {
    return r->width * r->height;
}

void init_rect(struct Rect *r, int x, int y, int w, int h) {
    r->origin.x = x;
    r->origin.y = y;
    r->width = w;
    r->height = h;
}

int rect_perimeter(struct Rect *r) {
    return 2 * (r->width + r->height);
}

void swap_points(struct Point *a, struct Point *b) {
    struct Point tmp;
    tmp.x = a->x;
    tmp.y = a->y;
    a->x = b->x;
    a->y = b->y;
    b->x = tmp.x;
    b->y = tmp.y;
}

int distance_sq(struct Point *a, struct Point *b) {
    int dx = a->x - b->x;
    int dy = a->y - b->y;
    return dx * dx + dy * dy;
}

void fill_points_array(void) {
    int i;
    for (i = 0; i < 4; i = i + 1) {
        g_points[i].x = i * 10;
        g_points[i].y = i * 10 + 5;
    }
}

int sum_all_points(void) {
    int i;
    int total = 0;
    for (i = 0; i < 4; i = i + 1) {
        total = total + g_points[i].x + g_points[i].y;
    }
    return total;
}

int main(void) {
    // Uninitialized global struct should be zero
    // EXPECT: 0
    printf("%d\n", g_origin.x);
    // EXPECT: 0
    printf("%d\n", g_origin.y);

    // Initialized global struct
    // EXPECT: 10
    printf("%d\n", g_p1.x);
    // EXPECT: 20
    printf("%d\n", g_p1.y);

    // EXPECT: 30
    printf("%d\n", point_sum(&g_p1));

    // Modify via function
    set_point(&g_origin, 5, 7);
    // EXPECT: 5
    printf("%d\n", g_origin.x);
    // EXPECT: 7
    printf("%d\n", g_origin.y);

    // Move point
    move_point(&g_p1, 3, -5);
    // EXPECT: 13
    printf("%d\n", g_p1.x);
    // EXPECT: 15
    printf("%d\n", g_p1.y);

    // Test rect
    init_rect(&g_rect, 0, 0, 10, 20);
    // EXPECT: 200
    printf("%d\n", rect_area(&g_rect));
    // EXPECT: 60
    printf("%d\n", rect_perimeter(&g_rect));
    // EXPECT: 0
    printf("%d\n", g_rect.origin.x);

    // Swap global points
    // g_p1 = {13, 15}, g_p2 = {30, 40}
    swap_points(&g_p1, &g_p2);
    // EXPECT: 30
    printf("%d\n", g_p1.x);
    // EXPECT: 40
    printf("%d\n", g_p1.y);
    // EXPECT: 13
    printf("%d\n", g_p2.x);
    // EXPECT: 15
    printf("%d\n", g_p2.y);

    // Distance squared between p1(30,40) and p2(13,15)
    // dx=17, dy=25, 289+625=914
    // EXPECT: 914
    printf("%d\n", distance_sq(&g_p1, &g_p2));

    // Test global struct array
    fill_points_array();
    // points = {(0,5),(10,15),(20,25),(30,35)}
    // EXPECT: 0
    printf("%d\n", g_points[0].x);
    // EXPECT: 5
    printf("%d\n", g_points[0].y);
    // EXPECT: 20
    printf("%d\n", g_points[2].x);
    // EXPECT: 35
    printf("%d\n", g_points[3].y);

    // sum = (0+5)+(10+15)+(20+25)+(30+35) = 5+25+45+65 = 140
    // EXPECT: 140
    printf("%d\n", sum_all_points());

    // Modify individual element in array
    g_points[1].x = 100;
    // EXPECT: 100
    printf("%d\n", g_points[1].x);

    // Nested struct field via rect
    g_rect.origin.x = 42;
    g_rect.origin.y = 99;
    // EXPECT: 42
    printf("%d\n", g_rect.origin.x);
    // EXPECT: 99
    printf("%d\n", g_rect.origin.y);

    return 0;
}
