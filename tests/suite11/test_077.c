int printf(const char *fmt, ...);

// Greedy Graph Coloring
// Assign colors to vertices such that no two adjacent vertices share a color

int adj[12][12];
int adj_cnt[12];
int N;
int color[12];

void init(int n) {
    int i;
    N = n;
    for (i = 0; i < n; i++) {
        adj_cnt[i] = 0;
        color[i] = -1;
    }
}

void add_edge(int u, int v) {
    adj[u][adj_cnt[u]] = v;
    adj_cnt[u] = adj_cnt[u] + 1;
    adj[v][adj_cnt[v]] = u;
    adj_cnt[v] = adj_cnt[v] + 1;
}

int greedy_color() {
    int v;
    int i;
    int c;
    int neighbor;
    int used[12];
    int max_color;

    max_color = 0;

    for (v = 0; v < N; v++) {
        // Mark colors used by neighbors
        for (i = 0; i < N; i++) {
            used[i] = 0;
        }
        for (i = 0; i < adj_cnt[v]; i++) {
            neighbor = adj[v][i];
            if (color[neighbor] != -1) {
                used[color[neighbor]] = 1;
            }
        }
        // Find first available color
        for (c = 0; c < N; c++) {
            if (!used[c]) {
                color[v] = c;
                if (c > max_color) max_color = c;
                break;
            }
        }
    }
    return max_color + 1;
}

int verify_coloring() {
    int v;
    int i;
    int neighbor;
    for (v = 0; v < N; v++) {
        for (i = 0; i < adj_cnt[v]; i++) {
            neighbor = adj[v][i];
            if (color[v] == color[neighbor]) {
                return 0;
            }
        }
    }
    return 1;
}

int main() {
    int num_colors;
    int valid;
    int i;
    int color_counts[12];

    // Petersen-like graph with 8 vertices
    // Outer cycle: 0-1-2-3-4-0
    // Inner connections: 0-5, 1-6, 2-7, 3-5, 4-6
    init(8);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(3, 4);
    add_edge(4, 0);
    add_edge(0, 5);
    add_edge(1, 6);
    add_edge(2, 7);
    add_edge(3, 5);
    add_edge(4, 6);
    add_edge(5, 7);

    printf("Graph with %d vertices:\n", N); // EXPECT: Graph with 8 vertices:
    printf("Edges: 0-1 1-2 2-3 3-4 4-0\n"); // EXPECT: Edges: 0-1 1-2 2-3 3-4 4-0
    printf("       0-5 1-6 2-7 3-5 4-6 5-7\n"); // EXPECT:        0-5 1-6 2-7 3-5 4-6 5-7

    num_colors = greedy_color();
    printf("Colors used: %d\n", num_colors); // EXPECT: Colors used: 3

    printf("Vertex colors:\n"); // EXPECT: Vertex colors:
    for (i = 0; i < N; i++) {
        printf("  v%d: color %d\n", i, color[i]);
    }
    // EXPECT:   v0: color 0
    // EXPECT:   v1: color 1
    // EXPECT:   v2: color 0
    // EXPECT:   v3: color 1
    // EXPECT:   v4: color 2
    // EXPECT:   v5: color 2
    // EXPECT:   v6: color 0
    // EXPECT:   v7: color 1

    valid = verify_coloring();
    printf("Coloring valid: %d\n", valid); // EXPECT: Coloring valid: 1

    // Count vertices per color
    for (i = 0; i < num_colors; i++) {
        color_counts[i] = 0;
    }
    for (i = 0; i < N; i++) {
        color_counts[color[i]] = color_counts[color[i]] + 1;
    }
    for (i = 0; i < num_colors; i++) {
        printf("Color %d: %d vertices\n", i, color_counts[i]);
    }
    // EXPECT: Color 0: 3 vertices
    // EXPECT: Color 1: 3 vertices
    // EXPECT: Color 2: 2 vertices

    // Test with a complete graph K4 (needs 4 colors)
    init(4);
    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(0, 3);
    add_edge(1, 2);
    add_edge(1, 3);
    add_edge(2, 3);

    num_colors = greedy_color();
    printf("K4 colors used: %d\n", num_colors); // EXPECT: K4 colors used: 4
    valid = verify_coloring();
    printf("K4 coloring valid: %d\n", valid); // EXPECT: K4 coloring valid: 1

    // Test with bipartite graph (needs 2 colors)
    init(6);
    add_edge(0, 3);
    add_edge(0, 4);
    add_edge(1, 3);
    add_edge(1, 5);
    add_edge(2, 4);
    add_edge(2, 5);

    num_colors = greedy_color();
    printf("Bipartite colors: %d\n", num_colors); // EXPECT: Bipartite colors: 2
    valid = verify_coloring();
    printf("Bipartite valid: %d\n", valid); // EXPECT: Bipartite valid: 1

    return 0;
}
