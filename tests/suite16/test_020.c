int printf(const char *fmt, ...);

// Test 020: Connected component labeling and region analysis on binary image

int find_root(int parent[36], int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]]; // path compression
        x = parent[x];
    }
    return x;
}

void union_labels(int parent[36], int a, int b) {
    int ra;
    int rb;
    ra = find_root(parent, a);
    rb = find_root(parent, b);
    if (ra != rb) {
        if (ra < rb) {
            parent[rb] = ra;
        } else {
            parent[ra] = rb;
        }
    }
}

int label_components(int img[36], int labels[36], int w, int h) {
    int parent[36];
    int next_label;
    int i;
    int j;
    int num_labels;

    next_label = 1;
    for (i = 0; i < w * h; i++) {
        labels[i] = 0;
        parent[i] = i;
    }

    // First pass: assign provisional labels
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            int idx;
            idx = i * w + j;
            if (img[idx] == 0) continue;

            int left;
            int above;
            left = 0;
            above = 0;
            if (j > 0 && labels[idx - 1] > 0) {
                left = labels[idx - 1];
            }
            if (i > 0 && labels[idx - w] > 0) {
                above = labels[idx - w];
            }

            if (left == 0 && above == 0) {
                labels[idx] = next_label;
                next_label = next_label + 1;
            } else if (left > 0 && above == 0) {
                labels[idx] = left;
            } else if (left == 0 && above > 0) {
                labels[idx] = above;
            } else {
                // Both neighbors have labels
                int smaller;
                if (left < above) {
                    smaller = left;
                } else {
                    smaller = above;
                }
                labels[idx] = smaller;
                union_labels(parent, left, above);
            }
        }
    }

    // Second pass: flatten labels
    for (i = 0; i < w * h; i++) {
        if (labels[i] > 0) {
            labels[i] = find_root(parent, labels[i]);
        }
    }

    // Relabel to sequential 1,2,3...
    int remap[36];
    for (i = 0; i < 36; i++) remap[i] = 0;
    num_labels = 0;
    for (i = 0; i < w * h; i++) {
        if (labels[i] > 0) {
            if (remap[labels[i]] == 0) {
                num_labels = num_labels + 1;
                remap[labels[i]] = num_labels;
            }
            labels[i] = remap[labels[i]];
        }
    }

    return num_labels;
}

int region_size(int labels[36], int size, int label) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < size; i++) {
        if (labels[i] == label) c = c + 1;
    }
    return c;
}

int region_sum_row(int labels[36], int w, int h, int label) {
    int i;
    int j;
    int sum;
    sum = 0;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            if (labels[i * w + j] == label) {
                sum = sum + i;
            }
        }
    }
    return sum;
}

int region_sum_col(int labels[36], int w, int h, int label) {
    int i;
    int j;
    int sum;
    sum = 0;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            if (labels[i * w + j] == label) {
                sum = sum + j;
            }
        }
    }
    return sum;
}

int main() {
    int img[36];
    int labels[36];
    int r[6];
    int i;
    int j;
    int n;

    // Create binary image with 3 separate regions
    for (i = 0; i < 36; i++) img[i] = 0;

    // Region A: top-left 2x2 block
    img[0*6+0] = 1; img[0*6+1] = 1;
    img[1*6+0] = 1; img[1*6+1] = 1;

    // Region B: middle 2x3 block
    img[2*6+2] = 1; img[2*6+3] = 1; img[2*6+4] = 1;
    img[3*6+2] = 1; img[3*6+3] = 1; img[3*6+4] = 1;

    // Region C: bottom-right single pixel area
    img[4*6+5] = 1;
    img[5*6+4] = 1; img[5*6+5] = 1;

    printf("Input image:\n"); // EXPECT: Input image:
    for (j = 0; j < 6; j++) r[j] = img[0*6+j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 1 1 0 0 0 0
    for (j = 0; j < 6; j++) r[j] = img[1*6+j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 1 1 0 0 0 0
    for (j = 0; j < 6; j++) r[j] = img[2*6+j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 0 1 1 1 0
    for (j = 0; j < 6; j++) r[j] = img[3*6+j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 0 1 1 1 0
    for (j = 0; j < 6; j++) r[j] = img[4*6+j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 0 0 0 0 1
    for (j = 0; j < 6; j++) r[j] = img[5*6+j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 0 0 0 1 1

    n = label_components(img, labels, 6, 6);
    printf("Num components: %d\n", n); // EXPECT: Num components: 3

    printf("Labels row 0:\n"); // EXPECT: Labels row 0:
    for (j = 0; j < 6; j++) r[j] = labels[0*6+j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 1 1 0 0 0 0

    printf("Labels row 2:\n"); // EXPECT: Labels row 2:
    for (j = 0; j < 6; j++) r[j] = labels[2*6+j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 0 2 2 2 0

    printf("Labels row 5:\n"); // EXPECT: Labels row 5:
    for (j = 0; j < 6; j++) r[j] = labels[5*6+j];
    printf("%d %d %d %d %d %d\n", r[0], r[1], r[2], r[3], r[4], r[5]); // EXPECT: 0 0 0 0 3 3

    printf("Region 1 size: %d\n", region_size(labels, 36, 1)); // EXPECT: Region 1 size: 4
    printf("Region 2 size: %d\n", region_size(labels, 36, 2)); // EXPECT: Region 2 size: 6
    printf("Region 3 size: %d\n", region_size(labels, 36, 3)); // EXPECT: Region 3 size: 3

    // Compute centroids (sum of coords / size)
    int s1;
    int s2;
    int s3;
    s1 = region_size(labels, 36, 1);
    s2 = region_size(labels, 36, 2);
    s3 = region_size(labels, 36, 3);

    printf("Region 1 centroid row: %d\n", region_sum_row(labels, 6, 6, 1) / s1); // EXPECT: Region 1 centroid row: 0
    printf("Region 1 centroid col: %d\n", region_sum_col(labels, 6, 6, 1) / s1); // EXPECT: Region 1 centroid col: 0
    printf("Region 2 centroid row: %d\n", region_sum_row(labels, 6, 6, 2) / s2); // EXPECT: Region 2 centroid row: 2
    printf("Region 2 centroid col: %d\n", region_sum_col(labels, 6, 6, 2) / s2); // EXPECT: Region 2 centroid col: 3
    printf("Region 3 centroid row: %d\n", region_sum_row(labels, 6, 6, 3) / s3); // EXPECT: Region 3 centroid row: 4
    printf("Region 3 centroid col: %d\n", region_sum_col(labels, 6, 6, 3) / s3); // EXPECT: Region 3 centroid col: 4

    return 0;
}
