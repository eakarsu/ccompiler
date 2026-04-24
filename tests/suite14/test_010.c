int printf(const char *fmt, ...);

/* Protocol state machine: TCP-like connection handshake.
   States: CLOSED=0, LISTEN=1, SYN_SENT=2, SYN_RCVD=3,
           ESTABLISHED=4, FIN_WAIT=5, CLOSE_WAIT=6, TIME_WAIT=7
   Events: PASSIVE_OPEN=0, ACTIVE_OPEN=1, SYN=2, SYN_ACK=3,
           ACK=4, FIN=5, CLOSE=6, TIMEOUT=7 */

typedef struct {
    int state;
    int seq_num;
    int ack_num;
    int data_sent;
    int data_received;
    int error_count;
    int transition_count;
} Connection;

void conn_init(Connection *c) {
    c->state = 0; /* CLOSED */
    c->seq_num = 100;
    c->ack_num = 0;
    c->data_sent = 0;
    c->data_received = 0;
    c->error_count = 0;
    c->transition_count = 0;
}

int conn_handle_event(Connection *c, int event) {
    int old_state = c->state;
    int valid = 1;

    if (c->state == 0) {
        /* CLOSED */
        if (event == 0) {
            c->state = 1; /* LISTEN */
        } else if (event == 1) {
            c->state = 2; /* SYN_SENT */
            c->seq_num++;
        } else {
            valid = 0;
        }
    } else if (c->state == 1) {
        /* LISTEN */
        if (event == 2) {
            c->state = 3; /* SYN_RCVD */
            c->ack_num = 200;
            c->seq_num++;
        } else if (event == 6) {
            c->state = 0; /* CLOSED */
        } else {
            valid = 0;
        }
    } else if (c->state == 2) {
        /* SYN_SENT */
        if (event == 3) {
            c->state = 4; /* ESTABLISHED */
            c->ack_num = 201;
            c->seq_num++;
        } else if (event == 2) {
            /* Simultaneous open */
            c->state = 3; /* SYN_RCVD */
            c->ack_num = 200;
        } else if (event == 6) {
            c->state = 0; /* CLOSED */
        } else {
            valid = 0;
        }
    } else if (c->state == 3) {
        /* SYN_RCVD */
        if (event == 4) {
            c->state = 4; /* ESTABLISHED */
        } else if (event == 6) {
            c->state = 5; /* FIN_WAIT */
            c->seq_num++;
        } else {
            valid = 0;
        }
    } else if (c->state == 4) {
        /* ESTABLISHED */
        if (event == 5) {
            c->state = 6; /* CLOSE_WAIT */
            c->ack_num++;
        } else if (event == 6) {
            c->state = 5; /* FIN_WAIT */
            c->seq_num++;
        } else if (event == 4) {
            /* Data ACK */
            c->data_received++;
        } else {
            valid = 0;
        }
    } else if (c->state == 5) {
        /* FIN_WAIT */
        if (event == 4) {
            c->state = 7; /* TIME_WAIT */
        } else if (event == 5) {
            c->state = 7; /* TIME_WAIT (simultaneous close) */
            c->ack_num++;
        } else {
            valid = 0;
        }
    } else if (c->state == 6) {
        /* CLOSE_WAIT */
        if (event == 6) {
            c->state = 7; /* TIME_WAIT (send our FIN) */
            c->seq_num++;
        } else {
            valid = 0;
        }
    } else if (c->state == 7) {
        /* TIME_WAIT */
        if (event == 7) {
            c->state = 0; /* CLOSED */
        } else {
            valid = 0;
        }
    }

    if (!valid) {
        c->error_count++;
    } else if (old_state != c->state) {
        c->transition_count++;
    }

    return valid;
}

void send_data(Connection *c, int amount) {
    if (c->state == 4) {
        c->data_sent = c->data_sent + amount;
        c->seq_num = c->seq_num + amount;
    }
}

char *sname(int state) {
    if (state == 0) return "CLOSED";
    if (state == 1) return "LISTEN";
    if (state == 2) return "SYN_SENT";
    if (state == 3) return "SYN_RCVD";
    if (state == 4) return "ESTAB";
    if (state == 5) return "FIN_WAIT";
    if (state == 6) return "CLOSE_WAIT";
    if (state == 7) return "TIME_WAIT";
    return "UNKNOWN";
}

void test_server_handshake(void) {
    Connection c;
    int r;

    printf("=== Server Handshake ===\n");
    // EXPECT: === Server Handshake ===

    conn_init(&c);
    printf("init: %s\n", sname(c.state));
    // EXPECT: init: CLOSED

    r = conn_handle_event(&c, 0); /* PASSIVE_OPEN */
    printf("passive_open: %s ok=%d\n", sname(c.state), r);
    // EXPECT: passive_open: LISTEN ok=1

    r = conn_handle_event(&c, 2); /* SYN received */
    printf("syn_rcvd: %s seq=%d ack=%d\n", sname(c.state), c.seq_num, c.ack_num);
    // EXPECT: syn_rcvd: SYN_RCVD seq=101 ack=200

    r = conn_handle_event(&c, 4); /* ACK received */
    printf("established: %s trans=%d\n", sname(c.state), c.transition_count);
    // EXPECT: established: ESTAB trans=3
}

void test_client_handshake(void) {
    Connection c;

    printf("=== Client Handshake ===\n");
    // EXPECT: === Client Handshake ===

    conn_init(&c);
    conn_handle_event(&c, 1); /* ACTIVE_OPEN -> SYN_SENT */
    printf("syn_sent: %s seq=%d\n", sname(c.state), c.seq_num);
    // EXPECT: syn_sent: SYN_SENT seq=101

    conn_handle_event(&c, 3); /* SYN_ACK -> ESTABLISHED */
    printf("established: %s seq=%d ack=%d\n", sname(c.state), c.seq_num, c.ack_num);
    // EXPECT: established: ESTAB seq=102 ack=201

    printf("transitions: %d\n", c.transition_count);
    // EXPECT: transitions: 2
}

void test_data_transfer(void) {
    Connection c;

    printf("=== Data Transfer ===\n");
    // EXPECT: === Data Transfer ===

    conn_init(&c);
    conn_handle_event(&c, 1); /* ACTIVE_OPEN */
    conn_handle_event(&c, 3); /* SYN_ACK -> ESTABLISHED */

    send_data(&c, 50);
    printf("after send 50: seq=%d sent=%d\n", c.seq_num, c.data_sent);
    // EXPECT: after send 50: seq=152 sent=50

    conn_handle_event(&c, 4); /* ACK */
    printf("data ack: recv=%d\n", c.data_received);
    // EXPECT: data ack: recv=1

    send_data(&c, 30);
    printf("after send 30: seq=%d sent=%d\n", c.seq_num, c.data_sent);
    // EXPECT: after send 30: seq=182 sent=80

    conn_handle_event(&c, 4); /* ACK */
    conn_handle_event(&c, 4); /* ACK */
    printf("total recv: %d\n", c.data_received);
    // EXPECT: total recv: 3
}

void test_graceful_close(void) {
    Connection c;

    printf("=== Graceful Close ===\n");
    // EXPECT: === Graceful Close ===

    conn_init(&c);
    conn_handle_event(&c, 1); /* ACTIVE_OPEN */
    conn_handle_event(&c, 3); /* SYN_ACK -> ESTABLISHED */

    conn_handle_event(&c, 6); /* CLOSE -> FIN_WAIT */
    printf("fin_wait: %s\n", sname(c.state));
    // EXPECT: fin_wait: FIN_WAIT

    conn_handle_event(&c, 4); /* ACK -> TIME_WAIT */
    printf("time_wait: %s\n", sname(c.state));
    // EXPECT: time_wait: TIME_WAIT

    conn_handle_event(&c, 7); /* TIMEOUT -> CLOSED */
    printf("closed: %s\n", sname(c.state));
    // EXPECT: closed: CLOSED

    printf("transitions: %d\n", c.transition_count);
    // EXPECT: transitions: 5
}

void test_passive_close(void) {
    Connection c;

    printf("=== Passive Close ===\n");
    // EXPECT: === Passive Close ===

    conn_init(&c);
    conn_handle_event(&c, 0); /* PASSIVE_OPEN -> LISTEN */
    conn_handle_event(&c, 2); /* SYN -> SYN_RCVD */
    conn_handle_event(&c, 4); /* ACK -> ESTABLISHED */

    /* Receive FIN from remote */
    conn_handle_event(&c, 5); /* FIN -> CLOSE_WAIT */
    printf("close_wait: %s ack=%d\n", sname(c.state), c.ack_num);
    // EXPECT: close_wait: CLOSE_WAIT ack=201

    /* Send our FIN */
    conn_handle_event(&c, 6); /* CLOSE -> TIME_WAIT */
    printf("time_wait: %s seq=%d\n", sname(c.state), c.seq_num);
    // EXPECT: time_wait: TIME_WAIT seq=102

    conn_handle_event(&c, 7); /* TIMEOUT -> CLOSED */
    printf("final: %s\n", sname(c.state));
    // EXPECT: final: CLOSED

    printf("transitions: %d errors: %d\n", c.transition_count, c.error_count);
    // EXPECT: transitions: 6 errors: 0
}

void test_invalid_events(void) {
    Connection c;
    int r;

    printf("=== Invalid Events ===\n");
    // EXPECT: === Invalid Events ===

    conn_init(&c);

    /* Can't receive SYN in CLOSED */
    r = conn_handle_event(&c, 2);
    printf("syn in closed: ok=%d err=%d\n", r, c.error_count);
    // EXPECT: syn in closed: ok=0 err=1

    /* Can't send ACK in CLOSED */
    r = conn_handle_event(&c, 4);
    printf("ack in closed: ok=%d err=%d\n", r, c.error_count);
    // EXPECT: ack in closed: ok=0 err=2

    /* Open, then try invalid */
    conn_handle_event(&c, 1); /* SYN_SENT */
    r = conn_handle_event(&c, 4); /* ACK invalid in SYN_SENT */
    printf("ack in syn_sent: ok=%d err=%d\n", r, c.error_count);
    // EXPECT: ack in syn_sent: ok=0 err=3

    printf("state still: %s\n", sname(c.state));
    // EXPECT: state still: SYN_SENT
}

int main(void) {
    test_server_handshake();
    test_client_handshake();
    test_data_transfer();
    test_graceful_close();
    test_passive_close();
    test_invalid_events();

    printf("Protocol SM done.\n");
    // EXPECT: Protocol SM done.
    return 0;
}
