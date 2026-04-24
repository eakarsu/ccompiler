int printf(const char *fmt, ...);
// EXPECT: Get 5: 50 (found=1)\nGet 10: 100 (found=1)\nGet 21: 210 (found=1)\nGet 99: 0 (found=0)\nCount: 5\nUpdated 5: 55\nUpdated 21: 211\nCount after updates: 5
// Test: Hash table with open addressing (linear probing)

int ht_keys[64];
int ht_vals[64];
int ht_used[64];
int ht_size;

void ht_init(int size) {
    ht_size = size;
    int i;
    for (i = 0; i < size; i++) {
        ht_keys[i] = 0;
        ht_vals[i] = 0;
        ht_used[i] = 0;
    }
}

int ht_hash(int key) {
    int h = key % ht_size;
    if (h < 0) h = h + ht_size;
    return h;
}

int ht_insert(int key, int val) {
    int h = ht_hash(key);
    int i;
    for (i = 0; i < ht_size; i++) {
        int idx = (h + i) % ht_size;
        if (ht_used[idx] == 0) {
            ht_keys[idx] = key;
            ht_vals[idx] = val;
            ht_used[idx] = 1;
            return 1;
        }
        if (ht_used[idx] == 1 && ht_keys[idx] == key) {
            ht_vals[idx] = val;
            return 1;
        }
    }
    return 0;
}

int ht_get(int key, int *found) {
    int h = ht_hash(key);
    int i;
    for (i = 0; i < ht_size; i++) {
        int idx = (h + i) % ht_size;
        if (ht_used[idx] == 0) {
            *found = 0;
            return 0;
        }
        if (ht_used[idx] == 1 && ht_keys[idx] == key) {
            *found = 1;
            return ht_vals[idx];
        }
    }
    *found = 0;
    return 0;
}

int ht_count(void) {
    int c = 0;
    int i;
    for (i = 0; i < ht_size; i++) {
        if (ht_used[i]) c++;
    }
    return c;
}

int main(void) {
    ht_init(16);

    ht_insert(5, 50);
    ht_insert(10, 100);
    ht_insert(15, 150);
    ht_insert(21, 210);
    ht_insert(37, 370);

    int found;
    int val;

    val = ht_get(5, &found);
    printf("Get 5: %d (found=%d)\n", val, found);
    val = ht_get(10, &found);
    printf("Get 10: %d (found=%d)\n", val, found);
    val = ht_get(21, &found);
    printf("Get 21: %d (found=%d)\n", val, found);
    val = ht_get(99, &found);
    printf("Get 99: %d (found=%d)\n", val, found);

    printf("Count: %d\n", ht_count());

    ht_insert(5, 55);
    val = ht_get(5, &found);
    printf("Updated 5: %d\n", val);

    ht_insert(21, 211);
    val = ht_get(21, &found);
    printf("Updated 21: %d\n", val);

    printf("Count after updates: %d\n", ht_count());

    return 0;
}
