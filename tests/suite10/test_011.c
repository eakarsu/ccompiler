int printf(const char *fmt, ...);

// IP Address Parsing and Validation

struct IPAddr {
    int octets[4];
    int valid;
};

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int parse_octet(char *s, int *pos) {
    int val = 0;
    int count = 0;
    while (s[*pos] != '\0' && s[*pos] != '.') {
        if (!is_digit(s[*pos])) return -1;
        val = val * 10 + (s[*pos] - '0');
        *pos = *pos + 1;
        count = count + 1;
        if (count > 3) return -1;
    }
    if (count == 0) return -1;
    if (val > 255) return -1;
    return val;
}

struct IPAddr parse_ip(char *s) {
    struct IPAddr addr;
    int pos = 0;
    int i;
    addr.valid = 1;

    for (i = 0; i < 4; i++) {
        int val = parse_octet(s, &pos);
        if (val < 0) {
            addr.valid = 0;
            return addr;
        }
        addr.octets[i] = val;
        if (i < 3) {
            if (s[pos] != '.') {
                addr.valid = 0;
                return addr;
            }
            pos = pos + 1;
        }
    }
    if (s[pos] != '\0') {
        addr.valid = 0;
    }
    return addr;
}

int is_private(struct IPAddr addr) {
    if (addr.octets[0] == 10) return 1;
    if (addr.octets[0] == 172 && addr.octets[1] >= 16 && addr.octets[1] <= 31) return 1;
    if (addr.octets[0] == 192 && addr.octets[1] == 168) return 1;
    return 0;
}

int is_loopback(struct IPAddr addr) {
    return addr.octets[0] == 127;
}

int is_multicast(struct IPAddr addr) {
    return addr.octets[0] >= 224 && addr.octets[0] <= 239;
}

int get_class(struct IPAddr addr) {
    if (addr.octets[0] < 128) return 0;
    if (addr.octets[0] < 192) return 1;
    if (addr.octets[0] < 224) return 2;
    if (addr.octets[0] < 240) return 3;
    return 4;
}

int ip_to_int(struct IPAddr addr) {
    int result = 0;
    int i;
    for (i = 0; i < 4; i++) {
        result = result * 256 + addr.octets[i];
    }
    return result;
}

int ips_equal(struct IPAddr a, struct IPAddr b) {
    int i;
    for (i = 0; i < 4; i++) {
        if (a.octets[i] != b.octets[i]) return 0;
    }
    return 1;
}

int main() {
    struct IPAddr ip;
    char *classes[5];
    classes[0] = "A";
    classes[1] = "B";
    classes[2] = "C";
    classes[3] = "D";
    classes[4] = "E";

    // Test 1: parse valid IP
    ip = parse_ip("192.168.1.100");
    printf("parse 192.168.1.100: valid=%d\n", ip.valid);
    // EXPECT: parse 192.168.1.100: valid=1
    printf("octets: %d.%d.%d.%d\n", ip.octets[0], ip.octets[1], ip.octets[2], ip.octets[3]);
    // EXPECT: octets: 192.168.1.100

    // Test 2: parse another valid IP
    ip = parse_ip("10.0.0.1");
    printf("parse 10.0.0.1: valid=%d\n", ip.valid);
    // EXPECT: parse 10.0.0.1: valid=1
    printf("octets: %d.%d.%d.%d\n", ip.octets[0], ip.octets[1], ip.octets[2], ip.octets[3]);
    // EXPECT: octets: 10.0.0.1

    // Test 3: invalid - octet too large
    ip = parse_ip("256.1.1.1");
    printf("parse 256.1.1.1: valid=%d\n", ip.valid);
    // EXPECT: parse 256.1.1.1: valid=0

    // Test 4: invalid - missing octet
    ip = parse_ip("1.2.3");
    printf("parse 1.2.3: valid=%d\n", ip.valid);
    // EXPECT: parse 1.2.3: valid=0

    // Test 5: invalid - letters
    ip = parse_ip("1.2.abc.4");
    printf("parse 1.2.abc.4: valid=%d\n", ip.valid);
    // EXPECT: parse 1.2.abc.4: valid=0

    // Test 6: private IP checks
    ip = parse_ip("10.20.30.40");
    printf("10.20.30.40 private=%d\n", is_private(ip));
    // EXPECT: 10.20.30.40 private=1

    ip = parse_ip("172.16.0.1");
    printf("172.16.0.1 private=%d\n", is_private(ip));
    // EXPECT: 172.16.0.1 private=1

    ip = parse_ip("192.168.0.1");
    printf("192.168.0.1 private=%d\n", is_private(ip));
    // EXPECT: 192.168.0.1 private=1

    ip = parse_ip("8.8.8.8");
    printf("8.8.8.8 private=%d\n", is_private(ip));
    // EXPECT: 8.8.8.8 private=0

    // Test 7: loopback
    ip = parse_ip("127.0.0.1");
    printf("127.0.0.1 loopback=%d\n", is_loopback(ip));
    // EXPECT: 127.0.0.1 loopback=1

    ip = parse_ip("8.8.8.8");
    printf("8.8.8.8 loopback=%d\n", is_loopback(ip));
    // EXPECT: 8.8.8.8 loopback=0

    // Test 8: multicast
    ip = parse_ip("224.0.0.1");
    printf("224.0.0.1 multicast=%d\n", is_multicast(ip));
    // EXPECT: 224.0.0.1 multicast=1

    ip = parse_ip("239.255.255.255");
    printf("239.255.255.255 multicast=%d\n", is_multicast(ip));
    // EXPECT: 239.255.255.255 multicast=1

    ip = parse_ip("192.168.1.1");
    printf("192.168.1.1 multicast=%d\n", is_multicast(ip));
    // EXPECT: 192.168.1.1 multicast=0

    // Test 9: IP class
    ip = parse_ip("10.0.0.1");
    printf("10.0.0.1 class=%s\n", classes[get_class(ip)]);
    // EXPECT: 10.0.0.1 class=A

    ip = parse_ip("172.16.0.1");
    printf("172.16.0.1 class=%s\n", classes[get_class(ip)]);
    // EXPECT: 172.16.0.1 class=B

    ip = parse_ip("192.168.1.1");
    printf("192.168.1.1 class=%s\n", classes[get_class(ip)]);
    // EXPECT: 192.168.1.1 class=C

    ip = parse_ip("224.0.0.1");
    printf("224.0.0.1 class=%s\n", classes[get_class(ip)]);
    // EXPECT: 224.0.0.1 class=D

    // Test 10: IP equality
    struct IPAddr a = parse_ip("10.0.0.1");
    struct IPAddr b = parse_ip("10.0.0.1");
    struct IPAddr c = parse_ip("10.0.0.2");
    printf("10.0.0.1 == 10.0.0.1: %d\n", ips_equal(a, b));
    // EXPECT: 10.0.0.1 == 10.0.0.1: 1
    printf("10.0.0.1 == 10.0.0.2: %d\n", ips_equal(a, c));
    // EXPECT: 10.0.0.1 == 10.0.0.2: 0

    return 0;
}
