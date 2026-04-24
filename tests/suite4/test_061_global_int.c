int printf(const char *fmt, ...);

// Test global int variables: initialization, modification from main and other functions.

int g_zero;
int g_pos = 42;
int g_neg = -17;
int g_large = 100000;
int g_counter = 0;

int get_pos(void) {
    return g_pos;
}

int get_neg(void) {
    return g_neg;
}

void set_pos(int val) {
    g_pos = val;
}

void set_neg(int val) {
    g_neg = val;
}

void increment_counter(void) {
    g_counter = g_counter + 1;
}

int add_globals(void) {
    return g_pos + g_neg;
}

void swap_globals(void) {
    int tmp = g_pos;
    g_pos = g_neg;
    g_neg = tmp;
}

void multiply_counter(int factor) {
    g_counter = g_counter * factor;
}

int compute_with_globals(int x) {
    return x + g_pos - g_neg + g_large;
}

void reset_all(void) {
    g_zero = 0;
    g_pos = 42;
    g_neg = -17;
    g_large = 100000;
    g_counter = 0;
}

int accumulate(int n) {
    int i;
    int sum = 0;
    for (i = 0; i < n; i = i + 1) {
        increment_counter();
        sum = sum + g_counter;
    }
    return sum;
}

void double_pos(void) {
    g_pos = g_pos * 2;
}

int main(void) {
    // EXPECT: 0
    printf("%d\n", g_zero);

    // EXPECT: 42
    printf("%d\n", g_pos);

    // EXPECT: -17
    printf("%d\n", g_neg);

    // EXPECT: 100000
    printf("%d\n", g_large);

    // EXPECT: 42
    printf("%d\n", get_pos());

    // EXPECT: -17
    printf("%d\n", get_neg());

    set_pos(99);
    // EXPECT: 99
    printf("%d\n", g_pos);

    set_neg(-50);
    // EXPECT: -50
    printf("%d\n", g_neg);

    // EXPECT: 49
    printf("%d\n", add_globals());

    swap_globals();
    // EXPECT: -50
    printf("%d\n", g_pos);
    // EXPECT: 99
    printf("%d\n", g_neg);

    g_counter = 5;
    multiply_counter(3);
    // EXPECT: 15
    printf("%d\n", g_counter);

    reset_all();
    // EXPECT: 42
    printf("%d\n", g_pos);
    // EXPECT: -17
    printf("%d\n", g_neg);
    // EXPECT: 0
    printf("%d\n", g_counter);

    // compute_with_globals(10) = 10 + 42 - (-17) + 100000 = 100069
    // EXPECT: 100069
    printf("%d\n", compute_with_globals(10));

    reset_all();
    // accumulate(5): counter goes 1,2,3,4,5; sum = 1+2+3+4+5 = 15
    // EXPECT: 15
    printf("%d\n", accumulate(5));
    // EXPECT: 5
    printf("%d\n", g_counter);

    reset_all();
    double_pos();
    // EXPECT: 84
    printf("%d\n", g_pos);

    double_pos();
    // EXPECT: 168
    printf("%d\n", g_pos);

    g_zero = 777;
    // EXPECT: 777
    printf("%d\n", g_zero);

    g_large = -999;
    // EXPECT: -999
    printf("%d\n", g_large);

    return 0;
}
