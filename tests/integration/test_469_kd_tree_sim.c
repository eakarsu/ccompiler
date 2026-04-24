int printf(const char *fmt, ...);
// EXPECT: Nodes: 6\nNearest to (3,5): (2,3)\nNearest to (8,2): (8,1)\nNearest to (1,1): (2,3)\nRange [3,2]-[8,5]:\n  (5,4)\n  (7,2)\nFound: 2\nRange [0,0]-[10,10]:\n  (2,3)\n  (5,4)\n  (8,1)\n  (7,2)\n  (9,6)\n  (4,7)\nFound: 6\nDone

// KD-tree for 2D nearest neighbor search

struct Point {
    int x;
    int y;
};

struct KDNode {
    struct Point point;
    int left;
    int right;
    int valid;
};

struct KDTree {
    struct KDNode nodes[32];
    int root;
    int next_id;
};

void kd_init(struct KDTree *t) {
    t->root = -1;
    t->next_id = 0;
    int i = 0;
    while (i < 32) {
        t->nodes[i].valid = 0;
        t->nodes[i].left = -1;
        t->nodes[i].right = -1;
        i = i + 1;
    }
}

int kd_new_node(struct KDTree *t, int x, int y) {
    int id = t->next_id;
    t->next_id = t->next_id + 1;
    t->nodes[id].point.x = x;
    t->nodes[id].point.y = y;
    t->nodes[id].left = -1;
    t->nodes[id].right = -1;
    t->nodes[id].valid = 1;
    return id;
}

int kd_insert_rec(struct KDTree *t, int node, int x, int y, int depth) {
    if (node < 0) {
        return kd_new_node(t, x, y);
    }
    int use_x = (depth % 2 == 0);
    int go_left;
    if (use_x) {
        go_left = (x < t->nodes[node].point.x);
    } else {
        go_left = (y < t->nodes[node].point.y);
    }
    if (go_left) {
        t->nodes[node].left = kd_insert_rec(t, t->nodes[node].left, x, y, depth + 1);
    } else {
        t->nodes[node].right = kd_insert_rec(t, t->nodes[node].right, x, y, depth + 1);
    }
    return node;
}

void kd_insert(struct KDTree *t, int x, int y) {
    t->root = kd_insert_rec(t, t->root, x, y, 0);
}

int kd_dist_sq(int x1, int y1, int x2, int y2) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    return dx * dx + dy * dy;
}

void kd_nearest_rec(struct KDTree *t, int node, int qx, int qy, int depth,
                    int *best_x, int *best_y, int *best_dist) {
    if (node < 0) return;

    int d = kd_dist_sq(t->nodes[node].point.x, t->nodes[node].point.y, qx, qy);
    if (d < *best_dist) {
        *best_dist = d;
        *best_x = t->nodes[node].point.x;
        *best_y = t->nodes[node].point.y;
    }

    int use_x = (depth % 2 == 0);
    int diff;
    if (use_x) {
        diff = qx - t->nodes[node].point.x;
    } else {
        diff = qy - t->nodes[node].point.y;
    }

    int first;
    int second;
    if (diff < 0) {
        first = t->nodes[node].left;
        second = t->nodes[node].right;
    } else {
        first = t->nodes[node].right;
        second = t->nodes[node].left;
    }

    kd_nearest_rec(t, first, qx, qy, depth + 1, best_x, best_y, best_dist);

    if (diff * diff < *best_dist) {
        kd_nearest_rec(t, second, qx, qy, depth + 1, best_x, best_y, best_dist);
    }
}

void kd_nearest(struct KDTree *t, int qx, int qy, int *rx, int *ry) {
    int best_dist = 999999999;
    *rx = 0;
    *ry = 0;
    kd_nearest_rec(t, t->root, qx, qy, 0, rx, ry, &best_dist);
}

void kd_range_search(struct KDTree *t, int node, int lx, int ly, int hx, int hy,
                     int depth, int *count) {
    if (node < 0) return;
    int px = t->nodes[node].point.x;
    int py = t->nodes[node].point.y;
    if (px >= lx && px <= hx && py >= ly && py <= hy) {
        printf("  (%d,%d)\n", px, py);
        *count = *count + 1;
    }
    int use_x = (depth % 2 == 0);
    if (use_x) {
        if (lx <= px) kd_range_search(t, t->nodes[node].left, lx, ly, hx, hy, depth + 1, count);
        if (hx >= px) kd_range_search(t, t->nodes[node].right, lx, ly, hx, hy, depth + 1, count);
    } else {
        if (ly <= py) kd_range_search(t, t->nodes[node].left, lx, ly, hx, hy, depth + 1, count);
        if (hy >= py) kd_range_search(t, t->nodes[node].right, lx, ly, hx, hy, depth + 1, count);
    }
}

int main(void) {
    struct KDTree tree;
    kd_init(&tree);

    kd_insert(&tree, 2, 3);
    kd_insert(&tree, 5, 4);
    kd_insert(&tree, 9, 6);
    kd_insert(&tree, 4, 7);
    kd_insert(&tree, 8, 1);
    kd_insert(&tree, 7, 2);

    printf("Nodes: %d\n", tree.next_id);

    int rx, ry;
    kd_nearest(&tree, 3, 5, &rx, &ry);
    printf("Nearest to (3,5): (%d,%d)\n", rx, ry);

    kd_nearest(&tree, 8, 2, &rx, &ry);
    printf("Nearest to (8,2): (%d,%d)\n", rx, ry);

    kd_nearest(&tree, 1, 1, &rx, &ry);
    printf("Nearest to (1,1): (%d,%d)\n", rx, ry);

    int count = 0;
    printf("Range [3,2]-[8,5]:\n");
    kd_range_search(&tree, tree.root, 3, 2, 8, 5, 0, &count);
    printf("Found: %d\n", count);

    count = 0;
    printf("Range [0,0]-[10,10]:\n");
    kd_range_search(&tree, tree.root, 0, 0, 10, 10, 0, &count);
    printf("Found: %d\n", count);

    printf("Done\n");
    return 0;
}
