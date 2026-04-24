int printf(const char *fmt, ...);

enum ShapeType {
    SHAPE_CIRCLE,
    SHAPE_RECT,
    SHAPE_TRIANGLE
};

struct Circle {
    int radius;
    int cx;
    int cy;
};

struct Rectangle {
    int width;
    int height;
    int x;
    int y;
};

struct Triangle {
    int base;
    int height;
    int x;
    int y;
};

union ShapeData {
    struct Circle circle;
    struct Rectangle rect;
    struct Triangle tri;
};

struct Shape {
    int type;
    union ShapeData data;
};

void init_circle(struct Shape *s, int r, int cx, int cy) {
    s->type = SHAPE_CIRCLE;
    s->data.circle.radius = r;
    s->data.circle.cx = cx;
    s->data.circle.cy = cy;
}

void init_rect(struct Shape *s, int w, int h, int x, int y) {
    s->type = SHAPE_RECT;
    s->data.rect.width = w;
    s->data.rect.height = h;
    s->data.rect.x = x;
    s->data.rect.y = y;
}

void init_triangle(struct Shape *s, int base, int h, int x, int y) {
    s->type = SHAPE_TRIANGLE;
    s->data.tri.base = base;
    s->data.tri.height = h;
    s->data.tri.x = x;
    s->data.tri.y = y;
}

int shape_area_approx(struct Shape *s) {
    if (s->type == SHAPE_CIRCLE) {
        return 3 * s->data.circle.radius * s->data.circle.radius;
    } else if (s->type == SHAPE_RECT) {
        return s->data.rect.width * s->data.rect.height;
    } else if (s->type == SHAPE_TRIANGLE) {
        return (s->data.tri.base * s->data.tri.height) / 2;
    }
    return 0;
}

int shape_perimeter_approx(struct Shape *s) {
    if (s->type == SHAPE_CIRCLE) {
        return 6 * s->data.circle.radius;
    } else if (s->type == SHAPE_RECT) {
        return 2 * (s->data.rect.width + s->data.rect.height);
    } else if (s->type == SHAPE_TRIANGLE) {
        return s->data.tri.base * 3;
    }
    return 0;
}

void print_shape(struct Shape *s) {
    if (s->type == SHAPE_CIRCLE) {
        printf("Circle: r=%d at (%d,%d)\n",
               s->data.circle.radius, s->data.circle.cx, s->data.circle.cy);
    } else if (s->type == SHAPE_RECT) {
        printf("Rect: %dx%d at (%d,%d)\n",
               s->data.rect.width, s->data.rect.height,
               s->data.rect.x, s->data.rect.y);
    } else if (s->type == SHAPE_TRIANGLE) {
        printf("Tri: base=%d h=%d at (%d,%d)\n",
               s->data.tri.base, s->data.tri.height,
               s->data.tri.x, s->data.tri.y);
    }
}

void translate_shape(struct Shape *s, int dx, int dy) {
    if (s->type == SHAPE_CIRCLE) {
        s->data.circle.cx = s->data.circle.cx + dx;
        s->data.circle.cy = s->data.circle.cy + dy;
    } else if (s->type == SHAPE_RECT) {
        s->data.rect.x = s->data.rect.x + dx;
        s->data.rect.y = s->data.rect.y + dy;
    } else if (s->type == SHAPE_TRIANGLE) {
        s->data.tri.x = s->data.tri.x + dx;
        s->data.tri.y = s->data.tri.y + dy;
    }
}

int main() {
    struct Shape shapes[3];

    init_circle(&shapes[0], 5, 10, 10);
    init_rect(&shapes[1], 8, 6, 20, 20);
    init_triangle(&shapes[2], 10, 7, 30, 30);

    // EXPECT: Shape 0:
    printf("Shape 0:\n");
    // EXPECT: Circle: r=5 at (10,10)
    print_shape(&shapes[0]);
    // EXPECT: Shape 1:
    printf("Shape 1:\n");
    // EXPECT: Rect: 8x6 at (20,20)
    print_shape(&shapes[1]);
    // EXPECT: Shape 2:
    printf("Shape 2:\n");
    // EXPECT: Tri: base=10 h=7 at (30,30)
    print_shape(&shapes[2]);

    // EXPECT: Areas:
    printf("Areas:\n");
    int i;
    int total_area = 0;
    for (i = 0; i < 3; i = i + 1) {
        int a = shape_area_approx(&shapes[i]);
        total_area = total_area + a;
        // EXPECT:   Shape area: 75
        // EXPECT:   Shape area: 48
        // EXPECT:   Shape area: 35
        printf("  Shape area: %d\n", a);
    }
    // EXPECT: Total area: 158
    printf("Total area: %d\n", total_area);

    // EXPECT: Perimeters:
    printf("Perimeters:\n");
    for (i = 0; i < 3; i = i + 1) {
        // EXPECT:   Shape perim: 30
        // EXPECT:   Shape perim: 28
        // EXPECT:   Shape perim: 30
        printf("  Shape perim: %d\n", shape_perimeter_approx(&shapes[i]));
    }

    translate_shape(&shapes[0], 5, -3);
    // EXPECT: Translated circle:
    printf("Translated circle:\n");
    // EXPECT: Circle: r=5 at (15,7)
    print_shape(&shapes[0]);

    translate_shape(&shapes[1], -10, 10);
    // EXPECT: Translated rect:
    printf("Translated rect:\n");
    // EXPECT: Rect: 8x6 at (10,30)
    print_shape(&shapes[1]);

    // EXPECT: Type of shape 0: 0
    printf("Type of shape 0: %d\n", shapes[0].type);
    // EXPECT: Type of shape 1: 1
    printf("Type of shape 1: %d\n", shapes[1].type);
    // EXPECT: Type of shape 2: 2
    printf("Type of shape 2: %d\n", shapes[2].type);

    return 0;
}
