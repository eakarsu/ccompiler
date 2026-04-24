int printf(const char *fmt, ...);

// K-Nearest Neighbors with integer distance (Manhattan)
// Classify test points based on K nearest training points

// Training data: 2D points with labels (0 or 1)
int train_x[20];
int train_y[20];
int train_label[20];
int train_count;

int abs_val(int x) {
    if (x < 0) return -x;
    return x;
}

int manhattan_dist(int x1, int y1, int x2, int y2) {
    return abs_val(x1 - x2) + abs_val(y1 - y2);
}

// Squared Euclidean distance (integer)
int sq_euclidean(int x1, int y1, int x2, int y2) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    return dx * dx + dy * dy;
}

// Find K nearest neighbors, return majority vote
// dist_type: 0 = manhattan, 1 = squared euclidean
int knn_classify(int tx, int ty, int k, int dist_type) {
    int distances[20];
    int indices[20];
    int i = 0;
    while (i < train_count) {
        if (dist_type == 0) {
            distances[i] = manhattan_dist(tx, ty, train_x[i], train_y[i]);
        } else {
            distances[i] = sq_euclidean(tx, ty, train_x[i], train_y[i]);
        }
        indices[i] = i;
        i = i + 1;
    }

    // Selection sort to find K smallest
    i = 0;
    while (i < k) {
        int min_idx = i;
        int j = i + 1;
        while (j < train_count) {
            if (distances[j] < distances[min_idx]) {
                min_idx = j;
            }
            j = j + 1;
        }
        // Swap
        int tmp = distances[i];
        distances[i] = distances[min_idx];
        distances[min_idx] = tmp;
        tmp = indices[i];
        indices[i] = indices[min_idx];
        indices[min_idx] = tmp;
        i = i + 1;
    }

    // Vote among K nearest
    int vote0 = 0;
    int vote1 = 0;
    i = 0;
    while (i < k) {
        if (train_label[indices[i]] == 0) {
            vote0 = vote0 + 1;
        } else {
            vote1 = vote1 + 1;
        }
        i = i + 1;
    }

    if (vote0 >= vote1) return 0;
    return 1;
}

// Leave-one-out cross-validation accuracy
int loocv_accuracy(int k, int dist_type) {
    int correct = 0;
    int i = 0;
    while (i < train_count) {
        // Temporarily remove point i
        int saved_x = train_x[i];
        int saved_y = train_y[i];
        int saved_label = train_label[i];

        // Move last to position i
        train_x[i] = train_x[train_count - 1];
        train_y[i] = train_y[train_count - 1];
        train_label[i] = train_label[train_count - 1];
        train_count = train_count - 1;

        int pred = knn_classify(saved_x, saved_y, k, dist_type);
        if (pred == saved_label) correct = correct + 1;

        // Restore
        train_count = train_count + 1;
        // Shift back: put saved at end, restore swapped
        int temp_x = train_x[i];
        int temp_y = train_y[i];
        int temp_label = train_label[i];

        train_x[i] = saved_x;
        train_y[i] = saved_y;
        train_label[i] = saved_label;

        train_x[train_count - 1] = temp_x;
        train_y[train_count - 1] = temp_y;
        train_label[train_count - 1] = temp_label;

        i = i + 1;
    }
    return correct;
}

int main() {
    // Create two clusters
    // Cluster 0 around (2,2)
    train_x[0] = 1; train_y[0] = 1; train_label[0] = 0;
    train_x[1] = 2; train_y[1] = 1; train_label[1] = 0;
    train_x[2] = 1; train_y[2] = 2; train_label[2] = 0;
    train_x[3] = 2; train_y[3] = 2; train_label[3] = 0;
    train_x[4] = 3; train_y[4] = 2; train_label[4] = 0;
    train_x[5] = 2; train_y[5] = 3; train_label[5] = 0;
    train_x[6] = 1; train_y[6] = 3; train_label[6] = 0;
    train_x[7] = 3; train_y[7] = 1; train_label[7] = 0;

    // Cluster 1 around (8,8)
    train_x[8] = 7; train_y[8] = 7; train_label[8] = 1;
    train_x[9] = 8; train_y[9] = 7; train_label[9] = 1;
    train_x[10] = 7; train_y[10] = 8; train_label[10] = 1;
    train_x[11] = 8; train_y[11] = 8; train_label[11] = 1;
    train_x[12] = 9; train_y[12] = 8; train_label[12] = 1;
    train_x[13] = 8; train_y[13] = 9; train_label[13] = 1;
    train_x[14] = 9; train_y[14] = 9; train_label[14] = 1;
    train_x[15] = 7; train_y[15] = 9; train_label[15] = 1;

    train_count = 16;
    printf("Training points: %d\n", train_count);
    // EXPECT: Training points: 16

    // Test with K=1
    int c = knn_classify(2, 2, 1, 0);
    printf("K=1 (2,2): %d\n", c);
    // EXPECT: K=1 (2,2): 0

    c = knn_classify(8, 8, 1, 0);
    printf("K=1 (8,8): %d\n", c);
    // EXPECT: K=1 (8,8): 1

    // Middle point
    c = knn_classify(5, 5, 3, 0);
    printf("K=3 (5,5): %d\n", c);
    // EXPECT: K=3 (5,5): 0

    c = knn_classify(5, 5, 5, 0);
    printf("K=5 (5,5): %d\n", c);
    // EXPECT: K=5 (5,5): 1

    // Test with euclidean distance
    c = knn_classify(5, 5, 3, 1);
    printf("K=3 eucl (5,5): %d\n", c);
    // EXPECT: K=3 eucl (5,5): 0

    c = knn_classify(6, 6, 3, 1);
    printf("K=3 eucl (6,6): %d\n", c);
    // EXPECT: K=3 eucl (6,6): 1

    // Test near cluster boundary
    c = knn_classify(4, 4, 3, 0);
    printf("K=3 (4,4): %d\n", c);
    // EXPECT: K=3 (4,4): 0

    c = knn_classify(6, 6, 3, 0);
    printf("K=3 (6,6): %d\n", c);
    // EXPECT: K=3 (6,6): 1

    // Leave-one-out cross-validation
    int acc = loocv_accuracy(3, 0);
    printf("LOOCV K=3: %d/%d correct\n", acc, train_count);
    // EXPECT: LOOCV K=3: 16/16 correct

    acc = loocv_accuracy(1, 0);
    printf("LOOCV K=1: %d/%d correct\n", acc, train_count);
    // EXPECT: LOOCV K=1: 16/16 correct

    // Large K (all points)
    c = knn_classify(0, 0, 16, 0);
    printf("K=16 (0,0): %d\n", c);
    // EXPECT: K=16 (0,0): 0

    printf("KNN done\n");
    // EXPECT: KNN done

    return 0;
}
