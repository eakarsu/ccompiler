int printf(const char *fmt, ...);

// TCP State Machine Simulation
// Implements simplified TCP connection states

enum tcp_state {
    TCP_CLOSED,
    TCP_LISTEN,
    TCP_SYN_SENT,
    TCP_SYN_RECEIVED,
    TCP_ESTABLISHED,
    TCP_FIN_WAIT_1,
    TCP_FIN_WAIT_2,
    TCP_CLOSE_WAIT,
    TCP_CLOSING,
    TCP_LAST_ACK,
    TCP_TIME_WAIT
};

enum tcp_event {
    EVT_PASSIVE_OPEN,
    EVT_ACTIVE_OPEN,
    EVT_SYN_RECV,
    EVT_SYN_ACK_RECV,
    EVT_ACK_RECV,
    EVT_CLOSE,
    EVT_FIN_RECV,
    EVT_TIMEOUT
};

struct tcp_conn {
    int state;
    int transitions;
    int log[32];
    int log_count;
};

void tcp_init(struct tcp_conn *conn) {
    conn->state = TCP_CLOSED;
    conn->transitions = 0;
    conn->log_count = 0;
    int i;
    for (i = 0; i < 32; i++) {
        conn->log[i] = -1;
    }
}

void tcp_log(struct tcp_conn *conn, int new_state) {
    if (conn->log_count < 32) {
        conn->log[conn->log_count] = new_state;
        conn->log_count = conn->log_count + 1;
    }
    conn->state = new_state;
    conn->transitions = conn->transitions + 1;
}

void tcp_event(struct tcp_conn *conn, int evt) {
    int st = conn->state;

    if (st == TCP_CLOSED) {
        if (evt == EVT_PASSIVE_OPEN) {
            tcp_log(conn, TCP_LISTEN);
        } else if (evt == EVT_ACTIVE_OPEN) {
            tcp_log(conn, TCP_SYN_SENT);
        }
    } else if (st == TCP_LISTEN) {
        if (evt == EVT_SYN_RECV) {
            tcp_log(conn, TCP_SYN_RECEIVED);
        } else if (evt == EVT_CLOSE) {
            tcp_log(conn, TCP_CLOSED);
        }
    } else if (st == TCP_SYN_SENT) {
        if (evt == EVT_SYN_ACK_RECV) {
            tcp_log(conn, TCP_ESTABLISHED);
        } else if (evt == EVT_SYN_RECV) {
            tcp_log(conn, TCP_SYN_RECEIVED);
        } else if (evt == EVT_CLOSE) {
            tcp_log(conn, TCP_CLOSED);
        }
    } else if (st == TCP_SYN_RECEIVED) {
        if (evt == EVT_ACK_RECV) {
            tcp_log(conn, TCP_ESTABLISHED);
        } else if (evt == EVT_CLOSE) {
            tcp_log(conn, TCP_FIN_WAIT_1);
        }
    } else if (st == TCP_ESTABLISHED) {
        if (evt == EVT_CLOSE) {
            tcp_log(conn, TCP_FIN_WAIT_1);
        } else if (evt == EVT_FIN_RECV) {
            tcp_log(conn, TCP_CLOSE_WAIT);
        }
    } else if (st == TCP_FIN_WAIT_1) {
        if (evt == EVT_ACK_RECV) {
            tcp_log(conn, TCP_FIN_WAIT_2);
        } else if (evt == EVT_FIN_RECV) {
            tcp_log(conn, TCP_CLOSING);
        }
    } else if (st == TCP_FIN_WAIT_2) {
        if (evt == EVT_FIN_RECV) {
            tcp_log(conn, TCP_TIME_WAIT);
        }
    } else if (st == TCP_CLOSE_WAIT) {
        if (evt == EVT_CLOSE) {
            tcp_log(conn, TCP_LAST_ACK);
        }
    } else if (st == TCP_CLOSING) {
        if (evt == EVT_ACK_RECV) {
            tcp_log(conn, TCP_TIME_WAIT);
        }
    } else if (st == TCP_LAST_ACK) {
        if (evt == EVT_ACK_RECV) {
            tcp_log(conn, TCP_CLOSED);
        }
    } else if (st == TCP_TIME_WAIT) {
        if (evt == EVT_TIMEOUT) {
            tcp_log(conn, TCP_CLOSED);
        }
    }
}

char *state_name(int s) {
    if (s == TCP_CLOSED) return "CLOSED";
    if (s == TCP_LISTEN) return "LISTEN";
    if (s == TCP_SYN_SENT) return "SYN_SENT";
    if (s == TCP_SYN_RECEIVED) return "SYN_RCVD";
    if (s == TCP_ESTABLISHED) return "ESTABLISHED";
    if (s == TCP_FIN_WAIT_1) return "FIN_WAIT_1";
    if (s == TCP_FIN_WAIT_2) return "FIN_WAIT_2";
    if (s == TCP_CLOSE_WAIT) return "CLOSE_WAIT";
    if (s == TCP_CLOSING) return "CLOSING";
    if (s == TCP_LAST_ACK) return "LAST_ACK";
    if (s == TCP_TIME_WAIT) return "TIME_WAIT";
    return "UNKNOWN";
}

int main() {
    struct tcp_conn conn;

    printf("=== TCP State Machine ===\n"); // EXPECT: === TCP State Machine ===

    // Scenario 1: Normal client connection and close
    // CLOSED -> SYN_SENT -> ESTABLISHED -> FIN_WAIT_1 -> FIN_WAIT_2 -> TIME_WAIT -> CLOSED
    tcp_init(&conn);
    printf("Start: %s\n", state_name(conn.state)); // EXPECT: Start: CLOSED

    tcp_event(&conn, EVT_ACTIVE_OPEN);
    printf("After open: %s\n", state_name(conn.state)); // EXPECT: After open: SYN_SENT

    tcp_event(&conn, EVT_SYN_ACK_RECV);
    printf("After syn-ack: %s\n", state_name(conn.state)); // EXPECT: After syn-ack: ESTABLISHED

    tcp_event(&conn, EVT_CLOSE);
    printf("After close: %s\n", state_name(conn.state)); // EXPECT: After close: FIN_WAIT_1

    tcp_event(&conn, EVT_ACK_RECV);
    printf("After ack: %s\n", state_name(conn.state)); // EXPECT: After ack: FIN_WAIT_2

    tcp_event(&conn, EVT_FIN_RECV);
    printf("After fin: %s\n", state_name(conn.state)); // EXPECT: After fin: TIME_WAIT

    tcp_event(&conn, EVT_TIMEOUT);
    printf("After timeout: %s\n", state_name(conn.state)); // EXPECT: After timeout: CLOSED

    printf("Transitions: %d\n", conn.transitions); // EXPECT: Transitions: 6

    // Scenario 2: Server-side flow
    // CLOSED -> LISTEN -> SYN_RECEIVED -> ESTABLISHED -> CLOSE_WAIT -> LAST_ACK -> CLOSED
    tcp_init(&conn);
    tcp_event(&conn, EVT_PASSIVE_OPEN);
    printf("Server listen: %s\n", state_name(conn.state)); // EXPECT: Server listen: LISTEN

    tcp_event(&conn, EVT_SYN_RECV);
    printf("Server syn: %s\n", state_name(conn.state)); // EXPECT: Server syn: SYN_RCVD

    tcp_event(&conn, EVT_ACK_RECV);
    printf("Server estab: %s\n", state_name(conn.state)); // EXPECT: Server estab: ESTABLISHED

    tcp_event(&conn, EVT_FIN_RECV);
    printf("Server fin: %s\n", state_name(conn.state)); // EXPECT: Server fin: CLOSE_WAIT

    tcp_event(&conn, EVT_CLOSE);
    printf("Server close: %s\n", state_name(conn.state)); // EXPECT: Server close: LAST_ACK

    tcp_event(&conn, EVT_ACK_RECV);
    printf("Server done: %s\n", state_name(conn.state)); // EXPECT: Server done: CLOSED

    printf("Transitions: %d\n", conn.transitions); // EXPECT: Transitions: 6

    // Scenario 3: Simultaneous close (CLOSING path)
    tcp_init(&conn);
    tcp_event(&conn, EVT_ACTIVE_OPEN);
    tcp_event(&conn, EVT_SYN_ACK_RECV);
    tcp_event(&conn, EVT_CLOSE);
    printf("Before simul: %s\n", state_name(conn.state)); // EXPECT: Before simul: FIN_WAIT_1

    tcp_event(&conn, EVT_FIN_RECV);
    printf("Simul close: %s\n", state_name(conn.state)); // EXPECT: Simul close: CLOSING

    tcp_event(&conn, EVT_ACK_RECV);
    printf("After simul: %s\n", state_name(conn.state)); // EXPECT: After simul: TIME_WAIT

    // Test log
    printf("Log count: %d\n", conn.log_count); // EXPECT: Log count: 5
    printf("Log 0: %s\n", state_name(conn.log[0])); // EXPECT: Log 0: SYN_SENT
    printf("Log 4: %s\n", state_name(conn.log[4])); // EXPECT: Log 4: TIME_WAIT

    // Scenario 4: Invalid event - no transition
    tcp_init(&conn);
    tcp_event(&conn, EVT_FIN_RECV);  // invalid in CLOSED state
    printf("No change: %s\n", state_name(conn.state)); // EXPECT: No change: CLOSED
    printf("Trans: %d\n", conn.transitions); // EXPECT: Trans: 0

    printf("Done\n"); // EXPECT: Done
    return 0;
}
