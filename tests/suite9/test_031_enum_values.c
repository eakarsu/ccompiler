int printf(const char *fmt, ...);

enum Color {
    RED = 0,
    GREEN = 1,
    BLUE = 2,
    YELLOW = 10,
    CYAN = 11,
    MAGENTA = 20
};

enum Negative {
    NEG_THREE = -3,
    NEG_TWO = -2,
    NEG_ONE = -1,
    ZERO = 0,
    POS_ONE = 1
};

enum Power {
    P0 = 1,
    P1 = 2,
    P2 = 4,
    P3 = 8,
    P4 = 16,
    P5 = 32,
    P6 = 64
};

enum AutoSeq {
    FIRST,
    SECOND,
    THIRD,
    JUMP = 100,
    AFTER_JUMP,
    AFTER_JUMP2
};

void test_basic_values() {
    // EXPECT: RED=0 GREEN=1 BLUE=2
    printf("RED=%d GREEN=%d BLUE=%d\n", RED, GREEN, BLUE);
    // EXPECT: YELLOW=10 CYAN=11 MAGENTA=20
    printf("YELLOW=%d CYAN=%d MAGENTA=%d\n", YELLOW, CYAN, MAGENTA);
}

void test_negative_values() {
    // EXPECT: NEG_THREE=-3 NEG_TWO=-2 NEG_ONE=-1
    printf("NEG_THREE=%d NEG_TWO=%d NEG_ONE=%d\n", NEG_THREE, NEG_TWO, NEG_ONE);
    // EXPECT: ZERO=0 POS_ONE=1
    printf("ZERO=%d POS_ONE=%d\n", ZERO, POS_ONE);
}

void test_power_values() {
    // EXPECT: P0=1 P1=2 P2=4
    printf("P0=%d P1=%d P2=%d\n", P0, P1, P2);
    // EXPECT: P3=8 P4=16 P5=32 P6=64
    printf("P3=%d P4=%d P5=%d P6=%d\n", P3, P4, P5, P6);
}

void test_auto_sequence() {
    // EXPECT: FIRST=0 SECOND=1 THIRD=2
    printf("FIRST=%d SECOND=%d THIRD=%d\n", FIRST, SECOND, THIRD);
    // EXPECT: JUMP=100 AFTER_JUMP=101 AFTER_JUMP2=102
    printf("JUMP=%d AFTER_JUMP=%d AFTER_JUMP2=%d\n", JUMP, AFTER_JUMP, AFTER_JUMP2);
}

void test_enum_arithmetic() {
    int a = RED + BLUE;
    int b = YELLOW - GREEN;
    int c = P3 * 2;
    int d = P6 / P2;
    // EXPECT: a=2 b=9
    printf("a=%d b=%d\n", a, b);
    // EXPECT: c=16 d=16
    printf("c=%d d=%d\n", c, d);
}

void test_enum_comparison() {
    int r1 = (RED < GREEN);
    int r2 = (MAGENTA > CYAN);
    int r3 = (NEG_ONE < ZERO);
    int r4 = (POS_ONE == 1);
    // EXPECT: r1=1 r2=1
    printf("r1=%d r2=%d\n", r1, r2);
    // EXPECT: r3=1 r4=1
    printf("r3=%d r4=%d\n", r3, r4);
}

void test_enum_in_loop() {
    int sum = 0;
    int i;
    for (i = NEG_THREE; i <= POS_ONE; i++) {
        sum = sum + i;
    }
    // EXPECT: sum=-5
    printf("sum=%d\n", sum);
}

void test_enum_switch() {
    enum Color c = BLUE;
    int val = 0;
    switch (c) {
        case RED: val = 100; break;
        case GREEN: val = 200; break;
        case BLUE: val = 300; break;
        default: val = -1; break;
    }
    // EXPECT: switch_val=300
    printf("switch_val=%d\n", val);
}

int main() {
    test_basic_values();
    test_negative_values();
    test_power_values();
    test_auto_sequence();
    test_enum_arithmetic();
    test_enum_comparison();
    test_enum_in_loop();
    test_enum_switch();
    // EXPECT: done
    printf("done\n");
    return 0;
}
