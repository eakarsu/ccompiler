int printf(const char *fmt, ...);
// EXPECT: Short-circuit AND:\nfalse&&?: result=0 calls=1\ntrue&&true: result=1 calls=2\ntrue&&false: result=0 calls=2\nShort-circuit OR:\ntrue||?: result=1 calls=1\nfalse||true: result=1 calls=2\nfalse||false: result=1 calls=2\nChained AND:\nT&&T&&F: result=0 calls=3\nF&&?&&?: result=0 calls=1\nChained OR:\nF||F||T: result=1 calls=3\nMixed AND/OR:\n(T||?)&&(T||?): result=1 calls=2\nNOT tests:\n!0=1 !1=0 !5=0 !-1=0\nGuard pattern:\narr[0]=10 qualifies\narr[1]=20 qualifies
int counter;

int inc_and_check(int val, int threshold) {
    counter++;
    return val > threshold;
}

int dec_and_check(int val, int threshold) {
    counter++;
    return val < threshold;
}

int main(void) {
    printf("Short-circuit AND:\n");
    counter = 0;
    int result = inc_and_check(5, 10) && inc_and_check(3, 1);
    printf("false&&?: result=%d calls=%d\n", result, counter);

    counter = 0;
    result = inc_and_check(15, 10) && inc_and_check(3, 1);
    printf("true&&true: result=%d calls=%d\n", result, counter);

    counter = 0;
    result = inc_and_check(15, 10) && inc_and_check(3, 5);
    printf("true&&false: result=%d calls=%d\n", result, counter);

    printf("Short-circuit OR:\n");
    counter = 0;
    result = inc_and_check(15, 10) || inc_and_check(3, 1);
    printf("true||?: result=%d calls=%d\n", result, counter);

    counter = 0;
    result = inc_and_check(5, 10) || inc_and_check(3, 1);
    printf("false||true: result=%d calls=%d\n", result, counter);

    counter = 0;
    result = inc_and_check(5, 10) || inc_and_check(5, 1);
    printf("false||false: result=%d calls=%d\n", result, counter);

    printf("Chained AND:\n");
    counter = 0;
    result = inc_and_check(20, 10) && inc_and_check(15, 10) && inc_and_check(5, 10);
    printf("T&&T&&F: result=%d calls=%d\n", result, counter);

    counter = 0;
    result = inc_and_check(5, 10) && inc_and_check(15, 10) && inc_and_check(20, 10);
    printf("F&&?&&?: result=%d calls=%d\n", result, counter);

    printf("Chained OR:\n");
    counter = 0;
    result = inc_and_check(5, 10) || inc_and_check(3, 10) || inc_and_check(20, 10);
    printf("F||F||T: result=%d calls=%d\n", result, counter);

    printf("Mixed AND/OR:\n");
    counter = 0;
    result = (inc_and_check(20, 10) || inc_and_check(5, 10)) && (inc_and_check(15, 10) || inc_and_check(3, 10));
    printf("(T||?)&&(T||?): result=%d calls=%d\n", result, counter);

    printf("NOT tests:\n");
    printf("!0=%d !1=%d !5=%d !-1=%d\n", !0, !1, !5, !(-1));

    printf("Guard pattern:\n");
    int arr[5];
    arr[0] = 10; arr[1] = 20; arr[2] = 0; arr[3] = 40; arr[4] = 50;
    int i;
    for (i = 0; i < 5; i++) {
        if (arr[i] != 0 && 100 / arr[i] > 3) {
            printf("arr[%d]=%d qualifies\n", i, arr[i]);
        }
    }

    return 0;
}
