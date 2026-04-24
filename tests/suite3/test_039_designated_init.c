int printf(const char *fmt, ...);

struct Point {
    int x;
    int y;
};

struct Color {
    int r;
    int g;
    int b;
};

struct Pixel {
    struct Point pos;
    struct Color col;
    int          alpha;
};

struct Config {
    int width;
    int height;
    int flags;
    int depth;
    int padding;
};

struct Named {
    int a;
    int b;
    int c;
    int d;
};

int sum_config(struct Config *c) {
    return c->width + c->height + c->flags + c->depth + c->padding;
}

int main(void) {
    /* --- Designated init for a simple struct --- */
    struct Point p1 = { .x = 10, .y = 20 };
    printf("%d\n", p1.x);                  // EXPECT: 10
    printf("%d\n", p1.y);                  // EXPECT: 20

    /* Reverse order */
    struct Point p2 = { .y = 99, .x = 77 };
    printf("%d\n", p2.x);                  // EXPECT: 77
    printf("%d\n", p2.y);                  // EXPECT: 99

    /* Partial: unspecified members default to 0 */
    struct Color c1 = { .r = 255 };
    printf("%d\n", c1.r);                  // EXPECT: 255
    printf("%d\n", c1.g);                  // EXPECT: 0
    printf("%d\n", c1.b);                  // EXPECT: 0

    /* Only middle field */
    struct Color c2 = { .g = 128 };
    printf("%d\n", c2.r);                  // EXPECT: 0
    printf("%d\n", c2.g);                  // EXPECT: 128
    printf("%d\n", c2.b);                  // EXPECT: 0

    /* Only last field */
    struct Color c3 = { .b = 64 };
    printf("%d\n", c3.r);                  // EXPECT: 0
    printf("%d\n", c3.g);                  // EXPECT: 0
    printf("%d\n", c3.b);                  // EXPECT: 64

    /* --- Designated init for array --- */
    int arr[6] = { [0] = 10, [3] = 30, [5] = 50 };
    printf("%d\n", arr[0]);               // EXPECT: 10
    printf("%d\n", arr[1]);               // EXPECT: 0
    printf("%d\n", arr[2]);               // EXPECT: 0
    printf("%d\n", arr[3]);               // EXPECT: 30
    printf("%d\n", arr[4]);               // EXPECT: 0
    printf("%d\n", arr[5]);               // EXPECT: 50

    /* Reverse-order array designators */
    int arr2[4] = { [3] = 99, [1] = 55 };
    printf("%d\n", arr2[0]);              // EXPECT: 0
    printf("%d\n", arr2[1]);              // EXPECT: 55
    printf("%d\n", arr2[2]);              // EXPECT: 0
    printf("%d\n", arr2[3]);              // EXPECT: 99

    /* --- Nested designated init .outer.inner = val --- */
    struct Pixel px = {
        .pos.x = 5,
        .pos.y = 15,
        .col.r = 200,
        .col.g = 100,
        .col.b = 50,
        .alpha  = 128
    };
    printf("%d\n", px.pos.x);             // EXPECT: 5
    printf("%d\n", px.pos.y);             // EXPECT: 15
    printf("%d\n", px.col.r);             // EXPECT: 200
    printf("%d\n", px.col.g);             // EXPECT: 100
    printf("%d\n", px.col.b);             // EXPECT: 50
    printf("%d\n", px.alpha);             // EXPECT: 128

    /* --- Config: all fields via designators --- */
    struct Config cfg = {
        .width   = 1920,
        .height  = 1080,
        .flags   = 3,
        .depth   = 32,
        .padding = 0
    };
    printf("%d\n", cfg.width);            // EXPECT: 1920
    printf("%d\n", cfg.height);           // EXPECT: 1080
    printf("%d\n", cfg.flags);            // EXPECT: 3
    printf("%d\n", cfg.depth);            // EXPECT: 32
    /* sum = 1920+1080+3+32+0 = 3035 */
    printf("%d\n", sum_config(&cfg));     // EXPECT: 3035

    /* --- Partial designated init: unspecified = 0 --- */
    struct Config cfg2 = { .depth = 8 };
    printf("%d\n", cfg2.width);           // EXPECT: 0
    printf("%d\n", cfg2.height);          // EXPECT: 0
    printf("%d\n", cfg2.depth);           // EXPECT: 8
    printf("%d\n", cfg2.flags);           // EXPECT: 0

    /* --- Named struct with designators out of order --- */
    struct Named nm = { .d = 40, .b = 20, .a = 10, .c = 30 };
    printf("%d\n", nm.a);                 // EXPECT: 10
    printf("%d\n", nm.b);                 // EXPECT: 20
    printf("%d\n", nm.c);                 // EXPECT: 30
    printf("%d\n", nm.d);                 // EXPECT: 40

    /* --- Array of structs with designated init --- */
    struct Point pts[3] = {
        [0] = { .x = 1, .y = 2 },
        [2] = { .x = 9, .y = 8 }
    };
    printf("%d %d\n", pts[0].x, pts[0].y); // EXPECT: 1 2
    printf("%d %d\n", pts[1].x, pts[1].y); // EXPECT: 0 0
    printf("%d %d\n", pts[2].x, pts[2].y); // EXPECT: 9 8

    /* --- Mix designated and positional --- */
    struct Named mix = { 1, 2, .d = 100 };
    /* positional fills a=1, b=2; .d=100; c unspecified=0 */
    printf("%d\n", mix.a);                // EXPECT: 1
    printf("%d\n", mix.b);                // EXPECT: 2
    printf("%d\n", mix.c);                // EXPECT: 0
    printf("%d\n", mix.d);                // EXPECT: 100

    return 0;
}
