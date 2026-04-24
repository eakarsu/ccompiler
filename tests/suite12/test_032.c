int printf(const char *fmt, ...);

struct Packet {
    int seq_num;
    int data;
    int acked;
};

struct SlidingWindow {
    int window_size;
    int base;
    int next_seq;
    int total_packets;
    int sent_count;
    int ack_count;
};

int sw_init(struct SlidingWindow *sw, int wsize, int total) {
    sw->window_size = wsize;
    sw->base = 0;
    sw->next_seq = 0;
    sw->total_packets = total;
    sw->sent_count = 0;
    sw->ack_count = 0;
    return 0;
}

int can_send(struct SlidingWindow *sw) {
    if (sw->next_seq < sw->base + sw->window_size && sw->next_seq < sw->total_packets) {
        return 1;
    }
    return 0;
}

int send_packet(struct SlidingWindow *sw, struct Packet *pkts) {
    if (can_send(sw)) {
        int idx = sw->next_seq;
        pkts[idx].seq_num = idx;
        pkts[idx].data = (idx + 1) * 100;
        pkts[idx].acked = 0;
        sw->sent_count = sw->sent_count + 1;
        printf("Send pkt seq=%d data=%d\n", idx, pkts[idx].data);
        sw->next_seq = sw->next_seq + 1;
        return 1;
    }
    return 0;
}

int recv_ack(struct SlidingWindow *sw, struct Packet *pkts, int ack_seq) {
    if (ack_seq >= sw->base && ack_seq < sw->next_seq) {
        pkts[ack_seq].acked = 1;
        sw->ack_count = sw->ack_count + 1;
        printf("Recv ACK seq=%d\n", ack_seq);
        while (sw->base < sw->total_packets && pkts[sw->base].acked) {
            sw->base = sw->base + 1;
        }
        printf("Window base=%d next=%d\n", sw->base, sw->next_seq);
        return 1;
    }
    return 0;
}

int window_occupancy(struct SlidingWindow *sw) {
    return sw->next_seq - sw->base;
}

int main() {
    struct Packet packets[12];
    struct SlidingWindow sw;
    int i;

    for (i = 0; i < 12; i = i + 1) {
        packets[i].seq_num = 0;
        packets[i].data = 0;
        packets[i].acked = 0;
    }

    sw_init(&sw, 4, 10);
    printf("=== Sliding Window Protocol ===\n");
    // EXPECT: === Sliding Window Protocol ===
    printf("Window size: %d, Total packets: %d\n", sw.window_size, sw.total_packets);
    // EXPECT: Window size: 4, Total packets: 10

    printf("--- Send initial window ---\n");
    // EXPECT: --- Send initial window ---
    send_packet(&sw, packets);
    // EXPECT: Send pkt seq=0 data=100
    send_packet(&sw, packets);
    // EXPECT: Send pkt seq=1 data=200
    send_packet(&sw, packets);
    // EXPECT: Send pkt seq=2 data=300
    send_packet(&sw, packets);
    // EXPECT: Send pkt seq=3 data=400

    printf("Occupancy: %d\n", window_occupancy(&sw));
    // EXPECT: Occupancy: 4

    int ok = can_send(&sw);
    printf("Can send more: %d\n", ok);
    // EXPECT: Can send more: 0

    printf("--- Receive ACKs and slide ---\n");
    // EXPECT: --- Receive ACKs and slide ---
    recv_ack(&sw, packets, 0);
    // EXPECT: Recv ACK seq=0
    // EXPECT: Window base=1 next=4

    send_packet(&sw, packets);
    // EXPECT: Send pkt seq=4 data=500

    recv_ack(&sw, packets, 1);
    // EXPECT: Recv ACK seq=1
    // EXPECT: Window base=2 next=5

    recv_ack(&sw, packets, 2);
    // EXPECT: Recv ACK seq=2
    // EXPECT: Window base=3 next=5

    send_packet(&sw, packets);
    // EXPECT: Send pkt seq=5 data=600
    send_packet(&sw, packets);
    // EXPECT: Send pkt seq=6 data=700

    printf("--- Out of order ACK ---\n");
    // EXPECT: --- Out of order ACK ---
    recv_ack(&sw, packets, 4);
    // EXPECT: Recv ACK seq=4
    // EXPECT: Window base=3 next=7

    recv_ack(&sw, packets, 3);
    // EXPECT: Recv ACK seq=3
    // EXPECT: Window base=5 next=7

    printf("--- Continue sending ---\n");
    // EXPECT: --- Continue sending ---
    send_packet(&sw, packets);
    // EXPECT: Send pkt seq=7 data=800
    send_packet(&sw, packets);
    // EXPECT: Send pkt seq=8 data=900

    recv_ack(&sw, packets, 5);
    // EXPECT: Recv ACK seq=5
    // EXPECT: Window base=6 next=9

    send_packet(&sw, packets);
    // EXPECT: Send pkt seq=9 data=1000

    recv_ack(&sw, packets, 6);
    // EXPECT: Recv ACK seq=6
    // EXPECT: Window base=7 next=10
    recv_ack(&sw, packets, 7);
    // EXPECT: Recv ACK seq=7
    // EXPECT: Window base=8 next=10
    recv_ack(&sw, packets, 8);
    // EXPECT: Recv ACK seq=8
    // EXPECT: Window base=9 next=10
    recv_ack(&sw, packets, 9);
    // EXPECT: Recv ACK seq=9
    // EXPECT: Window base=10 next=10

    printf("Sent: %d, Acked: %d\n", sw.sent_count, sw.ack_count);
    // EXPECT: Sent: 10, Acked: 10
    printf("All done: %d\n", sw.base == sw.total_packets);
    // EXPECT: All done: 1

    return 0;
}
