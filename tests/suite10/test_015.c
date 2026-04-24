int printf(const char *fmt, ...);

// Routing Table Lookup

struct Route {
    int network[4];
    int mask[4];
    int gateway[4];
    int metric;
    int active;
};

struct RoutingTable {
    struct Route routes[16];
    int count;
};

void init_routing_table(struct RoutingTable *rt) {
    rt->count = 0;
}

void add_route(struct RoutingTable *rt, int *net, int *mask, int *gw, int metric) {
    if (rt->count >= 16) return;
    int idx = rt->count;
    int i;
    for (i = 0; i < 4; i++) {
        rt->routes[idx].network[i] = net[i];
        rt->routes[idx].mask[i] = mask[i];
        rt->routes[idx].gateway[i] = gw[i];
    }
    rt->routes[idx].metric = metric;
    rt->routes[idx].active = 1;
    rt->count = rt->count + 1;
}

int matches_route(int *ip, struct Route *r) {
    if (!r->active) return 0;
    int i;
    for (i = 0; i < 4; i++) {
        if ((ip[i] & r->mask[i]) != r->network[i]) return 0;
    }
    return 1;
}

int count_mask_bits(int *mask) {
    int total = 0;
    int i;
    for (i = 0; i < 4; i++) {
        int b = mask[i];
        while (b > 0) {
            total = total + (b & 1);
            b = b / 2;
        }
    }
    return total;
}

int lookup_route(struct RoutingTable *rt, int *dst_ip) {
    int best = -1;
    int best_prefix = -1;
    int best_metric = 99999;
    int i;

    for (i = 0; i < rt->count; i++) {
        if (!matches_route(dst_ip, &rt->routes[i])) continue;
        int prefix = count_mask_bits(rt->routes[i].mask);
        if (prefix > best_prefix) {
            best = i;
            best_prefix = prefix;
            best_metric = rt->routes[i].metric;
        } else if (prefix == best_prefix && rt->routes[i].metric < best_metric) {
            best = i;
            best_metric = rt->routes[i].metric;
        }
    }
    return best;
}

void remove_route(struct RoutingTable *rt, int idx) {
    if (idx >= 0 && idx < rt->count) {
        rt->routes[idx].active = 0;
    }
}

int count_active_routes(struct RoutingTable *rt) {
    int count = 0;
    int i;
    for (i = 0; i < rt->count; i++) {
        if (rt->routes[i].active) count = count + 1;
    }
    return count;
}

int main() {
    struct RoutingTable rt;
    init_routing_table(&rt);

    // Setup networks, masks, gateways
    int net1[4]; net1[0] = 192; net1[1] = 168; net1[2] = 1; net1[3] = 0;
    int mask1[4]; mask1[0] = 255; mask1[1] = 255; mask1[2] = 255; mask1[3] = 0;
    int gw1[4]; gw1[0] = 192; gw1[1] = 168; gw1[2] = 1; gw1[3] = 1;

    int net2[4]; net2[0] = 10; net2[1] = 0; net2[2] = 0; net2[3] = 0;
    int mask2[4]; mask2[0] = 255; mask2[1] = 0; mask2[2] = 0; mask2[3] = 0;
    int gw2[4]; gw2[0] = 10; gw2[1] = 0; gw2[2] = 0; gw2[3] = 1;

    int net3[4]; net3[0] = 0; net3[1] = 0; net3[2] = 0; net3[3] = 0;
    int mask3[4]; mask3[0] = 0; mask3[1] = 0; mask3[2] = 0; mask3[3] = 0;
    int gw3[4]; gw3[0] = 192; gw3[1] = 168; gw3[2] = 1; gw3[3] = 254;

    int net4[4]; net4[0] = 192; net4[1] = 168; net4[2] = 1; net4[3] = 128;
    int mask4[4]; mask4[0] = 255; mask4[1] = 255; mask4[2] = 255; mask4[3] = 128;
    int gw4[4]; gw4[0] = 192; gw4[1] = 168; gw4[2] = 1; gw4[3] = 129;

    // Add routes
    add_route(&rt, net1, mask1, gw1, 10);
    add_route(&rt, net2, mask2, gw2, 20);
    add_route(&rt, net3, mask3, gw3, 100);
    add_route(&rt, net4, mask4, gw4, 5);

    printf("routes: %d\n", rt.count);
    // EXPECT: routes: 4

    // Test 1: lookup 192.168.1.200 - should match /25 (most specific)
    int dst1[4]; dst1[0] = 192; dst1[1] = 168; dst1[2] = 1; dst1[3] = 200;
    int idx = lookup_route(&rt, dst1);
    printf("lookup 192.168.1.200: route=%d\n", idx);
    // EXPECT: lookup 192.168.1.200: route=3
    printf("via %d.%d.%d.%d\n", rt.routes[idx].gateway[0], rt.routes[idx].gateway[1], rt.routes[idx].gateway[2], rt.routes[idx].gateway[3]);
    // EXPECT: via 192.168.1.129

    // Test 2: lookup 192.168.1.50 - should match /24 (not /25)
    int dst2[4]; dst2[0] = 192; dst2[1] = 168; dst2[2] = 1; dst2[3] = 50;
    idx = lookup_route(&rt, dst2);
    printf("lookup 192.168.1.50: route=%d\n", idx);
    // EXPECT: lookup 192.168.1.50: route=0
    printf("via %d.%d.%d.%d\n", rt.routes[idx].gateway[0], rt.routes[idx].gateway[1], rt.routes[idx].gateway[2], rt.routes[idx].gateway[3]);
    // EXPECT: via 192.168.1.1

    // Test 3: lookup 10.5.5.5 - should match /8
    int dst3[4]; dst3[0] = 10; dst3[1] = 5; dst3[2] = 5; dst3[3] = 5;
    idx = lookup_route(&rt, dst3);
    printf("lookup 10.5.5.5: route=%d\n", idx);
    // EXPECT: lookup 10.5.5.5: route=1
    printf("via %d.%d.%d.%d\n", rt.routes[idx].gateway[0], rt.routes[idx].gateway[1], rt.routes[idx].gateway[2], rt.routes[idx].gateway[3]);
    // EXPECT: via 10.0.0.1

    // Test 4: lookup 8.8.8.8 - should match default route
    int dst4[4]; dst4[0] = 8; dst4[1] = 8; dst4[2] = 8; dst4[3] = 8;
    idx = lookup_route(&rt, dst4);
    printf("lookup 8.8.8.8: route=%d\n", idx);
    // EXPECT: lookup 8.8.8.8: route=2
    printf("via %d.%d.%d.%d\n", rt.routes[idx].gateway[0], rt.routes[idx].gateway[1], rt.routes[idx].gateway[2], rt.routes[idx].gateway[3]);
    // EXPECT: via 192.168.1.254

    // Test 5: remove route and lookup again
    remove_route(&rt, 3);
    printf("active routes: %d\n", count_active_routes(&rt));
    // EXPECT: active routes: 3
    int dst5[4]; dst5[0] = 192; dst5[1] = 168; dst5[2] = 1; dst5[3] = 200;
    idx = lookup_route(&rt, dst5);
    printf("lookup after remove: route=%d\n", idx);
    // EXPECT: lookup after remove: route=0

    // Test 6: mask bit counting
    printf("mask bits /24: %d\n", count_mask_bits(mask1));
    // EXPECT: mask bits /24: 24
    printf("mask bits /8: %d\n", count_mask_bits(mask2));
    // EXPECT: mask bits /8: 8
    printf("mask bits /0: %d\n", count_mask_bits(mask3));
    // EXPECT: mask bits /0: 0
    printf("mask bits /25: %d\n", count_mask_bits(mask4));
    // EXPECT: mask bits /25: 25

    return 0;
}
