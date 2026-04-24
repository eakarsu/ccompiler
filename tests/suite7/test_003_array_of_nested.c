int printf(const char *fmt, ...);

struct Point {
    int x;
    int y;
};

struct Rect {
    struct Point origin;
    struct Point size;
};

struct Scene {
    struct Rect rects[4];
    int count;
};

int rect_area(struct Rect *r) {
    return r->size.x * r->size.y;
}

int rect_perimeter(struct Rect *r) {
    return 2 * (r->size.x + r->size.y);
}

void rect_init(struct Rect *r, int ox, int oy, int w, int h) {
    r->origin.x = ox;
    r->origin.y = oy;
    r->size.x = w;
    r->size.y = h;
}

void scene_add(struct Scene *s, int ox, int oy, int w, int h) {
    if (s->count < 4) {
        rect_init(&s->rects[s->count], ox, oy, w, h);
        s->count = s->count + 1;
    }
}

int scene_total_area(struct Scene *s) {
    int total = 0;
    int i;
    for (i = 0; i < s->count; i = i + 1) {
        total = total + rect_area(&s->rects[i]);
    }
    return total;
}

int scene_total_perimeter(struct Scene *s) {
    int total = 0;
    int i;
    for (i = 0; i < s->count; i = i + 1) {
        total = total + rect_perimeter(&s->rects[i]);
    }
    return total;
}

void scene_translate(struct Scene *s, int dx, int dy) {
    int i;
    for (i = 0; i < s->count; i = i + 1) {
        s->rects[i].origin.x = s->rects[i].origin.x + dx;
        s->rects[i].origin.y = s->rects[i].origin.y + dy;
    }
}

int scene_rightmost(struct Scene *s) {
    int max_x = -9999;
    int i;
    for (i = 0; i < s->count; i = i + 1) {
        int right = s->rects[i].origin.x + s->rects[i].size.x;
        if (right > max_x) max_x = right;
    }
    return max_x;
}

int scene_bottommost(struct Scene *s) {
    int max_y = -9999;
    int i;
    for (i = 0; i < s->count; i = i + 1) {
        int bottom = s->rects[i].origin.y + s->rects[i].size.y;
        if (bottom > max_y) max_y = bottom;
    }
    return max_y;
}

int point_dist_sq(struct Point *a, struct Point *b) {
    int dx = a->x - b->x;
    int dy = a->y - b->y;
    return dx * dx + dy * dy;
}

int main(void) {
    struct Scene scene;
    scene.count = 0;

    scene_add(&scene, 0, 0, 10, 5);
    scene_add(&scene, 20, 10, 8, 6);
    scene_add(&scene, 5, 15, 3, 3);
    scene_add(&scene, 30, 0, 4, 12);

    // EXPECT: count=4
    printf("count=%d\n", scene.count);

    // EXPECT: rect[0] area=50
    printf("rect[0] area=%d\n", rect_area(&scene.rects[0]));
    // EXPECT: rect[1] area=48
    printf("rect[1] area=%d\n", rect_area(&scene.rects[1]));
    // EXPECT: rect[2] area=9
    printf("rect[2] area=%d\n", rect_area(&scene.rects[2]));
    // EXPECT: rect[3] area=48
    printf("rect[3] area=%d\n", rect_area(&scene.rects[3]));

    // EXPECT: total area=155
    printf("total area=%d\n", scene_total_area(&scene));

    // EXPECT: rect[0] perimeter=30
    printf("rect[0] perimeter=%d\n", rect_perimeter(&scene.rects[0]));

    // EXPECT: total perimeter=102
    printf("total perimeter=%d\n", scene_total_perimeter(&scene));

    // EXPECT: rightmost=34
    printf("rightmost=%d\n", scene_rightmost(&scene));
    // EXPECT: bottommost=18
    printf("bottommost=%d\n", scene_bottommost(&scene));

    scene_translate(&scene, 100, 200);
    // EXPECT: after translate: rect[0] origin=100,200
    printf("after translate: rect[0] origin=%d,%d\n",
           scene.rects[0].origin.x, scene.rects[0].origin.y);
    // EXPECT: after translate: rect[1] origin=120,210
    printf("after translate: rect[1] origin=%d,%d\n",
           scene.rects[1].origin.x, scene.rects[1].origin.y);

    // EXPECT: rightmost after=134
    printf("rightmost after=%d\n", scene_rightmost(&scene));

    struct Point p1;
    p1.x = scene.rects[0].origin.x;
    p1.y = scene.rects[0].origin.y;
    struct Point p2;
    p2.x = scene.rects[1].origin.x;
    p2.y = scene.rects[1].origin.y;
    // EXPECT: dist_sq=500
    printf("dist_sq=%d\n", point_dist_sq(&p1, &p2));

    // EXPECT: areas unchanged=155
    printf("areas unchanged=%d\n", scene_total_area(&scene));

    return 0;
}
