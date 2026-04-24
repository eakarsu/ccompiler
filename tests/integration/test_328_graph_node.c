int printf(const char *fmt, ...);
// EXPECT: nodes: 5\ndegree(1): 2\ndegree(4): 1\nedge 1->2: 1\nedge 2->1: 0\nedge 1->5: 0\nreachable from 1: 5
// Test: graph node with structs

struct GraphNode {
    int id;
    int neighbors[8];
    int num_neighbors;
    int visited;
};

struct Graph {
    struct GraphNode nodes[8];
    int num_nodes;
};

void graph_init(struct Graph *g) {
    g->num_nodes = 0;
}

int graph_add_node(struct Graph *g, int id) {
    if (g->num_nodes >= 8) return -1;
    int idx = g->num_nodes;
    g->nodes[idx].id = id;
    g->nodes[idx].num_neighbors = 0;
    g->nodes[idx].visited = 0;
    g->num_nodes = g->num_nodes + 1;
    return idx;
}

int graph_find(struct Graph *g, int id) {
    int i;
    for (i = 0; i < g->num_nodes; i++) {
        if (g->nodes[i].id == id) return i;
    }
    return -1;
}

void graph_add_edge(struct Graph *g, int from_id, int to_id) {
    int fi = graph_find(g, from_id);
    int ti = graph_find(g, to_id);
    if (fi < 0 || ti < 0) return;
    struct GraphNode *n = &g->nodes[fi];
    if (n->num_neighbors < 8) {
        n->neighbors[n->num_neighbors] = to_id;
        n->num_neighbors = n->num_neighbors + 1;
    }
}

int graph_degree(struct Graph *g, int id) {
    int idx = graph_find(g, id);
    if (idx < 0) return 0;
    return g->nodes[idx].num_neighbors;
}

int graph_has_edge(struct Graph *g, int from_id, int to_id) {
    int fi = graph_find(g, from_id);
    if (fi < 0) return 0;
    int i;
    for (i = 0; i < g->nodes[fi].num_neighbors; i++) {
        if (g->nodes[fi].neighbors[i] == to_id) return 1;
    }
    return 0;
}

void graph_reset_visited(struct Graph *g) {
    int i;
    for (i = 0; i < g->num_nodes; i++) {
        g->nodes[i].visited = 0;
    }
}

int graph_bfs_count(struct Graph *g, int start_id) {
    graph_reset_visited(g);
    int queue[8];
    int front = 0;
    int back = 0;
    int count = 0;
    int si = graph_find(g, start_id);
    if (si < 0) return 0;
    queue[back] = si; back = back + 1;
    g->nodes[si].visited = 1;
    while (front < back) {
        int ci = queue[front]; front = front + 1;
        count = count + 1;
        int i;
        for (i = 0; i < g->nodes[ci].num_neighbors; i++) {
            int ni = graph_find(g, g->nodes[ci].neighbors[i]);
            if (ni >= 0 && !g->nodes[ni].visited) {
                g->nodes[ni].visited = 1;
                queue[back] = ni; back = back + 1;
            }
        }
    }
    return count;
}

int main(void) {
    struct Graph g;
    graph_init(&g);
    graph_add_node(&g, 1);
    graph_add_node(&g, 2);
    graph_add_node(&g, 3);
    graph_add_node(&g, 4);
    graph_add_node(&g, 5);

    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 1, 3);
    graph_add_edge(&g, 2, 4);
    graph_add_edge(&g, 3, 4);
    graph_add_edge(&g, 4, 5);

    printf("nodes: %d\n", g.num_nodes);
    printf("degree(1): %d\n", graph_degree(&g, 1));
    printf("degree(4): %d\n", graph_degree(&g, 4));
    printf("edge 1->2: %d\n", graph_has_edge(&g, 1, 2));
    printf("edge 2->1: %d\n", graph_has_edge(&g, 2, 1));
    printf("edge 1->5: %d\n", graph_has_edge(&g, 1, 5));

    int reachable = graph_bfs_count(&g, 1);
    printf("reachable from 1: %d\n", reachable);

    return 0;
}
