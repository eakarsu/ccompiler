int printf(const char *fmt, ...);

// Subnet Mask Calculator

struct SubnetInfo {
    int mask[4];
    int network[4];
    int broadcast[4];
    int prefix_len;
    int num_hosts;
};

void compute_mask(int prefix, int *mask) {
    int i;
    for (i = 0; i < 4; i++) {
        if (prefix >= 8) {
            mask[i] = 255;
            prefix = prefix - 8;
        } else if (prefix > 0) {
            int val = 0;
            int bit = 128;
            int j;
            for (j = 0; j < prefix; j++) {
                val = val + bit;
                bit = bit / 2;
            }
            mask[i] = val;
            prefix = 0;
        } else {
            mask[i] = 0;
        }
    }
}

void compute_network(int *ip, int *mask, int *net) {
    int i;
    for (i = 0; i < 4; i++) {
        net[i] = ip[i] & mask[i];
    }
}

void compute_broadcast(int *net, int *mask, int *bcast) {
    int i;
    for (i = 0; i < 4; i++) {
        bcast[i] = net[i] | (255 - mask[i]);
    }
}

int power_of_2(int n) {
    int result = 1;
    int i;
    for (i = 0; i < n; i++) {
        result = result * 2;
    }
    return result;
}

struct SubnetInfo calc_subnet(int *ip, int prefix) {
    struct SubnetInfo info;
    info.prefix_len = prefix;

    compute_mask(prefix, info.mask);
    compute_network(ip, info.mask, info.network);
    compute_broadcast(info.network, info.mask, info.broadcast);

    if (prefix <= 30) {
        info.num_hosts = power_of_2(32 - prefix) - 2;
    } else {
        info.num_hosts = 0;
    }

    return info;
}

int same_subnet(int *ip1, int *ip2, int *mask) {
    int i;
    for (i = 0; i < 4; i++) {
        if ((ip1[i] & mask[i]) != (ip2[i] & mask[i])) return 0;
    }
    return 1;
}

int is_valid_prefix(int prefix) {
    return prefix >= 0 && prefix <= 32;
}

int count_ones_in_byte(int b) {
    int count = 0;
    int bit = 128;
    while (bit > 0) {
        if (b & bit) count = count + 1;
        bit = bit / 2;
    }
    return count;
}

int mask_to_prefix(int *mask) {
    int total = 0;
    int i;
    for (i = 0; i < 4; i++) {
        total = total + count_ones_in_byte(mask[i]);
    }
    return total;
}

int main() {
    int ip[4];
    struct SubnetInfo info;

    // Test 1: /24 subnet
    ip[0] = 192; ip[1] = 168; ip[2] = 1; ip[3] = 100;
    info = calc_subnet(ip, 24);
    printf("mask: %d.%d.%d.%d\n", info.mask[0], info.mask[1], info.mask[2], info.mask[3]);
    // EXPECT: mask: 255.255.255.0
    printf("network: %d.%d.%d.%d\n", info.network[0], info.network[1], info.network[2], info.network[3]);
    // EXPECT: network: 192.168.1.0
    printf("broadcast: %d.%d.%d.%d\n", info.broadcast[0], info.broadcast[1], info.broadcast[2], info.broadcast[3]);
    // EXPECT: broadcast: 192.168.1.255
    printf("hosts: %d\n", info.num_hosts);
    // EXPECT: hosts: 254

    // Test 2: /16 subnet
    ip[0] = 172; ip[1] = 16; ip[2] = 50; ip[3] = 10;
    info = calc_subnet(ip, 16);
    printf("mask16: %d.%d.%d.%d\n", info.mask[0], info.mask[1], info.mask[2], info.mask[3]);
    // EXPECT: mask16: 255.255.0.0
    printf("net16: %d.%d.%d.%d\n", info.network[0], info.network[1], info.network[2], info.network[3]);
    // EXPECT: net16: 172.16.0.0
    printf("hosts16: %d\n", info.num_hosts);
    // EXPECT: hosts16: 65534

    // Test 3: /28 subnet
    ip[0] = 10; ip[1] = 0; ip[2] = 0; ip[3] = 200;
    info = calc_subnet(ip, 28);
    printf("mask28: %d.%d.%d.%d\n", info.mask[0], info.mask[1], info.mask[2], info.mask[3]);
    // EXPECT: mask28: 255.255.255.240
    printf("net28: %d.%d.%d.%d\n", info.network[0], info.network[1], info.network[2], info.network[3]);
    // EXPECT: net28: 10.0.0.192
    printf("bcast28: %d.%d.%d.%d\n", info.broadcast[0], info.broadcast[1], info.broadcast[2], info.broadcast[3]);
    // EXPECT: bcast28: 10.0.0.207
    printf("hosts28: %d\n", info.num_hosts);
    // EXPECT: hosts28: 14

    // Test 4: same subnet check
    int ip1[4]; ip1[0] = 192; ip1[1] = 168; ip1[2] = 1; ip1[3] = 10;
    int ip2[4]; ip2[0] = 192; ip2[1] = 168; ip2[2] = 1; ip2[3] = 200;
    int ip3[4]; ip3[0] = 192; ip3[1] = 168; ip3[2] = 2; ip3[3] = 10;
    int mask24[4]; mask24[0] = 255; mask24[1] = 255; mask24[2] = 255; mask24[3] = 0;
    printf("same /24 subnet 1-2: %d\n", same_subnet(ip1, ip2, mask24));
    // EXPECT: same /24 subnet 1-2: 1
    printf("same /24 subnet 1-3: %d\n", same_subnet(ip1, ip3, mask24));
    // EXPECT: same /24 subnet 1-3: 0

    // Test 5: mask to prefix conversion
    int m1[4]; m1[0] = 255; m1[1] = 255; m1[2] = 255; m1[3] = 0;
    printf("mask2prefix 255.255.255.0: %d\n", mask_to_prefix(m1));
    // EXPECT: mask2prefix 255.255.255.0: 24

    int m2[4]; m2[0] = 255; m2[1] = 255; m2[2] = 0; m2[3] = 0;
    printf("mask2prefix 255.255.0.0: %d\n", mask_to_prefix(m2));
    // EXPECT: mask2prefix 255.255.0.0: 16

    int m3[4]; m3[0] = 255; m3[1] = 255; m3[2] = 255; m3[3] = 240;
    printf("mask2prefix 255.255.255.240: %d\n", mask_to_prefix(m3));
    // EXPECT: mask2prefix 255.255.255.240: 28

    // Test 6: valid prefix check
    printf("valid prefix 24: %d\n", is_valid_prefix(24));
    // EXPECT: valid prefix 24: 1
    printf("valid prefix 33: %d\n", is_valid_prefix(33));
    // EXPECT: valid prefix 33: 0
    printf("valid prefix -1: %d\n", is_valid_prefix(-1));
    // EXPECT: valid prefix -1: 0

    // Test 7: /8 subnet
    ip[0] = 10; ip[1] = 50; ip[2] = 100; ip[3] = 200;
    info = calc_subnet(ip, 8);
    printf("net8: %d.%d.%d.%d\n", info.network[0], info.network[1], info.network[2], info.network[3]);
    // EXPECT: net8: 10.0.0.0
    printf("bcast8: %d.%d.%d.%d\n", info.broadcast[0], info.broadcast[1], info.broadcast[2], info.broadcast[3]);
    // EXPECT: bcast8: 10.255.255.255

    return 0;
}
