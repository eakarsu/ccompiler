int printf(const char *fmt, ...);

// Protocol State Machine - TCP Connection Simulation

enum TCPState {
    STATE_CLOSED,
    STATE_LISTEN,
    STATE_SYN_SENT,
    STATE_SYN_RECEIVED,
    STATE_ESTABLISHED,
    STATE_FIN_WAIT_1,
    STATE_FIN_WAIT_2,
    STATE_CLOSE_WAIT,
    STATE_CLOSING,
    STATE_LAST_ACK,
    STATE_TIME_WAIT
};

enum TCPEvent {
    EVT_PASSIVE_OPEN,
    EVT_ACTIVE_OPEN,
    EVT_SYN_RCVD,
    EVT_SYN_ACK_RCVD,
    EVT_ACK_RCVD,
    EVT_FIN_RCVD,
    EVT_CLOSE,
    EVT_TIMEOUT
};

struct TCPConnection {
    int state;
    int seq_num;
    int ack_num;
    int transitions;
    int error;
};

char *state_name(int state) {
    if (state == STATE_CLOSED) return "CLOSED";
    if (state == STATE_LISTEN) return "LISTEN";
    if (state == STATE_SYN_SENT) return "SYN_SENT";
    if (state == STATE_SYN_RECEIVED) return "SYN_RECEIVED";
    if (state == STATE_ESTABLISHED) return "ESTABLISHED";
    if (state == STATE_FIN_WAIT_1) return "FIN_WAIT_1";
    if (state == STATE_FIN_WAIT_2) return "FIN_WAIT_2";
    if (state == STATE_CLOSE_WAIT) return "CLOSE_WAIT";
    if (state == STATE_CLOSING) return "CLOSING";
    if (state == STATE_LAST_ACK) return "LAST_ACK";
    if (state == STATE_TIME_WAIT) return "TIME_WAIT";
    return "UNKNOWN";
}

char *event_name(int evt) {
    if (evt == EVT_PASSIVE_OPEN) return "PASSIVE_OPEN";
    if (evt == EVT_ACTIVE_OPEN) return "ACTIVE_OPEN";
    if (evt == EVT_SYN_RCVD) return "SYN_RCVD";
    if (evt == EVT_SYN_ACK_RCVD) return "SYN_ACK_RCVD";
    if (evt == EVT_ACK_RCVD) return "ACK_RCVD";
    if (evt == EVT_FIN_RCVD) return "FIN_RCVD";
    if (evt == EVT_CLOSE) return "CLOSE";
    if (evt == EVT_TIMEOUT) return "TIMEOUT";
    return "UNKNOWN";
}

void init_connection(struct TCPConnection *conn) {
    conn->state = STATE_CLOSED;
    conn->seq_num = 1000;
    conn->ack_num = 0;
    conn->transitions = 0;
    conn->error = 0;
}

int process_event(struct TCPConnection *conn, int event) {
    int old_state = conn->state;

    switch (conn->state) {
    case STATE_CLOSED:
        if (event == EVT_PASSIVE_OPEN) {
            conn->state = STATE_LISTEN;
        } else if (event == EVT_ACTIVE_OPEN) {
            conn->state = STATE_SYN_SENT;
            conn->seq_num = conn->seq_num + 1;
        } else {
            conn->error = 1;
            return 0;
        }
        break;

    case STATE_LISTEN:
        if (event == EVT_SYN_RCVD) {
            conn->state = STATE_SYN_RECEIVED;
            conn->ack_num = conn->ack_num + 1;
        } else if (event == EVT_CLOSE) {
            conn->state = STATE_CLOSED;
        } else {
            conn->error = 1;
            return 0;
        }
        break;

    case STATE_SYN_SENT:
        if (event == EVT_SYN_ACK_RCVD) {
            conn->state = STATE_ESTABLISHED;
            conn->ack_num = conn->ack_num + 1;
        } else if (event == EVT_CLOSE) {
            conn->state = STATE_CLOSED;
        } else {
            conn->error = 1;
            return 0;
        }
        break;

    case STATE_SYN_RECEIVED:
        if (event == EVT_ACK_RCVD) {
            conn->state = STATE_ESTABLISHED;
        } else if (event == EVT_CLOSE) {
            conn->state = STATE_FIN_WAIT_1;
        } else {
            conn->error = 1;
            return 0;
        }
        break;

    case STATE_ESTABLISHED:
        if (event == EVT_FIN_RCVD) {
            conn->state = STATE_CLOSE_WAIT;
        } else if (event == EVT_CLOSE) {
            conn->state = STATE_FIN_WAIT_1;
            conn->seq_num = conn->seq_num + 1;
        } else {
            conn->error = 1;
            return 0;
        }
        break;

    case STATE_FIN_WAIT_1:
        if (event == EVT_ACK_RCVD) {
            conn->state = STATE_FIN_WAIT_2;
        } else if (event == EVT_FIN_RCVD) {
            conn->state = STATE_CLOSING;
        } else {
            conn->error = 1;
            return 0;
        }
        break;

    case STATE_FIN_WAIT_2:
        if (event == EVT_FIN_RCVD) {
            conn->state = STATE_TIME_WAIT;
            conn->ack_num = conn->ack_num + 1;
        } else {
            conn->error = 1;
            return 0;
        }
        break;

    case STATE_CLOSE_WAIT:
        if (event == EVT_CLOSE) {
            conn->state = STATE_LAST_ACK;
            conn->seq_num = conn->seq_num + 1;
        } else {
            conn->error = 1;
            return 0;
        }
        break;

    case STATE_CLOSING:
        if (event == EVT_ACK_RCVD) {
            conn->state = STATE_TIME_WAIT;
        } else {
            conn->error = 1;
            return 0;
        }
        break;

    case STATE_LAST_ACK:
        if (event == EVT_ACK_RCVD) {
            conn->state = STATE_CLOSED;
        } else {
            conn->error = 1;
            return 0;
        }
        break;

    case STATE_TIME_WAIT:
        if (event == EVT_TIMEOUT) {
            conn->state = STATE_CLOSED;
        } else {
            conn->error = 1;
            return 0;
        }
        break;

    default:
        conn->error = 1;
        return 0;
    }

    if (conn->state != old_state) {
        conn->transitions = conn->transitions + 1;
    }
    return 1;
}

int main() {
    struct TCPConnection conn;

    // Test 1: Client-side 3-way handshake (active open)
    init_connection(&conn);
    printf("initial: %s\n", state_name(conn.state));
    // EXPECT: initial: CLOSED

    process_event(&conn, EVT_ACTIVE_OPEN);
    printf("after active open: %s\n", state_name(conn.state));
    // EXPECT: after active open: SYN_SENT

    process_event(&conn, EVT_SYN_ACK_RCVD);
    printf("after syn-ack: %s\n", state_name(conn.state));
    // EXPECT: after syn-ack: ESTABLISHED

    printf("transitions: %d\n", conn.transitions);
    // EXPECT: transitions: 2
    printf("seq: %d ack: %d\n", conn.seq_num, conn.ack_num);
    // EXPECT: seq: 1001 ack: 1

    // Test 2: Client-initiated close (active close)
    process_event(&conn, EVT_CLOSE);
    printf("after close: %s\n", state_name(conn.state));
    // EXPECT: after close: FIN_WAIT_1

    process_event(&conn, EVT_ACK_RCVD);
    printf("after ack: %s\n", state_name(conn.state));
    // EXPECT: after ack: FIN_WAIT_2

    process_event(&conn, EVT_FIN_RCVD);
    printf("after fin: %s\n", state_name(conn.state));
    // EXPECT: after fin: TIME_WAIT

    process_event(&conn, EVT_TIMEOUT);
    printf("after timeout: %s\n", state_name(conn.state));
    // EXPECT: after timeout: CLOSED

    printf("total transitions: %d\n", conn.transitions);
    // EXPECT: total transitions: 6

    // Test 3: Server-side handshake (passive open)
    struct TCPConnection server;
    init_connection(&server);

    process_event(&server, EVT_PASSIVE_OPEN);
    printf("server: %s\n", state_name(server.state));
    // EXPECT: server: LISTEN

    process_event(&server, EVT_SYN_RCVD);
    printf("server: %s\n", state_name(server.state));
    // EXPECT: server: SYN_RECEIVED

    process_event(&server, EVT_ACK_RCVD);
    printf("server: %s\n", state_name(server.state));
    // EXPECT: server: ESTABLISHED

    // Test 4: Server passive close (receives FIN first)
    process_event(&server, EVT_FIN_RCVD);
    printf("server: %s\n", state_name(server.state));
    // EXPECT: server: CLOSE_WAIT

    process_event(&server, EVT_CLOSE);
    printf("server: %s\n", state_name(server.state));
    // EXPECT: server: LAST_ACK

    process_event(&server, EVT_ACK_RCVD);
    printf("server: %s\n", state_name(server.state));
    // EXPECT: server: CLOSED

    printf("server transitions: %d\n", server.transitions);
    // EXPECT: server transitions: 6

    // Test 5: Invalid transition
    struct TCPConnection bad;
    init_connection(&bad);
    int ok = process_event(&bad, EVT_FIN_RCVD);
    printf("invalid transition: ok=%d error=%d\n", ok, bad.error);
    // EXPECT: invalid transition: ok=0 error=1
    printf("state unchanged: %s\n", state_name(bad.state));
    // EXPECT: state unchanged: CLOSED

    // Test 6: simultaneous close (CLOSING state)
    struct TCPConnection simclose;
    init_connection(&simclose);
    process_event(&simclose, EVT_ACTIVE_OPEN);
    process_event(&simclose, EVT_SYN_ACK_RCVD);
    process_event(&simclose, EVT_CLOSE);
    printf("before simul: %s\n", state_name(simclose.state));
    // EXPECT: before simul: FIN_WAIT_1

    process_event(&simclose, EVT_FIN_RCVD);
    printf("closing: %s\n", state_name(simclose.state));
    // EXPECT: closing: CLOSING

    process_event(&simclose, EVT_ACK_RCVD);
    printf("time wait: %s\n", state_name(simclose.state));
    // EXPECT: time wait: TIME_WAIT

    // Test 7: event names
    printf("evt 0: %s\n", event_name(EVT_PASSIVE_OPEN));
    // EXPECT: evt 0: PASSIVE_OPEN
    printf("evt 1: %s\n", event_name(EVT_ACTIVE_OPEN));
    // EXPECT: evt 1: ACTIVE_OPEN
    printf("evt 6: %s\n", event_name(EVT_CLOSE));
    // EXPECT: evt 6: CLOSE

    // Test 8: Listen then close
    struct TCPConnection lc;
    init_connection(&lc);
    process_event(&lc, EVT_PASSIVE_OPEN);
    printf("listen: %s\n", state_name(lc.state));
    // EXPECT: listen: LISTEN
    process_event(&lc, EVT_CLOSE);
    printf("closed: %s\n", state_name(lc.state));
    // EXPECT: closed: CLOSED

    return 0;
}
