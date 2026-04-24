int printf(const char *fmt, ...);

/* Helper functions to generate values for printf args */
int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }

int main(void) {
    /* Basic %d integer formatting */
    printf("%d\n", 0);              // EXPECT: 0
    printf("%d\n", 1);              // EXPECT: 1
    printf("%d\n", -1);             // EXPECT: -1
    printf("%d\n", 42);             // EXPECT: 42
    printf("%d\n", -42);            // EXPECT: -42
    printf("%d\n", 2147483647);     // EXPECT: 2147483647
    printf("%d\n", -2147483648);    // EXPECT: -2147483648

    /* Basic %s string formatting */
    printf("%s\n", "hello");           // EXPECT: hello
    printf("%s\n", "world");           // EXPECT: world
    printf("%s\n", "");
    printf("%s\n", "foo bar baz");     // EXPECT: foo bar baz
    printf("%s\n", "C is great");      // EXPECT: C is great

    /* Basic %c character formatting */
    printf("%c\n", 'A');            // EXPECT: A
    printf("%c\n", 'z');            // EXPECT: z
    printf("%c\n", '0');            // EXPECT: 0
    printf("%c\n", '!');            // EXPECT: !
    printf("%c\n", '+');            // EXPECT: +

    /* Basic %x hex formatting (lowercase) */
    printf("%x\n", 0);              // EXPECT: 0
    printf("%x\n", 1);              // EXPECT: 1
    printf("%x\n", 10);             // EXPECT: a
    printf("%x\n", 15);             // EXPECT: f
    printf("%x\n", 16);             // EXPECT: 10
    printf("%x\n", 255);            // EXPECT: ff
    printf("%x\n", 256);            // EXPECT: 100
    printf("%x\n", 4096);           // EXPECT: 1000
    printf("%x\n", 65535);          // EXPECT: ffff
    printf("%x\n", 0xdeadbeef);     // EXPECT: deadbeef

    /* Escaped percent sign */
    printf("100%%\n");              // EXPECT: 100%
    printf("50%% off\n");           // EXPECT: 50% off
    printf("%%\n");                 // EXPECT: %

    /* Multiple args of same type */
    printf("%d %d\n", 1, 2);                    // EXPECT: 1 2
    printf("%d %d %d\n", 10, 20, 30);           // EXPECT: 10 20 30
    printf("%d %d %d %d\n", 1, 2, 3, 4);        // EXPECT: 1 2 3 4
    printf("%s %s\n", "foo", "bar");             // EXPECT: foo bar
    printf("%c%c%c\n", 'a', 'b', 'c');          // EXPECT: abc

    /* Mixed types */
    printf("%d %s\n", 42, "hello");              // EXPECT: 42 hello
    printf("%s %d\n", "value:", 99);             // EXPECT: value: 99
    printf("%c %d\n", 'X', 24);                 // EXPECT: X 24
    printf("%d %x\n", 255, 255);                // EXPECT: 255 ff
    printf("%s %c %d\n", "item", 'A', 1);       // EXPECT: item A 1

    /* Expressions as arguments */
    printf("%d\n", add(3, 4));                  // EXPECT: 7
    printf("%d\n", mul(6, 7));                  // EXPECT: 42
    printf("%d %d\n", add(1, 2), mul(3, 4));    // EXPECT: 3 12

    /* Strings with various content */
    printf("%s\n", "hello, world");             // EXPECT: hello, world
    printf("%s\n", "test123");                  // EXPECT: test123
    printf("%s\n", "spaces in here");           // EXPECT: spaces in here

    /* Combining format strings with literals */
    printf("x=%d y=%d\n", 5, 10);              // EXPECT: x=5 y=10
    printf("sum=%d\n", add(100, 200));          // EXPECT: sum=300
    printf("hex=%x dec=%d\n", 16, 16);          // EXPECT: hex=10 dec=16
    printf("char=%c code=%d\n", 'A', 65);       // EXPECT: char=A code=65

    /* printf with many args */
    printf("%d %d %d %d %d\n", 1, 4, 9, 16, 25);           // EXPECT: 1 4 9 16 25
    printf("%d %d %d %d %d %d\n", 2, 3, 5, 7, 11, 13);     // EXPECT: 2 3 5 7 11 13

    /* Newline and other literal chars in format */
    printf("line1\nline2\n");   // EXPECT: line1
                                 // EXPECT: line2
    printf("tab:\there\n");     // EXPECT: tab:	here

    /* %d with computed values */
    int a = 100, b = 37;
    printf("%d\n", a + b);      // EXPECT: 137
    printf("%d\n", a - b);      // EXPECT: 63
    printf("%d\n", a * b / 10); // EXPECT: 370

    /* More hex values */
    printf("%x\n", 0xa);        // EXPECT: a
    printf("%x\n", 0xab);       // EXPECT: ab
    printf("%x\n", 0xabc);      // EXPECT: abc
    printf("%x\n", 0xabcd);     // EXPECT: abcd

    /* Repeated string format */
    printf("%s=%d\n", "x", 42);          // EXPECT: x=42
    printf("%s=%d\n", "result", 100);    // EXPECT: result=100
    printf("%c%c%c%c%c\n", 'h','e','l','l','o'); // EXPECT: hello

    return 0;
}
