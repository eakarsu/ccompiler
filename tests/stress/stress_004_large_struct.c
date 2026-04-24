// stress_004_large_struct.c - Very large structs stress test
// EXPECT: arr_of_structs=550
// EXPECT: nested_struct=787
// EXPECT: struct_arrays=100
// EXPECT: mixed_struct=1331
// EXPECT: big_by_ptr=528
// EXPECT: heap_struct=1056
int printf(const char *fmt, ...);
void *malloc(long size);
void free(void *ptr);

// --- Struct with 30+ fields ---
struct big {
    int f0, f1, f2, f3, f4, f5, f6, f7, f8, f9;
    int f10, f11, f12, f13, f14, f15, f16, f17, f18, f19;
    int f20, f21, f22, f23, f24, f25, f26, f27, f28, f29;
    int f30, f31;
};

int sum_big(struct big *b) {
    return b->f0+b->f1+b->f2+b->f3+b->f4+b->f5+b->f6+b->f7+b->f8+b->f9
         + b->f10+b->f11+b->f12+b->f13+b->f14+b->f15+b->f16+b->f17+b->f18+b->f19
         + b->f20+b->f21+b->f22+b->f23+b->f24+b->f25+b->f26+b->f27+b->f28+b->f29
         + b->f30+b->f31;
}

// --- 5-level nested structs ---
struct depth5 { int val; };
struct depth4 { struct depth5 d5; int x; };
struct depth3 { struct depth4 d4; int x; };
struct depth2 { struct depth3 d3; int x; };
struct depth1 { struct depth2 d2; int x; };

// --- Struct containing arrays ---
struct with_arrays {
    int nums[10];
    char name[16];
    int matrix[3][3];
};

// --- Struct with mixed types ---
struct mixed {
    char c;
    int i;
    long l;
    char str[8];
    int arr[4];
    struct depth5 nested;
};

// --- Array of large structs ---
void test_array_of_structs(void) {
    struct big arr[5];
    int i;
    for (i = 0; i < 5; i++) {
        arr[i].f0 = i;
        arr[i].f1 = i * 2;
        arr[i].f2 = i * 3;
        arr[i].f3 = i * 4;
        arr[i].f4 = i * 5;
        arr[i].f5 = i * 6;
        arr[i].f6 = i * 7;
        arr[i].f7 = i * 8;
        arr[i].f8 = i * 9;
        arr[i].f9 = i * 10;
        arr[i].f10 = 0; arr[i].f11 = 0; arr[i].f12 = 0; arr[i].f13 = 0;
        arr[i].f14 = 0; arr[i].f15 = 0; arr[i].f16 = 0; arr[i].f17 = 0;
        arr[i].f18 = 0; arr[i].f19 = 0; arr[i].f20 = 0; arr[i].f21 = 0;
        arr[i].f22 = 0; arr[i].f23 = 0; arr[i].f24 = 0; arr[i].f25 = 0;
        arr[i].f26 = 0; arr[i].f27 = 0; arr[i].f28 = 0; arr[i].f29 = 0;
        arr[i].f30 = 0; arr[i].f31 = 0;
    }
    int total = 0;
    for (i = 0; i < 5; i++) {
        total += sum_big(&arr[i]);
    }
    // for i=0..4: sum of i*(1+2+..+10) = i*55
    // total = 0*55 + 1*55 + 2*55 + 3*55 + 4*55 = 55*10 = 550
    printf("arr_of_structs=%d\n", total);
}

void test_nested_struct(void) {
    struct depth1 d;
    d.d2.d3.d4.d5.val = 777;
    d.d2.d3.d4.x = 1;
    d.d2.d3.x = 2;
    d.d2.x = 3;
    d.x = 4;
    int sum = d.d2.d3.d4.d5.val + d.d2.d3.d4.x + d.d2.d3.x + d.d2.x + d.x;
    printf("nested_struct=%d\n", sum); // 777+1+2+3+4 = 787
}

void test_struct_with_arrays(void) {
    struct with_arrays s;
    int i;
    for (i = 0; i < 10; i++) s.nums[i] = i + 1;
    s.name[0] = 'H'; s.name[1] = 'e'; s.name[2] = 'l';
    s.name[3] = 'l'; s.name[4] = 'o'; s.name[5] = '\0';
    int j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            s.matrix[i][j] = i * 3 + j + 1;

    int sum = 0;
    for (i = 0; i < 10; i++) sum += s.nums[i];
    // 1+2+...+10 = 55
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            sum += s.matrix[i][j];
    // 1+2+...+9 = 45
    // total = 55 + 45 = 100
    printf("struct_arrays=%d\n", sum);
}

void test_mixed_struct(void) {
    struct mixed m;
    m.c = 'Z'; // 90
    m.i = 42;
    m.l = 1000;
    m.str[0] = 'A'; m.str[1] = 'B'; m.str[2] = '\0';
    m.arr[0] = 10; m.arr[1] = 20; m.arr[2] = 30; m.arr[3] = 40;
    m.nested.val = 99;
    int sum = m.c + m.i + (int)m.l + m.arr[0] + m.arr[1] + m.arr[2] + m.arr[3] + m.nested.val;
    // 90+42+1000+10+20+30+40+99 = 1331
    printf("mixed_struct=%d\n", sum);
}

void test_pass_big_by_ptr(void) {
    struct big b;
    int i;
    int *fields = (int *)&b;
    for (i = 0; i < 32; i++) {
        fields[i] = i + 1;
    }
    int s = sum_big(&b);
    // 1+2+...+32 = 528
    printf("big_by_ptr=%d\n", s);
}

void test_struct_on_heap(void) {
    struct big *p = (struct big *)malloc(sizeof(struct big));
    int i;
    int *fields = (int *)p;
    for (i = 0; i < 32; i++) {
        fields[i] = (i + 1) * 2;
    }
    int s = sum_big(p);
    // 2+4+...+64 = 2*(1+2+...+32) = 2*528 = 1056
    printf("heap_struct=%d\n", s);
    free(p);
}

int main(void) {
    test_array_of_structs();
    test_nested_struct();
    test_struct_with_arrays();
    test_mixed_struct();
    test_pass_big_by_ptr();
    test_struct_on_heap();
    return 0;
}
