int printf(const char *fmt, ...);

// Test 096: Iterator pattern in C (external iterators over arrays/ranges)

struct ArrayIter {
    int *data;
    int len;
    int pos;
};

struct RangeIter {
    int current;
    int end;
    int step;
};

struct FilterIter {
    int *data;
    int len;
    int pos;
    int modulus;  // keep elements where elem % modulus == 0
};

struct MapIter {
    int *data;
    int len;
    int pos;
    int offset;  // add this to each element
};

void array_iter_init(struct ArrayIter *it, int *data, int len) {
    it->data = data;
    it->len = len;
    it->pos = 0;
}

int array_iter_has_next(struct ArrayIter *it) {
    return it->pos < it->len;
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

void filter_iter_init(struct FilterIter *it, int *data, int len, int mod) {
    it->data = data;
    it->len = len;
    it->pos = 0;
    it->modulus = mod;
}

int filter_iter_has_next(struct FilterIter *it) {
    while (it->pos < it->len) {
        if (it->data[it->pos] % it->modulus == 0) {
            return 1;
        }
        it->pos = it->pos + 1;
    }
    return 0;
}

int filter_iter_next(struct FilterIter *it) {
    int val;
    val = it->data[it->pos];
    it->pos = it->pos + 1;
    return val;
}

void map_iter_init(struct MapIter *it, int *data, int len, int offset) {
    it->data = data;
    it->len = len;
    it->pos = 0;
    it->offset = offset;
}

int map_iter_has_next(struct MapIter *it) {
    return it->pos < it->len;
}

int map_iter_next(struct MapIter *it) {
    int val;
    val = it->data[it->pos] + it->offset;
    it->pos = it->pos + 1;
    return val;
}

// Utility: collect from array iterator into buffer
int collect_array_iter(struct ArrayIter *it, int *out) {
    int n;
    n = 0;
    while (array_iter_has_next(it)) {
        out[n] = array_iter_next(it);
        n = n + 1;
    }
    return n;
}

// Sum using array iterator
int sum_array_iter(struct ArrayIter *it) {
    int total;
    total = 0;
    while (array_iter_has_next(it)) {
        total = total + array_iter_next(it);
    }
    return total;
}

void print_array(const char *label, int *arr, int len) {
    int i;
    printf("%s:", label);
    for (i = 0; i < len; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");
}

int main(void) {
    int data[8];
    int buf[16];
    int n;
    int val;
    struct ArrayIter ait;
    struct RangeIter rit;
    struct FilterIter fit;
    struct MapIter mit;

    data[0] = 10; data[1] = 20; data[2] = 30; data[3] = 40;
    data[4] = 50; data[5] = 60; data[6] = 70; data[7] = 80;

    // Array iterator
    array_iter_init(&ait, data, 8);
    n = collect_array_iter(&ait, buf);
    print_array("arr_iter", buf, n); // EXPECT: arr_iter: 10 20 30 40 50 60 70 80

    // Sum via iterator
    array_iter_init(&ait, data, 8);
    printf("sum: %d\n", sum_array_iter(&ait)); // EXPECT: sum: 360

    // Partial iteration
    array_iter_init(&ait, data, 8);
    val = array_iter_next(&ait);
    printf("first: %d\n", val); // EXPECT: first: 10
    val = array_iter_next(&ait);
    printf("second: %d\n", val); // EXPECT: second: 20
    printf("remaining: %d\n", sum_array_iter(&ait)); // EXPECT: remaining: 330

    // Range iterator: 0 to 5
    range_iter_init(&rit, 0, 5, 1);
    n = 0;
    while (range_iter_has_next(&rit)) {
        buf[n] = range_iter_next(&rit);
        n = n + 1;
    }
    print_array("range05", buf, n); // EXPECT: range05: 0 1 2 3 4

    // Range iterator: 0 to 10, step 2
    range_iter_init(&rit, 0, 10, 2);
    n = 0;
    while (range_iter_has_next(&rit)) {
        buf[n] = range_iter_next(&rit);
        n = n + 1;
    }
    print_array("range_s2", buf, n); // EXPECT: range_s2: 0 2 4 6 8

    // Range iterator: 10 down to 0, step -3
    range_iter_init(&rit, 10, 0, -3);
    n = 0;
    while (range_iter_has_next(&rit)) {
        buf[n] = range_iter_next(&rit);
        n = n + 1;
    }
    print_array("range_dn", buf, n); // EXPECT: range_dn: 10 7 4 1

    // Filter iterator: divisible by 3
    filter_iter_init(&fit, data, 8, 3);
    n = 0;
    while (filter_iter_has_next(&fit)) {
        buf[n] = filter_iter_next(&fit);
        n = n + 1;
    }
    print_array("div3", buf, n); // EXPECT: div3: 30 60

    // Filter iterator: divisible by 4
    filter_iter_init(&fit, data, 8, 4);
    n = 0;
    while (filter_iter_has_next(&fit)) {
        buf[n] = filter_iter_next(&fit);
        n = n + 1;
    }
    print_array("div4", buf, n); // EXPECT: div4: 20 40 60 80

    // Map iterator: add 100
    map_iter_init(&mit, data, 8, 100);
    n = 0;
    while (map_iter_has_next(&mit)) {
        buf[n] = map_iter_next(&mit);
        n = n + 1;
    }
    print_array("map+100", buf, n); // EXPECT: map+100: 110 120 130 140 150 160 170 180

    // Map iterator: subtract 5
    map_iter_init(&mit, data, 4, -5);
    n = 0;
    while (map_iter_has_next(&mit)) {
        buf[n] = map_iter_next(&mit);
        n = n + 1;
    }
    print_array("map-5", buf, n); // EXPECT: map-5: 5 15 25 35

    printf("done\n"); // EXPECT: done
    return 0;
}
