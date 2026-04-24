int printf(const char *fmt, ...);

// PWM Generator Simulation
// Duty cycle as integer percent, multiple channels

enum PwmState { PWM_OFF, PWM_ON };
enum PwmAlign { PWM_EDGE, PWM_CENTER };

struct PwmChannel {
    int id;
    int duty_percent;   // 0-100
    int period;         // total counts per cycle
    int state;
    int alignment;
    int output;         // current output level
    int counter;
    int cycles_complete;
};

struct PwmController {
    struct PwmChannel channels[4];
    int num_channels;
    int prescaler;
    int base_freq;
};

void pwm_init_channel(struct PwmChannel *ch, int id, int period) {
    ch->id = id;
    ch->duty_percent = 0;
    ch->period = period;
    ch->state = PWM_OFF;
    ch->alignment = PWM_EDGE;
    ch->output = 0;
    ch->counter = 0;
    ch->cycles_complete = 0;
}

void pwm_init_controller(struct PwmController *ctrl, int base_freq, int prescaler) {
    int i;
    ctrl->num_channels = 4;
    ctrl->prescaler = prescaler;
    ctrl->base_freq = base_freq;
    for (i = 0; i < 4; i++) {
        pwm_init_channel(&(ctrl->channels[i]), i, 100);
    }
}

void pwm_set_duty(struct PwmController *ctrl, int ch, int duty) {
    if (ch < 0 || ch >= 4) return;
    if (duty < 0) duty = 0;
    if (duty > 100) duty = 100;
    ctrl->channels[ch].duty_percent = duty;
}

void pwm_set_period(struct PwmController *ctrl, int ch, int period) {
    if (ch < 0 || ch >= 4) return;
    if (period < 1) period = 1;
    ctrl->channels[ch].period = period;
}

void pwm_enable(struct PwmController *ctrl, int ch) {
    if (ch < 0 || ch >= 4) return;
    ctrl->channels[ch].state = PWM_ON;
    ctrl->channels[ch].counter = 0;
}

void pwm_disable(struct PwmController *ctrl, int ch) {
    if (ch < 0 || ch >= 4) return;
    ctrl->channels[ch].state = PWM_OFF;
    ctrl->channels[ch].output = 0;
}

void pwm_tick_channel(struct PwmChannel *ch) {
    int threshold;
    if (ch->state != PWM_ON) return;

    threshold = (ch->duty_percent * ch->period) / 100;

    if (ch->alignment == PWM_EDGE) {
        // Edge-aligned: high for first 'threshold' counts
        if (ch->counter < threshold) {
            ch->output = 1;
        } else {
            ch->output = 0;
        }
    } else {
        // Center-aligned: high in the middle
        int half = ch->period / 2;
        int half_duty = threshold / 2;
        int low = half - half_duty;
        int high = half + half_duty;
        if (ch->counter >= low && ch->counter < high) {
            ch->output = 1;
        } else {
            ch->output = 0;
        }
    }

    ch->counter = ch->counter + 1;
    if (ch->counter >= ch->period) {
        ch->counter = 0;
        ch->cycles_complete = ch->cycles_complete + 1;
    }
}

void pwm_tick_all(struct PwmController *ctrl) {
    int i;
    for (i = 0; i < 4; i++) {
        pwm_tick_channel(&(ctrl->channels[i]));
    }
}

// Measure duty cycle by counting high samples over one period
int pwm_measure_duty(struct PwmController *ctrl, int ch) {
    int high_count = 0;
    int i;
    int period;
    if (ch < 0 || ch >= 4) return -1;
    period = ctrl->channels[ch].period;

    // Reset counter for measurement
    ctrl->channels[ch].counter = 0;

    for (i = 0; i < period; i++) {
        pwm_tick_channel(&(ctrl->channels[ch]));
        if (ctrl->channels[ch].output) {
            high_count = high_count + 1;
        }
    }
    return (high_count * 100) / period;
}

int pwm_get_output_freq(struct PwmController *ctrl, int ch) {
    if (ch < 0 || ch >= 4) return 0;
    int period = ctrl->channels[ch].period;
    if (period == 0) return 0;
    return ctrl->base_freq / (ctrl->prescaler * period);
}

int main() {
    struct PwmController ctrl;
    int duty;
    int i;

    pwm_init_controller(&ctrl, 1000000, 1);
    printf("PWM controller initialized\n");
    // EXPECT: PWM controller initialized

    // Configure channel 0: 50% duty, period 100
    pwm_set_duty(&ctrl, 0, 50);
    pwm_enable(&ctrl, 0);
    printf("Channel 0: duty=%d period=%d\n", ctrl.channels[0].duty_percent, ctrl.channels[0].period);
    // EXPECT: Channel 0: duty=50 period=100

    // Measure duty cycle
    duty = pwm_measure_duty(&ctrl, 0);
    printf("Measured duty ch0: %d\n", duty);
    // EXPECT: Measured duty ch0: 50

    // Configure channel 1: 75% duty
    pwm_set_duty(&ctrl, 1, 75);
    pwm_enable(&ctrl, 1);
    duty = pwm_measure_duty(&ctrl, 1);
    printf("Measured duty ch1: %d\n", duty);
    // EXPECT: Measured duty ch1: 75

    // Configure channel 2: 25% duty
    pwm_set_duty(&ctrl, 2, 25);
    pwm_enable(&ctrl, 2);
    duty = pwm_measure_duty(&ctrl, 2);
    printf("Measured duty ch2: %d\n", duty);
    // EXPECT: Measured duty ch2: 25

    // Test 0% and 100% duty
    pwm_set_duty(&ctrl, 3, 0);
    pwm_enable(&ctrl, 3);
    duty = pwm_measure_duty(&ctrl, 3);
    printf("Measured duty ch3 (0): %d\n", duty);
    // EXPECT: Measured duty ch3 (0): 0

    pwm_set_duty(&ctrl, 3, 100);
    duty = pwm_measure_duty(&ctrl, 3);
    printf("Measured duty ch3 (100): %d\n", duty);
    // EXPECT: Measured duty ch3 (100): 100

    // Test edge-aligned output pattern (10 ticks, 50% duty, period 10)
    pwm_set_period(&ctrl, 0, 10);
    pwm_set_duty(&ctrl, 0, 50);
    ctrl.channels[0].counter = 0;
    printf("Edge-aligned pattern: ");
    for (i = 0; i < 10; i++) {
        pwm_tick_channel(&(ctrl.channels[0]));
        printf("%d", ctrl.channels[0].output);
    }
    printf("\n");
    // EXPECT: Edge-aligned pattern: 1111100000

    // Test center-aligned
    ctrl.channels[1].alignment = PWM_CENTER;
    pwm_set_period(&ctrl, 1, 10);
    pwm_set_duty(&ctrl, 1, 60);
    ctrl.channels[1].counter = 0;
    printf("Center-aligned pattern: ");
    for (i = 0; i < 10; i++) {
        pwm_tick_channel(&(ctrl.channels[1]));
        if (ctrl.channels[1].output) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("\n");
    // EXPECT: Center-aligned pattern: 0011111100

    // Test frequency calculation
    pwm_set_period(&ctrl, 0, 1000);
    int freq = pwm_get_output_freq(&ctrl, 0);
    printf("Output frequency: %d\n", freq);
    // EXPECT: Output frequency: 1000

    // Test disable
    pwm_disable(&ctrl, 0);
    printf("Ch0 after disable output: %d\n", ctrl.channels[0].output);
    // EXPECT: Ch0 after disable output: 0

    // Test duty clamping
    pwm_set_duty(&ctrl, 2, 150);
    printf("Clamped duty: %d\n", ctrl.channels[2].duty_percent);
    // EXPECT: Clamped duty: 100

    pwm_set_duty(&ctrl, 2, -10);
    printf("Clamped negative duty: %d\n", ctrl.channels[2].duty_percent);
    // EXPECT: Clamped negative duty: 0

    // Cycle count
    printf("Ch1 cycles: %d\n", ctrl.channels[1].cycles_complete);
    // EXPECT: Ch1 cycles: 2

    printf("PWM test complete\n");
    // EXPECT: PWM test complete

    return 0;
}
