int printf(const char *fmt, ...);
// EXPECT: Default:\nConfig: 100x50 color=0 border=1 pad=5 vis=1 en=1 label=default\nArea: 5000\nCustom button:\nConfig: 200x100 color=255 border=1 pad=5 vis=1 en=1 label=button\nArea: 20000\nHidden widget:\nConfig: 50x50 color=0 border=0 pad=0 vis=0 en=1 label=hidden\nCanvas:\nConfig: 800x600 color=128 border=3 pad=10 vis=1 en=0 label=canvas\nArea: 480000

struct Config {
    int width;
    int height;
    int color;
    int border;
    int padding;
    int visible;
    int enabled;
    char *label;
};

struct ConfigBuilder {
    struct Config cfg;
};

void builder_init(struct ConfigBuilder *b) {
    b->cfg.width = 100;
    b->cfg.height = 50;
    b->cfg.color = 0;
    b->cfg.border = 1;
    b->cfg.padding = 5;
    b->cfg.visible = 1;
    b->cfg.enabled = 1;
    b->cfg.label = "default";
}

void builder_set_size(struct ConfigBuilder *b, int w, int h) {
    b->cfg.width = w;
    b->cfg.height = h;
}

void builder_set_color(struct ConfigBuilder *b, int c) {
    b->cfg.color = c;
}

void builder_set_border(struct ConfigBuilder *b, int border) {
    b->cfg.border = border;
}

void builder_set_padding(struct ConfigBuilder *b, int padding) {
    b->cfg.padding = padding;
}

void builder_set_visible(struct ConfigBuilder *b, int v) {
    b->cfg.visible = v;
}

void builder_set_enabled(struct ConfigBuilder *b, int e) {
    b->cfg.enabled = e;
}

void builder_set_label(struct ConfigBuilder *b, char *label) {
    b->cfg.label = label;
}

struct Config builder_build(struct ConfigBuilder *b) {
    return b->cfg;
}

void config_print(struct Config *c) {
    printf("Config: %dx%d color=%d border=%d pad=%d vis=%d en=%d label=%s\n",
           c->width, c->height, c->color, c->border,
           c->padding, c->visible, c->enabled, c->label);
}

int config_area(struct Config *c) {
    return c->width * c->height;
}

int main(void) {
    struct ConfigBuilder b;

    builder_init(&b);
    struct Config c1 = builder_build(&b);
    printf("Default:\n");
    config_print(&c1);
    printf("Area: %d\n", config_area(&c1));

    builder_init(&b);
    builder_set_size(&b, 200, 100);
    builder_set_color(&b, 255);
    builder_set_label(&b, "button");
    struct Config c2 = builder_build(&b);
    printf("Custom button:\n");
    config_print(&c2);
    printf("Area: %d\n", config_area(&c2));

    builder_init(&b);
    builder_set_size(&b, 50, 50);
    builder_set_border(&b, 0);
    builder_set_padding(&b, 0);
    builder_set_visible(&b, 0);
    builder_set_label(&b, "hidden");
    struct Config c3 = builder_build(&b);
    printf("Hidden widget:\n");
    config_print(&c3);

    builder_init(&b);
    builder_set_size(&b, 800, 600);
    builder_set_color(&b, 128);
    builder_set_border(&b, 3);
    builder_set_padding(&b, 10);
    builder_set_enabled(&b, 0);
    builder_set_label(&b, "canvas");
    struct Config c4 = builder_build(&b);
    printf("Canvas:\n");
    config_print(&c4);
    printf("Area: %d\n", config_area(&c4));

    return 0;
}
