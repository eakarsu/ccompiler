int printf(const char *fmt, ...);

/* Basic enum: auto-assigned 0,1,2,3 */
enum Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

/* Enum with explicit values */
enum Color {
    RED   = 1,
    GREEN = 2,
    BLUE  = 4
};

/* Enum with gaps and explicit values */
enum Status {
    OK        = 0,
    WARN      = 10,
    ERROR_VAL = 20,
    CRITICAL  = 100
};

/* Enum with negative values */
enum Temp {
    COLD    = -10,
    COOL    = -5,
    NEUTRAL = 0,
    WARM    = 5,
    HOT     = 10
};

/* Enum mixing auto and explicit */
enum Mixed {
    A = 5,
    B,        /* 6 */
    C,        /* 7 */
    D = 20,
    E         /* 21 */
};

/* Helper: direction to string via switch (returns int code) */
int dir_code(enum Direction d) {
    switch (d) {
        case NORTH: return 1;
        case EAST:  return 2;
        case SOUTH: return 3;
        case WEST:  return 4;
    }
    return -1;
}

/* Check if color flag is set */
int has_color(int flags, enum Color c) {
    return (flags & c) != 0;
}

int main(void) {
    /* --- Auto-assigned values --- */
    printf("%d\n", (int)NORTH);              // EXPECT: 0
    printf("%d\n", (int)EAST);               // EXPECT: 1
    printf("%d\n", (int)SOUTH);              // EXPECT: 2
    printf("%d\n", (int)WEST);               // EXPECT: 3

    /* --- Explicit values --- */
    printf("%d\n", (int)RED);                // EXPECT: 1
    printf("%d\n", (int)GREEN);              // EXPECT: 2
    printf("%d\n", (int)BLUE);               // EXPECT: 4

    /* --- Status with gaps --- */
    printf("%d\n", (int)OK);                 // EXPECT: 0
    printf("%d\n", (int)WARN);               // EXPECT: 10
    printf("%d\n", (int)ERROR_VAL);          // EXPECT: 20
    printf("%d\n", (int)CRITICAL);           // EXPECT: 100

    /* --- Negative enum values --- */
    printf("%d\n", (int)COLD);               // EXPECT: -10
    printf("%d\n", (int)COOL);               // EXPECT: -5
    printf("%d\n", (int)NEUTRAL);            // EXPECT: 0
    printf("%d\n", (int)WARM);               // EXPECT: 5
    printf("%d\n", (int)HOT);                // EXPECT: 10

    /* --- Mixed auto/explicit --- */
    printf("%d\n", (int)A);                  // EXPECT: 5
    printf("%d\n", (int)B);                  // EXPECT: 6
    printf("%d\n", (int)C);                  // EXPECT: 7
    printf("%d\n", (int)D);                  // EXPECT: 20
    printf("%d\n", (int)E);                  // EXPECT: 21

    /* --- Enum in switch --- */
    enum Direction dirs[4] = {NORTH, EAST, SOUTH, WEST};
    int i;
    for (i = 0; i < 4; i++) {
        printf("%d\n", dir_code(dirs[i]));
    }
    // EXPECT: 1
    // EXPECT: 2
    // EXPECT: 3
    // EXPECT: 4

    /* --- Enum comparison --- */
    enum Status s = WARN;
    printf("%d\n", s == WARN);              // EXPECT: 1
    printf("%d\n", s == OK);               // EXPECT: 0
    printf("%d\n", s < ERROR_VAL);         // EXPECT: 1
    printf("%d\n", s > OK);                // EXPECT: 1

    /* --- Enum arithmetic --- */
    int sum_dirs = (int)NORTH + (int)EAST + (int)SOUTH + (int)WEST;
    /* 0+1+2+3 = 6 */
    printf("%d\n", sum_dirs);              // EXPECT: 6

    int color_flags = (int)RED | (int)GREEN | (int)BLUE;
    /* 1|2|4 = 7 */
    printf("%d\n", color_flags);           // EXPECT: 7

    printf("%d\n", has_color(color_flags, RED));   // EXPECT: 1
    printf("%d\n", has_color(color_flags, GREEN)); // EXPECT: 1
    printf("%d\n", has_color(color_flags, BLUE));  // EXPECT: 1
    printf("%d\n", has_color(5, GREEN));           // EXPECT: 0

    /* --- Negative enum arithmetic --- */
    int temp_range = (int)HOT - (int)COLD;  /* 10 - (-10) = 20 */
    printf("%d\n", temp_range);            // EXPECT: 20

    int mid = ((int)HOT + (int)COLD) / 2;  /* (10 + -10)/2 = 0 */
    printf("%d\n", mid);                   // EXPECT: 0

    /* --- Enum used as array index --- */
    int scores[4];
    scores[NORTH] = 100;
    scores[EAST]  = 200;
    scores[SOUTH] = 300;
    scores[WEST]  = 400;
    printf("%d\n", scores[NORTH]);         // EXPECT: 100
    printf("%d\n", scores[EAST]);          // EXPECT: 200
    printf("%d\n", scores[SOUTH]);         // EXPECT: 300
    printf("%d\n", scores[WEST]);          // EXPECT: 400

    /* --- Enum iteration (cast to int) --- */
    int total = 0;
    int d2;
    for (d2 = NORTH; d2 <= WEST; d2++) {
        total += d2;
    }
    /* 0+1+2+3 = 6 */
    printf("%d\n", total);                 // EXPECT: 6

    /* --- Enum in struct field --- */
    struct { enum Color col; int brightness; } light;
    light.col        = BLUE;
    light.brightness = 80;
    printf("%d\n", (int)light.col);        // EXPECT: 4
    printf("%d\n", light.brightness);      // EXPECT: 80

    return 0;
}
