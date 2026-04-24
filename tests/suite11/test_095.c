int printf(const char *fmt, ...);

// Test: Post/pre increment in complex expressions

int global_val;

int get_and_bump() {
    int old;
    old = global_val;
    global_val = global_val + 1;
    return old;
}

int main() {
    int a;
    int b;
    int result;

    // Basic post-increment
    a = 5;
    b = a++;
    printf("%d\n", b); // EXPECT: 5
    printf("%d\n", a); // EXPECT: 6

    // Basic pre-increment
    a = 5;
    b = ++a;
    printf("%d\n", b); // EXPECT: 6
    printf("%d\n", a); // EXPECT: 6

    // Basic post-decrement
    a = 5;
    b = a--;
    printf("%d\n", b); // EXPECT: 5
    printf("%d\n", a); // EXPECT: 4

    // Basic pre-decrement
    a = 5;
    b = --a;
    printf("%d\n", b); // EXPECT: 4
    printf("%d\n", a); // EXPECT: 4

    // Post-increment in addition
    a = 10;
    result = a++ + 5;
    printf("%d\n", result); // EXPECT: 15
    printf("%d\n", a);      // EXPECT: 11

    // Pre-increment in addition
    a = 10;
    result = ++a + 5;
    printf("%d\n", result); // EXPECT: 16
    printf("%d\n", a);      // EXPECT: 11

    // Post-increment in array indexing
    int arr[5];
    arr[0] = 100;
    arr[1] = 200;
    arr[2] = 300;
    arr[3] = 400;
    arr[4] = 500;
    int idx;
    idx = 0;
    result = arr[idx++];
    printf("%d\n", result); // EXPECT: 100
    printf("%d\n", idx);    // EXPECT: 1

    result = arr[idx++];
    printf("%d\n", result); // EXPECT: 200
    printf("%d\n", idx);    // EXPECT: 2

    // Pre-increment in array indexing
    idx = 0;
    result = arr[++idx];
    printf("%d\n", result); // EXPECT: 200
    printf("%d\n", idx);    // EXPECT: 1

    // Post-increment in loop
    a = 0;
    int sum;
    sum = 0;
    while (a < 5) {
        sum = sum + a++;
    }
    printf("%d\n", sum); // EXPECT: 10
    printf("%d\n", a);   // EXPECT: 5

    // Pre-increment in conditional
    a = 5;
    if (++a > 5) {
        printf("%d\n", 1); // EXPECT: 1
    }
    printf("%d\n", a); // EXPECT: 6

    // Post-increment in conditional
    a = 5;
    if (a++ > 5) {
        printf("%d\n", 1);
    } else {
        printf("%d\n", 0); // EXPECT: 0
    }
    printf("%d\n", a); // EXPECT: 6

    // Increment on pointer
    int *p;
    p = arr;
    result = *p++;
    printf("%d\n", result); // EXPECT: 100
    result = *p;
    printf("%d\n", result); // EXPECT: 200

    p = arr;
    result = *++p;
    printf("%d\n", result); // EXPECT: 200

    // Post-decrement in loop
    a = 5;
    sum = 0;
    while (a > 0) {
        sum = sum + a--;
    }
    printf("%d\n", sum); // EXPECT: 15
    printf("%d\n", a);   // EXPECT: 0

    // Increment used to walk through array
    p = arr;
    sum = 0;
    int i;
    for (i = 0; i < 5; i++) {
        sum = sum + *p++;
    }
    printf("%d\n", sum); // EXPECT: 1500

    // Multiple increments in sequence (separate statements)
    a = 0;
    a++;
    a++;
    a++;
    printf("%d\n", a); // EXPECT: 3

    // Pre-decrement chain
    a = 10;
    --a;
    --a;
    --a;
    printf("%d\n", a); // EXPECT: 7

    // Post-increment in for loop
    sum = 0;
    for (i = 0; i < 10; i++) {
        sum = sum + i;
    }
    printf("%d\n", sum); // EXPECT: 45

    // Increment with ternary
    a = 3;
    result = (a++ > 3) ? 1 : 0;
    printf("%d\n", result); // EXPECT: 0
    printf("%d\n", a);      // EXPECT: 4

    a = 3;
    result = (++a > 3) ? 1 : 0;
    printf("%d\n", result); // EXPECT: 1
    printf("%d\n", a);      // EXPECT: 4

    return 0;
}
