int printf(const char *fmt, ...);

// Packet checksum computation

typedef struct {
    int src_ip;
    int dst_ip;
    int src_port;
    int dst_port;
    int protocol;     // 6=TCP, 17=UDP
    int length;
    int data[16];
    int data_len;
} Packet;

int ip4(int a, int b, int c, int d) {
    return (a << 24) | (b << 16) | (c << 8) | d;
}

// Simple internet-style checksum: 16-bit ones-complement sum
// We simulate with 16-bit arithmetic
int checksum16(int *data, int count) {
    int sum = 0;
    int i;
    for (i = 0; i < count; i++) {
        sum = sum + (data[i] & 0xFFFF);
        // Fold carry
        if (sum > 0xFFFF) {
            sum = (sum & 0xFFFF) + 1;
        }
    }
    return (~sum) & 0xFFFF;
}

// IP header checksum (simplified)
int ip_header_checksum(Packet *p) {
    int words[8];
    words[0] = (p->src_ip >> 16) & 0xFFFF;
    words[1] = p->src_ip & 0xFFFF;
    words[2] = (p->dst_ip >> 16) & 0xFFFF;
    words[3] = p->dst_ip & 0xFFFF;
    words[4] = p->protocol;
    words[5] = p->length;
    words[6] = p->src_port;
    words[7] = p->dst_port;
    return checksum16(words, 8);
}

// Data payload checksum
int data_checksum(Packet *p) {
    return checksum16(p->data, p->data_len);
}

// XOR-based checksum (like simple parity check)
int xor_checksum(int *data, int count) {
    int result = 0;
    int i;
    for (i = 0; i < count; i++) {
        result = result ^ data[i];
    }
    return result;
}

// Fletcher-16 checksum (simplified)
int fletcher16(int *data, int count) {
    int sum1 = 0;
    int sum2 = 0;
    int i;
    for (i = 0; i < count; i++) {
        sum1 = (sum1 + (data[i] & 0xFF)) % 255;
        sum2 = (sum2 + sum1) % 255;
    }
    return (sum2 << 8) | sum1;
}

// Adler-like checksum (simplified to 16-bit)
int adler16(int *data, int count) {
    int a = 1;
    int b = 0;
    int i;
    for (i = 0; i < count; i++) {
        a = (a + (data[i] & 0xFF)) % 251;
        b = (b + a) % 251;
    }
    return (b << 8) | a;
}

// CRC-like computation (simplified 8-bit)
int crc8(int *data, int count) {
    int crc = 0;
    int i;
    int j;
    for (i = 0; i < count; i++) {
        crc = crc ^ (data[i] & 0xFF);
        for (j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0x8C;  // polynomial
            } else {
                crc = crc >> 1;
            }
        }
    }
    return crc & 0xFF;
}

// Verify a checksum by including the checksum value
int verify_checksum16(int *data, int count, int cksum) {
    int words[20];
    int i;
    for (i = 0; i < count && i < 19; i++) {
        words[i] = data[i];
    }
    words[count] = cksum;
    // If checksum is correct, result should be 0 or 0xFFFF
    int sum = 0;
    for (i = 0; i <= count; i++) {
        sum = sum + (words[i] & 0xFFFF);
        if (sum > 0xFFFF) {
            sum = (sum & 0xFFFF) + 1;
        }
    }
    return (sum & 0xFFFF) == 0xFFFF;
}

int main(void) {
    // Test basic checksum
    int data1[4];
    data1[0] = 0x0001;
    data1[1] = 0x00F2;
    data1[2] = 0x0003;
    data1[3] = 0x0004;
    int ck = checksum16(data1, 4);
    printf("cksum=%d\n", ck);
    // EXPECT: cksum=65285

    // Verify the checksum
    int ok = verify_checksum16(data1, 4, ck);
    printf("verify=%d\n", ok);
    // EXPECT: verify=1

    // XOR checksum
    int data2[4];
    data2[0] = 0xAA;
    data2[1] = 0x55;
    data2[2] = 0xFF;
    data2[3] = 0x00;
    int xck = xor_checksum(data2, 4);
    // 0xAA ^ 0x55 = 0xFF, 0xFF ^ 0xFF = 0x00, 0x00 ^ 0x00 = 0x00
    printf("xor=%d\n", xck);
    // EXPECT: xor=0

    data2[3] = 0x01;
    xck = xor_checksum(data2, 4);
    // 0xAA ^ 0x55 = 0xFF, 0xFF ^ 0xFF = 0x00, 0x00 ^ 0x01 = 0x01
    printf("xor=%d\n", xck);
    // EXPECT: xor=1

    // Fletcher-16 test
    int data3[4];
    data3[0] = 1;
    data3[1] = 2;
    data3[2] = 3;
    data3[3] = 4;
    int fck = fletcher16(data3, 4);
    // sum1: 1,3,6,10 mod 255 = 10
    // sum2: 1,4,10,20 mod 255 = 20
    // result = (20 << 8) | 10 = 5130
    printf("fletcher=%d\n", fck);
    // EXPECT: fletcher=5130

    // Adler-16 test
    int ack = adler16(data3, 4);
    // a: 1->2->4->7->11 all mod 251 = 11
    // b: 0->2->6->13->24 all mod 251 = 24
    // result = (24 << 8) | 11 = 6155
    printf("adler=%d\n", ack);
    // EXPECT: adler=6155

    // CRC-8 test with known data
    int data4[3];
    data4[0] = 0x31;
    data4[1] = 0x32;
    data4[2] = 0x33;
    int cr = crc8(data4, 3);
    printf("crc8=%d\n", cr);
    // EXPECT: crc8=149

    // Packet checksum test
    Packet pkt;
    pkt.src_ip = ip4(192,168,1,10);
    pkt.dst_ip = ip4(192,168,1,20);
    pkt.src_port = 8080;
    pkt.dst_port = 80;
    pkt.protocol = 6; // TCP
    pkt.length = 40;
    int hck = ip_header_checksum(&pkt);
    printf("hdr_cksum=%d\n", hck);
    // EXPECT: hdr_cksum=23682

    // Packet data checksum
    pkt.data[0] = 0x4865;  // "He"
    pkt.data[1] = 0x6C6C;  // "ll"
    pkt.data[2] = 0x6F00;  // "o\0"
    pkt.data_len = 3;
    int dck = data_checksum(&pkt);
    printf("data_cksum=%d\n", dck);
    // EXPECT: data_cksum=56365

    // All zeros checksum
    int zeros[4];
    zeros[0] = 0;
    zeros[1] = 0;
    zeros[2] = 0;
    zeros[3] = 0;
    ck = checksum16(zeros, 4);
    printf("zero_cksum=%d\n", ck);
    // EXPECT: zero_cksum=65535

    // Single value
    int one[1];
    one[0] = 0x1234;
    ck = checksum16(one, 1);
    printf("single_cksum=%d\n", ck);
    // EXPECT: single_cksum=60875

    // XOR of identical values = 0
    int same[2];
    same[0] = 12345;
    same[1] = 12345;
    printf("xor_same=%d\n", xor_checksum(same, 2));
    // EXPECT: xor_same=0

    printf("checksum done\n");
    // EXPECT: checksum done

    return 0;
}
