int printf(const char *fmt, ...);

// Message queue simulation (single-threaded)
// Simulates inter-thread message passing with priorities

struct Message {
    int sender;
    int receiver;
    int type;
    int payload;
    int priority;
    int delivered;
};

struct MessageQueue {
    struct Message msgs[16];
    int count;
    int total_sent;
    int total_delivered;
};

void mq_init(struct MessageQueue *mq) {
    int i;
    mq->count = 0;
    mq->total_sent = 0;
    mq->total_delivered = 0;
    for (i = 0; i < 16; i = i + 1) {
        mq->msgs[i].sender = -1;
        mq->msgs[i].receiver = -1;
        mq->msgs[i].type = 0;
        mq->msgs[i].payload = 0;
        mq->msgs[i].priority = 0;
        mq->msgs[i].delivered = 0;
    }
}

int mq_send(struct MessageQueue *mq, int from, int to, int type, int payload, int prio) {
    int idx;
    int i;
    int j;
    if (mq->count >= 16) return 0;

    // Insert sorted by priority (higher priority first)
    idx = mq->count;
    for (i = 0; i < mq->count; i = i + 1) {
        if (prio > mq->msgs[i].priority) {
            idx = i;
            break;
        }
    }
    // Shift elements right
    for (j = mq->count; j > idx; j = j - 1) {
        mq->msgs[j] = mq->msgs[j - 1];
    }
    mq->msgs[idx].sender = from;
    mq->msgs[idx].receiver = to;
    mq->msgs[idx].type = type;
    mq->msgs[idx].payload = payload;
    mq->msgs[idx].priority = prio;
    mq->msgs[idx].delivered = 0;
    mq->count = mq->count + 1;
    mq->total_sent = mq->total_sent + 1;
    return 1;
}

int mq_receive(struct MessageQueue *mq, int receiver) {
    int i;
    int j;
    int payload;
    for (i = 0; i < mq->count; i = i + 1) {
        if (mq->msgs[i].receiver == receiver && !mq->msgs[i].delivered) {
            payload = mq->msgs[i].payload;
            mq->msgs[i].delivered = 1;
            // Remove from queue
            for (j = i; j < mq->count - 1; j = j + 1) {
                mq->msgs[j] = mq->msgs[j + 1];
            }
            mq->count = mq->count - 1;
            mq->total_delivered = mq->total_delivered + 1;
            return payload;
        }
    }
    return -999;
}

int mq_peek_priority(struct MessageQueue *mq) {
    if (mq->count == 0) return -1;
    return mq->msgs[0].priority;
}

int mq_count_for(struct MessageQueue *mq, int receiver) {
    int cnt;
    int i;
    cnt = 0;
    for (i = 0; i < mq->count; i = i + 1) {
        if (mq->msgs[i].receiver == receiver) {
            cnt = cnt + 1;
        }
    }
    return cnt;
}

// Mailbox: per-thread message buffer
struct Mailbox {
    int inbox[8];
    int inbox_count;
    int owner;
};

void mailbox_init(struct Mailbox *mb, int owner) {
    int i;
    mb->owner = owner;
    mb->inbox_count = 0;
    for (i = 0; i < 8; i = i + 1) {
        mb->inbox[i] = 0;
    }
}

int mailbox_deliver(struct Mailbox *mb, int val) {
    if (mb->inbox_count >= 8) return 0;
    mb->inbox[mb->inbox_count] = val;
    mb->inbox_count = mb->inbox_count + 1;
    return 1;
}

int mailbox_read(struct Mailbox *mb) {
    int val;
    int i;
    if (mb->inbox_count <= 0) return -999;
    val = mb->inbox[0];
    for (i = 0; i < mb->inbox_count - 1; i = i + 1) {
        mb->inbox[i] = mb->inbox[i + 1];
    }
    mb->inbox_count = mb->inbox_count - 1;
    return val;
}

int mailbox_sum(struct Mailbox *mb) {
    int s;
    int i;
    s = 0;
    for (i = 0; i < mb->inbox_count; i = i + 1) {
        s = s + mb->inbox[i];
    }
    return s;
}

int main() {
    struct MessageQueue mq;
    mq_init(&mq);

    // Send messages with different priorities
    mq_send(&mq, 0, 1, 1, 100, 5);
    mq_send(&mq, 0, 1, 1, 200, 10);
    mq_send(&mq, 0, 1, 1, 300, 1);
    mq_send(&mq, 2, 1, 2, 400, 7);
    printf("count: %d\n", mq.count); // EXPECT: count: 4
    printf("top prio: %d\n", mq_peek_priority(&mq)); // EXPECT: top prio: 10

    // Receive highest priority first
    int v = mq_receive(&mq, 1);
    printf("recv1: %d\n", v); // EXPECT: recv1: 200

    v = mq_receive(&mq, 1);
    printf("recv2: %d\n", v); // EXPECT: recv2: 400

    v = mq_receive(&mq, 1);
    printf("recv3: %d\n", v); // EXPECT: recv3: 100

    v = mq_receive(&mq, 1);
    printf("recv4: %d\n", v); // EXPECT: recv4: 300

    printf("count: %d\n", mq.count); // EXPECT: count: 0
    printf("delivered: %d\n", mq.total_delivered); // EXPECT: delivered: 4

    // No message for receiver 2
    v = mq_receive(&mq, 2);
    printf("no msg: %d\n", v); // EXPECT: no msg: -999

    // Multi-receiver test
    mq_send(&mq, 0, 1, 0, 10, 1);
    mq_send(&mq, 0, 2, 0, 20, 1);
    mq_send(&mq, 0, 1, 0, 30, 2);
    mq_send(&mq, 0, 3, 0, 40, 1);
    printf("for r1: %d\n", mq_count_for(&mq, 1)); // EXPECT: for r1: 2
    printf("for r2: %d\n", mq_count_for(&mq, 2)); // EXPECT: for r2: 1
    printf("for r3: %d\n", mq_count_for(&mq, 3)); // EXPECT: for r3: 1

    int r1msg = mq_receive(&mq, 1);
    printf("r1 got: %d\n", r1msg); // EXPECT: r1 got: 30
    int r2msg = mq_receive(&mq, 2);
    printf("r2 got: %d\n", r2msg); // EXPECT: r2 got: 20

    // Mailbox test
    struct Mailbox mb;
    mailbox_init(&mb, 1);

    mailbox_deliver(&mb, 10);
    mailbox_deliver(&mb, 20);
    mailbox_deliver(&mb, 30);
    printf("mb count: %d\n", mb.inbox_count); // EXPECT: mb count: 3
    printf("mb sum: %d\n", mailbox_sum(&mb)); // EXPECT: mb sum: 60

    int r = mailbox_read(&mb);
    printf("mb read: %d\n", r); // EXPECT: mb read: 10
    r = mailbox_read(&mb);
    printf("mb read: %d\n", r); // EXPECT: mb read: 20
    printf("mb count: %d\n", mb.inbox_count); // EXPECT: mb count: 1

    printf("total sent: %d\n", mq.total_sent); // EXPECT: total sent: 8

    return 0;
}
