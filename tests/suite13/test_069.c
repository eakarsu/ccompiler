int printf(const char *fmt, ...);

// Hash-based deduplication
// Remove duplicate integers from an array using a hash set

enum { DEDUP_SIZE = 64, DEDUP_EMPTY = -999999 };

struct DedupSet {
    int data[64];
    int count;
};

void ds_init(struct DedupSet *ds) {
    int i;
    for (i = 0; i < DEDUP_SIZE; i++) {
        ds->data[i] = DEDUP_EMPTY;
    }
    ds->count = 0;
}

int ds_hash(int val) {
    int h = val;
    if (h < 0) h = -h;
    return h % DEDUP_SIZE;
}

int ds_add(struct DedupSet *ds, int val) {
    int idx;
    int start;
    idx = ds_hash(val);
    start = idx;
    while (1) {
        if (ds->data[idx] == DEDUP_EMPTY) {
            ds->data[idx] = val;
            ds->count = ds->count + 1;
            return 1;
        }
        if (ds->data[idx] == val) {
            return 0;
        }
        idx = (idx + 1) % DEDUP_SIZE;
        if (idx == start) return 0;
    }
    return 0;
}

int ds_contains(struct DedupSet *ds, int val) {
    int idx;
    int start;
    idx = ds_hash(val);
    start = idx;
    while (1) {
        if (ds->data[idx] == DEDUP_EMPTY) return 0;
        if (ds->data[idx] == val) return 1;
        idx = (idx + 1) % DEDUP_SIZE;
        if (idx == start) return 0;
    }
    return 0;
}

int dedup_array(int *arr, int n, int *out) {
    struct DedupSet ds;
    int i;
    int out_count;
    ds_init(&ds);
    out_count = 0;
    for (i = 0; i < n; i++) {
        if (ds_add(&ds, arr[i])) {
            out[out_count] = arr[i];
            out_count = out_count + 1;
        }
    }
    return out_count;
}

int count_duplicates(int *arr, int n) {
    struct DedupSet ds;
    int i;
    int dups;
    ds_init(&ds);
    dups = 0;
    for (i = 0; i < n; i++) {
        if (!ds_add(&ds, arr[i])) {
            dups = dups + 1;
        }
    }
    return dups;
}

int first_duplicate(int *arr, int n) {
    struct DedupSet ds;
    int i;
    ds_init(&ds);
    for (i = 0; i < n; i++) {
        if (!ds_add(&ds, arr[i])) {
            return arr[i];
        }
    }
    return -1;
}

int all_unique(int *arr, int n) {
    struct DedupSet ds;
    int i;
    ds_init(&ds);
    for (i = 0; i < n; i++) {
        if (!ds_add(&ds, arr[i])) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int input[20];
    int output[20];
    int n;
    int i;

    input[0] = 5;
    input[1] = 3;
    input[2] = 5;
    input[3] = 7;
    input[4] = 3;
    input[5] = 1;
    input[6] = 7;
    input[7] = 5;
    input[8] = 9;
    input[9] = 1;

    n = dedup_array(input, 10, output);
    printf("%d\n", n);
    // EXPECT: 5

    for (i = 0; i < n; i++) {
        printf("%d\n", output[i]);
    }
    // EXPECT: 5
    // EXPECT: 3
    // EXPECT: 7
    // EXPECT: 1
    // EXPECT: 9

    printf("%d\n", count_duplicates(input, 10));
    // EXPECT: 5

    printf("%d\n", first_duplicate(input, 10));
    // EXPECT: 5

    input[0] = 1;
    input[1] = 2;
    input[2] = 3;
    input[3] = 4;
    input[4] = 5;
    printf("%d\n", all_unique(input, 5));
    // EXPECT: 1

    printf("%d\n", count_duplicates(input, 5));
    // EXPECT: 0

    printf("%d\n", first_duplicate(input, 5));
    // EXPECT: -1

    input[0] = 10;
    input[1] = 20;
    input[2] = 10;
    input[3] = 30;
    input[4] = 20;
    input[5] = 40;
    input[6] = 10;
    printf("%d\n", all_unique(input, 7));
    // EXPECT: 0

    printf("%d\n", count_duplicates(input, 7));
    // EXPECT: 3

    n = dedup_array(input, 7, output);
    printf("%d\n", n);
    // EXPECT: 4

    for (i = 0; i < n; i++) {
        printf("%d\n", output[i]);
    }
    // EXPECT: 10
    // EXPECT: 20
    // EXPECT: 30
    // EXPECT: 40

    printf("%d\n", first_duplicate(input, 7));
    // EXPECT: 10

    input[0] = 42;
    n = dedup_array(input, 1, output);
    printf("%d\n", n);
    // EXPECT: 1
    printf("%d\n", output[0]);
    // EXPECT: 42

    input[0] = 100;
    input[1] = 100;
    input[2] = 100;
    n = dedup_array(input, 3, output);
    printf("%d\n", n);
    // EXPECT: 1
    printf("%d\n", output[0]);
    // EXPECT: 100

    printf("%d\n", count_duplicates(input, 3));
    // EXPECT: 2

    return 0;
}
