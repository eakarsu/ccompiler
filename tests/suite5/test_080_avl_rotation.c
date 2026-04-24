int printf(const char *fmt, ...);

struct Node {
    int key;
    int left;
    int right;
    int height;
};

struct Node nodes[64];
int node_count = 0;

int new_node(int key) {
    int idx = node_count;
    nodes[idx].key = key;
    nodes[idx].left = -1;
    nodes[idx].right = -1;
    nodes[idx].height = 1;
    node_count = node_count + 1;
    return idx;
}

int get_height(int n) {
    if (n == -1) return 0;
    return nodes[n].height;
}

int max_val(int a, int b) {
    if (a > b) return a;
    return b;
}

void update_height(int n) {
    if (n == -1) return;
    nodes[n].height = 1 + max_val(get_height(nodes[n].left),
                                    get_height(nodes[n].right));
}

int balance_factor(int n) {
    if (n == -1) return 0;
    return get_height(nodes[n].left) - get_height(nodes[n].right);
}

int rotate_right(int y) {
    int x;
    int t2;
    x = nodes[y].left;
    t2 = nodes[x].right;
    nodes[x].right = y;
    nodes[y].left = t2;
    update_height(y);
    update_height(x);
    return x;
}

int rotate_left(int x) {
    int y;
    int t2;
    y = nodes[x].right;
    t2 = nodes[y].left;
    nodes[y].left = x;
    nodes[x].right = t2;
    update_height(x);
    update_height(y);
    return y;
}

int avl_insert(int root, int key) {
    int bf;
    if (root == -1) {
        return new_node(key);
    }
    if (key < nodes[root].key) {
        nodes[root].left = avl_insert(nodes[root].left, key);
    } else if (key > nodes[root].key) {
        nodes[root].right = avl_insert(nodes[root].right, key);
    } else {
        return root;
    }

    update_height(root);
    bf = balance_factor(root);

    if (bf > 1 && key < nodes[nodes[root].left].key) {
        return rotate_right(root);
    }
    if (bf < -1 && key > nodes[nodes[root].right].key) {
        return rotate_left(root);
    }
    if (bf > 1 && key > nodes[nodes[root].left].key) {
        nodes[root].left = rotate_left(nodes[root].left);
        return rotate_right(root);
    }
    if (bf < -1 && key < nodes[nodes[root].right].key) {
        nodes[root].right = rotate_right(nodes[root].right);
        return rotate_left(root);
    }

    return root;
}

int count_nodes(int root) {
    if (root == -1) return 0;
    return 1 + count_nodes(nodes[root].left) + count_nodes(nodes[root].right);
}

int is_balanced(int root) {
    int bf;
    if (root == -1) return 1;
    bf = balance_factor(root);
    if (bf < -1 || bf > 1) return 0;
    return is_balanced(nodes[root].left) && is_balanced(nodes[root].right);
}

int is_bst(int root, int lo, int hi) {
    if (root == -1) return 1;
    if (nodes[root].key <= lo || nodes[root].key >= hi) return 0;
    return is_bst(nodes[root].left, lo, nodes[root].key)
        && is_bst(nodes[root].right, nodes[root].key, hi);
}

int inorder_buf[64];
int inorder_cnt;

void inorder(int root) {
    if (root == -1) return;
    inorder(nodes[root].left);
    inorder_buf[inorder_cnt] = nodes[root].key;
    inorder_cnt = inorder_cnt + 1;
    inorder(nodes[root].right);
}

int main(void) {
    int root;
    int i;

    root = -1;
    root = avl_insert(root, 10);
    root = avl_insert(root, 20);
    root = avl_insert(root, 30);

    // EXPECT: root after LL: 20
    printf("root after LL: %d\n", nodes[root].key);
    // EXPECT: balanced: 1
    printf("balanced: %d\n", is_balanced(root));
    // EXPECT: is bst: 1
    printf("is bst: %d\n", is_bst(root, -99999, 99999));
    // EXPECT: height: 2
    printf("height: %d\n", get_height(root));

    root = avl_insert(root, 40);
    root = avl_insert(root, 50);
    // EXPECT: after more: balanced: 1
    printf("after more: balanced: %d\n", is_balanced(root));
    // EXPECT: after more: bst: 1
    printf("after more: bst: %d\n", is_bst(root, -99999, 99999));
    // EXPECT: count: 5
    printf("count: %d\n", count_nodes(root));
    // EXPECT: height5: 3
    printf("height5: %d\n", get_height(root));

    root = avl_insert(root, 25);
    // EXPECT: count6: 6
    printf("count6: %d\n", count_nodes(root));
    // EXPECT: balanced6: 1
    printf("balanced6: %d\n", is_balanced(root));

    inorder_cnt = 0;
    inorder(root);
    // EXPECT: inorder: 10 20 25 30 40 50
    printf("inorder:");
    for (i = 0; i < inorder_cnt; i = i + 1) {
        printf(" %d", inorder_buf[i]);
    }
    printf("\n");

    node_count = 0;
    root = -1;
    root = avl_insert(root, 30);
    root = avl_insert(root, 20);
    root = avl_insert(root, 10);

    // EXPECT: RR root: 20
    printf("RR root: %d\n", nodes[root].key);

    node_count = 0;
    root = -1;
    root = avl_insert(root, 30);
    root = avl_insert(root, 10);
    root = avl_insert(root, 20);

    // EXPECT: LR root: 20
    printf("LR root: %d\n", nodes[root].key);

    node_count = 0;
    root = -1;
    root = avl_insert(root, 10);
    root = avl_insert(root, 30);
    root = avl_insert(root, 20);

    // EXPECT: RL root: 20
    printf("RL root: %d\n", nodes[root].key);

    node_count = 0;
    root = -1;
    for (i = 1; i < 16; i = i + 1) {
        root = avl_insert(root, i);
    }
    // EXPECT: seq balanced: 1
    printf("seq balanced: %d\n", is_balanced(root));
    // EXPECT: seq bst: 1
    printf("seq bst: %d\n", is_bst(root, -99999, 99999));
    // EXPECT: seq count: 15
    printf("seq count: %d\n", count_nodes(root));
    // EXPECT: seq height: 4
    printf("seq height: %d\n", get_height(root));

    return 0;
}
