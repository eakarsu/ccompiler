int printf(const char *fmt, ...);

// Timer/counter with prescaler simulation

enum timer_mode {
    TIMER_STOPPED,
    TIMER_FREE_RUN,
    TIMER_ONE_SHOT,
    TIMER_UP_DOWN
};

struct timer_config {
    int prescaler;
    int reload_value;
    int mode;
    int interrupt_enable;
};

struct timer {
    struct timer_config config;
    int counter;
    int direction;
    int overflow_count;
    int running;
    int interrupt_flag;
};

void timer_init(struct timer *t, int prescaler, int reload, int mode) {
    t->config.prescaler = prescaler;
    t->config.reload_value = reload;
    t->config.mode = mode;
    t->config.interrupt_enable = 0;
    t->counter = 0;
    t->direction = 1;
    t->overflow_count = 0;
    t->running = 0;
    t->interrupt_flag = 0;
}

void timer_start(struct timer *t) {
    t->running = 1;
}

void timer_stop(struct timer *t) {
    t->running = 0;
}

void timer_reset(struct timer *t) {
    t->counter = 0;
    t->direction = 1;
    t->overflow_count = 0;
    t->interrupt_flag = 0;
}

int timer_tick(struct timer *t, int sys_clocks) {
    int effective_ticks;
    int i;
    int events = 0;

    if (!t->running) return 0;

    effective_ticks = sys_clocks / t->config.prescaler;

    for (i = 0; i < effective_ticks; i++) {
        if (t->config.mode == TIMER_FREE_RUN) {
            t->counter++;
            if (t->counter >= t->config.reload_value) {
                t->counter = 0;
                t->overflow_count++;
                events++;
                if (t->config.interrupt_enable) {
                    t->interrupt_flag = 1;
                }
            }
        } else if (t->config.mode == TIMER_ONE_SHOT) {
            t->counter++;
            if (t->counter >= t->config.reload_value) {
                t->counter = t->config.reload_value;
                t->running = 0;
                t->overflow_count++;
                events++;
                if (t->config.interrupt_enable) {
                    t->interrupt_flag = 1;
                }
                break;
            }
        } else if (t->config.mode == TIMER_UP_DOWN) {
            t->counter = t->counter + t->direction;
            if (t->counter >= t->config.reload_value) {
                t->direction = -1;
                t->overflow_count++;
                events++;
                if (t->config.interrupt_enable) {
                    t->interrupt_flag = 1;
                }
            } else if (t->counter <= 0) {
                t->direction = 1;
                events++;
            }
        }
    }
    return events;
}

int timer_get_elapsed_us(struct timer *t, int clock_mhz) {
    int ticks_per_us = clock_mhz / t->config.prescaler;
    if (ticks_per_us == 0) ticks_per_us = 1;
    return t->counter / ticks_per_us;
}

int compute_frequency(int reload, int prescaler, int clock_hz) {
    int divisor = reload * prescaler;
    if (divisor == 0) return 0;
    return clock_hz / divisor;
}

int main(void) {
    struct timer t1;
    struct timer t2;
    struct timer t3;
    int events;

    timer_init(&t1, 1, 10, TIMER_FREE_RUN);
    printf("%d\n", t1.counter);
    // EXPECT: 0
    printf("%d\n", t1.running);
    // EXPECT: 0

    timer_start(&t1);
    printf("%d\n", t1.running);
    // EXPECT: 1

    events = timer_tick(&t1, 5);
    printf("%d\n", t1.counter);
    // EXPECT: 5
    printf("%d\n", events);
    // EXPECT: 0

    events = timer_tick(&t1, 5);
    printf("%d\n", t1.counter);
    // EXPECT: 0
    printf("%d\n", t1.overflow_count);
    // EXPECT: 1
    printf("%d\n", events);
    // EXPECT: 1

    events = timer_tick(&t1, 25);
    printf("%d\n", t1.counter);
    // EXPECT: 5
    printf("%d\n", t1.overflow_count);
    // EXPECT: 3
    printf("%d\n", events);
    // EXPECT: 2

    timer_init(&t2, 4, 10, TIMER_FREE_RUN);
    timer_start(&t2);
    events = timer_tick(&t2, 40);
    printf("%d\n", t2.counter);
    // EXPECT: 0
    printf("%d\n", t2.overflow_count);
    // EXPECT: 1
    printf("%d\n", events);
    // EXPECT: 1

    timer_init(&t3, 1, 5, TIMER_ONE_SHOT);
    timer_start(&t3);
    events = timer_tick(&t3, 3);
    printf("%d\n", t3.counter);
    // EXPECT: 3
    printf("%d\n", t3.running);
    // EXPECT: 1
    printf("%d\n", events);
    // EXPECT: 0

    events = timer_tick(&t3, 5);
    printf("%d\n", t3.counter);
    // EXPECT: 5
    printf("%d\n", t3.running);
    // EXPECT: 0
    printf("%d\n", events);
    // EXPECT: 1

    events = timer_tick(&t3, 10);
    printf("%d\n", t3.counter);
    // EXPECT: 5
    printf("%d\n", events);
    // EXPECT: 0

    struct timer t4;
    timer_init(&t4, 1, 4, TIMER_UP_DOWN);
    timer_start(&t4);
    events = timer_tick(&t4, 3);
    printf("%d\n", t4.counter);
    // EXPECT: 3
    printf("%d\n", events);
    // EXPECT: 0

    events = timer_tick(&t4, 1);
    printf("%d\n", t4.counter);
    // EXPECT: 4
    printf("%d\n", t4.direction);
    // EXPECT: -1
    printf("%d\n", events);
    // EXPECT: 1

    events = timer_tick(&t4, 4);
    printf("%d\n", t4.counter);
    // EXPECT: 0
    printf("%d\n", t4.direction);
    // EXPECT: 1
    printf("%d\n", events);
    // EXPECT: 1

    t1.config.interrupt_enable = 1;
    timer_reset(&t1);
    timer_start(&t1);
    timer_tick(&t1, 10);
    printf("%d\n", t1.interrupt_flag);
    // EXPECT: 1

    int freq = compute_frequency(1000, 8, 16000000);
    printf("%d\n", freq);
    // EXPECT: 2000

    freq = compute_frequency(256, 64, 8000000);
    printf("%d\n", freq);
    // EXPECT: 488

    printf("%s\n", "timer_done");
    // EXPECT: timer_done

    return 0;
}
