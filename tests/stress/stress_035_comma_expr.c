// stress_035_comma_expr.c - Comma expression abuse
// EXPECT: for_comma=45
// EXPECT: for_multi_init=5 5
// EXPECT: for_multi_update=5 50
// EXPECT: while_comma=5
// EXPECT: while_side=6
// EXPECT: return_comma=42
// EXPECT: deep_nest=5
// EXPECT: deeper_nest=99
// EXPECT: arr_index_comma=30
// EXPECT: side_effect_order=1 2 3
// EXPECT: ternary_comma_t=20
// EXPECT: ternary_comma_f=30
// EXPECT: comma_assign=7
// EXPECT: comma_in_call=300
// EXPECT: comma_chain_side=6
// EXPECT: comma_void=done
// EXPECT: comma_cond=10
// EXPECT: comma_complex_for=55
// EXPECT: pass_done=1

int printf(const char *fmt, ...);

// Return via comma: evaluates left, discards, returns right
int comma_return(void) {
    return (1, 2, 3, 42);
}

// Deep nested comma
int deep_comma(void) {
    return (1, (2, (3, (4, 5))));
}

int deeper_comma(void) {
    return (0, (1, (2, (3, (4, (5, (6, (7, (8, 99)))))))));
}

// Comma with side effects to verify evaluation order
int global_order[3];
int order_idx;

int set_order(int val) {
    global_order[order_idx] = val;
    order_idx++;
    return val;
}

// Comma selecting array index
int comma_arr_index(void) {
    int arr[5];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;
    // (0, 1, 2) evaluates to 2
    return arr[(0, 1, 2)];
}

// Comma in ternary branches
int ternary_comma(int cond) {
    return cond ? (10, 20) : (40, 30);
}

int main(void) {
    // Comma in for loop init and update
    int i;
    int sum = 0;
    for (i = 0; i < 10; i++) {
        sum = sum + i;
    }
    printf("for_comma=%d\n", sum);

    // Multiple comma in for init
    int a;
    int b;
    for (a = 10, b = 0; a > 5; a--, b++) {
        // empty
    }
    printf("for_multi_init=%d %d\n", a, b); // a=5 after loop? No: a starts 10, decrements each iter
    // a=10,b=0: 10>5 -> a=9,b=1
    // a=9,b=1: 9>5 -> a=8,b=2
    // a=8,b=2: 8>5 -> a=7,b=3
    // a=7,b=3: 7>5 -> a=6,b=4
    // a=6,b=4: 6>5 -> a=5,b=5
    // a=5,b=5: 5>5 is false -> done
    // So a=5, b=5... but EXPECT says "5 50". Let me fix EXPECT.

    // For multi update with comma
    int x;
    int y;
    for (x = 0, y = 0; x < 5; x++, y = y + 10) {
        // empty
    }
    printf("for_multi_update=%d %d\n", x, y);

    // Comma in while condition
    int wc = 0;
    int wv = 0;
    while (wv++, wc < 5) {
        wc++;
    }
    printf("while_comma=%d\n", wc);
    printf("while_side=%d\n", wv); // wv increments once more when wc=5 check fails
    // wv=0->1, wc=0<5->wc=1
    // wv=1->2, wc=1<5->wc=2
    // wv=2->3, wc=2<5->wc=3
    // wv=3->4, wc=3<5->wc=4
    // wv=4->5, wc=4<5->wc=5
    // wv=5->6, wc=5<5 false -> exit
    // wc=5, wv=6... fix EXPECT

    // Return comma
    printf("return_comma=%d\n", comma_return());

    // Deep nested comma
    printf("deep_nest=%d\n", deep_comma());
    printf("deeper_nest=%d\n", deeper_comma());

    // Comma as array index
    printf("arr_index_comma=%d\n", comma_arr_index());

    // Side effect order via comma
    order_idx = 0;
    int discard;
    discard = (set_order(1), set_order(2), set_order(3));
    printf("side_effect_order=%d %d %d\n",
           global_order[0], global_order[1], global_order[2]);

    // Ternary with comma
    printf("ternary_comma_t=%d\n", ternary_comma(1));
    printf("ternary_comma_f=%d\n", ternary_comma(0));

    // Comma in assignment (right side)
    int ca;
    ca = (3, 5, 7);
    printf("comma_assign=%d\n", ca);

    // Comma in function call arguments (each arg is not comma-separated
    // but we can use comma operator inside parens)
    printf("comma_in_call=%d\n", (100, 200, 300));

    // Chain of comma with side effects
    int sc = 0;
    int result;
    result = (sc = sc + 1, sc = sc + 2, sc = sc + 3, sc);
    printf("comma_chain_side=%d\n", result);

    // Comma as void expression
    (printf("comma_void=done\n"), 0);

    // Comma in if condition
    int cv = 0;
    if (cv = 10, cv > 5) {
        printf("comma_cond=%d\n", cv);
    }

    // Complex for with multiple comma
    int fi;
    int fs = 0;
    int ft = 0;
    for (fi = 0, fs = 0; fi < 10; fi++, fs = fs + fi, ft++) {
        // fs accumulates fi after increment
    }
    // fi: 0->1->2...->10
    // After fi=0: fi++=1, fs=0+1=1, ft=1
    // After fi=1: fi++=2, fs=1+2=3, ft=2
    // After fi=2: fi++=3, fs=3+3=6, ft=3
    // ...
    // After fi=9: fi++=10, fs=prev+10, ft=10
    // fs = 1+2+3+...+10 = 55
    // Hmm let me re-trace more carefully
    // init: fi=0, fs=0
    // cond: 0<10 true, body (nop), update: fi++(fi=1), fs=0+1=1, ft++(ft=1)
    // cond: 1<10 true, body (nop), update: fi++(fi=2), fs=1+2=3, ft++(ft=2)
    // cond: 2<10 true, body (nop), update: fi++(fi=3), fs=3+3=6, ft++(ft=3)
    // cond: 3<10 true, body (nop), update: fi++(fi=4), fs=6+4=10, ft++(ft=4)
    // cond: 4<10 true, body (nop), update: fi++(fi=5), fs=10+5=15, ft++(ft=5)
    // cond: 5<10 true, body (nop), update: fi++(fi=6), fs=15+6=21, ft++(ft=6)
    // cond: 6<10 true, body (nop), update: fi++(fi=7), fs=21+7=28, ft++(ft=7)
    // cond: 7<10 true, body (nop), update: fi++(fi=8), fs=28+8=36, ft++(ft=8)
    // cond: 8<10 true, body (nop), update: fi++(fi=9), fs=36+9=45, ft++(ft=9)
    // cond: 9<10 true, body (nop), update: fi++(fi=10), fs=45+10=55, ft++(ft=10)
    // cond: 10<10 false -> exit
    // fs=55
    // But I wrote EXPECT: comma_complex_for=100. Fix to 55.
    printf("comma_complex_for=%d\n", fs);

    printf("pass_done=1\n");
    return 0;
}
