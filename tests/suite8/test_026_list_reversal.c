int printf(const char *fmt, ...);

/* List reversal algorithms using array-backed linked list */

struct Node {
    int data;
    int next;
};

struct List {
    struct Node nodes[32];
    int head;
    int free_idx;
    int size;
};

void list_init(struct List *list) {
    int i;
    list->head = -1;
    list->free_idx = 0;
    list->size = 0;
    for (i = 0; i < 31; i++) {
        list->nodes[i].next = i + 1;
        list->nodes[i].data = 0;
    }
    list->nodes[31].next = -1;
    list->nodes[31].data = 0;
}

int list_alloc(struct List *list) {
    int idx;
    if (list->free_idx == -1) return -1;
    idx = list->free_idx;
    list->free_idx = list->nodes[idx].next;
    list->nodes[idx].next = -1;
    return idx;
}

void list_append(struct List *list, int val) {
    int idx = list_alloc(list);
    int cur;
    if (idx == -1) return;
    list->nodes[idx].data = val;
    list->nodes[idx].next = -1;
    if (list->head == -1) {
        list->head = idx;
    } else {
        cur = list->head;
        while (list->nodes[cur].next != -1) {
            cur = list->nodes[cur].next;
        }
        list->nodes[cur].next = idx;
    }
    list->size = list->size + 1;
}

void list_print(struct List *list) {
    int cur = list->head;
    int first = 1;
    while (cur != -1) {
        if (!first) printf(" ");
        printf("%d", list->nodes[cur].data);
        first = 0;
        cur = list->nodes[cur].next;
    }
    printf("\n");
}

/* Iterative reversal */
void list_reverse_iterative(struct List *list) {
    int prev_idx = -1;
    int cur = list->head;
    int next_idx;

    while (cur != -1) {
        next_idx = list->nodes[cur].next;
        list->nodes[cur].next = prev_idx;
        prev_idx = cur;
        cur = next_idx;
    }
    list->head = prev_idx;
}

/* Reverse first k nodes */
void list_reverse_k(struct List *list, int k) {
    int prev_idx = -1;
    int cur = list->head;
    int next_idx;
    int count = 0;
    int orig_head = list->head;

    while (cur != -1 && count < k) {
        next_idx = list->nodes[cur].next;
        list->nodes[cur].next = prev_idx;
        prev_idx = cur;
        cur = next_idx;
        count = count + 1;
    }
    /* Connect the original head to the rest */
    if (orig_head != -1) {
        list->nodes[orig_head].next = cur;
    }
    list->head = prev_idx;
}

/* Check if list is a palindrome (using array copy) */
int list_is_palindrome(struct List *list) {
    int vals[32];
    int cur = list->head;
    int n = 0;
    int i;

    while (cur != -1) {
        vals[n] = list->nodes[cur].data;
        n = n + 1;
        cur = list->nodes[cur].next;
    }

    for (i = 0; i < n / 2; i++) {
        if (vals[i] != vals[n - 1 - i]) return 0;
    }
    return 1;
}

/* Count nodes in list */
int list_count(struct List *list) {
    int cur = list->head;
    int count = 0;
    while (cur != -1) {
        count = count + 1;
        cur = list->nodes[cur].next;
    }
    return count;
}

/* Get nth element from end */
int list_nth_from_end(struct List *list, int n) {
    int total = list_count(list);
    int target;
    int cur;
    int i;
    if (n > total) return -999;
    target = total - n;
    cur = list->head;
    for (i = 0; i < target; i++) {
        cur = list->nodes[cur].next;
    }
    return list->nodes[cur].data;
}

int main() {
    struct List list;
    int val;

    /* Test iterative reversal */
    list_init(&list);
    list_append(&list, 1);
    list_append(&list, 2);
    list_append(&list, 3);
    list_append(&list, 4);
    list_append(&list, 5);

    printf("original: ");
    // EXPECT: original: 1 2 3 4 5
    list_print(&list);

    list_reverse_iterative(&list);
    printf("reversed: ");
    // EXPECT: reversed: 5 4 3 2 1
    list_print(&list);

    list_reverse_iterative(&list);
    printf("back: ");
    // EXPECT: back: 1 2 3 4 5
    list_print(&list);

    /* Test reverse first k */
    list_reverse_k(&list, 3);
    printf("reverse_k(3): ");
    // EXPECT: reverse_k(3): 3 2 1 4 5
    list_print(&list);

    /* Test palindrome */
    list_init(&list);
    list_append(&list, 1);
    list_append(&list, 2);
    list_append(&list, 3);
    list_append(&list, 2);
    list_append(&list, 1);
    printf("palindrome 12321=%d\n", list_is_palindrome(&list));
    // EXPECT: palindrome 12321=1

    list_init(&list);
    list_append(&list, 1);
    list_append(&list, 2);
    list_append(&list, 3);
    printf("palindrome 123=%d\n", list_is_palindrome(&list));
    // EXPECT: palindrome 123=0

    /* Test nth from end */
    list_init(&list);
    list_append(&list, 10);
    list_append(&list, 20);
    list_append(&list, 30);
    list_append(&list, 40);
    list_append(&list, 50);

    val = list_nth_from_end(&list, 1);
    printf("1st from end=%d\n", val);
    // EXPECT: 1st from end=50

    val = list_nth_from_end(&list, 3);
    printf("3rd from end=%d\n", val);
    // EXPECT: 3rd from end=30

    val = list_nth_from_end(&list, 5);
    printf("5th from end=%d\n", val);
    // EXPECT: 5th from end=10

    printf("count=%d\n", list_count(&list));
    // EXPECT: count=5

    return 0;
}
