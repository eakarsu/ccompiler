int printf(const char *fmt, ...);

struct Node {
    int key;
    int left;
    int right;
};

struct Node nodes[64];
int node_count = 0;

int new_node(int key) {
    int idx = node_count;
    nodes[idx].key = key;
    nodes[idx].left = -1;
    nodes[idx].right = -1;
    node_count = node_count + 1;
    return idx;
}

int insert(int root, int key) {
    if (root == -1) {
        return new_node(key);
    }
    if (key < nodes[root].key) {
        nodes[root].left = insert(nodes[root].left, key);
    } else if (key > nodes[root].key) {
        nodes[root].right = insert(nodes[root].right, key);
    }
    return root;
}

int height(int root) {
    int lh;
    int rh;
    if (root == -1) {
        return 0;
    }
    lh = height(nodes[root].left);
    rh = height(nodes[root].right);
    if (lh > rh) {
        return lh + 1;
    }
    return rh + 1;
}

int abs_val(int x) {
    if (x < 0) return 0 - x;
    return x;
}

int is_balanced(int root) {
    int lh;
    int rh;
    if (root == -1) {
        return 1;
    }
    lh = height(nodes[root].left);
    rh = height(nodes[root].right);
    if (abs_val(lh - rh) > 1) {
        return 0;
    }
    if (!is_balanced(nodes[root].left)) {
        return 0;
    }
    if (!is_balanced(nodes[root].right)) {
        return 0;
    }
    return 1;
}

int is_bst_helper(int root, int min_val, int max_val) {
    if (root == -1) {
        return 1;
    }
    if (nodes[root].key <= min_val) {
        return 0;
    }
    if (nodes[root].key >= max_val) {
        return 0;
    }
    if (!is_bst_helper(nodes[root].left, min_val, nodes[root].key)) {
        return 0;
    }
    if (!is_bst_helper(nodes[root].right, nodes[root].key, max_val)) {
        return 0;
    }
    return 1;
}

int is_bst(int root) {
    return is_bst_helper(root, -99999, 99999);
}

int count_nodes(int root) {
    if (root == -1) return 0;
    return 1 + count_nodes(nodes[root].left) + count_nodes(nodes[root].right);
}

int is_full(int root) {
    if (root == -1) return 1;
    if (nodes[root].left == -1 && nodes[root].right == -1) {
        return 1;
    }
    if (nodes[root].left != -1 && nodes[root].right != -1) {
        return is_full(nodes[root].left) && is_full(nodes[root].right);
    }
    return 0;
}

int main(void) {
    int root;
    int skewed;
    int bad_bst;

    root = -1;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);

    // EXPECT: balanced: 1
    printf("balanced: %d\n", is_balanced(root));
    // EXPECT: is_bst: 1
    printf("is_bst: %d\n", is_bst(root));
    // EXPECT: height: 3
    printf("height: %d\n", height(root));
    // EXPECT: full: 1
    printf("full: %d\n", is_full(root));

    skewed = -1;
    skewed = insert(skewed, 10);
    skewed = insert(skewed, 20);
    skewed = insert(skewed, 30);
    skewed = insert(skewed, 40);
    skewed = insert(skewed, 50);

    // EXPECT: skewed balanced: 0
    printf("skewed balanced: %d\n", is_balanced(skewed));
    // EXPECT: skewed bst: 1
    printf("skewed bst: %d\n", is_bst(skewed));
    // EXPECT: skewed height: 5
    printf("skewed height: %d\n", height(skewed));
    // EXPECT: skewed full: 0
    printf("skewed full: %d\n", is_full(skewed));

    bad_bst = new_node(50);
    nodes[bad_bst].left = new_node(60);
    nodes[bad_bst].right = new_node(40);

    // EXPECT: bad_bst balanced: 1
    printf("bad_bst balanced: %d\n", is_balanced(bad_bst));
    // EXPECT: bad_bst is_bst: 0
    printf("bad_bst is_bst: %d\n", is_bst(bad_bst));

    root = insert(root, 10);
    // EXPECT: after insert balanced: 1
    printf("after insert balanced: %d\n", is_balanced(root));
    // EXPECT: after insert bst: 1
    printf("after insert bst: %d\n", is_bst(root));

    root = insert(root, 5);
    root = insert(root, 3);
    // EXPECT: deep insert balanced: 0
    printf("deep insert balanced: %d\n", is_balanced(root));
    // EXPECT: deep insert bst: 1
    printf("deep insert bst: %d\n", is_bst(root));
    // EXPECT: deep insert height: 6
    printf("deep insert height: %d\n", height(root));

    // EXPECT: count: 10
    printf("count: %d\n", count_nodes(root));
    // EXPECT: skewed count: 5
    printf("skewed count: %d\n", count_nodes(skewed));

    return 0;
}
