int printf(const char *fmt, ...);

// MAC address table - switch learning simulation

typedef struct {
    int mac_hi;     // upper 24 bits
    int mac_lo;     // lower 24 bits
    int port;       // switch port number
    int age;        // aging timer
    int valid;
} MacEntry;

typedef struct {
    MacEntry table[16];
    int count;
    int max_entries;
    int aging_time;
} MacTable;

void mac_init(MacTable *t) {
    int i;
    for (i = 0; i < 16; i++) {
        t->table[i].valid = 0;
        t->table[i].age = 0;
    }
    t->count = 0;
    t->max_entries = 16;
    t->aging_time = 300;
}

int mac_equal(int hi1, int lo1, int hi2, int lo2) {
    return hi1 == hi2 && lo1 == lo2;
}

// Learn a MAC address on a port
// Returns: 0=updated, 1=added, -1=table full
int mac_learn(MacTable *t, int mac_hi, int mac_lo, int port) {
    int i;
    // Check if already known
    for (i = 0; i < t->count; i++) {
        if (t->table[i].valid &&
            mac_equal(t->table[i].mac_hi, t->table[i].mac_lo, mac_hi, mac_lo)) {
            t->table[i].port = port;
            t->table[i].age = 0;
            return 0;
        }
    }
    // Find empty slot (could be invalidated entry)
    for (i = 0; i < t->count; i++) {
        if (!t->table[i].valid) {
            t->table[i].mac_hi = mac_hi;
            t->table[i].mac_lo = mac_lo;
            t->table[i].port = port;
            t->table[i].age = 0;
            t->table[i].valid = 1;
            return 1;
        }
    }
    // Add at end
    if (t->count < t->max_entries) {
        t->table[t->count].mac_hi = mac_hi;
        t->table[t->count].mac_lo = mac_lo;
        t->table[t->count].port = port;
        t->table[t->count].age = 0;
        t->table[t->count].valid = 1;
        t->count = t->count + 1;
        return 1;
    }
    return -1;
}

// Lookup: returns port number, or -1 if not found (flood)
int mac_lookup(MacTable *t, int mac_hi, int mac_lo) {
    int i;
    for (i = 0; i < t->count; i++) {
        if (t->table[i].valid &&
            mac_equal(t->table[i].mac_hi, t->table[i].mac_lo, mac_hi, mac_lo)) {
            return t->table[i].port;
        }
    }
    return -1; // flood
}

void mac_age_tick(MacTable *t, int seconds) {
    int i;
    for (i = 0; i < t->count; i++) {
        if (t->table[i].valid) {
            t->table[i].age = t->table[i].age + seconds;
            if (t->table[i].age >= t->aging_time) {
                t->table[i].valid = 0;
            }
        }
    }
}

int mac_count_valid(MacTable *t) {
    int c = 0;
    int i;
    for (i = 0; i < t->count; i++) {
        if (t->table[i].valid) c = c + 1;
    }
    return c;
}

int mac_count_on_port(MacTable *t, int port) {
    int c = 0;
    int i;
    for (i = 0; i < t->count; i++) {
        if (t->table[i].valid && t->table[i].port == port) {
            c = c + 1;
        }
    }
    return c;
}

// Simulate frame forwarding
// Returns: port to forward to, or -1 for flood
int forward_frame(MacTable *t, int src_hi, int src_lo,
                  int dst_hi, int dst_lo, int in_port) {
    // Learn source MAC
    mac_learn(t, src_hi, src_lo, in_port);
    // Lookup destination MAC
    int out_port = mac_lookup(t, dst_hi, dst_lo);
    // Don't forward back to source port
    if (out_port == in_port) return -2; // filter
    return out_port;
}

void mac_flush_port(MacTable *t, int port) {
    int i;
    for (i = 0; i < t->count; i++) {
        if (t->table[i].valid && t->table[i].port == port) {
            t->table[i].valid = 0;
        }
    }
}

int main(void) {
    MacTable sw;
    mac_init(&sw);

    // Learn MAC addresses on different ports
    int r = mac_learn(&sw, 0xAA, 0xBB, 1);
    printf("learn=%d\n", r);
    // EXPECT: learn=1

    r = mac_learn(&sw, 0xCC, 0xDD, 2);
    printf("learn=%d\n", r);
    // EXPECT: learn=1

    r = mac_learn(&sw, 0xEE, 0xFF, 3);
    printf("learn=%d\n", r);
    // EXPECT: learn=1

    r = mac_learn(&sw, 0x11, 0x22, 1);
    printf("learn=%d\n", r);
    // EXPECT: learn=1

    printf("count=%d\n", mac_count_valid(&sw));
    // EXPECT: count=4

    // Lookup tests
    int p = mac_lookup(&sw, 0xAA, 0xBB);
    printf("port=%d\n", p);
    // EXPECT: port=1

    p = mac_lookup(&sw, 0xCC, 0xDD);
    printf("port=%d\n", p);
    // EXPECT: port=2

    p = mac_lookup(&sw, 0x99, 0x88);
    printf("port=%d\n", p);
    // EXPECT: port=-1

    // Count on port
    printf("port1_count=%d\n", mac_count_on_port(&sw, 1));
    // EXPECT: port1_count=2

    printf("port2_count=%d\n", mac_count_on_port(&sw, 2));
    // EXPECT: port2_count=1

    // Update: MAC moves to new port
    r = mac_learn(&sw, 0xAA, 0xBB, 4);
    printf("update=%d\n", r);
    // EXPECT: update=0

    p = mac_lookup(&sw, 0xAA, 0xBB);
    printf("new_port=%d\n", p);
    // EXPECT: new_port=4

    printf("port1_count=%d\n", mac_count_on_port(&sw, 1));
    // EXPECT: port1_count=1

    // Frame forwarding test
    // src=0x33:0x44 on port 5 -> dst=0xCC:0xDD (known on port 2)
    int fwd = forward_frame(&sw, 0x33, 0x44, 0xCC, 0xDD, 5);
    printf("forward=%d\n", fwd);
    // EXPECT: forward=2

    // Source was learned
    p = mac_lookup(&sw, 0x33, 0x44);
    printf("learned_port=%d\n", p);
    // EXPECT: learned_port=5

    // Forward to unknown -> flood
    fwd = forward_frame(&sw, 0x33, 0x44, 0x77, 0x88, 5);
    printf("flood=%d\n", fwd);
    // EXPECT: flood=-1

    // Forward back to same port -> filter
    fwd = forward_frame(&sw, 0xCC, 0xDD, 0xCC, 0xDD, 2);
    printf("filter=%d\n", fwd);
    // EXPECT: filter=-2

    // Aging test
    mac_age_tick(&sw, 200);
    printf("valid=%d\n", mac_count_valid(&sw));
    // EXPECT: valid=5

    mac_age_tick(&sw, 100);
    printf("valid=%d\n", mac_count_valid(&sw));
    // EXPECT: valid=0

    // Flush port test
    mac_learn(&sw, 0xA1, 0xA2, 7);
    mac_learn(&sw, 0xA3, 0xA4, 7);
    mac_learn(&sw, 0xA5, 0xA6, 8);
    printf("valid=%d\n", mac_count_valid(&sw));
    // EXPECT: valid=3

    mac_flush_port(&sw, 7);
    printf("valid=%d\n", mac_count_valid(&sw));
    // EXPECT: valid=1

    printf("mac done\n");
    // EXPECT: mac done

    return 0;
}
