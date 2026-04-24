int printf(const char *fmt, ...);

struct Point {
    int x;
    int y;
};

struct Rect {
    int x;
    int y;
    int w;
    int h;
};

struct Color {
    int r;
    int g;
    int b;
};

/* Functions that take struct by value */
int point_sum(struct Point p) {
    return p.x + p.y;
}

int rect_area(struct Rect r) {
    return r.w * r.h;
}

int rect_perimeter(struct Rect r) {
    return 2 * (r.w + r.h);
}

struct Point add_points(struct Point a, struct Point b) {
    struct Point r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

int color_brightness(struct Color c) {
    /* simple average */
    return (c.r + c.g + c.b) / 3;
}

/* Function that takes pointer to struct */
void shift_point(struct Point *p, int dx, int dy) {
    p->x += dx;
    p->y += dy;
}

int is_inside(struct Rect r, struct Point p) {
    return p.x >= r.x && p.x < r.x + r.w &&
           p.y >= r.y && p.y < r.y + r.h;
}

int main(void) {
    /* --- Compound literal: create struct in-place --- */
    struct Point p = (struct Point){3, 4};
    printf("%d\n", p.x);                         // EXPECT: 3
    printf("%d\n", p.y);                         // EXPECT: 4

    /* --- Pass compound literal directly to function --- */
    printf("%d\n", point_sum((struct Point){5, 6}));    // EXPECT: 11
    printf("%d\n", point_sum((struct Point){0, 0}));    // EXPECT: 0
    printf("%d\n", point_sum((struct Point){-3, 3}));   // EXPECT: 0
    printf("%d\n", point_sum((struct Point){100, 200})); // EXPECT: 300

    /* --- Rect compound literal --- */
    printf("%d\n", rect_area((struct Rect){0, 0, 8, 5}));        // EXPECT: 40
    printf("%d\n", rect_perimeter((struct Rect){0, 0, 6, 4}));   // EXPECT: 20

    /* --- Member access on compound literal --- */
    printf("%d\n", (struct Point){7, 8}.x);      // EXPECT: 7
    printf("%d\n", (struct Point){7, 8}.y);      // EXPECT: 8
    printf("%d\n", (struct Rect){1, 2, 10, 20}.w); // EXPECT: 10
    printf("%d\n", (struct Rect){1, 2, 10, 20}.h); // EXPECT: 20

    /* --- Compound literal as argument to multi-arg function --- */
    struct Point sum = add_points((struct Point){1, 2}, (struct Point){3, 4});
    printf("%d\n", sum.x);                       // EXPECT: 4
    printf("%d\n", sum.y);                       // EXPECT: 6

    sum = add_points((struct Point){10, 20}, (struct Point){-5, -10});
    printf("%d\n", sum.x);                       // EXPECT: 5
    printf("%d\n", sum.y);                       // EXPECT: 10

    /* --- Color compound literal --- */
    printf("%d\n", color_brightness((struct Color){90, 90, 90}));  // EXPECT: 90
    printf("%d\n", color_brightness((struct Color){0, 0, 0}));     // EXPECT: 0
    printf("%d\n", color_brightness((struct Color){120, 60, 30})); // EXPECT: 70

    /* --- Compound literal assigned to variable, then modified --- */
    struct Rect r = (struct Rect){10, 20, 30, 40};
    printf("%d\n", r.x);                         // EXPECT: 10
    printf("%d\n", r.y);                         // EXPECT: 20
    printf("%d\n", r.w);                         // EXPECT: 30
    printf("%d\n", r.h);                         // EXPECT: 40
    printf("%d\n", rect_area(r));                // EXPECT: 1200

    /* Modify and reassign via another compound literal */
    r = (struct Rect){0, 0, 5, 5};
    printf("%d\n", rect_area(r));                // EXPECT: 25

    /* --- is_inside: rect and point compound literals --- */
    printf("%d\n", is_inside((struct Rect){0, 0, 10, 10}, (struct Point){5, 5}));  // EXPECT: 1
    printf("%d\n", is_inside((struct Rect){0, 0, 10, 10}, (struct Point){10, 5})); // EXPECT: 0
    printf("%d\n", is_inside((struct Rect){0, 0, 10, 10}, (struct Point){-1, 5})); // EXPECT: 0
    printf("%d\n", is_inside((struct Rect){2, 3, 5, 4},   (struct Point){4, 5}));  // EXPECT: 1

    /* --- Compound literal in loop --- */
    int i;
    int total = 0;
    for (i = 0; i < 5; i++) {
        total += point_sum((struct Point){i, i * 2});
        /* i=0: 0+0=0, i=1: 1+2=3, i=2: 2+4=6, i=3: 3+6=9, i=4: 4+8=12 => total=30 */
    }
    printf("%d\n", total);                       // EXPECT: 30

    /* --- Store compound literal in array element --- */
    struct Point pts[3];
    pts[0] = (struct Point){10, 10};
    pts[1] = (struct Point){20, 20};
    pts[2] = (struct Point){30, 30};
    int psum = 0;
    for (i = 0; i < 3; i++) {
        psum += point_sum(pts[i]);
    }
    /* (20 + 40 + 60) = 120 */
    printf("%d\n", psum);                        // EXPECT: 120

    /* --- Nested compound literal usage --- */
    /* add_points result used as arg */
    printf("%d\n", point_sum(add_points((struct Point){1,1},
                                        (struct Point){2,2})));  // EXPECT: 6
    /* (1+2)+(1+2) = 3+3 = 6 */

    return 0;
}
