int printf(const char *fmt, ...);

typedef int (*AreaFn)(void *self);
typedef int (*PerimFn)(void *self);
typedef int (*DescFn)(void *self);

struct VTable {
    AreaFn area;
    PerimFn perimeter;
    DescFn describe;
};

struct Shape {
    struct VTable *vt;
    int id;
};

struct Rect {
    struct VTable *vt;
    int id;
    int width;
    int height;
};

struct Circle {
    struct VTable *vt;
    int id;
    int radius;
};

struct Triangle {
    struct VTable *vt;
    int id;
    int base;
    int height;
    int side1;
    int side2;
    int side3;
};

int rect_area(void *self) {
    struct Rect *r = (struct Rect *)self;
    return r->width * r->height;
}

int rect_perim(void *self) {
    struct Rect *r = (struct Rect *)self;
    return 2 * (r->width + r->height);
}

int rect_desc(void *self) {
    struct Rect *r = (struct Rect *)self;
    return 1;
}

int circle_area(void *self) {
    struct Circle *c = (struct Circle *)self;
    return 3 * c->radius * c->radius;
}

int circle_perim(void *self) {
    struct Circle *c = (struct Circle *)self;
    return 6 * c->radius;
}

int circle_desc(void *self) {
    struct Circle *c = (struct Circle *)self;
    return 2;
}

int tri_area(void *self) {
    struct Triangle *t = (struct Triangle *)self;
    return (t->base * t->height) / 2;
}

int tri_perim(void *self) {
    struct Triangle *t = (struct Triangle *)self;
    return t->side1 + t->side2 + t->side3;
}

int tri_desc(void *self) {
    struct Triangle *t = (struct Triangle *)self;
    return 3;
}

int shape_area(struct Shape *s) {
    return s->vt->area((void *)s);
}

int shape_perim(struct Shape *s) {
    return s->vt->perimeter((void *)s);
}

int shape_desc(struct Shape *s) {
    return s->vt->describe((void *)s);
}

struct VTable rect_vt;
struct VTable circle_vt;
struct VTable tri_vt;

int main(void) {
    rect_vt.area = rect_area;
    rect_vt.perimeter = rect_perim;
    rect_vt.describe = rect_desc;

    circle_vt.area = circle_area;
    circle_vt.perimeter = circle_perim;
    circle_vt.describe = circle_desc;

    tri_vt.area = tri_area;
    tri_vt.perimeter = tri_perim;
    tri_vt.describe = tri_desc;

    struct Rect r;
    r.vt = &rect_vt;
    r.id = 1;
    r.width = 5;
    r.height = 10;

    struct Circle c;
    c.vt = &circle_vt;
    c.id = 2;
    c.radius = 7;

    struct Triangle t;
    t.vt = &tri_vt;
    t.id = 3;
    t.base = 6;
    t.height = 8;
    t.side1 = 6;
    t.side2 = 8;
    t.side3 = 10;

    // EXPECT: rect_area=50
    printf("rect_area=%d\n", shape_area((struct Shape *)&r));
    // EXPECT: rect_perim=30
    printf("rect_perim=%d\n", shape_perim((struct Shape *)&r));
    // EXPECT: rect_desc=1
    printf("rect_desc=%d\n", shape_desc((struct Shape *)&r));

    // EXPECT: circle_area=147
    printf("circle_area=%d\n", shape_area((struct Shape *)&c));
    // EXPECT: circle_perim=42
    printf("circle_perim=%d\n", shape_perim((struct Shape *)&c));
    // EXPECT: circle_desc=2
    printf("circle_desc=%d\n", shape_desc((struct Shape *)&c));

    // EXPECT: tri_area=24
    printf("tri_area=%d\n", shape_area((struct Shape *)&t));
    // EXPECT: tri_perim=24
    printf("tri_perim=%d\n", shape_perim((struct Shape *)&t));
    // EXPECT: tri_desc=3
    printf("tri_desc=%d\n", shape_desc((struct Shape *)&t));

    struct Shape *shapes[3];
    shapes[0] = (struct Shape *)&r;
    shapes[1] = (struct Shape *)&c;
    shapes[2] = (struct Shape *)&t;

    int total_area = 0;
    int total_perim = 0;
    int i = 0;
    while (i < 3) {
        total_area = total_area + shape_area(shapes[i]);
        total_perim = total_perim + shape_perim(shapes[i]);
        i = i + 1;
    }
    // EXPECT: total_area=221
    printf("total_area=%d\n", total_area);
    // EXPECT: total_perim=96
    printf("total_perim=%d\n", total_perim);

    int max_area = 0;
    int max_id = 0;
    i = 0;
    while (i < 3) {
        int a = shape_area(shapes[i]);
        if (a > max_area) {
            max_area = a;
            max_id = shapes[i]->id;
        }
        i = i + 1;
    }
    // EXPECT: max_area=147
    printf("max_area=%d\n", max_area);
    // EXPECT: max_id=2
    printf("max_id=%d\n", max_id);

    r.width = 20;
    r.height = 20;
    // EXPECT: updated_rect_area=400
    printf("updated_rect_area=%d\n", shape_area(shapes[0]));
    // EXPECT: updated_rect_perim=80
    printf("updated_rect_perim=%d\n", shape_perim(shapes[0]));

    return 0;
}
