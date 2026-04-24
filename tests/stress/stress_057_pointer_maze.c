// stress_057_pointer_maze.c - Pointer maze stress test
// Build a graph of nodes, traverse paths, detect cycles, count reachable nodes.
// EXPECT: built=10 nodes
// EXPECT: reachable_from_0=10
// EXPECT: path_0_to_9=found
// EXPECT: path_len=9
// EXPECT: cycle_detect=cycle found at node 5
// EXPECT: no_cycle=no cycle
// EXPECT: null_safe=safe
// EXPECT: ptr_eq=equal
// EXPECT: ptr_ne=not equal
// EXPECT: reverse_list=10 9 8 7 6 5 4 3 2 1
// EXPECT: double_ptr=42
// EXPECT: self_ref=self ok
// EXPECT: pointer_tests=12

int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

// ---- Graph node ----
struct GNode {
    int id;
    struct GNode *next;      // linked list of adjacency (simplified: single edge)
    struct GNode *adj[4];    // up to 4 neighbors
    int adj_count;
    int visited;
};

struct GNode nodes[10];

void init_nodes(void) {
    int i;
    for (i = 0; i < 10; i++) {
        nodes[i].id = i;
        nodes[i].next = (struct GNode *)0;
        nodes[i].adj_count = 0;
        nodes[i].visited = 0;
        int j;
        for (j = 0; j < 4; j++) {
            nodes[i].adj[j] = (struct GNode *)0;
        }
    }
}

void add_edge(struct GNode *from, struct GNode *to) {
    if (from->adj_count < 4) {
        from->adj[from->adj_count++] = to;
    }
}

// BFS using array as queue
int count_reachable(struct GNode *start) {
    struct GNode *queue[10];
    int qhead = 0, qtail = 0;
    int visited[10];
    int i;
    for (i = 0; i < 10; i++) visited[i] = 0;

    queue[qtail++] = start;
    visited[start->id] = 1;
    int count = 0;

    while (qhead < qtail) {
        struct GNode *cur = queue[qhead++];
        count++;
        for (i = 0; i < cur->adj_count; i++) {
            struct GNode *neighbor = cur->adj[i];
            if (neighbor && !visited[neighbor->id]) {
                visited[neighbor->id] = 1;
                queue[qtail++] = neighbor;
            }
        }
    }
    return count;
}

// DFS path finding
int find_path(struct GNode *start, struct GNode *target, int *path, int depth, int *visited) {
    path[depth] = start->id;
    if (start == target) return depth + 1;
    visited[start->id] = 1;
    int i;
    for (i = 0; i < start->adj_count; i++) {
        struct GNode *neighbor = start->adj[i];
        if (neighbor && !visited[neighbor->id]) {
            int result = find_path(neighbor, target, path, depth + 1, visited);
            if (result > 0) return result;
        }
    }
    visited[start->id] = 0;
    return 0;
}

// Tortoise and hare cycle detection on linked list
int detect_cycle(struct GNode *head) {
    struct GNode *slow = head;
    struct GNode *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next;
        if (fast) fast = fast->next;
        if (slow == fast) return slow->id;
    }
    return -1;
}

// ---- Linked list for reversal test ----
struct LNode {
    int val;
    struct LNode *next;
};

struct LNode lnodes[10];

struct LNode *build_list(int n) {
    int i;
    for (i = 0; i < n; i++) {
        lnodes[i].val = i + 1;
        if (i < n - 1)
            lnodes[i].next = &lnodes[i + 1];
        else
            lnodes[i].next = (struct LNode *)0;
    }
    return &lnodes[0];
}

struct LNode *reverse_list(struct LNode *head) {
    struct LNode *prev = (struct LNode *)0;
    struct LNode *cur = head;
    while (cur) {
        struct LNode *tmp = cur->next;
        cur->next = prev;
        prev = cur;
        cur = tmp;
    }
    return prev;
}

int main(void) {
    int tests = 0;

    // Build linear graph: 0->1->2->...->9
    init_nodes();
    int i;
    for (i = 0; i < 9; i++) {
        add_edge(&nodes[i], &nodes[i + 1]);
    }
    printf("built=%d nodes\n", 10);
    tests++;

    // Count reachable from node 0
    int r = count_reachable(&nodes[0]);
    printf("reachable_from_0=%d\n", r);
    tests++;

    // Find path from 0 to 9
    int path[10];
    int visited[10];
    for (i = 0; i < 10; i++) visited[i] = 0;
    int plen = find_path(&nodes[0], &nodes[9], path, 0, visited);
    if (plen > 0)
        printf("path_0_to_9=found\n");
    else
        printf("path_0_to_9=not found\n");
    tests++;

    printf("path_len=%d\n", plen - 1);
    tests++;

    // Cycle detection: create cycle 0->1->2->...->5->...->9, then 9->5
    // Build as linked list via .next
    for (i = 0; i < 9; i++) {
        nodes[i].next = &nodes[i + 1];
    }
    nodes[9].next = &nodes[5]; // cycle back to node 5
    int cycle_node = detect_cycle(&nodes[0]);
    printf("cycle_detect=cycle found at node %d\n", cycle_node);
    tests++;

    // No cycle test
    nodes[9].next = (struct GNode *)0; // remove cycle
    int no_cycle = detect_cycle(&nodes[0]);
    if (no_cycle == -1)
        printf("no_cycle=no cycle\n");
    else
        printf("no_cycle=cycle at %d\n", no_cycle);
    tests++;

    // NULL pointer safety
    struct GNode *null_ptr = (struct GNode *)0;
    if (!null_ptr)
        printf("null_safe=safe\n");
    else
        printf("null_safe=FAIL\n");
    tests++;

    // Pointer equality
    struct GNode *p1 = &nodes[3];
    struct GNode *p2 = &nodes[3];
    struct GNode *p3 = &nodes[4];
    if (p1 == p2)
        printf("ptr_eq=equal\n");
    else
        printf("ptr_eq=FAIL\n");
    tests++;

    if (p1 != p3)
        printf("ptr_ne=not equal\n");
    else
        printf("ptr_ne=FAIL\n");
    tests++;

    // Reverse linked list
    struct LNode *list = build_list(10);
    struct LNode *rev = reverse_list(list);
    printf("reverse_list=");
    struct LNode *cur = rev;
    while (cur) {
        printf("%d", cur->val);
        if (cur->next) printf(" ");
        cur = cur->next;
    }
    printf("\n");
    tests++;

    // Double pointer (pointer to pointer)
    int val = 42;
    int *ptr = &val;
    int **dptr = &ptr;
    printf("double_ptr=%d\n", **dptr);
    tests++;

    // Self-referential pointer
    struct GNode self;
    self.id = 99;
    self.adj[0] = &self;
    self.adj_count = 1;
    if (self.adj[0]->adj[0]->id == 99)
        printf("self_ref=self ok\n");
    else
        printf("self_ref=FAIL\n");
    tests++;

    printf("pointer_tests=%d\n", tests);

    return 0;
}
