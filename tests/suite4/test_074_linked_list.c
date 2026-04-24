int printf(const char *fmt, ...);

struct Node {
    int data;
    struct Node *next;
};

struct Node nodes[20];
int node_count;

struct Node *new_node(int val) {
    struct Node *n;
    n = &nodes[node_count];
    node_count = node_count + 1;
    n->data = val;
    n->next = 0;
    return n;
}

struct Node *push_front(struct Node *head, int val) {
    struct Node *n;
    n = new_node(val);
    n->next = head;
    return n;
}

struct Node *push_back(struct Node *head, int val) {
    struct Node *n;
    struct Node *cur;
    n = new_node(val);
    if (head == 0) {
        return n;
    }
    cur = head;
    while (cur->next != 0) {
        cur = cur->next;
    }
    cur->next = n;
    return head;
}

int list_length(struct Node *head) {
    int count;
    count = 0;
    while (head != 0) {
        count = count + 1;
        head = head->next;
    }
    return count;
}

int list_sum(struct Node *head) {
    int s;
    s = 0;
    while (head != 0) {
        s = s + head->data;
        head = head->next;
    }
    return s;
}

int list_find(struct Node *head, int val) {
    int idx;
    idx = 0;
    while (head != 0) {
        if (head->data == val) {
            return idx;
        }
        idx = idx + 1;
        head = head->next;
    }
    return -1;
}

int list_get(struct Node *head, int idx) {
    int i;
    for (i = 0; i < idx; i = i + 1) {
        head = head->next;
    }
    return head->data;
}

struct Node *list_reverse(struct Node *head) {
    struct Node *prev;
    struct Node *cur;
    struct Node *next;
    prev = 0;
    cur = head;
    while (cur != 0) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    return prev;
}

void print_list(struct Node *head) {
    int first;
    first = 1;
    while (head != 0) {
        if (first) {
            first = 0;
        } else {
            printf(" ");
        }
        printf("%d", head->data);
        head = head->next;
    }
}

int main(void) {
    struct Node *list;
    struct Node *list2;

    node_count = 0;
    list = 0;

    list = push_back(list, 10);
    list = push_back(list, 20);
    list = push_back(list, 30);

    // EXPECT: list: 10 20 30
    printf("list: ");
    print_list(list);
    printf("\n");

    // EXPECT: len: 3
    printf("len: %d\n", list_length(list));

    // EXPECT: sum: 60
    printf("sum: %d\n", list_sum(list));

    list = push_front(list, 5);
    // EXPECT: after push_front: 5 10 20 30
    printf("after push_front: ");
    print_list(list);
    printf("\n");

    // EXPECT: len: 4
    printf("len: %d\n", list_length(list));

    // EXPECT: find 20: 2
    printf("find 20: %d\n", list_find(list, 20));
    // EXPECT: find 5: 0
    printf("find 5: %d\n", list_find(list, 5));
    // EXPECT: find 30: 3
    printf("find 30: %d\n", list_find(list, 30));
    // EXPECT: find 99: -1
    printf("find 99: %d\n", list_find(list, 99));

    // EXPECT: get 0: 5
    printf("get 0: %d\n", list_get(list, 0));
    // EXPECT: get 2: 20
    printf("get 2: %d\n", list_get(list, 2));
    // EXPECT: get 3: 30
    printf("get 3: %d\n", list_get(list, 3));

    list = list_reverse(list);
    // EXPECT: reversed: 30 20 10 5
    printf("reversed: ");
    print_list(list);
    printf("\n");

    // EXPECT: rev head: 30
    printf("rev head: %d\n", list->data);

    list2 = 0;
    list2 = push_front(list2, 1);
    // EXPECT: single: 1
    printf("single: ");
    print_list(list2);
    printf("\n");

    // EXPECT: single len: 1
    printf("single len: %d\n", list_length(list2));

    list2 = list_reverse(list2);
    // EXPECT: single rev: 1
    printf("single rev: %d\n", list2->data);

    // EXPECT: done
    printf("done\n");

    return 0;
}
