int printf(const char *fmt, ...);
// EXPECT: 10/3 = 3\n10/5 = 2\n1/2 = 0\n7/2 = 3\n-7/2 = -3\n7/-2 = -3\n-7/-2 = 3\n42/1 = 42\n42/-1 = -42\n-42/1 = -42\n-42/-1 = 42\n0/5 = 0\n0/-5 = 0\n1000/2 = 500\n/2 = 250\n/2 = 125\n/2 = 62\n/2 = 31\navg = 30\n1000000/1000 = 1000\n999999/1000 = 999\n720!/div = 1
// Test: integer division edge cases

int main(void) {
    int a, b;
    int r;

    /* Basic division */
    printf("10/3 = %d\n", 10 / 3);
    printf("10/5 = %d\n", 10 / 5);
    printf("1/2 = %d\n", 1 / 2);

    /* Truncation toward zero */
    printf("7/2 = %d\n", 7 / 2);
    printf("-7/2 = %d\n", (-7) / 2);
    printf("7/-2 = %d\n", 7 / (-2));
    printf("-7/-2 = %d\n", (-7) / (-2));

    /* Division by 1 and -1 */
    printf("42/1 = %d\n", 42 / 1);
    printf("42/-1 = %d\n", 42 / (-1));
    printf("-42/1 = %d\n", (-42) / 1);
    printf("-42/-1 = %d\n", (-42) / (-1));

    /* Zero dividend */
    printf("0/5 = %d\n", 0 / 5);
    printf("0/-5 = %d\n", 0 / (-5));

    /* Successive divisions */
    a = 1000;
    a = a / 2;
    printf("1000/2 = %d\n", a);
    a = a / 2;
    printf("/2 = %d\n", a);
    a = a / 2;
    printf("/2 = %d\n", a);
    a = a / 2;
    printf("/2 = %d\n", a);
    a = a / 2;
    printf("/2 = %d\n", a);

    /* Division as part of average */
    {
        int arr[5];
        int sum, i;
        arr[0] = 10;
        arr[1] = 20;
        arr[2] = 30;
        arr[3] = 40;
        arr[4] = 50;
        sum = 0;
        for (i = 0; i < 5; i = i + 1) {
            sum = sum + arr[i];
        }
        printf("avg = %d\n", sum / 5);
    }

    /* Large number divisions */
    printf("1000000/1000 = %d\n", 1000000 / 1000);
    printf("999999/1000 = %d\n", 999999 / 1000);

    /* Chain of divisions */
    r = 720 / 6 / 5 / 4 / 3 / 2;
    printf("720!/div = %d\n", r);

    return 0;
}
