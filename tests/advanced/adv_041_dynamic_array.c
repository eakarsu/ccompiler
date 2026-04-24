// EXPECT: === Dynamic Array (Vector) ===
// EXPECT: Initial: size=0, cap=4
// EXPECT: After push 10,20,30: size=3, cap=4
// EXPECT: v[0]=10 v[1]=20 v[2]=30
// EXPECT: After push 40,50: size=5, cap=8
// EXPECT: v[0]=10 v[1]=20 v[2]=30 v[3]=40 v[4]=50
// EXPECT: Pop: 50
// EXPECT: Pop: 40
// EXPECT: After pops: size=3, cap=8
// EXPECT: After shrink: size=3, cap=4
// EXPECT: Set v[1]=99: v[0]=10 v[1]=99 v[2]=30
// EXPECT: After clear: size=0, cap=4
// EXPECT: Stress push 0..19: size=20, cap=32
// EXPECT: Stress values OK
// EXPECT: After stress pops: size=10, cap=32
// EXPECT: After shrink: size=10, cap=16
// EXPECT: === Done ===

int printf(const char *fmt, ...);
void *malloc(unsigned long size);
void *realloc(void *ptr, unsigned long size);
void free(void *ptr);

struct Vec {
    int *data;
    int size;
    int cap;
};

void vec_init(struct Vec *v, int initial_cap) {
    v->size = 0;
    v->cap = initial_cap;
    v->data = (int *)malloc(initial_cap * 4);
}

void vec_grow(struct Vec *v) {
    int new_cap = v->cap * 2;
    int *new_data = (int *)realloc(v->data, new_cap * 4);
    v->data = new_data;
    v->cap = new_cap;
}

void vec_push(struct Vec *v, int val) {
    if (v->size >= v->cap) {
        vec_grow(v);
    }
    v->data[v->size] = val;
    v->size = v->size + 1;
}

int vec_pop(struct Vec *v) {
    if (v->size <= 0) return -1;
    v->size = v->size - 1;
    return v->data[v->size];
}

int vec_get(struct Vec *v, int idx) {
    if (idx < 0 || idx >= v->size) return -1;
    return v->data[idx];
}

void vec_set(struct Vec *v, int idx, int val) {
    if (idx >= 0 && idx < v->size) {
        v->data[idx] = val;
    }
}

void vec_shrink(struct Vec *v) {
    int new_cap = v->cap;
    while (new_cap > 4 && new_cap / 2 >= v->size) {
        new_cap = new_cap / 2;
    }
    if (new_cap < v->cap) {
        v->data = (int *)realloc(v->data, new_cap * 4);
        v->cap = new_cap;
    }
}

void vec_clear(struct Vec *v) {
    v->size = 0;
}

void vec_free(struct Vec *v) {
    free(v->data);
    v->data = (int *)0;
    v->size = 0;
    v->cap = 0;
}

void vec_print(struct Vec *v) {
    int i = 0;
    while (i < v->size) {
        if (i > 0) printf(" ");
        printf("v[%d]=%d", i, v->data[i]);
        i = i + 1;
    }
    printf("\n");
}

int main(void) {
    struct Vec v;

    printf("=== Dynamic Array (Vector) ===\n");

    vec_init(&v, 4);
    printf("Initial: size=%d, cap=%d\n", v.size, v.cap);

    vec_push(&v, 10);
    vec_push(&v, 20);
    vec_push(&v, 30);
    printf("After push 10,20,30: size=%d, cap=%d\n", v.size, v.cap);
    vec_print(&v);

    vec_push(&v, 40);
    vec_push(&v, 50);
    printf("After push 40,50: size=%d, cap=%d\n", v.size, v.cap);
    vec_print(&v);

    int p1 = vec_pop(&v);
    int p2 = vec_pop(&v);
    printf("Pop: %d\n", p1);
    printf("Pop: %d\n", p2);
    printf("After pops: size=%d, cap=%d\n", v.size, v.cap);

    vec_shrink(&v);
    printf("After shrink: size=%d, cap=%d\n", v.size, v.cap);

    vec_set(&v, 1, 99);
    printf("Set v[1]=99: ");
    vec_print(&v);

    vec_clear(&v);
    printf("After clear: size=%d, cap=%d\n", v.size, v.cap);

    // Stress test: push 20 elements
    int i = 0;
    while (i < 20) {
        vec_push(&v, i);
        i = i + 1;
    }
    printf("Stress push 0..19: size=%d, cap=%d\n", v.size, v.cap);

    int ok = 1;
    i = 0;
    while (i < 20) {
        if (vec_get(&v, i) != i) ok = 0;
        i = i + 1;
    }
    if (ok) printf("Stress values OK\n");

    // Pop 10
    i = 0;
    while (i < 10) {
        vec_pop(&v);
        i = i + 1;
    }
    printf("After stress pops: size=%d, cap=%d\n", v.size, v.cap);

    vec_shrink(&v);
    printf("After shrink: size=%d, cap=%d\n", v.size, v.cap);

    vec_free(&v);

    printf("=== Done ===\n");
    return 0;
}
