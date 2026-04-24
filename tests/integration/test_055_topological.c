int printf(const char *fmt, ...);
// EXPECT: T1:cycle=0 edges=6 dfs:5 4 2 3 1 0 valid=1 T2:cnt=6 order:4 5 0 2 3 1 valid=1 T3:cycle=1 kahn=1 hasCycle=1 T4:cycle=0 kahn:0 1 2 3 4 5 6 7 src=1 sink=1 T5:cycle=0 kahn:0 1 2 3 valid=1 T6:cycle=0 src=2 sink=2 valid=1 T7:r03=1 r05=1 r35=0 r30=0 r50=0 T8:cycle=0 lp=3 T9:cycle=0 src=1 sink=9 edges=9 T10:cycle=0 r40=1 r04=0 k0=4 k4=0 T11:cycle=0 edges=10 lp=4 valid=1 src=1 sink=1 T12:cycle=1 DONE

/* =========================================================================
 * Test 055: Topological Sort Using DFS on Adjacency Matrix
 * =========================================================================
 * Implements topological sort via DFS, cycle detection,
 * in-degree based (Kahn's) topological sort, and reachability checks.
 * ========================================================================= */

/* Adjacency matrix for up to 20 nodes */
int adj[20][20];
int visited[20];
int rec_stack[20];
int topo_order[20];
int topo_idx;
int num_nodes;

void graph_init(int n) {
    int i;
    int j;
    num_nodes = n;
    topo_idx = 0;
    for (i = 0; i < n; i = i + 1) {
        visited[i] = 0;
        rec_stack[i] = 0;
        topo_order[i] = -1;
        for (j = 0; j < n; j = j + 1) {
            adj[i][j] = 0;
        }
    }
}

void add_edge(int u, int v) {
    adj[u][v] = 1;
}

/* DFS-based topological sort helper. Returns 1 if cycle detected. */
int dfs_topo(int u) {
    int v;
    visited[u] = 1;
    rec_stack[u] = 1;
    for (v = 0; v < num_nodes; v = v + 1) {
        if (adj[u][v]) {
            if (visited[v] == 0) {
                if (dfs_topo(v)) {
                    return 1;
                }
            } else if (rec_stack[v]) {
                return 1;
            }
        }
    }
    rec_stack[u] = 0;
    topo_order[topo_idx] = u;
    topo_idx = topo_idx + 1;
    return 0;
}

/* Perform topological sort. Returns 1 if cycle, 0 if success. */
int topological_sort(void) {
    int i;
    topo_idx = 0;
    for (i = 0; i < num_nodes; i = i + 1) {
        visited[i] = 0;
        rec_stack[i] = 0;
    }
    for (i = 0; i < num_nodes; i = i + 1) {
        if (visited[i] == 0) {
            if (dfs_topo(i)) {
                return 1;
            }
        }
    }
    return 0;
}

/* In-degree based topological sort (Kahn's algorithm) */
int indegree[20];
int kahn_order[20];
int kahn_queue[20];

int kahn_sort(void) {
    int i;
    int j;
    int front;
    int back;
    int u;
    int count;

    for (i = 0; i < num_nodes; i = i + 1) {
        indegree[i] = 0;
    }
    for (i = 0; i < num_nodes; i = i + 1) {
        for (j = 0; j < num_nodes; j = j + 1) {
            if (adj[i][j]) {
                indegree[j] = indegree[j] + 1;
            }
        }
    }

    front = 0;
    back = 0;
    for (i = 0; i < num_nodes; i = i + 1) {
        if (indegree[i] == 0) {
            kahn_queue[back] = i;
            back = back + 1;
        }
    }

    count = 0;
    while (front < back) {
        u = kahn_queue[front];
        front = front + 1;
        kahn_order[count] = u;
        count = count + 1;
        for (j = 0; j < num_nodes; j = j + 1) {
            if (adj[u][j]) {
                indegree[j] = indegree[j] - 1;
                if (indegree[j] == 0) {
                    kahn_queue[back] = j;
                    back = back + 1;
                }
            }
        }
    }

    return count;
}

/* Check if topological order is valid */
int verify_topo(int *order, int n) {
    int pos[20];
    int i;
    int j;
    for (i = 0; i < n; i = i + 1) {
        pos[order[i]] = i;
    }
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < n; j = j + 1) {
            if (adj[i][j]) {
                if (pos[i] >= pos[j]) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

/* Compute reachability: can we reach v from u? */
int can_reach(int u, int v) {
    int vis[20];
    int stack[20];
    int top;
    int cur;
    int w;
    int i;

    for (i = 0; i < num_nodes; i = i + 1) {
        vis[i] = 0;
    }
    top = 0;
    stack[top] = u;
    top = top + 1;
    vis[u] = 1;

    while (top > 0) {
        top = top - 1;
        cur = stack[top];
        if (cur == v) {
            return 1;
        }
        for (w = 0; w < num_nodes; w = w + 1) {
            if (adj[cur][w] && vis[w] == 0) {
                vis[w] = 1;
                stack[top] = w;
                top = top + 1;
            }
        }
    }
    return 0;
}

/* Count edges */
int count_edges(void) {
    int cnt;
    int i;
    int j;
    cnt = 0;
    for (i = 0; i < num_nodes; i = i + 1) {
        for (j = 0; j < num_nodes; j = j + 1) {
            if (adj[i][j]) {
                cnt = cnt + 1;
            }
        }
    }
    return cnt;
}

/* Count source nodes (zero in-degree) */
int count_sources(void) {
    int i;
    int j;
    int cnt;
    int has_in;
    cnt = 0;
    for (j = 0; j < num_nodes; j = j + 1) {
        has_in = 0;
        for (i = 0; i < num_nodes; i = i + 1) {
            if (adj[i][j]) {
                has_in = 1;
            }
        }
        if (has_in == 0) {
            cnt = cnt + 1;
        }
    }
    return cnt;
}

/* Count sink nodes (zero out-degree) */
int count_sinks(void) {
    int i;
    int j;
    int cnt;
    int has_out;
    cnt = 0;
    for (i = 0; i < num_nodes; i = i + 1) {
        has_out = 0;
        for (j = 0; j < num_nodes; j = j + 1) {
            if (adj[i][j]) {
                has_out = 1;
            }
        }
        if (has_out == 0) {
            cnt = cnt + 1;
        }
    }
    return cnt;
}

/* Compute longest path in DAG */
int dp_dist[20];

int longest_path(void) {
    int i;
    int j;
    int k;
    int max_d;

    for (i = 0; i < num_nodes; i = i + 1) {
        dp_dist[i] = 0;
    }

    /* Process in reverse topological order (topo_order is reverse post-order) */
    for (k = num_nodes - 1; k >= 0; k = k - 1) {
        i = topo_order[k];
        for (j = 0; j < num_nodes; j = j + 1) {
            if (adj[i][j]) {
                if (dp_dist[i] + 1 > dp_dist[j]) {
                    dp_dist[j] = dp_dist[i] + 1;
                }
            }
        }
    }

    max_d = 0;
    for (i = 0; i < num_nodes; i = i + 1) {
        if (dp_dist[i] > max_d) {
            max_d = dp_dist[i];
        }
    }
    return max_d;
}

/* =========================================================================
 * Test functions
 * ========================================================================= */

void test_basic_topo(void) {
    int i;
    int cycle;

    printf("T1:");
    graph_init(6);

    /* DAG: 5->2, 5->0, 4->0, 4->1, 2->3, 3->1 */
    add_edge(5, 2);
    add_edge(5, 0);
    add_edge(4, 0);
    add_edge(4, 1);
    add_edge(2, 3);
    add_edge(3, 1);

    cycle = topological_sort();
    printf("cycle=%d", cycle);
    printf(" edges=%d", count_edges());

    /* Print DFS topo order (reversed) */
    printf(" dfs:");
    for (i = num_nodes - 1; i >= 0; i = i - 1) {
        printf("%d ", topo_order[i]);
    }

    /* Verify */
    {
        int rev[6];
        int a;
        for (a = 0; a < 6; a = a + 1) {
            rev[a] = topo_order[5 - a];
        }
        printf("valid=%d", verify_topo(rev, 6));
    }
}

void test_kahn(void) {
    int cnt;
    int i;

    printf(" T2:");
    graph_init(6);

    add_edge(5, 2);
    add_edge(5, 0);
    add_edge(4, 0);
    add_edge(4, 1);
    add_edge(2, 3);
    add_edge(3, 1);

    cnt = kahn_sort();
    printf("cnt=%d", cnt);
    printf(" order:");
    for (i = 0; i < cnt; i = i + 1) {
        printf("%d ", kahn_order[i]);
    }
    printf("valid=%d", verify_topo(kahn_order, cnt));
}

void test_cycle_detection(void) {
    int cycle;

    printf(" T3:");
    graph_init(4);

    /* Cycle: 0->1->2->3->1 */
    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(3, 1);

    cycle = topological_sort();
    printf("cycle=%d", cycle);

    {
        int cnt;
        cnt = kahn_sort();
        printf(" kahn=%d", cnt);
        printf(" hasCycle=%d", cnt < num_nodes);
    }
}

void test_linear_chain(void) {
    int i;
    int cycle;

    printf(" T4:");
    graph_init(8);

    for (i = 0; i < 7; i = i + 1) {
        add_edge(i, i + 1);
    }

    cycle = topological_sort();
    printf("cycle=%d", cycle);

    {
        int cnt;
        cnt = kahn_sort();
        printf(" kahn:");
        for (i = 0; i < cnt; i = i + 1) {
            printf("%d ", kahn_order[i]);
        }
    }
    printf("src=%d", count_sources());
    printf(" sink=%d", count_sinks());
}

void test_diamond(void) {
    int cycle;
    int cnt;
    int i;

    printf(" T5:");
    graph_init(4);

    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(1, 3);
    add_edge(2, 3);

    cycle = topological_sort();
    printf("cycle=%d", cycle);

    cnt = kahn_sort();
    printf(" kahn:");
    for (i = 0; i < cnt; i = i + 1) {
        printf("%d ", kahn_order[i]);
    }
    printf("valid=%d", verify_topo(kahn_order, cnt));
}

void test_disconnected(void) {
    int cycle;
    int cnt;

    printf(" T6:");
    graph_init(6);

    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(3, 4);
    add_edge(4, 5);

    cycle = topological_sort();
    printf("cycle=%d", cycle);
    printf(" src=%d", count_sources());
    printf(" sink=%d", count_sinks());

    cnt = kahn_sort();
    printf(" valid=%d", verify_topo(kahn_order, cnt));
}

void test_reachability(void) {
    printf(" T7:");
    graph_init(6);

    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(0, 4);
    add_edge(4, 5);

    printf("r03=%d", can_reach(0, 3));
    printf(" r05=%d", can_reach(0, 5));
    printf(" r35=%d", can_reach(3, 5));
    printf(" r30=%d", can_reach(3, 0));
    printf(" r50=%d", can_reach(5, 0));
}

void test_longest_path(void) {
    int cycle;

    printf(" T8:");
    graph_init(7);

    add_edge(0, 1);
    add_edge(1, 2);
    add_edge(2, 3);
    add_edge(0, 4);
    add_edge(4, 5);
    add_edge(5, 6);
    add_edge(4, 3);

    cycle = topological_sort();
    printf("cycle=%d", cycle);
    printf(" lp=%d", longest_path());
}

void test_wide_graph(void) {
    int i;
    int cycle;

    printf(" T9:");
    graph_init(10);

    for (i = 1; i < 10; i = i + 1) {
        add_edge(0, i);
    }

    cycle = topological_sort();
    printf("cycle=%d", cycle);
    printf(" src=%d", count_sources());
    printf(" sink=%d", count_sinks());
    printf(" edges=%d", count_edges());
}

void test_reverse_chain(void) {
    int i;
    int cycle;

    printf(" T10:");
    graph_init(5);

    for (i = 4; i > 0; i = i - 1) {
        add_edge(i, i - 1);
    }

    cycle = topological_sort();
    printf("cycle=%d", cycle);
    printf(" r40=%d", can_reach(4, 0));
    printf(" r04=%d", can_reach(0, 4));

    {
        int cnt;
        cnt = kahn_sort();
        printf(" k0=%d", kahn_order[0]);
        printf(" k4=%d", kahn_order[4]);
    }
}

void test_complex_dag(void) {
    int cycle;
    int cnt;

    printf(" T11:");
    graph_init(8);

    add_edge(0, 1);
    add_edge(0, 2);
    add_edge(1, 3);
    add_edge(1, 4);
    add_edge(2, 4);
    add_edge(2, 5);
    add_edge(3, 6);
    add_edge(4, 6);
    add_edge(5, 7);
    add_edge(6, 7);

    cycle = topological_sort();
    printf("cycle=%d", cycle);
    printf(" edges=%d", count_edges());
    printf(" lp=%d", longest_path());

    cnt = kahn_sort();
    printf(" valid=%d", verify_topo(kahn_order, cnt));
    printf(" src=%d", count_sources());
    printf(" sink=%d", count_sinks());
}

void test_self_loop(void) {
    int cycle;

    printf(" T12:");
    graph_init(3);

    add_edge(0, 1);
    add_edge(1, 1);
    add_edge(1, 2);

    cycle = topological_sort();
    printf("cycle=%d", cycle);
    printf(" DONE\n");
}

int main(void) {
    test_basic_topo();
    test_kahn();
    test_cycle_detection();
    test_linear_chain();
    test_diamond();
    test_disconnected();
    test_reachability();
    test_longest_path();
    test_wide_graph();
    test_reverse_chain();
    test_complex_dag();
    test_self_loop();
    return 0;
}
