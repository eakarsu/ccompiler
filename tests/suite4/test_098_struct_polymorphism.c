int printf(const char *fmt, ...);

enum ShapeType {
    SHAPE_CIRCLE,
    SHAPE_RECT,
    SHAPE_TRIANGLE
};

struct Circle {
    int radius;
};

struct Rectangle {
    int width;
    int height;
};

struct Triangle {
    int base;
    int height;
};

union ShapeData {
    struct Circle circle;
    struct Rectangle rect;
    struct Triangle tri;
};

struct Shape {
    int type;
    union ShapeData data;
    int x;
    int y;
};

/* Area computation: multiply by 100 for integer approximation */
/* Circle: pi * r * r ~ 314 * r * r / 100 */
int shape_area(struct Shape *s) {
    switch (s->type) {
        case SHAPE_CIRCLE:
            return 314 * s->data.circle.radius * s->data.circle.radius / 100;
        case SHAPE_RECT:
            return s->data.rect.width * s->data.rect.height;
        case SHAPE_TRIANGLE:
            return s->data.tri.base * s->data.tri.height / 2;
    }
    return 0;
}

/* Perimeter * 100 for circles, exact for others */
int shape_perimeter(struct Shape *s) {
    switch (s->type) {
        case SHAPE_CIRCLE:
            /* 2 * pi * r ~ 628 * r / 100 */
            return 628 * s->data.circle.radius / 100;
        case SHAPE_RECT:
            return 2 * (s->data.rect.width + s->data.rect.height);
        case SHAPE_TRIANGLE: {
            /* Approximate: base + 2 * height (rough for right triangle) */
            return s->data.tri.base + 2 * s->data.tri.height;
        }
    }
    return 0;
}

int shape_contains_point(struct Shape *s, int px, int py) {
    int dx = px - s->x;
    int dy = py - s->y;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;

    switch (s->type) {
        case SHAPE_CIRCLE:
            /* Simple bounding box check instead of true distance */
            return dx <= s->data.circle.radius && dy <= s->data.circle.radius;
        case SHAPE_RECT:
            return dx <= s->data.rect.width / 2 && dy <= s->data.rect.height / 2;
        case SHAPE_TRIANGLE:
            return dx <= s->data.tri.base / 2 && dy <= s->data.tri.height / 2;
    }
    return 0;
}

void shape_move(struct Shape *s, int dx, int dy) {
    s->x = s->x + dx;
    s->y = s->y + dy;
}

struct Shape make_circle(int x, int y, int r) {
    struct Shape s;
    s.type = SHAPE_CIRCLE;
    s.data.circle.radius = r;
    s.x = x;
    s.y = y;
    return s;
}

struct Shape make_rect(int x, int y, int w, int h) {
    struct Shape s;
    s.type = SHAPE_RECT;
    s.data.rect.width = w;
    s.data.rect.height = h;
    s.x = x;
    s.y = y;
    return s;
}

struct Shape make_triangle(int x, int y, int b, int h) {
    struct Shape s;
    s.type = SHAPE_TRIANGLE;
    s.data.tri.base = b;
    s.data.tri.height = h;
    s.x = x;
    s.y = y;
    return s;
}

int main(void) {
    /* Create shapes */
    struct Shape c1 = make_circle(0, 0, 10);
    struct Shape r1 = make_rect(5, 5, 20, 10);
    struct Shape t1 = make_triangle(3, 3, 12, 8);

    /* Test type tags */
    // EXPECT: Circle type: 0
    printf("Circle type: %d\n", c1.type);
    // EXPECT: Rect type: 1
    printf("Rect type: %d\n", r1.type);
    // EXPECT: Triangle type: 2
    printf("Triangle type: %d\n", t1.type);

    /* Test areas */
    /* Circle: 314 * 10 * 10 / 100 = 314 */
    // EXPECT: Circle area: 314
    printf("Circle area: %d\n", shape_area(&c1));
    /* Rect: 20 * 10 = 200 */
    // EXPECT: Rect area: 200
    printf("Rect area: %d\n", shape_area(&r1));
    /* Triangle: 12 * 8 / 2 = 48 */
    // EXPECT: Triangle area: 48
    printf("Triangle area: %d\n", shape_area(&t1));

    /* Test perimeters */
    /* Circle: 628 * 10 / 100 = 62 */
    // EXPECT: Circle perimeter: 62
    printf("Circle perimeter: %d\n", shape_perimeter(&c1));
    /* Rect: 2 * (20 + 10) = 60 */
    // EXPECT: Rect perimeter: 60
    printf("Rect perimeter: %d\n", shape_perimeter(&r1));
    /* Triangle: 12 + 2*8 = 28 */
    // EXPECT: Triangle perimeter: 28
    printf("Triangle perimeter: %d\n", shape_perimeter(&t1));

    /* Test contains point */
    // EXPECT: Circle contains (0,0): 1
    printf("Circle contains (0,0): %d\n", shape_contains_point(&c1, 0, 0));
    // EXPECT: Circle contains (5,5): 1
    printf("Circle contains (5,5): %d\n", shape_contains_point(&c1, 5, 5));
    // EXPECT: Circle contains (20,20): 0
    printf("Circle contains (20,20): %d\n", shape_contains_point(&c1, 20, 20));

    // EXPECT: Rect contains (5,5): 1
    printf("Rect contains (5,5): %d\n", shape_contains_point(&r1, 5, 5));
    // EXPECT: Rect contains (100,100): 0
    printf("Rect contains (100,100): %d\n", shape_contains_point(&r1, 100, 100));

    /* Test move */
    shape_move(&c1, 10, 20);
    // EXPECT: Circle after move: 10 20
    printf("Circle after move: %d %d\n", c1.x, c1.y);

    /* Test with array of shapes (polymorphic dispatch) */
    struct Shape shapes[4];
    shapes[0] = make_circle(0, 0, 5);
    shapes[1] = make_rect(0, 0, 10, 4);
    shapes[2] = make_triangle(0, 0, 6, 4);
    shapes[3] = make_circle(0, 0, 3);

    int total_area = 0;
    int i;
    for (i = 0; i < 4; i = i + 1) {
        total_area = total_area + shape_area(&shapes[i]);
    }
    /* Circle(5): 314*25/100=78, Rect: 40, Tri: 12, Circle(3): 314*9/100=28 */
    /* Total: 78 + 40 + 12 + 28 = 158 */
    // EXPECT: Total area: 158
    printf("Total area: %d\n", total_area);

    int total_perim = 0;
    for (i = 0; i < 4; i = i + 1) {
        total_perim = total_perim + shape_perimeter(&shapes[i]);
    }
    /* Circle(5): 628*5/100=31, Rect: 2*(10+4)=28, Tri: 6+2*4=14, Circle(3): 628*3/100=18 */
    /* Total: 31 + 28 + 14 + 18 = 91 */
    // EXPECT: Total perimeter: 91
    printf("Total perimeter: %d\n", total_perim);

    /* Count shapes by type */
    int circles = 0;
    int rects = 0;
    int tris = 0;
    for (i = 0; i < 4; i = i + 1) {
        switch (shapes[i].type) {
            case SHAPE_CIRCLE: circles = circles + 1; break;
            case SHAPE_RECT: rects = rects + 1; break;
            case SHAPE_TRIANGLE: tris = tris + 1; break;
        }
    }
    // EXPECT: Circles: 2
    printf("Circles: %d\n", circles);
    // EXPECT: Rects: 1
    printf("Rects: %d\n", rects);
    // EXPECT: Triangles: 1
    printf("Triangles: %d\n", tris);

    /* Move all shapes and verify */
    for (i = 0; i < 4; i = i + 1) {
        shape_move(&shapes[i], i * 10, i * 5);
    }
    // EXPECT: Shape 0 pos: 0 0
    printf("Shape 0 pos: %d %d\n", shapes[0].x, shapes[0].y);
    // EXPECT: Shape 3 pos: 30 15
    printf("Shape 3 pos: %d %d\n", shapes[3].x, shapes[3].y);

    return 0;
}
