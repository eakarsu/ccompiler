int printf(const char *fmt, ...);

/* Hash functions: DJB2 and FNV-1a style (simplified for ints) */

/* DJB2-like hash on int array */
int hash_djb2(int *data, int len) {
    int hash;
    int i;
    hash = 5381;
    i = 0;
    while (i < len) {
        hash = ((hash << 5) + hash) + data[i];
        i = i + 1;
    }
    if (hash < 0) hash = 0 - hash;
    return hash % 10000;
}

/* FNV-1a-like hash on int array (simplified) */
int hash_fnv1a(int *data, int len) {
    int hash;
    int i;
    hash = 2166;
    i = 0;
    while (i < len) {
        hash = hash ^ data[i];
        hash = (hash * 257) & 0xFFFF;
        i = i + 1;
    }
    if (hash < 0) hash = 0 - hash;
    return hash % 10000;
}

/* Simple weighted sum hash */
int hash_sum(int *data, int len) {
    int s;
    int i;
    s = 0;
    i = 0;
    while (i < len) {
        s = s + data[i] * (i + 1);
        i = i + 1;
    }
    if (s < 0) s = 0 - s;
    return s % 10000;
}

/* Rotate hash */
int hash_rotate(int *data, int len) {
    int h;
    int i;
    h = 0;
    i = 0;
    while (i < len) {
        h = ((h << 4) | ((h >> 12) & 0xF)) & 0xFFFF;
        h = h ^ data[i];
        i = i + 1;
    }
    if (h < 0) h = 0 - h;
    return h % 10000;
}

/* Simple hash table */
struct hash_table {
    int buckets[16];
    int used[16];
    int collisions;
};

void ht_init(struct hash_table *ht) {
    int i;
    i = 0;
    while (i < 16) {
        ht->buckets[i] = 0;
        ht->used[i] = 0;
        i = i + 1;
    }
    ht->collisions = 0;
}

void ht_insert(struct hash_table *ht, int val, int hash) {
    int idx;
    idx = hash % 16;
    if (idx < 0) idx = 0 - idx;
    if (ht->used[idx]) {
        ht->collisions = ht->collisions + 1;
    }
    ht->buckets[idx] = val;
    ht->used[idx] = 1;
}

int ht_count_used(struct hash_table *ht) {
    int c;
    int i;
    c = 0;
    i = 0;
    while (i < 16) {
        if (ht->used[i]) c = c + 1;
        i = i + 1;
    }
    return c;
}

int main() {
    int data1[4];
    int data2[4];
    int data4[1];
    int h1;
    int h2;
    int h3;
    int h4;
    struct hash_table ht;
    int i;
    int j;
    int hashes[8];
    int distinct;
    int is_dup;

    data1[0] = 1; data1[1] = 2; data1[2] = 3; data1[3] = 4;

    /* Test 1: DJB2 hash */
    h1 = hash_djb2(data1, 4);
    printf("djb2: %d\n", h1);
    // EXPECT: djb2: 1823

    /* Test 2: FNV-1a hash on [1,2,3,4] */
    /* hash=2166
       i=0: hash=2166^1=2167, hash=(2167*257)&0xFFFF
       2167*257 = 2167*256+2167 = 554752+2167 = 556919
       556919 & 0xFFFF = 556919 % 65536 = 556919 - 8*65536 = 556919 - 524288 = 32631
       i=1: hash=32631^2=32629, hash=(32629*257)&0xFFFF
       32629*257 = 32629*256+32629 = 8352,9024+32629 wait:
       32629*256 = 8353024, +32629 = 8385653
       8385653 & 0xFFFF = 8385653 % 65536 = 8385653/65536=127.xx
       127*65536=8323072, 8385653-8323072=62581
       Actually 128*65536=8388608, that's > 8385653. So 127*65536=8323072.
       8385653-8323072=62581
       i=2: hash=62581^3=62582, hash=(62582*257)&0xFFFF
       62582*256=16020992, +62582=16083574
       16083574%65536: 16083574/65536=245.xx, 245*65536=16056320, 16083574-16056320=27254
       i=3: hash=27254^4=27250, hash=(27250*257)&0xFFFF
       27250*256=6976000, +27250=7003250
       7003250%65536: 7003250/65536=106.xx, 106*65536=6946816, 7003250-6946816=56434
       hash=56434, positive, 56434%10000=6434 */
    h2 = hash_fnv1a(data1, 4);
    printf("fnv1a: %d\n", h2);
    // EXPECT: fnv1a: 6434

    /* Test 3: Sum hash on [1,2,3,4] */
    /* s = 1*1 + 2*2 + 3*3 + 4*4 = 1+4+9+16 = 30 */
    h3 = hash_sum(data1, 4);
    printf("sum: %d\n", h3);
    // EXPECT: sum: 30

    /* Test 4: Rotate hash on [1,2,3,4] */
    /* h=0
       i=0: rotl4(0)=0, h=0^1=1
       i=1: rotl4(1)=16, h=16^2=18
       i=2: rotl4(18): 18<<4=288, (18>>12)&0xF=0, h=288|0=288, h=288^3=291
       i=3: rotl4(291): 291<<4=4656, (291>>12)&0xF=0, h=4656, h=4656^4=4660
       4660%10000=4660 */
    h4 = hash_rotate(data1, 4);
    printf("rotate: %d\n", h4);
    // EXPECT: rotate: 4660

    /* Test 5: Different inputs -> different djb2 */
    data2[0] = 4; data2[1] = 3; data2[2] = 2; data2[3] = 1;
    printf("diff: %d\n", hash_djb2(data1, 4) != hash_djb2(data2, 4) ? 1 : 0);
    // EXPECT: diff: 1

    /* Test 6: Same input always same hash */
    printf("stable: %d\n", hash_djb2(data1, 4) == hash_djb2(data1, 4) ? 1 : 0);
    // EXPECT: stable: 1

    /* Test 7: Empty input */
    printf("empty_djb2: %d\n", hash_djb2(data1, 0));
    // EXPECT: empty_djb2: 5381

    printf("empty_fnv: %d\n", hash_fnv1a(data1, 0));
    // EXPECT: empty_fnv: 2166

    printf("empty_sum: %d\n", hash_sum(data1, 0));
    // EXPECT: empty_sum: 0

    /* Test 8: Single element */
    data4[0] = 42;
    /* djb2: hash = (5381<<5)+5381+42 = 172192+5381+42 = 177615, 177615%10000=7615 */
    printf("single: %d\n", hash_djb2(data4, 1));
    // EXPECT: single: 7615

    /* Test 9: Hash table */
    ht_init(&ht);
    i = 0;
    while (i < 4) {
        data4[0] = (i + 1) * 10;
        ht_insert(&ht, data4[0], hash_djb2(data4, 1));
        i = i + 1;
    }
    i = 0;
    while (i < 4) {
        data4[0] = (i + 1) * 100;
        ht_insert(&ht, data4[0], hash_djb2(data4, 1));
        i = i + 1;
    }
    printf("ht_used: %d\n", ht_count_used(&ht));
    // EXPECT: ht_used: 6
    printf("ht_coll: %d\n", ht.collisions);
    // EXPECT: ht_coll: 2

    /* Test 10: Count distinct hashes for inputs 1..8 */
    i = 0;
    while (i < 8) {
        data4[0] = i + 1;
        hashes[i] = hash_djb2(data4, 1);
        i = i + 1;
    }
    distinct = 0;
    i = 0;
    while (i < 8) {
        is_dup = 0;
        j = 0;
        while (j < i) {
            if (hashes[j] == hashes[i]) is_dup = 1;
            j = j + 1;
        }
        if (!is_dup) distinct = distinct + 1;
        i = i + 1;
    }
    printf("distinct: %d\n", distinct);
    // EXPECT: distinct: 8

    printf("hash_done\n");
    // EXPECT: hash_done
    return 0;
}
