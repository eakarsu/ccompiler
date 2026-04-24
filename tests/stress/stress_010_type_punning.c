// stress_010_type_punning.c - Type system stress test
// EXPECT: union_pun=10
// EXPECT: byte_read=46
// EXPECT: sz_int=4
// EXPECT: sz_char=1
// EXPECT: sz_long=8
// EXPECT: sz_arr=40
// EXPECT: sz_ptr=8
// EXPECT: sz_expr=8
// EXPECT: align_int=4
// EXPECT: align_long=8
// EXPECT: align_char=1
// EXPECT: align_inner=8
// EXPECT: align_outer=8
// EXPECT: bool=01110110
// EXPECT: enum_arr=15,110
// EXPECT: typedef_chain=100
// EXPECT: union_sz=16
// EXPECT: union_c=88
// EXPECT: padded=10
// EXPECT: padded_sz=24
// EXPECT: conv=127,127,127,127
// EXPECT: overflow=0
// EXPECT: ptr_as_int=1
// EXPECT: ptr_as_char=1
// EXPECT: neg_enum=-3
int printf(const char *fmt, ...);

// --- Union type punning ---
union intbytes {
    int i;
    char bytes[4];
};

void test_union_punning(void) {
    union intbytes u;
    u.i = 0x04030201;
    // Little-endian: bytes[0]=1, bytes[1]=2, bytes[2]=3, bytes[3]=4
    int sum = u.bytes[0] + u.bytes[1] + u.bytes[2] + u.bytes[3];
    printf("union_pun=%d\n", sum); // 10
}

// --- Casting int* to char* and reading bytes ---
void test_byte_read(void) {
    int val = 0x0A0B0C0D;
    char *p = (char *)&val;
    // Little-endian: p[0]=0x0D=13, p[1]=0x0C=12, p[2]=0x0B=11, p[3]=0x0A=10
    int sum = p[0] + p[1] + p[2] + p[3];
    printf("byte_read=%d\n", sum); // 46
}

// --- sizeof on complex expressions ---
void test_sizeof(void) {
    int a;
    char c;
    long l;
    int arr[10];
    struct { int x; int y; char c; } s;
    int *p;

    printf("sz_int=%d\n", (int)sizeof(a));          // 4
    printf("sz_char=%d\n", (int)sizeof(c));          // 1
    printf("sz_long=%d\n", (int)sizeof(l));          // 8
    printf("sz_arr=%d\n", (int)sizeof(arr));          // 40
    printf("sz_ptr=%d\n", (int)sizeof(p));            // 8
    // sizeof expression (not evaluated)
    printf("sz_expr=%d\n", (int)sizeof(a + l));       // 8 (promoted to long)
}

// --- _Alignof on nested structs ---
struct inner_align { long l; char c; };
struct outer_align { char c; struct inner_align inner; };

void test_alignof(void) {
    printf("align_int=%d\n", (int)_Alignof(int));          // 4
    printf("align_long=%d\n", (int)_Alignof(long));         // 8
    printf("align_char=%d\n", (int)_Alignof(char));         // 1
    printf("align_inner=%d\n", (int)_Alignof(struct inner_align)); // 8
    printf("align_outer=%d\n", (int)_Alignof(struct outer_align)); // 8
}

// --- _Bool from every type ---
void test_bool(void) {
    _Bool b1 = 0;
    _Bool b2 = 1;
    _Bool b3 = 42;
    _Bool b4 = -1;
    _Bool b5 = 0;
    int x = 100;
    _Bool b6 = x;
    char c = 'A';
    _Bool b7 = c;
    long l = 0;
    _Bool b8 = l;

    printf("bool=%d%d%d%d%d%d%d%d\n", b1, b2, b3, b4, b5, b6, b7, b8);
    // 0,1,1,1,0,1,1,0 -> 01110110
}

// --- Enum used as array size ---
enum sizes {
    SZ_SMALL = 5,
    SZ_MEDIUM = 10,
    SZ_LARGE = 20
};

void test_enum_array_size(void) {
    int small[SZ_SMALL];
    int medium[SZ_MEDIUM];
    int i;
    for (i = 0; i < SZ_SMALL; i++) small[i] = i + 1;
    for (i = 0; i < SZ_MEDIUM; i++) medium[i] = (i + 1) * 2;

    int sum_s = 0, sum_m = 0;
    for (i = 0; i < SZ_SMALL; i++) sum_s += small[i];
    for (i = 0; i < SZ_MEDIUM; i++) sum_m += medium[i];
    // sum_s = 1+2+3+4+5 = 15
    // sum_m = 2+4+6+...+20 = 2*(1+2+...+10) = 110
    printf("enum_arr=%d,%d\n", sum_s, sum_m); // 15,110
}

// --- Typedef chains (5 levels) ---
typedef int type1;
typedef type1 type2;
typedef type2 type3;
typedef type3 type4;
typedef type4 type5;

void test_typedef_chain(void) {
    type5 a = 42;
    type4 b = 58;
    type3 c = a + b;
    type2 d = c * 2;
    type1 e = d - 100;
    printf("typedef_chain=%d\n", e); // (42+58)*2 - 100 = 100
}

// --- Union with different sized members ---
union multi {
    char c;
    int i;
    long l;
    char arr[16];
};

void test_union_sizes(void) {
    union multi u;
    printf("union_sz=%d\n", (int)sizeof(u)); // 16 (size of largest member)
    u.l = 0;
    u.c = 'X'; // 88
    printf("union_c=%d\n", u.c); // 88
}

// --- Struct padding and size ---
struct padded {
    char a;     // offset 0, size 1
    // 3 bytes padding
    int b;      // offset 4, size 4
    char c;     // offset 8, size 1
    // 7 bytes padding
    long d;     // offset 16, size 8
};

void test_struct_padding(void) {
    struct padded p;
    p.a = 1; p.b = 2; p.c = 3; p.d = 4;
    int total = p.a + p.b + p.c + (int)p.d;
    printf("padded=%d\n", total); // 10
    printf("padded_sz=%d\n", (int)sizeof(struct padded)); // 24
}

// --- Implicit conversions ---
void test_conversions(void) {
    char c = 127;
    int i = c;
    long l = i;
    char back = (char)l;
    printf("conv=%d,%d,%ld,%d\n", c, i, l, back); // 127,127,127,127

    // Integer overflow behavior (unsigned)
    unsigned int u = 4294967295u; // 0xFFFFFFFF
    unsigned int u2 = u + 1;
    printf("overflow=%u\n", u2); // 0
}

// --- Pointer to different types, same address ---
void test_ptr_types(void) {
    long val = 0x0807060504030201L;
    long *lp = &val;
    int *ip = (int *)lp;
    char *cp = (char *)lp;

    // Little-endian: first 4 bytes = 0x04030201
    printf("ptr_as_int=%d\n", *ip == 0x04030201); // 1

    // First byte
    printf("ptr_as_char=%d\n", cp[0]); // 1
}

// --- Enum with negative values ---
enum neg_enum {
    NEG3 = -3,
    NEG2 = -2,
    NEG1 = -1,
    ZERO = 0,
    POS1 = 1,
    POS2 = 2
};

void test_neg_enum(void) {
    enum neg_enum e = NEG3;
    int sum = NEG3 + NEG2 + NEG1 + ZERO + POS1 + POS2;
    // -3 + -2 + -1 + 0 + 1 + 2 = -3
    printf("neg_enum=%d\n", sum);
}

int main(void) {
    test_union_punning();
    test_byte_read();
    test_sizeof();
    test_alignof();
    test_bool();
    test_enum_array_size();
    test_typedef_chain();
    test_union_sizes();
    test_struct_padding();
    test_conversions();
    test_ptr_types();
    test_neg_enum();
    return 0;
}
