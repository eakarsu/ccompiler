// Enum edge cases: large values, negative, arithmetic, switch, array index, overlapping
int printf(const char *fmt, ...);

enum color {
    RED = 0,
    GREEN = 1,
    BLUE = 2
};

enum large_vals {
    SMALL = 1,
    MEDIUM = 1000,
    LARGE = 1000000,
    HUGE = 100000000
};

enum negative {
    NEG_THREE = -3,
    NEG_TWO = -2,
    NEG_ONE = -1,
    ZERO = 0,
    POS_ONE = 1,
    POS_TWO = 2
};

enum overlapping_a {
    OVA_X = 10,
    OVA_Y = 20,
    OVA_Z = 30
};

enum overlapping_b {
    OVB_X = 10,
    OVB_Y = 20,
    OVB_Z = 30
};

enum auto_vals {
    AUTO_A,
    AUTO_B,
    AUTO_C = 100,
    AUTO_D,
    AUTO_E,
    AUTO_F = -5,
    AUTO_G,
    AUTO_H
};

enum { ANON_A = 42, ANON_B = 99, ANON_C = -7 };

void test_basic_enum(void) {
    enum color c = RED;
    printf("%d %d %d\n", c, GREEN, BLUE);
    // EXPECT: 0 1 2
}

void test_large_values(void) {
    printf("%d %d %d %d\n", SMALL, MEDIUM, LARGE, HUGE);
    // EXPECT: 1 1000 1000000 100000000
}

void test_negative_values(void) {
    printf("%d %d %d %d %d %d\n", NEG_THREE, NEG_TWO, NEG_ONE, ZERO, POS_ONE, POS_TWO);
    // EXPECT: -3 -2 -1 0 1 2
}

void test_enum_arithmetic(void) {
    int a = LARGE + MEDIUM;
    int b = HUGE - LARGE;
    int c = NEG_THREE * NEG_TWO;
    int d = POS_TWO + NEG_ONE;
    printf("%d %d %d %d\n", a, b, c, d);
    // EXPECT: 1001000 99000000 6 1
}

void test_enum_switch(void) {
    int i;
    for (i = -3; i <= 2; i++) {
        switch (i) {
            case NEG_THREE: printf("neg3\n"); break;
            case NEG_TWO:   printf("neg2\n"); break;
            case NEG_ONE:   printf("neg1\n"); break;
            case ZERO:      printf("zero\n"); break;
            case POS_ONE:   printf("pos1\n"); break;
            case POS_TWO:   printf("pos2\n"); break;
        }
    }
    // EXPECT: neg3
    // EXPECT: neg2
    // EXPECT: neg1
    // EXPECT: zero
    // EXPECT: pos1
    // EXPECT: pos2
}

void test_enum_as_array_index(void) {
    int vals[3];
    vals[RED] = 100;
    vals[GREEN] = 200;
    vals[BLUE] = 300;
    printf("%d %d %d\n", vals[RED], vals[GREEN], vals[BLUE]);
    // EXPECT: 100 200 300
}

void test_enum_comparison(void) {
    int x = 1;
    printf("eq=%d lt=%d gt=%d\n", x == GREEN, NEG_ONE < ZERO, POS_TWO > POS_ONE);
    // EXPECT: eq=1 lt=1 gt=1
}

void test_overlapping_enums(void) {
    int a = OVA_X;
    int b = OVB_X;
    printf("overlap eq=%d\n", a == b);
    // EXPECT: overlap eq=1
    printf("%d %d %d\n", OVA_X + OVB_Y, OVA_Y + OVB_Z, OVA_Z + OVB_X);
    // EXPECT: 30 50 40
}

void test_auto_values(void) {
    printf("%d %d %d %d %d %d %d %d\n",
           AUTO_A, AUTO_B, AUTO_C, AUTO_D, AUTO_E, AUTO_F, AUTO_G, AUTO_H);
    // EXPECT: 0 1 100 101 102 -5 -4 -3
}

void test_anonymous_enum(void) {
    printf("%d %d %d\n", ANON_A, ANON_B, ANON_C);
    // EXPECT: 42 99 -7
    int x = ANON_A + ANON_C;
    printf("sum=%d\n", x);
    // EXPECT: sum=35
}

void test_enum_in_loop(void) {
    enum color c;
    for (c = RED; c <= BLUE; c++) {
        printf("color %d\n", c);
    }
    // EXPECT: color 0
    // EXPECT: color 1
    // EXPECT: color 2
}

void test_enum_ternary(void) {
    int v = (NEG_ONE < 0) ? LARGE : SMALL;
    printf("ternary=%d\n", v);
    // EXPECT: ternary=1000000
}

void test_enum_bitwise(void) {
    int a = RED | GREEN;
    int b = GREEN & BLUE;
    int c = RED ^ BLUE;
    printf("%d %d %d\n", a, b, c);
    // EXPECT: 1 0 2
}

void test_enum_nested_switch(void) {
    enum color outer = GREEN;
    enum large_vals inner_val = MEDIUM;
    int result = 0;
    switch (outer) {
        case RED: result = 1; break;
        case GREEN:
            switch (inner_val) {
                case SMALL: result = 10; break;
                case MEDIUM: result = 20; break;
                case LARGE: result = 30; break;
                default: result = 40; break;
            }
            break;
        case BLUE: result = 3; break;
    }
    printf("nested=%d\n", result);
    // EXPECT: nested=20
}

int main(void) {
    test_basic_enum();
    test_large_values();
    test_negative_values();
    test_enum_arithmetic();
    test_enum_switch();
    test_enum_as_array_index();
    test_enum_comparison();
    test_overlapping_enums();
    test_auto_values();
    test_anonymous_enum();
    test_enum_in_loop();
    test_enum_ternary();
    test_enum_bitwise();
    test_enum_nested_switch();
    return 0;
}
