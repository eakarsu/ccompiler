int printf(const char *fmt, ...);

// IP address parsing and subnet calculations

typedef struct {
    int a;
    int b;
    int c;
    int d;
} IPv4;

void ip_from_int(int addr, IPv4 *out) {
    out->a = (addr >> 24) & 255;
    out->b = (addr >> 16) & 255;
    out->c = (addr >> 8) & 255;
    out->d = addr & 255;
}

int ip_to_int(IPv4 *ip) {
    return (ip->a << 24) | (ip->b << 16) | (ip->c << 8) | ip->d;
}

void print_ip(IPv4 *ip) {
    printf("%d.%d.%d.%d\n", ip->a, ip->b, ip->c, ip->d);
}

int make_mask(int prefix_len) {
    if (prefix_len == 0) return 0;
    // Create a mask with prefix_len leading 1-bits
    // Use unsigned-style shift via subtraction
    int mask = 0;
    int i;
    for (i = 31; i >= 32 - prefix_len; i--) {
        mask = mask | (1 << i);
    }
    return mask;
}

void get_network(IPv4 *ip, int prefix_len, IPv4 *net) {
    int addr = ip_to_int(ip);
    int mask = make_mask(prefix_len);
    int net_addr = addr & mask;
    ip_from_int(net_addr, net);
}

void get_broadcast(IPv4 *ip, int prefix_len, IPv4 *bcast) {
    int addr = ip_to_int(ip);
    int mask = make_mask(prefix_len);
    int host_mask = ~mask;
    int bcast_addr = addr | host_mask;
    ip_from_int(bcast_addr, bcast);
}

int count_hosts(int prefix_len) {
    if (prefix_len >= 31) return 0;
    int host_bits = 32 - prefix_len;
    return (1 << host_bits) - 2;
}

int same_subnet(IPv4 *a, IPv4 *b, int prefix_len) {
    int mask = make_mask(prefix_len);
    int net_a = ip_to_int(a) & mask;
    int net_b = ip_to_int(b) & mask;
    return net_a == net_b;
}

int is_private(IPv4 *ip) {
    // 10.x.x.x
    if (ip->a == 10) return 1;
    // 172.16.x.x - 172.31.x.x
    if (ip->a == 172 && ip->b >= 16 && ip->b <= 31) return 1;
    // 192.168.x.x
    if (ip->a == 192 && ip->b == 168) return 1;
    return 0;
}

int parse_octet(char *str, int start, int *end) {
    int val = 0;
    int i = start;
    while (str[i] >= '0' && str[i] <= '9') {
        val = val * 10 + (str[i] - '0');
        i = i + 1;
    }
    *end = i;
    return val;
}

void parse_ip_string(char *str, IPv4 *ip) {
    int pos = 0;
    int end;
    ip->a = parse_octet(str, pos, &end);
    pos = end + 1;
    ip->b = parse_octet(str, pos, &end);
    pos = end + 1;
    ip->c = parse_octet(str, pos, &end);
    pos = end + 1;
    ip->d = parse_octet(str, pos, &end);
}

int main(void) {
    IPv4 ip;
    IPv4 net;
    IPv4 bcast;

    // Parse an IP address string
    parse_ip_string("192.168.1.100", &ip);
    printf("%d.%d.%d.%d\n", ip.a, ip.b, ip.c, ip.d);
    // EXPECT: 192.168.1.100

    // Network address for /24
    get_network(&ip, 24, &net);
    printf("%d.%d.%d.%d\n", net.a, net.b, net.c, net.d);
    // EXPECT: 192.168.1.0

    // Broadcast address for /24
    get_broadcast(&ip, 24, &bcast);
    printf("%d.%d.%d.%d\n", bcast.a, bcast.b, bcast.c, bcast.d);
    // EXPECT: 192.168.1.255

    // Host count for /24
    printf("%d\n", count_hosts(24));
    // EXPECT: 254

    // Host count for /16
    printf("%d\n", count_hosts(16));
    // EXPECT: 65534

    // Host count for /30
    printf("%d\n", count_hosts(30));
    // EXPECT: 2

    // Test /16 subnet
    parse_ip_string("10.0.5.23", &ip);
    get_network(&ip, 16, &net);
    printf("%d.%d.%d.%d\n", net.a, net.b, net.c, net.d);
    // EXPECT: 10.0.0.0

    get_broadcast(&ip, 16, &bcast);
    printf("%d.%d.%d.%d\n", bcast.a, bcast.b, bcast.c, bcast.d);
    // EXPECT: 10.0.255.255

    // Same subnet test
    IPv4 ip2;
    parse_ip_string("192.168.1.50", &ip2);
    parse_ip_string("192.168.1.200", &ip);
    printf("%d\n", same_subnet(&ip, &ip2, 24));
    // EXPECT: 1

    parse_ip_string("192.168.2.50", &ip2);
    printf("%d\n", same_subnet(&ip, &ip2, 24));
    // EXPECT: 0

    printf("%d\n", same_subnet(&ip, &ip2, 16));
    // EXPECT: 1

    // Private address detection
    parse_ip_string("10.0.0.1", &ip);
    printf("%d\n", is_private(&ip));
    // EXPECT: 1

    parse_ip_string("172.20.1.5", &ip);
    printf("%d\n", is_private(&ip));
    // EXPECT: 1

    parse_ip_string("192.168.0.1", &ip);
    printf("%d\n", is_private(&ip));
    // EXPECT: 1

    parse_ip_string("8.8.8.8", &ip);
    printf("%d\n", is_private(&ip));
    // EXPECT: 0

    // IP to int and back
    parse_ip_string("1.2.3.4", &ip);
    int val = ip_to_int(&ip);
    // 1*16777216 + 2*65536 + 3*256 + 4 = 16909060
    printf("%d\n", val);
    // EXPECT: 16909060

    IPv4 recovered;
    ip_from_int(val, &recovered);
    printf("%d.%d.%d.%d\n", recovered.a, recovered.b, recovered.c, recovered.d);
    // EXPECT: 1.2.3.4

    // /8 subnet
    parse_ip_string("10.255.128.64", &ip);
    get_network(&ip, 8, &net);
    printf("%d.%d.%d.%d\n", net.a, net.b, net.c, net.d);
    // EXPECT: 10.0.0.0

    get_broadcast(&ip, 8, &bcast);
    printf("%d.%d.%d.%d\n", bcast.a, bcast.b, bcast.c, bcast.d);
    // EXPECT: 10.255.255.255

    printf("done\n");
    // EXPECT: done

    return 0;
}
