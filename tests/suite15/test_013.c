int printf(const char *fmt, ...);

// ARP table simulation - IP to MAC address mapping

typedef struct {
    int ip_addr;        // IP as integer
    int mac_hi;         // upper 24 bits of MAC
    int mac_lo;         // lower 24 bits of MAC
    int valid;          // entry is valid
    int age;            // age in seconds (for expiry)
} ArpEntry;

typedef struct {
    ArpEntry entries[16];
    int count;
    int max_age;
} ArpTable;

int ip4(int a, int b, int c, int d) {
    return (a << 24) | (b << 16) | (c << 8) | d;
}

void print_ip(int addr) {
    int a = (addr >> 24) & 255;
    int b = (addr >> 16) & 255;
    int c = (addr >> 8) & 255;
    int d = addr & 255;
    printf("%d.%d.%d.%d", a, b, c, d);
}

void print_mac(int hi, int lo) {
    int b5 = (hi >> 16) & 255;
    int b4 = (hi >> 8) & 255;
    int b3 = hi & 255;
    int b2 = (lo >> 16) & 255;
    int b1 = (lo >> 8) & 255;
    int b0 = lo & 255;
    printf("%d:%d:%d:%d:%d:%d", b5, b4, b3, b2, b1, b0);
}

void arp_init(ArpTable *t) {
    int i;
    for (i = 0; i < 16; i++) {
        t->entries[i].valid = 0;
        t->entries[i].age = 0;
    }
    t->count = 0;
    t->max_age = 300; // 5 minutes
}

int arp_lookup(ArpTable *t, int ip, int *mac_hi, int *mac_lo) {
    int i;
    for (i = 0; i < t->count; i++) {
        if (t->entries[i].valid && t->entries[i].ip_addr == ip) {
            *mac_hi = t->entries[i].mac_hi;
            *mac_lo = t->entries[i].mac_lo;
            return 1;
        }
    }
    return 0;
}

int arp_add(ArpTable *t, int ip, int mac_hi, int mac_lo) {
    // Check if already exists -> update
    int i;
    for (i = 0; i < t->count; i++) {
        if (t->entries[i].valid && t->entries[i].ip_addr == ip) {
            t->entries[i].mac_hi = mac_hi;
            t->entries[i].mac_lo = mac_lo;
            t->entries[i].age = 0;
            return 0; // updated
        }
    }
    // Add new entry
    if (t->count < 16) {
        t->entries[t->count].ip_addr = ip;
        t->entries[t->count].mac_hi = mac_hi;
        t->entries[t->count].mac_lo = mac_lo;
        t->entries[t->count].valid = 1;
        t->entries[t->count].age = 0;
        t->count = t->count + 1;
        return 1; // added
    }
    return -1; // table full
}

void arp_age(ArpTable *t, int seconds) {
    int i;
    for (i = 0; i < t->count; i++) {
        if (t->entries[i].valid) {
            t->entries[i].age = t->entries[i].age + seconds;
        }
    }
}

int arp_expire(ArpTable *t) {
    int expired = 0;
    int i;
    for (i = 0; i < t->count; i++) {
        if (t->entries[i].valid && t->entries[i].age >= t->max_age) {
            t->entries[i].valid = 0;
            expired = expired + 1;
        }
    }
    return expired;
}

int arp_count_valid(ArpTable *t) {
    int c = 0;
    int i;
    for (i = 0; i < t->count; i++) {
        if (t->entries[i].valid) {
            c = c + 1;
        }
    }
    return c;
}

int arp_remove(ArpTable *t, int ip) {
    int i;
    for (i = 0; i < t->count; i++) {
        if (t->entries[i].valid && t->entries[i].ip_addr == ip) {
            t->entries[i].valid = 0;
            return 1;
        }
    }
    return 0;
}

int main(void) {
    ArpTable table;
    arp_init(&table);

    // Add some entries: MAC bytes as decimal
    // AA:BB:CC:DD:EE:01 -> hi=0xAABBCC=11189196, lo=0xDDEE01=14544385
    int r = arp_add(&table, ip4(192,168,1,1), 11189196, 14544385);
    printf("add=%d\n", r);
    // EXPECT: add=1

    // AA:BB:CC:DD:EE:02 -> hi=11189196, lo=0xDDEE02=14544386
    r = arp_add(&table, ip4(192,168,1,2), 11189196, 14544386);
    printf("add=%d\n", r);
    // EXPECT: add=1

    // AA:BB:CC:11:22:33 -> hi=11189196, lo=0x112233=1122867
    r = arp_add(&table, ip4(192,168,1,10), 11189196, 1122867);
    printf("add=%d\n", r);
    // EXPECT: add=1

    printf("count=%d\n", arp_count_valid(&table));
    // EXPECT: count=3

    // Lookup existing
    int mhi, mlo;
    int found = arp_lookup(&table, ip4(192,168,1,2), &mhi, &mlo);
    printf("found=%d lo=%d\n", found, mlo);
    // EXPECT: found=1 lo=14544386

    // Lookup non-existing
    found = arp_lookup(&table, ip4(192,168,1,99), &mhi, &mlo);
    printf("found=%d\n", found);
    // EXPECT: found=0

    // Update existing entry
    r = arp_add(&table, ip4(192,168,1,1), 11189196, 999999);
    printf("update=%d\n", r);
    // EXPECT: update=0

    // Verify update
    found = arp_lookup(&table, ip4(192,168,1,1), &mhi, &mlo);
    printf("found=%d lo=%d\n", found, mlo);
    // EXPECT: found=1 lo=999999

    // Count should still be 3
    printf("count=%d\n", arp_count_valid(&table));
    // EXPECT: count=3

    // Age entries by 200 seconds
    arp_age(&table, 200);
    int expired = arp_expire(&table);
    printf("expired=%d\n", expired);
    // EXPECT: expired=0

    // Age by 100 more (total 300 = max_age)
    arp_age(&table, 100);
    expired = arp_expire(&table);
    printf("expired=%d\n", expired);
    // EXPECT: expired=3

    printf("count=%d\n", arp_count_valid(&table));
    // EXPECT: count=0

    // Re-add and test removal
    arp_add(&table, ip4(10,0,0,1), 1, 1);
    arp_add(&table, ip4(10,0,0,2), 2, 2);
    arp_add(&table, ip4(10,0,0,3), 3, 3);
    printf("count=%d\n", arp_count_valid(&table));
    // EXPECT: count=3

    r = arp_remove(&table, ip4(10,0,0,2));
    printf("removed=%d\n", r);
    // EXPECT: removed=1

    printf("count=%d\n", arp_count_valid(&table));
    // EXPECT: count=2

    r = arp_remove(&table, ip4(10,0,0,99));
    printf("removed=%d\n", r);
    // EXPECT: removed=0

    printf("arp done\n");
    // EXPECT: arp done

    return 0;
}
