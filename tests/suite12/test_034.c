int printf(const char *fmt, ...);

struct CongestionCtrl {
    int cwnd;
    int ssthresh;
    int mss;
    int rtt_count;
    int dup_ack_count;
    int in_slow_start;
    int total_sent;
    int total_lost;
};

int cc_init(struct CongestionCtrl *cc, int mss, int ssthresh) {
    cc->cwnd = mss;
    cc->ssthresh = ssthresh;
    cc->mss = mss;
    cc->rtt_count = 0;
    cc->dup_ack_count = 0;
    cc->in_slow_start = 1;
    cc->total_sent = 0;
    cc->total_lost = 0;
    return 0;
}

int cc_on_ack(struct CongestionCtrl *cc) {
    cc->dup_ack_count = 0;
    if (cc->in_slow_start) {
        cc->cwnd = cc->cwnd + cc->mss;
        if (cc->cwnd >= cc->ssthresh) {
            cc->in_slow_start = 0;
            printf("Exit slow start: cwnd=%d ssthresh=%d\n", cc->cwnd, cc->ssthresh);
        }
    } else {
        int increment = (cc->mss * cc->mss) / cc->cwnd;
        if (increment < 1) increment = 1;
        cc->cwnd = cc->cwnd + increment;
    }
    cc->total_sent = cc->total_sent + cc->mss;
    return cc->cwnd;
}

int cc_on_loss(struct CongestionCtrl *cc) {
    cc->ssthresh = cc->cwnd / 2;
    if (cc->ssthresh < cc->mss * 2) {
        cc->ssthresh = cc->mss * 2;
    }
    cc->cwnd = cc->mss;
    cc->in_slow_start = 1;
    cc->dup_ack_count = 0;
    cc->total_lost = cc->total_lost + 1;
    printf("Loss detected: cwnd=%d ssthresh=%d\n", cc->cwnd, cc->ssthresh);
    return cc->cwnd;
}

int cc_on_dup_ack(struct CongestionCtrl *cc) {
    cc->dup_ack_count = cc->dup_ack_count + 1;
    if (cc->dup_ack_count >= 3) {
        printf("Triple dup ACK (count=%d)\n", cc->dup_ack_count);
        cc->ssthresh = cc->cwnd / 2;
        if (cc->ssthresh < cc->mss * 2) {
            cc->ssthresh = cc->mss * 2;
        }
        cc->cwnd = cc->ssthresh + 3 * cc->mss;
        cc->in_slow_start = 0;
        printf("Fast recovery: cwnd=%d ssthresh=%d\n", cc->cwnd, cc->ssthresh);
        cc->dup_ack_count = 0;
        return 1;
    }
    return 0;
}

int cc_rtt_tick(struct CongestionCtrl *cc) {
    cc->rtt_count = cc->rtt_count + 1;
    return cc->rtt_count;
}

int main() {
    struct CongestionCtrl cc;
    cc_init(&cc, 100, 800);

    printf("=== AIMD Congestion Control ===\n");
    // EXPECT: === AIMD Congestion Control ===
    printf("Initial: cwnd=%d ssthresh=%d mss=%d\n", cc.cwnd, cc.ssthresh, cc.mss);
    // EXPECT: Initial: cwnd=100 ssthresh=800 mss=100

    printf("--- Slow Start Phase ---\n");
    // EXPECT: --- Slow Start Phase ---
    int i;
    for (i = 0; i < 6; i = i + 1) {
        cc_on_ack(&cc);
    }
    printf("After 6 ACKs: cwnd=%d\n", cc.cwnd);
    // EXPECT: After 6 ACKs: cwnd=700

    cc_on_ack(&cc);
    // EXPECT: Exit slow start: cwnd=800 ssthresh=800
    printf("cwnd=%d slow_start=%d\n", cc.cwnd, cc.in_slow_start);
    // EXPECT: cwnd=800 slow_start=0

    printf("--- Congestion Avoidance (AIMD) ---\n");
    // EXPECT: --- Congestion Avoidance (AIMD) ---
    for (i = 0; i < 8; i = i + 1) {
        cc_on_ack(&cc);
        cc_rtt_tick(&cc);
    }
    printf("After 8 CA ACKs: cwnd=%d\n", cc.cwnd);
    // EXPECT: After 8 CA ACKs: cwnd=891

    printf("--- Timeout Loss ---\n");
    // EXPECT: --- Timeout Loss ---
    cc_on_loss(&cc);
    // EXPECT: Loss detected: cwnd=100 ssthresh=445
    printf("Back to slow start: %d\n", cc.in_slow_start);
    // EXPECT: Back to slow start: 1

    printf("--- Recover from loss ---\n");
    // EXPECT: --- Recover from loss ---
    for (i = 0; i < 3; i = i + 1) {
        cc_on_ack(&cc);
    }
    printf("Growing: cwnd=%d\n", cc.cwnd);
    // EXPECT: Growing: cwnd=400

    cc_on_ack(&cc);
    // EXPECT: Exit slow start: cwnd=500 ssthresh=445

    printf("--- Triple Dup ACK ---\n");
    // EXPECT: --- Triple Dup ACK ---
    cc.cwnd = 1000;
    cc.in_slow_start = 0;
    cc_on_dup_ack(&cc);
    printf("Dup count: %d\n", cc.dup_ack_count);
    // EXPECT: Dup count: 1
    cc_on_dup_ack(&cc);
    printf("Dup count: %d\n", cc.dup_ack_count);
    // EXPECT: Dup count: 2
    cc_on_dup_ack(&cc);
    // EXPECT: Triple dup ACK (count=3)
    // EXPECT: Fast recovery: cwnd=800 ssthresh=500

    printf("After fast recovery: cwnd=%d\n", cc.cwnd);
    // EXPECT: After fast recovery: cwnd=800

    printf("--- Statistics ---\n");
    // EXPECT: --- Statistics ---
    printf("Total sent: %d\n", cc.total_sent);
    // EXPECT: Total sent: 1900
    printf("Total losses: %d\n", cc.total_lost);
    // EXPECT: Total losses: 1
    printf("RTT count: %d\n", cc.rtt_count);
    // EXPECT: RTT count: 8

    printf("--- Min ssthresh test ---\n");
    // EXPECT: --- Min ssthresh test ---
    struct CongestionCtrl cc2;
    cc_init(&cc2, 100, 400);
    cc2.cwnd = 150;
    cc_on_loss(&cc2);
    // EXPECT: Loss detected: cwnd=100 ssthresh=200
    printf("ssthresh floor works: %d\n", cc2.ssthresh);
    // EXPECT: ssthresh floor works: 200

    return 0;
}
