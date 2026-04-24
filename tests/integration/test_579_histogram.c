int printf(const char *fmt, ...);
// EXPECT: Data distribution (0-99):\n  0- 18 |###### (2)\n 19- 37 |############# (4)\n 38- 56 |#################### (6)\n 57- 75 |############# (4)\n 76- 99 |############# (4)\nTotal: 20\nStats: min=12 max=93 avg=51\nBin percentages:\n    0- 18: 10%\n   19- 37: 20%\n   38- 56: 30%\n   57- 75: 20%\n   76- 99: 20%

#define NUM_BINS 5
#define MAX_DATA 20

struct Histogram {
    int bins[NUM_BINS];
    int bin_min[NUM_BINS];
    int bin_max[NUM_BINS];
    int total_count;
};

void hist_init(struct Histogram *h, int range_min, int range_max) {
    int bin_size = (range_max - range_min) / NUM_BINS;
    int i;
    for (i = 0; i < NUM_BINS; i++) {
        h->bins[i] = 0;
        h->bin_min[i] = range_min + i * bin_size;
        h->bin_max[i] = range_min + (i + 1) * bin_size - 1;
    }
    h->bin_max[NUM_BINS - 1] = range_max;
    h->total_count = 0;
}

void hist_add(struct Histogram *h, int value) {
    int i;
    for (i = 0; i < NUM_BINS; i++) {
        if (value >= h->bin_min[i] && value <= h->bin_max[i]) {
            h->bins[i]++;
            h->total_count++;
            return;
        }
    }
}

void hist_display(struct Histogram *h) {
    int max_count = 0;
    int i;
    for (i = 0; i < NUM_BINS; i++) {
        if (h->bins[i] > max_count) max_count = h->bins[i];
    }

    for (i = 0; i < NUM_BINS; i++) {
        printf("%3d-%3d |", h->bin_min[i], h->bin_max[i]);
        int bar_len = 0;
        if (max_count > 0) {
            bar_len = h->bins[i] * 20 / max_count;
        }
        int j;
        for (j = 0; j < bar_len; j++) {
            printf("#");
        }
        printf(" (%d)\n", h->bins[i]);
    }
}

int main(void) {
    int data[MAX_DATA];
    data[0] = 12; data[1] = 45; data[2] = 67; data[3] = 23; data[4] = 89;
    data[5] = 34; data[6] = 56; data[7] = 78; data[8] = 15; data[9] = 42;
    data[10] = 93; data[11] = 27; data[12] = 61; data[13] = 38; data[14] = 74;
    data[15] = 51; data[16] = 86; data[17] = 19; data[18] = 55; data[19] = 70;
    int n = 20;

    struct Histogram h;
    hist_init(&h, 0, 99);

    int i;
    for (i = 0; i < n; i++) {
        hist_add(&h, data[i]);
    }

    printf("Data distribution (0-99):\n");
    hist_display(&h);
    printf("Total: %d\n", h.total_count);

    int sum = 0;
    int min_val = data[0];
    int max_val = data[0];
    for (i = 0; i < n; i++) {
        sum = sum + data[i];
        if (data[i] < min_val) min_val = data[i];
        if (data[i] > max_val) max_val = data[i];
    }
    printf("Stats: min=%d max=%d avg=%d\n", min_val, max_val, sum / n);

    printf("Bin percentages:\n");
    for (i = 0; i < NUM_BINS; i++) {
        int pct = h.bins[i] * 100 / h.total_count;
        printf("  %3d-%3d: %d%%\n", h.bin_min[i], h.bin_max[i], pct);
    }

    return 0;
}
