int printf(const char *fmt, ...);
// EXPECT: Vertices: 6, Edges: 18\nBFS from 0: 0 2 1 4 3 5\nDFS from 0: 0 1 2 3 4 5\nDijkstra from 0: d[0]=0 d[1]=3 d[2]=2 d[3]=8 d[4]=10 d[5]=13\nDone

// Graph with adjacency list using arrays

struct Edge {
    int to;
    int weight;
    int next;  // index of next edge in list
};

struct Graph {
    struct Edge edges[64];
    int head[16];
    int num_vertices;
    int num_edges;
};

void graph_init(struct Graph *g, int v) {
    g->num_vertices = v;
    g->num_edges = 0;
    int i = 0;
    while (i < 16) {
        g->head[i] = -1;
        i = i + 1;
    }
}

void graph_add_edge(struct Graph *g, int from, int to, int weight) {
    int e = g->num_edges;
    g->edges[e].to = to;
    g->edges[e].weight = weight;
    g->edges[e].next = g->head[from];
    g->head[from] = e;
    g->num_edges = g->num_edges + 1;
}

void graph_add_undirected(struct Graph *g, int u, int v, int w) {
    graph_add_edge(g, u, v, w);
    graph_add_edge(g, v, u, w);
}

// BFS
void graph_bfs(struct Graph *g, int start) {
    int visited[16];
    int queue[16];
    int front = 0;
    int back = 0;
    int i = 0;
    while (i < 16) {
        visited[i] = 0;
        i = i + 1;
    }

    visited[start] = 1;
    queue[back] = start;
    back = back + 1;

    printf("BFS from %d:", start);
    while (front < back) {
        int v = queue[front];
        front = front + 1;
        printf(" %d", v);

        int e = g->head[v];
        while (e >= 0) {
            int to = g->edges[e].to;
            if (!visited[to]) {
                visited[to] = 1;
                queue[back] = to;
                back = back + 1;
            }
            e = g->edges[e].next;
        }
    }
    printf("\n");
}

// DFS iterative
void graph_dfs(struct Graph *g, int start) {
    int visited[16];
    int stack[16];
    int top = 0;
    int i = 0;
    while (i < 16) {
        visited[i] = 0;
        i = i + 1;
    }

    stack[top] = start;
    top = top + 1;

    printf("DFS from %d:", start);
    while (top > 0) {
        top = top - 1;
        int v = stack[top];
        if (visited[v]) continue;
        visited[v] = 1;
        printf(" %d", v);

        int e = g->head[v];
        while (e >= 0) {
            if (!visited[g->edges[e].to]) {
                stack[top] = g->edges[e].to;
                top = top + 1;
            }
            e = g->edges[e].next;
        }
    }
    printf("\n");
}

// Dijkstra (simple O(V^2))
void graph_dijkstra(struct Graph *g, int start) {
    int dist[16];
    int visited[16];
    int i = 0;
    while (i < g->num_vertices) {
        dist[i] = 999999;
        visited[i] = 0;
        i = i + 1;
    }
    dist[start] = 0;

    int count = 0;
    while (count < g->num_vertices) {
        int u = -1;
        int min_d = 999999;
        i = 0;
        while (i < g->num_vertices) {
            if (!visited[i] && dist[i] < min_d) {
                min_d = dist[i];
                u = i;
            }
            i = i + 1;
        }
        if (u < 0) break;
        visited[u] = 1;

        int e = g->head[u];
        while (e >= 0) {
            int v = g->edges[e].to;
            int w = g->edges[e].weight;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
            }
            e = g->edges[e].next;
        }
        count = count + 1;
    }

    printf("Dijkstra from %d:", start);
    i = 0;
    while (i < g->num_vertices) {
        printf(" d[%d]=%d", i, dist[i]);
        i = i + 1;
    }
    printf("\n");
}

int main(void) {
    struct Graph g;
    graph_init(&g, 6);

    graph_add_undirected(&g, 0, 1, 4);
    graph_add_undirected(&g, 0, 2, 2);
    graph_add_undirected(&g, 1, 2, 1);
    graph_add_undirected(&g, 1, 3, 5);
    graph_add_undirected(&g, 2, 3, 8);
    graph_add_undirected(&g, 2, 4, 10);
    graph_add_undirected(&g, 3, 4, 2);
    graph_add_undirected(&g, 3, 5, 6);
    graph_add_undirected(&g, 4, 5, 3);

    printf("Vertices: %d, Edges: %d\n", g.num_vertices, g.num_edges);

    graph_bfs(&g, 0);
    graph_dfs(&g, 0);
    graph_dijkstra(&g, 0);

    printf("Done\n");
    return 0;
}
