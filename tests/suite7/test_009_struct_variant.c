int printf(const char *fmt, ...);

struct Point2D {
    int x;
    int y;
};

struct Circle {
    int cx;
    int cy;
    int radius;
};

struct Rect {
    int x;
    int y;
    int w;
    int h;
};

struct Line {
    int x1;
    int y1;
    int x2;
    int y2;
};

union ShapeData {
    struct Point2D point;
    struct Circle circle;
    struct Rect rect;
    struct Line line;
};

struct Shape {
    int type;
    union ShapeData data;
};

struct Shape make_point(int x, int y) {
    struct Shape s;
    s.type = 0;
    s.data.point.x = x;
    s.data.point.y = y;
    return s;
}

struct Shape make_circle(int cx, int cy, int r) {
    struct Shape s;
    s.type = 1;
    s.data.circle.cx = cx;
    s.data.circle.cy = cy;
    s.data.circle.radius = r;
    return s;
}

struct Shape make_rect(int x, int y, int w, int h) {
    struct Shape s;
    s.type = 2;
    s.data.rect.x = x;
    s.data.rect.y = y;
    s.data.rect.w = w;
    s.data.rect.h = h;
    return s;
}

struct Shape make_line(int x1, int y1, int x2, int y2) {
    struct Shape s;
    s.type = 3;
    s.data.line.x1 = x1;
    s.data.line.y1 = y1;
    s.data.line.x2 = x2;
    s.data.line.y2 = y2;
    return s;
}

int shape_area(struct Shape *s) {
    if (s->type == 1) {
        /* approximate circle area as 3 * r * r */
        return 3 * s->data.circle.radius * s->data.circle.radius;
    }
    if (s->type == 2) {
        return s->data.rect.w * s->data.rect.h;
    }
    return 0;
}

int shape_perimeter(struct Shape *s) {
    if (s->type == 1) {
        /* approximate circle perimeter as 6 * r */
        return 6 * s->data.circle.radius;
    }
    if (s->type == 2) {
        return 2 * (s->data.rect.w + s->data.rect.h);
    }
    if (s->type == 3) {
        int dx = s->data.line.x2 - s->data.line.x1;
        int dy = s->data.line.y2 - s->data.line.y1;
        /* return squared length for simplicity */
        return dx * dx + dy * dy;
    }
    return 0;
}

char *shape_name(struct Shape *s) {
    if (s->type == 0) return "point";
    if (s->type == 1) return "circle";
    if (s->type == 2) return "rect";
    if (s->type == 3) return "line";
    return "unknown";
}

void shape_translate(struct Shape *s, int dx, int dy) {
    if (s->type == 0) {
        s->data.point.x = s->data.point.x + dx;
        s->data.point.y = s->data.point.y + dy;
    } else if (s->type == 1) {
        s->data.circle.cx = s->data.circle.cx + dx;
        s->data.circle.cy = s->data.circle.cy + dy;
    } else if (s->type == 2) {
        s->data.rect.x = s->data.rect.x + dx;
        s->data.rect.y = s->data.rect.y + dy;
    } else if (s->type == 3) {
        s->data.line.x1 = s->data.line.x1 + dx;
        s->data.line.y1 = s->data.line.y1 + dy;
        s->data.line.x2 = s->data.line.x2 + dx;
        s->data.line.y2 = s->data.line.y2 + dy;
    }
}

int main(void) {
    struct Shape shapes[6];
    shapes[0] = make_point(5, 10);
    shapes[1] = make_circle(0, 0, 10);
    shapes[2] = make_rect(1, 2, 8, 6);
    shapes[3] = make_line(0, 0, 3, 4);
    shapes[4] = make_circle(10, 20, 5);
    shapes[5] = make_rect(0, 0, 4, 4);

    // EXPECT: shape[0]=point
    printf("shape[0]=%s\n", shape_name(&shapes[0]));
    // EXPECT: shape[1]=circle
    printf("shape[1]=%s\n", shape_name(&shapes[1]));
    // EXPECT: shape[2]=rect
    printf("shape[2]=%s\n", shape_name(&shapes[2]));
    // EXPECT: shape[3]=line
    printf("shape[3]=%s\n", shape_name(&shapes[3]));

    // EXPECT: point coords=5,10
    printf("point coords=%d,%d\n", shapes[0].data.point.x, shapes[0].data.point.y);

    // EXPECT: circle area=300
    printf("circle area=%d\n", shape_area(&shapes[1]));
    // EXPECT: circle perim=60
    printf("circle perim=%d\n", shape_perimeter(&shapes[1]));

    // EXPECT: rect area=48
    printf("rect area=%d\n", shape_area(&shapes[2]));
    // EXPECT: rect perim=28
    printf("rect perim=%d\n", shape_perimeter(&shapes[2]));

    // EXPECT: line length_sq=25
    printf("line length_sq=%d\n", shape_perimeter(&shapes[3]));

    // EXPECT: point area=0
    printf("point area=%d\n", shape_area(&shapes[0]));

    int total_area = 0;
    int i;
    for (i = 0; i < 6; i = i + 1) {
        total_area = total_area + shape_area(&shapes[i]);
    }
    // EXPECT: total area=439
    printf("total area=%d\n", total_area);

    shape_translate(&shapes[0], 100, 200);
    // EXPECT: translated point=105,210
    printf("translated point=%d,%d\n", shapes[0].data.point.x, shapes[0].data.point.y);

    shape_translate(&shapes[2], 10, 20);
    // EXPECT: translated rect=11,22
    printf("translated rect=%d,%d\n", shapes[2].data.rect.x, shapes[2].data.rect.y);
    // EXPECT: rect area unchanged=48
    printf("rect area unchanged=%d\n", shape_area(&shapes[2]));

    // EXPECT: small circle area=75
    printf("small circle area=%d\n", shape_area(&shapes[4]));

    // EXPECT: square area=16
    printf("square area=%d\n", shape_area(&shapes[5]));

    return 0;
}
