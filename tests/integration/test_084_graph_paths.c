int printf(const char *fmt, ...);
// EXPECT: === Dijkstra Basic Test ===\nDist 0->0: 0\nDist 0->1: 1\nDist 0->2: 3\nDist 0->3: 4\nDist 0->4: 4\nDist 0->5: 4\nPath 0->5: 0->1->2->5\nPath 0->4: 0->1->4\nEdges: 14\nTotalWeight: 36\n=== Dijkstra Directed Test ===\nDist 0->0: 0\nDist 0->1: 8\nDist 0->2: 9\nDist 0->3: 5\nDist 0->4: 7\nPath 0->2: 0->3->1->2\nPath 0->4: 0->3->4\nHasCycle: 1\n=== Floyd-Warshall Test ===\nFW 0->0: 0\nFW 0->1: 3\nFW 0->2: 5\nFW 0->3: 6\nFW 1->0: 5\nFW 1->1: 0\nFW 1->2: 2\nFW 1->3: 3\nFW 2->0: 3\nFW 2->1: 6\nFW 2->2: 0\nFW 2->3: 1\nFW 3->0: 2\nFW 3->1: 5\nFW 3->2: 7\nFW 3->3: 0\nFW path 0->3: 0->1->2->3\nFW path 1->0: 1->2->3->0\nFW path 3->1: 3->0->1\n=== BFS Shortest Path Test ===\nBFS dist 0->0: 0\nBFS dist 0->1: 1\nBFS dist 0->2: 1\nBFS dist 0->3: 2\nBFS dist 0->4: 2\nBFS dist 0->5: 2\nBFS dist 0->6: 2\nBFS path 0->5: 0->2->5\nBFS path 0->6: 0->2->6\nBFS path 0->4: 0->1->4\n=== Connected Components Test ===\nNumComponents: 3\nComp 0: 0\nComp 1: 0\nComp 2: 0\nComp 3: 1\nComp 4: 1\nComp 5: 1\nComp 6: 2\nComp 7: 2\n=== Degree Test ===\nVertex 0: in=1 out=2\nVertex 1: in=1 out=1\nVertex 2: in=2 out=1\nVertex 3: in=1 out=1\nVertex 4: in=1 out=1\nEdges: 6\nHasCycle: 1\n=== No Path Test ===\nDist 0->2: 8\nDist 0->3: 99999\nDist 0->4: 99999\nComponents: 3\n=== Dense Graph Test ===\nDense edges: 20\nDense weight: 170\nDense dist 0->0: 0\nDense dist 0->1: 2\nDense dist 0->2: 3\nDense dist 0->3: 4\nDense dist 0->4: 5\nFW matches Dijkstra: 1
// ============================================================
// test_084_graph_paths.c
// Graph algorithms on small adjacency matrix graphs:
//   1. Dijkstra's shortest path (array-based)
//   2. Floyd-Warshall all-pairs shortest paths
//   3. Path reconstruction
//   4. BFS shortest path (unweighted)
//   5. Connected components
// ============================================================

// ---- Constants ----
// Maximum number of vertices
// Using small graphs (up to 8 vertices) for deterministic testing

// INF represents no direct edge
int INF_VAL;

// ---- Graph representation: adjacency matrix ----
int graph[10][10];
int num_vertices;

// ---- Initialize graph ----
void graph_init(int n) {
    int i;
    int j;
    num_vertices = n;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i == j) {
                graph[i][j] = 0;
            } else {
                graph[i][j] = 99999; // INF
            }
        }
    }
}

// ---- Add weighted directed edge ----
void add_edge(int from, int to, int weight) {
    graph[from][to] = weight;
}

// ---- Add weighted undirected edge ----
void add_undirected_edge(int from, int to, int weight) {
    graph[from][to] = weight;
    graph[to][from] = weight;
}

// ============================================================
// Dijkstra's Algorithm (array-based, no heap)
// ============================================================
int dist[10];
int prev_node[10];
int visited[10];

void dijkstra(int src) {
    int i;
    int j;
    int n = num_vertices;

    // Initialize distances
    for (i = 0; i < n; i++) {
        dist[i] = 99999;
        prev_node[i] = -1;
        visited[i] = 0;
    }
    dist[src] = 0;

    // Process all vertices
    for (i = 0; i < n; i++) {
        // Find unvisited vertex with minimum distance
        int u = -1;
        int min_dist = 99999;
        for (j = 0; j < n; j++) {
            if (!visited[j] && dist[j] < min_dist) {
                min_dist = dist[j];
                u = j;
            }
        }

        if (u == -1) break; // No reachable unvisited vertex

        visited[u] = 1;

        // Relax edges from u
        for (j = 0; j < n; j++) {
            if (!visited[j] && graph[u][j] < 99999) {
                int new_dist = dist[u] + graph[u][j];
                if (new_dist < dist[j]) {
                    dist[j] = new_dist;
                    prev_node[j] = u;
                }
            }
        }
    }
}

// Print shortest path from src to dest
void print_path(int dest) {
    // Reconstruct path using prev_node array
    int path[10];
    int path_len = 0;
    int current = dest;

    while (current != -1) {
        path[path_len] = current;
        path_len++;
        current = prev_node[current];
    }

    // Print in reverse (from src to dest)
    int i;
    for (i = path_len - 1; i >= 0; i--) {
        printf("%d", path[i]);
        if (i > 0) printf("->");
    }
    printf("\n");
}

// ============================================================
// Floyd-Warshall Algorithm
// ============================================================
int fw_dist[10][10];
int fw_next[10][10]; // For path reconstruction

void floyd_warshall() {
    int i;
    int j;
    int k;
    int n = num_vertices;

    // Initialize distance and next-hop matrices
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            fw_dist[i][j] = graph[i][j];
            if (graph[i][j] < 99999 && i != j) {
                fw_next[i][j] = j;
            } else {
                fw_next[i][j] = -1;
            }
        }
        // Distance to self is 0
        fw_dist[i][i] = 0;
        fw_next[i][i] = i;
    }

    // Main Floyd-Warshall loop
    for (k = 0; k < n; k++) {
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                if (fw_dist[i][k] < 99999 && fw_dist[k][j] < 99999) {
                    int through_k = fw_dist[i][k] + fw_dist[k][j];
                    if (through_k < fw_dist[i][j]) {
                        fw_dist[i][j] = through_k;
                        fw_next[i][j] = fw_next[i][k];
                    }
                }
            }
        }
    }
}

// Print Floyd-Warshall path from src to dest
void fw_print_path(int src, int dest) {
    if (fw_next[src][dest] == -1) {
        printf("no path\n");
        return;
    }
    int current = src;
    printf("%d", current);
    while (current != dest) {
        current = fw_next[current][dest];
        printf("->%d", current);
    }
    printf("\n");
}

// ============================================================
// BFS shortest path (unweighted graph)
// ============================================================
int bfs_dist[10];
int bfs_prev[10];

void bfs(int src) {
    int n = num_vertices;
    int i;

    // Initialize
    for (i = 0; i < n; i++) {
        bfs_dist[i] = -1;
        bfs_prev[i] = -1;
    }
    bfs_dist[src] = 0;

    // Queue for BFS
    int queue[20];
    int q_front = 0;
    int q_rear = 0;
    queue[q_rear] = src;
    q_rear++;

    while (q_front < q_rear) {
        int u = queue[q_front];
        q_front++;

        for (i = 0; i < n; i++) {
            // Check if there's an edge (weight < 99999) and not visited
            if (graph[u][i] < 99999 && graph[u][i] > 0 && bfs_dist[i] == -1) {
                bfs_dist[i] = bfs_dist[u] + 1;
                bfs_prev[i] = u;
                queue[q_rear] = i;
                q_rear++;
            }
        }
    }
}

// Print BFS path
void bfs_print_path(int dest) {
    if (bfs_dist[dest] == -1) {
        printf("no path\n");
        return;
    }
    int path[10];
    int path_len = 0;
    int current = dest;
    while (current != -1) {
        path[path_len] = current;
        path_len++;
        current = bfs_prev[current];
    }
    int i;
    for (i = path_len - 1; i >= 0; i--) {
        printf("%d", path[i]);
        if (i > 0) printf("->");
    }
    printf("\n");
}

// ============================================================
// Connected components (using DFS)
// ============================================================
int component[10];
int comp_visited[10];

void dfs_component(int v, int comp_id) {
    comp_visited[v] = 1;
    component[v] = comp_id;
    int i;
    for (i = 0; i < num_vertices; i++) {
        if (graph[v][i] < 99999 && graph[v][i] > 0 && !comp_visited[i]) {
            dfs_component(i, comp_id);
        }
    }
}

int find_components() {
    int i;
    int comp_id = 0;
    for (i = 0; i < num_vertices; i++) {
        comp_visited[i] = 0;
        component[i] = -1;
    }
    for (i = 0; i < num_vertices; i++) {
        if (!comp_visited[i]) {
            dfs_component(i, comp_id);
            comp_id++;
        }
    }
    return comp_id;
}

// ============================================================
// Degree calculations
// ============================================================
int in_degree(int v) {
    int deg = 0;
    int i;
    for (i = 0; i < num_vertices; i++) {
        if (graph[i][v] < 99999 && graph[i][v] > 0) {
            deg++;
        }
    }
    return deg;
}

int out_degree(int v) {
    int deg = 0;
    int i;
    for (i = 0; i < num_vertices; i++) {
        if (graph[v][i] < 99999 && graph[v][i] > 0) {
            deg++;
        }
    }
    return deg;
}

// ============================================================
// Check if graph has a cycle (DFS-based for directed graph)
// ============================================================
int cycle_color[10]; // 0=white, 1=gray, 2=black

int has_cycle_dfs(int v) {
    cycle_color[v] = 1; // gray
    int i;
    for (i = 0; i < num_vertices; i++) {
        if (graph[v][i] < 99999 && graph[v][i] > 0) {
            if (cycle_color[i] == 1) {
                return 1; // back edge = cycle
            }
            if (cycle_color[i] == 0) {
                if (has_cycle_dfs(i)) return 1;
            }
        }
    }
    cycle_color[v] = 2; // black
    return 0;
}

int has_cycle() {
    int i;
    for (i = 0; i < num_vertices; i++) {
        cycle_color[i] = 0;
    }
    for (i = 0; i < num_vertices; i++) {
        if (cycle_color[i] == 0) {
            if (has_cycle_dfs(i)) return 1;
        }
    }
    return 0;
}

// ============================================================
// Count total edges in graph
// ============================================================
int count_edges() {
    int count = 0;
    int i;
    int j;
    for (i = 0; i < num_vertices; i++) {
        for (j = 0; j < num_vertices; j++) {
            if (graph[i][j] < 99999 && graph[i][j] > 0) {
                count++;
            }
        }
    }
    return count;
}

// ============================================================
// Total weight of all edges
// ============================================================
int total_weight() {
    int w = 0;
    int i;
    int j;
    for (i = 0; i < num_vertices; i++) {
        for (j = 0; j < num_vertices; j++) {
            if (graph[i][j] < 99999 && graph[i][j] > 0) {
                w = w + graph[i][j];
            }
        }
    }
    return w;
}

// ============================================================
// Test functions
// ============================================================

void test_dijkstra_basic() {
    printf("=== Dijkstra Basic Test ===\n");
    // Graph with 6 vertices
    // 0 --1-- 1 --2-- 2
    // |       |       |
    // 4       3       1
    // |       |       |
    // 3 --5-- 4 --2-- 5
    graph_init(6);
    add_undirected_edge(0, 1, 1);
    add_undirected_edge(1, 2, 2);
    add_undirected_edge(0, 3, 4);
    add_undirected_edge(1, 4, 3);
    add_undirected_edge(2, 5, 1);
    add_undirected_edge(3, 4, 5);
    add_undirected_edge(4, 5, 2);

    dijkstra(0);

    printf("Dist 0->0: %d\n", dist[0]);
    printf("Dist 0->1: %d\n", dist[1]);
    printf("Dist 0->2: %d\n", dist[2]);
    printf("Dist 0->3: %d\n", dist[3]);
    printf("Dist 0->4: %d\n", dist[4]);
    printf("Dist 0->5: %d\n", dist[5]);

    printf("Path 0->5: ");
    print_path(5);
    printf("Path 0->4: ");
    print_path(4);

    printf("Edges: %d\n", count_edges());
    printf("TotalWeight: %d\n", total_weight());
}

void test_dijkstra_directed() {
    printf("=== Dijkstra Directed Test ===\n");
    // Directed graph with 5 vertices
    graph_init(5);
    add_edge(0, 1, 10);
    add_edge(0, 3, 5);
    add_edge(1, 2, 1);
    add_edge(1, 3, 2);
    add_edge(2, 4, 4);
    add_edge(3, 1, 3);
    add_edge(3, 2, 9);
    add_edge(3, 4, 2);
    add_edge(4, 0, 7);
    add_edge(4, 2, 6);

    dijkstra(0);

    printf("Dist 0->0: %d\n", dist[0]);
    printf("Dist 0->1: %d\n", dist[1]);
    printf("Dist 0->2: %d\n", dist[2]);
    printf("Dist 0->3: %d\n", dist[3]);
    printf("Dist 0->4: %d\n", dist[4]);

    printf("Path 0->2: ");
    print_path(2);
    printf("Path 0->4: ");
    print_path(4);

    printf("HasCycle: %d\n", has_cycle());
}

void test_floyd_warshall() {
    printf("=== Floyd-Warshall Test ===\n");
    // Graph with 4 vertices
    graph_init(4);
    add_edge(0, 1, 3);
    add_edge(0, 2, 7);
    add_edge(1, 0, 8);
    add_edge(1, 2, 2);
    add_edge(2, 0, 5);
    add_edge(2, 3, 1);
    add_edge(3, 0, 2);

    floyd_warshall();

    // Print all-pairs shortest distances
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            printf("FW %d->%d: %d\n", i, j, fw_dist[i][j]);
        }
    }

    // Print some paths
    printf("FW path 0->3: ");
    fw_print_path(0, 3);
    printf("FW path 1->0: ");
    fw_print_path(1, 0);
    printf("FW path 3->1: ");
    fw_print_path(3, 1);
}

void test_bfs_shortest() {
    printf("=== BFS Shortest Path Test ===\n");
    // Unweighted graph (all edges weight 1)
    graph_init(7);
    add_undirected_edge(0, 1, 1);
    add_undirected_edge(0, 2, 1);
    add_undirected_edge(1, 3, 1);
    add_undirected_edge(1, 4, 1);
    add_undirected_edge(2, 5, 1);
    add_undirected_edge(2, 6, 1);
    add_undirected_edge(3, 5, 1);

    bfs(0);

    printf("BFS dist 0->0: %d\n", bfs_dist[0]);
    printf("BFS dist 0->1: %d\n", bfs_dist[1]);
    printf("BFS dist 0->2: %d\n", bfs_dist[2]);
    printf("BFS dist 0->3: %d\n", bfs_dist[3]);
    printf("BFS dist 0->4: %d\n", bfs_dist[4]);
    printf("BFS dist 0->5: %d\n", bfs_dist[5]);
    printf("BFS dist 0->6: %d\n", bfs_dist[6]);

    printf("BFS path 0->5: ");
    bfs_print_path(5);
    printf("BFS path 0->6: ");
    bfs_print_path(6);
    printf("BFS path 0->4: ");
    bfs_print_path(4);
}

void test_connected_components() {
    printf("=== Connected Components Test ===\n");
    // Graph with 3 components
    graph_init(8);
    // Component 0: vertices 0,1,2
    add_undirected_edge(0, 1, 1);
    add_undirected_edge(1, 2, 1);
    // Component 1: vertices 3,4,5
    add_undirected_edge(3, 4, 1);
    add_undirected_edge(4, 5, 1);
    // Component 2: vertices 6,7
    add_undirected_edge(6, 7, 1);

    int nc = find_components();
    printf("NumComponents: %d\n", nc);
    printf("Comp 0: %d\n", component[0]);
    printf("Comp 1: %d\n", component[1]);
    printf("Comp 2: %d\n", component[2]);
    printf("Comp 3: %d\n", component[3]);
    printf("Comp 4: %d\n", component[4]);
    printf("Comp 5: %d\n", component[5]);
    printf("Comp 6: %d\n", component[6]);
    printf("Comp 7: %d\n", component[7]);
}

void test_degree() {
    printf("=== Degree Test ===\n");
    graph_init(5);
    add_edge(0, 1, 1);
    add_edge(0, 2, 1);
    add_edge(1, 2, 1);
    add_edge(2, 3, 1);
    add_edge(3, 4, 1);
    add_edge(4, 0, 1);

    int i;
    for (i = 0; i < 5; i++) {
        printf("Vertex %d: in=%d out=%d\n", i, in_degree(i), out_degree(i));
    }
    printf("Edges: %d\n", count_edges());
    printf("HasCycle: %d\n", has_cycle());
}

void test_no_path() {
    printf("=== No Path Test ===\n");
    // Graph where some vertices are unreachable
    graph_init(5);
    add_edge(0, 1, 3);
    add_edge(1, 2, 5);
    // Vertices 3 and 4 are isolated

    dijkstra(0);
    printf("Dist 0->2: %d\n", dist[2]);
    printf("Dist 0->3: %d\n", dist[3]);
    printf("Dist 0->4: %d\n", dist[4]);

    int nc = find_components();
    printf("Components: %d\n", nc);
}

void test_dense_graph() {
    printf("=== Dense Graph Test ===\n");
    // Complete graph with 5 vertices
    graph_init(5);
    int i;
    int j;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (i != j) {
                // Weight = (i+1)*(j+1)
                add_edge(i, j, (i + 1) * (j + 1));
            }
        }
    }

    printf("Dense edges: %d\n", count_edges());
    printf("Dense weight: %d\n", total_weight());

    dijkstra(0);
    for (i = 0; i < 5; i++) {
        printf("Dense dist 0->%d: %d\n", i, dist[i]);
    }

    floyd_warshall();
    // Check that Floyd-Warshall matches Dijkstra from vertex 0
    int match = 1;
    for (i = 0; i < 5; i++) {
        if (fw_dist[0][i] != dist[i]) {
            match = 0;
        }
    }
    printf("FW matches Dijkstra: %d\n", match);
}


int main() {
    test_dijkstra_basic();
    test_dijkstra_directed();
    test_floyd_warshall();
    test_bfs_shortest();
    test_connected_components();
    test_degree();
    test_no_path();
    test_dense_graph();
    return 0;
}
