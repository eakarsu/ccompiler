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

int search(int root, int key) {
    if (root == -1) {
        return 0;
    }
    if (key == nodes[root].key) {
        return 1;
    }
    if (key < nodes[root].key) {
        return search(nodes[root].left, key);
    }
    return search(nodes[root].right, key);
}

int find_min(int root) {
    if (root == -1) {
        return -1;
    }
    if (nodes[root].left == -1) {
        return nodes[root].key;
    }
    return find_min(nodes[root].left);
}

int find_max(int root) {
    if (root == -1) {
        return -1;
    }
    if (nodes[root].right == -1) {
        return nodes[root].key;
    }
    return find_max(nodes[root].right);
}

int count_nodes(int root) {
    if (root == -1) {
        return 0;
    }
    return 1 + count_nodes(nodes[root].left) + count_nodes(nodes[root].right);
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

int count_leaves(int root) {
    if (root == -1) {
        return 0;
    }
    if (nodes[root].left == -1 && nodes[root].right == -1) {
        return 1;
    }
    return count_leaves(nodes[root].left) + count_leaves(nodes[root].right);
}

int sum_keys(int root) {
    if (root == -1) {
        return 0;
    }
    return nodes[root].key + sum_keys(nodes[root].left) + sum_keys(nodes[root].right);
}

int main(void) {
    int root;
    int root2;
    int i;
    int start;

    root = -1;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);

    // EXPECT: search 50: 1
    printf("search 50: %d\n", search(root, 50));
    // EXPECT: search 30: 1
    printf("search 30: %d\n", search(root, 30));
    // EXPECT: search 80: 1
    printf("search 80: %d\n", search(root, 80));
    // EXPECT: search 25: 0
    printf("search 25: %d\n", search(root, 25));
    // EXPECT: search 99: 0
    printf("search 99: %d\n", search(root, 99));

    // EXPECT: min: 20
    printf("min: %d\n", find_min(root));
    // EXPECT: max: 80
    printf("max: %d\n", find_max(root));

    // EXPECT: count: 7
    printf("count: %d\n", count_nodes(root));
    // EXPECT: height: 3
    printf("height: %d\n", height(root));
    // EXPECT: leaves: 4
    printf("leaves: %d\n", count_leaves(root));
    // EXPECT: sum: 350
    printf("sum: %d\n", sum_keys(root));

    start = node_count;
    root2 = -1;
    root2 = insert(root2, 10);
    root2 = insert(root2, 5);
    root2 = insert(root2, 15);
    root2 = insert(root2, 3);
    root2 = insert(root2, 7);

    // EXPECT: tree2 count: 5
    printf("tree2 count: %d\n", count_nodes(root2));
    // EXPECT: tree2 min: 3
    printf("tree2 min: %d\n", find_min(root2));
    // EXPECT: tree2 max: 15
    printf("tree2 max: %d\n", find_max(root2));
    // EXPECT: tree2 height: 3
    printf("tree2 height: %d\n", height(root2));

    root = insert(root, 25);
    // EXPECT: after insert count: 8
    printf("after insert count: %d\n", count_nodes(root));
    // EXPECT: search 25 now: 1
    printf("search 25 now: %d\n", search(root, 25));
    // EXPECT: new sum: 375
    printf("new sum: %d\n", sum_keys(root));

    return 0;
}
