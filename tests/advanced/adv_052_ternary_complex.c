// EXPECT: === Complex Ternary Expressions ===
// EXPECT: basic_ternary: max(3,7)=7
// EXPECT: basic_ternary: max(10,2)=10
// EXPECT: nested_ternary: sign(-5)=-1
// EXPECT: nested_ternary: sign(0)=0
// EXPECT: nested_ternary: sign(42)=1
// EXPECT: ternary_assign: abs(-7)=7
// EXPECT: ternary_assign: abs(3)=3
// EXPECT: clamp(150,0,100)=100
// EXPECT: clamp(-20,0,100)=0
// EXPECT: clamp(50,0,100)=50
// EXPECT: ternary_in_call: 10
// EXPECT: ternary_in_call: 20
// EXPECT: median(1,2,3)=2
// EXPECT: median(3,1,2)=2
// EXPECT: median(5,5,5)=5
// EXPECT: ternary_side_effect: a=2 b=10
// EXPECT: ternary_chain: grade(95)=A
// EXPECT: ternary_chain: grade(85)=B
// EXPECT: ternary_chain: grade(75)=C
// EXPECT: ternary_chain: grade(65)=D
// EXPECT: ternary_chain: grade(50)=F
// EXPECT: ternary_ptr: picked=100
// EXPECT: ternary_array_idx: arr[1]=20
// EXPECT: ternary_bitwise: 0xFF
// EXPECT: sort3: 1 2 3
// EXPECT: sort3: 1 2 3
// EXPECT: sort3: 1 2 3

int printf(const char *fmt, ...);

int max2(int a, int b) {
    return a > b ? a : b;
}

int abs_val(int x) {
    return x < 0 ? -x : x;
}

int sign(int x) {
    return x < 0 ? -1 : (x > 0 ? 1 : 0);
}

int clamp(int val, int lo, int hi) {
    return val < lo ? lo : (val > hi ? hi : val);
}

char grade(int score) {
    return score >= 90 ? 'A'
         : score >= 80 ? 'B'
         : score >= 70 ? 'C'
         : score >= 60 ? 'D'
         : 'F';
}

int median3(int a, int b, int c) {
    return a > b
        ? (b > c ? b : (a > c ? c : a))
        : (a > c ? a : (b > c ? c : b));
}

void sort3(int *a, int *b, int *c) {
    int x = *a;
    int y = *b;
    int z = *c;
    // Use ternary to find min, mid, max
    int mn = x < y ? (x < z ? x : z) : (y < z ? y : z);
    int mx = x > y ? (x > z ? x : z) : (y > z ? y : z);
    int md = (x + y + z) - mn - mx;
    *a = mn;
    *b = md;
    *c = mx;
}

int main(void) {
    printf("=== Complex Ternary Expressions ===\n");

    // Basic ternary for max
    printf("basic_ternary: max(3,7)=%d\n", max2(3, 7));
    printf("basic_ternary: max(10,2)=%d\n", max2(10, 2));

    // Nested ternary for sign
    printf("nested_ternary: sign(-5)=%d\n", sign(-5));
    printf("nested_ternary: sign(0)=%d\n", sign(0));
    printf("nested_ternary: sign(42)=%d\n", sign(42));

    // Ternary in assignment for absolute value
    printf("ternary_assign: abs(-7)=%d\n", abs_val(-7));
    printf("ternary_assign: abs(3)=%d\n", abs_val(3));

    // Clamp
    printf("clamp(150,0,100)=%d\n", clamp(150, 0, 100));
    printf("clamp(-20,0,100)=%d\n", clamp(-20, 0, 100));
    printf("clamp(50,0,100)=%d\n", clamp(50, 0, 100));

    // Ternary as function argument
    {
        int a = 10;
        int b = 20;
        printf("ternary_in_call: %d\n", (a < b) ? a : b);
        printf("ternary_in_call: %d\n", (a > b) ? a : b);
    }

    // Median of three
    printf("median(1,2,3)=%d\n", median3(1, 2, 3));
    printf("median(3,1,2)=%d\n", median3(3, 1, 2));
    printf("median(5,5,5)=%d\n", median3(5, 5, 5));

    // Ternary with side effects
    {
        int a = 1;
        int b = 10;
        int flag = 1;
        // Only one side should execute
        int result = flag ? (a = a + 1, a) : (b = b + 1, b);
        // a should be 2, b should still be 10
        printf("ternary_side_effect: a=%d b=%d\n", a, b);
    }

    // Grade chain
    printf("ternary_chain: grade(95)=%c\n", grade(95));
    printf("ternary_chain: grade(85)=%c\n", grade(85));
    printf("ternary_chain: grade(75)=%c\n", grade(75));
    printf("ternary_chain: grade(65)=%c\n", grade(65));
    printf("ternary_chain: grade(50)=%c\n", grade(50));

    // Ternary to pick pointer
    {
        int x = 100;
        int y = 200;
        int flag = 1;
        int *p = flag ? &x : &y;
        printf("ternary_ptr: picked=%d\n", *p);
    }

    // Ternary as array index
    {
        int arr[3];
        arr[0] = 10;
        arr[1] = 20;
        arr[2] = 30;
        int idx = 1;
        int val = arr[idx > 2 ? 0 : idx];
        printf("ternary_array_idx: arr[%d]=%d\n", idx, val);
    }

    // Ternary with bitwise
    {
        int mask = 1;
        int val = mask ? 0xFF : 0x00;
        printf("ternary_bitwise: 0x%X\n", val);
    }

    // Sort three values using ternary-based sort3
    {
        int a, b, c;

        a = 3; b = 1; c = 2;
        sort3(&a, &b, &c);
        printf("sort3: %d %d %d\n", a, b, c);

        a = 2; b = 3; c = 1;
        sort3(&a, &b, &c);
        printf("sort3: %d %d %d\n", a, b, c);

        a = 1; b = 2; c = 3;
        sort3(&a, &b, &c);
        printf("sort3: %d %d %d\n", a, b, c);
    }

    return 0;
}
