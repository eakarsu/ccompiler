int printf(const char *fmt, ...);
// EXPECT: 20+7 = 27\n20-7 = 13\n20*7 = 140\n20/7 = 2\n20%7 = 6\ninvalid = -1\ndays: 31 28 31 30 31 30 31 31 30 31 30 31 \nyear days = 365\ntypes: 2 3 1 4 0 2 1 3 1 4 \nZABBAZABBA\n1 2 3 10 20 30 100 200 300 
// Test: switch statements with arithmetic

int compute_op(int a, int b, int op) {
    int result;
    result = 0;
    switch (op) {
        case 0: result = a + b; break;
        case 1: result = a - b; break;
        case 2: result = a * b; break;
        case 3: result = (b != 0) ? a / b : 0; break;
        case 4: result = (b != 0) ? a % b : 0; break;
        default: result = -1; break;
    }
    return result;
}

int days_in_month(int month) {
    int days;
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            days = 31; break;
        case 4: case 6: case 9: case 11:
            days = 30; break;
        case 2:
            days = 28; break;
        default:
            days = 0; break;
    }
    return days;
}

int char_type(char c) {
    /* 0=other, 1=digit, 2=upper, 3=lower, 4=space */
    switch (c) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return 1;
        case ' ': case '\t': case '\n':
            return 4;
        default:
            if (c >= 'A' && c <= 'Z') return 2;
            if (c >= 'a' && c <= 'z') return 3;
            return 0;
    }
}

int main(void) {
    int i;
    int a, b;

    a = 20;
    b = 7;

    /* Calculator operations */
    printf("20+7 = %d\n", compute_op(a, b, 0));
    printf("20-7 = %d\n", compute_op(a, b, 1));
    printf("20*7 = %d\n", compute_op(a, b, 2));
    printf("20/7 = %d\n", compute_op(a, b, 3));
    printf("20%%7 = %d\n", compute_op(a, b, 4));
    printf("invalid = %d\n", compute_op(a, b, 9));

    /* Days in each month */
    printf("days: ");
    for (i = 1; i <= 12; i++) {
        printf("%d ", days_in_month(i));
    }
    printf("\n");

    /* Total days in year */
    {
        int total;
        total = 0;
        for (i = 1; i <= 12; i++) {
            total += days_in_month(i);
        }
        printf("year days = %d\n", total);
    }

    /* Character classification */
    printf("types: ");
    {
        char test[10];
        test[0] = 'A'; test[1] = 'z'; test[2] = '5';
        test[3] = ' '; test[4] = '@'; test[5] = 'M';
        test[6] = '0'; test[7] = 'b'; test[8] = '9';
        test[9] = '\t';
        for (i = 0; i < 10; i++) {
            printf("%d ", char_type(test[i]));
        }
    }
    printf("\n");

    /* Switch with computed values */
    for (i = 0; i < 10; i++) {
        int v;
        v = i * i % 5;
        switch (v) {
            case 0: printf("Z"); break;
            case 1: printf("A"); break;
            case 4: printf("B"); break;
            default: printf("?"); break;
        }
    }
    printf("\n");

    /* Nested switch */
    for (i = 1; i <= 3; i++) {
        int j;
        for (j = 1; j <= 3; j++) {
            switch (i) {
                case 1: printf("%d ", j * 1); break;
                case 2: printf("%d ", j * 10); break;
                case 3: printf("%d ", j * 100); break;
            }
        }
    }
    printf("\n");

    return 0;
}
