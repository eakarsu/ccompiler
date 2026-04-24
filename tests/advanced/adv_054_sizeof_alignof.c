
// EXPECT: === sizeof and _Alignof ===
// EXPECT: sizeof(char)=1
// EXPECT: sizeof(int)=4
// EXPECT: sizeof(long)=8
// EXPECT: sizeof(char*)=8
// EXPECT: sizeof(int*)=8
// EXPECT: sizeof(void*)=8
// EXPECT: sizeof_arr10: 40
// EXPECT: sizeof_arr_chars: 5
// EXPECT: sizeof_expr: 4
// EXPECT: sizeof_struct_basic: 8
// EXPECT: sizeof_struct_padded: 12
// EXPECT: sizeof_struct_packed_fields: 24
// EXPECT: sizeof_nested: 16
// EXPECT: sizeof_array_of_struct: 32
// EXPECT: sizeof_ptr_vs_arr: ptr=8 arr=20
// EXPECT: sizeof_deref: 4
// EXPECT: alignof_char: 1
// EXPECT: alignof_int: 4
// EXPECT: alignof_long: 8
// EXPECT: alignof_ptr: 8
// EXPECT: alignof_struct: 4
// EXPECT: alignof_struct_with_long: 8
// EXPECT: array_from_sizeof: 5 elements OK
// EXPECT: sizeof_in_loop: total_bytes=40
// EXPECT: sizeof_conditional: 4
// EXPECT: sizeof_no_eval: x unchanged=10
// EXPECT: sizeof_string_literal: 6
// EXPECT: sizeof_union: 8
// EXPECT: struct_offset_check: OK
int printf(const char *fmt, ...);

struct Basic {
    int x;
    int y;
};

struct Padded {
    char a;
    // 3 bytes padding
    int b;
    char c;
    // 3 bytes padding to align struct size to 4
};

struct ThreeFields {
    long a;
    int b;
    long c;
};

struct Nested {
    struct Basic inner;
    int z;
    int w;
};

struct WithLong {
    char a;
    long b;
};

union IntOrLong {
    int i;
    long l;
};

struct ForOffset {
    char a;
    int b;
    char c;
    long d;
};

int main(void) {
    printf("=== sizeof and _Alignof ===\n");

    // Basic type sizes
    printf("sizeof(char)=%d\n", (int)sizeof(char));
    printf("sizeof(int)=%d\n", (int)sizeof(int));
    printf("sizeof(long)=%d\n", (int)sizeof(long));

    // Pointer sizes (always 8 on ARM64)
    printf("sizeof(char*)=%d\n", (int)sizeof(char *));
    printf("sizeof(int*)=%d\n", (int)sizeof(int *));
    printf("sizeof(void*)=%d\n", (int)sizeof(void *));

    // Array sizes
    {
        int arr[10];
        printf("sizeof_arr10: %d\n", (int)sizeof(arr));
    }
    {
        char s[5];
        printf("sizeof_arr_chars: %d\n", (int)sizeof(s));
    }

    // sizeof expression (doesn't need parens for exprs)
    {
        int x = 42;
        printf("sizeof_expr: %d\n", (int)sizeof(x));
    }

    // Struct sizes
    printf("sizeof_struct_basic: %d\n", (int)sizeof(struct Basic));
    printf("sizeof_struct_padded: %d\n", (int)sizeof(struct Padded));
    printf("sizeof_struct_packed_fields: %d\n", (int)sizeof(struct ThreeFields));

    // Nested struct
    printf("sizeof_nested: %d\n", (int)sizeof(struct Nested));

    // Array of structs
    {
        struct Basic arr[4];
        printf("sizeof_array_of_struct: %d\n", (int)sizeof(arr));
    }

    // Pointer vs array sizeof
    {
        int arr[5];
        int *p = arr;
        printf("sizeof_ptr_vs_arr: ptr=%d arr=%d\n",
               (int)sizeof(p), (int)sizeof(arr));
    }

    // sizeof dereferenced pointer
    {
        int x = 10;
        int *p = &x;
        printf("sizeof_deref: %d\n", (int)sizeof(*p));
    }

    // _Alignof basic types
    printf("alignof_char: %d\n", (int)_Alignof(char));
    printf("alignof_int: %d\n", (int)_Alignof(int));
    printf("alignof_long: %d\n", (int)_Alignof(long));
    printf("alignof_ptr: %d\n", (int)_Alignof(int *));

    // _Alignof structs
    printf("alignof_struct: %d\n", (int)_Alignof(struct Basic));
    printf("alignof_struct_with_long: %d\n", (int)_Alignof(struct WithLong));

    // Use sizeof to compute array element count
    {
        int data[5];
        data[0] = 1;
        data[1] = 2;
        data[2] = 3;
        data[3] = 4;
        data[4] = 5;
        int n = (int)(sizeof(data) / sizeof(data[0]));
        int ok = (n == 5);
        printf("array_from_sizeof: %d elements %s\n", n, ok ? "OK" : "FAIL");
    }

    // sizeof in a loop
    {
        int arr[10];
        int total = 0;
        int i;
        for (i = 0; i < 10; i++) {
            total += (int)sizeof(int);
        }
        printf("sizeof_in_loop: total_bytes=%d\n", total);
    }

    // sizeof in conditional
    {
        int val = (sizeof(int) == 4) ? 4 : 0;
        printf("sizeof_conditional: %d\n", val);
    }

    // sizeof does NOT evaluate its operand
    {
        int x = 10;
        int s = (int)sizeof(x = 20);  // x should remain 10
        printf("sizeof_no_eval: x unchanged=%d\n", x);
    }

    // sizeof string literal (includes null terminator)
    {
        printf("sizeof_string_literal: %d\n", (int)sizeof("hello"));
    }

    // sizeof union
    {
        printf("sizeof_union: %d\n", (int)sizeof(union IntOrLong));
    }

    // Struct offset consistency check
    {
        struct ForOffset obj;
        obj.a = 1;
        obj.b = 2;
        obj.c = 3;
        obj.d = 4;
        char *base = (char *)&obj;
        // We can't compute offsetof without macros, but we can verify
        // that reading back through the struct pointer works
        int ok = (obj.a == 1 && obj.b == 2 && obj.c == 3 && obj.d == 4);
        printf("struct_offset_check: %s\n", ok ? "OK" : "FAIL");
    }

    return 0;
}
