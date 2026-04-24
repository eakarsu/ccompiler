// EXPECT: === Create & Print ===
// EXPECT: list: 10
// EXPECT: length: 1
// EXPECT: === Append ===
// EXPECT: list: 10 -> 20 -> 30
// EXPECT: length: 3
// EXPECT: === Prepend ===
// EXPECT: list: 5 -> 10 -> 20 -> 30
// EXPECT: length: 4
// EXPECT: === Search ===
// EXPECT: search 20: found
// EXPECT: search 99: not found
// EXPECT: === Delete Head ===
// EXPECT: list: 10 -> 20 -> 30
// EXPECT: length: 3
// EXPECT: === Delete Middle ===
// EXPECT: list: 10 -> 30
// EXPECT: length: 2
// EXPECT: === Delete Tail ===
// EXPECT: list: 10
// EXPECT: length: 1
// EXPECT: === Reverse ===
// EXPECT: list: 30 -> 20 -> 10
// EXPECT: === Free All ===
// EXPECT: freed 3 nodes

int printf(const char *fmt, ...);
void *malloc(long size);
void free(void *ptr);

struct Node {
    int data;
    struct Node *next;
};

struct Node *create_node(int data) {
    struct Node *n = (struct Node *)malloc(sizeof(struct Node));
    n->data = data;
    n->next = 0;
    return n;
}

void append(struct Node **head, int data) {
    struct Node *new_node = create_node(data);
    if (*head == 0) {
        *head = new_node;
        return;
    }
    struct Node *cur = *head;
    while (cur->next != 0) {
        cur = cur->next;
    }
    cur->next = new_node;
}

void prepend(struct Node **head, int data) {
    struct Node *new_node = create_node(data);
    new_node->next = *head;
    *head = new_node;
}

void print_list(struct Node *head) {
    printf("list:");
    struct Node *cur = head;
    int first = 1;
    while (cur != 0) {
        if (first) {
            printf(" %d", cur->data);
            first = 0;
        } else {
            printf(" -> %d", cur->data);
        }
        cur = cur->next;
    }
    printf("\n");
}

int length(struct Node *head) {
    int count = 0;
    struct Node *cur = head;
    while (cur != 0) {
        count = count + 1;
        cur = cur->next;
    }
    return count;
}

int search(struct Node *head, int value) {
    struct Node *cur = head;
    while (cur != 0) {
        if (cur->data == value) return 1;
        cur = cur->next;
    }
    return 0;
}

void delete_node(struct Node **head, int value) {
    if (*head == 0) return;
    if ((*head)->data == value) {
        struct Node *tmp = *head;
        *head = (*head)->next;
        free(tmp);
        return;
    }
    struct Node *cur = *head;
    while (cur->next != 0) {
        if (cur->next->data == value) {
            struct Node *tmp = cur->next;
            cur->next = cur->next->next;
            free(tmp);
            return;
        }
        cur = cur->next;
    }
}

void reverse(struct Node **head) {
    struct Node *prev = 0;
    struct Node *cur = *head;
    struct Node *next;
    while (cur != 0) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    *head = prev;
}

int free_all(struct Node **head) {
    int count = 0;
    struct Node *cur = *head;
    while (cur != 0) {
        struct Node *tmp = cur;
        cur = cur->next;
        free(tmp);
        count = count + 1;
    }
    *head = 0;
    return count;
}

void test_create(void) {
    printf("=== Create & Print ===\n");
    struct Node *list = 0;
    append(&list, 10);
    print_list(list);
    printf("length: %d\n", length(list));
    free_all(&list);
}

void test_append(void) {
    printf("=== Append ===\n");
    struct Node *list = 0;
    append(&list, 10);
    append(&list, 20);
    append(&list, 30);
    print_list(list);
    printf("length: %d\n", length(list));
    free_all(&list);
}

void test_prepend(void) {
    printf("=== Prepend ===\n");
    struct Node *list = 0;
    append(&list, 10);
    append(&list, 20);
    append(&list, 30);
    prepend(&list, 5);
    print_list(list);
    printf("length: %d\n", length(list));
    free_all(&list);
}

void test_search(void) {
    printf("=== Search ===\n");
    struct Node *list = 0;
    append(&list, 10);
    append(&list, 20);
    append(&list, 30);
    if (search(list, 20)) printf("search 20: found\n");
    else printf("search 20: not found\n");
    if (search(list, 99)) printf("search 99: found\n");
    else printf("search 99: not found\n");
    free_all(&list);
}

void test_delete_head(void) {
    printf("=== Delete Head ===\n");
    struct Node *list = 0;
    append(&list, 5);
    append(&list, 10);
    append(&list, 20);
    append(&list, 30);
    delete_node(&list, 5);
    print_list(list);
    printf("length: %d\n", length(list));
    free_all(&list);
}

void test_delete_middle(void) {
    printf("=== Delete Middle ===\n");
    struct Node *list = 0;
    append(&list, 10);
    append(&list, 20);
    append(&list, 30);
    delete_node(&list, 20);
    print_list(list);
    printf("length: %d\n", length(list));
    free_all(&list);
}

void test_delete_tail(void) {
    printf("=== Delete Tail ===\n");
    struct Node *list = 0;
    append(&list, 10);
    append(&list, 30);
    delete_node(&list, 30);
    print_list(list);
    printf("length: %d\n", length(list));
    free_all(&list);
}

void test_reverse(void) {
    printf("=== Reverse ===\n");
    struct Node *list = 0;
    append(&list, 10);
    append(&list, 20);
    append(&list, 30);
    reverse(&list);
    print_list(list);
    free_all(&list);
}

void test_free_all(void) {
    printf("=== Free All ===\n");
    struct Node *list = 0;
    append(&list, 1);
    append(&list, 2);
    append(&list, 3);
    int n = free_all(&list);
    printf("freed %d nodes\n", n);
}

int main(void) {
    test_create();
    test_append();
    test_prepend();
    test_search();
    test_delete_head();
    test_delete_middle();
    test_delete_tail();
    test_reverse();
    test_free_all();
    return 0;
}
