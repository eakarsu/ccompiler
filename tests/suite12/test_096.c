int printf(const char *fmt, ...);

// Enum with 50 values used in giant switch

enum Color {
    RED, ORANGE, YELLOW, GREEN, BLUE,
    INDIGO, VIOLET, WHITE, BLACK, GRAY,
    CRIMSON, SCARLET, MAROON, BURGUNDY, CHERRY,
    CORAL, SALMON, PEACH, APRICOT, GOLD,
    AMBER, BRONZE, COPPER, TAN, KHAKI,
    OLIVE, LIME, EMERALD, TEAL, CYAN,
    AZURE, COBALT, NAVY, MIDNIGHT, ROYAL,
    LAVENDER, PLUM, MAGENTA, FUCHSIA, PINK,
    ROSE, MAUVE, LILAC, ORCHID, PERIWINKLE,
    IVORY, CREAM, BEIGE, LINEN, SNOW
};

int color_to_group(enum Color c) {
    switch (c) {
        case RED: case CRIMSON: case SCARLET: case MAROON:
        case BURGUNDY: case CHERRY:
            return 1;
        case ORANGE: case CORAL: case SALMON: case PEACH:
        case APRICOT:
            return 2;
        case YELLOW: case GOLD: case AMBER: case BRONZE:
        case COPPER:
            return 3;
        case GREEN: case OLIVE: case LIME: case EMERALD:
        case TEAL:
            return 4;
        case BLUE: case AZURE: case COBALT: case NAVY:
        case MIDNIGHT: case ROYAL:
            return 5;
        case INDIGO: case VIOLET: case LAVENDER: case PLUM:
        case MAGENTA: case FUCHSIA:
            return 6;
        case PINK: case ROSE: case MAUVE: case LILAC:
        case ORCHID: case PERIWINKLE:
            return 7;
        case WHITE: case IVORY: case CREAM: case BEIGE:
        case LINEN: case SNOW:
            return 8;
        case BLACK: case GRAY: case TAN: case KHAKI:
        case CYAN:
            return 9;
        default:
            return 0;
    }
}

int color_warmth(enum Color c) {
    switch (c) {
        case RED: return 10;
        case ORANGE: return 9;
        case YELLOW: return 8;
        case GREEN: return 5;
        case BLUE: return 2;
        case INDIGO: return 3;
        case VIOLET: return 4;
        case WHITE: return 5;
        case BLACK: return 1;
        case GRAY: return 3;
        case CRIMSON: return 10;
        case SCARLET: return 9;
        case MAROON: return 8;
        case BURGUNDY: return 7;
        case CHERRY: return 9;
        case CORAL: return 8;
        case SALMON: return 7;
        case PEACH: return 6;
        case APRICOT: return 6;
        case GOLD: return 8;
        case AMBER: return 7;
        case BRONZE: return 6;
        case COPPER: return 6;
        case TAN: return 5;
        case KHAKI: return 4;
        case OLIVE: return 4;
        case LIME: return 5;
        case EMERALD: return 4;
        case TEAL: return 3;
        case CYAN: return 3;
        case AZURE: return 3;
        case COBALT: return 2;
        case NAVY: return 1;
        case MIDNIGHT: return 1;
        case ROYAL: return 2;
        case LAVENDER: return 5;
        case PLUM: return 4;
        case MAGENTA: return 6;
        case FUCHSIA: return 7;
        case PINK: return 7;
        case ROSE: return 6;
        case MAUVE: return 5;
        case LILAC: return 5;
        case ORCHID: return 5;
        case PERIWINKLE: return 4;
        case IVORY: return 6;
        case CREAM: return 6;
        case BEIGE: return 5;
        case LINEN: return 5;
        case SNOW: return 4;
        default: return 0;
    }
}

int count_group(int group) {
    int count;
    int i;
    count = 0;
    for (i = 0; i < 50; i = i + 1) {
        if (color_to_group(i) == group) {
            count = count + 1;
        }
    }
    return count;
}

int sum_warmth_in_group(int group) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < 50; i = i + 1) {
        if (color_to_group(i) == group) {
            sum = sum + color_warmth(i);
        }
    }
    return sum;
}

int total_warmth() {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < 50; i = i + 1) {
        sum = sum + color_warmth(i);
    }
    return sum;
}

int main() {
    // Group tests
    printf("%d\n", color_to_group(RED));       // EXPECT: 1
    printf("%d\n", color_to_group(ORANGE));    // EXPECT: 2
    printf("%d\n", color_to_group(YELLOW));    // EXPECT: 3
    printf("%d\n", color_to_group(GREEN));     // EXPECT: 4
    printf("%d\n", color_to_group(BLUE));      // EXPECT: 5
    printf("%d\n", color_to_group(INDIGO));    // EXPECT: 6
    printf("%d\n", color_to_group(PINK));      // EXPECT: 7
    printf("%d\n", color_to_group(WHITE));     // EXPECT: 8
    printf("%d\n", color_to_group(BLACK));     // EXPECT: 9

    // Warmth tests
    printf("%d\n", color_warmth(RED));     // EXPECT: 10
    printf("%d\n", color_warmth(BLUE));    // EXPECT: 2
    printf("%d\n", color_warmth(NAVY));    // EXPECT: 1
    printf("%d\n", color_warmth(CRIMSON)); // EXPECT: 10
    printf("%d\n", color_warmth(SNOW));    // EXPECT: 4

    // Count per group
    printf("%d\n", count_group(1)); // EXPECT: 6
    printf("%d\n", count_group(2)); // EXPECT: 5
    printf("%d\n", count_group(3)); // EXPECT: 5
    printf("%d\n", count_group(4)); // EXPECT: 5
    printf("%d\n", count_group(5)); // EXPECT: 6
    printf("%d\n", count_group(6)); // EXPECT: 6
    printf("%d\n", count_group(7)); // EXPECT: 6
    printf("%d\n", count_group(8)); // EXPECT: 6
    printf("%d\n", count_group(9)); // EXPECT: 5

    // Sum of warmth in red group (1): 10+10+9+8+7+9 = 53
    printf("%d\n", sum_warmth_in_group(1)); // EXPECT: 53

    // Sum of warmth in blue group (5): 2+3+2+1+1+2 = 11
    printf("%d\n", sum_warmth_in_group(5)); // EXPECT: 11

    // Total warmth across all 50 colors
    printf("%d\n", total_warmth()); // EXPECT: 264

    // Enum value checks
    printf("%d\n", RED);   // EXPECT: 0
    printf("%d\n", SNOW);  // EXPECT: 49
    printf("%d\n", CYAN);  // EXPECT: 29

    printf("%s\n", "done"); // EXPECT: done
    return 0;
}
