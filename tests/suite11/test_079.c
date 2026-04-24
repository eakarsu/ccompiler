int printf(const char *fmt, ...);

// Euler Path and Euler Circuit Detection
// Euler path: visits every edge exactly once
// Euler circuit: Euler path that starts and ends at the same vertex
//
// For undirected graphs:
//   Euler circuit exists iff all vertices have even degree and graph is connected
//   Euler path exists iff exactly 0 or 2 vertices have odd degree and graph is connected

int adj[12][12];
int N;

void init(int n) {
    int i;
    int j;
    N = n;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            adj[i][j] = 0;
        }
    }
}

void add_edge(int u, int v) {
    adj[u][v] = adj[u][v] + 1;
    adj[v][u] = adj[v][u] + 1;
}

int degree(int v) {
    int i;
    int d;
    d = 0;
    for (i = 0; i < N; i++) {
        d = d + adj[v][i];
    }
    return d;
}

// Check connectivity via BFS (only among vertices with degree > 0)
int is_connected() {
    int visited[12];
    int queue[12];
    int front;
    int back;
    int start;
    int u;
    int v;
    int i;

    for (i = 0; i < N; i++) visited[i] = 0;

    // Find first vertex with edges
    start = -1;
    for (i = 0; i < N; i++) {
        if (degree(i) > 0) {
            start = i;
            break;
        }
    }
    if (start == -1) return 1; // No edges, trivially connected

    front = 0;
    back = 0;
    queue[back] = start;
    back = back + 1;
    visited[start] = 1;

    while (front < back) {
        u = queue[front];
        front = front + 1;
        for (v = 0; v < N; v++) {
            if (adj[u][v] > 0 && !visited[v]) {
                visited[v] = 1;
                queue[back] = v;
                back = back + 1;
            }
        }
    }

    // Check all vertices with edges are visited
    for (i = 0; i < N; i++) {
        if (degree(i) > 0 && !visited[i]) return 0;
    }
    return 1;
}

struct EulerResult {
    int has_circuit;
    int has_path;
    int odd_count;
    int start_vertex;
    int connected;
};

void check_euler(struct EulerResult *res) {
    int i;
    int d;
    int first_odd;

    res->odd_count = 0;
    res->has_circuit = 0;
    res->has_path = 0;
    res->start_vertex = -1;
    first_odd = -1;

    for (i = 0; i < N; i++) {
        d = degree(i);
        if (d % 2 != 0) {
            res->odd_count = res->odd_count + 1;
            if (first_odd == -1) first_odd = i;
        }
    }

    res->connected = is_connected();

    if (!res->connected) return;

    if (res->odd_count == 0) {
        res->has_circuit = 1;
        res->has_path = 1;
        res->start_vertex = 0;
    } else if (res->odd_count == 2) {
        res->has_circuit = 0;
        res->has_path = 1;
        res->start_vertex = first_odd;
    }
}

int main() {
    struct EulerResult er;
    int i;

    // Test 1: Euler circuit (all even degrees)
    // Square: 0-1-2-3-0
    printf("Test 1: Square graph\n"); // EXPECT: Test 1: Square graph
    init(4);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(3, 0);

    printf("Degrees: "); // EXPECT: Degrees: 2 2 2 2
    for (i = 0; i < N; i++) {
        if (i > 0) printf(" ");
        printf("%d", degree(i));
    }
    printf("\n");

    check_euler(&er);
    printf("Connected: %d\n", er.connected); // EXPECT: Connected: 1
    printf("Odd degree count: %d\n", er.odd_count); // EXPECT: Odd degree count: 0
    printf("Has Euler circuit: %d\n", er.has_circuit); // EXPECT: Has Euler circuit: 1
    printf("Has Euler path: %d\n", er.has_path); // EXPECT: Has Euler path: 1

    // Test 2: Euler path but no circuit (exactly 2 odd-degree vertices)
    // Path: 0-1-2-3 with extra edge 1-3
    printf("Test 2: Graph with Euler path\n"); // EXPECT: Test 2: Graph with Euler path
    init(4);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(1, 3);

    printf("Degrees: "); // EXPECT: Degrees: 1 3 2 2
    for (i = 0; i < N; i++) {
        if (i > 0) printf(" ");
        printf("%d", degree(i));
    }
    printf("\n");

    check_euler(&er);
    printf("Connected: %d\n", er.connected); // EXPECT: Connected: 1
    printf("Odd degree count: %d\n", er.odd_count); // EXPECT: Odd degree count: 2
    printf("Has Euler circuit: %d\n", er.has_circuit); // EXPECT: Has Euler circuit: 0
    printf("Has Euler path: %d\n", er.has_path); // EXPECT: Has Euler path: 1
    printf("Start vertex: %d\n", er.start_vertex); // EXPECT: Start vertex: 0

    // Test 3: No Euler path (4 odd-degree vertices)
    printf("Test 3: No Euler path\n"); // EXPECT: Test 3: No Euler path
    init(4);
    add_edge(0, 1);
    add_edge(2, 3);

    printf("Degrees: "); // EXPECT: Degrees: 1 1 1 1
    for (i = 0; i < N; i++) {
        if (i > 0) printf(" ");
        printf("%d", degree(i));
    }
    printf("\n");

    check_euler(&er);
    printf("Connected: %d\n", er.connected); // EXPECT: Connected: 0
    printf("Has Euler path: %d\n", er.has_path); // EXPECT: Has Euler path: 0

    // Test 4: Complete graph K5 (Euler circuit, all degree 4)
    printf("Test 4: K5\n"); // EXPECT: Test 4: K5
    init(5);
    add_edge(0, 1); add_edge(0, 2); add_edge(0, 3); add_edge(0, 4);
    add_edge(1, 2); add_edge(1, 3); add_edge(1, 4);
    add_edge(2, 3); add_edge(2, 4);
    add_edge(3, 4);

    printf("Degrees: "); // EXPECT: Degrees: 4 4 4 4 4
    for (i = 0; i < 5; i++) {
        if (i > 0) printf(" ");
        printf("%d", degree(i));
    }
    printf("\n");

    check_euler(&er);
    printf("Has Euler circuit: %d\n", er.has_circuit); // EXPECT: Has Euler circuit: 1

    // Test 5: House graph (square + triangle on top)
    printf("Test 5: House graph\n"); // EXPECT: Test 5: House graph
    init(5);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(3, 0);
    add_edge(2, 4);
    add_edge(3, 4);

    printf("Degrees: "); // EXPECT: Degrees: 2 2 3 3 2
    for (i = 0; i < 5; i++) {
        if (i > 0) printf(" ");
        printf("%d", degree(i));
    }
    printf("\n");

    check_euler(&er);
    printf("Odd count: %d\n", er.odd_count); // EXPECT: Odd count: 2
    printf("Has Euler path: %d\n", er.has_path); // EXPECT: Has Euler path: 1
    printf("Has Euler circuit: %d\n", er.has_circuit); // EXPECT: Has Euler circuit: 0
    printf("Start: %d\n", er.start_vertex); // EXPECT: Start: 2

    return 0;
}
