// stress_098_type_system.c - Type system exercise torture test
// Exercises: every integer type, unsigned variants, enum, struct, union,
// pointers, arrays, typedef, sizeof, type promotion, casting.
// EXPECT: sizeof_char=1
// EXPECT: sizeof_int=4
// EXPECT: sizeof_long=8
// EXPECT: sizeof_ptr=8
// EXPECT: char_assign=65
// EXPECT: int_assign=12345
// EXPECT: long_assign=9876543210
// EXPECT: unsigned_wrap=0
// EXPECT: ulong_big=18446744073709551615
// EXPECT: enum_vals=0 1 5 6
// EXPECT: struct_size=12
// EXPECT: struct_vals=10 20 30
// EXPECT: union_size=8
// EXPECT: union_overlap=1
// EXPECT: nested_struct=100 200 300
// EXPECT: arr_int=15
// EXPECT: arr_char=hello
// EXPECT: ptr_deref=42
// EXPECT: ptr_arith=30
// EXPECT: ptr_to_ptr=99
// EXPECT: typedef_use=77
// EXPECT: func_ptr=15
// EXPECT: cast_chain=66
// EXPECT: promote_char=200
// EXPECT: unsigned_cmp=1
// EXPECT: bool_test=1 0 1
// EXPECT: enum_neg=-5 -4 10 11
// EXPECT: array_of_struct=6 15 24
// EXPECT: ptr_array=10 20 30
// EXPECT: all_pass=1

int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

// Enums
enum Color { RED, GREEN, BLUE = 5, YELLOW };

// Enums with negative values
enum Signed { NEG5 = -5, NEG4, POS10 = 10, POS11 };

// Struct
struct Vec3 {
    int x;
    int y;
    int z;
};

// Nested struct
struct Nested {
    struct Vec3 pos;
    int extra;
};

// Union
union Data {
    int i;
    long l;
    char c[8];
};

// Typedef
typedef int MyInt;
typedef struct Vec3 Vec3T;
typedef int (*FuncPtr)(int, int);

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }

// Struct with all integer types
struct AllTypes {
    char c;
    int i;
    long l;
    unsigned int ui;
    unsigned long ul;
};

int main(void) {
    int pass = 1;

    // sizeof tests
    printf("sizeof_char=%d\n", (int)sizeof(char));
    if (sizeof(char) != 1) pass = 0;
    printf("sizeof_int=%d\n", (int)sizeof(int));
    if (sizeof(int) != 4) pass = 0;
    printf("sizeof_long=%d\n", (int)sizeof(long));
    if (sizeof(long) != 8) pass = 0;
    printf("sizeof_ptr=%d\n", (int)sizeof(int *));
    if (sizeof(int *) != 8) pass = 0;

    // char assignment
    char c = 'A';
    printf("char_assign=%d\n", (int)c);
    if (c != 65) pass = 0;

    // int assignment
    int i = 12345;
    printf("int_assign=%d\n", i);
    if (i != 12345) pass = 0;

    // long assignment
    long l = 9876543210L;
    printf("long_assign=%ld\n", l);
    if (l != 9876543210L) pass = 0;

    // unsigned wrap-around
    unsigned int u = 0;
    u = u - 1;  // 4294967295
    u = u + 1;  // wraps to 0
    printf("unsigned_wrap=%u\n", u);
    if (u != 0) pass = 0;

    // unsigned long big value
    unsigned long ul = 0;
    ul = ul - 1;  // max unsigned long
    printf("ulong_big=%lu\n", ul);

    // enum
    printf("enum_vals=%d %d %d %d\n", RED, GREEN, BLUE, YELLOW);
    if (RED != 0 || GREEN != 1 || BLUE != 5 || YELLOW != 6) pass = 0;

    // struct
    struct Vec3 v;
    v.x = 10; v.y = 20; v.z = 30;
    printf("struct_size=%d\n", (int)sizeof(struct Vec3));
    if (sizeof(struct Vec3) != 12) pass = 0;
    printf("struct_vals=%d %d %d\n", v.x, v.y, v.z);
    if (v.x != 10 || v.y != 20 || v.z != 30) pass = 0;

    // union
    union Data d;
    printf("union_size=%d\n", (int)sizeof(union Data));
    if (sizeof(union Data) != 8) pass = 0;
    d.l = 0;
    d.i = 1;
    // On little-endian, the first 4 bytes of d.l are now 1
    // but the full d.l is not 1 if the upper bytes were different
    // d.l was 0, so upper 4 bytes are still 0
    // d.i = 1 sets lower 4 bytes to 1, upper 4 still 0
    int overlap = (d.l == 1) ? 1 : 0;
    printf("union_overlap=%d\n", overlap);
    if (!overlap) pass = 0;

    // nested struct
    struct Nested n;
    n.pos.x = 100; n.pos.y = 200; n.pos.z = 300;
    n.extra = 400;
    printf("nested_struct=%d %d %d\n", n.pos.x, n.pos.y, n.pos.z);
    if (n.pos.x != 100 || n.pos.y != 200 || n.pos.z != 300) pass = 0;

    // array of int
    int arr[5];
    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
    int sum = 0;
    int j;
    for (j = 0; j < 5; j++) sum = sum + arr[j];
    printf("arr_int=%d\n", sum);
    if (sum != 15) pass = 0;

    // array of char (string)
    char str[6];
    str[0] = 'h'; str[1] = 'e'; str[2] = 'l'; str[3] = 'l'; str[4] = 'o'; str[5] = 0;
    printf("arr_char=%s\n", str);

    // pointer dereference
    int x = 42;
    int *px = &x;
    printf("ptr_deref=%d\n", *px);
    if (*px != 42) pass = 0;

    // pointer arithmetic
    int parr[5];
    parr[0] = 10; parr[1] = 20; parr[2] = 30; parr[3] = 40; parr[4] = 50;
    int *p = parr;
    p = p + 2;
    printf("ptr_arith=%d\n", *p);
    if (*p != 30) pass = 0;

    // pointer to pointer
    int val = 99;
    int *pv = &val;
    int **ppv = &pv;
    printf("ptr_to_ptr=%d\n", **ppv);
    if (**ppv != 99) pass = 0;

    // typedef
    MyInt mi = 77;
    printf("typedef_use=%d\n", mi);
    if (mi != 77) pass = 0;

    // function pointer
    FuncPtr fp = add;
    int fr = fp(10, 5);
    printf("func_ptr=%d\n", fr);
    if (fr != 15) pass = 0;

    // cast chain: int -> char -> int
    int big = 322;   // 322 = 256 + 66 = 'B' when truncated to char
    char cc = (char)big;
    int back = (int)cc;
    // On signed char, 322 % 256 = 66 = 'B'
    printf("cast_chain=%d\n", back);
    if (back != 66) pass = 0;

    // char promotion in arithmetic
    char a = 100;
    char b = 100;
    int promoted = a + b;  // should be 200, not overflow
    printf("promote_char=%d\n", promoted);
    if (promoted != 200) pass = 0;

    // unsigned comparison
    unsigned int ua = 3000000000U;
    unsigned int ub = 1000000000U;
    int cmp = (ua > ub) ? 1 : 0;
    printf("unsigned_cmp=%d\n", cmp);
    if (cmp != 1) pass = 0;

    // _Bool (use int 0/1 pattern for compatibility)
    int b1 = (5 > 3);
    int b2 = (3 > 5);
    int b3 = (b1 && !b2);
    printf("bool_test=%d %d %d\n", b1, b2, b3);
    if (b1 != 1 || b2 != 0 || b3 != 1) pass = 0;

    // enum with negative values
    printf("enum_neg=%d %d %d %d\n", NEG5, NEG4, POS10, POS11);
    if (NEG5 != -5 || NEG4 != -4 || POS10 != 10 || POS11 != 11) pass = 0;

    // array of struct
    struct Vec3 vecs[3];
    vecs[0].x = 1; vecs[0].y = 2; vecs[0].z = 3;
    vecs[1].x = 4; vecs[1].y = 5; vecs[1].z = 6;
    vecs[2].x = 7; vecs[2].y = 8; vecs[2].z = 9;
    printf("array_of_struct=%d %d %d\n",
        vecs[0].x + vecs[0].y + vecs[0].z,
        vecs[1].x + vecs[1].y + vecs[1].z,
        vecs[2].x + vecs[2].y + vecs[2].z);
    if (vecs[0].x + vecs[0].y + vecs[0].z != 6) pass = 0;
    if (vecs[2].x + vecs[2].y + vecs[2].z != 24) pass = 0;

    // array of pointers
    int v1 = 10;
    int v2 = 20;
    int v3 = 30;
    int *ptrs[3];
    ptrs[0] = &v1;
    ptrs[1] = &v2;
    ptrs[2] = &v3;
    printf("ptr_array=%d %d %d\n", *ptrs[0], *ptrs[1], *ptrs[2]);
    if (*ptrs[0] != 10 || *ptrs[1] != 20 || *ptrs[2] != 30) pass = 0;

    printf("all_pass=%d\n", pass);
    return 0;
}
