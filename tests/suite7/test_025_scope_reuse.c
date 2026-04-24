int printf(const char *fmt, ...);

void test_sequential_blocks(void) {
    {
        int x = 10;
        int y = 20;
        // EXPECT: block1 x=10 y=20
        printf("block1 x=%d y=%d\n", x, y);
    }
    {
        int x = 30;
        int y = 40;
        // EXPECT: block2 x=30 y=40
        printf("block2 x=%d y=%d\n", x, y);
    }
    {
        int x = 50;
        int y = 60;
        // EXPECT: block3 x=50 y=60
        printf("block3 x=%d y=%d\n", x, y);
    }
}

void test_reuse_different_types(void) {
    {
        int val = 100;
        // EXPECT: int val = 100
        printf("int val = %d\n", val);
    }
    {
        char val = 65;
        // EXPECT: char val = 65
        printf("char val = %d\n", val);
    }
    {
        int val = 300;
        // EXPECT: next int val = 300
        printf("next int val = %d\n", val);
    }
}

void test_reuse_arrays(void) {
    {
        int arr[3];
        arr[0] = 1; arr[1] = 2; arr[2] = 3;
        // EXPECT: arr1: 1 2 3
        printf("arr1: %d %d %d\n", arr[0], arr[1], arr[2]);
    }
    {
        int arr[3];
        arr[0] = 10; arr[1] = 20; arr[2] = 30;
        // EXPECT: arr2: 10 20 30
        printf("arr2: %d %d %d\n", arr[0], arr[1], arr[2]);
    }
    {
        int arr[3];
        arr[0] = 100; arr[1] = 200; arr[2] = 300;
        // EXPECT: arr3: 100 200 300
        printf("arr3: %d %d %d\n", arr[0], arr[1], arr[2]);
    }
}

void test_reuse_in_loop(void) {
    int i;
    int sum = 0;
    for (i = 0; i < 5; i = i + 1) {
        {
            int temp = i * 7;
            sum = sum + temp;
        }
    }
    // EXPECT: loop reuse sum = 70
    printf("loop reuse sum = %d\n", sum);
}

void test_many_sequential(void) {
    int total = 0;
    { int x = 1; total = total + x; }
    { int x = 2; total = total + x; }
    { int x = 3; total = total + x; }
    { int x = 4; total = total + x; }
    { int x = 5; total = total + x; }
    { int x = 6; total = total + x; }
    { int x = 7; total = total + x; }
    { int x = 8; total = total + x; }
    { int x = 9; total = total + x; }
    { int x = 10; total = total + x; }
    // EXPECT: many sequential total = 55
    printf("many sequential total = %d\n", total);
}

void test_reuse_with_outer(void) {
    int x = 999;
    {
        int y = 1;
        // EXPECT: outer x with y: 999 1
        printf("outer x with y: %d %d\n", x, y);
    }
    {
        int y = 2;
        // EXPECT: outer x with y: 999 2
        printf("outer x with y: %d %d\n", x, y);
    }
    {
        int y = 3;
        // EXPECT: outer x with y: 999 3
        printf("outer x with y: %d %d\n", x, y);
    }
    // EXPECT: x still 999
    printf("x still %d\n", x);
}

int main(void) {
    test_sequential_blocks();
    test_reuse_different_types();
    test_reuse_arrays();
    test_reuse_in_loop();
    test_many_sequential();
    test_reuse_with_outer();
    return 0;
}
