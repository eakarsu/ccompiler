// EXPECT: sizeof_int=4
// EXPECT: sizeof_char=1
// EXPECT: sizeof_ptr=8
// EXPECT: sizeof_long=8
// EXPECT: sizeof_short=2
// EXPECT: sizeof_arr=40
// EXPECT: struct_size_ok=1
// EXPECT: align_ok=1
// EXPECT: enum_size=4
// EXPECT: expr_check=1
// EXPECT: shift_check=1
// EXPECT: negative_check=1
// EXPECT: char_range=1
// EXPECT: multi_assert=1
// EXPECT: sizeof_nested=1
// EXPECT: const_arith=1
// EXPECT: all_passed=1

int printf(const char *fmt, ...);

// Basic sizeof checks
_Static_assert(sizeof(int) == 4, "int must be 4 bytes");
_Static_assert(sizeof(char) == 1, "char must be 1 byte");
_Static_assert(sizeof(short) == 2, "short must be 2 bytes");
_Static_assert(sizeof(long) == 8, "long must be 8 bytes on 64-bit");

// Pointer size
_Static_assert(sizeof(void *) == 8, "pointers must be 8 bytes on 64-bit");
_Static_assert(sizeof(int *) == 8, "int pointer must be 8 bytes");

// Array sizeof
_Static_assert(sizeof(int[10]) == 40, "int[10] must be 40 bytes");

// Struct size checks
struct SmallStruct {
    int x;
    int y;
};
_Static_assert(sizeof(struct SmallStruct) == 8, "SmallStruct must be 8 bytes");

struct CharStruct {
    char a;
    char b;
    char c;
    char d;
};
_Static_assert(sizeof(struct CharStruct) == 4, "CharStruct must be 4 bytes");

// Alignment-related size check
struct Padded {
    char c;
    int i;
};
// On most platforms, this is 8 due to padding
_Static_assert(sizeof(struct Padded) >= 5, "Padded struct must be at least 5 bytes");

// Enum size
enum Color { RED, GREEN, BLUE };
_Static_assert(sizeof(enum Color) == 4, "enum must be 4 bytes");

// Constant expression checks
_Static_assert(100 > 0, "100 must be positive");
_Static_assert(1 + 1 == 2, "basic arithmetic");
_Static_assert(10 * 10 == 100, "multiplication");
_Static_assert(256 / 16 == 16, "division");

// Shift checks
_Static_assert((1 << 8) == 256, "shift left");
_Static_assert((256 >> 4) == 16, "shift right");

// Negative / sign checks
_Static_assert(-1 < 0, "negative check");
_Static_assert(-(-5) == 5, "double negation");

// Char range
_Static_assert(sizeof(char) * 8 == 8, "char must be 8 bits");

// Multiple assertions on same type
_Static_assert(sizeof(int) >= 2, "int at least 2 bytes");
_Static_assert(sizeof(int) <= 8, "int at most 8 bytes");
_Static_assert(sizeof(int) == 4, "int exactly 4 bytes");

// Nested struct sizeof
struct Outer {
    struct SmallStruct inner;
    int z;
};
_Static_assert(sizeof(struct Outer) >= 12, "Outer must be at least 12 bytes");

// Constant arithmetic in assert
_Static_assert((3 + 4) * 2 == 14, "constant arithmetic check");
_Static_assert(100 - 50 + 25 == 75, "subtraction and addition");

int main(void) {
    int passed = 1;

    printf("sizeof_int=%d\n", (int)sizeof(int));
    if (sizeof(int) != 4) passed = 0;

    printf("sizeof_char=%d\n", (int)sizeof(char));
    if (sizeof(char) != 1) passed = 0;

    printf("sizeof_ptr=%d\n", (int)sizeof(void *));
    if (sizeof(void *) != 8) passed = 0;

    printf("sizeof_long=%d\n", (int)sizeof(long));
    if (sizeof(long) != 8) passed = 0;

    printf("sizeof_short=%d\n", (int)sizeof(short));
    if (sizeof(short) != 2) passed = 0;

    int arr[10];
    printf("sizeof_arr=%d\n", (int)sizeof(arr));
    if (sizeof(arr) != 40) passed = 0;

    printf("struct_size_ok=%d\n", (int)(sizeof(struct SmallStruct) == 8));
    if (sizeof(struct SmallStruct) != 8) passed = 0;

    printf("align_ok=%d\n", (int)(sizeof(struct Padded) >= 5));

    printf("enum_size=%d\n", (int)sizeof(enum Color));
    if (sizeof(enum Color) != 4) passed = 0;

    printf("expr_check=%d\n", (1 + 1 == 2) ? 1 : 0);

    printf("shift_check=%d\n", ((1 << 8) == 256) ? 1 : 0);

    printf("negative_check=%d\n", (-1 < 0) ? 1 : 0);

    printf("char_range=%d\n", (sizeof(char) * 8 == 8) ? 1 : 0);

    printf("multi_assert=%d\n", (sizeof(int) == 4) ? 1 : 0);

    printf("sizeof_nested=%d\n", (sizeof(struct Outer) >= 12) ? 1 : 0);

    printf("const_arith=%d\n", ((3 + 4) * 2 == 14) ? 1 : 0);

    printf("all_passed=%d\n", passed);
    return passed ? 0 : 1;
}
