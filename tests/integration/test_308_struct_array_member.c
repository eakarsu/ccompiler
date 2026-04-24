int printf(const char *fmt, ...);
// EXPECT: array: [5, 3, 8, 1, 9]\nsize: 5\nsum: 26\nmax: 9\nget[2]: 8\nreversed: [9, 1, 8, 3, 5]
// Test: struct with array member

struct IntArray {
    int data[10];
    int size;
};

void arr_init(struct IntArray *a) {
    a->size = 0;
}

void arr_push(struct IntArray *a, int val) {
    if (a->size < 10) {
        a->data[a->size] = val;
        a->size = a->size + 1;
    }
}

int arr_get(struct IntArray *a, int idx) {
    if (idx >= 0 && idx < a->size) {
        return a->data[idx];
    }
    return -1;
}

int arr_sum(struct IntArray *a) {
    int sum = 0;
    int i;
    for (i = 0; i < a->size; i++) {
        sum = sum + a->data[i];
    }
    return sum;
}

int arr_max(struct IntArray *a) {
    if (a->size == 0) return 0;
    int max = a->data[0];
    int i;
    for (i = 1; i < a->size; i++) {
        if (a->data[i] > max) max = a->data[i];
    }
    return max;
}

void arr_reverse(struct IntArray *a) {
    int i = 0;
    int j = a->size - 1;
    while (i < j) {
        int tmp = a->data[i];
        a->data[i] = a->data[j];
        a->data[j] = tmp;
        i = i + 1;
        j = j - 1;
    }
}

void arr_print(struct IntArray *a) {
    int i;
    printf("[");
    for (i = 0; i < a->size; i++) {
        if (i > 0) printf(", ");
        printf("%d", a->data[i]);
    }
    printf("]\n");
}

int main(void) {
    struct IntArray arr;
    arr_init(&arr);

    arr_push(&arr, 5);
    arr_push(&arr, 3);
    arr_push(&arr, 8);
    arr_push(&arr, 1);
    arr_push(&arr, 9);

    printf("array: "); arr_print(&arr);
    printf("size: %d\n", arr.size);
    printf("sum: %d\n", arr_sum(&arr));
    printf("max: %d\n", arr_max(&arr));
    printf("get[2]: %d\n", arr_get(&arr, 2));

    arr_reverse(&arr);
    printf("reversed: "); arr_print(&arr);

    return 0;
}
