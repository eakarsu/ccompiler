int printf(const char *fmt, ...);

// Load Balancer - Round Robin and Weighted Round Robin

struct Server {
    int id;
    int weight;
    int current_load;
    int total_requests;
    int active;
};

struct LoadBalancer {
    struct Server servers[8];
    int count;
    int rr_index;
    int total_dispatched;
};

void lb_init(struct LoadBalancer *lb) {
    lb->count = 0;
    lb->rr_index = 0;
    lb->total_dispatched = 0;
}

void lb_add_server(struct LoadBalancer *lb, int id, int weight) {
    int idx = lb->count;
    lb->servers[idx].id = id;
    lb->servers[idx].weight = weight;
    lb->servers[idx].current_load = 0;
    lb->servers[idx].total_requests = 0;
    lb->servers[idx].active = 1;
    lb->count = lb->count + 1;
}

int lb_round_robin(struct LoadBalancer *lb) {
    int attempts = 0;
    while (attempts < lb->count) {
        int idx = lb->rr_index;
        lb->rr_index = (lb->rr_index + 1) % lb->count;
        if (lb->servers[idx].active) {
            lb->servers[idx].current_load = lb->servers[idx].current_load + 1;
            lb->servers[idx].total_requests = lb->servers[idx].total_requests + 1;
            lb->total_dispatched = lb->total_dispatched + 1;
            return lb->servers[idx].id;
        }
        attempts = attempts + 1;
    }
    return -1;
}

int lb_weighted_rr(struct LoadBalancer *lb) {
    int best = -1;
    int best_ratio = 1000000;
    int i;

    for (i = 0; i < lb->count; i++) {
        if (lb->servers[i].active == 0) continue;
        int ratio;
        if (lb->servers[i].weight == 0) {
            ratio = 1000000;
        } else {
            ratio = (lb->servers[i].current_load * 100) / lb->servers[i].weight;
        }
        if (ratio < best_ratio) {
            best_ratio = ratio;
            best = i;
        }
    }
    if (best >= 0) {
        lb->servers[best].current_load = lb->servers[best].current_load + 1;
        lb->servers[best].total_requests = lb->servers[best].total_requests + 1;
        lb->total_dispatched = lb->total_dispatched + 1;
        return lb->servers[best].id;
    }
    return -1;
}

int lb_least_loaded(struct LoadBalancer *lb) {
    int best = -1;
    int min_load = 1000000;
    int i;
    for (i = 0; i < lb->count; i++) {
        if (lb->servers[i].active && lb->servers[i].current_load < min_load) {
            min_load = lb->servers[i].current_load;
            best = i;
        }
    }
    if (best >= 0) {
        lb->servers[best].current_load = lb->servers[best].current_load + 1;
        lb->servers[best].total_requests = lb->servers[best].total_requests + 1;
        lb->total_dispatched = lb->total_dispatched + 1;
        return lb->servers[best].id;
    }
    return -1;
}

void lb_complete_request(struct LoadBalancer *lb, int server_id) {
    int i;
    for (i = 0; i < lb->count; i++) {
        if (lb->servers[i].id == server_id && lb->servers[i].current_load > 0) {
            lb->servers[i].current_load = lb->servers[i].current_load - 1;
            return;
        }
    }
}

void lb_set_active(struct LoadBalancer *lb, int server_id, int active) {
    int i;
    for (i = 0; i < lb->count; i++) {
        if (lb->servers[i].id == server_id) {
            lb->servers[i].active = active;
            return;
        }
    }
}

int main() {
    struct LoadBalancer lb;
    int i;
    int target;

    // EXPECT: === Load Balancer ===
    printf("=== Load Balancer ===\n");

    lb_init(&lb);
    lb_add_server(&lb, 1, 3);
    lb_add_server(&lb, 2, 2);
    lb_add_server(&lb, 3, 1);

    // EXPECT: Round Robin test:
    printf("Round Robin test:\n");
    for (i = 0; i < 6; i++) {
        target = lb_round_robin(&lb);
        printf("  Request %d -> Server %d\n", i, target);
    }
    // EXPECT:   Request 0 -> Server 1
    // EXPECT:   Request 1 -> Server 2
    // EXPECT:   Request 2 -> Server 3
    // EXPECT:   Request 3 -> Server 1
    // EXPECT:   Request 4 -> Server 2
    // EXPECT:   Request 5 -> Server 3

    // EXPECT: Server loads after RR:
    printf("Server loads after RR:\n");
    for (i = 0; i < lb.count; i++) {
        printf("  S%d: load=%d total=%d\n", lb.servers[i].id,
               lb.servers[i].current_load, lb.servers[i].total_requests);
    }
    // EXPECT:   S1: load=2 total=2
    // EXPECT:   S2: load=2 total=2
    // EXPECT:   S3: load=2 total=2

    struct LoadBalancer lb2;
    lb_init(&lb2);
    lb_add_server(&lb2, 10, 3);
    lb_add_server(&lb2, 20, 2);
    lb_add_server(&lb2, 30, 1);

    // EXPECT: Weighted RR test:
    printf("Weighted RR test:\n");
    for (i = 0; i < 6; i++) {
        target = lb_weighted_rr(&lb2);
        printf("  Request %d -> Server %d\n", i, target);
    }
    // EXPECT:   Request 0 -> Server 10
    // EXPECT:   Request 1 -> Server 20
    // EXPECT:   Request 2 -> Server 30
    // EXPECT:   Request 3 -> Server 10
    // EXPECT:   Request 4 -> Server 20
    // EXPECT:   Request 5 -> Server 10

    // EXPECT: Weighted loads:
    printf("Weighted loads:\n");
    for (i = 0; i < lb2.count; i++) {
        printf("  S%d(w=%d): load=%d\n", lb2.servers[i].id,
               lb2.servers[i].weight, lb2.servers[i].current_load);
    }
    // EXPECT:   S10(w=3): load=3
    // EXPECT:   S20(w=2): load=2
    // EXPECT:   S30(w=1): load=1

    lb_set_active(&lb, 2, 0);
    target = lb_round_robin(&lb);
    // EXPECT: After disabling S2, next RR: 1
    printf("After disabling S2, next RR: %d\n", target);

    // EXPECT: Total dispatched: 7
    printf("Total dispatched: %d\n", lb.total_dispatched);

    // EXPECT: All load balancer tests passed
    printf("All load balancer tests passed\n");
    return 0;
}
