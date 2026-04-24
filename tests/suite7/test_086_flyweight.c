int printf(const char *fmt, ...);

struct Color {
    int r;
    int g;
    int b;
};

struct ColorPool {
    struct Color colors[16];
    int count;
};

struct Pixel {
    int x;
    int y;
    int color_index;
};

struct Canvas {
    struct Pixel pixels[32];
    int pixel_count;
};

void pool_init(struct ColorPool *pool) {
    pool->count = 0;
}

int pool_find(struct ColorPool *pool, int r, int g, int b) {
    int i;
    for (i = 0; i < pool->count; i++) {
        if (pool->colors[i].r == r &&
            pool->colors[i].g == g &&
            pool->colors[i].b == b) {
            return i;
        }
    }
    return -1;
}

int pool_add(struct ColorPool *pool, int r, int g, int b) {
    int idx;
    idx = pool_find(pool, r, g, b);
    if (idx >= 0) return idx;
    idx = pool->count;
    pool->colors[idx].r = r;
    pool->colors[idx].g = g;
    pool->colors[idx].b = b;
    pool->count = pool->count + 1;
    return idx;
}

int pool_get_r(struct ColorPool *pool, int idx) {
    return pool->colors[idx].r;
}

int pool_get_g(struct ColorPool *pool, int idx) {
    return pool->colors[idx].g;
}

int pool_get_b(struct ColorPool *pool, int idx) {
    return pool->colors[idx].b;
}

void canvas_init(struct Canvas *c) {
    c->pixel_count = 0;
}

void canvas_add_pixel(struct Canvas *c, int x, int y, int color_idx) {
    struct Pixel *p;
    p = &c->pixels[c->pixel_count];
    p->x = x;
    p->y = y;
    p->color_index = color_idx;
    c->pixel_count = c->pixel_count + 1;
}

int canvas_count_color(struct Canvas *c, int color_idx) {
    int i;
    int count;
    count = 0;
    for (i = 0; i < c->pixel_count; i++) {
        if (c->pixels[i].color_index == color_idx) {
            count = count + 1;
        }
    }
    return count;
}

int canvas_unique_colors(struct Canvas *c) {
    int seen[16];
    int n;
    int i;
    int j;
    int found;
    n = 0;
    for (i = 0; i < c->pixel_count; i++) {
        found = 0;
        for (j = 0; j < n; j++) {
            if (seen[j] == c->pixels[i].color_index) {
                found = 1;
            }
        }
        if (found == 0) {
            seen[n] = c->pixels[i].color_index;
            n = n + 1;
        }
    }
    return n;
}

int memory_saved(struct Canvas *c, int color_size) {
    int without_sharing;
    int with_sharing;
    int unique;
    unique = canvas_unique_colors(c);
    without_sharing = c->pixel_count * color_size;
    with_sharing = unique * color_size + c->pixel_count;
    return without_sharing - with_sharing;
}

int main(void) {
    struct ColorPool pool;
    struct Canvas canvas;
    int red;
    int green;
    int blue;
    int white;
    int red2;
    int i;

    pool_init(&pool);
    canvas_init(&canvas);

    red = pool_add(&pool, 255, 0, 0);
    green = pool_add(&pool, 0, 255, 0);
    blue = pool_add(&pool, 0, 0, 255);
    white = pool_add(&pool, 255, 255, 255);

    // EXPECT: pool size: 4
    printf("pool size: %d\n", pool.count);

    // EXPECT: red index: 0
    printf("red index: %d\n", red);
    // EXPECT: green index: 1
    printf("green index: %d\n", green);
    // EXPECT: blue index: 2
    printf("blue index: %d\n", blue);
    // EXPECT: white index: 3
    printf("white index: %d\n", white);

    red2 = pool_add(&pool, 255, 0, 0);
    // EXPECT: red2 reused: 0
    printf("red2 reused: %d\n", red2);
    // EXPECT: pool still: 4
    printf("pool still: %d\n", pool.count);

    // EXPECT: red color: 255 0 0
    printf("red color: %d %d %d\n", pool_get_r(&pool, red), pool_get_g(&pool, red), pool_get_b(&pool, red));
    // EXPECT: white color: 255 255 255
    printf("white color: %d %d %d\n", pool_get_r(&pool, white), pool_get_g(&pool, white), pool_get_b(&pool, white));

    for (i = 0; i < 5; i++) {
        canvas_add_pixel(&canvas, i, 0, red);
    }
    for (i = 0; i < 3; i++) {
        canvas_add_pixel(&canvas, i, 1, green);
    }
    for (i = 0; i < 4; i++) {
        canvas_add_pixel(&canvas, i, 2, blue);
    }
    canvas_add_pixel(&canvas, 0, 3, white);
    canvas_add_pixel(&canvas, 1, 3, white);

    // EXPECT: pixel count: 14
    printf("pixel count: %d\n", canvas.pixel_count);
    // EXPECT: red pixels: 5
    printf("red pixels: %d\n", canvas_count_color(&canvas, red));
    // EXPECT: green pixels: 3
    printf("green pixels: %d\n", canvas_count_color(&canvas, green));
    // EXPECT: blue pixels: 4
    printf("blue pixels: %d\n", canvas_count_color(&canvas, blue));
    // EXPECT: white pixels: 2
    printf("white pixels: %d\n", canvas_count_color(&canvas, white));

    // EXPECT: unique colors: 4
    printf("unique colors: %d\n", canvas_unique_colors(&canvas));

    // EXPECT: memory saved: 16
    printf("memory saved: %d\n", memory_saved(&canvas, 3));

    return 0;
}
