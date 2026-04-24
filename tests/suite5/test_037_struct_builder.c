int printf(const char *fmt, ...);

struct Config {
    int width;
    int height;
    int depth;
    int color_r;
    int color_g;
    int color_b;
    int border;
    int padding;
    int margin;
    int visible;
    int enabled;
    int layer;
};

void config_init(struct Config *c) {
    c->width = 100;
    c->height = 100;
    c->depth = 1;
    c->color_r = 0;
    c->color_g = 0;
    c->color_b = 0;
    c->border = 0;
    c->padding = 0;
    c->margin = 0;
    c->visible = 1;
    c->enabled = 1;
    c->layer = 0;
}

void config_set_size(struct Config *c, int w, int h) {
    c->width = w;
    c->height = h;
}

void config_set_depth(struct Config *c, int d) {
    c->depth = d;
}

void config_set_color(struct Config *c, int r, int g, int b) {
    c->color_r = r;
    c->color_g = g;
    c->color_b = b;
}

void config_set_border(struct Config *c, int b) {
    c->border = b;
}

void config_set_padding(struct Config *c, int p) {
    c->padding = p;
}

void config_set_margin(struct Config *c, int m) {
    c->margin = m;
}

void config_set_visible(struct Config *c, int v) {
    c->visible = v;
}

void config_set_enabled(struct Config *c, int e) {
    c->enabled = e;
}

void config_set_layer(struct Config *c, int l) {
    c->layer = l;
}

int config_total_width(struct Config *c) {
    return c->width + 2 * c->padding + 2 * c->border + 2 * c->margin;
}

int config_total_height(struct Config *c) {
    return c->height + 2 * c->padding + 2 * c->border + 2 * c->margin;
}

int config_area(struct Config *c) {
    int tw = config_total_width(c);
    int th = config_total_height(c);
    return tw * th;
}

int config_volume(struct Config *c) {
    return config_area(c) * c->depth;
}

int config_color_sum(struct Config *c) {
    return c->color_r + c->color_g + c->color_b;
}

void config_copy(struct Config *dst, struct Config *src) {
    dst->width = src->width;
    dst->height = src->height;
    dst->depth = src->depth;
    dst->color_r = src->color_r;
    dst->color_g = src->color_g;
    dst->color_b = src->color_b;
    dst->border = src->border;
    dst->padding = src->padding;
    dst->margin = src->margin;
    dst->visible = src->visible;
    dst->enabled = src->enabled;
    dst->layer = src->layer;
}

int main(void) {
    struct Config c;
    config_init(&c);

    // EXPECT: def_width=100
    printf("def_width=%d\n", c.width);
    // EXPECT: def_height=100
    printf("def_height=%d\n", c.height);
    // EXPECT: def_visible=1
    printf("def_visible=%d\n", c.visible);

    config_set_size(&c, 200, 150);
    config_set_color(&c, 255, 128, 64);
    config_set_border(&c, 2);
    config_set_padding(&c, 5);
    config_set_margin(&c, 10);
    config_set_depth(&c, 3);
    config_set_layer(&c, 5);

    // EXPECT: width=200
    printf("width=%d\n", c.width);
    // EXPECT: height=150
    printf("height=%d\n", c.height);
    // EXPECT: color_r=255
    printf("color_r=%d\n", c.color_r);

    // EXPECT: total_w=234
    printf("total_w=%d\n", config_total_width(&c));
    // EXPECT: total_h=184
    printf("total_h=%d\n", config_total_height(&c));
    // EXPECT: area=43056
    printf("area=%d\n", config_area(&c));
    // EXPECT: volume=129168
    printf("volume=%d\n", config_volume(&c));
    // EXPECT: color_sum=447
    printf("color_sum=%d\n", config_color_sum(&c));

    struct Config c2;
    config_copy(&c2, &c);
    config_set_size(&c2, 50, 50);
    config_set_color(&c2, 0, 255, 0);
    config_set_border(&c2, 1);
    config_set_padding(&c2, 3);
    config_set_margin(&c2, 0);

    // EXPECT: c2_total_w=58
    printf("c2_total_w=%d\n", config_total_width(&c2));
    // EXPECT: c2_total_h=58
    printf("c2_total_h=%d\n", config_total_height(&c2));
    // EXPECT: c2_area=3364
    printf("c2_area=%d\n", config_area(&c2));

    // EXPECT: c1_unchanged=234
    printf("c1_unchanged=%d\n", config_total_width(&c));

    config_set_visible(&c2, 0);
    config_set_enabled(&c2, 0);
    // EXPECT: c2_vis=0
    printf("c2_vis=%d\n", c2.visible);
    // EXPECT: c2_en=0
    printf("c2_en=%d\n", c2.enabled);

    struct Config arr[4];
    int i = 0;
    while (i < 4) {
        config_init(&arr[i]);
        config_set_size(&arr[i], (i + 1) * 10, (i + 1) * 10);
        config_set_border(&arr[i], i);
        i = i + 1;
    }
    int total_area = 0;
    i = 0;
    while (i < 4) {
        total_area = total_area + config_area(&arr[i]);
        i = i + 1;
    }
    // EXPECT: batch_area=3856
    printf("batch_area=%d\n", total_area);

    return 0;
}
