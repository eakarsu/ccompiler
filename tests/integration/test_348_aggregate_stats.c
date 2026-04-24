int printf(const char *fmt, ...);
// EXPECT: overall: count=10 sum=438 min=19 max=73 avg=43\nsensor 1: count=4 sum=101 min=19 max=31 avg=25\nsensor 2: count=3 sum=138 min=41 max=52 avg=46\nsensor 3: count=3 sum=199 min=59 max=73 avg=66\nabove 50: 4\nabove 30: 7\nvariance: 316
// Test: aggregate stats from structs

struct Measurement {
    int sensor_id;
    int value;
    int timestamp;
};

struct Stats {
    int count;
    int sum;
    int min;
    int max;
    int avg;
};

void stats_init(struct Stats *s) {
    s->count = 0;
    s->sum = 0;
    s->min = 0;
    s->max = 0;
    s->avg = 0;
}

void stats_add(struct Stats *s, int value) {
    if (s->count == 0) {
        s->min = value;
        s->max = value;
    } else {
        if (value < s->min) s->min = value;
        if (value > s->max) s->max = value;
    }
    s->sum = s->sum + value;
    s->count = s->count + 1;
    s->avg = s->sum / s->count;
}

void stats_print(struct Stats *s) {
    printf("count=%d sum=%d min=%d max=%d avg=%d\n",
           s->count, s->sum, s->min, s->max, s->avg);
}

void compute_stats(struct Measurement data[], int n, struct Stats *result) {
    stats_init(result);
    int i;
    for (i = 0; i < n; i++) {
        stats_add(result, data[i].value);
    }
}

void compute_stats_for_sensor(struct Measurement data[], int n, int sensor, struct Stats *result) {
    stats_init(result);
    int i;
    for (i = 0; i < n; i++) {
        if (data[i].sensor_id == sensor) {
            stats_add(result, data[i].value);
        }
    }
}

int count_above_threshold(struct Measurement data[], int n, int threshold) {
    int count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (data[i].value > threshold) count = count + 1;
    }
    return count;
}

int variance_approx(struct Measurement data[], int n) {
    if (n == 0) return 0;
    int avg = 0;
    int i;
    for (i = 0; i < n; i++) avg = avg + data[i].value;
    avg = avg / n;
    int var = 0;
    for (i = 0; i < n; i++) {
        int diff = data[i].value - avg;
        var = var + diff * diff;
    }
    return var / n;
}

int main(void) {
    struct Measurement data[10];
    data[0].sensor_id=1; data[0].value=23; data[0].timestamp=100;
    data[1].sensor_id=2; data[1].value=45; data[1].timestamp=101;
    data[2].sensor_id=1; data[2].value=31; data[2].timestamp=102;
    data[3].sensor_id=3; data[3].value=67; data[3].timestamp=103;
    data[4].sensor_id=2; data[4].value=52; data[4].timestamp=104;
    data[5].sensor_id=1; data[5].value=19; data[5].timestamp=105;
    data[6].sensor_id=3; data[6].value=73; data[6].timestamp=106;
    data[7].sensor_id=2; data[7].value=41; data[7].timestamp=107;
    data[8].sensor_id=1; data[8].value=28; data[8].timestamp=108;
    data[9].sensor_id=3; data[9].value=59; data[9].timestamp=109;

    struct Stats overall;
    compute_stats(data, 10, &overall);
    printf("overall: "); stats_print(&overall);

    struct Stats s1;
    compute_stats_for_sensor(data, 10, 1, &s1);
    printf("sensor 1: "); stats_print(&s1);

    struct Stats s2;
    compute_stats_for_sensor(data, 10, 2, &s2);
    printf("sensor 2: "); stats_print(&s2);

    struct Stats s3;
    compute_stats_for_sensor(data, 10, 3, &s3);
    printf("sensor 3: "); stats_print(&s3);

    printf("above 50: %d\n", count_above_threshold(data, 10, 50));
    printf("above 30: %d\n", count_above_threshold(data, 10, 30));
    printf("variance: %d\n", variance_approx(data, 10));

    return 0;
}
