// stress_086_object_system.c - Object system with vtable pattern, polymorphism
// EXPECT: circle_area=314
// EXPECT: circle_perim=62
// EXPECT: circle_name=Circle
// EXPECT: rect_area=200
// EXPECT: rect_perim=60
// EXPECT: rect_name=Rectangle
// EXPECT: tri_area=600
// EXPECT: tri_perim=120
// EXPECT: tri_name=Triangle
// EXPECT: poly_0=Circle:314
// EXPECT: poly_1=Rectangle:200
// EXPECT: poly_2=Triangle:600
// EXPECT: poly_3=Circle:1256
// EXPECT: poly_4=Rectangle:2500
// EXPECT: total_area=4870
// EXPECT: total_perim=567
// EXPECT: largest=Rectangle:2500
// EXPECT: clone_circle=314
// EXPECT: clone_rect=200
// EXPECT: describe_circle=Circle(r=10)
// EXPECT: describe_rect=Rectangle(20x10)
// EXPECT: describe_tri=Triangle(30,40,50)
// EXPECT: shapes_by_area=200 314 600 1256 2500
// EXPECT: pass_done=1

int printf(const char *fmt, ...);

// --- VTable (interface) ---
struct Shape;

struct ShapeVTable {
    int (*area)(struct Shape *self);
    int (*perimeter)(struct Shape *self);
    char *(*name)(struct Shape *self);
    void (*describe)(struct Shape *self);
};

// --- Base "class" ---
struct Shape {
    struct ShapeVTable *vtable;
};

// --- Circle ---
struct Circle {
    struct Shape base;
    int radius;
};

int circle_area(struct Shape *self) {
    struct Circle *c = (struct Circle *)self;
    // Approximate pi as 314/100
    return c->radius * c->radius * 314 / 100;
}

int circle_perimeter(struct Shape *self) {
    struct Circle *c = (struct Circle *)self;
    return 2 * c->radius * 314 / 100;
}

char *circle_name(struct Shape *self) {
    return "Circle";
}

void circle_describe(struct Shape *self) {
    struct Circle *c = (struct Circle *)self;
    printf("Circle(r=%d)", c->radius);
}

struct ShapeVTable circle_vtable;

void init_circle(struct Circle *c, int radius) {
    c->base.vtable = &circle_vtable;
    c->radius = radius;
}

// --- Rectangle ---
struct Rectangle {
    struct Shape base;
    int width;
    int height;
};

int rect_area(struct Shape *self) {
    struct Rectangle *r = (struct Rectangle *)self;
    return r->width * r->height;
}

int rect_perimeter(struct Shape *self) {
    struct Rectangle *r = (struct Rectangle *)self;
    return 2 * (r->width + r->height);
}

char *rect_name(struct Shape *self) {
    return "Rectangle";
}

void rect_describe(struct Shape *self) {
    struct Rectangle *r = (struct Rectangle *)self;
    printf("Rectangle(%dx%d)", r->width, r->height);
}

struct ShapeVTable rect_vtable;

void init_rect(struct Rectangle *r, int w, int h) {
    r->base.vtable = &rect_vtable;
    r->width = w;
    r->height = h;
}

// --- Triangle ---
struct Triangle {
    struct Shape base;
    int a;
    int b;
    int c;
    int height;  // height for area computation (base = a)
};

int tri_area(struct Shape *self) {
    struct Triangle *t = (struct Triangle *)self;
    return t->a * t->height / 2;
}

int tri_perimeter(struct Shape *self) {
    struct Triangle *t = (struct Triangle *)self;
    return t->a + t->b + t->c;
}

char *tri_name(struct Shape *self) {
    return "Triangle";
}

void tri_describe(struct Shape *self) {
    struct Triangle *t = (struct Triangle *)self;
    printf("Triangle(%d,%d,%d)", t->a, t->b, t->c);
}

struct ShapeVTable tri_vtable;

void init_tri(struct Triangle *t, int a, int b, int c, int h) {
    t->base.vtable = &tri_vtable;
    t->a = a;
    t->b = b;
    t->c = c;
    t->height = h;
}

// --- Virtual dispatch helpers ---
int shape_area(struct Shape *s) {
    return s->vtable->area(s);
}

int shape_perimeter(struct Shape *s) {
    return s->vtable->perimeter(s);
}

char *shape_name(struct Shape *s) {
    return s->vtable->name(s);
}

void shape_describe(struct Shape *s) {
    s->vtable->describe(s);
}

// --- Bubble sort shapes by area ---
void sort_shapes_by_area(struct Shape **arr, int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (shape_area(arr[j]) > shape_area(arr[j + 1])) {
                struct Shape *tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

int main(void) {
    // Initialize vtables
    circle_vtable.area = circle_area;
    circle_vtable.perimeter = circle_perimeter;
    circle_vtable.name = circle_name;
    circle_vtable.describe = circle_describe;

    rect_vtable.area = rect_area;
    rect_vtable.perimeter = rect_perimeter;
    rect_vtable.name = rect_name;
    rect_vtable.describe = rect_describe;

    tri_vtable.area = tri_area;
    tri_vtable.perimeter = tri_perimeter;
    tri_vtable.name = tri_name;
    tri_vtable.describe = tri_describe;

    // Create shapes
    struct Circle c1;
    init_circle(&c1, 10);

    struct Rectangle r1;
    init_rect(&r1, 20, 10);

    struct Triangle t1;
    init_tri(&t1, 30, 40, 50, 40);

    // Direct calls through vtable
    printf("circle_area=%d\n", shape_area((struct Shape *)&c1));
    printf("circle_perim=%d\n", shape_perimeter((struct Shape *)&c1));
    printf("circle_name=%s\n", shape_name((struct Shape *)&c1));

    printf("rect_area=%d\n", shape_area((struct Shape *)&r1));
    printf("rect_perim=%d\n", shape_perimeter((struct Shape *)&r1));
    printf("rect_name=%s\n", shape_name((struct Shape *)&r1));

    printf("tri_area=%d\n", shape_area((struct Shape *)&t1));
    printf("tri_perim=%d\n", shape_perimeter((struct Shape *)&t1));
    printf("tri_name=%s\n", shape_name((struct Shape *)&t1));

    // Polymorphic array
    struct Circle c2;
    init_circle(&c2, 20);
    struct Rectangle r2;
    init_rect(&r2, 50, 50);

    struct Shape *shapes[5];
    shapes[0] = (struct Shape *)&c1;
    shapes[1] = (struct Shape *)&r1;
    shapes[2] = (struct Shape *)&t1;
    shapes[3] = (struct Shape *)&c2;
    shapes[4] = (struct Shape *)&r2;

    int i;
    for (i = 0; i < 5; i++) {
        printf("poly_%d=%s:%d\n", i, shape_name(shapes[i]), shape_area(shapes[i]));
    }

    // Compute totals
    int total_area = 0;
    int total_perim = 0;
    for (i = 0; i < 5; i++) {
        total_area += shape_area(shapes[i]);
        total_perim += shape_perimeter(shapes[i]);
    }
    printf("total_area=%d\n", total_area);
    printf("total_perim=%d\n", total_perim);

    // Find largest by area
    int max_area = 0;
    int max_idx = 0;
    for (i = 0; i < 5; i++) {
        int a = shape_area(shapes[i]);
        if (a > max_area) {
            max_area = a;
            max_idx = i;
        }
    }
    printf("largest=%s:%d\n", shape_name(shapes[max_idx]), max_area);

    // "Clone" test - create copies and verify
    struct Circle c3;
    c3 = c1; // struct copy
    printf("clone_circle=%d\n", shape_area((struct Shape *)&c3));

    struct Rectangle r3;
    r3 = r1;
    printf("clone_rect=%d\n", shape_area((struct Shape *)&r3));

    // Describe
    printf("describe_circle=");
    shape_describe((struct Shape *)&c1);
    printf("\n");

    printf("describe_rect=");
    shape_describe((struct Shape *)&r1);
    printf("\n");

    printf("describe_tri=");
    shape_describe((struct Shape *)&t1);
    printf("\n");

    // Sort by area and print
    sort_shapes_by_area(shapes, 5);
    printf("shapes_by_area=");
    for (i = 0; i < 5; i++) {
        if (i > 0) printf(" ");
        printf("%d", shape_area(shapes[i]));
    }
    printf("\n");

    printf("pass_done=1\n");
    return 0;
}
