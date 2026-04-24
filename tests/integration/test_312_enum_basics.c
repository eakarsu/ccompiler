int printf(const char *fmt, ...);
// EXPECT: RED = 0\nGREEN = 1\nBLUE = 2\nYELLOW = 3\nWHITE = 4\nred\nblue\nyellow\ncolor_value(GREEN) = 1\nSUMMER = 2\nis_warm(SUMMER) = 1\nis_warm(WINTER) = 0\nspring\nautumn\nSMALL = 10\nLARGE = 30\nsize_in_cm(MEDIUM) = 40\nsize_in_cm(XLARGE) = 80
// Test: enum basics

enum Color {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    WHITE
};

enum Season {
    SPRING = 1,
    SUMMER = 2,
    AUTUMN = 3,
    WINTER = 4
};

enum Size {
    SMALL = 10,
    MEDIUM = 20,
    LARGE = 30,
    XLARGE = 40
};

int color_value(enum Color c) {
    return c;
}

int is_warm_season(enum Season s) {
    return s == SPRING || s == SUMMER;
}

int size_in_cm(enum Size s) {
    return s * 2;
}

void print_color_name(enum Color c) {
    if (c == RED) printf("red");
    else if (c == GREEN) printf("green");
    else if (c == BLUE) printf("blue");
    else if (c == YELLOW) printf("yellow");
    else if (c == WHITE) printf("white");
    else printf("unknown");
    printf("\n");
}

void print_season_name(enum Season s) {
    if (s == SPRING) printf("spring");
    else if (s == SUMMER) printf("summer");
    else if (s == AUTUMN) printf("autumn");
    else if (s == WINTER) printf("winter");
    else printf("unknown");
    printf("\n");
}

int main(void) {
    enum Color c = RED;
    printf("RED = %d\n", c);
    printf("GREEN = %d\n", GREEN);
    printf("BLUE = %d\n", BLUE);
    printf("YELLOW = %d\n", YELLOW);
    printf("WHITE = %d\n", WHITE);

    print_color_name(RED);
    print_color_name(BLUE);
    print_color_name(YELLOW);

    printf("color_value(GREEN) = %d\n", color_value(GREEN));

    enum Season s = SUMMER;
    printf("SUMMER = %d\n", s);
    printf("is_warm(SUMMER) = %d\n", is_warm_season(SUMMER));
    printf("is_warm(WINTER) = %d\n", is_warm_season(WINTER));

    print_season_name(SPRING);
    print_season_name(AUTUMN);

    printf("SMALL = %d\n", SMALL);
    printf("LARGE = %d\n", LARGE);
    printf("size_in_cm(MEDIUM) = %d\n", size_in_cm(MEDIUM));
    printf("size_in_cm(XLARGE) = %d\n", size_in_cm(XLARGE));

    return 0;
}
