// stress_005_ptr_chains.c - Complex pointer chains stress test
// EXPECT: triple_ptr=99
// EXPECT: ptr_array=1200
// EXPECT: struct_ptr_arith=1500
// EXPECT: ptr_cast=10
// EXPECT: fn_ptr_chain=50
// EXPECT: ptr_struct_arr=100
// EXPECT: ptr_diff=9
// EXPECT: fn_ptr_arr=36
// EXPECT: ptr_swap=222,111
// EXPECT: linked_list=100
int printf(const char *fmt, ...);
void *malloc(long size);
void free(void *ptr);

// --- Triple pointer ---
void test_triple_ptr(void) {
    int val = 42;
    int *p = &val;
    int **pp = &p;
    int ***ppp = &pp;
    ***ppp = 99;
    printf("triple_ptr=%d\n", val);
}

// --- Array of pointers to arrays ---
void test_ptr_array(void) {
    int a[5] = {10, 20, 30, 40, 50};
    int b[5] = {60, 70, 80, 90, 100};
    int c[5] = {110, 120, 130, 140, 150};
    int *ptrs[3];
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;

    int sum = 0;
    int i, j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 5; j++)
            sum += ptrs[i][j];
    // 10+20+...+150 = sum of 15 terms, 10*15 + 10*(0+1+2+...+14) = 150+1050 = 1200
    // Actually: 10+20+30+40+50+60+70+80+90+100+110+120+130+140+150
    // = (10+150)*15/2 = 160*15/2 = 1200
    printf("ptr_array=%d\n", sum);
}

// --- Pointer arithmetic on struct pointers ---
struct item { int id; int value; };

void test_struct_ptr_arith(void) {
    struct item items[5];
    int i;
    for (i = 0; i < 5; i++) {
        items[i].id = i;
        items[i].value = (i + 1) * 100;
    }
    struct item *p = items;
    int sum = 0;
    for (i = 0; i < 5; i++) {
        sum += (p + i)->value;
    }
    // 100+200+300+400+500 = 1500
    printf("struct_ptr_arith=%d\n", sum);
}

// --- Casting between pointer types ---
void test_ptr_cast(void) {
    int x = 0x04030201;
    char *cp = (char *)&x;
    // On little-endian: cp[0]=1, cp[1]=2, cp[2]=3, cp[3]=4
    int sum = cp[0] + cp[1] + cp[2] + cp[3];
    printf("ptr_cast=%d\n", sum); // 1+2+3+4 = 10
}

// --- Function pointer returning function pointer ---
typedef int (*intfn)(int);

int double_it(int x) { return x * 2; }
int triple_it(int x) { return x * 3; }
int square_it(int x) { return x * x; }

typedef intfn (*fn_selector)(int);

intfn select_fn(int choice) {
    if (choice == 0) return double_it;
    if (choice == 1) return triple_it;
    return square_it;
}

void test_fn_ptr_chain(void) {
    fn_selector sel = select_fn;
    intfn f0 = sel(0);
    intfn f1 = sel(1);
    intfn f2 = sel(2);
    int r = f0(5) + f1(5) + f2(5);
    // 10 + 15 + 25 = 50
    printf("fn_ptr_chain=%d\n", r);
}

// --- Pointer to array of structs via indirection ---
void test_ptr_to_struct_array(void) {
    struct item arr[4];
    arr[0].id = 0; arr[0].value = 10;
    arr[1].id = 1; arr[1].value = 20;
    arr[2].id = 2; arr[2].value = 30;
    arr[3].id = 3; arr[3].value = 40;

    struct item *p = arr;
    struct item **pp = &p;

    int sum = (*pp)[0].value + (*pp)[1].value + (*pp)[2].value + (*pp)[3].value;
    printf("ptr_struct_arr=%d\n", sum); // 10+20+30+40 = 100
}

// --- Pointer differences ---
void test_ptr_diff(void) {
    int arr[10];
    int *start = &arr[0];
    int *end = &arr[9];
    long diff = end - start;
    printf("ptr_diff=%ld\n", diff); // 9
}

// --- Array of function pointers called in loop ---
void test_fn_ptr_array(void) {
    intfn fns[3];
    fns[0] = double_it;
    fns[1] = triple_it;
    fns[2] = square_it;
    int sum = 0;
    int i;
    for (i = 0; i < 3; i++) {
        sum += fns[i](4);
    }
    // double_it(4)=8, triple_it(4)=12, square_it(4)=16 => 36
    printf("fn_ptr_arr=%d\n", sum);
}

// --- Pointer to pointer swap ---
void swap_ptrs(int **a, int **b) {
    int *tmp = *a;
    *a = *b;
    *b = tmp;
}

void test_ptr_swap(void) {
    int x = 111, y = 222;
    int *px = &x, *py = &y;
    swap_ptrs(&px, &py);
    printf("ptr_swap=%d,%d\n", *px, *py); // 222,111
}

// --- Pointer chain walk ---
struct node {
    int val;
    struct node *next;
};

void test_linked_list(void) {
    struct node n4 = {40, 0};
    struct node n3 = {30, &n4};
    struct node n2 = {20, &n3};
    struct node n1 = {10, &n2};

    struct node *cur = &n1;
    int sum = 0;
    while (cur) {
        sum += cur->val;
        cur = cur->next;
    }
    printf("linked_list=%d\n", sum); // 10+20+30+40 = 100
}

int main(void) {
    test_triple_ptr();
    test_ptr_array();
    test_struct_ptr_arith();
    test_ptr_cast();
    test_fn_ptr_chain();
    test_ptr_to_struct_array();
    test_ptr_diff();
    test_fn_ptr_array();
    test_ptr_swap();
    test_linked_list();
    return 0;
}
