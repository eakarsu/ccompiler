int printf(const char *fmt, ...);

struct RangeIter {
    int current;
    int end;
    int step;
    int count;
};

void range_init(struct RangeIter *it, int start, int end, int step) {
    it->current = start;
    it->end = end;
    it->step = step;
    it->count = 0;
}

int range_has_next(struct RangeIter *it) {
    if (it->step > 0) {
        return it->current < it->end;
    }
    return it->current > it->end;
}

int range_next(struct RangeIter *it) {
    int val = it->current;
    it->current = it->current + it->step;
    it->count = it->count + 1;
    return val;
}

struct ArrayIter {
    int *data;
    int size;
    int pos;
};

void array_iter_init(struct ArrayIter *it, int *arr, int size) {
    it->data = arr;
    it->size = size;
    it->pos = 0;
}

int array_has_next(struct ArrayIter *it) {
    return it->pos < it->size;
}

int array_next(struct ArrayIter *it) {
    int val = it->data[it->pos];
    it->pos = it->pos + 1;
    return val;
}

void array_reset(struct ArrayIter *it) {
    it->pos = 0;
}

struct FilterIter {
    int *data;
    int size;
    int pos;
    int min_val;
};

void filter_init(struct FilterIter *it, int *arr, int size, int min) {
    it->data = arr;
    it->size = size;
    it->pos = 0;
    it->min_val = min;
}

int filter_advance(struct FilterIter *it) {
    while (it->pos < it->size) {
        if (it->data[it->pos] >= it->min_val) {
            return 1;
        }
        it->pos = it->pos + 1;
    }
    return 0;
}

int filter_next(struct FilterIter *it) {
    int val = it->data[it->pos];
    it->pos = it->pos + 1;
    return val;
}

int main(void) {
    struct RangeIter ri;
    range_init(&ri, 0, 5, 1);

    int sum = 0;
    while (range_has_next(&ri)) {
        sum = sum + range_next(&ri);
    }
    // EXPECT: range_sum=10
    printf("range_sum=%d\n", sum);
    // EXPECT: range_count=5
    printf("range_count=%d\n", ri.count);

    range_init(&ri, 10, 0, -2);
    sum = 0;
    while (range_has_next(&ri)) {
        sum = sum + range_next(&ri);
    }
    // EXPECT: rev_sum=30
    printf("rev_sum=%d\n", sum);
    // EXPECT: rev_count=5
    printf("rev_count=%d\n", ri.count);

    range_init(&ri, 0, 10, 3);
    int product = 1;
    while (range_has_next(&ri)) {
        int v = range_next(&ri);
        if (v > 0) {
            product = product * v;
        }
    }
    // EXPECT: step3_product=162
    printf("step3_product=%d\n", product);
    // EXPECT: step3_count=4
    printf("step3_count=%d\n", ri.count);

    int arr[8];
    arr[0] = 3; arr[1] = 7; arr[2] = 1;
    arr[3] = 9; arr[4] = 2; arr[5] = 8;
    arr[6] = 4; arr[7] = 6;

    struct ArrayIter ai;
    array_iter_init(&ai, arr, 8);

    sum = 0;
    while (array_has_next(&ai)) {
        sum = sum + array_next(&ai);
    }
    // EXPECT: arr_sum=40
    printf("arr_sum=%d\n", sum);

    array_reset(&ai);
    int max = -1;
    while (array_has_next(&ai)) {
        int v = array_next(&ai);
        if (v > max) max = v;
    }
    // EXPECT: arr_max=9
    printf("arr_max=%d\n", max);

    array_reset(&ai);
    int min = 9999;
    while (array_has_next(&ai)) {
        int v = array_next(&ai);
        if (v < min) min = v;
    }
    // EXPECT: arr_min=1
    printf("arr_min=%d\n", min);

    struct FilterIter fi;
    filter_init(&fi, arr, 8, 5);
    sum = 0;
    int fcount = 0;
    while (filter_advance(&fi)) {
        sum = sum + filter_next(&fi);
        fcount = fcount + 1;
    }
    // EXPECT: filter_sum=30
    printf("filter_sum=%d\n", sum);
    // EXPECT: filter_count=4
    printf("filter_count=%d\n", fcount);

    filter_init(&fi, arr, 8, 8);
    sum = 0;
    fcount = 0;
    while (filter_advance(&fi)) {
        sum = sum + filter_next(&fi);
        fcount = fcount + 1;
    }
    // EXPECT: filter8_sum=17
    printf("filter8_sum=%d\n", sum);
    // EXPECT: filter8_count=2
    printf("filter8_count=%d\n", fcount);

    range_init(&ri, 1, 6, 1);
    int factorial = 1;
    while (range_has_next(&ri)) {
        factorial = factorial * range_next(&ri);
    }
    // EXPECT: factorial=120
    printf("factorial=%d\n", factorial);

    range_init(&ri, 0, 100, 7);
    sum = 0;
    while (range_has_next(&ri)) {
        sum = sum + range_next(&ri);
    }
    // EXPECT: step7_sum=735
    printf("step7_sum=%d\n", sum);
    // EXPECT: step7_count=15
    printf("step7_count=%d\n", ri.count);

    return 0;
}
