int printf(const char *fmt, ...);

// Gossip Protocol Simulation
// Simulates epidemic-style information dissemination

struct NodeInfo {
    int id;
    int is_alive;
    int knows_data[8];    // which data items this node knows
    int data_values[8];   // values of known data
    int generation[8];    // generation/version of each data item
};

void node_init(struct NodeInfo *n, int id) {
    int i;
    n->id = id;
    n->is_alive = 1;
    for (i = 0; i < 8; i = i + 1) {
        n->knows_data[i] = 0;
        n->data_values[i] = 0;
        n->generation[i] = 0;
    }
}

void node_set_data(struct NodeInfo *n, int key, int value, int gen) {
    n->knows_data[key] = 1;
    n->data_values[key] = value;
    n->generation[key] = gen;
}

// Gossip from src to dst: dst learns what src knows
int gossip_exchange(struct NodeInfo *src, struct NodeInfo *dst) {
    int i;
    int updates;
    updates = 0;

    if (src->is_alive == 0 || dst->is_alive == 0) {
        return 0;
    }

    for (i = 0; i < 8; i = i + 1) {
        if (src->knows_data[i]) {
            if (dst->knows_data[i] == 0 || src->generation[i] > dst->generation[i]) {
                dst->knows_data[i] = 1;
                dst->data_values[i] = src->data_values[i];
                dst->generation[i] = src->generation[i];
                updates = updates + 1;
            }
        }
    }
    return updates;
}

int count_informed(struct NodeInfo *nodes, int n, int key) {
    int i;
    int count;
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        if (nodes[i].knows_data[key] && nodes[i].is_alive) {
            count = count + 1;
        }
    }
    return count;
}

// Simple deterministic gossip round: each node talks to next neighbor
void gossip_round(struct NodeInfo *nodes, int n) {
    int i;
    int target;
    for (i = 0; i < n; i = i + 1) {
        if (nodes[i].is_alive == 0) continue;
        target = (i + 1) % n;
        if (nodes[target].is_alive == 0) {
            target = (i + 2) % n;
        }
        gossip_exchange(&nodes[i], &nodes[target]);
    }
}

// Random-ish gossip: each node talks to a "random" partner
void gossip_round_spread(struct NodeInfo *nodes, int n, int seed) {
    int i;
    int target;
    for (i = 0; i < n; i = i + 1) {
        if (nodes[i].is_alive == 0) continue;
        target = (i * seed + 3) % n;
        if (target == i) target = (target + 1) % n;
        if (nodes[target].is_alive == 0) {
            target = (target + 1) % n;
        }
        gossip_exchange(&nodes[i], &nodes[target]);
    }
}

void print_knowledge(struct NodeInfo *nodes, int n, int key) {
    int i;
    printf("Key %d known by:", key);
    for (i = 0; i < n; i = i + 1) {
        if (nodes[i].knows_data[key]) {
            printf(" N%d", i);
        }
    }
    printf("\n");
}

int main() {
    struct NodeInfo nodes[6];
    int i;
    int informed;
    int updates;
    int rounds;

    for (i = 0; i < 6; i = i + 1) {
        node_init(&nodes[i], i);
    }

    // Node 0 has data item 0
    node_set_data(&nodes[0], 0, 42, 1);
    printf("Initial: Node 0 has key 0 = 42\n");
    // EXPECT: Initial: Node 0 has key 0 = 42

    informed = count_informed(nodes, 6, 0);
    printf("Informed nodes: %d\n", informed);
    // EXPECT: Informed nodes: 1

    // Round 1
    gossip_round(nodes, 6);
    informed = count_informed(nodes, 6, 0);
    printf("After round 1: %d informed\n", informed);
    // EXPECT: After round 1: 6 informed

    // Round 2
    gossip_round(nodes, 6);
    informed = count_informed(nodes, 6, 0);
    printf("After round 2: %d informed\n", informed);
    // EXPECT: After round 2: 6 informed

    // Round 3
    gossip_round(nodes, 6);
    informed = count_informed(nodes, 6, 0);
    printf("After round 3: %d informed\n", informed);
    // EXPECT: After round 3: 6 informed

    // More rounds to converge
    gossip_round(nodes, 6);
    gossip_round(nodes, 6);
    informed = count_informed(nodes, 6, 0);
    printf("After round 5: %d informed\n", informed);
    // EXPECT: After round 5: 6 informed

    print_knowledge(nodes, 6, 0);
    // EXPECT: Key 0 known by: N0 N1 N2 N3 N4 N5

    // Test version/generation update
    printf("--- Version update test ---\n");
    // EXPECT: --- Version update test ---
    node_set_data(&nodes[0], 0, 99, 2);
    gossip_round(nodes, 6);
    gossip_round(nodes, 6);
    gossip_round(nodes, 6);
    gossip_round(nodes, 6);
    gossip_round(nodes, 6);

    printf("Node 3 value for key 0: %d\n", nodes[3].data_values[0]);
    // EXPECT: Node 3 value for key 0: 99
    printf("Node 5 value for key 0: %d\n", nodes[5].data_values[0]);
    // EXPECT: Node 5 value for key 0: 99

    // Test with a dead node
    printf("--- Failure test ---\n");
    // EXPECT: --- Failure test ---

    // Reset
    for (i = 0; i < 6; i = i + 1) {
        node_init(&nodes[i], i);
    }
    nodes[2].is_alive = 0;
    node_set_data(&nodes[0], 1, 77, 1);

    for (i = 0; i < 6; i = i + 1) {
        gossip_round(nodes, 6);
    }
    informed = count_informed(nodes, 6, 1);
    printf("Informed with node 2 down: %d\n", informed);
    // EXPECT: Informed with node 2 down: 5

    printf("Node 2 knows key 1: %d\n", nodes[2].knows_data[1]);
    // EXPECT: Node 2 knows key 1: 0

    printf("Gossip protocol simulation done\n");
    // EXPECT: Gossip protocol simulation done

    return 0;
}
