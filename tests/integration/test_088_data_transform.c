int printf(const char *fmt, ...);
// EXPECT: 4 3 7 1 1 4 7 1 1 7 28 1 4 2 6 3 5 1 0 0 0 1 1 1 1 1 1 4 5 1 1 3 0 2 6 1 1 6 1 6 1 4 15 9 4 2 3 8 4 5

// Data transformation: array to balanced BST (array-based), flatten nested structures,
// data rearrangement algorithms

// Array-based BST
int bst_val[100];
int bst_left[100];
int bst_right[100];
int bst_count;

void bst_init(void) {
    int i;
    bst_count = 0;
    for (i = 0; i < 100; i = i + 1) {
        bst_val[i] = 0;
        bst_left[i] = -1;
        bst_right[i] = -1;
    }
}

int bst_new_node(int v) {
    int idx;
    idx = bst_count;
    bst_val[idx] = v;
    bst_left[idx] = -1;
    bst_right[idx] = -1;
    bst_count = bst_count + 1;
    return idx;
}

// Build balanced BST from sorted array
int build_balanced_bst(int arr[], int start, int end) {
    int mid;
    int node;
    if (start > end) return -1;
    mid = (start + end) / 2;
    node = bst_new_node(arr[mid]);
    bst_left[node] = build_balanced_bst(arr, start, mid - 1);
    bst_right[node] = build_balanced_bst(arr, mid + 1, end);
    return node;
}

// BST insert
int bst_insert(int root, int v) {
    int node;
    int cur;
    node = bst_new_node(v);
    if (root == -1) return node;
    cur = root;
    while (1) {
        if (v < bst_val[cur]) {
            if (bst_left[cur] == -1) {
                bst_left[cur] = node;
                return root;
            }
            cur = bst_left[cur];
        } else {
            if (bst_right[cur] == -1) {
                bst_right[cur] = node;
                return root;
            }
            cur = bst_right[cur];
        }
    }
    return root;
}

// BST height
int bst_height(int root) {
    int lh;
    int rh;
    if (root == -1) return 0;
    lh = bst_height(bst_left[root]);
    rh = bst_height(bst_right[root]);
    if (lh > rh) return lh + 1;
    return rh + 1;
}

// BST node count
int bst_node_count(int root) {
    if (root == -1) return 0;
    return 1 + bst_node_count(bst_left[root]) + bst_node_count(bst_right[root]);
}

// BST inorder to array
int inorder_arr[100];
int inorder_idx;

void bst_inorder(int root) {
    if (root == -1) return;
    bst_inorder(bst_left[root]);
    inorder_arr[inorder_idx] = bst_val[root];
    inorder_idx = inorder_idx + 1;
    bst_inorder(bst_right[root]);
}

// BST search
int bst_search(int root, int v) {
    if (root == -1) return 0;
    if (bst_val[root] == v) return 1;
    if (v < bst_val[root]) return bst_search(bst_left[root], v);
    return bst_search(bst_right[root], v);
}

// BST min
int bst_min(int root) {
    while (bst_left[root] != -1) {
        root = bst_left[root];
    }
    return bst_val[root];
}

// BST max
int bst_max(int root) {
    while (bst_right[root] != -1) {
        root = bst_right[root];
    }
    return bst_val[root];
}

// BST sum
int bst_sum(int root) {
    if (root == -1) return 0;
    return bst_val[root] + bst_sum(bst_left[root]) + bst_sum(bst_right[root]);
}

// Check if BST is valid
int bst_is_valid_helper(int root, int min_val, int max_val) {
    if (root == -1) return 1;
    if (bst_val[root] <= min_val || bst_val[root] >= max_val) return 0;
    return bst_is_valid_helper(bst_left[root], min_val, bst_val[root]) &&
           bst_is_valid_helper(bst_right[root], bst_val[root], max_val);
}

int bst_is_valid(int root) {
    return bst_is_valid_helper(root, -99999, 99999);
}

// BST is balanced check
int bst_is_balanced(int root) {
    int lh;
    int rh;
    int diff;
    if (root == -1) return 1;
    lh = bst_height(bst_left[root]);
    rh = bst_height(bst_right[root]);
    diff = lh - rh;
    if (diff < 0) diff = -diff;
    if (diff > 1) return 0;
    return bst_is_balanced(bst_left[root]) && bst_is_balanced(bst_right[root]);
}

// Level order traversal using queue
int level_order[100];

int bst_level_order(int root) {
    int queue2[100];
    int front;
    int back;
    int idx;
    int cur;
    if (root == -1) return 0;
    front = 0;
    back = 0;
    queue2[back] = root;
    back = back + 1;
    idx = 0;
    while (front < back) {
        cur = queue2[front];
        front = front + 1;
        level_order[idx] = bst_val[cur];
        idx = idx + 1;
        if (bst_left[cur] != -1) {
            queue2[back] = bst_left[cur];
            back = back + 1;
        }
        if (bst_right[cur] != -1) {
            queue2[back] = bst_right[cur];
            back = back + 1;
        }
    }
    return idx;
}

// Array rearrangement: Dutch National Flag (3-way partition)
void dutch_flag(int arr[], int n) {
    int low;
    int mid;
    int high;
    int t;
    low = 0;
    mid = 0;
    high = n - 1;
    while (mid <= high) {
        if (arr[mid] == 0) {
            t = arr[low]; arr[low] = arr[mid]; arr[mid] = t;
            low = low + 1;
            mid = mid + 1;
        } else if (arr[mid] == 1) {
            mid = mid + 1;
        } else {
            t = arr[mid]; arr[mid] = arr[high]; arr[high] = t;
            high = high - 1;
        }
    }
}

// Rearrange array: even indices get smaller, odd get larger
// Zigzag rearrangement: a[0]<a[1]>a[2]<a[3]>...
void zigzag(int arr[], int n) {
    int i;
    int t;
    for (i = 0; i < n - 1; i = i + 1) {
        if (i % 2 == 0) {
            if (arr[i] > arr[i + 1]) {
                t = arr[i]; arr[i] = arr[i + 1]; arr[i + 1] = t;
            }
        } else {
            if (arr[i] < arr[i + 1]) {
                t = arr[i]; arr[i] = arr[i + 1]; arr[i + 1] = t;
            }
        }
    }
}

// Rotate array right by k
void rotate_right(int arr[], int n, int k) {
    int temp[100];
    int i;
    k = k % n;
    for (i = 0; i < n; i = i + 1) {
        temp[(i + k) % n] = arr[i];
    }
    for (i = 0; i < n; i = i + 1) {
        arr[i] = temp[i];
    }
}

// Move all zeros to end
void move_zeros(int arr[], int n) {
    int j;
    int i;
    j = 0;
    for (i = 0; i < n; i = i + 1) {
        if (arr[i] != 0) {
            arr[j] = arr[i];
            j = j + 1;
        }
    }
    while (j < n) {
        arr[j] = 0;
        j = j + 1;
    }
}

// Separate odd and even: even first, odd second
void separate_even_odd(int arr[], int n) {
    int temp[100];
    int j;
    int i;
    j = 0;
    for (i = 0; i < n; i = i + 1) {
        if (arr[i] % 2 == 0) {
            temp[j] = arr[i];
            j = j + 1;
        }
    }
    for (i = 0; i < n; i = i + 1) {
        if (arr[i] % 2 != 0) {
            temp[j] = arr[i];
            j = j + 1;
        }
    }
    for (i = 0; i < n; i = i + 1) {
        arr[i] = temp[i];
    }
}

// Flatten 2D array to 1D
void flatten_2d(int mat[10][10], int rows, int cols, int out[]) {
    int i;
    int j;
    int idx;
    idx = 0;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            out[idx] = mat[i][j];
            idx = idx + 1;
        }
    }
}

// Unflatten 1D to 2D
void unflatten_2d(int in[], int mat[10][10], int rows, int cols) {
    int i;
    int j;
    int idx;
    idx = 0;
    for (i = 0; i < rows; i = i + 1) {
        for (j = 0; j < cols; j = j + 1) {
            mat[i][j] = in[idx];
            idx = idx + 1;
        }
    }
}

// Merge two sorted arrays into one sorted array
void merge_sorted_arrays(int a[], int na, int b[], int nb, int out[]) {
    int i;
    int j;
    int k;
    i = 0;
    j = 0;
    k = 0;
    while (i < na && j < nb) {
        if (a[i] <= b[j]) {
            out[k] = a[i];
            i = i + 1;
        } else {
            out[k] = b[j];
            j = j + 1;
        }
        k = k + 1;
    }
    while (i < na) {
        out[k] = a[i];
        i = i + 1;
        k = k + 1;
    }
    while (j < nb) {
        out[k] = b[j];
        j = j + 1;
        k = k + 1;
    }
}

// Interleave two arrays: a[0],b[0],a[1],b[1],...
void interleave_arrays(int a[], int b[], int n, int out[]) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        out[2 * i] = a[i];
        out[2 * i + 1] = b[i];
    }
}

// Deinterleave: split interleaved array into two
void deinterleave(int in[], int n, int a[], int b[]) {
    int i;
    for (i = 0; i < n; i = i + 1) {
        a[i] = in[2 * i];
        b[i] = in[2 * i + 1];
    }
}

// Prefix sum array
void prefix_sum(int arr[], int n, int ps[]) {
    int i;
    ps[0] = arr[0];
    for (i = 1; i < n; i = i + 1) {
        ps[i] = ps[i - 1] + arr[i];
    }
}

// Range sum using prefix sums
int range_sum(int ps[], int l, int r) {
    if (l == 0) return ps[r];
    return ps[r] - ps[l - 1];
}

// Compress: replace runs with (value, count) pairs
// Return number of pairs
int rle_compress(int arr[], int n, int vals[], int counts[]) {
    int i;
    int npairs;
    if (n == 0) return 0;
    npairs = 0;
    vals[0] = arr[0];
    counts[0] = 1;
    for (i = 1; i < n; i = i + 1) {
        if (arr[i] == vals[npairs]) {
            counts[npairs] = counts[npairs] + 1;
        } else {
            npairs = npairs + 1;
            vals[npairs] = arr[i];
            counts[npairs] = 1;
        }
    }
    return npairs + 1;
}

// Decompress RLE
int rle_decompress(int vals[], int counts[], int npairs, int out[]) {
    int i;
    int j;
    int idx;
    idx = 0;
    for (i = 0; i < npairs; i = i + 1) {
        for (j = 0; j < counts[i]; j = j + 1) {
            out[idx] = vals[i];
            idx = idx + 1;
        }
    }
    return idx;
}

// Cumulative max
void cumulative_max(int arr[], int n, int out[]) {
    int i;
    int mx;
    mx = arr[0];
    out[0] = mx;
    for (i = 1; i < n; i = i + 1) {
        if (arr[i] > mx) mx = arr[i];
        out[i] = mx;
    }
}

int main(void) {
    int arr[20];
    int arr2[20];
    int out[40];
    int ps[20];
    int vals[20];
    int counts[20];
    int mat[10][10];
    int root;
    int cnt;
    int i;

    // Test 1-4: Build balanced BST from sorted array [1..7]
    bst_init();
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5; arr[5]=6; arr[6]=7;
    root = build_balanced_bst(arr, 0, 6);
    printf("%d", bst_val[root]);        // 4 (middle element)
    printf(" %d", bst_height(root));     // 3
    printf(" %d", bst_node_count(root)); // 7
    printf(" %d", bst_is_balanced(root)); // 1

    // Test 5-7: Inorder traversal should be sorted
    inorder_idx = 0;
    bst_inorder(root);
    printf(" %d", inorder_arr[0]); // 1
    printf(" %d", inorder_arr[3]); // 4
    printf(" %d", inorder_arr[6]); // 7

    // Test 8-10: BST search, min, max
    printf(" %d", bst_search(root, 5)); // 1
    printf(" %d", bst_min(root));       // 1
    printf(" %d", bst_max(root));       // 7

    // Test 11-12: BST sum and validity
    printf(" %d", bst_sum(root));      // 1+2+3+4+5+6+7 = 28
    printf(" %d", bst_is_valid(root)); // 1

    // Test 13-15: Level order traversal
    cnt = bst_level_order(root);
    printf(" %d", level_order[0]); // 4 (root)
    printf(" %d", level_order[1]); // 2 (left child)
    printf(" %d", level_order[2]); // 6 (right child)

    // Test 16-18: Build BST by insertion
    bst_init();
    root = -1;
    root = bst_insert(root, 50);
    root = bst_insert(root, 30);
    root = bst_insert(root, 70);
    root = bst_insert(root, 20);
    root = bst_insert(root, 40);
    printf(" %d", bst_height(root));     // 3
    printf(" %d", bst_node_count(root)); // 5
    printf(" %d", bst_is_valid(root));   // 1

    // Test 19-23: Dutch National Flag
    arr[0]=2; arr[1]=0; arr[2]=1; arr[3]=2; arr[4]=0; arr[5]=1; arr[6]=0;
    dutch_flag(arr, 7);
    printf(" %d", arr[0]); // 0
    printf(" %d", arr[1]); // 0
    printf(" %d", arr[2]); // 0
    printf(" %d", arr[3]); // 1
    printf(" %d", arr[4]); // 1

    // Test 24-27: Zigzag rearrangement
    arr[0]=4; arr[1]=3; arr[2]=7; arr[3]=8; arr[4]=6; arr[5]=2; arr[6]=1;
    zigzag(arr, 7);
    // a[0]<a[1]>a[2]<a[3]>a[4]<a[5]>a[6]
    printf(" %d", arr[0] < arr[1]); // 1
    printf(" %d", arr[1] > arr[2]); // 1
    printf(" %d", arr[2] < arr[3]); // 1
    printf(" %d", arr[4] < arr[5]); // 1

    // Test 28-30: Rotate right
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    rotate_right(arr, 5, 2);
    // [4,5,1,2,3]
    printf(" %d", arr[0]); // 4
    printf(" %d", arr[1]); // 5
    printf(" %d", arr[2]); // 1

    // Test 31-33: Move zeros
    arr[0]=0; arr[1]=1; arr[2]=0; arr[3]=3; arr[4]=12;
    move_zeros(arr, 5);
    printf(" %d", arr[0]); // 1
    printf(" %d", arr[1]); // 3
    printf(" %d", arr[4]); // 0

    // Test 34-36: Separate even and odd
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5; arr[5]=6;
    separate_even_odd(arr, 6);
    // evens: 2,4,6 then odds: 1,3,5
    printf(" %d", arr[0]); // 2
    printf(" %d", arr[2]); // 6
    printf(" %d", arr[3]); // 1

    // Test 37-38: Flatten 2D
    mat[0][0]=1; mat[0][1]=2; mat[0][2]=3;
    mat[1][0]=4; mat[1][1]=5; mat[1][2]=6;
    flatten_2d(mat, 2, 3, out);
    printf(" %d", out[0]); // 1
    printf(" %d", out[5]); // 6

    // Test 39-40: Merge sorted arrays
    arr[0]=1; arr[1]=3; arr[2]=5;
    arr2[0]=2; arr2[1]=4; arr2[2]=6;
    merge_sorted_arrays(arr, 3, arr2, 3, out);
    printf(" %d", out[0]); // 1
    printf(" %d", out[5]); // 6

    // Test 41-42: Interleave
    arr[0]=1; arr[1]=2; arr[2]=3;
    arr2[0]=4; arr2[1]=5; arr2[2]=6;
    interleave_arrays(arr, arr2, 3, out);
    printf(" %d", out[0]); // 1
    printf(" %d", out[1]); // 4

    // Test 43-44: Prefix sum
    arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
    prefix_sum(arr, 5, ps);
    printf(" %d", ps[4]); // 15
    printf(" %d", range_sum(ps, 1, 3)); // 2+3+4 = 9

    // Test 45-47: RLE compress
    arr[0]=1; arr[1]=1; arr[2]=2; arr[3]=2; arr[4]=2;
    arr[5]=3; arr[6]=3; arr[7]=1;
    cnt = rle_compress(arr, 8, vals, counts);
    printf(" %d", cnt);       // 4 pairs
    printf(" %d", counts[0]); // 2
    printf(" %d", counts[1]); // 3

    // Test 48: RLE decompress
    cnt = rle_decompress(vals, counts, 4, out);
    printf(" %d", cnt); // 8

    // Test 49-50: Cumulative max
    arr[0]=3; arr[1]=1; arr[2]=4; arr[3]=1; arr[4]=5;
    cumulative_max(arr, 5, out);
    printf(" %d", out[2]); // 4
    printf(" %d", out[4]); // 5

    printf("\n");
    return 0;
}
