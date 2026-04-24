int printf(const char *fmt, ...);

/* Pass by value: original is unchanged */
void increment_val(int x) {
    x += 1; /* local copy only */
}

/* Pass by pointer: original is modified */
void increment_ptr(int *x) {
    *x += 1;
}

void swap_val(int a, int b) {
    int tmp = a;
    a = b;
    b = tmp;
    /* caller's values unchanged */
}

void swap_ptr(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Modify multiple values through pointers */
void min_max(int a, int b, int *mn, int *mx) {
    if (a < b) { *mn = a; *mx = b; }
    else        { *mn = b; *mx = a; }
}

/* Array passed as pointer (decays) */
int array_sum(int *arr, int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i++) total += arr[i];
    return total;
}

void array_scale(int *arr, int n, int factor) {
    int i;
    for (i = 0; i < n; i++) arr[i] *= factor;
}

void array_fill(int *arr, int n, int value) {
    int i;
    for (i = 0; i < n; i++) arr[i] = value;
}

int array_max(int *arr, int n) {
    int m = arr[0];
    int i;
    for (i = 1; i < n; i++) if (arr[i] > m) m = arr[i];
    return m;
}

/* Struct passing */
struct Point {
    int x;
    int y;
};

struct Point make_point(int x, int y) {
    struct Point p;
    p.x = x;
    p.y = y;
    return p;
}

/* Pass struct by value */
int point_distance_sq(struct Point a, struct Point b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return dx * dx + dy * dy;
}

struct Point point_midpoint_scaled(struct Point a, struct Point b) {
    struct Point result;
    result.x = (a.x + b.x);  /* sum, not divide */
    result.y = (a.y + b.y);
    return result;
}

/* Pass struct by pointer */
void point_translate(struct Point *p, int dx, int dy) {
    p->x += dx;
    p->y += dy;
}

void point_scale(struct Point *p, int factor) {
    p->x *= factor;
    p->y *= factor;
}

/* Struct with more fields */
struct Rect {
    int x;
    int y;
    int w;
    int h;
};

int rect_area(struct Rect r) {
    return r.w * r.h;
}

int rect_perimeter(struct Rect r) {
    return 2 * (r.w + r.h);
}

void rect_grow(struct Rect *r, int amount) {
    r->x -= amount;
    r->y -= amount;
    r->w += 2 * amount;
    r->h += 2 * amount;
}

/* Pass pointer to pointer */
void reset_ptr(int **pp, int *new_target) {
    *pp = new_target;
}

/* Output param pattern */
int divide_with_rem(int a, int b, int *rem) {
    *rem = a % b;
    return a / b;
}

int main(void) {
    /* Pass by value: no change */
    int x = 10;
    increment_val(x);
    printf("%d\n", x);          // EXPECT: 10

    /* Pass by pointer: modified */
    increment_ptr(&x);
    printf("%d\n", x);          // EXPECT: 11

    /* swap by value: no change */
    int a = 3, b = 7;
    swap_val(a, b);
    printf("%d\n", a);          // EXPECT: 3
    printf("%d\n", b);          // EXPECT: 7

    /* swap by pointer: changed */
    swap_ptr(&a, &b);
    printf("%d\n", a);          // EXPECT: 7
    printf("%d\n", b);          // EXPECT: 3

    /* min_max */
    int mn, mx;
    min_max(8, 3, &mn, &mx);
    printf("%d\n", mn);         // EXPECT: 3
    printf("%d\n", mx);         // EXPECT: 8
    min_max(5, 5, &mn, &mx);
    printf("%d\n", mn);         // EXPECT: 5
    printf("%d\n", mx);         // EXPECT: 5

    /* Array passing */
    int arr[5];
    arr[0]=10; arr[1]=20; arr[2]=30; arr[3]=40; arr[4]=50;
    printf("%d\n", array_sum(arr, 5));    // EXPECT: 150
    printf("%d\n", array_max(arr, 5));    // EXPECT: 50

    array_scale(arr, 5, 2);
    printf("%d\n", arr[0]);     // EXPECT: 20
    printf("%d\n", arr[4]);     // EXPECT: 100
    printf("%d\n", array_sum(arr, 5)); // EXPECT: 300

    array_fill(arr, 5, 7);
    printf("%d\n", array_sum(arr, 5)); // EXPECT: 35

    /* Struct by value */
    struct Point p1; p1.x = 0; p1.y = 0;
    struct Point p2; p2.x = 3; p2.y = 4;
    printf("%d\n", point_distance_sq(p1, p2)); // EXPECT: 25

    struct Point p3 = make_point(1, 2);
    struct Point p4 = make_point(5, 8);
    printf("%d\n", p3.x);       // EXPECT: 1
    printf("%d\n", p3.y);       // EXPECT: 2

    struct Point sum_pt = point_midpoint_scaled(p3, p4);
    printf("%d\n", sum_pt.x);   // EXPECT: 6
    printf("%d\n", sum_pt.y);   // EXPECT: 10

    /* Struct by pointer */
    struct Point p5; p5.x = 2; p5.y = 3;
    point_translate(&p5, 5, -1);
    printf("%d\n", p5.x);       // EXPECT: 7
    printf("%d\n", p5.y);       // EXPECT: 2

    point_scale(&p5, 3);
    printf("%d\n", p5.x);       // EXPECT: 21
    printf("%d\n", p5.y);       // EXPECT: 6

    /* Rect struct */
    struct Rect r;
    r.x=0; r.y=0; r.w=10; r.h=5;
    printf("%d\n", rect_area(r));       // EXPECT: 50
    printf("%d\n", rect_perimeter(r));  // EXPECT: 30

    rect_grow(&r, 2);
    printf("%d\n", r.x);        // EXPECT: -2
    printf("%d\n", r.y);        // EXPECT: -2
    printf("%d\n", r.w);        // EXPECT: 14
    printf("%d\n", r.h);        // EXPECT: 9
    printf("%d\n", rect_area(r)); // EXPECT: 126

    /* Pointer-to-pointer */
    int v1 = 100, v2 = 200;
    int *ptr = &v1;
    printf("%d\n", *ptr);       // EXPECT: 100
    reset_ptr(&ptr, &v2);
    printf("%d\n", *ptr);       // EXPECT: 200

    /* Output param */
    int rem;
    int quot = divide_with_rem(17, 5, &rem);
    printf("%d\n", quot);       // EXPECT: 3
    printf("%d\n", rem);        // EXPECT: 2

    quot = divide_with_rem(100, 7, &rem);
    printf("%d\n", quot);       // EXPECT: 14
    printf("%d\n", rem);        // EXPECT: 2

    return 0;
}
