int printf(const char *fmt, ...);
// EXPECT: Default: Config: 640x480x32 flags=0\nCustom: Config: 1920x1080x24 flags=1\nCopy: Config: 640x480x32 flags=0\nEqual to default: 1\nEqual to custom: 0\npair[0] = {1, 10}\npair[1] = {2, 20}\npair[2] = {3, 30}\npair[3] = {4, 40}\npair[4] = {5, 50}\ntotal value: 150
// Test: struct initialization

struct Config {
    int width;
    int height;
    int depth;
    int flags;
};

struct Pair {
    int key;
    int value;
};

void print_config(struct Config *c) {
    printf("Config: %dx%dx%d flags=%d\n", c->width, c->height, c->depth, c->flags);
}

struct Config default_config(void) {
    struct Config c;
    c.width = 640;
    c.height = 480;
    c.depth = 32;
    c.flags = 0;
    return c;
}

struct Config custom_config(int w, int h) {
    struct Config c;
    c.width = w;
    c.height = h;
    c.depth = 24;
    c.flags = 1;
    return c;
}

void copy_config(struct Config *dest, struct Config *src) {
    dest->width = src->width;
    dest->height = src->height;
    dest->depth = src->depth;
    dest->flags = src->flags;
}

int configs_equal(struct Config *a, struct Config *b) {
    return a->width == b->width &&
           a->height == b->height &&
           a->depth == b->depth &&
           a->flags == b->flags;
}

int main(void) {
    struct Config def = default_config();
    printf("Default: "); print_config(&def);

    struct Config custom = custom_config(1920, 1080);
    printf("Custom: "); print_config(&custom);

    struct Config copy;
    copy_config(&copy, &def);
    printf("Copy: "); print_config(&copy);
    printf("Equal to default: %d\n", configs_equal(&copy, &def));
    printf("Equal to custom: %d\n", configs_equal(&copy, &custom));

    struct Pair pairs[5];
    int i;
    for (i = 0; i < 5; i++) {
        pairs[i].key = i + 1;
        pairs[i].value = (i + 1) * 10;
    }
    for (i = 0; i < 5; i++) {
        printf("pair[%d] = {%d, %d}\n", i, pairs[i].key, pairs[i].value);
    }

    int total = 0;
    for (i = 0; i < 5; i++) {
        total = total + pairs[i].value;
    }
    printf("total value: %d\n", total);

    return 0;
}
