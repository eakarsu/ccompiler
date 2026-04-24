int printf(const char *fmt, ...);

// Infix to Postfix (RPN) Conversion using Shunting-Yard Algorithm

int op_stack[64];
int op_sp;

char output[256];
int out_pos;

void reset(void) {
    op_sp = 0;
    out_pos = 0;
    output[0] = 0;
}

void op_push(int val) {
    op_stack[op_sp] = val;
    op_sp = op_sp + 1;
}

int op_pop(void) {
    op_sp = op_sp - 1;
    return op_stack[op_sp];
}

int op_peek(void) {
    if (op_sp == 0) return -1;
    return op_stack[op_sp - 1];
}

void out_char(int ch) {
    output[out_pos] = ch;
    out_pos = out_pos + 1;
    output[out_pos] = 0;
}

int is_operator(int ch) {
    if (ch == 43) return 1; // +
    if (ch == 45) return 1; // -
    if (ch == 42) return 1; // *
    if (ch == 47) return 1; // /
    return 0;
}

int precedence(int op) {
    if (op == 43 || op == 45) return 1;
    if (op == 42 || op == 47) return 2;
    return 0;
}

// Shunting-yard: convert infix to postfix
void infix_to_postfix(char *expr) {
    int i;
    int ch;
    int top;

    reset();
    i = 0;
    while (expr[i] != 0) {
        ch = expr[i];
        if (ch >= 48 && ch <= 57) {
            // digit: output directly
            out_char(ch);
        } else if (ch == 40) {
            // '(': push to stack
            op_push(ch);
        } else if (ch == 41) {
            // ')': pop until '('
            while (op_sp > 0 && op_peek() != 40) {
                out_char(32); // space
                out_char(op_pop());
            }
            if (op_sp > 0) {
                op_pop(); // discard '('
            }
        } else if (is_operator(ch)) {
            out_char(32); // space before operator output
            while (op_sp > 0 && is_operator(op_peek()) && precedence(op_peek()) >= precedence(ch)) {
                out_char(op_pop());
                out_char(32);
            }
            op_push(ch);
        }
        // skip spaces
        i = i + 1;
    }
    // pop remaining operators
    while (op_sp > 0) {
        out_char(32);
        out_char(op_pop());
    }
}

int str_equal(char *a, char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void test_convert(char *infix, char *expected) {
    infix_to_postfix(infix);
    if (str_equal(output, expected)) {
        printf("OK: %s -> %s\n", infix, output);
    } else {
        printf("FAIL: %s -> %s (expected %s)\n", infix, output, expected);
    }
}

int main(void) {
    // Simple addition: 3+4 -> 3 4 +
    test_convert("3+4", "3 4 +");
    printf("test 1 done\n"); // EXPECT: OK: 3+4 -> 3 4 +
    // EXPECT: test 1 done

    // Precedence: 2+3*4 -> 2 3 4 * +
    test_convert("2+3*4", "2 3 4 * +");
    printf("test 2 done\n"); // EXPECT: OK: 2+3*4 -> 2 3 4 * +
    // EXPECT: test 2 done

    // Parens: (2+3)*4 -> 2 3 + 4 *
    test_convert("(2+3)*4", "2 3 + 4 *");
    printf("test 3 done\n"); // EXPECT: OK: (2+3)*4 -> 2 3 + 4 *
    // EXPECT: test 3 done

    // Subtraction: 9-3-2 -> 9 3 - 2 -
    test_convert("9-3-2", "9 3 - 2 -");
    printf("test 4 done\n"); // EXPECT: OK: 9-3-2 -> 9 3 - 2 -
    // EXPECT: test 4 done

    // Mixed: 1+2*3-4 -> 1 2 3 * + 4 -
    test_convert("1+2*3-4", "1 2 3 * + 4 -");
    printf("test 5 done\n"); // EXPECT: OK: 1+2*3-4 -> 1 2 3 * + 4 -
    // EXPECT: test 5 done

    // Division: 8/4+2 -> 8 4 / 2 +
    test_convert("8/4+2", "8 4 / 2 +");
    printf("test 6 done\n"); // EXPECT: OK: 8/4+2 -> 8 4 / 2 +
    // EXPECT: test 6 done

    // Nested parens: ((1+2)) -> 1 2 +
    test_convert("((1+2))", "1 2 +");
    printf("test 7 done\n"); // EXPECT: OK: ((1+2)) -> 1 2 +
    // EXPECT: test 7 done

    // Complex: (1+2)*(3+4) -> 1 2 + 3 4 + *
    test_convert("(1+2)*(3+4)", "1 2 + 3 4 + *");
    printf("test 8 done\n"); // EXPECT: OK: (1+2)*(3+4) -> 1 2 + 3 4 + *
    // EXPECT: test 8 done

    printf("all postfix conversion tests passed\n"); // EXPECT: all postfix conversion tests passed
    return 0;
}
