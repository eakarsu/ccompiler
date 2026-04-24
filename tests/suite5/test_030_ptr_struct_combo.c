int printf(const char *fmt, ...);

struct Vec {
    int data[4];
    int len;
};

struct Table {
    int keys[8];
    int vals[8];
    int count;
};

int vec_sum(struct Vec *v) {
    int s = 0;
    int i = 0;
    while (i < v->len) {
        s = s + v->data[i];
        i = i + 1;
    }
    return s;
}

void table_put(struct Table *t, int key, int val) {
    int i = 0;
    while (i < t->count) {
        if (t->keys[i] == key) {
            t->vals[i] = val;
            return;
        }
        i = i + 1;
    }
    t->keys[t->count] = key;
    t->vals[t->count] = val;
    t->count = t->count + 1;
}

int table_get(struct Table *t, int key) {
    int i = 0;
    while (i < t->count) {
        if (t->keys[i] == key) {
            return t->vals[i];
        }
        i = i + 1;
    }
    return -1;
}

int main(void) {
    // Array of structs
    struct Vec vecs[3];
    vecs[0].data[0] = 1;
    vecs[0].data[1] = 2;
    vecs[0].data[2] = 3;
    vecs[0].len = 3;

    vecs[1].data[0] = 10;
    vecs[1].data[1] = 20;
    vecs[1].len = 2;

    vecs[2].data[0] = 100;
    vecs[2].data[1] = 200;
    vecs[2].data[2] = 300;
    vecs[2].data[3] = 400;
    vecs[2].len = 4;

    // Access via pointer to struct array
    struct Vec *vp = &vecs[0];

    // EXPECT: vec0 sum: 6
    printf("vec0 sum: %d\n", vec_sum(vp));
    // EXPECT: vec1 sum: 30
    printf("vec1 sum: %d\n", vec_sum(vp + 1));
    // EXPECT: vec2 sum: 1000
    printf("vec2 sum: %d\n", vec_sum(vp + 2));

    // Sum all vecs through pointer traversal
    int total = 0;
    int i = 0;
    while (i < 3) {
        total = total + vec_sum(vp + i);
        i = i + 1;
    }
    // EXPECT: all vecs sum: 1036
    printf("all vecs sum: %d\n", total);

    // Pointer array to structs
    struct Vec *ptrs[3];
    ptrs[0] = &vecs[2];
    ptrs[1] = &vecs[0];
    ptrs[2] = &vecs[1];

    // Access through pointer array
    // EXPECT: ptrs[0] sum: 1000
    printf("ptrs[0] sum: %d\n", vec_sum(ptrs[0]));
    // EXPECT: ptrs[1] sum: 6
    printf("ptrs[1] sum: %d\n", vec_sum(ptrs[1]));
    // EXPECT: ptrs[2] sum: 30
    printf("ptrs[2] sum: %d\n", vec_sum(ptrs[2]));

    // Modify struct through pointer
    ptrs[1]->data[0] = 99;
    // EXPECT: modified vec0[0]: 99
    printf("modified vec0[0]: %d\n", vecs[0].data[0]);
    // EXPECT: vec0 new sum: 104
    printf("vec0 new sum: %d\n", vec_sum(&vecs[0]));

    // Table with pointer access
    struct Table tab;
    tab.count = 0;

    table_put(&tab, 1, 100);
    table_put(&tab, 5, 500);
    table_put(&tab, 3, 300);
    table_put(&tab, 7, 700);

    // EXPECT: get(1): 100
    printf("get(1): %d\n", table_get(&tab, 1));
    // EXPECT: get(5): 500
    printf("get(5): %d\n", table_get(&tab, 5));
    // EXPECT: get(7): 700
    printf("get(7): %d\n", table_get(&tab, 7));
    // EXPECT: get(9): -1
    printf("get(9): %d\n", table_get(&tab, 9));

    // Update existing key
    table_put(&tab, 5, 555);
    // EXPECT: updated get(5): 555
    printf("updated get(5): %d\n", table_get(&tab, 5));
    // EXPECT: table count: 4
    printf("table count: %d\n", tab.count);

    // Struct containing pointer to another struct
    struct Vec *best = &vecs[0];
    i = 1;
    while (i < 3) {
        if (vec_sum(&vecs[i]) > vec_sum(best)) {
            best = &vecs[i];
        }
        i = i + 1;
    }
    // EXPECT: best vec sum: 1000
    printf("best vec sum: %d\n", vec_sum(best));
    // EXPECT: best vec len: 4
    printf("best vec len: %d\n", best->len);

    // Traverse and compute dot product style
    struct Vec a;
    a.data[0] = 1;
    a.data[1] = 2;
    a.data[2] = 3;
    a.data[3] = 4;
    a.len = 4;

    struct Vec b;
    b.data[0] = 5;
    b.data[1] = 6;
    b.data[2] = 7;
    b.data[3] = 8;
    b.len = 4;

    struct Vec *pa = &a;
    struct Vec *pb = &b;
    int dot = 0;
    i = 0;
    while (i < pa->len) {
        dot = dot + pa->data[i] * pb->data[i];
        i = i + 1;
    }
    // EXPECT: dot product: 70
    printf("dot product: %d\n", dot);

    return 0;
}
