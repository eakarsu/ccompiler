int printf(const char *fmt, ...);

/* Complex typedef scenarios: basic, pointer, function pointer, struct, array, chained */

/* --- Basic typedef --- */
typedef int Integer;
typedef char Byte;
typedef unsigned int UInt;

/* --- Typedef pointer --- */
typedef int *IntPtr;
typedef char *CStr;
typedef const char *ConstCStr;

/* --- Typedef struct --- */
typedef struct {
    int x;
    int y;
} Vec2;

typedef struct Node {
    int val;
    struct Node *next;
} Node;

/* --- Typedef function pointer --- */
typedef int (*BinaryOp)(int, int);
typedef int (*UnaryOp)(int);
typedef void (*Printer)(int);

/* --- Typedef array --- */
typedef int IntArray5[5];
typedef char Buffer32[32];

/* --- Typedef in typedef (chained) --- */
typedef Integer Count;
typedef IntPtr PInt;
typedef Vec2 Point2D;

/* --- Typedef struct with typedef'd members --- */
typedef struct {
    Vec2 pos;
    Vec2 vel;
    Integer mass;
} Particle;

/* --- Typedef recursive struct --- */
typedef struct List {
    Integer data;
    struct List *next;
} List;

/* --- Functions using typedefs --- */
Integer add(Integer a, Integer b) { return a + b; }
Integer mul(Integer a, Integer b) { return a * b; }
Integer subtract(Integer a, Integer b) { return a - b; }
Integer negate(Integer a) { return -a; }

void print_int(int v) { printf("%d\n", v); }

/* Apply binary op */
Integer apply_bin(BinaryOp op, Integer a, Integer b) { return op(a, b); }
/* Apply unary op */
Integer apply_un(UnaryOp op, Integer a) { return op(a); }

Vec2 vec2_add(Vec2 a, Vec2 b) {
    Vec2 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

/* Sum a typedef'd int array */
Integer sum_arr5(IntArray5 arr) {
    int i;
    Integer s = 0;
    for (i = 0; i < 5; i++) s += arr[i];
    return s;
}

int main(void) {
    /* --- Basic typedef usage --- */
    Integer a = 10, b = 20;
    printf("%d\n", a + b);         // EXPECT: 30

    Byte ch = 65;
    printf("%d\n", (int)ch);       // EXPECT: 65

    Count cnt = 7;
    printf("%d\n", cnt);           // EXPECT: 7

    /* --- Typedef pointer --- */
    Integer val = 42;
    IntPtr p = &val;
    printf("%d\n", *p);            // EXPECT: 42
    *p = 99;
    printf("%d\n", val);           // EXPECT: 99

    PInt pp = &val;
    *pp = 100;
    printf("%d\n", val);           // EXPECT: 100

    /* --- Typedef string pointer --- */
    CStr greeting = "hello";
    printf("%s\n", greeting);      // EXPECT: hello
    ConstCStr cg = "world";
    printf("%s\n", cg);            // EXPECT: world

    /* --- Typedef struct Vec2 --- */
    Vec2 v1;
    v1.x = 3;
    v1.y = 4;
    printf("%d\n", v1.x);         // EXPECT: 3
    printf("%d\n", v1.y);         // EXPECT: 4

    Point2D v2;
    v2.x = 10;
    v2.y = 20;
    Vec2 v3 = vec2_add(v1, v2);
    printf("%d\n", v3.x);         // EXPECT: 13
    printf("%d\n", v3.y);         // EXPECT: 24

    /* --- Typedef function pointer --- */
    BinaryOp fadd = add;
    BinaryOp fmul = mul;
    BinaryOp fsub = subtract;
    printf("%d\n", fadd(5, 6));    // EXPECT: 11
    printf("%d\n", fmul(3, 7));    // EXPECT: 21
    printf("%d\n", fsub(10, 4));   // EXPECT: 6

    UnaryOp fneg = negate;
    printf("%d\n", fneg(15));      // EXPECT: -15

    /* Apply via helper */
    printf("%d\n", apply_bin(add, 8, 9));     // EXPECT: 17
    printf("%d\n", apply_bin(mul, 6, 7));     // EXPECT: 42
    printf("%d\n", apply_un(negate, 25));     // EXPECT: -25

    /* Array of function pointers via typedef */
    BinaryOp ops[3] = {add, mul, subtract};
    printf("%d\n", ops[0](4, 5));  // EXPECT: 9
    printf("%d\n", ops[1](4, 5));  // EXPECT: 20
    printf("%d\n", ops[2](4, 5));  // EXPECT: -1

    /* --- Typedef array --- */
    IntArray5 arr = {10, 20, 30, 40, 50};
    printf("%d\n", arr[0]);        // EXPECT: 10
    printf("%d\n", arr[4]);        // EXPECT: 50
    printf("%d\n", sum_arr5(arr)); // EXPECT: 150

    /* --- Typedef struct Particle --- */
    Particle part;
    part.pos.x = 5;
    part.pos.y = 10;
    part.vel.x = 1;
    part.vel.y = -1;
    part.mass = 3;
    /* Move one step */
    part.pos.x += part.vel.x;
    part.pos.y += part.vel.y;
    printf("%d\n", part.pos.x);   // EXPECT: 6
    printf("%d\n", part.pos.y);   // EXPECT: 9
    printf("%d\n", part.mass);    // EXPECT: 3

    /* --- Typedef recursive linked list --- */
    List n1, n2, n3;
    n1.data = 100; n1.next = &n2;
    n2.data = 200; n2.next = &n3;
    n3.data = 300; n3.next = 0;

    List *cur = &n1;
    Integer lsum = 0;
    while (cur) {
        lsum += cur->data;
        cur = cur->next;
    }
    printf("%d\n", lsum);         // EXPECT: 600

    /* --- Printer typedef --- */
    Printer pr = print_int;
    pr(77);                        // EXPECT: 77
    pr(apply_bin(mul, 6, 8));      // EXPECT: 48

    /* --- Chained typedef: Count = Integer = int --- */
    Count total = 0;
    int i;
    for (i = 1; i <= 10; i++) total += (Count)i;
    printf("%d\n", total);         // EXPECT: 55

    return 0;
}
