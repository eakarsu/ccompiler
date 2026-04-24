// EXPECT: === Typedef Basic Types ===
// EXPECT: byte val: 65
// EXPECT: word val: 1000
// EXPECT: flag val: 1
// EXPECT: === Typedef Struct ===
// EXPECT: vec: x=3 y=4
// EXPECT: magnitude squared: 25
// EXPECT: === Typedef Function Pointer ===
// EXPECT: add(3,4) = 7
// EXPECT: sub(10,3) = 7
// EXPECT: mul(5,6) = 30
// EXPECT: === Typedef Array ===
// EXPECT: row: 1 2 3 4 5
// EXPECT: === Nested Typedefs ===
// EXPECT: nested ptr value: 42
// EXPECT: === Typedef Enum ===
// EXPECT: direction: 2
// EXPECT: === Typedef with Struct Pointer ===
// EXPECT: node value: 100
// EXPECT: next value: 200
// EXPECT: === Typedef in Function Params ===
// EXPECT: sum of row: 15
// EXPECT: === Typedef Matrix ===
// EXPECT: matrix[0][0]=1 matrix[1][1]=5 matrix[2][2]=9
// EXPECT: === Typedef Chain ===
// EXPECT: chain value: 77

int printf(const char *fmt, ...);

typedef char Byte;
typedef int Word;
typedef int Bool;

typedef struct {
    int x;
    int y;
} Vec2;

typedef int (*BinOp)(int a, int b);

typedef int Row[5];

typedef int *IntPtr;
typedef IntPtr *IntPtrPtr;

enum Dir {
    DIR_NORTH,
    DIR_EAST,
    DIR_SOUTH,
    DIR_WEST
};
typedef enum Dir Direction;

struct ListNode {
    int value;
    struct ListNode *next;
};
typedef struct ListNode *NodePtr;

typedef int Matrix3x3[3][3];

typedef int MyInt;
typedef MyInt MyInt2;
typedef MyInt2 MyInt3;

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }

int sum_row(Row r) {
    int s = 0;
    int i;
    for (i = 0; i < 5; i++) {
        s = s + r[i];
    }
    return s;
}

void test_basic_types(void) {
    printf("=== Typedef Basic Types ===\n");
    Byte b = 65;
    Word w = 1000;
    Bool flag = 1;
    printf("byte val: %d\n", (int)b);
    printf("word val: %d\n", w);
    printf("flag val: %d\n", flag);
}

void test_struct_typedef(void) {
    printf("=== Typedef Struct ===\n");
    Vec2 v;
    v.x = 3;
    v.y = 4;
    printf("vec: x=%d y=%d\n", v.x, v.y);
    int mag2 = v.x * v.x + v.y * v.y;
    printf("magnitude squared: %d\n", mag2);
}

void test_func_ptr_typedef(void) {
    printf("=== Typedef Function Pointer ===\n");
    BinOp op;
    op = add;
    printf("add(3,4) = %d\n", op(3, 4));
    op = sub;
    printf("sub(10,3) = %d\n", op(10, 3));
    op = mul;
    printf("mul(5,6) = %d\n", op(5, 6));
}

void test_array_typedef(void) {
    printf("=== Typedef Array ===\n");
    Row r;
    int i;
    for (i = 0; i < 5; i++) {
        r[i] = i + 1;
    }
    printf("row:");
    for (i = 0; i < 5; i++) {
        printf(" %d", r[i]);
    }
    printf("\n");
}

void test_nested_typedef(void) {
    printf("=== Nested Typedefs ===\n");
    int val = 42;
    IntPtr p = &val;
    IntPtrPtr pp = &p;
    printf("nested ptr value: %d\n", **pp);
}

void test_enum_typedef(void) {
    printf("=== Typedef Enum ===\n");
    Direction d = DIR_SOUTH;
    printf("direction: %d\n", d);
}

void test_struct_ptr_typedef(void) {
    printf("=== Typedef with Struct Pointer ===\n");
    struct ListNode n1;
    struct ListNode n2;
    n1.value = 100;
    n1.next = &n2;
    n2.value = 200;
    n2.next = 0;
    NodePtr p = &n1;
    printf("node value: %d\n", p->value);
    printf("next value: %d\n", p->next->value);
}

void test_func_params(void) {
    printf("=== Typedef in Function Params ===\n");
    Row r;
    int i;
    for (i = 0; i < 5; i++) {
        r[i] = i + 1;
    }
    printf("sum of row: %d\n", sum_row(r));
}

void test_matrix_typedef(void) {
    printf("=== Typedef Matrix ===\n");
    Matrix3x3 m;
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            m[i][j] = i * 3 + j + 1;
        }
    }
    printf("matrix[0][0]=%d matrix[1][1]=%d matrix[2][2]=%d\n",
           m[0][0], m[1][1], m[2][2]);
}

void test_chain(void) {
    printf("=== Typedef Chain ===\n");
    MyInt3 val = 77;
    printf("chain value: %d\n", val);
}

int main(void) {
    test_basic_types();
    test_struct_typedef();
    test_func_ptr_typedef();
    test_array_typedef();
    test_nested_typedef();
    test_enum_typedef();
    test_struct_ptr_typedef();
    test_func_params();
    test_matrix_typedef();
    test_chain();
    return 0;
}
