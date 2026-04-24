int printf(const char *fmt, ...);

struct DHCPLease {
    int client_id;
    int ip_address;
    int lease_time;
    int remaining;
    int state;
    int renew_count;
};

struct DHCPServer {
    struct DHCPLease leases[8];
    int lease_count;
    int pool_start;
    int pool_end;
    int next_ip;
    int default_lease_time;
    int total_discovers;
    int total_requests;
    int total_releases;
};

int dhcp_server_init(struct DHCPServer *srv, int start, int end, int lease_t) {
    srv->lease_count = 0;
    srv->pool_start = start;
    srv->pool_end = end;
    srv->next_ip = start;
    srv->default_lease_time = lease_t;
    srv->total_discovers = 0;
    srv->total_requests = 0;
    srv->total_releases = 0;
    int i;
    for (i = 0; i < 8; i = i + 1) {
        srv->leases[i].client_id = 0;
        srv->leases[i].ip_address = 0;
        srv->leases[i].lease_time = 0;
        srv->leases[i].remaining = 0;
        srv->leases[i].state = 0;
        srv->leases[i].renew_count = 0;
    }
    return 0;
}

int dhcp_find_lease(struct DHCPServer *srv, int client_id) {
    int i;
    for (i = 0; i < srv->lease_count; i = i + 1) {
        if (srv->leases[i].client_id == client_id && srv->leases[i].state > 0) {
            return i;
        }
    }
    return -1;
}

int dhcp_find_ip(struct DHCPServer *srv, int ip) {
    int i;
    for (i = 0; i < srv->lease_count; i = i + 1) {
        if (srv->leases[i].ip_address == ip && srv->leases[i].state > 0) {
            return i;
        }
    }
    return -1;
}

int dhcp_alloc_ip(struct DHCPServer *srv) {
    int attempts = 0;
    while (attempts < (srv->pool_end - srv->pool_start + 1)) {
        int ip = srv->next_ip;
        srv->next_ip = srv->next_ip + 1;
        if (srv->next_ip > srv->pool_end) {
            srv->next_ip = srv->pool_start;
        }
        if (dhcp_find_ip(srv, ip) < 0) {
            return ip;
        }
        attempts = attempts + 1;
    }
    return -1;
}

int dhcp_discover(struct DHCPServer *srv, int client_id) {
    srv->total_discovers = srv->total_discovers + 1;
    int existing = dhcp_find_lease(srv, client_id);
    if (existing >= 0) {
        printf("DISCOVER: client=%d -> OFFER ip=%d (existing)\n",
               client_id, srv->leases[existing].ip_address);
        return srv->leases[existing].ip_address;
    }
    int ip = dhcp_alloc_ip(srv);
    if (ip < 0) {
        printf("DISCOVER: client=%d -> NO IP AVAILABLE\n", client_id);
        return -1;
    }
    int idx = srv->lease_count;
    srv->leases[idx].client_id = client_id;
    srv->leases[idx].ip_address = ip;
    srv->leases[idx].lease_time = srv->default_lease_time;
    srv->leases[idx].remaining = 0;
    srv->leases[idx].state = 1;
    srv->leases[idx].renew_count = 0;
    srv->lease_count = srv->lease_count + 1;
    printf("DISCOVER: client=%d -> OFFER ip=%d\n", client_id, ip);
    return ip;
}

int dhcp_request(struct DHCPServer *srv, int client_id, int requested_ip) {
    srv->total_requests = srv->total_requests + 1;
    int idx = dhcp_find_lease(srv, client_id);
    if (idx < 0) {
        printf("REQUEST: client=%d ip=%d -> NAK (no offer)\n", client_id, requested_ip);
        return -1;
    }
    if (srv->leases[idx].ip_address != requested_ip) {
        printf("REQUEST: client=%d ip=%d -> NAK (wrong ip)\n", client_id, requested_ip);
        return -1;
    }
    srv->leases[idx].state = 2;
    srv->leases[idx].remaining = srv->leases[idx].lease_time;
    printf("REQUEST: client=%d ip=%d -> ACK (lease=%d)\n",
           client_id, requested_ip, srv->leases[idx].lease_time);
    return 0;
}

int dhcp_renew(struct DHCPServer *srv, int client_id) {
    int idx = dhcp_find_lease(srv, client_id);
    if (idx < 0 || srv->leases[idx].state != 2) {
        printf("RENEW: client=%d -> NAK\n", client_id);
        return -1;
    }
    srv->leases[idx].remaining = srv->leases[idx].lease_time;
    srv->leases[idx].renew_count = srv->leases[idx].renew_count + 1;
    printf("RENEW: client=%d ip=%d renewed (count=%d)\n",
           client_id, srv->leases[idx].ip_address, srv->leases[idx].renew_count);
    return 0;
}

int dhcp_release(struct DHCPServer *srv, int client_id) {
    srv->total_releases = srv->total_releases + 1;
    int idx = dhcp_find_lease(srv, client_id);
    if (idx < 0) {
        printf("RELEASE: client=%d -> not found\n", client_id);
        return -1;
    }
    printf("RELEASE: client=%d ip=%d freed\n", client_id, srv->leases[idx].ip_address);
    srv->leases[idx].state = 0;
    return 0;
}

int dhcp_tick(struct DHCPServer *srv, int seconds) {
    int expired = 0;
    int i;
    for (i = 0; i < srv->lease_count; i = i + 1) {
        if (srv->leases[i].state == 2) {
            srv->leases[i].remaining = srv->leases[i].remaining - seconds;
            if (srv->leases[i].remaining <= 0) {
                printf("EXPIRED: client=%d ip=%d\n", srv->leases[i].client_id, srv->leases[i].ip_address);
                srv->leases[i].state = 0;
                expired = expired + 1;
            }
        }
    }
    return expired;
}

int main() {
    struct DHCPServer srv;
    dhcp_server_init(&srv, 100, 104, 3600);

    printf("=== DHCP Lease Manager ===\n");
    // EXPECT: === DHCP Lease Manager ===
    printf("Pool: %d-%d, lease_time=%d\n", srv.pool_start, srv.pool_end, srv.default_lease_time);
    // EXPECT: Pool: 100-104, lease_time=3600

    printf("--- Client joins ---\n");
    // EXPECT: --- Client joins ---
    int ip1 = dhcp_discover(&srv, 1);
    // EXPECT: DISCOVER: client=1 -> OFFER ip=100
    dhcp_request(&srv, 1, ip1);
    // EXPECT: REQUEST: client=1 ip=100 -> ACK (lease=3600)

    int ip2 = dhcp_discover(&srv, 2);
    // EXPECT: DISCOVER: client=2 -> OFFER ip=101
    dhcp_request(&srv, 2, ip2);
    // EXPECT: REQUEST: client=2 ip=101 -> ACK (lease=3600)

    int ip3 = dhcp_discover(&srv, 3);
    // EXPECT: DISCOVER: client=3 -> OFFER ip=102
    dhcp_request(&srv, 3, ip3);
    // EXPECT: REQUEST: client=3 ip=102 -> ACK (lease=3600)

    printf("--- Renewal ---\n");
    // EXPECT: --- Renewal ---
    dhcp_renew(&srv, 1);
    // EXPECT: RENEW: client=1 ip=100 renewed (count=1)
    dhcp_renew(&srv, 1);
    // EXPECT: RENEW: client=1 ip=100 renewed (count=2)

    printf("--- Existing client rediscovers ---\n");
    // EXPECT: --- Existing client rediscovers ---
    dhcp_discover(&srv, 2);
    // EXPECT: DISCOVER: client=2 -> OFFER ip=101 (existing)

    printf("--- Release and reuse ---\n");
    // EXPECT: --- Release and reuse ---
    dhcp_release(&srv, 2);
    // EXPECT: RELEASE: client=2 ip=101 freed

    int ip4 = dhcp_discover(&srv, 4);
    // EXPECT: DISCOVER: client=4 -> OFFER ip=103
    dhcp_request(&srv, 4, ip4);
    // EXPECT: REQUEST: client=4 ip=103 -> ACK (lease=3600)

    int ip5 = dhcp_discover(&srv, 5);
    // EXPECT: DISCOVER: client=5 -> OFFER ip=104
    dhcp_request(&srv, 5, ip5);
    // EXPECT: REQUEST: client=5 ip=104 -> ACK (lease=3600)

    printf("--- Pool reuse (ip 101 freed) ---\n");
    // EXPECT: --- Pool reuse (ip 101 freed) ---
    int ip6 = dhcp_discover(&srv, 6);
    // EXPECT: DISCOVER: client=6 -> OFFER ip=101
    dhcp_request(&srv, 6, ip6);
    // EXPECT: REQUEST: client=6 ip=101 -> ACK (lease=3600)

    printf("--- Expiration ---\n");
    // EXPECT: --- Expiration ---
    srv.leases[0].remaining = 100;
    int exp = dhcp_tick(&srv, 150);
    // EXPECT: EXPIRED: client=1 ip=100
    printf("Expired: %d\n", exp);
    // EXPECT: Expired: 1

    printf("--- Stats ---\n");
    // EXPECT: --- Stats ---
    printf("Discovers: %d\n", srv.total_discovers);
    // EXPECT: Discovers: 7
    printf("Requests: %d\n", srv.total_requests);
    // EXPECT: Requests: 6
    printf("Releases: %d\n", srv.total_releases);
    // EXPECT: Releases: 1

    return 0;
}
