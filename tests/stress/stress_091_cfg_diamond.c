// stress_091_cfg_diamond.c - Diamond-shaped control flow torture test
// Exercises: nested diamonds, chained diamonds, diamonds with side effects,
// diamonds feeding loops, diamonds selecting functions to call.
// EXPECT: simple_diamond=10
// EXPECT: simple_diamond=75
// EXPECT: chain5=90
// EXPECT: chain5=90
// EXPECT: nested_dd=100
// EXPECT: nested_dd=64
// EXPECT: nested_dd=36
// EXPECT: side_effect=15
// EXPECT: side_effect=27
// EXPECT: diamond_loop=264
// EXPECT: diamond_loop=360
// EXPECT: diamond_select=50
// EXPECT: diamond_select=30
// EXPECT: mega_diamond=4225
// EXPECT: mega_diamond=4900
// EXPECT: all_pass=1

int printf(const char *fmt, ...);

// Simple diamond: if/else that rejoins
int simple_diamond(int x) {
    int r;
    if (x > 10) {
        r = x * 5;
    } else {
        r = x + 5;
    }
    return r;
}

// 5 chained diamonds: result of one feeds the next
int chain5(int x) {
    int a;
    if (x > 0) { a = x + 10; } else { a = -x + 10; }

    int b;
    if (a > 15) { b = a * 2; } else { b = a + 5; }

    int c;
    if (b % 2 == 0) { c = b + 10; } else { c = b - 5; }

    int d;
    if (c > 30) { d = c + 20; } else { d = c * 3; }

    int e;
    if (d > 50) { e = d; } else { e = 50; }

    return e;
}

// Nested diamond: diamond inside each arm of an outer diamond
int nested_dd(int x, int y) {
    int r;
    if (x > 0) {
        // inner diamond on true side
        if (y > 0) {
            r = x * y;
        } else {
            r = x * (-y);
        }
    } else {
        // inner diamond on false side
        if (y > 0) {
            r = (-x) * y;
        } else {
            r = (-x) * (-y);
        }
    }
    return r;
}

// Diamond with side effects on both paths
int side_global;

int side_effect_diamond(int x) {
    side_global = 0;
    int r = 0;
    // diamond 1
    if (x > 5) { side_global = side_global + 10; r = 1; }
    else { side_global = side_global + 20; r = 2; }
    // diamond 2
    if (x > 3) { side_global = side_global + 3; r = r + 10; }
    else { side_global = side_global + 6; r = r + 20; }
    // diamond 3
    if (r > 15) { side_global = side_global + 1; }
    else { side_global = side_global + 2; }

    return side_global;
}

// Diamonds feeding into loops
int diamond_loop(int x) {
    int start;
    int step;
    int limit;

    if (x > 10) { start = x; } else { start = 10; }
    if (x % 2 == 0) { step = 2; } else { step = 3; }
    if (x > 5) { limit = start + 20; } else { limit = start + 30; }

    int sum = 0;
    int i;
    for (i = start; i < limit; i = i + step) {
        // inner diamond inside loop
        if (i % 2 == 0) {
            sum = sum + i;
        } else {
            sum = sum + i * 2;
        }
    }
    return sum;
}

// Diamond selects which function to call
int mul3(int x) { return x * 3; }
int mul4(int x) { return x * 4; }
int add10(int x) { return x + 10; }
int sub5(int x) { return x - 5; }

int diamond_select(int x) {
    int (*f1)(int);
    int (*f2)(int);

    if (x > 5) {
        f1 = mul3;
    } else {
        f1 = mul4;
    }

    if (x % 2 == 0) {
        f2 = add10;
    } else {
        f2 = sub5;
    }

    return f1(x) + f2(x);
}

// Mega diamond: 10 chained diamonds computing a value
int mega_diamond(int x) {
    int a; if (x > 50) { a = x; } else { a = 50; }
    int b; if (a % 3 == 0) { b = a + 1; } else { b = a; }
    int c; if (b > 45) { c = b - 5; } else { c = b + 5; }
    int d; if (c * 2 > 100) { d = c; } else { d = 50; }
    int e; if (d == 50) { e = d * 2; } else { e = d; }
    int f; if (e > 80) { f = e - 30; } else { f = e; }
    int g; if (f < 60) { g = f + 10; } else { g = f; }
    int h; if (g % 5 == 0) { h = g * g; } else { h = g * 2; }
    int i; if (h > 200) { i = h; } else { i = 200; }
    int j; if (i > 1000) { j = i; } else { j = i / 2; }
    return j;
}

int main(void) {
    int pass = 1;

    // simple diamond tests
    printf("simple_diamond=%d\n", simple_diamond(5));
    printf("simple_diamond=%d\n", simple_diamond(15));
    if (simple_diamond(5) != 10) pass = 0;
    if (simple_diamond(15) != 75) pass = 0;

    // chained diamonds
    printf("chain5=%d\n", chain5(20));
    printf("chain5=%d\n", chain5(-5));
    if (chain5(20) != 90) pass = 0;
    if (chain5(-5) != 90) pass = 0;

    // nested diamonds
    printf("nested_dd=%d\n", nested_dd(10, 10));
    printf("nested_dd=%d\n", nested_dd(-8, -8));
    printf("nested_dd=%d\n", nested_dd(-6, 6));
    if (nested_dd(10, 10) != 100) pass = 0;
    if (nested_dd(-8, -8) != 64) pass = 0;
    if (nested_dd(-6, 6) != 36) pass = 0;

    // side effects
    printf("side_effect=%d\n", side_effect_diamond(10));
    printf("side_effect=%d\n", side_effect_diamond(1));
    if (side_effect_diamond(10) != 15) pass = 0;
    if (side_effect_diamond(1) != 27) pass = 0;

    // diamond loop
    printf("diamond_loop=%d\n", diamond_loop(15));
    printf("diamond_loop=%d\n", diamond_loop(3));
    if (diamond_loop(15) != 264) pass = 0;
    if (diamond_loop(3) != 360) pass = 0;

    // diamond select
    printf("diamond_select=%d\n", diamond_select(10));
    printf("diamond_select=%d\n", diamond_select(4));
    if (diamond_select(10) != 50) pass = 0;
    if (diamond_select(4) != 30) pass = 0;

    // mega diamond
    printf("mega_diamond=%d\n", mega_diamond(100));
    printf("mega_diamond=%d\n", mega_diamond(10));
    if (mega_diamond(100) != 4225) pass = 0;
    if (mega_diamond(10) != 4900) pass = 0;

    printf("all_pass=%d\n", pass);
    return 0;
}
