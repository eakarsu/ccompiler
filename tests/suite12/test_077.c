int printf(const char *fmt, ...);

// Log Replication
// Simulates a replicated log across nodes (like Raft log replication)

struct LogEntry {
    int term;
    int command;
    int committed;
};

struct ReplicaNode {
    int id;
    int is_alive;
    struct LogEntry log[16];
    int log_len;
    int commit_index;
    int applied_index;
};

void replica_init(struct ReplicaNode *n, int id) {
    int i;
    n->id = id;
    n->is_alive = 1;
    n->log_len = 0;
    n->commit_index = -1;
    n->applied_index = -1;
    for (i = 0; i < 16; i = i + 1) {
        n->log[i].term = 0;
        n->log[i].command = 0;
        n->log[i].committed = 0;
    }
}

int append_entry(struct ReplicaNode *n, int term, int cmd) {
    int idx;
    if (n->log_len >= 16) return -1;
    idx = n->log_len;
    n->log[idx].term = term;
    n->log[idx].command = cmd;
    n->log[idx].committed = 0;
    n->log_len = n->log_len + 1;
    return idx;
}

// Replicate from leader to follower
int replicate_entry(struct ReplicaNode *leader, struct ReplicaNode *follower, int idx) {
    int f_idx;
    if (follower->is_alive == 0) return 0;
    if (idx >= leader->log_len) return 0;

    // Check if follower already has this entry
    if (idx < follower->log_len) {
        if (follower->log[idx].term == leader->log[idx].term) {
            return 1;  // already has it
        }
        // Conflict: truncate follower log
        follower->log_len = idx;
    }

    f_idx = append_entry(follower, leader->log[idx].term, leader->log[idx].command);
    if (f_idx >= 0) return 1;
    return 0;
}

int replicate_all_entries(struct ReplicaNode *leader, struct ReplicaNode *follower) {
    int i;
    int ok;
    int count;
    count = 0;
    for (i = 0; i < leader->log_len; i = i + 1) {
        ok = replicate_entry(leader, follower, i);
        if (ok) count = count + 1;
    }
    return count;
}

// Count how many nodes have entry at index
int count_replicated(struct ReplicaNode *nodes, int n, int idx) {
    int i;
    int count;
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        if (nodes[i].log_len > idx && nodes[i].is_alive) {
            count = count + 1;
        }
    }
    return count;
}

void try_commit(struct ReplicaNode *leader, struct ReplicaNode *nodes, int n) {
    int i;
    int replicas;
    int majority;
    majority = n / 2 + 1;

    for (i = leader->commit_index + 1; i < leader->log_len; i = i + 1) {
        replicas = count_replicated(nodes, n, i);
        if (replicas >= majority) {
            leader->commit_index = i;
            leader->log[i].committed = 1;
        }
    }
}

void apply_committed(struct ReplicaNode *n) {
    while (n->applied_index < n->commit_index) {
        n->applied_index = n->applied_index + 1;
        printf("Node %d applies cmd %d (term %d)\n",
               n->id,
               n->log[n->applied_index].command,
               n->log[n->applied_index].term);
    }
}

void print_log(struct ReplicaNode *n) {
    int i;
    printf("Node %d log (len=%d, commit=%d):", n->id, n->log_len, n->commit_index);
    for (i = 0; i < n->log_len; i = i + 1) {
        printf(" [t%d:c%d]", n->log[i].term, n->log[i].command);
    }
    printf("\n");
}

int main() {
    struct ReplicaNode nodes[5];
    int i;
    int rep;

    for (i = 0; i < 5; i = i + 1) {
        replica_init(&nodes[i], i);
    }

    // Node 0 is leader
    // Append entries to leader
    append_entry(&nodes[0], 1, 10);
    append_entry(&nodes[0], 1, 20);
    append_entry(&nodes[0], 1, 30);
    print_log(&nodes[0]);
    // EXPECT: Node 0 log (len=3, commit=-1): [t1:c10] [t1:c20] [t1:c30]

    // Replicate to followers
    for (i = 1; i < 5; i = i + 1) {
        rep = replicate_all_entries(&nodes[0], &nodes[i]);
        printf("Replicated %d entries to Node %d\n", rep, i);
    }
    // EXPECT: Replicated 3 entries to Node 1
    // EXPECT: Replicated 3 entries to Node 2
    // EXPECT: Replicated 3 entries to Node 3
    // EXPECT: Replicated 3 entries to Node 4

    // Try commit
    try_commit(&nodes[0], nodes, 5);
    printf("Leader commit index: %d\n", nodes[0].commit_index);
    // EXPECT: Leader commit index: 2

    // Apply on leader
    apply_committed(&nodes[0]);
    // EXPECT: Node 0 applies cmd 10 (term 1)
    // EXPECT: Node 0 applies cmd 20 (term 1)
    // EXPECT: Node 0 applies cmd 30 (term 1)

    // Propagate commit index to followers and apply
    for (i = 1; i < 5; i = i + 1) {
        nodes[i].commit_index = nodes[0].commit_index;
    }
    apply_committed(&nodes[2]);
    // EXPECT: Node 2 applies cmd 10 (term 1)
    // EXPECT: Node 2 applies cmd 20 (term 1)
    // EXPECT: Node 2 applies cmd 30 (term 1)

    // New term: add more entries, node 3 is down
    printf("--- Node 3 goes down ---\n");
    // EXPECT: --- Node 3 goes down ---
    nodes[3].is_alive = 0;

    append_entry(&nodes[0], 2, 40);
    append_entry(&nodes[0], 2, 50);

    for (i = 1; i < 5; i = i + 1) {
        rep = replicate_all_entries(&nodes[0], &nodes[i]);
    }

    try_commit(&nodes[0], nodes, 5);
    printf("Leader commit index after term 2: %d\n", nodes[0].commit_index);
    // EXPECT: Leader commit index after term 2: 4

    print_log(&nodes[0]);
    // EXPECT: Node 0 log (len=5, commit=4): [t1:c10] [t1:c20] [t1:c30] [t2:c40] [t2:c50]
    print_log(&nodes[3]);
    // EXPECT: Node 3 log (len=3, commit=2): [t1:c10] [t1:c20] [t1:c30]

    // Node 3 comes back, catch up
    printf("--- Node 3 recovers ---\n");
    // EXPECT: --- Node 3 recovers ---
    nodes[3].is_alive = 1;
    rep = replicate_all_entries(&nodes[0], &nodes[3]);
    printf("Caught up %d entries to Node 3\n", rep);
    // EXPECT: Caught up 5 entries to Node 3

    print_log(&nodes[3]);
    // EXPECT: Node 3 log (len=5, commit=2): [t1:c10] [t1:c20] [t1:c30] [t2:c40] [t2:c50]

    printf("Log replication done\n");
    // EXPECT: Log replication done

    return 0;
}
