int printf(const char *fmt, ...);

// Motor Controller Simulation
// PWM-based DC motor control with speed ramping and current limiting

struct Motor {
    int target_speed;    // target RPM
    int current_speed;   // current RPM
    int pwm_duty;        // 0-1000 (0.0-100.0%)
    int current_draw;    // milliamps
    int max_current;     // max milliamps
    int acceleration;    // RPM per tick
    int enabled;
    int direction;       // 1=forward, -1=reverse
    int stall_count;
    int total_ticks;
};

void motor_init(struct Motor *m, int max_current, int accel) {
    m->target_speed = 0;
    m->current_speed = 0;
    m->pwm_duty = 0;
    m->current_draw = 0;
    m->max_current = max_current;
    m->acceleration = accel;
    m->enabled = 0;
    m->direction = 1;
    m->stall_count = 0;
    m->total_ticks = 0;
}

void motor_enable(struct Motor *m) {
    m->enabled = 1;
}

void motor_disable(struct Motor *m) {
    m->enabled = 0;
    m->pwm_duty = 0;
}

void motor_set_target(struct Motor *m, int rpm) {
    if (rpm < 0) {
        m->direction = -1;
        m->target_speed = -rpm;
    } else {
        m->direction = 1;
        m->target_speed = rpm;
    }
}

int motor_compute_pwm(struct Motor *m) {
    int error;
    int pwm;

    error = m->target_speed - m->current_speed;
    pwm = m->pwm_duty + (error * 10) / 100;

    if (pwm < 0) pwm = 0;
    if (pwm > 1000) pwm = 1000;

    return pwm;
}

int motor_estimate_current(int pwm, int speed) {
    // Current = base + load proportional to PWM
    int base;
    int load;
    base = 50;  // 50mA idle
    load = (pwm * 20) / 100;  // up to 200mA at full PWM
    return base + load;
}

void motor_tick(struct Motor *m) {
    int new_pwm;
    int est_current;

    if (!m->enabled) {
        if (m->current_speed > 0) {
            m->current_speed = m->current_speed - m->acceleration * 2;
            if (m->current_speed < 0) m->current_speed = 0;
        }
        return;
    }

    m->total_ticks = m->total_ticks + 1;

    new_pwm = motor_compute_pwm(m);
    est_current = motor_estimate_current(new_pwm, m->current_speed);

    // Current limiting
    if (est_current > m->max_current) {
        new_pwm = ((m->max_current - 50) * 100) / 20;
        if (new_pwm < 0) new_pwm = 0;
        est_current = motor_estimate_current(new_pwm, m->current_speed);
    }

    m->pwm_duty = new_pwm;
    m->current_draw = est_current;

    // Speed response
    if (m->current_speed < m->target_speed) {
        m->current_speed = m->current_speed + m->acceleration;
        if (m->current_speed > m->target_speed) {
            m->current_speed = m->target_speed;
        }
    } else if (m->current_speed > m->target_speed) {
        m->current_speed = m->current_speed - m->acceleration;
        if (m->current_speed < m->target_speed) {
            m->current_speed = m->target_speed;
        }
    }

    // Stall detection
    if (m->pwm_duty > 500 && m->current_speed < 10) {
        m->stall_count = m->stall_count + 1;
    } else {
        m->stall_count = 0;
    }
}

int motor_is_stalled(struct Motor *m) {
    return m->stall_count > 5;
}

int motor_actual_speed(struct Motor *m) {
    return m->current_speed * m->direction;
}

int main() {
    struct Motor mot;
    int i;

    printf("=== Motor Controller Test ===\n");
    // EXPECT: === Motor Controller Test ===

    // Test 1: Init and enable
    motor_init(&mot, 300, 50);
    printf("Init: speed=%d, pwm=%d, enabled=%d\n", mot.current_speed, mot.pwm_duty, mot.enabled);
    // EXPECT: Init: speed=0, pwm=0, enabled=0

    motor_enable(&mot);
    printf("Enabled: %d\n", mot.enabled);
    // EXPECT: Enabled: 1

    // Test 2: Speed ramp up
    motor_set_target(&mot, 500);
    printf("Target: %d, dir=%d\n", mot.target_speed, mot.direction);
    // EXPECT: Target: 500, dir=1

    for (i = 0; i < 5; i = i + 1) {
        motor_tick(&mot);
    }
    printf("After 5 ticks: speed=%d, pwm=%d, current=%d\n", mot.current_speed, mot.pwm_duty, mot.current_draw);
    // EXPECT: After 5 ticks: speed=250, pwm=200, current=90

    for (i = 0; i < 5; i = i + 1) {
        motor_tick(&mot);
    }
    printf("After 10 ticks: speed=%d\n", mot.current_speed);
    // EXPECT: After 10 ticks: speed=500

    // Test 3: Direction change
    motor_init(&mot, 300, 50);
    motor_enable(&mot);
    motor_set_target(&mot, -200);
    printf("Reverse: target=%d, dir=%d\n", mot.target_speed, mot.direction);
    // EXPECT: Reverse: target=200, dir=-1

    for (i = 0; i < 4; i = i + 1) {
        motor_tick(&mot);
    }
    printf("Reverse speed: %d (actual: %d)\n", mot.current_speed, motor_actual_speed(&mot));
    // EXPECT: Reverse speed: 200 (actual: -200)

    // Test 4: Disable and coast down
    motor_init(&mot, 300, 50);
    motor_enable(&mot);
    motor_set_target(&mot, 300);
    for (i = 0; i < 6; i = i + 1) {
        motor_tick(&mot);
    }
    printf("Before disable: speed=%d\n", mot.current_speed);
    // EXPECT: Before disable: speed=300

    motor_disable(&mot);
    printf("Disabled: enabled=%d, pwm=%d\n", mot.enabled, mot.pwm_duty);
    // EXPECT: Disabled: enabled=0, pwm=0

    motor_tick(&mot);
    motor_tick(&mot);
    printf("Coasting: speed=%d\n", mot.current_speed);
    // EXPECT: Coasting: speed=100

    motor_tick(&mot);
    printf("Stopped: speed=%d\n", mot.current_speed);
    // EXPECT: Stopped: speed=0

    // Test 5: Current limiting
    motor_init(&mot, 150, 100);
    motor_enable(&mot);
    motor_set_target(&mot, 1000);
    for (i = 0; i < 3; i = i + 1) {
        motor_tick(&mot);
    }
    printf("Current limited: current=%d, max=%d\n", mot.current_draw, mot.max_current);
    // EXPECT: Current limited: current=104, max=150

    // Test 6: Tick counter
    motor_init(&mot, 300, 50);
    motor_enable(&mot);
    motor_set_target(&mot, 100);
    for (i = 0; i < 7; i = i + 1) {
        motor_tick(&mot);
    }
    printf("Total ticks: %d\n", mot.total_ticks);
    // EXPECT: Total ticks: 7

    printf("Motor controller done\n");
    // EXPECT: Motor controller done

    return 0;
}
