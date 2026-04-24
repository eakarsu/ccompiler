int printf(const char *fmt, ...);

/* Binary tree using array-based nodes with explicit left/right indices.
   Index -1 means null. */

struct Node {
    int val;
    int left;
    int right;
};

struct Node nodes[20];
int node_count;

int new_node(int val) {
    int idx;
    idx = node_count;
    nodes[idx].val = val;
    nodes[idx].left = -1;
    nodes[idx].right = -1;
    node_count = node_count + 1;
    return idx;
}

void set_left(int parent, int child) {
    nodes[parent].left = child;
}

void set_right(int parent, int child) {
    nodes[parent].right = child;
}

/*       1
        / \
       2   3
      / \   \
     4   5   6
    /
   7
*/

void preorder(int idx) {
    if (idx == -1) return;
    // EXPECT: pre: 1
    // EXPECT: pre: 2
    // EXPECT: pre: 4
    // EXPECT: pre: 7
    // EXPECT: pre: 5
    // EXPECT: pre: 3
    // EXPECT: pre: 6
    printf("pre: %d\n", nodes[idx].val);
    preorder(nodes[idx].left);
    preorder(nodes[idx].right);
}

void inorder(int idx) {
    if (idx == -1) return;
    inorder(nodes[idx].left);
    // EXPECT: in: 7
    // EXPECT: in: 4
    // EXPECT: in: 2
    // EXPECT: in: 5
    // EXPECT: in: 1
    // EXPECT: in: 3
    // EXPECT: in: 6
    printf("in: %d\n", nodes[idx].val);
    inorder(nodes[idx].right);
}

void postorder(int idx) {
    if (idx == -1) return;
    postorder(nodes[idx].left);
    postorder(nodes[idx].right);
    // EXPECT: post: 7
    // EXPECT: post: 4
    // EXPECT: post: 5
    // EXPECT: post: 2
    // EXPECT: post: 6
    // EXPECT: post: 3
    // EXPECT: post: 1
    printf("post: %d\n", nodes[idx].val);
}

int tree_height(int idx) {
    int lh;
    int rh;
    if (idx == -1) return 0;
    lh = tree_height(nodes[idx].left);
    rh = tree_height(nodes[idx].right);
    if (lh > rh) return lh + 1;
    return rh + 1;
}

int count_leaves(int idx) {
    if (idx == -1) return 0;
    if (nodes[idx].left == -1 && nodes[idx].right == -1) return 1;
    return count_leaves(nodes[idx].left) + count_leaves(nodes[idx].right);
}

int count_nodes(int idx) {
    if (idx == -1) return 0;
    return 1 + count_nodes(nodes[idx].left) + count_nodes(nodes[idx].right);
}

int main(void) {
    int n1; int n2; int n3; int n4; int n5; int n6; int n7;

    node_count = 0;
    n1 = new_node(1);
    n2 = new_node(2);
    n3 = new_node(3);
    n4 = new_node(4);
    n5 = new_node(5);
    n6 = new_node(6);
    n7 = new_node(7);

    set_left(n1, n2);
    set_right(n1, n3);
    set_left(n2, n4);
    set_right(n2, n5);
    set_right(n3, n6);
    set_left(n4, n7);

    preorder(n1);
    inorder(n1);
    postorder(n1);

    // EXPECT: height=4
    printf("height=%d\n", tree_height(n1));
    // EXPECT: leaves=3
    printf("leaves=%d\n", count_leaves(n1));
    // EXPECT: total=7
    printf("total=%d\n", count_nodes(n1));

    return 0;
}
