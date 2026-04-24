int printf(const char *fmt, ...);
// EXPECT: Packet 1: 0x45006421\n  version: 1\n  type: 2\n  length: 100\n  seq: 0\n  checksum: 0x45\n  valid: 1\nPacket 2: 0x872AFF52\n  version: 2\n  type: 5\n  length: 255\n  seq: 42\n  valid: 1\nCorrupted: valid=0\nPacket sequence:\n  seq=0 len=50 checksum=0x03 valid=1\n  seq=1 len=60 checksum=0x0C valid=1\n  seq=2 len=70 checksum=0x75 valid=1\n  seq=3 len=80 checksum=0x62 valid=1\n  seq=4 len=90 checksum=0x6F valid=1\nProtocol types:\n  SYN type=2\n  ACK type=1\n  DATA type=0 len=128\n  FIN type=4
// Test: bit-level protocol simulation

// Simulate a simple packet protocol:
// Bits [0-3]:   version (4 bits)
// Bits [4-7]:   type (4 bits)
// Bits [8-15]:  length (8 bits)
// Bits [16-23]: sequence number (8 bits)
// Bits [24-31]: checksum (8 bits)

int make_packet(int version, int type, int length, int seq) {
    int pkt = 0;
    pkt = pkt | (version & 0xF);
    pkt = pkt | ((type & 0xF) << 4);
    pkt = pkt | ((length & 0xFF) << 8);
    pkt = pkt | ((seq & 0xFF) << 16);
    // Compute checksum: XOR of all other bytes
    int cksum = (pkt & 0xFF) ^ ((pkt >> 8) & 0xFF) ^ ((pkt >> 16) & 0xFF);
    pkt = pkt | ((cksum & 0xFF) << 24);
    return pkt;
}

int get_version(int pkt)  { return pkt & 0xF; }
int get_type(int pkt)     { return (pkt >> 4) & 0xF; }
int get_length(int pkt)   { return (pkt >> 8) & 0xFF; }
int get_seq(int pkt)      { return (pkt >> 16) & 0xFF; }
int get_checksum(int pkt) { return (pkt >> 24) & 0xFF; }

int verify_checksum(int pkt) {
    int byte0 = pkt & 0xFF;
    int byte1 = (pkt >> 8) & 0xFF;
    int byte2 = (pkt >> 16) & 0xFF;
    int byte3 = (pkt >> 24) & 0xFF;
    return byte3 == (byte0 ^ byte1 ^ byte2);
}

int main(void) {
    // Create packets
    int pkt1 = make_packet(1, 2, 100, 0);
    printf("Packet 1: 0x%08X\n", pkt1);
    printf("  version: %d\n", get_version(pkt1));
    printf("  type: %d\n", get_type(pkt1));
    printf("  length: %d\n", get_length(pkt1));
    printf("  seq: %d\n", get_seq(pkt1));
    printf("  checksum: 0x%02X\n", get_checksum(pkt1));
    printf("  valid: %d\n", verify_checksum(pkt1));

    int pkt2 = make_packet(2, 5, 255, 42);
    printf("Packet 2: 0x%08X\n", pkt2);
    printf("  version: %d\n", get_version(pkt2));
    printf("  type: %d\n", get_type(pkt2));
    printf("  length: %d\n", get_length(pkt2));
    printf("  seq: %d\n", get_seq(pkt2));
    printf("  valid: %d\n", verify_checksum(pkt2));

    // Corrupt a packet and detect
    int corrupted = pkt1 ^ (1 << 10);  // flip bit 10
    printf("Corrupted: valid=%d\n", verify_checksum(corrupted));

    // Simulate packet sequence
    printf("Packet sequence:\n");
    int i;
    for (i = 0; i < 5; i++) {
        int p = make_packet(1, 3, 50 + i * 10, i);
        printf("  seq=%d len=%d checksum=0x%02X valid=%d\n",
               get_seq(p), get_length(p), get_checksum(p), verify_checksum(p));
    }

    // Protocol flags in type field
    enum {
        TYPE_DATA = 0,
        TYPE_ACK = 1,
        TYPE_SYN = 2,
        TYPE_FIN = 4,
        TYPE_RST = 8
    };

    int syn = make_packet(1, TYPE_SYN, 0, 0);
    int ack = make_packet(1, TYPE_ACK, 0, 0);
    int data = make_packet(1, TYPE_DATA, 128, 1);
    int fin = make_packet(1, TYPE_FIN, 0, 2);

    printf("Protocol types:\n");
    printf("  SYN type=%d\n", get_type(syn));
    printf("  ACK type=%d\n", get_type(ack));
    printf("  DATA type=%d len=%d\n", get_type(data), get_length(data));
    printf("  FIN type=%d\n", get_type(fin));

    return 0;
}
