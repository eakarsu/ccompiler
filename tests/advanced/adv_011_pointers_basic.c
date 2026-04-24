int printf(const char *fmt, ...);
// EXPECT: === Basic Pointer Tests ===
// EXPECT: addr_of: x has an address
// EXPECT: deref: *p=42
// EXPECT: modify_via_ptr: x=100
// EXPECT: ptr_to_ptr: **pp=55
// EXPECT: modify_via_pp: x=77
// EXPECT: null_ptr: p is null
// EXPECT: null_assign: p is null after assign
// EXPECT: ptr_compare_eq: ptrs equal
// EXPECT: ptr_compare_neq: ptrs not equal
// EXPECT: ptr_lt: p1 < p2
// EXPECT: void_ptr: val=123
// EXPECT: ptr_arith_inc: arr[1]=20
// EXPECT: ptr_arith_add: arr[3]=40
// EXPECT: ptr_arith_sub: arr[1]=20
// EXPECT: ptr_diff: diff=3
// EXPECT: ptr_array_walk: 10 20 30 40 50
// EXPECT: ptr_swap: a=200 b=100
// EXPECT: ptr_to_array: 1 2 3 4 5
// EXPECT: ptr_chain: val=999
// EXPECT: ptr_incr_loop: sum=150
// EXPECT: ptr_cast_char: first byte ok
// EXPECT: ptr_reassign: val=30
// EXPECT: ptr_self_ref: node val=42
// EXPECT: ptr_gt: p2 > p1
// EXPECT: ptr_ge: p1 >= p1
// EXPECT: ptr_le: p1 <= p2
// EXPECT: void_ptr_roundtrip: char_val=A
// EXPECT: ptr_to_local: v=88
// EXPECT: ptr_const_val: *cp=7
// EXPECT: ptr_multi_deref: final=12345
// EXPECT: === All Pointer Tests Passed ===

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int sum_via_ptr(int *arr, int n) {
    int s = 0;
    int *p = arr;
    int i;
    for (i = 0; i < n; i++) {
        s = s + *p;
        p = p + 1;
    }
    return s;
}

struct Node {
    int val;
    struct Node *next;
};

void set_via_ptr(int *dest, int value) {
    *dest = value;
}

int read_via_ptr(int *src) {
    return *src;
}

int main() {
    printf("=== Basic Pointer Tests ===\n");

    // address-of
    int x = 42;
    int *p = &x;
    if (p != 0) {
        printf("addr_of: x has an address\n");
    }

    // dereference
    printf("deref: *p=%d\n", *p);

    // modify through pointer
    *p = 100;
    printf("modify_via_ptr: x=%d\n", x);

    // pointer to pointer
    x = 55;
    int **pp = &p;
    printf("ptr_to_ptr: **pp=%d\n", **pp);

    // modify through pointer-to-pointer
    **pp = 77;
    printf("modify_via_pp: x=%d\n", x);

    // null pointer
    int *np = 0;
    if (np == 0) {
        printf("null_ptr: p is null\n");
    }

    // null assignment
    int *np2 = &x;
    np2 = 0;
    if (np2 == 0) {
        printf("null_assign: p is null after assign\n");
    }

    // pointer comparison - equal
    int *p1 = &x;
    int *p2 = &x;
    if (p1 == p2) {
        printf("ptr_compare_eq: ptrs equal\n");
    }

    // pointer comparison - not equal
    int y = 10;
    int *p3 = &y;
    if (p1 != p3) {
        printf("ptr_compare_neq: ptrs not equal\n");
    }

    // pointer less-than (array elements)
    int arr5[5];
    arr5[0] = 1; arr5[1] = 2; arr5[2] = 3; arr5[3] = 4; arr5[4] = 5;
    int *pa = &arr5[0];
    int *pb = &arr5[3];
    if (pa < pb) {
        printf("ptr_lt: p1 < p2\n");
    }

    // void pointer
    int vv = 123;
    void *vp = &vv;
    int *ip = (int *)vp;
    printf("void_ptr: val=%d\n", *ip);

    // pointer arithmetic - increment
    int arr[5];
    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 50;
    int *q = arr;
    q = q + 1;
    printf("ptr_arith_inc: arr[1]=%d\n", *q);

    // pointer arithmetic - add offset
    q = arr + 3;
    printf("ptr_arith_add: arr[3]=%d\n", *q);

    // pointer arithmetic - subtract offset
    q = arr + 3;
    q = q - 2;
    printf("ptr_arith_sub: arr[1]=%d\n", *q);

    // pointer difference
    int *start = &arr[1];
    int *end = &arr[4];
    long diff = end - start;
    printf("ptr_diff: diff=%ld\n", diff);

    // walk array with pointer
    printf("ptr_array_walk:");
    int *wp = arr;
    int i;
    for (i = 0; i < 5; i++) {
        printf(" %d", *(wp + i));
    }
    printf("\n");

    // swap via pointers
    int sa = 100;
    int sb = 200;
    swap(&sa, &sb);
    printf("ptr_swap: a=%d b=%d\n", sa, sb);

    // pointer to array iteration
    printf("ptr_to_array:");
    for (i = 0; i < 5; i++) {
        printf(" %d", arr5[i]);
    }
    printf("\n");

    // chain of pointers
    int val = 999;
    int *c1 = &val;
    int **c2 = &c1;
    int ***c3 = &c2;
    printf("ptr_chain: val=%d\n", ***c3);

    // pointer increment in loop to sum array
    int data[5];
    data[0] = 10; data[1] = 20; data[2] = 30; data[3] = 40; data[4] = 50;
    int s = sum_via_ptr(data, 5);
    printf("ptr_incr_loop: sum=%d\n", s);

    // cast pointer to char* and read first byte
    int big = 1;
    char *cp = (char *)&big;
    // On little-endian (ARM64), first byte should be 1
    if (*cp == 1) {
        printf("ptr_cast_char: first byte ok\n");
    }

    // pointer reassignment
    int ra = 10;
    int rb = 20;
    int rc = 30;
    int *rp = &ra;
    rp = &rb;
    rp = &rc;
    printf("ptr_reassign: val=%d\n", *rp);

    // self-referential struct pointer
    struct Node n;
    n.val = 42;
    n.next = 0;
    struct Node *nptr = &n;
    printf("ptr_self_ref: node val=%d\n", nptr->val);

    // pointer greater-than
    int *pg1 = &arr5[0];
    int *pg2 = &arr5[4];
    if (pg2 > pg1) {
        printf("ptr_gt: p2 > p1\n");
    }

    // pointer greater-equal (same pointer)
    if (pg1 >= pg1) {
        printf("ptr_ge: p1 >= p1\n");
    }

    // pointer less-equal
    if (pg1 <= pg2) {
        printf("ptr_le: p1 <= p2\n");
    }

    // void pointer roundtrip with char
    char ch = 'A';
    void *vp2 = &ch;
    char *cp2 = (char *)vp2;
    printf("void_ptr_roundtrip: char_val=%c\n", *cp2);

    // pointer to local variable via helper function
    int v = 0;
    set_via_ptr(&v, 88);
    printf("ptr_to_local: v=%d\n", v);

    // pointer initialized to address of const-like value
    int cval = 7;
    int *cptr = &cval;
    printf("ptr_const_val: *cp=%d\n", read_via_ptr(cptr));

    // multiple levels of dereference in expression
    int base = 12345;
    int *lv1 = &base;
    int **lv2 = &lv1;
    int ***lv3 = &lv2;
    int final = ***lv3;
    printf("ptr_multi_deref: final=%d\n", final);

    printf("=== All Pointer Tests Passed ===\n");
    return 0;
}
