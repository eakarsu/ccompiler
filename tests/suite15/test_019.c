int printf(const char *fmt, ...);

// CIDR notation and address range calculations

typedef struct {
    int addr;       // IP address as integer
    int prefix;     // prefix length (0-32)
} CidrBlock;

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

int make_mask(int prefix_len) {
    int mask = 0;
    int i;
    for (i = 31; i >= 32 - prefix_len; i--) {
        mask = mask | (1 << i);
    }
    return mask;
}

int network_addr(CidrBlock *c) {
    return c->addr & make_mask(c->prefix);
}

int broadcast_addr(CidrBlock *c) {
    int host_mask = ~make_mask(c->prefix);
    return c->addr | host_mask;
}

int first_host(CidrBlock *c) {
    if (c->prefix >= 31) return network_addr(c);
    return network_addr(c) + 1;
}

int last_host(CidrBlock *c) {
    if (c->prefix >= 31) return broadcast_addr(c);
    return broadcast_addr(c) - 1;
}

// Number of total addresses in the block
int block_size(int prefix) {
    if (prefix == 0) return 0; // special case to avoid overflow
    return 1 << (32 - prefix);
}

// Number of usable host addresses
int host_count(int prefix) {
    if (prefix >= 31) return 0;
    return (1 << (32 - prefix)) - 2;
}

// Check if IP is in CIDR block
int ip_in_block(int ip, CidrBlock *c) {
    int mask = make_mask(c->prefix);
    return (ip & mask) == (c->addr & mask);
}

// Check if block b is a subnet of block a
int is_subnet_of(CidrBlock *sub, CidrBlock *super) {
    if (sub->prefix < super->prefix) return 0;
    int mask = make_mask(super->prefix);
    return (sub->addr & mask) == (super->addr & mask);
}

// Split a CIDR block into two halves (prefix + 1)
void split_block(CidrBlock *orig, CidrBlock *lo, CidrBlock *hi) {
    int new_prefix = orig->prefix + 1;
    lo->prefix = new_prefix;
    hi->prefix = new_prefix;
    lo->addr = network_addr(orig);
    // The high half starts at network + half of block size
    int half = 1 << (32 - new_prefix);
    hi->addr = network_addr(orig) + half;
}

// Summarize two adjacent blocks into one (if possible)
int can_summarize(CidrBlock *a, CidrBlock *b) {
    if (a->prefix != b->prefix) return 0;
    if (a->prefix == 0) return 0;
    int parent_prefix = a->prefix - 1;
    int parent_mask = make_mask(parent_prefix);
    return (a->addr & parent_mask) == (b->addr & parent_mask);
}

int main(void) {
    CidrBlock block;

    // 192.168.1.0/24
    block.addr = ip4(192, 168, 1, 0);
    block.prefix = 24;

    print_ip(network_addr(&block));
    printf("\n");
    // EXPECT: 192.168.1.0

    print_ip(broadcast_addr(&block));
    printf("\n");
    // EXPECT: 192.168.1.255

    print_ip(first_host(&block));
    printf("\n");
    // EXPECT: 192.168.1.1

    print_ip(last_host(&block));
    printf("\n");
    // EXPECT: 192.168.1.254

    printf("size=%d hosts=%d\n", block_size(24), host_count(24));
    // EXPECT: size=256 hosts=254

    // 10.0.0.0/8
    block.addr = ip4(10, 0, 0, 0);
    block.prefix = 8;
    printf("size=%d hosts=%d\n", block_size(8), host_count(8));
    // EXPECT: size=16777216 hosts=16777214

    print_ip(first_host(&block));
    printf("\n");
    // EXPECT: 10.0.0.1

    print_ip(last_host(&block));
    printf("\n");
    // EXPECT: 10.255.255.254

    // IP in block test
    printf("in=%d\n", ip_in_block(ip4(10, 1, 2, 3), &block));
    // EXPECT: in=1

    printf("in=%d\n", ip_in_block(ip4(11, 0, 0, 1), &block));
    // EXPECT: in=0

    // /28 block
    block.addr = ip4(172, 16, 0, 0);
    block.prefix = 28;
    printf("size=%d hosts=%d\n", block_size(28), host_count(28));
    // EXPECT: size=16 hosts=14

    print_ip(network_addr(&block));
    printf("\n");
    // EXPECT: 172.16.0.0

    print_ip(broadcast_addr(&block));
    printf("\n");
    // EXPECT: 172.16.0.15

    // Subnet relationship
    CidrBlock big;
    big.addr = ip4(10, 0, 0, 0);
    big.prefix = 8;

    CidrBlock small;
    small.addr = ip4(10, 1, 0, 0);
    small.prefix = 16;

    printf("subnet=%d\n", is_subnet_of(&small, &big));
    // EXPECT: subnet=1

    printf("subnet=%d\n", is_subnet_of(&big, &small));
    // EXPECT: subnet=0

    CidrBlock other;
    other.addr = ip4(172, 16, 0, 0);
    other.prefix = 16;
    printf("subnet=%d\n", is_subnet_of(&other, &big));
    // EXPECT: subnet=0

    // Split a /24 into two /25s
    CidrBlock orig;
    orig.addr = ip4(192, 168, 1, 0);
    orig.prefix = 24;

    CidrBlock lo;
    CidrBlock hi;
    split_block(&orig, &lo, &hi);

    print_ip(lo.addr);
    printf("/%d\n", lo.prefix);
    // EXPECT: 192.168.1.0/25

    print_ip(hi.addr);
    printf("/%d\n", hi.prefix);
    // EXPECT: 192.168.1.128/25

    printf("lo_hosts=%d hi_hosts=%d\n", host_count(lo.prefix), host_count(hi.prefix));
    // EXPECT: lo_hosts=126 hi_hosts=126

    // Can summarize test
    CidrBlock a;
    a.addr = ip4(192, 168, 1, 0);
    a.prefix = 25;
    CidrBlock b;
    b.addr = ip4(192, 168, 1, 128);
    b.prefix = 25;
    printf("summarize=%d\n", can_summarize(&a, &b));
    // EXPECT: summarize=1

    // Cannot summarize non-adjacent
    b.addr = ip4(192, 168, 2, 128);
    printf("summarize=%d\n", can_summarize(&a, &b));
    // EXPECT: summarize=0

    // /30 - point to point
    block.addr = ip4(10, 0, 0, 4);
    block.prefix = 30;
    printf("size=%d hosts=%d\n", block_size(30), host_count(30));
    // EXPECT: size=4 hosts=2

    printf("cidr done\n");
    // EXPECT: cidr done

    return 0;
}
