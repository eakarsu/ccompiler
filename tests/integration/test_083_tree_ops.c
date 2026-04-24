int printf(const char *fmt, ...);
// EXPECT: === Basic Tree Tests ===\nNodes: 7\nLeaves: 4\nHeight: 3\nMin: 20\nMax: 80\nSum: 350\nIsBST: 1\nBalanced: 1\nDiameter: 4\nMaxWidth: 4\n=== Search Tests ===\nSearch 50: 1\nSearch 30: 2\nSearch 70: 3\nSearch 20: 4\nSearch 40: 5\nSearch 99: -1\nSearch 10: -1\nSearch 60: 6\nSearch 80: 7\n=== Traversal Tests ===\nLevel: 50 30 70 20 40 60 80\nInorder: 20 30 40 50 60 70 80 \nPreorder: 50 30 20 40 70 60 80 \nPostorder: 20 40 30 60 80 70 50 \n=== Delete Tests ===\nDelete 20 (leaf): 1\nNodes after: 6\nSearch 20: -1\nDelete 80 (leaf): 1\nNodes after: 5\nDelete 30 (non-leaf): 0\nNodes still: 5\n=== Depth and Width Tests ===\nDepth0: 1\nDepth1: 2\nDepth2: 4\nDepth3: 4\nHeight: 4\nMaxWidth: 4\nNodes: 11\nLeaves: 6\n=== Path Sum Tests ===\nPathSum 75: 1\nPathSum 85: 1\nPathSum 115: 1\nPathSum 170: 1\nPathSum 150: 1\nPathSum 100: 0\nPathSum 0: 0\n=== Mirror Tests ===\nBefore mirror level: 50 30 70 20 40\nBefore mirror inorder: 20 30 40 50 70 \nAfter mirror level: 50 70 30 40 20\nIsBST after mirror: 0\nRestored level: 50 30 70 20 40\nIsBST restored: 1\n=== Skewed Tree Tests ===\nSkew height: 5\nSkew nodes: 5\nSkew leaves: 1\nSkew balanced: 0\nSkew diameter: 4\nSkew level: 10 20 30 40 50\n=== Single Node Tests ===\nSingle height: 1\nSingle nodes: 1\nSingle leaves: 1\nSingle min: 42\nSingle max: 42\nSingle balanced: 1\nSingle diameter: 0\nSingle isBST: 1
// ============================================================
// test_083_tree_ops.c
// Binary tree stored in array representation:
//   - Root at index 1 (index 0 unused)
//   - Left child of i is at 2*i
//   - Right child of i is at 2*i+1
//   - Parent of i is at i/2
// Operations: insert, delete, search, height, diameter,
//             mirror, level-order values, leaf count.
// ============================================================

// ---- Tree array storage ----
// tree[i] holds value, tree_exists[i] indicates if node exists
int tree[128];
int tree_exists[128];

// ---- Initialize tree (all nodes empty) ----
void tree_init() {
    int i;
    for (i = 0; i < 128; i++) {
        tree[i] = 0;
        tree_exists[i] = 0;
    }
}

// ---- Insert into BST stored in array ----
// Returns 1 on success, 0 if array bounds exceeded
int tree_insert(int val) {
    int idx = 1;
    while (idx < 128) {
        if (!tree_exists[idx]) {
            // Empty slot: place the value here
            tree[idx] = val;
            tree_exists[idx] = 1;
            return 1;
        }
        if (val < tree[idx]) {
            idx = idx * 2; // go left
        } else if (val > tree[idx]) {
            idx = idx * 2 + 1; // go right
        } else {
            // Duplicate value, do nothing
            return 0;
        }
    }
    return 0; // out of bounds
}

// ---- Search for a value ----
// Returns index if found, -1 otherwise
int tree_search(int val) {
    int idx = 1;
    while (idx < 128) {
        if (!tree_exists[idx]) {
            return -1; // not found
        }
        if (val == tree[idx]) {
            return idx;
        } else if (val < tree[idx]) {
            idx = idx * 2;
        } else {
            idx = idx * 2 + 1;
        }
    }
    return -1;
}

// ---- Check if a node has left child ----
int has_left(int idx) {
    int left = idx * 2;
    if (left >= 128) return 0;
    return tree_exists[left];
}

// ---- Check if a node has right child ----
int has_right(int idx) {
    int right = idx * 2 + 1;
    if (right >= 128) return 0;
    return tree_exists[right];
}

// ---- Check if a node is a leaf ----
int is_leaf(int idx) {
    if (!tree_exists[idx]) return 0;
    return !has_left(idx) && !has_right(idx);
}

// ---- Count all nodes in subtree rooted at idx ----
int count_nodes(int idx) {
    if (idx >= 128 || !tree_exists[idx]) return 0;
    return 1 + count_nodes(idx * 2) + count_nodes(idx * 2 + 1);
}

// ---- Count leaf nodes in subtree rooted at idx ----
int count_leaves(int idx) {
    if (idx >= 128 || !tree_exists[idx]) return 0;
    if (is_leaf(idx)) return 1;
    return count_leaves(idx * 2) + count_leaves(idx * 2 + 1);
}

// ---- Height of subtree rooted at idx ----
int tree_height(int idx) {
    if (idx >= 128 || !tree_exists[idx]) return 0;
    int lh = tree_height(idx * 2);
    int rh = tree_height(idx * 2 + 1);
    if (lh > rh) {
        return lh + 1;
    } else {
        return rh + 1;
    }
}

// ---- Diameter of subtree rooted at idx ----
// Diameter = longest path between any two nodes (in edges)
int max_val(int a, int b) {
    if (a > b) return a;
    return b;
}

// Helper: compute height and diameter simultaneously
// Returns height; updates diameter via pointer
void tree_diameter_helper(int idx, int *height, int *diameter) {
    if (idx >= 128 || !tree_exists[idx]) {
        *height = 0;
        *diameter = 0;
        return;
    }
    int lh, ld, rh, rd;
    tree_diameter_helper(idx * 2, &lh, &ld);
    tree_diameter_helper(idx * 2 + 1, &rh, &rd);

    *height = max_val(lh, rh) + 1;
    // Diameter through this node = lh + rh
    // Overall diameter = max of (lh+rh, ld, rd)
    int through = lh + rh;
    *diameter = max_val(through, max_val(ld, rd));
}

int tree_diameter(int idx) {
    int h, d;
    tree_diameter_helper(idx, &h, &d);
    return d;
}

// ---- Mirror the tree (swap left and right children recursively) ----
void tree_mirror(int idx) {
    if (idx >= 128 || !tree_exists[idx]) return;

    // Swap left and right subtrees
    // We need to do this recursively, swapping entire subtrees
    // Since it's array-based, we swap the child nodes and recurse
    int left = idx * 2;
    int right = idx * 2 + 1;

    if (left < 128 && right < 128) {
        // Swap node existence and values
        int tmp_val = tree[left];
        int tmp_ex = tree_exists[left];
        tree[left] = tree[right];
        tree_exists[left] = tree_exists[right];
        tree[right] = tmp_val;
        tree_exists[right] = tmp_ex;

        // We need to swap entire subtrees, not just immediate children
        // For array representation, we need a deep swap
    }

    tree_mirror(left);
    tree_mirror(right);
}

// Deep mirror: swap entire subtrees using a buffer
// This is more complex for array representation
void deep_swap_subtrees(int idx1, int idx2) {
    if (idx1 >= 128 || idx2 >= 128) return;

    // Swap current nodes
    int tmp_val = tree[idx1];
    int tmp_ex = tree_exists[idx1];
    tree[idx1] = tree[idx2];
    tree_exists[idx1] = tree_exists[idx2];
    tree[idx2] = tmp_val;
    tree_exists[idx2] = tmp_ex;

    // Recursively swap children
    deep_swap_subtrees(idx1 * 2, idx2 * 2);
    deep_swap_subtrees(idx1 * 2 + 1, idx2 * 2 + 1);
}

void tree_deep_mirror(int idx) {
    if (idx >= 128 || !tree_exists[idx]) return;
    int left = idx * 2;
    int right = idx * 2 + 1;
    // Swap entire left and right subtrees
    deep_swap_subtrees(left, right);
    // Recurse on children
    tree_deep_mirror(left);
    tree_deep_mirror(right);
}

// ---- Level-order traversal: print values level by level ----
void tree_level_order() {
    // BFS using a queue (array-based)
    int queue[128];
    int q_front = 0;
    int q_rear = 0;

    if (tree_exists[1]) {
        queue[q_rear] = 1;
        q_rear++;
    }

    int first = 1;
    while (q_front < q_rear) {
        int idx = queue[q_front];
        q_front++;

        if (!first) {
            printf(" ");
        }
        printf("%d", tree[idx]);
        first = 0;

        int left = idx * 2;
        int right = idx * 2 + 1;
        if (left < 128 && tree_exists[left]) {
            queue[q_rear] = left;
            q_rear++;
        }
        if (right < 128 && tree_exists[right]) {
            queue[q_rear] = right;
            q_rear++;
        }
    }
    printf("\n");
}

// ---- In-order traversal ----
void tree_inorder(int idx) {
    if (idx >= 128 || !tree_exists[idx]) return;
    tree_inorder(idx * 2);
    printf("%d ", tree[idx]);
    tree_inorder(idx * 2 + 1);
}

// ---- Pre-order traversal ----
void tree_preorder(int idx) {
    if (idx >= 128 || !tree_exists[idx]) return;
    printf("%d ", tree[idx]);
    tree_preorder(idx * 2);
    tree_preorder(idx * 2 + 1);
}

// ---- Post-order traversal ----
void tree_postorder(int idx) {
    if (idx >= 128 || !tree_exists[idx]) return;
    tree_postorder(idx * 2);
    tree_postorder(idx * 2 + 1);
    printf("%d ", tree[idx]);
}

// ---- Find minimum value in BST ----
int tree_find_min(int idx) {
    if (idx >= 128 || !tree_exists[idx]) return -1;
    while (idx * 2 < 128 && tree_exists[idx * 2]) {
        idx = idx * 2;
    }
    return tree[idx];
}

// ---- Find maximum value in BST ----
int tree_find_max(int idx) {
    if (idx >= 128 || !tree_exists[idx]) return -1;
    while (idx * 2 + 1 < 128 && tree_exists[idx * 2 + 1]) {
        idx = idx * 2 + 1;
    }
    return tree[idx];
}

// ---- Sum of all nodes ----
int tree_sum(int idx) {
    if (idx >= 128 || !tree_exists[idx]) return 0;
    return tree[idx] + tree_sum(idx * 2) + tree_sum(idx * 2 + 1);
}

// ---- Delete a node (simplified: only works for leaves and single-child) ----
// Full BST deletion in array representation is complex;
// we implement a simplified version
int tree_delete_leaf(int val) {
    int idx = tree_search(val);
    if (idx == -1) return 0; // not found
    if (!is_leaf(idx)) return 0; // not a leaf
    tree_exists[idx] = 0;
    tree[idx] = 0;
    return 1;
}

// ---- Check if tree is a valid BST ----
int is_bst_helper(int idx, int min_val, int max_val) {
    if (idx >= 128 || !tree_exists[idx]) return 1;
    if (tree[idx] <= min_val || tree[idx] >= max_val) return 0;
    return is_bst_helper(idx * 2, min_val, tree[idx]) &&
           is_bst_helper(idx * 2 + 1, tree[idx], max_val);
}

int is_bst() {
    return is_bst_helper(1, -100000, 100000);
}

// ---- Count nodes at a given depth ----
int count_at_depth(int idx, int current_depth, int target_depth) {
    if (idx >= 128 || !tree_exists[idx]) return 0;
    if (current_depth == target_depth) return 1;
    return count_at_depth(idx * 2, current_depth + 1, target_depth) +
           count_at_depth(idx * 2 + 1, current_depth + 1, target_depth);
}

// ---- Width of tree at each level, return max width ----
int tree_max_width() {
    int max_w = 0;
    int h = tree_height(1);
    int d;
    for (d = 0; d < h; d++) {
        int w = count_at_depth(1, 0, d);
        if (w > max_w) max_w = w;
    }
    return max_w;
}

// ---- Check if tree is balanced ----
int is_balanced(int idx) {
    if (idx >= 128 || !tree_exists[idx]) return 1;
    int lh = tree_height(idx * 2);
    int rh = tree_height(idx * 2 + 1);
    int diff = lh - rh;
    if (diff < 0) diff = -diff;
    if (diff > 1) return 0;
    return is_balanced(idx * 2) && is_balanced(idx * 2 + 1);
}

// ---- Path sum: check if there's a root-to-leaf path with given sum ----
int has_path_sum(int idx, int target) {
    if (idx >= 128 || !tree_exists[idx]) return 0;
    int remaining = target - tree[idx];
    if (is_leaf(idx)) {
        return remaining == 0;
    }
    return has_path_sum(idx * 2, remaining) || has_path_sum(idx * 2 + 1, remaining);
}

// ============================================================
// Test functions
// ============================================================

void test_basic_tree() {
    printf("=== Basic Tree Tests ===\n");
    tree_init();

    // Build a BST by inserting values
    // Insert: 50, 30, 70, 20, 40, 60, 80
    tree_insert(50);
    tree_insert(30);
    tree_insert(70);
    tree_insert(20);
    tree_insert(40);
    tree_insert(60);
    tree_insert(80);

    printf("Nodes: %d\n", count_nodes(1));
    printf("Leaves: %d\n", count_leaves(1));
    printf("Height: %d\n", tree_height(1));
    printf("Min: %d\n", tree_find_min(1));
    printf("Max: %d\n", tree_find_max(1));
    printf("Sum: %d\n", tree_sum(1));
    printf("IsBST: %d\n", is_bst());
    printf("Balanced: %d\n", is_balanced(1));
    printf("Diameter: %d\n", tree_diameter(1));
    printf("MaxWidth: %d\n", tree_max_width());
}

void test_search() {
    printf("=== Search Tests ===\n");
    // Tree should still have values from basic test
    printf("Search 50: %d\n", tree_search(50));
    printf("Search 30: %d\n", tree_search(30));
    printf("Search 70: %d\n", tree_search(70));
    printf("Search 20: %d\n", tree_search(20));
    printf("Search 40: %d\n", tree_search(40));
    printf("Search 99: %d\n", tree_search(99));
    printf("Search 10: %d\n", tree_search(10));
    printf("Search 60: %d\n", tree_search(60));
    printf("Search 80: %d\n", tree_search(80));
}

void test_traversals() {
    printf("=== Traversal Tests ===\n");
    printf("Level: ");
    tree_level_order();
    printf("Inorder: ");
    tree_inorder(1);
    printf("\n");
    printf("Preorder: ");
    tree_preorder(1);
    printf("\n");
    printf("Postorder: ");
    tree_postorder(1);
    printf("\n");
}

void test_delete() {
    printf("=== Delete Tests ===\n");
    // Delete leaf nodes
    int d1 = tree_delete_leaf(20);
    printf("Delete 20 (leaf): %d\n", d1);
    printf("Nodes after: %d\n", count_nodes(1));
    printf("Search 20: %d\n", tree_search(20));

    int d2 = tree_delete_leaf(80);
    printf("Delete 80 (leaf): %d\n", d2);
    printf("Nodes after: %d\n", count_nodes(1));

    // Try to delete non-leaf
    int d3 = tree_delete_leaf(30);
    printf("Delete 30 (non-leaf): %d\n", d3);
    printf("Nodes still: %d\n", count_nodes(1));
}

void test_depth_and_width() {
    printf("=== Depth and Width Tests ===\n");
    tree_init();
    // Build a different tree
    tree_insert(40);
    tree_insert(20);
    tree_insert(60);
    tree_insert(10);
    tree_insert(30);
    tree_insert(50);
    tree_insert(70);
    tree_insert(5);
    tree_insert(15);
    tree_insert(25);
    tree_insert(35);

    printf("Depth0: %d\n", count_at_depth(1, 0, 0));
    printf("Depth1: %d\n", count_at_depth(1, 0, 1));
    printf("Depth2: %d\n", count_at_depth(1, 0, 2));
    printf("Depth3: %d\n", count_at_depth(1, 0, 3));
    printf("Height: %d\n", tree_height(1));
    printf("MaxWidth: %d\n", tree_max_width());
    printf("Nodes: %d\n", count_nodes(1));
    printf("Leaves: %d\n", count_leaves(1));
}

void test_path_sum() {
    printf("=== Path Sum Tests ===\n");
    // Using tree from depth test: 40,20,60,10,30,50,70,5,15,25,35
    // Path: 40->20->10->5 = 75
    printf("PathSum 75: %d\n", has_path_sum(1, 75));
    // Path: 40->20->10->15 = 85
    printf("PathSum 85: %d\n", has_path_sum(1, 85));
    // Path: 40->20->30->25 = 115
    printf("PathSum 115: %d\n", has_path_sum(1, 115));
    // Path: 40->60->70 = 170
    printf("PathSum 170: %d\n", has_path_sum(1, 170));
    // Path: 40->60->50 = 150
    printf("PathSum 150: %d\n", has_path_sum(1, 150));
    // No path sums to 100
    printf("PathSum 100: %d\n", has_path_sum(1, 100));
    // No path sums to 0
    printf("PathSum 0: %d\n", has_path_sum(1, 0));
}

void test_mirror() {
    printf("=== Mirror Tests ===\n");
    tree_init();
    // Build a small tree
    tree_insert(50);
    tree_insert(30);
    tree_insert(70);
    tree_insert(20);
    tree_insert(40);

    printf("Before mirror level: ");
    tree_level_order();
    printf("Before mirror inorder: ");
    tree_inorder(1);
    printf("\n");

    // Mirror the tree
    tree_deep_mirror(1);

    printf("After mirror level: ");
    tree_level_order();

    // After mirroring, it's no longer a valid BST
    printf("IsBST after mirror: %d\n", is_bst());

    // Mirror again to restore
    tree_deep_mirror(1);
    printf("Restored level: ");
    tree_level_order();
    printf("IsBST restored: %d\n", is_bst());
}

void test_skewed_tree() {
    printf("=== Skewed Tree Tests ===\n");
    tree_init();
    // Build a right-skewed tree
    tree_insert(10);
    tree_insert(20);
    tree_insert(30);
    tree_insert(40);
    tree_insert(50);

    printf("Skew height: %d\n", tree_height(1));
    printf("Skew nodes: %d\n", count_nodes(1));
    printf("Skew leaves: %d\n", count_leaves(1));
    printf("Skew balanced: %d\n", is_balanced(1));
    printf("Skew diameter: %d\n", tree_diameter(1));
    printf("Skew level: ");
    tree_level_order();
}

void test_single_node() {
    printf("=== Single Node Tests ===\n");
    tree_init();
    tree_insert(42);

    printf("Single height: %d\n", tree_height(1));
    printf("Single nodes: %d\n", count_nodes(1));
    printf("Single leaves: %d\n", count_leaves(1));
    printf("Single min: %d\n", tree_find_min(1));
    printf("Single max: %d\n", tree_find_max(1));
    printf("Single balanced: %d\n", is_balanced(1));
    printf("Single diameter: %d\n", tree_diameter(1));
    printf("Single isBST: %d\n", is_bst());
}


int main() {
    test_basic_tree();
    test_search();
    test_traversals();
    test_delete();
    test_depth_and_width();
    test_path_sum();
    test_mirror();
    test_skewed_tree();
    test_single_node();
    return 0;
}
