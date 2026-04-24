// stress_076_graph_torture.c - Graph algorithm stress test
// EXPECT: edge_count=13
// EXPECT: bfs_visited_count=5
// EXPECT: bfs_dist_0_to_3=2
// EXPECT: bfs_dist_0_to_4=1
// EXPECT: bfs_unreachable=-1
// EXPECT: dfs_visited_count=5
// EXPECT: dfs_start=0
// EXPECT: num_components=3
// EXPECT: same_comp_0_4=1
// EXPECT: same_comp_5_8=1
// EXPECT: diff_comp_0_5=1
// EXPECT: shortest_0_3=2
// EXPECT: shortest_0_4=1
// EXPECT: topo_count=8
// EXPECT: topo_valid=1
// EXPECT: topo_last=7

int printf(const char *fmt, ...);

// Adjacency matrix representation for 12 nodes
int adj[12][12];
int num_nodes;

void graph_init(int n) {
    num_nodes = n;
    int i;
    int j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            adj[i][j] = 0;
}

void add_edge(int u, int v) {
    adj[u][v] = 1;
    adj[v][u] = 1;
}

void add_directed_edge(int u, int v) {
    adj[u][v] = 1;
}

int count_edges_undirected(void) {
    int count = 0;
    int i;
    int j;
    for (i = 0; i < num_nodes; i++)
        for (j = i + 1; j < num_nodes; j++)
            if (adj[i][j]) count++;
    return count;
}

// BFS
int bfs_visited[12];
int bfs_dist[12];
int bfs_order[12];
int bfs_count;

void bfs(int start) {
    int queue[12];
    int front = 0;
    int back = 0;
    int i;
    for (i = 0; i < num_nodes; i++) {
        bfs_visited[i] = 0;
        bfs_dist[i] = -1;
    }
    bfs_count = 0;
    bfs_visited[start] = 1;
    bfs_dist[start] = 0;
    queue[back++] = start;
    while (front < back) {
        int u = queue[front++];
        bfs_order[bfs_count++] = u;
        for (i = 0; i < num_nodes; i++) {
            if (adj[u][i] && !bfs_visited[i]) {
                bfs_visited[i] = 1;
                bfs_dist[i] = bfs_dist[u] + 1;
                queue[back++] = i;
            }
        }
    }
}

// DFS
int dfs_visited[12];
int dfs_order[12];
int dfs_count;

void dfs_helper(int u) {
    dfs_visited[u] = 1;
    dfs_order[dfs_count++] = u;
    int i;
    for (i = 0; i < num_nodes; i++) {
        if (adj[u][i] && !dfs_visited[i]) {
            dfs_helper(i);
        }
    }
}

void dfs(int start) {
    int i;
    for (i = 0; i < num_nodes; i++) dfs_visited[i] = 0;
    dfs_count = 0;
    dfs_helper(start);
}

// Connected components
int comp_id[12];

int find_components(void) {
    int i;
    for (i = 0; i < num_nodes; i++) {
        dfs_visited[i] = 0;
        comp_id[i] = -1;
    }
    int num_comp = 0;
    for (i = 0; i < num_nodes; i++) {
        if (!dfs_visited[i]) {
            // BFS to mark component
            int queue[12];
            int front = 0;
            int back = 0;
            dfs_visited[i] = 1;
            comp_id[i] = num_comp;
            queue[back++] = i;
            while (front < back) {
                int u = queue[front++];
                int j;
                for (j = 0; j < num_nodes; j++) {
                    if (adj[u][j] && !dfs_visited[j]) {
                        dfs_visited[j] = 1;
                        comp_id[j] = num_comp;
                        queue[back++] = j;
                    }
                }
            }
            num_comp++;
        }
    }
    return num_comp;
}

// Topological sort (for DAG) using Kahn's algorithm
// Uses a separate directed graph
int dag[12][12];
int dag_nodes;

void dag_init(int n) {
    dag_nodes = n;
    int i;
    int j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            dag[i][j] = 0;
}

void dag_add_edge(int u, int v) {
    dag[u][v] = 1;
}

int topo_order[12];

int topo_sort(void) {
    int in_degree[12];
    int i;
    int j;
    for (i = 0; i < dag_nodes; i++) {
        in_degree[i] = 0;
        for (j = 0; j < dag_nodes; j++) {
            if (dag[j][i]) in_degree[i]++;
        }
    }
    int queue[12];
    int front = 0;
    int back = 0;
    for (i = 0; i < dag_nodes; i++) {
        if (in_degree[i] == 0) queue[back++] = i;
    }
    int count = 0;
    while (front < back) {
        int u = queue[front++];
        topo_order[count++] = u;
        for (i = 0; i < dag_nodes; i++) {
            if (dag[u][i]) {
                in_degree[i]--;
                if (in_degree[i] == 0) queue[back++] = i;
            }
        }
    }
    return count; // If < dag_nodes, cycle exists
}

// Verify topological order
int verify_topo(int count) {
    int pos[12];
    int i;
    int j;
    for (i = 0; i < count; i++) pos[topo_order[i]] = i;
    for (i = 0; i < dag_nodes; i++) {
        for (j = 0; j < dag_nodes; j++) {
            if (dag[i][j] && pos[i] >= pos[j]) return 0;
        }
    }
    return 1;
}

// Shortest path (BFS-based, unweighted)
int shortest_path(int src, int dst) {
    bfs(src);
    return bfs_dist[dst];
}

int main(void) {
    // Build an undirected graph with 12 nodes
    // 3 connected components:
    // Component 0: 0-1-2-3-4 (ring + extra edges)
    // Component 1: 5-6-7-8 (line)
    // Component 2: 9-10-11 (triangle)
    graph_init(12);
    // Component 0: ring 0-1-2-3-4-0 plus 0-2, 1-3
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(3, 4);
    add_edge(4, 0);
    add_edge(0, 2);
    add_edge(1, 3);
    // Component 1: 5-6-7-8
    add_edge(5, 6);
    add_edge(6, 7);
    add_edge(7, 8);
    // Component 2: 9-10-11 triangle
    add_edge(9, 10);
    add_edge(10, 11);
    add_edge(11, 9);

    printf("edge_count=%d\n", count_edges_undirected());

    // BFS from node 0
    bfs(0);
    printf("bfs_visited_count=%d\n", bfs_count);
    printf("bfs_dist_0_to_3=%d\n", bfs_dist[3]);
    printf("bfs_dist_0_to_4=%d\n", bfs_dist[4]);
    printf("bfs_unreachable=%d\n", bfs_dist[5]); // -1 (different component)

    // DFS from node 0
    dfs(0);
    printf("dfs_visited_count=%d\n", dfs_count);
    printf("dfs_start=%d\n", dfs_order[0]);

    // Connected components
    int ncomp = find_components();
    printf("num_components=%d\n", ncomp);
    printf("same_comp_0_4=%d\n", comp_id[0] == comp_id[4]);
    printf("same_comp_5_8=%d\n", comp_id[5] == comp_id[8]);
    printf("diff_comp_0_5=%d\n", comp_id[0] != comp_id[5]);

    // Shortest path
    printf("shortest_0_3=%d\n", shortest_path(0, 3));
    printf("shortest_0_4=%d\n", shortest_path(0, 4));

    // Topological sort on a DAG (8 nodes)
    dag_init(8);
    // Typical course prerequisite DAG
    dag_add_edge(0, 2);
    dag_add_edge(0, 3);
    dag_add_edge(1, 3);
    dag_add_edge(1, 4);
    dag_add_edge(2, 5);
    dag_add_edge(3, 5);
    dag_add_edge(3, 6);
    dag_add_edge(4, 6);
    dag_add_edge(5, 7);
    dag_add_edge(6, 7);

    int topo_count = topo_sort();
    printf("topo_count=%d\n", topo_count);
    printf("topo_valid=%d\n", verify_topo(topo_count));
    printf("topo_last=%d\n", topo_order[topo_count - 1]);

    return 0;
}
