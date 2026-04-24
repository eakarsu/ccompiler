int printf(const char *fmt, ...);

int simple_ternary(int x) {
    return x > 0 ? 1 : -1;
}

int chained_ternary(int x) {
    return x > 0 ? 1 : x == 0 ? 0 : -1;
}

int multi_chain(int x) {
    return x >= 90 ? 4
         : x >= 80 ? 3
         : x >= 70 ? 2
         : x >= 60 ? 1
         : 0;
}

int nested_both(int a, int b) {
    return (a > 0 ? (b > 0 ? 1 : 2) : (b > 0 ? 3 : 4));
}

int ternary_in_args(int x) {
    int a = x > 5 ? 100 : 0;
    int b = x > 3 ? 10 : 0;
    int c = x > 1 ? 1 : 0;
    return a + b + c;
}

int ternary_as_condition(int x) {
    int val = (x > 0 ? x : -x);
    if (val > 10) return 1;
    return 0;
}

int ternary_assign(int x) {
    int a;
    int b;
    a = x > 0 ? x * 2 : x * 3;
    b = a > 10 ? a - 10 : a + 10;
    return b;
}

int ternary_in_loop(int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        sum = sum + (i % 2 == 0 ? i : -i);
    }
    return sum;
}

int ternary_side_effect(int x) {
    int a = 0;
    int b = 0;
    int r;
    r = x > 0 ? (a = 10, a + 5) : (b = 20, b + 5);
    return r * 100 + a * 10 + b;
}

int classify_char(int c) {
    return (c >= 48 && c <= 57) ? 1
         : (c >= 65 && c <= 90) ? 2
         : (c >= 97 && c <= 122) ? 3
         : 0;
}

int clamp(int val, int lo, int hi) {
    return val < lo ? lo : val > hi ? hi : val;
}

int abs_val(int x) {
    return x >= 0 ? x : -x;
}

int sign(int x) {
    return x > 0 ? 1 : x < 0 ? -1 : 0;
}

int ternary_index(int idx) {
    int arr[4];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    int safe = idx >= 0 && idx < 4 ? idx : 0;
    return arr[safe];
}

int deep_nest(int a, int b, int c) {
    return a > 0
        ? (b > 0
            ? (c > 0 ? 111 : 110)
            : (c > 0 ? 101 : 100))
        : (b > 0
            ? (c > 0 ? 11 : 10)
            : (c > 0 ? 1 : 0));
}

int main(void) {
    // EXPECT: simple 5: 1
    printf("simple 5: %d\n", simple_ternary(5));
    // EXPECT: simple -3: -1
    printf("simple -3: %d\n", simple_ternary(-3));
    // EXPECT: chain 5: 1
    printf("chain 5: %d\n", chained_ternary(5));
    // EXPECT: chain 0: 0
    printf("chain 0: %d\n", chained_ternary(0));
    // EXPECT: chain -3: -1
    printf("chain -3: %d\n", chained_ternary(-3));
    // EXPECT: multi 95: 4
    printf("multi 95: %d\n", multi_chain(95));
    // EXPECT: multi 75: 2
    printf("multi 75: %d\n", multi_chain(75));
    // EXPECT: multi 50: 0
    printf("multi 50: %d\n", multi_chain(50));
    // EXPECT: nested 1,1: 1
    printf("nested 1,1: %d\n", nested_both(1, 1));
    // EXPECT: nested 1,-1: 2
    printf("nested 1,-1: %d\n", nested_both(1, -1));
    // EXPECT: nested -1,1: 3
    printf("nested -1,1: %d\n", nested_both(-1, 1));
    // EXPECT: nested -1,-1: 4
    printf("nested -1,-1: %d\n", nested_both(-1, -1));
    // EXPECT: args 6: 111
    printf("args 6: %d\n", ternary_in_args(6));
    // EXPECT: args 2: 1
    printf("args 2: %d\n", ternary_in_args(2));
    // EXPECT: cond 15: 1
    printf("cond 15: %d\n", ternary_as_condition(15));
    // EXPECT: cond -15: 1
    printf("cond -15: %d\n", ternary_as_condition(-15));
    // EXPECT: assign 7: 4
    printf("assign 7: %d\n", ternary_assign(7));
    // EXPECT: assign -3: 1
    printf("assign -3: %d\n", ternary_assign(-3));
    // EXPECT: loop 6: -3
    printf("loop 6: %d\n", ternary_in_loop(6));
    // EXPECT: sideeff 5: 1600
    printf("sideeff 5: %d\n", ternary_side_effect(5));
    // EXPECT: sideeff -5: 2520
    printf("sideeff -5: %d\n", ternary_side_effect(-5));
    // EXPECT: classify 65: 2
    printf("classify 65: %d\n", classify_char(65));
    // EXPECT: classify 97: 3
    printf("classify 97: %d\n", classify_char(97));
    // EXPECT: clamp 5,1,10: 5
    printf("clamp 5,1,10: %d\n", clamp(5, 1, 10));
    // EXPECT: clamp -3,0,10: 0
    printf("clamp -3,0,10: %d\n", clamp(-3, 0, 10));
    // EXPECT: clamp 15,0,10: 10
    printf("clamp 15,0,10: %d\n", clamp(15, 0, 10));
    // EXPECT: sign 42: 1
    printf("sign 42: %d\n", sign(42));
    // EXPECT: sign -7: -1
    printf("sign -7: %d\n", sign(-7));
    // EXPECT: sign 0: 0
    printf("sign 0: %d\n", sign(0));
    // EXPECT: idx 2: 30
    printf("idx 2: %d\n", ternary_index(2));
    // EXPECT: idx 99: 10
    printf("idx 99: %d\n", ternary_index(99));
    // EXPECT: deep 1,1,1: 111
    printf("deep 1,1,1: %d\n", deep_nest(1, 1, 1));
    // EXPECT: deep -1,-1,1: 1
    printf("deep -1,-1,1: %d\n", deep_nest(-1, -1, 1));

    return 0;
}
