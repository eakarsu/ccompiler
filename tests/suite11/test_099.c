int printf(const char *fmt, ...);

// Test: Deeply nested ternary expressions (8+ levels)

int clamp(int val, int lo, int hi) {
    return (val < lo) ? lo : ((val > hi) ? hi : val);
}

int main() {
    int x;
    int result;

    // 8-level nested ternary: classify a number 0-7
    x = 0;
    result = (x == 0) ? 10
           : (x == 1) ? 20
           : (x == 2) ? 30
           : (x == 3) ? 40
           : (x == 4) ? 50
           : (x == 5) ? 60
           : (x == 6) ? 70
           : (x == 7) ? 80
           : -1;
    printf("%d\n", result); // EXPECT: 10

    x = 4;
    result = (x == 0) ? 10
           : (x == 1) ? 20
           : (x == 2) ? 30
           : (x == 3) ? 40
           : (x == 4) ? 50
           : (x == 5) ? 60
           : (x == 6) ? 70
           : (x == 7) ? 80
           : -1;
    printf("%d\n", result); // EXPECT: 50

    x = 7;
    result = (x == 0) ? 10
           : (x == 1) ? 20
           : (x == 2) ? 30
           : (x == 3) ? 40
           : (x == 4) ? 50
           : (x == 5) ? 60
           : (x == 6) ? 70
           : (x == 7) ? 80
           : -1;
    printf("%d\n", result); // EXPECT: 80

    x = 9;
    result = (x == 0) ? 10
           : (x == 1) ? 20
           : (x == 2) ? 30
           : (x == 3) ? 40
           : (x == 4) ? 50
           : (x == 5) ? 60
           : (x == 6) ? 70
           : (x == 7) ? 80
           : -1;
    printf("%d\n", result); // EXPECT: -1

    // 10-level nested ternary: range classification
    x = 5;
    result = (x < 0)   ? -1
           : (x < 10)  ? 0
           : (x < 20)  ? 1
           : (x < 30)  ? 2
           : (x < 40)  ? 3
           : (x < 50)  ? 4
           : (x < 60)  ? 5
           : (x < 70)  ? 6
           : (x < 80)  ? 7
           : (x < 90)  ? 8
           : 9;
    printf("%d\n", result); // EXPECT: 0

    x = 55;
    result = (x < 0)   ? -1
           : (x < 10)  ? 0
           : (x < 20)  ? 1
           : (x < 30)  ? 2
           : (x < 40)  ? 3
           : (x < 50)  ? 4
           : (x < 60)  ? 5
           : (x < 70)  ? 6
           : (x < 80)  ? 7
           : (x < 90)  ? 8
           : 9;
    printf("%d\n", result); // EXPECT: 5

    x = 95;
    result = (x < 0)   ? -1
           : (x < 10)  ? 0
           : (x < 20)  ? 1
           : (x < 30)  ? 2
           : (x < 40)  ? 3
           : (x < 50)  ? 4
           : (x < 60)  ? 5
           : (x < 70)  ? 6
           : (x < 80)  ? 7
           : (x < 90)  ? 8
           : 9;
    printf("%d\n", result); // EXPECT: 9

    // Nested ternary with expressions in both branches
    int a;
    int b;
    a = 3;
    b = 7;
    result = (a > b) ? (a - b)
           : (a == b) ? 0
           : (b - a > 5) ? 100
           : (b - a > 3) ? 50
           : (b - a > 1) ? 25
           : (b - a > 0) ? 10
           : (b - a == 0) ? 5
           : (a + b > 10) ? 200
           : -1;
    printf("%d\n", result); // EXPECT: 50

    // Ternary in loop
    int i;
    int sum;
    sum = 0;
    for (i = 0; i < 10; i = i + 1) {
        int cat;
        cat = (i < 2) ? 1
            : (i < 4) ? 2
            : (i < 6) ? 3
            : (i < 8) ? 4
            : (i < 9) ? 5
            : (i < 10) ? 6
            : (i < 11) ? 7
            : (i < 12) ? 8
            : 9;
        sum = sum + cat;
    }
    // i=0:1, i=1:1, i=2:2, i=3:2, i=4:3, i=5:3, i=6:4, i=7:4, i=8:5, i=9:6
    // sum = 1+1+2+2+3+3+4+4+5+6 = 31
    printf("%d\n", sum); // EXPECT: 31

    // Nested ternary as function argument
    x = 42;
    printf("%d\n",
        (x > 100) ? 4
      : (x > 50)  ? 3
      : (x > 25)  ? 2
      : (x > 10)  ? 1
      : (x > 5)   ? -1
      : (x > 2)   ? -2
      : (x > 0)   ? -3
      : (x == 0)  ? -4
      : -5); // EXPECT: 2

    // Clamp function uses ternary
    printf("%d\n", clamp(-5, 0, 100));  // EXPECT: 0
    printf("%d\n", clamp(50, 0, 100));  // EXPECT: 50
    printf("%d\n", clamp(200, 0, 100)); // EXPECT: 100

    // Ternary with arithmetic in conditions
    a = 15;
    result = (a * 2 > 50)     ? 1
           : (a * 2 > 40)     ? 2
           : (a * 2 > 30)     ? 3
           : (a * 2 > 20)     ? 4
           : (a + 10 > 30)    ? 5
           : (a + 10 > 20)    ? 6
           : (a - 5 > 5)      ? 7
           : (a - 5 > 0)      ? 8
           : 9;
    printf("%d\n", result); // EXPECT: 4

    // Deep ternary selecting array index
    int arr[5];
    arr[0] = 111;
    arr[1] = 222;
    arr[2] = 333;
    arr[3] = 444;
    arr[4] = 555;
    x = 3;
    int idx;
    idx = (x == 0) ? 0
        : (x == 1) ? 1
        : (x == 2) ? 2
        : (x == 3) ? 3
        : (x == 4) ? 4
        : (x == 5) ? 0
        : (x == 6) ? 1
        : (x == 7) ? 2
        : 0;
    printf("%d\n", arr[idx]); // EXPECT: 444

    return 0;
}
