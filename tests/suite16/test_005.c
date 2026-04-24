int printf(const char *fmt, ...);

// Arithmetic interpreter with variable storage
// Variables a-z (26 variables), assignment, expression evaluation

int vars[26];

void vars_init(void) {
    int i;
    for (i = 0; i < 26; i = i + 1) {
        vars[i] = 0;
    }
}

int var_get(int name) {
    // name is 'a'-'z' (97-122)
    return vars[name - 97];
}

void var_set(int name, int val) {
    vars[name - 97] = val;
}

// Simple tokenizer/evaluator for statements like:
// "a=5" or "b=a+3" or "a+b*c"
// Only single-letter variables and single-digit constants

char prog[256];
int pos;

void set_prog(char *s) {
    int i;
    i = 0;
    while (s[i] != 0) {
        prog[i] = s[i];
        i = i + 1;
    }
    prog[i] = 0;
    pos = 0;
}

int peek(void) {
    while (prog[pos] == 32) pos = pos + 1;
    return prog[pos];
}

int advance(void) {
    int ch;
    while (prog[pos] == 32) pos = pos + 1;
    ch = prog[pos];
    pos = pos + 1;
    return ch;
}

int is_alpha(int ch) {
    return ch >= 97 && ch <= 122;
}

int is_digit(int ch) {
    return ch >= 48 && ch <= 57;
}

int eval_expr(void);

int eval_atom(void) {
    int ch;
    int num;
    ch = peek();
    if (ch == 40) {
        // '('
        advance();
        num = eval_expr();
        if (peek() == 41) advance(); // ')'
        return num;
    }
    if (is_alpha(ch)) {
        advance();
        return var_get(ch);
    }
    // number
    num = 0;
    while (is_digit(peek())) {
        ch = advance();
        num = num * 10 + (ch - 48);
    }
    return num;
}

int eval_term(void) {
    int left;
    int op;
    left = eval_atom();
    while (peek() == 42 || peek() == 47) {
        op = advance();
        if (op == 42)
            left = left * eval_atom();
        else
            left = left / eval_atom();
    }
    return left;
}

int eval_expr(void) {
    int left;
    int op;
    left = eval_term();
    while (peek() == 43 || peek() == 45) {
        op = advance();
        if (op == 43)
            left = left + eval_term();
        else
            left = left - eval_term();
    }
    return left;
}

// Execute a statement: either "var=expr" or just "expr" (returns value)
int exec_stmt(char *s) {
    int ch;
    int val;
    set_prog(s);

    ch = peek();
    // Check if it's an assignment: letter followed by '='
    if (is_alpha(ch)) {
        int saved_pos;
        int var_name;
        saved_pos = pos;
        var_name = advance();
        if (peek() == 61) {
            // '='
            advance();
            val = eval_expr();
            var_set(var_name, val);
            return val;
        }
        // Not assignment, reset and evaluate as expression
        pos = saved_pos;
    }
    return eval_expr();
}

int main(void) {
    int r;

    vars_init();

    // Simple assignment
    r = exec_stmt("a=5");
    printf("a=5 -> %d\n", r); // EXPECT: a=5 -> 5

    r = exec_stmt("b=3");
    printf("b=3 -> %d\n", r); // EXPECT: b=3 -> 3

    // Expression with variables
    r = exec_stmt("a+b");
    printf("a+b = %d\n", r); // EXPECT: a+b = 8

    r = exec_stmt("a*b");
    printf("a*b = %d\n", r); // EXPECT: a*b = 15

    r = exec_stmt("a-b");
    printf("a-b = %d\n", r); // EXPECT: a-b = 2

    // Assignment with expression
    r = exec_stmt("c=a+b*2");
    printf("c=a+b*2 -> %d\n", r); // EXPECT: c=a+b*2 -> 11

    // Use the new variable
    r = exec_stmt("c");
    printf("c = %d\n", r); // EXPECT: c = 11

    // Chained computation
    r = exec_stmt("d=c-a");
    printf("d=c-a -> %d\n", r); // EXPECT: d=c-a -> 6

    r = exec_stmt("e=d*d");
    printf("e=d*d -> %d\n", r); // EXPECT: e=d*d -> 36

    // Complex expression
    r = exec_stmt("a+b+c+d+e");
    printf("sum = %d\n", r); // EXPECT: sum = 61

    // Parenthesized expression
    r = exec_stmt("(a+b)*(c-d)");
    printf("(a+b)*(c-d) = %d\n", r); // EXPECT: (a+b)*(c-d) = 40

    // Reassignment
    r = exec_stmt("a=100");
    printf("a=100 -> %d\n", r); // EXPECT: a=100 -> 100

    r = exec_stmt("a/b");
    printf("a/b = %d\n", r); // EXPECT: a/b = 33

    // Multi-digit constants
    r = exec_stmt("f=42");
    printf("f=42 -> %d\n", r); // EXPECT: f=42 -> 42

    r = exec_stmt("f+8");
    printf("f+8 = %d\n", r); // EXPECT: f+8 = 50

    printf("all variable interpreter tests passed\n"); // EXPECT: all variable interpreter tests passed
    return 0;
}
