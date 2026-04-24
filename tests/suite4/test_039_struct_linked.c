int printf(const char *fmt, ...);

// Test: Struct with pointer to same type (linked list), building and traversing

struct Node {
    int value;
    struct Node *next;
};

struct DNode {
    int data;
    struct DNode *prev;
    struct DNode *next;
};

int list_length(struct Node *head) {
    int count = 0;
    struct Node *cur = head;
    while (cur != 0) {
        count = count + 1;
        cur = cur->next;
    }
    return count;
}

int list_sum(struct Node *head) {
    int total = 0;
    struct Node *cur = head;
    while (cur != 0) {
        total = total + cur->value;
        cur = cur->next;
    }
    return total;
}

int list_max(struct Node *head) {
    int max = head->value;
    struct Node *cur = head->next;
    while (cur != 0) {
        if (cur->value > max) {
            max = cur->value;
        }
        cur = cur->next;
    }
    return max;
}

int list_min(struct Node *head) {
    int min = head->value;
    struct Node *cur = head->next;
    while (cur != 0) {
        if (cur->value < min) {
            min = cur->value;
        }
        cur = cur->next;
    }
    return min;
}

int list_contains(struct Node *head, int val) {
    struct Node *cur = head;
    while (cur != 0) {
        if (cur->value == val) {
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

int list_last(struct Node *head) {
    struct Node *cur = head;
    while (cur->next != 0) {
        cur = cur->next;
    }
    return cur->value;
}

int main(void) {
    // Build a linked list of 5 nodes on the stack
    struct Node n0;
    struct Node n1;
    struct Node n2;
    struct Node n3;
    struct Node n4;

    n0.value = 10;
    n1.value = 20;
    n2.value = 30;
    n3.value = 40;
    n4.value = 50;

    n0.next = &n1;
    n1.next = &n2;
    n2.next = &n3;
    n3.next = &n4;
    n4.next = 0;

    // Traverse and print each value
    struct Node *cur = &n0;
    // EXPECT: 10
    printf("%d\n", cur->value);
    cur = cur->next;
    // EXPECT: 20
    printf("%d\n", cur->value);
    cur = cur->next;
    // EXPECT: 30
    printf("%d\n", cur->value);
    cur = cur->next;
    // EXPECT: 40
    printf("%d\n", cur->value);
    cur = cur->next;
    // EXPECT: 50
    printf("%d\n", cur->value);

    // List length
    int len = list_length(&n0);
    // EXPECT: 5
    printf("%d\n", len);

    // List sum: 10+20+30+40+50 = 150
    int sum = list_sum(&n0);
    // EXPECT: 150
    printf("%d\n", sum);

    // List max
    int max = list_max(&n0);
    // EXPECT: 50
    printf("%d\n", max);

    // List min
    int min = list_min(&n0);
    // EXPECT: 10
    printf("%d\n", min);

    // Contains check
    int has30 = list_contains(&n0, 30);
    // EXPECT: 1
    printf("%d\n", has30);

    int has99 = list_contains(&n0, 99);
    // EXPECT: 0
    printf("%d\n", has99);

    // Last element
    int last = list_last(&n0);
    // EXPECT: 50
    printf("%d\n", last);

    // Modify a node in the middle
    n2.value = 300;
    int new_sum = list_sum(&n0);
    // 10+20+300+40+50 = 420
    // EXPECT: 420
    printf("%d\n", new_sum);

    // Shorter list: remove last two nodes
    n2.next = 0;
    int short_len = list_length(&n0);
    // EXPECT: 3
    printf("%d\n", short_len);

    int short_sum = list_sum(&n0);
    // 10+20+300 = 330
    // EXPECT: 330
    printf("%d\n", short_sum);

    // Doubly-linked list (3 nodes)
    struct DNode d0;
    struct DNode d1;
    struct DNode d2;
    d0.data = 100;
    d1.data = 200;
    d2.data = 300;
    d0.prev = 0;
    d0.next = &d1;
    d1.prev = &d0;
    d1.next = &d2;
    d2.prev = &d1;
    d2.next = 0;

    // Forward traversal sum
    int dsum = 0;
    struct DNode *dc = &d0;
    while (dc != 0) {
        dsum = dsum + dc->data;
        dc = dc->next;
    }
    // EXPECT: 600
    printf("%d\n", dsum);

    // Backward traversal from last
    // EXPECT: 300
    printf("%d\n", d2.data);
    // EXPECT: 200
    printf("%d\n", d2.prev->data);
    // EXPECT: 100
    printf("%d\n", d2.prev->prev->data);

    return 0;
}
