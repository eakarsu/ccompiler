// EXPECT: === Basic Enum ===
// EXPECT: RED=0 GREEN=1 BLUE=2
// EXPECT: === Explicit Values ===
// EXPECT: HTTP_OK=200 NOT_FOUND=404 SERVER_ERR=500
// EXPECT: === Negative Values ===
// EXPECT: NEG2=-2 NEG1=-1 ZERO=0 POS1=1
// EXPECT: === Enum in Switch ===
// EXPECT: Monday
// EXPECT: Wednesday
// EXPECT: Friday
// EXPECT: Weekend!
// EXPECT: === Enum Arithmetic ===
// EXPECT: next color after RED: 1
// EXPECT: prev color before BLUE: 1
// EXPECT: === Enum as Array Index ===
// EXPECT: cat[0] = Fruit
// EXPECT: cat[1] = Vegetable
// EXPECT: cat[2] = Meat
// EXPECT: cat[3] = Dairy
// EXPECT: === Enum Comparison ===
// EXPECT: WARN is less severe than ERROR
// EXPECT: ERROR is at least ERROR level
// EXPECT: === Enum Loop ===
// EXPECT: season 0
// EXPECT: season 1
// EXPECT: season 2
// EXPECT: season 3
// EXPECT: === Enum Flags ===
// EXPECT: combined flags: 7
// EXPECT: has READ: yes
// EXPECT: has WRITE: yes
// EXPECT: has EXEC: yes

int printf(const char *fmt, ...);

enum Color {
    RED,
    GREEN,
    BLUE
};

enum HttpStatus {
    HTTP_OK = 200,
    NOT_FOUND = 404,
    SERVER_ERR = 500
};

enum Signed {
    NEG2 = -2,
    NEG1 = -1,
    ZERO = 0,
    POS1 = 1
};

enum Day {
    MON, TUE, WED, THU, FRI, SAT, SUN
};

enum Category {
    CAT_FRUIT,
    CAT_VEGETABLE,
    CAT_MEAT,
    CAT_DAIRY,
    CAT_COUNT
};

enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
};

enum Season {
    SPRING,
    SUMMER,
    AUTUMN,
    WINTER,
    SEASON_COUNT
};

enum Permission {
    PERM_READ  = 1,
    PERM_WRITE = 2,
    PERM_EXEC  = 4
};

void test_basic(void) {
    printf("=== Basic Enum ===\n");
    printf("RED=%d GREEN=%d BLUE=%d\n", RED, GREEN, BLUE);
}

void test_explicit(void) {
    printf("=== Explicit Values ===\n");
    printf("HTTP_OK=%d NOT_FOUND=%d SERVER_ERR=%d\n", HTTP_OK, NOT_FOUND, SERVER_ERR);
}

void test_negative(void) {
    printf("=== Negative Values ===\n");
    printf("NEG2=%d NEG1=%d ZERO=%d POS1=%d\n", NEG2, NEG1, ZERO, POS1);
}

void print_day(enum Day d) {
    switch (d) {
        case MON: printf("Monday\n"); break;
        case TUE: printf("Tuesday\n"); break;
        case WED: printf("Wednesday\n"); break;
        case THU: printf("Thursday\n"); break;
        case FRI: printf("Friday\n"); break;
        case SAT: printf("Weekend!\n"); break;
        case SUN: printf("Weekend!\n"); break;
    }
}

void test_switch(void) {
    printf("=== Enum in Switch ===\n");
    print_day(MON);
    print_day(WED);
    print_day(FRI);
    print_day(SAT);
}

void test_arithmetic(void) {
    printf("=== Enum Arithmetic ===\n");
    enum Color c = RED;
    int next = c + 1;
    printf("next color after RED: %d\n", next);
    enum Color c2 = BLUE;
    int prev = c2 - 1;
    printf("prev color before BLUE: %d\n", prev);
}

void test_array_index(void) {
    printf("=== Enum as Array Index ===\n");
    char *names[4];
    names[CAT_FRUIT] = "Fruit";
    names[CAT_VEGETABLE] = "Vegetable";
    names[CAT_MEAT] = "Meat";
    names[CAT_DAIRY] = "Dairy";
    int i;
    for (i = 0; i < CAT_COUNT; i++) {
        printf("cat[%d] = %s\n", i, names[i]);
    }
}

void test_comparison(void) {
    printf("=== Enum Comparison ===\n");
    enum LogLevel l1 = LOG_WARN;
    enum LogLevel l2 = LOG_ERROR;
    if (l1 < l2) {
        printf("WARN is less severe than ERROR\n");
    }
    if (l2 >= LOG_ERROR) {
        printf("ERROR is at least ERROR level\n");
    }
}

void test_loop(void) {
    printf("=== Enum Loop ===\n");
    int s;
    for (s = SPRING; s < SEASON_COUNT; s++) {
        printf("season %d\n", s);
    }
}

void test_flags(void) {
    printf("=== Enum Flags ===\n");
    int perms = PERM_READ | PERM_WRITE | PERM_EXEC;
    printf("combined flags: %d\n", perms);
    if (perms & PERM_READ)  printf("has READ: yes\n");
    if (perms & PERM_WRITE) printf("has WRITE: yes\n");
    if (perms & PERM_EXEC)  printf("has EXEC: yes\n");
}

int main(void) {
    test_basic();
    test_explicit();
    test_negative();
    test_switch();
    test_arithmetic();
    test_array_index();
    test_comparison();
    test_loop();
    test_flags();
    return 0;
}
