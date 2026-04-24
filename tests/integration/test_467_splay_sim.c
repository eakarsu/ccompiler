int printf(const char *fmt, ...);
// EXPECT: Inorder: 5 10 15 20 25 30 40 50\nRoot after inserts: 5\nRoot after search 30: 30\nRoot after search 10: 10\nRoot after search 50: 50\nInorder: 5 10 15 20 25 30 40 50\nDepth: 7\nSearch 99: 0\nSearch 5: 1\nDone

// Splay tree simulation

struct SplayNode {
    int key;
    int left;
    int right;
    int parent;
    int valid;
};

struct SplayTree {
    struct SplayNode nodes[32];
    int root;
    int next_id;
};

void splay_init(struct SplayTree *t) {
    t->root = -1;
    t->next_id = 0;
    int i = 0;
    while (i < 32) {
        t->nodes[i].valid = 0;
        t->nodes[i].left = -1;
        t->nodes[i].right = -1;
        t->nodes[i].parent = -1;
        i = i + 1;
    }
}

void splay_set_child(struct SplayTree *t, int parent, int child, int is_left) {
    if (is_left) t->nodes[parent].left = child;
    else t->nodes[parent].right = child;
    if (child >= 0) t->nodes[child].parent = parent;
}

void splay_rotate(struct SplayTree *t, int x) {
    int p = t->nodes[x].parent;
    int g = t->nodes[p].parent;
    int is_left = (t->nodes[p].left == x);

    if (is_left) {
        splay_set_child(t, p, t->nodes[x].right, 1);
        splay_set_child(t, x, p, 0);
    } else {
        splay_set_child(t, p, t->nodes[x].left, 0);
        splay_set_child(t, x, p, 1);
    }

    t->nodes[x].parent = g;
    if (g >= 0) {
        if (t->nodes[g].left == p) t->nodes[g].left = x;
        else t->nodes[g].right = x;
    }
}

void splay(struct SplayTree *t, int x) {
    while (t->nodes[x].parent >= 0) {
        int p = t->nodes[x].parent;
        int g = t->nodes[p].parent;
        if (g >= 0) {
            int x_is_left = (t->nodes[p].left == x);
            int p_is_left = (t->nodes[g].left == p);
            if (x_is_left == p_is_left) {
                splay_rotate(t, p);
            } else {
                splay_rotate(t, x);
            }
        }
        splay_rotate(t, x);
    }
    t->root = x;
}

void splay_insert(struct SplayTree *t, int key) {
    int id = t->next_id;
    t->next_id = t->next_id + 1;
    t->nodes[id].key = key;
    t->nodes[id].left = -1;
    t->nodes[id].right = -1;
    t->nodes[id].parent = -1;
    t->nodes[id].valid = 1;

    if (t->root < 0) {
        t->root = id;
        return;
    }

    int cur = t->root;
    int par = -1;
    while (cur >= 0) {
        par = cur;
        if (key < t->nodes[cur].key) cur = t->nodes[cur].left;
        else cur = t->nodes[cur].right;
    }
    t->nodes[id].parent = par;
    if (key < t->nodes[par].key) t->nodes[par].left = id;
    else t->nodes[par].right = id;
    splay(t, id);
}

int splay_search(struct SplayTree *t, int key) {
    int cur = t->root;
    while (cur >= 0) {
        if (key == t->nodes[cur].key) {
            splay(t, cur);
            return 1;
        }
        if (key < t->nodes[cur].key) cur = t->nodes[cur].left;
        else cur = t->nodes[cur].right;
    }
    return 0;
}

void splay_inorder(struct SplayTree *t, int node) {
    if (node < 0) return;
    splay_inorder(t, t->nodes[node].left);
    printf(" %d", t->nodes[node].key);
    splay_inorder(t, t->nodes[node].right);
}

int splay_depth(struct SplayTree *t, int node) {
    if (node < 0) return 0;
    int l = splay_depth(t, t->nodes[node].left);
    int r = splay_depth(t, t->nodes[node].right);
    return 1 + (l > r ? l : r);
}

int main(void) {
    struct SplayTree tree;
    splay_init(&tree);

    splay_insert(&tree, 10);
    splay_insert(&tree, 20);
    splay_insert(&tree, 30);
    splay_insert(&tree, 40);
    splay_insert(&tree, 50);
    splay_insert(&tree, 25);
    splay_insert(&tree, 15);
    splay_insert(&tree, 5);

    printf("Inorder:");
    splay_inorder(&tree, tree.root);
    printf("\n");

    printf("Root after inserts: %d\n", tree.nodes[tree.root].key);

    // Search brings to root
    splay_search(&tree, 30);
    printf("Root after search 30: %d\n", tree.nodes[tree.root].key);

    splay_search(&tree, 10);
    printf("Root after search 10: %d\n", tree.nodes[tree.root].key);

    splay_search(&tree, 50);
    printf("Root after search 50: %d\n", tree.nodes[tree.root].key);

    // Verify inorder still correct
    printf("Inorder:");
    splay_inorder(&tree, tree.root);
    printf("\n");

    printf("Depth: %d\n", splay_depth(&tree, tree.root));

    // Search miss
    printf("Search 99: %d\n", splay_search(&tree, 99));
    printf("Search 5: %d\n", splay_search(&tree, 5));

    printf("Done\n");
    return 0;
}
