int printf(const char *fmt, ...);

int main(void) {
    int arr[10];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;
    arr[5] = 60;
    arr[6] = 70;
    arr[7] = 80;
    arr[8] = 90;
    arr[9] = 100;

    int *begin = &arr[0];
    int *end = &arr[10];
    int *mid = &arr[5];

    // Pointer comparison: less than
    int lt = (begin < end) ? 1 : 0;
    // EXPECT: begin < end: 1
    printf("begin < end: %d\n", lt);

    // Pointer comparison: greater than
    int gt = (end > begin) ? 1 : 0;
    // EXPECT: end > begin: 1
    printf("end > begin: %d\n", gt);

    // Pointer equality
    int *p1 = &arr[3];
    int *p2 = &arr[3];
    int eq = (p1 == p2) ? 1 : 0;
    // EXPECT: equal ptrs: 1
    printf("equal ptrs: %d\n", eq);

    // Pointer inequality
    int neq = (begin != end) ? 1 : 0;
    // EXPECT: not equal: 1
    printf("not equal: %d\n", neq);

    // Pointer distance (subtraction)
    int dist = (int)(end - begin);
    // EXPECT: distance: 10
    printf("distance: %d\n", dist);

    int dist2 = (int)(mid - begin);
    // EXPECT: half distance: 5
    printf("half distance: %d\n", dist2);

    // Range check: is pointer within array?
    int *test1 = &arr[3];
    int in_range1 = (test1 >= begin && test1 < end) ? 1 : 0;
    // EXPECT: in range: 1
    printf("in range: %d\n", in_range1);

    // Pointer at boundary
    int *at_end = end;
    int at_boundary = (at_end >= begin && at_end < end) ? 1 : 0;
    // EXPECT: at end in range: 0
    printf("at end in range: %d\n", at_boundary);

    // Walking pointers and comparing
    int *left = begin;
    int *right = &arr[9];
    int swaps = 0;
    while (left < right) {
        if (*left > *right) {
            int tmp = *left;
            *left = *right;
            *right = tmp;
            swaps = swaps + 1;
        }
        left = left + 1;
        right = right - 1;
    }
    // Already sorted, no swaps needed
    // EXPECT: swaps: 0
    printf("swaps: %d\n", swaps);

    // Now reverse the array and count swaps to re-sort
    int rev[10];
    rev[0] = 100;
    rev[1] = 90;
    rev[2] = 80;
    rev[3] = 70;
    rev[4] = 60;
    rev[5] = 50;
    rev[6] = 40;
    rev[7] = 30;
    rev[8] = 20;
    rev[9] = 10;

    left = &rev[0];
    right = &rev[9];
    swaps = 0;
    while (left < right) {
        if (*left > *right) {
            int tmp = *left;
            *left = *right;
            *right = tmp;
            swaps = swaps + 1;
        }
        left = left + 1;
        right = right - 1;
    }
    // EXPECT: rev swaps: 5
    printf("rev swaps: %d\n", swaps);
    // EXPECT: rev[0] after: 10
    printf("rev[0] after: %d\n", rev[0]);
    // EXPECT: rev[9] after: 100
    printf("rev[9] after: %d\n", rev[9]);

    // Find element using pointer scan
    int target = 50;
    int *found = begin;
    while (found < end) {
        if (*found == target) {
            break;
        }
        found = found + 1;
    }
    int idx = (int)(found - begin);
    // EXPECT: found at: 4
    printf("found at: %d\n", idx);

    // Partition: count elements less than pivot
    int data[8];
    data[0] = 5;
    data[1] = 2;
    data[2] = 8;
    data[3] = 1;
    data[4] = 9;
    data[5] = 3;
    data[6] = 7;
    data[7] = 4;

    int pivot = 5;
    int less_count = 0;
    int *dp = &data[0];
    int *dend = &data[8];
    while (dp < dend) {
        if (*dp < pivot) {
            less_count = less_count + 1;
        }
        dp = dp + 1;
    }
    // EXPECT: less than 5: 4
    printf("less than 5: %d\n", less_count);

    // Pointer-based array max
    int *maxp = &data[0];
    int *cur = &data[1];
    while (cur < dend) {
        if (*cur > *maxp) {
            maxp = cur;
        }
        cur = cur + 1;
    }
    // EXPECT: max val: 9
    printf("max val: %d\n", *maxp);
    int maxidx = (int)(maxp - &data[0]);
    // EXPECT: max idx: 4
    printf("max idx: %d\n", maxidx);

    return 0;
}
