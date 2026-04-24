int printf(const char *fmt, ...);

typedef int MyInt;
typedef MyInt MyInt2;
typedef MyInt2 MyInt3;

typedef char MyChar;
typedef MyChar *MyString;

typedef struct {
    MyInt x;
    MyInt y;
} Vec2;

typedef Vec2 Position;
typedef Position *PositionPtr;

typedef struct {
    Position pos;
    MyInt speed;
    MyString name;
} Entity;

typedef Entity *EntityPtr;

typedef int (*Comparator)(MyInt, MyInt);
typedef Comparator CmpFunc;

int ascending(MyInt a, MyInt b) {
    return a - b;
}

int descending(MyInt a, MyInt b) {
    return b - a;
}

void test_basic_chain() {
    MyInt a = 10;
    MyInt2 b = 20;
    MyInt3 c = 30;
    // EXPECT: a=10 b=20 c=30
    printf("a=%d b=%d c=%d\n", a, b, c);

    MyInt3 sum = a + b + c;
    // EXPECT: sum=60
    printf("sum=%d\n", sum);
}

void test_char_chain() {
    MyChar ch = 65;
    MyString s = "hello";
    // EXPECT: ch=65
    printf("ch=%d\n", ch);
    // EXPECT: str=hello
    printf("str=%s\n", s);
}

void test_struct_chain() {
    Position p;
    p.x = 5;
    p.y = 10;
    // EXPECT: pos(5,10)
    printf("pos(%d,%d)\n", p.x, p.y);

    PositionPtr pp = &p;
    pp->x = 50;
    pp->y = 100;
    // EXPECT: pos(50,100)
    printf("pos(%d,%d)\n", p.x, p.y);
}

void test_entity() {
    Entity e;
    e.pos.x = 3;
    e.pos.y = 7;
    e.speed = 15;
    e.name = "Player";
    // EXPECT: entity Player at(3,7) speed=15
    printf("entity %s at(%d,%d) speed=%d\n", e.name, e.pos.x, e.pos.y, e.speed);

    EntityPtr ep = &e;
    ep->pos.x = 10;
    ep->speed = 25;
    // EXPECT: entity Player at(10,7) speed=25
    printf("entity %s at(%d,%d) speed=%d\n", ep->name, ep->pos.x, ep->pos.y, ep->speed);
}

void sort3(MyInt *arr, CmpFunc cmp) {
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = i + 1; j < 3; j++) {
            if (cmp(arr[i], arr[j]) > 0) {
                MyInt tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }
    }
}

void test_comparator() {
    MyInt vals[3];
    vals[0] = 30;
    vals[1] = 10;
    vals[2] = 20;

    CmpFunc cmp = ascending;
    sort3(vals, cmp);
    // EXPECT: asc: 10 20 30
    printf("asc: %d %d %d\n", vals[0], vals[1], vals[2]);

    vals[0] = 30;
    vals[1] = 10;
    vals[2] = 20;

    cmp = descending;
    sort3(vals, cmp);
    // EXPECT: desc: 30 20 10
    printf("desc: %d %d %d\n", vals[0], vals[1], vals[2]);
}

typedef struct {
    MyInt values[5];
    MyInt len;
} TypedArray;

typedef TypedArray *TypedArrayPtr;

MyInt typed_array_sum(TypedArrayPtr arr) {
    MyInt s = 0;
    MyInt i;
    for (i = 0; i < arr->len; i++) {
        s = s + arr->values[i];
    }
    return s;
}

void test_typed_array() {
    TypedArray ta;
    ta.len = 5;
    ta.values[0] = 2;
    ta.values[1] = 4;
    ta.values[2] = 6;
    ta.values[3] = 8;
    ta.values[4] = 10;

    TypedArrayPtr tap = &ta;
    MyInt3 total = typed_array_sum(tap);
    // EXPECT: typed_sum=30
    printf("typed_sum=%d\n", total);
    // EXPECT: typed_len=5
    printf("typed_len=%d\n", tap->len);
}

void test_mixed_math() {
    MyInt a = 7;
    MyInt2 b = 3;
    MyInt3 c = a * b + b;
    MyInt d = c / a;
    // EXPECT: mixed c=24 d=3
    printf("mixed c=%d d=%d\n", c, d);
}

int main() {
    test_basic_chain();
    test_char_chain();
    test_struct_chain();
    test_entity();
    test_comparator();
    test_typed_array();
    test_mixed_math();
    // EXPECT: done
    printf("done\n");
    return 0;
}
