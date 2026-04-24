int printf(const char *fmt, ...);

struct StopWaitPacket {
    int seq;
    int data;
    int is_ack;
    int ack_seq;
    int corrupted;
};

struct StopWaitSender {
    int current_seq;
    int waiting_for_ack;
    int timeout_count;
    int total_sent;
    int total_retransmits;
    int last_data;
};

struct StopWaitReceiver {
    int expected_seq;
    int total_received;
    int total_duplicates;
    int received_data[20];
    int recv_count;
};

int sw_sender_init(struct StopWaitSender *s) {
    s->current_seq = 0;
    s->waiting_for_ack = 0;
    s->timeout_count = 0;
    s->total_sent = 0;
    s->total_retransmits = 0;
    s->last_data = 0;
    return 0;
}

int sw_receiver_init(struct StopWaitReceiver *r) {
    r->expected_seq = 0;
    r->total_received = 0;
    r->total_duplicates = 0;
    r->recv_count = 0;
    int i;
    for (i = 0; i < 20; i = i + 1) {
        r->received_data[i] = 0;
    }
    return 0;
}

int sw_make_packet(struct StopWaitPacket *p, int seq, int data) {
    p->seq = seq;
    p->data = data;
    p->is_ack = 0;
    p->ack_seq = -1;
    p->corrupted = 0;
    return 0;
}

int sw_send(struct StopWaitSender *s, struct StopWaitPacket *p, int data) {
    sw_make_packet(p, s->current_seq, data);
    s->last_data = data;
    s->waiting_for_ack = 1;
    s->total_sent = s->total_sent + 1;
    printf("SEND: seq=%d data=%d\n", p->seq, p->data);
    return 0;
}

int sw_retransmit(struct StopWaitSender *s, struct StopWaitPacket *p) {
    sw_make_packet(p, s->current_seq, s->last_data);
    s->total_retransmits = s->total_retransmits + 1;
    s->total_sent = s->total_sent + 1;
    printf("RETRANSMIT: seq=%d data=%d\n", p->seq, p->data);
    return 0;
}

int sw_receive(struct StopWaitReceiver *r, struct StopWaitPacket *pkt, struct StopWaitPacket *ack_out) {
    if (pkt->corrupted) {
        printf("RECV: corrupted packet, send NAK\n");
        ack_out->is_ack = 1;
        ack_out->ack_seq = -1;
        ack_out->corrupted = 0;
        return -1;
    }
    if (pkt->seq == r->expected_seq) {
        printf("RECV: seq=%d data=%d (accepted)\n", pkt->seq, pkt->data);
        if (r->recv_count < 20) {
            r->received_data[r->recv_count] = pkt->data;
            r->recv_count = r->recv_count + 1;
        }
        r->total_received = r->total_received + 1;
        ack_out->is_ack = 1;
        ack_out->ack_seq = r->expected_seq;
        ack_out->corrupted = 0;
        r->expected_seq = 1 - r->expected_seq;
        return 1;
    } else {
        printf("RECV: seq=%d (duplicate, expected %d)\n", pkt->seq, r->expected_seq);
        r->total_duplicates = r->total_duplicates + 1;
        ack_out->is_ack = 1;
        ack_out->ack_seq = 1 - r->expected_seq;
        ack_out->corrupted = 0;
        return 0;
    }
}

int sw_process_ack(struct StopWaitSender *s, struct StopWaitPacket *ack) {
    if (ack->corrupted) {
        printf("ACK corrupted, will retransmit\n");
        s->timeout_count = s->timeout_count + 1;
        return -1;
    }
    if (ack->ack_seq == s->current_seq) {
        printf("ACK: seq=%d (accepted)\n", ack->ack_seq);
        s->current_seq = 1 - s->current_seq;
        s->waiting_for_ack = 0;
        return 1;
    }
    printf("ACK: seq=%d (wrong, expected %d)\n", ack->ack_seq, s->current_seq);
    return 0;
}

int main() {
    struct StopWaitSender sender;
    struct StopWaitReceiver receiver;
    struct StopWaitPacket pkt;
    struct StopWaitPacket ack;

    sw_sender_init(&sender);
    sw_receiver_init(&receiver);

    printf("=== Stop-and-Wait Reliable Transfer ===\n");
    // EXPECT: === Stop-and-Wait Reliable Transfer ===

    printf("--- Normal transfer ---\n");
    // EXPECT: --- Normal transfer ---
    sw_send(&sender, &pkt, 42);
    // EXPECT: SEND: seq=0 data=42
    sw_receive(&receiver, &pkt, &ack);
    // EXPECT: RECV: seq=0 data=42 (accepted)
    sw_process_ack(&sender, &ack);
    // EXPECT: ACK: seq=0 (accepted)

    sw_send(&sender, &pkt, 99);
    // EXPECT: SEND: seq=1 data=99
    sw_receive(&receiver, &pkt, &ack);
    // EXPECT: RECV: seq=1 data=99 (accepted)
    sw_process_ack(&sender, &ack);
    // EXPECT: ACK: seq=1 (accepted)

    printf("--- Packet loss (timeout + retransmit) ---\n");
    // EXPECT: --- Packet loss (timeout + retransmit) ---
    sw_send(&sender, &pkt, 77);
    // EXPECT: SEND: seq=0 data=77
    printf("Packet lost in transit!\n");
    // EXPECT: Packet lost in transit!
    sw_retransmit(&sender, &pkt);
    // EXPECT: RETRANSMIT: seq=0 data=77
    sw_receive(&receiver, &pkt, &ack);
    // EXPECT: RECV: seq=0 data=77 (accepted)
    sw_process_ack(&sender, &ack);
    // EXPECT: ACK: seq=0 (accepted)

    printf("--- Corrupted packet ---\n");
    // EXPECT: --- Corrupted packet ---
    sw_send(&sender, &pkt, 55);
    // EXPECT: SEND: seq=1 data=55
    pkt.corrupted = 1;
    sw_receive(&receiver, &pkt, &ack);
    // EXPECT: RECV: corrupted packet, send NAK
    sw_retransmit(&sender, &pkt);
    // EXPECT: RETRANSMIT: seq=1 data=55
    pkt.corrupted = 0;
    sw_receive(&receiver, &pkt, &ack);
    // EXPECT: RECV: seq=1 data=55 (accepted)
    sw_process_ack(&sender, &ack);
    // EXPECT: ACK: seq=1 (accepted)

    printf("--- Corrupted ACK ---\n");
    // EXPECT: --- Corrupted ACK ---
    sw_send(&sender, &pkt, 33);
    // EXPECT: SEND: seq=0 data=33
    sw_receive(&receiver, &pkt, &ack);
    // EXPECT: RECV: seq=0 data=33 (accepted)
    ack.corrupted = 1;
    sw_process_ack(&sender, &ack);
    // EXPECT: ACK corrupted, will retransmit
    sw_retransmit(&sender, &pkt);
    // EXPECT: RETRANSMIT: seq=0 data=33
    sw_receive(&receiver, &pkt, &ack);
    // EXPECT: RECV: seq=0 (duplicate, expected 1)
    sw_process_ack(&sender, &ack);
    // EXPECT: ACK: seq=0 (accepted)

    printf("--- Statistics ---\n");
    // EXPECT: --- Statistics ---
    printf("Sender: sent=%d retransmits=%d\n", sender.total_sent, sender.total_retransmits);
    // EXPECT: Sender: sent=8 retransmits=3
    printf("Receiver: received=%d duplicates=%d\n", receiver.total_received, receiver.total_duplicates);
    // EXPECT: Receiver: received=5 duplicates=1
    printf("Data: %d %d %d %d %d\n", receiver.received_data[0], receiver.received_data[1], receiver.received_data[2], receiver.received_data[3], receiver.received_data[4]);
    // EXPECT: Data: 42 99 77 55 33

    return 0;
}
