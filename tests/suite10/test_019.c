int printf(const char *fmt, ...);

// Port Scanner Simulation

struct PortInfo {
    int port;
    int open;
    char *service;
};

struct ScanResult {
    struct PortInfo ports[32];
    int scanned;
    int open_count;
    int closed_count;
};

char *get_service_name(int port) {
    if (port == 21) return "FTP";
    if (port == 22) return "SSH";
    if (port == 23) return "Telnet";
    if (port == 25) return "SMTP";
    if (port == 53) return "DNS";
    if (port == 80) return "HTTP";
    if (port == 110) return "POP3";
    if (port == 143) return "IMAP";
    if (port == 443) return "HTTPS";
    if (port == 993) return "IMAPS";
    if (port == 995) return "POP3S";
    if (port == 3306) return "MySQL";
    if (port == 5432) return "PostgreSQL";
    if (port == 8080) return "HTTP-Alt";
    if (port == 8443) return "HTTPS-Alt";
    return "Unknown";
}

int is_well_known(int port) {
    return port >= 1 && port <= 1023;
}

int is_registered(int port) {
    return port >= 1024 && port <= 49151;
}

int is_dynamic(int port) {
    return port >= 49152 && port <= 65535;
}

// Simulated open ports (deterministic based on port number)
int simulate_port_open(int port, int seed) {
    int hash = (port * 7 + seed * 13) % 100;
    // Simulate some common ports as open
    if (port == 22 && seed == 1) return 1;
    if (port == 80 && seed == 1) return 1;
    if (port == 443 && seed == 1) return 1;
    if (port == 8080 && seed == 1) return 1;
    if (hash < 10) return 1;
    return 0;
}

void init_scan_result(struct ScanResult *result) {
    result->scanned = 0;
    result->open_count = 0;
    result->closed_count = 0;
}

void scan_port(struct ScanResult *result, int port, int seed) {
    if (result->scanned >= 32) return;
    int idx = result->scanned;
    result->ports[idx].port = port;
    result->ports[idx].open = simulate_port_open(port, seed);
    result->ports[idx].service = get_service_name(port);
    if (result->ports[idx].open) {
        result->open_count = result->open_count + 1;
    } else {
        result->closed_count = result->closed_count + 1;
    }
    result->scanned = result->scanned + 1;
}

void scan_range(struct ScanResult *result, int start, int end, int seed) {
    int port;
    for (port = start; port <= end && result->scanned < 32; port++) {
        scan_port(result, port, seed);
    }
}

void scan_common_ports(struct ScanResult *result, int seed) {
    int common[10];
    common[0] = 22; common[1] = 80; common[2] = 443;
    common[3] = 21; common[4] = 25; common[5] = 53;
    common[6] = 110; common[7] = 143; common[8] = 8080;
    common[9] = 3306;
    int i;
    for (i = 0; i < 10; i++) {
        scan_port(result, common[i], seed);
    }
}

int find_open_port(struct ScanResult *result, int port) {
    int i;
    for (i = 0; i < result->scanned; i++) {
        if (result->ports[i].port == port && result->ports[i].open) {
            return 1;
        }
    }
    return 0;
}

char *port_category(int port) {
    if (is_well_known(port)) return "well-known";
    if (is_registered(port)) return "registered";
    if (is_dynamic(port)) return "dynamic";
    return "invalid";
}

int count_open_in_range(struct ScanResult *result, int start, int end) {
    int count = 0;
    int i;
    for (i = 0; i < result->scanned; i++) {
        if (result->ports[i].port >= start && result->ports[i].port <= end && result->ports[i].open) {
            count = count + 1;
        }
    }
    return count;
}

int main() {
    struct ScanResult result;

    // Test 1: service name lookup
    printf("port 22: %s\n", get_service_name(22));
    // EXPECT: port 22: SSH
    printf("port 80: %s\n", get_service_name(80));
    // EXPECT: port 80: HTTP
    printf("port 443: %s\n", get_service_name(443));
    // EXPECT: port 443: HTTPS
    printf("port 3306: %s\n", get_service_name(3306));
    // EXPECT: port 3306: MySQL
    printf("port 9999: %s\n", get_service_name(9999));
    // EXPECT: port 9999: Unknown

    // Test 2: port categories
    printf("cat 80: %s\n", port_category(80));
    // EXPECT: cat 80: well-known
    printf("cat 8080: %s\n", port_category(8080));
    // EXPECT: cat 8080: registered
    printf("cat 50000: %s\n", port_category(50000));
    // EXPECT: cat 50000: dynamic
    printf("cat 0: %s\n", port_category(0));
    // EXPECT: cat 0: invalid

    // Test 3: well-known port checks
    printf("well-known 80: %d\n", is_well_known(80));
    // EXPECT: well-known 80: 1
    printf("well-known 1024: %d\n", is_well_known(1024));
    // EXPECT: well-known 1024: 0

    // Test 4: scan common ports with seed=1
    init_scan_result(&result);
    scan_common_ports(&result, 1);
    printf("scanned: %d\n", result.scanned);
    // EXPECT: scanned: 10
    printf("open: %d\n", result.open_count);
    // EXPECT: open: 4
    printf("closed: %d\n", result.closed_count);
    // EXPECT: closed: 6

    // Test 5: check specific ports
    printf("port 22 open: %d\n", find_open_port(&result, 22));
    // EXPECT: port 22 open: 1
    printf("port 80 open: %d\n", find_open_port(&result, 80));
    // EXPECT: port 80 open: 1
    printf("port 443 open: %d\n", find_open_port(&result, 443));
    // EXPECT: port 443 open: 1
    printf("port 21 open: %d\n", find_open_port(&result, 21));
    // EXPECT: port 21 open: 0

    // Test 6: scan port range
    struct ScanResult result2;
    init_scan_result(&result2);
    scan_range(&result2, 20, 30, 2);
    printf("range scanned: %d\n", result2.scanned);
    // EXPECT: range scanned: 11
    printf("range open: %d\n", result2.open_count);
    // EXPECT: range open: 2

    // Test 7: individual port scan
    struct ScanResult result3;
    init_scan_result(&result3);
    scan_port(&result3, 80, 1);
    printf("single scan: port=%d open=%d service=%s\n", result3.ports[0].port, result3.ports[0].open, result3.ports[0].service);
    // EXPECT: single scan: port=80 open=1 service=HTTP

    // Test 8: count open in range
    printf("open in well-known: %d\n", count_open_in_range(&result, 1, 1023));
    // EXPECT: open in well-known: 3
    printf("open in registered: %d\n", count_open_in_range(&result, 1024, 49151));
    // EXPECT: open in registered: 1

    // Test 9: scan with different seed
    struct ScanResult result4;
    init_scan_result(&result4);
    scan_common_ports(&result4, 0);
    printf("seed0 open: %d\n", result4.open_count);
    // EXPECT: seed0 open: 2
    printf("seed0 closed: %d\n", result4.closed_count);
    // EXPECT: seed0 closed: 8

    return 0;
}
