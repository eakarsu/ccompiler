int printf(const char *fmt, ...);

// Red-black tree color tracking (simplified insertion with recoloring)

enum Color { RED, BLACK };

struct RBNode {
    int key;
    int color;
    int left;
    int right;
    int parent;
};

struct RBTree {
    struct RBNode nodes[64];
    int size;
    int root;
    int recolors;
};

void rb_init(struct RBTree *t) {
    t->size = 0;
    t->root = -1;
    t->recolors = 0;
}

int rb_new_node(struct RBTree *t, int key, int parent) {
    int n = t->size;
    t->size = t->size + 1;
    t->nodes[n].key = key;
    t->nodes[n].color = RED;
    t->nodes[n].left = -1;
    t->nodes[n].right = -1;
    t->nodes[n].parent = parent;
    return n;
}

int rb_color(struct RBTree *t, int idx) {
    if (idx == -1) return BLACK;
    return t->nodes[idx].color;
}

void rb_rotate_left(struct RBTree *t, int x) {
    int y = t->nodes[x].right;
    int p = t->nodes[x].parent;
    t->nodes[x].right = t->nodes[y].left;
    if (t->nodes[y].left != -1)
        t->nodes[t->nodes[y].left].parent = x;
    t->nodes[y].left = x;
    t->nodes[y].parent = p;
    t->nodes[x].parent = y;
    if (p == -1) {
        t->root = y;
    } else if (t->nodes[p].left == x) {
        t->nodes[p].left = y;
    } else {
        t->nodes[p].right = y;
    }
}

void rb_rotate_right(struct RBTree *t, int y) {
    int x = t->nodes[y].left;
    int p = t->nodes[y].parent;
    t->nodes[y].left = t->nodes[x].right;
    if (t->nodes[x].right != -1)
        t->nodes[t->nodes[x].right].parent = y;
    t->nodes[x].right = y;
    t->nodes[x].parent = p;
    t->nodes[y].parent = x;
    if (p == -1) {
        t->root = x;
    } else if (t->nodes[p].left == y) {
        t->nodes[p].left = x;
    } else {
        t->nodes[p].right = x;
    }
}

void rb_fix(struct RBTree *t, int z) {
    int p;
    int gp;
    int uncle;
    while (t->nodes[z].parent != -1 && rb_color(t, t->nodes[z].parent) == RED) {
        p = t->nodes[z].parent;
        gp = t->nodes[p].parent;
        if (gp == -1) break;
        if (p == t->nodes[gp].left) {
            uncle = t->nodes[gp].right;
            if (rb_color(t, uncle) == RED) {
                t->nodes[p].color = BLACK;
                t->nodes[uncle].color = BLACK;
                t->nodes[gp].color = RED;
                t->recolors = t->recolors + 3;
                z = gp;
            } else {
                if (z == t->nodes[p].right) {
                    z = p;
                    rb_rotate_left(t, z);
                    p = t->nodes[z].parent;
                    gp = t->nodes[p].parent;
                }
                t->nodes[p].color = BLACK;
                t->nodes[gp].color = RED;
                t->recolors = t->recolors + 2;
                rb_rotate_right(t, gp);
            }
        } else {
            uncle = t->nodes[gp].left;
            if (rb_color(t, uncle) == RED) {
                t->nodes[p].color = BLACK;
                t->nodes[uncle].color = BLACK;
                t->nodes[gp].color = RED;
                t->recolors = t->recolors + 3;
                z = gp;
            } else {
                if (z == t->nodes[p].left) {
                    z = p;
                    rb_rotate_right(t, z);
                    p = t->nodes[z].parent;
                    gp = t->nodes[p].parent;
                }
                t->nodes[p].color = BLACK;
                t->nodes[gp].color = RED;
                t->recolors = t->recolors + 2;
                rb_rotate_left(t, gp);
            }
        }
    }
    if (t->nodes[t->root].color != BLACK) {
        t->nodes[t->root].color = BLACK;
        t->recolors = t->recolors + 1;
    }
}

void rb_insert(struct RBTree *t, int key) {
    int cur;
    int parent;
    int n;
    if (t->root == -1) {
        n = rb_new_node(t, key, -1);
        t->root = n;
        t->nodes[n].color = BLACK;
        t->recolors = t->recolors + 1;
        return;
    }
    cur = t->root;
    parent = -1;
    while (cur != -1) {
        parent = cur;
        if (key < t->nodes[cur].key) {
            cur = t->nodes[cur].left;
        } else {
            cur = t->nodes[cur].right;
        }
    }
    n = rb_new_node(t, key, parent);
    if (key < t->nodes[parent].key) {
        t->nodes[parent].left = n;
    } else {
        t->nodes[parent].right = n;
    }
    rb_fix(t, n);
}

int rb_count_color(struct RBTree *t, int color) {
    int count = 0;
    int i;
    for (i = 0; i < t->size; i++) {
        if (t->nodes[i].color == color) {
            count = count + 1;
        }
    }
    return count;
}

int main() {
    struct RBTree tree;
    rb_init(&tree);

    rb_insert(&tree, 10);
    printf("after 10: black=%d red=%d recolors=%d\n",
        rb_count_color(&tree, BLACK), rb_count_color(&tree, RED), tree.recolors);
    // EXPECT: after 10: black=1 red=0 recolors=1

    rb_insert(&tree, 20);
    rb_insert(&tree, 30);
    printf("after 20,30: black=%d red=%d recolors=%d\n",
        rb_count_color(&tree, BLACK), rb_count_color(&tree, RED), tree.recolors);
    // EXPECT: after 20,30: black=1 red=2 recolors=3

    rb_insert(&tree, 15);
    printf("after 15: black=%d red=%d recolors=%d\n",
        rb_count_color(&tree, BLACK), rb_count_color(&tree, RED), tree.recolors);
    // EXPECT: after 15: black=3 red=1 recolors=7

    rb_insert(&tree, 25);
    rb_insert(&tree, 5);
    rb_insert(&tree, 1);
    printf("after 25,5,1: black=%d red=%d recolors=%d\n",
        rb_count_color(&tree, BLACK), rb_count_color(&tree, RED), tree.recolors);
    // EXPECT: after 25,5,1: black=4 red=3 recolors=10

    printf("root key: %d\n", tree.nodes[tree.root].key);
    // EXPECT: root key: 20
    printf("root color: %s\n", tree.nodes[tree.root].color == BLACK ? "black" : "red");
    // EXPECT: root color: black
    printf("total nodes: %d\n", tree.size);
    // EXPECT: total nodes: 7

    return 0;
}
