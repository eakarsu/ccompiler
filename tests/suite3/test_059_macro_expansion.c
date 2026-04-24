int printf(const char *fmt, ...);

/* Macro expansion order, nested macro calls, macro arguments as expressions */

/* Level 1: simple macros */
#define A   2
#define B   3
#define C   (A + B)         /* expands to (2+3) = 5 */

/* Level 2: macro calling macro */
#define D   (C * A)         /* (5) * 2 = 10 */
#define E   (D + C)         /* 10 + 5 = 15 */

/* Function-like macros with expression args */
#define DOUBLE(x)   ((x) + (x))
#define TRIPLE(x)   ((x) + (x) + (x))
#define ADD(x, y)   ((x) + (y))
#define MUL(x, y)   ((x) * (y))

/* Macro that uses another macro's result as arg */
#define QUAD(x)     DOUBLE(DOUBLE(x))
/* QUAD(3) = DOUBLE(DOUBLE(3)) = DOUBLE(6) = 12 */

/* Macro expansion with nested calls */
#define SQ(x)       MUL((x), (x))
#define SUMSQ(a,b)  ADD(SQ(a), SQ(b))

/* Macro for array size using constant expression */
#define N       5
#define M       (N + N)         /* 10 */
#define NM      (N * M)         /* 50 */

/* Macro in struct definition */
#define COORD_COUNT 3

/* Macro argument that is a macro */
#define IDENTITY(x) (x)
#define WRAP(x)     IDENTITY(x)

/* Expansion inside expression */
#define BASE    10
#define OFFSET  7
#define ADDR(i) (BASE + (i) * OFFSET)
/* ADDR(0)=10, ADDR(1)=17, ADDR(2)=24, ADDR(3)=31 */

/* Macro with side-effect argument (used once so safe) */
#define SAFE_SQ(x)  ((x) * (x))

/* Nested conditional macro */
#define MAX(a, b)   ((a) > (b) ? (a) : (b))
#define MIN(a, b)   ((a) < (b) ? (a) : (b))
#define MED3(a,b,c) MAX(MIN(a,b), MIN(MAX(a,b), c))
/* Median of three */

/* Struct using COORD_COUNT */
struct Coords {
    int v[COORD_COUNT];
};

int main(void) {
    /* Object macro expansion chain */
    printf("%d\n", A);                   // EXPECT: 2
    printf("%d\n", B);                   // EXPECT: 3
    printf("%d\n", C);                   // EXPECT: 5
    printf("%d\n", D);                   // EXPECT: 10
    printf("%d\n", E);                   // EXPECT: 15

    /* Function macro with simple args */
    printf("%d\n", DOUBLE(4));           // EXPECT: 8
    printf("%d\n", TRIPLE(4));           // EXPECT: 12
    printf("%d\n", ADD(3, 7));           // EXPECT: 10
    printf("%d\n", MUL(3, 7));           // EXPECT: 21

    /* Function macro with expression args */
    printf("%d\n", DOUBLE(A + B));       // EXPECT: 10
    /* DOUBLE(5) = 5+5 = 10 */
    printf("%d\n", MUL(A, B + 1));       // EXPECT: 8
    /* MUL(2, 4) = 8 */
    printf("%d\n", ADD(C, D));           // EXPECT: 15
    /* ADD(5, 10) = 15 */

    /* Nested macro call: QUAD */
    printf("%d\n", QUAD(3));             // EXPECT: 12
    /* DOUBLE(DOUBLE(3))=DOUBLE(6)=12 */
    printf("%d\n", QUAD(5));             // EXPECT: 20
    printf("%d\n", QUAD(1));             // EXPECT: 4

    /* SUMSQ: macro calling macros */
    printf("%d\n", SUMSQ(3, 4));         // EXPECT: 25
    /* 9+16 = 25 */
    printf("%d\n", SUMSQ(5, 12));        // EXPECT: 169
    /* 25+144 = 169 */
    printf("%d\n", SUMSQ(0, 7));         // EXPECT: 49

    /* Array sized by macro constant expression */
    int arr[N];
    int i;
    for (i = 0; i < N; i++) arr[i] = i + 1;
    printf("%d\n", arr[0]);              // EXPECT: 1
    printf("%d\n", arr[4]);              // EXPECT: 5

    int arr2[M];
    for (i = 0; i < M; i++) arr2[i] = i * 2;
    printf("%d\n", arr2[0]);             // EXPECT: 0
    printf("%d\n", arr2[9]);             // EXPECT: 18

    printf("%d\n", NM);                  // EXPECT: 50
    /* N*M = 5*10 = 50 */

    /* WRAP and IDENTITY */
    printf("%d\n", WRAP(42));            // EXPECT: 42
    printf("%d\n", WRAP(C + D));         // EXPECT: 15
    /* IDENTITY(5+10) = 15 */

    /* ADDR macro: base + offset*i */
    printf("%d\n", ADDR(0));             // EXPECT: 10
    printf("%d\n", ADDR(1));             // EXPECT: 17
    printf("%d\n", ADDR(2));             // EXPECT: 24
    printf("%d\n", ADDR(3));             // EXPECT: 31

    /* Struct using COORD_COUNT macro */
    struct Coords c;
    c.v[0] = 10;
    c.v[1] = 20;
    c.v[2] = 30;
    printf("%d\n", c.v[0]);              // EXPECT: 10
    printf("%d\n", c.v[1]);              // EXPECT: 20
    printf("%d\n", c.v[2]);              // EXPECT: 30

    /* MED3: median of three */
    printf("%d\n", MED3(1, 2, 3));       // EXPECT: 2
    printf("%d\n", MED3(3, 1, 2));       // EXPECT: 2
    printf("%d\n", MED3(5, 5, 5));       // EXPECT: 5
    printf("%d\n", MED3(10, 3, 7));      // EXPECT: 7

    /* Macro argument as complex expression */
    int x = 4, y = 6;
    printf("%d\n", SQ(x + y));           // EXPECT: 100
    /* x+y=10, SQ(10)=100 */
    printf("%d\n", DOUBLE(x * y));       // EXPECT: 48
    /* x*y=24, DOUBLE(24)=48 */
    printf("%d\n", ADD(x * x, y * y));   // EXPECT: 52
    /* 16 + 36 = 52 */

    return 0;
}
