int printf(const char *fmt, ...);

/* Heavy-Light Decomposition (Simplified)
   On a rooted tree, decompose into heavy and light chains.
   Support path queries (sum of node values along path). */

int parent_arr[16];
int depth[16];
int subtree_size[16];
int heavy_child[16];   /* heavy child of each node, -1 if leaf */
int chain_head[16];    /* head of chain containing node */
int chain_pos[16];     /* position in chain */
int values[16];        /* node values */

/* Adjacency list (tree) */
int adj[16][4];       /* adj[u] = list of neighbors */
int adj_count[16];

int n_nodes;

void add_edge(int u, int v) {
    adj[u][adj_count[u]] = v;
    adj_count[u] = adj_count[u] + 1;
    adj[v][adj_count[v]] = u;
    adj_count[v] = adj_count[v] + 1;
}

/* DFS to compute parent, depth, subtree size, heavy child */
/* Stack-based DFS to avoid deep recursion */
int stack[32];
int stack_state[32]; /* 0 = first visit, 1 = post-visit */
int stack_child_idx[32]; /* which child to process next */
int sp;

void compute_sizes(int root) {
    int u;
    int v;
    int i;
    int max_size;
    int state;

    sp = 0;
    stack[sp] = root;
    stack_state[sp] = 0;
    stack_child_idx[sp] = 0;
    parent_arr[root] = -1;
    depth[root] = 0;
    sp = 1;

    while (sp > 0) {
        sp = sp - 1;
        u = stack[sp];
        state = stack_state[sp];

        if (state == 0) {
            /* First visit: push post-visit, then children */
            subtree_size[u] = 1;
            stack[sp] = u;
            stack_state[sp] = 1;
            sp = sp + 1;

            for (i = adj_count[u] - 1; i >= 0; i--) {
                v = adj[u][i];
                if (v != parent_arr[u]) {
                    parent_arr[v] = u;
                    depth[v] = depth[u] + 1;
                    stack[sp] = v;
                    stack_state[sp] = 0;
                    sp = sp + 1;
                }
            }
        } else {
            /* Post-visit: compute subtree size and heavy child */
            max_size = 0;
            heavy_child[u] = -1;

            for (i = 0; i < adj_count[u]; i++) {
                v = adj[u][i];
                if (v != parent_arr[u]) {
                    subtree_size[u] = subtree_size[u] + subtree_size[v];
                    if (subtree_size[v] > max_size) {
                        max_size = subtree_size[v];
                        heavy_child[u] = v;
                    }
                }
            }
        }
    }
}

void decompose(int root) {
    int u;
    int v;
    int i;
    int pos;

    /* BFS-like traversal to assign chains */
    /* Process heavy paths first */
    sp = 0;
    stack[sp] = root;
    sp = 1;
    chain_head[root] = root;
    pos = 0;

    while (sp > 0) {
        sp = sp - 1;
        u = stack[sp];
        chain_pos[u] = pos;
        pos = pos + 1;

        /* Push light children first (processed later), heavy child last (processed next) */
        for (i = 0; i < adj_count[u]; i++) {
            v = adj[u][i];
            if (v != parent_arr[u] && v != heavy_child[u]) {
                chain_head[v] = v;  /* light child starts new chain */
                stack[sp] = v;
                sp = sp + 1;
            }
        }

        if (heavy_child[u] >= 0) {
            v = heavy_child[u];
            chain_head[v] = chain_head[u]; /* same chain as parent */
            stack[sp] = v;
            sp = sp + 1;
        }
    }
}

/* Path sum query using HLD: sum values on path from u to v */
int path_sum(int u, int v) {
    int sum = 0;

    while (chain_head[u] != chain_head[v]) {
        /* Move the deeper chain head up */
        if (depth[chain_head[u]] < depth[chain_head[v]]) {
            /* Swap u and v */
            int tmp = u;
            u = v;
            v = tmp;
        }
        /* Sum from u up to chain_head[u] */
        int cur = u;
        while (cur != parent_arr[chain_head[u]]) {
            sum = sum + values[cur];
            if (cur == chain_head[u]) break;
            cur = parent_arr[cur];
        }
        u = parent_arr[chain_head[u]];
    }

    /* Now u and v are on the same chain. Sum between them. */
    if (depth[u] > depth[v]) {
        int tmp = u;
        u = v;
        v = tmp;
    }
    /* u is higher (or equal), v is lower */
    while (v != u) {
        sum = sum + values[v];
        v = parent_arr[v];
    }
    sum = sum + values[u];

    return sum;
}

/*
   Tree structure (rooted at 0):
        0 (val=1)
       / \
      1    2 (val=2, val=3)
     / \     \
    3   4     5 (val=4, val=5, val=6)
   /
  6 (val=7)
*/

void build_tree(void) {
    int i;
    n_nodes = 7;

    for (i = 0; i < 16; i++) {
        adj_count[i] = 0;
        parent_arr[i] = -1;
    }

    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(1, 3);
    add_edge(1, 4);
    add_edge(2, 5);
    add_edge(3, 6);

    values[0] = 1;
    values[1] = 2;
    values[2] = 3;
    values[3] = 4;
    values[4] = 5;
    values[5] = 6;
    values[6] = 7;
}

void test_tree_properties(void) {
    printf("=== Tree Properties ===\n");
    // EXPECT: === Tree Properties ===

    printf("Size[0]: %d\n", subtree_size[0]);
    // EXPECT: Size[0]: 7
    printf("Size[1]: %d\n", subtree_size[1]);
    // EXPECT: Size[1]: 4
    printf("Size[2]: %d\n", subtree_size[2]);
    // EXPECT: Size[2]: 2
    printf("Size[3]: %d\n", subtree_size[3]);
    // EXPECT: Size[3]: 2
    printf("Size[6]: %d\n", subtree_size[6]);
    // EXPECT: Size[6]: 1

    printf("Depth[0]: %d\n", depth[0]);
    // EXPECT: Depth[0]: 0
    printf("Depth[6]: %d\n", depth[6]);
    // EXPECT: Depth[6]: 3

    /* Heavy children: 0->1 (size 4 vs 2), 1->3 (size 2 vs 1), 3->6, 2->5 */
    printf("Heavy[0]: %d\n", heavy_child[0]);
    // EXPECT: Heavy[0]: 1
    printf("Heavy[1]: %d\n", heavy_child[1]);
    // EXPECT: Heavy[1]: 3
    printf("Heavy[2]: %d\n", heavy_child[2]);
    // EXPECT: Heavy[2]: 5
    printf("Heavy[3]: %d\n", heavy_child[3]);
    // EXPECT: Heavy[3]: 6
}

void test_chains(void) {
    printf("=== Chain Heads ===\n");
    // EXPECT: === Chain Heads ===
    /* Heavy chain from 0: 0->1->3->6 */
    printf("ChainHead[0]: %d\n", chain_head[0]);
    // EXPECT: ChainHead[0]: 0
    printf("ChainHead[1]: %d\n", chain_head[1]);
    // EXPECT: ChainHead[1]: 0
    printf("ChainHead[3]: %d\n", chain_head[3]);
    // EXPECT: ChainHead[3]: 0
    printf("ChainHead[6]: %d\n", chain_head[6]);
    // EXPECT: ChainHead[6]: 0
    /* Light edges start new chains: 4, 2, 5 */
    printf("ChainHead[4]: %d\n", chain_head[4]);
    // EXPECT: ChainHead[4]: 4
    printf("ChainHead[2]: %d\n", chain_head[2]);
    // EXPECT: ChainHead[2]: 2
    printf("ChainHead[5]: %d\n", chain_head[5]);
    // EXPECT: ChainHead[5]: 2
}

void test_path_queries(void) {
    int s;

    printf("=== Path Queries ===\n");
    // EXPECT: === Path Queries ===

    /* Path 6 to 0: 7+4+2+1 = 14 */
    s = path_sum(6, 0);
    printf("Path 6->0: %d\n", s);
    // EXPECT: Path 6->0: 14

    /* Path 6 to 4: 7+4+2+5 = 18 */
    s = path_sum(6, 4);
    printf("Path 6->4: %d\n", s);
    // EXPECT: Path 6->4: 18

    /* Path 4 to 5: 5+2+1+3+6 = 17 */
    s = path_sum(4, 5);
    printf("Path 4->5: %d\n", s);
    // EXPECT: Path 4->5: 17

    /* Path 6 to 5: 7+4+2+1+3+6 = 23 */
    s = path_sum(6, 5);
    printf("Path 6->5: %d\n", s);
    // EXPECT: Path 6->5: 23

    /* Path 0 to 0: just node 0 = 1 */
    s = path_sum(0, 0);
    printf("Path 0->0: %d\n", s);
    // EXPECT: Path 0->0: 1

    /* Path 3 to 1: 4+2 = 6... wait 3->1: nodes 3,1 => 4+2 = 6 */
    s = path_sum(3, 1);
    printf("Path 3->1: %d\n", s);
    // EXPECT: Path 3->1: 6
}

int main(void) {
    build_tree();
    compute_sizes(0);
    decompose(0);

    test_tree_properties();
    test_chains();
    test_path_queries();

    printf("All HLD tests passed!\n");
    // EXPECT: All HLD tests passed!
    return 0;
}
