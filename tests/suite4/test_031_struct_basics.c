int printf(const char *fmt, ...);

// Test: Struct declaration, initialization, member access with dot operator

struct Point {
    int x;
    int y;
};

struct Rectangle {
    int width;
    int height;
};

struct Person {
    int age;
    int id;
    char initial;
};

struct Triple {
    int a;
    int b;
    int c;
};

int main(void) {
    // Basic struct declaration and member assignment
    struct Point p1;
    p1.x = 10;
    p1.y = 20;
    // EXPECT: 10 20
    printf("%d %d\n", p1.x, p1.y);

    // Struct initialization with assignment
    struct Point p2;
    p2.x = 100;
    p2.y = 200;
    // EXPECT: 100 200
    printf("%d %d\n", p2.x, p2.y);

    // Struct arithmetic on members
    int sum = p1.x + p1.y;
    // EXPECT: 30
    printf("%d\n", sum);

    // Modify struct members
    p1.x = p1.x + 5;
    p1.y = p1.y * 2;
    // EXPECT: 15 40
    printf("%d %d\n", p1.x, p1.y);

    // Rectangle struct
    struct Rectangle r;
    r.width = 50;
    r.height = 30;
    int area = r.width * r.height;
    // EXPECT: 1500
    printf("%d\n", area);

    // Perimeter calculation
    int perimeter = 2 * (r.width + r.height);
    // EXPECT: 160
    printf("%d\n", perimeter);

    // Person struct with char member
    struct Person person;
    person.age = 25;
    person.id = 1001;
    person.initial = 'A';
    // EXPECT: 25 1001
    printf("%d %d\n", person.age, person.id);

    // Char member as int
    int charval = person.initial;
    // EXPECT: 65
    printf("%d\n", charval);

    // Triple struct
    struct Triple t;
    t.a = 3;
    t.b = 7;
    t.c = 11;
    int triplesum = t.a + t.b + t.c;
    // EXPECT: 21
    printf("%d\n", triplesum);

    // Product of triple members
    int product = t.a * t.b * t.c;
    // EXPECT: 231
    printf("%d\n", product);

    // Multiple struct instances
    struct Point points_a;
    struct Point points_b;
    struct Point points_c;
    points_a.x = 1; points_a.y = 2;
    points_b.x = 3; points_b.y = 4;
    points_c.x = 5; points_c.y = 6;
    int totalx = points_a.x + points_b.x + points_c.x;
    int totaly = points_a.y + points_b.y + points_c.y;
    // EXPECT: 9 12
    printf("%d %d\n", totalx, totaly);

    // Conditional based on struct member
    struct Rectangle r2;
    r2.width = 40;
    r2.height = 40;
    int is_square = (r2.width == r2.height) ? 1 : 0;
    // EXPECT: 1
    printf("%d\n", is_square);

    // Non-square check
    int not_square = (r.width == r.height) ? 1 : 0;
    // EXPECT: 0
    printf("%d\n", not_square);

    // Struct member used in loop
    struct Triple counter;
    counter.a = 0;
    counter.b = 0;
    counter.c = 0;
    int i;
    for (i = 0; i < 5; i = i + 1) {
        counter.a = counter.a + 1;
        counter.b = counter.b + 2;
        counter.c = counter.c + 3;
    }
    // EXPECT: 5 10 15
    printf("%d %d %d\n", counter.a, counter.b, counter.c);

    // Sizeof struct (at least sum of member sizes)
    int sp = sizeof(struct Point);
    int has_size = (sp >= 8) ? 1 : 0;
    // EXPECT: 1
    printf("%d\n", has_size);

    // Struct member as array index
    struct Point idx;
    idx.x = 2;
    idx.y = 4;
    int arr[5];
    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 50;
    // EXPECT: 30 50
    printf("%d %d\n", arr[idx.x], arr[idx.y]);

    return 0;
}
