int printf(const char *fmt, ...);

struct Point {
    int x;
    int y;
};

struct Mixed {
    char a;
    int b;
    char c;
};

struct Packed8 {
    int x;
    int y;
};

struct WithChar {
    char a;
    char b;
    char c;
    char d;
};

struct Nested {
    struct Point p;
    int z;
};

int main(void) {
    /* sizeof fundamental types */
    printf("%d\n", (int)sizeof(char));          // EXPECT: 1
    printf("%d\n", (int)sizeof(short));         // EXPECT: 2
    printf("%d\n", (int)sizeof(int));           // EXPECT: 4
    printf("%d\n", (int)sizeof(long));          // EXPECT: 8
    /* On 64-bit LP64 (ARM64 macOS): long = 8 bytes */
    printf("%d\n", (int)sizeof(long long));     // EXPECT: 8
    printf("%d\n", (int)sizeof(unsigned char)); // EXPECT: 1
    printf("%d\n", (int)sizeof(unsigned int));  // EXPECT: 4
    printf("%d\n", (int)sizeof(unsigned long)); // EXPECT: 8

    /* sizeof pointer types - all 8 bytes on 64-bit */
    printf("%d\n", (int)sizeof(char *));        // EXPECT: 8
    printf("%d\n", (int)sizeof(int *));         // EXPECT: 8
    printf("%d\n", (int)sizeof(void *));        // EXPECT: 8
    printf("%d\n", (int)sizeof(long *));        // EXPECT: 8

    /* sizeof array: total size = element_size * count */
    int arr_int[10];
    printf("%d\n", (int)sizeof(arr_int));       // EXPECT: 40
    /* 10 * sizeof(int) = 10 * 4 = 40 */

    char arr_char[20];
    printf("%d\n", (int)sizeof(arr_char));      // EXPECT: 20
    /* 20 * sizeof(char) = 20 * 1 = 20 */

    long arr_long[5];
    printf("%d\n", (int)sizeof(arr_long));      // EXPECT: 40
    /* 5 * sizeof(long) = 5 * 8 = 40 */

    int arr2d[3][4];
    printf("%d\n", (int)sizeof(arr2d));         // EXPECT: 48
    /* 3 * 4 * sizeof(int) = 12 * 4 = 48 */

    /* sizeof expression (not evaluated) */
    int e_val = 5;
    printf("%d\n", (int)sizeof(e_val + 1));     // EXPECT: 4
    /* sizeof(int expression) = 4 */
    printf("%d\n", (int)sizeof(e_val * 1000000)); // EXPECT: 4

    char e_char = 'A';
    printf("%d\n", (int)sizeof(e_char));        // EXPECT: 1

    /* sizeof struct Point {int x; int y;} = 8 */
    printf("%d\n", (int)sizeof(struct Point));  // EXPECT: 8

    /* sizeof struct with padding */
    /* struct Mixed { char a; int b; char c; }
       char a: 1 byte + 3 padding
       int b:  4 bytes
       char c: 1 byte + 3 padding (for alignment)
       total = 12 */
    printf("%d\n", (int)sizeof(struct Mixed));  // EXPECT: 12

    /* struct Packed8 { int x; int y; } = 8 bytes, no padding */
    printf("%d\n", (int)sizeof(struct Packed8));// EXPECT: 8

    /* struct WithChar { char a; char b; char c; char d; } = 4 bytes */
    printf("%d\n", (int)sizeof(struct WithChar));// EXPECT: 4

    /* struct Nested { struct Point p; int z; }
       p.x: 4, p.y: 4, z: 4, pad: 4 to align? No.
       struct Point = 8, int z = 4, total = 12 bytes (no trailing pad needed
       because max alignment is 4 from int) */
    printf("%d\n", (int)sizeof(struct Nested)); // EXPECT: 12

    /* sizeof variable of struct type */
    struct Point pt;
    printf("%d\n", (int)sizeof(pt));            // EXPECT: 8

    /* Number of elements in array: sizeof(arr)/sizeof(arr[0]) */
    int data[7];
    int nelem = (int)(sizeof(data) / sizeof(data[0]));
    printf("%d\n", nelem);                      // EXPECT: 7
    /* 28 / 4 = 7 */

    char str_arr[13];
    int nchar = (int)(sizeof(str_arr) / sizeof(str_arr[0]));
    printf("%d\n", nchar);                      // EXPECT: 13
    /* 13 / 1 = 13 */

    /* sizeof pointer vs array: pointer is 8, array is full size */
    int arr3[5];
    int *ptr3 = arr3;
    printf("%d\n", (int)sizeof(arr3));          // EXPECT: 20
    printf("%d\n", (int)sizeof(ptr3));          // EXPECT: 8
    /* arr3 = 5*4=20, ptr3 = pointer = 8 */

    /* sizeof string literal */
    printf("%d\n", (int)sizeof("hello"));       // EXPECT: 6
    /* "hello" = 5 chars + null terminator = 6 */
    printf("%d\n", (int)sizeof(""));            // EXPECT: 1
    /* "" = just null terminator = 1 */

    /* sizeof in expression */
    int buf[sizeof(int) * 4];
    /* sizeof(int)=4, 4*4=16, but this is a VLA if sizeof(int) not constant...
       Actually sizeof(int) IS a compile-time constant, so buf[16] */
    printf("%d\n", (int)sizeof(buf));           // EXPECT: 64
    /* 16 * sizeof(int) = 16 * 4 = 64 */

    /* sizeof short */
    short s_var = 100;
    printf("%d\n", (int)sizeof(s_var));         // EXPECT: 2
    printf("%d\n", (int)sizeof(short));         // EXPECT: 2

    /* Size relationships */
    printf("%d\n", (int)sizeof(char) <= (int)sizeof(short)); // EXPECT: 1
    printf("%d\n", (int)sizeof(short) <= (int)sizeof(int));  // EXPECT: 1
    printf("%d\n", (int)sizeof(int) <= (int)sizeof(long));   // EXPECT: 1

    return 0;
}
