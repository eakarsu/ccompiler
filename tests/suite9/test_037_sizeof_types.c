int printf(const char *fmt, ...);

typedef struct {
    int x;
    int y;
} Pair;

typedef struct {
    int a;
    int b;
    int c;
} Triple;

typedef struct {
    char c;
    int i;
} CharInt;

typedef struct {
    int vals[10];
} TenInts;

typedef struct {
    Pair p;
    int z;
} PairPlus;

enum SmallEnum {
    SE_A = 0,
    SE_B = 1,
    SE_C = 2
};

void test_basic_sizes() {
    int si = sizeof(int);
    int sc = sizeof(char);
    // EXPECT: sizeof_int=4
    printf("sizeof_int=%d\n", si);
    // EXPECT: sizeof_char=1
    printf("sizeof_char=%d\n", sc);
}

void test_pointer_sizes() {
    int sp = sizeof(int *);
    int scp = sizeof(char *);
    int svp = sizeof(void *);
    // EXPECT: ptr_sizes_equal=1
    printf("ptr_sizes_equal=%d\n", (sp == scp) && (scp == svp));
    // EXPECT: ptr_size=8
    printf("ptr_size=%d\n", sp);
}

void test_array_sizes() {
    int arr5[5];
    int arr10[10];
    char carr[20];
    int sa5 = sizeof(arr5);
    int sa10 = sizeof(arr10);
    int sca = sizeof(carr);
    // EXPECT: arr5_size=20
    printf("arr5_size=%d\n", sa5);
    // EXPECT: arr10_size=40
    printf("arr10_size=%d\n", sa10);
    // EXPECT: carr20_size=20
    printf("carr20_size=%d\n", sca);
}

void test_struct_sizes() {
    int sp = sizeof(Pair);
    int st = sizeof(Triple);
    // EXPECT: pair_size=8
    printf("pair_size=%d\n", sp);
    // EXPECT: triple_size=12
    printf("triple_size=%d\n", st);
}

void test_struct_array_member() {
    int sti = sizeof(TenInts);
    // EXPECT: tenints_size=40
    printf("tenints_size=%d\n", sti);
}

void test_nested_struct_size() {
    int spp = sizeof(PairPlus);
    // EXPECT: pairplus_size=12
    printf("pairplus_size=%d\n", spp);
}

void test_sizeof_expr() {
    int a = 42;
    int sa = sizeof(a);
    // EXPECT: sizeof_var=4
    printf("sizeof_var=%d\n", sa);

    char c = 65;
    int sc = sizeof(c);
    // EXPECT: sizeof_charvar=1
    printf("sizeof_charvar=%d\n", sc);

    Pair p;
    p.x = 1;
    p.y = 2;
    int sp = sizeof(p);
    // EXPECT: sizeof_pairvar=8
    printf("sizeof_pairvar=%d\n", sp);
}

void test_sizeof_in_calc() {
    int elem_size = sizeof(int);
    int num_elems = 5;
    int total = elem_size * num_elems;
    // EXPECT: array_bytes=20
    printf("array_bytes=%d\n", total);

    int struct_elems = 3;
    int struct_total = sizeof(Pair) * struct_elems;
    // EXPECT: struct_array_bytes=24
    printf("struct_array_bytes=%d\n", struct_total);
}

void test_sizeof_pointer_deref() {
    int *ip;
    int si = sizeof(*ip);
    // EXPECT: deref_int_size=4
    printf("deref_int_size=%d\n", si);

    Pair *pp;
    int sp = sizeof(*pp);
    // EXPECT: deref_pair_size=8
    printf("deref_pair_size=%d\n", sp);
}

void test_count_elements() {
    int arr[8];
    int count = sizeof(arr) / sizeof(arr[0]);
    // EXPECT: element_count=8
    printf("element_count=%d\n", count);

    Pair parr[5];
    int pcount = sizeof(parr) / sizeof(parr[0]);
    // EXPECT: pair_count=5
    printf("pair_count=%d\n", pcount);
}

int main() {
    test_basic_sizes();
    test_pointer_sizes();
    test_array_sizes();
    test_struct_sizes();
    test_struct_array_member();
    test_nested_struct_size();
    test_sizeof_expr();
    test_sizeof_in_calc();
    test_sizeof_pointer_deref();
    test_count_elements();
    // EXPECT: done
    printf("done\n");
    return 0;
}
