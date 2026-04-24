int printf(const char *fmt, ...);
// EXPECT: === Mega 1: Ternary + Arithmetic + Comparison + FuncCall ===\nadd(3,4) = 7\nr1 = 14\ncheck false branch: r1b = 0\n=== Mega 2: Pointer Deref + Array Index + Struct Member + Arithmetic ===\n*(p+1) + s.arr[0] = 30\n*(p+2) - *(p+0) = 20\nsum via ptr = 100\n=== Mega 3: Bitwise + Shift + Cast + Ternary ===\nnibble swap 0xFF = 255\nnibble swap 0xA5 = 90\nhigh nibble of 0x37 = 3\nlow nibble of 0x37 = 7\ncombined = 115\n=== Mega 4: Nested Function Calls + Pointer + Struct ===\nmax(min(10,20), min(30,5)) = 10\nclamp(50,0,100) = 50\nclamp(-5,0,100) = 0\nclamp(200,0,100) = 100\nresult via struct ptr = 42\n=== Mega 5: Array + Loop + Compound Assignment + Comparison ===\nsum 1..10 = 55\neven sum = 30\nodd sum = 25\nproduct 1..5 = 120\nverified = 1\n=== Mega 6: Enum + Switch + Function Pointer + Struct ===\n10 + 20 = 30\n30 - 8 = 22\n7 * 6 = 42\nchain: ((5+3)*4)-2 = 30\n=== Mega 7: Pointer Arithmetic + Cast + Bitwise ===\noriginal = 67305985\nbyte[0] = 1\nbyte[1] = 2\nbyte[2] = 3\nbyte[3] = 4\nreconstructed = 67305985\nmatch = 1\n=== Mega 8: Nested Struct + Deep Access + Ternary ===\ndeep val = 42\nr8a = 84\ndeep val neg = -10\nr8b = 10\npath select = 99\ncombined depth = 141\n=== Mega 9: Do-While + Break + Compound ===\nacc (odd 1..15) = 64\nloop count = 16\ninner acc = 10\nfinal result = 74\n=== Mega 10: Recursive Function + Array + Struct ===\nfilled 6 entries\nentry[0] = 0\nentry[1] = 1\nentry[2] = 4\nentry[3] = 9\nentry[4] = 16\nentry[5] = 25\nsum of squares = 55\nfib_fill: 1 1 2 3 5 8\nfib sum = 20\n=== All mega-expression tests passed ===

// Test 629: Multi-feature mega-expression stress test
// Each section combines 5+ language features in a single expression

// Helper functions
int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul_fn(int a, int b) { return a * b; }
int max(int a, int b) { return (a > b) ? a : b; }
int min(int a, int b) { return (a < b) ? a : b; }
int clamp(int val, int lo, int hi) { return min(max(val, lo), hi); }

// Types for Mega 6: Enum + Function Pointer + Struct
enum Op { OP_ADD, OP_SUB, OP_MUL };
typedef int (*BinOp)(int, int);

struct Expr {
    enum Op op;
    int a;
    int b;
};

int eval_expr(struct Expr *e) {
    BinOp ops[3];
    ops[0] = add;
    ops[1] = sub;
    ops[2] = mul_fn;
    return ops[e->op](e->a, e->b);
}

// Types for Mega 8: 3-level nested struct
struct Inner { int val; };
struct Middle { struct Inner c; int extra; };
struct Outer { int id; struct Middle b; };

// Types for Mega 10: Recursive struct fill
struct Entry { int idx; int val; };

int fill_recursive(struct Entry *arr, int n, int max_n) {
    if (n >= max_n) return n;
    arr[n].idx = n;
    arr[n].val = n * n;
    return fill_recursive(arr, n + 1, max_n);
}

struct FibEntry { int idx; int fval; };

int fib_fill(struct FibEntry *arr, int n, int max_n, int prev1, int prev2) {
    if (n >= max_n) return n;
    int val;
    if (n == 0) {
        val = 1;
    } else if (n == 1) {
        val = 1;
    } else {
        val = prev1 + prev2;
    }
    arr[n].idx = n;
    arr[n].fval = val;
    return fib_fill(arr, n + 1, max_n, val, prev1);
}

int main() {
    // === Mega 1: Ternary + Arithmetic + Comparison + Function Call ===
    // Features: ternary, arithmetic (*), comparison (>), function call, var init
    printf("=== Mega 1: Ternary + Arithmetic + Comparison + FuncCall ===\n");
    {
        int a = add(3, 4);
        printf("add(3,4) = %d\n", a);
        // Mega-expression: ternary + comparison + function call + arithmetic
        int r1 = (add(3, 4) > 5) ? add(3, 4) * 2 : 0;
        printf("r1 = %d\n", r1);
        // False branch: add(1,2)=3, 3>5 is false, so r1b=0
        int r1b = (add(1, 2) > 5) ? add(1, 2) * 2 : 0;
        printf("check false branch: r1b = %d\n", r1b);
    }

    // === Mega 2: Pointer Deref + Array Index + Struct Member + Arithmetic ===
    // Features: struct, array member, pointer, dereference, pointer arith, add
    printf("=== Mega 2: Pointer Deref + Array Index + Struct Member + Arithmetic ===\n");
    {
        struct S2 { int arr[4]; };
        struct S2 s;
        s.arr[0] = 10; s.arr[1] = 20; s.arr[2] = 30; s.arr[3] = 40;
        int *p = s.arr;
        // Mega: pointer deref + struct member + pointer arith + addition
        int r2 = *(p + 1) + s.arr[0];
        printf("*(p+1) + s.arr[0] = %d\n", r2);
        int r2b = *(p + 2) - *(p + 0);
        printf("*(p+2) - *(p+0) = %d\n", r2b);
        // Accumulate via pointer in loop
        int sum = 0;
        int i;
        for (i = 0; i < 4; i++) {
            sum = sum + *(p + i);
        }
        printf("sum via ptr = %d\n", sum);
    }

    // === Mega 3: Bitwise + Shift + Cast + Ternary ===
    // Features: bitwise AND/OR, shift left/right, ternary, hex literals, var init
    printf("=== Mega 3: Bitwise + Shift + Cast + Ternary ===\n");
    {
        // Nibble swap: (low<<4) | (high)
        int x1 = 0xFF;
        int r3a = ((x1 & 0x0F) << 4) | ((x1 >> 4) & 0x0F);
        printf("nibble swap 0xFF = %d\n", r3a);   // 255
        // 0xA5: low=5, high=0xA=10. swap -> 0x5A = 90
        int x2 = 0xA5;
        int r3b = ((x2 & 0x0F) << 4) | ((x2 >> 4) & 0x0F);
        printf("nibble swap 0xA5 = %d\n", r3b);   // 90
        // Extract nibbles of 0x37: high=3, low=7
        int x3 = 0x37;
        int high = (x3 >> 4) & 0x0F;
        int low = x3 & 0x0F;
        printf("high nibble of 0x37 = %d\n", high);
        printf("low nibble of 0x37 = %d\n", low);
        // Ternary + bitwise: high(3) > low(7)? no -> (low<<4)|high = 0x73 = 115
        int r3c = (high > low) ? ((high << 4) | low) : ((low << 4) | high);
        printf("combined = %d\n", r3c);
    }

    // === Mega 4: Nested Function Calls + Pointer + Struct ===
    // Features: nested calls (max/min), clamp, struct, pointer-to-struct, ternary
    printf("=== Mega 4: Nested Function Calls + Pointer + Struct ===\n");
    {
        // max(min(10,20), min(30,5)) = max(10,5) = 10
        int r4 = max(min(10, 20), min(30, 5));
        printf("max(min(10,20), min(30,5)) = %d\n", r4);
        printf("clamp(50,0,100) = %d\n", clamp(50, 0, 100));
        printf("clamp(-5,0,100) = %d\n", clamp(-5, 0, 100));
        printf("clamp(200,0,100) = %d\n", clamp(200, 0, 100));
        // Struct + pointer + function call combo
        struct Pair { int x; int y; };
        struct Pair pr;
        pr.x = 17; pr.y = 25;
        struct Pair *pp = &pr;
        // max(17,25)=25, add(17,0)=17, total=42
        int r4b = max(pp->x, pp->y) + add(pp->x, 0);
        printf("result via struct ptr = %d\n", r4b);
    }

    // === Mega 5: Array + Loop + Compound Assignment + Comparison ===
    // Features: array, for loop, compound assignment (+=, *=), modulo, ternary
    printf("=== Mega 5: Array + Loop + Compound Assignment + Comparison ===\n");
    {
        int arr[10];
        int i;
        for (i = 0; i < 10; i++) arr[i] = i + 1;
        int total = 0;
        for (i = 0; i < 10; i++) total += arr[i];
        printf("sum 1..10 = %d\n", total);
        int even_sum = 0;
        int odd_sum = 0;
        for (i = 0; i < 10; i++) {
            if (arr[i] % 2 == 0) even_sum += arr[i];
            else odd_sum += arr[i];
        }
        printf("even sum = %d\n", even_sum);
        printf("odd sum = %d\n", odd_sum);
        int product = 1;
        for (i = 0; i < 5; i++) product *= arr[i];
        printf("product 1..5 = %d\n", product);
        int verified = (total == 55) ? 1 : 0;
        printf("verified = %d\n", verified);
    }

    // === Mega 6: Enum + Switch + Function Pointer + Struct ===
    // Features: enum, struct w/ enum field, function pointer typedef, fp array,
    //           indirect call via blr, chained evaluation
    printf("=== Mega 6: Enum + Switch + Function Pointer + Struct ===\n");
    {
        struct Expr e1; e1.op = OP_ADD; e1.a = 10; e1.b = 20;
        printf("10 + 20 = %d\n", eval_expr(&e1));
        struct Expr e2; e2.op = OP_SUB; e2.a = 30; e2.b = 8;
        printf("30 - 8 = %d\n", eval_expr(&e2));
        struct Expr e3; e3.op = OP_MUL; e3.a = 7; e3.b = 6;
        printf("7 * 6 = %d\n", eval_expr(&e3));
        // Chain: ((5+3)*4)-2 = (8*4)-2 = 32-2 = 30
        struct Expr step1; step1.op = OP_ADD; step1.a = 5; step1.b = 3;
        int v1 = eval_expr(&step1);
        struct Expr step2; step2.op = OP_MUL; step2.a = v1; step2.b = 4;
        int v2 = eval_expr(&step2);
        struct Expr step3; step3.op = OP_SUB; step3.a = v2; step3.b = 2;
        int v3 = eval_expr(&step3);
        printf("chain: ((5+3)*4)-2 = %d\n", v3);
    }

    // === Mega 7: Pointer Arithmetic + Cast + Bitwise ===
    // Features: pointer arith, char* cast, bitwise OR, shifts, array access, &
    printf("=== Mega 7: Pointer Arithmetic + Cast + Bitwise ===\n");
    {
        // 0x04030201 = 67305985, little-endian: bytes 01 02 03 04
        int val = 0x04030201;
        printf("original = %d\n", val);
        char *bp = (char *)&val;
        int b0 = bp[0] & 0xFF;
        int b1 = bp[1] & 0xFF;
        int b2 = bp[2] & 0xFF;
        int b3 = bp[3] & 0xFF;
        printf("byte[0] = %d\n", b0);
        printf("byte[1] = %d\n", b1);
        printf("byte[2] = %d\n", b2);
        printf("byte[3] = %d\n", b3);
        // Mega: reconstruct from bytes with shifts + OR
        int reconstructed = b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
        printf("reconstructed = %d\n", reconstructed);
        int match = (reconstructed == val) ? 1 : 0;
        printf("match = %d\n", match);
    }

    // === Mega 8: Nested Struct + Deep Access + Ternary ===
    // Features: 3-level nested struct, deep member access, ternary, mul, neg, cmp
    printf("=== Mega 8: Nested Struct + Deep Access + Ternary ===\n");
    {
        struct Outer s;
        s.id = 1; s.b.extra = 99; s.b.c.val = 42;
        printf("deep val = %d\n", s.b.c.val);
        // 42>0 true -> 42*2 = 84
        int r8a = (s.b.c.val > 0) ? s.b.c.val * 2 : -s.b.c.val;
        printf("r8a = %d\n", r8a);
        // Test negative branch: -10>0 false -> -(-10) = 10
        struct Outer s2;
        s2.id = 2; s2.b.extra = 77; s2.b.c.val = -10;
        printf("deep val neg = %d\n", s2.b.c.val);
        int r8b = (s2.b.c.val > 0) ? s2.b.c.val * 2 : -s2.b.c.val;
        printf("r8b = %d\n", r8b);
        // 1<2 true -> s.b.extra = 99
        int r8c = (s.id < s2.id) ? s.b.extra : s2.b.extra;
        printf("path select = %d\n", r8c);
        // 84 + 10 + 42 + min(77,5)=5 -> 141
        int r8d = r8a + r8b + s.b.c.val + min(s2.b.extra, 5);
        printf("combined depth = %d\n", r8d);
    }

    // === Mega 9: Do-While + Break + Compound Assignment + Nested Control ===
    // Features: do-while, break, compound assignment (+=), modulo, comparison,
    //           nested if-else, nested do-while loops
    printf("=== Mega 9: Do-While + Break + Compound ===\n");
    {
        // Sum odd numbers 1..15 using do-while + if-else + break
        int acc = 0;
        int i = 0;
        int loop_count = 0;
        do {
            i++;
            loop_count++;
            if (i > 15) break;
            if (i % 2 != 0) {
                acc += i;
            }
        } while (i <= 20);
        // Odd 1..15: 1+3+5+7+9+11+13+15 = 64, loop_count = 16 (i goes 1..16, breaks at 16)
        printf("acc (odd 1..15) = %d\n", acc);
        printf("loop count = %d\n", loop_count);

        // Nested do-while: triangular accumulation
        int inner_acc = 0;
        int j = 0;
        do {
            j++;
            if (j > 4) break;
            int k = 0;
            do {
                k++;
                if (k > j) break;
                inner_acc += 1;
            } while (k < 10);
        } while (j < 10);
        // j=1:1, j=2:2, j=3:3, j=4:4 -> inner_acc = 10
        printf("inner acc = %d\n", inner_acc);
        int final_result = acc + inner_acc;
        printf("final result = %d\n", final_result);
    }

    // === Mega 10: Recursive Function + Array + Struct ===
    // Features: recursion, struct, array of structs, pointer param, return val, loop
    printf("=== Mega 10: Recursive Function + Array + Struct ===\n");
    {
        struct Entry entries[6];
        int count = fill_recursive(entries, 0, 6);
        printf("filled %d entries\n", count);
        int i;
        for (i = 0; i < 6; i++) {
            printf("entry[%d] = %d\n", entries[i].idx, entries[i].val);
        }
        // 0+1+4+9+16+25 = 55
        int sum_sq = 0;
        for (i = 0; i < count; i++) sum_sq += entries[i].val;
        printf("sum of squares = %d\n", sum_sq);

        // Fibonacci via recursion + struct array
        struct FibEntry fibs[6];
        fib_fill(fibs, 0, 6, 0, 0);
        printf("fib_fill:");
        for (i = 0; i < 6; i++) printf(" %d", fibs[i].fval);
        printf("\n");
        // 1+1+2+3+5+8 = 20
        int fib_sum = 0;
        for (i = 0; i < 6; i++) fib_sum += fibs[i].fval;
        printf("fib sum = %d\n", fib_sum);
    }

    printf("=== All mega-expression tests passed ===\n");
    return 0;
}
