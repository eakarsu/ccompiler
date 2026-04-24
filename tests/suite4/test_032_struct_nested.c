int printf(const char *fmt, ...);

// Test: Nested structs, accessing deeply nested members

struct Inner {
    int val;
    int flag;
};

struct Middle {
    struct Inner inner;
    int mid_val;
};

struct Outer {
    struct Middle middle;
    int outer_val;
};

struct Coord {
    int x;
    int y;
};

struct Line {
    struct Coord start;
    struct Coord end;
};

struct Box {
    struct Line top;
    struct Line bottom;
};

struct Level1 {
    int a;
};

struct Level2 {
    struct Level1 l1;
    int b;
};

struct Level3 {
    struct Level2 l2;
    int c;
};

int main(void) {
    // Basic nested struct
    struct Middle m;
    m.inner.val = 42;
    m.inner.flag = 1;
    m.mid_val = 100;
    // EXPECT: 42 1 100
    printf("%d %d %d\n", m.inner.val, m.inner.flag, m.mid_val);

    // Three levels of nesting
    struct Outer o;
    o.middle.inner.val = 7;
    o.middle.inner.flag = 0;
    o.middle.mid_val = 77;
    o.outer_val = 777;
    // EXPECT: 7 0 77 777
    printf("%d %d %d %d\n", o.middle.inner.val, o.middle.inner.flag, o.middle.mid_val, o.outer_val);

    // Sum of all nested values
    int sum = o.middle.inner.val + o.middle.inner.flag + o.middle.mid_val + o.outer_val;
    // EXPECT: 861
    printf("%d\n", sum);

    // Line struct with nested coords
    struct Line line;
    line.start.x = 0;
    line.start.y = 0;
    line.end.x = 10;
    line.end.y = 20;
    // EXPECT: 0 0 10 20
    printf("%d %d %d %d\n", line.start.x, line.start.y, line.end.x, line.end.y);

    // Length calculation (dx + dy for simplicity)
    int dx = line.end.x - line.start.x;
    int dy = line.end.y - line.start.y;
    // EXPECT: 10 20
    printf("%d %d\n", dx, dy);

    // Box with nested lines and coords
    struct Box box;
    box.top.start.x = 1;
    box.top.start.y = 10;
    box.top.end.x = 5;
    box.top.end.y = 10;
    box.bottom.start.x = 1;
    box.bottom.start.y = 2;
    box.bottom.end.x = 5;
    box.bottom.end.y = 2;
    // EXPECT: 1 10 5 10
    printf("%d %d %d %d\n", box.top.start.x, box.top.start.y, box.top.end.x, box.top.end.y);
    // EXPECT: 1 2 5 2
    printf("%d %d %d %d\n", box.bottom.start.x, box.bottom.start.y, box.bottom.end.x, box.bottom.end.y);

    // Width and height of box
    int width = box.top.end.x - box.top.start.x;
    int height = box.top.start.y - box.bottom.start.y;
    // EXPECT: 4 8
    printf("%d %d\n", width, height);

    // Box area
    int boxarea = width * height;
    // EXPECT: 32
    printf("%d\n", boxarea);

    // Modify nested members
    o.middle.inner.val = o.middle.inner.val * 3;
    o.middle.mid_val = o.middle.mid_val + 23;
    // EXPECT: 21 100
    printf("%d %d\n", o.middle.inner.val, o.middle.mid_val);

    // Level3 deeply nested
    struct Level3 l3;
    l3.l2.l1.a = 111;
    l3.l2.b = 222;
    l3.c = 333;
    int total = l3.l2.l1.a + l3.l2.b + l3.c;
    // EXPECT: 666
    printf("%d\n", total);

    // Conditional on nested member
    int deep_flag;
    if (l3.l2.l1.a > 100) {
        deep_flag = 1;
    } else {
        deep_flag = 0;
    }
    // EXPECT: 1
    printf("%d\n", deep_flag);

    // Nested struct in loop
    struct Outer arr[3];
    int i;
    for (i = 0; i < 3; i = i + 1) {
        arr[i].middle.inner.val = i * 10;
        arr[i].middle.mid_val = i * 100;
        arr[i].outer_val = i * 1000;
    }
    // EXPECT: 0 0 0
    printf("%d %d %d\n", arr[0].middle.inner.val, arr[0].middle.mid_val, arr[0].outer_val);
    // EXPECT: 10 100 1000
    printf("%d %d %d\n", arr[1].middle.inner.val, arr[1].middle.mid_val, arr[1].outer_val);
    // EXPECT: 20 200 2000
    printf("%d %d %d\n", arr[2].middle.inner.val, arr[2].middle.mid_val, arr[2].outer_val);

    // Product of nested values
    int prod = l3.l2.l1.a * l3.l2.b;
    // EXPECT: 24642
    printf("%d\n", prod);

    return 0;
}
