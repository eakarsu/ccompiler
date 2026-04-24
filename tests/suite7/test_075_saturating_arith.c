int printf(const char *fmt, ...);

int sat_add(int a, int b) {
    if (b > 0 && a > 2147483647 - b) return 2147483647;
    if (b < 0 && a < (-2147483647 - 1) - b) return -2147483647 - 1;
    return a + b;
}

int sat_sub(int a, int b) {
    if (b < 0 && a > 2147483647 + b) return 2147483647;
    if (b > 0 && a < (-2147483647 - 1) + b) return -2147483647 - 1;
    return a - b;
}

int sat_mul(int a, int b) {
    if (a == 0 || b == 0) return 0;
    if (a > 0 && b > 0) {
        if (a > 2147483647 / b) return 2147483647;
    }
    if (a > 0 && b < 0) {
        if (b < (-2147483647 - 1) / a) return -2147483647 - 1;
    }
    if (a < 0 && b > 0) {
        if (a < (-2147483647 - 1) / b) return -2147483647 - 1;
    }
    if (a < 0 && b < 0) {
        if (a < 2147483647 / b) return 2147483647;
    }
    return a * b;
}

int sat_neg(int a) {
    if (a == (-2147483647 - 1)) return 2147483647;
    return -a;
}

int sat_abs(int a) {
    if (a == (-2147483647 - 1)) return 2147483647;
    if (a < 0) return -a;
    return a;
}

int clamp(int val, int lo, int hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

int sat_add_chain(int a, int b, int c, int d) {
    int r = sat_add(a, b);
    r = sat_add(r, c);
    r = sat_add(r, d);
    return r;
}

int sat_sub_chain(int a, int b, int c) {
    int r = sat_sub(a, b);
    r = sat_sub(r, c);
    return r;
}

int sat_accumulate(int arr[], int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        sum = sat_add(sum, arr[i]);
    }
    return sum;
}

int sat_dot_product(int a[], int b[], int n) {
    int sum = 0;
    int i;
    for (i = 0; i < n; i++) {
        int prod = sat_mul(a[i], b[i]);
        sum = sat_add(sum, prod);
    }
    return sum;
}

int main(void) {
    // EXPECT: sat_add(100,200)=300
    printf("sat_add(100,200)=%d\n", sat_add(100, 200));

    // EXPECT: sat_add(2147483647,1)=2147483647
    printf("sat_add(2147483647,1)=%d\n", sat_add(2147483647, 1));

    // EXPECT: sat_add(-2147483648,-1)=-2147483648
    printf("sat_add(-2147483648,-1)=%d\n", sat_add(-2147483647 - 1, -1));

    // EXPECT: sat_sub(100,200)=-100
    printf("sat_sub(100,200)=%d\n", sat_sub(100, 200));

    // EXPECT: sat_sub(-2147483648,1)=-2147483648
    printf("sat_sub(-2147483648,1)=%d\n", sat_sub(-2147483647 - 1, 1));

    // EXPECT: sat_sub(2147483647,-1)=2147483647
    printf("sat_sub(2147483647,-1)=%d\n", sat_sub(2147483647, -1));

    // EXPECT: sat_mul(100,200)=20000
    printf("sat_mul(100,200)=%d\n", sat_mul(100, 200));

    // EXPECT: sat_mul(2147483647,2)=2147483647
    printf("sat_mul(2147483647,2)=%d\n", sat_mul(2147483647, 2));

    // EXPECT: sat_mul(-2147483648,2)=-2147483648
    printf("sat_mul(-2147483648,2)=%d\n", sat_mul(-2147483647 - 1, 2));

    // EXPECT: sat_mul(2147483647,-1)=-2147483647
    printf("sat_mul(2147483647,-1)=%d\n", sat_mul(2147483647, -1));

    // EXPECT: sat_neg(-2147483648)=2147483647
    printf("sat_neg(-2147483648)=%d\n", sat_neg(-2147483647 - 1));

    // EXPECT: sat_neg(42)=-42
    printf("sat_neg(42)=%d\n", sat_neg(42));

    // EXPECT: sat_abs(-2147483648)=2147483647
    printf("sat_abs(-2147483648)=%d\n", sat_abs(-2147483647 - 1));

    // EXPECT: sat_abs(-42)=42
    printf("sat_abs(-42)=%d\n", sat_abs(-42));

    // EXPECT: clamp(50,0,100)=50
    printf("clamp(50,0,100)=%d\n", clamp(50, 0, 100));

    // EXPECT: clamp(-10,0,100)=0
    printf("clamp(-10,0,100)=%d\n", clamp(-10, 0, 100));

    // EXPECT: clamp(200,0,100)=100
    printf("clamp(200,0,100)=%d\n", clamp(200, 0, 100));

    // EXPECT: chain(2147483640,5,5,5)=2147483647
    printf("chain(2147483640,5,5,5)=%d\n", sat_add_chain(2147483640, 5, 5, 5));

    // EXPECT: sub_chain(-2147483640,5,5)=-2147483648
    printf("sub_chain(-2147483640,5,5)=%d\n", sat_sub_chain(-2147483640, 5, 5));

    int arr1[4];
    arr1[0] = 2147483647;
    arr1[1] = 100;
    arr1[2] = 200;
    arr1[3] = 300;
    // EXPECT: sat_accum=2147483647
    printf("sat_accum=%d\n", sat_accumulate(arr1, 4));

    int arr2[3];
    arr2[0] = 10;
    arr2[1] = 20;
    arr2[2] = 30;
    int arr3[3];
    arr3[0] = 1;
    arr3[1] = 2;
    arr3[2] = 3;
    // EXPECT: sat_dot=140
    printf("sat_dot=%d\n", sat_dot_product(arr2, arr3, 3));

    return 0;
}
