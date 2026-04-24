int printf(const char *fmt, ...);

// Actor model simulation (single-threaded)
// Each actor has a mailbox and processes messages sequentially

struct ActorMsg {
    int from;
    int type;     // 0=ping, 1=pong, 2=compute, 3=result, 4=stop
    int payload;
};

struct Actor {
    int id;
    int alive;
    int state;
    struct ActorMsg mailbox[8];
    int mail_count;
    int msgs_processed;
    int msgs_sent;
};

void actor_init(struct Actor *a, int id) {
    int i;
    a->id = id;
    a->alive = 1;
    a->state = 0;
    a->mail_count = 0;
    a->msgs_processed = 0;
    a->msgs_sent = 0;
    for (i = 0; i < 8; i = i + 1) {
        a->mailbox[i].from = -1;
        a->mailbox[i].type = 0;
        a->mailbox[i].payload = 0;
    }
}

int actor_send(struct Actor *target, int from, int type, int payload) {
    if (!target->alive) return 0;
    if (target->mail_count >= 8) return 0;
    target->mailbox[target->mail_count].from = from;
    target->mailbox[target->mail_count].type = type;
    target->mailbox[target->mail_count].payload = payload;
    target->mail_count = target->mail_count + 1;
    return 1;
}

int actor_has_mail(struct Actor *a) {
    return a->mail_count > 0;
}

// Process one message: returns the type processed or -1
int actor_process_one(struct Actor *a, struct Actor *actors, int n) {
    struct ActorMsg msg;
    int i;
    int sender_idx;
    if (!a->alive || a->mail_count <= 0) return -1;

    // Take first message
    msg = a->mailbox[0];
    for (i = 0; i < a->mail_count - 1; i = i + 1) {
        a->mailbox[i] = a->mailbox[i + 1];
    }
    a->mail_count = a->mail_count - 1;
    a->msgs_processed = a->msgs_processed + 1;

    // Find sender in actors array
    sender_idx = -1;
    for (i = 0; i < n; i = i + 1) {
        if (actors[i].id == msg.from) {
            sender_idx = i;
        }
    }

    if (msg.type == 0) {
        // Ping -> send Pong back
        if (sender_idx >= 0) {
            actor_send(&actors[sender_idx], a->id, 1, msg.payload);
            a->msgs_sent = a->msgs_sent + 1;
        }
    } else if (msg.type == 2) {
        // Compute -> double the payload, send result
        a->state = a->state + msg.payload;
        if (sender_idx >= 0) {
            actor_send(&actors[sender_idx], a->id, 3, msg.payload * 2);
            a->msgs_sent = a->msgs_sent + 1;
        }
    } else if (msg.type == 3) {
        // Result -> accumulate in state
        a->state = a->state + msg.payload;
    } else if (msg.type == 4) {
        // Stop
        a->alive = 0;
    }

    return msg.type;
}

// Supervisor: restart dead actors
int supervisor_check(struct Actor *actors, int n) {
    int restarted;
    int i;
    restarted = 0;
    for (i = 0; i < n; i = i + 1) {
        if (!actors[i].alive) {
            actors[i].alive = 1;
            actors[i].state = 0;
            actors[i].mail_count = 0;
            restarted = restarted + 1;
        }
    }
    return restarted;
}

// Count alive actors
int count_alive(struct Actor *actors, int n) {
    int c;
    int i;
    c = 0;
    for (i = 0; i < n; i = i + 1) {
        if (actors[i].alive) c = c + 1;
    }
    return c;
}

// Total messages processed across all actors
int total_processed(struct Actor *actors, int n) {
    int t;
    int i;
    t = 0;
    for (i = 0; i < n; i = i + 1) {
        t = t + actors[i].msgs_processed;
    }
    return t;
}

int main() {
    struct Actor actors[4];
    int i;

    for (i = 0; i < 4; i = i + 1) {
        actor_init(&actors[i], i);
    }
    printf("alive: %d\n", count_alive(actors, 4)); // EXPECT: alive: 4

    // Ping-pong between actor 0 and actor 1
    actor_send(&actors[1], 0, 0, 42);
    printf("a1 mail: %d\n", actor_has_mail(&actors[1])); // EXPECT: a1 mail: 1

    int t = actor_process_one(&actors[1], actors, 4);
    printf("processed type: %d\n", t); // EXPECT: processed type: 0

    // Actor 0 should have a pong message
    printf("a0 mail: %d\n", actor_has_mail(&actors[0])); // EXPECT: a0 mail: 1
    t = actor_process_one(&actors[0], actors, 4);
    printf("pong type: %d\n", t); // EXPECT: pong type: 1

    // Compute messages: actor 0 sends compute to actor 2
    actor_send(&actors[2], 0, 2, 10);
    actor_send(&actors[2], 0, 2, 20);
    actor_send(&actors[2], 0, 2, 30);

    // Process all compute messages
    actor_process_one(&actors[2], actors, 4);
    actor_process_one(&actors[2], actors, 4);
    actor_process_one(&actors[2], actors, 4);
    printf("a2 state: %d\n", actors[2].state); // EXPECT: a2 state: 60
    printf("a2 processed: %d\n", actors[2].msgs_processed); // EXPECT: a2 processed: 3

    // Actor 0 should have 3 result messages
    printf("a0 mail count: %d\n", actors[0].mail_count); // EXPECT: a0 mail count: 3
    actor_process_one(&actors[0], actors, 4);
    actor_process_one(&actors[0], actors, 4);
    actor_process_one(&actors[0], actors, 4);
    printf("a0 state: %d\n", actors[0].state); // EXPECT: a0 state: 120

    // Stop actor 3
    actor_send(&actors[3], 0, 4, 0);
    actor_process_one(&actors[3], actors, 4);
    printf("a3 alive: %d\n", actors[3].alive); // EXPECT: a3 alive: 0
    printf("alive: %d\n", count_alive(actors, 4)); // EXPECT: alive: 3

    // Send to dead actor fails
    printf("send dead: %d\n", actor_send(&actors[3], 0, 0, 1)); // EXPECT: send dead: 0

    // Supervisor restarts dead actors
    int restarted = supervisor_check(actors, 4);
    printf("restarted: %d\n", restarted); // EXPECT: restarted: 1
    printf("alive: %d\n", count_alive(actors, 4)); // EXPECT: alive: 4

    // After restart, state is reset
    printf("a3 state: %d\n", actors[3].state); // EXPECT: a3 state: 0

    // Now actor 3 can receive messages again
    actor_send(&actors[3], 1, 2, 50);
    actor_process_one(&actors[3], actors, 4);
    printf("a3 state: %d\n", actors[3].state); // EXPECT: a3 state: 50

    printf("total processed: %d\n", total_processed(actors, 4)); // EXPECT: total processed: 10
    printf("a1 sent: %d\n", actors[1].msgs_sent); // EXPECT: a1 sent: 1
    printf("a2 sent: %d\n", actors[2].msgs_sent); // EXPECT: a2 sent: 3

    return 0;
}
