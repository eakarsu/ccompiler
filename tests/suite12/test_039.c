int printf(const char *fmt, ...);

struct Backend {
    int id;
    int ip;
    int port;
    int healthy;
    int active_conns;
    int total_requests;
    int consecutive_fails;
    int weight;
};

struct HealthCheck {
    int interval;
    int threshold_up;
    int threshold_down;
};

struct LoadBalancer {
    struct Backend backends[6];
    int num_backends;
    int next_rr;
    int total_routed;
    int total_failed;
    struct HealthCheck hc;
};

int lb_init(struct LoadBalancer *lb, int hc_interval, int hc_up, int hc_down) {
    lb->num_backends = 0;
    lb->next_rr = 0;
    lb->total_routed = 0;
    lb->total_failed = 0;
    lb->hc.interval = hc_interval;
    lb->hc.threshold_up = hc_up;
    lb->hc.threshold_down = hc_down;
    return 0;
}

int lb_add_backend(struct LoadBalancer *lb, int id, int ip, int port, int weight) {
    if (lb->num_backends >= 6) return -1;
    int idx = lb->num_backends;
    lb->backends[idx].id = id;
    lb->backends[idx].ip = ip;
    lb->backends[idx].port = port;
    lb->backends[idx].healthy = 1;
    lb->backends[idx].active_conns = 0;
    lb->backends[idx].total_requests = 0;
    lb->backends[idx].consecutive_fails = 0;
    lb->backends[idx].weight = weight;
    lb->num_backends = lb->num_backends + 1;
    printf("Added backend %d (ip=%d port=%d weight=%d)\n", id, ip, port, weight);
    return idx;
}

int lb_route_rr(struct LoadBalancer *lb) {
    int attempts = 0;
    while (attempts < lb->num_backends) {
        int idx = lb->next_rr;
        lb->next_rr = (lb->next_rr + 1);
        if (lb->next_rr >= lb->num_backends) {
            lb->next_rr = 0;
        }
        if (lb->backends[idx].healthy) {
            lb->backends[idx].active_conns = lb->backends[idx].active_conns + 1;
            lb->backends[idx].total_requests = lb->backends[idx].total_requests + 1;
            lb->total_routed = lb->total_routed + 1;
            printf("Route -> backend %d (ip=%d conns=%d)\n",
                   lb->backends[idx].id, lb->backends[idx].ip,
                   lb->backends[idx].active_conns);
            return idx;
        }
        attempts = attempts + 1;
    }
    printf("Route -> FAILED (no healthy backends)\n");
    lb->total_failed = lb->total_failed + 1;
    return -1;
}

int lb_route_least_conn(struct LoadBalancer *lb) {
    int best = -1;
    int best_conns = 999999;
    int i;
    for (i = 0; i < lb->num_backends; i = i + 1) {
        if (lb->backends[i].healthy && lb->backends[i].active_conns < best_conns) {
            best = i;
            best_conns = lb->backends[i].active_conns;
        }
    }
    if (best < 0) {
        printf("Least-conn -> FAILED\n");
        lb->total_failed = lb->total_failed + 1;
        return -1;
    }
    lb->backends[best].active_conns = lb->backends[best].active_conns + 1;
    lb->backends[best].total_requests = lb->backends[best].total_requests + 1;
    lb->total_routed = lb->total_routed + 1;
    printf("Least-conn -> backend %d (conns=%d)\n", lb->backends[best].id, lb->backends[best].active_conns);
    return best;
}

int lb_finish_conn(struct LoadBalancer *lb, int idx) {
    if (idx >= 0 && idx < lb->num_backends) {
        lb->backends[idx].active_conns = lb->backends[idx].active_conns - 1;
        if (lb->backends[idx].active_conns < 0) {
            lb->backends[idx].active_conns = 0;
        }
    }
    return 0;
}

int lb_health_check(struct LoadBalancer *lb, int idx, int success) {
    if (success) {
        lb->backends[idx].consecutive_fails = 0;
        if (lb->backends[idx].healthy == 0) {
            lb->backends[idx].healthy = 1;
            printf("Health: backend %d UP\n", lb->backends[idx].id);
        }
    } else {
        lb->backends[idx].consecutive_fails = lb->backends[idx].consecutive_fails + 1;
        if (lb->backends[idx].consecutive_fails >= lb->hc.threshold_down) {
            if (lb->backends[idx].healthy) {
                lb->backends[idx].healthy = 0;
                printf("Health: backend %d DOWN (fails=%d)\n",
                       lb->backends[idx].id, lb->backends[idx].consecutive_fails);
            }
        }
    }
    return 0;
}

int lb_print_status(struct LoadBalancer *lb) {
    int i;
    int healthy = 0;
    for (i = 0; i < lb->num_backends; i = i + 1) {
        if (lb->backends[i].healthy) healthy = healthy + 1;
    }
    printf("LB: %d/%d healthy, routed=%d, failed=%d\n",
           healthy, lb->num_backends, lb->total_routed, lb->total_failed);
    return 0;
}

int main() {
    struct LoadBalancer lb;
    lb_init(&lb, 10, 1, 3);

    printf("=== Load Balancer with Health Checks ===\n");
    // EXPECT: === Load Balancer with Health Checks ===

    lb_add_backend(&lb, 1, 10, 8080, 1);
    // EXPECT: Added backend 1 (ip=10 port=8080 weight=1)
    lb_add_backend(&lb, 2, 11, 8080, 1);
    // EXPECT: Added backend 2 (ip=11 port=8080 weight=1)
    lb_add_backend(&lb, 3, 12, 8080, 1);
    // EXPECT: Added backend 3 (ip=12 port=8080 weight=1)

    printf("--- Round-robin routing ---\n");
    // EXPECT: --- Round-robin routing ---
    lb_route_rr(&lb);
    // EXPECT: Route -> backend 1 (ip=10 conns=1)
    lb_route_rr(&lb);
    // EXPECT: Route -> backend 2 (ip=11 conns=1)
    lb_route_rr(&lb);
    // EXPECT: Route -> backend 3 (ip=12 conns=1)
    lb_route_rr(&lb);
    // EXPECT: Route -> backend 1 (ip=10 conns=2)

    printf("--- Backend failure ---\n");
    // EXPECT: --- Backend failure ---
    lb_health_check(&lb, 1, 0);
    lb_health_check(&lb, 1, 0);
    lb_health_check(&lb, 1, 0);
    // EXPECT: Health: backend 2 DOWN (fails=3)

    lb_route_rr(&lb);
    // EXPECT: Route -> backend 3 (ip=12 conns=2)
    lb_route_rr(&lb);
    // EXPECT: Route -> backend 1 (ip=10 conns=3)

    printf("--- Backend recovery ---\n");
    // EXPECT: --- Backend recovery ---
    lb_health_check(&lb, 1, 1);
    // EXPECT: Health: backend 2 UP
    lb_route_rr(&lb);
    // EXPECT: Route -> backend 2 (ip=11 conns=2)

    printf("--- Least connections ---\n");
    // EXPECT: --- Least connections ---
    lb_finish_conn(&lb, 0);
    lb_finish_conn(&lb, 0);
    lb_finish_conn(&lb, 0);
    lb_route_least_conn(&lb);
    // EXPECT: Least-conn -> backend 1 (conns=1)
    lb_route_least_conn(&lb);
    // EXPECT: Least-conn -> backend 1 (conns=2)
    lb_route_least_conn(&lb);
    // EXPECT: Least-conn -> backend 1 (conns=3)

    lb_print_status(&lb);
    // EXPECT: LB: 3/3 healthy, routed=10, failed=0

    printf("--- All backends down ---\n");
    // EXPECT: --- All backends down ---
    lb.backends[0].healthy = 0;
    lb.backends[1].healthy = 0;
    lb.backends[2].healthy = 0;
    lb_route_rr(&lb);
    // EXPECT: Route -> FAILED (no healthy backends)

    lb_print_status(&lb);
    // EXPECT: LB: 0/3 healthy, routed=10, failed=1

    return 0;
}
