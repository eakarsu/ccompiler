int printf(const char *fmt, ...);

// Register allocation using simplified graph coloring
// Models interference graph and attempts coloring with K registers

struct IntfGraph {
    int adj[16][16];    // adjacency matrix (interference)
    int degree[16];     // degree of each node
    int color[16];      // assigned color (-1 = uncolored)
    int spilled[16];    // 1 if spilled to stack
    int active[16];     // 1 if node is in graph
    int num_nodes;
};

void graph_init(struct IntfGraph *g, int n) {
    g->num_nodes = n;
    int i, j;
    for (i = 0; i < 16; i++) {
        g->degree[i] = 0;
        g->color[i] = -1;
        g->spilled[i] = 0;
        g->active[i] = (i < n) ? 1 : 0;
        for (j = 0; j < 16; j++) {
            g->adj[i][j] = 0;
        }
    }
}

void add_interference(struct IntfGraph *g, int a, int b) {
    if (a == b) return;
    if (!g->adj[a][b]) {
        g->adj[a][b] = 1;
        g->adj[b][a] = 1;
        g->degree[a] = g->degree[a] + 1;
        g->degree[b] = g->degree[b] + 1;
    }
}

int compute_degree(struct IntfGraph *g, int node) {
    int d = 0;
    int i;
    for (i = 0; i < g->num_nodes; i++) {
        if (g->adj[node][i] && g->active[i]) {
            d = d + 1;
        }
    }
    return d;
}

// Find node with minimum degree that is < k
int find_simplify_candidate(struct IntfGraph *g, int k) {
    int best = -1;
    int best_deg = 9999;
    int i;
    for (i = 0; i < g->num_nodes; i++) {
        if (!g->active[i]) continue;
        int d = compute_degree(g, i);
        if (d < k && d < best_deg) {
            best = i;
            best_deg = d;
        }
    }
    return best;
}

// Find node with max degree (for spilling)
int find_spill_candidate(struct IntfGraph *g) {
    int best = -1;
    int best_deg = -1;
    int i;
    for (i = 0; i < g->num_nodes; i++) {
        if (!g->active[i]) continue;
        int d = compute_degree(g, i);
        if (d > best_deg) {
            best = i;
            best_deg = d;
        }
    }
    return best;
}

// Try to color a node
int try_color(struct IntfGraph *g, int node, int k) {
    int used[16];
    int i;
    for (i = 0; i < 16; i++) used[i] = 0;

    // Mark colors used by neighbors
    for (i = 0; i < g->num_nodes; i++) {
        if (g->adj[node][i] && g->color[i] >= 0) {
            used[g->color[i]] = 1;
        }
    }

    // Find first available color
    for (i = 0; i < k; i++) {
        if (!used[i]) {
            g->color[node] = i;
            return 1;
        }
    }
    return 0;
}

// Simplified graph coloring allocator
int allocate_registers(struct IntfGraph *g, int k) {
    int stack[16];
    int stack_top = 0;
    int spill_count = 0;
    int i;

    // Simplify phase: push nodes onto stack
    while (1) {
        int active_count = 0;
        for (i = 0; i < g->num_nodes; i++) {
            if (g->active[i]) active_count = active_count + 1;
        }
        if (active_count == 0) break;

        int node = find_simplify_candidate(g, k);
        if (node >= 0) {
            // Simplify: remove low-degree node
            g->active[node] = 0;
            stack[stack_top] = node;
            stack_top = stack_top + 1;
        } else {
            // Potential spill: remove highest-degree node
            node = find_spill_candidate(g);
            if (node >= 0) {
                g->active[node] = 0;
                stack[stack_top] = node;
                stack_top = stack_top + 1;
            } else {
                break;
            }
        }
    }

    // Select phase: pop and color
    while (stack_top > 0) {
        stack_top = stack_top - 1;
        int node = stack[stack_top];
        g->active[node] = 1;

        if (!try_color(g, node, k)) {
            g->spilled[node] = 1;
            g->color[node] = -1;
            spill_count = spill_count + 1;
        }
    }

    return spill_count;
}

int count_colors_used(struct IntfGraph *g) {
    int used[16];
    int i;
    for (i = 0; i < 16; i++) used[i] = 0;
    for (i = 0; i < g->num_nodes; i++) {
        if (g->color[i] >= 0) used[g->color[i]] = 1;
    }
    int c = 0;
    for (i = 0; i < 16; i++) {
        if (used[i]) c = c + 1;
    }
    return c;
}

int verify_coloring(struct IntfGraph *g) {
    int i, j;
    for (i = 0; i < g->num_nodes; i++) {
        if (g->spilled[i]) continue;
        for (j = i + 1; j < g->num_nodes; j++) {
            if (g->spilled[j]) continue;
            if (g->adj[i][j] && g->color[i] == g->color[j]) {
                return 0; // conflict!
            }
        }
    }
    return 1;
}

int main() {
    struct IntfGraph g;

    // Test 1: Triangle graph (3 nodes, all interfere), 3 colors
    printf("=== RegAlloc Test 1: Triangle, k=3 ===\n");
    // EXPECT: === RegAlloc Test 1: Triangle, k=3 ===
    graph_init(&g, 3);
    add_interference(&g, 0, 1);
    add_interference(&g, 1, 2);
    add_interference(&g, 0, 2);

    int spills = allocate_registers(&g, 3);
    printf("spills: %d\n", spills);
    // EXPECT: spills: 0
    printf("valid: %d\n", verify_coloring(&g));
    // EXPECT: valid: 1
    printf("colors used: %d\n", count_colors_used(&g));
    // EXPECT: colors used: 3

    // Test 2: Triangle graph, only 2 colors -> spill
    printf("=== RegAlloc Test 2: Triangle, k=2 ===\n");
    // EXPECT: === RegAlloc Test 2: Triangle, k=2 ===
    graph_init(&g, 3);
    add_interference(&g, 0, 1);
    add_interference(&g, 1, 2);
    add_interference(&g, 0, 2);

    spills = allocate_registers(&g, 2);
    printf("spills: %d\n", spills);
    // EXPECT: spills: 1

    // Test 3: Linear chain (0-1-2-3), 2 colors enough
    printf("=== RegAlloc Test 3: Chain, k=2 ===\n");
    // EXPECT: === RegAlloc Test 3: Chain, k=2 ===
    graph_init(&g, 4);
    add_interference(&g, 0, 1);
    add_interference(&g, 1, 2);
    add_interference(&g, 2, 3);

    spills = allocate_registers(&g, 2);
    printf("spills: %d\n", spills);
    // EXPECT: spills: 0
    printf("valid: %d\n", verify_coloring(&g));
    // EXPECT: valid: 1
    printf("colors used: %d\n", count_colors_used(&g));
    // EXPECT: colors used: 2

    // Test 4: Star graph (node 0 interferes with all others)
    printf("=== RegAlloc Test 4: Star, k=4 ===\n");
    // EXPECT: === RegAlloc Test 4: Star, k=4 ===
    graph_init(&g, 5);
    add_interference(&g, 0, 1);
    add_interference(&g, 0, 2);
    add_interference(&g, 0, 3);
    add_interference(&g, 0, 4);

    spills = allocate_registers(&g, 4);
    printf("spills: %d\n", spills);
    // EXPECT: spills: 0
    printf("valid: %d\n", verify_coloring(&g));
    // EXPECT: valid: 1
    // Node 0 needs unique color; others can share since they don't interfere
    printf("colors used: %d\n", count_colors_used(&g));
    // EXPECT: colors used: 2

    // Test 5: Complete graph K5, k=5
    printf("=== RegAlloc Test 5: K5, k=5 ===\n");
    // EXPECT: === RegAlloc Test 5: K5, k=5 ===
    graph_init(&g, 5);
    int i, j;
    for (i = 0; i < 5; i++) {
        for (j = i + 1; j < 5; j++) {
            add_interference(&g, i, j);
        }
    }

    spills = allocate_registers(&g, 5);
    printf("spills: %d\n", spills);
    // EXPECT: spills: 0
    printf("valid: %d\n", verify_coloring(&g));
    // EXPECT: valid: 1
    printf("colors used: %d\n", count_colors_used(&g));
    // EXPECT: colors used: 5

    // Test 6: Complete K5, only 3 colors -> spills
    printf("=== RegAlloc Test 6: K5, k=3 ===\n");
    // EXPECT: === RegAlloc Test 6: K5, k=3 ===
    graph_init(&g, 5);
    for (i = 0; i < 5; i++) {
        for (j = i + 1; j < 5; j++) {
            add_interference(&g, i, j);
        }
    }

    spills = allocate_registers(&g, 3);
    printf("spills: %d\n", spills);
    // EXPECT: spills: 2

    // Test 7: No interferences (all get color 0)
    printf("=== RegAlloc Test 7: No interference ===\n");
    // EXPECT: === RegAlloc Test 7: No interference ===
    graph_init(&g, 4);
    spills = allocate_registers(&g, 1);
    printf("spills: %d\n", spills);
    // EXPECT: spills: 0
    printf("colors used: %d\n", count_colors_used(&g));
    // EXPECT: colors used: 1
    printf("valid: %d\n", verify_coloring(&g));
    // EXPECT: valid: 1

    printf("done\n");
    // EXPECT: done
    return 0;
}
