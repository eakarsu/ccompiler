int printf(const char *fmt, ...);
// EXPECT: *p = 10\n*(p+1) = 20\n*(p+3) = 40\n10 20 30 40 50 60 70 80 90 100 \nptr diff = 5\nbackwards: 100 90 80 70 60 \nsum = 550\nmodified: arr[0]=999 arr[9]=111\nstr: Hello\nlen = 5
// Test: pointer arithmetic basics

int main(void) {
    int arr[10];
    int *p;
    int *q;
    int i;
    int sum;
    char str[20];
    char *cp;

    /* Fill array */
    for (i = 0; i < 10; i++) {
        arr[i] = (i + 1) * 10;
    }

    /* Pointer to beginning */
    p = arr;
    printf("*p = %d\n", *p);

    /* Pointer increment */
    p = p + 1;
    printf("*(p+1) = %d\n", *p);

    p = p + 2;
    printf("*(p+3) = %d\n", *p);

    /* Array access via pointer */
    p = arr;
    for (i = 0; i < 10; i++) {
        printf("%d ", *(p + i));
    }
    printf("\n");

    /* Pointer subtraction */
    p = &arr[2];
    q = &arr[7];
    printf("ptr diff = %d\n", (int)(q - p));

    /* Walk backwards */
    p = &arr[9];
    printf("backwards: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", *(p - i));
    }
    printf("\n");

    /* Sum via pointer iteration */
    sum = 0;
    p = arr;
    for (i = 0; i < 10; i++) {
        sum += *p;
        p++;
    }
    printf("sum = %d\n", sum);

    /* Pointer to modify array */
    p = arr;
    *(p + 0) = 999;
    *(p + 9) = 111;
    printf("modified: arr[0]=%d arr[9]=%d\n", arr[0], arr[9]);

    /* Char pointer arithmetic */
    str[0] = 'H'; str[1] = 'e'; str[2] = 'l';
    str[3] = 'l'; str[4] = 'o'; str[5] = '\0';
    cp = str;
    printf("str: ");
    while (*cp != '\0') {
        printf("%c", *cp);
        cp++;
    }
    printf("\n");

    /* String length via pointer */
    cp = str;
    i = 0;
    while (*cp != '\0') {
        i++;
        cp++;
    }
    printf("len = %d\n", i);

    return 0;
}
