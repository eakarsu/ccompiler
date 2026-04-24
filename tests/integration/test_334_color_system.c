int printf(const char *fmt, ...);
// EXPECT: red=(255,0,0)\ngreen=(0,255,0)\nblue=(0,0,255)\nwhite=(255,255,255)\nblack=(0,0,0)\nbrightness red=85\nbrightness white=255\nblend red+blue=(127,0,127)\ninvert red=(0,255,255)\ngrayscale white=1\ngrayscale red=0\nyellow=(255,255,0)\nbrightness yellow=170\nblend cyan+magenta=(127,127,255)
// Test: enum-based color system

enum ColorName {
    COLOR_BLACK,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_YELLOW,
    COLOR_CYAN,
    COLOR_MAGENTA,
    COLOR_WHITE
};

struct RGB {
    int r;
    int g;
    int b;
};

struct RGB color_to_rgb(enum ColorName c) {
    struct RGB rgb;
    rgb.r = 0; rgb.g = 0; rgb.b = 0;
    switch (c) {
        case COLOR_BLACK:   break;
        case COLOR_RED:     rgb.r = 255; break;
        case COLOR_GREEN:   rgb.g = 255; break;
        case COLOR_BLUE:    rgb.b = 255; break;
        case COLOR_YELLOW:  rgb.r = 255; rgb.g = 255; break;
        case COLOR_CYAN:    rgb.g = 255; rgb.b = 255; break;
        case COLOR_MAGENTA: rgb.r = 255; rgb.b = 255; break;
        case COLOR_WHITE:   rgb.r = 255; rgb.g = 255; rgb.b = 255; break;
    }
    return rgb;
}

int rgb_brightness(struct RGB *c) {
    return (c->r + c->g + c->b) / 3;
}

struct RGB rgb_blend(struct RGB *a, struct RGB *b) {
    struct RGB result;
    result.r = (a->r + b->r) / 2;
    result.g = (a->g + b->g) / 2;
    result.b = (a->b + b->b) / 2;
    return result;
}

struct RGB rgb_invert(struct RGB *c) {
    struct RGB result;
    result.r = 255 - c->r;
    result.g = 255 - c->g;
    result.b = 255 - c->b;
    return result;
}

int rgb_equal(struct RGB *a, struct RGB *b) {
    return a->r == b->r && a->g == b->g && a->b == b->b;
}

void rgb_print(struct RGB *c) {
    printf("(%d,%d,%d)", c->r, c->g, c->b);
}

int is_grayscale(struct RGB *c) {
    return c->r == c->g && c->g == c->b;
}

int main(void) {
    struct RGB red = color_to_rgb(COLOR_RED);
    struct RGB green = color_to_rgb(COLOR_GREEN);
    struct RGB blue = color_to_rgb(COLOR_BLUE);
    struct RGB white = color_to_rgb(COLOR_WHITE);
    struct RGB black = color_to_rgb(COLOR_BLACK);

    printf("red="); rgb_print(&red); printf("\n");
    printf("green="); rgb_print(&green); printf("\n");
    printf("blue="); rgb_print(&blue); printf("\n");
    printf("white="); rgb_print(&white); printf("\n");
    printf("black="); rgb_print(&black); printf("\n");

    printf("brightness red=%d\n", rgb_brightness(&red));
    printf("brightness white=%d\n", rgb_brightness(&white));

    struct RGB blend = rgb_blend(&red, &blue);
    printf("blend red+blue="); rgb_print(&blend); printf("\n");

    struct RGB inv = rgb_invert(&red);
    printf("invert red="); rgb_print(&inv); printf("\n");

    printf("grayscale white=%d\n", is_grayscale(&white));
    printf("grayscale red=%d\n", is_grayscale(&red));

    struct RGB yellow = color_to_rgb(COLOR_YELLOW);
    printf("yellow="); rgb_print(&yellow); printf("\n");
    printf("brightness yellow=%d\n", rgb_brightness(&yellow));

    struct RGB cyan = color_to_rgb(COLOR_CYAN);
    struct RGB magenta = color_to_rgb(COLOR_MAGENTA);
    struct RGB blend2 = rgb_blend(&cyan, &magenta);
    printf("blend cyan+magenta="); rgb_print(&blend2); printf("\n");

    return 0;
}
