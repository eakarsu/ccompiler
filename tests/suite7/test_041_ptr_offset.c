int printf(const char *fmt, ...);

int main(void) {
    int arr[10];
    int i;
    for (i = 0; i < 10; i++) {
        arr[i] = (i + 1) * 10;
    }

    int *p = arr;

    // EXPECT: arr[0] = 10
    printf("arr[0] = %d\n", *p);

    // EXPECT: arr[3] = 40
    printf("arr[3] = %d\n", *(p + 3));

    // EXPECT: arr[7] = 80
    printf("arr[7] = %d\n", *(p + 7));

    // EXPECT: arr[9] = 100
    printf("arr[9] = %d\n", *(p + 9));

    int *q = p + 5;
    // EXPECT: val at p+5 = 60
    printf("val at p+5 = %d\n", *q);

    // EXPECT: q-1 = 50
    printf("q-1 = %d\n", *(q - 1));

    // EXPECT: q-3 = 30
    printf("q-3 = %d\n", *(q - 3));

    // EXPECT: distance q-p = 5
    printf("distance q-p = %d\n", (int)(q - p));

    int *r = p + 9;
    // EXPECT: distance r-p = 9
    printf("distance r-p = %d\n", (int)(r - p));

    // EXPECT: distance r-q = 4
    printf("distance r-q = %d\n", (int)(r - q));

    char str[12];
    str[0] = 'H'; str[1] = 'e'; str[2] = 'l'; str[3] = 'l'; str[4] = 'o';
    str[5] = ' '; str[6] = 'W'; str[7] = 'o'; str[8] = 'r'; str[9] = 'l';
    str[10] = 'd'; str[11] = 0;

    char *cp = str;
    // EXPECT: char[0] = 72
    printf("char[0] = %d\n", *cp);

    // EXPECT: char[6] = 87
    printf("char[6] = %d\n", *(cp + 6));

    char *cp2 = cp + 10;
    // EXPECT: char distance = 10
    printf("char distance = %d\n", (int)(cp2 - cp));

    // EXPECT: char at cp2 = 100
    printf("char at cp2 = %d\n", *cp2);

    int nums[5];
    nums[0] = 100; nums[1] = 200; nums[2] = 300; nums[3] = 400; nums[4] = 500;
    int *start = nums;
    int *end = nums + 4;

    // EXPECT: start = 100
    printf("start = %d\n", *start);

    // EXPECT: end = 500
    printf("end = %d\n", *end);

    // EXPECT: span = 4
    printf("span = %d\n", (int)(end - start));

    int *mid = start + 2;
    // EXPECT: mid = 300
    printf("mid = %d\n", *mid);

    // EXPECT: mid-start = 2
    printf("mid-start = %d\n", (int)(mid - start));

    // EXPECT: end-mid = 2
    printf("end-mid = %d\n", (int)(end - mid));

    p = arr;
    int sum = 0;
    for (i = 0; i < 10; i++) {
        sum = sum + *(p + i);
    }
    // EXPECT: sum = 550
    printf("sum = %d\n", sum);

    p = arr + 9;
    int rsum = 0;
    for (i = 0; i < 10; i++) {
        rsum = rsum + *(p - i);
    }
    // EXPECT: rsum = 550
    printf("rsum = %d\n", rsum);

    int data[4];
    data[0] = 1; data[1] = 2; data[2] = 3; data[3] = 4;
    int *d1 = data;
    int *d2 = data + 1;
    int *d3 = data + 2;
    int *d4 = data + 3;

    // EXPECT: diffs = 1 1 1
    printf("diffs = %d %d %d\n", (int)(d2 - d1), (int)(d3 - d2), (int)(d4 - d3));

    // EXPECT: d4-d1 = 3
    printf("d4-d1 = %d\n", (int)(d4 - d1));

    return 0;
}
