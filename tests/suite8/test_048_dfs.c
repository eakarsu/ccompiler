int printf(const char *fmt, ...);

/* Depth-First Search on a graph (adjacency matrix), iterative + recursive */

int adj[10][10];
int nv;

void init_g(int n) {
    int i; int j;
    nv = n;
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++)
            adj[i][j] = 0;
}

void add_e(int u, int v) {
    adj[u][v] = 1;
    adj[v][u] = 1;
}

/* Recursive DFS */
int vis_r[10];
int rec_order[10];
int rec_cnt;

void dfs_rec(int u) {
    int i;
    vis_r[u] = 1;
    rec_order[rec_cnt] = u;
    rec_cnt = rec_cnt + 1;
    for (i = 0; i < nv; i++) {
        if (adj[u][i] && vis_r[i] == 0) {
            dfs_rec(i);
        }
    }
}

/* Iterative DFS using explicit stack */
int vis_i[10];
int iter_order[10];
int iter_cnt;

int stack[20];
int sp;

void stack_push(int v) {
    stack[sp] = v;
    sp = sp + 1;
}

int stack_pop(void) {
    sp = sp - 1;
    return stack[sp];
}

int stack_empty(void) {
    return sp == 0;
}

void dfs_iter(int start) {
    int cur;
    int i;

    for (i = 0; i < nv; i++) vis_i[i] = 0;
    iter_cnt = 0;
    sp = 0;

    stack_push(start);

    while (stack_empty() == 0) {
        cur = stack_pop();
        if (vis_i[cur]) continue;
        vis_i[cur] = 1;
        iter_order[iter_cnt] = cur;
        iter_cnt = iter_cnt + 1;

        /* Push neighbors in reverse order for consistent ordering */
        for (i = nv - 1; i >= 0; i--) {
            if (adj[cur][i] && vis_i[i] == 0) {
                stack_push(i);
            }
        }
    }
}

/* Check if graph has a cycle (undirected) using DFS */
int vis_c[10];

int has_cycle_dfs(int u, int parent_v) {
    int i;
    vis_c[u] = 1;
    for (i = 0; i < nv; i++) {
        if (adj[u][i] == 0) continue;
        if (vis_c[i] == 0) {
            if (has_cycle_dfs(i, u)) return 1;
        } else if (i != parent_v) {
            return 1;
        }
    }
    return 0;
}

int has_cycle(void) {
    int i;
    for (i = 0; i < nv; i++) vis_c[i] = 0;
    return has_cycle_dfs(0, -1);
}

/* Count connected components */
int vis_cc[10];

void dfs_mark(int u) {
    int i;
    vis_cc[u] = 1;
    for (i = 0; i < nv; i++) {
        if (adj[u][i] && vis_cc[i] == 0) {
            dfs_mark(i);
        }
    }
}

int count_components(void) {
    int i;
    int cnt;
    for (i = 0; i < nv; i++) vis_cc[i] = 0;
    cnt = 0;
    for (i = 0; i < nv; i++) {
        if (vis_cc[i] == 0) {
            dfs_mark(i);
            cnt = cnt + 1;
        }
    }
    return cnt;
}

int main(void) {
    int i;

    /* Graph:
       0 -- 1 -- 2
       |         |
       3    4 -- 5
            |
            6
    */
    init_g(7);
    add_e(0, 1);
    add_e(1, 2);
    add_e(0, 3);
    add_e(2, 5);
    add_e(4, 5);
    add_e(4, 6);

    /* Recursive DFS from 0 */
    for (i = 0; i < nv; i++) vis_r[i] = 0;
    rec_cnt = 0;
    dfs_rec(0);

    // EXPECT: DFS recursive: 0 1 2 5 4 6 3
    printf("DFS recursive:");
    for (i = 0; i < rec_cnt; i++) {
        printf(" %d", rec_order[i]);
    }
    printf("\n");

    /* Iterative DFS from 0 */
    dfs_iter(0);

    // EXPECT: DFS iterative: 0 1 2 5 4 6 3
    printf("DFS iterative:");
    for (i = 0; i < iter_cnt; i++) {
        printf(" %d", iter_order[i]);
    }
    printf("\n");

    // EXPECT: has_cycle=0
    printf("has_cycle=%d\n", has_cycle());

    // EXPECT: components=1
    printf("components=%d\n", count_components());

    /* Disconnect node 6 by removing edges */
    adj[4][6] = 0;
    adj[6][4] = 0;

    // EXPECT: components_after=2
    printf("components_after=%d\n", count_components());

    /* Tree (no cycle): remove edge 2-5 to break cycle */
    adj[2][5] = 0;
    adj[5][2] = 0;
    /* Now 0-1-2, 0-3, 4-5 are separate, 6 separate */
    /* Check cycle on the 0-1-2-3 component */
    init_g(4);
    add_e(0, 1);
    add_e(1, 2);
    add_e(0, 3);
    // EXPECT: tree_cycle=0
    printf("tree_cycle=%d\n", has_cycle());

    return 0;
}
