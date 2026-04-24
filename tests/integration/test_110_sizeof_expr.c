int printf(const char *fmt, ...);
// EXPECT: sizeof(int) = 4\nsizeof(char) = 1\nsizeof(a) = 4\nsizeof(c) = 1\nsizeof(arr) = 40\nsizeof(str) = 20\nsizeof(p) = 8\narr count = 10\nstr count = 20\nsizeof(int)*10 = 40\nsizeof(matrix) = 48\nmatrix rows = 3\nmatrix cols = 4\nsizeof(struct point) = 8\nbefore sizeof: a=5\nsizeof(a+1) = 4\nafter sizeof: a=5\nsizeof('A') = 4\ndata[19] = 361
// Test: sizeof expressions

int main(void) {
    int a;
    char c;
    int arr[10];
    char str[20];
    int *p;
    int matrix[3][4];

    /* Basic sizeof types */
    printf("sizeof(int) = %d\n", (int)sizeof(int));
    printf("sizeof(char) = %d\n", (int)sizeof(char));

    /* sizeof variables */
    printf("sizeof(a) = %d\n", (int)sizeof(a));
    printf("sizeof(c) = %d\n", (int)sizeof(c));

    /* sizeof arrays */
    printf("sizeof(arr) = %d\n", (int)sizeof(arr));
    printf("sizeof(str) = %d\n", (int)sizeof(str));

    /* sizeof pointer */
    printf("sizeof(p) = %d\n", (int)sizeof(p));

    /* Array element count using sizeof */
    printf("arr count = %d\n", (int)(sizeof(arr) / sizeof(arr[0])));
    printf("str count = %d\n", (int)(sizeof(str) / sizeof(str[0])));

    /* sizeof in expressions */
    a = (int)sizeof(int) * 10;
    printf("sizeof(int)*10 = %d\n", a);

    /* sizeof 2D array */
    printf("sizeof(matrix) = %d\n", (int)sizeof(matrix));
    printf("matrix rows = %d\n", (int)(sizeof(matrix) / sizeof(matrix[0])));
    printf("matrix cols = %d\n", (int)(sizeof(matrix[0]) / sizeof(matrix[0][0])));

    /* sizeof struct */
    {
        struct point {
            int x;
            int y;
        };
        struct point pt;
        printf("sizeof(struct point) = %d\n", (int)sizeof(pt));
    }

    /* sizeof expression (not evaluated) */
    a = 5;
    printf("before sizeof: a=%d\n", a);
    printf("sizeof(a+1) = %d\n", (int)sizeof(a + 1));
    printf("after sizeof: a=%d\n", a);

    /* sizeof char literal */
    printf("sizeof('A') = %d\n", (int)sizeof('A'));

    /* sizeof used as array size computation */
    {
        int data[20];
        int count;
        int i;
        count = (int)(sizeof(data) / sizeof(data[0]));
        for (i = 0; i < count; i++) {
            data[i] = i * i;
        }
        printf("data[19] = %d\n", data[19]);
    }

    return 0;
}
