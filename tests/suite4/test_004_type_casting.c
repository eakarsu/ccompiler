int printf(const char *fmt, ...);

// Test explicit type casts: int to char, char to int, pointer casts,
// truncation behavior, and widening conversions.

struct Pair {
    int first;
    int second;
};

union IntChar {
    int i;
    char c;
};

int widen_char(char c) {
    int i = (int)c;
    return i;
}

char narrow_int(int i) {
    char c = (char)i;
    return c;
}

int cast_chain(int val) {
    // int -> char -> int (truncation then sign extension)
    char c = (char)val;
    int result = (int)c;
    return result;
}

int pointer_to_int_via_array(int *arr, int idx) {
    // Access through pointer arithmetic
    int *p = arr + idx;
    return *p;
}

int cast_and_add(int a, char b) {
    return a + (int)b;
}

int char_array_sum(char *arr, int len) {
    int sum = 0;
    int i;
    for (i = 0; i < len; i = i + 1) {
        sum = sum + (int)arr[i];
    }
    return sum;
}

int truncate_and_compare(int big, int small_target) {
    char truncated = (char)big;
    return (int)truncated == small_target;
}

int main(void) {
    // Basic widening: char -> int
    char c1 = 42;
    // EXPECT: widen 42 = 42
    printf("widen %d = %d\n", 42, widen_char(c1));

    // Widening negative char
    char c2 = (char)(-5);
    // EXPECT: widen -5 = -5
    printf("widen -5 = %d\n", widen_char(c2));

    // Narrowing: int 300 -> char
    // 300 = 256 + 44, so (char)300 = 44
    // EXPECT: narrow 300 = 44
    printf("narrow 300 = %d\n", (int)narrow_int(300));

    // Narrowing: int -1 -> char -> int
    // EXPECT: cast_chain(-1) = -1
    printf("cast_chain(-1) = %d\n", cast_chain(-1));

    // Narrowing: int 128 -> char -> int (wraps to -128)
    // EXPECT: cast_chain(128) = -128
    printf("cast_chain(128) = %d\n", cast_chain(128));

    // Narrowing: int 255 -> char -> int (wraps to -1)
    // EXPECT: cast_chain(255) = -1
    printf("cast_chain(255) = %d\n", cast_chain(255));

    // Narrowing: int 256 -> char -> int (wraps to 0)
    // EXPECT: cast_chain(256) = 0
    printf("cast_chain(256) = %d\n", cast_chain(256));

    // Narrowing: int 1000 -> char -> int
    // 1000 % 256 = 232, as signed char = 232 - 256 = -24
    // EXPECT: cast_chain(1000) = -24
    printf("cast_chain(1000) = %d\n", cast_chain(1000));

    // Pointer arithmetic through cast
    int arr[5];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;

    // EXPECT: arr[0] = 10
    printf("arr[0] = %d\n", pointer_to_int_via_array(arr, 0));

    // EXPECT: arr[3] = 40
    printf("arr[3] = %d\n", pointer_to_int_via_array(arr, 3));

    // EXPECT: arr[4] = 50
    printf("arr[4] = %d\n", pointer_to_int_via_array(arr, 4));

    // Cast in arithmetic
    char b = 100;
    // EXPECT: 500 + 100 = 600
    printf("500 + 100 = %d\n", cast_and_add(500, b));

    // Negative char in arithmetic
    char neg = (char)(-50);
    // EXPECT: 200 + -50 = 150
    printf("200 + -50 = %d\n", cast_and_add(200, neg));

    // Char array sum with casts
    char chars[4];
    chars[0] = 10;
    chars[1] = 20;
    chars[2] = 30;
    chars[3] = 40;
    // EXPECT: char_sum = 100
    printf("char_sum = %d\n", char_array_sum(chars, 4));

    // Truncation comparison
    // 300 truncated = 44, compare with 44
    // EXPECT: trunc(300)==44 = 1
    printf("trunc(300)==44 = %d\n", truncate_and_compare(300, 44));

    // 256 truncated = 0, compare with 0
    // EXPECT: trunc(256)==0 = 1
    printf("trunc(256)==0 = %d\n", truncate_and_compare(256, 0));

    // Union type punning: write int, read char
    union IntChar u;
    u.i = 0;
    u.i = 65; // 'A' in lowest byte (little-endian)
    // EXPECT: union char = 65
    printf("union char = %d\n", (int)u.c);

    u.i = 258; // 258 = 256 + 2, lowest byte = 2
    // EXPECT: union low byte = 2
    printf("union low byte = %d\n", (int)u.c);

    // Struct field access with casts
    struct Pair p;
    p.first = 1000;
    p.second = -500;
    char cf = (char)p.first;  // 1000 % 256 = 232, signed = -24
    char cs = (char)p.second; // -500: 500 = 256+244, so lowest byte=12 unsigned; -500 & 0xFF = 12, signed = 12
    // EXPECT: (char)1000 = -24
    printf("(char)1000 = %d\n", (int)cf);

    // -500 in binary: ...1111_1110_0000_1100, lowest byte = 0x0C = 12
    // EXPECT: (char)-500 = 12
    printf("(char)-500 = %d\n", (int)cs);

    // Series of casts
    int big = 100000;
    char small = (char)big;
    int back = (int)small;
    // 100000 = 0x186A0, lowest byte = 0xA0 = 160, signed char = -96
    // EXPECT: 100000 -> char -> int = -96
    printf("100000 -> char -> int = %d\n", back);

    // Cast zero
    // EXPECT: (char)0 = 0
    printf("(char)0 = %d\n", (int)(char)0);

    // Cast preserves through identity
    int v = 42;
    // EXPECT: (int)42 = 42
    printf("(int)42 = %d\n", (int)v);

    return 0;
}
