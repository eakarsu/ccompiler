int printf(const char *fmt, ...);

// Linked list intersection detection
// Two singly linked lists that may share a common tail

struct Node {
    int data;
    int next;
};

struct Pool {
    struct Node nodes[30];
    int free_idx;
};

void pool_init(struct Pool *p) {
    int i;
    p->free_idx = 0;
    for (i = 0; i < 29; i++) {
        p->nodes[i].next = i + 1;
        p->nodes[i].data = 0;
    }
    p->nodes[29].next = -1;
    p->nodes[29].data = 0;
}

int pool_alloc(struct Pool *p) {
    int idx;
    if (p->free_idx == -1) return -1;
    idx = p->free_idx;
    p->free_idx = p->nodes[idx].next;
    p->nodes[idx].next = -1;
    return idx;
}

// Build a chain of nodes and return the head index
// values: array of values, n: count
int build_chain(struct Pool *p, int *values, int n) {
    int head = -1;
    int tail = -1;
    int i;
    int idx;
    for (i = 0; i < n; i++) {
        idx = pool_alloc(p);
        if (idx == -1) return -1;
        p->nodes[idx].data = values[i];
        p->nodes[idx].next = -1;
        if (head == -1) {
            head = idx;
        } else {
            p->nodes[tail].next = idx;
        }
        tail = idx;
    }
    return head;
}

// Get the tail node index
int get_tail(struct Pool *p, int head) {
    int cur = head;
    if (cur == -1) return -1;
    while (p->nodes[cur].next != -1) {
        cur = p->nodes[cur].next;
    }
    return cur;
}

// Get length of a list
int list_len(struct Pool *p, int head) {
    int count = 0;
    int cur = head;
    while (cur != -1) {
        count++;
        cur = p->nodes[cur].next;
    }
    return count;
}

// Get node at position
int get_at(struct Pool *p, int head, int pos) {
    int cur = head;
    int i;
    for (i = 0; i < pos; i++) {
        if (cur == -1) return -1;
        cur = p->nodes[cur].next;
    }
    return cur;
}

// Connect tail of list A to a specific node in the pool
void connect_tail(struct Pool *p, int head_a, int target) {
    int tail = get_tail(p, head_a);
    if (tail != -1) {
        p->nodes[tail].next = target;
    }
}

// Find intersection node using length difference method
// Returns data of intersection node, or -1 if no intersection
int find_intersection(struct Pool *p, int head_a, int head_b) {
    int len_a = list_len(p, head_a);
    int len_b = list_len(p, head_b);
    int diff;
    int cur_a = head_a;
    int cur_b = head_b;
    int i;

    // Advance the longer list
    if (len_a > len_b) {
        diff = len_a - len_b;
        for (i = 0; i < diff; i++) {
            cur_a = p->nodes[cur_a].next;
        }
    } else {
        diff = len_b - len_a;
        for (i = 0; i < diff; i++) {
            cur_b = p->nodes[cur_b].next;
        }
    }

    // Walk together until they meet
    while (cur_a != -1 && cur_b != -1) {
        if (cur_a == cur_b) {
            return p->nodes[cur_a].data;
        }
        cur_a = p->nodes[cur_a].next;
        cur_b = p->nodes[cur_b].next;
    }

    return -1;
}

// Alternative: Find intersection using two-pointer technique
// Each pointer, upon reaching end, jumps to the other list's head
int find_intersection_two_ptr(struct Pool *p, int head_a, int head_b) {
    int ptr_a = head_a;
    int ptr_b = head_b;
    int switched_a = 0;
    int switched_b = 0;
    int max_steps;
    int step;

    if (head_a == -1 || head_b == -1) return -1;

    max_steps = list_len(p, head_a) + list_len(p, head_b) + 2;

    for (step = 0; step < max_steps; step++) {
        if (ptr_a == ptr_b) return p->nodes[ptr_a].data;
        if (p->nodes[ptr_a].next == -1 && !switched_a) {
            ptr_a = head_b;
            switched_a = 1;
        } else {
            ptr_a = p->nodes[ptr_a].next;
        }
        if (p->nodes[ptr_b].next == -1 && !switched_b) {
            ptr_b = head_a;
            switched_b = 1;
        } else {
            ptr_b = p->nodes[ptr_b].next;
        }
        if (ptr_a == -1 || ptr_b == -1) return -1;
    }
    return -1;
}

void print_list(struct Pool *p, int head) {
    int cur = head;
    int first = 1;
    while (cur != -1) {
        if (!first) printf(" -> ");
        printf("%d", p->nodes[cur].data);
        first = 0;
        cur = p->nodes[cur].next;
    }
    printf("\n");
}

int main() {
    struct Pool p;
    int head_a;
    int head_b;
    int common;
    int result;
    int vals_a[5];
    int vals_b[5];
    int vals_c[5];

    // Test 1: Two lists that intersect
    // A: 1 -> 2 -> 3 \
    //                   -> 7 -> 8 -> 9
    // B: 4 -> 5 -> 6 /
    pool_init(&p);

    vals_a[0] = 1; vals_a[1] = 2; vals_a[2] = 3;
    head_a = build_chain(&p, vals_a, 3);

    vals_b[0] = 4; vals_b[1] = 5; vals_b[2] = 6;
    head_b = build_chain(&p, vals_b, 3);

    vals_c[0] = 7; vals_c[1] = 8; vals_c[2] = 9;
    common = build_chain(&p, vals_c, 3);

    connect_tail(&p, head_a, common);
    connect_tail(&p, head_b, common);

    printf("List A: ");
    print_list(&p, head_a);
    // EXPECT: List A: 1 -> 2 -> 3 -> 7 -> 8 -> 9

    printf("List B: ");
    print_list(&p, head_b);
    // EXPECT: List B: 4 -> 5 -> 6 -> 7 -> 8 -> 9

    result = find_intersection(&p, head_a, head_b);
    printf("Intersection (length method): %d\n", result);
    // EXPECT: Intersection (length method): 7

    result = find_intersection_two_ptr(&p, head_a, head_b);
    printf("Intersection (two-ptr method): %d\n", result);
    // EXPECT: Intersection (two-ptr method): 7

    // Test 2: No intersection
    pool_init(&p);
    vals_a[0] = 10; vals_a[1] = 20;
    head_a = build_chain(&p, vals_a, 2);

    vals_b[0] = 30; vals_b[1] = 40;
    head_b = build_chain(&p, vals_b, 2);

    result = find_intersection(&p, head_a, head_b);
    printf("No intersection: %d\n", result);
    // EXPECT: No intersection: -1

    // Test 3: Intersection at first node of shorter list
    // A: 1 -> 2 -> 3 -> 4 -> 5
    // B: 3 -> 4 -> 5  (shares tail starting at node with data=3)
    pool_init(&p);
    vals_a[0] = 1; vals_a[1] = 2;
    head_a = build_chain(&p, vals_a, 2);

    vals_c[0] = 3; vals_c[1] = 4; vals_c[2] = 5;
    common = build_chain(&p, vals_c, 3);

    connect_tail(&p, head_a, common);
    head_b = common;

    printf("List A: ");
    print_list(&p, head_a);
    // EXPECT: List A: 1 -> 2 -> 3 -> 4 -> 5

    printf("List B: ");
    print_list(&p, head_b);
    // EXPECT: List B: 3 -> 4 -> 5

    result = find_intersection(&p, head_a, head_b);
    printf("Intersection at B head: %d\n", result);
    // EXPECT: Intersection at B head: 3

    // Test 4: Lists of different lengths with intersection
    // A: 100
    // B: 200 -> 300 -> 400
    // Common: 500 -> 600
    pool_init(&p);
    vals_a[0] = 100;
    head_a = build_chain(&p, vals_a, 1);

    vals_b[0] = 200; vals_b[1] = 300; vals_b[2] = 400;
    head_b = build_chain(&p, vals_b, 3);

    vals_c[0] = 500; vals_c[1] = 600;
    common = build_chain(&p, vals_c, 2);

    connect_tail(&p, head_a, common);
    connect_tail(&p, head_b, common);

    printf("Len A: %d\n", list_len(&p, head_a));
    // EXPECT: Len A: 3

    printf("Len B: %d\n", list_len(&p, head_b));
    // EXPECT: Len B: 5

    result = find_intersection(&p, head_a, head_b);
    printf("Intersection diff lengths: %d\n", result);
    // EXPECT: Intersection diff lengths: 500

    result = find_intersection_two_ptr(&p, head_a, head_b);
    printf("Two-ptr diff lengths: %d\n", result);
    // EXPECT: Two-ptr diff lengths: 500

    return 0;
}
