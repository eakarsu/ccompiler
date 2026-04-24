int printf(const char *fmt, ...);
// EXPECT: [0] bool(true) truthy=1 num=1\n[1] bool(false) truthy=0 num=0\n[2] int(42) truthy=1 num=42\n[3] int(0) truthy=0 num=0\n[4] array[10,20,30] truthy=1 num=60\ntruthy count: 3\ntotal: 103
// Test: union-based tagged values

enum Tag {
    TAG_BOOL,
    TAG_INT,
    TAG_ARRAY
};

struct SmallArray {
    int elems[4];
    int len;
};

union Payload {
    int bool_val;
    int int_val;
    struct SmallArray arr;
};

struct Tagged {
    enum Tag tag;
    union Payload data;
};

struct Tagged tag_bool(int b) {
    struct Tagged t;
    t.tag = TAG_BOOL;
    t.data.bool_val = b ? 1 : 0;
    return t;
}

struct Tagged tag_int(int v) {
    struct Tagged t;
    t.tag = TAG_INT;
    t.data.int_val = v;
    return t;
}

struct Tagged tag_array(int a, int b, int c) {
    struct Tagged t;
    t.tag = TAG_ARRAY;
    t.data.arr.elems[0] = a;
    t.data.arr.elems[1] = b;
    t.data.arr.elems[2] = c;
    t.data.arr.len = 3;
    return t;
}

void print_tagged(struct Tagged *t) {
    switch (t->tag) {
        case TAG_BOOL:
            printf("bool(%s)", t->data.bool_val ? "true" : "false");
            break;
        case TAG_INT:
            printf("int(%d)", t->data.int_val);
            break;
        case TAG_ARRAY: {
            printf("array[");
            int i;
            for (i = 0; i < t->data.arr.len; i++) {
                if (i > 0) printf(",");
                printf("%d", t->data.arr.elems[i]);
            }
            printf("]");
            break;
        }
    }
}

int is_truthy(struct Tagged *t) {
    switch (t->tag) {
        case TAG_BOOL: return t->data.bool_val;
        case TAG_INT:  return t->data.int_val != 0;
        case TAG_ARRAY: return t->data.arr.len > 0;
    }
    return 0;
}

int to_number(struct Tagged *t) {
    switch (t->tag) {
        case TAG_BOOL: return t->data.bool_val;
        case TAG_INT:  return t->data.int_val;
        case TAG_ARRAY: {
            int sum = 0;
            int i;
            for (i = 0; i < t->data.arr.len; i++) {
                sum = sum + t->data.arr.elems[i];
            }
            return sum;
        }
    }
    return 0;
}

int main(void) {
    struct Tagged vals[5];
    vals[0] = tag_bool(1);
    vals[1] = tag_bool(0);
    vals[2] = tag_int(42);
    vals[3] = tag_int(0);
    vals[4] = tag_array(10, 20, 30);

    int i;
    for (i = 0; i < 5; i++) {
        printf("[%d] ", i);
        print_tagged(&vals[i]);
        printf(" truthy=%d num=%d\n", is_truthy(&vals[i]), to_number(&vals[i]));
    }

    int truthy_count = 0;
    int total = 0;
    for (i = 0; i < 5; i++) {
        if (is_truthy(&vals[i])) truthy_count = truthy_count + 1;
        total = total + to_number(&vals[i]);
    }
    printf("truthy count: %d\n", truthy_count);
    printf("total: %d\n", total);

    return 0;
}
