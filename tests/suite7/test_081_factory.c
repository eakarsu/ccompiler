int printf(const char *fmt, ...);

enum ShapeType {
    SHAPE_CIRCLE,
    SHAPE_RECT,
    SHAPE_TRIANGLE
};

struct Shape {
    int type;
    int params[4];
    int area;
};

int compute_circle_area(int radius) {
    return 3 * radius * radius;
}

int compute_rect_area(int w, int h) {
    return w * h;
}

int compute_triangle_area(int base, int height) {
    return (base * height) / 2;
}

struct Shape create_shape(int type, int p0, int p1, int p2, int p3) {
    struct Shape s;
    s.type = type;
    s.params[0] = p0;
    s.params[1] = p1;
    s.params[2] = p2;
    s.params[3] = p3;

    if (type == SHAPE_CIRCLE) {
        s.area = compute_circle_area(p0);
    } else if (type == SHAPE_RECT) {
        s.area = compute_rect_area(p0, p1);
    } else if (type == SHAPE_TRIANGLE) {
        s.area = compute_triangle_area(p0, p1);
    } else {
        s.area = 0;
    }
    return s;
}

int shape_perimeter(struct Shape *s) {
    if (s->type == SHAPE_CIRCLE) {
        return 6 * s->params[0];
    } else if (s->type == SHAPE_RECT) {
        return 2 * (s->params[0] + s->params[1]);
    } else if (s->type == SHAPE_TRIANGLE) {
        return s->params[0] + s->params[1] + s->params[2];
    }
    return 0;
}

char *shape_name(int type) {
    if (type == SHAPE_CIRCLE) return "circle";
    if (type == SHAPE_RECT) return "rect";
    if (type == SHAPE_TRIANGLE) return "triangle";
    return "unknown";
}

int shape_is_large(struct Shape *s) {
    return s->area > 50;
}

int total_area(struct Shape shapes[], int n) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < n; i++) {
        sum = sum + shapes[i].area;
    }
    return sum;
}

int count_by_type(struct Shape shapes[], int n, int type) {
    int count;
    int i;
    count = 0;
    for (i = 0; i < n; i++) {
        if (shapes[i].type == type) {
            count = count + 1;
        }
    }
    return count;
}

int largest_shape(struct Shape shapes[], int n) {
    int best;
    int idx;
    int i;
    best = shapes[0].area;
    idx = 0;
    for (i = 1; i < n; i++) {
        if (shapes[i].area > best) {
            best = shapes[i].area;
            idx = i;
        }
    }
    return idx;
}

int main(void) {
    struct Shape shapes[6];
    int i;
    int idx;

    shapes[0] = create_shape(SHAPE_CIRCLE, 5, 0, 0, 0);
    shapes[1] = create_shape(SHAPE_RECT, 10, 4, 0, 0);
    shapes[2] = create_shape(SHAPE_TRIANGLE, 6, 8, 10, 0);
    shapes[3] = create_shape(SHAPE_CIRCLE, 3, 0, 0, 0);
    shapes[4] = create_shape(SHAPE_RECT, 7, 7, 0, 0);
    shapes[5] = create_shape(SHAPE_TRIANGLE, 10, 5, 12, 0);

    // EXPECT: shape[0]: circle area=75
    printf("shape[0]: %s area=%d\n", shape_name(shapes[0].type), shapes[0].area);
    // EXPECT: shape[1]: rect area=40
    printf("shape[1]: %s area=%d\n", shape_name(shapes[1].type), shapes[1].area);
    // EXPECT: shape[2]: triangle area=24
    printf("shape[2]: %s area=%d\n", shape_name(shapes[2].type), shapes[2].area);
    // EXPECT: shape[3]: circle area=27
    printf("shape[3]: %s area=%d\n", shape_name(shapes[3].type), shapes[3].area);
    // EXPECT: shape[4]: rect area=49
    printf("shape[4]: %s area=%d\n", shape_name(shapes[4].type), shapes[4].area);
    // EXPECT: shape[5]: triangle area=25
    printf("shape[5]: %s area=%d\n", shape_name(shapes[5].type), shapes[5].area);

    // EXPECT: perimeter[0]: 30
    printf("perimeter[0]: %d\n", shape_perimeter(&shapes[0]));
    // EXPECT: perimeter[1]: 28
    printf("perimeter[1]: %d\n", shape_perimeter(&shapes[1]));
    // EXPECT: perimeter[2]: 24
    printf("perimeter[2]: %d\n", shape_perimeter(&shapes[2]));

    // EXPECT: large[0]: 1
    printf("large[0]: %d\n", shape_is_large(&shapes[0]));
    // EXPECT: large[2]: 0
    printf("large[2]: %d\n", shape_is_large(&shapes[2]));

    // EXPECT: total area: 240
    printf("total area: %d\n", total_area(shapes, 6));

    // EXPECT: circles: 2
    printf("circles: %d\n", count_by_type(shapes, 6, SHAPE_CIRCLE));
    // EXPECT: rects: 2
    printf("rects: %d\n", count_by_type(shapes, 6, SHAPE_RECT));
    // EXPECT: triangles: 2
    printf("triangles: %d\n", count_by_type(shapes, 6, SHAPE_TRIANGLE));

    idx = largest_shape(shapes, 6);
    // EXPECT: largest: 0 area=75
    printf("largest: %d area=%d\n", idx, shapes[idx].area);

    return 0;
}
