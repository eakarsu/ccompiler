int printf(const char *fmt, ...);

// PWM generation and duty cycle simulation

enum pwm_align {
    PWM_EDGE_ALIGNED,
    PWM_CENTER_ALIGNED
};

enum pwm_polarity {
    PWM_ACTIVE_HIGH,
    PWM_ACTIVE_LOW
};

struct pwm_channel {
    int period;
    int compare;
    int polarity;
    int alignment;
    int enabled;
    int counter;
    int output;
    int direction;
};

struct pwm_timer {
    struct pwm_channel channels[4];
    int prescaler;
    int clock_freq;
    int active_channels;
    int total_cycles;
};

void pwm_init_channel(struct pwm_channel *ch, int period, int compare, int pol, int align) {
    ch->period = period;
    ch->compare = compare;
    ch->polarity = pol;
    ch->alignment = align;
    ch->enabled = 0;
    ch->counter = 0;
    ch->output = 0;
    ch->direction = 1;
}

void pwm_enable(struct pwm_channel *ch) {
    ch->enabled = 1;
}

void pwm_disable(struct pwm_channel *ch) {
    ch->enabled = 0;
    ch->output = 0;
}

void pwm_set_duty(struct pwm_channel *ch, int compare) {
    if (compare < 0) compare = 0;
    if (compare > ch->period) compare = ch->period;
    ch->compare = compare;
}

int pwm_get_duty_percent(struct pwm_channel *ch) {
    if (ch->period == 0) return 0;
    return (ch->compare * 100) / ch->period;
}

int pwm_step_edge(struct pwm_channel *ch) {
    int new_output;
    if (!ch->enabled) return ch->output;

    ch->counter++;
    if (ch->counter >= ch->period) {
        ch->counter = 0;
    }

    if (ch->counter < ch->compare) {
        new_output = (ch->polarity == PWM_ACTIVE_HIGH) ? 1 : 0;
    } else {
        new_output = (ch->polarity == PWM_ACTIVE_HIGH) ? 0 : 1;
    }
    ch->output = new_output;
    return new_output;
}

int pwm_step_center(struct pwm_channel *ch) {
    int new_output;
    int half_comp;
    if (!ch->enabled) return ch->output;

    ch->counter = ch->counter + ch->direction;
    if (ch->counter >= ch->period) {
        ch->direction = -1;
        ch->counter = ch->period;
    } else if (ch->counter <= 0) {
        ch->direction = 1;
        ch->counter = 0;
    }

    half_comp = ch->compare / 2;
    int center = ch->period / 2;
    int low_thresh = center - half_comp;
    int high_thresh = center + half_comp;

    if (ch->counter >= low_thresh && ch->counter <= high_thresh) {
        new_output = (ch->polarity == PWM_ACTIVE_HIGH) ? 1 : 0;
    } else {
        new_output = (ch->polarity == PWM_ACTIVE_HIGH) ? 0 : 1;
    }
    ch->output = new_output;
    return new_output;
}

int pwm_step(struct pwm_channel *ch) {
    if (ch->alignment == PWM_EDGE_ALIGNED) {
        return pwm_step_edge(ch);
    } else {
        return pwm_step_center(ch);
    }
}

void pwm_timer_init(struct pwm_timer *t, int prescaler, int clock_freq) {
    int i;
    t->prescaler = prescaler;
    t->clock_freq = clock_freq;
    t->active_channels = 0;
    t->total_cycles = 0;
    for (i = 0; i < 4; i++) {
        pwm_init_channel(&t->channels[i], 100, 0, PWM_ACTIVE_HIGH, PWM_EDGE_ALIGNED);
    }
}

int pwm_get_frequency(struct pwm_timer *t, int ch_idx) {
    int period = t->channels[ch_idx].period;
    if (period == 0 || t->prescaler == 0) return 0;
    return t->clock_freq / (t->prescaler * period);
}

int pwm_simulate_high_count(struct pwm_channel *ch, int steps) {
    int i;
    int high_count = 0;
    int saved_counter = ch->counter;
    int saved_dir = ch->direction;

    for (i = 0; i < steps; i++) {
        int out = pwm_step(ch);
        if (out) high_count++;
    }

    ch->counter = saved_counter;
    ch->direction = saved_dir;
    return high_count;
}

int pwm_dead_time_check(int ch1_out, int ch2_out) {
    if (ch1_out && ch2_out) return 1;
    return 0;
}

int main(void) {
    struct pwm_channel ch;
    struct pwm_timer timer;
    int i;
    int out;

    pwm_init_channel(&ch, 10, 5, PWM_ACTIVE_HIGH, PWM_EDGE_ALIGNED);
    printf("%d\n", ch.period);
    // EXPECT: 10
    printf("%d\n", ch.compare);
    // EXPECT: 5
    printf("%d\n", ch.enabled);
    // EXPECT: 0

    pwm_enable(&ch);
    printf("%d\n", ch.enabled);
    // EXPECT: 1

    printf("%d\n", pwm_get_duty_percent(&ch));
    // EXPECT: 50

    int high_count = 0;
    for (i = 0; i < 10; i++) {
        out = pwm_step(&ch);
        if (out) high_count++;
    }
    printf("%d\n", high_count);
    // EXPECT: 5

    pwm_set_duty(&ch, 3);
    printf("%d\n", pwm_get_duty_percent(&ch));
    // EXPECT: 30

    ch.counter = 0;
    high_count = 0;
    for (i = 0; i < 10; i++) {
        out = pwm_step(&ch);
        if (out) high_count++;
    }
    printf("%d\n", high_count);
    // EXPECT: 3

    struct pwm_channel inv_ch;
    pwm_init_channel(&inv_ch, 10, 7, PWM_ACTIVE_LOW, PWM_EDGE_ALIGNED);
    pwm_enable(&inv_ch);
    high_count = 0;
    for (i = 0; i < 10; i++) {
        out = pwm_step(&inv_ch);
        if (out) high_count++;
    }
    printf("%d\n", high_count);
    // EXPECT: 3

    struct pwm_channel cen_ch;
    pwm_init_channel(&cen_ch, 20, 10, PWM_ACTIVE_HIGH, PWM_CENTER_ALIGNED);
    pwm_enable(&cen_ch);

    out = pwm_step(&cen_ch);
    printf("%d\n", out);
    // EXPECT: 0

    int center_high = 0;
    cen_ch.counter = 0;
    cen_ch.direction = 1;
    for (i = 0; i < 40; i++) {
        out = pwm_step(&cen_ch);
        if (out) center_high++;
    }
    printf("%d\n", center_high);
    // EXPECT: 22

    pwm_timer_init(&timer, 8, 16000000);
    printf("%d\n", timer.prescaler);
    // EXPECT: 8
    printf("%d\n", timer.clock_freq);
    // EXPECT: 16000000

    pwm_init_channel(&timer.channels[0], 1000, 500, PWM_ACTIVE_HIGH, PWM_EDGE_ALIGNED);
    pwm_enable(&timer.channels[0]);
    int freq = pwm_get_frequency(&timer, 0);
    printf("%d\n", freq);
    // EXPECT: 2000

    pwm_init_channel(&timer.channels[1], 200, 100, PWM_ACTIVE_HIGH, PWM_EDGE_ALIGNED);
    pwm_enable(&timer.channels[1]);
    freq = pwm_get_frequency(&timer, 1);
    printf("%d\n", freq);
    // EXPECT: 10000

    pwm_set_duty(&ch, 0);
    printf("%d\n", pwm_get_duty_percent(&ch));
    // EXPECT: 0

    pwm_set_duty(&ch, 10);
    printf("%d\n", pwm_get_duty_percent(&ch));
    // EXPECT: 100

    pwm_set_duty(&ch, 15);
    printf("%d\n", pwm_get_duty_percent(&ch));
    // EXPECT: 100

    int conflict = pwm_dead_time_check(1, 1);
    printf("%d\n", conflict);
    // EXPECT: 1
    conflict = pwm_dead_time_check(1, 0);
    printf("%d\n", conflict);
    // EXPECT: 0

    printf("%s\n", "pwm_done");
    // EXPECT: pwm_done

    return 0;
}
