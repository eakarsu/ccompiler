int printf(const char *fmt, ...);

// Vector Clock Simulation
// Tracks causal ordering across multiple processes

struct VectorClock {
    int times[4];
    int pid;
    int n;
};

void vc_init(struct VectorClock *vc, int pid, int n) {
    int i;
    vc->pid = pid;
    vc->n = n;
    for (i = 0; i < 4; i = i + 1) {
        vc->times[i] = 0;
    }
}

void vc_internal(struct VectorClock *vc) {
    vc->times[vc->pid] = vc->times[vc->pid] + 1;
}

void vc_send(struct VectorClock *vc) {
    vc->times[vc->pid] = vc->times[vc->pid] + 1;
}

void vc_receive(struct VectorClock *dst, struct VectorClock *src) {
    int i;
    dst->times[dst->pid] = dst->times[dst->pid] + 1;
    for (i = 0; i < dst->n; i = i + 1) {
        if (src->times[i] > dst->times[i]) {
            dst->times[i] = src->times[i];
        }
    }
}

void vc_copy(struct VectorClock *dst, struct VectorClock *src) {
    int i;
    dst->pid = src->pid;
    dst->n = src->n;
    for (i = 0; i < 4; i = i + 1) {
        dst->times[i] = src->times[i];
    }
}

void vc_print(struct VectorClock *vc, int label) {
    printf("VC[P%d]: [%d,%d,%d]\n", label, vc->times[0], vc->times[1], vc->times[2]);
}

// Returns 1 if a happened-before b
int vc_hb(struct VectorClock *a, struct VectorClock *b) {
    int i;
    int all_leq;
    int some_lt;
    all_leq = 1;
    some_lt = 0;
    for (i = 0; i < a->n; i = i + 1) {
        if (a->times[i] > b->times[i]) {
            all_leq = 0;
        }
        if (a->times[i] < b->times[i]) {
            some_lt = 1;
        }
    }
    return all_leq && some_lt;
}

int vc_concurrent(struct VectorClock *a, struct VectorClock *b) {
    int ab;
    int ba;
    ab = vc_hb(a, b);
    ba = vc_hb(b, a);
    if (ab == 0 && ba == 0) {
        return 1;
    }
    return 0;
}

int main() {
    struct VectorClock p0;
    struct VectorClock p1;
    struct VectorClock p2;
    struct VectorClock msg;
    int result;

    vc_init(&p0, 0, 3);
    vc_init(&p1, 1, 3);
    vc_init(&p2, 2, 3);

    // P0 does internal event
    vc_internal(&p0);
    vc_print(&p0, 0);
    // EXPECT: VC[P0]: [1,0,0]

    // P0 sends to P1
    vc_send(&p0);
    vc_copy(&msg, &p0);
    vc_print(&p0, 0);
    // EXPECT: VC[P0]: [2,0,0]

    // P1 receives from P0
    vc_receive(&p1, &msg);
    vc_print(&p1, 1);
    // EXPECT: VC[P1]: [2,1,0]

    // P1 does internal event
    vc_internal(&p1);
    vc_print(&p1, 1);
    // EXPECT: VC[P1]: [2,2,0]

    // P1 sends to P2
    vc_send(&p1);
    vc_copy(&msg, &p1);
    vc_print(&p1, 1);
    // EXPECT: VC[P1]: [2,3,0]

    // P2 receives from P1
    vc_receive(&p2, &msg);
    vc_print(&p2, 2);
    // EXPECT: VC[P2]: [2,3,1]

    // P0 does another internal event
    vc_internal(&p0);
    vc_print(&p0, 0);
    // EXPECT: VC[P0]: [3,0,0]

    // Check happened-before: P0[2,0,0] -> P1[2,1,0]?
    // We need a snapshot. Use current values for testing.
    result = vc_hb(&p0, &p2);
    printf("P0 -> P2: %d\n", result);
    // EXPECT: P0 -> P2: 0

    result = vc_concurrent(&p0, &p2);
    printf("P0 || P2: %d\n", result);
    // EXPECT: P0 || P2: 1

    // P2 sends to P0
    vc_send(&p2);
    vc_copy(&msg, &p2);
    vc_receive(&p0, &msg);
    vc_print(&p0, 0);
    // EXPECT: VC[P0]: [4,3,2]

    vc_print(&p2, 2);
    // EXPECT: VC[P2]: [2,3,2]

    result = vc_hb(&p2, &p0);
    printf("P2 -> P0: %d\n", result);
    // EXPECT: P2 -> P0: 1

    printf("Vector clock simulation done\n");
    // EXPECT: Vector clock simulation done

    return 0;
}
