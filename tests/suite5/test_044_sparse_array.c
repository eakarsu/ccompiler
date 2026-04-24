int printf(const char *fmt, ...);

int sp_indices[64];
int sp_values[64];
int sp_count;

void sparse_init(void) {
    sp_count = 0;
}

int sparse_find(int idx) {
    int i;
    i = 0;
    while (i < sp_count) {
        if (sp_indices[i] == idx) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

void sparse_set(int idx, int val) {
    int pos;
    pos = sparse_find(idx);
    if (pos >= 0) {
        sp_values[pos] = val;
    } else {
        sp_indices[sp_count] = idx;
        sp_values[sp_count] = val;
        sp_count = sp_count + 1;
    }
}

int sparse_get(int idx, int default_val) {
    int pos;
    pos = sparse_find(idx);
    if (pos >= 0) {
        return sp_values[pos];
    }
    return default_val;
}

int sparse_remove(int idx) {
    int pos;
    pos = sparse_find(idx);
    if (pos < 0) return 0;
    sp_indices[pos] = sp_indices[sp_count - 1];
    sp_values[pos] = sp_values[sp_count - 1];
    sp_count = sp_count - 1;
    return 1;
}

void sparse_compress(void) {
    int i;
    int j;
    i = 0;
    while (i < sp_count) {
        if (sp_values[i] == 0) {
            sp_indices[i] = sp_indices[sp_count - 1];
            sp_values[i] = sp_values[sp_count - 1];
            sp_count = sp_count - 1;
        } else {
            i = i + 1;
        }
    }
}

int sparse_sum(void) {
    int i;
    int total;
    total = 0;
    i = 0;
    while (i < sp_count) {
        total = total + sp_values[i];
        i = i + 1;
    }
    return total;
}

int sparse_max_index(void) {
    int i;
    int mx;
    if (sp_count == 0) return -1;
    mx = sp_indices[0];
    i = 1;
    while (i < sp_count) {
        if (sp_indices[i] > mx) {
            mx = sp_indices[i];
        }
        i = i + 1;
    }
    return mx;
}

int main(void) {
    int val;
    int ret;

    sparse_init();

    sparse_set(0, 10);
    sparse_set(100, 20);
    sparse_set(500, 30);

    // EXPECT: count: 3
    printf("count: %d\n", sp_count);

    val = sparse_get(0, -1);
    // EXPECT: get 0: 10
    printf("get 0: %d\n", val);

    val = sparse_get(100, -1);
    // EXPECT: get 100: 20
    printf("get 100: %d\n", val);

    val = sparse_get(500, -1);
    // EXPECT: get 500: 30
    printf("get 500: %d\n", val);

    val = sparse_get(999, -1);
    // EXPECT: get 999: -1
    printf("get 999: %d\n", val);

    sparse_set(100, 25);
    val = sparse_get(100, -1);
    // EXPECT: updated 100: 25
    printf("updated 100: %d\n", val);

    // EXPECT: count still: 3
    printf("count still: %d\n", sp_count);

    // EXPECT: sum: 65
    printf("sum: %d\n", sparse_sum());

    // EXPECT: max idx: 500
    printf("max idx: %d\n", sparse_max_index());

    ret = sparse_remove(100);
    // EXPECT: remove 100: 1
    printf("remove 100: %d\n", ret);

    // EXPECT: count: 2
    printf("count: %d\n", sp_count);

    ret = sparse_remove(100);
    // EXPECT: remove again: 0
    printf("remove again: %d\n", ret);

    sparse_set(200, 0);
    sparse_set(300, 0);
    sparse_set(400, 50);
    // EXPECT: before compress: 5
    printf("before compress: %d\n", sp_count);

    sparse_compress();
    // EXPECT: after compress: 3
    printf("after compress: %d\n", sp_count);

    // EXPECT: sum after: 90
    printf("sum after: %d\n", sparse_sum());

    sparse_init();
    // EXPECT: reinit count: 0
    printf("reinit count: %d\n", sp_count);

    val = sparse_get(0, 42);
    // EXPECT: default: 42
    printf("default: %d\n", val);

    return 0;
}
