int printf(const char *fmt, ...);

int main(void) {
    /* Basic char variables */
    char c1 = 'A';
    char c2 = 'a';
    char c3 = '0';
    printf("%c\n", c1);          // EXPECT: A
    printf("%c\n", c2);          // EXPECT: a
    printf("%c\n", c3);          // EXPECT: 0
    printf("%d\n", c1);          // EXPECT: 65
    printf("%d\n", c2);          // EXPECT: 97
    printf("%d\n", c3);          // EXPECT: 48

    /* Char arithmetic - promoted to int */
    printf("%d\n", c1 + 1);      // EXPECT: 66
    /* 'A'=65, 65+1=66 */
    printf("%c\n", (char)(c1 + 1)); // EXPECT: B
    printf("%d\n", c2 - c1);     // EXPECT: 32
    /* 'a'-'A' = 97-65 = 32 */

    /* Case conversion via arithmetic */
    char upper = 'Z';
    char lower = (char)('Z' + 32);
    printf("%c\n", upper);        // EXPECT: Z
    printf("%c\n", lower);        // EXPECT: z
    printf("%d\n", upper);        // EXPECT: 90
    printf("%d\n", lower);        // EXPECT: 122

    /* Digit char to int value */
    char digit = '7';
    int val = digit - '0';
    printf("%d\n", val);          // EXPECT: 7
    /* '7'=55, '0'=48, 55-48=7 */

    /* Signed char range: -128 to 127 */
    signed char sc_min = -128;
    signed char sc_max = 127;
    printf("%d\n", sc_min);       // EXPECT: -128
    printf("%d\n", sc_max);       // EXPECT: 127
    printf("%d\n", sc_min + 1);   // EXPECT: -127
    printf("%d\n", sc_max - 1);   // EXPECT: 126

    /* Signed char arithmetic promoted to int */
    signed char neg = -10;
    signed char pos = 20;
    printf("%d\n", neg + pos);    // EXPECT: 10
    printf("%d\n", neg * 3);      // EXPECT: -30
    printf("%d\n", pos / 3);      // EXPECT: 6
    /* 20/3 = 6 */

    /* Unsigned char range: 0 to 255 */
    unsigned char uc_max = 255;
    unsigned char uc_zero = 0;
    printf("%d\n", uc_max);       // EXPECT: 255
    printf("%d\n", uc_zero);      // EXPECT: 0
    printf("%d\n", uc_max - 1);   // EXPECT: 254

    /* Unsigned char wrapping via assignment */
    unsigned char uc = 250;
    printf("%d\n", uc + 5);       // EXPECT: 255
    /* 250+5=255, fits in int (promoted), printed as int */
    printf("%d\n", uc + 6);       // EXPECT: 256
    /* promoted to int addition: 250+6=256 as int */

    /* Char array indexing */
    char arr[6];
    arr[0] = 'H';
    arr[1] = 'e';
    arr[2] = 'l';
    arr[3] = 'l';
    arr[4] = 'o';
    arr[5] = '\0';
    printf("%s\n", arr);          // EXPECT: Hello
    printf("%c\n", arr[0]);       // EXPECT: H
    printf("%c\n", arr[4]);       // EXPECT: o
    printf("%d\n", arr[1]);       // EXPECT: 101
    /* 'e' = 101 */

    /* Iterating over char array values */
    char nums[5];
    nums[0] = 10;
    nums[1] = 20;
    nums[2] = 30;
    nums[3] = 40;
    nums[4] = 50;
    int total = 0;
    total = total + nums[0];
    total = total + nums[1];
    total = total + nums[2];
    total = total + nums[3];
    total = total + nums[4];
    printf("%d\n", total);        // EXPECT: 150

    /* Char comparison */
    char x = 'M';
    char y = 'N';
    printf("%d\n", x < y);        // EXPECT: 1
    printf("%d\n", x == y);       // EXPECT: 0
    printf("%d\n", x > 'A');      // EXPECT: 1
    /* 'M'=77 > 'A'=65, true=1 */

    /* Char in conditional */
    char ch = 'B';
    int is_upper = (ch >= 'A' && ch <= 'Z');
    printf("%d\n", is_upper);     // EXPECT: 1
    char lc = 'b';
    int is_lower = (lc >= 'a' && lc <= 'z');
    printf("%d\n", is_lower);     // EXPECT: 1
    int is_digit_char = (ch >= '0' && ch <= '9');
    printf("%d\n", is_digit_char);// EXPECT: 0

    /* Promotion: char + char yields int */
    char p = 100;
    char q = 100;
    int result = p + q;
    printf("%d\n", result);       // EXPECT: 200
    /* Each promoted to int before addition */

    /* Negative signed char multiplication */
    signed char sc = -5;
    printf("%d\n", sc * sc);      // EXPECT: 25
    printf("%d\n", sc + 128);     // EXPECT: 123

    /* String literal char access */
    char *str = "ABCDE";
    printf("%c\n", str[0]);       // EXPECT: A
    printf("%c\n", str[2]);       // EXPECT: C
    printf("%c\n", str[4]);       // EXPECT: E
    printf("%d\n", str[4] - str[0]); // EXPECT: 4
    /* 'E'=69, 'A'=65, 69-65=4 */

    return 0;
}
