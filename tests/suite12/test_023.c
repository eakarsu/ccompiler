int printf(const char *fmt, ...);

// Multi-channel Audio Mixer
// Mixes up to 4 channels with volume and panning (integer math)
// Volume: 0..100, Pan: -100 (left) to +100 (right)

struct Channel {
    int samples[16];
    int volume;      // 0..100
    int pan;         // -100..100 (center=0)
    int muted;       // 0 or 1
    int solo;        // 0 or 1
};

struct MixResult {
    int left;
    int right;
};

struct MixResult mix_sample(struct Channel *channels, int num_ch, int sample_idx) {
    struct MixResult result;
    result.left = 0;
    result.right = 0;

    // Check if any channel has solo
    int has_solo = 0;
    int i;
    for (i = 0; i < num_ch; i = i + 1) {
        if (channels[i].solo) has_solo = 1;
    }

    for (i = 0; i < num_ch; i = i + 1) {
        // Skip muted channels
        if (channels[i].muted) continue;
        // If solo mode active, skip non-solo channels
        if (has_solo && channels[i].solo == 0) continue;

        int raw = channels[i].samples[sample_idx];
        // Apply volume
        int scaled = (raw * channels[i].volume) / 100;
        // Apply panning: left = (100 - pan)/200, right = (100 + pan)/200
        // Scaled to avoid overflow
        int left_gain = 100 - channels[i].pan;  // 0..200
        int right_gain = 100 + channels[i].pan;  // 0..200

        result.left = result.left + (scaled * left_gain) / 200;
        result.right = result.right + (scaled * right_gain) / 200;
    }

    return result;
}

int clamp(int value, int min_val, int max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

int compute_peak(int *buffer, int len) {
    int peak = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        int v = buffer[i];
        if (v < 0) v = -v;
        if (v > peak) peak = v;
    }
    return peak;
}

int main() {
    struct Channel ch[4];
    int i;

    // Channel 0: simple square wave, center pan, full volume
    for (i = 0; i < 16; i = i + 1) {
        if (i < 8) ch[0].samples[i] = 100;
        else ch[0].samples[i] = -100;
    }
    ch[0].volume = 100;
    ch[0].pan = 0;
    ch[0].muted = 0;
    ch[0].solo = 0;

    // Channel 1: constant tone, panned left
    for (i = 0; i < 16; i = i + 1) {
        ch[1].samples[i] = 50;
    }
    ch[1].volume = 80;
    ch[1].pan = -100;
    ch[1].muted = 0;
    ch[1].solo = 0;

    // Channel 2: ramp, panned right
    for (i = 0; i < 16; i = i + 1) {
        ch[2].samples[i] = i * 10;
    }
    ch[2].volume = 50;
    ch[2].pan = 100;
    ch[2].muted = 0;
    ch[2].solo = 0;

    // Channel 3: muted channel
    for (i = 0; i < 16; i = i + 1) {
        ch[3].samples[i] = 200;
    }
    ch[3].volume = 100;
    ch[3].pan = 0;
    ch[3].muted = 1;
    ch[3].solo = 0;

    // Mix first 8 samples
    printf("Mix L: ");
    for (i = 0; i < 8; i = i + 1) {
        struct MixResult r = mix_sample(ch, 4, i);
        printf("%d", r.left);
        if (i < 7) printf(" ");
    }
    printf("\n");
    // EXPECT: Mix L: 90 90 90 90 90 90 90 90

    printf("Mix R: ");
    for (i = 0; i < 8; i = i + 1) {
        struct MixResult r = mix_sample(ch, 4, i);
        printf("%d", r.right);
        if (i < 7) printf(" ");
    }
    printf("\n");
    // EXPECT: Mix R: 50 55 60 65 70 75 80 85

    // Test solo mode - solo channel 2
    ch[2].solo = 1;
    printf("Solo ch2 L: ");
    for (i = 0; i < 4; i = i + 1) {
        struct MixResult r = mix_sample(ch, 4, i);
        printf("%d", r.left);
        if (i < 3) printf(" ");
    }
    printf("\n");
    // EXPECT: Solo ch2 L: 0 0 0 0

    printf("Solo ch2 R: ");
    for (i = 0; i < 4; i = i + 1) {
        struct MixResult r = mix_sample(ch, 4, i);
        printf("%d", r.right);
        if (i < 3) printf(" ");
    }
    printf("\n");
    // EXPECT: Solo ch2 R: 0 5 10 15

    ch[2].solo = 0;

    // Peak detection on channel 0
    int peak0 = compute_peak(ch[0].samples, 16);
    printf("Ch0 peak: %d\n", peak0);
    // EXPECT: Ch0 peak: 100

    // Clamp test
    int c1 = clamp(500, -200, 200);
    printf("Clamp 500: %d\n", c1);
    // EXPECT: Clamp 500: 200

    int c2 = clamp(-300, -200, 200);
    printf("Clamp -300: %d\n", c2);
    // EXPECT: Clamp -300: -200

    // Mix with channel 1 muted too
    ch[1].muted = 1;
    printf("Muted ch1 L0: ");
    struct MixResult r0 = mix_sample(ch, 4, 0);
    printf("%d\n", r0.left);
    // EXPECT: Muted ch1 L0: 50

    printf("Muted ch1 R0: ");
    printf("%d\n", r0.right);
    // EXPECT: Muted ch1 R0: 50

    return 0;
}
