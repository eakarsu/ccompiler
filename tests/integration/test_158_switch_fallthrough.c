int printf(const char *fmt, ...);
// EXPECT: Days remaining from month:\nmonth 1: 365 days left\nmonth 2: 334 days left\nmonth 3: 306 days left\nmonth 4: 275 days left\nmonth 5: 245 days left\nmonth 6: 214 days left\nmonth 7: 184 days left\nmonth 8: 153 days left\nmonth 9: 122 days left\nmonth 10: 92 days left\nmonth 11: 61 days left\nmonth 12: 31 days left\nCharacter classification:\nclass('5')=1\nclass('a')=2\nclass('Z')=4\nclass(' ')=3\nclass('E')=2\nclass('#')=4\nclass('0')=1\nclass('u')=2\nclass('x')=4\nclass('9')=1\nCumulative fallthrough:\nstart=1 total=15\nstart=2 total=14\nstart=3 total=12\nstart=4 total=9\nstart=5 total=5
int days_remaining(int month) {
    int days = 0;
    switch (month) {
        case 1: days = days + 31;
        case 2: days = days + 28;
        case 3: days = days + 31;
        case 4: days = days + 30;
        case 5: days = days + 31;
        case 6: days = days + 30;
        case 7: days = days + 31;
        case 8: days = days + 31;
        case 9: days = days + 30;
        case 10: days = days + 31;
        case 11: days = days + 30;
        case 12: days = days + 31;
    }
    return days;
}

int classify_char(char c) {
    int result = 0;
    switch (c) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            result = 1;
            break;
        case 'a': case 'e': case 'i': case 'o': case 'u':
        case 'A': case 'E': case 'I': case 'O': case 'U':
            result = 2;
            break;
        case ' ': case '\t': case '\n':
            result = 3;
            break;
        default:
            result = 4;
            break;
    }
    return result;
}

int main(void) {
    int i;
    printf("Days remaining from month:\n");
    for (i = 1; i <= 12; i++) {
        printf("month %d: %d days left\n", i, days_remaining(i));
    }

    printf("Character classification:\n");
    char chars[10];
    chars[0] = '5';
    chars[1] = 'a';
    chars[2] = 'Z';
    chars[3] = ' ';
    chars[4] = 'E';
    chars[5] = '#';
    chars[6] = '0';
    chars[7] = 'u';
    chars[8] = 'x';
    chars[9] = '9';
    for (i = 0; i < 10; i++) {
        printf("class('%c')=%d\n", chars[i], classify_char(chars[i]));
    }

    printf("Cumulative fallthrough:\n");
    int total;
    for (i = 1; i <= 5; i++) {
        total = 0;
        switch (i) {
            case 1: total = total + 1;
            case 2: total = total + 2;
            case 3: total = total + 3;
            case 4: total = total + 4;
            case 5: total = total + 5;
        }
        printf("start=%d total=%d\n", i, total);
    }

    return 0;
}
