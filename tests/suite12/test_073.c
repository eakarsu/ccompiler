int printf(const char *fmt, ...);

// Simplified Raft Leader Election
// Simulates term-based leader election with voting

struct RaftNode {
    int id;
    int current_term;
    int voted_for;       // -1 means no vote
    int state;           // 0=follower, 1=candidate, 2=leader
    int votes_received;
    int is_alive;
    int log_length;
};

void init_node(struct RaftNode *n, int id) {
    n->id = id;
    n->current_term = 0;
    n->voted_for = -1;
    n->state = 0;
    n->votes_received = 0;
    n->is_alive = 1;
    n->log_length = 0;
}

char *state_name(int state) {
    if (state == 0) return "follower";
    if (state == 1) return "candidate";
    if (state == 2) return "leader";
    return "unknown";
}

void start_election(struct RaftNode *node) {
    node->current_term = node->current_term + 1;
    node->state = 1;
    node->voted_for = node->id;
    node->votes_received = 1;
}

int request_vote(struct RaftNode *voter, int candidate_id, int candidate_term, int candidate_log_len) {
    if (voter->is_alive == 0) {
        return 0;
    }
    if (candidate_term < voter->current_term) {
        return 0;
    }
    if (candidate_term > voter->current_term) {
        voter->current_term = candidate_term;
        voter->voted_for = -1;
        voter->state = 0;
    }
    if (voter->voted_for == -1 || voter->voted_for == candidate_id) {
        if (candidate_log_len >= voter->log_length) {
            voter->voted_for = candidate_id;
            return 1;
        }
    }
    return 0;
}

int check_majority(int votes, int total) {
    return votes > (total / 2);
}

void become_leader(struct RaftNode *node) {
    node->state = 2;
}

void run_election(struct RaftNode *nodes, int n, int candidate_idx) {
    int i;
    int vote;
    struct RaftNode *cand;
    cand = &nodes[candidate_idx];

    start_election(cand);
    printf("Node %d starts election, term %d\n", cand->id, cand->current_term);

    for (i = 0; i < n; i = i + 1) {
        if (i == candidate_idx) {
            continue;
        }
        vote = request_vote(&nodes[i], cand->id, cand->current_term, cand->log_length);
        if (vote) {
            cand->votes_received = cand->votes_received + 1;
            printf("Node %d votes for Node %d\n", nodes[i].id, cand->id);
        } else {
            printf("Node %d rejects Node %d\n", nodes[i].id, cand->id);
        }
    }

    if (check_majority(cand->votes_received, n)) {
        become_leader(cand);
        printf("Node %d becomes leader with %d votes\n", cand->id, cand->votes_received);
    } else {
        printf("Node %d fails election with %d votes\n", cand->id, cand->votes_received);
    }
}

int main() {
    struct RaftNode nodes[5];
    int i;

    for (i = 0; i < 5; i = i + 1) {
        init_node(&nodes[i], i);
    }

    // Election 1: Node 0 runs, all alive
    printf("=== Election 1 ===\n");
    // EXPECT: === Election 1 ===
    run_election(nodes, 5, 0);
    // EXPECT: Node 0 starts election, term 1
    // EXPECT: Node 1 votes for Node 0
    // EXPECT: Node 2 votes for Node 0
    // EXPECT: Node 3 votes for Node 0
    // EXPECT: Node 4 votes for Node 0
    // EXPECT: Node 0 becomes leader with 5 votes
    printf("Node 0 state: %s\n", state_name(nodes[0].state));
    // EXPECT: Node 0 state: leader

    // Reset for next election
    for (i = 0; i < 5; i = i + 1) {
        init_node(&nodes[i], i);
    }

    // Election 2: Node 2 runs, nodes 3 and 4 are down
    printf("=== Election 2 ===\n");
    // EXPECT: === Election 2 ===
    nodes[3].is_alive = 0;
    nodes[4].is_alive = 0;
    run_election(nodes, 5, 2);
    // EXPECT: Node 2 starts election, term 1
    // EXPECT: Node 0 votes for Node 2
    // EXPECT: Node 1 votes for Node 2
    // EXPECT: Node 3 rejects Node 2
    // EXPECT: Node 4 rejects Node 2
    // EXPECT: Node 2 becomes leader with 3 votes

    // Reset for competing elections
    for (i = 0; i < 5; i = i + 1) {
        init_node(&nodes[i], i);
    }

    // Election 3: Node 1 runs first, then Node 3 tries
    printf("=== Election 3 ===\n");
    // EXPECT: === Election 3 ===
    run_election(nodes, 5, 1);
    // EXPECT: Node 1 starts election, term 1
    // EXPECT: Node 0 votes for Node 1
    // EXPECT: Node 2 votes for Node 1
    // EXPECT: Node 3 votes for Node 1
    // EXPECT: Node 4 votes for Node 1
    // EXPECT: Node 1 becomes leader with 5 votes

    // Node 3 tries with same term - should fail (already voted)
    printf("=== Election 4 ===\n");
    // EXPECT: === Election 4 ===
    run_election(nodes, 5, 3);
    // EXPECT: Node 3 starts election, term 2
    // EXPECT: Node 0 votes for Node 3
    // EXPECT: Node 1 votes for Node 3
    // EXPECT: Node 2 votes for Node 3
    // EXPECT: Node 4 votes for Node 3
    // EXPECT: Node 3 becomes leader with 5 votes

    printf("Term of node 3: %d\n", nodes[3].current_term);
    // EXPECT: Term of node 3: 2
    printf("Raft election simulation done\n");
    // EXPECT: Raft election simulation done

    return 0;
}
