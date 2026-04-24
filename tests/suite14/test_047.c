int printf(const char *fmt, ...);

// Lambda calculus evaluator (Church numerals, simplified)
// Represent Church numerals as integers, implement combinators as functions

// Church numeral: n is represented as applying f n times to x
// church_zero(f, x) = x
// church_succ(n)(f, x) = f(n(f, x))
// church_add(m, n) = m + n
// church_mul(m, n) = m * n
// church_exp(m, n) = m^n (n applied to m)

// We represent Church numerals as plain ints and evaluate

// Combinator-style evaluation
typedef struct {
    int tag;    // 0=num, 1=succ, 2=add, 3=mul, 4=pred, 5=iszero
    int val;
    int arg1;
    int arg2;
} Term;

// Heap for terms
Term heap[256];
int heap_top;

void heap_init(void) {
    heap_top = 0;
    int i = 0;
    while (i < 256) {
        heap[i].tag = 0;
        heap[i].val = 0;
        heap[i].arg1 = 0;
        heap[i].arg2 = 0;
        i = i + 1;
    }
}

int make_num(int n) {
    int idx = heap_top;
    heap_top = heap_top + 1;
    heap[idx].tag = 0;
    heap[idx].val = n;
    return idx;
}

int make_succ(int arg) {
    int idx = heap_top;
    heap_top = heap_top + 1;
    heap[idx].tag = 1;
    heap[idx].arg1 = arg;
    return idx;
}

int make_add(int a, int b) {
    int idx = heap_top;
    heap_top = heap_top + 1;
    heap[idx].tag = 2;
    heap[idx].arg1 = a;
    heap[idx].arg2 = b;
    return idx;
}

int make_mul(int a, int b) {
    int idx = heap_top;
    heap_top = heap_top + 1;
    heap[idx].tag = 3;
    heap[idx].arg1 = a;
    heap[idx].arg2 = b;
    return idx;
}

int make_pred(int arg) {
    int idx = heap_top;
    heap_top = heap_top + 1;
    heap[idx].tag = 4;
    heap[idx].arg1 = arg;
    return idx;
}

int make_iszero(int arg) {
    int idx = heap_top;
    heap_top = heap_top + 1;
    heap[idx].tag = 5;
    heap[idx].arg1 = arg;
    return idx;
}

// Evaluate a term to a number
int eval(int idx) {
    Term *t = &heap[idx];
    int a;
    int b;

    if (t->tag == 0) {
        return t->val;
    } else if (t->tag == 1) {
        a = eval(t->arg1);
        return a + 1;
    } else if (t->tag == 2) {
        a = eval(t->arg1);
        b = eval(t->arg2);
        return a + b;
    } else if (t->tag == 3) {
        a = eval(t->arg1);
        b = eval(t->arg2);
        return a * b;
    } else if (t->tag == 4) {
        a = eval(t->arg1);
        if (a > 0) return a - 1;
        return 0;
    } else if (t->tag == 5) {
        a = eval(t->arg1);
        if (a == 0) return 1;
        return 0;
    }
    return 0;
}

// Church numeral operations directly on ints
int church_to_int(int n) { return n; }
int church_succ(int n) { return n + 1; }
int church_add(int m, int n) { return m + n; }
int church_mul(int m, int n) { return m * n; }
int church_pred(int n) { if (n > 0) return n - 1; return 0; }
int church_iszero(int n) { return n == 0 ? 1 : 0; }

// Church power: m^n
int church_pow(int m, int n) {
    int result = 1;
    int i = 0;
    while (i < n) {
        result = result * m;
        i = i + 1;
    }
    return result;
}

// Apply f n times to x (Church numeral application)
int church_apply(int n, int x, int op) {
    // op: 0=increment, 1=double, 2=square
    int result = x;
    int i = 0;
    while (i < n) {
        if (op == 0) {
            result = result + 1;
        } else if (op == 1) {
            result = result * 2;
        } else if (op == 2) {
            result = result * result;
        }
        i = i + 1;
    }
    return result;
}

// Fibonacci using Church-style repeated application
int fib(int n) {
    int a = 0;
    int b = 1;
    int i = 0;
    int tmp;
    while (i < n) {
        tmp = a + b;
        a = b;
        b = tmp;
        i = i + 1;
    }
    return a;
}

// SKI combinator calculus simulation
// S x y z = x z (y z)
// K x y = x
// I x = x
// We simulate with integers
int ski_I(int x) { return x; }
int ski_K(int x, int y) { return x; }

int main(void) {
    int result;

    // Test 1: Church zero
    printf("t1: %d\n", church_to_int(0));
    // EXPECT: t1: 0

    // Test 2: Church successor
    printf("t2: %d\n", church_succ(0));
    // EXPECT: t2: 1

    // Test 3: Church add
    printf("t3: %d\n", church_add(3, 4));
    // EXPECT: t3: 7

    // Test 4: Church multiply
    printf("t4: %d\n", church_mul(3, 4));
    // EXPECT: t4: 12

    // Test 5: Church power 2^10 = 1024
    printf("t5: %d\n", church_pow(2, 10));
    // EXPECT: t5: 1024

    // Test 6: Church predecessor
    printf("t6: %d\n", church_pred(5));
    // EXPECT: t6: 4

    // Test 7: Church iszero
    printf("t7: %d %d\n", church_iszero(0), church_iszero(3));
    // EXPECT: t7: 1 0

    // Test 8: Apply increment 5 times to 0
    printf("t8: %d\n", church_apply(5, 0, 0));
    // EXPECT: t8: 5

    // Test 9: Apply double 4 times to 1 => 1*2*2*2*2 = 16
    printf("t9: %d\n", church_apply(4, 1, 1));
    // EXPECT: t9: 16

    // Test 10: Fibonacci via Church-style
    printf("t10: %d\n", fib(10));
    // EXPECT: t10: 55

    // Test 11: SKI combinators
    printf("t11: I=%d K=%d\n", ski_I(42), ski_K(7, 99));
    // EXPECT: t11: I=42 K=7

    // Test 12: Term evaluation - number
    heap_init();
    int t_five = make_num(5);
    printf("t12: %d\n", eval(t_five));
    // EXPECT: t12: 5

    // Test 13: Term evaluation - succ(succ(3)) = 5
    heap_init();
    int t3 = make_num(3);
    int ts3 = make_succ(t3);
    int tss3 = make_succ(ts3);
    printf("t13: %d\n", eval(tss3));
    // EXPECT: t13: 5

    // Test 14: Term evaluation - add(mul(2,3), 4) = 10
    heap_init();
    int t2 = make_num(2);
    t3 = make_num(3);
    int t4 = make_num(4);
    int tmul = make_mul(t2, t3);
    int tadd = make_add(tmul, t4);
    printf("t14: %d\n", eval(tadd));
    // EXPECT: t14: 10

    // Test 15: Term evaluation - pred(iszero(0)) should be pred(1) = 0
    heap_init();
    int t0 = make_num(0);
    int tiz = make_iszero(t0);
    int tpred = make_pred(tiz);
    printf("t15: %d\n", eval(tpred));
    // EXPECT: t15: 0

    // Test 16: Complex term: add(succ(2), mul(3, pred(5)))
    // = add(3, mul(3, 4)) = add(3, 12) = 15
    heap_init();
    t2 = make_num(2);
    t3 = make_num(3);
    int t5 = make_num(5);
    int tsucc2 = make_succ(t2);           // 3
    int tpred5 = make_pred(t5);           // 4
    int t3b = make_num(3);
    int tmul34 = make_mul(t3b, tpred5);   // 12
    int tresult = make_add(tsucc2, tmul34); // 15
    printf("t16: %d\n", eval(tresult));
    // EXPECT: t16: 15

    printf("lambda calc done\n");
    // EXPECT: lambda calc done

    return 0;
}
