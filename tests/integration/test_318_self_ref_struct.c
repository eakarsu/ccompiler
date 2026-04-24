int printf(const char *fmt, ...);
// EXPECT: list: 10 -> 20 -> 30 -> 40 -> 50\nlength: 5\nsum: 150\nmax: 50\ncontains 30: 1\ncontains 99: 0\nfound 30, next=40\nprepended: 5 -> 10 -> 20 -> 30 -> 40 -> 50\nnew length: 6\nnew sum: 155
// Test: self-referential struct (linked list node)

struct Node {
    int data;
    struct Node *next;
};

int list_length(struct Node *head) {
    int count = 0;
    struct Node *curr = head;
    while (curr) {
        count = count + 1;
        curr = curr->next;
    }
    return count;
}

int list_sum(struct Node *head) {
    int sum = 0;
    struct Node *curr = head;
    while (curr) {
        sum = sum + curr->data;
        curr = curr->next;
    }
    return sum;
}

int list_max(struct Node *head) {
    if (!head) return 0;
    int max = head->data;
    struct Node *curr = head->next;
    while (curr) {
        if (curr->data > max) max = curr->data;
        curr = curr->next;
    }
    return max;
}

int list_contains(struct Node *head, int val) {
    struct Node *curr = head;
    while (curr) {
        if (curr->data == val) return 1;
        curr = curr->next;
    }
    return 0;
}

struct Node *list_find(struct Node *head, int val) {
    struct Node *curr = head;
    while (curr) {
        if (curr->data == val) return curr;
        curr = curr->next;
    }
    return 0;
}

void list_print(struct Node *head) {
    struct Node *curr = head;
    while (curr) {
        printf("%d", curr->data);
        if (curr->next) printf(" -> ");
        curr = curr->next;
    }
    printf("\n");
}

int main(void) {
    struct Node nodes[6];
    nodes[0].data = 10; nodes[0].next = &nodes[1];
    nodes[1].data = 20; nodes[1].next = &nodes[2];
    nodes[2].data = 30; nodes[2].next = &nodes[3];
    nodes[3].data = 40; nodes[3].next = &nodes[4];
    nodes[4].data = 50; nodes[4].next = 0;

    struct Node *head = &nodes[0];

    printf("list: "); list_print(head);
    printf("length: %d\n", list_length(head));
    printf("sum: %d\n", list_sum(head));
    printf("max: %d\n", list_max(head));
    printf("contains 30: %d\n", list_contains(head, 30));
    printf("contains 99: %d\n", list_contains(head, 99));

    struct Node *found = list_find(head, 30);
    if (found) {
        printf("found 30, next=%d\n", found->next->data);
    }

    nodes[5].data = 5;
    nodes[5].next = head;
    head = &nodes[5];
    printf("prepended: "); list_print(head);
    printf("new length: %d\n", list_length(head));
    printf("new sum: %d\n", list_sum(head));

    return 0;
}
