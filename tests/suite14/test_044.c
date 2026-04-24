int printf(const char *fmt, ...);

// Postfix notation evaluator with variables (a-z mapped to 0-25)

typedef struct {
    int stack[64];
    int sp;
    int vars[26];
} PostfixEval;

void pf_init(PostfixEval *e) {
    e->sp = 0;
    int i = 0;
    while (i < 64) { e->stack[i] = 0; i = i + 1; }
    i = 0;
    while (i < 26) { e->vars[i] = 0; i = i + 1; }
}

void pf_push(PostfixEval *e, int val) {
    e->stack[e->sp] = val;
    e->sp = e->sp + 1;
}

int pf_pop(PostfixEval *e) {
    e->sp = e->sp - 1;
    return e->stack[e->sp];
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_lower(char c) {
    return c >= 'a' && c <= 'z';
}

// Evaluate a postfix expression string
// Digits 0-9 push single digit values
// a-z push variable values
// + - * / % are operators
// = pops value and variable index (uses previous char stored), assigns
// Uppercase A-Z: assign top of stack to var (A=0, B=1, ...)
int pf_eval(PostfixEval *e, char *expr) {
    int i = 0;
    int a;
    int b;

    while (expr[i] != 0) {
        char c = expr[i];

        if (is_digit(c)) {
            // Parse multi-digit: keep reading digits
            int num = 0;
            while (expr[i] != 0 && is_digit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i = i + 1;
            }
            pf_push(e, num);
            // don't increment i again
            continue;
        } else if (is_lower(c)) {
            pf_push(e, e->vars[c - 'a']);
            i = i + 1;
        } else if (c == '+') {
            b = pf_pop(e);
            a = pf_pop(e);
            pf_push(e, a + b);
            i = i + 1;
        } else if (c == '-') {
            b = pf_pop(e);
            a = pf_pop(e);
            pf_push(e, a - b);
            i = i + 1;
        } else if (c == '*') {
            b = pf_pop(e);
            a = pf_pop(e);
            pf_push(e, a * b);
            i = i + 1;
        } else if (c == '/') {
            b = pf_pop(e);
            a = pf_pop(e);
            pf_push(e, a / b);
            i = i + 1;
        } else if (c == '%') {
            b = pf_pop(e);
            a = pf_pop(e);
            pf_push(e, a % b);
            i = i + 1;
        } else if (c >= 'A' && c <= 'Z') {
            // Store top of stack into variable
            a = pf_pop(e);
            e->vars[c - 'A'] = a;
            i = i + 1;
        } else {
            // space or other: skip
            i = i + 1;
        }
    }

    if (e->sp > 0) {
        return pf_pop(e);
    }
    return 0;
}

int main(void) {
    PostfixEval e;
    int result;

    // Test 1: 3 4 + = 7
    pf_init(&e);
    result = pf_eval(&e, "3 4+");
    printf("t1: %d\n", result);
    // EXPECT: t1: 7

    // Test 2: 5 3 - = 2
    pf_init(&e);
    result = pf_eval(&e, "5 3-");
    printf("t2: %d\n", result);
    // EXPECT: t2: 2

    // Test 3: 6 7 * = 42
    pf_init(&e);
    result = pf_eval(&e, "6 7*");
    printf("t3: %d\n", result);
    // EXPECT: t3: 42

    // Test 4: 20 4 / = 5
    pf_init(&e);
    result = pf_eval(&e, "20 4/");
    printf("t4: %d\n", result);
    // EXPECT: t4: 5

    // Test 5: 17 5 % = 2
    pf_init(&e);
    result = pf_eval(&e, "17 5%");
    printf("t5: %d\n", result);
    // EXPECT: t5: 2

    // Test 6: (3 + 4) * 2 = 14 => postfix "3 4 + 2 *"
    pf_init(&e);
    result = pf_eval(&e, "3 4+ 2*");
    printf("t6: %d\n", result);
    // EXPECT: t6: 14

    // Test 7: 2 3 + 4 5 + * = 5 * 9 = 45 => postfix "2 3 + 4 5 + *"
    pf_init(&e);
    result = pf_eval(&e, "2 3+ 4 5+*");
    printf("t7: %d\n", result);
    // EXPECT: t7: 45

    // Test 8: Store into variable and retrieve
    // "10 A" stores 10 into var[0], then "a 5 +" retrieves var[0]=10, adds 5
    pf_init(&e);
    pf_eval(&e, "10A");
    result = pf_eval(&e, "a 5+");
    printf("t8: %d\n", result);
    // EXPECT: t8: 15

    // Test 9: Multiple variables
    // Store 3 in A, 7 in B, compute a*b
    pf_init(&e);
    pf_eval(&e, "3A");
    pf_eval(&e, "7B");
    result = pf_eval(&e, "a b*");
    printf("t9: %d\n", result);
    // EXPECT: t9: 21

    // Test 10: Complex expression with variables
    // A=4, B=5, C=A+B=9, then C*C = 81
    pf_init(&e);
    pf_eval(&e, "4A");
    pf_eval(&e, "5B");
    pf_eval(&e, "a b+C");  // C = 9
    result = pf_eval(&e, "c c*");
    printf("t10: %d\n", result);
    // EXPECT: t10: 81

    // Test 11: Multi-digit numbers
    pf_init(&e);
    result = pf_eval(&e, "100 23+");
    printf("t11: %d\n", result);
    // EXPECT: t11: 123

    // Test 12: Chain of operations
    // 10 2 + 3 * 4 - = (10+2)*3 - 4 = 36-4 = 32
    pf_init(&e);
    result = pf_eval(&e, "10 2+ 3* 4-");
    printf("t12: %d\n", result);
    // EXPECT: t12: 32

    // Test 13: Variable accumulation
    // A=0, then A = A+10 three times => 30
    pf_init(&e);
    pf_eval(&e, "0A");
    pf_eval(&e, "a 10+A");
    pf_eval(&e, "a 10+A");
    pf_eval(&e, "a 10+A");
    printf("t13: %d\n", e.vars[0]);
    // EXPECT: t13: 30

    printf("postfix eval done\n");
    // EXPECT: postfix eval done

    return 0;
}
