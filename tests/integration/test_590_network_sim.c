int printf(const char *fmt, ...);
// EXPECT: Network topology:\n    A    B    C    D    E    \nA   0    4    2    0    0    \nB   4    0    1    5    0    \nC   2    1    0    8    10   \nD   0    5    8    0    2    \nE   0    0    10   2    0    \n\nShortest paths from A:\n  A -> B: cost=3\n  A -> C: cost=2\n  A -> D: cost=8\n  A -> E: cost=10\n\nShortest paths from D:\n  D -> A: cost=8\n  D -> B: cost=5\n  D -> C: cost=6\n  D -> E: cost=2\n\nTotal bandwidth: 32\nNodes: 5
#define MAX_NODES 6
#define MAX_EDGES 10
#define INF 99999

struct Edge {
    int from;
    int to;
    int weight;
};

struct Network {
    int adj[MAX_NODES][MAX_NODES];
    char *names[MAX_NODES];
    int num_nodes;
};

void net_init(struct Network *n) {
    n->num_nodes = 0;
    int i;
    int j;
    for (i = 0; i < MAX_NODES; i++) {
        for (j = 0; j < MAX_NODES; j++) {
            n->adj[i][j] = 0;
        }
    }
}

int net_add_node(struct Network *n, char *name) {
    if (n->num_nodes >= MAX_NODES) return -1;
    n->names[n->num_nodes] = name;
    n->num_nodes++;
    return n->num_nodes - 1;
}

void net_add_edge(struct Network *n, int from, int to, int weight) {
    n->adj[from][to] = weight;
    n->adj[to][from] = weight;
}

void net_print(struct Network *n) {
    int i;
    int j;
    printf("    ");
    for (i = 0; i < n->num_nodes; i++) printf("%-5s", n->names[i]);
    printf("\n");
    for (i = 0; i < n->num_nodes; i++) {
        printf("%-4s", n->names[i]);
        for (j = 0; j < n->num_nodes; j++) {
            printf("%-5d", n->adj[i][j]);
        }
        printf("\n");
    }
}

void dijkstra(struct Network *n, int src) {
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    int prev[MAX_NODES];
    int i;
    int j;

    for (i = 0; i < n->num_nodes; i++) {
        dist[i] = INF;
        visited[i] = 0;
        prev[i] = -1;
    }
    dist[src] = 0;

    int count;
    for (count = 0; count < n->num_nodes; count++) {
        int u = -1;
        int min_dist = INF;
        for (i = 0; i < n->num_nodes; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                u = i;
            }
        }
        if (u == -1) break;
        visited[u] = 1;

        for (j = 0; j < n->num_nodes; j++) {
            if (n->adj[u][j] > 0 && !visited[j]) {
                int new_dist = dist[u] + n->adj[u][j];
                if (new_dist < dist[j]) {
                    dist[j] = new_dist;
                    prev[j] = u;
                }
            }
        }
    }

    printf("Shortest paths from %s:\n", n->names[src]);
    for (i = 0; i < n->num_nodes; i++) {
        if (i == src) continue;
        if (dist[i] == INF) {
            printf("  %s -> %s: unreachable\n", n->names[src], n->names[i]);
        } else {
            printf("  %s -> %s: cost=%d\n", n->names[src], n->names[i], dist[i]);
        }
    }
}

int net_total_bandwidth(struct Network *n) {
    int total = 0;
    int i;
    int j;
    for (i = 0; i < n->num_nodes; i++) {
        for (j = i + 1; j < n->num_nodes; j++) {
            total = total + n->adj[i][j];
        }
    }
    return total;
}

int main(void) {
    struct Network net;
    net_init(&net);

    int a = net_add_node(&net, "A");
    int b = net_add_node(&net, "B");
    int c = net_add_node(&net, "C");
    int d = net_add_node(&net, "D");
    int e = net_add_node(&net, "E");

    net_add_edge(&net, a, b, 4);
    net_add_edge(&net, a, c, 2);
    net_add_edge(&net, b, c, 1);
    net_add_edge(&net, b, d, 5);
    net_add_edge(&net, c, d, 8);
    net_add_edge(&net, c, e, 10);
    net_add_edge(&net, d, e, 2);

    printf("Network topology:\n");
    net_print(&net);

    printf("\n");
    dijkstra(&net, a);

    printf("\n");
    dijkstra(&net, d);

    printf("\nTotal bandwidth: %d\n", net_total_bandwidth(&net));
    printf("Nodes: %d\n", net.num_nodes);

    return 0;
}
