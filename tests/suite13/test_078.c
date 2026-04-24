int printf(const char *fmt, ...);

// Minimum spanning tree using Prim's algorithm (array-based)

struct Graph {
    int weight[8][8];  // 0 means no edge
    int n;
};

void graph_init(struct Graph *g, int n) {
    int i;
    int j;
    g->n = n;
    for (i = 0; i < 8; i = i + 1) {
        for (j = 0; j < 8; j = j + 1) {
            g->weight[i][j] = 0;
        }
    }
}

void graph_add_edge(struct Graph *g, int u, int v, int w) {
    g->weight[u][v] = w;
    g->weight[v][u] = w;
}

// Prim's MST: returns total MST weight
// parent[i] = parent of node i in MST, parent[start] = -1
int prim_mst(struct Graph *g, int start, int *parent) {
    int in_mst[8];
    int key[8];  // minimum weight edge to include node
    int i;
    int iter;
    int u;
    int v;
    int min_key;
    int total;

    for (i = 0; i < g->n; i = i + 1) {
        key[i] = 999999;
        in_mst[i] = 0;
        parent[i] = -1;
    }
    key[start] = 0;

    for (iter = 0; iter < g->n; iter = iter + 1) {
        // Pick minimum key vertex not in MST
        u = -1;
        min_key = 999999;
        for (i = 0; i < g->n; i = i + 1) {
            if (!in_mst[i] && key[i] < min_key) {
                min_key = key[i];
                u = i;
            }
        }
        if (u == -1) break;

        in_mst[u] = 1;

        // Update keys of adjacent vertices
        for (v = 0; v < g->n; v = v + 1) {
            if (g->weight[u][v] > 0 && !in_mst[v] && g->weight[u][v] < key[v]) {
                key[v] = g->weight[u][v];
                parent[v] = u;
            }
        }
    }

    total = 0;
    for (i = 0; i < g->n; i = i + 1) {
        if (parent[i] != -1) {
            total = total + g->weight[i][parent[i]];
        }
    }
    return total;
}

// Count edges in MST
int mst_edge_count(int *parent, int n) {
    int i;
    int cnt;
    cnt = 0;
    for (i = 0; i < n; i = i + 1) {
        if (parent[i] != -1) cnt = cnt + 1;
    }
    return cnt;
}

// Find maximum edge weight in MST
int mst_max_edge(struct Graph *g, int *parent, int n) {
    int i;
    int maxw;
    maxw = 0;
    for (i = 0; i < n; i = i + 1) {
        if (parent[i] != -1) {
            if (g->weight[i][parent[i]] > maxw) {
                maxw = g->weight[i][parent[i]];
            }
        }
    }
    return maxw;
}

// Check degree of a node in MST
int mst_degree(int *parent, int n, int node) {
    int i;
    int deg;
    deg = 0;
    // Check if node is parent of any other node
    for (i = 0; i < n; i = i + 1) {
        if (parent[i] == node) deg = deg + 1;
    }
    // Check if node has a parent
    if (parent[node] != -1) deg = deg + 1;
    return deg;
}

int main() {
    struct Graph g;
    int parent[8];
    int total;

    // Graph: 6 nodes
    //   0 --(1)-- 1 --(6)-- 2
    //   |         |         |
    //  (3)       (2)       (5)
    //   |         |         |
    //   3 --(4)-- 4 --(7)-- 5
    graph_init(&g, 6);
    graph_add_edge(&g, 0, 1, 1);
    graph_add_edge(&g, 0, 3, 3);
    graph_add_edge(&g, 1, 2, 6);
    graph_add_edge(&g, 1, 4, 2);
    graph_add_edge(&g, 2, 5, 5);
    graph_add_edge(&g, 3, 4, 4);
    graph_add_edge(&g, 4, 5, 7);

    // MST from 0:
    // Pick 0 (key=0). Update: 1->1, 3->3.
    // Pick 1 (key=1, parent=0). Update: 2->6, 4->2.
    // Pick 4 (key=2, parent=1). Update: 3->min(3,4)=3, 5->7.
    // Pick 3 (key=3, parent=0). No better updates.
    // Pick 2 (key=6, parent=1). Update: 5->min(7,5)=5.
    // Pick 5 (key=5, parent=2).
    // MST edges: 0-1(1), 1-4(2), 0-3(3), 1-2(6), 2-5(5) = total 17
    total = prim_mst(&g, 0, parent);
    printf("%d\n", total);
    // EXPECT: 17

    printf("%d\n", mst_edge_count(parent, 6));
    // EXPECT: 5

    // parent array: parent[0]=-1, parent[1]=0, parent[2]=1, parent[3]=0, parent[4]=1, parent[5]=2
    printf("%d %d %d %d %d %d\n", parent[0], parent[1], parent[2], parent[3], parent[4], parent[5]);
    // EXPECT: -1 0 1 0 1 2

    printf("%d\n", mst_max_edge(&g, parent, 6));
    // EXPECT: 6

    // Degree of node 1 in MST: children 2,4, parent 0 -> degree 3
    printf("%d\n", mst_degree(parent, 6, 1));
    // EXPECT: 3
    // Degree of node 0: children 1,3, no parent -> degree 2
    printf("%d\n", mst_degree(parent, 6, 0));
    // EXPECT: 2
    // Degree of node 5: parent 2, no children -> degree 1
    printf("%d\n", mst_degree(parent, 6, 5));
    // EXPECT: 1

    // Triangle graph: 3 nodes, all connected
    // 0--(2)--1--(3)--2--(1)--0
    graph_init(&g, 3);
    graph_add_edge(&g, 0, 1, 2);
    graph_add_edge(&g, 1, 2, 3);
    graph_add_edge(&g, 0, 2, 1);

    // MST: edges 0-2(1) and 0-1(2) = total 3
    // Start at 0: pick 0. Update 1->2, 2->1.
    // Pick 2 (key=1, parent=0). Update 1->min(2,3)=2.
    // Pick 1 (key=2, parent=0).
    total = prim_mst(&g, 0, parent);
    printf("%d\n", total);
    // EXPECT: 3
    printf("%d %d %d\n", parent[0], parent[1], parent[2]);
    // EXPECT: -1 0 0
    printf("%d\n", mst_edge_count(parent, 3));
    // EXPECT: 2

    // Linear graph: 0--(5)--1--(3)--2--(8)--3
    graph_init(&g, 4);
    graph_add_edge(&g, 0, 1, 5);
    graph_add_edge(&g, 1, 2, 3);
    graph_add_edge(&g, 2, 3, 8);

    total = prim_mst(&g, 0, parent);
    printf("%d\n", total);
    // EXPECT: 16
    printf("%d\n", mst_max_edge(&g, parent, 4));
    // EXPECT: 8

    // Complete graph K4 with varying weights
    // 0-1:1, 0-2:4, 0-3:3, 1-2:2, 1-3:5, 2-3:6
    graph_init(&g, 4);
    graph_add_edge(&g, 0, 1, 1);
    graph_add_edge(&g, 0, 2, 4);
    graph_add_edge(&g, 0, 3, 3);
    graph_add_edge(&g, 1, 2, 2);
    graph_add_edge(&g, 1, 3, 5);
    graph_add_edge(&g, 2, 3, 6);

    // MST from 0: pick 0, update 1->1,2->4,3->3
    // Pick 1(key=1,parent=0). Update 2->min(4,2)=2, 3->min(3,5)=3
    // Pick 2(key=2,parent=1). Update 3->min(3,6)=3
    // Pick 3(key=3,parent=0). Total: 1+2+3=6
    total = prim_mst(&g, 0, parent);
    printf("%d\n", total);
    // EXPECT: 6
    printf("%d %d %d %d\n", parent[0], parent[1], parent[2], parent[3]);
    // EXPECT: -1 0 1 0

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
