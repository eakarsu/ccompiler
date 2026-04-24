int printf(const char *fmt, ...);

/* Traffic light controller with sensor input.
   States: NS_GREEN=0, NS_YELLOW=1, EW_GREEN=2, EW_YELLOW=3, ALL_RED=4
   Inputs: TICK=0, SENSOR_NS=1, SENSOR_EW=2, EMERGENCY=3
   Each state has a minimum timer before transitioning. */

typedef struct {
    int state;
    int timer;
    int ns_green_count;
    int ew_green_count;
    int emergency_count;
    int cycle_count;
} TrafficLight;

int min_time[5];

void init_timings(void) {
    min_time[0] = 5;  /* NS_GREEN: 5 ticks minimum */
    min_time[1] = 2;  /* NS_YELLOW: 2 ticks */
    min_time[2] = 5;  /* EW_GREEN: 5 ticks minimum */
    min_time[3] = 2;  /* EW_YELLOW: 2 ticks */
    min_time[4] = 1;  /* ALL_RED: 1 tick */
}

void tl_init(TrafficLight *tl) {
    tl->state = 0; /* start NS_GREEN */
    tl->timer = 0;
    tl->ns_green_count = 0;
    tl->ew_green_count = 0;
    tl->emergency_count = 0;
    tl->cycle_count = 0;
}

void tl_step(TrafficLight *tl, int input) {
    int s = tl->state;
    int t = tl->timer;

    if (input == 3) {
        /* Emergency: go to ALL_RED immediately */
        tl->state = 4;
        tl->timer = 0;
        tl->emergency_count++;
        return;
    }

    t++;
    tl->timer = t;

    if (s == 0) {
        /* NS_GREEN */
        tl->ns_green_count++;
        if (t >= min_time[0] && input == 2) {
            tl->state = 1; /* NS_YELLOW */
            tl->timer = 0;
        }
    } else if (s == 1) {
        /* NS_YELLOW */
        if (t >= min_time[1]) {
            tl->state = 4; /* ALL_RED */
            tl->timer = 0;
        }
    } else if (s == 2) {
        /* EW_GREEN */
        tl->ew_green_count++;
        if (t >= min_time[2] && input == 1) {
            tl->state = 3; /* EW_YELLOW */
            tl->timer = 0;
        }
    } else if (s == 3) {
        /* EW_YELLOW */
        if (t >= min_time[3]) {
            tl->state = 4; /* ALL_RED */
            tl->timer = 0;
        }
    } else if (s == 4) {
        /* ALL_RED */
        if (t >= min_time[4]) {
            if (tl->ns_green_count > tl->ew_green_count) {
                tl->state = 2; /* EW_GREEN */
            } else {
                tl->state = 0; /* NS_GREEN */
            }
            tl->timer = 0;
            tl->cycle_count++;
        }
    }
}

char *state_name(int state) {
    if (state == 0) return "NS_GRN";
    if (state == 1) return "NS_YLW";
    if (state == 2) return "EW_GRN";
    if (state == 3) return "EW_YLW";
    if (state == 4) return "ALL_RED";
    return "UNKNOWN";
}

void test_basic_cycle(void) {
    TrafficLight tl;
    int i;

    printf("=== Basic Cycle ===\n");
    // EXPECT: === Basic Cycle ===

    tl_init(&tl);
    init_timings();

    printf("initial: %s\n", state_name(tl.state));
    // EXPECT: initial: NS_GRN

    /* 5 ticks with no EW sensor: stays NS_GREEN */
    for (i = 0; i < 5; i++) {
        tl_step(&tl, 0);
    }
    printf("after 5 ticks: %s timer=%d\n", state_name(tl.state), tl.timer);
    // EXPECT: after 5 ticks: NS_GRN timer=5

    /* Now EW sensor triggers: timer=6 >= 5 and input==2 */
    tl_step(&tl, 2);
    printf("ew sensor: %s\n", state_name(tl.state));
    // EXPECT: ew sensor: NS_YLW

    /* 2 ticks of yellow */
    tl_step(&tl, 0);
    tl_step(&tl, 0);
    printf("after yellow: %s\n", state_name(tl.state));
    // EXPECT: after yellow: ALL_RED

    /* 1 tick of all-red. ns_green_count=6 > ew_green_count=0 -> EW_GRN */
    tl_step(&tl, 0);
    printf("after all-red: %s\n", state_name(tl.state));
    // EXPECT: after all-red: EW_GRN
}

void test_ew_to_ns(void) {
    TrafficLight tl;
    int i;

    printf("=== EW to NS ===\n");
    // EXPECT: === EW to NS ===

    tl_init(&tl);
    init_timings();

    /* Force into EW_GREEN: NS for 6 steps, yellow 2, all_red 1 */
    for (i = 0; i < 5; i++) tl_step(&tl, 0);
    tl_step(&tl, 2);  /* -> NS_YLW (ns_green_count=6) */
    tl_step(&tl, 0); tl_step(&tl, 0); /* -> ALL_RED */
    tl_step(&tl, 0);  /* -> EW_GRN (6 > 0) */

    printf("now in: %s\n", state_name(tl.state));
    // EXPECT: now in: EW_GRN

    /* EW_GREEN for 5 ticks, then NS sensor on 6th */
    for (i = 0; i < 5; i++) tl_step(&tl, 0);
    tl_step(&tl, 1); /* NS sensor -> EW_YELLOW (ew_green_count=6) */
    printf("ns sensor: %s\n", state_name(tl.state));
    // EXPECT: ns sensor: EW_YLW

    tl_step(&tl, 0); tl_step(&tl, 0); /* -> ALL_RED */
    /* ns_green_count=6, ew_green_count=6, not >, so NS_GRN */
    tl_step(&tl, 0);
    printf("back to: %s\n", state_name(tl.state));
    // EXPECT: back to: NS_GRN
}

void test_emergency(void) {
    TrafficLight tl;
    int i;

    printf("=== Emergency ===\n");
    // EXPECT: === Emergency ===

    tl_init(&tl);
    init_timings();

    /* 3 ticks of NS_GREEN (ns_green_count=3), then emergency */
    for (i = 0; i < 3; i++) tl_step(&tl, 0);
    tl_step(&tl, 3); /* emergency -> ALL_RED */
    printf("emergency: %s emg=%d\n", state_name(tl.state), tl.emergency_count);
    // EXPECT: emergency: ALL_RED emg=1

    /* After all-red: ns_green_count=3 > ew_green_count=0 -> EW_GRN */
    tl_step(&tl, 0);
    printf("after emg: %s\n", state_name(tl.state));
    // EXPECT: after emg: EW_GRN
}

void test_counters(void) {
    TrafficLight tl;
    int i;

    printf("=== Counters ===\n");
    // EXPECT: === Counters ===

    tl_init(&tl);
    init_timings();

    /* NS phase: 5 ticks + 1 sensor tick = 6 ns_green increments */
    for (i = 0; i < 5; i++) tl_step(&tl, 0);
    tl_step(&tl, 2);  /* -> NS_YLW, ns_green_count=6 */
    tl_step(&tl, 0); tl_step(&tl, 0); /* -> ALL_RED */
    tl_step(&tl, 0); /* -> EW_GRN (6>0), cycle_count=1 */

    /* EW phase: 5 ticks + 1 sensor tick = 6 ew_green increments */
    for (i = 0; i < 5; i++) tl_step(&tl, 0);
    tl_step(&tl, 1);  /* -> EW_YLW, ew_green_count=6 */
    tl_step(&tl, 0); tl_step(&tl, 0); /* -> ALL_RED */
    tl_step(&tl, 0); /* -> NS_GRN (6 not > 6), cycle_count=2 */

    printf("ns_green=%d ew_green=%d cycles=%d\n",
           tl.ns_green_count, tl.ew_green_count, tl.cycle_count);
    // EXPECT: ns_green=6 ew_green=6 cycles=2
}

void test_sensor_timing(void) {
    TrafficLight tl;
    int i;

    printf("=== Sensor Timing ===\n");
    // EXPECT: === Sensor Timing ===

    tl_init(&tl);
    init_timings();

    /* EW sensor before minimum time should be ignored */
    for (i = 0; i < 3; i++) tl_step(&tl, 0);  /* 3 ticks < 5 min */
    tl_step(&tl, 2);  /* EW sensor, but timer=4 < 5, still NS_GRN */
    printf("early sensor: %s timer=%d\n", state_name(tl.state), tl.timer);
    // EXPECT: early sensor: NS_GRN timer=4

    /* One more tick, then EW sensor triggers */
    tl_step(&tl, 0);  /* timer=5 >= 5 but input=TICK */
    tl_step(&tl, 2);  /* timer=6 >= 5 and input=EW_SENSOR */
    printf("late sensor: %s\n", state_name(tl.state));
    // EXPECT: late sensor: NS_YLW

    printf("ns ticks: %d\n", tl.ns_green_count);
    // EXPECT: ns ticks: 6
}

int main(void) {
    test_basic_cycle();
    test_ew_to_ns();
    test_emergency();
    test_counters();
    test_sensor_timing();

    printf("Traffic light done.\n");
    // EXPECT: Traffic light done.
    return 0;
}
