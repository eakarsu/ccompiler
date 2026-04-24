int printf(const char *fmt, ...);
// EXPECT: 1: 1 1 1 1 0 0 0\n2: 20 30 40 50 60 70 80 \n3: 10 90\n4: 30 40 50 60 80 -1\n5: 70 60 40 -1\n6: 4 30 40 50 70 1\n7: 7 10 30 40 50 60 70 80 1\n8: 6 20 40 50 60 70 80 1\n9: 6 60 20 30 40 60 70 80 1\n10: 3 5 1\n11: 0 1 3 7\n12: 1 1\n13: 10 10 1 12 23 34 45 56 67 78 89 90 \n14: 3 30 50 70 1\n15: 20 30 40 50 60 70 80 7\n16: 80 70 60 50 40 30 20 7\n17: 5 5 10 20 30 40 50 \n18: 3 30 50 70 1\n19: 2 1 0 1\n20: 15 4 6 93 1
// ============================================================================
// Binary Search Tree Using Array-Based Node Pool
// ============================================================================
// This file implements a BST using a fixed array of nodes (node pool).
// Each node has: key, left child index, right child index, parent index.
// -1 indicates null (no child/parent).
// Operations: insert, search, delete, inorder traversal, min/max, successor.
// ============================================================================

#define MAX_NODES 100
#define NIL -1

// Node pool arrays
int bst_key[MAX_NODES];
int bst_left[MAX_NODES];
int bst_right[MAX_NODES];
int bst_parent[MAX_NODES];

// Pool management
int pool_next;  // next free slot
int root;       // root of the BST

// Inorder traversal result buffer
int inorder_buf[MAX_NODES];
int inorder_count;

// ============================================================================
// Initialize the BST (empty tree)
// ============================================================================
void bst_init(void) {
    int i;
    for (i = 0; i < MAX_NODES; i = i + 1) {
        bst_key[i] = 0;
        bst_left[i] = NIL;
        bst_right[i] = NIL;
        bst_parent[i] = NIL;
    }
    pool_next = 0;
    root = NIL;
    inorder_count = 0;
}

// ============================================================================
// Allocate a new node from the pool
// Returns the index of the new node, or -1 if pool is full
// ============================================================================
int alloc_node(int k) {
    int idx;
    if (pool_next >= MAX_NODES) return NIL;
    idx = pool_next;
    pool_next = pool_next + 1;
    bst_key[idx] = k;
    bst_left[idx] = NIL;
    bst_right[idx] = NIL;
    bst_parent[idx] = NIL;
    return idx;
}

// ============================================================================
// Insert a key into the BST
// Returns the index of the newly inserted node
// ============================================================================
int bst_insert(int k) {
    int new_node;
    int curr;
    int par;

    new_node = alloc_node(k);
    if (new_node == NIL) return NIL;

    if (root == NIL) {
        root = new_node;
        return new_node;
    }

    curr = root;
    par = NIL;

    while (curr != NIL) {
        par = curr;
        if (k < bst_key[curr]) {
            curr = bst_left[curr];
        } else {
            curr = bst_right[curr];
        }
    }

    bst_parent[new_node] = par;
    if (k < bst_key[par]) {
        bst_left[par] = new_node;
    } else {
        bst_right[par] = new_node;
    }

    return new_node;
}

// ============================================================================
// Search for a key in the BST
// Returns the index of the node, or NIL if not found
// ============================================================================
int bst_search(int k) {
    int curr;
    curr = root;
    while (curr != NIL) {
        if (k == bst_key[curr]) return curr;
        if (k < bst_key[curr]) {
            curr = bst_left[curr];
        } else {
            curr = bst_right[curr];
        }
    }
    return NIL;
}

// ============================================================================
// Find the minimum node in the subtree rooted at node
// Returns the index of the minimum node
// ============================================================================
int bst_minimum(int node) {
    if (node == NIL) return NIL;
    while (bst_left[node] != NIL) {
        node = bst_left[node];
    }
    return node;
}

// ============================================================================
// Find the maximum node in the subtree rooted at node
// Returns the index of the maximum node
// ============================================================================
int bst_maximum(int node) {
    if (node == NIL) return NIL;
    while (bst_right[node] != NIL) {
        node = bst_right[node];
    }
    return node;
}

// ============================================================================
// Find the successor of a node (next larger key)
// Returns the index of the successor node, or NIL
// ============================================================================
int bst_successor(int node) {
    int par;

    if (node == NIL) return NIL;

    // If right subtree exists, successor is the minimum of right subtree
    if (bst_right[node] != NIL) {
        return bst_minimum(bst_right[node]);
    }

    // Otherwise, go up until we find an ancestor we are in the left subtree of
    par = bst_parent[node];
    while (par != NIL && node == bst_right[par]) {
        node = par;
        par = bst_parent[par];
    }
    return par;
}

// ============================================================================
// Find the predecessor of a node (next smaller key)
// Returns the index of the predecessor node, or NIL
// ============================================================================
int bst_predecessor(int node) {
    int par;

    if (node == NIL) return NIL;

    if (bst_left[node] != NIL) {
        return bst_maximum(bst_left[node]);
    }

    par = bst_parent[node];
    while (par != NIL && node == bst_left[par]) {
        node = par;
        par = bst_parent[par];
    }
    return par;
}

// ============================================================================
// Transplant: replace subtree rooted at u with subtree rooted at v
// ============================================================================
void bst_transplant(int u, int v) {
    if (bst_parent[u] == NIL) {
        root = v;
    } else if (u == bst_left[bst_parent[u]]) {
        bst_left[bst_parent[u]] = v;
    } else {
        bst_right[bst_parent[u]] = v;
    }
    if (v != NIL) {
        bst_parent[v] = bst_parent[u];
    }
}

// ============================================================================
// Delete a node from the BST
// ============================================================================
void bst_delete(int node) {
    int succ;

    if (node == NIL) return;

    if (bst_left[node] == NIL) {
        bst_transplant(node, bst_right[node]);
    } else if (bst_right[node] == NIL) {
        bst_transplant(node, bst_left[node]);
    } else {
        // Node has two children: replace with successor
        succ = bst_minimum(bst_right[node]);
        if (bst_parent[succ] != node) {
            bst_transplant(succ, bst_right[succ]);
            bst_right[succ] = bst_right[node];
            bst_parent[bst_right[succ]] = succ;
        }
        bst_transplant(node, succ);
        bst_left[succ] = bst_left[node];
        bst_parent[bst_left[succ]] = succ;
    }
}

// ============================================================================
// Inorder traversal: fills inorder_buf with keys in sorted order
// ============================================================================
void inorder_helper(int node) {
    if (node == NIL) return;
    inorder_helper(bst_left[node]);
    inorder_buf[inorder_count] = bst_key[node];
    inorder_count = inorder_count + 1;
    inorder_helper(bst_right[node]);
}

void bst_inorder(void) {
    inorder_count = 0;
    inorder_helper(root);
}

// ============================================================================
// Count the number of nodes in the tree
// ============================================================================
int count_nodes(int node) {
    if (node == NIL) return 0;
    return 1 + count_nodes(bst_left[node]) + count_nodes(bst_right[node]);
}

// ============================================================================
// Compute the height of the tree
// ============================================================================
int tree_height(int node) {
    int lh;
    int rh;
    if (node == NIL) return 0;
    lh = tree_height(bst_left[node]);
    rh = tree_height(bst_right[node]);
    if (lh > rh) return lh + 1;
    return rh + 1;
}

// ============================================================================
// Check if the tree satisfies BST property
// ============================================================================
int is_bst_helper(int node, int min_val, int max_val) {
    if (node == NIL) return 1;
    if (bst_key[node] < min_val || bst_key[node] > max_val) return 0;
    return is_bst_helper(bst_left[node], min_val, bst_key[node] - 1) &&
           is_bst_helper(bst_right[node], bst_key[node], max_val);
}

int is_bst(void) {
    return is_bst_helper(root, -999999, 999999);
}

// ============================================================================
// Test Case 1: Basic insert and search
// ============================================================================
void test_case_1(void) {
    printf("%d", 1);
    printf("%c", ':');
    printf("%c", ' ');

    bst_init();
    bst_insert(50);
    bst_insert(30);
    bst_insert(70);
    bst_insert(20);
    bst_insert(40);
    bst_insert(60);
    bst_insert(80);

    // Search for existing keys
    printf("%d", bst_search(50) != NIL ? 1 : 0);
    printf("%c", ' ');
    printf("%d", bst_search(30) != NIL ? 1 : 0);
    printf("%c", ' ');
    printf("%d", bst_search(70) != NIL ? 1 : 0);
    printf("%c", ' ');
    printf("%d", bst_search(20) != NIL ? 1 : 0);
    printf("%c", ' ');

    // Search for non-existing keys
    printf("%d", bst_search(10) != NIL ? 1 : 0);
    printf("%c", ' ');
    printf("%d", bst_search(55) != NIL ? 1 : 0);
    printf("%c", ' ');
    printf("%d", bst_search(100) != NIL ? 1 : 0);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 2: Inorder traversal
// ============================================================================
void test_case_2(void) {
    int i;

    printf("%d", 2);
    printf("%c", ':');
    printf("%c", ' ');

    bst_init();
    bst_insert(50);
    bst_insert(30);
    bst_insert(70);
    bst_insert(20);
    bst_insert(40);
    bst_insert(60);
    bst_insert(80);

    bst_inorder();
    for (i = 0; i < inorder_count; i = i + 1) {
        printf("%d", inorder_buf[i]);
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 3: Min and Max
// ============================================================================
void test_case_3(void) {
    int min_node;
    int max_node;

    printf("%d", 3);
    printf("%c", ':');
    printf("%c", ' ');

    bst_init();
    bst_insert(50);
    bst_insert(30);
    bst_insert(70);
    bst_insert(20);
    bst_insert(40);
    bst_insert(60);
    bst_insert(80);
    bst_insert(10);
    bst_insert(90);

    min_node = bst_minimum(root);
    max_node = bst_maximum(root);
    printf("%d", bst_key[min_node]);
    printf("%c", ' ');
    printf("%d", bst_key[max_node]);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 4: Successor
// ============================================================================
void test_case_4(void) {
    int node;
    int succ;

    printf("%d", 4);
    printf("%c", ':');
    printf("%c", ' ');

    bst_init();
    bst_insert(50);
    bst_insert(30);
    bst_insert(70);
    bst_insert(20);
    bst_insert(40);
    bst_insert(60);
    bst_insert(80);

    // Successor of 20 is 30
    node = bst_search(20);
    succ = bst_successor(node);
    printf("%d", bst_key[succ]);
    printf("%c", ' ');

    // Successor of 30 is 40
    node = bst_search(30);
    succ = bst_successor(node);
    printf("%d", bst_key[succ]);
    printf("%c", ' ');

    // Successor of 40 is 50
    node = bst_search(40);
    succ = bst_successor(node);
    printf("%d", bst_key[succ]);
    printf("%c", ' ');

    // Successor of 50 is 60
    node = bst_search(50);
    succ = bst_successor(node);
    printf("%d", bst_key[succ]);
    printf("%c", ' ');

    // Successor of 70 is 80
    node = bst_search(70);
    succ = bst_successor(node);
    printf("%d", bst_key[succ]);
    printf("%c", ' ');

    // Successor of 80 is NIL
    node = bst_search(80);
    succ = bst_successor(node);
    printf("%d", succ);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 5: Predecessor
// ============================================================================
void test_case_5(void) {
    int node;
    int pred;

    printf("%d", 5);
    printf("%c", ':');
    printf("%c", ' ');

    bst_init();
    bst_insert(50);
    bst_insert(30);
    bst_insert(70);
    bst_insert(20);
    bst_insert(40);
    bst_insert(60);
    bst_insert(80);

    // Predecessor of 80 is 70
    node = bst_search(80);
    pred = bst_predecessor(node);
    printf("%d", bst_key[pred]);
    printf("%c", ' ');

    // Predecessor of 70 is 60
    node = bst_search(70);
    pred = bst_predecessor(node);
    printf("%d", bst_key[pred]);
    printf("%c", ' ');

    // Predecessor of 50 is 40
    node = bst_search(50);
    pred = bst_predecessor(node);
    printf("%d", bst_key[pred]);
    printf("%c", ' ');

    // Predecessor of 20 is NIL
    node = bst_search(20);
    pred = bst_predecessor(node);
    printf("%d", pred);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 6: Delete leaf node
// ============================================================================
void test_case_6(void) {
    int node;
    int i;

    printf("%d", 6);
    printf("%c", ':');
    printf("%c", ' ');

    bst_init();
    bst_insert(50);
    bst_insert(30);
    bst_insert(70);
    bst_insert(20);
    bst_insert(40);

    // Delete leaf node 20
    node = bst_search(20);
    bst_delete(node);

    printf("%d", count_nodes(root));
    printf("%c", ' ');

    bst_inorder();
    for (i = 0; i < inorder_count; i = i + 1) {
        printf("%d", inorder_buf[i]);
        printf("%c", ' ');
    }
    printf("%d", is_bst());
    printf("%c", '\n');
}

// ============================================================================
// Test Case 7: Delete node with one child
// ============================================================================
void test_case_7(void) {
    int node;
    int i;

    printf("%d", 7);
    printf("%c", ':');
    printf("%c", ' ');

    bst_init();
    bst_insert(50);
    bst_insert(30);
    bst_insert(70);
    bst_insert(20);
    bst_insert(40);
    bst_insert(60);
    bst_insert(80);
    bst_insert(10);

    // Delete 20 (which has left child 10)
    node = bst_search(20);
    bst_delete(node);

    printf("%d", count_nodes(root));
    printf("%c", ' ');

    bst_inorder();
    for (i = 0; i < inorder_count; i = i + 1) {
        printf("%d", inorder_buf[i]);
        printf("%c", ' ');
    }
    printf("%d", is_bst());
    printf("%c", '\n');
}

// ============================================================================
// Test Case 8: Delete node with two children
// ============================================================================
void test_case_8(void) {
    int node;
    int i;

    printf("%d", 8);
    printf("%c", ':');
    printf("%c", ' ');

    bst_init();
    bst_insert(50);
    bst_insert(30);
    bst_insert(70);
    bst_insert(20);
    bst_insert(40);
    bst_insert(60);
    bst_insert(80);

    // Delete 30 (has children 20 and 40)
    node = bst_search(30);
    bst_delete(node);

    printf("%d", count_nodes(root));
    printf("%c", ' ');

    bst_inorder();
    for (i = 0; i < inorder_count; i = i + 1) {
        printf("%d", inorder_buf[i]);
        printf("%c", ' ');
    }
    printf("%d", is_bst());
    printf("%c", '\n');
}

// ============================================================================
// Test Case 9: Delete root
// ============================================================================
void test_case_9(void) {
    int node;
    int i;

    printf("%d", 9);
    printf("%c", ':');
    printf("%c", ' ');

    bst_init();
    bst_insert(50);
    bst_insert(30);
    bst_insert(70);
    bst_insert(20);
    bst_insert(40);
    bst_insert(60);
    bst_insert(80);

    // Delete root (50)
    node = bst_search(50);
    bst_delete(node);

    printf("%d", count_nodes(root));
    printf("%c", ' ');
    printf("%d", bst_key[root]);
    printf("%c", ' ');

    bst_inorder();
    for (i = 0; i < inorder_count; i = i + 1) {
        printf("%d", inorder_buf[i]);
        printf("%c", ' ');
    }
    printf("%d", is_bst());
    printf("%c", '\n');
}

// ============================================================================
// Test Case 10: Tree height
// ============================================================================
void test_case_10(void) {
    printf("%d", 10);
    printf("%c", ':');
    printf("%c", ' ');

    // Balanced tree
    bst_init();
    bst_insert(50);
    bst_insert(30);
    bst_insert(70);
    bst_insert(20);
    bst_insert(40);
    bst_insert(60);
    bst_insert(80);

    printf("%d", tree_height(root));
    printf("%c", ' ');

    // Skewed tree (ascending insertion)
    bst_init();
    bst_insert(10);
    bst_insert(20);
    bst_insert(30);
    bst_insert(40);
    bst_insert(50);

    printf("%d", tree_height(root));
    printf("%c", ' ');

    // Single node
    bst_init();
    bst_insert(42);
    printf("%d", tree_height(root));
    printf("%c", '\n');
}

// ============================================================================
// Test Case 11: Node count
// ============================================================================
void test_case_11(void) {
    printf("%d", 11);
    printf("%c", ':');
    printf("%c", ' ');

    bst_init();
    printf("%d", count_nodes(root));
    printf("%c", ' ');

    bst_insert(10);
    printf("%d", count_nodes(root));
    printf("%c", ' ');

    bst_insert(5);
    bst_insert(15);
    printf("%d", count_nodes(root));
    printf("%c", ' ');

    bst_insert(3);
    bst_insert(7);
    bst_insert(12);
    bst_insert(20);
    printf("%d", count_nodes(root));
    printf("%c", '\n');
}

// ============================================================================
// Test Case 12: BST property verification
// ============================================================================
void test_case_12(void) {
    printf("%d", 12);
    printf("%c", ':');
    printf("%c", ' ');

    bst_init();
    bst_insert(50);
    bst_insert(30);
    bst_insert(70);
    bst_insert(20);
    bst_insert(40);
    printf("%d", is_bst());
    printf("%c", ' ');

    bst_insert(60);
    bst_insert(80);
    printf("%d", is_bst());
    printf("%c", '\n');
}

// ============================================================================
// Test Case 13: Insert and search many values
// ============================================================================
void test_case_13(void) {
    int vals[10];
    int i;
    int found;

    printf("%d", 13);
    printf("%c", ':');
    printf("%c", ' ');

    bst_init();
    vals[0] = 45; vals[1] = 23; vals[2] = 67; vals[3] = 12; vals[4] = 34;
    vals[5] = 56; vals[6] = 78; vals[7] = 1; vals[8] = 89; vals[9] = 90;

    for (i = 0; i < 10; i = i + 1) {
        bst_insert(vals[i]);
    }

    printf("%d", count_nodes(root));
    printf("%c", ' ');

    // Search all
    found = 0;
    for (i = 0; i < 10; i = i + 1) {
        if (bst_search(vals[i]) != NIL) found = found + 1;
    }
    printf("%d", found);
    printf("%c", ' ');

    // Inorder should be sorted
    bst_inorder();
    for (i = 0; i < inorder_count; i = i + 1) {
        printf("%d", inorder_buf[i]);
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 14: Delete multiple nodes
// ============================================================================
void test_case_14(void) {
    int node;
    int i;

    printf("%d", 14);
    printf("%c", ':');
    printf("%c", ' ');

    bst_init();
    bst_insert(50);
    bst_insert(30);
    bst_insert(70);
    bst_insert(20);
    bst_insert(40);
    bst_insert(60);
    bst_insert(80);

    // Delete 20, 40, 60, 80 (all leaves)
    node = bst_search(20);
    bst_delete(node);
    node = bst_search(40);
    bst_delete(node);
    node = bst_search(60);
    bst_delete(node);
    node = bst_search(80);
    bst_delete(node);

    printf("%d", count_nodes(root));
    printf("%c", ' ');

    bst_inorder();
    for (i = 0; i < inorder_count; i = i + 1) {
        printf("%d", inorder_buf[i]);
        printf("%c", ' ');
    }
    printf("%d", is_bst());
    printf("%c", '\n');
}

// ============================================================================
// Test Case 15: Successor traversal (walk through all elements)
// ============================================================================
void test_case_15(void) {
    int node;
    int count;

    printf("%d", 15);
    printf("%c", ':');
    printf("%c", ' ');

    bst_init();
    bst_insert(50);
    bst_insert(30);
    bst_insert(70);
    bst_insert(20);
    bst_insert(40);
    bst_insert(60);
    bst_insert(80);

    // Start from minimum, walk through successors
    node = bst_minimum(root);
    count = 0;
    while (node != NIL) {
        printf("%d", bst_key[node]);
        printf("%c", ' ');
        node = bst_successor(node);
        count = count + 1;
    }
    printf("%d", count);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 16: Predecessor traversal (walk backwards)
// ============================================================================
void test_case_16(void) {
    int node;
    int count;

    printf("%d", 16);
    printf("%c", ':');
    printf("%c", ' ');

    bst_init();
    bst_insert(50);
    bst_insert(30);
    bst_insert(70);
    bst_insert(20);
    bst_insert(40);
    bst_insert(60);
    bst_insert(80);

    // Start from maximum, walk through predecessors
    node = bst_maximum(root);
    count = 0;
    while (node != NIL) {
        printf("%d", bst_key[node]);
        printf("%c", ' ');
        node = bst_predecessor(node);
        count = count + 1;
    }
    printf("%d", count);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 17: Descending insertion (worst case skew)
// ============================================================================
void test_case_17(void) {
    int i;

    printf("%d", 17);
    printf("%c", ':');
    printf("%c", ' ');

    bst_init();
    bst_insert(50);
    bst_insert(40);
    bst_insert(30);
    bst_insert(20);
    bst_insert(10);

    printf("%d", tree_height(root));
    printf("%c", ' ');
    printf("%d", count_nodes(root));
    printf("%c", ' ');

    bst_inorder();
    for (i = 0; i < inorder_count; i = i + 1) {
        printf("%d", inorder_buf[i]);
        printf("%c", ' ');
    }
    printf("%c", '\n');
}

// ============================================================================
// Test Case 18: Delete then reinsert
// ============================================================================
void test_case_18(void) {
    int node;
    int i;

    printf("%d", 18);
    printf("%c", ':');
    printf("%c", ' ');

    bst_init();
    bst_insert(50);
    bst_insert(30);
    bst_insert(70);

    // Delete 30
    node = bst_search(30);
    bst_delete(node);

    // Reinsert 30
    bst_insert(30);

    printf("%d", count_nodes(root));
    printf("%c", ' ');

    bst_inorder();
    for (i = 0; i < inorder_count; i = i + 1) {
        printf("%d", inorder_buf[i]);
        printf("%c", ' ');
    }
    printf("%d", is_bst());
    printf("%c", '\n');
}

// ============================================================================
// Test Case 19: Delete all nodes one by one
// ============================================================================
void test_case_19(void) {
    int node;

    printf("%d", 19);
    printf("%c", ':');
    printf("%c", ' ');

    bst_init();
    bst_insert(50);
    bst_insert(30);
    bst_insert(70);

    node = bst_search(30);
    bst_delete(node);
    printf("%d", count_nodes(root));
    printf("%c", ' ');

    node = bst_search(70);
    bst_delete(node);
    printf("%d", count_nodes(root));
    printf("%c", ' ');

    node = bst_search(50);
    bst_delete(node);
    printf("%d", count_nodes(root));
    printf("%c", ' ');

    // Root should be NIL now
    printf("%d", root == NIL ? 1 : 0);
    printf("%c", '\n');
}

// ============================================================================
// Test Case 20: Large tree operations
// ============================================================================
void test_case_20(void) {
    int i;
    int vals[15];
    int min_node;
    int max_node;

    printf("%d", 20);
    printf("%c", ':');
    printf("%c", ' ');

    bst_init();
    vals[0] = 50; vals[1] = 25; vals[2] = 75; vals[3] = 12;
    vals[4] = 37; vals[5] = 62; vals[6] = 87; vals[7] = 6;
    vals[8] = 18; vals[9] = 31; vals[10] = 43; vals[11] = 56;
    vals[12] = 68; vals[13] = 81; vals[14] = 93;

    for (i = 0; i < 15; i = i + 1) {
        bst_insert(vals[i]);
    }

    printf("%d", count_nodes(root));
    printf("%c", ' ');
    printf("%d", tree_height(root));
    printf("%c", ' ');

    min_node = bst_minimum(root);
    max_node = bst_maximum(root);
    printf("%d", bst_key[min_node]);
    printf("%c", ' ');
    printf("%d", bst_key[max_node]);
    printf("%c", ' ');

    printf("%d", is_bst());
    printf("%c", '\n');
}

// ============================================================================
// Main function
// ============================================================================

// 2: 20 30 40 50 60 70 80
// 3: 10 90
// 4: 30 40 50 60 80 -1
// 5: 70 60 40 -1
// 6: 4 30 40 50 70 1
// 7: 7 10 30 40 50 60 70 80 1
// 8: 6 20 40 50 60 70 80 1
// 9: 6 60 20 30 40 60 70 80 1
// 10: 3 5 1
// 11: 0 1 3 7
// 12: 1 1
// 13: 10 10 1 12 23 34 45 56 67 78 89 90
// 14: 3 30 50 70 1
// 15: 20 30 40 50 60 70 80 7
// 16: 80 70 60 50 40 30 20 7
// 17: 5 5 10 20 30 40 50
// 18: 3 30 50 70 1
// 19: 2 1 0 1
// 20: 15 4 6 93 1

int main(void) {
    test_case_1();
    test_case_2();
    test_case_3();
    test_case_4();
    test_case_5();
    test_case_6();
    test_case_7();
    test_case_8();
    test_case_9();
    test_case_10();
    test_case_11();
    test_case_12();
    test_case_13();
    test_case_14();
    test_case_15();
    test_case_16();
    test_case_17();
    test_case_18();
    test_case_19();
    test_case_20();
    return 0;
}
