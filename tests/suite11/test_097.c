int printf(const char *fmt, ...);

// Test: Nested sizeof expressions

struct Pair {
    int first;
    int second;
};

struct Triple {
    int a;
    int b;
    int c;
};

struct WithArray {
    int data[8];
    int count;
};

struct WithPointer {
    int *ptr;
    int len;
};

struct Outer {
    struct Pair p;
    int extra;
};

int main() {
    int result;

    // sizeof of sizeof result (sizeof returns size_t, treated as int here)
    result = (int)sizeof((int)sizeof(int));
    printf("%d\n", result); // EXPECT: 4

    // sizeof applied to arithmetic with sizeof
    result = (int)sizeof((int)sizeof(int) + 1);
    printf("%d\n", result); // EXPECT: 4

    // sizeof struct vs sizeof members
    struct Pair p;
    int sz_struct;
    int sz_members;
    sz_struct = (int)sizeof(struct Pair);
    sz_members = (int)sizeof(p.first) + (int)sizeof(p.second);
    printf("%d\n", sz_struct);  // EXPECT: 8
    printf("%d\n", sz_members); // EXPECT: 8

    // sizeof struct with array
    printf("%d\n", (int)sizeof(struct WithArray)); // EXPECT: 36

    // sizeof struct with pointer
    printf("%d\n", (int)sizeof(struct WithPointer)); // EXPECT: 16

    // sizeof nested struct
    printf("%d\n", (int)sizeof(struct Outer)); // EXPECT: 12

    // sizeof in sizeof (nested)
    int x;
    x = (int)sizeof(int);
    result = (int)sizeof(x);
    printf("%d\n", result); // EXPECT: 4

    // sizeof array of structs
    struct Pair pairs[5];
    printf("%d\n", (int)sizeof(pairs)); // EXPECT: 40

    // Count elements using sizeof
    int count;
    count = (int)sizeof(pairs) / (int)sizeof(pairs[0]);
    printf("%d\n", count); // EXPECT: 5

    // sizeof pointer to struct vs struct
    struct Pair *pp;
    printf("%d\n", (int)sizeof(pp));  // EXPECT: 8
    printf("%d\n", (int)sizeof(*pp)); // EXPECT: 8

    // sizeof Triple
    printf("%d\n", (int)sizeof(struct Triple)); // EXPECT: 12

    // sizeof comparison in conditions
    if ((int)sizeof(struct Pair) < (int)sizeof(struct Triple)) {
        printf("%d\n", 1); // EXPECT: 1
    }

    if ((int)sizeof(struct Triple) < (int)sizeof(struct WithArray)) {
        printf("%d\n", 1); // EXPECT: 1
    }

    // sizeof used in loop calculations
    struct Triple triples[4];
    int total_bytes;
    total_bytes = (int)sizeof(triples);
    printf("%d\n", total_bytes); // EXPECT: 48
    int num_triples;
    num_triples = total_bytes / (int)sizeof(struct Triple);
    printf("%d\n", num_triples); // EXPECT: 4

    // sizeof expression does not evaluate side effects
    int a;
    a = 10;
    result = (int)sizeof(a + 1);
    printf("%d\n", a);      // EXPECT: 10
    printf("%d\n", result); // EXPECT: 4

    // sizeof char array in struct
    struct CharBuf {
        char buf[32];
        int len;
    };
    printf("%d\n", (int)sizeof(struct CharBuf)); // EXPECT: 36

    // sizeof array element
    int big[100];
    printf("%d\n", (int)sizeof(big));    // EXPECT: 400
    printf("%d\n", (int)sizeof(big[0])); // EXPECT: 4
    count = (int)sizeof(big) / (int)sizeof(big[0]);
    printf("%d\n", count); // EXPECT: 100

    // sizeof multi-dimensional array
    int grid[3][4];
    printf("%d\n", (int)sizeof(grid));       // EXPECT: 48
    printf("%d\n", (int)sizeof(grid[0]));    // EXPECT: 16
    printf("%d\n", (int)sizeof(grid[0][0])); // EXPECT: 4

    // Rows and columns from sizeof
    int rows;
    int cols;
    rows = (int)sizeof(grid) / (int)sizeof(grid[0]);
    cols = (int)sizeof(grid[0]) / (int)sizeof(grid[0][0]);
    printf("%d\n", rows); // EXPECT: 3
    printf("%d\n", cols); // EXPECT: 4

    // sizeof in ternary
    result = ((int)sizeof(int) == 4) ? 111 : 222;
    printf("%d\n", result); // EXPECT: 111

    // sizeof pointer arithmetic result
    int *ip;
    result = (int)sizeof(ip + 1);
    printf("%d\n", result); // EXPECT: 8

    // sizeof char
    char c;
    printf("%d\n", (int)sizeof(c)); // EXPECT: 1

    // sizeof in multiplication
    result = 10 * (int)sizeof(int);
    printf("%d\n", result); // EXPECT: 40

    return 0;
}
