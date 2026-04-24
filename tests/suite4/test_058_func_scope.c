int printf(const char *fmt, ...);

// Global variable
int g_counter = 0;

// Function with local that shadows global
int shadow_global(void) {
    int g_counter = 100;
    return g_counter;
}

// Function that uses the actual global
int read_global(void) {
    return g_counter;
}

void increment_global(void) {
    g_counter = g_counter + 1;
}

// Two functions with same-named locals
int func_a(int x) {
    int result = x * 2;
    int temp = result + 1;
    return temp;
}

int func_b(int x) {
    int result = x * 3;
    int temp = result - 1;
    return temp;
}

// Nested scopes within a function
int nested_scope(int x) {
    int a = x;
    {
        int a = x * 2;
        x = a;
    }
    return a + x;
}

// Multiple levels of nesting
int deep_scope(int x) {
    int sum = 0;
    {
        int a = x;
        sum = sum + a;
        {
            int b = x * 2;
            sum = sum + b;
            {
                int c = x * 3;
                sum = sum + c;
            }
        }
    }
    return sum;
}

// For loop scope: loop variable
int loop_scope(void) {
    int sum = 0;
    int i;
    for (i = 0; i < 5; i = i + 1) {
        int val = i * 10;
        sum = sum + val;
    }
    // i should be 5 after the loop
    return sum + i;
}

// While loop scope
int while_scope(void) {
    int total = 0;
    int count = 0;
    while (count < 4) {
        int val = count * count;
        total = total + val;
        count = count + 1;
    }
    return total;
}

// Variables in different branches
int branch_scope(int x) {
    int result;
    if (x > 0) {
        int temp = x * 2;
        result = temp;
    } else {
        int temp = x * -3;
        result = temp;
    }
    return result;
}

// Function parameters scope
int param_scope(int a, int b) {
    int a_local = a + 10;
    int b_local = b + 20;
    return a_local + b_local;
}

// Scope with pointers
int pointer_scope(void) {
    int x = 5;
    int y = 10;
    int *p;
    {
        p = &x;
        *p = 50;
    }
    {
        p = &y;
        *p = 100;
    }
    return x + y;
}

// Same-name in sibling scopes
int sibling_scope(int x) {
    int result = 0;
    {
        int tmp = x + 1;
        result = result + tmp;
    }
    {
        int tmp = x + 2;
        result = result + tmp;
    }
    {
        int tmp = x + 3;
        result = result + tmp;
    }
    return result;
}

// Switch scope
int switch_scope(int x) {
    int result = 0;
    switch (x) {
        case 1: {
            int val = 10;
            result = val;
            break;
        }
        case 2: {
            int val = 20;
            result = val;
            break;
        }
        case 3: {
            int val = 30;
            result = val;
            break;
        }
    }
    return result;
}

// Variable reuse across calls
int counter_func(void) {
    int local = 0;
    local = local + 1;
    return local;
}

// Global array accessed from functions
int g_arr[5];

void init_global_arr(void) {
    int i;
    for (i = 0; i < 5; i = i + 1) {
        g_arr[i] = (i + 1) * 10;
    }
}

int sum_global_arr(void) {
    int total = 0;
    int i;
    for (i = 0; i < 5; i = i + 1) {
        total = total + g_arr[i];
    }
    return total;
}

int main(void) {
    // Shadow global
    // EXPECT: 100
    printf("%d\n", shadow_global());
    // Global unchanged
    // EXPECT: 0
    printf("%d\n", read_global());

    // Modify global
    increment_global();
    increment_global();
    increment_global();
    // EXPECT: 3
    printf("%d\n", read_global());

    // Same-named locals in different functions
    // func_a(5) = 5*2 + 1 = 11
    // EXPECT: 11
    printf("%d\n", func_a(5));
    // func_b(5) = 5*3 - 1 = 14
    // EXPECT: 14
    printf("%d\n", func_b(5));

    // Nested scope: a=10 stays, x becomes 20, return 10+20=30
    // EXPECT: 30
    printf("%d\n", nested_scope(10));

    // Deep scope: x=5, sum = 5 + 10 + 15 = 30
    // EXPECT: 30
    printf("%d\n", deep_scope(5));

    // Loop scope: sum = 0+10+20+30+40 = 100, i=5, total=105
    // EXPECT: 105
    printf("%d\n", loop_scope());

    // While scope: 0*0 + 1*1 + 2*2 + 3*3 = 0+1+4+9 = 14
    // EXPECT: 14
    printf("%d\n", while_scope());

    // Branch scope
    // EXPECT: 14
    printf("%d\n", branch_scope(7));
    // EXPECT: 12
    printf("%d\n", branch_scope(-4));

    // Param scope: (3+10) + (7+20) = 13 + 27 = 40
    // EXPECT: 40
    printf("%d\n", param_scope(3, 7));

    // Pointer scope: x=50, y=100, return 150
    // EXPECT: 150
    printf("%d\n", pointer_scope());

    // Sibling scope: x=10, (11)+(12)+(13) = 36
    // EXPECT: 36
    printf("%d\n", sibling_scope(10));

    // Switch scope
    // EXPECT: 10
    printf("%d\n", switch_scope(1));
    // EXPECT: 20
    printf("%d\n", switch_scope(2));
    // EXPECT: 30
    printf("%d\n", switch_scope(3));

    // counter_func always returns 1 (local resets each call)
    // EXPECT: 1
    printf("%d\n", counter_func());
    // EXPECT: 1
    printf("%d\n", counter_func());
    // EXPECT: 1
    printf("%d\n", counter_func());

    // Global array
    init_global_arr();
    // EXPECT: 150
    printf("%d\n", sum_global_arr());
    // EXPECT: 10 20 30 40 50
    printf("%d %d %d %d %d\n", g_arr[0], g_arr[1], g_arr[2], g_arr[3], g_arr[4]);

    return 0;
}
