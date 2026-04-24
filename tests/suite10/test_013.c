int printf(const char *fmt, ...);

// Packet Header Builder

struct EthernetHeader {
    int dst_mac[6];
    int src_mac[6];
    int ethertype;
};

struct IPv4Header {
    int version;
    int ihl;
    int total_length;
    int ttl;
    int protocol;
    int src_ip[4];
    int dst_ip[4];
    int checksum;
};

struct TCPHeader {
    int src_port;
    int dst_port;
    int seq_num;
    int ack_num;
    int data_offset;
    int flags;
    int window_size;
    int checksum;
};

struct Packet {
    struct EthernetHeader eth;
    struct IPv4Header ip;
    struct TCPHeader tcp;
    int payload_len;
};

void build_ethernet(struct EthernetHeader *eth, int *dst, int *src, int etype) {
    int i;
    for (i = 0; i < 6; i++) {
        eth->dst_mac[i] = dst[i];
        eth->src_mac[i] = src[i];
    }
    eth->ethertype = etype;
}

void build_ipv4(struct IPv4Header *ip, int *src, int *dst, int proto, int payload) {
    ip->version = 4;
    ip->ihl = 5;
    ip->total_length = 20 + payload;
    ip->ttl = 64;
    ip->protocol = proto;
    int i;
    for (i = 0; i < 4; i++) {
        ip->src_ip[i] = src[i];
        ip->dst_ip[i] = dst[i];
    }
    // Simple checksum simulation
    ip->checksum = (ip->version + ip->ihl + ip->total_length + ip->ttl + ip->protocol) & 255;
}

void build_tcp(struct TCPHeader *tcp, int sport, int dport, int seq, int ack, int flags) {
    tcp->src_port = sport;
    tcp->dst_port = dport;
    tcp->seq_num = seq;
    tcp->ack_num = ack;
    tcp->data_offset = 5;
    tcp->flags = flags;
    tcp->window_size = 65535;
    tcp->checksum = (sport + dport + seq + ack + flags) & 255;
}

int get_tcp_flag_syn(int flags) { return (flags & 2) ? 1 : 0; }
int get_tcp_flag_ack(int flags) { return (flags & 16) ? 1 : 0; }
int get_tcp_flag_fin(int flags) { return (flags & 1) ? 1 : 0; }
int get_tcp_flag_rst(int flags) { return (flags & 4) ? 1 : 0; }
int get_tcp_flag_psh(int flags) { return (flags & 8) ? 1 : 0; }

char *proto_name(int proto) {
    if (proto == 6) return "TCP";
    if (proto == 17) return "UDP";
    if (proto == 1) return "ICMP";
    return "UNKNOWN";
}

int total_packet_size(struct Packet *pkt) {
    return 14 + pkt->ip.total_length;
}

void build_full_packet(struct Packet *pkt, int *smac, int *dmac,
                        int *sip, int *dip,
                        int sport, int dport,
                        int seq, int flags, int payload) {
    build_ethernet(&pkt->eth, dmac, smac, 2048);
    build_ipv4(&pkt->ip, sip, dip, 6, 20 + payload);
    build_tcp(&pkt->tcp, sport, dport, seq, 0, flags);
    pkt->payload_len = payload;
}

int main() {
    struct EthernetHeader eth;
    int dst[6]; dst[0] = 255; dst[1] = 255; dst[2] = 255; dst[3] = 255; dst[4] = 255; dst[5] = 255;
    int src[6]; src[0] = 170; src[1] = 187; src[2] = 204; src[3] = 221; src[4] = 238; src[5] = 255;

    // Test 1: Ethernet header
    build_ethernet(&eth, dst, src, 2048);
    printf("eth dst: %d:%d:%d:%d:%d:%d\n", eth.dst_mac[0], eth.dst_mac[1], eth.dst_mac[2], eth.dst_mac[3], eth.dst_mac[4], eth.dst_mac[5]);
    // EXPECT: eth dst: 255:255:255:255:255:255
    printf("eth src: %d:%d:%d:%d:%d:%d\n", eth.src_mac[0], eth.src_mac[1], eth.src_mac[2], eth.src_mac[3], eth.src_mac[4], eth.src_mac[5]);
    // EXPECT: eth src: 170:187:204:221:238:255
    printf("ethertype: %d\n", eth.ethertype);
    // EXPECT: ethertype: 2048

    // Test 2: IPv4 header
    struct IPv4Header iph;
    int sip[4]; sip[0] = 192; sip[1] = 168; sip[2] = 1; sip[3] = 10;
    int dip[4]; dip[0] = 10; dip[1] = 0; dip[2] = 0; dip[3] = 1;
    build_ipv4(&iph, sip, dip, 6, 40);
    printf("ip ver=%d ihl=%d len=%d ttl=%d\n", iph.version, iph.ihl, iph.total_length, iph.ttl);
    // EXPECT: ip ver=4 ihl=5 len=60 ttl=64
    printf("ip src: %d.%d.%d.%d\n", iph.src_ip[0], iph.src_ip[1], iph.src_ip[2], iph.src_ip[3]);
    // EXPECT: ip src: 192.168.1.10
    printf("ip dst: %d.%d.%d.%d\n", iph.dst_ip[0], iph.dst_ip[1], iph.dst_ip[2], iph.dst_ip[3]);
    // EXPECT: ip dst: 10.0.0.1
    printf("ip proto: %s\n", proto_name(iph.protocol));
    // EXPECT: ip proto: TCP

    // Test 3: TCP header with SYN
    struct TCPHeader tcp;
    build_tcp(&tcp, 12345, 80, 1000, 0, 2);
    printf("tcp %d->%d seq=%d flags=%d\n", tcp.src_port, tcp.dst_port, tcp.seq_num, tcp.flags);
    // EXPECT: tcp 12345->80 seq=1000 flags=2
    printf("syn=%d ack=%d fin=%d rst=%d\n", get_tcp_flag_syn(tcp.flags), get_tcp_flag_ack(tcp.flags), get_tcp_flag_fin(tcp.flags), get_tcp_flag_rst(tcp.flags));
    // EXPECT: syn=1 ack=0 fin=0 rst=0

    // Test 4: TCP header with SYN+ACK
    build_tcp(&tcp, 80, 12345, 5000, 1001, 18);
    printf("synack flags=%d\n", tcp.flags);
    // EXPECT: synack flags=18
    printf("syn=%d ack=%d\n", get_tcp_flag_syn(tcp.flags), get_tcp_flag_ack(tcp.flags));
    // EXPECT: syn=1 ack=1

    // Test 5: Full packet construction
    struct Packet pkt;
    build_full_packet(&pkt, src, dst, sip, dip, 8080, 443, 42, 2, 100);
    printf("pkt sport=%d dport=%d\n", pkt.tcp.src_port, pkt.tcp.dst_port);
    // EXPECT: pkt sport=8080 dport=443
    printf("pkt total_size=%d\n", total_packet_size(&pkt));
    // EXPECT: pkt total_size=154
    printf("pkt payload=%d\n", pkt.payload_len);
    // EXPECT: pkt payload=100

    // Test 6: protocol name lookup
    printf("proto 6: %s\n", proto_name(6));
    // EXPECT: proto 6: TCP
    printf("proto 17: %s\n", proto_name(17));
    // EXPECT: proto 17: UDP
    printf("proto 1: %s\n", proto_name(1));
    // EXPECT: proto 1: ICMP
    printf("proto 99: %s\n", proto_name(99));
    // EXPECT: proto 99: UNKNOWN

    // Test 7: TCP PSH+ACK
    build_tcp(&tcp, 443, 8080, 999, 500, 24);
    printf("psh=%d ack=%d syn=%d\n", get_tcp_flag_psh(tcp.flags), get_tcp_flag_ack(tcp.flags), get_tcp_flag_syn(tcp.flags));
    // EXPECT: psh=1 ack=1 syn=0

    return 0;
}
