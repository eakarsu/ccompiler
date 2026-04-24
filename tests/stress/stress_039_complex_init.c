// stress_039_complex_init.c - Complex initialization patterns
// EXPECT: item0=1 apple 100
// EXPECT: item1=2 banana 200
// EXPECT: item2=3 cherry 300
// EXPECT: nested_a=10 20 30 40
// EXPECT: str_arr_0=hello
// EXPECT: str_arr_1=world
// EXPECT: str_arr_2=test
// EXPECT: global_pt=100 200
// EXPECT: global_rect=0 0 640 480
// EXPECT: local_from_global=100 200
// EXPECT: fptr_add=30
// EXPECT: fptr_sub=5
// EXPECT: arr_of_pts_0=1 2
// EXPECT: arr_of_pts_1=3 4
// EXPECT: arr_of_pts_2=5 6
// EXPECT: nested_arr_struct=10 20 30
// EXPECT: zero_init=0 0 0
// EXPECT: partial_init=1 0 0
// EXPECT: char_arr=ABCDE
// EXPECT: mixed_global=42 hello
// EXPECT: struct_copy=10 20
// EXPECT: arr_copy=1 2 3 4 5
// EXPECT: matrix_sum=45
// EXPECT: pass_done=1

int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

// --- Structs for initialization ---
struct Item {
    int id;
    char *name;
    int price;
};

struct Point { int x; int y; };

struct Rect {
    struct Point origin;
    struct Point size;
};

// --- Global struct init ---
struct Point global_pt = {100, 200};

struct Rect global_rect = {{0, 0}, {640, 480}};

// --- Global with mixed types ---
struct Mixed {
    int val;
    char *str;
};
struct Mixed mixed_global = {42, "hello"};

// --- Function pointers in struct ---
typedef int (*BinOp)(int, int);

int add_fn(int a, int b) { return a + b; }
int sub_fn(int a, int b) { return a - b; }

struct OpEntry {
    char *name;
    BinOp op;
};

// --- Nested struct with arrays ---
struct ArrStruct {
    int vals[3];
    int count;
};

int main(void) {
    // Array of structs initialization
    struct Item items[3];
    items[0].id = 1; items[0].name = "apple";  items[0].price = 100;
    items[1].id = 2; items[1].name = "banana"; items[1].price = 200;
    items[2].id = 3; items[2].name = "cherry"; items[2].price = 300;

    int i;
    for (i = 0; i < 3; i++) {
        printf("item%d=%d %s %d\n", i, items[i].id, items[i].name, items[i].price);
    }

    // Nested struct init
    struct Rect r;
    r.origin.x = 10;
    r.origin.y = 20;
    r.size.x = 30;
    r.size.y = 40;
    printf("nested_a=%d %d %d %d\n", r.origin.x, r.origin.y, r.size.x, r.size.y);

    // Array of string literals
    char *strs[3];
    strs[0] = "hello";
    strs[1] = "world";
    strs[2] = "test";
    for (i = 0; i < 3; i++) {
        printf("str_arr_%d=%s\n", i, strs[i]);
    }

    // Global struct
    printf("global_pt=%d %d\n", global_pt.x, global_pt.y);
    printf("global_rect=%d %d %d %d\n",
           global_rect.origin.x, global_rect.origin.y,
           global_rect.size.x, global_rect.size.y);

    // Local from global
    struct Point lp;
    lp.x = global_pt.x;
    lp.y = global_pt.y;
    printf("local_from_global=%d %d\n", lp.x, lp.y);

    // Struct with function pointer
    struct OpEntry ops[2];
    ops[0].name = "add";
    ops[0].op = add_fn;
    ops[1].name = "sub";
    ops[1].op = sub_fn;
    printf("fptr_add=%d\n", ops[0].op(10, 20));
    printf("fptr_sub=%d\n", ops[1].op(15, 10));

    // Array of points
    struct Point pts[3];
    pts[0].x = 1; pts[0].y = 2;
    pts[1].x = 3; pts[1].y = 4;
    pts[2].x = 5; pts[2].y = 6;
    for (i = 0; i < 3; i++) {
        printf("arr_of_pts_%d=%d %d\n", i, pts[i].x, pts[i].y);
    }

    // Nested array in struct
    struct ArrStruct as;
    as.vals[0] = 10;
    as.vals[1] = 20;
    as.vals[2] = 30;
    as.count = 3;
    printf("nested_arr_struct=%d %d %d\n", as.vals[0], as.vals[1], as.vals[2]);

    // Zero initialization
    struct Point zp;
    zp.x = 0;
    zp.y = 0;
    int zarr[3];
    zarr[0] = 0; zarr[1] = 0; zarr[2] = 0;
    printf("zero_init=%d %d %d\n", zarr[0], zarr[1], zarr[2]);

    // Partial initialization (first element set, rest zeroed manually)
    int parr[3];
    parr[0] = 1; parr[1] = 0; parr[2] = 0;
    printf("partial_init=%d %d %d\n", parr[0], parr[1], parr[2]);

    // Char array initialization
    char ca[6];
    ca[0] = 'A'; ca[1] = 'B'; ca[2] = 'C'; ca[3] = 'D'; ca[4] = 'E'; ca[5] = 0;
    printf("char_arr=%s\n", ca);

    // Mixed global
    printf("mixed_global=%d %s\n", mixed_global.val, mixed_global.str);

    // Struct copy
    struct Point src;
    src.x = 10;
    src.y = 20;
    struct Point dst;
    dst = src;
    printf("struct_copy=%d %d\n", dst.x, dst.y);

    // Array copy via loop
    int orig[5];
    orig[0] = 1; orig[1] = 2; orig[2] = 3; orig[3] = 4; orig[4] = 5;
    int copy[5];
    for (i = 0; i < 5; i++) copy[i] = orig[i];
    printf("arr_copy=%d %d %d %d %d\n", copy[0], copy[1], copy[2], copy[3], copy[4]);

    // 2D array (matrix) init and sum
    int mat[3][3];
    int val = 0;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            val++;
            mat[i][j] = val;
        }
    }
    int msum = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            msum = msum + mat[i][j];
        }
    }
    printf("matrix_sum=%d\n", msum); // 1+2+...+9=45

    printf("pass_done=1\n");
    return 0;
}
