int printf(const char *fmt, ...);

// TCP sliding window protocol simulation

typedef struct {
    int seq_num;
    int data;
    int acked;
} Segment;

typedef struct {
    Segment buffer[16];
    int window_size;
    int base;          // oldest unacked sequence number
    int next_seq;      // next sequence number to send
    int total_sent;
    int total_acked;
    int retransmits;
} TcpSender;

typedef struct {
    int expected_seq;
    int received[32];
    int recv_count;
} TcpReceiver;

void sender_init(TcpSender *s, int win_size) {
    s->window_size = win_size;
    s->base = 0;
    s->next_seq = 0;
    s->total_sent = 0;
    s->total_acked = 0;
    s->retransmits = 0;
    int i;
    for (i = 0; i < 16; i++) {
        s->buffer[i].seq_num = -1;
        s->buffer[i].data = 0;
        s->buffer[i].acked = 0;
    }
}

void receiver_init(TcpReceiver *r) {
    r->expected_seq = 0;
    r->recv_count = 0;
    int i;
    for (i = 0; i < 32; i++) {
        r->received[i] = -1;
    }
}

int can_send(TcpSender *s) {
    return s->next_seq < s->base + s->window_size;
}

int send_segment(TcpSender *s, int data) {
    if (!can_send(s)) return -1;
    int idx = s->next_seq % 16;
    s->buffer[idx].seq_num = s->next_seq;
    s->buffer[idx].data = data;
    s->buffer[idx].acked = 0;
    s->next_seq = s->next_seq + 1;
    s->total_sent = s->total_sent + 1;
    return s->next_seq - 1;
}

// Receiver processes a segment, returns ACK number
// Returns expected_seq (cumulative ACK)
int receive_segment(TcpReceiver *r, int seq, int data) {
    if (seq == r->expected_seq) {
        r->received[r->recv_count] = data;
        r->recv_count = r->recv_count + 1;
        r->expected_seq = r->expected_seq + 1;
        return r->expected_seq;
    }
    // Out of order - send duplicate ACK
    return r->expected_seq;
}

void process_ack(TcpSender *s, int ack_num) {
    // ACK acknowledges all segments with seq < ack_num
    while (s->base < ack_num) {
        int idx = s->base % 16;
        if (!s->buffer[idx].acked) {
            s->buffer[idx].acked = 1;
            s->total_acked = s->total_acked + 1;
        }
        s->base = s->base + 1;
    }
}

int window_used(TcpSender *s) {
    return s->next_seq - s->base;
}

int retransmit(TcpSender *s) {
    // Retransmit the base segment
    if (s->base < s->next_seq) {
        s->retransmits = s->retransmits + 1;
        return s->buffer[s->base % 16].data;
    }
    return -1;
}

int main(void) {
    TcpSender sender;
    TcpReceiver receiver;

    sender_init(&sender, 4);
    receiver_init(&receiver);

    printf("window=%d\n", sender.window_size);
    // EXPECT: window=4

    // Send 4 segments (fills window)
    int s0 = send_segment(&sender, 100);
    int s1 = send_segment(&sender, 200);
    int s2 = send_segment(&sender, 300);
    int s3 = send_segment(&sender, 400);
    printf("sent=%d %d %d %d\n", s0, s1, s2, s3);
    // EXPECT: sent=0 1 2 3

    printf("used=%d\n", window_used(&sender));
    // EXPECT: used=4

    // Window full - can't send
    printf("can_send=%d\n", can_send(&sender));
    // EXPECT: can_send=0

    // Receiver gets segments 0 and 1 in order
    int ack = receive_segment(&receiver, 0, 100);
    printf("ack=%d\n", ack);
    // EXPECT: ack=1

    ack = receive_segment(&receiver, 1, 200);
    printf("ack=%d\n", ack);
    // EXPECT: ack=2

    // Process ACK=2 (acks seqs 0,1)
    process_ack(&sender, 2);
    printf("base=%d acked=%d\n", sender.base, sender.total_acked);
    // EXPECT: base=2 acked=2

    printf("used=%d\n", window_used(&sender));
    // EXPECT: used=2

    printf("can_send=%d\n", can_send(&sender));
    // EXPECT: can_send=1

    // Send more segments
    int s4 = send_segment(&sender, 500);
    int s5 = send_segment(&sender, 600);
    printf("sent=%d %d\n", s4, s5);
    // EXPECT: sent=4 5

    printf("used=%d\n", window_used(&sender));
    // EXPECT: used=4

    // Simulate packet loss: segment 2 lost, receiver gets 3
    ack = receive_segment(&receiver, 3, 400);
    // Out of order, dup ACK
    printf("dup_ack=%d\n", ack);
    // EXPECT: dup_ack=2

    // Retransmit segment 2
    int rdata = retransmit(&sender);
    printf("retransmit_data=%d\n", rdata);
    // EXPECT: retransmit_data=300

    printf("retransmits=%d\n", sender.retransmits);
    // EXPECT: retransmits=1

    // Now receiver gets segment 2 (retransmitted)
    ack = receive_segment(&receiver, 2, 300);
    printf("ack=%d\n", ack);
    // EXPECT: ack=3

    // Process ACK=3
    process_ack(&sender, 3);
    printf("base=%d acked=%d\n", sender.base, sender.total_acked);
    // EXPECT: base=3 acked=3

    // Receiver gets remaining segments
    ack = receive_segment(&receiver, 3, 400);
    printf("ack=%d\n", ack);
    // EXPECT: ack=4

    ack = receive_segment(&receiver, 4, 500);
    printf("ack=%d\n", ack);
    // EXPECT: ack=5

    ack = receive_segment(&receiver, 5, 600);
    printf("ack=%d\n", ack);
    // EXPECT: ack=6

    process_ack(&sender, 6);
    printf("base=%d acked=%d\n", sender.base, sender.total_acked);
    // EXPECT: base=6 acked=6

    // Check received data
    printf("recv_count=%d\n", receiver.recv_count);
    // EXPECT: recv_count=6

    printf("data: %d %d %d %d %d %d\n",
           receiver.received[0], receiver.received[1],
           receiver.received[2], receiver.received[3],
           receiver.received[4], receiver.received[5]);
    // EXPECT: data: 100 200 300 400 500 600

    printf("total_sent=%d retransmits=%d\n", sender.total_sent, sender.retransmits);
    // EXPECT: total_sent=6 retransmits=1

    printf("tcp done\n");
    // EXPECT: tcp done

    return 0;
}
