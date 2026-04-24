int printf(const char *fmt, ...);

int bv[8];

void bv_init(void) {
    int i;
    i = 0;
    while (i < 8) {
        bv[i] = 0;
        i = i + 1;
    }
}

void bv_set(int bit) {
    int word;
    int offset;
    word = bit / 32;
    offset = bit % 32;
    bv[word] = bv[word] | (1 << offset);
}

void bv_clear(int bit) {
    int word;
    int offset;
    word = bit / 32;
    offset = bit % 32;
    bv[word] = bv[word] & (~(1 << offset));
}

int bv_test(int bit) {
    int word;
    int offset;
    word = bit / 32;
    offset = bit % 32;
    if ((bv[word] & (1 << offset)) != 0) {
        return 1;
    }
    return 0;
}

int popcount_word(int x) {
    int count;
    int tmp;
    count = 0;
    tmp = x;
    while (tmp != 0) {
        count = count + (tmp & 1);
        tmp = (tmp >> 1) & 0x7FFFFFFF;
        /* mask to avoid sign extension */
    }
    return count;
}

int bv_popcount(void) {
    int total;
    int i;
    total = 0;
    i = 0;
    while (i < 8) {
        total = total + popcount_word(bv[i]);
        i = i + 1;
    }
    return total;
}

int bv2[8];

void bv_union(int dst[], int a[], int b[]) {
    int i;
    i = 0;
    while (i < 8) {
        dst[i] = a[i] | b[i];
        i = i + 1;
    }
}

void bv_intersect(int dst[], int a[], int b[]) {
    int i;
    i = 0;
    while (i < 8) {
        dst[i] = a[i] & b[i];
        i = i + 1;
    }
}

int popcount_arr(int arr[]) {
    int total;
    int i;
    total = 0;
    i = 0;
    while (i < 8) {
        total = total + popcount_word(arr[i]);
        i = i + 1;
    }
    return total;
}

int result[8];

int main(void) {
    int i;
    int ret;

    bv_init();

    bv_set(0);
    bv_set(5);
    bv_set(31);
    bv_set(32);
    bv_set(63);

    ret = bv_test(0);
    // EXPECT: bit 0: 1
    printf("bit 0: %d\n", ret);

    ret = bv_test(5);
    // EXPECT: bit 5: 1
    printf("bit 5: %d\n", ret);

    ret = bv_test(31);
    // EXPECT: bit 31: 1
    printf("bit 31: %d\n", ret);

    ret = bv_test(32);
    // EXPECT: bit 32: 1
    printf("bit 32: %d\n", ret);

    ret = bv_test(1);
    // EXPECT: bit 1: 0
    printf("bit 1: %d\n", ret);

    // EXPECT: popcount: 5
    printf("popcount: %d\n", bv_popcount());

    bv_clear(5);
    ret = bv_test(5);
    // EXPECT: after clear 5: 0
    printf("after clear 5: %d\n", ret);

    // EXPECT: popcount: 4
    printf("popcount: %d\n", bv_popcount());

    i = 0;
    while (i < 8) {
        bv2[i] = 0;
        i = i + 1;
    }
    bv2[0] = bv2[0] | (1 << 0);
    bv2[0] = bv2[0] | (1 << 3);
    bv2[0] = bv2[0] | (1 << 5);
    bv2[1] = bv2[1] | (1 << 0);

    bv_union(result, bv, bv2);
    // EXPECT: union pop: 6
    printf("union pop: %d\n", popcount_arr(result));

    bv_intersect(result, bv, bv2);
    // EXPECT: intersect pop: 2
    printf("intersect pop: %d\n", popcount_arr(result));

    bv_init();
    i = 0;
    while (i < 16) {
        bv_set(i * 2);
        i = i + 1;
    }
    // EXPECT: even bits pop: 16
    printf("even bits pop: %d\n", bv_popcount());

    ret = bv_test(4);
    // EXPECT: bit 4: 1
    printf("bit 4: %d\n", ret);

    ret = bv_test(7);
    // EXPECT: bit 7: 0
    printf("bit 7: %d\n", ret);

    bv_set(7);
    // EXPECT: after set 7 pop: 17
    printf("after set 7 pop: %d\n", bv_popcount());

    bv_clear(0);
    bv_clear(2);
    // EXPECT: after clear 2 pop: 15
    printf("after clear 2 pop: %d\n", bv_popcount());

    return 0;
}
