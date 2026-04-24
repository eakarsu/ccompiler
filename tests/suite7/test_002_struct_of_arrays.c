int printf(const char *fmt, ...);

struct MultiArray {
    int keys[8];
    int vals[8];
    int count;
};

void ma_init(struct MultiArray *ma) {
    int i;
    for (i = 0; i < 8; i = i + 1) {
        ma->keys[i] = 0;
        ma->vals[i] = 0;
    }
    ma->count = 0;
}

void ma_insert(struct MultiArray *ma, int key, int val) {
    if (ma->count < 8) {
        ma->keys[ma->count] = key;
        ma->vals[ma->count] = val;
        ma->count = ma->count + 1;
    }
}

int ma_lookup(struct MultiArray *ma, int key) {
    int i;
    for (i = 0; i < ma->count; i = i + 1) {
        if (ma->keys[i] == key) {
            return ma->vals[i];
        }
    }
    return -1;
}

int ma_sum_vals(struct MultiArray *ma) {
    int total = 0;
    int i;
    for (i = 0; i < ma->count; i = i + 1) {
        total = total + ma->vals[i];
    }
    return total;
}

int ma_sum_keys(struct MultiArray *ma) {
    int total = 0;
    int i;
    for (i = 0; i < ma->count; i = i + 1) {
        total = total + ma->keys[i];
    }
    return total;
}

void ma_zip_add(struct MultiArray *dst, struct MultiArray *a, struct MultiArray *b) {
    int i;
    int n = a->count;
    if (b->count < n) n = b->count;
    ma_init(dst);
    for (i = 0; i < n; i = i + 1) {
        ma_insert(dst, a->keys[i] + b->keys[i], a->vals[i] + b->vals[i]);
    }
}

void ma_scale(struct MultiArray *ma, int factor) {
    int i;
    for (i = 0; i < ma->count; i = i + 1) {
        ma->vals[i] = ma->vals[i] * factor;
    }
}

void ma_reverse(struct MultiArray *ma) {
    int i;
    int n = ma->count;
    for (i = 0; i < n / 2; i = i + 1) {
        int tk = ma->keys[i];
        int tv = ma->vals[i];
        ma->keys[i] = ma->keys[n - 1 - i];
        ma->vals[i] = ma->vals[n - 1 - i];
        ma->keys[n - 1 - i] = tk;
        ma->vals[n - 1 - i] = tv;
    }
}

int ma_dot_product(struct MultiArray *a, struct MultiArray *b) {
    int result = 0;
    int i;
    int n = a->count;
    if (b->count < n) n = b->count;
    for (i = 0; i < n; i = i + 1) {
        result = result + a->vals[i] * b->vals[i];
    }
    return result;
}

int main(void) {
    struct MultiArray ma;
    ma_init(&ma);

    ma_insert(&ma, 10, 100);
    ma_insert(&ma, 20, 200);
    ma_insert(&ma, 30, 300);
    ma_insert(&ma, 40, 400);

    // EXPECT: count=4
    printf("count=%d\n", ma.count);

    // EXPECT: lookup 20=200
    printf("lookup 20=%d\n", ma_lookup(&ma, 20));

    // EXPECT: lookup 99=-1
    printf("lookup 99=%d\n", ma_lookup(&ma, 99));

    // EXPECT: sum_vals=1000
    printf("sum_vals=%d\n", ma_sum_vals(&ma));

    // EXPECT: sum_keys=100
    printf("sum_keys=%d\n", ma_sum_keys(&ma));

    struct MultiArray mb;
    ma_init(&mb);
    ma_insert(&mb, 1, 5);
    ma_insert(&mb, 2, 6);
    ma_insert(&mb, 3, 7);
    ma_insert(&mb, 4, 8);

    struct MultiArray mc;
    ma_zip_add(&mc, &ma, &mb);
    // EXPECT: zip count=4
    printf("zip count=%d\n", mc.count);
    // EXPECT: zip[0] key=11 val=105
    printf("zip[0] key=%d val=%d\n", mc.keys[0], mc.vals[0]);
    // EXPECT: zip[3] key=44 val=408
    printf("zip[3] key=%d val=%d\n", mc.keys[3], mc.vals[3]);

    ma_scale(&mb, 10);
    // EXPECT: scaled[0]=50
    printf("scaled[0]=%d\n", mb.vals[0]);
    // EXPECT: scaled[3]=80
    printf("scaled[3]=%d\n", mb.vals[3]);

    ma_reverse(&ma);
    // EXPECT: rev key[0]=40
    printf("rev key[0]=%d\n", ma.keys[0]);
    // EXPECT: rev val[0]=400
    printf("rev val[0]=%d\n", ma.vals[0]);
    // EXPECT: rev key[3]=10
    printf("rev key[3]=%d\n", ma.keys[3]);

    // EXPECT: dot=60000
    printf("dot=%d\n", ma_dot_product(&ma, &mb));

    ma_insert(&ma, 50, 500);
    // EXPECT: after insert count=5
    printf("after insert count=%d\n", ma.count);

    // EXPECT: lookup 50=500
    printf("lookup 50=%d\n", ma_lookup(&ma, 50));

    int i;
    int key_sum = 0;
    for (i = 0; i < ma.count; i = i + 1) {
        key_sum = key_sum + ma.keys[i];
    }
    // EXPECT: final key_sum=150
    printf("final key_sum=%d\n", key_sum);

    return 0;
}
