int printf(const char *fmt, ...);

struct NATEntry {
    int internal_ip;
    int internal_port;
    int external_port;
    int remote_ip;
    int remote_port;
    int active;
    int packet_count;
    int ttl;
};

struct NATTable {
    struct NATEntry entries[10];
    int count;
    int external_ip;
    int next_port;
};

int nat_init(struct NATTable *nat, int ext_ip) {
    nat->count = 0;
    nat->external_ip = ext_ip;
    nat->next_port = 10000;
    int i;
    for (i = 0; i < 10; i = i + 1) {
        nat->entries[i].active = 0;
        nat->entries[i].packet_count = 0;
        nat->entries[i].ttl = 0;
    }
    return 0;
}

int nat_lookup(struct NATTable *nat, int int_ip, int int_port, int rem_ip, int rem_port) {
    int i;
    for (i = 0; i < nat->count; i = i + 1) {
        if (nat->entries[i].active &&
            nat->entries[i].internal_ip == int_ip &&
            nat->entries[i].internal_port == int_port &&
            nat->entries[i].remote_ip == rem_ip &&
            nat->entries[i].remote_port == rem_port) {
            return i;
        }
    }
    return -1;
}

int nat_lookup_external(struct NATTable *nat, int ext_port) {
    int i;
    for (i = 0; i < nat->count; i = i + 1) {
        if (nat->entries[i].active && nat->entries[i].external_port == ext_port) {
            return i;
        }
    }
    return -1;
}

int nat_outbound(struct NATTable *nat, int int_ip, int int_port, int rem_ip, int rem_port) {
    int idx = nat_lookup(nat, int_ip, int_port, rem_ip, rem_port);
    if (idx >= 0) {
        nat->entries[idx].packet_count = nat->entries[idx].packet_count + 1;
        nat->entries[idx].ttl = 300;
        printf("NAT out: %d:%d -> %d:%d (existing ext_port=%d)\n",
               int_ip, int_port, rem_ip, rem_port, nat->entries[idx].external_port);
        return nat->entries[idx].external_port;
    }
    if (nat->count >= 10) {
        printf("NAT table full!\n");
        return -1;
    }
    idx = nat->count;
    nat->entries[idx].internal_ip = int_ip;
    nat->entries[idx].internal_port = int_port;
    nat->entries[idx].external_port = nat->next_port;
    nat->entries[idx].remote_ip = rem_ip;
    nat->entries[idx].remote_port = rem_port;
    nat->entries[idx].active = 1;
    nat->entries[idx].packet_count = 1;
    nat->entries[idx].ttl = 300;
    nat->count = nat->count + 1;
    printf("NAT out: %d:%d -> %d:%d (new ext_port=%d)\n",
           int_ip, int_port, rem_ip, rem_port, nat->next_port);
    nat->next_port = nat->next_port + 1;
    return nat->entries[idx].external_port;
}

int nat_inbound(struct NATTable *nat, int ext_port, int rem_ip, int rem_port) {
    int idx = nat_lookup_external(nat, ext_port);
    if (idx < 0) {
        printf("NAT in: ext_port=%d -> DROPPED (no mapping)\n", ext_port);
        return -1;
    }
    if (nat->entries[idx].remote_ip != rem_ip || nat->entries[idx].remote_port != rem_port) {
        printf("NAT in: ext_port=%d -> DROPPED (wrong remote)\n", ext_port);
        return -1;
    }
    nat->entries[idx].packet_count = nat->entries[idx].packet_count + 1;
    nat->entries[idx].ttl = 300;
    printf("NAT in: ext_port=%d -> %d:%d\n", ext_port,
           nat->entries[idx].internal_ip, nat->entries[idx].internal_port);
    return idx;
}

int nat_expire(struct NATTable *nat, int decrement) {
    int expired = 0;
    int i;
    for (i = 0; i < nat->count; i = i + 1) {
        if (nat->entries[i].active) {
            nat->entries[i].ttl = nat->entries[i].ttl - decrement;
            if (nat->entries[i].ttl <= 0) {
                printf("NAT expire: %d:%d ext_port=%d\n",
                       nat->entries[i].internal_ip,
                       nat->entries[i].internal_port,
                       nat->entries[i].external_port);
                nat->entries[i].active = 0;
                expired = expired + 1;
            }
        }
    }
    return expired;
}

int nat_print_stats(struct NATTable *nat) {
    int active = 0;
    int total_pkts = 0;
    int i;
    for (i = 0; i < nat->count; i = i + 1) {
        if (nat->entries[i].active) {
            active = active + 1;
            total_pkts = total_pkts + nat->entries[i].packet_count;
        }
    }
    printf("NAT stats: active=%d total_pkts=%d\n", active, total_pkts);
    return active;
}

int main() {
    struct NATTable nat;
    nat_init(&nat, 200);

    printf("=== NAT Table Simulation ===\n");
    // EXPECT: === NAT Table Simulation ===
    printf("External IP: %d\n", nat.external_ip);
    // EXPECT: External IP: 200

    printf("--- Outbound connections ---\n");
    // EXPECT: --- Outbound connections ---
    nat_outbound(&nat, 10, 5000, 100, 80);
    // EXPECT: NAT out: 10:5000 -> 100:80 (new ext_port=10000)
    nat_outbound(&nat, 10, 5001, 100, 443);
    // EXPECT: NAT out: 10:5001 -> 100:443 (new ext_port=10001)
    nat_outbound(&nat, 11, 6000, 101, 80);
    // EXPECT: NAT out: 11:6000 -> 101:80 (new ext_port=10002)

    printf("--- Reuse existing mapping ---\n");
    // EXPECT: --- Reuse existing mapping ---
    nat_outbound(&nat, 10, 5000, 100, 80);
    // EXPECT: NAT out: 10:5000 -> 100:80 (existing ext_port=10000)

    printf("--- Inbound traffic ---\n");
    // EXPECT: --- Inbound traffic ---
    nat_inbound(&nat, 10000, 100, 80);
    // EXPECT: NAT in: ext_port=10000 -> 10:5000
    nat_inbound(&nat, 10001, 100, 443);
    // EXPECT: NAT in: ext_port=10001 -> 10:5001

    printf("--- Blocked inbound ---\n");
    // EXPECT: --- Blocked inbound ---
    nat_inbound(&nat, 9999, 100, 80);
    // EXPECT: NAT in: ext_port=9999 -> DROPPED (no mapping)
    nat_inbound(&nat, 10000, 999, 80);
    // EXPECT: NAT in: ext_port=10000 -> DROPPED (wrong remote)

    nat_print_stats(&nat);
    // EXPECT: NAT stats: active=3 total_pkts=6

    printf("--- Expire entries ---\n");
    // EXPECT: --- Expire entries ---
    nat.entries[1].ttl = 50;
    int exp = nat_expire(&nat, 100);
    // EXPECT: NAT expire: 10:5001 ext_port=10001
    printf("Expired: %d\n", exp);
    // EXPECT: Expired: 1

    nat_print_stats(&nat);
    // EXPECT: NAT stats: active=2 total_pkts=4

    printf("--- New connection after expiry ---\n");
    // EXPECT: --- New connection after expiry ---
    nat_outbound(&nat, 12, 7000, 102, 8080);
    // EXPECT: NAT out: 12:7000 -> 102:8080 (new ext_port=10003)

    nat_print_stats(&nat);
    // EXPECT: NAT stats: active=3 total_pkts=5

    return 0;
}
