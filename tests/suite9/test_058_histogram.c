int printf(const char *fmt, ...);

// Histogram computation: bucket data into ranges and display
// Also computes statistics: min, max, mean, median

int hist_buckets[10];
int hist_count;
int hist_min_val;
int hist_max_val;
int hist_bucket_width;

void hist_init(int bmin, int bmax, int nbuckets) {
    int i;
    hist_count = 0;
    hist_min_val = bmin;
    hist_max_val = bmax;
    hist_bucket_width = (bmax - bmin) / nbuckets;
    for (i = 0; i < 10; i++) {
        hist_buckets[i] = 0;
    }
}

void hist_add(int val) {
    int idx;
    if (val < hist_min_val) val = hist_min_val;
    if (val >= hist_max_val) val = hist_max_val - 1;
    idx = (val - hist_min_val) / hist_bucket_width;
    if (idx < 0) idx = 0;
    if (idx >= 10) idx = 9;
    hist_buckets[idx] = hist_buckets[idx] + 1;
    hist_count = hist_count + 1;
}

int hist_get_bucket(int idx) {
    return hist_buckets[idx];
}

int hist_max_bucket(void) {
    int i;
    int mx;
    mx = 0;
    for (i = 0; i < 10; i++) {
        if (hist_buckets[i] > mx) mx = hist_buckets[i];
    }
    return mx;
}

int hist_empty_buckets(void) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < 10; i++) {
        if (hist_buckets[i] == 0) c = c + 1;
    }
    return c;
}

// Simple sorting for median computation
int sorted_data[100];
int sorted_len;

void sort_data(int *data, int n) {
    int i;
    int j;
    int tmp;
    sorted_len = n;
    for (i = 0; i < n; i++) {
        sorted_data[i] = data[i];
    }
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (sorted_data[j] > sorted_data[j + 1]) {
                tmp = sorted_data[j];
                sorted_data[j] = sorted_data[j + 1];
                sorted_data[j + 1] = tmp;
            }
        }
    }
}

int compute_min(int *data, int n) {
    int i;
    int mn;
    mn = data[0];
    for (i = 1; i < n; i++) {
        if (data[i] < mn) mn = data[i];
    }
    return mn;
}

int compute_max(int *data, int n) {
    int i;
    int mx;
    mx = data[0];
    for (i = 1; i < n; i++) {
        if (data[i] > mx) mx = data[i];
    }
    return mx;
}

int compute_sum(int *data, int n) {
    int i;
    int s;
    s = 0;
    for (i = 0; i < n; i++) {
        s = s + data[i];
    }
    return s;
}

int compute_median(int *data, int n) {
    sort_data(data, n);
    if (n % 2 == 1) {
        return sorted_data[n / 2];
    }
    return (sorted_data[n / 2 - 1] + sorted_data[n / 2]) / 2;
}

int main(void) {
    int data[30];
    int n;
    int i;
    int sum;

    // Test data: exam scores from 0 to 100
    data[0] = 45; data[1] = 67; data[2] = 89; data[3] = 23;
    data[4] = 78; data[5] = 56; data[6] = 92; data[7] = 34;
    data[8] = 71; data[9] = 85; data[10] = 63; data[11] = 49;
    data[12] = 77; data[13] = 88; data[14] = 52; data[15] = 61;
    data[16] = 73; data[17] = 95; data[18] = 41; data[19] = 68;
    n = 20;

    // Statistics
    // EXPECT: min=23
    printf("min=%d\n", compute_min(data, n));
    // EXPECT: max=95
    printf("max=%d\n", compute_max(data, n));
    sum = compute_sum(data, n);
    // EXPECT: sum=1307
    printf("sum=%d\n", sum);
    // EXPECT: mean=65
    printf("mean=%d\n", sum / n);
    // EXPECT: median=67
    printf("median=%d\n", compute_median(data, n));

    // Create histogram with 10 buckets from 0 to 100
    hist_init(0, 100, 10);
    for (i = 0; i < n; i++) {
        hist_add(data[i]);
    }

    // EXPECT: total count=20
    printf("total count=%d\n", hist_count);

    // Print bucket counts
    // bucket 0: [0,10) -> 0
    // bucket 1: [10,20) -> 0
    // bucket 2: [20,30) -> 1 (23)
    // bucket 3: [30,40) -> 1 (34)
    // bucket 4: [40,50) -> 3 (45,49,41)
    // bucket 5: [50,60) -> 2 (56,52)
    // bucket 6: [60,70) -> 4 (67,63,61,68)
    // bucket 7: [70,80) -> 4 (78,71,77,73)
    // bucket 8: [80,90) -> 3 (89,85,88)
    // bucket 9: [90,100) -> 2 (92,95)
    // EXPECT: b0=0
    printf("b0=%d\n", hist_get_bucket(0));
    // EXPECT: b1=0
    printf("b1=%d\n", hist_get_bucket(1));
    // EXPECT: b2=1
    printf("b2=%d\n", hist_get_bucket(2));
    // EXPECT: b3=1
    printf("b3=%d\n", hist_get_bucket(3));
    // EXPECT: b4=3
    printf("b4=%d\n", hist_get_bucket(4));
    // EXPECT: b5=2
    printf("b5=%d\n", hist_get_bucket(5));
    // EXPECT: b6=4
    printf("b6=%d\n", hist_get_bucket(6));
    // EXPECT: b7=4
    printf("b7=%d\n", hist_get_bucket(7));
    // EXPECT: b8=3
    printf("b8=%d\n", hist_get_bucket(8));
    // EXPECT: b9=2
    printf("b9=%d\n", hist_get_bucket(9));

    // EXPECT: max bucket=4
    printf("max bucket=%d\n", hist_max_bucket());
    // EXPECT: empty buckets=2
    printf("empty buckets=%d\n", hist_empty_buckets());

    // EXPECT: done
    printf("done\n");

    return 0;
}
