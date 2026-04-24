// EXPECT: === Arrow Operator ===
// EXPECT: via pointer: x=10 y=20
// EXPECT: modified: x=30 y=40
// EXPECT: === Pass By Pointer ===
// EXPECT: scaled: x=20 y=60
// EXPECT: === Linked Nodes ===
// EXPECT: node chain: 1 -> 2 -> 3 -> END
// EXPECT: === Struct Pointer Array ===
// EXPECT: student 0: id=100 grade=90
// EXPECT: student 1: id=101 grade=85
// EXPECT: student 2: id=102 grade=95
// EXPECT: === Double Pointer ===
// EXPECT: via double ptr: x=77 y=88
// EXPECT: === Pointer to Nested ===
// EXPECT: inner via outer ptr: a=5 b=10
// EXPECT: === Self-Referential Walk ===
// EXPECT: walk: 10 20 30
// EXPECT: === Swap Structs ===
// EXPECT: after swap: a=(2,4) b=(1,3)

int printf(const char *fmt, ...);

struct Point {
    int x;
    int y;
};

struct Node {
    int value;
    struct Node *next;
};

struct Student {
    int id;
    int grade;
};

struct Inner {
    int a;
    int b;
};

struct Outer {
    struct Inner inner;
    int c;
};

void scale_point(struct Point *p, int factor) {
    p->x = p->x * factor;
    p->y = p->y * factor;
}

void swap_points(struct Point *a, struct Point *b) {
    struct Point tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void test_arrow(void) {
    printf("=== Arrow Operator ===\n");
    struct Point p;
    p.x = 10;
    p.y = 20;
    struct Point *pp = &p;
    printf("via pointer: x=%d y=%d\n", pp->x, pp->y);
    pp->x = 30;
    pp->y = 40;
    printf("modified: x=%d y=%d\n", p.x, p.y);
}

void test_pass_by_ptr(void) {
    printf("=== Pass By Pointer ===\n");
    struct Point p;
    p.x = 10;
    p.y = 30;
    scale_point(&p, 2);
    printf("scaled: x=%d y=%d\n", p.x, p.y);
}

void test_linked_nodes(void) {
    printf("=== Linked Nodes ===\n");
    struct Node n1;
    struct Node n2;
    struct Node n3;
    n1.value = 1;
    n2.value = 2;
    n3.value = 3;
    n1.next = &n2;
    n2.next = &n3;
    n3.next = 0;

    printf("node chain: ");
    struct Node *cur = &n1;
    while (cur) {
        printf("%d -> ", cur->value);
        cur = cur->next;
    }
    printf("END\n");
}

void test_struct_ptr_array(void) {
    printf("=== Struct Pointer Array ===\n");
    struct Student s0;
    s0.id = 100;
    s0.grade = 90;
    struct Student s1;
    s1.id = 101;
    s1.grade = 85;
    struct Student s2;
    s2.id = 102;
    s2.grade = 95;

    struct Student *arr[3];
    arr[0] = &s0;
    arr[1] = &s1;
    arr[2] = &s2;

    int i;
    for (i = 0; i < 3; i++) {
        printf("student %d: id=%d grade=%d\n", i, arr[i]->id, arr[i]->grade);
    }
}

void test_double_pointer(void) {
    printf("=== Double Pointer ===\n");
    struct Point p;
    p.x = 77;
    p.y = 88;
    struct Point *pp = &p;
    struct Point **ppp = &pp;
    printf("via double ptr: x=%d y=%d\n", (*ppp)->x, (*ppp)->y);
}

void test_pointer_to_nested(void) {
    printf("=== Pointer to Nested ===\n");
    struct Outer o;
    o.inner.a = 5;
    o.inner.b = 10;
    o.c = 15;
    struct Outer *op = &o;
    printf("inner via outer ptr: a=%d b=%d\n", op->inner.a, op->inner.b);
}

void test_self_ref_walk(void) {
    printf("=== Self-Referential Walk ===\n");
    struct Node a;
    struct Node b;
    struct Node c;
    a.value = 10;
    b.value = 20;
    c.value = 30;
    a.next = &b;
    b.next = &c;
    c.next = 0;

    printf("walk:");
    struct Node *n = &a;
    while (n) {
        printf(" %d", n->value);
        n = n->next;
    }
    printf("\n");
}

void test_swap(void) {
    printf("=== Swap Structs ===\n");
    struct Point a;
    a.x = 1;
    a.y = 3;
    struct Point b;
    b.x = 2;
    b.y = 4;
    swap_points(&a, &b);
    printf("after swap: a=(%d,%d) b=(%d,%d)\n", a.x, a.y, b.x, b.y);
}

int main(void) {
    test_arrow();
    test_pass_by_ptr();
    test_linked_nodes();
    test_struct_ptr_array();
    test_double_pointer();
    test_pointer_to_nested();
    test_self_ref_walk();
    test_swap();
    return 0;
}
