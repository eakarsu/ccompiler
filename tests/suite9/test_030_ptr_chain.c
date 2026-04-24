int printf(const char *fmt, ...);

// Test: Pointer chain traversal - linked list via arrays, multi-level indirection

struct Node {
    int value;
    int next_idx;  // index into array, -1 = end
};

struct TreeNode {
    int value;
    int left_idx;   // -1 = null
    int right_idx;  // -1 = null
};

// Traverse a linked list implemented via array indices
int list_sum(struct Node *nodes, int head) {
    int sum = 0;
    int cur = head;
    while (cur != -1) {
        sum = sum + nodes[cur].value;
        cur = nodes[cur].next_idx;
    }
    return sum;
}

int list_length(struct Node *nodes, int head) {
    int len = 0;
    int cur = head;
    while (cur != -1) {
        len = len + 1;
        cur = nodes[cur].next_idx;
    }
    return len;
}

// Find value in list, return index or -1
int list_find(struct Node *nodes, int head, int target) {
    int cur = head;
    while (cur != -1) {
        if (nodes[cur].value == target) {
            return cur;
        }
        cur = nodes[cur].next_idx;
    }
    return -1;
}

// In-order traversal sum of binary tree via array
int tree_sum(struct TreeNode *tree, int root) {
    if (root == -1) return 0;
    int left_sum = tree_sum(tree, tree[root].left_idx);
    int right_sum = tree_sum(tree, tree[root].right_idx);
    return left_sum + tree[root].value + right_sum;
}

int tree_depth(struct TreeNode *tree, int root) {
    if (root == -1) return 0;
    int ld = tree_depth(tree, tree[root].left_idx);
    int rd = tree_depth(tree, tree[root].right_idx);
    if (ld > rd) return ld + 1;
    return rd + 1;
}

int main() {
    // Build a linked list: 10 -> 20 -> 30 -> 40 -> 50
    struct Node pool[10];
    pool[0].value = 10;
    pool[0].next_idx = 1;
    pool[1].value = 20;
    pool[1].next_idx = 2;
    pool[2].value = 30;
    pool[2].next_idx = 3;
    pool[3].value = 40;
    pool[3].next_idx = 4;
    pool[4].value = 50;
    pool[4].next_idx = -1;

    int head = 0;

    // Traverse and sum
    int s = list_sum(pool, head);
    // EXPECT: list sum: 150
    printf("list sum: %d\n", s);

    // Length
    int len = list_length(pool, head);
    // EXPECT: list length: 5
    printf("list length: %d\n", len);

    // Find
    int found = list_find(pool, head, 30);
    // EXPECT: find 30: 2
    printf("find 30: %d\n", found);

    int not_found = list_find(pool, head, 99);
    // EXPECT: find 99: -1
    printf("find 99: %d\n", not_found);

    // Pointer chain: follow multiple levels of indirection
    int val = 42;
    int *p1 = &val;
    int **p2 = &p1;
    int ***p3 = &p2;

    // EXPECT: chain depth 1: 42
    printf("chain depth 1: %d\n", *p1);
    // EXPECT: chain depth 2: 42
    printf("chain depth 2: %d\n", **p2);
    // EXPECT: chain depth 3: 42
    printf("chain depth 3: %d\n", ***p3);

    // Modify through chain
    ***p3 = 100;
    // EXPECT: val after chain mod: 100
    printf("val after chain mod: %d\n", val);

    // Array of pointers forming a chain
    int vals[5];
    vals[0] = 1;
    vals[1] = 2;
    vals[2] = 3;
    vals[3] = 4;
    vals[4] = 5;

    // next[i] tells which index to visit next, -1 = stop
    // Visit order: 0 -> 3 -> 1 -> 4 -> 2 -> stop
    int next[5];
    next[0] = 3;
    next[1] = 4;
    next[2] = -1;
    next[3] = 1;
    next[4] = 2;

    int chain_sum = 0;
    int cur = 0;
    int steps = 0;
    while (cur != -1) {
        chain_sum = chain_sum + vals[cur];
        cur = next[cur];
        steps = steps + 1;
    }
    // Visit: vals[0]=1, vals[3]=4, vals[1]=2, vals[4]=5, vals[2]=3 => 15
    // EXPECT: chain sum: 15
    printf("chain sum: %d\n", chain_sum);
    // EXPECT: chain steps: 5
    printf("chain steps: %d\n", steps);

    // Build a binary tree:
    //        10 (0)
    //       /    \
    //     5(1)   15(2)
    //    / \       \
    //   3(3) 7(4)  20(5)
    struct TreeNode tree[6];
    tree[0].value = 10;
    tree[0].left_idx = 1;
    tree[0].right_idx = 2;

    tree[1].value = 5;
    tree[1].left_idx = 3;
    tree[1].right_idx = 4;

    tree[2].value = 15;
    tree[2].left_idx = -1;
    tree[2].right_idx = 5;

    tree[3].value = 3;
    tree[3].left_idx = -1;
    tree[3].right_idx = -1;

    tree[4].value = 7;
    tree[4].left_idx = -1;
    tree[4].right_idx = -1;

    tree[5].value = 20;
    tree[5].left_idx = -1;
    tree[5].right_idx = -1;

    int ts = tree_sum(tree, 0);
    // EXPECT: tree sum: 60
    printf("tree sum: %d\n", ts);

    int td = tree_depth(tree, 0);
    // EXPECT: tree depth: 3
    printf("tree depth: %d\n", td);

    // Reverse linked list in-place (by changing next_idx)
    int prev = -1;
    cur = head;
    while (cur != -1) {
        int nxt = pool[cur].next_idx;
        pool[cur].next_idx = prev;
        prev = cur;
        cur = nxt;
    }
    head = prev;
    // Now list is: 50 -> 40 -> 30 -> 20 -> 10

    // Print reversed list values
    cur = head;
    // EXPECT: reversed: 50 40 30 20 10
    printf("reversed:");
    while (cur != -1) {
        printf(" %d", pool[cur].value);
        cur = pool[cur].next_idx;
    }
    printf("\n");

    // Sum after reversal should be same
    int rs = list_sum(pool, head);
    // EXPECT: reversed sum: 150
    printf("reversed sum: %d\n", rs);

    // EXPECT: ptr_chain done
    printf("ptr_chain done\n");

    return 0;
}
