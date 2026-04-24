int printf(const char *fmt, ...);

/* Basic struct declaration and member access */
struct Point {
    int x;
    int y;
};

struct Rectangle {
    int width;
    int height;
    int color;
};

struct Person {
    int age;
    int score;
    int id;
};

/* Function taking struct by value */
int area(struct Rectangle r) {
    return r.width * r.height;
}

/* Function taking pointer to struct */
void scale(struct Point *p, int factor) {
    p->x = p->x * factor;
    p->y = p->y * factor;
}

/* Function modifying struct through pointer */
void set_person(struct Person *p, int age, int score, int id) {
    p->age   = age;
    p->score = score;
    p->id    = id;
}

int main(void) {
    /* Basic initialization with {} */
    struct Point pt = {3, 4};
    printf("%d\n", pt.x);                      // EXPECT: 3
    printf("%d\n", pt.y);                      // EXPECT: 4

    /* Member write via . operator */
    pt.x = 10;
    pt.y = 20;
    printf("%d\n", pt.x);                      // EXPECT: 10
    printf("%d\n", pt.y);                      // EXPECT: 20

    /* Pointer access via -> */
    struct Point *pp = &pt;
    printf("%d\n", pp->x);                     // EXPECT: 10
    printf("%d\n", pp->y);                     // EXPECT: 20

    pp->x = 7;
    pp->y = 9;
    printf("%d\n", pt.x);                      // EXPECT: 7
    printf("%d\n", pt.y);                      // EXPECT: 9

    /* Struct with three members */
    struct Rectangle rect = {8, 5, 255};
    printf("%d\n", rect.width);                // EXPECT: 8
    printf("%d\n", rect.height);               // EXPECT: 5
    printf("%d\n", rect.color);                // EXPECT: 255

    /* Pass struct by value to function */
    printf("%d\n", area(rect));                // EXPECT: 40

    /* Modifying the original after passing by value should not affect result */
    rect.width = 2;
    printf("%d\n", rect.width);               // EXPECT: 2

    /* scale via pointer */
    struct Point pt2 = {3, 5};
    scale(&pt2, 4);
    printf("%d\n", pt2.x);                    // EXPECT: 12
    printf("%d\n", pt2.y);                    // EXPECT: 20

    /* Struct assignment (copy) */
    struct Point pt3 = {100, 200};
    struct Point pt4;
    pt4 = pt3;
    printf("%d\n", pt4.x);                    // EXPECT: 100
    printf("%d\n", pt4.y);                    // EXPECT: 200

    /* Modifying the copy does not affect original */
    pt4.x = 999;
    printf("%d\n", pt3.x);                    // EXPECT: 100
    printf("%d\n", pt4.x);                    // EXPECT: 999

    /* Person struct via pointer setter */
    struct Person alice;
    set_person(&alice, 30, 95, 1);
    printf("%d\n", alice.age);                // EXPECT: 30
    printf("%d\n", alice.score);             // EXPECT: 95
    printf("%d\n", alice.id);               // EXPECT: 1

    /* Member arithmetic using . */
    struct Rectangle r2 = {6, 7, 0};
    int perimeter = 2 * (r2.width + r2.height);
    printf("%d\n", perimeter);               // EXPECT: 26

    /* Chained -> dereference then member access */
    struct Rectangle r3 = {3, 3, 9};
    struct Rectangle *rp = &r3;
    rp->width  = rp->width  * 2;
    rp->height = rp->height * 3;
    printf("%d\n", r3.width);               // EXPECT: 6
    printf("%d\n", r3.height);              // EXPECT: 9

    /* Struct initialized to zero (partial init) */
    struct Point origin = {0};
    printf("%d\n", origin.x);              // EXPECT: 0
    printf("%d\n", origin.y);              // EXPECT: 0

    /* Compute distance squared between two points */
    struct Point a = {1, 1};
    struct Point b = {4, 5};
    int dx = b.x - a.x;   /* 3 */
    int dy = b.y - a.y;   /* 4 */
    printf("%d\n", dx * dx + dy * dy);    // EXPECT: 25

    /* Struct copy then independent modification */
    struct Person p1;
    struct Person p2;
    set_person(&p1, 25, 80, 10);
    p2 = p1;
    p2.age = 26;
    p2.score = 90;
    printf("%d\n", p1.age);               // EXPECT: 25
    printf("%d\n", p1.score);            // EXPECT: 80
    printf("%d\n", p2.age);               // EXPECT: 26
    printf("%d\n", p2.score);            // EXPECT: 90

    /* Using -> to add to a member */
    struct Rectangle bigr = {100, 200, 0};
    struct Rectangle *brp = &bigr;
    brp->width  = brp->width  + 50;
    brp->height = brp->height - 50;
    printf("%d\n", bigr.width);          // EXPECT: 150
    printf("%d\n", bigr.height);         // EXPECT: 150
    printf("%d\n", area(bigr));          // EXPECT: 22500

    return 0;
}
