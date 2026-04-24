int printf(const char *fmt, ...);

// Test sizeof for basic types, arrays, structs, unions, pointers, and expressions.
// Assumes: int=4, char=1, pointer=8 (64-bit ARM64).

struct Empty {
    char dummy;
};

struct OneInt {
    int x;
};

struct TwoInts {
    int a;
    int b;
};

struct Mixed {
    char c;
    int i;
};

struct Nested {
    struct TwoInts pair;
    int extra;
};

struct CharArray {
    char data[10];
};

struct Large {
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    int h;
};

union IntOrChar {
    int i;
    char c;
};

union TwoIntsU {
    int a;
    int b;
};

union Mixed2 {
    int i;
    char arr[8];
};

struct WithPointer {
    int *p;
    int val;
};

struct ThreeChars {
    char a;
    char b;
    char c;
};

typedef int MyInt;
typedef char MyChar;
typedef struct TwoInts Pair;

int main(void) {
    // Basic type sizes
    // EXPECT: sizeof(char) = 1
    printf("sizeof(char) = %d\n", (int)sizeof(char));

    // EXPECT: sizeof(int) = 4
    printf("sizeof(int) = %d\n", (int)sizeof(int));

    // Pointer sizes (64-bit)
    int *ip;
    // EXPECT: sizeof(int*) = 8
    printf("sizeof(int*) = %d\n", (int)sizeof(ip));

    char *cp;
    // EXPECT: sizeof(char*) = 8
    printf("sizeof(char*) = %d\n", (int)sizeof(cp));

    // Array sizes
    int arr5[5];
    // EXPECT: sizeof(int[5]) = 20
    printf("sizeof(int[5]) = %d\n", (int)sizeof(arr5));

    char carr10[10];
    // EXPECT: sizeof(char[10]) = 10
    printf("sizeof(char[10]) = %d\n", (int)sizeof(carr10));

    int arr1[1];
    // EXPECT: sizeof(int[1]) = 4
    printf("sizeof(int[1]) = %d\n", (int)sizeof(arr1));

    // Struct sizes
    // EXPECT: sizeof(OneInt) = 4
    printf("sizeof(OneInt) = %d\n", (int)sizeof(struct OneInt));

    // EXPECT: sizeof(TwoInts) = 8
    printf("sizeof(TwoInts) = %d\n", (int)sizeof(struct TwoInts));

    // Mixed struct: char + padding + int = 8 (with alignment padding)
    // EXPECT: sizeof(Mixed) = 8
    printf("sizeof(Mixed) = %d\n", (int)sizeof(struct Mixed));

    // Nested struct: TwoInts(8) + int(4) = 12
    // EXPECT: sizeof(Nested) = 12
    printf("sizeof(Nested) = %d\n", (int)sizeof(struct Nested));

    // Struct with char array: 10 bytes, no padding needed for chars
    // EXPECT: sizeof(CharArray) = 10
    printf("sizeof(CharArray) = %d\n", (int)sizeof(struct CharArray));

    // Large struct: 8 ints = 32
    // EXPECT: sizeof(Large) = 32
    printf("sizeof(Large) = %d\n", (int)sizeof(struct Large));

    // Struct with pointer: ptr(8) + int(4) + padding(4) = 16
    // EXPECT: sizeof(WithPointer) = 16
    printf("sizeof(WithPointer) = %d\n", (int)sizeof(struct WithPointer));

    // Three chars: 3 bytes (no alignment requirement beyond 1)
    // EXPECT: sizeof(ThreeChars) = 3
    printf("sizeof(ThreeChars) = %d\n", (int)sizeof(struct ThreeChars));

    // Union sizes (largest member)
    // EXPECT: sizeof(IntOrChar) = 4
    printf("sizeof(IntOrChar) = %d\n", (int)sizeof(union IntOrChar));

    // EXPECT: sizeof(TwoIntsU) = 4
    printf("sizeof(TwoIntsU) = %d\n", (int)sizeof(union TwoIntsU));

    // Union with char[8] and int: max(4, 8) = 8
    // EXPECT: sizeof(Mixed2) = 8
    printf("sizeof(Mixed2) = %d\n", (int)sizeof(union Mixed2));

    // Typedef sizes
    MyInt mi;
    // EXPECT: sizeof(MyInt) = 4
    printf("sizeof(MyInt) = %d\n", (int)sizeof(mi));

    MyChar mc;
    // EXPECT: sizeof(MyChar) = 1
    printf("sizeof(MyChar) = %d\n", (int)sizeof(mc));

    Pair pp;
    // EXPECT: sizeof(Pair) = 8
    printf("sizeof(Pair) = %d\n", (int)sizeof(pp));

    // sizeof on expressions (does not evaluate)
    int x = 10;
    // EXPECT: sizeof(x) = 4
    printf("sizeof(x) = %d\n", (int)sizeof(x));

    // EXPECT: sizeof(x+1) = 4
    printf("sizeof(x+1) = %d\n", (int)sizeof(x + 1));

    char ch = 5;
    // sizeof(ch) is 1, but sizeof(ch+1) is 4 due to integer promotion
    // EXPECT: sizeof(ch) = 1
    printf("sizeof(ch) = %d\n", (int)sizeof(ch));

    // Element count via sizeof trick
    int data[8];
    int count = (int)sizeof(data) / (int)sizeof(data[0]);
    // EXPECT: count(data) = 8
    printf("count(data) = %d\n", count);

    char str[20];
    int scount = (int)sizeof(str) / (int)sizeof(str[0]);
    // EXPECT: count(str) = 20
    printf("count(str) = %d\n", scount);

    return 0;
}
