int printf(const char *fmt, ...);

// Double-ended priority queue (min-max heap simulation using sorted array)
// Supports: insert, get_min, get_max, remove_min, remove_max

struct DEPQ {
    int data[20];
    int size;
};

void depq_init(struct DEPQ *q) {
    q->size = 0;
}

// Insert maintaining sorted order
void depq_insert(struct DEPQ *q, int val) {
    int i;
    int j;
    if (q->size >= 20) return;

    // Find insertion point (keep array sorted ascending)
    i = q->size - 1;
    while (i >= 0 && q->data[i] > val) {
        q->data[i + 1] = q->data[i];
        i = i - 1;
    }
    q->data[i + 1] = val;
    q->size = q->size + 1;
}

int depq_get_min(struct DEPQ *q) {
    if (q->size == 0) return -1;
    return q->data[0];
}

int depq_get_max(struct DEPQ *q) {
    if (q->size == 0) return -1;
    return q->data[q->size - 1];
}

int depq_remove_min(struct DEPQ *q) {
    int val;
    int i;
    if (q->size == 0) return -1;
    val = q->data[0];
    for (i = 0; i < q->size - 1; i++) {
        q->data[i] = q->data[i + 1];
    }
    q->size = q->size - 1;
    return val;
}

int depq_remove_max(struct DEPQ *q) {
    int val;
    if (q->size == 0) return -1;
    val = q->data[q->size - 1];
    q->size = q->size - 1;
    return val;
}

int depq_is_empty(struct DEPQ *q) {
    return q->size == 0;
}

int depq_median(struct DEPQ *q) {
    if (q->size == 0) return -1;
    return q->data[q->size / 2];
}

// Remove a specific value, returns 1 if found
int depq_remove_val(struct DEPQ *q, int val) {
    int i;
    int j;
    for (i = 0; i < q->size; i++) {
        if (q->data[i] == val) {
            for (j = i; j < q->size - 1; j++) {
                q->data[j] = q->data[j + 1];
            }
            q->size = q->size - 1;
            return 1;
        }
    }
    return 0;
}

int depq_contains(struct DEPQ *q, int val) {
    int lo;
    int hi;
    int mid;
    lo = 0;
    hi = q->size - 1;
    while (lo <= hi) {
        mid = (lo + hi) / 2;
        if (q->data[mid] == val) return 1;
        if (q->data[mid] < val) lo = mid + 1;
        else hi = mid - 1;
    }
    return 0;
}

int main() {
    struct DEPQ q;
    int val;

    depq_init(&q);
    printf("Empty: %d\n", depq_is_empty(&q));
    // EXPECT: Empty: 1

    depq_insert(&q, 50);
    depq_insert(&q, 30);
    depq_insert(&q, 70);
    depq_insert(&q, 10);
    depq_insert(&q, 90);

    printf("Size: %d\n", q.size);
    // EXPECT: Size: 5

    printf("Min: %d\n", depq_get_min(&q));
    // EXPECT: Min: 10

    printf("Max: %d\n", depq_get_max(&q));
    // EXPECT: Max: 90

    printf("Median: %d\n", depq_median(&q));
    // EXPECT: Median: 50

    printf("Contains 30: %d\n", depq_contains(&q, 30));
    // EXPECT: Contains 30: 1

    printf("Contains 40: %d\n", depq_contains(&q, 40));
    // EXPECT: Contains 40: 0

    // Remove min
    val = depq_remove_min(&q);
    printf("Removed min: %d\n", val);
    // EXPECT: Removed min: 10

    printf("New min: %d\n", depq_get_min(&q));
    // EXPECT: New min: 30

    printf("Size: %d\n", q.size);
    // EXPECT: Size: 4

    // Remove max
    val = depq_remove_max(&q);
    printf("Removed max: %d\n", val);
    // EXPECT: Removed max: 90

    printf("New max: %d\n", depq_get_max(&q));
    // EXPECT: New max: 70

    printf("Size: %d\n", q.size);
    // EXPECT: Size: 3

    // Insert more
    depq_insert(&q, 40);
    depq_insert(&q, 60);
    depq_insert(&q, 20);
    depq_insert(&q, 80);

    printf("Size after inserts: %d\n", q.size);
    // EXPECT: Size after inserts: 7

    printf("Min: %d\n", depq_get_min(&q));
    // EXPECT: Min: 20

    printf("Max: %d\n", depq_get_max(&q));
    // EXPECT: Max: 80

    // Median of [20, 30, 40, 50, 60, 70, 80] at index 3
    printf("Median: %d\n", depq_median(&q));
    // EXPECT: Median: 50

    // Remove specific value
    printf("Remove 50: %d\n", depq_remove_val(&q, 50));
    // EXPECT: Remove 50: 1

    printf("Remove 99: %d\n", depq_remove_val(&q, 99));
    // EXPECT: Remove 99: 0

    printf("Size: %d\n", q.size);
    // EXPECT: Size: 6

    // Median of [20, 30, 40, 60, 70, 80] at index 3
    printf("Median now: %d\n", depq_median(&q));
    // EXPECT: Median now: 60

    // Drain alternating min/max
    val = depq_remove_min(&q);
    printf("Drain min: %d\n", val);
    // EXPECT: Drain min: 20

    val = depq_remove_max(&q);
    printf("Drain max: %d\n", val);
    // EXPECT: Drain max: 80

    val = depq_remove_min(&q);
    printf("Drain min: %d\n", val);
    // EXPECT: Drain min: 30

    val = depq_remove_max(&q);
    printf("Drain max: %d\n", val);
    // EXPECT: Drain max: 70

    printf("Remaining size: %d\n", q.size);
    // EXPECT: Remaining size: 2

    printf("Remaining min: %d\n", depq_get_min(&q));
    // EXPECT: Remaining min: 40

    printf("Remaining max: %d\n", depq_get_max(&q));
    // EXPECT: Remaining max: 60

    return 0;
}
