int printf(const char *fmt, ...);
// EXPECT: Dataset 1:\nData: 45 67 23 67 89 34 67 12 56 78\nCount: 10\nSum: 538\nMean: 53\nMedian: 61\nMode: 67 (freq=3)\nMin: 12\nMax: 89\nRange: 77\nVariance(x100): 55240\n\nDataset 2:\nData: 5 10 15 20 25 30 35\nCount: 7\nSum: 140\nMean: 20\nMedian: 20\nMode: 5 (freq=1)\nMin: 5\nMax: 35\nRange: 30\nVariance(x100): 10000
#define MAX_DATA 15

struct Stats {
    int count;
    int sum;
    int mean;
    int min;
    int max;
    int range;
    int variance_x100;
    int median;
    int mode;
    int mode_freq;
};

void sort_array(int *arr, int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

struct Stats compute_stats(int *data, int n) {
    struct Stats s;
    s.count = n;
    s.sum = 0;
    s.min = data[0];
    s.max = data[0];

    int i;
    for (i = 0; i < n; i++) {
        s.sum = s.sum + data[i];
        if (data[i] < s.min) s.min = data[i];
        if (data[i] > s.max) s.max = data[i];
    }
    s.mean = s.sum / n;
    s.range = s.max - s.min;

    int sorted[MAX_DATA];
    for (i = 0; i < n; i++) sorted[i] = data[i];
    sort_array(sorted, n);

    if (n % 2 == 1) {
        s.median = sorted[n / 2];
    } else {
        s.median = (sorted[n / 2 - 1] + sorted[n / 2]) / 2;
    }

    int var_sum = 0;
    for (i = 0; i < n; i++) {
        int diff = data[i] - s.mean;
        var_sum = var_sum + diff * diff;
    }
    s.variance_x100 = var_sum * 100 / n;

    s.mode = data[0];
    s.mode_freq = 0;
    for (i = 0; i < n; i++) {
        int freq = 0;
        int j;
        for (j = 0; j < n; j++) {
            if (data[j] == data[i]) freq++;
        }
        if (freq > s.mode_freq) {
            s.mode_freq = freq;
            s.mode = data[i];
        }
    }

    return s;
}

void print_stats(struct Stats *s) {
    printf("Count: %d\n", s->count);
    printf("Sum: %d\n", s->sum);
    printf("Mean: %d\n", s->mean);
    printf("Median: %d\n", s->median);
    printf("Mode: %d (freq=%d)\n", s->mode, s->mode_freq);
    printf("Min: %d\n", s->min);
    printf("Max: %d\n", s->max);
    printf("Range: %d\n", s->range);
    printf("Variance(x100): %d\n", s->variance_x100);
}

int main(void) {
    int data1[10];
    data1[0] = 45; data1[1] = 67; data1[2] = 23; data1[3] = 67;
    data1[4] = 89; data1[5] = 34; data1[6] = 67; data1[7] = 12;
    data1[8] = 56; data1[9] = 78;

    printf("Dataset 1:\n");
    printf("Data: ");
    int i;
    for (i = 0; i < 10; i++) {
        if (i > 0) printf(" ");
        printf("%d", data1[i]);
    }
    printf("\n");
    struct Stats s1 = compute_stats(data1, 10);
    print_stats(&s1);

    int data2[7];
    data2[0] = 5; data2[1] = 10; data2[2] = 15;
    data2[3] = 20; data2[4] = 25; data2[5] = 30; data2[6] = 35;
    printf("\nDataset 2:\n");
    printf("Data: ");
    for (i = 0; i < 7; i++) {
        if (i > 0) printf(" ");
        printf("%d", data2[i]);
    }
    printf("\n");
    struct Stats s2 = compute_stats(data2, 7);
    print_stats(&s2);

    return 0;
}
