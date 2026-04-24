int printf(const char *fmt, ...);

// DNS Resolver Simulation

struct DNSRecord {
    char name[32];
    int type;   // 1=A, 5=CNAME, 2=NS
    int ip[4];  // for A records
    char cname[32]; // for CNAME records
    int ttl;
    int valid;
};

struct DNSCache {
    struct DNSRecord records[20];
    int count;
};

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = '\0';
}

int str_len(char *s) {
    int i = 0;
    while (s[i] != '\0') i = i + 1;
    return i;
}

void init_dns_cache(struct DNSCache *c) {
    c->count = 0;
    int i;
    for (i = 0; i < 20; i++) {
        c->records[i].valid = 0;
    }
}

void dns_add_a(struct DNSCache *c, char *name, int *ip, int ttl) {
    if (c->count >= 20) return;
    int idx = c->count;
    str_copy(c->records[idx].name, name);
    c->records[idx].type = 1;
    int i;
    for (i = 0; i < 4; i++) {
        c->records[idx].ip[i] = ip[i];
    }
    c->records[idx].ttl = ttl;
    c->records[idx].valid = 1;
    c->count = c->count + 1;
}

void dns_add_cname(struct DNSCache *c, char *name, char *cname, int ttl) {
    if (c->count >= 20) return;
    int idx = c->count;
    str_copy(c->records[idx].name, name);
    c->records[idx].type = 5;
    str_copy(c->records[idx].cname, cname);
    c->records[idx].ttl = ttl;
    c->records[idx].valid = 1;
    c->count = c->count + 1;
}

int dns_find(struct DNSCache *c, char *name, int type) {
    int i;
    for (i = 0; i < c->count; i++) {
        if (c->records[i].valid && c->records[i].type == type && str_eq(c->records[i].name, name)) {
            return i;
        }
    }
    return -1;
}

// Resolve with CNAME following (up to 3 hops)
int dns_resolve(struct DNSCache *c, char *name, int *result_ip) {
    char current[32];
    str_copy(current, name);
    int hops = 0;

    while (hops < 3) {
        // Try A record
        int idx = dns_find(c, current, 1);
        if (idx >= 0) {
            int i;
            for (i = 0; i < 4; i++) {
                result_ip[i] = c->records[idx].ip[i];
            }
            return 1;
        }
        // Try CNAME
        idx = dns_find(c, current, 5);
        if (idx >= 0) {
            str_copy(current, c->records[idx].cname);
            hops = hops + 1;
        } else {
            return 0;
        }
    }
    return 0;
}

char *type_name(int type) {
    if (type == 1) return "A";
    if (type == 5) return "CNAME";
    if (type == 2) return "NS";
    return "UNKNOWN";
}

int count_domain_labels(char *name) {
    int count = 1;
    int i = 0;
    while (name[i] != '\0') {
        if (name[i] == '.') count = count + 1;
        i = i + 1;
    }
    return count;
}

int is_subdomain(char *sub, char *parent) {
    int slen = str_len(sub);
    int plen = str_len(parent);
    if (slen <= plen) return 0;
    // Check if sub ends with .parent
    int offset = slen - plen;
    if (sub[offset - 1] != '.') return 0;
    int i;
    for (i = 0; i < plen; i++) {
        if (sub[offset + i] != parent[i]) return 0;
    }
    return 1;
}

int main() {
    struct DNSCache cache;
    init_dns_cache(&cache);

    // Add A records
    int ip1[4]; ip1[0] = 93; ip1[1] = 184; ip1[2] = 216; ip1[3] = 34;
    dns_add_a(&cache, "example.com", ip1, 3600);

    int ip2[4]; ip2[0] = 142; ip2[1] = 250; ip2[2] = 80; ip2[3] = 46;
    dns_add_a(&cache, "google.com", ip2, 300);

    int ip3[4]; ip3[0] = 151; ip3[1] = 101; ip3[2] = 1; ip3[3] = 69;
    dns_add_a(&cache, "reddit.com", ip3, 60);

    int ip4[4]; ip4[0] = 104; ip4[1] = 16; ip4[2] = 0; ip4[3] = 1;
    dns_add_a(&cache, "cdn.example.com", ip4, 120);

    // Add CNAME records
    dns_add_cname(&cache, "www.example.com", "example.com", 3600);
    dns_add_cname(&cache, "mail.example.com", "cdn.example.com", 1800);

    printf("records: %d\n", cache.count);
    // EXPECT: records: 6

    // Test 1: Direct A lookup
    int idx = dns_find(&cache, "example.com", 1);
    printf("find example.com A: %d\n", idx);
    // EXPECT: find example.com A: 0
    printf("type: %s\n", type_name(cache.records[idx].type));
    // EXPECT: type: A
    printf("ip: %d.%d.%d.%d\n", cache.records[idx].ip[0], cache.records[idx].ip[1], cache.records[idx].ip[2], cache.records[idx].ip[3]);
    // EXPECT: ip: 93.184.216.34

    // Test 2: Direct resolve
    int result[4];
    int ok = dns_resolve(&cache, "google.com", result);
    printf("resolve google.com: %d\n", ok);
    // EXPECT: resolve google.com: 1
    printf("resolved: %d.%d.%d.%d\n", result[0], result[1], result[2], result[3]);
    // EXPECT: resolved: 142.250.80.46

    // Test 3: CNAME resolution (www -> example.com)
    ok = dns_resolve(&cache, "www.example.com", result);
    printf("resolve www.example.com: %d\n", ok);
    // EXPECT: resolve www.example.com: 1
    printf("resolved: %d.%d.%d.%d\n", result[0], result[1], result[2], result[3]);
    // EXPECT: resolved: 93.184.216.34

    // Test 4: chained CNAME (mail -> cdn -> A)
    ok = dns_resolve(&cache, "mail.example.com", result);
    printf("resolve mail.example.com: %d\n", ok);
    // EXPECT: resolve mail.example.com: 1
    printf("resolved: %d.%d.%d.%d\n", result[0], result[1], result[2], result[3]);
    // EXPECT: resolved: 104.16.0.1

    // Test 5: not found
    ok = dns_resolve(&cache, "unknown.org", result);
    printf("resolve unknown.org: %d\n", ok);
    // EXPECT: resolve unknown.org: 0

    // Test 6: domain label counting
    printf("labels example.com: %d\n", count_domain_labels("example.com"));
    // EXPECT: labels example.com: 2
    printf("labels www.example.com: %d\n", count_domain_labels("www.example.com"));
    // EXPECT: labels www.example.com: 3
    printf("labels a.b.c.d.e: %d\n", count_domain_labels("a.b.c.d.e"));
    // EXPECT: labels a.b.c.d.e: 5

    // Test 7: subdomain check
    printf("www.example.com sub of example.com: %d\n", is_subdomain("www.example.com", "example.com"));
    // EXPECT: www.example.com sub of example.com: 1
    printf("example.com sub of example.com: %d\n", is_subdomain("example.com", "example.com"));
    // EXPECT: example.com sub of example.com: 0
    printf("other.org sub of example.com: %d\n", is_subdomain("other.org", "example.com"));
    // EXPECT: other.org sub of example.com: 0

    // Test 8: type name
    printf("type 1: %s\n", type_name(1));
    // EXPECT: type 1: A
    printf("type 5: %s\n", type_name(5));
    // EXPECT: type 5: CNAME
    printf("type 2: %s\n", type_name(2));
    // EXPECT: type 2: NS

    // Test 9: string length
    printf("len example.com: %d\n", str_len("example.com"));
    // EXPECT: len example.com: 11
    printf("len www.example.com: %d\n", str_len("www.example.com"));
    // EXPECT: len www.example.com: 15

    return 0;
}
