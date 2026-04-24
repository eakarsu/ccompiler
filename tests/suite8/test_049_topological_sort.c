int printf(const char *fmt, ...);

/* Topological sort on a directed acyclic graph (DAG)
   Using Kahn's algorithm (BFS-based) */

int adj[10][10];
int nv;

void init_graph(int n) {
    int i; int j;
    nv = n;
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++)
            adj[i][j] = 0;
}

void add_edge(int u, int v) {
    adj[u][v] = 1;   /* directed: u -> v */
}

int in_degree[10];
int topo_order[10];
int topo_cnt;

int queue[20];
int qf; int qb;

void compute_in_degrees(void) {
    int i; int j;
    for (i = 0; i < nv; i++) {
        in_degree[i] = 0;
        for (j = 0; j < nv; j++) {
            if (adj[j][i]) {
                in_degree[i] = in_degree[i] + 1;
            }
        }
    }
}

int topological_sort(void) {
    int i; int cur;

    compute_in_degrees();
    qf = 0; qb = 0;
    topo_cnt = 0;

    /* Enqueue all vertices with in-degree 0 */
    for (i = 0; i < nv; i++) {
        if (in_degree[i] == 0) {
            queue[qb] = i;
            qb = qb + 1;
        }
    }

    while (qf < qb) {
        cur = queue[qf];
        qf = qf + 1;
        topo_order[topo_cnt] = cur;
        topo_cnt = topo_cnt + 1;

        for (i = 0; i < nv; i++) {
            if (adj[cur][i]) {
                in_degree[i] = in_degree[i] - 1;
                if (in_degree[i] == 0) {
                    queue[qb] = i;
                    qb = qb + 1;
                }
            }
        }
    }

    /* If topo_cnt != nv, there is a cycle */
    return topo_cnt == nv;
}

/* Verify a topological order is valid */
int verify_topo(void) {
    int pos[10];
    int i; int j;

    for (i = 0; i < topo_cnt; i++) {
        pos[topo_order[i]] = i;
    }

    for (i = 0; i < nv; i++) {
        for (j = 0; j < nv; j++) {
            if (adj[i][j]) {
                if (pos[i] >= pos[j]) return 0;
            }
        }
    }
    return 1;
}

/* DFS-based topological sort */
int vis_dfs[10];
int dfs_stack[10];
int dfs_sp;

void dfs_topo(int u) {
    int i;
    vis_dfs[u] = 1;
    for (i = 0; i < nv; i++) {
        if (adj[u][i] && vis_dfs[i] == 0) {
            dfs_topo(i);
        }
    }
    dfs_stack[dfs_sp] = u;
    dfs_sp = dfs_sp + 1;
}

void dfs_topological_sort(void) {
    int i;
    for (i = 0; i < nv; i++) vis_dfs[i] = 0;
    dfs_sp = 0;
    for (i = 0; i < nv; i++) {
        if (vis_dfs[i] == 0) {
            dfs_topo(i);
        }
    }
}

int main(void) {
    int ok;
    int i;

    /*  DAG representing course prerequisites:
        0 -> 1 -> 3
        0 -> 2 -> 3
        2 -> 4
        3 -> 5
        4 -> 5
    */
    init_graph(6);
    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(1, 3);
    add_edge(2, 3);
    add_edge(2, 4);
    add_edge(3, 5);
    add_edge(4, 5);

    ok = topological_sort();
    // EXPECT: topo_sort_ok=1
    printf("topo_sort_ok=%d\n", ok);

    // EXPECT: kahn order: 0 1 2 3 4 5
    printf("kahn order:");
    for (i = 0; i < topo_cnt; i++) {
        printf(" %d", topo_order[i]);
    }
    printf("\n");

    // EXPECT: valid=1
    printf("valid=%d\n", verify_topo());

    /* DFS-based topological sort */
    dfs_topological_sort();

    // EXPECT: dfs order: 0 2 4 1 3 5
    printf("dfs order:");
    for (i = dfs_sp - 1; i >= 0; i--) {
        printf(" %d", dfs_stack[i]);
    }
    printf("\n");

    /* Test with a simple chain: 0->1->2->3 */
    init_graph(4);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);

    ok = topological_sort();
    // EXPECT: chain_ok=1
    printf("chain_ok=%d\n", ok);
    // EXPECT: chain: 0 1 2 3
    printf("chain:");
    for (i = 0; i < topo_cnt; i++) {
        printf(" %d", topo_order[i]);
    }
    printf("\n");

    /* Test with a cycle: 0->1->2->0 */
    init_graph(3);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 0);

    ok = topological_sort();
    // EXPECT: cycle_ok=0
    printf("cycle_ok=%d\n", ok);

    return 0;
}
