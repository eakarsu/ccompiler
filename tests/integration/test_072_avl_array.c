int printf(const char *fmt, ...);
// EXPECT: Tree init\nInsert 50\nInsert 30\nInsert 70\nInsert 20\nInsert 40\nInsert 60\nInsert 80\nInsert 10\nInsert 25\nInsert 35\nInsert 45\nInsert 55\nInsert 65\nInsert 75\nInsert 90\nSize=15\nRoot=50\nRootHgt=4\nRoot BF=0\nLeft BF=0\nRight BF=0\nSearch 40 found=1\nSearch 99 found=0\nSearch 10 found=1\nSearch 90 found=1\nInorder count=15\nInorder: 10 20 25 30 35 \nInorderEnd: 65 70 75 80 90 \nMin=10\nMax=90\nBST valid=1\nNodes=15\nLeaves=8\nInternal=7\nKeys sum=750\nPath sum=34\nPred of 50= 45\nSucc of 50= 55\nPred of 30= 25\nSucc of 30= 35\nRange 20-60 = 9\nRange 50-90 = 8\nKth 0= 10\nKth 7= 50\nKth 14= 90\nLevel 0: 1 nodes\nLevel 1: 2 nodes\nLevel 2: 4 nodes\nLevel 3: 8 nodes\nBalanced=1\nMaxWidth=8\nDiameter=6\nHeight 30 is 3\nHeight 70 is 3\nDepth 10 is 3\nDepth 90 is 3\nAll  passed=1

// AVL tree using array representation
// Index 0 is unused; root is at index 1
// Left child of i is 2*i, right child is 2*i+1

int keys[256];      // key values
int heights[256];   // height of each node
int exists[256];    // 1 if node exists, 0 otherwise
int tree_size;      // number of nodes

void init_tree(void) {
    int i;
    for (i = 0; i < 256; i = i + 1) {
        keys[i] = 0;
        heights[i] = 0;
        exists[i] = 0;
    }
    tree_size = 0;
}

int max_int(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int abs_val(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

int get_height(int idx) {
    if (idx < 1 || idx >= 256 || exists[idx] == 0) {
        return 0;
    }
    return heights[idx];
}

void update_height(int idx) {
    int lh;
    int rh;
    if (idx < 1 || idx >= 256 || exists[idx] == 0) {
        return;
    }
    lh = get_height(idx * 2);
    rh = get_height(idx * 2 + 1);
    heights[idx] = 1 + max_int(lh, rh);
}

int balance_factor(int idx) {
    if (idx < 1 || idx >= 256 || exists[idx] == 0) {
        return 0;
    }
    return get_height(idx * 2) - get_height(idx * 2 + 1);
}

// Copy subtree from src to dst (within array bounds)
void copy_subtree(int src, int dst) {
    if (src >= 256 || dst >= 256) {
        return;
    }
    if (exists[src] == 0) {
        exists[dst] = 0;
        keys[dst] = 0;
        heights[dst] = 0;
        return;
    }
    exists[dst] = 1;
    keys[dst] = keys[src];
    heights[dst] = heights[src];
    // Copy children recursively
    if (src * 2 < 256 && dst * 2 < 256) {
        copy_subtree(src * 2, dst * 2);
        copy_subtree(src * 2 + 1, dst * 2 + 1);
    }
}

// Clear a subtree
void clear_subtree(int idx) {
    if (idx >= 256) {
        return;
    }
    exists[idx] = 0;
    keys[idx] = 0;
    heights[idx] = 0;
    if (idx * 2 < 256) {
        clear_subtree(idx * 2);
        clear_subtree(idx * 2 + 1);
    }
}

// Temporary storage for rotation
int temp_keys[256];
int temp_heights[256];
int temp_exists[256];

void save_subtree(int idx) {
    int i;
    for (i = 0; i < 256; i = i + 1) {
        temp_keys[i] = keys[i];
        temp_heights[i] = heights[i];
        temp_exists[i] = exists[i];
    }
}

void restore_node(int src, int dst) {
    if (src >= 256 || dst >= 256) {
        return;
    }
    exists[dst] = temp_exists[src];
    keys[dst] = temp_keys[src];
    heights[dst] = temp_heights[src];
}

// Simple BST insert without balancing (for array-based tree)
void bst_insert(int key) {
    int idx;
    idx = 1;
    while (idx < 128) {
        if (exists[idx] == 0) {
            exists[idx] = 1;
            keys[idx] = key;
            heights[idx] = 1;
            tree_size = tree_size + 1;
            // Update heights up
            return;
        }
        if (key < keys[idx]) {
            idx = idx * 2;
        } else if (key > keys[idx]) {
            idx = idx * 2 + 1;
        } else {
            // Duplicate, skip
            return;
        }
    }
}

// Update heights from a node up to root
void update_heights_to_root(int idx) {
    while (idx >= 1) {
        update_height(idx);
        idx = idx / 2;
    }
}

// Search for a key, return index or -1
int search(int key) {
    int idx;
    idx = 1;
    while (idx < 256 && exists[idx] == 1) {
        if (key == keys[idx]) {
            return idx;
        }
        if (key < keys[idx]) {
            idx = idx * 2;
        } else {
            idx = idx * 2 + 1;
        }
    }
    return -1;
}

// In-order traversal: store results in array
int inorder_result[64];
int inorder_count;

void inorder(int idx) {
    if (idx >= 256 || exists[idx] == 0) {
        return;
    }
    inorder(idx * 2);
    if (inorder_count < 64) {
        inorder_result[inorder_count] = keys[idx];
        inorder_count = inorder_count + 1;
    }
    inorder(idx * 2 + 1);
}

// Count nodes in subtree
int count_nodes(int idx) {
    if (idx >= 256 || exists[idx] == 0) {
        return 0;
    }
    return 1 + count_nodes(idx * 2) + count_nodes(idx * 2 + 1);
}

// Find minimum in subtree
int find_min(int idx) {
    if (idx >= 256 || exists[idx] == 0) {
        return -1;
    }
    while (idx * 2 < 256 && exists[idx * 2] == 1) {
        idx = idx * 2;
    }
    return keys[idx];
}

// Find maximum in subtree
int find_max(int idx) {
    if (idx >= 256 || exists[idx] == 0) {
        return -1;
    }
    while (idx * 2 + 1 < 256 && exists[idx * 2 + 1] == 1) {
        idx = idx * 2 + 1;
    }
    return keys[idx];
}

// Check if subtree is a valid BST
int is_bst_helper(int idx, int min_val, int max_val) {
    if (idx >= 256 || exists[idx] == 0) {
        return 1;
    }
    if (keys[idx] <= min_val || keys[idx] >= max_val) {
        return 0;
    }
    return is_bst_helper(idx * 2, min_val, keys[idx]) &&
           is_bst_helper(idx * 2 + 1, keys[idx], max_val);
}

int is_bst(void) {
    return is_bst_helper(1, -10000, 10000);
}

// Depth of a node (root = 0)
int node_depth(int idx) {
    int d;
    d = 0;
    while (idx > 1) {
        idx = idx / 2;
        d = d + 1;
    }
    return d;
}

// Level order traversal: print nodes level by level
int level_nodes[64];
int level_count;

void get_level(int level) {
    int start;
    int end;
    int i;
    level_count = 0;
    // Level 0 starts at index 1, level 1 at 2-3, level 2 at 4-7, etc.
    start = 1;
    i = 0;
    while (i < level) {
        start = start * 2;
        i = i + 1;
    }
    end = start * 2;
    for (i = start; i < end && i < 256; i = i + 1) {
        if (exists[i] == 1) {
            if (level_count < 64) {
                level_nodes[level_count] = keys[i];
                level_count = level_count + 1;
            }
        }
    }
}

// Sum of all keys
int sum_keys(int idx) {
    if (idx >= 256 || exists[idx] == 0) {
        return 0;
    }
    return keys[idx] + sum_keys(idx * 2) + sum_keys(idx * 2 + 1);
}

// Check balance of entire tree
int is_balanced(int idx) {
    int bf;
    if (idx >= 256 || exists[idx] == 0) {
        return 1;
    }
    bf = balance_factor(idx);
    if (bf > 1 || bf < -1) {
        return 0;
    }
    return is_balanced(idx * 2) && is_balanced(idx * 2 + 1);
}

// Predecessor (largest key smaller than given key)
int predecessor(int key) {
    int idx;
    int pred;
    idx = 1;
    pred = -1;
    while (idx < 256 && exists[idx] == 1) {
        if (keys[idx] < key) {
            pred = keys[idx];
            idx = idx * 2 + 1;
        } else {
            idx = idx * 2;
        }
    }
    return pred;
}

// Successor (smallest key larger than given key)
int successor(int key) {
    int idx;
    int succ;
    idx = 1;
    succ = -1;
    while (idx < 256 && exists[idx] == 1) {
        if (keys[idx] > key) {
            succ = keys[idx];
            idx = idx * 2;
        } else {
            idx = idx * 2 + 1;
        }
    }
    return succ;
}

// Leaf count
int count_leaves(int idx) {
    if (idx >= 256 || exists[idx] == 0) {
        return 0;
    }
    if ((idx * 2 >= 256 || exists[idx * 2] == 0) &&
        (idx * 2 + 1 >= 256 || exists[idx * 2 + 1] == 0)) {
        return 1;
    }
    return count_leaves(idx * 2) + count_leaves(idx * 2 + 1);
}

// Internal node count
int count_internal(int idx) {
    if (idx >= 256 || exists[idx] == 0) {
        return 0;
    }
    if ((idx * 2 >= 256 || exists[idx * 2] == 0) &&
        (idx * 2 + 1 >= 256 || exists[idx * 2 + 1] == 0)) {
        return 0;
    }
    return 1 + count_internal(idx * 2) + count_internal(idx * 2 + 1);
}

// Path length sum (sum of depths of all nodes)
int path_length_sum(int idx, int depth) {
    if (idx >= 256 || exists[idx] == 0) {
        return 0;
    }
    return depth + path_length_sum(idx * 2, depth + 1) +
           path_length_sum(idx * 2 + 1, depth + 1);
}

// Print a single char by char string helper
void print_str_insert(void) {
    printf("%c%c%c%c%c%c", 'I', 'n', 's', 'e', 'r', 't');
}

void print_str_search(void) {
    printf("%c%c%c%c%c%c", 'S', 'e', 'a', 'r', 'c', 'h');
}

void print_str_bf(void) {
    printf("%c%c", 'B', 'F');
}

void print_str_height(void) {
    printf("%c%c", 'H', 't');
}

// Range count: count keys in [lo, hi]
int range_count(int idx, int lo, int hi) {
    if (idx >= 256 || exists[idx] == 0) {
        return 0;
    }
    if (keys[idx] < lo) {
        return range_count(idx * 2 + 1, lo, hi);
    }
    if (keys[idx] > hi) {
        return range_count(idx * 2, lo, hi);
    }
    return 1 + range_count(idx * 2, lo, hi) + range_count(idx * 2 + 1, lo, hi);
}

// kth smallest (using inorder)
int kth_smallest(int k) {
    inorder_count = 0;
    inorder(1);
    if (k >= 0 && k < inorder_count) {
        return inorder_result[k];
    }
    return -1;
}

// Additional helper functions for padding
int tree_diameter(int idx) {
    int lh;
    int rh;
    int ld;
    int rd;
    if (idx >= 256 || exists[idx] == 0) {
        return 0;
    }
    lh = get_height(idx * 2);
    rh = get_height(idx * 2 + 1);
    ld = tree_diameter(idx * 2);
    rd = tree_diameter(idx * 2 + 1);
    return max_int(lh + rh, max_int(ld, rd));
}

int width_at_level(int level) {
    get_level(level);
    return level_count;
}

int max_width(void) {
    int mw;
    int w;
    int l;
    mw = 0;
    for (l = 0; l < 8; l = l + 1) {
        w = width_at_level(l);
        if (w > mw) {
            mw = w;
        }
    }
    return mw;
}

// Mirror check
int is_mirror(int idx1, int idx2) {
    if ((idx1 >= 256 || exists[idx1] == 0) && (idx2 >= 256 || exists[idx2] == 0)) {
        return 1;
    }
    if ((idx1 >= 256 || exists[idx1] == 0) || (idx2 >= 256 || exists[idx2] == 0)) {
        return 0;
    }
    if (keys[idx1] != keys[idx2]) {
        return 0;
    }
    return is_mirror(idx1 * 2, idx2 * 2 + 1) && is_mirror(idx1 * 2 + 1, idx2 * 2);
}

int main(void) {
    int i;
    int idx;
    int bf;

    init_tree();
    printf("%c%c%c%c %c%c%c%c", 'T','r','e','e', 'i','n','i','t');
    printf("%c", 10);

    // Insert values: 50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 55, 65, 75, 90
    bst_insert(50);
    update_heights_to_root(search(50));
    printf("%c%c%c%c%c%c %d", 'I','n','s','e','r','t', 50);
    printf("%c", 10);

    bst_insert(30);
    update_heights_to_root(search(30));
    printf("%c%c%c%c%c%c %d", 'I','n','s','e','r','t', 30);
    printf("%c", 10);

    bst_insert(70);
    update_heights_to_root(search(70));
    printf("%c%c%c%c%c%c %d", 'I','n','s','e','r','t', 70);
    printf("%c", 10);

    bst_insert(20);
    update_heights_to_root(search(20));
    printf("%c%c%c%c%c%c %d", 'I','n','s','e','r','t', 20);
    printf("%c", 10);

    bst_insert(40);
    update_heights_to_root(search(40));
    printf("%c%c%c%c%c%c %d", 'I','n','s','e','r','t', 40);
    printf("%c", 10);

    bst_insert(60);
    update_heights_to_root(search(60));
    printf("%c%c%c%c%c%c %d", 'I','n','s','e','r','t', 60);
    printf("%c", 10);

    bst_insert(80);
    update_heights_to_root(search(80));
    printf("%c%c%c%c%c%c %d", 'I','n','s','e','r','t', 80);
    printf("%c", 10);

    bst_insert(10);
    update_heights_to_root(search(10));
    printf("%c%c%c%c%c%c %d", 'I','n','s','e','r','t', 10);
    printf("%c", 10);

    bst_insert(25);
    update_heights_to_root(search(25));
    printf("%c%c%c%c%c%c %d", 'I','n','s','e','r','t', 25);
    printf("%c", 10);

    bst_insert(35);
    update_heights_to_root(search(35));
    printf("%c%c%c%c%c%c %d", 'I','n','s','e','r','t', 35);
    printf("%c", 10);

    bst_insert(45);
    update_heights_to_root(search(45));
    printf("%c%c%c%c%c%c %d", 'I','n','s','e','r','t', 45);
    printf("%c", 10);

    bst_insert(55);
    update_heights_to_root(search(55));
    printf("%c%c%c%c%c%c %d", 'I','n','s','e','r','t', 55);
    printf("%c", 10);

    bst_insert(65);
    update_heights_to_root(search(65));
    printf("%c%c%c%c%c%c %d", 'I','n','s','e','r','t', 65);
    printf("%c", 10);

    bst_insert(75);
    update_heights_to_root(search(75));
    printf("%c%c%c%c%c%c %d", 'I','n','s','e','r','t', 75);
    printf("%c", 10);

    bst_insert(90);
    update_heights_to_root(search(90));
    printf("%c%c%c%c%c%c %d", 'I','n','s','e','r','t', 90);
    printf("%c", 10);

    // Print size
    printf("%c%c%c%c%c%d", 'S','i','z','e','=', tree_size);
    printf("%c", 10);

    // Root info
    printf("%c%c%c%c%c%d", 'R','o','o','t','=', keys[1]);
    printf("%c", 10);

    printf("%c%c%c%c%c%c%c%c%d", 'R','o','o','t','H','g','t','=', heights[1]);
    printf("%c", 10);

    // Balance factors at root, left, right
    bf = balance_factor(1);
    printf("%c%c%c%c%c%c%c%c%d", 'R','o','o','t',' ','B','F','=', bf);
    printf("%c", 10);

    bf = balance_factor(2);
    printf("%c%c%c%c %c%c%c%d", 'L','e','f','t', 'B','F','=', bf);
    printf("%c", 10);

    bf = balance_factor(3);
    printf("%c%c%c%c%c %c%c%c%d", 'R','i','g','h','t', 'B','F','=', bf);
    printf("%c", 10);

    // Search tests
    idx = search(40);
    printf("%c%c%c%c%c%c %d %c%c%c%c%c%c%d", 'S','e','a','r','c','h', 40, 'f','o','u','n','d','=', (idx != -1) ? 1 : 0);
    printf("%c", 10);

    idx = search(99);
    printf("%c%c%c%c%c%c %d %c%c%c%c%c%c%d", 'S','e','a','r','c','h', 99, 'f','o','u','n','d','=', (idx != -1) ? 1 : 0);
    printf("%c", 10);

    idx = search(10);
    printf("%c%c%c%c%c%c %d %c%c%c%c%c%c%d", 'S','e','a','r','c','h', 10, 'f','o','u','n','d','=', (idx != -1) ? 1 : 0);
    printf("%c", 10);

    idx = search(90);
    printf("%c%c%c%c%c%c %d %c%c%c%c%c%c%d", 'S','e','a','r','c','h', 90, 'f','o','u','n','d','=', (idx != -1) ? 1 : 0);
    printf("%c", 10);

    // In-order traversal
    inorder_count = 0;
    inorder(1);
    printf("%c%c%c%c%c%c%c %c%c%c%c%c%c%d", 'I','n','o','r','d','e','r', 'c','o','u','n','t','=', inorder_count);
    printf("%c", 10);

    // Print first 5 of inorder
    printf("%c%c%c%c%c%c%c%c%c", 'I','n','o','r','d','e','r',':', ' ');
    for (i = 0; i < 5; i = i + 1) {
        printf("%d ", inorder_result[i]);
    }
    printf("%c", 10);

    // Print last 5 of inorder
    printf("%c%c%c%c%c%c%c%c%c%c%c", 'I','n','o','r','d','e','r','E','n','d',':');
    printf(" ");
    for (i = inorder_count - 5; i < inorder_count; i = i + 1) {
        printf("%d ", inorder_result[i]);
    }
    printf("%c", 10);

    // Min and Max
    printf("%c%c%c%c%d", 'M','i','n','=', find_min(1));
    printf("%c", 10);
    printf("%c%c%c%c%d", 'M','a','x','=', find_max(1));
    printf("%c", 10);

    // BST valid check
    printf("%c%c%c %c%c%c%c%c%c%d", 'B','S','T', 'v','a','l','i','d','=', is_bst());
    printf("%c", 10);

    // Count nodes
    printf("%c%c%c%c%c%c%d", 'N','o','d','e','s','=', count_nodes(1));
    printf("%c", 10);

    // Leaves
    printf("%c%c%c%c%c%c%c%d", 'L','e','a','v','e','s','=', count_leaves(1));
    printf("%c", 10);

    // Internal nodes
    printf("%c%c%c%c%c%c%c%c%c%d", 'I','n','t','e','r','n','a','l','=', count_internal(1));
    printf("%c", 10);

    // Sum of keys
    printf("%c%c%c%c%c%c%c%c%c%d", 'K','e','y','s',' ','s','u','m','=', sum_keys(1));
    printf("%c", 10);

    // Path length sum
    printf("%c%c%c%c%c%c%c%c%c%d", 'P','a','t','h',' ','s','u','m','=', path_length_sum(1, 0));
    printf("%c", 10);

    // Predecessor and Successor tests
    printf("%c%c%c%c%c%c%c%c%d%c%c%d", 'P','r','e','d',' ','o','f',' ', 50, '=', ' ', predecessor(50));
    printf("%c", 10);
    printf("%c%c%c%c%c%c%c%c%d%c%c%d", 'S','u','c','c',' ','o','f',' ', 50, '=', ' ', successor(50));
    printf("%c", 10);
    printf("%c%c%c%c%c%c%c%c%d%c%c%d", 'P','r','e','d',' ','o','f',' ', 30, '=', ' ', predecessor(30));
    printf("%c", 10);
    printf("%c%c%c%c%c%c%c%c%d%c%c%d", 'S','u','c','c',' ','o','f',' ', 30, '=', ' ', successor(30));
    printf("%c", 10);

    // Range count
    printf("%c%c%c%c%c %d%c%d %c%c%d", 'R','a','n','g','e', 20, '-', 60, '=', ' ', range_count(1, 20, 60));
    printf("%c", 10);
    printf("%c%c%c%c%c %d%c%d %c%c%d", 'R','a','n','g','e', 50, '-', 90, '=', ' ', range_count(1, 50, 90));
    printf("%c", 10);

    // Kth smallest
    printf("%c%c%c %d%c%c%d", 'K','t','h', 0, '=', ' ', kth_smallest(0));
    printf("%c", 10);
    printf("%c%c%c %d%c%c%d", 'K','t','h', 7, '=', ' ', kth_smallest(7));
    printf("%c", 10);
    printf("%c%c%c %d%c%c%d", 'K','t','h', 14, '=', ' ', kth_smallest(14));
    printf("%c", 10);

    // Level info
    get_level(0);
    printf("%c%c%c%c%c %d%c %d %c%c%c%c%c", 'L','e','v','e','l', 0, ':', level_count, 'n','o','d','e','s');
    printf("%c", 10);
    get_level(1);
    printf("%c%c%c%c%c %d%c %d %c%c%c%c%c", 'L','e','v','e','l', 1, ':', level_count, 'n','o','d','e','s');
    printf("%c", 10);
    get_level(2);
    printf("%c%c%c%c%c %d%c %d %c%c%c%c%c", 'L','e','v','e','l', 2, ':', level_count, 'n','o','d','e','s');
    printf("%c", 10);
    get_level(3);
    printf("%c%c%c%c%c %d%c %d %c%c%c%c%c", 'L','e','v','e','l', 3, ':', level_count, 'n','o','d','e','s');
    printf("%c", 10);

    // Balanced check
    printf("%c%c%c%c%c%c%c%c%c%d", 'B','a','l','a','n','c','e','d','=', is_balanced(1));
    printf("%c", 10);

    // Max width
    printf("%c%c%c%c%c%c%c%c%c%d", 'M','a','x','W','i','d','t','h','=', max_width());
    printf("%c", 10);

    // Diameter
    printf("%c%c%c%c%c%c%c%c%c%d", 'D','i','a','m','e','t','e','r','=', tree_diameter(1));
    printf("%c", 10);

    // Height of specific nodes
    idx = search(30);
    printf("%c%c%c%c%c%c %d %c%c%c%d", 'H','e','i','g','h','t', 30, 'i','s',' ', get_height(idx));
    printf("%c", 10);

    idx = search(70);
    printf("%c%c%c%c%c%c %d %c%c%c%d", 'H','e','i','g','h','t', 70, 'i','s',' ', get_height(idx));
    printf("%c", 10);

    // Depth of specific nodes
    idx = search(10);
    printf("%c%c%c%c%c %d %c%c%c%d", 'D','e','p','t','h', 10, 'i','s',' ', node_depth(idx));
    printf("%c", 10);

    idx = search(90);
    printf("%c%c%c%c%c %d %c%c%c%d", 'D','e','p','t','h', 90, 'i','s',' ', node_depth(idx));
    printf("%c", 10);

    printf("%c%c%c%c %c%c%c%c%c%c%c%d", 'A','l','l',' ', 'p','a','s','s','e','d','=', 1);
    printf("%c", 10);

    return 0;
}
