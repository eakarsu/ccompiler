int printf(const char *fmt, ...);
// EXPECT: === 1. Array of function pointers ===\nops[0](10,3) = 13\nops[1](10,3) = 7\nops[2](10,3) = 30\nops[3](10,3) = 3\n=== 2. Callback pattern ===\napply(add,5,3) = 8\napply(sub,5,3) = 2\napply(mul,5,3) = 15\n=== 3. Typedef function pointer ===\nop=add: op(20,7) = 27\nop=mul: op(20,7) = 140\n=== 4. Struct return via function pointer ===\ndirect: x=3 y=4\nvia fp: x=5 y=6\n=== 5. Dispatch table ===\ndispatch(0,100,25) = 125\ndispatch(1,100,25) = 75\ndispatch(2,100,25) = 2500\ndispatch(3,100,25) = 4\n=== 6. Conditional fp assignment ===\nflag=1: op(30,12) = 42\nflag=0: op(30,12) = 18\n=== 7. Nested fp calls ===\nadd(mul(3,4),sub(10,3)) = 19\nmul(add(2,3),add(4,1)) = 25\n=== 8. Array iteration ===\ni=0: ops(20,4) = 24\ni=1: ops(20,4) = 16\ni=2: ops(20,4) = 80\ni=3: ops(20,4) = 5\nsum = 125\n=== 9. Function pointer comparison ===\nadd==add: 1\nadd==sub: 0\np1==p2 (both add): 1\np1==p2 (add,sub): 0\n=== 10. Struct with fp field ===\nacc after add 10: 10\nacc after add 20: 30\nacc after mul 3: 90\nvia ptr add 5: 95\n=== 11. Array of structs with fp ===\nop 0: result = 10\nop 1: result = 4\nop 2: result = 21\n=== 12. Chained dispatch ===\nafter sub 30: 70\nafter mul 2: 140\nafter add 10: 150\nafter div 3: 50\n=== 13. FP in conditional expression ===\nternary add: 30\nternary sub: 10\n=== 14. Double indirection ===\nselect(0)(8,2) = 10\nselect(1)(8,2) = 6\nselect(2)(8,2) = 16\nselect(3)(8,2) = 4\n=== 15. Unary fp array ===\nneg(7) = -7\nsquare(7) = 49\ndouble(7) = 14\nidentity(7) = 7\nDone.
// Test 611: Complex function pointer patterns (blr x16 stress test)

// ===== Basic arithmetic functions =====

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int div_fn(int a, int b) { return a / b; }

// ===== Unary functions =====

int neg(int a) { return -a; }
int square(int a) { return a * a; }
int double_val(int a) { return a * 2; }
int identity(int a) { return a; }

// ===== Typedef =====

typedef int (*BinOp)(int, int);
typedef int (*UnaryOp)(int);

// ===== Struct for section 4 =====

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

// ===== Callback: function pointer as parameter =====

int apply(BinOp fn, int a, int b) {
    return fn(a, b);
}

// ===== Dispatch table function =====

int dispatch(int op_index, int a, int b) {
    BinOp table[4];
    table[0] = add;
    table[1] = sub;
    table[2] = mul;
    table[3] = div_fn;
    return table[op_index](a, b);
}

// ===== Function that returns a function pointer =====

BinOp select_op(int index) {
    if (index == 0) return add;
    if (index == 1) return sub;
    if (index == 2) return mul;
    return div_fn;
}

// ===== Struct containing function pointer =====

struct Calculator {
    BinOp op;
    int acc;
};

// ===== Struct for named operations =====

struct NamedOp {
    BinOp op;
    int id;
};

// ===== Main =====

int main() {
    int i;
    int result;
    int sum;

    // === 1. Array of function pointers ===
    printf("=== 1. Array of function pointers ===\n");
    {
        int (*ops[4])(int, int);
        ops[0] = add;
        ops[1] = sub;
        ops[2] = mul;
        ops[3] = div_fn;

        printf("ops[0](10,3) = %d\n", ops[0](10, 3));
        printf("ops[1](10,3) = %d\n", ops[1](10, 3));
        printf("ops[2](10,3) = %d\n", ops[2](10, 3));
        printf("ops[3](10,3) = %d\n", ops[3](10, 3));
    }

    // === 2. Callback pattern ===
    printf("=== 2. Callback pattern ===\n");
    {
        printf("apply(add,5,3) = %d\n", apply(add, 5, 3));
        printf("apply(sub,5,3) = %d\n", apply(sub, 5, 3));
        printf("apply(mul,5,3) = %d\n", apply(mul, 5, 3));
    }

    // === 3. Typedef function pointer ===
    printf("=== 3. Typedef function pointer ===\n");
    {
        BinOp op;
        op = add;
        printf("op=add: op(20,7) = %d\n", op(20, 7));
        op = mul;
        printf("op=mul: op(20,7) = %d\n", op(20, 7));
    }

    // === 4. Struct return via function pointer ===
    printf("=== 4. Struct return via function pointer ===\n");
    {
        struct Point p1;
        struct Point p2;
        struct Point (*fp_make)(int, int);

        p1 = make_point(3, 4);
        printf("direct: x=%d y=%d\n", p1.x, p1.y);

        fp_make = make_point;
        p2 = fp_make(5, 6);
        printf("via fp: x=%d y=%d\n", p2.x, p2.y);
    }

    // === 5. Dispatch table ===
    printf("=== 5. Dispatch table ===\n");
    {
        printf("dispatch(0,100,25) = %d\n", dispatch(0, 100, 25));
        printf("dispatch(1,100,25) = %d\n", dispatch(1, 100, 25));
        printf("dispatch(2,100,25) = %d\n", dispatch(2, 100, 25));
        printf("dispatch(3,100,25) = %d\n", dispatch(3, 100, 25));
    }

    // === 6. Conditional fp assignment ===
    printf("=== 6. Conditional fp assignment ===\n");
    {
        BinOp op;
        int flag;

        flag = 1;
        op = (flag) ? add : sub;
        printf("flag=1: op(30,12) = %d\n", op(30, 12));

        flag = 0;
        op = (flag) ? add : sub;
        printf("flag=0: op(30,12) = %d\n", op(30, 12));
    }

    // === 7. Nested fp calls ===
    printf("=== 7. Nested fp calls ===\n");
    {
        // add(mul(3,4), sub(10,3)) = add(12, 7) = 19
        result = apply(add, apply(mul, 3, 4), apply(sub, 10, 3));
        printf("add(mul(3,4),sub(10,3)) = %d\n", result);

        // mul(add(2,3), add(4,1)) = mul(5, 5) = 25
        result = apply(mul, apply(add, 2, 3), apply(add, 4, 1));
        printf("mul(add(2,3),add(4,1)) = %d\n", result);
    }

    // === 8. Array iteration ===
    printf("=== 8. Array iteration ===\n");
    {
        BinOp ops[4];
        ops[0] = add;
        ops[1] = sub;
        ops[2] = mul;
        ops[3] = div_fn;

        sum = 0;
        for (i = 0; i < 4; i = i + 1) {
            result = ops[i](20, 4);
            printf("i=%d: ops(20,4) = %d\n", i, result);
            sum = sum + result;
        }
        printf("sum = %d\n", sum);
    }

    // === 9. Function pointer comparison ===
    printf("=== 9. Function pointer comparison ===\n");
    {
        BinOp p1;
        BinOp p2;

        printf("add==add: %d\n", (add == add) ? 1 : 0);
        printf("add==sub: %d\n", (add == sub) ? 1 : 0);

        p1 = add;
        p2 = add;
        printf("p1==p2 (both add): %d\n", (p1 == p2) ? 1 : 0);

        p2 = sub;
        printf("p1==p2 (add,sub): %d\n", (p1 == p2) ? 1 : 0);
    }

    // === 10. Struct with fp field ===
    printf("=== 10. Struct with fp field ===\n");
    {
        struct Calculator calc;
        struct Calculator *cp;

        calc.op = add;
        calc.acc = 0;

        calc.acc = calc.op(calc.acc, 10);
        printf("acc after add 10: %d\n", calc.acc);

        calc.acc = calc.op(calc.acc, 20);
        printf("acc after add 20: %d\n", calc.acc);

        calc.op = mul;
        calc.acc = calc.op(calc.acc, 3);
        printf("acc after mul 3: %d\n", calc.acc);

        calc.op = add;
        cp = &calc;
        cp->acc = cp->op(cp->acc, 5);
        printf("via ptr add 5: %d\n", cp->acc);
    }

    // === 11. Array of structs with fp ===
    printf("=== 11. Array of structs with fp ===\n");
    {
        struct NamedOp operations[3];

        operations[0].op = add;
        operations[0].id = 0;
        operations[1].op = sub;
        operations[1].id = 1;
        operations[2].op = mul;
        operations[2].id = 2;

        for (i = 0; i < 3; i = i + 1) {
            result = operations[i].op(7, 3);
            printf("op %d: result = %d\n", operations[i].id, result);
        }
    }

    // === 12. Chained dispatch ===
    printf("=== 12. Chained dispatch ===\n");
    {
        int val;
        int chain_ops[4];
        int chain_args[4];
        BinOp chain_fns[4];

        chain_fns[0] = sub;
        chain_fns[1] = mul;
        chain_fns[2] = add;
        chain_fns[3] = div_fn;

        chain_args[0] = 30;
        chain_args[1] = 2;
        chain_args[2] = 10;
        chain_args[3] = 3;

        val = 100;

        val = chain_fns[0](val, chain_args[0]);
        printf("after sub 30: %d\n", val);

        val = chain_fns[1](val, chain_args[1]);
        printf("after mul 2: %d\n", val);

        val = chain_fns[2](val, chain_args[2]);
        printf("after add 10: %d\n", val);

        val = chain_fns[3](val, chain_args[3]);
        printf("after div 3: %d\n", val);
    }

    // === 13. FP in conditional expression ===
    printf("=== 13. FP in conditional expression ===\n");
    {
        int x;
        int y;
        x = 20;
        y = 10;

        // Ternary selecting fp then calling
        result = ((1) ? add : sub)(x, y);
        printf("ternary add: %d\n", result);

        result = ((0) ? add : sub)(x, y);
        printf("ternary sub: %d\n", result);
    }

    // === 14. Double indirection: function returning fp ===
    printf("=== 14. Double indirection ===\n");
    {
        BinOp chosen;

        chosen = select_op(0);
        printf("select(0)(8,2) = %d\n", chosen(8, 2));

        chosen = select_op(1);
        printf("select(1)(8,2) = %d\n", chosen(8, 2));

        chosen = select_op(2);
        printf("select(2)(8,2) = %d\n", chosen(8, 2));

        chosen = select_op(3);
        printf("select(3)(8,2) = %d\n", chosen(8, 2));
    }

    // === 15. Unary fp array ===
    printf("=== 15. Unary fp array ===\n");
    {
        UnaryOp ufns[4];

        ufns[0] = neg;
        ufns[1] = square;
        ufns[2] = double_val;
        ufns[3] = identity;

        printf("neg(7) = %d\n", ufns[0](7));
        printf("square(7) = %d\n", ufns[1](7));
        printf("double(7) = %d\n", ufns[2](7));
        printf("identity(7) = %d\n", ufns[3](7));
    }

    printf("Done.\n");
    return 0;
}
