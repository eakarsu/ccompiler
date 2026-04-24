int printf(const char *fmt, ...);
// EXPECT: p1: x=3 y=4
// p2: x=10 y=20
// dist_sq=305
// moved: x=13 y=24
// scaled: x=6 y=8
// origin: x=0 y=0
// rect: x=1 y=2 w=10 h=5
// rect_area=50
// rect_perim=30
// person_age=25
// person_id=1001
// person_score=95
// nested: outer_a=1 inner_x=10 inner_y=20
// arr_struct_0: id=0 val=0
// arr_struct_1: id=1 val=10
// arr_struct_2: id=2 val=20
// arr_struct_3: id=3 val=30
// arr_struct_4: id=4 val=40
// arr_sum=100
// struct_ptr: x=42 y=84
// modify_via_ptr: x=100 y=200
// student_0: id=1 grade=90
// student_1: id=2 grade=85
// student_2: id=3 grade=95
// student_3: id=4 grade=70
// student_4: id=5 grade=88
// best_student_id=3
// avg_grade=85
// total_grade=428
// pair: a=10 b=20
// swap_pair: a=20 b=10
// color: r=255 g=128 b=0
// mix: r=127 g=64 b=0
// vec3: x=1 y=2 z=3
// vec3_dot=14
// vec3_add: x=2 y=4 z=6
// counter: val=5
// counter_after_inc: val=10
// stack_top=30
// stack_top=20
// stack_top=10
// stack_empty=1
// node_chain: 1 2 3 4 5
// matrix_elem_00=1
// matrix_elem_11=5
// matrix_trace=15
// complex_add: re=4 im=6
// complex_mul: re=-5 im=10
// range_len=7
// range_contains_5=1
// range_contains_10=0
// done

// =============================================================================
// TEST 006: Structs
// =============================================================================

struct Point {
    int x;
    int y;
};

struct Rect {
    int x;
    int y;
    int w;
    int h;
};

struct Person {
    int age;
    int id;
    int score;
};

struct Inner {
    int x;
    int y;
};

struct Outer {
    int a;
    struct Inner inner;
};

struct IdVal {
    int id;
    int val;
};

struct Student {
    int id;
    int grade;
};

struct Pair {
    int a;
    int b;
};

struct Color {
    int r;
    int g;
    int b;
};

struct Vec3 {
    int x;
    int y;
    int z;
};

struct Counter {
    int val;
};

struct Stack {
    int data[20];
    int top;
};

struct Node {
    int val;
    int next_idx;
};

struct Matrix3 {
    int data[9];
};

struct Complex {
    int re;
    int im;
};

struct Range {
    int lo;
    int hi;
};

// --- Point functions ---
struct Point make_point(int x, int y) {
    struct Point p;
    p.x = x;
    p.y = y;
    return p;
}

int distance_sq(struct Point a, struct Point b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return dx * dx + dy * dy;
}

struct Point move_point(struct Point p, int dx, int dy) {
    struct Point result;
    result.x = p.x + dx;
    result.y = p.y + dy;
    return result;
}

struct Point scale_point(struct Point p, int factor) {
    struct Point result;
    result.x = p.x * factor;
    result.y = p.y * factor;
    return result;
}

struct Point origin_point() {
    struct Point p;
    p.x = 0;
    p.y = 0;
    return p;
}

// --- Rect functions ---
int rect_area(struct Rect r) {
    return r.w * r.h;
}

int rect_perimeter(struct Rect r) {
    return 2 * (r.w + r.h);
}

// --- IdVal sum ---
int idval_sum(struct IdVal *arr, int n) {
    int s = 0;
    int i = 0;
    while (i < n) {
        s = s + arr[i].val;
        i = i + 1;
    }
    return s;
}

// --- Modify point via pointer ---
void modify_point(struct Point *p, int x, int y) {
    p->x = x;
    p->y = y;
}

// --- Student functions ---
int best_student(struct Student *students, int n) {
    int best_id = students[0].id;
    int best_grade = students[0].grade;
    int i = 1;
    while (i < n) {
        if (students[i].grade > best_grade) {
            best_grade = students[i].grade;
            best_id = students[i].id;
        }
        i = i + 1;
    }
    return best_id;
}

int avg_grade(struct Student *students, int n) {
    int s = 0;
    int i = 0;
    while (i < n) {
        s = s + students[i].grade;
        i = i + 1;
    }
    return s / n;
}

int total_grades(struct Student *students, int n) {
    int s = 0;
    int i = 0;
    while (i < n) {
        s = s + students[i].grade;
        i = i + 1;
    }
    return s;
}

// --- Pair functions ---
struct Pair make_pair(int a, int b) {
    struct Pair p;
    p.a = a;
    p.b = b;
    return p;
}

struct Pair swap_pair(struct Pair p) {
    struct Pair result;
    result.a = p.b;
    result.b = p.a;
    return result;
}

// --- Color functions ---
struct Color make_color(int r, int g, int b) {
    struct Color c;
    c.r = r;
    c.g = g;
    c.b = b;
    return c;
}

struct Color mix_color(struct Color a, struct Color b) {
    struct Color c;
    c.r = (a.r + b.r) / 2;
    c.g = (a.g + b.g) / 2;
    c.b = (a.b + b.b) / 2;
    return c;
}

// --- Vec3 functions ---
struct Vec3 make_vec3(int x, int y, int z) {
    struct Vec3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

int vec3_dot(struct Vec3 a, struct Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct Vec3 vec3_add(struct Vec3 a, struct Vec3 b) {
    struct Vec3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

// --- Counter functions ---
void counter_init(struct Counter *c, int val) {
    c->val = val;
}

void counter_inc(struct Counter *c) {
    c->val = c->val + 1;
}

void counter_add(struct Counter *c, int n) {
    c->val = c->val + n;
}

int counter_get(struct Counter *c) {
    return c->val;
}

// --- Stack functions ---
void stack_init(struct Stack *s) {
    s->top = -1;
}

void stack_push(struct Stack *s, int val) {
    s->top = s->top + 1;
    s->data[s->top] = val;
}

int stack_pop(struct Stack *s) {
    int val = s->data[s->top];
    s->top = s->top - 1;
    return val;
}

int stack_peek(struct Stack *s) {
    return s->data[s->top];
}

int stack_empty(struct Stack *s) {
    return s->top < 0;
}

// --- Matrix3 functions ---
void matrix3_set(struct Matrix3 *m, int r, int c, int val) {
    m->data[r * 3 + c] = val;
}

int matrix3_get(struct Matrix3 *m, int r, int c) {
    return m->data[r * 3 + c];
}

int matrix3_trace(struct Matrix3 *m) {
    return m->data[0] + m->data[4] + m->data[8];
}

// --- Complex functions ---
struct Complex complex_add(struct Complex a, struct Complex b) {
    struct Complex r;
    r.re = a.re + b.re;
    r.im = a.im + b.im;
    return r;
}

struct Complex complex_mul(struct Complex a, struct Complex b) {
    struct Complex r;
    r.re = a.re * b.re - a.im * b.im;
    r.im = a.re * b.im + a.im * b.re;
    return r;
}

// --- Range functions ---
int range_length(struct Range r) {
    return r.hi - r.lo + 1;
}

int range_contains(struct Range r, int val) {
    return val >= r.lo && val <= r.hi;
}

// --- Struct with array member: histogram ---
struct Histogram {
    int bins[10];
    int count;
};

void hist_init(struct Histogram *h) {
    h->count = 0;
    int i = 0;
    while (i < 10) {
        h->bins[i] = 0;
        i = i + 1;
    }
}

void hist_add(struct Histogram *h, int val) {
    if (val >= 0 && val < 10) {
        h->bins[val] = h->bins[val] + 1;
        h->count = h->count + 1;
    }
}

int hist_get(struct Histogram *h, int bin) {
    return h->bins[bin];
}

int main() {
    // --- Basic struct creation ---
    struct Point p1 = make_point(3, 4);
    printf("p1: x=%d y=%d\n", p1.x, p1.y);

    struct Point p2 = make_point(10, 20);
    printf("p2: x=%d y=%d\n", p2.x, p2.y);

    // dist_sq: (10-3)^2 + (20-4)^2 = 49 + 256 = 305
    int dsq = distance_sq(p1, p2);
    printf("dist_sq=%d\n", dsq);

    // move p2 by (3,4) => (13, 24)
    struct Point p3 = move_point(p2, 3, 4);
    printf("moved: x=%d y=%d\n", p3.x, p3.y);

    // scale p1 by 2 => (6, 8)
    struct Point p4 = scale_point(p1, 2);
    printf("scaled: x=%d y=%d\n", p4.x, p4.y);

    struct Point p0 = origin_point();
    printf("origin: x=%d y=%d\n", p0.x, p0.y);

    // --- Rect ---
    struct Rect rect;
    rect.x = 1; rect.y = 2; rect.w = 10; rect.h = 5;
    printf("rect: x=%d y=%d w=%d h=%d\n", rect.x, rect.y, rect.w, rect.h);
    printf("rect_area=%d\n", rect_area(rect));
    printf("rect_perim=%d\n", rect_perimeter(rect));

    // --- Person ---
    struct Person person;
    person.age = 25;
    person.id = 1001;
    person.score = 95;
    printf("person_age=%d\n", person.age);
    printf("person_id=%d\n", person.id);
    printf("person_score=%d\n", person.score);

    // --- Nested struct ---
    struct Outer outer;
    outer.a = 1;
    outer.inner.x = 10;
    outer.inner.y = 20;
    printf("nested: outer_a=%d inner_x=%d inner_y=%d\n",
        outer.a, outer.inner.x, outer.inner.y);

    // --- Array of structs ---
    struct IdVal arr[5];
    int i = 0;
    while (i < 5) {
        arr[i].id = i;
        arr[i].val = i * 10;
        i = i + 1;
    }
    printf("arr_struct_0: id=%d val=%d\n", arr[0].id, arr[0].val);
    printf("arr_struct_1: id=%d val=%d\n", arr[1].id, arr[1].val);
    printf("arr_struct_2: id=%d val=%d\n", arr[2].id, arr[2].val);
    printf("arr_struct_3: id=%d val=%d\n", arr[3].id, arr[3].val);
    printf("arr_struct_4: id=%d val=%d\n", arr[4].id, arr[4].val);
    printf("arr_sum=%d\n", idval_sum(arr, 5));

    // --- Struct pointer ---
    struct Point sp;
    sp.x = 42;
    sp.y = 84;
    struct Point *spp = &sp;
    printf("struct_ptr: x=%d y=%d\n", spp->x, spp->y);

    modify_point(&sp, 100, 200);
    printf("modify_via_ptr: x=%d y=%d\n", sp.x, sp.y);

    // --- Students ---
    struct Student students[5];
    students[0].id = 1; students[0].grade = 90;
    students[1].id = 2; students[1].grade = 85;
    students[2].id = 3; students[2].grade = 95;
    students[3].id = 4; students[3].grade = 70;
    students[4].id = 5; students[4].grade = 88;
    printf("student_0: id=%d grade=%d\n", students[0].id, students[0].grade);
    printf("student_1: id=%d grade=%d\n", students[1].id, students[1].grade);
    printf("student_2: id=%d grade=%d\n", students[2].id, students[2].grade);
    printf("student_3: id=%d grade=%d\n", students[3].id, students[3].grade);
    printf("student_4: id=%d grade=%d\n", students[4].id, students[4].grade);
    printf("best_student_id=%d\n", best_student(students, 5));
    printf("avg_grade=%d\n", avg_grade(students, 5));
    printf("total_grade=%d\n", total_grades(students, 5));

    // --- Pair ---
    struct Pair pr = make_pair(10, 20);
    printf("pair: a=%d b=%d\n", pr.a, pr.b);
    struct Pair pr2 = swap_pair(pr);
    printf("swap_pair: a=%d b=%d\n", pr2.a, pr2.b);

    // --- Color ---
    struct Color col = make_color(255, 128, 0);
    printf("color: r=%d g=%d b=%d\n", col.r, col.g, col.b);
    struct Color black = make_color(0, 0, 0);
    struct Color mixed = mix_color(col, black);
    printf("mix: r=%d g=%d b=%d\n", mixed.r, mixed.g, mixed.b);

    // --- Vec3 ---
    struct Vec3 v1 = make_vec3(1, 2, 3);
    printf("vec3: x=%d y=%d z=%d\n", v1.x, v1.y, v1.z);
    printf("vec3_dot=%d\n", vec3_dot(v1, v1));
    struct Vec3 v2 = vec3_add(v1, v1);
    printf("vec3_add: x=%d y=%d z=%d\n", v2.x, v2.y, v2.z);

    // --- Counter ---
    struct Counter cnt;
    counter_init(&cnt, 0);
    i = 0;
    while (i < 5) {
        counter_inc(&cnt);
        i = i + 1;
    }
    printf("counter: val=%d\n", counter_get(&cnt));
    counter_add(&cnt, 5);
    printf("counter_after_inc: val=%d\n", counter_get(&cnt));

    // --- Stack ---
    struct Stack stk;
    stack_init(&stk);
    stack_push(&stk, 10);
    stack_push(&stk, 20);
    stack_push(&stk, 30);
    printf("stack_top=%d\n", stack_pop(&stk));
    printf("stack_top=%d\n", stack_pop(&stk));
    printf("stack_top=%d\n", stack_pop(&stk));
    printf("stack_empty=%d\n", stack_empty(&stk));

    // --- Node chain ---
    struct Node nodes[5];
    nodes[0].val = 1; nodes[0].next_idx = 1;
    nodes[1].val = 2; nodes[1].next_idx = 2;
    nodes[2].val = 3; nodes[2].next_idx = 3;
    nodes[3].val = 4; nodes[3].next_idx = 4;
    nodes[4].val = 5; nodes[4].next_idx = -1;
    int idx = 0;
    printf("node_chain:");
    while (idx >= 0) {
        printf(" %d", nodes[idx].val);
        idx = nodes[idx].next_idx;
    }
    printf("\n");

    // --- Matrix3: data = {1,2,3,4,5,6,7,8,9} ---
    struct Matrix3 mat;
    int r = 0;
    while (r < 3) {
        int c = 0;
        while (c < 3) {
            matrix3_set(&mat, r, c, r * 3 + c + 1);
            c = c + 1;
        }
        r = r + 1;
    }
    printf("matrix_elem_00=%d\n", matrix3_get(&mat, 0, 0));
    printf("matrix_elem_11=%d\n", matrix3_get(&mat, 1, 1));
    // trace = 1 + 5 + 9 = 15
    printf("matrix_trace=%d\n", matrix3_trace(&mat));

    // --- Complex ---
    struct Complex c1;
    c1.re = 1; c1.im = 2;
    struct Complex c2;
    c2.re = 3; c2.im = 4;
    struct Complex csum = complex_add(c1, c2);
    printf("complex_add: re=%d im=%d\n", csum.re, csum.im);
    struct Complex cprod = complex_mul(c1, c2);
    printf("complex_mul: re=%d im=%d\n", cprod.re, cprod.im);

    // --- Range ---
    struct Range rng;
    rng.lo = 3;
    rng.hi = 9;
    printf("range_len=%d\n", range_length(rng));
    printf("range_contains_5=%d\n", range_contains(rng, 5));
    printf("range_contains_10=%d\n", range_contains(rng, 10));

    printf("done\n");
    return 0;
}
