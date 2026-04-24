int printf(const char *fmt, ...);
// EXPECT: Integer to Roman:\n  1 -> I\n  4 -> IV\n  9 -> IX\n  14 -> XIV\n  42 -> XLII\n  99 -> XCIX\n  399 -> CCCXCIX\n  494 -> CDXCIV\n  1000 -> M\n  1994 -> MCMXCIV\n  2024 -> MMXXIV\n  3999 -> MMMCMXCIX\n\nRoman to Integer:\n  I -> 1\n  IV -> 4\n  IX -> 9\n  XIV -> 14\n  XLII -> 42\n  XCIX -> 99\n  CCCXCIX -> 399\n  CDXCIV -> 494\n  M -> 1000\n  MCMXCIV -> 1994\n  MMXXIV -> 2024\n  MMMCMXCIX -> 3999\n\nRoundtrip verification:\n  1-100 roundtrip: PASS
// Test: Roman numeral converter

void int_to_roman(int num, char *result) {
    int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    char *symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    int pos = 0;
    int i;

    for (i = 0; i < 13; i++) {
        while (num >= values[i]) {
            char *s = symbols[i];
            int j = 0;
            while (s[j] != 0) {
                result[pos] = s[j];
                pos++;
                j++;
            }
            num = num - values[i];
        }
    }
    result[pos] = 0;
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

int roman_to_int(char *s) {
    int result = 0;
    int i = 0;
    while (s[i] != 0) {
        int curr = roman_char_val(s[i]);
        int next = 0;
        if (s[i + 1] != 0) next = roman_char_val(s[i + 1]);
        if (curr < next) {
            result = result + next - curr;
            i = i + 2;
        } else {
            result = result + curr;
            i++;
        }
    }
    return result;
}

int main(void) {
    char roman[64];

    int nums[] = {1, 4, 9, 14, 42, 99, 399, 494, 1000, 1994, 2024, 3999};
    int n = 12;
    int i;

    printf("Integer to Roman:\n");
    for (i = 0; i < n; i++) {
        int_to_roman(nums[i], roman);
        printf("  %d -> %s\n", nums[i], roman);
    }

    printf("\nRoman to Integer:\n");
    char *romans[] = {"I", "IV", "IX", "XIV", "XLII", "XCIX", "CCCXCIX", "CDXCIV", "M", "MCMXCIV", "MMXXIV", "MMMCMXCIX"};
    for (i = 0; i < n; i++) {
        printf("  %s -> %d\n", romans[i], roman_to_int(romans[i]));
    }

    printf("\nRoundtrip verification:\n");
    int all_ok = 1;
    for (i = 1; i <= 100; i++) {
        int_to_roman(i, roman);
        int back = roman_to_int(roman);
        if (back != i) {
            printf("  FAIL: %d -> %s -> %d\n", i, roman, back);
            all_ok = 0;
        }
    }
    printf("  1-100 roundtrip: %s\n", all_ok ? "PASS" : "FAIL");

    return 0;
}
