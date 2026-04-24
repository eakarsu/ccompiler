int printf(const char *fmt, ...);
// EXPECT: 1:1\n2:1\n3:1\n4:3\n5:2\n6:2\n7:3\n8:0\n9:1\n10:1\n11:1\n12:1\n13:0\n14:4\n15:1\n16:0\n17:4\n18:2\n19:5\n20:0\n21:4\n22:1\n23:5\n24:0\n25:0\n26:3\n27:1\n28:0\n29:1\n30:2\n31:-1\n32:-1\n33:6\n34:3\n35:1\n36:1\n37:1\n38:1\n39:3\n40:0\n41:5\n42:0\n43:4\n44:1\n45:-1\n46:5\n47:1\n48:5\n49:1\n50:5\n51:1\n52:4\n53:3\n54:0\n55:1\n56:6
// =============================================================================
// TEST 022: Graph Algorithms with Adjacency Matrix
// =============================================================================
// Tests graph representation using adjacency matrix, BFS, DFS, connected
// components, path finding, cycle detection, and topological analysis on
// small graphs (<=10 nodes).
// =============================================================================

// -----------------------------------------------------------------------------
// Graph storage: adjacency matrix and helpers
// -----------------------------------------------------------------------------
int adj[10][10];    // adjacency matrix
int num_nodes;      // current graph node count
int visited[10];    // visited array for traversals
int parent[10];     // parent tracking for path finding
int dist[10];       // distance array for BFS
int queue[100];     // queue for BFS
int q_front;
int q_back;
int dfs_order[10];  // DFS visit order
int dfs_idx;        // current index into dfs_order
int comp_id[10];    // component ID for each node

// -----------------------------------------------------------------------------
// Initialize graph with n nodes, no edges
// -----------------------------------------------------------------------------
void graph_init(int n) {
    int i;
    int j;
    num_nodes = n;
    for (i = 0; i < 10; i = i + 1) {
        for (j = 0; j < 10; j = j + 1) {
            adj[i][j] = 0;
        }
        visited[i] = 0;
        parent[i] = -1;
        dist[i] = -1;
        dfs_order[i] = -1;
        comp_id[i] = -1;
    }
    dfs_idx = 0;
    q_front = 0;
    q_back = 0;
}

// -----------------------------------------------------------------------------
// Add undirected edge
// -----------------------------------------------------------------------------
void add_edge(int u, int v) {
    adj[u][v] = 1;
    adj[v][u] = 1;
}

// -----------------------------------------------------------------------------
// Add directed edge
// -----------------------------------------------------------------------------
void add_directed_edge(int u, int v) {
    adj[u][v] = 1;
}

// -----------------------------------------------------------------------------
// Remove edge (undirected)
// -----------------------------------------------------------------------------
void remove_edge(int u, int v) {
    adj[u][v] = 0;
    adj[v][u] = 0;
}

// -----------------------------------------------------------------------------
// Check if edge exists
// -----------------------------------------------------------------------------
int has_edge(int u, int v) {
    return adj[u][v];
}

// -----------------------------------------------------------------------------
// Degree of a node (undirected)
// -----------------------------------------------------------------------------
int degree(int node) {
    int i;
    int d;
    d = 0;
    for (i = 0; i < num_nodes; i = i + 1) {
        if (adj[node][i]) d = d + 1;
    }
    return d;
}

// -----------------------------------------------------------------------------
// Count total edges (undirected graph: count/2)
// -----------------------------------------------------------------------------
int count_edges(void) {
    int i;
    int j;
    int c;
    c = 0;
    for (i = 0; i < num_nodes; i = i + 1) {
        for (j = 0; j < num_nodes; j = j + 1) {
            if (adj[i][j]) c = c + 1;
        }
    }
    return c / 2;
}

// -----------------------------------------------------------------------------
// Count directed edges
// -----------------------------------------------------------------------------
int count_directed_edges(void) {
    int i;
    int j;
    int c;
    c = 0;
    for (i = 0; i < num_nodes; i = i + 1) {
        for (j = 0; j < num_nodes; j = j + 1) {
            if (adj[i][j]) c = c + 1;
        }
    }
    return c;
}

// -----------------------------------------------------------------------------
// Reset visited array
// -----------------------------------------------------------------------------
void reset_visited(void) {
    int i;
    for (i = 0; i < 10; i = i + 1) {
        visited[i] = 0;
        parent[i] = -1;
        dist[i] = -1;
    }
    dfs_idx = 0;
}

// -----------------------------------------------------------------------------
// BFS from source node
// Returns number of nodes reachable
// -----------------------------------------------------------------------------
int bfs(int src) {
    int i;
    int curr;
    int count;
    q_front = 0;
    q_back = 0;
    queue[q_back] = src;
    q_back = q_back + 1;
    visited[src] = 1;
    dist[src] = 0;
    count = 0;

    while (q_front < q_back) {
        curr = queue[q_front];
        q_front = q_front + 1;
        count = count + 1;

        for (i = 0; i < num_nodes; i = i + 1) {
            if (adj[curr][i] && visited[i] == 0) {
                visited[i] = 1;
                parent[i] = curr;
                dist[i] = dist[curr] + 1;
                queue[q_back] = i;
                q_back = q_back + 1;
            }
        }
    }
    return count;
}

// -----------------------------------------------------------------------------
// DFS from a node using explicit stack
// Returns number of nodes visited
// -----------------------------------------------------------------------------
int stack[100];
int stack_top;

int dfs(int src) {
    int curr;
    int i;
    int count;
    stack_top = 0;
    stack[stack_top] = src;
    stack_top = stack_top + 1;
    count = 0;

    while (stack_top > 0) {
        stack_top = stack_top - 1;
        curr = stack[stack_top];
        if (visited[curr]) continue;
        visited[curr] = 1;
        dfs_order[dfs_idx] = curr;
        dfs_idx = dfs_idx + 1;
        count = count + 1;

        // Push neighbors in reverse order for consistent DFS order
        for (i = num_nodes - 1; i >= 0; i = i - 1) {
            if (adj[curr][i] && visited[i] == 0) {
                stack[stack_top] = i;
                stack_top = stack_top + 1;
            }
        }
    }
    return count;
}

// -----------------------------------------------------------------------------
// Find connected components (undirected graph)
// Returns number of components
// -----------------------------------------------------------------------------
int find_components(void) {
    int i;
    int num_comp;
    int j;
    int k;
    reset_visited();
    num_comp = 0;
    for (i = 0; i < num_nodes; i = i + 1) {
        if (visited[i] == 0) {
            q_front = 0;
            q_back = 0;
            queue[q_back] = i;
            q_back = q_back + 1;
            visited[i] = 1;
            comp_id[i] = num_comp;
            while (q_front < q_back) {
                j = queue[q_front];
                q_front = q_front + 1;
                for (k = 0; k < num_nodes; k = k + 1) {
                    if (adj[j][k] && visited[k] == 0) {
                        visited[k] = 1;
                        comp_id[k] = num_comp;
                        queue[q_back] = k;
                        q_back = q_back + 1;
                    }
                }
            }
            num_comp = num_comp + 1;
        }
    }
    return num_comp;
}

// -----------------------------------------------------------------------------
// Get shortest path length from BFS (must run bfs first)
// Returns -1 if unreachable
// -----------------------------------------------------------------------------
int shortest_path_len(int target) {
    return dist[target];
}

// -----------------------------------------------------------------------------
// Reconstruct path from src to target (after BFS)
// Returns path length, fills path_buf
// -----------------------------------------------------------------------------
int path_buf[10];

int reconstruct_path(int src, int target) {
    int path_len;
    int curr;
    int i;
    int tmp;
    if (dist[target] == -1) return 0;
    path_len = 0;
    curr = target;
    while (curr != -1) {
        path_buf[path_len] = curr;
        path_len = path_len + 1;
        if (curr == src) break;
        curr = parent[curr];
    }
    // Reverse path_buf
    for (i = 0; i < path_len / 2; i = i + 1) {
        tmp = path_buf[i];
        path_buf[i] = path_buf[path_len - 1 - i];
        path_buf[path_len - 1 - i] = tmp;
    }
    return path_len;
}

// -----------------------------------------------------------------------------
// Check if graph is connected
// -----------------------------------------------------------------------------
int is_connected(void) {
    int c;
    reset_visited();
    c = bfs(0);
    if (c == num_nodes) return 1;
    return 0;
}

// -----------------------------------------------------------------------------
// Check if undirected graph has a cycle
// -----------------------------------------------------------------------------
int has_cycle_undirected(void) {
    int edges;
    int components;
    edges = count_edges();
    components = find_components();
    if (edges > num_nodes - components) return 1;
    return 0;
}

// -----------------------------------------------------------------------------
// Check if directed graph has cycle using DFS coloring
// color: 0=white, 1=gray, 2=black
// -----------------------------------------------------------------------------
int dfs_color[10];

int dfs_cycle_check(int node) {
    int i;
    dfs_color[node] = 1;
    for (i = 0; i < num_nodes; i = i + 1) {
        if (adj[node][i]) {
            if (dfs_color[i] == 1) return 1;
            if (dfs_color[i] == 0) {
                if (dfs_cycle_check(i)) return 1;
            }
        }
    }
    dfs_color[node] = 2;
    return 0;
}

int has_cycle_directed(void) {
    int i;
    for (i = 0; i < 10; i = i + 1) dfs_color[i] = 0;
    for (i = 0; i < num_nodes; i = i + 1) {
        if (dfs_color[i] == 0) {
            if (dfs_cycle_check(i)) return 1;
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Topological sort using Kahn's algorithm
// Returns number of nodes in sorted order, or -1 if cycle
// -----------------------------------------------------------------------------
int topo_order[10];
int in_degree[10];

int topological_sort(void) {
    int i;
    int j;
    int count;
    int curr;
    for (i = 0; i < num_nodes; i = i + 1) {
        in_degree[i] = 0;
    }
    for (i = 0; i < num_nodes; i = i + 1) {
        for (j = 0; j < num_nodes; j = j + 1) {
            if (adj[i][j]) {
                in_degree[j] = in_degree[j] + 1;
            }
        }
    }
    q_front = 0;
    q_back = 0;
    for (i = 0; i < num_nodes; i = i + 1) {
        if (in_degree[i] == 0) {
            queue[q_back] = i;
            q_back = q_back + 1;
        }
    }
    count = 0;
    while (q_front < q_back) {
        curr = queue[q_front];
        q_front = q_front + 1;
        topo_order[count] = curr;
        count = count + 1;
        for (i = 0; i < num_nodes; i = i + 1) {
            if (adj[curr][i]) {
                in_degree[i] = in_degree[i] - 1;
                if (in_degree[i] == 0) {
                    queue[q_back] = i;
                    q_back = q_back + 1;
                }
            }
        }
    }
    if (count == num_nodes) return count;
    return -1;
}

// -----------------------------------------------------------------------------
// Count nodes at a given distance from source (after BFS)
// -----------------------------------------------------------------------------
int nodes_at_distance(int d) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < num_nodes; i = i + 1) {
        if (dist[i] == d) c = c + 1;
    }
    return c;
}

// -----------------------------------------------------------------------------
// Is graph bipartite? (2-colorable)
// Uses BFS coloring
// -----------------------------------------------------------------------------
int bip_color[10];

int is_bipartite(void) {
    int i;
    int curr;
    int j;
    for (i = 0; i < 10; i = i + 1) bip_color[i] = -1;
    bip_color[0] = 0;
    q_front = 0;
    q_back = 0;
    queue[q_back] = 0;
    q_back = q_back + 1;
    while (q_front < q_back) {
        curr = queue[q_front];
        q_front = q_front + 1;
        for (j = 0; j < num_nodes; j = j + 1) {
            if (adj[curr][j]) {
                if (bip_color[j] == -1) {
                    bip_color[j] = 1 - bip_color[curr];
                    queue[q_back] = j;
                    q_back = q_back + 1;
                }
                if (bip_color[j] == bip_color[curr]) return 0;
            }
        }
    }
    return 1;
}

// -----------------------------------------------------------------------------
// Sum of all degrees
// -----------------------------------------------------------------------------
int sum_degrees(void) {
    int i;
    int s;
    s = 0;
    for (i = 0; i < num_nodes; i = i + 1) {
        s = s + degree(i);
    }
    return s;
}

// =============================================================================
// Main test function
// =============================================================================
int main(void) {
    int test_num;
    int v;
    int plen;
    int i;

    test_num = 0;

    // ---- Graph 1: Simple triangle (0-1-2-0) ----
    graph_init(3);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 0);

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, has_edge(0, 1)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, has_edge(1, 0)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, has_edge(0, 2)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_edges()); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, degree(0)); // 2

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, degree(1)); // 2

    // BFS from 0
    reset_visited();
    v = bfs(0);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, v); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dist[0]); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dist[1]); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dist[2]); // 1

    // Connected?
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_connected()); // 1

    // Has cycle?
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, has_cycle_undirected()); // 1

    // Bipartite?
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_bipartite()); // 0

    // ---- Graph 2: Path graph 0-1-2-3-4 ----
    graph_init(5);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(3, 4);

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_edges()); // 4

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_connected()); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, has_cycle_undirected()); // 0

    // BFS from 0
    reset_visited();
    bfs(0);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dist[4]); // 4

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dist[2]); // 2

    // Path from 0 to 4
    plen = reconstruct_path(0, 4);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, plen); // 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, path_buf[0]); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, path_buf[4]); // 4

    // Bipartite?
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_bipartite()); // 1

    // DFS from 0
    reset_visited();
    v = dfs(0);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, v); // 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dfs_order[0]); // 0

    // ---- Graph 3: Disconnected graph ----
    graph_init(6);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(3, 4);
    // 5 isolated

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_connected()); // 0

    v = find_components();
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, v); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, comp_id[0] == comp_id[1] ? 1 : 0); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, comp_id[0] == comp_id[3] ? 1 : 0); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, comp_id[3] == comp_id[4] ? 1 : 0); // 1

    // BFS from 0
    reset_visited();
    bfs(0);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dist[2]); // 2

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dist[3]); // -1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dist[5]); // -1

    // ---- Graph 4: Complete graph K4 ----
    graph_init(4);
    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(0, 3);
    add_edge(1, 2);
    add_edge(1, 3);
    add_edge(2, 3);

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_edges()); // 6

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, degree(0)); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_connected()); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, has_cycle_undirected()); // 1

    // BFS from 0
    reset_visited();
    bfs(0);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dist[1]); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dist[3]); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, nodes_at_distance(1)); // 3

    // ---- Graph 5: Directed DAG for topological sort ----
    graph_init(5);
    add_directed_edge(0, 1);
    add_directed_edge(0, 2);
    add_directed_edge(1, 3);
    add_directed_edge(2, 3);
    add_directed_edge(3, 4);

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, has_cycle_directed()); // 0

    v = topological_sort();
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, v); // 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, topo_order[0]); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, topo_order[4]); // 4

    // ---- Graph 6: Directed graph WITH cycle ----
    graph_init(3);
    add_directed_edge(0, 1);
    add_directed_edge(1, 2);
    add_directed_edge(2, 0);

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, has_cycle_directed()); // 1

    v = topological_sort();
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, v); // -1

    // ---- Graph 7: Star graph ----
    graph_init(6);
    for (i = 1; i < 6; i = i + 1) {
        add_edge(0, i);
    }

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, degree(0)); // 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, degree(1)); // 1

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_edges()); // 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_bipartite()); // 1

    reset_visited();
    bfs(0);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, nodes_at_distance(1)); // 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dist[3]); // 1

    // ---- Graph 8: Remove edge test ----
    graph_init(4);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(3, 0);

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_edges()); // 4

    remove_edge(1, 2);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, count_edges()); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, has_edge(1, 2)); // 0

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, is_connected()); // 1

    // ---- Sum degrees test ----
    graph_init(4);
    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(0, 3);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, sum_degrees()); // 6 (each edge adds 2)

    return 0;
}
