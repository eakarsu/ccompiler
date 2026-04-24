int printf(const char *fmt, ...);

// Articulation Points (Cut Vertices) using iterative DFS
// Undirected graph: 7 vertices

int adj[10][10];
int adj_cnt[10];
int N;

int disc[10];
int low[10];
int parent_arr[10];
int is_ap[10];
int timer_val;

void init(int n) {
    int i;
    N = n;
    timer_val = 0;
    for (i = 0; i < n; i++) {
        adj_cnt[i] = 0;
        disc[i] = -1;
        low[i] = -1;
        parent_arr[i] = -1;
        is_ap[i] = 0;
    }
}

void add_edge(int u, int v) {
    adj[u][adj_cnt[u]] = v;
    adj_cnt[u] = adj_cnt[u] + 1;
    adj[v][adj_cnt[v]] = u;
    adj_cnt[v] = adj_cnt[v] + 1;
}

// Iterative DFS-based articulation point finding
void find_ap(int start) {
    int stack[64];
    int child_idx[64];
    int top;
    int u;
    int v;
    int idx;
    int child_count[10];
    int i;

    for (i = 0; i < N; i++) child_count[i] = 0;

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
                parent_arr[v] = u;
                disc[v] = timer_val;
                low[v] = timer_val;
                timer_val = timer_val + 1;

                if (parent_arr[u] == -1) {
                    child_count[u] = child_count[u] + 1;
                }

                top = top + 1;
                stack[top] = v;
                child_idx[top] = 0;
            } else if (v != parent_arr[u]) {
                if (disc[v] < low[u]) {
                    low[u] = disc[v];
                }
            }
        } else {
            // Done with u, backtrack
            if (top > 0) {
                int p;
                p = stack[top - 1];
                if (low[u] < low[p]) {
                    low[p] = low[u];
                }
                // Check AP condition for non-root
                if (parent_arr[p] != -1 && low[u] >= disc[p]) {
                    is_ap[p] = 1;
                }
            }
            top = top - 1;
        }
    }

    // Root is AP if it has 2+ children
    if (child_count[start] >= 2) {
        is_ap[start] = 1;
    }
}

int main() {
    int i;
    int ap_count;

    // Graph:
    //   0---1---3---5
    //   |   |   |   |
    //   +---2   4---6
    // Articulation points: 1 (separates {0,2} from {3,4,5,6}), 3 (separates {5,6} from rest)

    init(7);
    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(1, 2);
    add_edge(1, 3);
    add_edge(3, 4);
    add_edge(3, 5);
    add_edge(4, 3);
    add_edge(5, 6);

    find_ap(0);

    printf("Discovery times:\n"); // EXPECT: Discovery times:
    for (i = 0; i < N; i++) {
        printf("  disc[%d] = %d\n", i, disc[i]);
    }
    // EXPECT:   disc[0] = 0
    // EXPECT:   disc[1] = 1
    // EXPECT:   disc[2] = 2
    // EXPECT:   disc[3] = 3
    // EXPECT:   disc[4] = 4
    // EXPECT:   disc[5] = 5
    // EXPECT:   disc[6] = 6

    printf("Low values:\n"); // EXPECT: Low values:
    for (i = 0; i < N; i++) {
        printf("  low[%d] = %d\n", i, low[i]);
    }
    // EXPECT:   low[0] = 0
    // EXPECT:   low[1] = 0
    // EXPECT:   low[2] = 0
    // EXPECT:   low[3] = 3
    // EXPECT:   low[4] = 4
    // EXPECT:   low[5] = 5
    // EXPECT:   low[6] = 6

    printf("Articulation points:\n"); // EXPECT: Articulation points:
    ap_count = 0;
    for (i = 0; i < N; i++) {
        if (is_ap[i]) {
            printf("  vertex %d\n", i);
            ap_count = ap_count + 1;
        }
    }
    // EXPECT:   vertex 1
    // EXPECT:   vertex 3
    // EXPECT:   vertex 5
    printf("Total APs: %d\n", ap_count); // EXPECT: Total APs: 3

    // vertex 0 is not an AP (removing it still leaves 1-2 connected)
    printf("Is 0 AP: %d\n", is_ap[0]); // EXPECT: Is 0 AP: 0
    // vertex 1 is an AP (bridge between {0,2} and {3,...})
    printf("Is 1 AP: %d\n", is_ap[1]); // EXPECT: Is 1 AP: 1
    printf("Is 2 AP: %d\n", is_ap[2]); // EXPECT: Is 2 AP: 0
    printf("Is 3 AP: %d\n", is_ap[3]); // EXPECT: Is 3 AP: 1

    return 0;
}
