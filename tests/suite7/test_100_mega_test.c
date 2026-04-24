int printf(const char *fmt, ...);

// Mega test: nested structs, function pointers, recursion, enums, unions,
// arrays, pointers, scoping, goto, switch, all operators

// ---- Enums ----
enum Color { RED = 0, GREEN = 1, BLUE = 2, YELLOW = 3 };
enum Op { OP_ADD = 1, OP_SUB = 2, OP_MUL = 3, OP_DIV = 4, OP_MOD = 5 };
enum Status { INACTIVE = -1, IDLE = 0, ACTIVE = 1, ERROR = -2 };

// ---- Unions ----
union Value {
    int i;
    char c;
};

// ---- Nested structs ----
struct Point {
    int x;
    int y;
};

struct Rect {
    struct Point top_left;
    struct Point bottom_right;
};

struct Color3 {
    int r;
    int g;
    int b;
};

struct Pixel {
    struct Point pos;
    struct Color3 color;
    int alpha;
};

struct Node {
    int value;
    int left;
    int right;
};

struct Tree {
    struct Node nodes[15];
    int count;
};

// ---- Function pointer types ----
typedef int (*BinOp)(int, int);
typedef int (*UnaryOp)(int);
typedef int (*Predicate)(int);

// ---- Basic arithmetic functions ----
int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int safe_div(int a, int b) { return (b != 0) ? a / b : 0; }
int mod(int a, int b) { return (b != 0) ? a % b : 0; }

// ---- Unary functions ----
int negate(int x) { return -x; }
int square(int x) { return x * x; }
int identity(int x) { return x; }

// ---- Predicates ----
int is_positive(int x) { return x > 0; }
int is_even(int x) { return (x % 2 == 0) ? 1 : 0; }
int is_nonzero(int x) { return (x != 0) ? 1 : 0; }

// ---- Recursive functions ----
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int power(int base, int exp) {
    if (exp == 0) return 1;
    if (exp == 1) return base;
    int half = power(base, exp / 2);
    if (exp % 2 == 0) return half * half;
    return half * half * base;
}

int tree_sum(struct Tree *t, int idx) {
    if (idx < 0 || idx >= t->count) return 0;
    return t->nodes[idx].value +
           tree_sum(t, t->nodes[idx].left) +
           tree_sum(t, t->nodes[idx].right);
}

int tree_depth(struct Tree *t, int idx) {
    if (idx < 0 || idx >= t->count) return 0;
    int ld = tree_depth(t, t->nodes[idx].left);
    int rd = tree_depth(t, t->nodes[idx].right);
    return 1 + ((ld > rd) ? ld : rd);
}

// ---- Geometry ----
int rect_area(struct Rect *r) {
    int w = r->bottom_right.x - r->top_left.x;
    int h = r->bottom_right.y - r->top_left.y;
    if (w < 0) w = -w;
    if (h < 0) h = -h;
    return w * h;
}

int point_distance_sq(struct Point *a, struct Point *b) {
    int dx = a->x - b->x;
    int dy = a->y - b->y;
    return dx * dx + dy * dy;
}

// ---- Higher-order functions ----
int apply_binop(BinOp op, int a, int b) {
    return op(a, b);
}

int apply_unary(UnaryOp op, int x) {
    return op(x);
}

int count_matching(int *arr, int len, Predicate pred) {
    int count = 0;
    int i;
    for (i = 0; i < len; i++) {
        if (pred(arr[i])) count = count + 1;
    }
    return count;
}

int reduce(int *arr, int len, BinOp op, int init) {
    int result = init;
    int i;
    for (i = 0; i < len; i++) {
        result = op(result, arr[i]);
    }
    return result;
}

void map_inplace(int *arr, int len, UnaryOp op) {
    int i;
    for (i = 0; i < len; i++) {
        arr[i] = op(arr[i]);
    }
}

// ---- Array utilities ----
int array_sum(int *arr, int len) {
    int s = 0;
    int i;
    for (i = 0; i < len; i++) s = s + arr[i];
    return s;
}

int array_max(int *arr, int len) {
    int m = arr[0];
    int i;
    for (i = 1; i < len; i++) {
        if (arr[i] > m) m = arr[i];
    }
    return m;
}

void bubble_sort(int *arr, int len) {
    int i;
    int j;
    for (i = 0; i < len - 1; i++) {
        for (j = 0; j < len - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

int binary_search(int *arr, int len, int target) {
    int lo = 0;
    int hi = len - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

// ---- Dispatch table ----
int dispatch(enum Op op, int a, int b) {
    switch (op) {
        case OP_ADD: return a + b;
        case OP_SUB: return a - b;
        case OP_MUL: return a * b;
        case OP_DIV: return (b != 0) ? a / b : 0;
        case OP_MOD: return (b != 0) ? a % b : 0;
    }
    return 0;
}

// ---- Status string ----
int status_code(enum Status s) {
    switch (s) {
        case INACTIVE: return -1;
        case IDLE: return 0;
        case ACTIVE: return 1;
        case ERROR: return -2;
    }
    return -99;
}

// ---- Matrix operations ----
void mat_multiply(int a[3][3], int b[3][3], int result[3][3]) {
    int i;
    int j;
    int k;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[i][j] = 0;
            for (k = 0; k < 3; k++) {
                result[i][j] = result[i][j] + a[i][k] * b[k][j];
            }
        }
    }
}

int main(void) {
    int i;
    int result;

    // ---- Test 1: Enum values ----
    // EXPECT: Colors: 0 1 2 3
    printf("Colors: %d %d %d %d\n", RED, GREEN, BLUE, YELLOW);

    // EXPECT: Status: -1 0 1 -2
    printf("Status: %d %d %d %d\n", INACTIVE, IDLE, ACTIVE, ERROR);

    // ---- Test 2: Union ----
    {
        union Value v;
        v.i = 65;
        // EXPECT: Union int: 65
        printf("Union int: %d\n", v.i);
    }

    // ---- Test 3: Nested structs ----
    {
        struct Pixel px;
        px.pos.x = 10;
        px.pos.y = 20;
        px.color.r = 255;
        px.color.g = 128;
        px.color.b = 0;
        px.alpha = 200;
        // EXPECT: Pixel: 10,20 rgb=255,128,0 a=200
        printf("Pixel: %d,%d rgb=%d,%d,%d a=%d\n",
               px.pos.x, px.pos.y, px.color.r, px.color.g, px.color.b, px.alpha);
    }

    // ---- Test 4: Rectangle area ----
    {
        struct Rect r;
        r.top_left.x = 1;
        r.top_left.y = 1;
        r.bottom_right.x = 5;
        r.bottom_right.y = 4;
        // EXPECT: Rect area: 12
        printf("Rect area: %d\n", rect_area(&r));
    }

    // ---- Test 5: Point distance squared ----
    {
        struct Point p1;
        struct Point p2;
        p1.x = 0; p1.y = 0;
        p2.x = 3; p2.y = 4;
        // EXPECT: Dist sq: 25
        printf("Dist sq: %d\n", point_distance_sq(&p1, &p2));
    }

    // ---- Test 6: Function pointers ----
    {
        BinOp ops[4];
        ops[0] = add;
        ops[1] = sub;
        ops[2] = mul;
        ops[3] = safe_div;
        // EXPECT: FP add: 15
        printf("FP add: %d\n", ops[0](10, 5));
        // EXPECT: FP sub: 5
        printf("FP sub: %d\n", ops[1](10, 5));
        // EXPECT: FP mul: 50
        printf("FP mul: %d\n", ops[2](10, 5));
        // EXPECT: FP div: 2
        printf("FP div: %d\n", ops[3](10, 5));
    }

    // ---- Test 7: Higher-order ----
    // EXPECT: Apply add(3,4): 7
    printf("Apply add(3,4): %d\n", apply_binop(add, 3, 4));

    // EXPECT: Apply square(6): 36
    printf("Apply square(6): %d\n", apply_unary(square, 6));

    // EXPECT: Apply negate(7): -7
    printf("Apply negate(7): %d\n", apply_unary(negate, 7));

    // ---- Test 8: Counting with predicate ----
    {
        int data[8];
        data[0] = -3; data[1] = 0; data[2] = 5; data[3] = -1;
        data[4] = 8; data[5] = 2; data[6] = -4; data[7] = 6;
        // EXPECT: Positive count: 4
        printf("Positive count: %d\n", count_matching(data, 8, is_positive));
        // EXPECT: Even count: 5
        printf("Even count: %d\n", count_matching(data, 8, is_even));
    }

    // ---- Test 9: Reduce ----
    {
        int vals[5];
        vals[0] = 1; vals[1] = 2; vals[2] = 3; vals[3] = 4; vals[4] = 5;
        // EXPECT: Reduce sum: 15
        printf("Reduce sum: %d\n", reduce(vals, 5, add, 0));
        // EXPECT: Reduce mul: 120
        printf("Reduce mul: %d\n", reduce(vals, 5, mul, 1));
    }

    // ---- Test 10: Map in-place ----
    {
        int arr[4];
        arr[0] = 2; arr[1] = 3; arr[2] = 4; arr[3] = 5;
        map_inplace(arr, 4, square);
        // EXPECT: Squared: 4 9 16 25
        printf("Squared: %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3]);
    }

    // ---- Test 11: Recursion ----
    // EXPECT: Fact 7: 5040
    printf("Fact 7: %d\n", factorial(7));

    // EXPECT: Fib 10: 55
    printf("Fib 10: %d\n", fibonacci(10));

    // EXPECT: GCD 48,18: 6
    printf("GCD 48,18: %d\n", gcd(48, 18));

    // EXPECT: Power 2^10: 1024
    printf("Power 2^10: %d\n", power(2, 10));

    // ---- Test 12: Tree sum and depth ----
    {
        struct Tree t;
        t.count = 7;
        t.nodes[0].value = 10; t.nodes[0].left = 1; t.nodes[0].right = 2;
        t.nodes[1].value = 5;  t.nodes[1].left = 3; t.nodes[1].right = 4;
        t.nodes[2].value = 15; t.nodes[2].left = 5; t.nodes[2].right = 6;
        t.nodes[3].value = 2;  t.nodes[3].left = -1; t.nodes[3].right = -1;
        t.nodes[4].value = 7;  t.nodes[4].left = -1; t.nodes[4].right = -1;
        t.nodes[5].value = 12; t.nodes[5].left = -1; t.nodes[5].right = -1;
        t.nodes[6].value = 20; t.nodes[6].left = -1; t.nodes[6].right = -1;
        // EXPECT: Tree sum: 71
        printf("Tree sum: %d\n", tree_sum(&t, 0));
        // EXPECT: Tree depth: 3
        printf("Tree depth: %d\n", tree_depth(&t, 0));
    }

    // ---- Test 13: Sorting and binary search ----
    {
        int data[8];
        data[0] = 42; data[1] = 17; data[2] = 8; data[3] = 91;
        data[4] = 3; data[5] = 55; data[6] = 23; data[7] = 64;
        bubble_sort(data, 8);
        // EXPECT: Sorted: 3 8 17 23 42 55 64 91
        printf("Sorted: %d %d %d %d %d %d %d %d\n",
               data[0], data[1], data[2], data[3],
               data[4], data[5], data[6], data[7]);
        // EXPECT: Find 42: 4
        printf("Find 42: %d\n", binary_search(data, 8, 42));
        // EXPECT: Find 99: -1
        printf("Find 99: %d\n", binary_search(data, 8, 99));
    }

    // ---- Test 14: Switch dispatch ----
    // EXPECT: Dispatch add: 13
    printf("Dispatch add: %d\n", dispatch(OP_ADD, 8, 5));
    // EXPECT: Dispatch mod: 3
    printf("Dispatch mod: %d\n", dispatch(OP_MOD, 13, 5));

    // ---- Test 15: All operators ----
    {
        int a = 42;
        int b = 7;
        // EXPECT: Ops: 49 35 294 6 0
        printf("Ops: %d %d %d %d %d\n", a + b, a - b, a * b, a / b, a % b);
        // EXPECT: Bit: 2 47 45
        printf("Bit: %d %d %d\n", a & b, a | b, a ^ b);
        // EXPECT: Shift: 168 10
        printf("Shift: %d %d\n", a << 2, a >> 2);
        // EXPECT: Logic: 1 1 0
        printf("Logic: %d %d %d\n", a && b, a || b, !a);
        // EXPECT: Cmp: 0 1 1 0 1 0
        printf("Cmp: %d %d %d %d %d %d\n", a == b, a != b, a > b, a < b, a >= b, a <= b);
        // EXPECT: Ternary: 42
        printf("Ternary: %d\n", (a > b) ? a : b);
    }

    // ---- Test 16: Scoping ----
    {
        int x = 100;
        {
            int x = 200;
            {
                int x = 300;
                // EXPECT: Inner x: 300
                printf("Inner x: %d\n", x);
            }
            // EXPECT: Mid x: 200
            printf("Mid x: %d\n", x);
        }
        // EXPECT: Outer x: 100
        printf("Outer x: %d\n", x);
    }

    // ---- Test 17: Goto ----
    {
        int val = 0;
        goto skip_set;
        val = 999;
skip_set:
        // EXPECT: Goto skip: 0
        printf("Goto skip: %d\n", val);
    }

    {
        int sum = 0;
        int idx = 0;
loop_top:
        if (idx >= 5) goto loop_done;
        sum = sum + idx;
        idx = idx + 1;
        goto loop_top;
loop_done:
        // EXPECT: Goto loop sum: 10
        printf("Goto loop sum: %d\n", sum);
    }

    // ---- Test 18: Nested switch ----
    {
        int outer = 2;
        int inner = 1;
        int result = 0;
        switch (outer) {
            case 1: result = 10; break;
            case 2:
                switch (inner) {
                    case 0: result = 20; break;
                    case 1: result = 21; break;
                    case 2: result = 22; break;
                }
                break;
            case 3: result = 30; break;
        }
        // EXPECT: Nested switch: 21
        printf("Nested switch: %d\n", result);
    }

    // ---- Test 19: Pointer arithmetic and arrays ----
    {
        int arr[10];
        int *p;
        int *q;
        for (i = 0; i < 10; i++) arr[i] = (i + 1) * 10;
        p = arr;
        q = arr + 5;
        // EXPECT: Ptr: 10 60
        printf("Ptr: %d %d\n", *p, *q);
        // EXPECT: Ptr arith: 30 80
        printf("Ptr arith: %d %d\n", *(p + 2), *(q + 2));
    }

    // ---- Test 20: 2D array ----
    {
        int mat[3][3];
        int row;
        int col;
        for (row = 0; row < 3; row++) {
            for (col = 0; col < 3; col++) {
                mat[row][col] = row * 3 + col + 1;
            }
        }
        // EXPECT: Mat[0]: 1 2 3
        printf("Mat[0]: %d %d %d\n", mat[0][0], mat[0][1], mat[0][2]);
        // EXPECT: Mat[2]: 7 8 9
        printf("Mat[2]: %d %d %d\n", mat[2][0], mat[2][1], mat[2][2]);
    }

    // ---- Test 21: Matrix multiplication ----
    {
        int a[3][3];
        int b[3][3];
        int c[3][3];
        // Identity * [1,2,3; 4,5,6; 7,8,9]
        int r;
        int co;
        for (r = 0; r < 3; r++)
            for (co = 0; co < 3; co++)
                a[r][co] = (r == co) ? 1 : 0;
        b[0][0] = 1; b[0][1] = 2; b[0][2] = 3;
        b[1][0] = 4; b[1][1] = 5; b[1][2] = 6;
        b[2][0] = 7; b[2][1] = 8; b[2][2] = 9;
        mat_multiply(a, b, c);
        // EXPECT: MatMul[0]: 1 2 3
        printf("MatMul[0]: %d %d %d\n", c[0][0], c[0][1], c[0][2]);
        // EXPECT: MatMul[2]: 7 8 9
        printf("MatMul[2]: %d %d %d\n", c[2][0], c[2][1], c[2][2]);
    }

    // ---- Test 22: Complex control flow ----
    {
        int sum = 0;
        for (i = 0; i < 20; i++) {
            if (i % 3 == 0) continue;
            if (i > 15) break;
            sum = sum + i;
        }
        // EXPECT: Complex loop sum: 75
        printf("Complex loop sum: %d\n", sum);
    }

    // ---- Test 23: Do-while ----
    {
        int n = 1;
        int count = 0;
        do {
            n = n * 2;
            count = count + 1;
        } while (n < 100);
        // EXPECT: Do-while: n=128 count=7
        printf("Do-while: n=%d count=%d\n", n, count);
    }

    // ---- Test 24: Compound assignment ----
    {
        int v = 10;
        v += 5;
        // EXPECT: After +=: 15
        printf("After +=: %d\n", v);
        v *= 3;
        // EXPECT: After *=: 45
        printf("After *=: %d\n", v);
        v -= 10;
        // EXPECT: After -=: 35
        printf("After -=: %d\n", v);
        v /= 7;
        // EXPECT: After /=: 5
        printf("After /=: %d\n", v);
        v %= 3;
        // EXPECT: After mod=: 2
        printf("After mod=: %d\n", v);
    }

    // ---- Test 25: Struct array ----
    {
        struct Point pts[4];
        int total = 0;
        pts[0].x = 1; pts[0].y = 2;
        pts[1].x = 3; pts[1].y = 4;
        pts[2].x = 5; pts[2].y = 6;
        pts[3].x = 7; pts[3].y = 8;
        for (i = 0; i < 4; i++) {
            total = total + pts[i].x + pts[i].y;
        }
        // EXPECT: Point array sum: 36
        printf("Point array sum: %d\n", total);
    }

    // ---- Test 26: Increment/decrement ----
    {
        int a = 5;
        int b;
        b = a++;
        // EXPECT: Post inc: a=6 b=5
        printf("Post inc: a=%d b=%d\n", a, b);
        b = ++a;
        // EXPECT: Pre inc: a=7 b=7
        printf("Pre inc: a=%d b=%d\n", a, b);
        b = a--;
        // EXPECT: Post dec: a=6 b=7
        printf("Post dec: a=%d b=%d\n", a, b);
    }

    // ---- Test 27: Status codes with negative enums ----
    // EXPECT: Status INACTIVE: -1
    printf("Status INACTIVE: %d\n", status_code(INACTIVE));
    // EXPECT: Status ERROR: -2
    printf("Status ERROR: %d\n", status_code(ERROR));

    // ---- Test 28: Array max ----
    {
        int data[6];
        data[0] = 33; data[1] = 77; data[2] = 11;
        data[3] = 99; data[4] = 44; data[5] = 55;
        // EXPECT: Array max: 99
        printf("Array max: %d\n", array_max(data, 6));
        // EXPECT: Array sum: 319
        printf("Array sum: %d\n", array_sum(data, 6));
    }

    // ---- Final ----
    // EXPECT: MEGA TEST COMPLETE
    printf("MEGA TEST COMPLETE\n");

    return 0;
}
