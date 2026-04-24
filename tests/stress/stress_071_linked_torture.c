// stress_071_linked_torture.c - Linked list torture test
// EXPECT: build_ok=50
// EXPECT: sum_original=1275
// EXPECT: reverse_head=50
// EXPECT: reverse_tail=1
// EXPECT: sort_ok=1
// EXPECT: sort_first=1
// EXPECT: sort_last=50
// EXPECT: merge_count=100
// EXPECT: merge_sorted=1
// EXPECT: dedup_count=50
// EXPECT: splice_count=75
// EXPECT: checksum=2550

int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

typedef struct Node {
    int val;
    struct Node *next;
} Node;

Node *new_node(int v) {
    Node *n = (Node *)malloc(sizeof(Node));
    n->val = v;
    n->next = 0;
    return n;
}

int list_len(Node *h) {
    int c = 0;
    while (h) { c++; h = h->next; }
    return c;
}

int list_sum(Node *h) {
    int s = 0;
    while (h) { s += h->val; h = h->next; }
    return s;
}

Node *list_tail(Node *h) {
    if (!h) return 0;
    while (h->next) h = h->next;
    return h;
}

// Build list of n nodes with values 1..n
Node *build_list(int n) {
    Node *head = 0;
    Node *tail = 0;
    int i;
    for (i = 1; i <= n; i++) {
        Node *nd = new_node(i);
        if (!head) {
            head = nd;
            tail = nd;
        } else {
            tail->next = nd;
            tail = nd;
        }
    }
    return head;
}

// Reverse a linked list
Node *reverse_list(Node *h) {
    Node *prev = 0;
    Node *cur = h;
    Node *nxt;
    while (cur) {
        nxt = cur->next;
        cur->next = prev;
        prev = cur;
        cur = nxt;
    }
    return prev;
}

// Insertion sort on linked list
Node *sorted_insert(Node *sorted, Node *nd) {
    if (!sorted || nd->val <= sorted->val) {
        nd->next = sorted;
        return nd;
    }
    Node *cur = sorted;
    while (cur->next && cur->next->val < nd->val) {
        cur = cur->next;
    }
    nd->next = cur->next;
    cur->next = nd;
    return sorted;
}

Node *insertion_sort(Node *h) {
    Node *sorted = 0;
    Node *cur = h;
    while (cur) {
        Node *nxt = cur->next;
        sorted = sorted_insert(sorted, cur);
        cur = nxt;
    }
    return sorted;
}

// Merge two sorted lists
Node *merge_sorted_lists(Node *a, Node *b) {
    Node dummy;
    dummy.val = 0;
    dummy.next = 0;
    Node *tail = &dummy;
    while (a && b) {
        if (a->val <= b->val) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    if (a) tail->next = a;
    if (b) tail->next = b;
    return dummy.next;
}

// Check if list is sorted ascending
int is_sorted(Node *h) {
    if (!h) return 1;
    while (h->next) {
        if (h->val > h->next->val) return 0;
        h = h->next;
    }
    return 1;
}

// Remove duplicates from sorted list
Node *remove_duplicates(Node *h) {
    Node *cur = h;
    while (cur && cur->next) {
        if (cur->val == cur->next->val) {
            Node *dup = cur->next;
            cur->next = dup->next;
            free(dup);
        } else {
            cur = cur->next;
        }
    }
    return h;
}

// Splice list b after position pos in list a
Node *splice_after(Node *a, int pos, Node *b) {
    Node *cur = a;
    int i;
    for (i = 0; i < pos && cur; i++) {
        cur = cur->next;
    }
    if (!cur) return a;
    Node *b_tail = list_tail(b);
    if (b_tail) {
        b_tail->next = cur->next;
        cur->next = b;
    }
    return a;
}

// Compute checksum: sum of val*2 for all nodes
int checksum(Node *h) {
    int cs = 0;
    while (h) {
        cs += h->val * 2;
        h = h->next;
    }
    return cs;
}

void free_list(Node *h) {
    while (h) {
        Node *nxt = h->next;
        free(h);
        h = nxt;
    }
}

int main(void) {
    // Build list of 50 nodes: 1,2,3,...,50
    Node *list = build_list(50);
    printf("build_ok=%d\n", list_len(list));
    printf("sum_original=%d\n", list_sum(list));

    // Reverse: should be 50,49,...,1
    list = reverse_list(list);
    printf("reverse_head=%d\n", list->val);
    printf("reverse_tail=%d\n", list_tail(list)->val);

    // Sort the reversed list back
    list = insertion_sort(list);
    printf("sort_ok=%d\n", is_sorted(list));
    printf("sort_first=%d\n", list->val);
    printf("sort_last=%d\n", list_tail(list)->val);

    // Build second list 1..50, merge with first (now both sorted 1..50)
    Node *list2 = build_list(50);
    Node *merged = merge_sorted_lists(list, list2);
    printf("merge_count=%d\n", list_len(merged));
    printf("merge_sorted=%d\n", is_sorted(merged));

    // Remove duplicates from merged (1,1,2,2,...,50,50 -> 1,2,...,50)
    merged = remove_duplicates(merged);
    printf("dedup_count=%d\n", list_len(merged));

    // Build a small list 51..75 and splice after position 24
    Node *extra = build_list(25);
    // Shift values to 51..75
    Node *p = extra;
    while (p) { p->val += 50; p = p->next; }
    merged = splice_after(merged, 24, extra);
    printf("splice_count=%d\n", list_len(merged));

    // Compute checksum on a fresh list of 1..50
    Node *fresh = build_list(50);
    printf("checksum=%d\n", checksum(fresh));
    free_list(fresh);

    free_list(merged);
    return 0;
}
