int printf(const char *fmt, ...);

/* Mixed features: structs, designated init, compound literals, function pointers,
   callback patterns with enums */

/* --- Enums --- */
typedef enum {
    OP_ADD = 0,
    OP_SUB = 1,
    OP_MUL = 2,
    OP_DIV = 3,
    OP_MOD = 4
} OpCode;

typedef enum {
    STATUS_OK    = 0,
    STATUS_ERROR = 1,
    STATUS_DONE  = 2
} Status;

/* --- Struct with bitfields --- */
typedef struct {
    unsigned int r : 8;
    unsigned int g : 8;
    unsigned int b : 8;
    unsigned int a : 8;
} RGBA;

/* --- Struct with function pointer member --- */
typedef struct {
    int (*compute)(int, int);
    const char *name;
    int priority;
} Operation;

/* --- Struct with array member --- */
typedef struct {
    int data[8];
    int size;
    int capacity;
} IntVec;

/* --- Nested struct with designated init --- */
typedef struct {
    int x;
    int y;
    int z;
} Vec3;

typedef struct {
    Vec3 origin;
    Vec3 direction;
    int length;
} Ray;

/* --- Function pointer callback --- */
typedef int (*Predicate)(int);
typedef void (*Transform)(int *, int);

/* Operation implementations */
int op_add(int a, int b) { return a + b; }
int op_sub(int a, int b) { return a - b; }
int op_mul(int a, int b) { return a * b; }
int op_div(int a, int b) { return (b != 0) ? a / b : 0; }
int op_mod(int a, int b) { return (b != 0) ? a % b : 0; }

/* Dispatch by OpCode */
int dispatch(OpCode op, int a, int b) {
    Operation ops[5] = {
        {.compute = op_add, .name = "add", .priority = 1},
        {.compute = op_sub, .name = "sub", .priority = 1},
        {.compute = op_mul, .name = "mul", .priority = 2},
        {.compute = op_div, .name = "div", .priority = 2},
        {.compute = op_mod, .name = "mod", .priority = 2}
    };
    if (op < 0 || op > 4) return -1;
    return ops[op].compute(a, b);
}

/* Callback: apply predicate to array, count matches */
int count_matching(int *arr, int n, Predicate pred) {
    int c = 0, i;
    for (i = 0; i < n; i++) if (pred(arr[i])) c++;
    return c;
}

/* Predicate functions */
int is_positive(int x) { return x > 0; }
int is_even(int x) { return x % 2 == 0; }
int is_large(int x) { return x > 100; }

/* Transform: double all elements */
void double_all(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) arr[i] *= 2;
}

/* Transform: negate all elements */
void negate_all(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) arr[i] = -arr[i];
}

/* IntVec push */
void ivec_push(IntVec *v, int val) {
    if (v->size < v->capacity) {
        v->data[v->size++] = val;
    }
}

int ivec_sum(IntVec *v) {
    int s = 0, i;
    for (i = 0; i < v->size; i++) s += v->data[i];
    return s;
}

/* RGBA pack/unpack */
int rgba_to_int(RGBA c) {
    return ((int)c.r << 24) | ((int)c.g << 16) | ((int)c.b << 8) | (int)c.a;
}

int main(void) {
    /* --- Enum dispatch --- */
    printf("%d\n", dispatch(OP_ADD, 10, 5));   // EXPECT: 15
    printf("%d\n", dispatch(OP_SUB, 10, 5));   // EXPECT: 5
    printf("%d\n", dispatch(OP_MUL, 10, 5));   // EXPECT: 50
    printf("%d\n", dispatch(OP_DIV, 10, 5));   // EXPECT: 2
    printf("%d\n", dispatch(OP_MOD, 10, 3));   // EXPECT: 1

    /* --- Status enum --- */
    Status st = STATUS_OK;
    printf("%d\n", st);                        // EXPECT: 0
    st = STATUS_ERROR;
    printf("%d\n", st);                        // EXPECT: 1
    st = STATUS_DONE;
    printf("%d\n", st);                        // EXPECT: 2

    /* --- Struct with bitfields --- */
    RGBA col = {.r = 255, .g = 128, .b = 64, .a = 32};
    printf("%d\n", (int)col.r);                // EXPECT: 255
    printf("%d\n", (int)col.g);                // EXPECT: 128
    printf("%d\n", (int)col.b);                // EXPECT: 64
    printf("%d\n", (int)col.a);                // EXPECT: 32

    col.g = 0;
    col.b = 200;
    printf("%d\n", (int)col.g);                // EXPECT: 0
    printf("%d\n", (int)col.b);                // EXPECT: 200

    /* --- Compound literal with designated init passed to function --- */
    int r1 = dispatch(OP_ADD, (struct { int v; }){7}.v + 3, 10);
    /* Simpler: just use compound literal of Operation inline */
    /* Actually use direct struct compound literal */
    int r2 = op_add((int)(((RGBA){.r=10}).r), 5);
    printf("%d\n", r2);                        // EXPECT: 15

    /* --- Array of function pointers with enum index --- */
    int (*fptrs[5])(int, int) = {op_add, op_sub, op_mul, op_div, op_mod};
    printf("%d\n", fptrs[OP_ADD](3, 4));      // EXPECT: 7
    printf("%d\n", fptrs[OP_MUL](3, 4));      // EXPECT: 12
    printf("%d\n", fptrs[OP_MOD](10, 3));     // EXPECT: 1

    /* --- Callback pattern: count matching --- */
    int arr[8] = {-3, 5, -1, 8, 0, 12, -7, 3};
    printf("%d\n", count_matching(arr, 8, is_positive)); // EXPECT: 4
    printf("%d\n", count_matching(arr, 8, is_even));     // EXPECT: 3
    printf("%d\n", count_matching(arr, 8, is_large));    // EXPECT: 0

    /* --- Transform callback --- */
    int arr2[5] = {1, 2, 3, 4, 5};
    double_all(arr2, 5);
    /* 2,4,6,8,10 */
    printf("%d\n", arr2[0]);                   // EXPECT: 2
    printf("%d\n", arr2[4]);                   // EXPECT: 10

    negate_all(arr2, 5);
    /* -2,-4,-6,-8,-10 */
    printf("%d\n", arr2[0]);                   // EXPECT: -2
    printf("%d\n", arr2[4]);                   // EXPECT: -10

    /* Transform stored in variable */
    Transform t = double_all;
    int arr3[3] = {5, 10, 15};
    t(arr3, 3);
    printf("%d\n", arr3[0]);                   // EXPECT: 10
    printf("%d\n", arr3[2]);                   // EXPECT: 30

    /* --- IntVec with compound literal push --- */
    IntVec v;
    int i;
    for (i = 0; i < 8; i++) v.data[i] = 0;
    v.size = 0;
    v.capacity = 8;
    ivec_push(&v, 10);
    ivec_push(&v, 20);
    ivec_push(&v, 30);
    printf("%d\n", v.size);                    // EXPECT: 3
    printf("%d\n", ivec_sum(&v));              // EXPECT: 60

    /* --- Ray with designated init --- */
    Ray ray = {
        .origin    = {.x = 1, .y = 2, .z = 3},
        .direction = {.x = 0, .y = 1, .z = 0},
        .length    = 100
    };
    printf("%d\n", ray.origin.x);             // EXPECT: 1
    printf("%d\n", ray.origin.z);             // EXPECT: 3
    printf("%d\n", ray.direction.y);          // EXPECT: 1
    printf("%d\n", ray.length);               // EXPECT: 100

    /* Step ray: origin += direction */
    ray.origin.x += ray.direction.x;
    ray.origin.y += ray.direction.y;
    ray.origin.z += ray.direction.z;
    printf("%d\n", ray.origin.y);             // EXPECT: 3

    /* --- Function pointer in struct used with compound literal --- */
    Operation myop = {.compute = op_mul, .name = "mul", .priority = 2};
    printf("%d\n", myop.compute(6, 7));       // EXPECT: 42
    printf("%d\n", myop.priority);            // EXPECT: 2

    /* --- Array of Operations, iterate and compute --- */
    Operation pipeline[3] = {
        {op_add, "add", 1},
        {op_mul, "mul", 2},
        {op_sub, "sub", 1}
    };
    int val = 2;
    val = pipeline[0].compute(val, 3);  /* 2+3=5 */
    val = pipeline[1].compute(val, 4);  /* 5*4=20 */
    val = pipeline[2].compute(val, 8);  /* 20-8=12 */
    printf("%d\n", val);                       // EXPECT: 12

    return 0;
}
