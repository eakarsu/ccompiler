int printf(const char *fmt, ...);

// Iterator Pattern: next/has_next over various collections

struct ArrayIter {
    int *data;
    int size;
    int pos;
};

void array_iter_init(struct ArrayIter *it, int *data, int size) {
    it->data = data;
    it->size = size;
    it->pos = 0;
}

int array_iter_has_next(struct ArrayIter *it) {
    return it->pos < it->size;
}

int array_iter_next(struct ArrayIter *it) {
    int val;
    val = it->data[it->pos];
    it->pos = it->pos + 1;
    return val;
}

void array_iter_reset(struct ArrayIter *it) {
    it->pos = 0;
}

struct RangeIter {
    int current;
    int end;
    int step;
};

void range_iter_init(struct RangeIter *it, int start, int end, int step) {
    it->current = start;
    it->end = end;
    it->step = step;
}

int range_iter_has_next(struct RangeIter *it) {
    if (it->step > 0) return it->current < it->end;
    return it->current > it->end;
}

int range_iter_next(struct RangeIter *it) {
    int val;
    val = it->current;
    it->current = it->current + it->step;
    return val;
}

struct FilterIter {
    struct ArrayIter *inner;
    int peeked;
    int peek_val;
    int filter_mod;
};

void filter_iter_init(struct FilterIter *it, struct ArrayIter *inner, int mod) {
    it->inner = inner;
    it->peeked = 0;
    it->peek_val = 0;
    it->filter_mod = mod;
}

int filter_advance(struct FilterIter *it) {
    while (array_iter_has_next(it->inner)) {
        int v;
        v = array_iter_next(it->inner);
        if (v % it->filter_mod == 0) {
            it->peek_val = v;
            it->peeked = 1;
            return 1;
        }
    }
    it->peeked = 0;
    return 0;
}

int filter_iter_has_next(struct FilterIter *it) {
    if (it->peeked) return 1;
    return filter_advance(it);
}

int filter_iter_next(struct FilterIter *it) {
    if (!it->peeked) {
        filter_advance(it);
    }
    it->peeked = 0;
    return it->peek_val;
}

int main() {
    int arr[6];
    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 50; arr[5] = 60;

    struct ArrayIter ait;
    array_iter_init(&ait, arr, 6);

    printf("Array iteration:\n"); // EXPECT: Array iteration:
    int sum;
    sum = 0;
    while (array_iter_has_next(&ait)) {
        int v;
        v = array_iter_next(&ait);
        sum = sum + v;
    }
    printf("Sum: %d\n", sum); // EXPECT: Sum: 210

    array_iter_reset(&ait);
    printf("After reset, pos: %d\n", ait.pos); // EXPECT: After reset, pos: 0

    int first;
    first = array_iter_next(&ait);
    printf("First: %d\n", first); // EXPECT: First: 10

    printf("Range iteration:\n"); // EXPECT: Range iteration:
    struct RangeIter rit;
    range_iter_init(&rit, 0, 10, 2);
    printf("Evens:");  // EXPECT: Evens: 0 2 4 6 8
    while (range_iter_has_next(&rit)) {
        printf(" %d", range_iter_next(&rit));
    }
    printf("\n");

    range_iter_init(&rit, 10, 0, -3);
    printf("Countdown:"); // EXPECT: Countdown: 10 7 4 1
    while (range_iter_has_next(&rit)) {
        printf(" %d", range_iter_next(&rit));
    }
    printf("\n");

    printf("Filter iteration:\n"); // EXPECT: Filter iteration:
    int arr2[10];
    arr2[0] = 1; arr2[1] = 2; arr2[2] = 3; arr2[3] = 4; arr2[4] = 5;
    arr2[5] = 6; arr2[6] = 7; arr2[7] = 8; arr2[8] = 9; arr2[9] = 10;

    struct ArrayIter ait2;
    array_iter_init(&ait2, arr2, 10);
    struct FilterIter fit;
    filter_iter_init(&fit, &ait2, 3);

    printf("Multiples of 3:"); // EXPECT: Multiples of 3: 3 6 9
    while (filter_iter_has_next(&fit)) {
        printf(" %d", filter_iter_next(&fit));
    }
    printf("\n");

    printf("Done\n"); // EXPECT: Done
    return 0;
}
