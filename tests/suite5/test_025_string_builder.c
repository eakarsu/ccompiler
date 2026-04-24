int printf(const char *fmt, ...);

int main(void) {
    // Build a string char by char
    char buf[64];
    buf[0] = 'H';
    buf[1] = 'e';
    buf[2] = 'l';
    buf[3] = 'l';
    buf[4] = 'o';
    buf[5] = 0;

    // EXPECT: built: Hello
    printf("built: %s\n", buf);

    // String length via pointer walk
    int len = 0;
    char *p = buf;
    while (*p != 0) {
        len = len + 1;
        p = p + 1;
    }
    // EXPECT: len: 5
    printf("len: %d\n", len);

    // Append " World" manually
    buf[5] = ' ';
    buf[6] = 'W';
    buf[7] = 'o';
    buf[8] = 'r';
    buf[9] = 'l';
    buf[10] = 'd';
    buf[11] = 0;
    // EXPECT: appended: Hello World
    printf("appended: %s\n", buf);

    // String copy function inline
    char copy[64];
    int i = 0;
    while (buf[i] != 0) {
        copy[i] = buf[i];
        i = i + 1;
    }
    copy[i] = 0;
    // EXPECT: copy: Hello World
    printf("copy: %s\n", copy);

    // Substring extraction: extract "World"
    char sub[64];
    int src = 6;
    int dst = 0;
    while (buf[src] != 0) {
        sub[dst] = buf[src];
        src = src + 1;
        dst = dst + 1;
    }
    sub[dst] = 0;
    // EXPECT: sub: World
    printf("sub: %s\n", sub);

    // Build string from digits
    char digits[16];
    int num = 12345;
    int pos = 0;
    // Extract digits in reverse
    char tmp[16];
    int tpos = 0;
    while (num > 0) {
        tmp[tpos] = (char)(48 + (num - (num / 10) * 10));
        num = num / 10;
        tpos = tpos + 1;
    }
    // Reverse into digits
    int j = tpos - 1;
    while (j >= 0) {
        digits[pos] = tmp[j];
        pos = pos + 1;
        j = j - 1;
    }
    digits[pos] = 0;
    // EXPECT: digits: 12345
    printf("digits: %s\n", digits);

    // Concatenate two strings
    char s1[32];
    s1[0] = 'A';
    s1[1] = 'B';
    s1[2] = 'C';
    s1[3] = 0;

    char s2[32];
    s2[0] = 'D';
    s2[1] = 'E';
    s2[2] = 'F';
    s2[3] = 0;

    char cat[64];
    i = 0;
    while (s1[i] != 0) {
        cat[i] = s1[i];
        i = i + 1;
    }
    j = 0;
    while (s2[j] != 0) {
        cat[i] = s2[j];
        i = i + 1;
        j = j + 1;
    }
    cat[i] = 0;
    // EXPECT: concat: ABCDEF
    printf("concat: %s\n", cat);

    // String comparison
    char cmp1[8];
    cmp1[0] = 'a';
    cmp1[1] = 'b';
    cmp1[2] = 'c';
    cmp1[3] = 0;

    char cmp2[8];
    cmp2[0] = 'a';
    cmp2[1] = 'b';
    cmp2[2] = 'c';
    cmp2[3] = 0;

    int equal = 1;
    i = 0;
    while (cmp1[i] != 0 && cmp2[i] != 0) {
        if (cmp1[i] != cmp2[i]) {
            equal = 0;
        }
        i = i + 1;
    }
    if (cmp1[i] != cmp2[i]) {
        equal = 0;
    }
    // EXPECT: strings equal: 1
    printf("strings equal: %d\n", equal);

    // Count chars in string
    char countbuf[32];
    countbuf[0] = 'a';
    countbuf[1] = 'b';
    countbuf[2] = 'a';
    countbuf[3] = 'c';
    countbuf[4] = 'a';
    countbuf[5] = 0;

    int count = 0;
    i = 0;
    while (countbuf[i] != 0) {
        if (countbuf[i] == 'a') {
            count = count + 1;
        }
        i = i + 1;
    }
    // EXPECT: count of a: 3
    printf("count of a: %d\n", count);

    // Reverse a string in place
    char rev[16];
    rev[0] = 'A';
    rev[1] = 'B';
    rev[2] = 'C';
    rev[3] = 'D';
    rev[4] = 'E';
    rev[5] = 0;

    int rlen = 0;
    while (rev[rlen] != 0) {
        rlen = rlen + 1;
    }
    int left = 0;
    int right = rlen - 1;
    while (left < right) {
        char t = rev[left];
        rev[left] = rev[right];
        rev[right] = t;
        left = left + 1;
        right = right - 1;
    }
    // EXPECT: reversed: EDCBA
    printf("reversed: %s\n", rev);

    // Build string with spaces
    char spaced[32];
    spaced[0] = 'X';
    spaced[1] = ' ';
    spaced[2] = 'Y';
    spaced[3] = ' ';
    spaced[4] = 'Z';
    spaced[5] = 0;
    // EXPECT: spaced: X Y Z
    printf("spaced: %s\n", spaced);

    // Check first char code
    // EXPECT: first char code: 88
    printf("first char code: %d\n", (int)spaced[0]);

    // String to int conversion
    char numstr[8];
    numstr[0] = '4';
    numstr[1] = '2';
    numstr[2] = '0';
    numstr[3] = 0;
    int parsed = 0;
    i = 0;
    while (numstr[i] != 0) {
        parsed = parsed * 10 + (numstr[i] - 48);
        i = i + 1;
    }
    // EXPECT: parsed: 420
    printf("parsed: %d\n", parsed);

    // Extract prefix of length n
    char prefix[16];
    i = 0;
    while (i < 3 && buf[i] != 0) {
        prefix[i] = buf[i];
        i = i + 1;
    }
    prefix[i] = 0;
    // EXPECT: prefix: Hel
    printf("prefix: %s\n", prefix);

    // Length of concatenated string
    int catlen = 0;
    while (cat[catlen] != 0) {
        catlen = catlen + 1;
    }
    // EXPECT: concat len: 6
    printf("concat len: %d\n", catlen);

    return 0;
}
