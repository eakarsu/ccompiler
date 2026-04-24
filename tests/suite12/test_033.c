int printf(const char *fmt, ...);

struct FlowController {
    int send_window;
    int recv_window;
    int recv_buffer_size;
    int recv_buffer_used;
    int bytes_sent;
    int bytes_acked;
    int total_to_send;
};

int fc_init(struct FlowController *fc, int buf_size, int total) {
    fc->send_window = buf_size;
    fc->recv_window = buf_size;
    fc->recv_buffer_size = buf_size;
    fc->recv_buffer_used = 0;
    fc->bytes_sent = 0;
    fc->bytes_acked = 0;
    fc->total_to_send = total;
    return 0;
}

int fc_send(struct FlowController *fc, int bytes) {
    int actual;
    if (bytes > fc->recv_window) {
        actual = fc->recv_window;
    } else {
        actual = bytes;
    }
    if (actual <= 0) {
        printf("Flow blocked: recv_window=0\n");
        return 0;
    }
    fc->bytes_sent = fc->bytes_sent + actual;
    fc->recv_buffer_used = fc->recv_buffer_used + actual;
    fc->recv_window = fc->recv_buffer_size - fc->recv_buffer_used;
    printf("Sent %d bytes (total_sent=%d, recv_win=%d)\n", actual, fc->bytes_sent, fc->recv_window);
    return actual;
}

int fc_app_read(struct FlowController *fc, int bytes) {
    int actual;
    if (bytes > fc->recv_buffer_used) {
        actual = fc->recv_buffer_used;
    } else {
        actual = bytes;
    }
    fc->recv_buffer_used = fc->recv_buffer_used - actual;
    fc->recv_window = fc->recv_buffer_size - fc->recv_buffer_used;
    fc->bytes_acked = fc->bytes_acked + actual;
    printf("App read %d bytes (buf_used=%d, recv_win=%d)\n", actual, fc->recv_buffer_used, fc->recv_window);
    return actual;
}

int fc_window_update(struct FlowController *fc) {
    fc->send_window = fc->recv_window;
    printf("Window update: send_win=%d\n", fc->send_window);
    return fc->send_window;
}

int fc_is_complete(struct FlowController *fc) {
    return fc->bytes_acked >= fc->total_to_send;
}

int min_val(int a, int b) {
    if (a < b) return a;
    return b;
}

int main() {
    struct FlowController fc;
    fc_init(&fc, 1000, 3000);

    printf("=== Flow Control Simulation ===\n");
    // EXPECT: === Flow Control Simulation ===
    printf("Buffer: %d, Total: %d\n", fc.recv_buffer_size, fc.total_to_send);
    // EXPECT: Buffer: 1000, Total: 3000

    printf("--- Phase 1: Fill buffer ---\n");
    // EXPECT: --- Phase 1: Fill buffer ---
    fc_send(&fc, 400);
    // EXPECT: Sent 400 bytes (total_sent=400, recv_win=600)
    fc_send(&fc, 400);
    // EXPECT: Sent 400 bytes (total_sent=800, recv_win=200)
    fc_send(&fc, 400);
    // EXPECT: Sent 200 bytes (total_sent=1000, recv_win=0)

    printf("--- Phase 2: Window at zero ---\n");
    // EXPECT: --- Phase 2: Window at zero ---
    fc_send(&fc, 100);
    // EXPECT: Flow blocked: recv_window=0

    printf("--- Phase 3: App reads data ---\n");
    // EXPECT: --- Phase 3: App reads data ---
    fc_app_read(&fc, 500);
    // EXPECT: App read 500 bytes (buf_used=500, recv_win=500)
    fc_window_update(&fc);
    // EXPECT: Window update: send_win=500

    printf("--- Phase 4: Send more ---\n");
    // EXPECT: --- Phase 4: Send more ---
    fc_send(&fc, 500);
    // EXPECT: Sent 500 bytes (total_sent=1500, recv_win=0)

    fc_app_read(&fc, 1000);
    // EXPECT: App read 1000 bytes (buf_used=0, recv_win=1000)
    fc_window_update(&fc);
    // EXPECT: Window update: send_win=1000

    printf("--- Phase 5: Burst send ---\n");
    // EXPECT: --- Phase 5: Burst send ---
    fc_send(&fc, 300);
    // EXPECT: Sent 300 bytes (total_sent=1800, recv_win=700)
    fc_send(&fc, 300);
    // EXPECT: Sent 300 bytes (total_sent=2100, recv_win=400)
    fc_send(&fc, 300);
    // EXPECT: Sent 300 bytes (total_sent=2400, recv_win=100)

    fc_app_read(&fc, 900);
    // EXPECT: App read 900 bytes (buf_used=0, recv_win=1000)
    fc_window_update(&fc);
    // EXPECT: Window update: send_win=1000

    fc_send(&fc, 600);
    // EXPECT: Sent 600 bytes (total_sent=3000, recv_win=400)

    fc_app_read(&fc, 600);
    // EXPECT: App read 600 bytes (buf_used=0, recv_win=1000)

    printf("Complete: %d\n", fc_is_complete(&fc));
    // EXPECT: Complete: 1
    printf("Sent: %d, Acked: %d\n", fc.bytes_sent, fc.bytes_acked);
    // EXPECT: Sent: 3000, Acked: 3000

    printf("--- Slow reader scenario ---\n");
    // EXPECT: --- Slow reader scenario ---
    struct FlowController fc2;
    fc_init(&fc2, 200, 500);
    fc_send(&fc2, 100);
    // EXPECT: Sent 100 bytes (total_sent=100, recv_win=100)
    fc_send(&fc2, 100);
    // EXPECT: Sent 100 bytes (total_sent=200, recv_win=0)
    fc_send(&fc2, 100);
    // EXPECT: Flow blocked: recv_window=0
    fc_app_read(&fc2, 50);
    // EXPECT: App read 50 bytes (buf_used=150, recv_win=50)
    fc_send(&fc2, 100);
    // EXPECT: Sent 50 bytes (total_sent=250, recv_win=0)

    printf("Final buf_used: %d\n", fc2.recv_buffer_used);
    // EXPECT: Final buf_used: 200

    return 0;
}
