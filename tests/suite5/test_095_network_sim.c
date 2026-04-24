int printf(const char *fmt, ...);

struct Edge {
    int from;
    int to;
    int weight;
};

struct Node {
    char name[10];
    int id;
};

struct Network {
    struct Node nodes[16];
    int node_count;
    struct Edge edges[64];
    int edge_count;
    int routing[16][16];
};

void copy_str(char *dst, const char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int add_node(struct Network *net, const char *name) {
    int id = net->node_count;
    net->nodes[id].id = id;
    copy_str(net->nodes[id].name, name);
    net->node_count = net->node_count + 1;
    return id;
}

void add_edge(struct Network *net, int from, int to, int w) {
    int idx = net->edge_count;
    net->edges[idx].from = from;
    net->edges[idx].to = to;
    net->edges[idx].weight = w;
    net->edge_count = net->edge_count + 1;
    idx = net->edge_count;
    net->edges[idx].from = to;
    net->edges[idx].to = from;
    net->edges[idx].weight = w;
    net->edge_count = net->edge_count + 1;
}

void init_routing(struct Network *net) {
    int i = 0;
    while (i < 16) {
        int j = 0;
        while (j < 16) {
            if (i == j)
                net->routing[i][j] = 0;
            else
                net->routing[i][j] = 9999;
            j = j + 1;
        }
        i = i + 1;
    }
    i = 0;
    while (i < net->edge_count) {
        int f = net->edges[i].from;
        int t = net->edges[i].to;
        net->routing[f][t] = net->edges[i].weight;
        i = i + 1;
    }
}

void floyd_warshall(struct Network *net) {
    int n = net->node_count;
    int k = 0;
    while (k < n) {
        int i = 0;
        while (i < n) {
            int j = 0;
            while (j < n) {
                int through_k = net->routing[i][k]
                              + net->routing[k][j];
                if (through_k < net->routing[i][j]) {
                    net->routing[i][j] = through_k;
                }
                j = j + 1;
            }
            i = i + 1;
        }
        k = k + 1;
    }
}

int get_distance(struct Network *net, int from, int to) {
    return net->routing[from][to];
}

int count_reachable(struct Network *net, int from) {
    int c = 0;
    int i = 0;
    while (i < net->node_count) {
        if (i != from && net->routing[from][i] < 9999) {
            c = c + 1;
        }
        i = i + 1;
    }
    return c;
}

int hop_count(struct Network *net, int from, int to) {
    if (from == to) return 0;
    if (net->routing[from][to] >= 9999) return -1;
    int visited[16];
    int dist[16];
    int i = 0;
    while (i < 16) {
        visited[i] = 0;
        dist[i] = 9999;
        i = i + 1;
    }
    dist[from] = 0;
    int step = 0;
    while (step < net->node_count) {
        int u = -1;
        int best = 9999;
        i = 0;
        while (i < net->node_count) {
            if (!visited[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
            i = i + 1;
        }
        if (u < 0) break;
        visited[u] = 1;
        i = 0;
        while (i < net->edge_count) {
            if (net->edges[i].from == u) {
                int v = net->edges[i].to;
                if (dist[u] + 1 < dist[v]) {
                    dist[v] = dist[u] + 1;
                }
            }
            i = i + 1;
        }
        step = step + 1;
    }
    return dist[to];
}

int degree(struct Network *net, int node) {
    int d = 0;
    int i = 0;
    while (i < net->edge_count) {
        if (net->edges[i].from == node) d = d + 1;
        i = i + 1;
    }
    return d;
}

int main(void) {
    struct Network net;
    net.node_count = 0;
    net.edge_count = 0;

    int a = add_node(&net, "A");
    int b = add_node(&net, "B");
    int c = add_node(&net, "C");
    int d = add_node(&net, "D");
    int e = add_node(&net, "E");

    add_edge(&net, a, b, 4);
    add_edge(&net, a, c, 2);
    add_edge(&net, b, c, 1);
    add_edge(&net, b, d, 5);
    add_edge(&net, c, d, 8);
    add_edge(&net, d, e, 3);

    // EXPECT: nodes=5
    printf("nodes=%d\n", net.node_count);

    // EXPECT: edges=12
    printf("edges=%d\n", net.edge_count);

    init_routing(&net);
    floyd_warshall(&net);

    // EXPECT: A_to_B=3
    printf("A_to_B=%d\n", get_distance(&net, a, b));

    // EXPECT: A_to_C=2
    printf("A_to_C=%d\n", get_distance(&net, a, c));

    // EXPECT: A_to_D=8
    printf("A_to_D=%d\n", get_distance(&net, a, d));

    // EXPECT: A_to_E=11
    printf("A_to_E=%d\n", get_distance(&net, a, e));

    // EXPECT: B_to_E=8
    printf("B_to_E=%d\n", get_distance(&net, b, e));

    // EXPECT: reachable_A=4
    printf("reachable_A=%d\n", count_reachable(&net, a));

    // EXPECT: reachable_E=4
    printf("reachable_E=%d\n", count_reachable(&net, e));

    // EXPECT: hops_A_E=3
    printf("hops_A_E=%d\n", hop_count(&net, a, e));

    // EXPECT: hops_A_B=1
    printf("hops_A_B=%d\n", hop_count(&net, a, b));

    // EXPECT: hops_A_D=2
    printf("hops_A_D=%d\n", hop_count(&net, a, d));

    // EXPECT: hops_A_A=0
    printf("hops_A_A=%d\n", hop_count(&net, a, a));

    // EXPECT: deg_A=2
    printf("deg_A=%d\n", degree(&net, a));

    // EXPECT: deg_B=3
    printf("deg_B=%d\n", degree(&net, b));

    // EXPECT: deg_D=3
    printf("deg_D=%d\n", degree(&net, d));

    return 0;
}
