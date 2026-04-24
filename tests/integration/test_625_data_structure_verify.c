int printf(const char *fmt, ...);
// EXPECT: Inorder: 10 20 30 35 40 45 50 55 60 65 70 80 \nSearch 40: 1\nSearch 42: 0\nSearch 10: 1\nSearch 80: 1\nSearch 99: 0\nHeight: 4\nCount: 12\nInsert 5, 15, 25\nInorder: 5 10 15 20 25 30 35 40 45 50 55 60 65 70 80 \nCount: 15\nHeight: 5\nSorted: 1\nMin: 5\nMax: 80\nPreorder: 50 30 20 10 5 15 25 40 35 45 70 60 55 65 80 \nSum: 605\n--- Degenerate tree ---\nInorder: 1 2 3 4 5 \nPreorder: 1 2 3 4 5 \nHeight: 5\nCount: 5\nMin: 1\nMax: 5\nSum: 15\nSearch 3: 1\nSearch 6: 0\nSorted: 1\n--- Stress insert ---\nCount: 20\nHeight: 5\nMin: 2\nMax: 98\nSorted: 1\nDone

struct BSTNode {
    int key;
    int left;
    int right;
    int used;
};

struct BSTNode tree[64];
int node_count;

int new_node(int key) {
    int idx = node_count;
    node_count = node_count + 1;
    tree[idx].key = key;
    tree[idx].left = -1;
    tree[idx].right = -1;
    tree[idx].used = 1;
    return idx;
}

int insert(int root, int key) {
    if (root == -1) {
        return new_node(key);
    }
    if (key < tree[root].key) {
        tree[root].left = insert(tree[root].left, key);
    } else if (key > tree[root].key) {
        tree[root].right = insert(tree[root].right, key);
    }
    return root;
}

void inorder(int root) {
    if (root == -1) {
        return;
    }
    inorder(tree[root].left);
    printf("%d ", tree[root].key);
    inorder(tree[root].right);
}

void preorder(int root) {
    if (root == -1) {
        return;
    }
    printf("%d ", tree[root].key);
    preorder(tree[root].left);
    preorder(tree[root].right);
}

int search(int root, int key) {
    if (root == -1) {
        return 0;
    }
    if (key == tree[root].key) {
        return 1;
    }
    if (key < tree[root].key) {
        return search(tree[root].left, key);
    }
    return search(tree[root].right, key);
}

int height(int root) {
    int lh;
    int rh;
    if (root == -1) {
        return 0;
    }
    lh = height(tree[root].left);
    rh = height(tree[root].right);
    if (lh > rh) {
        return 1 + lh;
    }
    return 1 + rh;
}

int count_nodes(int root) {
    if (root == -1) {
        return 0;
    }
    return 1 + count_nodes(tree[root].left) + count_nodes(tree[root].right);
}

int find_min(int root) {
    if (root == -1) {
        return -1;
    }
    if (tree[root].left == -1) {
        return tree[root].key;
    }
    return find_min(tree[root].left);
}

int find_max(int root) {
    if (root == -1) {
        return -1;
    }
    if (tree[root].right == -1) {
        return tree[root].key;
    }
    return find_max(tree[root].right);
}

int sum_keys(int root) {
    if (root == -1) {
        return 0;
    }
    return tree[root].key + sum_keys(tree[root].left) + sum_keys(tree[root].right);
}

// Collect keys into an array via in-order traversal
// Returns the number of keys collected starting from position pos
int collect_inorder(int root, int *arr, int pos) {
    int p;
    if (root == -1) {
        return pos;
    }
    p = collect_inorder(tree[root].left, arr, pos);
    arr[p] = tree[root].key;
    p = p + 1;
    p = collect_inorder(tree[root].right, arr, p);
    return p;
}

// Check if an array is sorted in strict ascending order
int is_sorted(int *arr, int n) {
    int i;
    for (i = 0; i < n - 1; i = i + 1) {
        if (arr[i] >= arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int root;
    int root2;
    int root3;
    int h;
    int cnt;
    int sorted_arr[64];
    int n;
    int sorted;

    node_count = 0;
    root = -1;

    // ---- First tree: balanced BST ----

    // Insert 12 keys
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);
    root = insert(root, 10);
    root = insert(root, 35);
    root = insert(root, 45);
    root = insert(root, 55);
    root = insert(root, 65);

    // In-order traversal (should print sorted)
    printf("Inorder: ");
    inorder(root);
    printf("\n");

    // Search tests
    printf("Search 40: %d\n", search(root, 40));
    printf("Search 42: %d\n", search(root, 42));
    printf("Search 10: %d\n", search(root, 10));
    printf("Search 80: %d\n", search(root, 80));
    printf("Search 99: %d\n", search(root, 99));

    // Height
    h = height(root);
    printf("Height: %d\n", h);

    // Count nodes
    cnt = count_nodes(root);
    printf("Count: %d\n", cnt);

    // Insert 3 more keys
    printf("Insert 5, 15, 25\n");
    root = insert(root, 5);
    root = insert(root, 15);
    root = insert(root, 25);

    // In-order traversal again
    printf("Inorder: ");
    inorder(root);
    printf("\n");

    // Count again
    cnt = count_nodes(root);
    printf("Count: %d\n", cnt);

    // Height again
    h = height(root);
    printf("Height: %d\n", h);

    // Verify sorted property by collecting keys and checking
    n = collect_inorder(root, sorted_arr, 0);
    sorted = is_sorted(sorted_arr, n);
    printf("Sorted: %d\n", sorted);

    // Min and max
    printf("Min: %d\n", find_min(root));
    printf("Max: %d\n", find_max(root));

    // Preorder traversal
    printf("Preorder: ");
    preorder(root);
    printf("\n");

    // Sum of all keys
    printf("Sum: %d\n", sum_keys(root));

    // ---- Second tree: degenerate (right-skewed) ----

    printf("--- Degenerate tree ---\n");
    root2 = -1;
    root2 = insert(root2, 1);
    root2 = insert(root2, 2);
    root2 = insert(root2, 3);
    root2 = insert(root2, 4);
    root2 = insert(root2, 5);

    printf("Inorder: ");
    inorder(root2);
    printf("\n");

    printf("Preorder: ");
    preorder(root2);
    printf("\n");

    printf("Height: %d\n", height(root2));
    printf("Count: %d\n", count_nodes(root2));
    printf("Min: %d\n", find_min(root2));
    printf("Max: %d\n", find_max(root2));
    printf("Sum: %d\n", sum_keys(root2));
    printf("Search 3: %d\n", search(root2, 3));
    printf("Search 6: %d\n", search(root2, 6));

    n = collect_inorder(root2, sorted_arr, 0);
    sorted = is_sorted(sorted_arr, n);
    printf("Sorted: %d\n", sorted);

    // ---- Third tree: stress insert with various values ----

    printf("--- Stress insert ---\n");
    root3 = -1;
    root3 = insert(root3, 50);
    root3 = insert(root3, 25);
    root3 = insert(root3, 75);
    root3 = insert(root3, 12);
    root3 = insert(root3, 37);
    root3 = insert(root3, 62);
    root3 = insert(root3, 87);
    root3 = insert(root3, 6);
    root3 = insert(root3, 18);
    root3 = insert(root3, 31);
    root3 = insert(root3, 43);
    root3 = insert(root3, 56);
    root3 = insert(root3, 68);
    root3 = insert(root3, 81);
    root3 = insert(root3, 93);
    root3 = insert(root3, 2);
    root3 = insert(root3, 9);
    root3 = insert(root3, 15);
    root3 = insert(root3, 21);
    root3 = insert(root3, 98);

    printf("Count: %d\n", count_nodes(root3));
    printf("Height: %d\n", height(root3));
    printf("Min: %d\n", find_min(root3));
    printf("Max: %d\n", find_max(root3));

    n = collect_inorder(root3, sorted_arr, 0);
    sorted = is_sorted(sorted_arr, n);
    printf("Sorted: %d\n", sorted);

    printf("Done\n");

    return 0;
}
