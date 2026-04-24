int printf(const char *fmt, ...);
// EXPECT: [0] CONNECT client=42 ver=3\n[1] DATA seq=1 payload=12345 len=64\n[2] ACK seq=1 status=200\n[3] DATA seq=2 payload=67890 len=128\n[4] ACK seq=2 status=200\n[5] ERROR code=500\ndata messages: 2\ntotal payload: 80235
// Test: union-based protocol messages

enum MsgType {
    MSG_CONNECT,
    MSG_DATA,
    MSG_ACK,
    MSG_DISCONNECT,
    MSG_ERROR
};

struct ConnectMsg {
    int client_id;
    int version;
};

struct DataMsg {
    int seq_num;
    int payload;
    int length;
};

struct AckMsg {
    int seq_num;
    int status;
};

union MsgBody {
    struct ConnectMsg connect;
    struct DataMsg data;
    struct AckMsg ack;
    int error_code;
};

struct Message {
    enum MsgType type;
    union MsgBody body;
};

struct Message make_connect(int id, int ver) {
    struct Message m;
    m.type = MSG_CONNECT;
    m.body.connect.client_id = id;
    m.body.connect.version = ver;
    return m;
}

struct Message make_data(int seq, int payload, int len) {
    struct Message m;
    m.type = MSG_DATA;
    m.body.data.seq_num = seq;
    m.body.data.payload = payload;
    m.body.data.length = len;
    return m;
}

struct Message make_ack(int seq, int status) {
    struct Message m;
    m.type = MSG_ACK;
    m.body.ack.seq_num = seq;
    m.body.ack.status = status;
    return m;
}

struct Message make_error(int code) {
    struct Message m;
    m.type = MSG_ERROR;
    m.body.error_code = code;
    return m;
}

void process_message(struct Message *m) {
    switch (m->type) {
        case MSG_CONNECT:
            printf("CONNECT client=%d ver=%d\n",
                   m->body.connect.client_id, m->body.connect.version);
            break;
        case MSG_DATA:
            printf("DATA seq=%d payload=%d len=%d\n",
                   m->body.data.seq_num, m->body.data.payload, m->body.data.length);
            break;
        case MSG_ACK:
            printf("ACK seq=%d status=%d\n",
                   m->body.ack.seq_num, m->body.ack.status);
            break;
        case MSG_DISCONNECT:
            printf("DISCONNECT\n");
            break;
        case MSG_ERROR:
            printf("ERROR code=%d\n", m->body.error_code);
            break;
    }
}

int main(void) {
    struct Message msgs[6];
    msgs[0] = make_connect(42, 3);
    msgs[1] = make_data(1, 12345, 64);
    msgs[2] = make_ack(1, 200);
    msgs[3] = make_data(2, 67890, 128);
    msgs[4] = make_ack(2, 200);
    msgs[5] = make_error(500);

    int i;
    for (i = 0; i < 6; i++) {
        printf("[%d] ", i);
        process_message(&msgs[i]);
    }

    int data_count = 0;
    int total_payload = 0;
    for (i = 0; i < 6; i++) {
        if (msgs[i].type == MSG_DATA) {
            data_count = data_count + 1;
            total_payload = total_payload + msgs[i].body.data.payload;
        }
    }
    printf("data messages: %d\n", data_count);
    printf("total payload: %d\n", total_payload);

    return 0;
}
