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

int ser_buf[128];
int ser_len;

void serialize(int root) {
    if (root == -1) {
        ser_buf[ser_len] = -9999;
        ser_len = ser_len + 1;
        return;
    }
    ser_buf[ser_len] = nodes[root].key;
    ser_len = ser_len + 1;
    serialize(nodes[root].left);
    serialize(nodes[root].right);
}

int deser_idx;

int deserialize(void) {
    int val;
    int nd;
    val = ser_buf[deser_idx];
    deser_idx = deser_idx + 1;
    if (val == -9999) {
        return -1;
    }
    nd = new_node(val);
    nodes[nd].left = deserialize();
    nodes[nd].right = deserialize();
    return nd;
}

int count_nodes(int root) {
    if (root == -1) return 0;
    return 1 + count_nodes(nodes[root].left) + count_nodes(nodes[root].right);
}

int height(int root) {
    int lh;
    int rh;
    if (root == -1) return 0;
    lh = height(nodes[root].left);
    rh = height(nodes[root].right);
    if (lh > rh) return lh + 1;
    return rh + 1;
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

int trees_equal(int r1, int r2) {
    if (r1 == -1 && r2 == -1) return 1;
    if (r1 == -1 || r2 == -1) return 0;
    if (nodes[r1].key != nodes[r2].key) return 0;
    if (!trees_equal(nodes[r1].left, nodes[r2].left)) return 0;
    if (!trees_equal(nodes[r1].right, nodes[r2].right)) return 0;
    return 1;
}

int main(void) {
    int root;
    int root2;
    int i;
    int orig_count;

    root = -1;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);

    orig_count = count_nodes(root);
    // EXPECT: orig count: 7
    printf("orig count: %d\n", orig_count);
    // EXPECT: orig height: 3
    printf("orig height: %d\n", height(root));

    ser_len = 0;
    serialize(root);
    // EXPECT: ser len: 15
    printf("ser len: %d\n", ser_len);
    // EXPECT: ser[0]: 50
    printf("ser[0]: %d\n", ser_buf[0]);
    // EXPECT: ser[1]: 30
    printf("ser[1]: %d\n", ser_buf[1]);
    // EXPECT: ser[2]: 20
    printf("ser[2]: %d\n", ser_buf[2]);

    deser_idx = 0;
    root2 = deserialize();
    // EXPECT: deser count: 7
    printf("deser count: %d\n", count_nodes(root2));
    // EXPECT: deser height: 3
    printf("deser height: %d\n", height(root2));
    // EXPECT: root2 key: 50
    printf("root2 key: %d\n", nodes[root2].key);

    inorder_cnt = 0;
    inorder(root2);
    // EXPECT: deser inorder: 20 30 40 50 60 70 80
    printf("deser inorder:");
    for (i = 0; i < inorder_cnt; i = i + 1) {
        printf(" %d", inorder_buf[i]);
    }
    printf("\n");

    // EXPECT: trees equal: 1
    printf("trees equal: %d\n", trees_equal(root, root2));

    root = -1;
    root = insert(root, 10);
    root = insert(root, 5);
    root = insert(root, 15);

    ser_len = 0;
    serialize(root);
    // EXPECT: small ser len: 7
    printf("small ser len: %d\n", ser_len);

    deser_idx = 0;
    root2 = deserialize();
    // EXPECT: small deser count: 3
    printf("small deser count: %d\n", count_nodes(root2));
    // EXPECT: small equal: 1
    printf("small equal: %d\n", trees_equal(root, root2));

    // EXPECT: small root: 10
    printf("small root: %d\n", nodes[root2].key);

    return 0;
}
