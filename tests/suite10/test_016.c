int printf(const char *fmt, ...);

// ARP Cache Simulation

struct ARPEntry {
    int ip[4];
    int mac[6];
    int ttl;
    int valid;
};

struct ARPCache {
    struct ARPEntry entries[16];
    int count;
    int hits;
    int misses;
};

void init_arp_cache(struct ARPCache *cache) {
    cache->count = 0;
    cache->hits = 0;
    cache->misses = 0;
    int i;
    for (i = 0; i < 16; i++) {
        cache->entries[i].valid = 0;
    }
}

int ip_match(int *a, int *b) {
    int i;
    for (i = 0; i < 4; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

int find_arp_entry(struct ARPCache *cache, int *ip) {
    int i;
    for (i = 0; i < cache->count; i++) {
        if (cache->entries[i].valid && ip_match(cache->entries[i].ip, ip)) {
            return i;
        }
    }
    return -1;
}

int arp_lookup(struct ARPCache *cache, int *ip) {
    int idx = find_arp_entry(cache, ip);
    if (idx >= 0) {
        cache->hits = cache->hits + 1;
        return idx;
    }
    cache->misses = cache->misses + 1;
    return -1;
}

void arp_add(struct ARPCache *cache, int *ip, int *mac, int ttl) {
    // Check if already exists
    int idx = find_arp_entry(cache, ip);
    if (idx >= 0) {
        // Update existing entry
        int i;
        for (i = 0; i < 6; i++) {
            cache->entries[idx].mac[i] = mac[i];
        }
        cache->entries[idx].ttl = ttl;
        return;
    }

    // Find empty slot
    int slot = -1;
    int i;
    for (i = 0; i < 16; i++) {
        if (!cache->entries[i].valid) {
            slot = i;
            break;
        }
    }
    if (slot < 0) {
        // Evict oldest (lowest TTL)
        int min_ttl = 99999;
        for (i = 0; i < cache->count; i++) {
            if (cache->entries[i].valid && cache->entries[i].ttl < min_ttl) {
                min_ttl = cache->entries[i].ttl;
                slot = i;
            }
        }
    }

    for (i = 0; i < 4; i++) {
        cache->entries[slot].ip[i] = ip[i];
    }
    for (i = 0; i < 6; i++) {
        cache->entries[slot].mac[i] = mac[i];
    }
    cache->entries[slot].ttl = ttl;
    cache->entries[slot].valid = 1;
    if (slot >= cache->count) {
        cache->count = slot + 1;
    }
}

void arp_remove(struct ARPCache *cache, int *ip) {
    int idx = find_arp_entry(cache, ip);
    if (idx >= 0) {
        cache->entries[idx].valid = 0;
    }
}

void arp_age(struct ARPCache *cache) {
    int i;
    for (i = 0; i < cache->count; i++) {
        if (cache->entries[i].valid) {
            cache->entries[i].ttl = cache->entries[i].ttl - 1;
            if (cache->entries[i].ttl <= 0) {
                cache->entries[i].valid = 0;
            }
        }
    }
}

int arp_count_valid(struct ARPCache *cache) {
    int count = 0;
    int i;
    for (i = 0; i < cache->count; i++) {
        if (cache->entries[i].valid) count = count + 1;
    }
    return count;
}

int main() {
    struct ARPCache cache;
    init_arp_cache(&cache);

    int ip1[4]; ip1[0] = 192; ip1[1] = 168; ip1[2] = 1; ip1[3] = 1;
    int mac1[6]; mac1[0] = 170; mac1[1] = 187; mac1[2] = 204; mac1[3] = 0; mac1[4] = 0; mac1[5] = 1;

    int ip2[4]; ip2[0] = 192; ip2[1] = 168; ip2[2] = 1; ip2[3] = 2;
    int mac2[6]; mac2[0] = 170; mac2[1] = 187; mac2[2] = 204; mac2[3] = 0; mac2[4] = 0; mac2[5] = 2;

    int ip3[4]; ip3[0] = 10; ip3[1] = 0; ip3[2] = 0; ip3[3] = 1;
    int mac3[6]; mac3[0] = 221; mac3[1] = 238; mac3[2] = 255; mac3[3] = 0; mac3[4] = 0; mac3[5] = 3;

    // Test 1: empty cache lookup
    int idx = arp_lookup(&cache, ip1);
    printf("empty lookup: %d\n", idx);
    // EXPECT: empty lookup: -1
    printf("misses: %d\n", cache.misses);
    // EXPECT: misses: 1

    // Test 2: add and lookup
    arp_add(&cache, ip1, mac1, 300);
    idx = arp_lookup(&cache, ip1);
    printf("after add: %d\n", idx);
    // EXPECT: after add: 0
    printf("mac: %d:%d:%d:%d:%d:%d\n", cache.entries[idx].mac[0], cache.entries[idx].mac[1], cache.entries[idx].mac[2], cache.entries[idx].mac[3], cache.entries[idx].mac[4], cache.entries[idx].mac[5]);
    // EXPECT: mac: 170:187:204:0:0:1
    printf("hits: %d\n", cache.hits);
    // EXPECT: hits: 1

    // Test 3: add more entries
    arp_add(&cache, ip2, mac2, 200);
    arp_add(&cache, ip3, mac3, 100);
    printf("valid entries: %d\n", arp_count_valid(&cache));
    // EXPECT: valid entries: 3

    // Test 4: lookup all
    idx = arp_lookup(&cache, ip2);
    printf("lookup ip2: idx=%d\n", idx);
    // EXPECT: lookup ip2: idx=1
    idx = arp_lookup(&cache, ip3);
    printf("lookup ip3: idx=%d\n", idx);
    // EXPECT: lookup ip3: idx=2

    // Test 5: remove entry
    arp_remove(&cache, ip2);
    printf("after remove valid: %d\n", arp_count_valid(&cache));
    // EXPECT: after remove valid: 2
    idx = arp_lookup(&cache, ip2);
    printf("lookup removed: %d\n", idx);
    // EXPECT: lookup removed: -1

    // Test 6: update existing entry
    int mac1_new[6]; mac1_new[0] = 255; mac1_new[1] = 255; mac1_new[2] = 255; mac1_new[3] = 1; mac1_new[4] = 1; mac1_new[5] = 1;
    arp_add(&cache, ip1, mac1_new, 500);
    idx = arp_lookup(&cache, ip1);
    printf("updated mac: %d:%d:%d\n", cache.entries[idx].mac[0], cache.entries[idx].mac[1], cache.entries[idx].mac[2]);
    // EXPECT: updated mac: 255:255:255
    printf("updated ttl: %d\n", cache.entries[idx].ttl);
    // EXPECT: updated ttl: 500

    // Test 7: aging
    arp_age(&cache);
    arp_age(&cache);
    printf("ip1 ttl after 2 ages: %d\n", cache.entries[0].ttl);
    // EXPECT: ip1 ttl after 2 ages: 498
    printf("ip3 ttl after 2 ages: %d\n", cache.entries[2].ttl);
    // EXPECT: ip3 ttl after 2 ages: 98

    // Test 8: age to expiry
    int j;
    for (j = 0; j < 98; j++) {
        arp_age(&cache);
    }
    printf("valid after aging out ip3: %d\n", arp_count_valid(&cache));
    // EXPECT: valid after aging out ip3: 1
    idx = arp_lookup(&cache, ip3);
    printf("ip3 after expiry: %d\n", idx);
    // EXPECT: ip3 after expiry: -1

    // Test 9: stats
    printf("total hits: %d\n", cache.hits);
    // EXPECT: total hits: 4
    printf("total misses: %d\n", cache.misses);
    // EXPECT: total misses: 3

    return 0;
}
