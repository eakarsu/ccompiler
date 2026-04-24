// stress_070_return_stress.c - Return value stress test
// EXPECT: deep_nest_ret=42
// EXPECT: struct_ret_small=10 20
// EXPECT: struct_ret_large=1 2 3 4 5
// EXPECT: ternary_ret_pos=positive
// EXPECT: ternary_ret_neg=negative
// EXPECT: ternary_ret_zero=zero
// EXPECT: switch_ret_a=10
// EXPECT: switch_ret_b=20
// EXPECT: switch_ret_c=30
// EXPECT: switch_ret_default=99
// EXPECT: else_branch_ret=200
// EXPECT: complex_expr_ret=1073741824
// EXPECT: early_ret_found=42
// EXPECT: early_ret_notfound=-1
// EXPECT: do_while_ret=55
// EXPECT: nested_ternary_ret=3
// EXPECT: multi_cond_ret=win
// EXPECT: recursive_ret=120
// EXPECT: chain_ret=1000
// EXPECT: void_ptr_ret=99
// EXPECT: arith_chain_ret=362880
// EXPECT: conditional_side_effect=6
// EXPECT: ret_from_for=5
// EXPECT: ret_from_while=7
// EXPECT: ret_struct_ternary=100 200
// EXPECT: ret_through_ptr=42
// EXPECT: ret_cast_chain=65
// EXPECT: ret_comma_expr=30
// EXPECT: ret_bitwise_expr=170

int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

// --- Return from deep nesting (8 levels of if) ---
int deep_nested_return(int a, int b, int c, int d, int e, int f, int g, int h) {
    if (a > 0) {
        if (b > 0) {
            if (c > 0) {
                if (d > 0) {
                    if (e > 0) {
                        if (f > 0) {
                            if (g > 0) {
                                if (h > 0) {
                                    return a + b + c + d + e + f + g + h;
                                }
                                return -1;
                            }
                            return -2;
                        }
                        return -3;
                    }
                    return -4;
                }
                return -5;
            }
            return -6;
        }
        return -7;
    }
    return -8;
}

// --- Return struct by value (small, fits in registers) ---
struct Small {
    int x;
    int y;
};

struct Small make_small(int a, int b) {
    struct Small s;
    s.x = a;
    s.y = b;
    return s;
}

// --- Return struct by value (large, >16 bytes, via hidden pointer) ---
struct Large {
    int a, b, c, d, e;
};

struct Large make_large(int v1, int v2, int v3, int v4, int v5) {
    struct Large s;
    s.a = v1;
    s.b = v2;
    s.c = v3;
    s.d = v4;
    s.e = v5;
    return s;
}

// --- Return result of ternary ---
const char *ternary_sign(int x) {
    return x > 0 ? "positive" : (x < 0 ? "negative" : "zero");
}

// --- Return from switch case ---
int switch_return(char c) {
    switch (c) {
        case 'a': return 10;
        case 'b': return 20;
        case 'c': return 30;
        default: return 99;
    }
}

// --- Return from else branch ---
int else_branch_return(int x) {
    if (x > 100) {
        return x;
    } else if (x > 50) {
        return x * 2;
    } else if (x > 0) {
        return x * 3;
    } else {
        return 200;
    }
}

// --- Return of complex expression (many terms) ---
// Compute 2^30 via repeated multiplication in one expression
int complex_expression_return(int x) {
    return x * x * x * x * x * x * x * x * x * x *
           x * x * x * x * x * x * x * x * x * x *
           x * x * x * x * x * x * x * x * x * x;
}

// --- Early return patterns ---
int find_value(int *arr, int n, int target) {
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] == target) {
            return arr[i]; // early return
        }
    }
    return -1; // not found
}

// --- Return in do-while ---
int do_while_return(int n) {
    int sum = 0;
    int i = 1;
    do {
        sum += i;
        i++;
        if (i > n) {
            return sum;
        }
    } while (1);
    return -1; // unreachable
}

// --- Nested ternary return ---
int nested_ternary_return(int a, int b, int c) {
    return a > b ? (a > c ? 1 : 2) : (b > c ? 3 : 4);
}

// --- Multi-condition return ---
const char *multi_cond_return(int score) {
    if (score >= 90) return "win";
    if (score >= 70) return "good";
    if (score >= 50) return "ok";
    if (score >= 30) return "bad";
    return "fail";
}

// --- Recursive return ---
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// --- Chained function returns ---
int chain_a(int x) { return x + 100; }
int chain_b(int x) { return chain_a(x) + 200; }
int chain_c(int x) { return chain_b(x) + 300; }
int chain_d(int x) { return chain_c(x) + 400; }

// --- Return void pointer cast ---
void *make_int_ptr(int val) {
    int *p = malloc(sizeof(int));
    *p = val;
    return p;
}

// --- Return arithmetic chain (factorial via iteration) ---
int arith_chain_return(int n) {
    int result = 1;
    int i;
    for (i = 2; i <= n; i++) {
        result = result * i;
    }
    return result;
}

// --- Return with side effects in condition ---
int global_counter;

int increment_and_check(int limit) {
    global_counter++;
    return global_counter >= limit;
}

int conditional_side_effect(void) {
    global_counter = 0;
    while (!increment_and_check(6)) {
        // loop
    }
    return global_counter;
}

// --- Return from for loop body ---
int ret_from_for(void) {
    int i;
    for (i = 0; i < 100; i++) {
        if (i * i >= 25) {
            return i;
        }
    }
    return -1;
}

// --- Return from while loop ---
int ret_from_while(void) {
    int x = 1;
    while (x < 1000) {
        if (x >= 7) return x;
        x++;
    }
    return -1;
}

// --- Return struct from ternary ---
struct Small ret_struct_ternary(int flag) {
    struct Small a = {100, 200};
    struct Small b = {300, 400};
    if (flag) return a;
    return b;
}

// --- Return through pointer dereference ---
int ret_through_ptr(void) {
    int val = 42;
    int *p = &val;
    return *p;
}

// --- Return with cast chain ---
int ret_cast_chain(void) {
    char c = 'A'; // 65
    int i = (int)c;
    long l = (long)i;
    int result = (int)l;
    return result;
}

// --- Return comma expression ---
int ret_comma_expr(void) {
    int a = 10;
    int b = 20;
    int c = 30;
    // In C, comma operator evaluates left to right, returns rightmost
    return (a, b, c);
}

// --- Return bitwise expression ---
int ret_bitwise_expr(void) {
    // 0xAA = 10101010 = 170
    return (0xFF & 0xAA) | (0x00 & 0x55);
}

int main(void) {
    // Deep nesting: 1+2+3+4+5+6+7+14 = 42 (need to pick right args)
    // Let's use: 3+4+5+6+7+8+9+0 = 42? No.
    // 1+2+3+4+5+6+7+14 = 42. Use a=1,b=2,c=3,d=4,e=5,f=6,g=7,h=14
    printf("deep_nest_ret=%d\n", deep_nested_return(1, 2, 3, 4, 5, 6, 7, 14));

    // Struct return
    struct Small sm = make_small(10, 20);
    printf("struct_ret_small=%d %d\n", sm.x, sm.y);

    struct Large lg = make_large(1, 2, 3, 4, 5);
    printf("struct_ret_large=%d %d %d %d %d\n", lg.a, lg.b, lg.c, lg.d, lg.e);

    // Ternary return
    printf("ternary_ret_pos=%s\n", ternary_sign(5));
    printf("ternary_ret_neg=%s\n", ternary_sign(-3));
    printf("ternary_ret_zero=%s\n", ternary_sign(0));

    // Switch return
    printf("switch_ret_a=%d\n", switch_return('a'));
    printf("switch_ret_b=%d\n", switch_return('b'));
    printf("switch_ret_c=%d\n", switch_return('c'));
    printf("switch_ret_default=%d\n", switch_return('z'));

    // Else branch return
    printf("else_branch_ret=%d\n", else_branch_return(-10));

    // Complex expression: 2^30 = 1073741824
    printf("complex_expr_ret=%d\n", complex_expression_return(2));

    // Early return
    int arr[8] = {10, 20, 30, 42, 50, 60, 70, 80};
    printf("early_ret_found=%d\n", find_value(arr, 8, 42));
    printf("early_ret_notfound=%d\n", find_value(arr, 8, 99));

    // Do-while return: sum 1..10 = 55
    printf("do_while_ret=%d\n", do_while_return(10));

    // Nested ternary: a=1, b=5, c=3 -> a>b? no -> b>c? yes -> 3
    printf("nested_ternary_ret=%d\n", nested_ternary_return(1, 5, 3));

    // Multi condition
    printf("multi_cond_ret=%s\n", multi_cond_return(95));

    // Recursive: 5! = 120
    printf("recursive_ret=%d\n", factorial(5));

    // Chain: chain_d(0) = chain_c(0)+400 = (chain_b(0)+300)+400 = ((chain_a(0)+200)+300)+400
    //       = ((0+100)+200+300+400) = 1000
    printf("chain_ret=%d\n", chain_d(0));

    // Void pointer return
    int *ip = (int *)make_int_ptr(99);
    printf("void_ptr_ret=%d\n", *ip);
    free(ip);

    // Arithmetic chain: 9! = 362880
    printf("arith_chain_ret=%d\n", arith_chain_return(9));

    // Conditional side effect
    printf("conditional_side_effect=%d\n", conditional_side_effect());

    // Return from loops
    printf("ret_from_for=%d\n", ret_from_for());
    printf("ret_from_while=%d\n", ret_from_while());

    // Struct ternary return
    struct Small st = ret_struct_ternary(1);
    printf("ret_struct_ternary=%d %d\n", st.x, st.y);

    // Return through pointer
    printf("ret_through_ptr=%d\n", ret_through_ptr());

    // Cast chain
    printf("ret_cast_chain=%d\n", ret_cast_chain());

    // Comma expression return
    printf("ret_comma_expr=%d\n", ret_comma_expr());

    // Bitwise expression return
    printf("ret_bitwise_expr=%d\n", ret_bitwise_expr());

    return 0;
}
