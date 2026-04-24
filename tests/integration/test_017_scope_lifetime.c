int printf(const char *fmt, ...);
// EXPECT: g_value: 42\ng_counter: 0\ng_array[3]: 30\nshadow: 99\nglobal still: 42\ncounter: 3\nnested scope: 120\nshadow: 9\nloop scope: 20\nfunc_a: 100\nfunc_b: 200\nfunc_c: 300\ng_array[5]: 999\ng_array[0]: 0\nparam test: 32\nx unchanged: 1\nrecsum 5: 15\nrecsum 10: 55\nouter 4: 16\nouter 7: 28\ncomplex 8: 30\ncomplex -3: 6\nwhile scope: 30\ndowhile scope: 100\nfor accum 4: 40\nsum globals: 60\nbump 5: 5\nbump 3: 8\ndeep nest 1: 21\ndeep nest 10: 75\nbranch 5: 10\nbranch -4: -12\navg: 86\ncounted 10: 11\ncounted 10: 12\ncounted 10: 13\nmulti 60: 120\nmulti 30: 110\nmulti 10: 20

// ============================================================
// test_017_scope_lifetime.c
// Variable shadowing, block scope, function scope,
// global vs local, nested blocks, variable lifetime
// ============================================================

// -----------------------------------------------------------
// Global variables
// -----------------------------------------------------------
int g_counter;
int g_value;
int g_array[10];

// -----------------------------------------------------------
// Initialize globals
// -----------------------------------------------------------
void init_globals(void) {
    int i;
    g_counter = 0;
    g_value = 42;
    for (i = 0; i < 10; i = i + 1) {
        g_array[i] = i * 10;
    }
}

// -----------------------------------------------------------
// Increment global counter
// -----------------------------------------------------------
void inc_counter(void) {
    g_counter = g_counter + 1;
}

// -----------------------------------------------------------
// Get global counter
// -----------------------------------------------------------
int get_counter(void) {
    return g_counter;
}

// -----------------------------------------------------------
// Function that shadows global g_value with local
// -----------------------------------------------------------
int shadow_global(void) {
    int g_value;
    g_value = 99;
    return g_value;
}

// -----------------------------------------------------------
// Function that reads the actual global g_value
// -----------------------------------------------------------
int read_global(void) {
    return g_value;
}

// -----------------------------------------------------------
// Function with nested block scoping
// -----------------------------------------------------------
int nested_scope_test(int x) {
    int result;
    result = x;
    {
        int a;
        a = x + 10;
        result = result + a;
        {
            int b;
            b = a + 20;
            result = result + b;
            {
                int c;
                c = b + 30;
                result = result + c;
            }
            // c is out of scope here
        }
        // b is out of scope here
    }
    // a is out of scope here
    return result;
}

// -----------------------------------------------------------
// Function with variable shadowing in nested blocks
// -----------------------------------------------------------
int shadow_test(int x) {
    int v;
    int sum;
    v = 1;
    sum = v;
    {
        int v;
        v = 2;
        sum = sum + v;
        {
            int v;
            v = 3;
            sum = sum + v;
        }
        sum = sum + v; // v is 2 again
    }
    sum = sum + v; // v is 1 again
    return sum;
}

// -----------------------------------------------------------
// Function scope: local survives loop iterations
// -----------------------------------------------------------
int loop_scope_test(void) {
    int total;
    int i;
    total = 0;
    for (i = 0; i < 5; i = i + 1) {
        int temp;
        temp = i * 2;
        total = total + temp;
    }
    // i is still accessible here in C (function scope for loop var)
    return total;
}

// -----------------------------------------------------------
// Demonstrate that function-local vars are independent
// -----------------------------------------------------------
int func_a(void) {
    int x;
    x = 100;
    return x;
}

int func_b(void) {
    int x;
    x = 200;
    return x;
}

int func_c(void) {
    int x;
    x = 300;
    return x;
}

// -----------------------------------------------------------
// Global array modification from function
// -----------------------------------------------------------
void modify_global_array(int idx, int val) {
    g_array[idx] = val;
}

int read_global_array(int idx) {
    return g_array[idx];
}

// -----------------------------------------------------------
// Test parameter scope (params are local)
// -----------------------------------------------------------
int param_test(int a, int b) {
    a = a + 10;
    b = b + 20;
    return a + b;
}

// -----------------------------------------------------------
// Recursive function with local variables
// -----------------------------------------------------------
int recursive_sum(int n) {
    int local;
    if (n <= 0) {
        return 0;
    }
    local = n;
    return local + recursive_sum(n - 1);
}

// -----------------------------------------------------------
// Nested function calls with same-named locals
// -----------------------------------------------------------
int inner_func(int n) {
    int result;
    result = n * 3;
    return result;
}

int outer_func(int n) {
    int result;
    result = inner_func(n) + n;
    return result;
}

// -----------------------------------------------------------
// Complex nested blocks with multiple variables
// -----------------------------------------------------------
int complex_blocks(int input) {
    int x;
    int y;
    int z;
    x = input;
    y = 0;
    z = 0;

    if (x > 0) {
        int a;
        a = x * 2;
        y = a;
        if (a > 10) {
            int b;
            b = a - 10;
            z = b;
        } else {
            int b;
            b = a + 10;
            z = b;
        }
    } else {
        int a;
        a = -x;
        y = a;
        z = a * 2;
    }

    return x + y + z;
}

// -----------------------------------------------------------
// While loop with block-scoped variable
// -----------------------------------------------------------
int while_scope_test(void) {
    int sum;
    int i;
    sum = 0;
    i = 0;
    while (i < 5) {
        int squared;
        squared = i * i;
        sum = sum + squared;
        i = i + 1;
    }
    return sum;
}

// -----------------------------------------------------------
// Do-while with local variables
// -----------------------------------------------------------
int dowhile_scope_test(void) {
    int sum;
    int i;
    sum = 0;
    i = 1;
    do {
        int cube;
        cube = i * i * i;
        sum = sum + cube;
        i = i + 1;
    } while (i <= 4);
    return sum;
}

// -----------------------------------------------------------
// For loop with accumulator pattern
// -----------------------------------------------------------
int for_accum(int n) {
    int acc;
    int i;
    acc = 0;
    for (i = 1; i <= n; i = i + 1) {
        int term;
        term = i * i + i;
        acc = acc + term;
    }
    return acc;
}

// -----------------------------------------------------------
// Multiple globals interaction
// -----------------------------------------------------------
int g_x;
int g_y;
int g_z;

void set_globals(int x, int y, int z) {
    g_x = x;
    g_y = y;
    g_z = z;
}

int sum_globals(void) {
    return g_x + g_y + g_z;
}

// -----------------------------------------------------------
// Function that modifies and returns global
// -----------------------------------------------------------
int bump_counter(int amount) {
    g_counter = g_counter + amount;
    return g_counter;
}

// -----------------------------------------------------------
// Deeply nested scope test
// -----------------------------------------------------------
int deep_nest(int n) {
    int a;
    a = n;
    {
        int b;
        b = a + 1;
        {
            int c;
            c = b + 1;
            {
                int d;
                d = c + 1;
                {
                    int e;
                    e = d + 1;
                    {
                        int f;
                        f = e + 1;
                        return a + b + c + d + e + f;
                    }
                }
            }
        }
    }
}

// -----------------------------------------------------------
// Conditional scope with same variable names in branches
// -----------------------------------------------------------
int branch_scope(int x) {
    int result;
    result = 0;
    if (x > 0) {
        int temp;
        temp = x * 2;
        result = temp;
    } else {
        int temp;
        temp = x * 3;
        result = temp;
    }
    return result;
}

// -----------------------------------------------------------
// Test that pointer-accessed global arrays work
// -----------------------------------------------------------
int g_scores[5];

void init_scores(void) {
    g_scores[0] = 90;
    g_scores[1] = 85;
    g_scores[2] = 92;
    g_scores[3] = 78;
    g_scores[4] = 88;
}

int avg_scores(void) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < 5; i = i + 1) {
        sum = sum + g_scores[i];
    }
    return sum / 5;
}

// -----------------------------------------------------------
// Static-like behavior using globals
// -----------------------------------------------------------
int call_count_g;

int counted_func(int x) {
    call_count_g = call_count_g + 1;
    return x + call_count_g;
}

// -----------------------------------------------------------
// Multiple return paths with local cleanup
// -----------------------------------------------------------
int multi_return(int x) {
    int a;
    a = x * 2;
    if (a > 100) {
        return a;
    }
    {
        int b;
        b = a + 50;
        if (b > 100) {
            return b;
        }
    }
    return a;
}

// -----------------------------------------------------------
// MAIN
// -----------------------------------------------------------
int main(void) {
    int r;
    int i;
    int x;

    // ---- Global init and read ----
    init_globals();
    printf("g_value: %d\n", read_global());
    printf("g_counter: %d\n", get_counter());
    printf("g_array[3]: %d\n", read_global_array(3));

    // ---- Shadow global ----
    printf("shadow: %d\n", shadow_global());
    printf("global still: %d\n", read_global());

    // ---- Counter increment ----
    inc_counter();
    inc_counter();
    inc_counter();
    printf("counter: %d\n", get_counter());

    // ---- Nested scope ----
    // nested_scope_test(5):
    // result=5, a=15, result=5+15=20, b=15+20=35, result=20+35=55
    // c=35+30=65, result=55+65=120
    printf("nested scope: %d\n", nested_scope_test(5));

    // ---- Shadow test ----
    // v=1, sum=1, v=2 sum=3, v=3 sum=6, sum=6+2=8, sum=8+1=9
    printf("shadow: %d\n", shadow_test(0));

    // ---- Loop scope ----
    // 0+0+2+4+6+8 = 0+2+4+6+8 = 20? No: temp=i*2, so:
    // i=0: temp=0, total=0
    // i=1: temp=2, total=2
    // i=2: temp=4, total=6
    // i=3: temp=6, total=12
    // i=4: temp=8, total=20
    printf("loop scope: %d\n", loop_scope_test());

    // ---- Independent function locals ----
    printf("func_a: %d\n", func_a());
    printf("func_b: %d\n", func_b());
    printf("func_c: %d\n", func_c());

    // ---- Global array modify ----
    modify_global_array(5, 999);
    printf("g_array[5]: %d\n", read_global_array(5));
    printf("g_array[0]: %d\n", read_global_array(0));

    // ---- Parameter scope ----
    x = 1;
    r = param_test(x, x);
    printf("param test: %d\n", r);
    printf("x unchanged: %d\n", x);

    // ---- Recursive sum ----
    printf("recsum 5: %d\n", recursive_sum(5));
    printf("recsum 10: %d\n", recursive_sum(10));

    // ---- Nested function calls ----
    printf("outer 4: %d\n", outer_func(4));
    printf("outer 7: %d\n", outer_func(7));

    // ---- Complex blocks ----
    printf("complex 8: %d\n", complex_blocks(8));
    printf("complex -3: %d\n", complex_blocks(-3));

    // ---- While scope ----
    // 0+1+4+9+16 = 30
    printf("while scope: %d\n", while_scope_test());

    // ---- Do-while scope ----
    // 1+8+27+64 = 100
    printf("dowhile scope: %d\n", dowhile_scope_test());

    // ---- For accumulator ----
    // i=1: 2, i=2: 6, i=3: 12, i=4: 20 => 2+6+12+20 = 40
    printf("for accum 4: %d\n", for_accum(4));

    // ---- Multiple globals ----
    set_globals(10, 20, 30);
    printf("sum globals: %d\n", sum_globals());

    // ---- Bump counter ----
    g_counter = 0;
    printf("bump 5: %d\n", bump_counter(5));
    printf("bump 3: %d\n", bump_counter(3));

    // ---- Deep nest ----
    // a=1, b=2, c=3, d=4, e=5, f=6 => 1+2+3+4+5+6=21
    printf("deep nest 1: %d\n", deep_nest(1));
    // a=10, b=11, c=12, d=13, e=14, f=15 => 75
    printf("deep nest 10: %d\n", deep_nest(10));

    // ---- Branch scope ----
    printf("branch 5: %d\n", branch_scope(5));
    printf("branch -4: %d\n", branch_scope(-4));

    // ---- Scores ----
    init_scores();
    printf("avg: %d\n", avg_scores());

    // ---- Counted func ----
    call_count_g = 0;
    printf("counted 10: %d\n", counted_func(10));
    printf("counted 10: %d\n", counted_func(10));
    printf("counted 10: %d\n", counted_func(10));

    // ---- Multi return ----
    printf("multi 60: %d\n", multi_return(60));
    printf("multi 30: %d\n", multi_return(30));
    printf("multi 10: %d\n", multi_return(10));

    return 0;
}
