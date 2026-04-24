// stress_036_sizeof_dynamic.c - sizeof stress tests
// EXPECT: sizeof_char=1
// EXPECT: sizeof_int=4
// EXPECT: sizeof_long=8
// EXPECT: sizeof_charptr=8
// EXPECT: sizeof_intptr=8
// EXPECT: sizeof_longptr=8
// EXPECT: sizeof_arr10=40
// EXPECT: sizeof_arr3=12
// EXPECT: sizeof_chararr=20
// EXPECT: sizeof_padded=12
// EXPECT: sizeof_packed_min=8
// EXPECT: sizeof_nested=16
// EXPECT: no_side_effect=0
// EXPECT: no_side_effect2=10
// EXPECT: malloc_10_ints=40
// EXPECT: typedef_size=4
// EXPECT: enum_size=4
// EXPECT: sizeof_ptr_to_arr=8
// EXPECT: sizeof_struct_arr=48
// EXPECT: sizeof_expr_type=4
// EXPECT: sizeof_deref_ptr=4
// EXPECT: sizeof_union=8
// EXPECT: sizeof_void_ptr=8
// EXPECT: sizeof_func_ptr=8
// EXPECT: sizeof_2d_arr=24
// EXPECT: sizeof_char_in_struct=2
// EXPECT: sizeof_self_ref=16
// EXPECT: pass_done=1

int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

// Struct with known padding
struct Padded {
    char a;    // 1 byte + 3 padding
    int b;     // 4 bytes
    char c;    // 1 byte + 3 padding
};

// Struct with minimal size (8 bytes: int + int)
struct PackedMin {
    int x;
    int y;
};

// Nested struct
struct Inner { int a; int b; };
struct Nested {
    struct Inner inner;
    int c;
    int d;
};

// Typedef
typedef int MyInt;

// Enum
enum Color { RED, GREEN, BLUE };

// Struct array
struct Item { int id; long value; };

// 2D array
typedef int Row[6];

// Char-only struct
struct TwoChars { char a; char b; };

// Self-referential
struct SelfRef {
    int val;
    struct SelfRef *next;
};

int main(void) {
    // Basic types
    printf("sizeof_char=%d\n", (int)sizeof(char));
    printf("sizeof_int=%d\n", (int)sizeof(int));
    printf("sizeof_long=%d\n", (int)sizeof(long));

    // Pointers to types (all 8 on 64-bit)
    printf("sizeof_charptr=%d\n", (int)sizeof(char *));
    printf("sizeof_intptr=%d\n", (int)sizeof(int *));
    printf("sizeof_longptr=%d\n", (int)sizeof(long *));

    // Arrays
    int arr10[10];
    printf("sizeof_arr10=%d\n", (int)sizeof(arr10));
    int arr3[3];
    printf("sizeof_arr3=%d\n", (int)sizeof(arr3));
    char chararr[20];
    printf("sizeof_chararr=%d\n", (int)sizeof(chararr));

    // Struct with padding
    printf("sizeof_padded=%d\n", (int)sizeof(struct Padded));

    // Struct packed minimum
    printf("sizeof_packed_min=%d\n", (int)sizeof(struct PackedMin));

    // Nested struct
    printf("sizeof_nested=%d\n", (int)sizeof(struct Nested));

    // sizeof should NOT evaluate side effects
    int no_se = 0;
    int dummy = (int)sizeof(no_se++);
    printf("no_side_effect=%d\n", no_se); // should still be 0

    int arr_se[10];
    int se_idx = 10;
    int dummy2 = (int)sizeof(arr_se[se_idx++]); // should not evaluate index
    printf("no_side_effect2=%d\n", se_idx); // should still be 10

    // sizeof in malloc argument
    int *heap = malloc(10 * sizeof(int));
    printf("malloc_10_ints=%d\n", (int)(10 * sizeof(int)));
    free(heap);

    // typedef
    printf("typedef_size=%d\n", (int)sizeof(MyInt));

    // enum
    printf("enum_size=%d\n", (int)sizeof(enum Color));

    // pointer to array
    int (*parr)[5];
    printf("sizeof_ptr_to_arr=%d\n", (int)sizeof(parr));

    // struct array
    struct Item items[3];
    // sizeof(struct Item) = 16 (int=4 + padding=4 + long=8), 3*16=48
    printf("sizeof_struct_arr=%d\n", (int)sizeof(items));

    // sizeof on expression (should give type of result)
    int sv = 10;
    printf("sizeof_expr_type=%d\n", (int)sizeof(sv + 1));

    // sizeof dereferenced pointer
    int *sp = &sv;
    printf("sizeof_deref_ptr=%d\n", (int)sizeof(*sp));

    // sizeof union
    union UU { int a; long b; };
    printf("sizeof_union=%d\n", (int)sizeof(union UU));

    // sizeof void pointer
    printf("sizeof_void_ptr=%d\n", (int)sizeof(void *));

    // sizeof function pointer
    int (*fptr)(int);
    printf("sizeof_func_ptr=%d\n", (int)sizeof(fptr));

    // 2D array
    int arr2d[2][3];
    printf("sizeof_2d_arr=%d\n", (int)sizeof(arr2d));

    // Struct with only chars (padding check)
    printf("sizeof_char_in_struct=%d\n", (int)sizeof(struct TwoChars));

    // Self-referential struct: int(4) + padding(4) + ptr(8) = 16
    printf("sizeof_self_ref=%d\n", (int)sizeof(struct SelfRef));

    printf("pass_done=1\n");
    return 0;
}
