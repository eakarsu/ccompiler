int printf(const char *fmt, ...);

// Decision Tree Classifier (integer-only)
// Simple binary decision tree for classifying data points
// Each node: feature index, threshold, left/right child
// Leaves store class label

// Tree nodes stored as arrays
int node_feature[31];   // which feature to split on
int node_threshold[31]; // threshold value
int node_left[31];      // left child index (-1 for leaf)
int node_right[31];     // right child index (-1 for leaf)
int node_class[31];     // class label (for leaf nodes)
int node_is_leaf[31];
int node_count;

int create_leaf(int class_label) {
    int idx = node_count;
    node_count = node_count + 1;
    node_feature[idx] = -1;
    node_threshold[idx] = 0;
    node_left[idx] = -1;
    node_right[idx] = -1;
    node_class[idx] = class_label;
    node_is_leaf[idx] = 1;
    return idx;
}

int create_node(int feature, int threshold, int left, int right) {
    int idx = node_count;
    node_count = node_count + 1;
    node_feature[idx] = feature;
    node_threshold[idx] = threshold;
    node_left[idx] = left;
    node_right[idx] = right;
    node_class[idx] = -1;
    node_is_leaf[idx] = 0;
    return idx;
}

int classify(int root, int *features) {
    int cur = root;
    int depth = 0;
    while (node_is_leaf[cur] == 0) {
        int f = node_feature[cur];
        int t = node_threshold[cur];
        if (features[f] <= t) {
            cur = node_left[cur];
        } else {
            cur = node_right[cur];
        }
        depth = depth + 1;
        if (depth > 10) break;  // safety
    }
    return node_class[cur];
}

// Training data: 3 features, binary classification
int train_data[12][3];
int train_labels[12];

// Simple splitting: find best feature and threshold (brute force)
int count_class(int *indices, int n, int target) {
    int count = 0;
    int i = 0;
    while (i < n) {
        if (train_labels[indices[i]] == target) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

// Gini impurity * 100 (integer scaled)
int gini_impurity(int *indices, int n) {
    if (n == 0) return 0;
    int c0 = count_class(indices, n, 0);
    int c1 = n - c0;
    // gini = 1 - (c0/n)^2 - (c1/n)^2
    // scaled: 100 - (c0*c0*100)/(n*n) - (c1*c1*100)/(n*n)
    return 100 - (c0 * c0 * 100) / (n * n) - (c1 * c1 * 100) / (n * n);
}

int main() {
    node_count = 0;

    // Build a manual decision tree:
    //   feature 0 <= 5?
    //     yes: feature 1 <= 3?
    //       yes: class 0
    //       no:  class 1
    //     no:  feature 2 <= 7?
    //       yes: class 1
    //       no:  class 0

    int leaf00 = create_leaf(0);
    int leaf01 = create_leaf(1);
    int leaf10 = create_leaf(1);
    int leaf11 = create_leaf(0);

    int node_left_child = create_node(1, 3, leaf00, leaf01);
    int node_right_child = create_node(2, 7, leaf10, leaf11);
    int root = create_node(0, 5, node_left_child, node_right_child);

    printf("Tree has %d nodes\n", node_count);
    // EXPECT: Tree has 7 nodes

    // Test classification
    int f1[3];
    f1[0] = 3; f1[1] = 2; f1[2] = 5;
    int c = classify(root, f1);
    printf("(%d,%d,%d) -> class %d\n", f1[0], f1[1], f1[2], c);
    // EXPECT: (3,2,5) -> class 0

    int f2[3];
    f2[0] = 3; f2[1] = 5; f2[2] = 5;
    c = classify(root, f2);
    printf("(%d,%d,%d) -> class %d\n", f2[0], f2[1], f2[2], c);
    // EXPECT: (3,5,5) -> class 1

    int f3[3];
    f3[0] = 8; f3[1] = 2; f3[2] = 6;
    c = classify(root, f3);
    printf("(%d,%d,%d) -> class %d\n", f3[0], f3[1], f3[2], c);
    // EXPECT: (8,2,6) -> class 1

    int f4[3];
    f4[0] = 8; f4[1] = 2; f4[2] = 9;
    c = classify(root, f4);
    printf("(%d,%d,%d) -> class %d\n", f4[0], f4[1], f4[2], c);
    // EXPECT: (8,2,9) -> class 0

    // Boundary cases
    int f5[3];
    f5[0] = 5; f5[1] = 3; f5[2] = 7;
    c = classify(root, f5);
    printf("(%d,%d,%d) -> class %d\n", f5[0], f5[1], f5[2], c);
    // EXPECT: (5,3,7) -> class 0

    int f6[3];
    f6[0] = 5; f6[1] = 4; f6[2] = 7;
    c = classify(root, f6);
    printf("(%d,%d,%d) -> class %d\n", f6[0], f6[1], f6[2], c);
    // EXPECT: (5,4,7) -> class 1

    // Test Gini impurity
    int all_indices[8];
    int i = 0;
    while (i < 8) {
        all_indices[i] = i;
        i = i + 1;
    }

    // All same class
    i = 0;
    while (i < 8) {
        train_labels[i] = 0;
        i = i + 1;
    }
    int g = gini_impurity(all_indices, 8);
    printf("Gini all-0: %d\n", g);
    // EXPECT: Gini all-0: 0

    // Half and half
    train_labels[0] = 0; train_labels[1] = 0;
    train_labels[2] = 0; train_labels[3] = 0;
    train_labels[4] = 1; train_labels[5] = 1;
    train_labels[6] = 1; train_labels[7] = 1;
    g = gini_impurity(all_indices, 8);
    printf("Gini 4-4 split: %d\n", g);
    // EXPECT: Gini 4-4 split: 50

    // 6-2 split
    train_labels[4] = 0; train_labels[5] = 0;
    g = gini_impurity(all_indices, 8);
    printf("Gini 6-2 split: %d\n", g);
    // EXPECT: Gini 6-2 split: 38

    // Classify a batch
    int test_batch[6][3];
    test_batch[0][0] = 1; test_batch[0][1] = 1; test_batch[0][2] = 1;
    test_batch[1][0] = 4; test_batch[1][1] = 4; test_batch[1][2] = 4;
    test_batch[2][0] = 7; test_batch[2][1] = 7; test_batch[2][2] = 7;
    test_batch[3][0] = 7; test_batch[3][1] = 7; test_batch[3][2] = 9;
    test_batch[4][0] = 2; test_batch[4][1] = 8; test_batch[4][2] = 2;
    test_batch[5][0] = 6; test_batch[5][1] = 1; test_batch[5][2] = 5;

    printf("Batch results: ");
    i = 0;
    while (i < 6) {
        c = classify(root, test_batch[i]);
        printf("%d ", c);
        i = i + 1;
    }
    printf("\n");
    // EXPECT: Batch results: 0 1 1 0 1 1 

    printf("Decision tree done\n");
    // EXPECT: Decision tree done

    return 0;
}
