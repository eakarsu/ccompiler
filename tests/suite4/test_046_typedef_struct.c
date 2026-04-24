int printf(const char *fmt, ...);

struct Point {
    int x;
    int y;
};

typedef struct Point Point;
typedef struct Point *PointPtr;

typedef struct {
    int width;
    int height;
} Rect;

typedef struct {
    int r;
    int g;
    int b;
} Color;

typedef struct {
    Point position;
    Color color;
    int size;
} Sprite;

typedef Rect *RectPtr;

Point make_point(int x, int y) {
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

int point_sum(Point p) {
    return p.x + p.y;
}

int point_ptr_sum(PointPtr p) {
    return p->x + p->y;
}

Rect make_rect(int w, int h) {
    Rect r;
    r.width = w;
    r.height = h;
    return r;
}

int rect_area(Rect r) {
    return r.width * r.height;
}

int rect_ptr_area(RectPtr r) {
    return r->width * r->height;
}

Color make_color(int r, int g, int b) {
    Color c;
    c.r = r;
    c.g = g;
    c.b = b;
    return c;
}

int color_sum(Color c) {
    return c.r + c.g + c.b;
}

void modify_point(PointPtr p, int x, int y) {
    p->x = x;
    p->y = y;
}

int test_basic_typedef_struct(void) {
    Point p;
    p.x = 10;
    p.y = 20;
    return p.x + p.y; // 30
}

int test_make_point(void) {
    Point p = make_point(3, 7);
    return point_sum(p); // 10
}

int test_point_pointer(void) {
    Point p;
    p.x = 5;
    p.y = 15;
    PointPtr pp = &p;
    return point_ptr_sum(pp); // 20
}

int test_rect(void) {
    Rect r = make_rect(6, 8);
    return rect_area(r); // 48
}

int test_rect_ptr(void) {
    Rect r;
    r.width = 10;
    r.height = 5;
    RectPtr rp = &r;
    return rect_ptr_area(rp); // 50
}

int test_color(void) {
    Color c = make_color(100, 150, 200);
    return color_sum(c); // 450
}

int test_modify_via_ptr(void) {
    Point p;
    p.x = 0;
    p.y = 0;
    modify_point(&p, 42, 58);
    return p.x + p.y; // 100
}

int test_sprite(void) {
    Sprite s;
    s.position = make_point(10, 20);
    s.color = make_color(255, 128, 64);
    s.size = 5;
    return s.position.x + s.position.y + s.size;
    // 10 + 20 + 5 = 35
}

int test_sprite_color(void) {
    Sprite s;
    s.position = make_point(0, 0);
    s.color = make_color(10, 20, 30);
    s.size = 1;
    return color_sum(s.color); // 60
}

int test_array_of_typedef(void) {
    Point pts[3];
    pts[0] = make_point(1, 2);
    pts[1] = make_point(3, 4);
    pts[2] = make_point(5, 6);
    int sum = 0;
    int i;
    for (i = 0; i < 3; i = i + 1) {
        sum = sum + pts[i].x + pts[i].y;
    }
    return sum; // 1+2+3+4+5+6 = 21
}

int test_sizeof_typedef(void) {
    // Point has 2 ints = 8 bytes
    return sizeof(Point);
}

int test_sizeof_rect(void) {
    // Rect has 2 ints = 8 bytes
    return sizeof(Rect);
}

int main(void) {
    // EXPECT: basic typedef struct: 30
    printf("basic typedef struct: %d\n", test_basic_typedef_struct());

    // EXPECT: make point: 10
    printf("make point: %d\n", test_make_point());

    // EXPECT: point pointer: 20
    printf("point pointer: %d\n", test_point_pointer());

    // EXPECT: rect area: 48
    printf("rect area: %d\n", test_rect());

    // EXPECT: rect ptr area: 50
    printf("rect ptr area: %d\n", test_rect_ptr());

    // EXPECT: color sum: 450
    printf("color sum: %d\n", test_color());

    // EXPECT: modify via ptr: 100
    printf("modify via ptr: %d\n", test_modify_via_ptr());

    // EXPECT: sprite position+size: 35
    printf("sprite position+size: %d\n", test_sprite());

    // EXPECT: sprite color: 60
    printf("sprite color: %d\n", test_sprite_color());

    // EXPECT: array of typedef: 21
    printf("array of typedef: %d\n", test_array_of_typedef());

    // EXPECT: sizeof Point: 8
    printf("sizeof Point: %d\n", test_sizeof_typedef());

    // EXPECT: sizeof Rect: 8
    printf("sizeof Rect: %d\n", test_sizeof_rect());

    Point p1 = make_point(100, 200);
    Point p2 = make_point(300, 400);
    // EXPECT: two points sum: 1000
    printf("two points sum: %d\n", p1.x + p1.y + p2.x + p2.y);

    Rect rects[2];
    rects[0] = make_rect(3, 4);
    rects[1] = make_rect(5, 6);
    // EXPECT: rect areas: 42
    printf("rect areas: %d\n", rect_area(rects[0]) + rect_area(rects[1]));

    // EXPECT: sizeof Sprite: 24
    printf("sizeof Sprite: %d\n", (int)sizeof(Sprite));

    return 0;
}
