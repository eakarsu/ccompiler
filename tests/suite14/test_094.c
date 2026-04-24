int printf(const char *fmt, ...);

// Test 094: Nested struct access chains (a.b.c.d)

struct Vec2 {
    int x;
    int y;
};

struct Rect {
    struct Vec2 origin;
    struct Vec2 size;
};

struct Color {
    int r;
    int g;
    int b;
};

struct Widget {
    struct Rect bounds;
    struct Color color;
    int id;
};

struct Layout {
    struct Widget header;
    struct Widget body;
    struct Widget footer;
};

void test_two_deep(void) {
    struct Rect r;
    r.origin.x = 10;
    r.origin.y = 20;
    r.size.x = 100;
    r.size.y = 50;

    printf("%d\n", r.origin.x);
    // EXPECT: 10
    printf("%d\n", r.origin.y);
    // EXPECT: 20
    printf("%d\n", r.size.x);
    // EXPECT: 100
    printf("%d\n", r.size.y);
    // EXPECT: 50

    int area = r.size.x * r.size.y;
    printf("%d\n", area);
    // EXPECT: 5000
}

void test_three_deep(void) {
    struct Widget w;
    w.bounds.origin.x = 5;
    w.bounds.origin.y = 15;
    w.bounds.size.x = 200;
    w.bounds.size.y = 300;
    w.color.r = 255;
    w.color.g = 128;
    w.color.b = 0;
    w.id = 42;

    printf("%d\n", w.bounds.origin.x);
    // EXPECT: 5
    printf("%d\n", w.bounds.size.y);
    // EXPECT: 300
    printf("%d\n", w.color.r);
    // EXPECT: 255
    printf("%d\n", w.color.g);
    // EXPECT: 128
    printf("%d\n", w.id);
    // EXPECT: 42
}

void test_four_deep(void) {
    struct Layout layout;
    layout.header.bounds.origin.x = 0;
    layout.header.bounds.origin.y = 0;
    layout.header.bounds.size.x = 800;
    layout.header.bounds.size.y = 60;
    layout.header.color.r = 50;
    layout.header.color.g = 50;
    layout.header.color.b = 50;
    layout.header.id = 1;

    layout.body.bounds.origin.x = 0;
    layout.body.bounds.origin.y = 60;
    layout.body.bounds.size.x = 800;
    layout.body.bounds.size.y = 500;
    layout.body.color.r = 200;
    layout.body.color.g = 200;
    layout.body.color.b = 200;
    layout.body.id = 2;

    layout.footer.bounds.origin.x = 0;
    layout.footer.bounds.origin.y = 560;
    layout.footer.bounds.size.x = 800;
    layout.footer.bounds.size.y = 40;
    layout.footer.color.r = 100;
    layout.footer.color.g = 100;
    layout.footer.color.b = 100;
    layout.footer.id = 3;

    printf("%d\n", layout.header.bounds.origin.x);
    // EXPECT: 0
    printf("%d\n", layout.header.bounds.size.y);
    // EXPECT: 60
    printf("%d\n", layout.body.bounds.origin.y);
    // EXPECT: 60
    printf("%d\n", layout.body.bounds.size.y);
    // EXPECT: 500
    printf("%d\n", layout.footer.bounds.origin.y);
    // EXPECT: 560
    printf("%d\n", layout.footer.id);
    // EXPECT: 3
}

void test_nested_pointer_access(void) {
    struct Layout layout;
    layout.header.bounds.origin.x = 77;
    layout.header.bounds.origin.y = 88;
    layout.header.color.r = 11;
    layout.header.id = 99;

    struct Layout *lp = &layout;
    printf("%d\n", lp->header.bounds.origin.x);
    // EXPECT: 77
    printf("%d\n", lp->header.bounds.origin.y);
    // EXPECT: 88
    printf("%d\n", lp->header.color.r);
    // EXPECT: 11
    printf("%d\n", lp->header.id);
    // EXPECT: 99

    struct Widget *wp = &lp->header;
    printf("%d\n", wp->bounds.origin.x);
    // EXPECT: 77

    struct Vec2 *vp = &wp->bounds.origin;
    printf("%d\n", vp->x);
    // EXPECT: 77
    printf("%d\n", vp->y);
    // EXPECT: 88
}

void test_modify_nested(void) {
    struct Layout layout;
    layout.body.bounds.size.x = 640;
    layout.body.bounds.size.y = 480;

    struct Layout *lp = &layout;
    lp->body.bounds.size.x = 1024;
    lp->body.bounds.size.y = 768;

    printf("%d\n", layout.body.bounds.size.x);
    // EXPECT: 1024
    printf("%d\n", layout.body.bounds.size.y);
    // EXPECT: 768

    // compute total pixels
    int pixels = layout.body.bounds.size.x * layout.body.bounds.size.y;
    printf("%d\n", pixels);
    // EXPECT: 786432
}

int main(void) {
    test_two_deep();
    test_three_deep();
    test_four_deep();
    test_nested_pointer_access();
    test_modify_nested();
    return 0;
}
