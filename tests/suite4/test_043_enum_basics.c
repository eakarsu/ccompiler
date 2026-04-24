int printf(const char *fmt, ...);

enum Color {
    RED,
    GREEN,
    BLUE
};

enum Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST
};

enum Season {
    SPRING,
    SUMMER,
    AUTUMN,
    WINTER
};

int color_to_int(enum Color c) {
    return c;
}

int direction_count(enum Direction d) {
    int count = 0;
    switch (d) {
        case NORTH: count = 1; break;
        case SOUTH: count = 2; break;
        case EAST:  count = 3; break;
        case WEST:  count = 4; break;
    }
    return count;
}

int season_name(enum Season s) {
    switch (s) {
        case SPRING: return 10;
        case SUMMER: return 20;
        case AUTUMN: return 30;
        case WINTER: return 40;
    }
    return -1;
}

int compare_enums(enum Color a, enum Color b) {
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

int enum_in_loop(void) {
    int sum = 0;
    enum Color c;
    for (c = RED; c <= BLUE; c = c + 1) {
        sum = sum + c;
    }
    // RED=0 + GREEN=1 + BLUE=2 = 3
    return sum;
}

int enum_as_array_index(void) {
    int values[4];
    values[NORTH] = 10;
    values[SOUTH] = 20;
    values[EAST] = 30;
    values[WEST] = 40;
    return values[NORTH] + values[EAST]; // 10 + 30 = 40
}

int enum_in_if_chain(enum Direction d) {
    if (d == NORTH) return 100;
    if (d == SOUTH) return 200;
    if (d == EAST) return 300;
    return 400;
}

int enum_ternary(enum Color c) {
    return (c == RED) ? 1 : (c == GREEN) ? 2 : 3;
}

int enum_arithmetic(void) {
    enum Color c = BLUE; // 2
    int x = c * 10;      // 20
    int y = c + 3;       // 5
    return x + y;        // 25
}

int main(void) {
    // EXPECT: RED: 0
    printf("RED: %d\n", RED);

    // EXPECT: GREEN: 1
    printf("GREEN: %d\n", GREEN);

    // EXPECT: BLUE: 2
    printf("BLUE: %d\n", BLUE);

    // EXPECT: NORTH: 0
    printf("NORTH: %d\n", NORTH);

    // EXPECT: WEST: 3
    printf("WEST: %d\n", WEST);

    // EXPECT: color_to_int RED: 0
    printf("color_to_int RED: %d\n", color_to_int(RED));

    // EXPECT: color_to_int BLUE: 2
    printf("color_to_int BLUE: %d\n", color_to_int(BLUE));

    // EXPECT: direction NORTH: 1
    printf("direction NORTH: %d\n", direction_count(NORTH));

    // EXPECT: direction WEST: 4
    printf("direction WEST: %d\n", direction_count(WEST));

    // EXPECT: season SPRING: 10
    printf("season SPRING: %d\n", season_name(SPRING));

    // EXPECT: season WINTER: 40
    printf("season WINTER: %d\n", season_name(WINTER));

    // EXPECT: compare RED GREEN: -1
    printf("compare RED GREEN: %d\n", compare_enums(RED, GREEN));

    // EXPECT: compare BLUE RED: 1
    printf("compare BLUE RED: %d\n", compare_enums(BLUE, RED));

    // EXPECT: compare GREEN GREEN: 0
    printf("compare GREEN GREEN: %d\n", compare_enums(GREEN, GREEN));

    // EXPECT: enum loop sum: 3
    printf("enum loop sum: %d\n", enum_in_loop());

    // EXPECT: enum array index: 40
    printf("enum array index: %d\n", enum_as_array_index());

    // EXPECT: enum if NORTH: 100
    printf("enum if NORTH: %d\n", enum_in_if_chain(NORTH));

    // EXPECT: enum if WEST: 400
    printf("enum if WEST: %d\n", enum_in_if_chain(WEST));

    // EXPECT: enum ternary RED: 1
    printf("enum ternary RED: %d\n", enum_ternary(RED));

    // EXPECT: enum ternary BLUE: 3
    printf("enum ternary BLUE: %d\n", enum_ternary(BLUE));

    // EXPECT: enum arithmetic: 25
    printf("enum arithmetic: %d\n", enum_arithmetic());

    // EXPECT: sizeof enum Color: 4
    printf("sizeof enum Color: %d\n", (int)sizeof(enum Color));

    return 0;
}
