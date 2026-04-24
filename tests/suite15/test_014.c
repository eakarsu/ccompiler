int printf(const char *fmt, ...);

// DNS resolver simulation - domain name to IP cache

typedef struct {
    char name[32];
    int ip_addr;
    int ttl;        // time to live in seconds
    int valid;
} DnsRecord;

typedef struct {
    DnsRecord records[16];
    int count;
    int hits;
    int misses;
} DnsCache;

int ip4(int a, int b, int c, int d) {
    return (a << 24) | (b << 16) | (c << 8) | d;
}

void print_ip(int addr) {
    int a = (addr >> 24) & 255;
    int b = (addr >> 16) & 255;
    int c = (addr >> 8) & 255;
    int d = addr & 255;
    printf("%d.%d.%d.%d", a, b, c, d);
}

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0 && i < 31) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_len(char *s) {
    int i = 0;
    while (s[i] != 0) i = i + 1;
    return i;
}

void dns_init(DnsCache *c) {
    int i;
    for (i = 0; i < 16; i++) {
        c->records[i].valid = 0;
    }
    c->count = 0;
    c->hits = 0;
    c->misses = 0;
}

int dns_add(DnsCache *c, char *name, int ip, int ttl) {
    // Update existing
    int i;
    for (i = 0; i < c->count; i++) {
        if (c->records[i].valid && str_eq(c->records[i].name, name)) {
            c->records[i].ip_addr = ip;
            c->records[i].ttl = ttl;
            return 0;
        }
    }
    if (c->count >= 16) return -1;
    str_copy(c->records[c->count].name, name);
    c->records[c->count].ip_addr = ip;
    c->records[c->count].ttl = ttl;
    c->records[c->count].valid = 1;
    c->count = c->count + 1;
    return 1;
}

int dns_lookup(DnsCache *c, char *name) {
    int i;
    for (i = 0; i < c->count; i++) {
        if (c->records[i].valid && str_eq(c->records[i].name, name)) {
            c->hits = c->hits + 1;
            return c->records[i].ip_addr;
        }
    }
    c->misses = c->misses + 1;
    return 0;
}

// Simple hash for domain name
int dns_hash(char *name) {
    int h = 0;
    int i = 0;
    while (name[i] != 0) {
        h = h * 31 + name[i];
        i = i + 1;
    }
    // Keep positive
    if (h < 0) h = -h;
    return h;
}

void dns_tick(DnsCache *c, int seconds) {
    int i;
    for (i = 0; i < c->count; i++) {
        if (c->records[i].valid) {
            c->records[i].ttl = c->records[i].ttl - seconds;
            if (c->records[i].ttl <= 0) {
                c->records[i].valid = 0;
            }
        }
    }
}

int dns_count_valid(DnsCache *c) {
    int n = 0;
    int i;
    for (i = 0; i < c->count; i++) {
        if (c->records[i].valid) n = n + 1;
    }
    return n;
}

// Count how many labels (dot-separated) in a domain
int count_labels(char *name) {
    int count = 1;
    int i = 0;
    while (name[i] != 0) {
        if (name[i] == '.') count = count + 1;
        i = i + 1;
    }
    return count;
}

int main(void) {
    DnsCache cache;
    dns_init(&cache);

    // Add DNS records
    int r = dns_add(&cache, "example.com", ip4(93,184,216,34), 3600);
    printf("add=%d\n", r);
    // EXPECT: add=1

    r = dns_add(&cache, "google.com", ip4(142,250,80,46), 300);
    printf("add=%d\n", r);
    // EXPECT: add=1

    r = dns_add(&cache, "github.com", ip4(140,82,121,3), 600);
    printf("add=%d\n", r);
    // EXPECT: add=1

    r = dns_add(&cache, "local.dev", ip4(127,0,0,1), 60);
    printf("add=%d\n", r);
    // EXPECT: add=1

    printf("count=%d\n", dns_count_valid(&cache));
    // EXPECT: count=4

    // Lookup tests
    int ip = dns_lookup(&cache, "google.com");
    print_ip(ip);
    printf("\n");
    // EXPECT: 142.250.80.46

    ip = dns_lookup(&cache, "example.com");
    print_ip(ip);
    printf("\n");
    // EXPECT: 93.184.216.34

    ip = dns_lookup(&cache, "notfound.com");
    printf("ip=%d\n", ip);
    // EXPECT: ip=0

    printf("hits=%d misses=%d\n", cache.hits, cache.misses);
    // EXPECT: hits=2 misses=1

    // TTL expiry: advance 100 seconds
    dns_tick(&cache, 100);
    printf("valid=%d\n", dns_count_valid(&cache));
    // EXPECT: valid=3

    // local.dev had ttl=60, should be expired
    ip = dns_lookup(&cache, "local.dev");
    printf("ip=%d\n", ip);
    // EXPECT: ip=0

    // Advance 250 more (total 350), google.com ttl was 300
    dns_tick(&cache, 250);
    printf("valid=%d\n", dns_count_valid(&cache));
    // EXPECT: valid=2

    // google.com should be expired
    ip = dns_lookup(&cache, "google.com");
    printf("ip=%d\n", ip);
    // EXPECT: ip=0

    // Update existing record
    r = dns_add(&cache, "example.com", ip4(93,184,216,99), 7200);
    printf("update=%d\n", r);
    // EXPECT: update=0

    ip = dns_lookup(&cache, "example.com");
    print_ip(ip);
    printf("\n");
    // EXPECT: 93.184.216.99

    // Domain label counting
    printf("labels=%d\n", count_labels("www.example.com"));
    // EXPECT: labels=3

    printf("labels=%d\n", count_labels("a.b.c.d.e"));
    // EXPECT: labels=5

    printf("labels=%d\n", count_labels("localhost"));
    // EXPECT: labels=1

    // Hash test (deterministic)
    int h1 = dns_hash("abc");
    int h2 = dns_hash("abc");
    printf("hash_eq=%d\n", h1 == h2);
    // EXPECT: hash_eq=1

    int h3 = dns_hash("xyz");
    printf("hash_diff=%d\n", h1 != h3);
    // EXPECT: hash_diff=1

    printf("dns done\n");
    // EXPECT: dns done

    return 0;
}
