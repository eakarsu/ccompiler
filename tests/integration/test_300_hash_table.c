int printf(const char *fmt, ...);
// EXPECT: Hash table:\n  bucket[0]:\n  bucket[1]: {1:100}\n  bucket[2]: {18:1800} {10:1000} {2:200}\n  bucket[3]: {3:300}\n  bucket[4]:\n  bucket[5]:\n  bucket[6]:\n  bucket[7]:\nget(1) = 100\nget(2) = 200\nget(10) = 1000\ncontains(3) = 1\ncontains(99) = 0\nafter update(2,999): get(2) = 999\nnum entries = 5\nFrequencies:\n  1: 3\n  2: 1\n  3: 3\n  4: 2\n  5: 1
// Test: hash table with arrays (separate chaining using array pool)

struct Entry {
    int key;
    int value;
    int next;  // index in pool, -1 = end
    int used;
};

struct Entry pool[50];
int pool_top = 0;
int buckets[8];  // head index for each bucket, -1 = empty
int table_size = 8;
int num_entries = 0;

int alloc_entry(void) {
    int idx = pool_top;
    pool_top = pool_top + 1;
    pool[idx].next = -1;
    pool[idx].used = 1;
    return idx;
}

int hash(int key) {
    int h = key % table_size;
    if (h < 0) h = h + table_size;
    return h;
}

void ht_init(void) {
    int i;
    for (i = 0; i < table_size; i++) {
        buckets[i] = -1;
    }
    pool_top = 0;
    num_entries = 0;
}

void ht_put(int key, int value) {
    int h = hash(key);
    // Check if key exists
    int cur = buckets[h];
    while (cur != -1) {
        if (pool[cur].key == key) {
            pool[cur].value = value;
            return;
        }
        cur = pool[cur].next;
    }
    // Insert new
    int idx = alloc_entry();
    pool[idx].key = key;
    pool[idx].value = value;
    pool[idx].next = buckets[h];
    buckets[h] = idx;
    num_entries = num_entries + 1;
}

int ht_get(int key, int *found) {
    int h = hash(key);
    int cur = buckets[h];
    while (cur != -1) {
        if (pool[cur].key == key) {
            *found = 1;
            return pool[cur].value;
        }
        cur = pool[cur].next;
    }
    *found = 0;
    return 0;
}

int ht_contains(int key) {
    int found;
    ht_get(key, &found);
    return found;
}

void ht_print(void) {
    int i;
    for (i = 0; i < table_size; i++) {
        printf("  bucket[%d]:", i);
        int cur = buckets[i];
        while (cur != -1) {
            printf(" {%d:%d}", pool[cur].key, pool[cur].value);
            cur = pool[cur].next;
        }
        printf("\n");
    }
}

int main(void) {
    ht_init();

    // Insert entries
    ht_put(1, 100);
    ht_put(2, 200);
    ht_put(3, 300);
    ht_put(10, 1000);  // same bucket as 2 (10%8=2)
    ht_put(18, 1800);  // same bucket as 2 (18%8=2)

    printf("Hash table:\n");
    ht_print();

    // Get
    int found;
    printf("get(1) = %d\n", ht_get(1, &found));
    printf("get(2) = %d\n", ht_get(2, &found));
    printf("get(10) = %d\n", ht_get(10, &found));

    // Contains
    printf("contains(3) = %d\n", ht_contains(3));
    printf("contains(99) = %d\n", ht_contains(99));

    // Update
    ht_put(2, 999);
    printf("after update(2,999): get(2) = %d\n", ht_get(2, &found));

    printf("num entries = %d\n", num_entries);

    // Use hash table to count frequencies
    ht_init();
    int data[10];
    data[0]=3; data[1]=1; data[2]=4; data[3]=1; data[4]=5;
    data[5]=3; data[6]=3; data[7]=1; data[8]=4; data[9]=2;

    int i;
    for (i = 0; i < 10; i++) {
        int val = ht_get(data[i], &found);
        if (found) {
            ht_put(data[i], val + 1);
        } else {
            ht_put(data[i], 1);
        }
    }

    printf("Frequencies:\n");
    for (i = 1; i <= 5; i++) {
        int freq = ht_get(i, &found);
        if (found) {
            printf("  %d: %d\n", i, freq);
        }
    }

    return 0;
}
