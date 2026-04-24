int printf(const char *fmt, ...);

// Watchdog Timer Simulation
// Software watchdog with configurable timeout, window mode, early warning

enum WdtState { WDT_DISABLED, WDT_RUNNING, WDT_EXPIRED, WDT_RESET };
enum WdtMode { WDT_NORMAL, WDT_WINDOW };

struct WatchdogTimer {
    int counter;
    int timeout;
    int state;
    int mode;
    int window_start;   // for window mode: earliest allowed kick
    int window_end;     // for window mode: latest allowed kick (= timeout)
    int kick_count;
    int expire_count;
    int early_warn_threshold;
    int early_warn_flag;
    int reset_count;
    int ticks_total;
};

void wdt_init(struct WatchdogTimer *wdt, int timeout) {
    wdt->counter = 0;
    wdt->timeout = timeout;
    wdt->state = WDT_DISABLED;
    wdt->mode = WDT_NORMAL;
    wdt->window_start = 0;
    wdt->window_end = timeout;
    wdt->kick_count = 0;
    wdt->expire_count = 0;
    wdt->early_warn_threshold = timeout / 2;
    wdt->early_warn_flag = 0;
    wdt->reset_count = 0;
    wdt->ticks_total = 0;
}

void wdt_start(struct WatchdogTimer *wdt) {
    wdt->state = WDT_RUNNING;
    wdt->counter = 0;
}

void wdt_stop(struct WatchdogTimer *wdt) {
    wdt->state = WDT_DISABLED;
}

void wdt_set_window(struct WatchdogTimer *wdt, int start, int end) {
    wdt->mode = WDT_WINDOW;
    wdt->window_start = start;
    wdt->window_end = end;
    wdt->timeout = end;
}

void wdt_set_early_warning(struct WatchdogTimer *wdt, int threshold) {
    wdt->early_warn_threshold = threshold;
}

// Kick (feed/pet) the watchdog
int wdt_kick(struct WatchdogTimer *wdt) {
    if (wdt->state != WDT_RUNNING) return -1;

    if (wdt->mode == WDT_WINDOW) {
        // In window mode, kicking too early is also an error
        if (wdt->counter < wdt->window_start) {
            // Too early! Treat as violation
            wdt->state = WDT_EXPIRED;
            wdt->expire_count = wdt->expire_count + 1;
            return -2;
        }
    }

    wdt->counter = 0;
    wdt->early_warn_flag = 0;
    wdt->kick_count = wdt->kick_count + 1;
    return 0;
}

// Tick the watchdog one time unit
int wdt_tick(struct WatchdogTimer *wdt) {
    if (wdt->state != WDT_RUNNING) return 0;

    wdt->counter = wdt->counter + 1;
    wdt->ticks_total = wdt->ticks_total + 1;

    // Check early warning
    if (wdt->counter >= wdt->early_warn_threshold && !wdt->early_warn_flag) {
        wdt->early_warn_flag = 1;
    }

    // Check expiry
    if (wdt->counter >= wdt->timeout) {
        wdt->state = WDT_EXPIRED;
        wdt->expire_count = wdt->expire_count + 1;
        return 1;  // expired
    }

    return 0;
}

// Advance multiple ticks
int wdt_advance(struct WatchdogTimer *wdt, int ticks) {
    int i;
    int expired = 0;
    for (i = 0; i < ticks; i++) {
        if (wdt_tick(wdt)) {
            expired = 1;
        }
    }
    return expired;
}

// Simulate system reset from watchdog
void wdt_reset_system(struct WatchdogTimer *wdt) {
    wdt->reset_count = wdt->reset_count + 1;
    wdt->counter = 0;
    wdt->state = WDT_RUNNING;
    wdt->early_warn_flag = 0;
}

int wdt_time_remaining(struct WatchdogTimer *wdt) {
    if (wdt->state != WDT_RUNNING) return 0;
    return wdt->timeout - wdt->counter;
}

int wdt_percent_elapsed(struct WatchdogTimer *wdt) {
    if (wdt->timeout == 0) return 0;
    return (wdt->counter * 100) / wdt->timeout;
}

// Multi-watchdog system
struct WdtSystem {
    struct WatchdogTimer watchdogs[4];
    int count;
    int system_ok;
};

void wdt_system_init(struct WdtSystem *sys) {
    sys->count = 0;
    sys->system_ok = 1;
}

int wdt_system_add(struct WdtSystem *sys, int timeout) {
    if (sys->count >= 4) return -1;
    wdt_init(&(sys->watchdogs[sys->count]), timeout);
    sys->count = sys->count + 1;
    return sys->count - 1;
}

void wdt_system_start_all(struct WdtSystem *sys) {
    int i;
    for (i = 0; i < sys->count; i++) {
        wdt_start(&(sys->watchdogs[i]));
    }
}

int wdt_system_tick_all(struct WdtSystem *sys) {
    int i;
    int any_expired = 0;
    for (i = 0; i < sys->count; i++) {
        if (wdt_tick(&(sys->watchdogs[i]))) {
            any_expired = 1;
            sys->system_ok = 0;
        }
    }
    return any_expired;
}

int wdt_system_check(struct WdtSystem *sys) {
    int i;
    int warnings = 0;
    for (i = 0; i < sys->count; i++) {
        if (sys->watchdogs[i].early_warn_flag) {
            warnings = warnings + 1;
        }
    }
    return warnings;
}

int main() {
    struct WatchdogTimer wdt;
    struct WdtSystem sys;
    int result;
    int i;

    // Basic watchdog test
    wdt_init(&wdt, 100);
    printf("WDT initialized, timeout=%d\n", wdt.timeout);
    // EXPECT: WDT initialized, timeout=100

    wdt_start(&wdt);
    printf("WDT state: %d\n", wdt.state);
    // EXPECT: WDT state: 1

    // Tick 50 times
    wdt_advance(&wdt, 50);
    printf("Counter: %d, remaining: %d\n", wdt.counter, wdt_time_remaining(&wdt));
    // EXPECT: Counter: 50, remaining: 50

    printf("Percent elapsed: %d\n", wdt_percent_elapsed(&wdt));
    // EXPECT: Percent elapsed: 50

    // Check early warning (threshold = 50)
    printf("Early warning: %d\n", wdt.early_warn_flag);
    // EXPECT: Early warning: 1

    // Kick the watchdog
    result = wdt_kick(&wdt);
    printf("Kick result: %d\n", result);
    // EXPECT: Kick result: 0
    printf("Counter after kick: %d\n", wdt.counter);
    // EXPECT: Counter after kick: 0
    printf("Kick count: %d\n", wdt.kick_count);
    // EXPECT: Kick count: 1

    // Let it expire
    result = wdt_advance(&wdt, 100);
    printf("Expired: %d\n", result);
    // EXPECT: Expired: 1
    printf("State: %d\n", wdt.state);
    // EXPECT: State: 2
    printf("Expire count: %d\n", wdt.expire_count);
    // EXPECT: Expire count: 1

    // System reset
    wdt_reset_system(&wdt);
    printf("After reset, state: %d, counter: %d\n", wdt.state, wdt.counter);
    // EXPECT: After reset, state: 1, counter: 0
    printf("Reset count: %d\n", wdt.reset_count);
    // EXPECT: Reset count: 1

    // Window mode test
    wdt_init(&wdt, 100);
    wdt_set_window(&wdt, 30, 80);
    wdt_start(&wdt);
    printf("Window mode: start=%d end=%d\n", wdt.window_start, wdt.window_end);
    // EXPECT: Window mode: start=30 end=80

    // Kick too early (before window opens at 30)
    wdt_advance(&wdt, 10);
    result = wdt_kick(&wdt);
    printf("Early kick result: %d\n", result);
    // EXPECT: Early kick result: -2
    printf("State after early kick: %d\n", wdt.state);
    // EXPECT: State after early kick: 2

    // Reset and try proper window kick
    wdt_reset_system(&wdt);
    wdt_advance(&wdt, 40);  // within window [30, 80)
    result = wdt_kick(&wdt);
    printf("Window kick at 40: %d\n", result);
    // EXPECT: Window kick at 40: 0

    // Test multiple watchdog system
    wdt_system_init(&sys);
    wdt_system_add(&sys, 50);   // fast watchdog
    wdt_system_add(&sys, 100);  // slow watchdog
    wdt_system_add(&sys, 200);  // very slow watchdog
    printf("System watchdogs: %d\n", sys.count);
    // EXPECT: System watchdogs: 3

    wdt_system_start_all(&sys);

    // Advance 25 ticks
    for (i = 0; i < 25; i++) {
        wdt_system_tick_all(&sys);
    }
    int warnings = wdt_system_check(&sys);
    printf("Warnings at tick 25: %d\n", warnings);
    // EXPECT: Warnings at tick 25: 1

    // Kick the fast one, let slow ones run
    wdt_kick(&(sys.watchdogs[0]));

    // Advance to tick 50
    for (i = 0; i < 25; i++) {
        wdt_system_tick_all(&sys);
    }
    printf("System OK at tick 50: %d\n", sys.system_ok);
    // EXPECT: System OK at tick 50: 1

    warnings = wdt_system_check(&sys);
    printf("Warnings at tick 50: %d\n", warnings);
    // EXPECT: Warnings at tick 50: 2

    // Let fast watchdog expire (it was kicked at 25, now at 50 more)
    for (i = 0; i < 25; i++) {
        wdt_system_tick_all(&sys);
    }
    printf("System OK at tick 75: %d\n", sys.system_ok);
    // EXPECT: System OK at tick 75: 0

    printf("Fast WDT expires: %d\n", sys.watchdogs[0].expire_count);
    // EXPECT: Fast WDT expires: 1

    printf("Total ticks: %d\n", wdt.ticks_total);
    // EXPECT: Total ticks: 50

    printf("Watchdog test complete\n");
    // EXPECT: Watchdog test complete

    return 0;
}
