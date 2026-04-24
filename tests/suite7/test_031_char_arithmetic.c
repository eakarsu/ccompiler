int printf(const char *fmt, ...);

int char_to_upper(int c) {
    if (c >= 97 && c <= 122) {
        return c - 32;
    }
    return c;
}

int char_to_lower(int c) {
    if (c >= 65 && c <= 90) {
        return c + 32;
    }
    return c;
}

int is_uppercase(int c) {
    return (c >= 65 && c <= 90);
}

int is_lowercase(int c) {
    return (c >= 97 && c <= 122);
}

int is_letter(int c) {
    return is_uppercase(c) || is_lowercase(c);
}

int char_distance(int a, int b) {
    int diff = a - b;
    if (diff < 0) diff = -diff;
    return diff;
}

int next_char(int c) {
    return c + 1;
}

int prev_char(int c) {
    return c - 1;
}

int wrap_alpha(int c) {
    if (c > 122) return 97;
    if (c < 97) return 122;
    return c;
}

int sum_char_range(int start, int end) {
    int sum = 0;
    int i = start;
    while (i <= end) {
        sum = sum + i;
        i = i + 1;
    }
    return sum;
}

int count_in_range(int lo, int hi) {
    return hi - lo + 1;
}

int main(void) {
    int a;
    int b;
    int result;

    a = 'A';
    // EXPECT: A=65
    printf("A=%d\n", a);

    b = a + 1;
    // EXPECT: A+1=66
    printf("A+1=%d\n", b);

    result = 'a' - 'A';
    // EXPECT: diff=32
    printf("diff=%d\n", result);

    // EXPECT: upper_a=65
    printf("upper_a=%d\n", char_to_upper('a'));

    // EXPECT: upper_z=90
    printf("upper_z=%d\n", char_to_upper('z'));

    // EXPECT: upper_A=65
    printf("upper_A=%d\n", char_to_upper('A'));

    // EXPECT: lower_A=97
    printf("lower_A=%d\n", char_to_lower('A'));

    // EXPECT: lower_Z=122
    printf("lower_Z=%d\n", char_to_lower('Z'));

    // EXPECT: lower_a=97
    printf("lower_a=%d\n", char_to_lower('a'));

    // EXPECT: is_upper_A=1
    printf("is_upper_A=%d\n", is_uppercase('A'));

    // EXPECT: is_upper_a=0
    printf("is_upper_a=%d\n", is_uppercase('a'));

    // EXPECT: is_lower_z=1
    printf("is_lower_z=%d\n", is_lowercase('z'));

    // EXPECT: is_lower_Z=0
    printf("is_lower_Z=%d\n", is_lowercase('Z'));

    // EXPECT: is_letter_m=1
    printf("is_letter_m=%d\n", is_letter('m'));

    // EXPECT: is_letter_5=0
    printf("is_letter_5=%d\n", is_letter('5'));

    // EXPECT: dist_A_Z=25
    printf("dist_A_Z=%d\n", char_distance('A', 'Z'));

    // EXPECT: dist_a_e=4
    printf("dist_a_e=%d\n", char_distance('a', 'e'));

    // EXPECT: next_A=66
    printf("next_A=%d\n", next_char('A'));

    // EXPECT: prev_C=66
    printf("prev_C=%d\n", prev_char('C'));

    // EXPECT: wrap_123=97
    printf("wrap_123=%d\n", wrap_alpha(123));

    // EXPECT: wrap_96=122
    printf("wrap_96=%d\n", wrap_alpha(96));

    // EXPECT: wrap_109=109
    printf("wrap_109=%d\n", wrap_alpha(109));

    result = sum_char_range('A', 'C');
    // EXPECT: sum_ABC=198
    printf("sum_ABC=%d\n", result);

    // EXPECT: count_A_Z=26
    printf("count_A_Z=%d\n", count_in_range('A', 'Z'));

    // EXPECT: count_0_9=10
    printf("count_0_9=%d\n", count_in_range('0', '9'));

    char word[6];
    word[0] = 'h';
    word[1] = 'e';
    word[2] = 'l';
    word[3] = 'l';
    word[4] = 'o';
    word[5] = 0;

    int i = 0;
    char upper[6];
    while (word[i] != 0) {
        upper[i] = char_to_upper(word[i]);
        i = i + 1;
    }
    upper[i] = 0;
    // EXPECT: upper=HELLO
    printf("upper=%s\n", upper);

    // EXPECT: H=72
    printf("H=%d\n", upper[0]);

    return 0;
}
