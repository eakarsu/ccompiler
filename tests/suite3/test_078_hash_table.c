int printf(const char *fmt, ...);

/* Simple hash table with open addressing (linear probing), array-based.
   Keys and values are integers. Tombstone sentinel for deletes. */

#define HT_CAP    16
#define HT_EMPTY  -2147483647   /* sentinel: slot never used  (INT_MIN+1) */
#define HT_DEAD   -2147483648   /* sentinel: tombstone (INT_MIN)           */

typedef struct {
    int key;
    int val;
} HEntry;

typedef struct {
    HEntry slots[HT_CAP];
    int    count;  /* live entries */
} HashTable;

void ht_init(HashTable *ht) {
    int i;
    for (i = 0; i < HT_CAP; i++) {
        ht->slots[i].key = HT_EMPTY;
        ht->slots[i].val = 0;
    }
    ht->count = 0;
}

/* Simple hash for positive integers */
int ht_hash(int key) {
    int h = key % HT_CAP;
    if (h < 0) h += HT_CAP;
    return h;
}

/* Insert or update key->val; returns 1 if inserted new, 0 if updated */
int ht_insert(HashTable *ht, int key, int val) {
    int h = ht_hash(key);
    int i, first_dead = -1;
    for (i = 0; i < HT_CAP; i++) {
        int idx = (h + i) % HT_CAP;
        if (ht->slots[idx].key == HT_EMPTY) {
            /* Use first_dead if available, else use this slot */
            if (first_dead != -1) idx = first_dead;
            ht->slots[idx].key = key;
            ht->slots[idx].val = val;
            ht->count++;
            return 1;
        }
        if (ht->slots[idx].key == HT_DEAD) {
            if (first_dead == -1) first_dead = idx;
        } else if (ht->slots[idx].key == key) {
            ht->slots[idx].val = val;
            return 0;
        }
    }
    /* Table full (shouldn't happen in tests) — use first dead if any */
    if (first_dead != -1) {
        ht->slots[first_dead].key = key;
        ht->slots[first_dead].val = val;
        ht->count++;
        return 1;
    }
    return -1;
}

/* Lookup: returns pointer to val or 0 if not found (store result via out) */
int ht_lookup(HashTable *ht, int key, int *out) {
    int h = ht_hash(key);
    int i;
    for (i = 0; i < HT_CAP; i++) {
        int idx = (h + i) % HT_CAP;
        if (ht->slots[idx].key == HT_EMPTY) return 0;
        if (ht->slots[idx].key == HT_DEAD)  continue;
        if (ht->slots[idx].key == key) {
            *out = ht->slots[idx].val;
            return 1;
        }
    }
    return 0;
}

/* Delete: mark slot as tombstone */
int ht_delete(HashTable *ht, int key) {
    int h = ht_hash(key);
    int i;
    for (i = 0; i < HT_CAP; i++) {
        int idx = (h + i) % HT_CAP;
        if (ht->slots[idx].key == HT_EMPTY) return 0;
        if (ht->slots[idx].key == HT_DEAD)  continue;
        if (ht->slots[idx].key == key) {
            ht->slots[idx].key = HT_DEAD;
            ht->count--;
            return 1;
        }
    }
    return 0;
}

int main(void) {
    HashTable ht;
    ht_init(&ht);

    printf("%d\n", ht.count);  /* 0 */
    // EXPECT: 0

    /* Insert */
    printf("%d\n", ht_insert(&ht, 5,  100)); /* 1 (new) */
    // EXPECT: 1
    printf("%d\n", ht_insert(&ht, 21, 200)); /* 1 (new) (21%16=5, collides) */
    // EXPECT: 1
    printf("%d\n", ht_insert(&ht, 37, 300)); /* 1 (new) (37%16=5, collides) */
    // EXPECT: 1
    printf("%d\n", ht_insert(&ht, 10, 400)); /* 1 (new) */
    // EXPECT: 1
    printf("%d\n", ht_insert(&ht, 26, 500)); /* 1 (new) (26%16=10, collides) */
    // EXPECT: 1

    printf("%d\n", ht.count);  /* 5 */
    // EXPECT: 5

    /* Lookup */
    int v;
    printf("%d\n", ht_lookup(&ht, 5, &v));  /* found=1, v=100 */
    // EXPECT: 1
    printf("%d\n", v);
    // EXPECT: 100

    printf("%d\n", ht_lookup(&ht, 21, &v)); /* found=1, v=200 */
    // EXPECT: 1
    printf("%d\n", v);
    // EXPECT: 200

    printf("%d\n", ht_lookup(&ht, 37, &v)); /* found=1, v=300 */
    // EXPECT: 1
    printf("%d\n", v);
    // EXPECT: 300

    printf("%d\n", ht_lookup(&ht, 10, &v)); /* found=1, v=400 */
    // EXPECT: 1
    printf("%d\n", v);
    // EXPECT: 400

    printf("%d\n", ht_lookup(&ht, 26, &v)); /* found=1, v=500 */
    // EXPECT: 1
    printf("%d\n", v);
    // EXPECT: 500

    printf("%d\n", ht_lookup(&ht, 99, &v)); /* not found=0 */
    // EXPECT: 0

    /* Update existing key */
    printf("%d\n", ht_insert(&ht, 5, 999)); /* 0 (update) */
    // EXPECT: 0
    printf("%d\n", ht.count);               /* still 5 */
    // EXPECT: 5
    ht_lookup(&ht, 5, &v);
    printf("%d\n", v);                       /* 999 */
    // EXPECT: 999

    /* Delete */
    printf("%d\n", ht_delete(&ht, 21));     /* 1 */
    // EXPECT: 1
    printf("%d\n", ht.count);               /* 4 */
    // EXPECT: 4
    printf("%d\n", ht_lookup(&ht, 21, &v)); /* 0 (not found) */
    // EXPECT: 0

    /* 37 was probed past 21's slot; must still be findable */
    printf("%d\n", ht_lookup(&ht, 37, &v)); /* 1 */
    // EXPECT: 1
    printf("%d\n", v);                       /* 300 */
    // EXPECT: 300

    /* Delete non-existent */
    printf("%d\n", ht_delete(&ht, 99));     /* 0 */
    // EXPECT: 0

    /* Re-insert into tombstone slot */
    printf("%d\n", ht_insert(&ht, 53, 777)); /* 1 (new, 53%16=5) */
    // EXPECT: 1
    printf("%d\n", ht.count);               /* 5 */
    // EXPECT: 5
    ht_lookup(&ht, 53, &v);
    printf("%d\n", v);                       /* 777 */
    // EXPECT: 777

    /* Delete all five remaining entries */
    ht_delete(&ht, 5);
    ht_delete(&ht, 37);
    ht_delete(&ht, 53);
    ht_delete(&ht, 10);
    ht_delete(&ht, 26);
    printf("%d\n", ht.count);               /* 0 */
    // EXPECT: 0

    return 0;
}
