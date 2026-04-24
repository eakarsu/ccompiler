int printf(const char *fmt, ...);

struct Record {
    int key;
    int value;
};

void print_records(struct Record *recs, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("(%d,%d)", recs[i].key, recs[i].value);
    }
    printf("\n");
}

void insertion_sort_records(struct Record *recs, int n) {
    int i;
    int j;
    struct Record key_rec;
    for (i = 1; i < n; i++) {
        key_rec.key = recs[i].key;
        key_rec.value = recs[i].value;
        j = i - 1;
        while (j >= 0 && recs[j].key > key_rec.key) {
            recs[j + 1].key = recs[j].key;
            recs[j + 1].value = recs[j].value;
            j = j - 1;
        }
        recs[j + 1].key = key_rec.key;
        recs[j + 1].value = key_rec.value;
    }
}

void bubble_sort_records(struct Record *recs, int n) {
    int i;
    int j;
    struct Record tmp;
    int swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        for (j = 0; j < n - 1 - i; j++) {
            if (recs[j].key > recs[j + 1].key) {
                tmp.key = recs[j].key;
                tmp.value = recs[j].value;
                recs[j].key = recs[j + 1].key;
                recs[j].value = recs[j + 1].value;
                recs[j + 1].key = tmp.key;
                recs[j + 1].value = tmp.value;
                swapped = 1;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

void selection_sort_records(struct Record *recs, int n) {
    int i;
    int j;
    int min_idx;
    struct Record tmp;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (recs[j].key < recs[min_idx].key) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            tmp.key = recs[i].key;
            tmp.value = recs[i].value;
            recs[i].key = recs[min_idx].key;
            recs[i].value = recs[min_idx].value;
            recs[min_idx].key = tmp.key;
            recs[min_idx].value = tmp.value;
        }
    }
}

int is_stable(struct Record *recs, int n) {
    int i;
    int j;
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (recs[i].key == recs[j].key) {
                if (recs[i].value > recs[j].value) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int is_sorted_by_key(struct Record *recs, int n) {
    int i;
    for (i = 0; i < n - 1; i++) {
        if (recs[i].key > recs[i + 1].key) {
            return 0;
        }
    }
    return 1;
}

void copy_records(struct Record *dst, struct Record *src, int n) {
    int i;
    for (i = 0; i < n; i++) {
        dst[i].key = src[i].key;
        dst[i].value = src[i].value;
    }
}

int main() {
    struct Record data[8];
    struct Record work[8];

    data[0].key = 3; data[0].value = 1;
    data[1].key = 1; data[1].value = 1;
    data[2].key = 3; data[2].value = 2;
    data[3].key = 2; data[3].value = 1;
    data[4].key = 1; data[4].value = 2;
    data[5].key = 2; data[5].value = 2;
    data[6].key = 3; data[6].value = 3;
    data[7].key = 1; data[7].value = 3;

    printf("Test 1: Original data\n");
    // EXPECT: Test 1: Original data
    printf("Data: ");
    // EXPECT: Data: (3,1) (1,1) (3,2) (2,1) (1,2) (2,2) (3,3) (1,3)
    print_records(data, 8);

    printf("Test 2: Insertion sort (stable)\n");
    // EXPECT: Test 2: Insertion sort (stable)
    copy_records(work, data, 8);
    insertion_sort_records(work, 8);
    printf("Result: ");
    // EXPECT: Result: (1,1) (1,2) (1,3) (2,1) (2,2) (3,1) (3,2) (3,3)
    print_records(work, 8);
    printf("Sorted: %d\n", is_sorted_by_key(work, 8));
    // EXPECT: Sorted: 1
    printf("Stable: %d\n", is_stable(work, 8));
    // EXPECT: Stable: 1

    printf("Test 3: Bubble sort (stable)\n");
    // EXPECT: Test 3: Bubble sort (stable)
    copy_records(work, data, 8);
    bubble_sort_records(work, 8);
    printf("Result: ");
    // EXPECT: Result: (1,1) (1,2) (1,3) (2,1) (2,2) (3,1) (3,2) (3,3)
    print_records(work, 8);
    printf("Sorted: %d\n", is_sorted_by_key(work, 8));
    // EXPECT: Sorted: 1
    printf("Stable: %d\n", is_stable(work, 8));
    // EXPECT: Stable: 1

    printf("Test 4: Selection sort (not necessarily stable)\n");
    // EXPECT: Test 4: Selection sort (not necessarily stable)
    copy_records(work, data, 8);
    selection_sort_records(work, 8);
    printf("Sorted: %d\n", is_sorted_by_key(work, 8));
    // EXPECT: Sorted: 1

    printf("Test 5: Stability with two-key data\n");
    // EXPECT: Test 5: Stability with two-key data
    data[0].key = 5; data[0].value = 1;
    data[1].key = 5; data[1].value = 2;
    data[2].key = 5; data[2].value = 3;
    data[3].key = 1; data[3].value = 1;
    data[4].key = 1; data[4].value = 2;
    copy_records(work, data, 5);
    insertion_sort_records(work, 5);
    printf("Result: ");
    // EXPECT: Result: (1,1) (1,2) (5,1) (5,2) (5,3)
    print_records(work, 5);
    printf("Stable: %d\n", is_stable(work, 5));
    // EXPECT: Stable: 1

    printf("Test 6: All same keys\n");
    // EXPECT: Test 6: All same keys
    data[0].key = 7; data[0].value = 1;
    data[1].key = 7; data[1].value = 2;
    data[2].key = 7; data[2].value = 3;
    data[3].key = 7; data[3].value = 4;
    copy_records(work, data, 4);
    bubble_sort_records(work, 4);
    printf("Result: ");
    // EXPECT: Result: (7,1) (7,2) (7,3) (7,4)
    print_records(work, 4);
    printf("Stable: %d\n", is_stable(work, 4));
    // EXPECT: Stable: 1

    printf("Test 7: Already sorted input\n");
    // EXPECT: Test 7: Already sorted input
    data[0].key = 1; data[0].value = 1;
    data[1].key = 2; data[1].value = 1;
    data[2].key = 3; data[2].value = 1;
    data[3].key = 4; data[3].value = 1;
    data[4].key = 5; data[4].value = 1;
    copy_records(work, data, 5);
    insertion_sort_records(work, 5);
    printf("Result: ");
    // EXPECT: Result: (1,1) (2,1) (3,1) (4,1) (5,1)
    print_records(work, 5);
    printf("Sorted: %d\n", is_sorted_by_key(work, 5));
    // EXPECT: Sorted: 1
    printf("Stable: %d\n", is_stable(work, 5));
    // EXPECT: Stable: 1

    printf("All sort stability tests passed\n");
    // EXPECT: All sort stability tests passed
    return 0;
}
