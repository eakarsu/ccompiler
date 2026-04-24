int printf(const char *fmt, ...);

// Test 100: Enum arithmetic and type coercion

enum Color { RED, GREEN, BLUE, ALPHA };

enum Direction { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3 };

enum Signed { NEG_TWO = -2, NEG_ONE = -1, ZERO = 0, POS_ONE = 1, POS_TWO = 2 };

enum Power { P1 = 1, P2 = 2, P4 = 4, P8 = 8, P16 = 16, P32 = 32 };

void test_enum_values(void) {
    printf("%d\n", RED);
    // EXPECT: 0
    printf("%d\n", GREEN);
    // EXPECT: 1
    printf("%d\n", BLUE);
    // EXPECT: 2
    printf("%d\n", ALPHA);
    // EXPECT: 3
}

void test_enum_arithmetic(void) {
    enum Color c = BLUE;
    int val = c + 10;
    printf("%d\n", val);
    // EXPECT: 12

    val = c * 3;
    printf("%d\n", val);
    // EXPECT: 6

    val = ALPHA - RED;
    printf("%d\n", val);
    // EXPECT: 3

    // Enum + enum
    val = GREEN + BLUE;
    printf("%d\n", val);
    // EXPECT: 3
}

void test_enum_negative(void) {
    printf("%d\n", NEG_TWO);
    // EXPECT: -2
    printf("%d\n", NEG_ONE);
    // EXPECT: -1
    printf("%d\n", ZERO);
    // EXPECT: 0

    int sum = NEG_TWO + NEG_ONE + ZERO + POS_ONE + POS_TWO;
    // -2 + -1 + 0 + 1 + 2 = 0
    printf("%d\n", sum);
    // EXPECT: 0

    int product = NEG_TWO * POS_TWO;
    printf("%d\n", product);
    // EXPECT: -4
}

void test_enum_comparison(void) {
    enum Direction d = EAST;

    printf("%d\n", d == EAST);
    // EXPECT: 1
    printf("%d\n", d != NORTH);
    // EXPECT: 1
    printf("%d\n", d < SOUTH);
    // EXPECT: 1
    printf("%d\n", d > WEST);
    // EXPECT: 0
}

void test_enum_in_switch(void) {
    enum Direction d = SOUTH;
    int result;

    switch (d) {
    case NORTH:
        result = 0;
        break;
    case EAST:
        result = 90;
        break;
    case SOUTH:
        result = 180;
        break;
    case WEST:
        result = 270;
        break;
    default:
        result = -1;
        break;
    }
    printf("%d\n", result);
    // EXPECT: 180
}

void test_enum_array_index(void) {
    int scores[4];
    scores[RED] = 10;
    scores[GREEN] = 20;
    scores[BLUE] = 30;
    scores[ALPHA] = 40;

    int total = 0;
    int i;
    for (i = RED; i <= ALPHA; i++) {
        total = total + scores[i];
    }
    printf("%d\n", total);
    // EXPECT: 100
}

void test_enum_bitmask(void) {
    int flags = 0;
    flags = flags | P1;
    flags = flags | P4;
    flags = flags | P16;
    printf("%d\n", flags);
    // EXPECT: 21

    printf("%d\n", (flags & P1) != 0);
    // EXPECT: 1
    printf("%d\n", (flags & P2) != 0);
    // EXPECT: 0
    printf("%d\n", (flags & P4) != 0);
    // EXPECT: 1
    printf("%d\n", (flags & P8) != 0);
    // EXPECT: 0
    printf("%d\n", (flags & P16) != 0);
    // EXPECT: 1

    // Remove P4
    flags = flags & ~P4;
    printf("%d\n", flags);
    // EXPECT: 17
    printf("%d\n", (flags & P4) != 0);
    // EXPECT: 0
}

void test_enum_coercion_to_int(void) {
    enum Color c = BLUE;
    int *arr = (int *)0;

    // Enum used in integer expressions seamlessly
    int doubled = c * 2;
    printf("%d\n", doubled);
    // EXPECT: 4

    // Enum in conditional
    int ok = (c > 0) ? c : -1;
    printf("%d\n", ok);
    // EXPECT: 2

    // Enum assigned from int
    int raw = 3;
    enum Color from_int = raw;
    printf("%d\n", from_int);
    // EXPECT: 3
    printf("%d\n", from_int == ALPHA);
    // EXPECT: 1
}

void test_enum_iteration(void) {
    int count = 0;
    enum Direction d;
    for (d = NORTH; d <= WEST; d++) {
        count++;
    }
    printf("%d\n", count);
    // EXPECT: 4
}

void test_enum_struct(void) {
    struct Tagged {
        enum Color color;
        int value;
    };

    struct Tagged items[3];
    items[0].color = RED;
    items[0].value = 100;
    items[1].color = GREEN;
    items[1].value = 200;
    items[2].color = BLUE;
    items[2].value = 300;

    int i;
    int green_val = 0;
    for (i = 0; i < 3; i++) {
        if (items[i].color == GREEN) {
            green_val = items[i].value;
        }
    }
    printf("%d\n", green_val);
    // EXPECT: 200

    // Sum values where color > RED
    int sum = 0;
    for (i = 0; i < 3; i++) {
        if (items[i].color > RED) {
            sum = sum + items[i].value;
        }
    }
    // GREEN(200) + BLUE(300) = 500
    printf("%d\n", sum);
    // EXPECT: 500
}

void test_enum_complex_expr(void) {
    enum Signed a = POS_TWO;
    enum Signed b = NEG_ONE;

    int result = a * a + b * b;
    // 4 + 1 = 5
    printf("%d\n", result);
    // EXPECT: 5

    result = (a + b) * (a - b);
    // (2 + -1) * (2 - -1) = 1 * 3 = 3
    printf("%d\n", result);
    // EXPECT: 3

    // Chain of operations
    result = NEG_TWO;
    result = result * NEG_ONE;  // 2
    result = result + POS_TWO;  // 4
    result = result * POS_ONE;  // 4
    printf("%d\n", result);
    // EXPECT: 4
}

int main(void) {
    test_enum_values();
    test_enum_arithmetic();
    test_enum_negative();
    test_enum_comparison();
    test_enum_in_switch();
    test_enum_array_index();
    test_enum_bitmask();
    test_enum_coercion_to_int();
    test_enum_iteration();
    test_enum_struct();
    test_enum_complex_expr();
    return 0;
}
