int printf(const char *fmt, ...);

// Bipartite Graph Check using BFS coloring
// A graph is bipartite iff it can be 2-colored (no odd cycles)

int adj[12][12];
int adj_cnt[12];
int N;
int side[12];

void init(int n) {
    int i;
    N = n;
    for (i = 0; i < n; i++) {
        adj_cnt[i] = 0;
        side[i] = -1;
    }
}

void add_edge(int u, int v) {
    adj[u][adj_cnt[u]] = v;
    adj_cnt[u] = adj_cnt[u] + 1;
    adj[v][adj_cnt[v]] = u;
    adj_cnt[v] = adj_cnt[v] + 1;
}

// BFS-based bipartite check from a start vertex
// Returns 1 if bipartite, 0 if not
int bfs_bipartite(int start) {
    int queue[64];
    int front;
    int back;
    int u;
    int v;
    int i;

    front = 0;
    back = 0;
    queue[back] = start;
    back = back + 1;
    side[start] = 0;

    while (front < back) {
        u = queue[front];
        front = front + 1;

        for (i = 0; i < adj_cnt[u]; i++) {
            v = adj[u][i];
            if (side[v] == -1) {
                side[v] = 1 - side[u];
                queue[back] = v;
                back = back + 1;
            } else if (side[v] == side[u]) {
                return 0;
            }
        }
    }
    return 1;
}

int check_bipartite() {
    int i;
    int result;
    result = 1;
    for (i = 0; i < N; i++) {
        if (side[i] == -1) {
            if (!bfs_bipartite(i)) {
                result = 0;
            }
        }
    }
    return result;
}

void print_sides() {
    int i;
    int count0;
    int count1;
    count0 = 0;
    count1 = 0;
    for (i = 0; i < N; i++) {
        if (side[i] == 0) count0 = count0 + 1;
        if (side[i] == 1) count1 = count1 + 1;
    }
    printf("Side 0 count: %d\n", count0);
    printf("Side 1 count: %d\n", count1);
}

int main() {
    int result;
    int i;

    // Test 1: Bipartite graph (complete bipartite K3,3)
    printf("Test 1: K3,3 bipartite graph\n"); // EXPECT: Test 1: K3,3 bipartite graph
    init(6);
    // Set A: {0,1,2}, Set B: {3,4,5}
    add_edge(0, 3);
    add_edge(0, 4);
    add_edge(0, 5);
    add_edge(1, 3);
    add_edge(1, 4);
    add_edge(1, 5);
    add_edge(2, 3);
    add_edge(2, 4);
    add_edge(2, 5);

    result = check_bipartite();
    printf("Is bipartite: %d\n", result); // EXPECT: Is bipartite: 1

    printf("Vertex sides:\n"); // EXPECT: Vertex sides:
    for (i = 0; i < N; i++) {
        printf("  v%d: side %d\n", i, side[i]);
    }
    // EXPECT:   v0: side 0
    // EXPECT:   v1: side 0
    // EXPECT:   v2: side 0
    // EXPECT:   v3: side 1
    // EXPECT:   v4: side 1
    // EXPECT:   v5: side 1

    print_sides();
    // EXPECT: Side 0 count: 3
    // EXPECT: Side 1 count: 3

    // Test 2: Non-bipartite (triangle / odd cycle)
    printf("Test 2: Triangle (odd cycle)\n"); // EXPECT: Test 2: Triangle (odd cycle)
    init(3);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 0);

    result = check_bipartite();
    printf("Is bipartite: %d\n", result); // EXPECT: Is bipartite: 0

    // Test 3: Even cycle (bipartite)
    printf("Test 3: Even cycle C4\n"); // EXPECT: Test 3: Even cycle C4
    init(4);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(3, 0);

    result = check_bipartite();
    printf("Is bipartite: %d\n", result); // EXPECT: Is bipartite: 1
    for (i = 0; i < 4; i++) {
        printf("  v%d: side %d\n", i, side[i]);
    }
    // EXPECT:   v0: side 0
    // EXPECT:   v1: side 1
    // EXPECT:   v2: side 0
    // EXPECT:   v3: side 1

    // Test 4: Disconnected graph with two bipartite components
    printf("Test 4: Disconnected bipartite\n"); // EXPECT: Test 4: Disconnected bipartite
    init(6);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    // Component 2: 4-5
    add_edge(4, 5);

    result = check_bipartite();
    printf("Is bipartite: %d\n", result); // EXPECT: Is bipartite: 1

    // Test 5: Pentagon (5-cycle, odd, not bipartite)
    printf("Test 5: Pentagon C5\n"); // EXPECT: Test 5: Pentagon C5
    init(5);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(3, 4);
    add_edge(4, 0);

    result = check_bipartite();
    printf("Is bipartite: %d\n", result); // EXPECT: Is bipartite: 0

    // Test 6: Tree (always bipartite)
    printf("Test 6: Tree\n"); // EXPECT: Test 6: Tree
    init(7);
    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(1, 3);
    add_edge(1, 4);
    add_edge(2, 5);
    add_edge(2, 6);

    result = check_bipartite();
    printf("Is bipartite: %d\n", result); // EXPECT: Is bipartite: 1
    printf("Root side: %d\n", side[0]); // EXPECT: Root side: 0
    printf("Leaf 3 side: %d\n", side[3]); // EXPECT: Leaf 3 side: 0
    printf("Leaf 6 side: %d\n", side[6]); // EXPECT: Leaf 6 side: 0

    return 0;
}
