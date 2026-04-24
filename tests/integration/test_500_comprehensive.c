int printf(const char *fmt, ...);
// EXPECT: === Comprehensive Feature Test ===\n--- Function Pointers ---\n  add(15, 4) = 19\n  sub(15, 4) = 11\n  mul(15, 4) = 60\n  div(15, 4) = 3\n--- Shapes & Structs ---\n  Circle at (0,0): area=78\n  Rectangle at (3,4): area=48\n  Triangle at (-2,1): area=35\n  Total area: 161\n  Distance^2 (0,0)-(3,4): 25, sqrt~5\n--- Array Operations ---\n  Sum: 356\n  Sorted: 5 17 28 36 42 61 74 93\n  Find 42: idx=4\n  Find 99: idx=-1\n--- Number Theory ---\n  Fibonacci: 0 1 1 2 3 5 8 13 21 34 55 \n  Primes < 30: 2 3 5 7 11 13 17 19 23 29\n  GCD(48,18)=6 GCD(100,75)=25 GCD(17,13)=1\n--- Bitwise ---\n  170 & 85 = 0\n  170 | 85 = 255\n  170 ^ 85 = 255\n--- Ternary & Logic ---\n  max(10,20)=20\n  min(10,20)=10\n  10>5 && 3<7: 1\n  10<5 || 3<7: 1\nAll tests passed!

typedef int (*BinOp)(int, int);

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int div_op(int a, int b) { return b != 0 ? a / b : 0; }

enum Shape { CIRCLE, RECTANGLE, TRIANGLE };

struct Point {
    int x;
    int y;
};

struct ShapeData {
    enum Shape type;
    struct Point center;
    int param1;
    int param2;
};

struct Collection {
    struct ShapeData shapes[8];
    int count;
};

typedef struct {
    int data[10];
    int size;
    int sorted;
} IntArray;

int isqrt(int n) {
    int x = 1;
    while (x * x <= n) x++;
    return x - 1;
}

int distance_sq(struct Point a, struct Point b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return dx * dx + dy * dy;
}

int shape_area(struct ShapeData *s) {
    if (s->type == CIRCLE) {
        return 314 * s->param1 * s->param1 / 100;
    } else if (s->type == RECTANGLE) {
        return s->param1 * s->param2;
    } else {
        return s->param1 * s->param2 / 2;
    }
}

const char *shape_name(enum Shape s) {
    if (s == CIRCLE) return "Circle";
    if (s == RECTANGLE) return "Rectangle";
    return "Triangle";
}

void init_array(IntArray *arr) {
    arr->size = 0;
    arr->sorted = 0;
}

void array_push(IntArray *arr, int val) {
    if (arr->size < 10) {
        arr->data[arr->size] = val;
        arr->size++;
        arr->sorted = 0;
    }
}

void array_sort(IntArray *arr) {
    int i, j;
    for (i = 0; i < arr->size - 1; i++) {
        for (j = i + 1; j < arr->size; j++) {
            if (arr->data[j] < arr->data[i]) {
                int tmp = arr->data[i];
                arr->data[i] = arr->data[j];
                arr->data[j] = tmp;
            }
        }
    }
    arr->sorted = 1;
}

int array_binary_search(IntArray *arr, int val) {
    if (!arr->sorted) return -1;
    int lo = 0;
    int hi = arr->size - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (arr->data[mid] == val) return mid;
        if (arr->data[mid] < val) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

int array_sum(IntArray *arr) {
    int sum = 0;
    int i;
    for (i = 0; i < arr->size; i++) sum = sum + arr->data[i];
    return sum;
}

int apply_op(BinOp op, int a, int b) {
    return op(a, b);
}

int fibonacci(int n) {
    if (n <= 1) return n;
    int a = 0;
    int b = 1;
    int i;
    for (i = 2; i <= n; i++) {
        int t = a + b;
        a = b;
        b = t;
    }
    return b;
}

int is_prime(int n) {
    if (n < 2) return 0;
    int i;
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int main(void) {
    printf("=== Comprehensive Feature Test ===\n");

    printf("--- Function Pointers ---\n");
    BinOp ops[4];
    ops[0] = add; ops[1] = sub; ops[2] = mul; ops[3] = div_op;
    const char *op_names[4];
    op_names[0] = "add"; op_names[1] = "sub"; op_names[2] = "mul"; op_names[3] = "div";
    int i;
    for (i = 0; i < 4; i++) {
        printf("  %s(15, 4) = %d\n", op_names[i], apply_op(ops[i], 15, 4));
    }

    printf("--- Shapes & Structs ---\n");
    struct Collection coll;
    coll.count = 0;

    coll.shapes[0].type = CIRCLE;
    coll.shapes[0].center.x = 0; coll.shapes[0].center.y = 0;
    coll.shapes[0].param1 = 5; coll.shapes[0].param2 = 0;
    coll.shapes[1].type = RECTANGLE;
    coll.shapes[1].center.x = 3; coll.shapes[1].center.y = 4;
    coll.shapes[1].param1 = 6; coll.shapes[1].param2 = 8;
    coll.shapes[2].type = TRIANGLE;
    coll.shapes[2].center.x = -2; coll.shapes[2].center.y = 1;
    coll.shapes[2].param1 = 10; coll.shapes[2].param2 = 7;
    coll.count = 3;

    int total_area = 0;
    for (i = 0; i < coll.count; i++) {
        int a = shape_area(&coll.shapes[i]);
        total_area = total_area + a;
        printf("  %s at (%d,%d): area=%d\n",
               shape_name(coll.shapes[i].type),
               coll.shapes[i].center.x, coll.shapes[i].center.y, a);
    }
    printf("  Total area: %d\n", total_area);

    struct Point p1 = {0, 0};
    struct Point p2 = {3, 4};
    printf("  Distance^2 (0,0)-(3,4): %d, sqrt~%d\n",
           distance_sq(p1, p2), isqrt(distance_sq(p1, p2)));

    printf("--- Array Operations ---\n");
    IntArray arr;
    init_array(&arr);
    int vals[8];
    vals[0] = 42; vals[1] = 17; vals[2] = 93; vals[3] = 5;
    vals[4] = 28; vals[5] = 61; vals[6] = 74; vals[7] = 36;
    for (i = 0; i < 8; i++) array_push(&arr, vals[i]);

    printf("  Sum: %d\n", array_sum(&arr));
    array_sort(&arr);
    printf("  Sorted:");
    for (i = 0; i < arr.size; i++) printf(" %d", arr.data[i]);
    printf("\n");
    printf("  Find 42: idx=%d\n", array_binary_search(&arr, 42));
    printf("  Find 99: idx=%d\n", array_binary_search(&arr, 99));

    printf("--- Number Theory ---\n");
    printf("  Fibonacci: ");
    for (i = 0; i <= 10; i++) printf("%d ", fibonacci(i));
    printf("\n");

    printf("  Primes < 30:");
    for (i = 2; i < 30; i++) {
        if (is_prime(i)) printf(" %d", i);
    }
    printf("\n");

    printf("  GCD(48,18)=%d GCD(100,75)=%d GCD(17,13)=%d\n",
           gcd(48, 18), gcd(100, 75), gcd(17, 13));

    printf("--- Bitwise ---\n");
    int x = 170;
    int y = 85;
    printf("  %d & %d = %d\n", x, y, x & y);
    printf("  %d | %d = %d\n", x, y, x | y);
    printf("  %d ^ %d = %d\n", x, y, x ^ y);

    printf("--- Ternary & Logic ---\n");
    int a = 10;
    int b = 20;
    printf("  max(%d,%d)=%d\n", a, b, a > b ? a : b);
    printf("  min(%d,%d)=%d\n", a, b, a < b ? a : b);
    printf("  10>5 && 3<7: %d\n", (10 > 5) && (3 < 7));
    printf("  10<5 || 3<7: %d\n", (10 < 5) || (3 < 7));

    printf("All tests passed!\n");

    return 0;
}
