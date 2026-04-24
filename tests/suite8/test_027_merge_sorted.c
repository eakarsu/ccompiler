int printf(const char *fmt, ...);

/* Merge two sorted linked lists */

struct Node {
    int data;
    int next;
};

struct List {
    struct Node nodes[64];
    int head;
    int free_idx;
    int size;
};

void list_init(struct List *list) {
    int i;
    list->head = -1;
    list->free_idx = 0;
    list->size = 0;
    for (i = 0; i < 63; i++) {
        list->nodes[i].next = i + 1;
        list->nodes[i].data = 0;
    }
    list->nodes[63].next = -1;
    list->nodes[63].data = 0;
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

/* Merge two sorted lists stored in the same backing array.
 * Returns the head index of the merged list. */
int merge_sorted(struct List *list, int head1, int head2) {
    int dummy_data;
    int result_head = -1;
    int result_tail = -1;
    int cur1 = head1;
    int cur2 = head2;
    int chosen;

    while (cur1 != -1 && cur2 != -1) {
        if (list->nodes[cur1].data <= list->nodes[cur2].data) {
            chosen = cur1;
            cur1 = list->nodes[cur1].next;
        } else {
            chosen = cur2;
            cur2 = list->nodes[cur2].next;
        }
        list->nodes[chosen].next = -1;
        if (result_head == -1) {
            result_head = chosen;
            result_tail = chosen;
        } else {
            list->nodes[result_tail].next = chosen;
            result_tail = chosen;
        }
    }

    while (cur1 != -1) {
        chosen = cur1;
        cur1 = list->nodes[cur1].next;
        list->nodes[chosen].next = -1;
        if (result_head == -1) {
            result_head = chosen;
            result_tail = chosen;
        } else {
            list->nodes[result_tail].next = chosen;
            result_tail = chosen;
        }
    }

    while (cur2 != -1) {
        chosen = cur2;
        cur2 = list->nodes[cur2].next;
        list->nodes[chosen].next = -1;
        if (result_head == -1) {
            result_head = chosen;
            result_tail = chosen;
        } else {
            list->nodes[result_tail].next = chosen;
            result_tail = chosen;
        }
    }

    return result_head;
}

/* Count nodes from a given head */
int count_from(struct List *list, int head) {
    int cur = head;
    int count = 0;
    while (cur != -1) {
        count = count + 1;
        cur = list->nodes[cur].next;
    }
    return count;
}

/* Print from a given head index */
void print_from(struct List *list, int head) {
    int cur = head;
    int first = 1;
    while (cur != -1) {
        if (!first) printf(" ");
        printf("%d", list->nodes[cur].data);
        first = 0;
        cur = list->nodes[cur].next;
    }
    printf("\n");
}

/* Sorted insert into list */
void list_sorted_insert(struct List *list, int val) {
    int idx = list_alloc(list);
    int cur;
    int prev;
    if (idx == -1) return;
    list->nodes[idx].data = val;
    list->nodes[idx].next = -1;

    if (list->head == -1 || list->nodes[list->head].data >= val) {
        list->nodes[idx].next = list->head;
        list->head = idx;
    } else {
        cur = list->head;
        while (list->nodes[cur].next != -1 &&
               list->nodes[list->nodes[cur].next].data < val) {
            cur = list->nodes[cur].next;
        }
        list->nodes[idx].next = list->nodes[cur].next;
        list->nodes[cur].next = idx;
    }
    list->size = list->size + 1;
}

int main() {
    struct List list;
    int head1;
    int head2;
    int merged;
    int count;

    /* Build two sorted lists in the same backing store */
    list_init(&list);

    /* List 1: 1, 3, 5, 7, 9 */
    list_append(&list, 1);
    list_append(&list, 3);
    list_append(&list, 5);
    list_append(&list, 7);
    list_append(&list, 9);
    head1 = list.head;

    printf("list1: ");
    // EXPECT: list1: 1 3 5 7 9
    print_from(&list, head1);

    /* Build list2 separately, manually linking */
    /* List 2: 2, 4, 6, 8, 10 */
    list.head = -1;
    list_append(&list, 2);
    list_append(&list, 4);
    list_append(&list, 6);
    list_append(&list, 8);
    list_append(&list, 10);
    head2 = list.head;

    printf("list2: ");
    // EXPECT: list2: 2 4 6 8 10
    print_from(&list, head2);

    /* Merge */
    merged = merge_sorted(&list, head1, head2);
    printf("merged: ");
    // EXPECT: merged: 1 2 3 4 5 6 7 8 9 10
    print_from(&list, merged);

    count = count_from(&list, merged);
    printf("merged count=%d\n", count);
    // EXPECT: merged count=10

    /* Test sorted insert */
    list_init(&list);
    list_sorted_insert(&list, 30);
    list_sorted_insert(&list, 10);
    list_sorted_insert(&list, 50);
    list_sorted_insert(&list, 20);
    list_sorted_insert(&list, 40);
    printf("sorted insert: ");
    // EXPECT: sorted insert: 10 20 30 40 50
    list_print(&list);

    /* Merge with empty list */
    list_init(&list);
    list_append(&list, 5);
    list_append(&list, 15);
    list_append(&list, 25);
    head1 = list.head;
    head2 = -1;
    merged = merge_sorted(&list, head1, head2);
    printf("merge with empty: ");
    // EXPECT: merge with empty: 5 15 25
    print_from(&list, merged);

    printf("done\n");
    // EXPECT: done

    return 0;
}
