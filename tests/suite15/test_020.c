int printf(const char *fmt, ...);

// Simple firewall rules - allow/deny by port and IP range

enum Action {
    ACTION_DENY,
    ACTION_ALLOW
};

enum Protocol {
    PROTO_TCP,
    PROTO_UDP,
    PROTO_ANY
};

typedef struct {
    int src_ip;       // source IP (0 = any)
    int src_mask;     // source mask (0 = match all)
    int dst_ip;       // destination IP (0 = any)
    int dst_mask;     // destination mask (0 = match all)
    int dst_port_lo;  // destination port range low
    int dst_port_hi;  // destination port range high
    int protocol;     // PROTO_TCP, PROTO_UDP, PROTO_ANY
    int action;       // ACTION_ALLOW or ACTION_DENY
    int hit_count;    // how many times this rule matched
} FwRule;

typedef struct {
    FwRule rules[16];
    int count;
    int default_action;
    int total_packets;
    int allowed;
    int denied;
} Firewall;

int ip4(int a, int b, int c, int d) {
    return (a << 24) | (b << 16) | (c << 8) | d;
}

int make_mask(int prefix_len) {
    if (prefix_len == 0) return 0;
    int mask = 0;
    int i;
    for (i = 31; i >= 32 - prefix_len; i--) {
        mask = mask | (1 << i);
    }
    return mask;
}

void fw_init(Firewall *fw) {
    fw->count = 0;
    fw->default_action = ACTION_DENY;
    fw->total_packets = 0;
    fw->allowed = 0;
    fw->denied = 0;
}

void fw_add_rule(Firewall *fw, int src_ip, int src_mask,
                 int dst_ip, int dst_mask, int port_lo, int port_hi,
                 int proto, int action) {
    if (fw->count >= 16) return;
    int i = fw->count;
    fw->rules[i].src_ip = src_ip;
    fw->rules[i].src_mask = src_mask;
    fw->rules[i].dst_ip = dst_ip;
    fw->rules[i].dst_mask = dst_mask;
    fw->rules[i].dst_port_lo = port_lo;
    fw->rules[i].dst_port_hi = port_hi;
    fw->rules[i].protocol = proto;
    fw->rules[i].action = action;
    fw->rules[i].hit_count = 0;
    fw->count = fw->count + 1;
}

int ip_matches(int addr, int rule_ip, int rule_mask) {
    if (rule_mask == 0) return 1; // any
    return (addr & rule_mask) == (rule_ip & rule_mask);
}

int port_in_range(int port, int lo, int hi) {
    if (lo == 0 && hi == 0) return 1; // any port
    return port >= lo && port <= hi;
}

int proto_matches(int pkt_proto, int rule_proto) {
    if (rule_proto == PROTO_ANY) return 1;
    return pkt_proto == rule_proto;
}

// Evaluate firewall rules (first match wins)
int fw_evaluate(Firewall *fw, int src_ip, int dst_ip,
                int dst_port, int proto) {
    fw->total_packets = fw->total_packets + 1;
    int i;
    for (i = 0; i < fw->count; i++) {
        FwRule *r = &fw->rules[i];
        if (ip_matches(src_ip, r->src_ip, r->src_mask) &&
            ip_matches(dst_ip, r->dst_ip, r->dst_mask) &&
            port_in_range(dst_port, r->dst_port_lo, r->dst_port_hi) &&
            proto_matches(proto, r->protocol)) {
            r->hit_count = r->hit_count + 1;
            if (r->action == ACTION_ALLOW) {
                fw->allowed = fw->allowed + 1;
            } else {
                fw->denied = fw->denied + 1;
            }
            return r->action;
        }
    }
    // Default action
    if (fw->default_action == ACTION_ALLOW) {
        fw->allowed = fw->allowed + 1;
    } else {
        fw->denied = fw->denied + 1;
    }
    return fw->default_action;
}

int fw_get_hits(Firewall *fw, int rule_idx) {
    if (rule_idx < 0 || rule_idx >= fw->count) return -1;
    return fw->rules[rule_idx].hit_count;
}

int main(void) {
    Firewall fw;
    fw_init(&fw);

    // Rule 0: Allow TCP to port 80 (HTTP) from anywhere
    fw_add_rule(&fw, 0, 0, 0, 0, 80, 80, PROTO_TCP, ACTION_ALLOW);

    // Rule 1: Allow TCP to port 443 (HTTPS) from anywhere
    fw_add_rule(&fw, 0, 0, 0, 0, 443, 443, PROTO_TCP, ACTION_ALLOW);

    // Rule 2: Allow TCP to port 22 (SSH) from 10.0.0.0/8 only
    fw_add_rule(&fw, ip4(10,0,0,0), make_mask(8),
                0, 0, 22, 22, PROTO_TCP, ACTION_ALLOW);

    // Rule 3: Deny TCP to port 22 from anywhere else
    fw_add_rule(&fw, 0, 0, 0, 0, 22, 22, PROTO_TCP, ACTION_DENY);

    // Rule 4: Allow UDP to ports 53 (DNS)
    fw_add_rule(&fw, 0, 0, 0, 0, 53, 53, PROTO_UDP, ACTION_ALLOW);

    // Rule 5: Allow TCP to high ports 1024-65535 from 192.168.0.0/16
    fw_add_rule(&fw, ip4(192,168,0,0), make_mask(16),
                0, 0, 1024, 65535, PROTO_TCP, ACTION_ALLOW);

    printf("rules=%d\n", fw.count);
    // EXPECT: rules=6

    // Test: HTTP from anywhere -> ALLOW (rule 0)
    int r = fw_evaluate(&fw, ip4(8,8,8,8), ip4(192,168,1,1), 80, PROTO_TCP);
    printf("http=%d\n", r);
    // EXPECT: http=1

    // Test: HTTPS from anywhere -> ALLOW (rule 1)
    r = fw_evaluate(&fw, ip4(1,2,3,4), ip4(192,168,1,1), 443, PROTO_TCP);
    printf("https=%d\n", r);
    // EXPECT: https=1

    // Test: SSH from internal -> ALLOW (rule 2)
    r = fw_evaluate(&fw, ip4(10,0,1,5), ip4(192,168,1,1), 22, PROTO_TCP);
    printf("ssh_internal=%d\n", r);
    // EXPECT: ssh_internal=1

    // Test: SSH from external -> DENY (rule 3)
    r = fw_evaluate(&fw, ip4(8,8,8,8), ip4(192,168,1,1), 22, PROTO_TCP);
    printf("ssh_external=%d\n", r);
    // EXPECT: ssh_external=0

    // Test: DNS UDP -> ALLOW (rule 4)
    r = fw_evaluate(&fw, ip4(10,0,0,1), ip4(8,8,8,8), 53, PROTO_UDP);
    printf("dns=%d\n", r);
    // EXPECT: dns=1

    // Test: High port from internal -> ALLOW (rule 5)
    r = fw_evaluate(&fw, ip4(192,168,1,50), ip4(10,0,0,1), 8080, PROTO_TCP);
    printf("highport_int=%d\n", r);
    // EXPECT: highport_int=1

    // Test: High port from external -> DENY (default)
    r = fw_evaluate(&fw, ip4(8,8,8,8), ip4(10,0,0,1), 8080, PROTO_TCP);
    printf("highport_ext=%d\n", r);
    // EXPECT: highport_ext=0

    // Test: Random UDP port -> DENY (default)
    r = fw_evaluate(&fw, ip4(1,1,1,1), ip4(2,2,2,2), 9999, PROTO_UDP);
    printf("random=%d\n", r);
    // EXPECT: random=0

    // Check statistics
    printf("total=%d allowed=%d denied=%d\n",
           fw.total_packets, fw.allowed, fw.denied);
    // EXPECT: total=8 allowed=5 denied=3

    // Check hit counts
    printf("rule0_hits=%d\n", fw_get_hits(&fw, 0));
    // EXPECT: rule0_hits=1

    printf("rule1_hits=%d\n", fw_get_hits(&fw, 1));
    // EXPECT: rule1_hits=1

    printf("rule2_hits=%d\n", fw_get_hits(&fw, 2));
    // EXPECT: rule2_hits=1

    printf("rule3_hits=%d\n", fw_get_hits(&fw, 3));
    // EXPECT: rule3_hits=1

    printf("rule4_hits=%d\n", fw_get_hits(&fw, 4));
    // EXPECT: rule4_hits=1

    printf("rule5_hits=%d\n", fw_get_hits(&fw, 5));
    // EXPECT: rule5_hits=1

    // Test port range boundary
    r = fw_evaluate(&fw, ip4(192,168,0,1), ip4(10,0,0,1), 1024, PROTO_TCP);
    printf("port_lo=%d\n", r);
    // EXPECT: port_lo=1

    r = fw_evaluate(&fw, ip4(192,168,0,1), ip4(10,0,0,1), 1023, PROTO_TCP);
    printf("port_below=%d\n", r);
    // EXPECT: port_below=0

    // HTTP on UDP (should not match rule 0 which is TCP only)
    // But 80 is in no UDP rule -> default DENY
    r = fw_evaluate(&fw, ip4(1,1,1,1), ip4(2,2,2,2), 80, PROTO_UDP);
    printf("http_udp=%d\n", r);
    // EXPECT: http_udp=0

    printf("total=%d allowed=%d denied=%d\n",
           fw.total_packets, fw.allowed, fw.denied);
    // EXPECT: total=11 allowed=6 denied=5

    printf("firewall done\n");
    // EXPECT: firewall done

    return 0;
}
