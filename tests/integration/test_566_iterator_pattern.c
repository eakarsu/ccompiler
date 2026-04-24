int printf(const char *fmt, ...);
// EXPECT: Forward: 10 20 30 40 50 60 70 80 90 100 \nReverse: 100 90 80 70 60 50 40 30 20 10 \nStep 2: 10 30 50 70 90 \nStep 3: 10 40 70 100 \nSum: 550\nCount: 10

#define ITER_MAX 20

struct IntArray {
    int data[ITER_MAX];
    int size;
};

struct Iterator {
    int *data;
    int size;
    int pos;
    int step;
};

void arr_init(struct IntArray *a) {
    a->size = 0;
}

void arr_push(struct IntArray *a, int val) {
    if (a->size < ITER_MAX) {
        a->data[a->size] = val;
        a->size++;
    }
}

void iter_init(struct Iterator *it, struct IntArray *a) {
    it->data = a->data;
    it->size = a->size;
    it->pos = 0;
    it->step = 1;
}

void iter_init_reverse(struct Iterator *it, struct IntArray *a) {
    it->data = a->data;
    it->size = a->size;
    it->pos = a->size - 1;
    it->step = -1;
}

void iter_init_step(struct Iterator *it, struct IntArray *a, int step) {
    it->data = a->data;
    it->size = a->size;
    it->pos = 0;
    it->step = step;
}

int iter_has_next(struct Iterator *it) {
    if (it->step > 0) return it->pos < it->size;
    return it->pos >= 0;
}

int iter_next(struct Iterator *it) {
    int val = it->data[it->pos];
    it->pos = it->pos + it->step;
    return val;
}

void iter_reset(struct Iterator *it) {
    if (it->step > 0) {
        it->pos = 0;
    } else {
        it->pos = it->size - 1;
    }
}

int iter_sum(struct Iterator *it) {
    iter_reset(it);
    int total = 0;
    while (iter_has_next(it)) {
        total = total + iter_next(it);
    }
    return total;
}

int iter_count(struct Iterator *it) {
    iter_reset(it);
    int c = 0;
    while (iter_has_next(it)) {
        iter_next(it);
        c++;
    }
    return c;
}

int main(void) {
    struct IntArray arr;
    arr_init(&arr);
    int i;
    for (i = 1; i <= 10; i++) {
        arr_push(&arr, i * 10);
    }

    struct Iterator it;
    iter_init(&it, &arr);
    printf("Forward: ");
    while (iter_has_next(&it)) {
        printf("%d ", iter_next(&it));
    }
    printf("\n");

    iter_init_reverse(&it, &arr);
    printf("Reverse: ");
    while (iter_has_next(&it)) {
        printf("%d ", iter_next(&it));
    }
    printf("\n");

    iter_init_step(&it, &arr, 2);
    printf("Step 2: ");
    while (iter_has_next(&it)) {
        printf("%d ", iter_next(&it));
    }
    printf("\n");

    iter_init_step(&it, &arr, 3);
    printf("Step 3: ");
    while (iter_has_next(&it)) {
        printf("%d ", iter_next(&it));
    }
    printf("\n");

    iter_init(&it, &arr);
    printf("Sum: %d\n", iter_sum(&it));
    printf("Count: %d\n", iter_count(&it));

    return 0;
}
