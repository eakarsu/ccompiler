int printf(const char *fmt, ...);

// Two-Phase Commit Protocol (2PC)
// Simulates a coordinator managing distributed transactions

struct Participant {
    int id;
    int is_alive;
    int can_commit;    // 1 if prepared, 0 if will abort
    int state;         // 0=init, 1=prepared, 2=committed, 3=aborted
    int data;
    int pending_data;
};

struct Coordinator {
    int tx_id;
    int state;         // 0=init, 1=preparing, 2=committed, 3=aborted
    int num_participants;
    int votes_yes;
    int votes_no;
};

char *pstate_name(int state) {
    if (state == 0) return "init";
    if (state == 1) return "prepared";
    if (state == 2) return "committed";
    if (state == 3) return "aborted";
    return "unknown";
}

char *cstate_name(int state) {
    if (state == 0) return "init";
    if (state == 1) return "preparing";
    if (state == 2) return "committed";
    if (state == 3) return "aborted";
    return "unknown";
}

void init_participant(struct Participant *p, int id, int data) {
    p->id = id;
    p->is_alive = 1;
    p->can_commit = 1;
    p->state = 0;
    p->data = data;
    p->pending_data = data;
}

void init_coordinator(struct Coordinator *c, int n) {
    c->tx_id = 0;
    c->state = 0;
    c->num_participants = n;
    c->votes_yes = 0;
    c->votes_no = 0;
}

// Phase 1: Coordinator sends prepare, participants vote
int prepare_participant(struct Participant *p, int new_data) {
    if (p->is_alive == 0) {
        return -1;  // timeout/failure
    }
    p->pending_data = new_data;
    if (p->can_commit) {
        p->state = 1;
        return 1;  // vote yes
    }
    p->state = 3;
    return 0;  // vote no
}

void phase1(struct Coordinator *c, struct Participant *parts, int new_data) {
    int i;
    int vote;

    c->state = 1;
    c->tx_id = c->tx_id + 1;
    c->votes_yes = 0;
    c->votes_no = 0;

    printf("Coordinator: Phase 1 - Prepare (tx %d)\n", c->tx_id);

    for (i = 0; i < c->num_participants; i = i + 1) {
        vote = prepare_participant(&parts[i], new_data);
        if (vote == 1) {
            c->votes_yes = c->votes_yes + 1;
            printf("  P%d votes YES\n", parts[i].id);
        } else if (vote == 0) {
            c->votes_no = c->votes_no + 1;
            printf("  P%d votes NO\n", parts[i].id);
        } else {
            c->votes_no = c->votes_no + 1;
            printf("  P%d TIMEOUT\n", parts[i].id);
        }
    }
}

// Phase 2: Coordinator decides and notifies
void phase2_commit(struct Coordinator *c, struct Participant *parts) {
    int i;
    c->state = 2;
    printf("Coordinator: Phase 2 - COMMIT\n");
    for (i = 0; i < c->num_participants; i = i + 1) {
        if (parts[i].is_alive) {
            parts[i].data = parts[i].pending_data;
            parts[i].state = 2;
            printf("  P%d committed, data=%d\n", parts[i].id, parts[i].data);
        }
    }
}

void phase2_abort(struct Coordinator *c, struct Participant *parts) {
    int i;
    c->state = 3;
    printf("Coordinator: Phase 2 - ABORT\n");
    for (i = 0; i < c->num_participants; i = i + 1) {
        if (parts[i].is_alive) {
            parts[i].pending_data = parts[i].data;
            parts[i].state = 3;
            printf("  P%d aborted, data=%d\n", parts[i].id, parts[i].data);
        }
    }
}

void run_transaction(struct Coordinator *c, struct Participant *parts, int new_data) {
    phase1(c, parts, new_data);
    if (c->votes_yes == c->num_participants) {
        phase2_commit(c, parts);
    } else {
        phase2_abort(c, parts);
    }
    printf("Transaction result: %s\n", cstate_name(c->state));
}

int main() {
    struct Participant parts[4];
    struct Coordinator coord;
    int i;

    // Setup 4 participants
    for (i = 0; i < 4; i = i + 1) {
        init_participant(&parts[i], i, 0);
    }
    init_coordinator(&coord, 4);

    // Transaction 1: All agree
    printf("=== Transaction 1: All agree ===\n");
    // EXPECT: === Transaction 1: All agree ===
    run_transaction(&coord, parts, 100);
    // EXPECT: Coordinator: Phase 1 - Prepare (tx 1)
    // EXPECT:   P0 votes YES
    // EXPECT:   P1 votes YES
    // EXPECT:   P2 votes YES
    // EXPECT:   P3 votes YES
    // EXPECT: Coordinator: Phase 2 - COMMIT
    // EXPECT:   P0 committed, data=100
    // EXPECT:   P1 committed, data=100
    // EXPECT:   P2 committed, data=100
    // EXPECT:   P3 committed, data=100
    // EXPECT: Transaction result: committed

    // Transaction 2: P2 will abort
    printf("=== Transaction 2: P2 refuses ===\n");
    // EXPECT: === Transaction 2: P2 refuses ===
    parts[2].can_commit = 0;
    // Reset states
    for (i = 0; i < 4; i = i + 1) {
        parts[i].state = 0;
    }
    run_transaction(&coord, parts, 200);
    // EXPECT: Coordinator: Phase 1 - Prepare (tx 2)
    // EXPECT:   P0 votes YES
    // EXPECT:   P1 votes YES
    // EXPECT:   P2 votes NO
    // EXPECT:   P3 votes YES
    // EXPECT: Coordinator: Phase 2 - ABORT
    // EXPECT:   P0 aborted, data=100
    // EXPECT:   P1 aborted, data=100
    // EXPECT:   P2 aborted, data=100
    // EXPECT:   P3 aborted, data=100
    // EXPECT: Transaction result: aborted

    // Transaction 3: P1 is down
    printf("=== Transaction 3: P1 down ===\n");
    // EXPECT: === Transaction 3: P1 down ===
    parts[2].can_commit = 1;
    parts[1].is_alive = 0;
    for (i = 0; i < 4; i = i + 1) {
        parts[i].state = 0;
    }
    run_transaction(&coord, parts, 300);
    // EXPECT: Coordinator: Phase 1 - Prepare (tx 3)
    // EXPECT:   P0 votes YES
    // EXPECT:   P1 TIMEOUT
    // EXPECT:   P2 votes YES
    // EXPECT:   P3 votes YES
    // EXPECT: Coordinator: Phase 2 - ABORT
    // EXPECT:   P0 aborted, data=100
    // EXPECT:   P2 aborted, data=100
    // EXPECT:   P3 aborted, data=100
    // EXPECT: Transaction result: aborted

    printf("Final data: P0=%d P1=%d P2=%d P3=%d\n",
           parts[0].data, parts[1].data, parts[2].data, parts[3].data);
    // EXPECT: Final data: P0=100 P1=100 P2=100 P3=100

    printf("2PC simulation done\n");
    // EXPECT: 2PC simulation done

    return 0;
}
