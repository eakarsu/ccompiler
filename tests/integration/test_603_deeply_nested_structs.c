int printf(const char *fmt, ...);
// EXPECT: === Test 1: Basic 4-level nesting ===\nL4: c=A val=42\nL3: l4.c=A l4.val=42 x=100 arr=XYZ\nL2: l3.l4.val=42 y=200 ch=B\nL1: l2.l3.l4.val=42 z=300\nDeep access: 42\n=== Test 2: Pointer to nested struct ===\nBefore: val=42 z=300\nAfter modify: val=999 z=888\nDeep via ptr: 999\n=== Test 3: Array of nested structs ===\nelem[0]: l2.l3.l4.val=10 z=1\nelem[1]: l2.l3.l4.val=20 z=2\nelem[2]: l2.l3.l4.val=30 z=3\nSum of deep vals: 60\n=== Test 4: Mixed alignment stress ===\nA4: c1=a i1=111 c2=b i2=222\nA3: a4.c1=a a4.i1=111 c3=c i3=333\nA2: a3.a4.i1=111 a3.i3=333 c4=d i4=444\nA1: a2.a3.a4.i1=111 a2.i4=444 c5=e i5=555\n=== Test 5: Modify deep fields via function ===\nBefore: l2.l3.l4.val=42 l2.l3.l4.c=A l2.l3.x=100\nAfter: l2.l3.l4.val=84 l2.l3.l4.c=Z l2.l3.x=200\n=== Test 6: Return small nested struct ===\nReturned: s2.s3.s4.a=10 s2.s3.s4.b=20 extra=30\n=== Test 7: Array of structs at inner level ===\nouter.mid.items[0].id=100\nouter.mid.items[1].id=200\nouter.mid.items[2].id=300\nouter.mid.count=3\nouter.tag=42\nSum of items: 600\n=== Test 8: Copy deeply nested struct ===\nOriginal: l2.l3.l4.val=42 z=300\nCopy: l2.l3.l4.val=42 z=300\nAfter modify original: orig.val=999 copy.val=42\n=== Test 9: Sizeof verification ===\nsizeof_L4_ok=1\nsizeof_L3_ok=1\nsizeof_L2_ok=1\nsizeof_L1_ok=1\n=== Test 10: Nested struct computation ===\nComputed sum=1155\n=== Test 11: Deep pointer chain ===\np1->l2.l3.l4.val=42\np2 l3.l4.val=42\np3 l4.val=42\np4 val=42\nAll tests passed!\n

// Test 603: Deeply nested structs - stress test

struct L4 {
    char c;
    int val;
};

struct L3 {
    struct L4 l4;
    int x;
    char arr[3];
};

struct L2 {
    struct L3 l3;
    int y;
    char ch;
};

struct L1 {
    struct L2 l2;
    int z;
};
struct A4 {
    char c1;
    int i1;
    char c2;
    int i2;
};

struct A3 {
    struct A4 a4;
    char c3;
    int i3;
};

struct A2 {
    struct A3 a3;
    char c4;
    int i4;
};

struct A1 {
    struct A2 a2;
    char c5;
    int i5;
};

struct Small4 {
    int a;
    int b;
};

struct Small3 {
    struct Small4 s4;
};

struct Small2 {
    struct Small3 s3;
};

struct Small1 {
    struct Small2 s2;
    int extra;
};

struct Inner {
    int id;
};

struct Mid {
    struct Inner items[3];
    int count;
};

struct Outer {
    struct Mid mid;
    int tag;
};

void modify_deep(struct L1 *p) {
    p->l2.l3.l4.val = p->l2.l3.l4.val * 2;
    p->l2.l3.l4.c = 'Z';
    p->l2.l3.x = p->l2.l3.x * 2;
}

// Return a small nested struct by value
struct Small1 make_small(int a, int b, int e) {
    struct Small1 s;
    s.s2.s3.s4.a = a;
    s.s2.s3.s4.b = b;
    s.extra = e;
    return s;
}

// Compute sum of all deep fields in an L1
int sum_all_fields(struct L1 *p) {
    int sum = 0;
    sum = sum + p->l2.l3.l4.c;
    sum = sum + p->l2.l3.l4.val;
    sum = sum + p->l2.l3.x;
    sum = sum + p->l2.l3.arr[0];
    sum = sum + p->l2.l3.arr[1];
    sum = sum + p->l2.l3.arr[2];
    sum = sum + p->l2.y;
    sum = sum + p->l2.ch;
    sum = sum + p->z;
    return sum;
}

// ============================================================
// Test functions
// ============================================================

void test1_basic_nesting(void) {
    struct L1 obj;
    obj.l2.l3.l4.c = 'A';
    obj.l2.l3.l4.val = 42;
    obj.l2.l3.x = 100;
    obj.l2.l3.arr[0] = 'X';
    obj.l2.l3.arr[1] = 'Y';
    obj.l2.l3.arr[2] = 'Z';
    obj.l2.y = 200;
    obj.l2.ch = 'B';
    obj.z = 300;

    printf("=== Test 1: Basic 4-level nesting ===\n");
    printf("L4: c=%c val=%d\n", obj.l2.l3.l4.c, obj.l2.l3.l4.val);
    printf("L3: l4.c=%c l4.val=%d x=%d arr=%c%c%c\n",
           obj.l2.l3.l4.c, obj.l2.l3.l4.val, obj.l2.l3.x,
           obj.l2.l3.arr[0], obj.l2.l3.arr[1], obj.l2.l3.arr[2]);
    printf("L2: l3.l4.val=%d y=%d ch=%c\n",
           obj.l2.l3.l4.val, obj.l2.y, obj.l2.ch);
    printf("L1: l2.l3.l4.val=%d z=%d\n", obj.l2.l3.l4.val, obj.z);

    // Deep access chain
    int deep = obj.l2.l3.l4.val;
    printf("Deep access: %d\n", deep);
}

void test2_pointer_access(void) {
    struct L1 obj;
    obj.l2.l3.l4.c = 'A';
    obj.l2.l3.l4.val = 42;
    obj.l2.l3.x = 100;
    obj.l2.l3.arr[0] = 'X';
    obj.l2.l3.arr[1] = 'Y';
    obj.l2.l3.arr[2] = 'Z';
    obj.l2.y = 200;
    obj.l2.ch = 'B';
    obj.z = 300;

    struct L1 *ptr = &obj;

    printf("=== Test 2: Pointer to nested struct ===\n");
    printf("Before: val=%d z=%d\n", ptr->l2.l3.l4.val, ptr->z);

    ptr->l2.l3.l4.val = 999;
    ptr->z = 888;
    printf("After modify: val=%d z=%d\n", ptr->l2.l3.l4.val, ptr->z);
    printf("Deep via ptr: %d\n", ptr->l2.l3.l4.val);
}

void test3_array_of_nested(void) {
    struct L1 arr[3];
    int i;

    printf("=== Test 3: Array of nested structs ===\n");

    for (i = 0; i < 3; i = i + 1) {
        arr[i].l2.l3.l4.val = (i + 1) * 10;
        arr[i].l2.l3.l4.c = 'A';
        arr[i].l2.l3.x = 0;
        arr[i].l2.l3.arr[0] = 0;
        arr[i].l2.l3.arr[1] = 0;
        arr[i].l2.l3.arr[2] = 0;
        arr[i].l2.y = 0;
        arr[i].l2.ch = 0;
        arr[i].z = i + 1;
    }

    for (i = 0; i < 3; i = i + 1) {
        printf("elem[%d]: l2.l3.l4.val=%d z=%d\n",
               i, arr[i].l2.l3.l4.val, arr[i].z);
    }

    int sum = arr[0].l2.l3.l4.val + arr[1].l2.l3.l4.val + arr[2].l2.l3.l4.val;
    printf("Sum of deep vals: %d\n", sum);
}

void test4_alignment_stress(void) {
    struct A1 obj;
    obj.a2.a3.a4.c1 = 'a';
    obj.a2.a3.a4.i1 = 111;
    obj.a2.a3.a4.c2 = 'b';
    obj.a2.a3.a4.i2 = 222;
    obj.a2.a3.c3 = 'c';
    obj.a2.a3.i3 = 333;
    obj.a2.c4 = 'd';
    obj.a2.i4 = 444;
    obj.c5 = 'e';
    obj.i5 = 555;

    printf("=== Test 4: Mixed alignment stress ===\n");
    printf("A4: c1=%c i1=%d c2=%c i2=%d\n",
           obj.a2.a3.a4.c1, obj.a2.a3.a4.i1,
           obj.a2.a3.a4.c2, obj.a2.a3.a4.i2);
    printf("A3: a4.c1=%c a4.i1=%d c3=%c i3=%d\n",
           obj.a2.a3.a4.c1, obj.a2.a3.a4.i1,
           obj.a2.a3.c3, obj.a2.a3.i3);
    printf("A2: a3.a4.i1=%d a3.i3=%d c4=%c i4=%d\n",
           obj.a2.a3.a4.i1, obj.a2.a3.i3,
           obj.a2.c4, obj.a2.i4);
    printf("A1: a2.a3.a4.i1=%d a2.i4=%d c5=%c i5=%d\n",
           obj.a2.a3.a4.i1, obj.a2.i4,
           obj.c5, obj.i5);
}

void test5_modify_via_function(void) {
    struct L1 obj;
    obj.l2.l3.l4.c = 'A';
    obj.l2.l3.l4.val = 42;
    obj.l2.l3.x = 100;
    obj.l2.l3.arr[0] = 0;
    obj.l2.l3.arr[1] = 0;
    obj.l2.l3.arr[2] = 0;
    obj.l2.y = 200;
    obj.l2.ch = 'B';
    obj.z = 300;

    printf("=== Test 5: Modify deep fields via function ===\n");
    printf("Before: l2.l3.l4.val=%d l2.l3.l4.c=%c l2.l3.x=%d\n",
           obj.l2.l3.l4.val, obj.l2.l3.l4.c, obj.l2.l3.x);

    modify_deep(&obj);

    printf("After: l2.l3.l4.val=%d l2.l3.l4.c=%c l2.l3.x=%d\n",
           obj.l2.l3.l4.val, obj.l2.l3.l4.c, obj.l2.l3.x);
}

void test6_return_nested(void) {
    struct Small1 s = make_small(10, 20, 30);

    printf("=== Test 6: Return small nested struct ===\n");
    printf("Returned: s2.s3.s4.a=%d s2.s3.s4.b=%d extra=%d\n",
           s.s2.s3.s4.a, s.s2.s3.s4.b, s.extra);
}

void test7_inner_array(void) {
    struct Outer outer;
    outer.mid.items[0].id = 100;
    outer.mid.items[1].id = 200;
    outer.mid.items[2].id = 300;
    outer.mid.count = 3;
    outer.tag = 42;

    printf("=== Test 7: Array of structs at inner level ===\n");
    printf("outer.mid.items[0].id=%d\n", outer.mid.items[0].id);
    printf("outer.mid.items[1].id=%d\n", outer.mid.items[1].id);
    printf("outer.mid.items[2].id=%d\n", outer.mid.items[2].id);
    printf("outer.mid.count=%d\n", outer.mid.count);
    printf("outer.tag=%d\n", outer.tag);

    int sum = outer.mid.items[0].id + outer.mid.items[1].id + outer.mid.items[2].id;
    printf("Sum of items: %d\n", sum);
}

void test8_copy_nested(void) {
    struct L1 orig;
    orig.l2.l3.l4.c = 'A';
    orig.l2.l3.l4.val = 42;
    orig.l2.l3.x = 100;
    orig.l2.l3.arr[0] = 'X';
    orig.l2.l3.arr[1] = 'Y';
    orig.l2.l3.arr[2] = 'Z';
    orig.l2.y = 200;
    orig.l2.ch = 'B';
    orig.z = 300;

    // Copy by assignment
    struct L1 copy;
    copy = orig;

    printf("=== Test 8: Copy deeply nested struct ===\n");
    printf("Original: l2.l3.l4.val=%d z=%d\n", orig.l2.l3.l4.val, orig.z);
    printf("Copy: l2.l3.l4.val=%d z=%d\n", copy.l2.l3.l4.val, copy.z);

    // Modify original, verify copy is independent
    orig.l2.l3.l4.val = 999;
    printf("After modify original: orig.val=%d copy.val=%d\n",
           orig.l2.l3.l4.val, copy.l2.l3.l4.val);
}

void test9_sizeof(void) {
    // Manually verify sizeof at each level
    // L4: char(1) + pad(3) + int(4) = 8
    // L3: L4(8) + int(4) + char[3](3) + pad(1) = 16
    // L2: L3(16) + int(4) + char(1) + pad(3) = 24
    // L1: L2(24) + int(4) = 28

    int ok_l4 = (sizeof(struct L4) == 8);
    int ok_l3 = (sizeof(struct L3) == 16);
    int ok_l2 = (sizeof(struct L2) == 24);
    int ok_l1 = (sizeof(struct L1) == 28);

    printf("=== Test 9: Sizeof verification ===\n");
    printf("sizeof_L4_ok=%d\n", ok_l4);
    printf("sizeof_L3_ok=%d\n", ok_l3);
    printf("sizeof_L2_ok=%d\n", ok_l2);
    printf("sizeof_L1_ok=%d\n", ok_l1);
}

void test10_computation(void) {
    // Build an L1, then compute sum of all fields
    struct L1 obj;
    obj.l2.l3.l4.c = 'A';     // 65
    obj.l2.l3.l4.val = 42;
    obj.l2.l3.x = 100;
    obj.l2.l3.arr[0] = 'X';   // 88
    obj.l2.l3.arr[1] = 'Y';   // 89
    obj.l2.l3.arr[2] = 'Z';   // 90
    obj.l2.y = 200;
    obj.l2.ch = 'B';           // 66
    obj.z = 300;
    // sum = 65 + 42 + 100 + 88 + 89 + 90 + 200 + 66 + 300 = 1040

    // Also add alignment struct fields
    struct A1 a;
    a.a2.a3.a4.i1 = 10;
    a.a2.a3.i3 = 20;
    a.a2.i4 = 30;
    a.i5 = 55;
    // partial sum from A1 = 10 + 20 + 30 + 55 = 115

    int s1 = sum_all_fields(&obj);
    int s2 = a.a2.a3.a4.i1 + a.a2.a3.i3 + a.a2.i4 + a.i5;
    int total = s1 + s2;

    printf("=== Test 10: Nested struct computation ===\n");
    printf("Computed sum=%d\n", total);
}

void test11_deep_pointer_chain(void) {
    struct L1 obj;
    obj.l2.l3.l4.c = 'A';
    obj.l2.l3.l4.val = 42;
    obj.l2.l3.x = 100;
    obj.l2.l3.arr[0] = 0;
    obj.l2.l3.arr[1] = 0;
    obj.l2.l3.arr[2] = 0;
    obj.l2.y = 200;
    obj.l2.ch = 'B';
    obj.z = 300;

    // Pointers at each nesting level
    struct L1 *p1 = &obj;
    struct L2 *p2 = &obj.l2;
    struct L3 *p3 = &obj.l2.l3;
    struct L4 *p4 = &obj.l2.l3.l4;

    printf("=== Test 11: Deep pointer chain ===\n");
    printf("p1->l2.l3.l4.val=%d\n", p1->l2.l3.l4.val);
    printf("p2 l3.l4.val=%d\n", p2->l3.l4.val);
    printf("p3 l4.val=%d\n", p3->l4.val);
    printf("p4 val=%d\n", p4->val);
}

int main(void) {
    test1_basic_nesting();
    test2_pointer_access();
    test3_array_of_nested();
    test4_alignment_stress();
    test5_modify_via_function();
    test6_return_nested();
    test7_inner_array();
    test8_copy_nested();
    test9_sizeof();
    test10_computation();
    test11_deep_pointer_chain();
    printf("All tests passed!\n");
    return 0;
}
