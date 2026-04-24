int printf(const char *fmt, ...);
// EXPECT: Abs values: -5->5 -1->1 0->0 1->1 5->5 -100->100 42->42 \nMax/Min pairs:\nmax(-5,-1)=-1 min(-5,-1)=-5\nmax(-1,0)=0 min(-1,0)=-1\nmax(0,1)=1 min(0,1)=0\nmax(1,5)=5 min(1,5)=1\nmax(5,-100)=5 min(5,-100)=-100\nmax(-100,42)=42 min(-100,42)=-100\nMax of three:\nmax3(3,7,5)=7\nmax3(10,2,8)=10\nmax3(1,1,1)=1\nSign function: -5->-1 -1->-1 0->0 1->1 5->1 -100->-1 42->1 \nNested ternary classification:\n0:0 1:0 2:0 3:0 4:0 5:1 6:1 7:1 8:1 9:1 10:2 11:2 12:2 13:2 14:2 15:3 16:3 17:3 18:3 19:3 20:3 \nConditional accumulation:\npositive sum=21 negative sum=-18\nTernary in array index:\n100 200 300 400 400 300 200 100 
int abs_val(int x) {
    return x >= 0 ? x : -x;
}

int max2(int a, int b) {
    return a > b ? a : b;
}

int min2(int a, int b) {
    return a < b ? a : b;
}

int max3(int a, int b, int c) {
    return max2(max2(a, b), c);
}

int sign(int x) {
    return x > 0 ? 1 : (x < 0 ? -1 : 0);
}

int main(void) {
    int i;
    printf("Abs values: ");
    int vals[7];
    vals[0] = -5; vals[1] = -1; vals[2] = 0;
    vals[3] = 1; vals[4] = 5; vals[5] = -100; vals[6] = 42;
    for (i = 0; i < 7; i++) {
        printf("%d->%d ", vals[i], abs_val(vals[i]));
    }
    printf("\n");

    printf("Max/Min pairs:\n");
    for (i = 0; i < 6; i++) {
        int a = vals[i];
        int b = vals[i + 1];
        printf("max(%d,%d)=%d min(%d,%d)=%d\n", a, b, max2(a, b), a, b, min2(a, b));
    }

    printf("Max of three:\n");
    printf("max3(3,7,5)=%d\n", max3(3, 7, 5));
    printf("max3(10,2,8)=%d\n", max3(10, 2, 8));
    printf("max3(1,1,1)=%d\n", max3(1, 1, 1));

    printf("Sign function: ");
    for (i = 0; i < 7; i++) {
        printf("%d->%d ", vals[i], sign(vals[i]));
    }
    printf("\n");

    printf("Nested ternary classification:\n");
    for (i = 0; i <= 20; i++) {
        int cat = i < 5 ? 0 : i < 10 ? 1 : i < 15 ? 2 : 3;
        printf("%d:%d ", i, cat);
    }
    printf("\n");

    printf("Conditional accumulation:\n");
    int sum_pos = 0;
    int sum_neg = 0;
    int data[10];
    data[0] = 3; data[1] = -2; data[2] = 7; data[3] = -5; data[4] = 1;
    data[5] = -8; data[6] = 4; data[7] = 0; data[8] = -3; data[9] = 6;
    for (i = 0; i < 10; i++) {
        sum_pos = sum_pos + (data[i] > 0 ? data[i] : 0);
        sum_neg = sum_neg + (data[i] < 0 ? data[i] : 0);
    }
    printf("positive sum=%d negative sum=%d\n", sum_pos, sum_neg);

    printf("Ternary in array index:\n");
    int arr[4];
    arr[0] = 100; arr[1] = 200; arr[2] = 300; arr[3] = 400;
    for (i = 0; i < 8; i++) {
        int idx = i < 4 ? i : 7 - i;
        printf("%d ", arr[idx]);
    }
    printf("\n");

    return 0;
}
