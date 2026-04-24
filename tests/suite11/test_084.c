int printf(const char *fmt, ...);

// ADC Simulation
// Integer-based ADC with multiple channels, averaging, thresholds

enum AdcState { ADC_IDLE, ADC_CONVERTING, ADC_DONE };
enum AdcRes { ADC_8BIT, ADC_10BIT, ADC_12BIT };

struct AdcChannel {
    int channel_id;
    int raw_value;      // simulated analog input (0-4095 range for 12-bit)
    int converted;      // last conversion result
    int enabled;
};

struct AdcController {
    struct AdcChannel channels[8];
    int num_channels;
    int resolution;     // 8, 10, or 12 bits
    int max_value;      // 255, 1023, or 4095
    int state;
    int current_channel;
    int reference_mv;   // reference voltage in millivolts
    int sample_buffer[16];
    int buffer_count;
};

void adc_init(struct AdcController *adc, int resolution, int ref_mv) {
    int i;
    adc->num_channels = 8;
    adc->state = ADC_IDLE;
    adc->current_channel = 0;
    adc->reference_mv = ref_mv;
    adc->buffer_count = 0;

    if (resolution == ADC_8BIT) {
        adc->resolution = 8;
        adc->max_value = 255;
    } else if (resolution == ADC_10BIT) {
        adc->resolution = 10;
        adc->max_value = 1023;
    } else {
        adc->resolution = 12;
        adc->max_value = 4095;
    }

    for (i = 0; i < 8; i++) {
        adc->channels[i].channel_id = i;
        adc->channels[i].raw_value = 0;
        adc->channels[i].converted = 0;
        adc->channels[i].enabled = 0;
    }
    for (i = 0; i < 16; i++) {
        adc->sample_buffer[i] = 0;
    }
}

void adc_enable_channel(struct AdcController *adc, int ch) {
    if (ch < 0 || ch >= 8) return;
    adc->channels[ch].enabled = 1;
}

void adc_set_input(struct AdcController *adc, int ch, int value) {
    if (ch < 0 || ch >= 8) return;
    if (value < 0) value = 0;
    if (value > adc->max_value) value = adc->max_value;
    adc->channels[ch].raw_value = value;
}

int adc_convert(struct AdcController *adc, int ch) {
    if (ch < 0 || ch >= 8) return -1;
    if (!adc->channels[ch].enabled) return -1;

    adc->state = ADC_CONVERTING;
    adc->current_channel = ch;
    adc->channels[ch].converted = adc->channels[ch].raw_value;
    adc->state = ADC_DONE;
    return adc->channels[ch].converted;
}

int adc_to_millivolts(struct AdcController *adc, int raw) {
    // mv = (raw * reference_mv) / max_value
    return (raw * adc->reference_mv) / adc->max_value;
}

int adc_average_samples(struct AdcController *adc, int ch, int count) {
    int sum = 0;
    int i;
    int val;
    if (count > 16) count = 16;
    if (count < 1) count = 1;

    adc->buffer_count = 0;
    for (i = 0; i < count; i++) {
        val = adc_convert(adc, ch);
        if (val < 0) return -1;
        adc->sample_buffer[i] = val;
        sum = sum + val;
        adc->buffer_count = adc->buffer_count + 1;
    }
    return sum / count;
}

// Find min sample in buffer
int adc_min_sample(struct AdcController *adc) {
    int min_val;
    int i;
    if (adc->buffer_count == 0) return -1;
    min_val = adc->sample_buffer[0];
    for (i = 1; i < adc->buffer_count; i++) {
        if (adc->sample_buffer[i] < min_val) {
            min_val = adc->sample_buffer[i];
        }
    }
    return min_val;
}

// Find max sample in buffer
int adc_max_sample(struct AdcController *adc) {
    int max_val;
    int i;
    if (adc->buffer_count == 0) return -1;
    max_val = adc->sample_buffer[0];
    for (i = 1; i < adc->buffer_count; i++) {
        if (adc->sample_buffer[i] > max_val) {
            max_val = adc->sample_buffer[i];
        }
    }
    return max_val;
}

int adc_check_threshold(struct AdcController *adc, int ch, int low, int high) {
    int val = adc_convert(adc, ch);
    if (val < 0) return -1;
    if (val < low) return 0;   // below threshold
    if (val > high) return 2;  // above threshold
    return 1;                  // within range
}

// Scan all enabled channels
int adc_scan_all(struct AdcController *adc, int *results) {
    int i;
    int count = 0;
    for (i = 0; i < 8; i++) {
        if (adc->channels[i].enabled) {
            results[count] = adc_convert(adc, i);
            count = count + 1;
        }
    }
    return count;
}

int main() {
    struct AdcController adc;
    int val;
    int mv;
    int avg;
    int results[8];
    int scan_count;
    int i;

    // Initialize 12-bit ADC with 3300mV reference
    adc_init(&adc, ADC_12BIT, 3300);
    printf("ADC initialized: %d-bit, ref=%d mV\n", adc.resolution, adc.reference_mv);
    // EXPECT: ADC initialized: 12-bit, ref=3300 mV

    // Enable channels 0-3
    adc_enable_channel(&adc, 0);
    adc_enable_channel(&adc, 1);
    adc_enable_channel(&adc, 2);
    adc_enable_channel(&adc, 3);

    // Set simulated inputs
    adc_set_input(&adc, 0, 2048);  // ~1650mV (half)
    adc_set_input(&adc, 1, 4095);  // ~3300mV (full)
    adc_set_input(&adc, 2, 0);     // 0mV
    adc_set_input(&adc, 3, 1024);  // ~825mV (quarter)

    // Convert channel 0
    val = adc_convert(&adc, 0);
    printf("Ch0 raw: %d\n", val);
    // EXPECT: Ch0 raw: 2048

    mv = adc_to_millivolts(&adc, val);
    printf("Ch0 millivolts: %d\n", mv);
    // EXPECT: Ch0 millivolts: 1650

    // Convert channel 1
    val = adc_convert(&adc, 1);
    mv = adc_to_millivolts(&adc, val);
    printf("Ch1 raw: %d, mv: %d\n", val, mv);
    // EXPECT: Ch1 raw: 4095, mv: 3300

    // Convert channel 2
    val = adc_convert(&adc, 2);
    mv = adc_to_millivolts(&adc, val);
    printf("Ch2 raw: %d, mv: %d\n", val, mv);
    // EXPECT: Ch2 raw: 0, mv: 0

    // Convert channel 3
    val = adc_convert(&adc, 3);
    mv = adc_to_millivolts(&adc, val);
    printf("Ch3 raw: %d, mv: %d\n", val, mv);
    // EXPECT: Ch3 raw: 1024, mv: 825

    // Try disabled channel
    val = adc_convert(&adc, 5);
    printf("Disabled channel result: %d\n", val);
    // EXPECT: Disabled channel result: -1

    // Average samples on channel 0
    avg = adc_average_samples(&adc, 0, 8);
    printf("Ch0 average (8 samples): %d\n", avg);
    // EXPECT: Ch0 average (8 samples): 2048

    printf("Buffer count: %d\n", adc.buffer_count);
    // EXPECT: Buffer count: 8

    // Min and max from buffer (all same value)
    printf("Min sample: %d\n", adc_min_sample(&adc));
    // EXPECT: Min sample: 2048
    printf("Max sample: %d\n", adc_max_sample(&adc));
    // EXPECT: Max sample: 2048

    // Threshold checks
    val = adc_check_threshold(&adc, 0, 1000, 3000);
    printf("Ch0 threshold check (1000-3000): %d\n", val);
    // EXPECT: Ch0 threshold check (1000-3000): 1

    val = adc_check_threshold(&adc, 1, 1000, 3000);
    printf("Ch1 threshold check (1000-3000): %d\n", val);
    // EXPECT: Ch1 threshold check (1000-3000): 2

    val = adc_check_threshold(&adc, 2, 1000, 3000);
    printf("Ch2 threshold check (1000-3000): %d\n", val);
    // EXPECT: Ch2 threshold check (1000-3000): 0

    // Scan all enabled channels
    scan_count = adc_scan_all(&adc, results);
    printf("Scanned %d channels\n", scan_count);
    // EXPECT: Scanned 4 channels
    for (i = 0; i < scan_count; i++) {
        printf("Scan result %d: %d\n", i, results[i]);
    }
    // EXPECT: Scan result 0: 2048
    // EXPECT: Scan result 1: 4095
    // EXPECT: Scan result 2: 0
    // EXPECT: Scan result 3: 1024

    // Test 8-bit ADC
    adc_init(&adc, ADC_8BIT, 5000);
    adc_enable_channel(&adc, 0);
    adc_set_input(&adc, 0, 128);
    val = adc_convert(&adc, 0);
    mv = adc_to_millivolts(&adc, val);
    printf("8-bit ADC: raw=%d, mv=%d\n", val, mv);
    // EXPECT: 8-bit ADC: raw=128, mv=2509

    // Test clamping
    adc_set_input(&adc, 0, 999);
    val = adc_convert(&adc, 0);
    printf("Clamped value: %d\n", val);
    // EXPECT: Clamped value: 255

    printf("ADC test complete\n");
    // EXPECT: ADC test complete

    return 0;
}
