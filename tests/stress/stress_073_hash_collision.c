// stress_073_hash_collision.c - Hash table with forced collisions
// EXPECT: insert_40_ok=1
// EXPECT: all_found=40
// EXPECT: bucket_0_len=5
// EXPECT: bucket_1_len=5
// EXPECT: bucket_2_len=5
// EXPECT: bucket_3_len=5
// EXPECT: bucket_4_len=5
// EXPECT: bucket_5_len=5
// EXPECT: bucket_6_len=5
// EXPECT: bucket_7_len=5
// EXPECT: max_chain=5
// EXPECT: delete_mid_ok=1
// EXPECT: delete_head_ok=1
// EXPECT: after_delete_count=30
// EXPECT: rehash_count=30
// EXPECT: rehash_all_found=30

int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

typedef struct Entry {
    int key;
    int value;
    struct Entry *next;
} Entry;

typedef struct {
    Entry **buckets;
    int num_buckets;
    int count;
} HashMap;

HashMap *hm_create(int nbuckets) {
    HashMap *hm = (HashMap *)malloc(sizeof(HashMap));
    hm->num_buckets = nbuckets;
    hm->count = 0;
    hm->buckets = (Entry **)malloc(sizeof(Entry *) * nbuckets);
    int i;
    for (i = 0; i < nbuckets; i++) hm->buckets[i] = 0;
    return hm;
}

int hm_hash(HashMap *hm, int key) {
    int h = key;
    if (h < 0) h = -h;
    return h % hm->num_buckets;
}

void hm_insert(HashMap *hm, int key, int value) {
    int idx = hm_hash(hm, key);
    // Check if key exists
    Entry *e = hm->buckets[idx];
    while (e) {
        if (e->key == key) {
            e->value = value;
            return;
        }
        e = e->next;
    }
    // Insert new
    Entry *ne = (Entry *)malloc(sizeof(Entry));
    ne->key = key;
    ne->value = value;
    ne->next = hm->buckets[idx];
    hm->buckets[idx] = ne;
    hm->count++;
}

int hm_get(HashMap *hm, int key, int *out) {
    int idx = hm_hash(hm, key);
    Entry *e = hm->buckets[idx];
    while (e) {
        if (e->key == key) {
            *out = e->value;
            return 1;
        }
        e = e->next;
    }
    return 0;
}

int hm_delete(HashMap *hm, int key) {
    int idx = hm_hash(hm, key);
    Entry *e = hm->buckets[idx];
    Entry *prev = 0;
    while (e) {
        if (e->key == key) {
            if (prev) prev->next = e->next;
            else hm->buckets[idx] = e->next;
            free(e);
            hm->count--;
            return 1;
        }
        prev = e;
        e = e->next;
    }
    return 0;
}

int bucket_len(HashMap *hm, int idx) {
    int c = 0;
    Entry *e = hm->buckets[idx];
    while (e) { c++; e = e->next; }
    return c;
}

int max_chain_len(HashMap *hm) {
    int mx = 0;
    int i;
    for (i = 0; i < hm->num_buckets; i++) {
        int l = bucket_len(hm, i);
        if (l > mx) mx = l;
    }
    return mx;
}

// Rehash: create new table with different size, move all entries
HashMap *hm_rehash(HashMap *old, int new_size) {
    HashMap *nw = hm_create(new_size);
    int i;
    for (i = 0; i < old->num_buckets; i++) {
        Entry *e = old->buckets[i];
        while (e) {
            hm_insert(nw, e->key, e->value);
            e = e->next;
        }
    }
    return nw;
}

void hm_free(HashMap *hm) {
    int i;
    for (i = 0; i < hm->num_buckets; i++) {
        Entry *e = hm->buckets[i];
        while (e) {
            Entry *nxt = e->next;
            free(e);
            e = nxt;
        }
    }
    free(hm->buckets);
    free(hm);
}

int main(void) {
    // 8 buckets, insert 40 keys that are designed to distribute evenly
    // Keys: 0,1,2,...,39. hash = key % 8.
    // bucket 0: 0,8,16,24,32 (5 entries)
    // bucket 1: 1,9,17,25,33 (5 entries)
    // etc.
    HashMap *hm = hm_create(8);
    int i;
    for (i = 0; i < 40; i++) {
        hm_insert(hm, i, i * 10);
    }
    printf("insert_40_ok=%d\n", hm->count == 40);

    // Verify all can be found
    int found = 0;
    int val;
    for (i = 0; i < 40; i++) {
        if (hm_get(hm, i, &val) && val == i * 10) found++;
    }
    printf("all_found=%d\n", found);

    // Print chain lengths for each bucket
    for (i = 0; i < 8; i++) {
        printf("bucket_%d_len=%d\n", i, bucket_len(hm, i));
    }

    printf("max_chain=%d\n", max_chain_len(hm));

    // Delete from middle of chain: key 16 is in bucket 0 (chain: 32->24->16->8->0)
    int del_ok = hm_delete(hm, 16);
    // Verify 16 is gone but neighbors remain
    int v1;
    int f8 = hm_get(hm, 8, &v1);
    int f24 = hm_get(hm, 24, &v1);
    int f16 = hm_get(hm, 16, &v1);
    printf("delete_mid_ok=%d\n", del_ok && f8 && f24 && !f16);

    // Delete head of chain: key 32 (was head of bucket 0 chain)
    del_ok = hm_delete(hm, 32);
    int f32 = hm_get(hm, 32, &v1);
    int f0 = hm_get(hm, 0, &v1);
    printf("delete_head_ok=%d\n", del_ok && !f32 && f0);

    // Delete 8 more keys (one from each bucket to make it interesting)
    for (i = 0; i < 8; i++) {
        hm_delete(hm, i + 8); // delete 8,9,10,11,12,13,14,15
    }
    printf("after_delete_count=%d\n", hm->count);

    // Rehash into 16 buckets
    HashMap *hm2 = hm_rehash(hm, 16);
    printf("rehash_count=%d\n", hm2->count);

    // Verify all remaining keys found in rehashed table
    int rfound = 0;
    for (i = 0; i < 40; i++) {
        int v;
        if (hm_get(hm, i, &v)) {
            // This key should exist in rehashed table too
            int v2;
            if (hm_get(hm2, i, &v2) && v2 == v) rfound++;
        }
    }
    printf("rehash_all_found=%d\n", rfound);

    hm_free(hm);
    hm_free(hm2);
    return 0;
}
