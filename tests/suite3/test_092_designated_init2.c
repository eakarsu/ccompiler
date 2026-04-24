int printf(const char *fmt, ...);

/* Designated initializers: arrays [idx]=val, structs .field=val, mixed, nested */

struct Point {
    int x;
    int y;
};

struct Color {
    int r;
    int g;
    int b;
};

struct Pixel {
    struct Point pos;
    struct Color col;
};

struct Flags {
    int a;
    int b;
    int c;
    int d;
};

int main(void) {
    /* --- Array designated initializer: [idx]=val --- */
    int arr1[6] = {[3] = 99, [5] = 77};
    /* arr1[0]=0, arr1[1]=0, arr1[2]=0, arr1[3]=99, arr1[4]=0, arr1[5]=77 */
    printf("%d\n", arr1[0]);      // EXPECT: 0
    printf("%d\n", arr1[1]);      // EXPECT: 0
    printf("%d\n", arr1[2]);      // EXPECT: 0
    printf("%d\n", arr1[3]);      // EXPECT: 99
    printf("%d\n", arr1[4]);      // EXPECT: 0
    printf("%d\n", arr1[5]);      // EXPECT: 77

    /* --- Array: [0]=val overrides first, positional fills rest --- */
    int arr2[5] = {[0] = 10, 20, 30};
    /* arr2[0]=10, arr2[1]=20, arr2[2]=30, arr2[3]=0, arr2[4]=0 */
    printf("%d\n", arr2[0]);      // EXPECT: 10
    printf("%d\n", arr2[1]);      // EXPECT: 20
    printf("%d\n", arr2[2]);      // EXPECT: 30
    printf("%d\n", arr2[3]);      // EXPECT: 0
    printf("%d\n", arr2[4]);      // EXPECT: 0

    /* --- Array: designator in middle, positional after --- */
    int arr3[6] = {1, 2, [4] = 40, 50};
    /* arr3[0]=1, arr3[1]=2, arr3[2]=0, arr3[3]=0, arr3[4]=40, arr3[5]=50 */
    printf("%d\n", arr3[0]);      // EXPECT: 1
    printf("%d\n", arr3[1]);      // EXPECT: 2
    printf("%d\n", arr3[2]);      // EXPECT: 0
    printf("%d\n", arr3[3]);      // EXPECT: 0
    printf("%d\n", arr3[4]);      // EXPECT: 40
    printf("%d\n", arr3[5]);      // EXPECT: 50

    /* --- Struct designated initializer: .field=val --- */
    struct Point p1 = {.x = 5, .y = 10};
    printf("%d\n", p1.x);         // EXPECT: 5
    printf("%d\n", p1.y);         // EXPECT: 10

    /* --- Struct: only some fields designated (rest zero) --- */
    struct Flags f1 = {.b = 22, .d = 44};
    /* f1.a=0, f1.b=22, f1.c=0, f1.d=44 */
    printf("%d\n", f1.a);         // EXPECT: 0
    printf("%d\n", f1.b);         // EXPECT: 22
    printf("%d\n", f1.c);         // EXPECT: 0
    printf("%d\n", f1.d);         // EXPECT: 44

    /* --- Struct: designated out of declaration order --- */
    struct Color col1 = {.b = 255, .r = 128, .g = 64};
    printf("%d\n", col1.r);       // EXPECT: 128
    printf("%d\n", col1.g);       // EXPECT: 64
    printf("%d\n", col1.b);       // EXPECT: 255

    /* --- Mixed designated + positional in struct --- */
    /* .a=7 sets a, then next positional fills b */
    struct Flags f2 = {.a = 7, 8, .d = 9};
    /* a=7, b=8, c=0, d=9 */
    printf("%d\n", f2.a);         // EXPECT: 7
    printf("%d\n", f2.b);         // EXPECT: 8
    printf("%d\n", f2.c);         // EXPECT: 0
    printf("%d\n", f2.d);         // EXPECT: 9

    /* --- Nested struct designated init --- */
    struct Pixel px1 = {.pos = {.x = 100, .y = 200}, .col = {.r = 255, .g = 0, .b = 128}};
    printf("%d\n", px1.pos.x);    // EXPECT: 100
    printf("%d\n", px1.pos.y);    // EXPECT: 200
    printf("%d\n", px1.col.r);    // EXPECT: 255
    printf("%d\n", px1.col.g);    // EXPECT: 0
    printf("%d\n", px1.col.b);    // EXPECT: 128

    /* --- Array of structs with designated init --- */
    struct Point pts[4] = {[1] = {.x = 3, .y = 4}, [3] = {.x = 7, .y = 8}};
    /* pts[0]={0,0}, pts[1]={3,4}, pts[2]={0,0}, pts[3]={7,8} */
    printf("%d\n", pts[0].x);     // EXPECT: 0
    printf("%d\n", pts[0].y);     // EXPECT: 0
    printf("%d\n", pts[1].x);     // EXPECT: 3
    printf("%d\n", pts[1].y);     // EXPECT: 4
    printf("%d\n", pts[2].x);     // EXPECT: 0
    printf("%d\n", pts[2].y);     // EXPECT: 0
    printf("%d\n", pts[3].x);     // EXPECT: 7
    printf("%d\n", pts[3].y);     // EXPECT: 8

    /* --- Designated initializer: reuse index to override --- */
    int arr4[4] = {[2] = 10, [2] = 20};
    /* Second [2]=20 overrides first; arr4[2]=20 */
    printf("%d\n", arr4[0]);      // EXPECT: 0
    printf("%d\n", arr4[1]);      // EXPECT: 0
    printf("%d\n", arr4[2]);      // EXPECT: 20
    printf("%d\n", arr4[3]);      // EXPECT: 0

    /* --- Sum array with designated init --- */
    int arr5[5] = {[0] = 1, [2] = 3, [4] = 5};
    int sum = 0;
    int i;
    for (i = 0; i < 5; i++) sum += arr5[i];
    printf("%d\n", sum);          // EXPECT: 9

    /* --- Struct with all fields designated explicitly --- */
    struct Flags f3 = {.a = 10, .b = 20, .c = 30, .d = 40};
    printf("%d\n", f3.a + f3.b + f3.c + f3.d); // EXPECT: 100

    /* --- Nested array-of-structs positional + designated --- */
    struct Color palette[3] = {
        {.r = 255, .g = 0,   .b = 0},
        {.r = 0,   .g = 255, .b = 0},
        {.r = 0,   .g = 0,   .b = 255}
    };
    printf("%d\n", palette[0].r); // EXPECT: 255
    printf("%d\n", palette[1].g); // EXPECT: 255
    printf("%d\n", palette[2].b); // EXPECT: 255
    /* Sum of all channels: 255+255+255 = 765 */
    int csum = palette[0].r + palette[0].g + palette[0].b
             + palette[1].r + palette[1].g + palette[1].b
             + palette[2].r + palette[2].g + palette[2].b;
    printf("%d\n", csum);         // EXPECT: 765

    return 0;
}
