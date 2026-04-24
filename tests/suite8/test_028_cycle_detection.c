int printf(const char *fmt, ...);

/* Detect cycle in linked list using Floyd's tortoise and hare algorithm */

struct Node {
    int data;
    int next;
};

struct List {
    struct Node nodes[32];
    int head;
    int count;
};

void list_init(struct List *list) {
    int i;
    list->head = -1;
    list->count = 0;
    for (i = 0; i < 32; i++) {
        list->nodes[i].data = 0;
        list->nodes[i].next = -1;
    }
}

/* Add a node at a specific index in the backing array */
void list_set(struct List *list, int idx, int data, int next) {
    list->nodes[idx].data = data;
    list->nodes[idx].next = next;
}

/* Floyd's cycle detection: returns 1 if cycle exists, 0 otherwise */
int has_cycle(struct List *list) {
    int slow;
    int fast;

    if (list->head == -1) return 0;

    slow = list->head;
    fast = list->head;

    while (fast != -1 && list->nodes[fast].next != -1) {
        slow = list->nodes[slow].next;
        fast = list->nodes[list->nodes[fast].next].next;
        if (slow == fast) return 1;
    }
    return 0;
}

/* Find the start of the cycle (returns node index, or -1) */
int find_cycle_start(struct List *list) {
    int slow;
    int fast;
    int found;

    if (list->head == -1) return -1;

    slow = list->head;
    fast = list->head;
    found = 0;

    while (fast != -1 && list->nodes[fast].next != -1) {
        slow = list->nodes[slow].next;
        fast = list->nodes[list->nodes[fast].next].next;
        if (slow == fast) {
            found = 1;
            break;
        }
    }

    if (!found) return -1;

    /* Move slow to head, then advance both one step at a time */
    slow = list->head;
    while (slow != fast) {
        slow = list->nodes[slow].next;
        fast = list->nodes[fast].next;
    }
    return slow;
}

/* Find cycle length */
int cycle_length(struct List *list) {
    int slow;
    int fast;
    int found;
    int len;

    if (list->head == -1) return 0;

    slow = list->head;
    fast = list->head;
    found = 0;

    while (fast != -1 && list->nodes[fast].next != -1) {
        slow = list->nodes[slow].next;
        fast = list->nodes[list->nodes[fast].next].next;
        if (slow == fast) {
            found = 1;
            break;
        }
    }

    if (!found) return 0;

    /* Count cycle length */
    len = 1;
    fast = list->nodes[slow].next;
    while (fast != slow) {
        len = len + 1;
        fast = list->nodes[fast].next;
    }
    return len;
}

/* Print list (limited to max_steps to avoid infinite loop) */
void list_print_safe(struct List *list, int max_steps) {
    int cur = list->head;
    int steps = 0;
    int first = 1;
    while (cur != -1 && steps < max_steps) {
        if (!first) printf(" ");
        printf("%d", list->nodes[cur].data);
        first = 0;
        cur = list->nodes[cur].next;
        steps = steps + 1;
    }
    if (cur != -1) {
        printf(" ...");
    }
    printf("\n");
}

/* Count nodes in list without cycle */
int list_length(struct List *list) {
    int cur = list->head;
    int count = 0;
    int visited[32];
    int i;
    for (i = 0; i < 32; i++) visited[i] = 0;

    while (cur != -1 && !visited[cur]) {
        visited[cur] = 1;
        count = count + 1;
        cur = list->nodes[cur].next;
    }
    return count;
}

int main() {
    struct List list;
    int result;
    int start;
    int len;

    /* Test 1: No cycle: 10 -> 20 -> 30 -> 40 -> 50 -> nil */
    list_init(&list);
    list.head = 0;
    list_set(&list, 0, 10, 1);
    list_set(&list, 1, 20, 2);
    list_set(&list, 2, 30, 3);
    list_set(&list, 3, 40, 4);
    list_set(&list, 4, 50, -1);

    printf("no-cycle list: ");
    // EXPECT: no-cycle list: 10 20 30 40 50
    list_print_safe(&list, 10);

    result = has_cycle(&list);
    printf("has_cycle=%d\n", result);
    // EXPECT: has_cycle=0

    len = list_length(&list);
    printf("length=%d\n", len);
    // EXPECT: length=5

    /* Test 2: Cycle: 10 -> 20 -> 30 -> 40 -> 50 -> 30 (back to index 2) */
    list_init(&list);
    list.head = 0;
    list_set(&list, 0, 10, 1);
    list_set(&list, 1, 20, 2);
    list_set(&list, 2, 30, 3);
    list_set(&list, 3, 40, 4);
    list_set(&list, 4, 50, 2);  /* cycle back to node index 2 (data=30) */

    printf("cycle list: ");
    // EXPECT: cycle list: 10 20 30 40 50 30 40 ...
    list_print_safe(&list, 7);

    result = has_cycle(&list);
    printf("has_cycle=%d\n", result);
    // EXPECT: has_cycle=1

    start = find_cycle_start(&list);
    printf("cycle starts at node %d data=%d\n", start, list.nodes[start].data);
    // EXPECT: cycle starts at node 2 data=30

    len = cycle_length(&list);
    printf("cycle_length=%d\n", len);
    // EXPECT: cycle_length=3

    /* Test 3: Single node, no cycle */
    list_init(&list);
    list.head = 0;
    list_set(&list, 0, 99, -1);

    result = has_cycle(&list);
    printf("single no-cycle=%d\n", result);
    // EXPECT: single no-cycle=0

    /* Test 4: Single node self-loop */
    list_init(&list);
    list.head = 0;
    list_set(&list, 0, 77, 0);  /* points to itself */

    result = has_cycle(&list);
    printf("self-loop=%d\n", result);
    // EXPECT: self-loop=1

    start = find_cycle_start(&list);
    printf("self-loop start data=%d\n", list.nodes[start].data);
    // EXPECT: self-loop start data=77

    len = cycle_length(&list);
    printf("self-loop length=%d\n", len);
    // EXPECT: self-loop length=1

    /* Test 5: Cycle at tail: 1->2->3->4->3 */
    list_init(&list);
    list.head = 5;
    list_set(&list, 5, 1, 6);
    list_set(&list, 6, 2, 7);
    list_set(&list, 7, 3, 8);
    list_set(&list, 8, 4, 7);  /* cycle back to node 7 (data=3) */

    result = has_cycle(&list);
    printf("tail cycle=%d\n", result);
    // EXPECT: tail cycle=1

    start = find_cycle_start(&list);
    printf("tail cycle start data=%d\n", list.nodes[start].data);
    // EXPECT: tail cycle start data=3

    printf("done\n");
    // EXPECT: done

    return 0;
}
