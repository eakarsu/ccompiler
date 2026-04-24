int printf(const char *fmt, ...);

// ADC conversion simulation (integer scaled)

enum adc_resolution {
    ADC_8BIT,
    ADC_10BIT,
    ADC_12BIT
};

enum adc_ref {
    ADC_REF_INTERNAL,
    ADC_REF_EXTERNAL,
    ADC_REF_VCC
};

struct adc_channel {
    int raw_value;
    int max_value;
    int ref_mv;
    int resolution;
    int enabled;
    int calibration_offset;
    int calibration_gain;
};

struct adc_controller {
    struct adc_channel channels[8];
    int active_channel;
    int num_channels;
    int sample_rate;
    int conversion_done;
    int total_conversions;
};

void adc_init_channel(struct adc_channel *ch, int resolution, int ref_mv) {
    ch->resolution = resolution;
    ch->ref_mv = ref_mv;
    ch->enabled = 0;
    ch->raw_value = 0;
    ch->calibration_offset = 0;
    ch->calibration_gain = 1000;

    if (resolution == ADC_8BIT) {
        ch->max_value = 255;
    } else if (resolution == ADC_10BIT) {
        ch->max_value = 1023;
    } else {
        ch->max_value = 4095;
    }
}

void adc_enable(struct adc_channel *ch) {
    ch->enabled = 1;
}

void adc_set_calibration(struct adc_channel *ch, int offset, int gain_x1000) {
    ch->calibration_offset = offset;
    ch->calibration_gain = gain_x1000;
}

int adc_convert(struct adc_channel *ch, int input_mv) {
    int raw;
    if (!ch->enabled) return -1;
    if (ch->ref_mv == 0) return -1;

    raw = (input_mv * ch->max_value) / ch->ref_mv;
    if (raw < 0) raw = 0;
    if (raw > ch->max_value) raw = ch->max_value;

    raw = (raw * ch->calibration_gain) / 1000 + ch->calibration_offset;
    if (raw < 0) raw = 0;
    if (raw > ch->max_value) raw = ch->max_value;

    ch->raw_value = raw;
    return raw;
}

int adc_to_mv(struct adc_channel *ch) {
    if (ch->max_value == 0) return 0;
    return (ch->raw_value * ch->ref_mv) / ch->max_value;
}

int adc_to_temp_c(int raw_mv) {
    return (raw_mv - 500) / 10;
}

int adc_to_percent(struct adc_channel *ch) {
    if (ch->max_value == 0) return 0;
    return (ch->raw_value * 100) / ch->max_value;
}

void adc_ctrl_init(struct adc_controller *ctrl, int num_ch, int sample_rate) {
    int i;
    ctrl->num_channels = num_ch;
    ctrl->active_channel = 0;
    ctrl->sample_rate = sample_rate;
    ctrl->conversion_done = 0;
    ctrl->total_conversions = 0;
    for (i = 0; i < 8; i++) {
        adc_init_channel(&ctrl->channels[i], ADC_12BIT, 3300);
    }
}

int adc_ctrl_scan(struct adc_controller *ctrl, int *inputs_mv, int *results, int count) {
    int i;
    int converted = 0;
    for (i = 0; i < count && i < ctrl->num_channels; i++) {
        if (ctrl->channels[i].enabled) {
            results[i] = adc_convert(&ctrl->channels[i], inputs_mv[i]);
            ctrl->total_conversions++;
            converted++;
        } else {
            results[i] = -1;
        }
    }
    ctrl->conversion_done = 1;
    return converted;
}

int adc_average(int *samples, int count) {
    int sum = 0;
    int i;
    for (i = 0; i < count; i++) {
        sum = sum + samples[i];
    }
    if (count == 0) return 0;
    return sum / count;
}

int adc_find_max(int *samples, int count) {
    int max_val;
    int i;
    if (count == 0) return 0;
    max_val = samples[0];
    for (i = 1; i < count; i++) {
        if (samples[i] > max_val) {
            max_val = samples[i];
        }
    }
    return max_val;
}

int adc_find_min(int *samples, int count) {
    int min_val;
    int i;
    if (count == 0) return 0;
    min_val = samples[0];
    for (i = 1; i < count; i++) {
        if (samples[i] < min_val) {
            min_val = samples[i];
        }
    }
    return min_val;
}

int adc_moving_average(int *buffer, int buf_size, int new_sample) {
    int i;
    for (i = buf_size - 1; i > 0; i--) {
        buffer[i] = buffer[i - 1];
    }
    buffer[0] = new_sample;
    return adc_average(buffer, buf_size);
}

int main(void) {
    struct adc_channel ch;
    struct adc_controller ctrl;
    int result;

    adc_init_channel(&ch, ADC_12BIT, 3300);
    printf("%d\n", ch.max_value);
    // EXPECT: 4095
    printf("%d\n", ch.ref_mv);
    // EXPECT: 3300
    printf("%d\n", ch.enabled);
    // EXPECT: 0

    adc_enable(&ch);
    result = adc_convert(&ch, 1650);
    printf("%d\n", result);
    // EXPECT: 2047

    int mv = adc_to_mv(&ch);
    printf("%d\n", mv);
    // EXPECT: 1649

    result = adc_convert(&ch, 3300);
    printf("%d\n", result);
    // EXPECT: 4095

    mv = adc_to_mv(&ch);
    printf("%d\n", mv);
    // EXPECT: 3300

    result = adc_convert(&ch, 0);
    printf("%d\n", result);
    // EXPECT: 0
    mv = adc_to_mv(&ch);
    printf("%d\n", mv);
    // EXPECT: 0

    result = adc_convert(&ch, 5000);
    printf("%d\n", result);
    // EXPECT: 4095

    struct adc_channel ch8;
    adc_init_channel(&ch8, ADC_8BIT, 5000);
    adc_enable(&ch8);
    result = adc_convert(&ch8, 2500);
    printf("%d\n", result);
    // EXPECT: 127
    printf("%d\n", adc_to_percent(&ch8));
    // EXPECT: 49

    struct adc_channel ch10;
    adc_init_channel(&ch10, ADC_10BIT, 3300);
    adc_enable(&ch10);
    result = adc_convert(&ch10, 1000);
    printf("%d\n", result);
    // EXPECT: 310

    int temp_mv = 750;
    int temp_c = adc_to_temp_c(temp_mv);
    printf("%d\n", temp_c);
    // EXPECT: 25

    temp_mv = 1000;
    temp_c = adc_to_temp_c(temp_mv);
    printf("%d\n", temp_c);
    // EXPECT: 50

    adc_set_calibration(&ch, 10, 1020);
    result = adc_convert(&ch, 1650);
    printf("%d\n", result);
    // EXPECT: 2097

    adc_ctrl_init(&ctrl, 4, 1000);
    printf("%d\n", ctrl.num_channels);
    // EXPECT: 4
    printf("%d\n", ctrl.sample_rate);
    // EXPECT: 1000

    adc_enable(&ctrl.channels[0]);
    adc_enable(&ctrl.channels[1]);
    adc_enable(&ctrl.channels[2]);

    int inputs[4];
    int results[4];
    inputs[0] = 1000;
    inputs[1] = 2000;
    inputs[2] = 3000;
    inputs[3] = 500;

    int converted = adc_ctrl_scan(&ctrl, inputs, results, 4);
    printf("%d\n", converted);
    // EXPECT: 3
    printf("%d\n", results[0]);
    // EXPECT: 1240
    printf("%d\n", results[1]);
    // EXPECT: 2481
    printf("%d\n", results[2]);
    // EXPECT: 3722
    printf("%d\n", results[3]);
    // EXPECT: -1

    int samples[5];
    samples[0] = 100;
    samples[1] = 200;
    samples[2] = 300;
    samples[3] = 400;
    samples[4] = 500;
    printf("%d\n", adc_average(samples, 5));
    // EXPECT: 300
    printf("%d\n", adc_find_max(samples, 5));
    // EXPECT: 500
    printf("%d\n", adc_find_min(samples, 5));
    // EXPECT: 100

    int mavg_buf[4];
    mavg_buf[0] = 0;
    mavg_buf[1] = 0;
    mavg_buf[2] = 0;
    mavg_buf[3] = 0;
    int avg;
    avg = adc_moving_average(mavg_buf, 4, 100);
    printf("%d\n", avg);
    // EXPECT: 25
    avg = adc_moving_average(mavg_buf, 4, 200);
    printf("%d\n", avg);
    // EXPECT: 75
    avg = adc_moving_average(mavg_buf, 4, 300);
    printf("%d\n", avg);
    // EXPECT: 150
    avg = adc_moving_average(mavg_buf, 4, 400);
    printf("%d\n", avg);
    // EXPECT: 250

    printf("%s\n", "adc_done");
    // EXPECT: adc_done

    return 0;
}
