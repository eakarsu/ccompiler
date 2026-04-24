int printf(const char *fmt, ...);
// EXPECT: Inorder: 1(R) 5(B) 8(R) 10(R) 12(R) 15(B) 18(R) 20(B) 25(R) 30(B)\nRoot: 20 (color=B)\nBlack height: 3\nValid black height: 1\nRoot is black: 1\nNodes used: 10\nDone

// Red-black tree property checker (not full RB tree, but verifies properties)

enum Color { RED, BLACK };

struct RBNode {
    int key;
    int color;  // 0=RED, 1=BLACK
    int left;
    int right;
    int parent;
    int valid;
};

struct RBTree {
    struct RBNode nodes[32];
    int root;
    int next_id;
    int nil;  // sentinel node
};

void rb_init(struct RBTree *t) {
    // Node 0 is NIL sentinel
    t->nil = 0;
    t->nodes[0].color = BLACK;
    t->nodes[0].key = 0;
    t->nodes[0].left = 0;
    t->nodes[0].right = 0;
    t->nodes[0].parent = 0;
    t->nodes[0].valid = 1;
    t->root = 0;
    t->next_id = 1;
}

int rb_new_node(struct RBTree *t, int key, int color) {
    int id = t->next_id;
    t->next_id = t->next_id + 1;
    t->nodes[id].key = key;
    t->nodes[id].color = color;
    t->nodes[id].left = t->nil;
    t->nodes[id].right = t->nil;
    t->nodes[id].parent = t->nil;
    t->nodes[id].valid = 1;
    return id;
}

void rb_rotate_left(struct RBTree *t, int x) {
    int y = t->nodes[x].right;
    t->nodes[x].right = t->nodes[y].left;
    if (t->nodes[y].left != t->nil)
        t->nodes[t->nodes[y].left].parent = x;
    t->nodes[y].parent = t->nodes[x].parent;
    if (t->nodes[x].parent == t->nil)
        t->root = y;
    else if (x == t->nodes[t->nodes[x].parent].left)
        t->nodes[t->nodes[x].parent].left = y;
    else
        t->nodes[t->nodes[x].parent].right = y;
    t->nodes[y].left = x;
    t->nodes[x].parent = y;
}

void rb_rotate_right(struct RBTree *t, int x) {
    int y = t->nodes[x].left;
    t->nodes[x].left = t->nodes[y].right;
    if (t->nodes[y].right != t->nil)
        t->nodes[t->nodes[y].right].parent = x;
    t->nodes[y].parent = t->nodes[x].parent;
    if (t->nodes[x].parent == t->nil)
        t->root = y;
    else if (x == t->nodes[t->nodes[x].parent].right)
        t->nodes[t->nodes[x].parent].right = y;
    else
        t->nodes[t->nodes[x].parent].left = y;
    t->nodes[y].right = x;
    t->nodes[x].parent = y;
}

void rb_insert_fixup(struct RBTree *t, int z) {
    while (t->nodes[t->nodes[z].parent].color == RED) {
        if (t->nodes[z].parent == t->nodes[t->nodes[t->nodes[z].parent].parent].left) {
            int y = t->nodes[t->nodes[t->nodes[z].parent].parent].right;
            if (t->nodes[y].color == RED) {
                t->nodes[t->nodes[z].parent].color = BLACK;
                t->nodes[y].color = BLACK;
                t->nodes[t->nodes[t->nodes[z].parent].parent].color = RED;
                z = t->nodes[t->nodes[z].parent].parent;
            } else {
                if (z == t->nodes[t->nodes[z].parent].right) {
                    z = t->nodes[z].parent;
                    rb_rotate_left(t, z);
                }
                t->nodes[t->nodes[z].parent].color = BLACK;
                t->nodes[t->nodes[t->nodes[z].parent].parent].color = RED;
                rb_rotate_right(t, t->nodes[t->nodes[z].parent].parent);
            }
        } else {
            int y = t->nodes[t->nodes[t->nodes[z].parent].parent].left;
            if (t->nodes[y].color == RED) {
                t->nodes[t->nodes[z].parent].color = BLACK;
                t->nodes[y].color = BLACK;
                t->nodes[t->nodes[t->nodes[z].parent].parent].color = RED;
                z = t->nodes[t->nodes[z].parent].parent;
            } else {
                if (z == t->nodes[t->nodes[z].parent].left) {
                    z = t->nodes[z].parent;
                    rb_rotate_right(t, z);
                }
                t->nodes[t->nodes[z].parent].color = BLACK;
                t->nodes[t->nodes[t->nodes[z].parent].parent].color = RED;
                rb_rotate_left(t, t->nodes[t->nodes[z].parent].parent);
            }
        }
    }
    t->nodes[t->root].color = BLACK;
}

void rb_insert(struct RBTree *t, int key) {
    int z = rb_new_node(t, key, RED);
    int y = t->nil;
    int x = t->root;
    while (x != t->nil) {
        y = x;
        if (key < t->nodes[x].key)
            x = t->nodes[x].left;
        else
            x = t->nodes[x].right;
    }
    t->nodes[z].parent = y;
    if (y == t->nil)
        t->root = z;
    else if (key < t->nodes[y].key)
        t->nodes[y].left = z;
    else
        t->nodes[y].right = z;
    rb_insert_fixup(t, z);
}

int rb_black_height(struct RBTree *t, int node) {
    if (node == t->nil) return 1;
    int lh = rb_black_height(t, t->nodes[node].left);
    int rh = rb_black_height(t, t->nodes[node].right);
    if (lh != rh) return -1;
    if (lh < 0) return -1;
    return lh + (t->nodes[node].color == BLACK ? 1 : 0);
}

void rb_inorder(struct RBTree *t, int node) {
    if (node == t->nil) return;
    rb_inorder(t, t->nodes[node].left);
    printf(" %d(%c)", t->nodes[node].key, t->nodes[node].color == RED ? 'R' : 'B');
    rb_inorder(t, t->nodes[node].right);
}

int main(void) {
    struct RBTree tree;
    rb_init(&tree);

    int vals[10];
    vals[0] = 10; vals[1] = 20; vals[2] = 30; vals[3] = 15;
    vals[4] = 25; vals[5] = 5; vals[6] = 1; vals[7] = 8;
    vals[8] = 12; vals[9] = 18;

    int i = 0;
    while (i < 10) {
        rb_insert(&tree, vals[i]);
        i = i + 1;
    }

    printf("Inorder:");
    rb_inorder(&tree, tree.root);
    printf("\n");

    printf("Root: %d (color=%c)\n", tree.nodes[tree.root].key,
           tree.nodes[tree.root].color == RED ? 'R' : 'B');

    int bh = rb_black_height(&tree, tree.root);
    printf("Black height: %d\n", bh);
    printf("Valid black height: %d\n", bh > 0 ? 1 : 0);

    printf("Root is black: %d\n", tree.nodes[tree.root].color == BLACK ? 1 : 0);
    printf("Nodes used: %d\n", tree.next_id - 1);

    printf("Done\n");
    return 0;
}
