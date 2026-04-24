int printf(const char *fmt, ...);

// Lamport Clock Simulation
// Simulates logical clocks across multiple processes with send/receive events

struct LamportClock {
    int pid;
    int time;
};

struct Event {
    int type;    // 0=internal, 1=send, 2=receive
    int from;
    int to;
    int send_time;
};

void init_clock(struct LamportClock *c, int pid) {
    c->pid = pid;
    c->time = 0;
}

void internal_event(struct LamportClock *c) {
    c->time = c->time + 1;
}

int send_event(struct LamportClock *c) {
    c->time = c->time + 1;
    return c->time;
}

void receive_event(struct LamportClock *c, int sender_time) {
    if (sender_time > c->time) {
        c->time = sender_time + 1;
    } else {
        c->time = c->time + 1;
    }
}

int happened_before(int time_a, int time_b) {
    return time_a < time_b;
}

int are_concurrent(int time_a, int time_b) {
    return time_a == time_b;
}

void simulate_scenario_1(struct LamportClock *clocks) {
    int msg_time;

    // Process 0: internal event
    internal_event(&clocks[0]);
    printf("P0 after internal: %d\n", clocks[0].time);
    // EXPECT: P0 after internal: 1

    // Process 0 sends to Process 1
    msg_time = send_event(&clocks[0]);
    printf("P0 sends at: %d\n", msg_time);
    // EXPECT: P0 sends at: 2

    // Process 1: internal event first
    internal_event(&clocks[1]);
    printf("P1 after internal: %d\n", clocks[1].time);
    // EXPECT: P1 after internal: 1

    // Process 1 receives from Process 0
    receive_event(&clocks[1], msg_time);
    printf("P1 after receive: %d\n", clocks[1].time);
    // EXPECT: P1 after receive: 3

    // Process 1 sends to Process 2
    msg_time = send_event(&clocks[1]);
    printf("P1 sends at: %d\n", msg_time);
    // EXPECT: P1 sends at: 4

    // Process 2 receives
    receive_event(&clocks[2], msg_time);
    printf("P2 after receive: %d\n", clocks[2].time);
    // EXPECT: P2 after receive: 5
}

void simulate_scenario_2(struct LamportClock *clocks) {
    int msg1;
    int msg2;

    // Both P0 and P1 do internal events concurrently
    internal_event(&clocks[0]);
    internal_event(&clocks[1]);
    printf("P0 time: %d, P1 time: %d\n", clocks[0].time, clocks[1].time);
    // EXPECT: P0 time: 3, P1 time: 5

    // P0 sends to P2
    msg1 = send_event(&clocks[0]);
    printf("P0 sends msg1 at: %d\n", msg1);
    // EXPECT: P0 sends msg1 at: 4

    // P1 sends to P2
    msg2 = send_event(&clocks[1]);
    printf("P1 sends msg2 at: %d\n", msg2);
    // EXPECT: P1 sends msg2 at: 6

    // P2 receives msg1 first
    receive_event(&clocks[2], msg1);
    printf("P2 after msg1: %d\n", clocks[2].time);
    // EXPECT: P2 after msg1: 6

    // P2 receives msg2
    receive_event(&clocks[2], msg2);
    printf("P2 after msg2: %d\n", clocks[2].time);
    // EXPECT: P2 after msg2: 7
}

void check_ordering(struct LamportClock *clocks) {
    int hb;
    int conc;

    hb = happened_before(clocks[0].time, clocks[2].time);
    printf("P0 happened before P2: %d\n", hb);
    // EXPECT: P0 happened before P2: 1

    conc = are_concurrent(clocks[0].time, clocks[1].time);
    printf("P0 concurrent with P1: %d\n", conc);
    // EXPECT: P0 concurrent with P1: 0

    printf("Final clocks: P0=%d P1=%d P2=%d\n", clocks[0].time, clocks[1].time, clocks[2].time);
    // EXPECT: Final clocks: P0=4 P1=6 P2=7
}

int main() {
    struct LamportClock clocks[3];
    int i;

    for (i = 0; i < 3; i = i + 1) {
        init_clock(&clocks[i], i);
    }

    simulate_scenario_1(clocks);
    simulate_scenario_2(clocks);
    check_ordering(clocks);

    printf("Lamport clock simulation complete\n");
    // EXPECT: Lamport clock simulation complete

    return 0;
}
