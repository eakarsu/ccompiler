int printf(const char *fmt, ...);

// Test 085: Multi-channel message passing system

struct Packet {
    int channel;
    int seq;
    int data;
    int ack;
};

struct Channel {
    struct Packet buf[4];
    int head;
    int tail;
    int count;
    int next_seq;
    int last_ack;
};

void ch_init(struct Channel *ch) {
    int i;
    for (i = 0; i < 4; i++) {
        ch->buf[i].channel = 0;
        ch->buf[i].seq = 0;
        ch->buf[i].data = 0;
        ch->buf[i].ack = 0;
    }
    ch->head = 0;
    ch->tail = 0;
    ch->count = 0;
    ch->next_seq = 1;
    ch->last_ack = 0;
}

int ch_send(struct Channel *ch, int chan_id, int data) {
    if (ch->count >= 4) return 0;
    ch->buf[ch->tail].channel = chan_id;
    ch->buf[ch->tail].seq = ch->next_seq;
    ch->buf[ch->tail].data = data;
    ch->buf[ch->tail].ack = 0;
    ch->tail = (ch->tail + 1) % 4;
    ch->count = ch->count + 1;
    ch->next_seq = ch->next_seq + 1;
    return 1;
}

int ch_recv(struct Channel *ch, struct Packet *out) {
    if (ch->count <= 0) return 0;
    out->channel = ch->buf[ch->head].channel;
    out->seq = ch->buf[ch->head].seq;
    out->data = ch->buf[ch->head].data;
    out->ack = ch->buf[ch->head].ack;
    ch->head = (ch->head + 1) % 4;
    ch->count = ch->count - 1;
    ch->last_ack = out->seq;
    return 1;
}

int main() {
    struct Channel channels[3];
    struct Packet pkt;
    int i;
    int total;
    int ok;

    // Init all channels
    for (i = 0; i < 3; i++) {
        ch_init(&channels[i]);
    }

    // Send on channel 0
    ch_send(&channels[0], 0, 100);
    ch_send(&channels[0], 0, 200);
    ch_send(&channels[0], 0, 300);
    printf("Ch0 count: %d seq: %d\n", channels[0].count, channels[0].next_seq); // EXPECT: Ch0 count: 3 seq: 4

    // Send on channel 1
    ch_send(&channels[1], 1, 10);
    ch_send(&channels[1], 1, 20);
    printf("Ch1 count: %d seq: %d\n", channels[1].count, channels[1].next_seq); // EXPECT: Ch1 count: 2 seq: 3

    // Send on channel 2
    ch_send(&channels[2], 2, 1);
    ch_send(&channels[2], 2, 2);
    ch_send(&channels[2], 2, 3);
    ch_send(&channels[2], 2, 4);
    printf("Ch2 count: %d seq: %d\n", channels[2].count, channels[2].next_seq); // EXPECT: Ch2 count: 4 seq: 5

    // Ch2 is full
    ok = ch_send(&channels[2], 2, 5);
    printf("Ch2 overflow: %d\n", ok); // EXPECT: Ch2 overflow: 0

    // Receive from channel 0
    ch_recv(&channels[0], &pkt);
    printf("Recv ch%d seq=%d data=%d\n", pkt.channel, pkt.seq, pkt.data); // EXPECT: Recv ch0 seq=1 data=100
    ch_recv(&channels[0], &pkt);
    printf("Recv ch%d seq=%d data=%d\n", pkt.channel, pkt.seq, pkt.data); // EXPECT: Recv ch0 seq=2 data=200
    printf("Ch0 last_ack: %d\n", channels[0].last_ack); // EXPECT: Ch0 last_ack: 2

    // Receive all from channel 2
    total = 0;
    while (channels[2].count > 0) {
        ch_recv(&channels[2], &pkt);
        total = total + pkt.data;
    }
    printf("Ch2 total: %d\n", total); // EXPECT: Ch2 total: 10
    printf("Ch2 last_ack: %d\n", channels[2].last_ack); // EXPECT: Ch2 last_ack: 4

    // Send more on channel 0 (after draining some)
    ch_send(&channels[0], 0, 400);
    ch_send(&channels[0], 0, 500);
    ch_send(&channels[0], 0, 600);
    printf("Ch0 count: %d\n", channels[0].count); // EXPECT: Ch0 count: 4

    // Drain all channels and sum
    total = 0;
    for (i = 0; i < 3; i++) {
        while (channels[i].count > 0) {
            ch_recv(&channels[i], &pkt);
            total = total + pkt.data;
        }
    }
    printf("Grand total: %d\n", total); // EXPECT: Grand total: 1830

    // Round-robin simulation: send to each channel, then drain
    for (i = 0; i < 12; i++) {
        ch_send(&channels[i % 3], i % 3, (i + 1) * 5);
    }
    total = 0;
    for (i = 0; i < 3; i++) {
        while (channels[i].count > 0) {
            ch_recv(&channels[i], &pkt);
            total = total + pkt.data;
        }
    }
    printf("Round-robin total: %d\n", total); // EXPECT: Round-robin total: 390

    printf("PASS\n"); // EXPECT: PASS
    return 0;
}
