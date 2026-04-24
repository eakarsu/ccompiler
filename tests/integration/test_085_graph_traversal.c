int printf(const char *fmt, ...);
// EXPECT: 0 1 1 2 4 0 1 3 5 3 1 1 0 1 3 3 0 1 2 2 5 4 0 1 0 3 2 0 0 1 1 0 5 2 2 3 2 0 2 2 3 2 3 4 1 4 1 3 10 1

// Graph algorithms: BFS, DFS, shortest path, bipartite check, bridge detection

// Adjacency matrix representation
int adj[20][20];
int visited[20];
int dist[20];
int color[20];
int parent[20];
int disc[20];
int low[20];
int timer_val;
int queue[100];
int qfront;
int qback;
int n_vertices;
int bridge_count;

// Initialize graph
void graph_init(int n) {
    int i;
    int j;
    n_vertices = n;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            adj[i][j] = 0;
        }
        visited[i] = 0;
        dist[i] = -1;
        color[i] = -1;
        parent[i] = -1;
        disc[i] = -1;
        low[i] = -1;
    }
    timer_val = 0;
    bridge_count = 0;
}

// Add undirected edge
void add_edge(int u, int v) {
    adj[u][v] = 1;
    adj[v][u] = 1;
}

// Add directed edge
void add_directed_edge(int u, int v) {
    adj[u][v] = 1;
}

// Queue operations
void queue_init(void) {
    qfront = 0;
    qback = 0;
}

void enqueue(int v) {
    queue[qback] = v;
    qback = qback + 1;
}

int dequeue(void) {
    int v;
    v = queue[qfront];
    qfront = qfront + 1;
    return v;
}

int queue_empty(void) {
    if (qfront == qback) return 1;
    return 0;
}

// BFS from source, store distances
void bfs(int src, int n) {
    int i;
    int u;
    int v;
    for (i = 0; i < n; i = i + 1) {
        visited[i] = 0;
        dist[i] = -1;
    }
    queue_init();
    visited[src] = 1;
    dist[src] = 0;
    enqueue(src);
    while (queue_empty() == 0) {
        u = dequeue();
        for (v = 0; v < n; v = v + 1) {
            if (adj[u][v] && visited[v] == 0) {
                visited[v] = 1;
                dist[v] = dist[u] + 1;
                parent[v] = u;
                enqueue(v);
            }
        }
    }
}

// DFS order stored in result array
int dfs_order[20];
int dfs_count;

void dfs_visit(int u, int n) {
    int v;
    visited[u] = 1;
    dfs_order[dfs_count] = u;
    dfs_count = dfs_count + 1;
    for (v = 0; v < n; v = v + 1) {
        if (adj[u][v] && visited[v] == 0) {
            dfs_visit(v, n);
        }
    }
}

void dfs(int src, int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        visited[i] = 0;
    }
    dfs_count = 0;
    dfs_visit(src, n);
}

// Count connected components
int count_components(int n) {
    int i;
    int count;
    count = 0;
    for (i = 0; i < n; i = i + 1) {
        visited[i] = 0;
    }
    for (i = 0; i < n; i = i + 1) {
        if (visited[i] == 0) {
            dfs_count = 0;
            dfs_visit(i, n);
            count = count + 1;
        }
    }
    return count;
}

// Bipartite check using BFS
int is_bipartite(int n) {
    int i;
    int u;
    int v;
    for (i = 0; i < n; i = i + 1) {
        color[i] = -1;
    }
    for (i = 0; i < n; i = i + 1) {
        if (color[i] == -1) {
            queue_init();
            color[i] = 0;
            enqueue(i);
            while (queue_empty() == 0) {
                u = dequeue();
                for (v = 0; v < n; v = v + 1) {
                    if (adj[u][v]) {
                        if (color[v] == -1) {
                            color[v] = 1 - color[u];
                            enqueue(v);
                        } else if (color[v] == color[u]) {
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return 1;
}

// Bridge detection using DFS
void bridge_dfs(int u, int par, int n) {
    int v;
    disc[u] = timer_val;
    low[u] = timer_val;
    timer_val = timer_val + 1;
    for (v = 0; v < n; v = v + 1) {
        if (adj[u][v]) {
            if (disc[v] == -1) {
                bridge_dfs(v, u, n);
                if (low[v] < low[u]) low[u] = low[v];
                if (low[v] > disc[u]) {
                    bridge_count = bridge_count + 1;
                }
            } else if (v != par) {
                if (disc[v] < low[u]) low[u] = disc[v];
            }
        }
    }
}

int find_bridges(int n) {
    int i;
    bridge_count = 0;
    timer_val = 0;
    for (i = 0; i < n; i = i + 1) {
        disc[i] = -1;
        low[i] = -1;
    }
    for (i = 0; i < n; i = i + 1) {
        if (disc[i] == -1) {
            bridge_dfs(i, -1, n);
        }
    }
    return bridge_count;
}

// Topological sort (for DAG) using DFS
int topo_stack[20];
int topo_sp;

void topo_dfs(int u, int n) {
    int v;
    visited[u] = 1;
    for (v = 0; v < n; v = v + 1) {
        if (adj[u][v] && visited[v] == 0) {
            topo_dfs(v, n);
        }
    }
    topo_stack[topo_sp] = u;
    topo_sp = topo_sp + 1;
}

void topological_sort(int n) {
    int i;
    topo_sp = 0;
    for (i = 0; i < n; i = i + 1) {
        visited[i] = 0;
    }
    for (i = 0; i < n; i = i + 1) {
        if (visited[i] == 0) {
            topo_dfs(i, n);
        }
    }
}

// Check if path exists from u to v
int path_exists(int u, int v, int n) {
    bfs(u, n);
    if (dist[v] != -1) return 1;
    return 0;
}

// Degree of vertex
int degree(int u, int n) {
    int v;
    int d;
    d = 0;
    for (v = 0; v < n; v = v + 1) {
        if (adj[u][v]) d = d + 1;
    }
    return d;
}

// Check if graph has a cycle (undirected) using DFS
int has_cycle_dfs(int u, int par, int n) {
    int v;
    visited[u] = 1;
    for (v = 0; v < n; v = v + 1) {
        if (adj[u][v]) {
            if (visited[v] == 0) {
                if (has_cycle_dfs(v, u, n)) return 1;
            } else if (v != par) {
                return 1;
            }
        }
    }
    return 0;
}

int has_cycle(int n) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        visited[i] = 0;
    }
    for (i = 0; i < n; i = i + 1) {
        if (visited[i] == 0) {
            if (has_cycle_dfs(i, -1, n)) return 1;
        }
    }
    return 0;
}

// Shortest path length (unweighted) from u to v
int shortest_path(int u, int v, int n) {
    bfs(u, n);
    return dist[v];
}

// Check if graph is complete
int is_complete(int n) {
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            if (i != j && adj[i][j] == 0) return 0;
        }
    }
    return 1;
}

// Count edges
int count_edges(int n) {
    int i;
    int j;
    int cnt;
    cnt = 0;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            if (adj[i][j]) cnt = cnt + 1;
        }
    }
    return cnt;
}

// Vertex coloring: greedy, return number of colors used
int vertex_colors[20];

int greedy_coloring(int n) {
    int i;
    int v;
    int c;
    int used[20];
    int max_color;
    max_color = 0;
    for (i = 0; i < n; i = i + 1) {
        vertex_colors[i] = -1;
    }
    vertex_colors[0] = 0;
    for (i = 1; i < n; i = i + 1) {
        for (c = 0; c < n; c = c + 1) {
            used[c] = 0;
        }
        for (v = 0; v < n; v = v + 1) {
            if (adj[i][v] && vertex_colors[v] != -1) {
                used[vertex_colors[v]] = 1;
            }
        }
        for (c = 0; c < n; c = c + 1) {
            if (used[c] == 0) {
                vertex_colors[i] = c;
                if (c > max_color) max_color = c;
                break;
            }
        }
    }
    return max_color + 1;
}

// BFS level of each node
int node_level[20];

void bfs_levels(int src, int n) {
    int i;
    int u;
    int v;
    for (i = 0; i < n; i = i + 1) {
        node_level[i] = -1;
    }
    queue_init();
    node_level[src] = 0;
    enqueue(src);
    while (queue_empty() == 0) {
        u = dequeue();
        for (v = 0; v < n; v = v + 1) {
            if (adj[u][v] && node_level[v] == -1) {
                node_level[v] = node_level[u] + 1;
                enqueue(v);
            }
        }
    }
}

int main(void) {
    int i;

    // Test 1-5: Simple graph BFS (0-1-2-3 path, plus 0-2)
    graph_init(6);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(0, 2);
    add_edge(3, 4);
    add_edge(4, 5);
    bfs(0, 6);
    printf("%d", dist[0]); // 0
    printf(" %d", dist[1]); // 1
    printf(" %d", dist[2]); // 1
    printf(" %d", dist[3]); // 2
    printf(" %d", dist[5]); // 4

    // Test 6-9: DFS order
    graph_init(5);
    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(1, 3);
    add_edge(1, 4);
    dfs(0, 5);
    printf(" %d", dfs_order[0]); // 0
    printf(" %d", dfs_order[1]); // 1
    printf(" %d", dfs_order[2]); // 3
    printf(" %d", dfs_count);    // 5

    // Test 10-11: Connected components
    graph_init(6);
    add_edge(0, 1);
    add_edge(2, 3);
    add_edge(4, 5);
    printf(" %d", count_components(6)); // 3

    graph_init(4);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    printf(" %d", count_components(4)); // 1

    // Test 12-14: Bipartite check
    graph_init(4);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    printf(" %d", is_bipartite(4)); // 1 (path is bipartite)

    graph_init(3);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 0);
    printf(" %d", is_bipartite(3)); // 0 (odd cycle)

    graph_init(4);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(3, 0);
    printf(" %d", is_bipartite(4)); // 1 (even cycle)

    // Test 15-17: Bridge detection
    // 0-1-2-3, 0-2: bridge is 2-3, 3-4, 4-5 (3 bridges? let me recheck)
    graph_init(6);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(0, 2); // 0,1,2 form a cycle
    add_edge(2, 3);
    add_edge(3, 4);
    add_edge(4, 5);
    printf(" %d", find_bridges(6)); // bridges: 2-3, 3-4, 4-5 = 3

    // Chain: all edges are bridges
    graph_init(4);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    printf(" %d", find_bridges(4)); // 3

    // Complete graph: no bridges
    graph_init(4);
    add_edge(0, 1); add_edge(0, 2); add_edge(0, 3);
    add_edge(1, 2); add_edge(1, 3); add_edge(2, 3);
    printf(" %d", find_bridges(4)); // 0

    // Test 18-20: Shortest path
    graph_init(5);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(3, 4);
    add_edge(0, 4);
    printf(" %d", shortest_path(0, 4, 5)); // 1
    printf(" %d", shortest_path(0, 3, 5)); // 2
    printf(" %d", shortest_path(1, 4, 5)); // 2

    // Test 21-23: Topological sort of DAG
    graph_init(6);
    add_directed_edge(5, 2);
    add_directed_edge(5, 0);
    add_directed_edge(4, 0);
    add_directed_edge(4, 1);
    add_directed_edge(2, 3);
    add_directed_edge(3, 1);
    topological_sort(6);
    // topo_stack is reversed: topo_stack[5]=first, topo_stack[0]=last
    printf(" %d", topo_stack[5]); // should be 4 or 5
    printf(" %d", topo_stack[4]); // next in order
    printf(" %d", topo_stack[0]); // last

    // Test 24-25: Path exists
    graph_init(5);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(3, 4);
    printf(" %d", path_exists(0, 2, 5)); // 1
    printf(" %d", path_exists(0, 4, 5)); // 0

    // Test 26-28: Degree
    graph_init(5);
    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(0, 3);
    add_edge(1, 2);
    printf(" %d", degree(0, 5)); // 3
    printf(" %d", degree(1, 5)); // 2
    printf(" %d", degree(4, 5)); // 0

    // Test 29-30: Cycle detection
    graph_init(4);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    printf(" %d", has_cycle(4)); // 0

    graph_init(4);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 0);
    printf(" %d", has_cycle(4)); // 1

    // Test 31-32: Complete graph check
    graph_init(3);
    add_edge(0, 1); add_edge(0, 2); add_edge(1, 2);
    printf(" %d", is_complete(3)); // 1

    graph_init(3);
    add_edge(0, 1);
    printf(" %d", is_complete(3)); // 0

    // Test 33-34: Count edges
    graph_init(4);
    add_edge(0, 1); add_edge(0, 2); add_edge(0, 3);
    add_edge(1, 2); add_edge(2, 3);
    printf(" %d", count_edges(4)); // 5

    graph_init(5);
    add_edge(0, 1); add_edge(2, 3);
    printf(" %d", count_edges(5)); // 2

    // Test 35-37: Greedy coloring
    graph_init(5);
    add_edge(0, 1); add_edge(1, 2); add_edge(2, 3); add_edge(3, 4);
    printf(" %d", greedy_coloring(5)); // 2 (path graph)

    graph_init(3);
    add_edge(0, 1); add_edge(1, 2); add_edge(2, 0);
    printf(" %d", greedy_coloring(3)); // 3 (triangle)

    graph_init(4);
    add_edge(0, 1); add_edge(2, 3);
    printf(" %d", greedy_coloring(4)); // 2

    // Test 38-40: BFS levels
    graph_init(7);
    add_edge(0, 1); add_edge(0, 2);
    add_edge(1, 3); add_edge(1, 4);
    add_edge(2, 5); add_edge(2, 6);
    bfs_levels(0, 7);
    printf(" %d", node_level[0]); // 0
    printf(" %d", node_level[3]); // 2
    printf(" %d", node_level[6]); // 2

    // Test 41-43: Multi-source shortest paths
    graph_init(6);
    add_edge(0, 1); add_edge(1, 2); add_edge(2, 3);
    add_edge(3, 4); add_edge(4, 5); add_edge(0, 5);
    printf(" %d", shortest_path(0, 3, 6)); // 1->2->3 = 3 (or 0->5->4->3=3), dist=3
    printf(" %d", shortest_path(1, 5, 6)); // 1->0->5=2
    printf(" %d", shortest_path(2, 5, 6)); // 2->1->0->5=3 or 2->3->4->5=3

    // Test 44-46: Star graph properties
    graph_init(5);
    add_edge(0, 1); add_edge(0, 2); add_edge(0, 3); add_edge(0, 4);
    printf(" %d", degree(0, 5)); // 4
    printf(" %d", is_bipartite(5)); // 1
    printf(" %d", find_bridges(5)); // 4

    // Test 47-48: Directed graph stuff
    graph_init(4);
    add_directed_edge(0, 1);
    add_directed_edge(1, 2);
    add_directed_edge(2, 3);
    printf(" %d", degree(0, 4)); // out-degree = 1 (only adj[0][1] set)
    bfs(0, 4);
    printf(" %d", dist[3]); // 3

    // Test 49-50: Dense graph
    graph_init(5);
    add_edge(0, 1); add_edge(0, 2); add_edge(0, 3); add_edge(0, 4);
    add_edge(1, 2); add_edge(1, 3); add_edge(1, 4);
    add_edge(2, 3); add_edge(2, 4);
    add_edge(3, 4);
    printf(" %d", count_edges(5)); // 10
    printf(" %d", is_complete(5)); // 1

    printf("\n");
    return 0;
}
