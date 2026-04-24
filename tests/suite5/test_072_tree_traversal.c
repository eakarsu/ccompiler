int printf(const char *fmt, ...);

struct Node {
    int key;
    int left;
    int right;
};

struct Node nodes[64];
int node_count = 0;

int result[64];
int result_count = 0;

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

void inorder(int root) {
    if (root == -1) return;
    inorder(nodes[root].left);
    result[result_count] = nodes[root].key;
    result_count = result_count + 1;
    inorder(nodes[root].right);
}

void preorder(int root) {
    if (root == -1) return;
    result[result_count] = nodes[root].key;
    result_count = result_count + 1;
    preorder(nodes[root].left);
    preorder(nodes[root].right);
}

void postorder(int root) {
    if (root == -1) return;
    postorder(nodes[root].left);
    postorder(nodes[root].right);
    result[result_count] = nodes[root].key;
    result_count = result_count + 1;
}

int queue[64];
int q_front = 0;
int q_back = 0;

void q_push(int val) {
    queue[q_back] = val;
    q_back = q_back + 1;
}

int q_pop(void) {
    int val = queue[q_front];
    q_front = q_front + 1;
    return val;
}

int q_empty(void) {
    return q_front == q_back;
}

void level_order(int root) {
    int cur;
    q_front = 0;
    q_back = 0;
    if (root == -1) return;
    q_push(root);
    while (!q_empty()) {
        cur = q_pop();
        result[result_count] = nodes[cur].key;
        result_count = result_count + 1;
        if (nodes[cur].left != -1) {
            q_push(nodes[cur].left);
        }
        if (nodes[cur].right != -1) {
            q_push(nodes[cur].right);
        }
    }
}

void print_result(void) {
    int i;
    for (i = 0; i < result_count; i = i + 1) {
        if (i > 0) {
            printf(" %d", result[i]);
        } else {
            printf("%d", result[i]);
        }
    }
    printf("\n");
}

int main(void) {
    int root;
    int root2;

    root = -1;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);

    result_count = 0;
    inorder(root);
    // EXPECT: inorder: 20 30 40 50 60 70 80
    printf("inorder: ");
    print_result();

    result_count = 0;
    preorder(root);
    // EXPECT: preorder: 50 30 20 40 70 60 80
    printf("preorder: ");
    print_result();

    result_count = 0;
    postorder(root);
    // EXPECT: postorder: 20 40 30 60 80 70 50
    printf("postorder: ");
    print_result();

    result_count = 0;
    level_order(root);
    // EXPECT: levelorder: 50 30 70 20 40 60 80
    printf("levelorder: ");
    print_result();

    // EXPECT: inorder count: 7
    printf("inorder count: %d\n", 7);

    root2 = -1;
    root2 = insert(root2, 15);
    root2 = insert(root2, 10);
    root2 = insert(root2, 25);
    root2 = insert(root2, 5);
    root2 = insert(root2, 12);
    root2 = insert(root2, 20);
    root2 = insert(root2, 30);
    root2 = insert(root2, 1);

    result_count = 0;
    inorder(root2);
    // EXPECT: inorder2: 1 5 10 12 15 20 25 30
    printf("inorder2: ");
    print_result();

    result_count = 0;
    preorder(root2);
    // EXPECT: preorder2: 15 10 5 1 12 25 20 30
    printf("preorder2: ");
    print_result();

    result_count = 0;
    postorder(root2);
    // EXPECT: postorder2: 1 5 12 10 20 30 25 15
    printf("postorder2: ");
    print_result();

    result_count = 0;
    level_order(root2);
    // EXPECT: levelorder2: 15 10 25 5 12 20 30 1
    printf("levelorder2: ");
    print_result();

    // EXPECT: first inorder: 20
    result_count = 0;
    inorder(root);
    printf("first inorder: %d\n", result[0]);

    // EXPECT: last inorder: 80
    printf("last inorder: %d\n", result[6]);

    // EXPECT: first preorder: 50
    result_count = 0;
    preorder(root);
    printf("first preorder: %d\n", result[0]);

    // EXPECT: first postorder: 20
    result_count = 0;
    postorder(root);
    printf("first postorder: %d\n", result[0]);

    // EXPECT: last postorder: 50
    printf("last postorder: %d\n", result[6]);

    // EXPECT: first level: 50
    result_count = 0;
    level_order(root);
    printf("first level: %d\n", result[0]);

    return 0;
}
