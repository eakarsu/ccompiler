int printf(const char *fmt, ...);

int tcp_state_closed = 0;
int tcp_state_listen = 1;
int tcp_state_syn_sent = 2;
int tcp_state_syn_rcvd = 3;
int tcp_state_established = 4;
int tcp_state_fin_wait1 = 5;
int tcp_state_fin_wait2 = 6;
int tcp_state_close_wait = 7;
int tcp_state_closing = 8;
int tcp_state_last_ack = 9;
int tcp_state_time_wait = 10;

struct TCPConnection {
    int state;
    int seq_num;
    int ack_num;
    int local_port;
    int remote_port;
    int window_size;
    int mss;
    int retransmit_count;
};

int state_name_print(int state) {
    if (state == 0) { printf("CLOSED"); }
    if (state == 1) { printf("LISTEN"); }
    if (state == 2) { printf("SYN_SENT"); }
    if (state == 3) { printf("SYN_RCVD"); }
    if (state == 4) { printf("ESTABLISHED"); }
    if (state == 5) { printf("FIN_WAIT_1"); }
    if (state == 6) { printf("FIN_WAIT_2"); }
    if (state == 7) { printf("CLOSE_WAIT"); }
    if (state == 8) { printf("CLOSING"); }
    if (state == 9) { printf("LAST_ACK"); }
    if (state == 10) { printf("TIME_WAIT"); }
    return 0;
}

int tcp_init(struct TCPConnection *conn, int lport, int rport) {
    conn->state = 0;
    conn->seq_num = 1000;
    conn->ack_num = 0;
    conn->local_port = lport;
    conn->remote_port = rport;
    conn->window_size = 65535;
    conn->mss = 1460;
    conn->retransmit_count = 0;
    return 0;
}

int tcp_passive_open(struct TCPConnection *conn) {
    if (conn->state == 0) {
        conn->state = 1;
        printf("Passive open: ");
        state_name_print(conn->state);
        printf("\n");
        // EXPECT: === Server Side (Passive Open) ===
    }
    return 0;
}

int tcp_recv_syn(struct TCPConnection *conn, int remote_seq) {
    if (conn->state == 1) {
        conn->ack_num = remote_seq + 1;
        conn->state = 3;
        printf("Recv SYN (seq=%d): ", remote_seq);
        state_name_print(conn->state);
        printf("\n");
        // EXPECT: Passive open: LISTEN
    }
    return 0;
}

int tcp_recv_ack_synrcvd(struct TCPConnection *conn) {
    if (conn->state == 3) {
        conn->state = 4;
        printf("Recv ACK in SYN_RCVD: ");
        state_name_print(conn->state);
        printf("\n");
        // EXPECT: Recv SYN (seq=5000): SYN_RCVD
    }
    return 0;
}

int tcp_active_open(struct TCPConnection *conn) {
    if (conn->state == 0) {
        conn->state = 2;
        printf("Active open (SYN sent): ");
        state_name_print(conn->state);
        printf("\n");
    }
    return 0;
}

int tcp_recv_synack(struct TCPConnection *conn, int remote_seq) {
    if (conn->state == 2) {
        conn->ack_num = remote_seq + 1;
        conn->state = 4;
        printf("Recv SYN+ACK: ");
        state_name_print(conn->state);
        printf("\n");
    }
    return 0;
}

int tcp_close_active(struct TCPConnection *conn) {
    if (conn->state == 4) {
        conn->state = 5;
        printf("Close (FIN sent): ");
        state_name_print(conn->state);
        printf("\n");
        // EXPECT: Recv ACK in SYN_RCVD: ESTABLISHED
    }
    return 0;
}

int tcp_recv_ack_finwait1(struct TCPConnection *conn) {
    if (conn->state == 5) {
        conn->state = 6;
        printf("Recv ACK in FIN_WAIT_1: ");
        state_name_print(conn->state);
        printf("\n");
        // EXPECT: Server port: 80, seq: 1000, ack: 5001
    }
    return 0;
}

int tcp_recv_fin_finwait2(struct TCPConnection *conn) {
    if (conn->state == 6) {
        conn->state = 10;
        printf("Recv FIN in FIN_WAIT_2: ");
        state_name_print(conn->state);
        printf("\n");
        // EXPECT: Close (FIN sent): FIN_WAIT_1
    }
    return 0;
}

int tcp_timeout_timewait(struct TCPConnection *conn) {
    if (conn->state == 10) {
        conn->state = 0;
        printf("Timeout in TIME_WAIT: ");
        state_name_print(conn->state);
        printf("\n");
        // EXPECT: Recv ACK in FIN_WAIT_1: FIN_WAIT_2
    }
    return 0;
}

int tcp_recv_fin_established(struct TCPConnection *conn) {
    if (conn->state == 4) {
        conn->state = 7;
        printf("Recv FIN in ESTABLISHED: ");
        state_name_print(conn->state);
        printf("\n");
    }
    return 0;
}

int tcp_close_passive(struct TCPConnection *conn) {
    if (conn->state == 7) {
        conn->state = 9;
        printf("Close in CLOSE_WAIT: ");
        state_name_print(conn->state);
        printf("\n");
    }
    return 0;
}

int tcp_recv_ack_lastack(struct TCPConnection *conn) {
    if (conn->state == 9) {
        conn->state = 0;
        printf("Recv ACK in LAST_ACK: ");
        state_name_print(conn->state);
        printf("\n");
    }
    return 0;
}

int main() {
    struct TCPConnection server;
    tcp_init(&server, 80, 0);

    printf("=== Server Side (Passive Open) ===\n");
    // EXPECT: Recv FIN in FIN_WAIT_2: TIME_WAIT
    tcp_passive_open(&server);
    tcp_recv_syn(&server, 5000);
    tcp_recv_ack_synrcvd(&server);

    printf("Server port: %d, seq: %d, ack: %d\n", server.local_port, server.seq_num, server.ack_num);
    // EXPECT: Timeout in TIME_WAIT: CLOSED

    tcp_close_active(&server);
    tcp_recv_ack_finwait1(&server);
    tcp_recv_fin_finwait2(&server);
    tcp_timeout_timewait(&server);

    printf("=== Client Side (Active Close by Remote) ===\n");
    // EXPECT: === Client Side (Active Close by Remote) ===

    struct TCPConnection client;
    tcp_init(&client, 12345, 80);
    client.state = 4;
    tcp_recv_fin_established(&client);
    // EXPECT: Recv FIN in ESTABLISHED: CLOSE_WAIT
    tcp_close_passive(&client);
    // EXPECT: Close in CLOSE_WAIT: LAST_ACK
    tcp_recv_ack_lastack(&client);
    // EXPECT: Recv ACK in LAST_ACK: CLOSED

    printf("=== Simultaneous Close ===\n");
    // EXPECT: === Simultaneous Close ===
    struct TCPConnection peer;
    tcp_init(&peer, 9000, 9001);
    peer.state = 4;
    peer.state = 5;
    printf("Peer in FIN_WAIT_1\n");
    // EXPECT: Peer in FIN_WAIT_1
    peer.state = 8;
    printf("Recv FIN in FIN_WAIT_1: CLOSING\n");
    // EXPECT: Recv FIN in FIN_WAIT_1: CLOSING
    peer.state = 10;
    printf("Recv ACK in CLOSING: TIME_WAIT\n");
    // EXPECT: Recv ACK in CLOSING: TIME_WAIT
    peer.state = 0;
    printf("Timeout: CLOSED\n");
    // EXPECT: Timeout: CLOSED

    printf("Final state: %d\n", peer.state);
    // EXPECT: Final state: 0

    return 0;
}
