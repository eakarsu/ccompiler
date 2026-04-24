int printf(const char *fmt, ...);

// Network routing simulation: routing table, packet forwarding, TTL, hop counting

struct Route {
    int dest_network;
    int subnet_mask;
    int next_hop;
    int interface_id;
    int metric;
    int active;
};

struct RoutingTable {
    struct Route routes[16];
    int count;
};

struct Packet {
    int src_addr;
    int dst_addr;
    int ttl;
    int payload;
    int hops;
    int delivered;
    int dropped;
};

void rt_init(struct RoutingTable *rt) {
    rt->count = 0;
    int i;
    for (i = 0; i < 16; i++) rt->routes[i].active = 0;
}

void rt_add(struct RoutingTable *rt, int dest, int mask, int next, int iface, int metric) {
    int idx = rt->count;
    rt->routes[idx].dest_network = dest;
    rt->routes[idx].subnet_mask = mask;
    rt->routes[idx].next_hop = next;
    rt->routes[idx].interface_id = iface;
    rt->routes[idx].metric = metric;
    rt->routes[idx].active = 1;
    rt->count = rt->count + 1;
}

int match_route(struct RoutingTable *rt, int dst_addr) {
    int best = -1;
    int best_metric = 9999;
    int i;
    for (i = 0; i < rt->count; i++) {
        if (rt->routes[i].active == 0) continue;
        int masked = dst_addr & rt->routes[i].subnet_mask;
        if (masked == rt->routes[i].dest_network) {
            if (rt->routes[i].metric < best_metric) {
                best = i;
                best_metric = rt->routes[i].metric;
            }
        }
    }
    return best;
}

void forward_packet(struct RoutingTable *rt, struct Packet *pkt) {
    if (pkt->ttl <= 0) {
        pkt->dropped = 1;
        return;
    }
    int route_idx = match_route(rt, pkt->dst_addr);
    if (route_idx == -1) {
        pkt->dropped = 1;
        return;
    }
    pkt->ttl = pkt->ttl - 1;
    pkt->hops = pkt->hops + 1;
    if (pkt->ttl <= 0) {
        pkt->dropped = 1;
    }
}

void init_packet(struct Packet *pkt, int src, int dst, int ttl, int payload) {
    pkt->src_addr = src;
    pkt->dst_addr = dst;
    pkt->ttl = ttl;
    pkt->payload = payload;
    pkt->hops = 0;
    pkt->delivered = 0;
    pkt->dropped = 0;
}

// Simulate multi-hop routing through a chain of routers
int simulate_path(struct RoutingTable *routers, int num_routers, struct Packet *pkt) {
    int current_router = 0;
    while (pkt->dropped == 0 && pkt->delivered == 0 && current_router < num_routers) {
        int route_idx = match_route(&routers[current_router], pkt->dst_addr);
        if (route_idx == -1) {
            pkt->dropped = 1;
            return -1;
        }
        pkt->ttl = pkt->ttl - 1;
        pkt->hops = pkt->hops + 1;
        if (pkt->ttl <= 0) {
            pkt->dropped = 1;
            return -2;
        }
        int next = routers[current_router].routes[route_idx].next_hop;
        // If next_hop matches destination, delivered
        if (next == pkt->dst_addr) {
            pkt->delivered = 1;
            return pkt->hops;
        }
        // Find next router (next_hop is router index for simulation)
        current_router = current_router + 1;
    }
    pkt->dropped = 1;
    return -3;
}

int main(void) {
    struct RoutingTable rt;
    struct Packet pkt;
    int route_idx;

    // Build routing table
    rt_init(&rt);
    rt_add(&rt, 100, 255, 200, 1, 5);   // network 100/255 -> hop 200
    rt_add(&rt, 200, 255, 300, 2, 3);   // network 200/255 -> hop 300
    rt_add(&rt, 100, 255, 201, 3, 10);  // alternate for 100 (higher metric)
    rt_add(&rt, 0, 0, 999, 4, 100);     // default route (0/0 matches all)

    // EXPECT: Routes: 4
    printf("Routes: %d\n", rt.count);

    // Test route matching - exact match
    route_idx = match_route(&rt, 100);
    // EXPECT: Match 100: route=0 hop=200 metric=5
    printf("Match 100: route=%d hop=%d metric=%d\n",
           route_idx, rt.routes[route_idx].next_hop, rt.routes[route_idx].metric);

    // Test route matching - network 200
    route_idx = match_route(&rt, 200);
    // EXPECT: Match 200: route=1 hop=300
    printf("Match 200: route=%d hop=%d\n", route_idx, rt.routes[route_idx].next_hop);

    // Test default route
    route_idx = match_route(&rt, 50);
    // EXPECT: Match 50 (default): route=3 hop=999
    printf("Match 50 (default): route=%d hop=%d\n", route_idx, rt.routes[route_idx].next_hop);

    // Test packet forwarding
    init_packet(&pkt, 10, 100, 5, 42);
    // EXPECT: Pkt init: src=10 dst=100 ttl=5
    printf("Pkt init: src=%d dst=%d ttl=%d\n", pkt.src_addr, pkt.dst_addr, pkt.ttl);

    forward_packet(&rt, &pkt);
    // EXPECT: After fwd: ttl=4 hops=1 dropped=0
    printf("After fwd: ttl=%d hops=%d dropped=%d\n", pkt.ttl, pkt.hops, pkt.dropped);

    forward_packet(&rt, &pkt);
    // EXPECT: After fwd2: ttl=3 hops=2
    printf("After fwd2: ttl=%d hops=%d\n", pkt.ttl, pkt.hops);

    // Test TTL expiry
    init_packet(&pkt, 10, 100, 1, 42);
    forward_packet(&rt, &pkt);
    // EXPECT: TTL expired: dropped=1 ttl=0
    printf("TTL expired: dropped=%d ttl=%d\n", pkt.dropped, pkt.ttl);

    // Test no route found
    {
        struct RoutingTable empty;
        rt_init(&empty);
        init_packet(&pkt, 10, 100, 5, 42);
        forward_packet(&empty, &pkt);
        // EXPECT: No route: dropped=1
        printf("No route: dropped=%d\n", pkt.dropped);
    }

    // Test disable route
    rt.routes[0].active = 0;
    route_idx = match_route(&rt, 100);
    // EXPECT: After disable: route=2 hop=201 metric=10
    printf("After disable: route=%d hop=%d metric=%d\n",
           route_idx, rt.routes[route_idx].next_hop, rt.routes[route_idx].metric);

    // Re-enable
    rt.routes[0].active = 1;

    // Test multi-hop simulation
    {
        struct RoutingTable routers[3];
        struct Packet p2;
        int result;

        rt_init(&routers[0]);
        rt_add(&routers[0], 50, 255, 20, 1, 1);

        rt_init(&routers[1]);
        rt_add(&routers[1], 50, 255, 30, 1, 1);

        rt_init(&routers[2]);
        rt_add(&routers[2], 50, 255, 50, 1, 1); // next_hop == dst -> delivered

        init_packet(&p2, 10, 50, 10, 77);
        result = simulate_path(routers, 3, &p2);
        // EXPECT: Multi-hop: hops=3 delivered=1
        printf("Multi-hop: hops=%d delivered=%d\n", p2.hops, p2.delivered);

        // EXPECT: Multi-hop result=3 ttl=7
        printf("Multi-hop result=%d ttl=%d\n", result, p2.ttl);
    }

    // Test multi-hop TTL expiry
    {
        struct RoutingTable routers2[3];
        struct Packet p3;
        int result;

        rt_init(&routers2[0]);
        rt_add(&routers2[0], 50, 255, 20, 1, 1);
        rt_init(&routers2[1]);
        rt_add(&routers2[1], 50, 255, 30, 1, 1);
        rt_init(&routers2[2]);
        rt_add(&routers2[2], 50, 255, 50, 1, 1);

        init_packet(&p3, 10, 50, 2, 77);
        result = simulate_path(routers2, 3, &p3);
        // EXPECT: TTL multi: result=-2 dropped=1
        printf("TTL multi: result=%d dropped=%d\n", result, p3.dropped);
    }

    // Test metric comparison
    {
        struct RoutingTable rt2;
        rt_init(&rt2);
        rt_add(&rt2, 100, 255, 10, 1, 20);
        rt_add(&rt2, 100, 255, 11, 2, 5);
        rt_add(&rt2, 100, 255, 12, 3, 15);
        route_idx = match_route(&rt2, 100);
        // EXPECT: Best metric: hop=11 metric=5
        printf("Best metric: hop=%d metric=%d\n",
               rt2.routes[route_idx].next_hop, rt2.routes[route_idx].metric);
    }

    // EXPECT: Router sim complete
    printf("Router sim complete\n");

    return 0;
}
