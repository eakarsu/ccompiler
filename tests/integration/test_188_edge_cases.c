int printf(const char *fmt, ...);
// EXPECT: Empty loop tests:\nempty for sum=0\nempty while sum=0\nSingle iteration:\nfor once: 0\nwhile once: 0\ndo-while once: 0\nZero-trip for with side effects:\nx=42 (should be 42)\nMax iterations tracking:\nmax_i=99\nNested empty:\ncount=0 (should be 0)\nIf-else chain edge:\nmedium\nSwitch with no match:\nsw_result=-1\nTernary chains:\ni=0 grade=0\ni=1 grade=1\ni=2 grade=2\ni=3 grade=2\ni=4 grade=3\ni=5 grade=3\nBoolean arithmetic:\n(a>b)+(a>0)+(b>0)=3\n(a==b)+(a!=b)=1
int main(void) {
    printf("Empty loop tests:\n");
    int i;
    int sum = 0;
    for (i = 10; i < 5; i++) {
        sum = sum + i;
    }
    printf("empty for sum=%d\n", sum);

    i = 10;
    while (i < 5) {
        sum = sum + i;
        i++;
    }
    printf("empty while sum=%d\n", sum);

    printf("Single iteration:\n");
    for (i = 0; i < 1; i++) {
        printf("for once: %d\n", i);
    }
    i = 0;
    while (i < 1) {
        printf("while once: %d\n", i);
        i++;
    }
    i = 0;
    do {
        printf("do-while once: %d\n", i);
        i++;
    } while (i < 1);

    printf("Zero-trip for with side effects:\n");
    int x = 42;
    for (i = 0; i < 0; i++) {
        x = 99;
    }
    printf("x=%d (should be 42)\n", x);

    printf("Max iterations tracking:\n");
    int max_i = 0;
    for (i = 0; i < 100; i++) {
        max_i = i;
    }
    printf("max_i=%d\n", max_i);

    printf("Nested empty:\n");
    int j;
    int count = 0;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 0; j++) {
            count++;
        }
    }
    printf("count=%d (should be 0)\n", count);

    printf("If-else chain edge:\n");
    int val = 50;
    if (val < 0) {
        printf("negative\n");
    } else if (val == 0) {
        printf("zero\n");
    } else if (val < 10) {
        printf("small\n");
    } else if (val < 100) {
        printf("medium\n");
    } else {
        printf("large\n");
    }

    printf("Switch with no match:\n");
    int sw_result = -1;
    switch (99) {
        case 0: sw_result = 0; break;
        case 1: sw_result = 1; break;
        case 2: sw_result = 2; break;
    }
    printf("sw_result=%d\n", sw_result);

    printf("Ternary chains:\n");
    for (i = 0; i < 6; i++) {
        int grade = (i < 1) ? 0 : (i < 2) ? 1 : (i < 4) ? 2 : 3;
        printf("i=%d grade=%d\n", i, grade);
    }

    printf("Boolean arithmetic:\n");
    int a = 5;
    int b = 3;
    printf("(a>b)+(a>0)+(b>0)=%d\n", (a > b) + (a > 0) + (b > 0));
    printf("(a==b)+(a!=b)=%d\n", (a == b) + (a != b));

    return 0;
}
