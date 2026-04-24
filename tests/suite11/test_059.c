int printf(const char *fmt, ...);

// Fluid flow in a pipe network (integer-based)
// Flow = (P_in - P_out) * conductance / 1000

struct Pipe {
    int from_node;
    int to_node;
    int conductance;
    int flow;
};

struct Node {
    int pressure;
    int is_fixed;
    int flow_sum;
};

void init_node(struct Node *n, int pressure, int fixed) {
    n->pressure = pressure;
    n->is_fixed = fixed;
    n->flow_sum = 0;
}

void init_pipe(struct Pipe *p, int from, int to, int cond) {
    p->from_node = from;
    p->to_node = to;
    p->conductance = cond;
    p->flow = 0;
}

int compute_flow(struct Pipe *p, struct Node *nodes) {
    int dp = nodes[p->from_node].pressure - nodes[p->to_node].pressure;
    return dp * p->conductance / 1000;
}

void update_flows(struct Pipe *pipes, int npipes, struct Node *nodes) {
    int i;
    for (i = 0; i < npipes; i = i + 1) {
        pipes[i].flow = compute_flow(&pipes[i], nodes);
    }
}

void compute_node_flows(struct Pipe *pipes, int npipes, struct Node *nodes, int nnodes) {
    int i;
    for (i = 0; i < nnodes; i = i + 1) {
        nodes[i].flow_sum = 0;
    }
    for (i = 0; i < npipes; i = i + 1) {
        nodes[pipes[i].from_node].flow_sum = nodes[pipes[i].from_node].flow_sum - pipes[i].flow;
        nodes[pipes[i].to_node].flow_sum = nodes[pipes[i].to_node].flow_sum + pipes[i].flow;
    }
}

void solve_iteration(struct Pipe *pipes, int npipes, struct Node *nodes, int nnodes) {
    int i;
    int j;

    update_flows(pipes, npipes, nodes);
    compute_node_flows(pipes, npipes, nodes, nnodes);

    for (i = 0; i < nnodes; i = i + 1) {
        if (nodes[i].is_fixed) continue;

        int total_cond = 0;
        int weighted_p = 0;
        for (j = 0; j < npipes; j = j + 1) {
            if (pipes[j].from_node == i) {
                total_cond = total_cond + pipes[j].conductance;
                weighted_p = weighted_p + nodes[pipes[j].to_node].pressure * pipes[j].conductance / 1000;
            }
            if (pipes[j].to_node == i) {
                total_cond = total_cond + pipes[j].conductance;
                weighted_p = weighted_p + nodes[pipes[j].from_node].pressure * pipes[j].conductance / 1000;
            }
        }
        if (total_cond > 0) {
            nodes[i].pressure = weighted_p * 1000 / total_cond;
        }
    }
}

int main() {
    struct Node nodes[6];
    struct Pipe pipes[8];
    int i;
    int iter;

    printf("=== Pipe Network Flow ===\n");
    // EXPECT: === Pipe Network Flow ===

    // Test 1: Simple two-node network
    init_node(&nodes[0], 1000, 1);
    init_node(&nodes[1], 0, 1);
    init_pipe(&pipes[0], 0, 1, 500);

    update_flows(pipes, 1, nodes);
    printf("Simple pipe flow: %d\n", pipes[0].flow);
    // EXPECT: Simple pipe flow: 500

    // Test 2: Series pipes
    printf("--- Series ---\n");
    // EXPECT: --- Series ---
    init_node(&nodes[0], 1000, 1);
    init_node(&nodes[1], 500, 0);
    init_node(&nodes[2], 0, 1);
    init_pipe(&pipes[0], 0, 1, 400);
    init_pipe(&pipes[1], 1, 2, 400);

    for (iter = 0; iter < 20; iter = iter + 1) {
        solve_iteration(pipes, 2, nodes, 3);
    }

    printf("Node pressures: %d %d %d\n",
           nodes[0].pressure, nodes[1].pressure, nodes[2].pressure);
    // EXPECT: Node pressures: 1000 500 0
    printf("Pipe flows: %d %d\n", pipes[0].flow, pipes[1].flow);
    // EXPECT: Pipe flows: 200 200

    // Test 3: Parallel pipes
    printf("--- Parallel ---\n");
    // EXPECT: --- Parallel ---
    init_node(&nodes[0], 1000, 1);
    init_node(&nodes[1], 0, 1);
    init_pipe(&pipes[0], 0, 1, 300);
    init_pipe(&pipes[1], 0, 1, 500);

    update_flows(pipes, 2, nodes);
    int total = pipes[0].flow + pipes[1].flow;
    printf("Parallel flow: pipe1=%d pipe2=%d total=%d\n",
           pipes[0].flow, pipes[1].flow, total);
    // EXPECT: Parallel flow: pipe1=300 pipe2=500 total=800

    // Test 4: T-junction network
    printf("--- T-Junction ---\n");
    // EXPECT: --- T-Junction ---
    init_node(&nodes[0], 1000, 1);
    init_node(&nodes[1], 500, 0);
    init_node(&nodes[2], 0, 1);
    init_node(&nodes[3], 200, 1);

    init_pipe(&pipes[0], 0, 1, 600);
    init_pipe(&pipes[1], 1, 2, 400);
    init_pipe(&pipes[2], 1, 3, 300);

    for (iter = 0; iter < 30; iter = iter + 1) {
        solve_iteration(pipes, 3, nodes, 4);
    }

    printf("Junction pressure: %d\n", nodes[1].pressure);
    // EXPECT: Junction pressure: 507
    printf("Flows: p0=%d p1=%d p2=%d\n",
           pipes[0].flow, pipes[1].flow, pipes[2].flow);
    // EXPECT: Flows: p0=295 p1=202 p2=92

    // Test 5: Diamond network (symmetric)
    printf("--- Diamond ---\n");
    // EXPECT: --- Diamond ---
    init_node(&nodes[0], 1000, 1);
    init_node(&nodes[1], 500, 0);
    init_node(&nodes[2], 500, 0);
    init_node(&nodes[3], 0, 1);

    init_pipe(&pipes[0], 0, 1, 500);
    init_pipe(&pipes[1], 0, 2, 500);
    init_pipe(&pipes[2], 1, 3, 500);
    init_pipe(&pipes[3], 2, 3, 500);

    for (iter = 0; iter < 30; iter = iter + 1) {
        solve_iteration(pipes, 4, nodes, 4);
    }

    printf("Upper node: %d\n", nodes[1].pressure);
    // EXPECT: Upper node: 500
    printf("Lower node: %d\n", nodes[2].pressure);
    // EXPECT: Lower node: 500
    printf("Flows: %d %d %d %d\n",
           pipes[0].flow, pipes[1].flow, pipes[2].flow, pipes[3].flow);
    // EXPECT: Flows: 250 250 250 250

    // Test 6: Flow conservation check
    printf("--- Conservation ---\n");
    // EXPECT: --- Conservation ---
    compute_node_flows(pipes, 4, nodes, 4);
    for (i = 0; i < 4; i = i + 1) {
        printf("Node %d: net flow=%d\n", i, nodes[i].flow_sum);
    }
    // EXPECT: Node 0: net flow=-500
    // EXPECT: Node 1: net flow=0
    // EXPECT: Node 2: net flow=0
    // EXPECT: Node 3: net flow=500

    // Test 7: Asymmetric diamond
    printf("--- Asymmetric ---\n");
    // EXPECT: --- Asymmetric ---
    init_node(&nodes[0], 1000, 1);
    init_node(&nodes[1], 500, 0);
    init_node(&nodes[2], 500, 0);
    init_node(&nodes[3], 0, 1);

    init_pipe(&pipes[0], 0, 1, 800);
    init_pipe(&pipes[1], 0, 2, 200);
    init_pipe(&pipes[2], 1, 3, 800);
    init_pipe(&pipes[3], 2, 3, 200);

    for (iter = 0; iter < 30; iter = iter + 1) {
        solve_iteration(pipes, 4, nodes, 4);
    }

    printf("Asym upper: %d lower: %d\n", nodes[1].pressure, nodes[2].pressure);
    // EXPECT: Asym upper: 500 lower: 500
    printf("Asym flows: %d %d %d %d\n",
           pipes[0].flow, pipes[1].flow, pipes[2].flow, pipes[3].flow);
    // EXPECT: Asym flows: 400 100 400 100

    printf("Pipe network done\n");
    // EXPECT: Pipe network done

    return 0;
}
