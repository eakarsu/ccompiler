// All operators combined: arithmetic, bitwise, comparison, logical, assignment,
// increment/decrement, sizeof, ternary, comma, cast, address-of, deref, arrow, dot
int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

struct point {
    int x;
    int y;
};

struct node {
    int val;
    struct node *next;
};

void test_arithmetic(void) {
    int a = 100;
    int b = 37;
    printf("add=%d\n", a + b);
    // EXPECT: add=137
    printf("sub=%d\n", a - b);
    // EXPECT: sub=63
    printf("mul=%d\n", a * b);
    // EXPECT: mul=3700
    printf("div=%d\n", a / b);
    // EXPECT: div=2
    printf("mod=%d\n", a % b);
    // EXPECT: mod=26
    printf("neg=%d\n", -a);
    // EXPECT: neg=-100
    printf("complex=%d\n", (a + b) * 2 - a / b + a % b);
    // EXPECT: complex=298
}

void test_bitwise(void) {
    int a = 0xFF;
    int b = 0x0F;
    printf("and=%d\n", a & b);
    // EXPECT: and=15
    printf("or=%d\n", a | b);
    // EXPECT: or=255
    printf("xor=%d\n", a ^ b);
    // EXPECT: xor=240
    printf("not=%d\n", ~0 & 0xFF);
    // EXPECT: not=255
    printf("shl=%d\n", 1 << 10);
    // EXPECT: shl=1024
    printf("shr=%d\n", 1024 >> 5);
    // EXPECT: shr=32
    printf("complex=%d\n", ((a & b) | (a ^ b)) << 1);
    // EXPECT: complex=510
}

void test_comparison(void) {
    int a = 10;
    int b = 20;
    int c = 10;
    printf("eq=%d\n", a == c);
    // EXPECT: eq=1
    printf("neq=%d\n", a != b);
    // EXPECT: neq=1
    printf("lt=%d\n", a < b);
    // EXPECT: lt=1
    printf("gt=%d\n", b > a);
    // EXPECT: gt=1
    printf("le=%d\n", a <= c);
    // EXPECT: le=1
    printf("ge=%d\n", a >= c);
    // EXPECT: ge=1
    printf("chain=%d\n", (a < b) == (b > a));
    // EXPECT: chain=1
}

void test_logical(void) {
    int a = 1;
    int b = 0;
    int c = 42;
    printf("and=%d\n", a && c);
    // EXPECT: and=1
    printf("or=%d\n", b || a);
    // EXPECT: or=1
    printf("not=%d\n", !b);
    // EXPECT: not=1
    printf("notnot=%d\n", !!c);
    // EXPECT: notnot=1
    printf("complex=%d\n", (a && c) || (b && !a));
    // EXPECT: complex=1
    printf("short1=%d\n", 0 && (1/0));
    // EXPECT: short1=0
    printf("short2=%d\n", 1 || (1/0));
    // EXPECT: short2=1
}

void test_assignment_ops(void) {
    int x = 100;
    x += 10;
    printf("add_eq=%d\n", x);
    // EXPECT: add_eq=110
    x -= 20;
    printf("sub_eq=%d\n", x);
    // EXPECT: sub_eq=90
    x *= 3;
    printf("mul_eq=%d\n", x);
    // EXPECT: mul_eq=270
    x /= 9;
    printf("div_eq=%d\n", x);
    // EXPECT: div_eq=30
    x %= 7;
    printf("mod_eq=%d\n", x);
    // EXPECT: mod_eq=2
    x = 0xFF;
    x &= 0x0F;
    printf("and_eq=%d\n", x);
    // EXPECT: and_eq=15
    x |= 0xF0;
    printf("or_eq=%d\n", x);
    // EXPECT: or_eq=255
    x ^= 0xFF;
    printf("xor_eq=%d\n", x);
    // EXPECT: xor_eq=0
    x = 1;
    x <<= 8;
    printf("shl_eq=%d\n", x);
    // EXPECT: shl_eq=256
    x >>= 4;
    printf("shr_eq=%d\n", x);
    // EXPECT: shr_eq=16
}

void test_increment_decrement(void) {
    int a = 10;
    int b;
    b = a++;
    printf("post++ b=%d a=%d\n", b, a);
    // EXPECT: post++ b=10 a=11
    b = ++a;
    printf("pre++ b=%d a=%d\n", b, a);
    // EXPECT: pre++ b=12 a=12
    b = a--;
    printf("post-- b=%d a=%d\n", b, a);
    // EXPECT: post-- b=12 a=11
    b = --a;
    printf("pre-- b=%d a=%d\n", b, a);
    // EXPECT: pre-- b=10 a=10
}

void test_sizeof_op(void) {
    printf("int=%d\n", (int)sizeof(int));
    // EXPECT: int=4
    printf("char=%d\n", (int)sizeof(char));
    // EXPECT: char=1
    printf("ptr=%d\n", (int)sizeof(int *));
    // EXPECT: ptr=8
    int arr[10];
    printf("arr=%d\n", (int)sizeof(arr));
    // EXPECT: arr=40
    printf("struct=%d\n", (int)sizeof(struct point));
    // EXPECT: struct=8
}

void test_ternary(void) {
    int a = 5;
    int b = 10;
    printf("t1=%d\n", a < b ? a : b);
    // EXPECT: t1=5
    printf("t2=%d\n", a > b ? a : b);
    // EXPECT: t2=10
    printf("nested=%d\n", a < b ? (a == 5 ? 100 : 200) : 300);
    // EXPECT: nested=100
    printf("chain=%d\n", 0 ? 1 : 0 ? 2 : 3);
    // EXPECT: chain=3
}

void test_comma(void) {
    int a;
    a = (1, 2, 3, 4, 5);
    printf("comma=%d\n", a);
    // EXPECT: comma=5
    int x = 0;
    int y = 0;
    int i;
    for (i = 0, x = 10; i < 3; i++, x += 10) {
        y = y + x;
    }
    printf("for_comma x=%d y=%d\n", x, y);
    // EXPECT: for_comma x=40 y=60
}

void test_cast(void) {
    int a = 65;
    char c = (char)a;
    printf("int_to_char=%c\n", c);
    // EXPECT: int_to_char=A
    double d = 3;
    int i = (int)d;
    printf("float_to_int=%d\n", i);
    // EXPECT: float_to_int=3
    unsigned int u = (unsigned int)-1;
    printf("signed_to_unsigned=%u\n", u);
    // EXPECT: signed_to_unsigned=4294967295
}

void test_address_deref(void) {
    int x = 42;
    int *p = &x;
    printf("addr val=%d\n", *p);
    // EXPECT: addr val=42
    *p = 99;
    printf("deref assign=%d\n", x);
    // EXPECT: deref assign=99
    int **pp = &p;
    printf("double deref=%d\n", **pp);
    // EXPECT: double deref=99
}

void test_dot_arrow(void) {
    struct point p;
    p.x = 10;
    p.y = 20;
    printf("dot x=%d y=%d\n", p.x, p.y);
    // EXPECT: dot x=10 y=20

    struct point *pp = &p;
    pp->x = 30;
    pp->y = 40;
    printf("arrow x=%d y=%d\n", pp->x, pp->y);
    // EXPECT: arrow x=30 y=40
}

void test_linked_list_ops(void) {
    struct node n1, n2, n3;
    n1.val = 10;
    n2.val = 20;
    n3.val = 30;
    n1.next = &n2;
    n2.next = &n3;
    n3.next = 0;

    struct node *cur = &n1;
    int sum = 0;
    while (cur) {
        sum += cur->val;
        cur = cur->next;
    }
    printf("list sum=%d\n", sum);
    // EXPECT: list sum=60
}

void test_mega_expression(void) {
    int a = 10;
    int b = 3;
    int c = 7;
    int *p = &a;
    // Big combined expression
    int result = (*p + b) * c - (a % b) + (a > b ? a : b) + (int)sizeof(int)
                 + (a << 1) + (c >> 1) + (a & b) + (a | c) + (!0) + (~~a);
    printf("mega=%d\n", result);
    // EXPECT: mega=155
}

void test_compound_assign_chain(void) {
    int x = 1;
    x += 2;
    x *= 3;
    x -= 1;
    x /= 2;
    x %= 3;
    printf("chain=%d\n", x);
    // EXPECT: chain=1
}

void test_pointer_arithmetic(void) {
    int arr[5] = {10, 20, 30, 40, 50};
    int *p = arr;
    printf("p[0]=%d\n", *p);
    // EXPECT: p[0]=10
    p = p + 2;
    printf("p[2]=%d\n", *p);
    // EXPECT: p[2]=30
    printf("diff=%d\n", (int)(p - arr));
    // EXPECT: diff=2
    p = p - 1;
    printf("p-1=%d\n", *p);
    // EXPECT: p-1=20
}

int main(void) {
    test_arithmetic();
    test_bitwise();
    test_comparison();
    test_logical();
    test_assignment_ops();
    test_increment_decrement();
    test_sizeof_op();
    test_ternary();
    test_comma();
    test_cast();
    test_address_deref();
    test_dot_arrow();
    test_linked_list_ops();
    test_mega_expression();
    test_compound_assign_chain();
    test_pointer_arithmetic();
    return 0;
}
