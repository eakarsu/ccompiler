int printf(const char *fmt, ...);

int main(void) {
    /* Basic pre-increment */
    int a = 5;
    printf("%d\n", ++a);            // EXPECT: 6
    printf("%d\n", a);              // EXPECT: 6

    /* Basic post-increment */
    int b = 5;
    printf("%d\n", b++);            // EXPECT: 5
    printf("%d\n", b);              // EXPECT: 6

    /* Basic pre-decrement */
    int c = 10;
    printf("%d\n", --c);            // EXPECT: 9
    printf("%d\n", c);              // EXPECT: 9

    /* Basic post-decrement */
    int d = 10;
    printf("%d\n", d--);            // EXPECT: 10
    printf("%d\n", d);              // EXPECT: 9

    /* Increment in expressions: a = b++ + ++c */
    int e = 3, f = 4;
    /* b++ uses b=3 then increments to 4; ++c increments f to 5 first */
    int g = e++ + ++f;
    /* e++ yields 3, ++f yields 5, g = 3+5 = 8 */
    printf("%d\n", g);              // EXPECT: 8
    printf("%d\n", e);              // EXPECT: 4
    printf("%d\n", f);              // EXPECT: 5

    /* Post-increment in larger expression */
    int h = 10;
    int k = h++ * 2;
    /* h++ yields 10, k = 10*2 = 20, then h becomes 11 */
    printf("%d\n", k);              // EXPECT: 20
    printf("%d\n", h);              // EXPECT: 11

    /* Pre-increment in larger expression */
    int m = 10;
    int n = ++m * 2;
    /* ++m yields 11, n = 11*2 = 22 */
    printf("%d\n", n);              // EXPECT: 22
    printf("%d\n", m);              // EXPECT: 11

    /* Chained increments on different vars */
    int x = 0;
    x++;
    x++;
    x++;
    printf("%d\n", x);              // EXPECT: 3

    /* Decrement to zero and below */
    int cnt = 3;
    printf("%d\n", cnt--);          // EXPECT: 3
    printf("%d\n", cnt--);          // EXPECT: 2
    printf("%d\n", cnt--);          // EXPECT: 1
    printf("%d\n", cnt);            // EXPECT: 0

    /* Pre-decrement in loop-like pattern */
    int lv = 5;
    printf("%d\n", --lv);           // EXPECT: 4
    printf("%d\n", --lv);           // EXPECT: 3
    printf("%d\n", --lv);           // EXPECT: 2

    /* Char increment */
    char ch = 'A';
    ch++;
    printf("%c\n", ch);             // EXPECT: B
    ch++;
    printf("%c\n", ch);             // EXPECT: C
    ch--;
    printf("%c\n", ch);             // EXPECT: B

    /* Increment negative numbers */
    int neg = -3;
    printf("%d\n", neg++);          // EXPECT: -3
    printf("%d\n", neg);            // EXPECT: -2
    printf("%d\n", ++neg);          // EXPECT: -1
    printf("%d\n", ++neg);          // EXPECT: 0
    printf("%d\n", ++neg);          // EXPECT: 1

    /* Pointer increment */
    int arr[5];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;
    int *p = arr;
    printf("%d\n", *p);             // EXPECT: 10
    p++;
    printf("%d\n", *p);             // EXPECT: 20
    p++;
    printf("%d\n", *p);             // EXPECT: 30
    p--;
    printf("%d\n", *p);             // EXPECT: 20

    /* Post-increment pointer: use value then advance */
    int *q = arr;
    printf("%d\n", *q++);           // EXPECT: 10
    printf("%d\n", *q++);           // EXPECT: 20
    printf("%d\n", *q);             // EXPECT: 30

    /* Pre-increment pointer: advance then use value */
    int *r = arr;
    printf("%d\n", *++r);           // EXPECT: 20
    printf("%d\n", *++r);           // EXPECT: 30

    /* Array index with post-increment */
    int idx = 0;
    printf("%d\n", arr[idx++]);     // EXPECT: 10
    /* arr[0] = 10, idx becomes 1 */
    printf("%d\n", arr[idx++]);     // EXPECT: 20
    printf("%d\n", arr[idx++]);     // EXPECT: 30
    printf("%d\n", idx);            // EXPECT: 3

    /* Array index with pre-increment */
    int idx2 = 0;
    printf("%d\n", arr[++idx2]);    // EXPECT: 20
    /* idx2 becomes 1 first, arr[1] = 20 */
    printf("%d\n", arr[++idx2]);    // EXPECT: 30
    printf("%d\n", idx2);           // EXPECT: 2

    /* Multiple increments on same var in sequence */
    int v = 0;
    v++;
    ++v;
    v++;
    ++v;
    printf("%d\n", v);              // EXPECT: 4

    return 0;
}
