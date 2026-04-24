int printf(const char *fmt, ...);

// Go-style Channel simulation (single-threaded)
// Simulates buffered and unbuffered channels with send/receive

struct Channel {
    int buffer[8];
    int capacity;
    int head;
    int tail;
    int count;
    int closed;
    int total_sends;
    int total_recvs;
};

void chan_init(struct Channel *ch, int cap) {
    int i;
    ch->capacity = cap;
    ch->head = 0;
    ch->tail = 0;
    ch->count = 0;
    ch->closed = 0;
    ch->total_sends = 0;
    ch->total_recvs = 0;
    for (i = 0; i < 8; i = i + 1) {
        ch->buffer[i] = 0;
    }
}

int chan_send(struct Channel *ch, int val) {
    if (ch->closed) return -1;
    if (ch->count >= ch->capacity) return 0;
    ch->buffer[ch->tail] = val;
    ch->tail = (ch->tail + 1) % ch->capacity;
    ch->count = ch->count + 1;
    ch->total_sends = ch->total_sends + 1;
    return 1;
}

int chan_recv(struct Channel *ch) {
    int val;
    if (ch->count <= 0) {
        if (ch->closed) return -1;
        return -999;
    }
    val = ch->buffer[ch->head];
    ch->head = (ch->head + 1) % ch->capacity;
    ch->count = ch->count - 1;
    ch->total_recvs = ch->total_recvs + 1;
    return val;
}

void chan_close(struct Channel *ch) {
    ch->closed = 1;
}

int chan_len(struct Channel *ch) {
    return ch->count;
}

int chan_is_closed(struct Channel *ch) {
    return ch->closed;
}

// Select simulation: try multiple channels, return which one succeeded
// Returns channel index (0-3) or -1 if none ready
int chan_select_recv(struct Channel *chs, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        if (chs[i].count > 0) return i;
    }
    return -1;
}

// Pipeline: chain of channels
struct Pipeline {
    struct Channel stages[4];
    int num_stages;
    int items_processed;
};

void pipeline_init(struct Pipeline *p, int stages, int buf) {
    int i;
    p->num_stages = stages;
    p->items_processed = 0;
    for (i = 0; i < stages; i = i + 1) {
        chan_init(&p->stages[i], buf);
    }
}

int pipeline_push(struct Pipeline *p, int val) {
    return chan_send(&p->stages[0], val);
}

int pipeline_process_stage(struct Pipeline *p, int stage) {
    int val;
    int next;
    if (stage >= p->num_stages) return -1;
    val = chan_recv(&p->stages[stage]);
    if (val == -999 || val == -1) return -1;
    // Transform: multiply by 2 at each stage
    val = val * 2;
    next = stage + 1;
    if (next < p->num_stages) {
        chan_send(&p->stages[next], val);
    } else {
        p->items_processed = p->items_processed + 1;
    }
    return val;
}

// Fan-out: one sender to multiple receivers
int fanout_send(struct Channel *chs, int n, int val) {
    int sent;
    int i;
    sent = 0;
    for (i = 0; i < n; i = i + 1) {
        if (chan_send(&chs[i], val)) {
            sent = sent + 1;
        }
    }
    return sent;
}

int main() {
    struct Channel ch;
    chan_init(&ch, 4);

    printf("len: %d\n", chan_len(&ch)); // EXPECT: len: 0

    // Basic send/recv
    printf("send 10: %d\n", chan_send(&ch, 10)); // EXPECT: send 10: 1
    printf("send 20: %d\n", chan_send(&ch, 20)); // EXPECT: send 20: 1
    printf("send 30: %d\n", chan_send(&ch, 30)); // EXPECT: send 30: 1
    printf("len: %d\n", chan_len(&ch)); // EXPECT: len: 3

    printf("recv: %d\n", chan_recv(&ch)); // EXPECT: recv: 10
    printf("recv: %d\n", chan_recv(&ch)); // EXPECT: recv: 20
    printf("len: %d\n", chan_len(&ch)); // EXPECT: len: 1

    // Fill to capacity
    chan_send(&ch, 40);
    chan_send(&ch, 50);
    chan_send(&ch, 60);
    printf("len: %d\n", chan_len(&ch)); // EXPECT: len: 4
    printf("full send: %d\n", chan_send(&ch, 70)); // EXPECT: full send: 0

    // Close channel
    chan_close(&ch);
    printf("closed: %d\n", chan_is_closed(&ch)); // EXPECT: closed: 1
    printf("send closed: %d\n", chan_send(&ch, 80)); // EXPECT: send closed: -1

    // Can still receive from closed channel
    printf("recv closed: %d\n", chan_recv(&ch)); // EXPECT: recv closed: 30
    chan_recv(&ch);
    chan_recv(&ch);
    chan_recv(&ch);
    // Empty and closed
    printf("recv empty closed: %d\n", chan_recv(&ch)); // EXPECT: recv empty closed: -1

    printf("sends: %d\n", ch.total_sends); // EXPECT: sends: 6
    printf("recvs: %d\n", ch.total_recvs); // EXPECT: recvs: 6

    // Pipeline test: 3 stages, each multiplies by 2
    struct Pipeline pipe;
    pipeline_init(&pipe, 3, 4);

    pipeline_push(&pipe, 5);
    pipeline_push(&pipe, 10);

    // Process stage 0 -> stage 1: val * 2
    int v1 = pipeline_process_stage(&pipe, 0);
    printf("stage0: %d\n", v1); // EXPECT: stage0: 10

    // Process stage 1 -> stage 2: val * 2
    int v2 = pipeline_process_stage(&pipe, 1);
    printf("stage1: %d\n", v2); // EXPECT: stage1: 20

    // Process stage 2 -> output: val * 2
    int v3 = pipeline_process_stage(&pipe, 2);
    printf("stage2: %d\n", v3); // EXPECT: stage2: 40
    printf("processed: %d\n", pipe.items_processed); // EXPECT: processed: 1

    // Process second item through pipeline
    pipeline_process_stage(&pipe, 0); // 10 -> 20
    pipeline_process_stage(&pipe, 1); // 20 -> 40
    int v4 = pipeline_process_stage(&pipe, 2); // 40 -> 80
    printf("pipe2 out: %d\n", v4); // EXPECT: pipe2 out: 80
    printf("processed: %d\n", pipe.items_processed); // EXPECT: processed: 2

    // Fan-out test
    struct Channel fans[3];
    int fi;
    for (fi = 0; fi < 3; fi = fi + 1) {
        chan_init(&fans[fi], 4);
    }
    int sent = fanout_send(fans, 3, 42);
    printf("fanout sent: %d\n", sent); // EXPECT: fanout sent: 3
    printf("fan0: %d\n", chan_recv(&fans[0])); // EXPECT: fan0: 42
    printf("fan1: %d\n", chan_recv(&fans[1])); // EXPECT: fan1: 42
    printf("fan2: %d\n", chan_recv(&fans[2])); // EXPECT: fan2: 42

    return 0;
}
