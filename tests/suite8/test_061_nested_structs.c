int printf(const char *fmt, ...);

struct Point {
    int x;
    int y;
};

struct Rect {
    struct Point top_left;
    struct Point bottom_right;
};

struct Scene {
    struct Rect bounds;
    struct Rect viewport;
    int layer;
};

int rect_width(struct Rect *r) {
    return r->bottom_right.x - r->top_left.x;
}

int rect_height(struct Rect *r) {
    return r->bottom_right.y - r->top_left.y;
}

int rect_area(struct Rect *r) {
    int w = rect_width(r);
    int h = rect_height(r);
    return w * h;
}

void init_rect(struct Rect *r, int x1, int y1, int x2, int y2) {
    r->top_left.x = x1;
    r->top_left.y = y1;
    r->bottom_right.x = x2;
    r->bottom_right.y = y2;
}

int point_in_rect(struct Point *p, struct Rect *r) {
    if (p->x >= r->top_left.x && p->x <= r->bottom_right.x) {
        if (p->y >= r->top_left.y && p->y <= r->bottom_right.y) {
            return 1;
        }
    }
    return 0;
}

void print_point(struct Point *p) {
    printf("Point(%d, %d)\n", p->x, p->y);
}

void init_scene(struct Scene *s, int bx1, int by1, int bx2, int by2,
                int vx1, int vy1, int vx2, int vy2, int layer) {
    init_rect(&s->bounds, bx1, by1, bx2, by2);
    init_rect(&s->viewport, vx1, vy1, vx2, vy2);
    s->layer = layer;
}

int scene_bounds_area(struct Scene *s) {
    return rect_area(&s->bounds);
}

int scene_viewport_area(struct Scene *s) {
    return rect_area(&s->viewport);
}

int rects_overlap(struct Rect *a, struct Rect *b) {
    if (a->bottom_right.x < b->top_left.x) return 0;
    if (b->bottom_right.x < a->top_left.x) return 0;
    if (a->bottom_right.y < b->top_left.y) return 0;
    if (b->bottom_right.y < a->top_left.y) return 0;
    return 1;
}

void translate_rect(struct Rect *r, int dx, int dy) {
    r->top_left.x = r->top_left.x + dx;
    r->top_left.y = r->top_left.y + dy;
    r->bottom_right.x = r->bottom_right.x + dx;
    r->bottom_right.y = r->bottom_right.y + dy;
}

int main() {
    struct Scene scene;
    init_scene(&scene, 0, 0, 100, 100, 10, 10, 90, 90, 3);

    // EXPECT: Point(0, 0)
    print_point(&scene.bounds.top_left);
    // EXPECT: Point(100, 100)
    print_point(&scene.bounds.bottom_right);

    struct Point test_p;
    test_p.x = 5;
    test_p.y = 5;
    // EXPECT: Point(5, 5)
    print_point(&test_p);
    // EXPECT: Point(90, 90)
    print_point(&scene.viewport.bottom_right);

    // EXPECT: Bounds area: 10000
    printf("Bounds area: %d\n", scene_bounds_area(&scene));

    // EXPECT: Viewport area: 6400
    printf("Viewport area: %d\n", scene_viewport_area(&scene));

    // EXPECT: Width: 100
    printf("Width: %d\n", rect_width(&scene.bounds));
    // EXPECT: Height: 100
    printf("Height: %d\n", rect_height(&scene.bounds));

    // EXPECT: Layer: 3
    printf("Layer: %d\n", scene.layer);

    int inside = point_in_rect(&test_p, &scene.bounds);
    // EXPECT: Inside bounds: 1
    printf("Inside bounds: %d\n", inside);

    int inside_vp = point_in_rect(&test_p, &scene.viewport);
    // EXPECT: Inside viewport: 0
    printf("Inside viewport: %d\n", inside_vp);

    int overlap = rects_overlap(&scene.bounds, &scene.viewport);
    // EXPECT: Overlap: 1
    printf("Overlap: %d\n", overlap);

    struct Rect far;
    init_rect(&far, 200, 200, 300, 300);
    int no_overlap = rects_overlap(&scene.bounds, &far);
    // EXPECT: No overlap: 0
    printf("No overlap: %d\n", no_overlap);

    translate_rect(&scene.viewport, 5, 5);
    // EXPECT: Translated viewport TL: 15, 15
    printf("Translated viewport TL: %d, %d\n", scene.viewport.top_left.x, scene.viewport.top_left.y);
    // EXPECT: Translated viewport BR: 95, 95
    printf("Translated viewport BR: %d, %d\n", scene.viewport.bottom_right.x, scene.viewport.bottom_right.y);

    return 0;
}
