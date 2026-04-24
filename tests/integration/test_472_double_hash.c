int printf(const char *fmt, ...);
// EXPECT: Size: 7\nGet 5: 50 (found=1)\nGet 21: 210 (found=1)\nGet 37: 370 (found=1)\nGet 99: found=0\nProbes for 5: 1\nProbes for 21: 2\nProbes for 37: 2\nAfter removing 21:\nSize: 6\nGet 21: found=0\nGet 37: 370 (found=1)\nGet 53: 530 (found=1)\nFinal size: 7\nDone

// Double hashing hash table

struct DHEntry {
    int key;
    int value;
    int state;  // 0=empty, 1=occupied, 2=deleted
};

struct DoubleHash {
    struct DHEntry table[16];
    int capacity;
    int size;
};

void dh_init(struct DoubleHash *dh) {
    dh->capacity = 16;
    dh->size = 0;
    int i = 0;
    while (i < 16) {
        dh->table[i].state = 0;
        i = i + 1;
    }
}

int dh_hash1(int key, int cap) {
    int h = key;
    if (h < 0) h = -h;
    return h % cap;
}

int dh_hash2(int key, int cap) {
    int h = key;
    if (h < 0) h = -h;
    // Must be non-zero and coprime with capacity
    return 1 + (h % (cap - 1));
}

int dh_put(struct DoubleHash *dh, int key, int value) {
    if (dh->size >= dh->capacity) return -1;

    int h1 = dh_hash1(key, dh->capacity);
    int h2 = dh_hash2(key, dh->capacity);
    int i = 0;
    while (i < dh->capacity) {
        int idx = (h1 + i * h2) % dh->capacity;
        if (dh->table[idx].state != 1) {
            dh->table[idx].key = key;
            dh->table[idx].value = value;
            dh->table[idx].state = 1;
            dh->size = dh->size + 1;
            return idx;
        }
        if (dh->table[idx].key == key) {
            dh->table[idx].value = value;
            return idx;
        }
        i = i + 1;
    }
    return -1;
}

int dh_get(struct DoubleHash *dh, int key, int *found) {
    int h1 = dh_hash1(key, dh->capacity);
    int h2 = dh_hash2(key, dh->capacity);
    int i = 0;
    while (i < dh->capacity) {
        int idx = (h1 + i * h2) % dh->capacity;
        if (dh->table[idx].state == 0) {
            *found = 0;
            return 0;
        }
        if (dh->table[idx].state == 1 && dh->table[idx].key == key) {
            *found = 1;
            return dh->table[idx].value;
        }
        i = i + 1;
    }
    *found = 0;
    return 0;
}

int dh_remove(struct DoubleHash *dh, int key) {
    int h1 = dh_hash1(key, dh->capacity);
    int h2 = dh_hash2(key, dh->capacity);
    int i = 0;
    while (i < dh->capacity) {
        int idx = (h1 + i * h2) % dh->capacity;
        if (dh->table[idx].state == 0) return 0;
        if (dh->table[idx].state == 1 && dh->table[idx].key == key) {
            dh->table[idx].state = 2;  // deleted tombstone
            dh->size = dh->size - 1;
            return 1;
        }
        i = i + 1;
    }
    return 0;
}

int dh_probe_count(struct DoubleHash *dh, int key) {
    int h1 = dh_hash1(key, dh->capacity);
    int h2 = dh_hash2(key, dh->capacity);
    int i = 0;
    while (i < dh->capacity) {
        int idx = (h1 + i * h2) % dh->capacity;
        if (dh->table[idx].state == 0) return i + 1;
        if (dh->table[idx].state == 1 && dh->table[idx].key == key) return i + 1;
        i = i + 1;
    }
    return dh->capacity;
}

int main(void) {
    struct DoubleHash dh;
    dh_init(&dh);

    dh_put(&dh, 5, 50);
    dh_put(&dh, 21, 210);
    dh_put(&dh, 37, 370);
    dh_put(&dh, 10, 100);
    dh_put(&dh, 26, 260);
    dh_put(&dh, 15, 150);
    dh_put(&dh, 31, 310);

    printf("Size: %d\n", dh.size);

    int found;
    int v;
    v = dh_get(&dh, 5, &found);
    printf("Get 5: %d (found=%d)\n", v, found);
    v = dh_get(&dh, 21, &found);
    printf("Get 21: %d (found=%d)\n", v, found);
    v = dh_get(&dh, 37, &found);
    printf("Get 37: %d (found=%d)\n", v, found);
    v = dh_get(&dh, 99, &found);
    printf("Get 99: found=%d\n", found);

    printf("Probes for 5: %d\n", dh_probe_count(&dh, 5));
    printf("Probes for 21: %d\n", dh_probe_count(&dh, 21));
    printf("Probes for 37: %d\n", dh_probe_count(&dh, 37));

    // Remove and re-search
    dh_remove(&dh, 21);
    printf("After removing 21:\n");
    printf("Size: %d\n", dh.size);
    v = dh_get(&dh, 21, &found);
    printf("Get 21: found=%d\n", found);
    v = dh_get(&dh, 37, &found);
    printf("Get 37: %d (found=%d)\n", v, found);

    // Insert after delete
    dh_put(&dh, 53, 530);
    v = dh_get(&dh, 53, &found);
    printf("Get 53: %d (found=%d)\n", v, found);
    printf("Final size: %d\n", dh.size);

    printf("Done\n");
    return 0;
}
