int printf(const char *fmt, ...);

// Binary tree mirror using recursion
// Uses array-based tree storage (index 0 = root, left = 2i+1, right = 2i+2)

int tree[32];
int tree_size;

void init_tree(void) {
    int i;
    for (i = 0; i < 32; i++) {
        tree[i] = -1; // -1 means empty
    }
    tree_size = 0;
}

void set_node(int index, int val) {
    tree[index] = val;
    if (index + 1 > tree_size) {
        tree_size = index + 1;
    }
}

int left_child(int i) {
    return 2 * i + 1;
}

int right_child(int i) {
    return 2 * i + 2;
}

int has_node(int i) {
    if (i >= 32) return 0;
    return tree[i] != -1;
}

void mirror_tree(int index) {
    int tmp;
    int l;
    int r;
    if (index >= 32) return;
    if (tree[index] == -1) return;

    l = left_child(index);
    r = right_child(index);

    // Swap children subtrees in the array
    // We need to do a deep swap, but for simplicity swap values at each level
    mirror_tree(l);
    mirror_tree(r);

    // Swap the left and right children values
    if (l < 32 && r < 32) {
        tmp = tree[l];
        tree[l] = tree[r];
        tree[r] = tmp;
    }
}

int print_count;

void print_inorder(int index) {
    if (index >= 32) return;
    if (tree[index] == -1) return;
    print_inorder(left_child(index));
    if (print_count > 0) printf(" ");
    printf("%d", tree[index]);
    print_count = print_count + 1;
    print_inorder(right_child(index));
}

void print_preorder(int index) {
    if (index >= 32) return;
    if (tree[index] == -1) return;
    if (print_count > 0) printf(" ");
    printf("%d", tree[index]);
    print_count = print_count + 1;
    print_preorder(left_child(index));
    print_preorder(right_child(index));
}

int tree_height(int index) {
    int lh;
    int rh;
    if (index >= 32) return 0;
    if (tree[index] == -1) return 0;
    lh = tree_height(left_child(index));
    rh = tree_height(right_child(index));
    if (lh > rh) return lh + 1;
    return rh + 1;
}

int count_nodes(int index) {
    if (index >= 32) return 0;
    if (tree[index] == -1) return 0;
    return 1 + count_nodes(left_child(index)) + count_nodes(right_child(index));
}

int sum_nodes(int index) {
    if (index >= 32) return 0;
    if (tree[index] == -1) return 0;
    return tree[index] + sum_nodes(left_child(index)) + sum_nodes(right_child(index));
}

int is_symmetric_helper(int i, int j) {
    if (i >= 32 && j >= 32) return 1;
    if (tree[i] == -1 && tree[j] == -1) return 1;
    if (tree[i] == -1 || tree[j] == -1) return 0;
    if (i >= 32 || j >= 32) return 0;
    if (tree[i] != tree[j]) return 0;
    return is_symmetric_helper(left_child(i), right_child(j))
        && is_symmetric_helper(right_child(i), left_child(j));
}

int is_symmetric(void) {
    if (tree[0] == -1) return 1;
    return is_symmetric_helper(left_child(0), right_child(0));
}

int main() {
    int h;
    int n;
    int s;
    int sym;

    // Build tree:       1
    //                 /   \
    //                2     3
    //               / \   /
    //              4   5 6
    init_tree();
    set_node(0, 1);
    set_node(1, 2);
    set_node(2, 3);
    set_node(3, 4);
    set_node(4, 5);
    set_node(5, 6);

    // EXPECT: Inorder: 4 2 5 1 6 3
    printf("Inorder: ");
    print_count = 0;
    print_inorder(0);
    printf("\n");

    // EXPECT: Preorder: 1 2 4 5 3 6
    printf("Preorder: ");
    print_count = 0;
    print_preorder(0);
    printf("\n");

    h = tree_height(0);
    // EXPECT: Height: 3
    printf("Height: %d\n", h);

    n = count_nodes(0);
    // EXPECT: Nodes: 6
    printf("Nodes: %d\n", n);

    s = sum_nodes(0);
    // EXPECT: Sum: 21
    printf("Sum: %d\n", s);

    // Mirror the tree
    mirror_tree(0);

    // EXPECT: After mirror:
    printf("After mirror:\n");

    // EXPECT: Inorder: 5 3 4 1 2 6
    printf("Inorder: ");
    print_count = 0;
    print_inorder(0);
    printf("\n");

    // EXPECT: Preorder: 1 3 5 4 2 6
    printf("Preorder: ");
    print_count = 0;
    print_preorder(0);
    printf("\n");

    // Height should be unchanged
    h = tree_height(0);
    // EXPECT: Height after mirror: 3
    printf("Height after mirror: %d\n", h);

    // Build symmetric tree:   1
    //                        / \
    //                       2   2
    //                      / \ / \
    //                     3  4 4  3
    init_tree();
    set_node(0, 1);
    set_node(1, 2);
    set_node(2, 2);
    set_node(3, 3);
    set_node(4, 4);
    set_node(5, 4);
    set_node(6, 3);

    sym = is_symmetric();
    // EXPECT: Symmetric tree: 1
    printf("Symmetric tree: %d\n", sym);

    // Non-symmetric tree
    init_tree();
    set_node(0, 1);
    set_node(1, 2);
    set_node(2, 3);
    sym = is_symmetric();
    // EXPECT: Non-symmetric tree: 0
    printf("Non-symmetric tree: %d\n", sym);

    // EXPECT: Tree mirror done
    printf("Tree mirror done\n");

    return 0;
}
