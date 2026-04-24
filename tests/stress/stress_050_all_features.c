// stress_050_all_features.c - Kitchen sink: all C features in one program
// EXPECT: enum_val=3
// EXPECT: union_reinterpret=1
// EXPECT: bitfield_pack=7
// EXPECT: fptr_call=25
// EXPECT: compound_lit=30
// EXPECT: desig_init=42
// EXPECT: generic_type=1
// EXPECT: static_assert_ok=1
// EXPECT: bool_logic=1
// EXPECT: goto_result=100
// EXPECT: dowhile_sum=55
// EXPECT: ternary_chain=3
// EXPECT: macro_concat=77
// EXPECT: macro_str=5
// EXPECT: nested_struct=999
// EXPECT: switch_fall=6
// EXPECT: vla_sum=45
// EXPECT: all_verified=1

int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

// --- Enum with explicit values ---
enum Color { RED = 1, GREEN = 2, BLUE = 3, ALPHA = 255 };

// --- Union ---
union IntBytes {
    int ival;
    char bytes[4];
};

// --- Struct with bitfields ---
struct Flags {
    unsigned int a : 1;
    unsigned int b : 2;
    unsigned int c : 4;
};

// --- Function pointer typedef ---
typedef int (*MathOp)(int, int);

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }

// --- Nested struct ---
struct Inner {
    int x;
    int y;
};

struct Outer {
    struct Inner in;
    int z;
};

// --- _Generic macro ---
#define TYPE_ID(x) _Generic((x), \
    int: 1,                       \
    char: 2,                      \
    int *: 3,                     \
    default: 0                    \
)

// --- _Static_assert ---
_Static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");
_Static_assert(sizeof(char) == 1, "char must be 1 byte");

// --- Token pasting and stringize macros ---
#define PASTE(a, b) a##b
#define STR(x) #x
#define XSTR(x) STR(x)

int PASTE(var, 77) = 77;  // creates var77

int str_len(const char *s) {
    int n = 0;
    while (s[n] != 0) n++;
    return n;
}

// --- Complex function using many features ---
int compute_all(int n) {
    // Ternary chain
    int category = (n < 0) ? 0 : (n < 10) ? 1 : (n < 100) ? 2 : 3;
    return category;
}

int main(void) {
    int verify_all = 1;

    // --- Enum ---
    enum Color c = BLUE;
    printf("enum_val=%d\n", c);
    if (c != 3) verify_all = 0;

    // --- Union reinterpret ---
    {
        union IntBytes ub;
        ub.ival = 0;
        ub.bytes[0] = 1;  // Set least significant byte on little-endian
        printf("union_reinterpret=%d\n", ub.ival);
        if (ub.ival != 1) verify_all = 0;
    }

    // --- Bitfields ---
    {
        struct Flags f;
        f.a = 1;   // 1 bit: value 1
        f.b = 3;   // 2 bits: value 3
        f.c = 15;  // 4 bits: value 15
        // Just verify individual fields
        int sum = f.a + f.b + f.c;  // 1+3+15=19... EXPECT says 7
        // Let me use: f.a=1, f.b=2, f.c=4 -> sum=7
        f.a = 1;
        f.b = 2;
        f.c = 4;
        printf("bitfield_pack=%d\n", f.a + f.b + f.c);
        if (f.a + f.b + f.c != 7) verify_all = 0;
    }

    // --- Function pointer ---
    {
        MathOp op = mul;
        printf("fptr_call=%d\n", op(5, 5));
        if (op(5, 5) != 25) verify_all = 0;
    }

    // --- Compound literal ---
    {
        struct Inner p = (struct Inner){10, 20};
        printf("compound_lit=%d\n", p.x + p.y);
        if (p.x + p.y != 30) verify_all = 0;
    }

    // --- Designated initializer ---
    {
        struct Outer o = { .in = {.x = 42, .y = 0}, .z = 0 };
        printf("desig_init=%d\n", o.in.x);
        if (o.in.x != 42) verify_all = 0;
    }

    // --- _Generic ---
    {
        int v = 10;
        printf("generic_type=%d\n", TYPE_ID(v));
        if (TYPE_ID(v) != 1) verify_all = 0;
    }

    // --- _Static_assert passed (we just confirm we got here) ---
    printf("static_assert_ok=%d\n", 1);

    // --- _Bool ---
    {
        _Bool t = 1;
        _Bool f = 0;
        _Bool result = t && !f;
        printf("bool_logic=%d\n", result);
        if (result != 1) verify_all = 0;
    }

    // --- goto ---
    {
        int val = 0;
        goto skip_over;
        val = 50;  // should be skipped
    skip_over:
        val = 100;
        printf("goto_result=%d\n", val);
        if (val != 100) verify_all = 0;
    }

    // --- do-while ---
    {
        int sum = 0;
        int i = 1;
        do {
            sum = sum + i;
            i++;
        } while (i <= 10);
        printf("dowhile_sum=%d\n", sum);
        if (sum != 55) verify_all = 0;
    }

    // --- Ternary chain ---
    {
        int val = 500;
        printf("ternary_chain=%d\n", compute_all(val));
        if (compute_all(val) != 3) verify_all = 0;
    }

    // --- Macro concat ---
    printf("macro_concat=%d\n", var77);
    if (var77 != 77) verify_all = 0;

    // --- Macro stringize ---
    // STR(hello) = "hello", length = 5
    printf("macro_str=%d\n", str_len(STR(hello)));
    if (str_len(STR(hello)) != 5) verify_all = 0;

    // --- Nested struct access ---
    {
        struct Outer o;
        o.in.x = 999;
        o.in.y = 888;
        o.z = 777;
        printf("nested_struct=%d\n", o.in.x);
        if (o.in.x != 999) verify_all = 0;
    }

    // --- Switch with fallthrough ---
    {
        int val = 1;
        int sum = 0;
        switch (val) {
            case 1: sum = sum + 1;
            case 2: sum = sum + 2;
            case 3: sum = sum + 3;
                break;
            default: sum = 99;
        }
        // fallthrough from 1: 1+2+3=6
        printf("switch_fall=%d\n", sum);
        if (sum != 6) verify_all = 0;
    }

    // --- VLA (variable-length array) ---
    {
        int n = 10;
        int vla[n];
        int i;
        for (i = 0; i < n; i++) vla[i] = i;
        int sum = 0;
        for (i = 0; i < n; i++) sum = sum + vla[i];
        printf("vla_sum=%d\n", sum);  // 0+1+...+9=45
        if (sum != 45) verify_all = 0;
    }

    // --- Final verification ---
    printf("all_verified=%d\n", verify_all);

    return 0;
}
