// stress_062_align_stress.c - Alignment stress test
// EXPECT: alignof_char=1
// EXPECT: alignof_short=2
// EXPECT: alignof_int=4
// EXPECT: alignof_long=8
// EXPECT: alignof_ptr=8
// EXPECT: struct_a_size=32
// EXPECT: struct_a_align=16
// EXPECT: local_aligned16=1
// EXPECT: local_aligned32=1
// EXPECT: struct_member_align=1
// EXPECT: array_elem_spacing=16
// EXPECT: mixed_struct_size=32
// EXPECT: mixed_struct_align=8
// EXPECT: packed_equiv_size=16
// EXPECT: nested_align_outer=16
// EXPECT: nested_align_inner=16
// EXPECT: char_after_aligned=1
// EXPECT: struct_array_align=1
// EXPECT: all_aligned=1
// EXPECT: align_propagation=16
// EXPECT: offset_field_b=16
// EXPECT: offset_field_c=20
// EXPECT: func_local_align=1
// EXPECT: multi_align_ok=1

int printf(const char *fmt, ...);

// Basic _Alignof tests
void test_alignof(void) {
    printf("alignof_char=%d\n", (int)_Alignof(char));
    printf("alignof_short=%d\n", (int)_Alignof(short));
    printf("alignof_int=%d\n", (int)_Alignof(int));
    printf("alignof_long=%d\n", (int)_Alignof(long));
    printf("alignof_ptr=%d\n", (int)_Alignof(int*));
}

// Struct with _Alignas member
struct AlignedStruct {
    char a;
    _Alignas(16) int b;
    char c;
};

void test_struct_align(void) {
    printf("struct_a_size=%d\n", (int)sizeof(struct AlignedStruct));
    printf("struct_a_align=%d\n", (int)_Alignof(struct AlignedStruct));
}

// _Alignas on local variable
void test_local_aligned(void) {
    _Alignas(16) int x = 42;
    _Alignas(32) int y = 99;
    // Check alignment via pointer arithmetic
    unsigned long addr_x = (unsigned long)&x;
    unsigned long addr_y = (unsigned long)&y;
    printf("local_aligned16=%d\n", (addr_x % 16) == 0);
    printf("local_aligned32=%d\n", (addr_y % 32) == 0);
}

// Struct with mixed alignment members
void test_struct_member_align(void) {
    struct AlignedStruct s;
    s.a = 'A';
    s.b = 100;
    s.c = 'Z';

    // Check that b is 16-byte aligned relative to struct start
    unsigned long base = (unsigned long)&s;
    unsigned long addr_b = (unsigned long)&s.b;
    unsigned long offset_b = addr_b - base;
    // offset_b should be 16 (due to _Alignas(16))
    printf("struct_member_align=%d\n", (offset_b % 16) == 0);
}

// Array of aligned structs
struct Elem16 {
    _Alignas(16) int value;
};

void test_array_elem_spacing(void) {
    struct Elem16 arr[4];
    arr[0].value = 10;
    arr[1].value = 20;
    arr[2].value = 30;
    arr[3].value = 40;

    unsigned long diff = (unsigned long)&arr[1] - (unsigned long)&arr[0];
    printf("array_elem_spacing=%d\n", (int)diff);
}

// Mixed struct: char, int, long, pointer
struct MixedAlign {
    char a;     // offset 0
    int b;      // offset 4 (aligned to 4)
    char c;     // offset 8
    long d;     // offset 16 (aligned to 8) - actually offset 16 with padding
    char e;     // offset 24
};

void test_mixed_struct(void) {
    printf("mixed_struct_size=%d\n", (int)sizeof(struct MixedAlign));
    printf("mixed_struct_align=%d\n", (int)_Alignof(struct MixedAlign));
}

// Struct that looks packed but isn't
struct PackedEquiv {
    long a;  // 8 bytes
    long b;  // 8 bytes
};

void test_packed_equiv(void) {
    printf("packed_equiv_size=%d\n", (int)sizeof(struct PackedEquiv));
}

// Nested struct alignment propagation
struct InnerAligned {
    _Alignas(16) int x;
};

struct OuterAligned {
    char tag;
    struct InnerAligned inner;
};

void test_nested_alignment(void) {
    printf("nested_align_outer=%d\n", (int)_Alignof(struct OuterAligned));
    printf("nested_align_inner=%d\n", (int)_Alignof(struct InnerAligned));
}

// Access to members after aligned member
void test_char_after_aligned(void) {
    struct AlignedStruct s;
    s.a = 'X';
    s.b = 42;
    s.c = 'Y';
    // Verify no corruption: writing to b doesn't corrupt c
    printf("char_after_aligned=%d\n", s.c == 'Y');
}

// Array of structs with alignment
void test_struct_array_align(void) {
    struct AlignedStruct arr[3];
    int i;
    for (i = 0; i < 3; i++) {
        arr[i].a = 'A' + i;
        arr[i].b = (i + 1) * 100;
        arr[i].c = 'X' + i;
    }
    int ok = 1;
    for (i = 0; i < 3; i++) {
        unsigned long addr = (unsigned long)&arr[i];
        if (addr % _Alignof(struct AlignedStruct) != 0) ok = 0;
        if (arr[i].b != (i + 1) * 100) ok = 0;
    }
    printf("struct_array_align=%d\n", ok);
}

// Many aligned locals, stress the stack allocator
void test_multi_aligned(void) {
    _Alignas(16) int a = 1;
    _Alignas(16) int b = 2;
    _Alignas(16) int c = 3;
    _Alignas(16) int d = 4;
    _Alignas(16) int e = 5;
    _Alignas(16) int f = 6;
    _Alignas(16) int g = 7;
    _Alignas(16) int h = 8;

    int ok = 1;
    if ((unsigned long)&a % 16 != 0) ok = 0;
    if ((unsigned long)&b % 16 != 0) ok = 0;
    if ((unsigned long)&c % 16 != 0) ok = 0;
    if ((unsigned long)&d % 16 != 0) ok = 0;
    if ((unsigned long)&e % 16 != 0) ok = 0;
    if ((unsigned long)&f % 16 != 0) ok = 0;
    if ((unsigned long)&g % 16 != 0) ok = 0;
    if ((unsigned long)&h % 16 != 0) ok = 0;

    // Also verify values not corrupted
    if (a + b + c + d + e + f + g + h != 36) ok = 0;

    printf("all_aligned=%d\n", ok);
}

// Alignment propagation through typedef
typedef struct {
    _Alignas(16) int value;
} Aligned16Int;

void test_align_propagation(void) {
    printf("align_propagation=%d\n", (int)_Alignof(Aligned16Int));
}

// Verify offsets explicitly
void test_offsets(void) {
    struct AlignedStruct s;
    unsigned long base = (unsigned long)&s;
    unsigned long off_b = (unsigned long)&s.b - base;
    unsigned long off_c = (unsigned long)&s.c - base;
    // a at 0, b at 16 (aligned), c right after b (at 20) but could vary
    // Actually c is after b (4 bytes for int), so c at offset 20
    // But struct padding to alignment 16 makes total size 32
    printf("offset_field_b=%d\n", (int)off_b);      // should be 16 due to _Alignas(16)
    printf("offset_field_c=%d\n", (int)off_c);      // should be 20
}

// Function with aligned locals of different sizes
int func_with_aligned_locals(int n) {
    _Alignas(16) char buf[32];
    _Alignas(16) int vals[8];
    int i;

    for (i = 0; i < 32; i++) {
        buf[i] = (char)(i + n);
    }
    for (i = 0; i < 8; i++) {
        vals[i] = i * n;
    }

    int sum = 0;
    for (i = 0; i < 8; i++) {
        sum += vals[i];
    }
    for (i = 0; i < 32; i++) {
        sum += buf[i];
    }

    int ok = 1;
    if ((unsigned long)buf % 16 != 0) ok = 0;
    if ((unsigned long)vals % 16 != 0) ok = 0;
    return ok;
}

// Test multiple _Alignas locals don't overlap
void test_multi_align_no_overlap(void) {
    _Alignas(16) int a[4];
    _Alignas(16) int b[4];
    int i;

    for (i = 0; i < 4; i++) {
        a[i] = i + 1;      // 1,2,3,4
        b[i] = (i + 1) * 10; // 10,20,30,40
    }

    int ok = 1;
    // Verify no overlap: writing to b didn't corrupt a
    for (i = 0; i < 4; i++) {
        if (a[i] != i + 1) ok = 0;
        if (b[i] != (i + 1) * 10) ok = 0;
    }
    // Verify they don't overlap in memory
    unsigned long addr_a = (unsigned long)a;
    unsigned long addr_b = (unsigned long)b;
    long diff = (long)(addr_a - addr_b);
    if (diff < 0) diff = -diff;
    if (diff < 16) ok = 0; // They must be at least 16 bytes apart

    printf("multi_align_ok=%d\n", ok);
}

int main(void) {
    test_alignof();
    test_struct_align();
    test_local_aligned();
    test_struct_member_align();
    test_array_elem_spacing();
    test_mixed_struct();
    test_packed_equiv();
    test_nested_alignment();
    test_char_after_aligned();
    test_struct_array_align();
    test_multi_aligned();
    test_align_propagation();
    test_offsets();
    printf("func_local_align=%d\n", func_with_aligned_locals(3));
    test_multi_align_no_overlap();
    return 0;
}
