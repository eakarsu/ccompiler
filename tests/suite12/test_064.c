int printf(const char *fmt, ...);

// Sensor Fusion (weighted average, integer math)
// Combines multiple sensor readings with confidence weights

struct Sensor {
    int id;
    int value;       // sensor reading (x100 for precision)
    int weight;      // confidence weight (0-100)
    int noise;       // noise estimate
    int valid;       // 1 if reading is valid
};

struct FusionResult {
    int fused_value;
    int total_weight;
    int sensor_count;
    int confidence;
};

void sensor_init(struct Sensor *s, int id, int value, int weight, int noise) {
    s->id = id;
    s->value = value;
    s->weight = weight;
    s->noise = noise;
    s->valid = 1;
}

void sensor_invalidate(struct Sensor *s) {
    s->valid = 0;
    s->weight = 0;
}

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

struct FusionResult fuse_sensors(struct Sensor *sensors, int count) {
    struct FusionResult result;
    int total_weight;
    int weighted_sum;
    int valid_count;
    int i;

    total_weight = 0;
    weighted_sum = 0;
    valid_count = 0;

    for (i = 0; i < count; i = i + 1) {
        if (sensors[i].valid) {
            weighted_sum = weighted_sum + sensors[i].value * sensors[i].weight;
            total_weight = total_weight + sensors[i].weight;
            valid_count = valid_count + 1;
        }
    }

    if (total_weight > 0) {
        result.fused_value = weighted_sum / total_weight;
    } else {
        result.fused_value = 0;
    }
    result.total_weight = total_weight;
    result.sensor_count = valid_count;
    result.confidence = (total_weight * 100) / (count * 100);

    return result;
}

// Outlier detection: remove readings far from median
int find_median(struct Sensor *sensors, int count) {
    int vals[10];
    int n;
    int i;
    int j;
    int temp;

    n = 0;
    for (i = 0; i < count; i = i + 1) {
        if (sensors[i].valid) {
            vals[n] = sensors[i].value;
            n = n + 1;
        }
    }

    // Bubble sort
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (vals[j] > vals[j + 1]) {
                temp = vals[j];
                vals[j] = vals[j + 1];
                vals[j + 1] = temp;
            }
        }
    }

    if (n == 0) return 0;
    return vals[n / 2];
}

int remove_outliers(struct Sensor *sensors, int count, int threshold) {
    int median;
    int removed;
    int i;

    median = find_median(sensors, count);
    removed = 0;

    for (i = 0; i < count; i = i + 1) {
        if (sensors[i].valid) {
            if (abs_val(sensors[i].value - median) > threshold) {
                sensor_invalidate(&sensors[i]);
                removed = removed + 1;
            }
        }
    }
    return removed;
}

// Kalman-like filter (simplified integer version)
struct KalmanState {
    int estimate;    // current estimate (x100)
    int error;       // estimation error (x100)
};

void kalman_init(struct KalmanState *k, int initial, int error) {
    k->estimate = initial;
    k->error = error;
}

void kalman_update(struct KalmanState *k, int measurement, int meas_error) {
    int gain;
    int total_error;

    total_error = k->error + meas_error;
    if (total_error == 0) total_error = 1;

    gain = (k->error * 100) / total_error;

    k->estimate = k->estimate + (gain * (measurement - k->estimate)) / 100;
    k->error = ((100 - gain) * k->error) / 100;
    if (k->error < 1) k->error = 1;
}

int main() {
    struct Sensor sensors[5];
    struct FusionResult fr;
    struct KalmanState ks;
    int median;
    int removed;
    int i;

    printf("=== Sensor Fusion Test ===\n");
    // EXPECT: === Sensor Fusion Test ===

    // Test 1: Basic fusion of 3 sensors
    sensor_init(&sensors[0], 0, 1000, 80, 10);
    sensor_init(&sensors[1], 1, 1020, 60, 15);
    sensor_init(&sensors[2], 2, 980, 90, 8);

    fr = fuse_sensors(sensors, 3);
    printf("Fused: value=%d, weight=%d, count=%d\n", fr.fused_value, fr.total_weight, fr.sensor_count);
    // EXPECT: Fused: value=997, weight=230, count=3

    // Test 2: One invalid sensor
    sensor_init(&sensors[0], 0, 500, 50, 10);
    sensor_init(&sensors[1], 1, 520, 70, 12);
    sensor_init(&sensors[2], 2, 490, 60, 9);
    sensor_invalidate(&sensors[1]);

    fr = fuse_sensors(sensors, 3);
    printf("With invalid: value=%d, count=%d\n", fr.fused_value, fr.sensor_count);
    // EXPECT: With invalid: value=494, count=2

    // Test 3: Outlier detection
    sensor_init(&sensors[0], 0, 100, 50, 5);
    sensor_init(&sensors[1], 1, 105, 50, 5);
    sensor_init(&sensors[2], 2, 98, 50, 5);
    sensor_init(&sensors[3], 3, 500, 50, 5);
    sensor_init(&sensors[4], 4, 102, 50, 5);

    median = find_median(sensors, 5);
    printf("Median: %d\n", median);
    // EXPECT: Median: 102

    removed = remove_outliers(sensors, 5, 50);
    printf("Outliers removed: %d\n", removed);
    // EXPECT: Outliers removed: 1

    fr = fuse_sensors(sensors, 5);
    printf("After outlier removal: value=%d, count=%d\n", fr.fused_value, fr.sensor_count);
    // EXPECT: After outlier removal: value=101, count=4

    // Test 4: Equal weights
    sensor_init(&sensors[0], 0, 200, 50, 5);
    sensor_init(&sensors[1], 1, 300, 50, 5);
    fr = fuse_sensors(sensors, 2);
    printf("Equal weights: fused=%d\n", fr.fused_value);
    // EXPECT: Equal weights: fused=250

    // Test 5: Kalman filter
    kalman_init(&ks, 1000, 500);
    printf("Kalman init: est=%d, err=%d\n", ks.estimate, ks.error);
    // EXPECT: Kalman init: est=1000, err=500

    kalman_update(&ks, 1050, 200);
    printf("Kalman update 1: est=%d, err=%d\n", ks.estimate, ks.error);
    // EXPECT: Kalman update 1: est=1035, err=145

    kalman_update(&ks, 1040, 200);
    printf("Kalman update 2: est=%d, err=%d\n", ks.estimate, ks.error);
    // EXPECT: Kalman update 2: est=1037, err=84

    kalman_update(&ks, 1030, 200);
    printf("Kalman update 3: est=%d, err=%d\n", ks.estimate, ks.error);
    // EXPECT: Kalman update 3: est=1035, err=59

    // Test 6: All sensors invalid
    for (i = 0; i < 3; i = i + 1) {
        sensor_init(&sensors[i], i, 100, 50, 5);
        sensor_invalidate(&sensors[i]);
    }
    fr = fuse_sensors(sensors, 3);
    printf("All invalid: value=%d, count=%d\n", fr.fused_value, fr.sensor_count);
    // EXPECT: All invalid: value=0, count=0

    printf("Sensor fusion done\n");
    // EXPECT: Sensor fusion done

    return 0;
}
