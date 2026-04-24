int printf(const char *fmt, ...);

// Level-order traversal (BFS) with depth tracking
// Uses array-based tree and array-based queue

struct TreeNode {
    int val;
    int left;   // index, 0 = none
    int right;  // index, 0 = none
};

struct Tree {
    struct TreeNode nodes[32];
    int count;
};

void tree_init(struct Tree *t) {
    int i;
    for (i = 0; i < 32; i++) {
        t->nodes[i].val = 0;
        t->nodes[i].left = 0;
        t->nodes[i].right = 0;
    }
    t->count = 1; // 0 is sentinel
}

int tree_add(struct Tree *t, int val) {
    int idx = t->count;
    t->count = t->count + 1;
    t->nodes[idx].val = val;
    t->nodes[idx].left = 0;
    t->nodes[idx].right = 0;
    return idx;
}

void tree_link(struct Tree *t, int parent, int left, int right) {
    t->nodes[parent].left = left;
    t->nodes[parent].right = right;
}

// Queue for BFS
struct Queue {
    int node_idx[64];
    int depth[64];
    int front;
    int back;
};

void queue_init(struct Queue *q) {
    q->front = 0;
    q->back = 0;
}

void queue_push(struct Queue *q, int node, int d) {
    q->node_idx[q->back] = node;
    q->depth[q->back] = d;
    q->back = q->back + 1;
}

int queue_empty(struct Queue *q) {
    return q->front == q->back;
}

int queue_pop_node(struct Queue *q) {
    int val = q->node_idx[q->front];
    return val;
}

int queue_pop_depth(struct Queue *q) {
    int d = q->depth[q->front];
    q->front = q->front + 1;
    return d;
}

// Level order traversal, printing level boundaries
void level_order(struct Tree *t, int root) {
    struct Queue q;
    int cur_node;
    int cur_depth;
    int prev_depth;
    int first;

    if (root == 0) return;

    queue_init(&q);
    queue_push(&q, root, 0);
    prev_depth = -1;
    first = 1;

    while (!queue_empty(&q)) {
        cur_node = queue_pop_node(&q);
        cur_depth = queue_pop_depth(&q);

        if (cur_depth != prev_depth) {
            if (prev_depth >= 0) {
                printf("\n");
            }
            printf("Level %d:", cur_depth);
            prev_depth = cur_depth;
        }
        printf(" %d", t->nodes[cur_node].val);

        if (t->nodes[cur_node].left != 0) {
            queue_push(&q, t->nodes[cur_node].left, cur_depth + 1);
        }
        if (t->nodes[cur_node].right != 0) {
            queue_push(&q, t->nodes[cur_node].right, cur_depth + 1);
        }
    }
    printf("\n");
}

// Count nodes at a specific depth
int count_at_depth(struct Tree *t, int node, int target, int current) {
    if (node == 0) return 0;
    if (current == target) return 1;
    return count_at_depth(t, t->nodes[node].left, target, current + 1) +
           count_at_depth(t, t->nodes[node].right, target, current + 1);
}

// Width of tree at a given level
int width_at_level(struct Tree *t, int root, int level) {
    return count_at_depth(t, root, level, 0);
}

int height_helper(struct Tree *t, int n) {
    int lh;
    int rh;
    if (n == 0) return 0;
    lh = height_helper(t, t->nodes[n].left);
    rh = height_helper(t, t->nodes[n].right);
    if (lh > rh) return lh + 1;
    return rh + 1;
}

// Maximum width across all levels
int max_width(struct Tree *t, int root) {
    int h;
    int i;
    int w;
    int max_w;
    h = height_helper(t, root);
    max_w = 0;
    for (i = 0; i < h; i++) {
        w = width_at_level(t, root, i);
        if (w > max_w) max_w = w;
    }
    return max_w;
}

int main() {
    struct Tree t;
    tree_init(&t);

    //          1
    //        /   \
    //       2     3
    //      / \   / \
    //     4   5 6   7
    //    /       \
    //   8         9

    int n1 = tree_add(&t, 1);
    int n2 = tree_add(&t, 2);
    int n3 = tree_add(&t, 3);
    int n4 = tree_add(&t, 4);
    int n5 = tree_add(&t, 5);
    int n6 = tree_add(&t, 6);
    int n7 = tree_add(&t, 7);
    int n8 = tree_add(&t, 8);
    int n9 = tree_add(&t, 9);

    tree_link(&t, n1, n2, n3);
    tree_link(&t, n2, n4, n5);
    tree_link(&t, n3, n6, n7);
    tree_link(&t, n4, n8, 0);
    tree_link(&t, n6, 0, n9);

    printf("Level-order traversal:\n");
    // EXPECT: Level-order traversal:
    level_order(&t, n1);
    // EXPECT: Level 0: 1
    // EXPECT: Level 1: 2 3
    // EXPECT: Level 2: 4 5 6 7
    // EXPECT: Level 3: 8 9

    printf("Height: %d\n", height_helper(&t, n1));
    // EXPECT: Height: 4

    printf("Width at level 0: %d\n", width_at_level(&t, n1, 0));
    // EXPECT: Width at level 0: 1
    printf("Width at level 1: %d\n", width_at_level(&t, n1, 1));
    // EXPECT: Width at level 1: 2
    printf("Width at level 2: %d\n", width_at_level(&t, n1, 2));
    // EXPECT: Width at level 2: 4
    printf("Width at level 3: %d\n", width_at_level(&t, n1, 3));
    // EXPECT: Width at level 3: 2

    printf("Max width: %d\n", max_width(&t, n1));
    // EXPECT: Max width: 4

    // A simpler tree
    struct Tree t2;
    tree_init(&t2);
    //    10
    //   /
    //  20
    //   \
    //   30
    int a = tree_add(&t2, 10);
    int b = tree_add(&t2, 20);
    int c = tree_add(&t2, 30);
    tree_link(&t2, a, b, 0);
    tree_link(&t2, b, 0, c);

    printf("Simple tree:\n");
    // EXPECT: Simple tree:
    level_order(&t2, a);
    // EXPECT: Level 0: 10
    // EXPECT: Level 1: 20
    // EXPECT: Level 2: 30

    printf("Simple height: %d\n", height_helper(&t2, a));
    // EXPECT: Simple height: 3
    printf("Simple max width: %d\n", max_width(&t2, a));
    // EXPECT: Simple max width: 1

    return 0;
}
