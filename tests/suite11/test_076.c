int printf(const char *fmt, ...);

// Bridge Detection in undirected graph using iterative DFS
// A bridge is an edge whose removal disconnects the graph

int adj[12][12];
int adj_cnt[12];
int N;

int disc[12];
int low[12];
int par[12];
int timer_val;

struct Bridge {
    int u;
    int v;
};

struct Bridge bridges[20];
int bridge_count;

void init(int n) {
    int i;
    N = n;
    timer_val = 0;
    bridge_count = 0;
    for (i = 0; i < n; i++) {
        adj_cnt[i] = 0;
        disc[i] = -1;
        low[i] = -1;
        par[i] = -1;
    }
}

void add_edge(int u, int v) {
    adj[u][adj_cnt[u]] = v;
    adj_cnt[u] = adj_cnt[u] + 1;
    adj[v][adj_cnt[v]] = u;
    adj_cnt[v] = adj_cnt[v] + 1;
}

void find_bridges(int start) {
    int stack[64];
    int child_idx[64];
    int top;
    int u;
    int v;
    int idx;

    top = 0;
    stack[top] = start;
    child_idx[top] = 0;
    disc[start] = timer_val;
    low[start] = timer_val;
    timer_val = timer_val + 1;

    while (top >= 0) {
        u = stack[top];
        idx = child_idx[top];

        if (idx < adj_cnt[u]) {
            child_idx[top] = idx + 1;
            v = adj[u][idx];

            if (disc[v] == -1) {
                par[v] = u;
                disc[v] = timer_val;
                low[v] = timer_val;
                timer_val = timer_val + 1;

                top = top + 1;
                stack[top] = v;
                child_idx[top] = 0;
            } else if (v != par[u]) {
                if (disc[v] < low[u]) {
                    low[u] = disc[v];
                }
            }
        } else {
            // Backtrack
            if (top > 0) {
                int p;
                p = stack[top - 1];
                if (low[u] < low[p]) {
                    low[p] = low[u];
                }
                // Bridge condition: low[u] > disc[p]
                if (low[u] > disc[p]) {
                    bridges[bridge_count].u = p;
                    bridges[bridge_count].v = u;
                    bridge_count = bridge_count + 1;
                }
            }
            top = top - 1;
        }
    }
}

// Sort bridges for deterministic output
void sort_bridges() {
    int i;
    int j;
    struct Bridge tmp;
    for (i = 0; i < bridge_count - 1; i++) {
        for (j = 0; j < bridge_count - 1 - i; j++) {
            int swap;
            swap = 0;
            if (bridges[j].u > bridges[j + 1].u) swap = 1;
            if (bridges[j].u == bridges[j + 1].u && bridges[j].v > bridges[j + 1].v) swap = 1;
            if (swap) {
                tmp = bridges[j];
                bridges[j] = bridges[j + 1];
                bridges[j + 1] = tmp;
            }
        }
    }
}

int main() {
    int i;

    // Graph:
    // 0-1-2 form a triangle (cycle, no bridge here)
    // 2-3 is a bridge
    // 3-4-5 form a triangle (cycle, no bridge here)
    // 5-6 is a bridge
    init(7);
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(0, 2);
    add_edge(2, 3);
    add_edge(3, 4);
    add_edge(4, 5);
    add_edge(3, 5);
    add_edge(5, 6);

    printf("Graph edges:\n"); // EXPECT: Graph edges:
    printf("  0-1, 1-2, 0-2 (triangle)\n"); // EXPECT:   0-1, 1-2, 0-2 (triangle)
    printf("  2-3 (bridge)\n"); // EXPECT:   2-3 (bridge)
    printf("  3-4, 4-5, 3-5 (triangle)\n"); // EXPECT:   3-4, 4-5, 3-5 (triangle)
    printf("  5-6 (bridge)\n"); // EXPECT:   5-6 (bridge)

    find_bridges(0);

    printf("Discovery times:\n"); // EXPECT: Discovery times:
    for (i = 0; i < N; i++) {
        printf("  disc[%d]=%d low[%d]=%d\n", i, disc[i], i, low[i]);
    }
    // EXPECT:   disc[0]=0 low[0]=0
    // EXPECT:   disc[1]=1 low[1]=0
    // EXPECT:   disc[2]=2 low[2]=0
    // EXPECT:   disc[3]=3 low[3]=3
    // EXPECT:   disc[4]=4 low[4]=3
    // EXPECT:   disc[5]=5 low[5]=3
    // EXPECT:   disc[6]=6 low[6]=6

    sort_bridges();

    printf("Bridges found: %d\n", bridge_count); // EXPECT: Bridges found: 2
    for (i = 0; i < bridge_count; i++) {
        printf("  bridge: %d-%d\n", bridges[i].u, bridges[i].v);
    }
    // EXPECT:   bridge: 2-3
    // EXPECT:   bridge: 5-6

    // Verify non-bridges
    printf("Edge 0-1 is bridge: %d\n", 0); // EXPECT: Edge 0-1 is bridge: 0
    printf("Edge 2-3 is bridge: %d\n", 1); // EXPECT: Edge 2-3 is bridge: 1

    return 0;
}
