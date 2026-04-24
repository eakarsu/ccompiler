int printf(const char *fmt, ...);

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point top_left;
    Point bottom_right;
} Rect;

typedef struct {
    char *name;
    int age;
    int score;
} Student;

typedef struct {
    int data[10];
    int count;
} IntList;

Point make_point(int x, int y) {
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

void print_point(Point p) {
    // EXPECT: point(3,4)
    // EXPECT: point(10,20)
    // EXPECT: point(8,14)
    printf("point(%d,%d)\n", p.x, p.y);
}

int point_distance_sq(Point a, Point b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return dx * dx + dy * dy;
}

void test_point() {
    Point p1 = make_point(3, 4);
    Point p2 = make_point(10, 20);
    print_point(p1);
    print_point(p2);

    Point p3;
    p3.x = p1.x + p2.x / 2;
    p3.y = p1.y + p2.y / 2;
    print_point(p3);

    int dist = point_distance_sq(p1, p2);
    // EXPECT: dist_sq=305
    printf("dist_sq=%d\n", dist);
}

int rect_area(Rect r) {
    int w = r.bottom_right.x - r.top_left.x;
    int h = r.bottom_right.y - r.top_left.y;
    if (w < 0) w = -w;
    if (h < 0) h = -h;
    return w * h;
}

void test_rect() {
    Rect r;
    r.top_left = make_point(0, 0);
    r.bottom_right = make_point(10, 5);
    int area = rect_area(r);
    // EXPECT: rect_area=50
    printf("rect_area=%d\n", area);

    Rect r2;
    r2.top_left = make_point(3, 3);
    r2.bottom_right = make_point(15, 20);
    int area2 = rect_area(r2);
    // EXPECT: rect_area2=204
    printf("rect_area2=%d\n", area2);
}

void test_student() {
    Student s1;
    s1.name = "Alice";
    s1.age = 20;
    s1.score = 95;

    Student s2;
    s2.name = "Bob";
    s2.age = 22;
    s2.score = 87;

    Student s3;
    s3.name = "Carol";
    s3.age = 19;
    s3.score = 91;

    // EXPECT: Alice age=20 score=95
    printf("%s age=%d score=%d\n", s1.name, s1.age, s1.score);
    // EXPECT: Bob age=22 score=87
    printf("%s age=%d score=%d\n", s2.name, s2.age, s2.score);
    // EXPECT: Carol age=19 score=91
    printf("%s age=%d score=%d\n", s3.name, s3.age, s3.score);

    int avg = (s1.score + s2.score + s3.score) / 3;
    // EXPECT: avg_score=91
    printf("avg_score=%d\n", avg);
}

void intlist_add(IntList *list, int val) {
    if (list->count < 10) {
        list->data[list->count] = val;
        list->count = list->count + 1;
    }
}

int intlist_sum(IntList *list) {
    int s = 0;
    int i;
    for (i = 0; i < list->count; i++) {
        s = s + list->data[i];
    }
    return s;
}

void test_intlist() {
    IntList lst;
    lst.count = 0;

    intlist_add(&lst, 10);
    intlist_add(&lst, 20);
    intlist_add(&lst, 30);
    intlist_add(&lst, 40);
    intlist_add(&lst, 50);

    // EXPECT: list_count=5
    printf("list_count=%d\n", lst.count);
    // EXPECT: list_sum=150
    printf("list_sum=%d\n", intlist_sum(&lst));

    intlist_add(&lst, 60);
    // EXPECT: list_count2=6
    printf("list_count2=%d\n", lst.count);
    // EXPECT: list_sum2=210
    printf("list_sum2=%d\n", intlist_sum(&lst));
}

void test_typedef_pointer() {
    typedef Point *PointPtr;
    Point p;
    p.x = 100;
    p.y = 200;
    PointPtr pp = &p;
    // EXPECT: ptr_point(100,200)
    printf("ptr_point(%d,%d)\n", pp->x, pp->y);
    pp->x = 999;
    // EXPECT: modified_x=999
    printf("modified_x=%d\n", p.x);
}

int main() {
    test_point();
    test_rect();
    test_student();
    test_intlist();
    test_typedef_pointer();
    // EXPECT: done
    printf("done\n");
    return 0;
}
