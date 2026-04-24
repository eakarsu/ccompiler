int printf(const char *fmt, ...);

// Factory Pattern: struct creation via factory functions

enum ShapeType { SHAPE_CIRCLE, SHAPE_RECT, SHAPE_TRIANGLE };

struct Shape {
    enum ShapeType type;
    int params[4];
    char *name;
};

struct ShapePool {
    struct Shape shapes[16];
    int count;
};

void pool_init(struct ShapePool *pool) {
    pool->count = 0;
}

struct Shape *create_circle(struct ShapePool *pool, int radius) {
    struct Shape *s;
    s = &pool->shapes[pool->count];
    pool->count = pool->count + 1;
    s->type = SHAPE_CIRCLE;
    s->params[0] = radius;
    s->params[1] = 0;
    s->params[2] = 0;
    s->params[3] = 0;
    s->name = "circle";
    return s;
}

struct Shape *create_rect(struct ShapePool *pool, int w, int h) {
    struct Shape *s;
    s = &pool->shapes[pool->count];
    pool->count = pool->count + 1;
    s->type = SHAPE_RECT;
    s->params[0] = w;
    s->params[1] = h;
    s->params[2] = 0;
    s->params[3] = 0;
    s->name = "rect";
    return s;
}

struct Shape *create_triangle(struct ShapePool *pool, int base, int height) {
    struct Shape *s;
    s = &pool->shapes[pool->count];
    pool->count = pool->count + 1;
    s->type = SHAPE_TRIANGLE;
    s->params[0] = base;
    s->params[1] = height;
    s->params[2] = 0;
    s->params[3] = 0;
    s->name = "triangle";
    return s;
}

int shape_area_approx(struct Shape *s) {
    if (s->type == SHAPE_CIRCLE) {
        return 3 * s->params[0] * s->params[0];
    } else if (s->type == SHAPE_RECT) {
        return s->params[0] * s->params[1];
    } else if (s->type == SHAPE_TRIANGLE) {
        return (s->params[0] * s->params[1]) / 2;
    }
    return 0;
}

int shape_perimeter_approx(struct Shape *s) {
    if (s->type == SHAPE_CIRCLE) {
        return 6 * s->params[0];
    } else if (s->type == SHAPE_RECT) {
        return 2 * (s->params[0] + s->params[1]);
    } else if (s->type == SHAPE_TRIANGLE) {
        return s->params[0] + s->params[1] + s->params[0];
    }
    return 0;
}

char *shape_type_name(enum ShapeType t) {
    if (t == SHAPE_CIRCLE) return "Circle";
    if (t == SHAPE_RECT) return "Rectangle";
    return "Triangle";
}

void describe_shape(struct Shape *s) {
    printf("Shape: %s, area: %d, perim: %d\n",
        shape_type_name(s->type),
        shape_area_approx(s),
        shape_perimeter_approx(s));
}

int main() {
    struct ShapePool pool;
    pool_init(&pool);

    struct Shape *c1;
    c1 = create_circle(&pool, 5);
    printf("Created: %s, radius: %d\n", c1->name, c1->params[0]); // EXPECT: Created: circle, radius: 5

    struct Shape *r1;
    r1 = create_rect(&pool, 4, 6);
    printf("Created: %s, w: %d, h: %d\n", r1->name, r1->params[0], r1->params[1]); // EXPECT: Created: rect, w: 4, h: 6

    struct Shape *t1;
    t1 = create_triangle(&pool, 8, 3);
    printf("Created: %s, b: %d, h: %d\n", t1->name, t1->params[0], t1->params[1]); // EXPECT: Created: triangle, b: 8, h: 3

    printf("Pool count: %d\n", pool.count); // EXPECT: Pool count: 3

    describe_shape(c1); // EXPECT: Shape: Circle, area: 75, perim: 30
    describe_shape(r1); // EXPECT: Shape: Rectangle, area: 24, perim: 20
    describe_shape(t1); // EXPECT: Shape: Triangle, area: 12, perim: 19

    struct Shape *c2;
    c2 = create_circle(&pool, 10);
    struct Shape *r2;
    r2 = create_rect(&pool, 7, 7);

    printf("Pool count: %d\n", pool.count); // EXPECT: Pool count: 5

    int total_area;
    total_area = 0;
    int i;
    for (i = 0; i < pool.count; i = i + 1) {
        total_area = total_area + shape_area_approx(&pool.shapes[i]);
    }
    printf("Total area: %d\n", total_area); // EXPECT: Total area: 460

    int circles;
    int rects;
    int tris;
    circles = 0;
    rects = 0;
    tris = 0;
    for (i = 0; i < pool.count; i = i + 1) {
        if (pool.shapes[i].type == SHAPE_CIRCLE) circles = circles + 1;
        else if (pool.shapes[i].type == SHAPE_RECT) rects = rects + 1;
        else tris = tris + 1;
    }
    printf("Circles: %d, Rects: %d, Triangles: %d\n", circles, rects, tris); // EXPECT: Circles: 2, Rects: 2, Triangles: 1

    printf("Done\n"); // EXPECT: Done
    return 0;
}
