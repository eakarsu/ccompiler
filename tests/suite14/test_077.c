int printf(const char *fmt, ...);

typedef struct {
    int x;
    int y;
    int w;
    int h;
} Rect;

int rect_area(Rect r) {
    return r.w * r.h;
}

int rect_perimeter(Rect r) {
    return 2 * (r.w + r.h);
}

int rect_contains_point(Rect r, int px, int py) {
    return px >= r.x && px < r.x + r.w && py >= r.y && py < r.y + r.h;
}

int rect_contains_rect(Rect outer, Rect inner) {
    return inner.x >= outer.x &&
           inner.y >= outer.y &&
           inner.x + inner.w <= outer.x + outer.w &&
           inner.y + inner.h <= outer.y + outer.h;
}

int max_int(int a, int b) {
    if (a > b) return a;
    return b;
}

int min_int(int a, int b) {
    if (a < b) return a;
    return b;
}

int rects_overlap(Rect a, Rect b) {
    if (a.x >= b.x + b.w || b.x >= a.x + a.w) return 0;
    if (a.y >= b.y + b.h || b.y >= a.y + a.h) return 0;
    return 1;
}

int intersection_area(Rect a, Rect b) {
    int x1 = max_int(a.x, b.x);
    int y1 = max_int(a.y, b.y);
    int x2 = min_int(a.x + a.w, b.x + b.w);
    int y2 = min_int(a.y + a.h, b.y + b.h);
    if (x2 <= x1 || y2 <= y1) return 0;
    return (x2 - x1) * (y2 - y1);
}

int union_area(Rect a, Rect b) {
    return rect_area(a) + rect_area(b) - intersection_area(a, b);
}

Rect bounding_rect(Rect a, Rect b) {
    Rect result;
    result.x = min_int(a.x, b.x);
    result.y = min_int(a.y, b.y);
    int rx = max_int(a.x + a.w, b.x + b.w);
    int ry = max_int(a.y + a.h, b.y + b.h);
    result.w = rx - result.x;
    result.h = ry - result.y;
    return result;
}

Rect make_rect(int x, int y, int w, int h) {
    Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    return r;
}

int main(void) {
    Rect r1 = make_rect(0, 0, 10, 10);
    printf("%d\n", rect_area(r1));
    // EXPECT: 100

    printf("%d\n", rect_perimeter(r1));
    // EXPECT: 40

    printf("%d\n", rect_contains_point(r1, 5, 5));
    // EXPECT: 1

    printf("%d\n", rect_contains_point(r1, 10, 5));
    // EXPECT: 0

    printf("%d\n", rect_contains_point(r1, -1, 5));
    // EXPECT: 0

    Rect r2 = make_rect(5, 5, 10, 10);
    printf("%d\n", rects_overlap(r1, r2));
    // EXPECT: 1

    int ia1 = intersection_area(r1, r2);
    printf("%d\n", ia1);
    // EXPECT: 25

    int ua1 = union_area(r1, r2);
    printf("%d\n", ua1);
    // EXPECT: 175

    Rect r3 = make_rect(20, 20, 5, 5);
    printf("%d\n", rects_overlap(r1, r3));
    // EXPECT: 0

    printf("%d\n", intersection_area(r1, r3));
    // EXPECT: 0

    printf("%d\n", union_area(r1, r3));
    // EXPECT: 125

    Rect r4 = make_rect(2, 2, 3, 3);
    printf("%d\n", rect_contains_rect(r1, r4));
    // EXPECT: 1

    printf("%d\n", rect_contains_rect(r4, r1));
    // EXPECT: 0

    printf("%d\n", rect_contains_rect(r1, r2));
    // EXPECT: 0

    Rect br = bounding_rect(r1, r2);
    printf("%d %d %d %d\n", br.x, br.y, br.w, br.h);
    // EXPECT: 0 0 15 15

    printf("%d\n", rect_area(br));
    // EXPECT: 225

    Rect r5 = make_rect(0, 0, 10, 5);
    Rect r6 = make_rect(0, 5, 10, 5);
    printf("%d\n", intersection_area(r5, r6));
    // EXPECT: 0

    printf("%d\n", union_area(r5, r6));
    // EXPECT: 100

    Rect r7 = make_rect(3, 3, 4, 4);
    Rect r8 = make_rect(5, 5, 4, 4);
    int ia2 = intersection_area(r7, r8);
    printf("%d\n", ia2);
    // EXPECT: 4

    Rect br2 = bounding_rect(r7, r8);
    printf("%d %d %d %d\n", br2.x, br2.y, br2.w, br2.h);
    // EXPECT: 3 3 6 6

    Rect rects[4];
    rects[0] = make_rect(0, 0, 5, 5);
    rects[1] = make_rect(3, 0, 5, 5);
    rects[2] = make_rect(0, 3, 5, 5);
    rects[3] = make_rect(3, 3, 5, 5);

    int total_overlap = 0;
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        for (j = i + 1; j < 4; j++) {
            total_overlap = total_overlap + intersection_area(rects[i], rects[j]);
        }
    }
    printf("%d\n", total_overlap);
    // EXPECT: 48

    printf("done\n");
    // EXPECT: done

    return 0;
}
