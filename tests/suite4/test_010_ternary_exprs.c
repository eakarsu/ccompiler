int printf(const char *fmt, ...);

// Test ternary operator with nested ternaries, side effects,
// complex conditions, and various use cases.

int global_effect;

int with_effect(int val) {
    global_effect = global_effect + 1;
    return val;
}

int abs_val(int x) {
    return x < 0 ? -x : x;
}

int max2(int a, int b) {
    return a > b ? a : b;
}

int min2(int a, int b) {
    return a < b ? a : b;
}

int sign(int x) {
    return x > 0 ? 1 : (x < 0 ? -1 : 0);
}

int clamp(int val, int lo, int hi) {
    return val < lo ? lo : (val > hi ? hi : val);
}

int classify(int x) {
    // Returns: 0=zero, 1=small positive, 2=large positive,
    //         -1=small negative, -2=large negative
    return x == 0 ? 0 :
           (x > 0 ? (x <= 100 ? 1 : 2) :
                     (x >= -100 ? -1 : -2));
}

int safe_div(int a, int b) {
    return b != 0 ? a / b : 0;
}

int is_even(int x) {
    return x % 2 == 0 ? 1 : 0;
}

int grade(int score) {
    // Returns letter grade as ASCII: A=65, B=66, C=67, D=68, F=70
    return score >= 90 ? 65 :
           (score >= 80 ? 66 :
           (score >= 70 ? 67 :
           (score >= 60 ? 68 : 70)));
}

int fibonacci(int n) {
    // Iterative fibonacci using ternary for init
    int a = 0;
    int b = 1;
    int i;
    int temp;
    for (i = 0; i < n; i = i + 1) {
        temp = a + b;
        a = b;
        b = temp;
    }
    return a;
}

int max3(int a, int b, int c) {
    return a > b ? (a > c ? a : c) : (b > c ? b : c);
}

int min3(int a, int b, int c) {
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int median3(int a, int b, int c) {
    int mx = max3(a, b, c);
    int mn = min3(a, b, c);
    // median = a + b + c - max - min
    return a + b + c - mx - mn;
}

int bounded_inc(int val, int limit) {
    return val < limit ? val + 1 : limit;
}

int bounded_dec(int val, int limit) {
    return val > limit ? val - 1 : limit;
}

int main(void) {
    // Basic ternary
    // EXPECT: abs(-7) = 7
    printf("abs(%d) = %d\n", -7, abs_val(-7));

    // EXPECT: abs(5) = 5
    printf("abs(%d) = %d\n", 5, abs_val(5));

    // EXPECT: abs(0) = 0
    printf("abs(%d) = %d\n", 0, abs_val(0));

    // Max and min
    // EXPECT: max(3, 7) = 7
    printf("max(%d, %d) = %d\n", 3, 7, max2(3, 7));

    // EXPECT: min(3, 7) = 3
    printf("min(%d, %d) = %d\n", 3, 7, min2(3, 7));

    // EXPECT: max(-5, -2) = -2
    printf("max(%d, %d) = %d\n", -5, -2, max2(-5, -2));

    // Nested ternary: sign
    // EXPECT: sign(42) = 1
    printf("sign(%d) = %d\n", 42, sign(42));

    // EXPECT: sign(-10) = -1
    printf("sign(%d) = %d\n", -10, sign(-10));

    // EXPECT: sign(0) = 0
    printf("sign(%d) = %d\n", 0, sign(0));

    // Clamp (double nested)
    // EXPECT: clamp(5, 0, 10) = 5
    printf("clamp(%d, %d, %d) = %d\n", 5, 0, 10, clamp(5, 0, 10));

    // EXPECT: clamp(-5, 0, 10) = 0
    printf("clamp(%d, %d, %d) = %d\n", -5, 0, 10, clamp(-5, 0, 10));

    // EXPECT: clamp(15, 0, 10) = 10
    printf("clamp(%d, %d, %d) = %d\n", 15, 0, 10, clamp(15, 0, 10));

    // Classify (deeply nested)
    // EXPECT: classify(0) = 0
    printf("classify(%d) = %d\n", 0, classify(0));

    // EXPECT: classify(50) = 1
    printf("classify(%d) = %d\n", 50, classify(50));

    // EXPECT: classify(200) = 2
    printf("classify(%d) = %d\n", 200, classify(200));

    // EXPECT: classify(-50) = -1
    printf("classify(%d) = %d\n", -50, classify(-50));

    // EXPECT: classify(-200) = -2
    printf("classify(%d) = %d\n", -200, classify(-200));

    // Safe division
    // EXPECT: safe_div(10, 3) = 3
    printf("safe_div(%d, %d) = %d\n", 10, 3, safe_div(10, 3));

    // EXPECT: safe_div(10, 0) = 0
    printf("safe_div(%d, %d) = %d\n", 10, 0, safe_div(10, 0));

    // Is even
    // EXPECT: is_even(4) = 1
    printf("is_even(%d) = %d\n", 4, is_even(4));

    // EXPECT: is_even(7) = 0
    printf("is_even(%d) = %d\n", 7, is_even(7));

    // Grade (4 levels of nesting)
    // EXPECT: grade(95) = 65
    printf("grade(%d) = %d\n", 95, grade(95));

    // EXPECT: grade(85) = 66
    printf("grade(%d) = %d\n", 85, grade(85));

    // EXPECT: grade(75) = 67
    printf("grade(%d) = %d\n", 75, grade(75));

    // EXPECT: grade(65) = 68
    printf("grade(%d) = %d\n", 65, grade(65));

    // EXPECT: grade(50) = 70
    printf("grade(%d) = %d\n", 50, grade(50));

    // Max/min/median of three
    // EXPECT: max3(3, 7, 5) = 7
    printf("max3(%d, %d, %d) = %d\n", 3, 7, 5, max3(3, 7, 5));

    // EXPECT: min3(3, 7, 5) = 3
    printf("min3(%d, %d, %d) = %d\n", 3, 7, 5, min3(3, 7, 5));

    // EXPECT: median3(3, 7, 5) = 5
    printf("median3(%d, %d, %d) = %d\n", 3, 7, 5, median3(3, 7, 5));

    // EXPECT: median3(-1, -5, -3) = -3
    printf("median3(%d, %d, %d) = %d\n", -1, -5, -3, median3(-1, -5, -3));

    // Ternary with side effects: only one branch should execute
    global_effect = 0;
    int r1 = 1 ? with_effect(10) : with_effect(20);
    // EXPECT: effect_true: val=10 count=1
    printf("effect_true: val=%d count=%d\n", r1, global_effect);

    global_effect = 0;
    int r2 = 0 ? with_effect(10) : with_effect(20);
    // EXPECT: effect_false: val=20 count=1
    printf("effect_false: val=%d count=%d\n", r2, global_effect);

    // Ternary in loop
    int sum = 0;
    int i;
    for (i = 1; i <= 10; i = i + 1) {
        sum = sum + (i % 2 == 0 ? i : -i);
    }
    // -1+2-3+4-5+6-7+8-9+10 = 5
    // EXPECT: alt_sum = 5
    printf("alt_sum = %d\n", sum);

    // Bounded increment
    // EXPECT: bounded_inc(5, 10) = 6
    printf("bounded_inc(%d, %d) = %d\n", 5, 10, bounded_inc(5, 10));

    // EXPECT: bounded_inc(10, 10) = 10
    printf("bounded_inc(%d, %d) = %d\n", 10, 10, bounded_inc(10, 10));

    // Bounded decrement
    // EXPECT: bounded_dec(5, 0) = 4
    printf("bounded_dec(%d, %d) = %d\n", 5, 0, bounded_dec(5, 0));

    // EXPECT: bounded_dec(0, 0) = 0
    printf("bounded_dec(%d, %d) = %d\n", 0, 0, bounded_dec(0, 0));

    // Ternary as array index
    int arr[3];
    arr[0] = 100;
    arr[1] = 200;
    arr[2] = 300;
    int idx = 1 > 0 ? 2 : 0;
    // EXPECT: arr[ternary] = 300
    printf("arr[ternary] = %d\n", arr[idx]);

    // Chained ternary for mapping
    int val = 3;
    int mapped = val == 1 ? 10 :
                 (val == 2 ? 20 :
                 (val == 3 ? 30 : 0));
    // EXPECT: mapped(3) = 30
    printf("mapped(%d) = %d\n", val, mapped);

    return 0;
}
