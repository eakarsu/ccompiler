// stress_031_self_ref_struct.c - Self-referential struct patterns
// EXPECT: sll_sum=55
// EXPECT: sll_rev=1 2 3 4 5 6 7 8 9 10
// EXPECT: dll_fwd=1 2 3 4 5
// EXPECT: dll_bwd=5 4 3 2 1
// EXPECT: tree_inorder= 1 2 3 4 5 6 7
// EXPECT: tree_sum=28
// EXPECT: graph_adj_0=2 1
// EXPECT: graph_adj_1=2 0
// EXPECT: graph_adj_2=1 0
// EXPECT: circular_count=10
// EXPECT: circular_sum=55
// EXPECT: all_freed=1

int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

// --- Singly linked list ---
struct SLLNode {
    int val;
    struct SLLNode *next;
};

struct SLLNode *sll_push(struct SLLNode *head, int val) {
    struct SLLNode *n = malloc(sizeof(struct SLLNode));
    n->val = val;
    n->next = head;
    return n;
}

int sll_sum(struct SLLNode *head) {
    int s = 0;
    struct SLLNode *cur = head;
    while (cur) {
        s = s + cur->val;
        cur = cur->next;
    }
    return s;
}

struct SLLNode *sll_reverse(struct SLLNode *head) {
    struct SLLNode *prev = 0;
    struct SLLNode *cur = head;
    struct SLLNode *next;
    while (cur) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    return prev;
}

void sll_free(struct SLLNode *head) {
    struct SLLNode *cur = head;
    struct SLLNode *next;
    while (cur) {
        next = cur->next;
        free(cur);
        cur = next;
    }
}

// --- Doubly linked list ---
struct DLLNode {
    int val;
    struct DLLNode *prev;
    struct DLLNode *next;
};

struct DLLNode *dll_append(struct DLLNode *tail, int val) {
    struct DLLNode *n = malloc(sizeof(struct DLLNode));
    n->val = val;
    n->prev = tail;
    n->next = 0;
    if (tail) tail->next = n;
    return n;
}

struct DLLNode *dll_head(struct DLLNode *any) {
    while (any && any->prev) any = any->prev;
    return any;
}

struct DLLNode *dll_tail(struct DLLNode *any) {
    while (any && any->next) any = any->next;
    return any;
}

void dll_free(struct DLLNode *head) {
    struct DLLNode *cur = head;
    struct DLLNode *next;
    while (cur) {
        next = cur->next;
        free(cur);
        cur = next;
    }
}

// --- Binary tree ---
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

struct TreeNode *tree_new(int val) {
    struct TreeNode *n = malloc(sizeof(struct TreeNode));
    n->val = val;
    n->left = 0;
    n->right = 0;
    return n;
}

void tree_insert(struct TreeNode *root, int val) {
    if (val < root->val) {
        if (root->left) tree_insert(root->left, val);
        else root->left = tree_new(val);
    } else {
        if (root->right) tree_insert(root->right, val);
        else root->right = tree_new(val);
    }
}

void tree_inorder(struct TreeNode *n) {
    if (!n) return;
    tree_inorder(n->left);
    printf(" %d", n->val);
    tree_inorder(n->right);
}

int tree_sum(struct TreeNode *n) {
    if (!n) return 0;
    return n->val + tree_sum(n->left) + tree_sum(n->right);
}

void tree_free(struct TreeNode *n) {
    if (!n) return;
    tree_free(n->left);
    tree_free(n->right);
    free(n);
}

// --- Graph node with adjacency list ---
struct AdjNode {
    int vertex;
    struct AdjNode *next;
};

struct GraphNode {
    int id;
    struct AdjNode *adj;
};

void graph_add_edge(struct GraphNode *nodes, int u, int v) {
    struct AdjNode *a = malloc(sizeof(struct AdjNode));
    a->vertex = v;
    a->next = nodes[u].adj;
    nodes[u].adj = a;

    struct AdjNode *b = malloc(sizeof(struct AdjNode));
    b->vertex = u;
    b->next = nodes[v].adj;
    nodes[v].adj = b;
}

void graph_print_adj(struct GraphNode *nodes, int v) {
    printf("graph_adj_%d=", v);
    struct AdjNode *cur = nodes[v].adj;
    int first = 1;
    while (cur) {
        if (!first) printf(" ");
        printf("%d", cur->vertex);
        first = 0;
        cur = cur->next;
    }
    printf("\n");
}

void graph_free_adj(struct GraphNode *nodes, int n) {
    int i;
    for (i = 0; i < n; i++) {
        struct AdjNode *cur = nodes[i].adj;
        struct AdjNode *next;
        while (cur) {
            next = cur->next;
            free(cur);
            cur = next;
        }
    }
}

// --- Circular linked list ---
struct CNode {
    int val;
    struct CNode *next;
};

struct CNode *circular_insert(struct CNode *tail, int val) {
    struct CNode *n = malloc(sizeof(struct CNode));
    n->val = val;
    if (!tail) {
        n->next = n;
        return n;
    }
    n->next = tail->next;
    tail->next = n;
    return n;
}

int circular_count(struct CNode *tail) {
    if (!tail) return 0;
    int count = 1;
    struct CNode *cur = tail->next;
    while (cur != tail) {
        count++;
        cur = cur->next;
    }
    return count;
}

int circular_sum(struct CNode *tail) {
    if (!tail) return 0;
    int s = tail->val;
    struct CNode *cur = tail->next;
    while (cur != tail) {
        s = s + cur->val;
        cur = cur->next;
    }
    return s;
}

void circular_free(struct CNode *tail) {
    if (!tail) return;
    struct CNode *cur = tail->next;
    struct CNode *next;
    while (cur != tail) {
        next = cur->next;
        free(cur);
        cur = next;
    }
    free(tail);
}

int main(void) {
    // Singly linked list: push 1..10, sum, reverse, print
    struct SLLNode *sll = 0;
    int i;
    for (i = 1; i <= 10; i++) sll = sll_push(sll, i);
    printf("sll_sum=%d\n", sll_sum(sll));
    sll = sll_reverse(sll);
    printf("sll_rev=");
    struct SLLNode *sc = sll;
    int sfirst = 1;
    while (sc) {
        if (!sfirst) printf(" ");
        printf("%d", sc->val);
        sfirst = 0;
        sc = sc->next;
    }
    printf("\n");
    sll_free(sll);

    // Doubly linked list: append 1..5, print forward and backward
    struct DLLNode *dtail = 0;
    for (i = 1; i <= 5; i++) dtail = dll_append(dtail, i);
    struct DLLNode *dhead = dll_head(dtail);
    printf("dll_fwd=");
    struct DLLNode *dc = dhead;
    int dfirst = 1;
    while (dc) {
        if (!dfirst) printf(" ");
        printf("%d", dc->val);
        dfirst = 0;
        dc = dc->next;
    }
    printf("\n");
    printf("dll_bwd=");
    dc = dll_tail(dhead);
    dfirst = 1;
    while (dc) {
        if (!dfirst) printf(" ");
        printf("%d", dc->val);
        dfirst = 0;
        dc = dc->prev;
    }
    printf("\n");
    dll_free(dhead);

    // Binary search tree: insert 4,2,6,1,3,5,7 inorder and sum
    struct TreeNode *tree = tree_new(4);
    tree_insert(tree, 2);
    tree_insert(tree, 6);
    tree_insert(tree, 1);
    tree_insert(tree, 3);
    tree_insert(tree, 5);
    tree_insert(tree, 7);
    printf("tree_inorder=");
    tree_inorder(tree);
    printf("\n");
    printf("tree_sum=%d\n", tree_sum(tree));
    tree_free(tree);

    // Graph: 3 vertices, edges 0-1, 0-2, 1-2
    struct GraphNode gnodes[3];
    for (i = 0; i < 3; i++) { gnodes[i].id = i; gnodes[i].adj = 0; }
    graph_add_edge(gnodes, 0, 1);
    graph_add_edge(gnodes, 0, 2);
    graph_add_edge(gnodes, 1, 2);
    graph_print_adj(gnodes, 0);
    graph_print_adj(gnodes, 1);
    graph_print_adj(gnodes, 2);
    graph_free_adj(gnodes, 3);

    // Circular linked list: insert 1..10
    struct CNode *circ = 0;
    for (i = 1; i <= 10; i++) circ = circular_insert(circ, i);
    printf("circular_count=%d\n", circular_count(circ));
    printf("circular_sum=%d\n", circular_sum(circ));
    circular_free(circ);

    printf("all_freed=%d\n", 1);
    return 0;
}
