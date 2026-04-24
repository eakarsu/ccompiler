int printf(const char *fmt, ...);

// EXPECT: === Moving Average Filter ===
// Moving Average Filter (integer-based)
// Computes simple moving average using integer arithmetic

struct MovingAvg {
    int buffer[16];
    int size;
    int count;
    int index;
    int sum;
};

void ma_init(struct MovingAvg *ma, int size) {
    int i;
    ma->size = size;
    ma->count = 0;
    ma->index = 0;
    ma->sum = 0;
    for (i = 0; i < 16; i++) {
        ma->buffer[i] = 0;
    }
}

int ma_add(struct MovingAvg *ma, int value) {
    ma->sum = ma->sum - ma->buffer[ma->index] + value;
    ma->buffer[ma->index] = value;
    ma->index = (ma->index + 1) % ma->size;
    if (ma->count < ma->size) {
        ma->count = ma->count + 1;
    }
    return ma->sum / ma->count;
}

void test_basic_moving_avg(void) {
    struct MovingAvg ma;
    int result;

    ma_init(&ma, 3);

    result = ma_add(&ma, 10);
    printf("MA add 10: %d\n", result); // EXPECT: MA add 10: 10

    result = ma_add(&ma, 20);
    printf("MA add 20: %d\n", result); // EXPECT: MA add 20: 15

    result = ma_add(&ma, 30);
    printf("MA add 30: %d\n", result); // EXPECT: MA add 30: 20

    result = ma_add(&ma, 40);
    printf("MA add 40: %d\n", result); // EXPECT: MA add 40: 30

    result = ma_add(&ma, 50);
    printf("MA add 50: %d\n", result); // EXPECT: MA add 50: 40
}

void test_window_sizes(void) {
    struct MovingAvg ma2;
    struct MovingAvg ma5;
    int signal[8];
    int i;

    signal[0] = 5;
    signal[1] = 10;
    signal[2] = 15;
    signal[3] = 20;
    signal[4] = 25;
    signal[5] = 30;
    signal[6] = 35;
    signal[7] = 40;

    ma_init(&ma2, 2);
    ma_init(&ma5, 5);

    printf("Window size comparison:\n"); // EXPECT: Window size comparison:
    for (i = 0; i < 8; i++) {
        int r2 = ma_add(&ma2, signal[i]);
        int r5 = ma_add(&ma5, signal[i]);
        printf("  s[%d]=%d w2=%d w5=%d\n", i, signal[i], r2, r5);
    }
    // EXPECT:   s[0]=5 w2=5 w5=5
    // EXPECT:   s[1]=10 w2=7 w5=7
    // EXPECT:   s[2]=15 w2=12 w5=10
    // EXPECT:   s[3]=20 w2=17 w5=12
    // EXPECT:   s[4]=25 w2=22 w5=15
    // EXPECT:   s[5]=30 w2=27 w5=20
    // EXPECT:   s[6]=35 w2=32 w5=25
    // EXPECT:   s[7]=40 w2=37 w5=30
}

void test_noisy_signal(void) {
    struct MovingAvg ma;
    int noisy[10];
    int i;

    noisy[0] = 100;
    noisy[1] = 120;
    noisy[2] = 90;
    noisy[3] = 110;
    noisy[4] = 95;
    noisy[5] = 105;
    noisy[6] = 98;
    noisy[7] = 102;
    noisy[8] = 100;
    noisy[9] = 101;

    ma_init(&ma, 4);
    printf("Smoothed noisy signal:\n"); // EXPECT: Smoothed noisy signal:
    for (i = 0; i < 10; i++) {
        int smoothed = ma_add(&ma, noisy[i]);
        printf("  n[%d]=%d smooth=%d\n", i, noisy[i], smoothed);
    }
    // EXPECT:   n[0]=100 smooth=100
    // EXPECT:   n[1]=120 smooth=110
    // EXPECT:   n[2]=90 smooth=103
    // EXPECT:   n[3]=110 smooth=105
    // EXPECT:   n[4]=95 smooth=103
    // EXPECT:   n[5]=105 smooth=100
    // EXPECT:   n[6]=98 smooth=102
    // EXPECT:   n[7]=102 smooth=100
    // EXPECT:   n[8]=100 smooth=101
    // EXPECT:   n[9]=101 smooth=100
}

int main(void) {
    printf("=== Moving Average Filter ===\n");
    test_basic_moving_avg();
    test_window_sizes();
    test_noisy_signal();
    printf("All moving average tests passed.\n"); // EXPECT: All moving average tests passed.
    return 0;
}
