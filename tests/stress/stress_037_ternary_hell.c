// stress_037_ternary_hell.c - Ternary operator stress
// EXPECT: deep8=8
// EXPECT: deep8_mid=5
// EXPECT: deep8_first=1
// EXPECT: fptr_add=30
// EXPECT: fptr_mul=200
// EXPECT: ternary_side=1 0
// EXPECT: ternary_side2=0 1
// EXPECT: arr_idx=30
// EXPECT: arr_idx2=50
// EXPECT: func_arg=42
// EXPECT: func_arg2=-1
// EXPECT: nested_mixed=100
// EXPECT: chain_eq=equal
// EXPECT: chain_lt=less
// EXPECT: chain_gt=greater
// EXPECT: ternary_str=yes
// EXPECT: ternary_str2=no
// EXPECT: ternary_assign=7
// EXPECT: ternary_assign2=3
// EXPECT: ternary_call=10
// EXPECT: ternary_call2=6
// EXPECT: ternary_ptr=42
// EXPECT: ternary_ptr2=99
// EXPECT: ternary_loop_sum=20
// EXPECT: ternary_recursive=120
// EXPECT: pass_done=1

int printf(const char *fmt, ...);

// Functions for function pointer ternary test
int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }

// 8-deep nested ternary: returns the value of n clamped to 1..8
int deep_ternary(int n) {
    return n == 1 ? 1
         : n == 2 ? 2
         : n == 3 ? 3
         : n == 4 ? 4
         : n == 5 ? 5
         : n == 6 ? 6
         : n == 7 ? 7
         : 8;
}

// Side-effect in ternary branches
int side_a;
int side_b;

void ternary_side_effect(int cond) {
    cond ? (side_a = 1) : (side_b = 1);
}

// Ternary as array index
int ternary_arr_index(int *arr, int cond) {
    return arr[cond ? 2 : 4];
}

// Ternary in function argument
int identity(int x) { return x; }

int classify(int a, int b) {
    return a > b ? 1 : (a < b ? -1 : 0);
}

// Three-way comparison using ternary
void three_way(int a, int b) {
    printf("chain_%s\n",
           a > b ? "gt=greater" :
           a < b ? "lt=less" :
           "eq=equal");
}

// Nested mixed ternary with arithmetic
int nested_mixed(int a, int b, int c) {
    return (a > 0 ? a * 10 : -a * 10) + (b > 0 ? b : -b) + (c > 0 ? 0 : c);
}

// Ternary selecting function to call
int ternary_call(int cond, int a, int b) {
    return cond ? add(a, b) : mul(a, b);
}

// Ternary with pointer (select which var to read)
int ternary_ptr(int cond) {
    int x = 42;
    int y = 99;
    int *p = cond ? &x : &y;
    return *p;
}

// Ternary inside loop
int ternary_loop(void) {
    int sum = 0;
    int i;
    for (i = 0; i < 10; i++) {
        sum = sum + (i % 2 == 0 ? i : 0);
    }
    // 0+2+4+6+8 = 20... let me recount
    // i=0: even, sum=0
    // i=1: odd, sum=0
    // i=2: even, sum=2
    // i=3: odd, sum=2
    // i=4: even, sum=6
    // i=5: odd, sum=6
    // i=6: even, sum=12
    // i=7: odd, sum=12
    // i=8: even, sum=20
    // i=9: odd, sum=20
    return sum;
}

// Recursive with ternary (factorial)
int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

int main(void) {
    // 8-deep nested ternary
    printf("deep8=%d\n", deep_ternary(100));   // > 7 -> 8
    printf("deep8_mid=%d\n", deep_ternary(5)); // exactly 5
    printf("deep8_first=%d\n", deep_ternary(1)); // exactly 1

    // Ternary selecting function pointer
    int (*fp)(int, int);
    fp = 1 ? add : mul;
    printf("fptr_add=%d\n", fp(10, 20));
    fp = 0 ? add : mul;
    printf("fptr_mul=%d\n", fp(10, 20));

    // Ternary with side effects (only one branch executes)
    side_a = 0;
    side_b = 0;
    ternary_side_effect(1);
    printf("ternary_side=%d %d\n", side_a, side_b);
    side_a = 0;
    side_b = 0;
    ternary_side_effect(0);
    printf("ternary_side2=%d %d\n", side_a, side_b);

    // Ternary as array index
    int arr[5];
    arr[0] = 10; arr[1] = 20; arr[2] = 30; arr[3] = 40; arr[4] = 50;
    printf("arr_idx=%d\n", ternary_arr_index(arr, 1));  // arr[2]=30
    printf("arr_idx2=%d\n", ternary_arr_index(arr, 0)); // arr[4]=50

    // Ternary in function argument
    printf("func_arg=%d\n", identity(1 ? 42 : 0));
    printf("func_arg2=%d\n", classify(3, 5));

    // Nested mixed
    printf("nested_mixed=%d\n", nested_mixed(9, 10, 0)); // 90+10+0=100

    // Three-way comparison
    three_way(5, 5);  // eq
    three_way(3, 5);  // lt
    three_way(7, 5);  // gt

    // Ternary selecting string
    printf("ternary_str=%s\n", 1 ? "yes" : "no");
    printf("ternary_str2=%s\n", 0 ? "yes" : "no");

    // Ternary for assignment
    int ta;
    ta = 1 ? 7 : 3;
    printf("ternary_assign=%d\n", ta);
    ta = 0 ? 7 : 3;
    printf("ternary_assign2=%d\n", ta);

    // Ternary selecting function call
    printf("ternary_call=%d\n", ternary_call(1, 3, 7));  // add(3,7)=10
    printf("ternary_call2=%d\n", ternary_call(0, 3, 2)); // mul(3,2)=6

    // Ternary with pointer
    printf("ternary_ptr=%d\n", ternary_ptr(1));  // 42
    printf("ternary_ptr2=%d\n", ternary_ptr(0)); // 99

    // Ternary in loop
    printf("ternary_loop_sum=%d\n", ternary_loop());

    // Recursive factorial via ternary
    printf("ternary_recursive=%d\n", factorial(5));

    printf("pass_done=1\n");
    return 0;
}
