int printf(const char *fmt, ...);
// EXPECT: 1:3\n2:4\n3:25\n4:7\n5:5\n6:7\n7:5\n8:5\n9:5\n10:10\n11:30\n12:90\n13:30\n14:5\n15:-1\n16:4\n17:3\n18:12\n19:14\n20:1\n21:0\n22:5\n23:3\n24:1001\n25:5\n26:433\n27:86\n28:92\n29:78\n30:10\n31:2\n32:100\n33:200\n34:155\n35:175\n36:330\n37:4\n38:100\n39:2\n40:-1\n41:3\n42:70\n43:42\n44:99\n45:42
// =============================================================================
// TEST 027: Advanced Struct Usage
// =============================================================================
// Tests nested structs, arrays of structs, structs containing arrays,
// passing struct pointers, struct initialization patterns, and complex
// struct operations.
// =============================================================================

// -----------------------------------------------------------------------------
// Basic struct: Point2D
// -----------------------------------------------------------------------------
struct Point2D {
    int x;
    int y;
};

// -----------------------------------------------------------------------------
// Struct with array: Vector
// -----------------------------------------------------------------------------
struct Vector {
    int data[10];
    int size;
};

// -----------------------------------------------------------------------------
// Nested struct: Rectangle (contains two Point2D)
// -----------------------------------------------------------------------------
struct Rectangle {
    struct Point2D top_left;
    struct Point2D bottom_right;
};

// -----------------------------------------------------------------------------
// Struct containing another struct: Circle
// -----------------------------------------------------------------------------
struct Circle {
    struct Point2D center;
    int radius;
};

// -----------------------------------------------------------------------------
// Array of structs: Student record
// -----------------------------------------------------------------------------
struct Student {
    int id;
    int grades[5];
    int num_grades;
    int total;
};

// -----------------------------------------------------------------------------
// Complex nested struct: Department
// -----------------------------------------------------------------------------
struct Department {
    int dept_id;
    int num_students;
    struct Student students[5];
};

// -----------------------------------------------------------------------------
// Linked-list-like struct using array index as "pointer"
// -----------------------------------------------------------------------------
struct ListNode {
    int value;
    int next; // index of next node, -1 for null
};

// -----------------------------------------------------------------------------
// Point2D operations
// -----------------------------------------------------------------------------
int point_distance_sq(struct Point2D *a, struct Point2D *b) {
    int dx;
    int dy;
    dx = a->x - b->x;
    dy = a->y - b->y;
    return dx * dx + dy * dy;
}

void point_translate(struct Point2D *p, int dx, int dy) {
    p->x = p->x + dx;
    p->y = p->y + dy;
}

int point_manhattan(struct Point2D *a, struct Point2D *b) {
    int dx;
    int dy;
    dx = a->x - b->x;
    dy = a->y - b->y;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    return dx + dy;
}

struct Point2D point_midpoint(struct Point2D *a, struct Point2D *b) {
    struct Point2D mid;
    mid.x = (a->x + b->x) / 2;
    mid.y = (a->y + b->y) / 2;
    return mid;
}

// -----------------------------------------------------------------------------
// Vector operations
// -----------------------------------------------------------------------------
void vec_init(struct Vector *v) {
    int i;
    v->size = 0;
    for (i = 0; i < 10; i = i + 1) {
        v->data[i] = 0;
    }
}

void vec_push(struct Vector *v, int val) {
    if (v->size < 10) {
        v->data[v->size] = val;
        v->size = v->size + 1;
    }
}

int vec_get(struct Vector *v, int idx) {
    if (idx >= 0 && idx < v->size) return v->data[idx];
    return -1;
}

int vec_sum(struct Vector *v) {
    int i;
    int s;
    s = 0;
    for (i = 0; i < v->size; i = i + 1) {
        s = s + v->data[i];
    }
    return s;
}

int vec_max(struct Vector *v) {
    int i;
    int mx;
    if (v->size == 0) return -1;
    mx = v->data[0];
    for (i = 1; i < v->size; i = i + 1) {
        if (v->data[i] > mx) mx = v->data[i];
    }
    return mx;
}

int vec_min(struct Vector *v) {
    int i;
    int mn;
    if (v->size == 0) return -1;
    mn = v->data[0];
    for (i = 1; i < v->size; i = i + 1) {
        if (v->data[i] < mn) mn = v->data[i];
    }
    return mn;
}

// -----------------------------------------------------------------------------
// Rectangle operations
// -----------------------------------------------------------------------------
int rect_width(struct Rectangle *r) {
    int w;
    w = r->bottom_right.x - r->top_left.x;
    if (w < 0) w = -w;
    return w;
}

int rect_height(struct Rectangle *r) {
    int h;
    h = r->bottom_right.y - r->top_left.y;
    if (h < 0) h = -h;
    return h;
}

int rect_area(struct Rectangle *r) {
    return rect_width(r) * rect_height(r);
}

int rect_perimeter(struct Rectangle *r) {
    return 2 * (rect_width(r) + rect_height(r));
}

int rect_contains_point(struct Rectangle *r, struct Point2D *p) {
    if (p->x >= r->top_left.x && p->x <= r->bottom_right.x &&
        p->y >= r->top_left.y && p->y <= r->bottom_right.y) {
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Student operations
// -----------------------------------------------------------------------------
void student_init(struct Student *s, int id) {
    int i;
    s->id = id;
    s->num_grades = 0;
    s->total = 0;
    for (i = 0; i < 5; i = i + 1) {
        s->grades[i] = 0;
    }
}

void student_add_grade(struct Student *s, int grade) {
    if (s->num_grades < 5) {
        s->grades[s->num_grades] = grade;
        s->num_grades = s->num_grades + 1;
        s->total = s->total + grade;
    }
}

int student_average(struct Student *s) {
    if (s->num_grades == 0) return 0;
    return s->total / s->num_grades;
}

int student_highest(struct Student *s) {
    int i;
    int mx;
    if (s->num_grades == 0) return 0;
    mx = s->grades[0];
    for (i = 1; i < s->num_grades; i = i + 1) {
        if (s->grades[i] > mx) mx = s->grades[i];
    }
    return mx;
}

int student_lowest(struct Student *s) {
    int i;
    int mn;
    if (s->num_grades == 0) return 0;
    mn = s->grades[0];
    for (i = 1; i < s->num_grades; i = i + 1) {
        if (s->grades[i] < mn) mn = s->grades[i];
    }
    return mn;
}

// -----------------------------------------------------------------------------
// Department operations
// -----------------------------------------------------------------------------
void dept_init(struct Department *d, int id) {
    d->dept_id = id;
    d->num_students = 0;
}

void dept_add_student(struct Department *d, int student_id) {
    if (d->num_students < 5) {
        student_init(&d->students[d->num_students], student_id);
        d->num_students = d->num_students + 1;
    }
}

int dept_total_grades(struct Department *d) {
    int i;
    int total;
    total = 0;
    for (i = 0; i < d->num_students; i = i + 1) {
        total = total + d->students[i].total;
    }
    return total;
}

// -----------------------------------------------------------------------------
// ListNode operations (using array-based linked list)
// -----------------------------------------------------------------------------
struct ListNode nodes[20];
int node_count;

void list_init(void) {
    int i;
    node_count = 0;
    for (i = 0; i < 20; i = i + 1) {
        nodes[i].value = 0;
        nodes[i].next = -1;
    }
}

int list_add(int val) {
    int idx;
    idx = node_count;
    nodes[idx].value = val;
    nodes[idx].next = -1;
    node_count = node_count + 1;
    return idx;
}

void list_link(int from_idx, int to_idx) {
    nodes[from_idx].next = to_idx;
}

int list_length(int head) {
    int count;
    int curr;
    count = 0;
    curr = head;
    while (curr != -1) {
        count = count + 1;
        curr = nodes[curr].next;
    }
    return count;
}

int list_sum(int head) {
    int sum;
    int curr;
    sum = 0;
    curr = head;
    while (curr != -1) {
        sum = sum + nodes[curr].value;
        curr = nodes[curr].next;
    }
    return sum;
}

int list_find(int head, int val) {
    int curr;
    curr = head;
    while (curr != -1) {
        if (nodes[curr].value == val) return curr;
        curr = nodes[curr].next;
    }
    return -1;
}

// =============================================================================
// Main test function
// =============================================================================
int main(void) {
    int test_num;
    int v;
    struct Point2D p1;
    struct Point2D p2;
    struct Point2D mid;
    struct Vector vec;
    struct Rectangle rect;
    struct Circle circ;
    struct Student stu;
    struct Department dept;
    int head;
    int n1;
    int n2;
    int n3;
    int n4;

    test_num = 0;

    // ---- Point2D tests ----
    p1.x = 3;
    p1.y = 4;
    p2.x = 0;
    p2.y = 0;

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, p1.x); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, p1.y); // 4

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, point_distance_sq(&p1, &p2)); // 25

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, point_manhattan(&p1, &p2)); // 7

    point_translate(&p1, 2, 3);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, p1.x); // 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, p1.y); // 7

    // Midpoint
    p1.x = 0; p1.y = 0;
    p2.x = 10; p2.y = 10;
    mid = point_midpoint(&p1, &p2);
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, mid.x); // 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, mid.y); // 5

    // ---- Vector tests ----
    vec_init(&vec);
    vec_push(&vec, 10);
    vec_push(&vec, 20);
    vec_push(&vec, 30);
    vec_push(&vec, 5);
    vec_push(&vec, 25);

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, vec.size); // 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, vec_get(&vec, 0)); // 10

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, vec_get(&vec, 2)); // 30

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, vec_sum(&vec)); // 90

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, vec_max(&vec)); // 30

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, vec_min(&vec)); // 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, vec_get(&vec, 10)); // -1 (out of bounds)

    // ---- Rectangle tests ----
    rect.top_left.x = 1;
    rect.top_left.y = 1;
    rect.bottom_right.x = 5;
    rect.bottom_right.y = 4;

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, rect_width(&rect)); // 4

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, rect_height(&rect)); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, rect_area(&rect)); // 12

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, rect_perimeter(&rect)); // 14

    // Contains point?
    p1.x = 3; p1.y = 2;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, rect_contains_point(&rect, &p1)); // 1

    p1.x = 6; p1.y = 2;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, rect_contains_point(&rect, &p1)); // 0

    // ---- Circle struct test ----
    circ.center.x = 5;
    circ.center.y = 5;
    circ.radius = 3;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, circ.center.x); // 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, circ.radius); // 3

    // ---- Student tests ----
    student_init(&stu, 1001);
    student_add_grade(&stu, 85);
    student_add_grade(&stu, 90);
    student_add_grade(&stu, 78);
    student_add_grade(&stu, 92);
    student_add_grade(&stu, 88);

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, stu.id); // 1001

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, stu.num_grades); // 5

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, stu.total); // 433

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, student_average(&stu)); // 86

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, student_highest(&stu)); // 92

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, student_lowest(&stu)); // 78

    // ---- Department tests ----
    dept_init(&dept, 10);
    dept_add_student(&dept, 100);
    dept_add_student(&dept, 200);

    // Add grades to students via department
    student_add_grade(&dept.students[0], 75);
    student_add_grade(&dept.students[0], 80);
    student_add_grade(&dept.students[1], 90);
    student_add_grade(&dept.students[1], 85);

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dept.dept_id); // 10

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dept.num_students); // 2

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dept.students[0].id); // 100

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dept.students[1].id); // 200

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dept.students[0].total); // 155

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dept.students[1].total); // 175

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, dept_total_grades(&dept)); // 330

    // ---- Array-based linked list tests ----
    list_init();
    n1 = list_add(10);
    n2 = list_add(20);
    n3 = list_add(30);
    n4 = list_add(40);
    list_link(n1, n2);
    list_link(n2, n3);
    list_link(n3, n4);

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, list_length(n1)); // 4

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, list_sum(n1)); // 100

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, list_find(n1, 30)); // 2

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, list_find(n1, 99)); // -1

    // Length from middle
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, list_length(n2)); // 3

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, list_sum(n3)); // 70

    // ---- Struct assignment test ----
    p1.x = 42;
    p1.y = 99;
    p2 = p1;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, p2.x); // 42

    test_num = test_num + 1;
    printf("%d:%d\n", test_num, p2.y); // 99

    // Modify p1 should not affect p2
    p1.x = 0;
    test_num = test_num + 1;
    printf("%d:%d\n", test_num, p2.x); // 42

    return 0;
}
