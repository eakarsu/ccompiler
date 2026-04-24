int printf(const char *fmt, ...);
// EXPECT: push 10: OK\npush 20: OK\npush 30: OK\nget(1): val=20 err=OK\nget(10): val=0 err=NOT_FOUND\nset(0,99): OK\nset(5,50): INVALID\npop: val=30 err=OK\npop: val=20 err=OK\npop: val=99 err=OK\npop empty: UNDERFLOW\npush 12 items, last: OVERFLOW\nsize: 10
// Test: enum-based error codes

enum ErrorCode {
    ERR_NONE = 0,
    ERR_NOT_FOUND = -1,
    ERR_INVALID = -2,
    ERR_OVERFLOW = -3,
    ERR_UNDERFLOW = -4,
    ERR_PERM = -5
};

struct Result {
    int value;
    enum ErrorCode err;
};

struct SafeArray {
    int data[10];
    int size;
    int capacity;
};

void sa_init(struct SafeArray *a) {
    a->size = 0;
    a->capacity = 10;
}

struct Result sa_get(struct SafeArray *a, int idx) {
    struct Result r;
    if (idx < 0 || idx >= a->size) {
        r.value = 0;
        r.err = ERR_NOT_FOUND;
        return r;
    }
    r.value = a->data[idx];
    r.err = ERR_NONE;
    return r;
}

enum ErrorCode sa_push(struct SafeArray *a, int val) {
    if (a->size >= a->capacity) {
        return ERR_OVERFLOW;
    }
    a->data[a->size] = val;
    a->size = a->size + 1;
    return ERR_NONE;
}

enum ErrorCode sa_pop(struct SafeArray *a, int *out) {
    if (a->size == 0) {
        return ERR_UNDERFLOW;
    }
    a->size = a->size - 1;
    *out = a->data[a->size];
    return ERR_NONE;
}

enum ErrorCode sa_set(struct SafeArray *a, int idx, int val) {
    if (idx < 0 || idx >= a->size) {
        return ERR_INVALID;
    }
    a->data[idx] = val;
    return ERR_NONE;
}

void print_error(enum ErrorCode e) {
    switch (e) {
        case ERR_NONE: printf("OK"); break;
        case ERR_NOT_FOUND: printf("NOT_FOUND"); break;
        case ERR_INVALID: printf("INVALID"); break;
        case ERR_OVERFLOW: printf("OVERFLOW"); break;
        case ERR_UNDERFLOW: printf("UNDERFLOW"); break;
        case ERR_PERM: printf("PERMISSION"); break;
    }
}

int main(void) {
    struct SafeArray arr;
    sa_init(&arr);

    enum ErrorCode e;
    e = sa_push(&arr, 10);
    printf("push 10: "); print_error(e); printf("\n");
    e = sa_push(&arr, 20);
    printf("push 20: "); print_error(e); printf("\n");
    e = sa_push(&arr, 30);
    printf("push 30: "); print_error(e); printf("\n");

    struct Result r = sa_get(&arr, 1);
    printf("get(1): val=%d err=", r.value); print_error(r.err); printf("\n");

    r = sa_get(&arr, 10);
    printf("get(10): val=%d err=", r.value); print_error(r.err); printf("\n");

    e = sa_set(&arr, 0, 99);
    printf("set(0,99): "); print_error(e); printf("\n");
    e = sa_set(&arr, 5, 50);
    printf("set(5,50): "); print_error(e); printf("\n");

    int val;
    e = sa_pop(&arr, &val);
    printf("pop: val=%d err=", val); print_error(e); printf("\n");
    e = sa_pop(&arr, &val);
    printf("pop: val=%d err=", val); print_error(e); printf("\n");
    e = sa_pop(&arr, &val);
    printf("pop: val=%d err=", val); print_error(e); printf("\n");
    e = sa_pop(&arr, &val);
    printf("pop empty: "); print_error(e); printf("\n");

    int i;
    for (i = 0; i < 12; i++) {
        e = sa_push(&arr, i * 100);
    }
    printf("push 12 items, last: "); print_error(e); printf("\n");
    printf("size: %d\n", arr.size);

    return 0;
}
