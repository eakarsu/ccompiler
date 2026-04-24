int printf(const char *fmt, ...);

// Single int parameter
int square(int x) {
    return x * x;
}

// Single char parameter (promoted to int)
int char_to_int(char c) {
    return c;
}

int is_upper(char c) {
    return (c >= 65 && c <= 90) ? 1 : 0;
}

// Pointer parameter: modify the pointed-to value
void increment(int *p) {
    *p = *p + 1;
}

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// Array as pointer parameter
int array_sum(int *arr, int n) {
    int total = 0;
    int i;
    for (i = 0; i < n; i = i + 1) {
        total = total + arr[i];
    }
    return total;
}

int array_min(int *arr, int n) {
    int min = arr[0];
    int i;
    for (i = 1; i < n; i = i + 1) {
        if (arr[i] < min) min = arr[i];
    }
    return min;
}

// Struct passed by pointer
struct Point {
    int x;
    int y;
};

int point_distance_sq(struct Point *p1, struct Point *p2) {
    int dx = p1->x - p2->x;
    int dy = p1->y - p2->y;
    return dx * dx + dy * dy;
}

void point_translate(struct Point *p, int dx, int dy) {
    p->x = p->x + dx;
    p->y = p->y + dy;
}

struct Rect {
    int x;
    int y;
    int w;
    int h;
};

int rect_area(struct Rect *r) {
    return r->w * r->h;
}

int rect_contains(struct Rect *r, int px, int py) {
    return (px >= r->x && px < r->x + r->w &&
            py >= r->y && py < r->y + r->h) ? 1 : 0;
}

// Many parameters (6)
int sum6(int a, int b, int c, int d, int e, int f) {
    return a + b + c + d + e + f;
}

// 7 parameters - tests stack passing on ARM64
int sum7(int a, int b, int c, int d, int e, int f, int g) {
    return a + b + c + d + e + f + g;
}

// 8 parameters
int sum8(int a, int b, int c, int d, int e, int f, int g, int h) {
    return a + b + c + d + e + f + g + h;
}

// Mixed types: int and pointer
int read_and_add(int *p, int offset) {
    return *p + offset;
}

// Pointer to pointer
void set_ptr(int **pp, int *target) {
    *pp = target;
}

// String (char pointer) parameter
int string_length(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

int strings_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return (a[i] == 0 && b[i] == 0) ? 1 : 0;
}

int main(void) {
    // Single int
    // EXPECT: 49
    printf("%d\n", square(7));
    // EXPECT: 144
    printf("%d\n", square(12));

    // Char parameter: 'A' = 65
    // EXPECT: 65
    printf("%d\n", char_to_int('A'));
    // EXPECT: 1
    printf("%d\n", is_upper('Z'));
    // EXPECT: 0
    printf("%d\n", is_upper('a'));

    // Pointer parameter
    int val = 10;
    increment(&val);
    // EXPECT: 11
    printf("%d\n", val);

    int x = 3;
    int y = 7;
    swap(&x, &y);
    // EXPECT: 7 3
    printf("%d %d\n", x, y);

    // Array parameter
    int arr[] = {10, 20, 30, 40, 50};
    // EXPECT: 150
    printf("%d\n", array_sum(arr, 5));
    // EXPECT: 10
    printf("%d\n", array_min(arr, 5));

    // Struct by pointer
    struct Point p1;
    p1.x = 0;
    p1.y = 0;
    struct Point p2;
    p2.x = 3;
    p2.y = 4;
    // distance_sq = 9 + 16 = 25
    // EXPECT: 25
    printf("%d\n", point_distance_sq(&p1, &p2));

    point_translate(&p1, 5, 10);
    // EXPECT: 5 10
    printf("%d %d\n", p1.x, p1.y);

    struct Rect r;
    r.x = 10;
    r.y = 20;
    r.w = 30;
    r.h = 40;
    // EXPECT: 1200
    printf("%d\n", rect_area(&r));
    // EXPECT: 1
    printf("%d\n", rect_contains(&r, 15, 25));
    // EXPECT: 0
    printf("%d\n", rect_contains(&r, 5, 25));

    // Many parameters
    // EXPECT: 21
    printf("%d\n", sum6(1, 2, 3, 4, 5, 6));
    // EXPECT: 28
    printf("%d\n", sum7(1, 2, 3, 4, 5, 6, 7));
    // EXPECT: 36
    printf("%d\n", sum8(1, 2, 3, 4, 5, 6, 7, 8));

    // Mixed int and pointer
    int v = 100;
    // EXPECT: 142
    printf("%d\n", read_and_add(&v, 42));

    // Pointer to pointer
    int a = 11;
    int b = 22;
    int *ptr = &a;
    set_ptr(&ptr, &b);
    // EXPECT: 22
    printf("%d\n", *ptr);

    // String parameters
    char hello[] = "hello";
    // EXPECT: 5
    printf("%d\n", string_length(hello));
    char hello2[] = "hello";
    char world[] = "world";
    // EXPECT: 1
    printf("%d\n", strings_equal(hello, hello2));
    // EXPECT: 0
    printf("%d\n", strings_equal(hello, world));

    return 0;
}
