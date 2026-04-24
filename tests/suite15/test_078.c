int printf(const char *fmt, ...);

// Watchdog timer with reset simulation

enum wdt_mode {
    WDT_DISABLED,
    WDT_RESET_MODE,
    WDT_INTERRUPT_MODE,
    WDT_INTERRUPT_THEN_RESET
};

enum system_state {
    SYS_BOOT,
    SYS_INIT,
    SYS_RUNNING,
    SYS_FAULT,
    SYS_RESET
};

struct watchdog {
    int timeout;
    int counter;
    int mode;
    int enabled;
    int reset_count;
    int interrupt_flag;
    int window_min;
    int window_max;
    int window_enabled;
};

struct system_monitor {
    int state;
    int uptime;
    int fault_code;
    int boot_count;
    int last_reset_reason;
    int task_alive[4];
    int task_timeout[4];
    int task_counter[4];
};

void wdt_init(struct watchdog *w, int timeout, int mode) {
    w->timeout = timeout;
    w->counter = 0;
    w->mode = mode;
    w->enabled = 0;
    w->reset_count = 0;
    w->interrupt_flag = 0;
    w->window_min = 0;
    w->window_max = timeout;
    w->window_enabled = 0;
}

void wdt_enable(struct watchdog *w) {
    w->enabled = 1;
    w->counter = 0;
}

void wdt_disable(struct watchdog *w) {
    w->enabled = 0;
}

void wdt_set_window(struct watchdog *w, int min, int max) {
    w->window_min = min;
    w->window_max = max;
    w->window_enabled = 1;
}

int wdt_kick(struct watchdog *w) {
    if (!w->enabled) return -1;

    if (w->window_enabled) {
        if (w->counter < w->window_min) {
            return -2;
        }
        if (w->counter > w->window_max) {
            return -3;
        }
    }

    w->counter = 0;
    return 0;
}

int wdt_tick(struct watchdog *w) {
    if (!w->enabled) return 0;
    w->counter++;

    if (w->counter >= w->timeout) {
        if (w->mode == WDT_RESET_MODE) {
            w->reset_count++;
            w->counter = 0;
            return 2;
        } else if (w->mode == WDT_INTERRUPT_MODE) {
            w->interrupt_flag = 1;
            w->counter = 0;
            return 1;
        } else if (w->mode == WDT_INTERRUPT_THEN_RESET) {
            if (w->interrupt_flag) {
                w->reset_count++;
                w->interrupt_flag = 0;
                w->counter = 0;
                return 2;
            } else {
                w->interrupt_flag = 1;
                w->counter = 0;
                return 1;
            }
        }
    }
    return 0;
}

void sysmon_init(struct system_monitor *mon) {
    int i;
    mon->state = SYS_BOOT;
    mon->uptime = 0;
    mon->fault_code = 0;
    mon->boot_count = 1;
    mon->last_reset_reason = 0;
    for (i = 0; i < 4; i++) {
        mon->task_alive[i] = 0;
        mon->task_timeout[i] = 100;
        mon->task_counter[i] = 0;
    }
}

void sysmon_boot(struct system_monitor *mon) {
    mon->state = SYS_INIT;
    mon->uptime = 0;
}

void sysmon_start(struct system_monitor *mon) {
    mon->state = SYS_RUNNING;
}

void sysmon_task_checkin(struct system_monitor *mon, int task_id) {
    if (task_id >= 0 && task_id < 4) {
        mon->task_alive[task_id] = 1;
        mon->task_counter[task_id] = 0;
    }
}

int sysmon_tick(struct system_monitor *mon) {
    int i;
    int fault = 0;

    if (mon->state != SYS_RUNNING) return 0;

    mon->uptime++;

    for (i = 0; i < 4; i++) {
        if (mon->task_alive[i]) {
            mon->task_counter[i]++;
            if (mon->task_counter[i] >= mon->task_timeout[i]) {
                mon->fault_code = i + 1;
                mon->state = SYS_FAULT;
                fault = 1;
            }
        }
    }
    return fault;
}

void sysmon_reset(struct system_monitor *mon, int reason) {
    mon->state = SYS_RESET;
    mon->last_reset_reason = reason;
    mon->boot_count++;
    mon->fault_code = 0;
    int i;
    for (i = 0; i < 4; i++) {
        mon->task_alive[i] = 0;
        mon->task_counter[i] = 0;
    }
}

int simulate_system(struct watchdog *w, struct system_monitor *mon, int ticks, int kick_interval) {
    int i;
    int resets = 0;

    for (i = 0; i < ticks; i++) {
        if (kick_interval > 0 && (i % kick_interval) == 0) {
            wdt_kick(w);
        }
        int wdt_result = wdt_tick(w);
        if (wdt_result == 2) {
            sysmon_reset(mon, 1);
            sysmon_boot(mon);
            sysmon_start(mon);
            resets++;
        }
        sysmon_tick(mon);
    }
    return resets;
}

int main(void) {
    struct watchdog wdt;
    struct system_monitor mon;
    int result;
    int i;

    wdt_init(&wdt, 10, WDT_RESET_MODE);
    printf("%d\n", wdt.timeout);
    // EXPECT: 10
    printf("%d\n", wdt.enabled);
    // EXPECT: 0
    printf("%d\n", wdt.reset_count);
    // EXPECT: 0

    wdt_enable(&wdt);
    printf("%d\n", wdt.enabled);
    // EXPECT: 1

    for (i = 0; i < 5; i++) {
        wdt_tick(&wdt);
    }
    printf("%d\n", wdt.counter);
    // EXPECT: 5

    result = wdt_kick(&wdt);
    printf("%d\n", result);
    // EXPECT: 0
    printf("%d\n", wdt.counter);
    // EXPECT: 0

    for (i = 0; i < 10; i++) {
        result = wdt_tick(&wdt);
    }
    printf("%d\n", result);
    // EXPECT: 2
    printf("%d\n", wdt.reset_count);
    // EXPECT: 1

    struct watchdog wdt_int;
    wdt_init(&wdt_int, 5, WDT_INTERRUPT_MODE);
    wdt_enable(&wdt_int);
    for (i = 0; i < 5; i++) {
        result = wdt_tick(&wdt_int);
    }
    printf("%d\n", result);
    // EXPECT: 1
    printf("%d\n", wdt_int.interrupt_flag);
    // EXPECT: 1

    struct watchdog wdt_ir;
    wdt_init(&wdt_ir, 5, WDT_INTERRUPT_THEN_RESET);
    wdt_enable(&wdt_ir);
    for (i = 0; i < 5; i++) {
        wdt_tick(&wdt_ir);
    }
    printf("%d\n", wdt_ir.interrupt_flag);
    // EXPECT: 1
    for (i = 0; i < 5; i++) {
        result = wdt_tick(&wdt_ir);
    }
    printf("%d\n", result);
    // EXPECT: 2
    printf("%d\n", wdt_ir.reset_count);
    // EXPECT: 1

    struct watchdog wdt_win;
    wdt_init(&wdt_win, 20, WDT_RESET_MODE);
    wdt_set_window(&wdt_win, 5, 15);
    wdt_enable(&wdt_win);
    result = wdt_kick(&wdt_win);
    printf("%d\n", result);
    // EXPECT: -2

    for (i = 0; i < 7; i++) {
        wdt_tick(&wdt_win);
    }
    result = wdt_kick(&wdt_win);
    printf("%d\n", result);
    // EXPECT: 0

    sysmon_init(&mon);
    printf("%d\n", mon.state);
    // EXPECT: 0
    printf("%d\n", mon.boot_count);
    // EXPECT: 1

    sysmon_boot(&mon);
    sysmon_start(&mon);
    printf("%d\n", mon.state);
    // EXPECT: 2

    sysmon_task_checkin(&mon, 0);
    sysmon_task_checkin(&mon, 1);
    printf("%d\n", mon.task_alive[0]);
    // EXPECT: 1
    printf("%d\n", mon.task_alive[1]);
    // EXPECT: 1

    for (i = 0; i < 50; i++) {
        if (i % 20 == 0) {
            sysmon_task_checkin(&mon, 0);
            sysmon_task_checkin(&mon, 1);
        }
        sysmon_tick(&mon);
    }
    printf("%d\n", mon.state);
    // EXPECT: 2
    printf("%d\n", mon.uptime);
    // EXPECT: 50

    struct watchdog sim_wdt;
    struct system_monitor sim_mon;
    wdt_init(&sim_wdt, 10, WDT_RESET_MODE);
    wdt_enable(&sim_wdt);
    sysmon_init(&sim_mon);
    sysmon_boot(&sim_mon);
    sysmon_start(&sim_mon);

    int resets = simulate_system(&sim_wdt, &sim_mon, 50, 5);
    printf("%d\n", resets);
    // EXPECT: 0

    wdt_init(&sim_wdt, 10, WDT_RESET_MODE);
    wdt_enable(&sim_wdt);
    sysmon_init(&sim_mon);
    sysmon_boot(&sim_mon);
    sysmon_start(&sim_mon);
    resets = simulate_system(&sim_wdt, &sim_mon, 50, 0);
    printf("%d\n", resets);
    // EXPECT: 5

    printf("%d\n", sim_mon.boot_count);
    // EXPECT: 6

    printf("%s\n", "watchdog_done");
    // EXPECT: watchdog_done

    return 0;
}
