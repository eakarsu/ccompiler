int printf(const char *fmt, ...);

// Large enum with 30+ values

enum Color {
    RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET,
    CYAN, MAGENTA, PINK, BROWN, BLACK, WHITE, GRAY,
    LIME, TEAL, NAVY, MAROON, OLIVE, AQUA, CORAL,
    GOLD, SILVER, BRONZE, CRIMSON, IVORY, LAVENDER,
    PLUM, SALMON, TURQUOISE, BEIGE, KHAKI
};

enum ErrorCode {
    ERR_NONE,
    ERR_FILE_NOT_FOUND,
    ERR_PERMISSION_DENIED,
    ERR_INVALID_INPUT,
    ERR_OUT_OF_MEMORY,
    ERR_TIMEOUT,
    ERR_NETWORK,
    ERR_DISK_FULL,
    ERR_CORRUPT_DATA,
    ERR_OVERFLOW,
    ERR_UNDERFLOW,
    ERR_DIVIDE_BY_ZERO,
    ERR_NULL_POINTER,
    ERR_STACK_OVERFLOW,
    ERR_BUFFER_OVERFLOW,
    ERR_TYPE_MISMATCH,
    ERR_SYNTAX,
    ERR_SEMANTIC,
    ERR_LINKER,
    ERR_RUNTIME,
    ERR_ASSERTION,
    ERR_DEADLOCK,
    ERR_RACE_CONDITION,
    ERR_MEMORY_LEAK,
    ERR_SEGFAULT,
    ERR_BUS_ERROR,
    ERR_SIGNAL,
    ERR_EOF,
    ERR_PARSE,
    ERR_ENCODE,
    ERR_DECODE,
    ERR_AUTH,
    ERR_FORBIDDEN,
    ERR_NOT_IMPLEMENTED,
    ERR_DEPRECATED,
    ERR_UNKNOWN
};

int color_is_warm(enum Color c) {
    switch (c) {
        case RED: case ORANGE: case YELLOW: case PINK:
        case CORAL: case GOLD: case CRIMSON: case SALMON:
        case MAROON:
            return 1;
        default:
            return 0;
    }
}

int color_is_cool(enum Color c) {
    switch (c) {
        case BLUE: case GREEN: case INDIGO: case VIOLET:
        case CYAN: case TEAL: case NAVY: case AQUA:
        case TURQUOISE: case LAVENDER:
            return 1;
        default:
            return 0;
    }
}

int color_is_neutral(enum Color c) {
    switch (c) {
        case BLACK: case WHITE: case GRAY: case BROWN:
        case BEIGE: case IVORY: case KHAKI: case SILVER:
        case BRONZE: case OLIVE:
            return 1;
        default:
            return 0;
    }
}

int error_severity(enum ErrorCode e) {
    switch (e) {
        case ERR_NONE:
            return 0;
        case ERR_FILE_NOT_FOUND:
        case ERR_PERMISSION_DENIED:
        case ERR_INVALID_INPUT:
        case ERR_TIMEOUT:
        case ERR_EOF:
            return 1;
        case ERR_NETWORK:
        case ERR_DISK_FULL:
        case ERR_OVERFLOW:
        case ERR_UNDERFLOW:
        case ERR_DIVIDE_BY_ZERO:
        case ERR_TYPE_MISMATCH:
        case ERR_SYNTAX:
        case ERR_PARSE:
        case ERR_ENCODE:
        case ERR_DECODE:
        case ERR_AUTH:
        case ERR_FORBIDDEN:
        case ERR_DEPRECATED:
            return 2;
        case ERR_OUT_OF_MEMORY:
        case ERR_CORRUPT_DATA:
        case ERR_NULL_POINTER:
        case ERR_STACK_OVERFLOW:
        case ERR_BUFFER_OVERFLOW:
        case ERR_SEMANTIC:
        case ERR_LINKER:
        case ERR_RUNTIME:
        case ERR_ASSERTION:
        case ERR_NOT_IMPLEMENTED:
        case ERR_UNKNOWN:
            return 3;
        case ERR_SEGFAULT:
        case ERR_BUS_ERROR:
        case ERR_SIGNAL:
        case ERR_DEADLOCK:
        case ERR_RACE_CONDITION:
        case ERR_MEMORY_LEAK:
            return 4;
        default:
            return -1;
    }
}

int count_warm_colors() {
    int cnt = 0;
    int i = RED;
    while (i <= KHAKI) {
        if (color_is_warm(i)) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

int count_cool_colors() {
    int cnt = 0;
    int i = RED;
    while (i <= KHAKI) {
        if (color_is_cool(i)) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

int count_neutral_colors() {
    int cnt = 0;
    int i = RED;
    while (i <= KHAKI) {
        if (color_is_neutral(i)) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

int count_errors_by_severity(int sev) {
    int cnt = 0;
    int i = ERR_NONE;
    while (i <= ERR_UNKNOWN) {
        if (error_severity(i) == sev) cnt = cnt + 1;
        i = i + 1;
    }
    return cnt;
}

int main() {
    printf("RED warm: %d\n", color_is_warm(RED));
    // EXPECT: RED warm: 1
    printf("BLUE warm: %d\n", color_is_warm(BLUE));
    // EXPECT: BLUE warm: 0
    printf("BLUE cool: %d\n", color_is_cool(BLUE));
    // EXPECT: BLUE cool: 1
    printf("RED cool: %d\n", color_is_cool(RED));
    // EXPECT: RED cool: 0
    printf("BLACK neutral: %d\n", color_is_neutral(BLACK));
    // EXPECT: BLACK neutral: 1
    printf("RED neutral: %d\n", color_is_neutral(RED));
    // EXPECT: RED neutral: 0

    printf("warm count: %d\n", count_warm_colors());
    // EXPECT: warm count: 9
    printf("cool count: %d\n", count_cool_colors());
    // EXPECT: cool count: 10
    printf("neutral count: %d\n", count_neutral_colors());
    // EXPECT: neutral count: 10

    printf("sev NONE: %d\n", error_severity(ERR_NONE));
    // EXPECT: sev NONE: 0
    printf("sev FILE: %d\n", error_severity(ERR_FILE_NOT_FOUND));
    // EXPECT: sev FILE: 1
    printf("sev NETWORK: %d\n", error_severity(ERR_NETWORK));
    // EXPECT: sev NETWORK: 2
    printf("sev OOM: %d\n", error_severity(ERR_OUT_OF_MEMORY));
    // EXPECT: sev OOM: 3
    printf("sev SEGFAULT: %d\n", error_severity(ERR_SEGFAULT));
    // EXPECT: sev SEGFAULT: 4

    printf("sev0 count: %d\n", count_errors_by_severity(0));
    // EXPECT: sev0 count: 1
    printf("sev1 count: %d\n", count_errors_by_severity(1));
    // EXPECT: sev1 count: 5
    printf("sev2 count: %d\n", count_errors_by_severity(2));
    // EXPECT: sev2 count: 13
    printf("sev3 count: %d\n", count_errors_by_severity(3));
    // EXPECT: sev3 count: 11
    printf("sev4 count: %d\n", count_errors_by_severity(4));
    // EXPECT: sev4 count: 6

    int total = 1 + 5 + 13 + 11 + 6;
    printf("total errors: %d\n", total);
    // EXPECT: total errors: 36

    return 0;
}
