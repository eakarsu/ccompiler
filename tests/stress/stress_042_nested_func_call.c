// stress_042_nested_func_call.c - Deeply nested function calls as arguments
// EXPECT: deep6=720
// EXPECT: all_arg_calls=60
// EXPECT: call_as_index=99
// EXPECT: call_as_init=40
// EXPECT: sizeof_no_eval=4
// EXPECT: nested_binop=385
// EXPECT: recursive_chain=55
// EXPECT: multi_return_nest=1000
// EXPECT: cond_in_call=30
// EXPECT: call_in_ternary=200
// EXPECT: massive_nest=3628800
// EXPECT: array_call_idx=77

int printf(const char *fmt, ...);

// --- Helpers for deep nesting ---
int id(int x) { return x; }
int inc(int x) { return x + 1; }
int dbl(int x) { return x * 2; }
int sqr(int x) { return x * x; }
int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }
int sub(int a, int b) { return a - b; }

// Factorial
int fact(int n) {
    if (n <= 1) return 1;
    return n * fact(n - 1);
}

// --- Sum of 1..n ---
int sum_to(int n) {
    int s = 0;
    int i;
    for (i = 1; i <= n; i++) s = s + i;
    return s;
}

// Max of 3
int max3(int a, int b, int c) {
    int m = a;
    if (b > m) m = b;
    if (c > m) m = c;
    return m;
}

// Clamp
int clamp(int val, int lo, int hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

// Return index for array
int get_index(int base, int offset) {
    return base + offset;
}

// Struct for call-as-initializer test
struct Point {
    int x;
    int y;
};

struct Point make_point(int x, int y) {
    struct Point p;
    p.x = x;
    p.y = y;
    return p;
}

// --- Recursive chain: f calls g calls f ---
int g_chain(int n);
int f_chain(int n) {
    if (n <= 0) return 0;
    return n + g_chain(n - 1);
}
int g_chain(int n) {
    if (n <= 0) return 0;
    return f_chain(n);
}

int main(void) {
    // f(g(h(i(j(k(1)))))) - 6 levels deep
    // k=inc(1)=2, j=dbl(2)=4, i=sqr(4)=16, h=inc(16)=17, g=dbl(17)=34, f=sqr(34)=...
    // Actually let's use fact for a clean result
    // fact(fact(3)) = fact(6) = 720
    printf("deep6=%d\n", fact(fact(inc(inc(inc(0))))));

    // Function call in every argument position
    // add(mul(2,3), sub(mul(5,5), add(10,5))) = add(6, sub(25,15)) = add(6, 10) = 16
    // Actually: add(mul(3,4), mul(add(2,3), add(4,5))) = add(12, mul(5,9)) = add(12,45) = 57
    // Simpler: mul(add(2,3), add(5,7)) = mul(5,12) = 60
    printf("all_arg_calls=%d\n", mul(add(2, 3), add(5, 7)));

    // Function call as array index
    int arr[5];
    arr[0] = 11;
    arr[1] = 22;
    arr[2] = 33;
    arr[3] = 99;
    arr[4] = 55;
    printf("call_as_index=%d\n", arr[add(1, 2)]);

    // Function call as struct initializer component
    struct Point p = make_point(add(10, 15), mul(3, 5));
    printf("call_as_init=%d\n", add(p.x, p.y));

    // sizeof with function call inside (should NOT evaluate the call)
    int sz = sizeof(fact(100));
    printf("sizeof_no_eval=%d\n", sz);

    // Nested binop with function calls at every leaf
    // add(mul(add(1,2),add(3,4)), mul(add(5,6),add(7,8))) + add(mul(add(1,2),add(3,4)), mul(add(5,6),add(7,8)))
    // = add(mul(3,7), mul(11,15)) + same
    // = add(21, 165) + add(21, 165)
    // = 186 + 186 = 372... let me recalc
    // inner1 = add(mul(add(1,2), add(3,4)), mul(add(5,6), add(7,8)))
    //        = add(mul(3,7), mul(11,15)) = add(21, 165) = 186
    // Let me just do: add(mul(inc(4), inc(6)), mul(inc(8), inc(9)))
    // = add(mul(5,7), mul(9,10)) = add(35, 90) = 125
    // But let me also add more: add(add(...), add(...)) = 125 + something
    // Simpler: sum_to(add(mul(2,3), add(1,3))) = sum_to(6+4) = sum_to(10) = 55
    // Let's go with: mul(sum_to(add(2,3)), sum_to(add(5,5)))
    // = mul(sum_to(5), sum_to(10)) = mul(15, 55) = 825... too big
    // Just: add(sum_to(inc(9)), mul(add(10,10), add(5,5))) = add(55, 200) = 255
    // Actually let me keep it clean:
    // add(sum_to(mul(2,5)), mul(add(3,4), mul(5,9))) = add(sum_to(10), mul(7,45)) = add(55, 315) = 370
    // Let me just pick something verifiable:
    int nested_r = add(mul(add(3, 4), add(5, 6)), mul(add(7, 8), add(9, 10)));
    // = add(mul(7,11), mul(15,19)) = add(77, 285) = 362...
    // 7*11=77, 15*19=285, 77+285=362
    // Hmm, let me try: add(mul(5,7), mul(add(10,10), add(5,6)))
    // = add(35, mul(20,11)) = add(35, 220) = 255
    // Fine let's just go with the straightforward one:
    // add(mul(5,10), mul(11,15)) + mul(10,10) = 50+165+100 = 315
    // I'll just hardcode a correct one:
    // max3(mul(3,5), add(10,10), sub(100,80)) = max3(15, 20, 20) = 20
    // Let's do something we can verify:
    int v1 = add(mul(add(1, 2), add(3, 4)), mul(add(5, 6), add(7, 8)));
    // v1 = add(3*7, 11*15) = add(21, 165) = 186
    int v2 = add(mul(add(2, 3), add(4, 5)), mul(add(1, 1), add(3, 7)));
    // v2 = add(5*9, 2*10) = add(45, 20) = 65
    int v3 = max3(mul(10, 10), add(50, 84), sub(200, 66));
    // v3 = max3(100, 134, 134) = 134
    printf("nested_binop=%d\n", add(v1, add(v2, v3)));
    // = add(186, add(65, 134)) = add(186, 199) = 385

    // Recursive chain f->g->f
    printf("recursive_chain=%d\n", f_chain(10));

    // Deeply nested return value usage
    // fact(5) * sqr(dbl(inc(4))) = 120 * sqr(dbl(5)) = 120 * sqr(10) = 120 * 100 = 12000
    // Too big, let me use: mul(dbl(5), sqr(dbl(5))) = mul(10, sqr(10)) = mul(10, 100) = 1000
    printf("multi_return_nest=%d\n", mul(dbl(5), sqr(dbl(5))));

    // Conditional expression inside function call
    int flag = 1;
    printf("cond_in_call=%d\n", add(flag ? 10 : 5, flag ? 20 : 50));

    // Function call in ternary branches
    int sel = 0;
    int tr = sel ? add(1, 2) : mul(10, 20);
    printf("call_in_ternary=%d\n", tr);

    // Massive nesting: fact(10) via nested inc calls as arg
    // inc(inc(inc(inc(inc(inc(inc(inc(inc(inc(0)))))))))) = 10
    printf("massive_nest=%d\n", fact(inc(inc(inc(inc(inc(inc(inc(inc(inc(inc(0))))))))))));

    // Function call as array index, chained
    int lookup[5];
    lookup[0] = 3;
    lookup[1] = 0;
    lookup[2] = 4;
    lookup[3] = 1;
    lookup[4] = 2;
    int values[5];
    values[0] = 77;
    values[1] = 88;
    values[2] = 99;
    values[3] = 11;
    values[4] = 22;
    // values[lookup[add(1,2)]] = values[lookup[3]] = values[1] = 88...
    // add(1,2)=3, lookup[3]=1, values[1]=88
    // Hmm let me pick: values[lookup[sub(4,4)]] = values[lookup[0]] = values[3] = 11
    // I want 77: values[lookup[sub(3,2)]] = values[lookup[1]] = values[0] = 77
    printf("array_call_idx=%d\n", values[lookup[sub(3, 2)]]);

    return 0;
}
