int printf(const char *fmt, ...);
// EXPECT: === Network Protocol Simulation ===\n--- Three-Way Handshake ---\n  SEND: SYN seq=1000 ack=0 size=0 chk=232\n  RECV: SYN seq=1000 ack=0 size=0\n  SEND: SYN-ACK seq=2000 ack=1001 size=0 chk=186\n  RECV: SYN-ACK seq=2000 ack=1001 size=0\n  SEND: ACK seq=1001 ack=2001 size=0 chk=188\n  RECV: ACK seq=1001 ack=2001 size=0\nConnection established!\n--- Data Transfer ---\n  SEND: DATA seq=1001 ack=2001 size=100 chk=33\n  RECV: DATA seq=1001 ack=2001 size=100\n  SEND: ACK seq=2000 ack=1101 size=0 chk=31\n  RECV: ACK seq=2000 ack=1101 size=0\n  SEND: DATA seq=1101 ack=2001 size=200 chk=233\n  RECV: DATA seq=1101 ack=2001 size=200\n  SEND: ACK seq=2000 ack=1301 size=0 chk=231\n  RECV: ACK seq=2000 ack=1301 size=0\n  SEND: DATA seq=1301 ack=2001 size=150 chk=127\n  RECV: DATA seq=1301 ack=2001 size=150\n  SEND: ACK seq=2000 ack=1451 size=0 chk=125\n  RECV: ACK seq=2000 ack=1451 size=0\n  SEND: DATA seq=1451 ack=2001 size=100 chk=227\n  RECV: DATA seq=1451 ack=2001 size=100\n  SEND: ACK seq=2000 ack=1551 size=0 chk=225\n  RECV: ACK seq=2000 ack=1551 size=0\n  SEND: DATA seq=1551 ack=2001 size=50 chk=21\n  RECV: DATA seq=1551 ack=2001 size=50\n  SEND: ACK seq=2000 ack=1601 size=0 chk=19\n  RECV: ACK seq=2000 ack=1601 size=0\n--- Connection Teardown ---\n  SEND: FIN seq=1601 ack=2001 size=0 chk=22\n  RECV: FIN seq=1601 ack=2001 size=0\n  SEND: FIN-ACK seq=2000 ack=1602 size=0 chk=23\n  RECV: FIN-ACK seq=2000 ack=1602 size=0\n  SEND: ACK seq=1602 ack=2001 size=0 chk=21\n  RECV: ACK seq=1602 ack=2001 size=0\n--- Statistics ---\nClient: sent=9 pkts/600 bytes, recv=7 pkts/0 bytes\nServer: sent=7 pkts/0 bytes, recv=9 pkts/600 bytes

enum PacketType { SYN, SYN_ACK, ACK, DATA, FIN, FIN_ACK };

struct Packet {
    int seq_num;
    int ack_num;
    enum PacketType type;
    int data_size;
    int checksum;
};

struct Connection {
    int state;
    int local_seq;
    int remote_seq;
    int window_size;
    int bytes_sent;
    int bytes_received;
    int packets_sent;
    int packets_received;
    int retransmissions;
};

const char *packet_type_name(enum PacketType t) {
    if (t == SYN) return "SYN";
    if (t == SYN_ACK) return "SYN-ACK";
    if (t == ACK) return "ACK";
    if (t == DATA) return "DATA";
    if (t == FIN) return "FIN";
    return "FIN-ACK";
}

int compute_checksum(struct Packet *p) {
    int sum = p->seq_num + p->ack_num + p->type + p->data_size;
    return sum % 256;
}

void init_connection(struct Connection *conn) {
    conn->state = 0;
    conn->local_seq = 1000;
    conn->remote_seq = 0;
    conn->window_size = 4;
    conn->bytes_sent = 0;
    conn->bytes_received = 0;
    conn->packets_sent = 0;
    conn->packets_received = 0;
    conn->retransmissions = 0;
}

struct Packet make_packet(enum PacketType type, int seq, int ack, int size) {
    struct Packet p;
    p.type = type;
    p.seq_num = seq;
    p.ack_num = ack;
    p.data_size = size;
    p.checksum = compute_checksum(&p);
    return p;
}

void send_packet(struct Connection *conn, struct Packet *p) {
    conn->packets_sent++;
    conn->bytes_sent = conn->bytes_sent + p->data_size;
    printf("  SEND: %s seq=%d ack=%d size=%d chk=%d\n",
           packet_type_name(p->type), p->seq_num, p->ack_num,
           p->data_size, p->checksum);
}

void receive_packet(struct Connection *conn, struct Packet *p) {
    conn->packets_received++;
    conn->bytes_received = conn->bytes_received + p->data_size;
    printf("  RECV: %s seq=%d ack=%d size=%d\n",
           packet_type_name(p->type), p->seq_num, p->ack_num, p->data_size);
}

int main(void) {
    struct Connection client;
    struct Connection server;
    init_connection(&client);
    init_connection(&server);
    server.local_seq = 2000;

    printf("=== Network Protocol Simulation ===\n");

    printf("--- Three-Way Handshake ---\n");
    struct Packet syn = make_packet(SYN, client.local_seq, 0, 0);
    send_packet(&client, &syn);
    receive_packet(&server, &syn);

    server.remote_seq = syn.seq_num + 1;
    struct Packet syn_ack = make_packet(SYN_ACK, server.local_seq, server.remote_seq, 0);
    send_packet(&server, &syn_ack);
    receive_packet(&client, &syn_ack);

    client.remote_seq = syn_ack.seq_num + 1;
    client.local_seq = syn_ack.ack_num;
    struct Packet ack = make_packet(ACK, client.local_seq, client.remote_seq, 0);
    send_packet(&client, &ack);
    receive_packet(&server, &ack);
    printf("Connection established!\n");

    printf("--- Data Transfer ---\n");
    int data_sizes[5];
    data_sizes[0] = 100; data_sizes[1] = 200; data_sizes[2] = 150;
    data_sizes[3] = 100; data_sizes[4] = 50;

    int i;
    for (i = 0; i < 5; i++) {
        struct Packet data = make_packet(DATA, client.local_seq, client.remote_seq, data_sizes[i]);
        send_packet(&client, &data);
        receive_packet(&server, &data);

        client.local_seq = client.local_seq + data_sizes[i];
        server.remote_seq = client.local_seq;

        struct Packet data_ack = make_packet(ACK, server.local_seq, server.remote_seq, 0);
        send_packet(&server, &data_ack);
        receive_packet(&client, &data_ack);
    }

    printf("--- Connection Teardown ---\n");
    struct Packet fin = make_packet(FIN, client.local_seq, client.remote_seq, 0);
    send_packet(&client, &fin);
    receive_packet(&server, &fin);

    struct Packet fin_ack = make_packet(FIN_ACK, server.local_seq, client.local_seq + 1, 0);
    send_packet(&server, &fin_ack);
    receive_packet(&client, &fin_ack);

    struct Packet final_ack = make_packet(ACK, client.local_seq + 1, server.local_seq + 1, 0);
    send_packet(&client, &final_ack);
    receive_packet(&server, &final_ack);

    printf("--- Statistics ---\n");
    printf("Client: sent=%d pkts/%d bytes, recv=%d pkts/%d bytes\n",
           client.packets_sent, client.bytes_sent,
           client.packets_received, client.bytes_received);
    printf("Server: sent=%d pkts/%d bytes, recv=%d pkts/%d bytes\n",
           server.packets_sent, server.bytes_sent,
           server.packets_received, server.bytes_received);

    return 0;
}
