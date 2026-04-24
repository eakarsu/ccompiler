int printf(const char *fmt, ...);
// EXPECT: forward: 0 3 6 9 12 15 18 21 \nbackward: 21 18 15 12 9 6 3 0 \ndiff = 5\nstep2: 0 6 12 18 \nchars: abcde\nlo < hi\nsum = 84\nmodified: 0 3 60 9 120 15 180 21 
// Test: pointer arithmetic - increment, decrement, difference

int main(void) {
    int arr[8];
    int i;
    for (i = 0; i < 8; i++) {
        arr[i] = i * 3;
    }

    // Forward traversal with pointer increment
    int *p = arr;
    printf("forward: ");
    for (i = 0; i < 8; i++) {
        printf("%d ", *p);
        p = p + 1;
    }
    printf("\n");

    // Backward traversal with pointer decrement
    p = &arr[7];
    printf("backward: ");
    for (i = 0; i < 8; i++) {
        printf("%d ", *p);
        p = p - 1;
    }
    printf("\n");

    // Pointer difference
    int *first = &arr[1];
    int *last = &arr[6];
    printf("diff = %d\n", (int)(last - first));

    // Stepping by 2
    p = arr;
    printf("step2: ");
    for (i = 0; i < 4; i++) {
        printf("%d ", *p);
        p = p + 2;
    }
    printf("\n");

    // Char pointer arithmetic
    char word[6];
    word[0] = 'a';
    word[1] = 'b';
    word[2] = 'c';
    word[3] = 'd';
    word[4] = 'e';
    word[5] = '\0';

    char *cp = word;
    printf("chars: ");
    while (*cp) {
        printf("%c", *cp);
        cp = cp + 1;
    }
    printf("\n");

    // Pointer comparison
    int *lo = &arr[2];
    int *hi = &arr[5];
    if (lo < hi) {
        printf("lo < hi\n");
    }

    // Sum using pointer walk
    int sum = 0;
    p = arr;
    int *pend = arr + 8;
    while (p < pend) {
        sum = sum + *p;
        p = p + 1;
    }
    printf("sum = %d\n", sum);

    // Modify every other element
    p = arr;
    for (i = 0; i < 4; i++) {
        *p = *p * 10;
        p = p + 2;
    }
    printf("modified: ");
    for (i = 0; i < 8; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
