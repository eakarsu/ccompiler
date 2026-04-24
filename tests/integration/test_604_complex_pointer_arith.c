int printf(const char *fmt, ...);
// EXPECT: === Pointer Subtraction (element count) ===\np - q = 3\nq - p = -3\nend - start = 8\n=== Pointer Addition Strides ===\nchar stride: bytes between [0] and [1] = 1\nint stride: bytes between [0] and [1] = 4\nchar values: 10 20 30 40 50\nint values: 100 200 300 400 500\n=== Multi-level Indirection ===\nval = 42\n*ptr = 42\n**pp = 42\nModified through **pp: 99\n=== Triple Pointer ===\nbase = 77\n*p1 = 77\n**p2 = 77\n***p3 = 77\nSet via ***p3: 55\nVerify: base = 55\n=== *(arr+i) vs arr[i] ===\narr[0]=10 *(arr+0)=10 match=1\narr[1]=21 *(arr+1)=21 match=1\narr[2]=32 *(arr+2)=32 match=1\narr[3]=43 *(arr+3)=43 match=1\narr[4]=54 *(arr+4)=54 match=1\narr[5]=65 *(arr+5)=65 match=1\nAll matched: 1\n=== Negative Pointer Arithmetic ===\nmid points to arr[4] = 50\nmid[-1] = 40\nmid[-2] = 30\nmid[-3] = 20\nmid[-4] = 10\n*(mid-1) = 40\n*(mid-2) = 30\n*(mid-3) = 20\n=== Pointer Comparison ===\np < q: 1\np == q: 0\np != q: 1\np > q: 0\nAfter moving p forward:\np < q: 0\np == q: 1\np != q: 0\nAfter p passes q:\np < q: 0\np > q: 1\n=== Array of Pointers ===\n*ptrs[0] = 10\n*ptrs[1] = 20\n*ptrs[2] = 30\n*ptrs[3] = 40\n*ptrs[4] = 50\nSum via ptr array = 150\n=== Pointer to Struct ===\nDirect: x=10 y=20 z=30\nPtr:    x=10 y=20 z=30\nModify via ptr: x=100 y=200 z=300\nArray of structs via ptr:\n  [0] x=1 y=2\n  [1] x=3 y=4\n  [2] x=5 y=6\n  [3] x=7 y=8\n=== Pointer Cast (int* as char*) ===\nOriginal int: 0x04030201\nbyte[0] = 1\nbyte[1] = 2\nbyte[2] = 3\nbyte[3] = 4\n=== Function Pointers ===\nadd(3, 4) = 7\nsub(10, 3) = 7\nmul(3, 4) = 12\nOps via function pointer array:\n  op[0](10, 5) = 15\n  op[1](10, 5) = 5\n  op[2](10, 5) = 50\n=== Pointer Walk Through Array ===\nArray: 2 4 6 8 10 12 14 16 18 20\nForward walk: 2 4 6 8 10 12 14 16 18 20\nSum = 110\nReverse walk: 20 18 16 14 12 10 8 6 4 2\n=== Pointer Difference in Loop ===\nSearch 6: found at index 2\nSearch 14: found at index 6\nSearch 99: not found\n=== Nested Struct Pointer Access ===\nouter.inner.val = 42\nop->ip->val = 42\nModified: op->ip->val = 88\n=== Pointer Arithmetic Summary ===\nAll 12 sections passed.

struct Vec3 {
    int x;
    int y;
    int z;
};

struct Point2D {
    int x;
    int y;
};

struct Inner {
    int val;
};

struct Outer {
    struct Inner inner;
    int extra;
};

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }

// Section 1: Pointer subtraction returning element count
void test_ptr_subtraction(void) {
    int a[8];
    int i;
    for (i = 0; i < 8; i++) a[i] = i * 10;

    int *p = &a[5];
    int *q = &a[2];
    int diff1 = p - q;
    int diff2 = q - p;

    int *start = &a[0];
    int *end = &a[8];
    int diff3 = end - start;

    printf("=== Pointer Subtraction (element count) ===\n");
    printf("p - q = %d\n", diff1);
    printf("q - p = %d\n", diff2);
    printf("end - start = %d\n", diff3);
}

// Section 2: Pointer addition with different strides
void test_ptr_strides(void) {
    char ca[5];
    int ia[5];
    int i;

    ca[0] = 10; ca[1] = 20; ca[2] = 30; ca[3] = 40; ca[4] = 50;
    ia[0] = 100; ia[1] = 200; ia[2] = 300; ia[3] = 400; ia[4] = 500;

    char *cp0 = &ca[0];
    char *cp1 = &ca[1];
    int char_stride = (int)(cp1 - cp0);

    char *ip0 = (char *)&ia[0];
    char *ip1 = (char *)&ia[1];
    int int_stride = (int)(ip1 - ip0);

    printf("=== Pointer Addition Strides ===\n");
    printf("char stride: bytes between [0] and [1] = %d\n", char_stride);
    printf("int stride: bytes between [0] and [1] = %d\n", int_stride);

    printf("char values:");
    for (i = 0; i < 5; i++) {
        printf(" %d", *(cp0 + i));
    }
    printf("\n");

    printf("int values:");
    int *iap = ia;
    for (i = 0; i < 5; i++) {
        printf(" %d", *(iap + i));
    }
    printf("\n");
}

// Section 3: Multi-level indirection
void test_multi_level(void) {
    int val = 42;
    int *ptr = &val;
    int **pp = &ptr;

    printf("=== Multi-level Indirection ===\n");
    printf("val = %d\n", val);
    printf("*ptr = %d\n", *ptr);
    printf("**pp = %d\n", **pp);

    **pp = 99;
    printf("Modified through **pp: %d\n", val);
}

// Section 4: Triple pointer
void test_triple_pointer(void) {
    int base = 77;
    int *p1 = &base;
    int **p2 = &p1;
    int ***p3 = &p2;

    printf("=== Triple Pointer ===\n");
    printf("base = %d\n", base);
    printf("*p1 = %d\n", *p1);
    printf("**p2 = %d\n", **p2);
    printf("***p3 = %d\n", ***p3);

    ***p3 = 55;
    printf("Set via ***p3: %d\n", base);
    printf("Verify: base = %d\n", base);
}

// Section 5: *(arr + i) vs arr[i]
void test_deref_vs_index(void) {
    int arr[6];
    int i;
    for (i = 0; i < 6; i++) arr[i] = 10 + i * 11;
    printf("=== *(arr+i) vs arr[i] ===\n");
    int all_match = 1;
    for (i = 0; i < 6; i++) {
        int v1 = arr[i];
        int v2 = *(arr + i);
        int match = (v1 == v2);
        printf("arr[%d]=%d *(arr+%d)=%d match=%d\n", i, v1, i, v2, match);
        if (!match) all_match = 0;
    }
    printf("All matched: %d\n", all_match);
}

// Section 6: Negative pointer arithmetic
void test_negative_arith(void) {
    int arr[8];
    int i;
    for (i = 0; i < 8; i++) arr[i] = (i + 1) * 10;
    int *mid = &arr[4];

    printf("=== Negative Pointer Arithmetic ===\n");
    printf("mid points to arr[4] = %d\n", *mid);
    printf("mid[-1] = %d\n", mid[-1]);
    printf("mid[-2] = %d\n", mid[-2]);
    printf("mid[-3] = %d\n", mid[-3]);
    printf("mid[-4] = %d\n", mid[-4]);
    printf("*(mid-1) = %d\n", *(mid - 1));
    printf("*(mid-2) = %d\n", *(mid - 2));
    printf("*(mid-3) = %d\n", *(mid - 3));
}

// Section 7: Pointer comparison
void test_ptr_comparison(void) {
    int arr[10];
    int i;
    for (i = 0; i < 10; i++) arr[i] = i;

    int *p = &arr[2];
    int *q = &arr[7];

    printf("=== Pointer Comparison ===\n");
    printf("p < q: %d\n", p < q);
    printf("p == q: %d\n", p == q);
    printf("p != q: %d\n", p != q);
    printf("p > q: %d\n", p > q);

    // Move p forward to match q
    p = &arr[7];
    printf("After moving p forward:\n");
    printf("p < q: %d\n", p < q);
    printf("p == q: %d\n", p == q);
    printf("p != q: %d\n", p != q);

    // Move p past q
    p = &arr[9];
    printf("After p passes q:\n");
    printf("p < q: %d\n", p < q);
    printf("p > q: %d\n", p > q);
}

// Section 8: Array of pointers
void test_array_of_pointers(void) {
    int a = 10;
    int b = 20;
    int c = 30;
    int d = 40;
    int e = 50;

    int *ptrs[5];
    ptrs[0] = &a;
    ptrs[1] = &b;
    ptrs[2] = &c;
    ptrs[3] = &d;
    ptrs[4] = &e;

    printf("=== Array of Pointers ===\n");
    int sum = 0;
    int i;
    for (i = 0; i < 5; i++) {
        printf("*ptrs[%d] = %d\n", i, *ptrs[i]);
        sum = sum + *ptrs[i];
    }
    printf("Sum via ptr array = %d\n", sum);
}

// Section 9: Pointer to struct
void test_ptr_to_struct(void) {
    struct Vec3 v;
    v.x = 10;
    v.y = 20;
    v.z = 30;

    struct Vec3 *vp = &v;

    printf("=== Pointer to Struct ===\n");
    printf("Direct: x=%d y=%d z=%d\n", v.x, v.y, v.z);
    printf("Ptr:    x=%d y=%d z=%d\n", vp->x, vp->y, vp->z);

    vp->x = 100;
    vp->y = 200;
    vp->z = 300;
    printf("Modify via ptr: x=%d y=%d z=%d\n", v.x, v.y, v.z);

    // Array of structs accessed via pointer
    struct Point2D pts[4];
    pts[0].x = 1; pts[0].y = 2;
    pts[1].x = 3; pts[1].y = 4;
    pts[2].x = 5; pts[2].y = 6;
    pts[3].x = 7; pts[3].y = 8;

    struct Point2D *pp = pts;
    printf("Array of structs via ptr:\n");
    int i;
    for (i = 0; i < 4; i++) {
        printf("  [%d] x=%d y=%d\n", i, (pp + i)->x, (pp + i)->y);
    }
}

// Section 10: Pointer cast (int* as char*)
void test_ptr_cast(void) {
    // Use a known value: 0x04030201 = 67305985
    // On little-endian (ARM64): byte[0]=0x01, byte[1]=0x02, byte[2]=0x03, byte[3]=0x04
    int val = 0x04030201;
    char *cp = (char *)&val;

    printf("=== Pointer Cast (int* as char*) ===\n");
    printf("Original int: 0x%08x\n", val);
    printf("byte[0] = %d\n", cp[0]);
    printf("byte[1] = %d\n", cp[1]);
    printf("byte[2] = %d\n", cp[2]);
    printf("byte[3] = %d\n", cp[3]);
}

// Section 11: Function pointers
void test_function_pointers(void) {
    int (*fp)(int, int);

    printf("=== Function Pointers ===\n");

    fp = add;
    printf("add(3, 4) = %d\n", fp(3, 4));

    fp = sub;
    printf("sub(10, 3) = %d\n", fp(10, 3));

    fp = mul;
    printf("mul(3, 4) = %d\n", fp(3, 4));

    // Array of function pointers
    int (*ops[3])(int, int);
    ops[0] = add;
    ops[1] = sub;
    ops[2] = mul;

    printf("Ops via function pointer array:\n");
    printf("  op[0](10, 5) = %d\n", ops[0](10, 5));
    printf("  op[1](10, 5) = %d\n", ops[1](10, 5));
    printf("  op[2](10, 5) = %d\n", ops[2](10, 5));
}

// Section 12: Walking through array with pointer increment
void test_ptr_walk(void) {
    int arr[10];
    int i;
    for (i = 0; i < 10; i++) arr[i] = (i + 1) * 2;
    printf("=== Pointer Walk Through Array ===\n");

    // Print using index
    printf("Array:");
    for (i = 0; i < 10; i++) printf(" %d", arr[i]);
    printf("\n");

    // Forward walk with pointer
    printf("Forward walk:");
    int *p = arr;
    int *end = arr + 10;
    int sum = 0;
    while (p < end) {
        printf(" %d", *p);
        sum = sum + *p;
        p = p + 1;
    }
    printf("\n");
    printf("Sum = %d\n", sum);

    // Reverse walk
    printf("Reverse walk:");
    p = arr + 9;
    while (p >= arr) {
        printf(" %d", *p);
        p = p - 1;
    }
    printf("\n");
}

// Bonus: pointer difference used for search
int *find_in_array(int *arr, int len, int target) {
    int i;
    for (i = 0; i < len; i++) {
        if (arr[i] == target) return &arr[i];
    }
    return 0;
}

void test_ptr_diff_search(void) {
    int arr[10];
    int i;
    for (i = 0; i < 10; i++) arr[i] = (i + 1) * 2;
    printf("=== Pointer Difference in Loop ===\n");

    int *found;

    found = find_in_array(arr, 10, 6);
    if (found) {
        int idx = found - arr;
        printf("Search 6: found at index %d\n", idx);
    } else {
        printf("Search 6: not found\n");
    }

    found = find_in_array(arr, 10, 14);
    if (found) {
        int idx = found - arr;
        printf("Search 14: found at index %d\n", idx);
    } else {
        printf("Search 14: not found\n");
    }

    found = find_in_array(arr, 10, 99);
    if (found) {
        int idx = found - arr;
        printf("Search 99: found at index %d\n", idx);
    } else {
        printf("Search 99: not found\n");
    }
}

// Bonus: Nested struct pointer access
void test_nested_struct_ptr(void) {
    struct Outer o;
    o.inner.val = 42;
    o.extra = 7;

    struct Outer *op = &o;
    struct Inner *ip = &(op->inner);

    printf("=== Nested Struct Pointer Access ===\n");
    printf("outer.inner.val = %d\n", o.inner.val);
    printf("op->ip->val = %d\n", ip->val);

    ip->val = 88;
    printf("Modified: op->ip->val = %d\n", o.inner.val);
}

int main(void) {
    test_ptr_subtraction();
    test_ptr_strides();
    test_multi_level();
    test_triple_pointer();
    test_deref_vs_index();
    test_negative_arith();
    test_ptr_comparison();
    test_array_of_pointers();
    test_ptr_to_struct();
    test_ptr_cast();
    test_function_pointers();
    test_ptr_walk();
    test_ptr_diff_search();
    test_nested_struct_ptr();

    printf("=== Pointer Arithmetic Summary ===\n");
    printf("All 12 sections passed.\n");

    return 0;
}
