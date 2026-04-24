int printf(const char *fmt, ...);
// EXPECT: === Char to Int ===
// char 'A' as int: 65
// char 'Z' as int: 90
// char 'a' as int: 97
// char 'z' as int: 122
// char '0' as int: 48
// char '9' as int: 57
// char ' ' as int: 32
// char '!' as int: 33
// === Int to Char ===
// int 65 as char: A
// int 66 as char: B
// int 67 as char: C
// int 97 as char: a
// int 48 as char: 0
// int 49 as char: 1
// === Char Arithmetic ===
// 'A'+1 = 66 (char: B)
// 'A'+25 = 90 (char: Z)
// 'a'-'A' = 32
// 'Z'-'A' = 25
// '9'-'0' = 9
// '5'-'0' = 5
// === Case Conversion ===
// upper A -> lower a (97)
// upper B -> lower b (98)
// upper Z -> lower z (122)
// lower a -> upper A (65)
// lower z -> upper Z (90)
// === Digit Char to Int Value ===
// '0' -> 0
// '1' -> 1
// '5' -> 5
// '9' -> 9
// === Int Value to Digit Char ===
// 0 -> '0' (48)
// 3 -> '3' (51)
// 7 -> '7' (55)
// 9 -> '9' (57)
// === Truncation: Int to Char ===
// 200 as char: -56
// 255 as char: -1
// 256 as char: 0
// 257 as char: 1
// 128 as char: -128
// 127 as char: 127
// -1 as char: -1
// -128 as char: -128
// -129 as char: 127
// === Sign Extension ===
// char -1 as int: -1
// char -128 as int: -128
// char 127 as int: 127
// char -56 as int: -56
// === Mixed Arithmetic ===
// 'A' + 1 = 66
// 10 + '0' = 58
// 'a' - 32 = 65
// 100 * 'A' / 'A' = 100
// === Comparison: char vs int ===
// 'A'==65: 1
// 'A'<66: 1
// 'Z'>89: 1
// 'a'!=65: 1
// '0'==48: 1
// === Array of Chars as Ints ===
// msg[0]=72 (H)
// msg[1]=101 (e)
// msg[2]=108 (l)
// msg[3]=108 (l)
// msg[4]=111 (o)
// sum of codes=500
// === Is Upper ===
// is_upper('A')=1
// is_upper('Z')=1
// is_upper('a')=0
// is_upper('0')=0
// === Is Lower ===
// is_lower('a')=1
// is_lower('z')=1
// is_lower('A')=0
// is_lower('5')=0
// === Is Digit ===
// is_digit('0')=1
// is_digit('9')=1
// is_digit('a')=0
// is_digit('A')=0
// === Is Alpha ===
// is_alpha('A')=1
// is_alpha('z')=1
// is_alpha('5')=0
// is_alpha(' ')=0
// === To Upper String ===
// before: 104 101 108 108 111
// after:  72 69 76 76 79
// === To Lower String ===
// before: 72 69 76 76 79
// after:  104 101 108 108 111
// === Parse Integer from Chars ===
// parse "12345" = 12345
// parse "00042" = 42
// parse "99999" = 99999
// parse "10000" = 10000
// === Int to Char Digits ===
// 12345 -> digits: 1 2 3 4 5
// 9876 -> digits: 9 8 7 6
// 100 -> digits: 1 0 0
// === Char Pointer Traversal ===
// chars: 65 66 67 68 69
// reversed: 69 68 67 66 65
// === Wraparound Behavior ===
// 127+1 as char: -128
// -128-1 as char: 127
// 0-1 as char: -1
// === Bitwise on Chars ===
// 'A' & 0x0F = 1
// 'B' & 0x0F = 2
// 'a' & 0x0F = 1
// 'A' | 0x20 = 97
// 'a' & 0x5F = 65 (but not always correct)
// === ROT13 on Uppercase ===
// A -> N (78)
// N -> A (65)
// Z -> M (77)
// M -> Z (90)
// === Caesar Shift ===
// A shift 3 -> D (68)
// X shift 3 -> A (65)
// Y shift 3 -> B (66)
// Z shift 3 -> C (67)
// === Char Frequency Count ===
// freq[0]=1
// freq[1]=2
// freq[2]=3
// freq[3]=2
// freq[4]=1
// === All type conversion tests done ===

int is_upper(char c) {
    if (c >= 'A' && c <= 'Z') {
        return 1;
    }
    return 0;
}

int is_lower(char c) {
    if (c >= 'a' && c <= 'z') {
        return 1;
    }
    return 0;
}

int is_digit(char c) {
    if (c >= '0' && c <= '9') {
        return 1;
    }
    return 0;
}

int is_alpha(char c) {
    if (is_upper(c) || is_lower(c)) {
        return 1;
    }
    return 0;
}

char to_upper(char c) {
    if (is_lower(c)) {
        return c - 32;
    }
    return c;
}

char to_lower(char c) {
    if (is_upper(c)) {
        return c + 32;
    }
    return c;
}

int char_to_digit(char c) {
    return c - '0';
}

char digit_to_char(int d) {
    return d + '0';
}

int parse_int(char *digits, int len) {
    int result;
    int i;
    result = 0;
    i = 0;
    while (i < len) {
        result = result * 10 + char_to_digit(digits[i]);
        i = i + 1;
    }
    return result;
}

void int_to_digits(int val, int *digits, int *len) {
    int temp;
    int count;
    int i;
    int j;
    int tmp;
    if (val == 0) {
        digits[0] = 0;
        *len = 1;
        return;
    }
    temp = val;
    count = 0;
    while (temp > 0) {
        digits[count] = temp % 10;
        temp = temp / 10;
        count = count + 1;
    }
    // reverse
    i = 0;
    j = count - 1;
    while (i < j) {
        tmp = digits[i];
        digits[i] = digits[j];
        digits[j] = tmp;
        i = i + 1;
        j = j - 1;
    }
    *len = count;
}

char rot13_upper(char c) {
    int offset;
    offset = c - 'A';
    offset = (offset + 13) % 26;
    return 'A' + offset;
}

char caesar_shift_upper(char c, int shift) {
    int offset;
    offset = c - 'A';
    offset = (offset + shift) % 26;
    return 'A' + offset;
}

void to_upper_arr(char *arr, int len) {
    int i;
    i = 0;
    while (i < len) {
        arr[i] = to_upper(arr[i]);
        i = i + 1;
    }
}

void to_lower_arr(char *arr, int len) {
    int i;
    i = 0;
    while (i < len) {
        arr[i] = to_lower(arr[i]);
        i = i + 1;
    }
}

void print_char_codes(char *arr, int len) {
    int i;
    i = 0;
    while (i < len) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", (int)arr[i]);
        i = i + 1;
    }
    printf("\n");
}

void reverse_chars(char *arr, int len) {
    int i;
    int j;
    char tmp;
    i = 0;
    j = len - 1;
    while (i < j) {
        tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
        i = i + 1;
        j = j - 1;
    }
}

int main() {
    int i;
    int val;
    char c;
    char msg[5];
    int sum;
    char digits_c[5];
    int digits_i[10];
    int dlen;
    char chars[5];
    char ch;
    int freq_data[9];
    int freq[5];

    printf("=== Char to Int ===\n");
    printf("char 'A' as int: %d\n", (int)'A');
    printf("char 'Z' as int: %d\n", (int)'Z');
    printf("char 'a' as int: %d\n", (int)'a');
    printf("char 'z' as int: %d\n", (int)'z');
    printf("char '0' as int: %d\n", (int)'0');
    printf("char '9' as int: %d\n", (int)'9');
    printf("char ' ' as int: %d\n", (int)' ');
    printf("char '!' as int: %d\n", (int)'!');

    printf("=== Int to Char ===\n");
    printf("int 65 as char: %c\n", (char)65);
    printf("int 66 as char: %c\n", (char)66);
    printf("int 67 as char: %c\n", (char)67);
    printf("int 97 as char: %c\n", (char)97);
    printf("int 48 as char: %c\n", (char)48);
    printf("int 49 as char: %c\n", (char)49);

    printf("=== Char Arithmetic ===\n");
    val = 'A' + 1;
    printf("'A'+1 = %d (char: %c)\n", val, (char)val);
    val = 'A' + 25;
    printf("'A'+25 = %d (char: %c)\n", val, (char)val);
    printf("'a'-'A' = %d\n", 'a' - 'A');
    printf("'Z'-'A' = %d\n", 'Z' - 'A');
    printf("'9'-'0' = %d\n", '9' - '0');
    printf("'5'-'0' = %d\n", '5' - '0');

    printf("=== Case Conversion ===\n");
    c = to_lower('A');
    printf("upper A -> lower %c (%d)\n", c, (int)c);
    c = to_lower('B');
    printf("upper B -> lower %c (%d)\n", c, (int)c);
    c = to_lower('Z');
    printf("upper Z -> lower %c (%d)\n", c, (int)c);
    c = to_upper('a');
    printf("lower a -> upper %c (%d)\n", c, (int)c);
    c = to_upper('z');
    printf("lower z -> upper %c (%d)\n", c, (int)c);

    printf("=== Digit Char to Int Value ===\n");
    printf("'0' -> %d\n", char_to_digit('0'));
    printf("'1' -> %d\n", char_to_digit('1'));
    printf("'5' -> %d\n", char_to_digit('5'));
    printf("'9' -> %d\n", char_to_digit('9'));

    printf("=== Int Value to Digit Char ===\n");
    printf("0 -> '%c' (%d)\n", digit_to_char(0), (int)digit_to_char(0));
    printf("3 -> '%c' (%d)\n", digit_to_char(3), (int)digit_to_char(3));
    printf("7 -> '%c' (%d)\n", digit_to_char(7), (int)digit_to_char(7));
    printf("9 -> '%c' (%d)\n", digit_to_char(9), (int)digit_to_char(9));

    printf("=== Truncation: Int to Char ===\n");
    c = (char)200;
    printf("200 as char: %d\n", (int)c);
    c = (char)255;
    printf("255 as char: %d\n", (int)c);
    c = (char)256;
    printf("256 as char: %d\n", (int)c);
    c = (char)257;
    printf("257 as char: %d\n", (int)c);
    c = (char)128;
    printf("128 as char: %d\n", (int)c);
    c = (char)127;
    printf("127 as char: %d\n", (int)c);
    c = (char)(-1);
    printf("-1 as char: %d\n", (int)c);
    c = (char)(-128);
    printf("-128 as char: %d\n", (int)c);
    c = (char)(-129);
    printf("-129 as char: %d\n", (int)c);

    printf("=== Sign Extension ===\n");
    c = -1;
    val = (int)c;
    printf("char -1 as int: %d\n", val);
    c = -128;
    val = (int)c;
    printf("char -128 as int: %d\n", val);
    c = 127;
    val = (int)c;
    printf("char 127 as int: %d\n", val);
    c = (char)200;
    val = (int)c;
    printf("char -56 as int: %d\n", val);

    printf("=== Mixed Arithmetic ===\n");
    printf("'A' + 1 = %d\n", 'A' + 1);
    printf("10 + '0' = %d\n", 10 + '0');
    printf("'a' - 32 = %d\n", 'a' - 32);
    printf("100 * 'A' / 'A' = %d\n", 100 * 'A' / 'A');

    printf("=== Comparison: char vs int ===\n");
    printf("'A'==65: %d\n", ('A' == 65));
    printf("'A'<66: %d\n", ('A' < 66));
    printf("'Z'>89: %d\n", ('Z' > 89));
    printf("'a'!=65: %d\n", ('a' != 65));
    printf("'0'==48: %d\n", ('0' == 48));

    printf("=== Array of Chars as Ints ===\n");
    msg[0] = 'H';
    msg[1] = 'e';
    msg[2] = 'l';
    msg[3] = 'l';
    msg[4] = 'o';
    sum = 0;
    i = 0;
    while (i < 5) {
        printf("msg[%d]=%d (%c)\n", i, (int)msg[i], msg[i]);
        sum = sum + (int)msg[i];
        i = i + 1;
    }
    printf("sum of codes=%d\n", sum);

    printf("=== Is Upper ===\n");
    printf("is_upper('A')=%d\n", is_upper('A'));
    printf("is_upper('Z')=%d\n", is_upper('Z'));
    printf("is_upper('a')=%d\n", is_upper('a'));
    printf("is_upper('0')=%d\n", is_upper('0'));

    printf("=== Is Lower ===\n");
    printf("is_lower('a')=%d\n", is_lower('a'));
    printf("is_lower('z')=%d\n", is_lower('z'));
    printf("is_lower('A')=%d\n", is_lower('A'));
    printf("is_lower('5')=%d\n", is_lower('5'));

    printf("=== Is Digit ===\n");
    printf("is_digit('0')=%d\n", is_digit('0'));
    printf("is_digit('9')=%d\n", is_digit('9'));
    printf("is_digit('a')=%d\n", is_digit('a'));
    printf("is_digit('A')=%d\n", is_digit('A'));

    printf("=== Is Alpha ===\n");
    printf("is_alpha('A')=%d\n", is_alpha('A'));
    printf("is_alpha('z')=%d\n", is_alpha('z'));
    printf("is_alpha('5')=%d\n", is_alpha('5'));
    printf("is_alpha(' ')=%d\n", is_alpha(' '));

    printf("=== To Upper String ===\n");
    msg[0] = 'h';
    msg[1] = 'e';
    msg[2] = 'l';
    msg[3] = 'l';
    msg[4] = 'o';
    printf("before: ");
    print_char_codes(msg, 5);
    to_upper_arr(msg, 5);
    printf("after:  ");
    print_char_codes(msg, 5);

    printf("=== To Lower String ===\n");
    printf("before: ");
    print_char_codes(msg, 5);
    to_lower_arr(msg, 5);
    printf("after:  ");
    print_char_codes(msg, 5);

    printf("=== Parse Integer from Chars ===\n");
    digits_c[0] = '1'; digits_c[1] = '2'; digits_c[2] = '3';
    digits_c[3] = '4'; digits_c[4] = '5';
    printf("parse \"12345\" = %d\n", parse_int(digits_c, 5));

    digits_c[0] = '0'; digits_c[1] = '0'; digits_c[2] = '0';
    digits_c[3] = '4'; digits_c[4] = '2';
    printf("parse \"00042\" = %d\n", parse_int(digits_c, 5));

    digits_c[0] = '9'; digits_c[1] = '9'; digits_c[2] = '9';
    digits_c[3] = '9'; digits_c[4] = '9';
    printf("parse \"99999\" = %d\n", parse_int(digits_c, 5));

    digits_c[0] = '1'; digits_c[1] = '0'; digits_c[2] = '0';
    digits_c[3] = '0'; digits_c[4] = '0';
    printf("parse \"10000\" = %d\n", parse_int(digits_c, 5));

    printf("=== Int to Char Digits ===\n");
    int_to_digits(12345, digits_i, &dlen);
    printf("12345 -> digits:");
    i = 0;
    while (i < dlen) {
        printf(" %d", digits_i[i]);
        i = i + 1;
    }
    printf("\n");

    int_to_digits(9876, digits_i, &dlen);
    printf("9876 -> digits:");
    i = 0;
    while (i < dlen) {
        printf(" %d", digits_i[i]);
        i = i + 1;
    }
    printf("\n");

    int_to_digits(100, digits_i, &dlen);
    printf("100 -> digits:");
    i = 0;
    while (i < dlen) {
        printf(" %d", digits_i[i]);
        i = i + 1;
    }
    printf("\n");

    printf("=== Char Pointer Traversal ===\n");
    chars[0] = 65; chars[1] = 66; chars[2] = 67;
    chars[3] = 68; chars[4] = 69;
    printf("chars:");
    i = 0;
    while (i < 5) {
        printf(" %d", (int)chars[i]);
        i = i + 1;
    }
    printf("\n");
    reverse_chars(chars, 5);
    printf("reversed:");
    i = 0;
    while (i < 5) {
        printf(" %d", (int)chars[i]);
        i = i + 1;
    }
    printf("\n");

    printf("=== Wraparound Behavior ===\n");
    c = 127;
    c = c + 1;
    printf("127+1 as char: %d\n", (int)c);
    c = -128;
    c = c - 1;
    printf("-128-1 as char: %d\n", (int)c);
    c = 0;
    c = c - 1;
    printf("0-1 as char: %d\n", (int)c);

    printf("=== Bitwise on Chars ===\n");
    printf("'A' & 0x0F = %d\n", 'A' & 15);
    printf("'B' & 0x0F = %d\n", 'B' & 15);
    printf("'a' & 0x0F = %d\n", 'a' & 15);
    printf("'A' | 0x20 = %d\n", 'A' | 32);
    printf("'a' & 0x5F = %d (but not always correct)\n", 'a' & 95);

    printf("=== ROT13 on Uppercase ===\n");
    ch = rot13_upper('A');
    printf("A -> %c (%d)\n", ch, (int)ch);
    ch = rot13_upper('N');
    printf("N -> %c (%d)\n", ch, (int)ch);
    ch = rot13_upper('Z');
    printf("Z -> %c (%d)\n", ch, (int)ch);
    ch = rot13_upper('M');
    printf("M -> %c (%d)\n", ch, (int)ch);

    printf("=== Caesar Shift ===\n");
    ch = caesar_shift_upper('A', 3);
    printf("A shift 3 -> %c (%d)\n", ch, (int)ch);
    ch = caesar_shift_upper('X', 3);
    printf("X shift 3 -> %c (%d)\n", ch, (int)ch);
    ch = caesar_shift_upper('Y', 3);
    printf("Y shift 3 -> %c (%d)\n", ch, (int)ch);
    ch = caesar_shift_upper('Z', 3);
    printf("Z shift 3 -> %c (%d)\n", ch, (int)ch);

    printf("=== Char Frequency Count ===\n");
    freq_data[0] = 0;
    freq_data[1] = 1;
    freq_data[2] = 1;
    freq_data[3] = 2;
    freq_data[4] = 2;
    freq_data[5] = 2;
    freq_data[6] = 3;
    freq_data[7] = 3;
    freq_data[8] = 4;
    i = 0;
    while (i < 5) {
        freq[i] = 0;
        i = i + 1;
    }
    i = 0;
    while (i < 9) {
        freq[freq_data[i]] = freq[freq_data[i]] + 1;
        i = i + 1;
    }
    i = 0;
    while (i < 5) {
        printf("freq[%d]=%d\n", i, freq[i]);
        i = i + 1;
    }

    printf("=== All type conversion tests done ===\n");

    return 0;
}
