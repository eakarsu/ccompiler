int printf(const char *fmt, ...);

// Timer/Counter Module Simulation
// Simulates hardware timers with prescaler, compare match, overflow

enum TimerMode { TIMER_NORMAL, TIMER_CTC, TIMER_OVERFLOW };
enum TimerState { TIMER_STOPPED, TIMER_RUNNING, TIMER_PAUSED };

struct Timer {
    int id;
    int counter;
    int top;
    int prescaler;
    int mode;
    int state;
    int overflow_count;
    int compare_match_count;
    int compare_value;
    int ticks_total;
};

void timer_init(struct Timer *t, int id, int top, int prescaler) {
    t->id = id;
    t->counter = 0;
    t->top = top;
    t->prescaler = prescaler;
    t->mode = TIMER_NORMAL;
    t->state = TIMER_STOPPED;
    t->overflow_count = 0;
    t->compare_match_count = 0;
    t->compare_value = 0;
    t->ticks_total = 0;
}

void timer_set_mode(struct Timer *t, int mode) {
    t->mode = mode;
}

void timer_set_compare(struct Timer *t, int val) {
    t->compare_value = val;
}

void timer_start(struct Timer *t) {
    t->state = TIMER_RUNNING;
}

void timer_stop(struct Timer *t) {
    t->state = TIMER_STOPPED;
}

void timer_pause(struct Timer *t) {
    t->state = TIMER_PAUSED;
}

void timer_reset(struct Timer *t) {
    t->counter = 0;
    t->overflow_count = 0;
    t->compare_match_count = 0;
    t->ticks_total = 0;
}

// Simulate one prescaled tick
void timer_tick(struct Timer *t) {
    if (t->state != TIMER_RUNNING) return;

    t->ticks_total = t->ticks_total + 1;

    // Only increment counter every prescaler ticks
    if (t->ticks_total % t->prescaler != 0) return;

    t->counter = t->counter + 1;

    // Check compare match
    if (t->counter == t->compare_value && t->compare_value > 0) {
        t->compare_match_count = t->compare_match_count + 1;
        if (t->mode == TIMER_CTC) {
            t->counter = 0;
            return;
        }
    }

    // Check overflow
    if (t->counter >= t->top) {
        t->overflow_count = t->overflow_count + 1;
        t->counter = 0;
    }
}

// Simulate multiple ticks
void timer_advance(struct Timer *t, int ticks) {
    int i;
    for (i = 0; i < ticks; i++) {
        timer_tick(t);
    }
}

int timer_get_elapsed_us(struct Timer *t, int clock_mhz) {
    // elapsed = ticks_total / clock_mhz (microseconds)
    if (clock_mhz == 0) return 0;
    return t->ticks_total / clock_mhz;
}

int timer_get_frequency(struct Timer *t, int clock_mhz) {
    // Output frequency for CTC mode = clock / (2 * prescaler * (1 + compare))
    if (t->compare_value <= 0) return 0;
    int divisor = 2 * t->prescaler * (1 + t->compare_value);
    if (divisor == 0) return 0;
    return (clock_mhz * 1000000) / divisor;
}

int main() {
    struct Timer t0;
    struct Timer t1;

    // Test basic timer with prescaler 1
    timer_init(&t0, 0, 256, 1);
    timer_start(&t0);
    printf("Timer %d started\n", t0.id);
    // EXPECT: Timer 0 started

    // Tick 10 times
    timer_advance(&t0, 10);
    printf("Counter after 10 ticks: %d\n", t0.counter);
    // EXPECT: Counter after 10 ticks: 10

    // Tick to overflow (246 more to reach 256)
    timer_advance(&t0, 246);
    printf("Counter after 256 ticks: %d\n", t0.counter);
    // EXPECT: Counter after 256 ticks: 0
    printf("Overflow count: %d\n", t0.overflow_count);
    // EXPECT: Overflow count: 1

    // Continue for another full cycle
    timer_advance(&t0, 256);
    printf("Overflow count after 2 cycles: %d\n", t0.overflow_count);
    // EXPECT: Overflow count after 2 cycles: 2

    printf("Total ticks: %d\n", t0.ticks_total);
    // EXPECT: Total ticks: 512

    // Test CTC mode with compare match
    timer_init(&t1, 1, 256, 1);
    timer_set_mode(&t1, TIMER_CTC);
    timer_set_compare(&t1, 50);
    timer_start(&t1);
    printf("Timer %d CTC mode, compare=%d\n", t1.id, t1.compare_value);
    // EXPECT: Timer 1 CTC mode, compare=50

    // Tick 50 times to hit compare match
    timer_advance(&t1, 50);
    printf("Counter at compare: %d\n", t1.counter);
    // EXPECT: Counter at compare: 0
    printf("Compare matches: %d\n", t1.compare_match_count);
    // EXPECT: Compare matches: 1

    // Run 200 more ticks (should get 4 more matches at 50 each)
    timer_advance(&t1, 200);
    printf("Compare matches after 250 ticks: %d\n", t1.compare_match_count);
    // EXPECT: Compare matches after 250 ticks: 5

    // Test prescaler
    timer_init(&t0, 2, 100, 4);
    timer_start(&t0);
    timer_advance(&t0, 16);
    printf("Counter with prescaler 4 after 16 ticks: %d\n", t0.counter);
    // EXPECT: Counter with prescaler 4 after 16 ticks: 4

    timer_advance(&t0, 384);
    printf("Counter after 400 total: %d\n", t0.counter);
    // EXPECT: Counter after 400 total: 0
    printf("Overflows with prescaler: %d\n", t0.overflow_count);
    // EXPECT: Overflows with prescaler: 1

    // Test pause/resume
    timer_init(&t0, 3, 256, 1);
    timer_start(&t0);
    timer_advance(&t0, 30);
    timer_pause(&t0);
    timer_advance(&t0, 100);  // should not count
    printf("Counter after pause: %d\n", t0.counter);
    // EXPECT: Counter after pause: 30

    timer_start(&t0);
    timer_advance(&t0, 20);
    printf("Counter after resume: %d\n", t0.counter);
    // EXPECT: Counter after resume: 50

    // Test elapsed time calculation
    printf("Elapsed us (16MHz): %d\n", timer_get_elapsed_us(&t0, 16));
    // EXPECT: Elapsed us (16MHz): 3

    // Test frequency calculation for CTC
    timer_init(&t1, 4, 256, 8);
    timer_set_mode(&t1, TIMER_CTC);
    timer_set_compare(&t1, 124);
    int freq = timer_get_frequency(&t1, 16);
    printf("CTC frequency: %d\n", freq);
    // EXPECT: CTC frequency: 8000

    // Test reset
    timer_start(&t1);
    timer_advance(&t1, 1000);
    timer_reset(&t1);
    printf("After reset counter: %d\n", t1.counter);
    // EXPECT: After reset counter: 0
    printf("After reset overflows: %d\n", t1.overflow_count);
    // EXPECT: After reset overflows: 0

    printf("Timer test complete\n");
    // EXPECT: Timer test complete

    return 0;
}
