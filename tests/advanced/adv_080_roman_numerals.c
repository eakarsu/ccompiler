// EXPECT: === Roman Numerals ===
// EXPECT: 1 -> I -> 1
// EXPECT: 2 -> II -> 2
// EXPECT: 3 -> III -> 3
// EXPECT: 4 -> IV -> 4
// EXPECT: 5 -> V -> 5
// EXPECT: 6 -> VI -> 6
// EXPECT: 9 -> IX -> 9
// EXPECT: 10 -> X -> 10
// EXPECT: 14 -> XIV -> 14
// EXPECT: 40 -> XL -> 40
// EXPECT: 42 -> XLII -> 42
// EXPECT: 50 -> L -> 50
// EXPECT: 90 -> XC -> 90
// EXPECT: 99 -> XCIX -> 99
// EXPECT: 100 -> C -> 100
// EXPECT: 399 -> CCCXCIX -> 399
// EXPECT: 400 -> CD -> 400
// EXPECT: 500 -> D -> 500
// EXPECT: 900 -> CM -> 900
// EXPECT: 999 -> CMXCIX -> 999
// EXPECT: 1000 -> M -> 1000
// EXPECT: 1776 -> MDCCLXXVI -> 1776
// EXPECT: 1999 -> MCMXCIX -> 1999
// EXPECT: 2024 -> MMXXIV -> 2024
// EXPECT: 3999 -> MMMCMXCIX -> 3999
// EXPECT: Roundtrip 1..100: all passed
// EXPECT: Roundtrip 3900..3999: all passed
// EXPECT: All roman numeral tests passed

int printf(const char *fmt, ...);

int int_to_roman(int num, char *out) {
    int values[13];
    values[0] = 1000; values[1] = 900; values[2] = 500; values[3] = 400;
    values[4] = 100; values[5] = 90; values[6] = 50; values[7] = 40;
    values[8] = 10; values[9] = 9; values[10] = 5; values[11] = 4;
    values[12] = 1;

    char syms_a[13];
    char syms_b[13];

    syms_a[0] = 'M'; syms_b[0] = '\0';
    syms_a[1] = 'C'; syms_b[1] = 'M';
    syms_a[2] = 'D'; syms_b[2] = '\0';
    syms_a[3] = 'C'; syms_b[3] = 'D';
    syms_a[4] = 'C'; syms_b[4] = '\0';
    syms_a[5] = 'X'; syms_b[5] = 'C';
    syms_a[6] = 'L'; syms_b[6] = '\0';
    syms_a[7] = 'X'; syms_b[7] = 'L';
    syms_a[8] = 'X'; syms_b[8] = '\0';
    syms_a[9] = 'I'; syms_b[9] = 'X';
    syms_a[10] = 'V'; syms_b[10] = '\0';
    syms_a[11] = 'I'; syms_b[11] = 'V';
    syms_a[12] = 'I'; syms_b[12] = '\0';

    int pos = 0;
    int i;

    for (i = 0; i < 13; i++) {
        while (num >= values[i]) {
            out[pos] = syms_a[i];
            pos++;
            if (syms_b[i] != '\0') {
                out[pos] = syms_b[i];
                pos++;
            }
            num = num - values[i];
        }
    }
    out[pos] = '\0';
    return pos;
}

int roman_char_val(char c) {
    if (c == 'I') return 1;
    if (c == 'V') return 5;
    if (c == 'X') return 10;
    if (c == 'L') return 50;
    if (c == 'C') return 100;
    if (c == 'D') return 500;
    if (c == 'M') return 1000;
    return 0;
}

int roman_to_int(const char *s) {
    int result = 0;
    int i = 0;

    while (s[i] != '\0') {
        int curr = roman_char_val(s[i]);
        int next = 0;
        if (s[i + 1] != '\0') {
            next = roman_char_val(s[i + 1]);
        }

        if (curr < next) {
            result = result + (next - curr);
            i = i + 2;
        } else {
            result = result + curr;
            i++;
        }
    }

    return result;
}

void test_conversion(int num) {
    char buf[32];
    int_to_roman(num, buf);
    int back = roman_to_int(buf);
    printf("%d -> %s -> %d\n", num, buf, back);
}

void test_roundtrip(int start, int end) {
    char buf[32];
    int all_ok = 1;
    int i;

    for (i = start; i <= end; i++) {
        int_to_roman(i, buf);
        int back = roman_to_int(buf);
        if (back != i) {
            printf("FAIL: %d -> %s -> %d\n", i, buf, back);
            all_ok = 0;
        }
    }

    if (all_ok) {
        printf("Roundtrip %d..%d: all passed\n", start, end);
    }
}

int main(void) {
    printf("=== Roman Numerals ===\n");

    test_conversion(1);
    test_conversion(2);
    test_conversion(3);
    test_conversion(4);
    test_conversion(5);
    test_conversion(6);
    test_conversion(9);
    test_conversion(10);
    test_conversion(14);
    test_conversion(40);
    test_conversion(42);
    test_conversion(50);
    test_conversion(90);
    test_conversion(99);
    test_conversion(100);
    test_conversion(399);
    test_conversion(400);
    test_conversion(500);
    test_conversion(900);
    test_conversion(999);
    test_conversion(1000);
    test_conversion(1776);
    test_conversion(1999);
    test_conversion(2024);
    test_conversion(3999);

    test_roundtrip(1, 100);
    test_roundtrip(3900, 3999);

    printf("All roman numeral tests passed\n");
    return 0;
}
