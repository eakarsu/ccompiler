int printf(const char *fmt, ...);
// EXPECT: arr[0]=5 *p=5\narr[0]=5 *(p+0)=5\narr[1]=10 *(p+1)=10\narr[2]=15 *(p+2)=15\narr[3]=20 *(p+3)=20\narr[4]=25 *(p+4)=25\narr[5]=30 *(p+5)=30\np[3]=20\narr[2] after *(p+2)=99: 99\nend - start = 5\nstr = Hello\nmsg = World\narr == &arr[0]
// Test: array and pointer equivalence

int main(void) {
    int arr[6];
    int i;
    for (i = 0; i < 6; i++) {
        arr[i] = (i + 1) * 5;
    }

    // Array name decays to pointer
    int *p = arr;
    printf("arr[0]=%d *p=%d\n", arr[0], *p);

    // Indexing vs pointer arithmetic
    for (i = 0; i < 6; i++) {
        printf("arr[%d]=%d *(p+%d)=%d\n", i, arr[i], i, *(p + i));
    }

    // Pointer indexing
    printf("p[3]=%d\n", p[3]);

    // Modify through pointer arithmetic
    *(p + 2) = 99;
    printf("arr[2] after *(p+2)=99: %d\n", arr[2]);

    // Pointer subtraction
    int *start = &arr[0];
    int *end = &arr[5];
    printf("end - start = %d\n", (int)(end - start));

    // Char array equivalence
    char str[10];
    str[0] = 'H';
    str[1] = 'e';
    str[2] = 'l';
    str[3] = 'l';
    str[4] = 'o';
    str[5] = '\0';

    char *cp = str;
    printf("str = ");
    while (*cp) {
        printf("%c", *cp);
        cp = cp + 1;
    }
    printf("\n");

    // Array of chars via pointer
    char msg[20];
    char *wp = msg;
    *wp = 'W'; wp = wp + 1;
    *wp = 'o'; wp = wp + 1;
    *wp = 'r'; wp = wp + 1;
    *wp = 'l'; wp = wp + 1;
    *wp = 'd'; wp = wp + 1;
    *wp = '\0';
    printf("msg = %s\n", msg);

    // Equivalence of &arr[0] and arr
    if (arr == &arr[0]) {
        printf("arr == &arr[0]\n");
    }

    return 0;
}
