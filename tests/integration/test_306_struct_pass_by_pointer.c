int printf(const char *fmt, ...);
// EXPECT: c1: value=50 count=4 max=20 min=5 avg=12\nc2: value=103 count=2 max=100 min=3 avg=51\nmerged: value=153 count=6 max=100 min=3 avg=25\nreset: value=0 count=0 max=0 min=0 avg=0
// Test: struct passed to function by pointer

struct Counter {
    int value;
    int max_value;
    int min_value;
    int count;
};

void counter_init(struct Counter *c) {
    c->value = 0;
    c->max_value = 0;
    c->min_value = 0;
    c->count = 0;
}

void counter_add(struct Counter *c, int val) {
    c->value = c->value + val;
    c->count = c->count + 1;
    if (c->count == 1 || val > c->max_value) {
        c->max_value = val;
    }
    if (c->count == 1 || val < c->min_value) {
        c->min_value = val;
    }
}

int counter_average(struct Counter *c) {
    if (c->count == 0) return 0;
    return c->value / c->count;
}

void counter_reset(struct Counter *c) {
    c->value = 0;
    c->max_value = 0;
    c->min_value = 0;
    c->count = 0;
}

void counter_print(struct Counter *c) {
    printf("value=%d count=%d max=%d min=%d avg=%d\n",
           c->value, c->count, c->max_value, c->min_value,
           counter_average(c));
}

void counter_merge(struct Counter *dest, struct Counter *src) {
    dest->value = dest->value + src->value;
    dest->count = dest->count + src->count;
    if (src->max_value > dest->max_value) {
        dest->max_value = src->max_value;
    }
    if (src->count > 0 && (dest->count == src->count || src->min_value < dest->min_value)) {
        dest->min_value = src->min_value;
    }
}

int main(void) {
    struct Counter c1;
    counter_init(&c1);

    counter_add(&c1, 10);
    counter_add(&c1, 20);
    counter_add(&c1, 5);
    counter_add(&c1, 15);
    printf("c1: "); counter_print(&c1);

    struct Counter c2;
    counter_init(&c2);
    counter_add(&c2, 100);
    counter_add(&c2, 3);
    printf("c2: "); counter_print(&c2);

    counter_merge(&c1, &c2);
    printf("merged: "); counter_print(&c1);

    counter_reset(&c1);
    printf("reset: "); counter_print(&c1);

    return 0;
}
