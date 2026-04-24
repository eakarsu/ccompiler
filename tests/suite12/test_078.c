int printf(const char *fmt, ...);

// Failure Detector (Heartbeat-based)
// Simulates a heartbeat failure detection system with phi-accrual style suspicion

struct HeartbeatEntry {
    int node_id;
    int last_heartbeat;
    int interval_sum;
    int interval_count;
    int last_interval;
    int intervals[8];
};

struct FailureDetector {
    struct HeartbeatEntry entries[6];
    int num_nodes;
    int current_time;
    int timeout_threshold;
    int suspicion_threshold;
};

void fd_init(struct FailureDetector *fd, int n, int threshold) {
    int i;
    int j;
    fd->num_nodes = n;
    fd->current_time = 0;
    fd->timeout_threshold = threshold;
    fd->suspicion_threshold = threshold * 2;
    for (i = 0; i < n; i = i + 1) {
        fd->entries[i].node_id = i;
        fd->entries[i].last_heartbeat = 0;
        fd->entries[i].interval_sum = 0;
        fd->entries[i].interval_count = 0;
        fd->entries[i].last_interval = 0;
        for (j = 0; j < 8; j = j + 1) {
            fd->entries[i].intervals[j] = 0;
        }
    }
}

void fd_advance_time(struct FailureDetector *fd, int delta) {
    fd->current_time = fd->current_time + delta;
}

void fd_receive_heartbeat(struct FailureDetector *fd, int node_id) {
    int interval;
    int idx;
    struct HeartbeatEntry *e;

    e = &fd->entries[node_id];
    interval = fd->current_time - e->last_heartbeat;
    e->last_heartbeat = fd->current_time;

    if (e->interval_count > 0) {
        idx = e->interval_count % 8;
        if (idx >= 8) idx = 7;
        e->intervals[idx] = interval;
    }
    e->interval_sum = e->interval_sum + interval;
    e->interval_count = e->interval_count + 1;
    e->last_interval = interval;
}

int fd_get_avg_interval(struct FailureDetector *fd, int node_id) {
    struct HeartbeatEntry *e;
    e = &fd->entries[node_id];
    if (e->interval_count == 0) return fd->timeout_threshold;
    return e->interval_sum / e->interval_count;
}

// Returns suspicion level: 0=alive, 1=suspect, 2=dead
int fd_check_node(struct FailureDetector *fd, int node_id) {
    int elapsed;
    int avg;
    struct HeartbeatEntry *e;

    e = &fd->entries[node_id];
    elapsed = fd->current_time - e->last_heartbeat;
    avg = fd_get_avg_interval(fd, node_id);

    if (elapsed <= avg + fd->timeout_threshold) {
        return 0;  // alive
    }
    if (elapsed <= fd->suspicion_threshold + avg) {
        return 1;  // suspect
    }
    return 2;  // dead
}

char *status_name(int status) {
    if (status == 0) return "alive";
    if (status == 1) return "suspect";
    if (status == 2) return "dead";
    return "unknown";
}

void fd_print_status(struct FailureDetector *fd) {
    int i;
    int status;
    printf("Time %d:", fd->current_time);
    for (i = 0; i < fd->num_nodes; i = i + 1) {
        status = fd_check_node(fd, i);
        printf(" N%d=%s", i, status_name(status));
    }
    printf("\n");
}

int fd_count_alive(struct FailureDetector *fd) {
    int i;
    int count;
    count = 0;
    for (i = 0; i < fd->num_nodes; i = i + 1) {
        if (fd_check_node(fd, i) == 0) {
            count = count + 1;
        }
    }
    return count;
}

int fd_count_dead(struct FailureDetector *fd) {
    int i;
    int count;
    count = 0;
    for (i = 0; i < fd->num_nodes; i = i + 1) {
        if (fd_check_node(fd, i) == 2) {
            count = count + 1;
        }
    }
    return count;
}

int main() {
    struct FailureDetector fd;
    int status;
    int i;
    int alive;
    int dead;

    fd_init(&fd, 4, 10);

    // All nodes send initial heartbeat
    for (i = 0; i < 4; i = i + 1) {
        fd_receive_heartbeat(&fd, i);
    }
    printf("Initial heartbeats sent at time 0\n");
    // EXPECT: Initial heartbeats sent at time 0

    fd_print_status(&fd);
    // EXPECT: Time 0: N0=alive N1=alive N2=alive N3=alive

    // Time 5: all send heartbeat
    fd_advance_time(&fd, 5);
    for (i = 0; i < 4; i = i + 1) {
        fd_receive_heartbeat(&fd, i);
    }
    fd_print_status(&fd);
    // EXPECT: Time 5: N0=alive N1=alive N2=alive N3=alive

    // Time 10: all send except node 2
    fd_advance_time(&fd, 5);
    fd_receive_heartbeat(&fd, 0);
    fd_receive_heartbeat(&fd, 1);
    fd_receive_heartbeat(&fd, 3);
    fd_print_status(&fd);
    // EXPECT: Time 10: N0=alive N1=alive N2=alive N3=alive

    // Time 20: still no heartbeat from node 2
    fd_advance_time(&fd, 10);
    fd_receive_heartbeat(&fd, 0);
    fd_receive_heartbeat(&fd, 1);
    fd_receive_heartbeat(&fd, 3);

    status = fd_check_node(&fd, 2);
    printf("Node 2 status at time 20: %s\n", status_name(status));
    // EXPECT: Node 2 status at time 20: suspect

    // Time 30: node 2 still missing
    fd_advance_time(&fd, 10);
    fd_receive_heartbeat(&fd, 0);
    fd_receive_heartbeat(&fd, 1);
    fd_receive_heartbeat(&fd, 3);

    status = fd_check_node(&fd, 2);
    printf("Node 2 status at time 30: %s\n", status_name(status));
    // EXPECT: Node 2 status at time 30: dead

    fd_print_status(&fd);
    // EXPECT: Time 30: N0=alive N1=alive N2=dead N3=alive

    alive = fd_count_alive(&fd);
    dead = fd_count_dead(&fd);
    printf("Alive: %d, Dead: %d\n", alive, dead);
    // EXPECT: Alive: 3, Dead: 1

    // Node 2 recovers
    printf("--- Node 2 recovers ---\n");
    // EXPECT: --- Node 2 recovers ---
    fd_receive_heartbeat(&fd, 2);
    status = fd_check_node(&fd, 2);
    printf("Node 2 after recovery: %s\n", status_name(status));
    // EXPECT: Node 2 after recovery: alive

    // Check average intervals
    printf("Avg interval N0: %d\n", fd_get_avg_interval(&fd, 0));
    // EXPECT: Avg interval N0: 6
    printf("Avg interval N2: %d\n", fd_get_avg_interval(&fd, 2));
    // EXPECT: Avg interval N2: 10

    fd_print_status(&fd);
    // EXPECT: Time 30: N0=alive N1=alive N2=alive N3=alive

    printf("Failure detector simulation done\n");
    // EXPECT: Failure detector simulation done

    return 0;
}
