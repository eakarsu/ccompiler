int printf(const char *fmt, ...);
// EXPECT: djb2:4730,4873,3428,2797,1991,7615,3605,3905|sum:389,420,15,600,210,42|xor:3232,3216,2792,2304,2240|poly:5403,2049,6088,2597|fnv:763,6975,4484,3297|ck_xor:45,1,0|ck_add:133,15,210|fletcher:38534,8975|same_hash=1|diff_hash=1|ht_collis=12|ht_maxchn=4|ht_empt=4|ht_used=4|distrang=9130|uniq_hash=16|hash_avg=4971|ht32_collis= 5,empty=      21|zerohash:djb2=3605,xor=0,poly=0|single:7615,42,336|high:3905,1020|avalanch:h1=6830,h2=2767,diff=4063,changed=1|seq:8155,3428,2441,4012|sum_collis=1,djb2_collis=0|stable=1,1,1,1|hash_0=7573,0,0|sum_ht:collis=  8,used= 8|end

/* Simple hash function implementations and analysis */

/* DJB2-like hash (simplified for int arrays) */
int hash_djb2(int *data, int len) {
    int hash;
    int i;
    hash = 5381;
    i = 0;
    while (i < len) {
        hash = ((hash << 5) + hash) + data[i]; /* hash * 33 + data[i] */
        i = i + 1;
    }
    /* Keep it positive and manageable */
    if (hash < 0) hash = 0 - hash;
    return hash % 10000;
}

/* Simple sum hash */
int hash_sum(int *data, int len) {
    int s;
    int i;
    s = 0;
    i = 0;
    while (i < len) {
        s = s + data[i];
        i = i + 1;
    }
    if (s < 0) s = 0 - s;
    return s % 10000;
}

/* XOR-based hash */
int hash_xor(int *data, int len) {
    int h;
    int i;
    h = 0;
    i = 0;
    while (i < len) {
        h = h ^ data[i];
        h = ((h << 3) | ((h >> 29) & 7)); /* rotate left by 3 */
        i = i + 1;
    }
    if (h < 0) h = 0 - h;
    return h % 10000;
}

/* Polynomial rolling hash */
int hash_poly(int *data, int len) {
    int h;
    int i;
    int base;
    base = 31;
    h = 0;
    i = 0;
    while (i < len) {
        h = h * base + data[i];
        h = h % 100003; /* keep it manageable */
        i = i + 1;
    }
    if (h < 0) h = 0 - h;
    return h % 10000;
}

/* FNV-like hash */
int hash_fnv(int *data, int len) {
    int h;
    int i;
    h = 2166136;  /* truncated FNV offset basis */
    i = 0;
    while (i < len) {
        h = h ^ data[i];
        h = h * 16777; /* truncated FNV prime */
        h = h % 100003;
        i = i + 1;
    }
    if (h < 0) h = 0 - h;
    return h % 10000;
}

/* Checksum: simple XOR of all bytes */
int checksum_xor(int *data, int len) {
    int cs;
    int i;
    cs = 0;
    i = 0;
    while (i < len) {
        cs = cs ^ data[i];
        i = i + 1;
    }
    return cs;
}

/* Additive checksum mod 256 */
int checksum_add(int *data, int len) {
    int cs;
    int i;
    cs = 0;
    i = 0;
    while (i < len) {
        cs = (cs + data[i]) & 255;
        i = i + 1;
    }
    return cs;
}

/* Fletcher-like checksum */
int checksum_fletcher(int *data, int len) {
    int sum1;
    int sum2;
    int i;
    sum1 = 0;
    sum2 = 0;
    i = 0;
    while (i < len) {
        sum1 = (sum1 + data[i]) % 255;
        sum2 = (sum2 + sum1) % 255;
        i = i + 1;
    }
    return (sum2 << 8) | sum1;
}

/* Hash table for collision testing */
int ht_buckets[64];
int ht_counts[64];
int ht_size;

void ht_init(int size) {
    int i;
    ht_size = size;
    i = 0;
    while (i < 64) {
        ht_buckets[i] = -1;
        ht_counts[i] = 0;
        i = i + 1;
    }
}

void ht_insert(int hash_val) {
    int bucket;
    bucket = hash_val % ht_size;
    if (bucket < 0) bucket = 0 - bucket;
    ht_counts[bucket] = ht_counts[bucket] + 1;
    ht_buckets[bucket] = hash_val;
}

int ht_collisions(void) {
    int i;
    int cnt;
    cnt = 0;
    i = 0;
    while (i < ht_size) {
        if (ht_counts[i] > 1) {
            cnt = cnt + (ht_counts[i] - 1);
        }
        i = i + 1;
    }
    return cnt;
}

int ht_max_chain(void) {
    int i;
    int m;
    m = 0;
    i = 0;
    while (i < ht_size) {
        if (ht_counts[i] > m) {
            m = ht_counts[i];
        }
        i = i + 1;
    }
    return m;
}

int ht_empty_buckets(void) {
    int i;
    int cnt;
    cnt = 0;
    i = 0;
    while (i < ht_size) {
        if (ht_counts[i] == 0) {
            cnt = cnt + 1;
        }
        i = i + 1;
    }
    return cnt;
}

int ht_used_buckets(void) {
    return ht_size - ht_empty_buckets();
}

/* Distribution analysis: how spread are the hash values? */
int distribution_range(int *hashes, int count) {
    int min_v;
    int max_v;
    int i;
    min_v = hashes[0];
    max_v = hashes[0];
    i = 1;
    while (i < count) {
        if (hashes[i] < min_v) min_v = hashes[i];
        if (hashes[i] > max_v) max_v = hashes[i];
        i = i + 1;
    }
    return max_v - min_v;
}

/* Count unique hash values */
int count_unique(int *hashes, int count) {
    int i;
    int j;
    int unique;
    int found;
    unique = 0;
    i = 0;
    while (i < count) {
        found = 0;
        j = 0;
        while (j < i) {
            if (hashes[j] == hashes[i]) {
                found = 1;
                j = i; /* break */
            }
            j = j + 1;
        }
        if (!found) {
            unique = unique + 1;
        }
        i = i + 1;
    }
    return unique;
}

/* Average of hash values */
int hash_average(int *hashes, int count) {
    int s;
    int i;
    s = 0;
    i = 0;
    while (i < count) {
        s = s + hashes[i];
        i = i + 1;
    }
    return s / count;
}

int main(void) {
    int d1[4];
    int d2[4];
    int d3[5];
    int d4[3];
    int d5[6];
    int d6[1];
    int d7[4];
    int d8[4];
    int hashes[16];
    int h;
    int i;

    /* Test data */
    d1[0] = 72; d1[1] = 101; d1[2] = 108; d1[3] = 108; /* "Hell" */
    d2[0] = 87; d2[1] = 111; d2[2] = 114; d2[3] = 108; /* "Worl" */
    d3[0] = 1; d3[1] = 2; d3[2] = 3; d3[3] = 4; d3[4] = 5;
    d4[0] = 100; d4[1] = 200; d4[2] = 300;
    d5[0] = 10; d5[1] = 20; d5[2] = 30; d5[3] = 40; d5[4] = 50; d5[5] = 60;
    d6[0] = 42;
    d7[0] = 0; d7[1] = 0; d7[2] = 0; d7[3] = 0;
    d8[0] = 255; d8[1] = 255; d8[2] = 255; d8[3] = 255;

    /* DJB2 hashes */
    printf("%c%c%c%c%c", 'd', 'j', 'b', '2', ':');
    printf("%d", hash_djb2(d1, 4));
    printf("%c", ',');
    printf("%d", hash_djb2(d2, 4));
    printf("%c", ',');
    printf("%d", hash_djb2(d3, 5));
    printf("%c", ',');
    printf("%d", hash_djb2(d4, 3));
    printf("%c", ',');
    printf("%d", hash_djb2(d5, 6));
    printf("%c", ',');
    printf("%d", hash_djb2(d6, 1));
    printf("%c", ',');
    printf("%d", hash_djb2(d7, 4));
    printf("%c", ',');
    printf("%d", hash_djb2(d8, 4));

    /* Sum hashes */
    printf("%c%c%c%c%c", '|', 's', 'u', 'm', ':');
    printf("%d", hash_sum(d1, 4));
    printf("%c", ',');
    printf("%d", hash_sum(d2, 4));
    printf("%c", ',');
    printf("%d", hash_sum(d3, 5));
    printf("%c", ',');
    printf("%d", hash_sum(d4, 3));
    printf("%c", ',');
    printf("%d", hash_sum(d5, 6));
    printf("%c", ',');
    printf("%d", hash_sum(d6, 1));

    /* XOR hashes */
    printf("%c%c%c%c%c", '|', 'x', 'o', 'r', ':');
    printf("%d", hash_xor(d1, 4));
    printf("%c", ',');
    printf("%d", hash_xor(d2, 4));
    printf("%c", ',');
    printf("%d", hash_xor(d3, 5));
    printf("%c", ',');
    printf("%d", hash_xor(d4, 3));
    printf("%c", ',');
    printf("%d", hash_xor(d5, 6));

    /* Polynomial hashes */
    printf("%c%c%c%c%c%c", '|', 'p', 'o', 'l', 'y', ':');
    printf("%d", hash_poly(d1, 4));
    printf("%c", ',');
    printf("%d", hash_poly(d2, 4));
    printf("%c", ',');
    printf("%d", hash_poly(d3, 5));
    printf("%c", ',');
    printf("%d", hash_poly(d4, 3));

    /* FNV hashes */
    printf("%c%c%c%c%c", '|', 'f', 'n', 'v', ':');
    printf("%d", hash_fnv(d1, 4));
    printf("%c", ',');
    printf("%d", hash_fnv(d2, 4));
    printf("%c", ',');
    printf("%d", hash_fnv(d3, 5));
    printf("%c", ',');
    printf("%d", hash_fnv(d4, 3));

    /* Checksums */
    printf("%c%c%c%c%c%c%c%c", '|', 'c', 'k', '_', 'x', 'o', 'r', ':');
    printf("%d", checksum_xor(d1, 4));
    printf("%c", ',');
    printf("%d", checksum_xor(d3, 5));
    printf("%c", ',');
    printf("%d", checksum_xor(d7, 4));

    printf("%c%c%c%c%c%c%c%c", '|', 'c', 'k', '_', 'a', 'd', 'd', ':');
    printf("%d", checksum_add(d1, 4));
    printf("%c", ',');
    printf("%d", checksum_add(d3, 5));
    printf("%c", ',');
    printf("%d", checksum_add(d5, 6));

    printf("%c%c%c%c%c%c%c%c%c%c", '|', 'f', 'l', 'e', 't', 'c', 'h', 'e', 'r', ':');
    printf("%d", checksum_fletcher(d1, 4));
    printf("%c", ',');
    printf("%d", checksum_fletcher(d3, 5));

    /* Same input same hash test */
    printf("%c%c%c%c%c%c%c%c%c%c%c", '|', 's', 'a', 'm', 'e', '_', 'h', 'a', 's', 'h', '=');
    printf("%d", (hash_djb2(d1, 4) == hash_djb2(d1, 4)));

    /* Different input different hash test */
    printf("%c%c%c%c%c%c%c%c%c%c%c", '|', 'd', 'i', 'f', 'f', '_', 'h', 'a', 's', 'h', '=');
    printf("%d", (hash_djb2(d1, 4) != hash_djb2(d2, 4)));

    /* Collision testing with hash table */
    /* Hash 16 different data arrays into 8 buckets using djb2 */
    ht_init(8);
    /* Generate some test data and hash */
    {
        int td[4];
        i = 0;
        while (i < 16) {
            td[0] = i * 7 + 3;
            td[1] = i * 13 + 5;
            td[2] = i * 3 + 1;
            td[3] = i * 11 + 7;
            h = hash_djb2(td, 4);
            hashes[i] = h;
            ht_insert(h);
            i = i + 1;
        }
    }

    printf("%c%c%c%c%c%c%c%c%c%c%c", '|', 'h', 't', '_', 'c', 'o', 'l', 'l', 'i', 's', '=');
    printf("%d", ht_collisions());

    printf("%c%c%c%c%c%c%c%c%c%c%c", '|', 'h', 't', '_', 'm', 'a', 'x', 'c', 'h', 'n', '=');
    printf("%d", ht_max_chain());

    printf("%c%c%c%c%c%c%c%c%c", '|', 'h', 't', '_', 'e', 'm', 'p', 't', '=');
    printf("%d", ht_empty_buckets());

    printf("%c%c%c%c%c%c%c%c%c", '|', 'h', 't', '_', 'u', 's', 'e', 'd', '=');
    printf("%d", ht_used_buckets());

    /* Distribution analysis */
    printf("%c%c%c%c%c%c%c%c%c%c", '|', 'd', 'i', 's', 't', 'r', 'a', 'n', 'g', '=');
    printf("%d", distribution_range(hashes, 16));

    printf("%c%c%c%c%c%c%c%c%c%c%c", '|', 'u', 'n', 'i', 'q', '_', 'h', 'a', 's', 'h', '=');
    printf("%d", count_unique(hashes, 16));

    printf("%c%c%c%c%c%c%c%c%c", '|', 'h', 'a', 's', 'h', '_', 'a', 'v', 'g');
    printf("%c", '=');
    printf("%d", hash_average(hashes, 16));

    /* Test with larger table: 32 buckets */
    ht_init(32);
    {
        int td2[4];
        i = 0;
        while (i < 16) {
            td2[0] = i * 7 + 3;
            td2[1] = i * 13 + 5;
            td2[2] = i * 3 + 1;
            td2[3] = i * 11 + 7;
            h = hash_djb2(td2, 4);
            ht_insert(h);
            i = i + 1;
        }
    }
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c", '|', 'h', 't', '3', '2', '_', 'c', 'o', 'l', 'l', 'i', 's', '=', ' ');
    printf("%d", ht_collisions());
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c", ',', 'e', 'm', 'p', 't', 'y', '=', ' ', ' ', ' ', ' ', ' ', ' ');
    printf("%d", ht_empty_buckets());

    /* Zero data hash */
    printf("%c%c%c%c%c%c%c%c%c", '|', 'z', 'e', 'r', 'o', 'h', 'a', 's', 'h');
    printf("%c", ':');
    printf("%c%c%c%c%c", 'd', 'j', 'b', '2', '=');
    printf("%d", hash_djb2(d7, 4));
    printf("%c%c%c%c", ',', 'x', 'o', 'r');
    printf("%c", '=');
    printf("%d", hash_xor(d7, 4));
    printf("%c%c%c%c%c", ',', 'p', 'o', 'l', 'y');
    printf("%c", '=');
    printf("%d", hash_poly(d7, 4));

    /* Single element */
    printf("%c%c%c%c%c%c%c%c", '|', 's', 'i', 'n', 'g', 'l', 'e', ':');
    printf("%d", hash_djb2(d6, 1));
    printf("%c", ',');
    printf("%d", hash_sum(d6, 1));
    printf("%c", ',');
    printf("%d", hash_xor(d6, 1));

    /* High value data */
    printf("%c%c%c%c%c%c", '|', 'h', 'i', 'g', 'h', ':');
    printf("%d", hash_djb2(d8, 4));
    printf("%c", ',');
    printf("%d", hash_sum(d8, 4));

    /* Avalanche test: change 1 bit, see how hash changes */
    {
        int av1[4];
        int av2[4];
        int h1;
        int h2;
        int diff;
        av1[0] = 100; av1[1] = 200; av1[2] = 50; av1[3] = 75;
        av2[0] = 101; av2[1] = 200; av2[2] = 50; av2[3] = 75; /* changed 1 value */
        h1 = hash_djb2(av1, 4);
        h2 = hash_djb2(av2, 4);
        diff = h1 - h2;
        if (diff < 0) diff = 0 - diff;
        printf("%c%c%c%c%c%c%c%c%c", '|', 'a', 'v', 'a', 'l', 'a', 'n', 'c', 'h');
        printf("%c%c%c%c", ':', 'h', '1', '=');
        printf("%d", h1);
        printf("%c%c%c", ',', 'h', '2');
        printf("%c", '=');
        printf("%d", h2);
        printf("%c%c%c%c%c", ',', 'd', 'i', 'f', 'f');
        printf("%c", '=');
        printf("%d", diff);
        printf("%c%c%c%c%c%c%c%c%c", ',', 'c', 'h', 'a', 'n', 'g', 'e', 'd', '=');
        printf("%d", (h1 != h2));
    }

    /* Sequence hash: hash of 1..n for various n */
    {
        int seq[10];
        int n;
        n = 0;
        while (n < 10) {
            seq[n] = n + 1;
            n = n + 1;
        }
        printf("%c%c%c%c", '|', 's', 'e', 'q');
        printf("%c", ':');
        printf("%d", hash_djb2(seq, 3));
        printf("%c", ',');
        printf("%d", hash_djb2(seq, 5));
        printf("%c", ',');
        printf("%d", hash_djb2(seq, 8));
        printf("%c", ',');
        printf("%d", hash_djb2(seq, 10));
    }

    /* Collision test: same hash different data */
    {
        int ca[4];
        int cb[4];
        int ha;
        int hb;
        ca[0] = 1; ca[1] = 2; ca[2] = 3; ca[3] = 4;
        cb[0] = 4; cb[1] = 3; cb[2] = 2; cb[3] = 1;
        ha = hash_sum(ca, 4);
        hb = hash_sum(cb, 4);
        printf("%c%c%c%c%c%c%c%c%c%c%c%c", '|', 's', 'u', 'm', '_', 'c', 'o', 'l', 'l', 'i', 's', '=');
        printf("%d", (ha == hb));
        /* djb2 should NOT collide for these */
        ha = hash_djb2(ca, 4);
        hb = hash_djb2(cb, 4);
        printf("%c%c%c%c%c%c%c%c%c%c%c%c%c", ',', 'd', 'j', 'b', '2', '_', 'c', 'o', 'l', 'l', 'i', 's', '=');
        printf("%d", (ha == hb));
    }

    /* Hash stability: hash same data twice */
    {
        int st[3];
        st[0] = 99; st[1] = 88; st[2] = 77;
        printf("%c%c%c%c%c%c%c", '|', 's', 't', 'a', 'b', 'l', 'e');
        printf("%c", '=');
        printf("%d", (hash_djb2(st, 3) == hash_djb2(st, 3)));
        printf("%c", ',');
        printf("%d", (hash_xor(st, 3) == hash_xor(st, 3)));
        printf("%c", ',');
        printf("%d", (hash_poly(st, 3) == hash_poly(st, 3)));
        printf("%c", ',');
        printf("%d", (hash_fnv(st, 3) == hash_fnv(st, 3)));
    }

    /* Empty-ish hash test: single element 0 */
    {
        int ez[1];
        ez[0] = 0;
        printf("%c%c%c%c%c%c%c%c", '|', 'h', 'a', 's', 'h', '_', '0', '=');
        printf("%d", hash_djb2(ez, 1));
        printf("%c", ',');
        printf("%d", hash_sum(ez, 1));
        printf("%c", ',');
        printf("%d", hash_xor(ez, 1));
    }

    /* Bucket distribution for sum hash */
    ht_init(16);
    {
        int bd[4];
        i = 0;
        while (i < 16) {
            bd[0] = i * 3 + 1;
            bd[1] = i * 7 + 2;
            bd[2] = i * 11 + 3;
            bd[3] = i * 5 + 4;
            ht_insert(hash_sum(bd, 4));
            i = i + 1;
        }
    }
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", '|', 's', 'u', 'm', '_', 'h', 't', ':', 'c', 'o', 'l', 'l', 'i', 's', '=', ' ', ' ');
    printf("%d", ht_collisions());
    printf("%c%c%c%c%c%c%c", ',', 'u', 's', 'e', 'd', '=', ' ');
    printf("%d", ht_used_buckets());

    printf("%c%c%c%c", '|', 'e', 'n', 'd');
    printf("%c", '\n');

    return 0;
}
