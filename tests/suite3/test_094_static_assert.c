int printf(const char *fmt, ...);

/* _Static_assert: compile-time checks, program prints confirmation if passed */

/* Basic size assertions - these must hold on any C11 conforming implementation */
_Static_assert(sizeof(char) == 1, "char must be 1 byte");
_Static_assert(sizeof(short) >= 2, "short must be at least 2 bytes");
_Static_assert(sizeof(int) >= 2, "int must be at least 2 bytes");
_Static_assert(sizeof(long) >= 4, "long must be at least 4 bytes");
_Static_assert(sizeof(long long) >= 8, "long long must be at least 8 bytes");

/* Pointer size assertions */
_Static_assert(sizeof(void *) == sizeof(int *), "pointer sizes must match");
_Static_assert(sizeof(char *) == sizeof(int *), "char* and int* same size");

/* Integer type relationship assertions */
_Static_assert(sizeof(short) <= sizeof(int), "short <= int");
_Static_assert(sizeof(int) <= sizeof(long), "int <= long");
_Static_assert(sizeof(long) <= sizeof(long long), "long <= long long");

/* Constant expression assertions */
_Static_assert(1 + 1 == 2, "basic arithmetic");
_Static_assert(2 * 3 == 6, "multiplication");
_Static_assert(10 / 2 == 5, "division");
_Static_assert(10 % 3 == 1, "modulo");
_Static_assert(1 << 3 == 8, "left shift");
_Static_assert(16 >> 2 == 4, "right shift");

/* Boolean constant expressions */
_Static_assert(1, "literal 1 is true");
_Static_assert(0 == 0, "zero equals zero");
_Static_assert(1 != 0, "one not equal zero");
_Static_assert(-1 < 0, "negative is less than zero");
_Static_assert(100 > 0, "positive is greater than zero");

/* Bitwise constant expressions */
_Static_assert((0xFF & 0x0F) == 0x0F, "bitwise AND");
_Static_assert((0xF0 | 0x0F) == 0xFF, "bitwise OR");
_Static_assert((0xFF ^ 0xFF) == 0, "bitwise XOR self");
_Static_assert((~0 & 0xFF) == 0xFF, "complement and mask");

/* sizeof in struct contexts */
struct TwoInts { int a; int b; };
_Static_assert(sizeof(struct TwoInts) >= 2 * sizeof(int), "struct size >= sum of members");

struct SingleChar { char c; };
_Static_assert(sizeof(struct SingleChar) >= sizeof(char), "struct with char");

/* Nested constant expressions */
_Static_assert((2 + 3) * (4 - 1) == 15, "nested arithmetic: (2+3)*(4-1)=5*3=15");
_Static_assert(((1 + 2) * 3 - 1) == 8, "nested: (1+2)*3-1=9-1=8");

/* Logical constant expressions */
_Static_assert((1 && 1) == 1, "logical AND true");
_Static_assert((1 && 0) == 0, "logical AND false");
_Static_assert((0 || 1) == 1, "logical OR true");
_Static_assert((0 || 0) == 0, "logical OR false");
_Static_assert(!0 == 1, "logical NOT zero");
_Static_assert(!1 == 0, "logical NOT one");

/* Ternary in constant expression */
_Static_assert((1 ? 42 : 0) == 42, "ternary true branch");
_Static_assert((0 ? 0 : 99) == 99, "ternary false branch");

/* sizeof arithmetic */
_Static_assert(sizeof(int) * 8 >= 16, "int has at least 16 bits");
_Static_assert(sizeof(char) * 8 >= 8, "char has at least 8 bits");

/* Negative constant values */
_Static_assert(-5 + 5 == 0, "negation");
_Static_assert(-3 * -3 == 9, "negative times negative");
_Static_assert(-10 / 2 == -5, "negative division");

int main(void) {
    /* If we reach here, all _Static_assert checks passed at compile time */
    printf("static_assert_ok\n");            // EXPECT: static_assert_ok

    /* Print some sizeof values to confirm runtime behavior */
    printf("%d\n", (int)sizeof(char));       // EXPECT: 1
    printf("%d\n", (int)sizeof(short));      // EXPECT: 2
    printf("%d\n", (int)sizeof(int));        // EXPECT: 4
    printf("%d\n", (int)sizeof(long long));  // EXPECT: 8

    /* Print sizeof pointer (8 on 64-bit) */
    printf("%d\n", (int)sizeof(void *));     // EXPECT: 8

    /* Confirm constant expressions at runtime */
    int a = (1 + 1 == 2) ? 1 : 0;
    printf("%d\n", a);                       // EXPECT: 1

    int b = (sizeof(int) >= 4) ? 1 : 0;
    printf("%d\n", b);                       // EXPECT: 1

    /* sizeof in expressions */
    int arr[10];
    int n = (int)(sizeof(arr) / sizeof(arr[0]));
    printf("%d\n", n);                       // EXPECT: 10

    /* sizeof struct */
    struct TwoInts ti;
    int struct_size = (int)sizeof(ti);
    printf("%d\n", struct_size >= 8 ? 1 : 0); // EXPECT: 1

    /* sizeof char vs int relationship */
    printf("%d\n", (int)sizeof(char) < (int)sizeof(int) ? 1 : 0); // EXPECT: 1

    /* Bitwise constant at runtime */
    int bw = (0xAA & 0x0F);
    printf("%d\n", bw);                      // EXPECT: 10

    int bw2 = (0xA0 | 0x0A);
    printf("%d\n", bw2);                     // EXPECT: 170

    /* sizeof in loop bound */
    int buf[5];
    int i;
    for (i = 0; i < (int)(sizeof(buf)/sizeof(buf[0])); i++) {
        buf[i] = i * i;
    }
    int sum = 0;
    for (i = 0; i < 5; i++) sum += buf[i];
    /* 0+1+4+9+16 = 30 */
    printf("%d\n", sum);                     // EXPECT: 30

    /* sizeof expressions used in computation */
    int word_bits = (int)(sizeof(int) * 8);
    printf("%d\n", word_bits);               // EXPECT: 32

    /* Confirm negative assertions were truly compile-time */
    int neg_check = (-3 * -3 == 9) ? 1 : 0;
    printf("%d\n", neg_check);               // EXPECT: 1

    printf("all_checks_passed\n");           // EXPECT: all_checks_passed

    return 0;
}
