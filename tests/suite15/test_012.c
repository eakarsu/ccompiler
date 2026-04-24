int printf(const char *fmt, ...);

// Routing table lookup - longest prefix match

typedef struct {
    int network;    // network address as int
    int prefix_len; // CIDR prefix length
    int gateway;    // next hop gateway as int
    int metric;     // route metric (lower = preferred)
} Route;

int make_mask(int prefix_len) {
    int mask = 0;
    int i;
    for (i = 31; i >= 32 - prefix_len; i--) {
        mask = mask | (1 << i);
    }
    return mask;
}

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

int matches_route(int dest, Route *r) {
    int mask = make_mask(r->prefix_len);
    return (dest & mask) == r->network;
}

// Find the best matching route: longest prefix, then lowest metric
int find_route(Route *table, int count, int dest) {
    int best = -1;
    int best_prefix = -1;
    int best_metric = 999999;
    int i;
    for (i = 0; i < count; i++) {
        if (matches_route(dest, &table[i])) {
            if (table[i].prefix_len > best_prefix) {
                best = i;
                best_prefix = table[i].prefix_len;
                best_metric = table[i].metric;
            } else if (table[i].prefix_len == best_prefix &&
                       table[i].metric < best_metric) {
                best = i;
                best_metric = table[i].metric;
            }
        }
    }
    return best;
}

void init_route(Route *r, int net, int plen, int gw, int met) {
    r->network = net;
    r->prefix_len = plen;
    r->gateway = gw;
    r->metric = met;
}

int main(void) {
    Route table[8];
    int count = 0;

    // Default route 0.0.0.0/0 via 10.0.0.1
    init_route(&table[count], ip4(0,0,0,0), 0, ip4(10,0,0,1), 100);
    count = count + 1;

    // 10.0.0.0/8 via 10.0.0.1
    init_route(&table[count], ip4(10,0,0,0), 8, ip4(10,0,0,1), 10);
    count = count + 1;

    // 10.1.0.0/16 via 10.1.0.1
    init_route(&table[count], ip4(10,1,0,0), 16, ip4(10,1,0,1), 10);
    count = count + 1;

    // 10.1.1.0/24 via 10.1.1.1
    init_route(&table[count], ip4(10,1,1,0), 24, ip4(10,1,1,1), 10);
    count = count + 1;

    // 192.168.0.0/16 via 192.168.0.1
    init_route(&table[count], ip4(192,168,0,0), 16, ip4(192,168,0,1), 20);
    count = count + 1;

    // 192.168.1.0/24 via 192.168.1.254
    init_route(&table[count], ip4(192,168,1,0), 24, ip4(192,168,1,254), 10);
    count = count + 1;

    // Lookup 10.1.1.50 -> should match /24 route
    int dest = ip4(10,1,1,50);
    int idx = find_route(table, count, dest);
    printf("route_idx=%d prefix=%d\n", idx, table[idx].prefix_len);
    // EXPECT: route_idx=3 prefix=24

    print_ip(table[idx].gateway);
    printf("\n");
    // EXPECT: 10.1.1.1

    // Lookup 10.1.2.100 -> should match /16 route
    dest = ip4(10,1,2,100);
    idx = find_route(table, count, dest);
    printf("route_idx=%d prefix=%d\n", idx, table[idx].prefix_len);
    // EXPECT: route_idx=2 prefix=16

    print_ip(table[idx].gateway);
    printf("\n");
    // EXPECT: 10.1.0.1

    // Lookup 10.5.0.1 -> should match /8 route
    dest = ip4(10,5,0,1);
    idx = find_route(table, count, dest);
    printf("route_idx=%d prefix=%d\n", idx, table[idx].prefix_len);
    // EXPECT: route_idx=1 prefix=8

    // Lookup 192.168.1.42 -> should match /24
    dest = ip4(192,168,1,42);
    idx = find_route(table, count, dest);
    printf("route_idx=%d prefix=%d\n", idx, table[idx].prefix_len);
    // EXPECT: route_idx=5 prefix=24

    print_ip(table[idx].gateway);
    printf("\n");
    // EXPECT: 192.168.1.254

    // Lookup 192.168.5.10 -> should match /16
    dest = ip4(192,168,5,10);
    idx = find_route(table, count, dest);
    printf("route_idx=%d prefix=%d\n", idx, table[idx].prefix_len);
    // EXPECT: route_idx=4 prefix=16

    // Lookup 8.8.8.8 -> should match default route
    dest = ip4(8,8,8,8);
    idx = find_route(table, count, dest);
    printf("route_idx=%d prefix=%d\n", idx, table[idx].prefix_len);
    // EXPECT: route_idx=0 prefix=0

    print_ip(table[idx].gateway);
    printf("\n");
    // EXPECT: 10.0.0.1

    // Add a more specific route and test again
    // 10.1.1.48/28 via 10.1.1.49 (covers 10.1.1.48 - 10.1.1.63)
    init_route(&table[count], ip4(10,1,1,48), 28, ip4(10,1,1,49), 5);
    count = count + 1;

    dest = ip4(10,1,1,50);
    idx = find_route(table, count, dest);
    printf("route_idx=%d prefix=%d\n", idx, table[idx].prefix_len);
    // EXPECT: route_idx=6 prefix=28

    print_ip(table[idx].gateway);
    printf("\n");
    // EXPECT: 10.1.1.49

    // 10.1.1.100 should still use /24
    dest = ip4(10,1,1,100);
    idx = find_route(table, count, dest);
    printf("route_idx=%d prefix=%d\n", idx, table[idx].prefix_len);
    // EXPECT: route_idx=3 prefix=24

    printf("routing done\n");
    // EXPECT: routing done

    return 0;
}
