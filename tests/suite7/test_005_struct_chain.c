int printf(const char *fmt, ...);

struct IntNode {
    int val;
    struct IntNode *next;
};

struct StrNode {
    char name[16];
    int id;
    struct StrNode *next;
};

struct PairNode {
    int key;
    int value;
    struct PairNode *next;
};

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int int_list_sum(struct IntNode *head) {
    int total = 0;
    struct IntNode *cur = head;
    while (cur != 0) {
        total = total + cur->val;
        cur = cur->next;
    }
    return total;
}

int int_list_count(struct IntNode *head) {
    int c = 0;
    struct IntNode *cur = head;
    while (cur != 0) {
        c = c + 1;
        cur = cur->next;
    }
    return c;
}

int int_list_max(struct IntNode *head) {
    int mx = head->val;
    struct IntNode *cur = head->next;
    while (cur != 0) {
        if (cur->val > mx) mx = cur->val;
        cur = cur->next;
    }
    return mx;
}

int pair_list_find(struct PairNode *head, int key) {
    struct PairNode *cur = head;
    while (cur != 0) {
        if (cur->key == key) return cur->value;
        cur = cur->next;
    }
    return -1;
}

int pair_list_sum_values(struct PairNode *head) {
    int total = 0;
    struct PairNode *cur = head;
    while (cur != 0) {
        total = total + cur->value;
        cur = cur->next;
    }
    return total;
}

int main(void) {
    /* Integer linked list */
    struct IntNode n1, n2, n3, n4, n5;
    n1.val = 10; n1.next = &n2;
    n2.val = 20; n2.next = &n3;
    n3.val = 30; n3.next = &n4;
    n4.val = 40; n4.next = &n5;
    n5.val = 50; n5.next = 0;

    // EXPECT: int list sum=150
    printf("int list sum=%d\n", int_list_sum(&n1));
    // EXPECT: int list count=5
    printf("int list count=%d\n", int_list_count(&n1));
    // EXPECT: int list max=50
    printf("int list max=%d\n", int_list_max(&n1));

    /* partial list */
    // EXPECT: from n3 sum=120
    printf("from n3 sum=%d\n", int_list_sum(&n3));
    // EXPECT: from n3 count=3
    printf("from n3 count=%d\n", int_list_count(&n3));

    /* String nodes */
    struct StrNode s1, s2, s3;
    str_copy(s1.name, "alpha");
    s1.id = 1;
    s1.next = &s2;
    str_copy(s2.name, "beta");
    s2.id = 2;
    s2.next = &s3;
    str_copy(s3.name, "gamma");
    s3.id = 3;
    s3.next = 0;

    // EXPECT: s1: alpha id=1
    printf("s1: %s id=%d\n", s1.name, s1.id);
    // EXPECT: s2: beta id=2
    printf("s2: %s id=%d\n", s2.name, s2.id);
    // EXPECT: s3: gamma id=3
    printf("s3: %s id=%d\n", s3.name, s3.id);

    /* Walk string list counting ids */
    int id_sum = 0;
    struct StrNode *sc = &s1;
    while (sc != 0) {
        id_sum = id_sum + sc->id;
        sc = sc->next;
    }
    // EXPECT: id_sum=6
    printf("id_sum=%d\n", id_sum);

    /* Pair nodes - key/value lookup */
    struct PairNode p1, p2, p3, p4;
    p1.key = 100; p1.value = 10; p1.next = &p2;
    p2.key = 200; p2.value = 20; p2.next = &p3;
    p3.key = 300; p3.value = 30; p3.next = &p4;
    p4.key = 400; p4.value = 40; p4.next = 0;

    // EXPECT: find 200=20
    printf("find 200=%d\n", pair_list_find(&p1, 200));
    // EXPECT: find 400=40
    printf("find 400=%d\n", pair_list_find(&p1, 400));
    // EXPECT: find 999=-1
    printf("find 999=%d\n", pair_list_find(&p1, 999));
    // EXPECT: pair sum_values=100
    printf("pair sum_values=%d\n", pair_list_sum_values(&p1));

    /* Modify middle of chain */
    n3.val = 300;
    // EXPECT: modified sum=420
    printf("modified sum=%d\n", int_list_sum(&n1));

    /* Remove from chain by relinking */
    n2.next = &n4;
    // EXPECT: after remove count=4
    printf("after remove count=%d\n", int_list_count(&n1));
    // EXPECT: after remove sum=120
    printf("after remove sum=%d\n", int_list_sum(&n1));

    return 0;
}
