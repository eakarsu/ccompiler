int printf(const char *fmt, ...);

/* Complex enum patterns: nested switch on multiple enums, cross-enum mapping */

enum Color {
    COLOR_RED = 0,
    COLOR_GREEN = 1,
    COLOR_BLUE = 2,
    COLOR_YELLOW = 3,
    COLOR_COUNT = 4
};

enum Size {
    SIZE_SMALL = 0,
    SIZE_MEDIUM = 1,
    SIZE_LARGE = 2,
    SIZE_COUNT = 3
};

enum Shape {
    SHAPE_CIRCLE = 0,
    SHAPE_SQUARE = 1,
    SHAPE_TRIANGLE = 2,
    SHAPE_COUNT = 3
};

/* Item with multiple enum attributes */
struct Item {
    int color;
    int size;
    int shape;
    int id;
};

char *color_name(int c) {
    switch (c) {
        case COLOR_RED:    return "RED";
        case COLOR_GREEN:  return "GREEN";
        case COLOR_BLUE:   return "BLUE";
        case COLOR_YELLOW: return "YELLOW";
        default: return "?";
    }
}

char *size_name(int s) {
    switch (s) {
        case SIZE_SMALL:  return "SMALL";
        case SIZE_MEDIUM: return "MEDIUM";
        case SIZE_LARGE:  return "LARGE";
        default: return "?";
    }
}

char *shape_name(int sh) {
    switch (sh) {
        case SHAPE_CIRCLE:   return "CIRCLE";
        case SHAPE_SQUARE:   return "SQUARE";
        case SHAPE_TRIANGLE: return "TRIANGLE";
        default: return "?";
    }
}

/* Price based on nested switch: size then color */
int get_price(int size, int color) {
    int base = 0;
    switch (size) {
        case SIZE_SMALL:
            switch (color) {
                case COLOR_RED:    base = 10; break;
                case COLOR_GREEN:  base = 12; break;
                case COLOR_BLUE:   base = 15; break;
                case COLOR_YELLOW: base = 11; break;
            }
            break;
        case SIZE_MEDIUM:
            switch (color) {
                case COLOR_RED:    base = 20; break;
                case COLOR_GREEN:  base = 22; break;
                case COLOR_BLUE:   base = 25; break;
                case COLOR_YELLOW: base = 21; break;
            }
            break;
        case SIZE_LARGE:
            switch (color) {
                case COLOR_RED:    base = 30; break;
                case COLOR_GREEN:  base = 32; break;
                case COLOR_BLUE:   base = 35; break;
                case COLOR_YELLOW: base = 31; break;
            }
            break;
    }
    return base;
}

/* Shape complexity score */
int shape_complexity(int shape) {
    switch (shape) {
        case SHAPE_CIRCLE:   return 3;
        case SHAPE_SQUARE:   return 1;
        case SHAPE_TRIANGLE: return 2;
        default: return 0;
    }
}

/* Cross-enum mapping: color to "warmth" */
int color_warmth(int c) {
    /* RED and YELLOW are warm (1), GREEN and BLUE are cool (0) */
    if (c == COLOR_RED || c == COLOR_YELLOW) return 1;
    return 0;
}

/* Size to multiplier */
int size_multiplier(int s) {
    if (s == SIZE_SMALL) return 1;
    if (s == SIZE_MEDIUM) return 2;
    return 3;
}

/* Total score for an item */
int item_score(struct Item *it) {
    return get_price(it->size, it->color) + shape_complexity(it->shape) * size_multiplier(it->size);
}

/* Count items matching a filter */
int count_matching(struct Item items[], int count, int color, int size, int shape) {
    int n = 0;
    int i;
    for (i = 0; i < count; i++) {
        int match = 1;
        if (color >= 0 && items[i].color != color) match = 0;
        if (size >= 0 && items[i].size != size) match = 0;
        if (shape >= 0 && items[i].shape != shape) match = 0;
        if (match) n++;
    }
    return n;
}

/* Find most expensive item */
int find_most_expensive(struct Item items[], int count) {
    int max_price = -1;
    int max_id = -1;
    int i;
    for (i = 0; i < count; i++) {
        int p = get_price(items[i].size, items[i].color);
        if (p > max_price) {
            max_price = p;
            max_id = items[i].id;
        }
    }
    return max_id;
}

/* Group count: how many items of each color */
void count_by_color(struct Item items[], int count, int result[]) {
    int c;
    for (c = 0; c < COLOR_COUNT; c++) result[c] = 0;
    int i;
    for (i = 0; i < count; i++) {
        result[items[i].color]++;
    }
}

int main(void) {
    /* Test name functions */
    // EXPECT: Color: RED
    printf("Color: %s\n", color_name(COLOR_RED));
    // EXPECT: Size: MEDIUM
    printf("Size: %s\n", size_name(SIZE_MEDIUM));
    // EXPECT: Shape: TRIANGLE
    printf("Shape: %s\n", shape_name(SHAPE_TRIANGLE));

    /* Test nested switch pricing */
    // EXPECT: Small Red: 10
    printf("Small Red: %d\n", get_price(SIZE_SMALL, COLOR_RED));
    // EXPECT: Medium Blue: 25
    printf("Medium Blue: %d\n", get_price(SIZE_MEDIUM, COLOR_BLUE));
    // EXPECT: Large Green: 32
    printf("Large Green: %d\n", get_price(SIZE_LARGE, COLOR_GREEN));
    // EXPECT: Large Yellow: 31
    printf("Large Yellow: %d\n", get_price(SIZE_LARGE, COLOR_YELLOW));

    /* Test shape complexity */
    // EXPECT: Circle complexity: 3
    printf("Circle complexity: %d\n", shape_complexity(SHAPE_CIRCLE));
    // EXPECT: Square complexity: 1
    printf("Square complexity: %d\n", shape_complexity(SHAPE_SQUARE));

    /* Test color warmth */
    // EXPECT: Red warm: 1
    printf("Red warm: %d\n", color_warmth(COLOR_RED));
    // EXPECT: Blue warm: 0
    printf("Blue warm: %d\n", color_warmth(COLOR_BLUE));

    /* Create item inventory */
    struct Item items[8];
    items[0].color = COLOR_RED;    items[0].size = SIZE_SMALL;  items[0].shape = SHAPE_CIRCLE;   items[0].id = 0;
    items[1].color = COLOR_BLUE;   items[1].size = SIZE_LARGE;  items[1].shape = SHAPE_SQUARE;   items[1].id = 1;
    items[2].color = COLOR_GREEN;  items[2].size = SIZE_MEDIUM; items[2].shape = SHAPE_TRIANGLE; items[2].id = 2;
    items[3].color = COLOR_RED;    items[3].size = SIZE_LARGE;  items[3].shape = SHAPE_CIRCLE;   items[3].id = 3;
    items[4].color = COLOR_YELLOW; items[4].size = SIZE_SMALL;  items[4].shape = SHAPE_SQUARE;   items[4].id = 4;
    items[5].color = COLOR_BLUE;   items[5].size = SIZE_MEDIUM; items[5].shape = SHAPE_CIRCLE;   items[5].id = 5;
    items[6].color = COLOR_GREEN;  items[6].size = SIZE_SMALL;  items[6].shape = SHAPE_TRIANGLE; items[6].id = 6;
    items[7].color = COLOR_RED;    items[7].size = SIZE_MEDIUM; items[7].shape = SHAPE_SQUARE;   items[7].id = 7;

    /* Test item score */
    /* items[0]: price=10, complexity=3, multiplier=1 -> 10+3=13 */
    // EXPECT: Item 0 score: 13
    printf("Item 0 score: %d\n", item_score(&items[0]));
    /* items[1]: price=35, complexity=1, multiplier=3 -> 35+3=38 */
    // EXPECT: Item 1 score: 38
    printf("Item 1 score: %d\n", item_score(&items[1]));

    /* Count matching: all red items (color=0, size=-1, shape=-1) */
    // EXPECT: Red items: 3
    printf("Red items: %d\n", count_matching(items, 8, COLOR_RED, -1, -1));
    /* All small items */
    // EXPECT: Small items: 3
    printf("Small items: %d\n", count_matching(items, 8, -1, SIZE_SMALL, -1));
    /* All circles */
    // EXPECT: Circles: 3
    printf("Circles: %d\n", count_matching(items, 8, -1, -1, SHAPE_CIRCLE));
    /* Red and small */
    // EXPECT: Red+Small: 1
    printf("Red+Small: %d\n", count_matching(items, 8, COLOR_RED, SIZE_SMALL, -1));

    /* Find most expensive */
    /* Large Blue = 35 is most expensive */
    // EXPECT: Most expensive id: 1
    printf("Most expensive id: %d\n", find_most_expensive(items, 8));

    /* Count by color */
    int color_counts[4];
    count_by_color(items, 8, color_counts);
    // EXPECT: Red count: 3
    printf("Red count: %d\n", color_counts[COLOR_RED]);
    // EXPECT: Green count: 2
    printf("Green count: %d\n", color_counts[COLOR_GREEN]);
    // EXPECT: Blue count: 2
    printf("Blue count: %d\n", color_counts[COLOR_BLUE]);
    // EXPECT: Yellow count: 1
    printf("Yellow count: %d\n", color_counts[COLOR_YELLOW]);

    /* Sum all warm-colored item prices */
    int warm_total = 0;
    int i;
    for (i = 0; i < 8; i++) {
        if (color_warmth(items[i].color)) {
            warm_total += get_price(items[i].size, items[i].color);
        }
    }
    /* Red items: 10+30+20=60, Yellow: 11 -> 71 */
    // EXPECT: Warm total: 71
    printf("Warm total: %d\n", warm_total);

    return 0;
}
