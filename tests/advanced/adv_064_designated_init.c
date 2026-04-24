// EXPECT: pt.x=10 pt.y=20
// EXPECT: pt2.x=0 pt2.y=50
// EXPECT: arr[0]=0 arr[3]=99 arr[5]=55
// EXPECT: arr2[0]=10 arr2[1]=0 arr2[4]=40
// EXPECT: color.r=255 color.g=0 color.b=128
// EXPECT: out_order.z=30 out_order.x=10 out_order.y=20
// EXPECT: partial.a=1 partial.b=0 partial.c=3
// EXPECT: nested.pos.x=5 nested.pos.y=6 nested.val=99
// EXPECT: mix_arr[0]=1 mix_arr[1]=2 mix_arr[5]=50
// EXPECT: rect.tl.x=0 rect.tl.y=0 rect.br.x=100 rect.br.y=200
// EXPECT: config.flags=7 config.name_0=H
// EXPECT: sparr[0]=0 sparr[7]=77 sparr[9]=99
// EXPECT: multi.m[0]=10 multi.m[1]=0 multi.m[2]=30
// EXPECT: overwrite=99
// EXPECT: all_passed=1

int printf(const char *fmt, ...);

struct Point {
    int x;
    int y;
};

struct Point3D {
    int x;
    int y;
    int z;
};

struct Color {
    int r;
    int g;
    int b;
    int a;
};

struct Partial {
    int a;
    int b;
    int c;
    int d;
};

struct Nested {
    struct Point pos;
    int val;
};

struct Rect {
    struct Point tl;
    struct Point br;
};

struct Config {
    int flags;
    char name[8];
    int id;
};

struct Multi {
    int m[4];
    int tag;
};

int main(void) {
    int passed = 1;

    // Basic struct designated init
    struct Point pt = {.x = 10, .y = 20};
    printf("pt.x=%d pt.y=%d\n", pt.x, pt.y);
    if (pt.x != 10 || pt.y != 20) passed = 0;

    // Partial struct init (x defaults to 0)
    struct Point pt2 = {.y = 50};
    printf("pt2.x=%d pt2.y=%d\n", pt2.x, pt2.y);
    if (pt2.x != 0 || pt2.y != 50) passed = 0;

    // Array designated init
    int arr[6] = {[3] = 99, [5] = 55};
    printf("arr[0]=%d arr[3]=%d arr[5]=%d\n", arr[0], arr[3], arr[5]);
    if (arr[0] != 0 || arr[3] != 99 || arr[5] != 55) passed = 0;

    // Array with mix of designated and sequential
    int arr2[5] = {[0] = 10, [4] = 40};
    printf("arr2[0]=%d arr2[1]=%d arr2[4]=%d\n", arr2[0], arr2[1], arr2[4]);
    if (arr2[0] != 10 || arr2[1] != 0 || arr2[4] != 40) passed = 0;

    // Color struct (out of natural order)
    struct Color color = {.r = 255, .b = 128, .g = 0};
    printf("color.r=%d color.g=%d color.b=%d\n", color.r, color.g, color.b);
    if (color.r != 255 || color.g != 0 || color.b != 128) passed = 0;

    // Out of order Point3D
    struct Point3D out_order = {.z = 30, .x = 10, .y = 20};
    printf("out_order.z=%d out_order.x=%d out_order.y=%d\n",
           out_order.z, out_order.x, out_order.y);
    if (out_order.x != 10 || out_order.y != 20 || out_order.z != 30) passed = 0;

    // Partial init with gaps
    struct Partial partial = {.a = 1, .c = 3};
    printf("partial.a=%d partial.b=%d partial.c=%d\n", partial.a, partial.b, partial.c);
    if (partial.a != 1 || partial.b != 0 || partial.c != 3) passed = 0;

    // Nested struct designated init
    struct Nested nested = {.pos = {.x = 5, .y = 6}, .val = 99};
    printf("nested.pos.x=%d nested.pos.y=%d nested.val=%d\n",
           nested.pos.x, nested.pos.y, nested.val);
    if (nested.pos.x != 5 || nested.pos.y != 6 || nested.val != 99) passed = 0;

    // Mixed array: some designated, some not
    int mix_arr[6] = {1, 2, [5] = 50};
    printf("mix_arr[0]=%d mix_arr[1]=%d mix_arr[5]=%d\n",
           mix_arr[0], mix_arr[1], mix_arr[5]);
    if (mix_arr[0] != 1 || mix_arr[1] != 2 || mix_arr[5] != 50) passed = 0;

    // Nested struct: Rect with designated init
    struct Rect rect = {
        .tl = {.x = 0, .y = 0},
        .br = {.x = 100, .y = 200}
    };
    printf("rect.tl.x=%d rect.tl.y=%d rect.br.x=%d rect.br.y=%d\n",
           rect.tl.x, rect.tl.y, rect.br.x, rect.br.y);
    if (rect.tl.x != 0 || rect.br.x != 100 || rect.br.y != 200) passed = 0;

    // Config with char array partial init
    struct Config config = {.flags = 7, .name = {'H', 'i'}};
    printf("config.flags=%d config.name_0=%c\n", config.flags, config.name[0]);
    if (config.flags != 7 || config.name[0] != 'H') passed = 0;

    // Sparse array
    int sparr[10] = {[7] = 77, [9] = 99};
    printf("sparr[0]=%d sparr[7]=%d sparr[9]=%d\n", sparr[0], sparr[7], sparr[9]);
    if (sparr[0] != 0 || sparr[7] != 77 || sparr[9] != 99) passed = 0;

    // Struct with array member designated init
    struct Multi multi = {.m = {[0] = 10, [2] = 30}, .tag = 1};
    printf("multi.m[0]=%d multi.m[1]=%d multi.m[2]=%d\n",
           multi.m[0], multi.m[1], multi.m[2]);
    if (multi.m[0] != 10 || multi.m[1] != 0 || multi.m[2] != 30) passed = 0;

    // Overwrite: later designator wins
    int ow[3] = {[0] = 10, [0] = 99};
    printf("overwrite=%d\n", ow[0]);
    if (ow[0] != 99) passed = 0;

    printf("all_passed=%d\n", passed);
    return passed ? 0 : 1;
}
