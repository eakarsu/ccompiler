int printf(const char *fmt, ...);

// Massive nested struct access (5 levels deep)

struct Level5 {
    int value;
    int data[3];
};

struct Level4 {
    struct Level5 l5;
    int tag;
};

struct Level3 {
    struct Level4 l4;
    int count;
};

struct Level2 {
    struct Level3 l3;
    int flag;
};

struct Level1 {
    struct Level2 l2;
    int id;
};

void init_level5(struct Level5 *p, int v) {
    p->value = v;
    p->data[0] = v + 1;
    p->data[1] = v + 2;
    p->data[2] = v + 3;
}

void init_level4(struct Level4 *p, int v) {
    init_level5(&p->l5, v);
    p->tag = v * 10;
}

void init_level3(struct Level3 *p, int v) {
    init_level4(&p->l4, v);
    p->count = v * 100;
}

void init_level2(struct Level2 *p, int v) {
    init_level3(&p->l3, v);
    p->flag = v * 1000;
}

void init_level1(struct Level1 *p, int v) {
    init_level2(&p->l2, v);
    p->id = v * 10000;
}

void print_deep_value(struct Level1 *p) {
    printf("%d\n", p->l2.l3.l4.l5.value); // EXPECT: 42
}

void print_deep_data(struct Level1 *p) {
    printf("%d\n", p->l2.l3.l4.l5.data[0]); // EXPECT: 43
    printf("%d\n", p->l2.l3.l4.l5.data[1]); // EXPECT: 44
    printf("%d\n", p->l2.l3.l4.l5.data[2]); // EXPECT: 45
}

void print_tags(struct Level1 *p) {
    printf("%d\n", p->l2.l3.l4.tag); // EXPECT: 420
    printf("%d\n", p->l2.l3.count);  // EXPECT: 4200
    printf("%d\n", p->l2.flag);      // EXPECT: 42000
    printf("%d\n", p->id);           // EXPECT: 420000
}

void modify_deep(struct Level1 *p, int newval) {
    p->l2.l3.l4.l5.value = newval;
    p->l2.l3.l4.l5.data[0] = newval + 10;
    p->l2.l3.l4.l5.data[1] = newval + 20;
    p->l2.l3.l4.l5.data[2] = newval + 30;
    p->l2.l3.l4.tag = newval * 2;
    p->l2.l3.count = newval * 3;
    p->l2.flag = newval * 4;
    p->id = newval * 5;
}

void print_modified(struct Level1 *p) {
    printf("%d\n", p->l2.l3.l4.l5.value);   // EXPECT: 99
    printf("%d\n", p->l2.l3.l4.l5.data[0]); // EXPECT: 109
    printf("%d\n", p->l2.l3.l4.l5.data[1]); // EXPECT: 119
    printf("%d\n", p->l2.l3.l4.l5.data[2]); // EXPECT: 129
    printf("%d\n", p->l2.l3.l4.tag);        // EXPECT: 198
    printf("%d\n", p->l2.l3.count);         // EXPECT: 297
    printf("%d\n", p->l2.flag);             // EXPECT: 396
    printf("%d\n", p->id);                  // EXPECT: 495
}

void test_array_of_nested(struct Level1 *arr, int n) {
    int i;
    int sum;
    sum = 0;
    for (i = 0; i < n; i = i + 1) {
        init_level1(&arr[i], i + 1);
        sum = sum + arr[i].l2.l3.l4.l5.value;
    }
    printf("%d\n", sum); // EXPECT: 15
}

void test_pointer_chain(struct Level1 *p) {
    struct Level2 *p2;
    struct Level3 *p3;
    struct Level4 *p4;
    struct Level5 *p5;
    p2 = &p->l2;
    p3 = &p2->l3;
    p4 = &p3->l4;
    p5 = &p4->l5;
    p5->value = 777;
    printf("%d\n", p->l2.l3.l4.l5.value); // EXPECT: 777
}

int main() {
    struct Level1 s;
    struct Level1 arr[5];

    init_level1(&s, 42);
    print_deep_value(&s);
    print_deep_data(&s);
    print_tags(&s);

    modify_deep(&s, 99);
    print_modified(&s);

    test_array_of_nested(arr, 5);
    test_pointer_chain(&s);

    printf("%s\n", "done"); // EXPECT: done
    return 0;
}
