int printf(const char *fmt, ...);
// EXPECT: Unrolled sum (4x):\nsum=210\nUnrolled max (2x):\nmax=20\nUnrolled copy (4x):\n2 4 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 \nDuff-style remainder:\nsum of first 17 = 153\nUnrolled dot product (2x):\ndot=1540
int main(void) {
    int arr[20];
    int i;

    for (i = 0; i < 20; i++) {
        arr[i] = i + 1;
    }

    printf("Unrolled sum (4x):\n");
    int sum = 0;
    i = 0;
    while (i + 3 < 20) {
        sum = sum + arr[i] + arr[i + 1] + arr[i + 2] + arr[i + 3];
        i = i + 4;
    }
    while (i < 20) {
        sum = sum + arr[i];
        i++;
    }
    printf("sum=%d\n", sum);

    printf("Unrolled max (2x):\n");
    int max_val = arr[0];
    i = 1;
    while (i + 1 < 20) {
        if (arr[i] > max_val) max_val = arr[i];
        if (arr[i + 1] > max_val) max_val = arr[i + 1];
        i = i + 2;
    }
    if (i < 20 && arr[i] > max_val) max_val = arr[i];
    printf("max=%d\n", max_val);

    printf("Unrolled copy (4x):\n");
    int dest[20];
    i = 0;
    while (i + 3 < 20) {
        dest[i] = arr[i] * 2;
        dest[i + 1] = arr[i + 1] * 2;
        dest[i + 2] = arr[i + 2] * 2;
        dest[i + 3] = arr[i + 3] * 2;
        i = i + 4;
    }
    while (i < 20) {
        dest[i] = arr[i] * 2;
        i++;
    }
    for (i = 0; i < 20; i++) {
        printf("%d ", dest[i]);
    }
    printf("\n");

    printf("Duff-style remainder:\n");
    int count = 17;
    sum = 0;
    int remaining = count % 4;
    i = 0;
    while (remaining > 0) {
        sum = sum + arr[i];
        i++;
        remaining--;
    }
    while (i < count) {
        sum = sum + arr[i] + arr[i + 1] + arr[i + 2] + arr[i + 3];
        i = i + 4;
    }
    printf("sum of first %d = %d\n", count, sum);

    printf("Unrolled dot product (2x):\n");
    int b[20];
    for (i = 0; i < 20; i++) {
        b[i] = 20 - i;
    }
    int dot = 0;
    i = 0;
    while (i + 1 < 20) {
        dot = dot + arr[i] * b[i] + arr[i + 1] * b[i + 1];
        i = i + 2;
    }
    if (i < 20) dot = dot + arr[i] * b[i];
    printf("dot=%d\n", dot);

    return 0;
}
