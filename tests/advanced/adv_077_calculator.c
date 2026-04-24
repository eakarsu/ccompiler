// EXPECT: === Expression Calculator ===
// EXPECT: 3 + 4 = 7
// EXPECT: 10 - 3 = 7
// EXPECT: 6 * 7 = 42
// EXPECT: 100 / 4 = 25
// EXPECT: 2 + 3 * 4 = 14
// EXPECT: (2 + 3) * 4 = 20
// EXPECT: 10 - 2 * 3 = 4
// EXPECT: (10 - 2) * 3 = 24
// EXPECT: 100 / (5 * 4) = 5
// EXPECT: 1 + 2 + 3 + 4 + 5 = 15
// EXPECT: 2 * 3 + 4 * 5 = 26
// EXPECT: (1 + 2) * (3 + 4) = 21
// EXPECT: ((2 + 3) * (4 - 1)) = 15
// EXPECT: 42 = 42
// EXPECT: 0 + 0 = 0
// EXPECT: 1000 * 1000 = 1000000
// EXPECT: 999 / 3 = 333
// EXPECT: (((5))) = 5
// EXPECT: 10 - 3 - 2 = 5
// EXPECT: 100 / 10 / 2 = 5
// EXPECT: All calculator tests passed

int printf(const char *fmt, ...);

typedef struct {
    const char *input;
    int pos;
    int error;
} Parser;

void parser_init(Parser *p, const char *input) {
    p->input = input;
    p->pos = 0;
    p->error = 0;
}

char parser_peek(Parser *p) {
    return p->input[p->pos];
}

char parser_advance(Parser *p) {
    char c = p->input[p->pos];
    if (c != '\0') p->pos++;
    return c;
}

void parser_skip_spaces(Parser *p) {
    while (parser_peek(p) == ' ') {
        parser_advance(p);
    }
}

int parse_expr(Parser *p);

int parse_number(Parser *p) {
    parser_skip_spaces(p);
    int result = 0;
    int found = 0;

    while (parser_peek(p) >= '0' && parser_peek(p) <= '9') {
        result = result * 10 + (parser_peek(p) - '0');
        parser_advance(p);
        found = 1;
    }

    if (!found) {
        p->error = 1;
    }
    return result;
}

int parse_primary(Parser *p) {
    parser_skip_spaces(p);

    if (parser_peek(p) == '(') {
        parser_advance(p);
        int result = parse_expr(p);
        parser_skip_spaces(p);
        if (parser_peek(p) == ')') {
            parser_advance(p);
        } else {
            p->error = 1;
        }
        return result;
    }

    return parse_number(p);
}

int parse_term(Parser *p) {
    int left = parse_primary(p);

    while (1) {
        parser_skip_spaces(p);
        char op = parser_peek(p);
        if (op != '*' && op != '/') break;
        parser_advance(p);
        int right = parse_primary(p);
        if (op == '*') {
            left = left * right;
        } else {
            if (right == 0) {
                p->error = 1;
                return 0;
            }
            left = left / right;
        }
    }

    return left;
}

int parse_expr(Parser *p) {
    int left = parse_term(p);

    while (1) {
        parser_skip_spaces(p);
        char op = parser_peek(p);
        if (op != '+' && op != '-') break;
        parser_advance(p);
        int right = parse_term(p);
        if (op == '+') {
            left = left + right;
        } else {
            left = left - right;
        }
    }

    return left;
}

int evaluate(const char *expr) {
    Parser p;
    parser_init(&p, expr);
    int result = parse_expr(&p);
    if (p.error) return -99999;
    return result;
}

void test_eval(const char *expr, int expected) {
    int result = evaluate(expr);
    if (result == expected) {
        printf("%s = %d\n", expr, result);
    } else {
        printf("FAIL: %s expected %d got %d\n", expr, expected, result);
    }
}

int main(void) {
    printf("=== Expression Calculator ===\n");

    test_eval("3 + 4", 7);
    test_eval("10 - 3", 7);
    test_eval("6 * 7", 42);
    test_eval("100 / 4", 25);

    test_eval("2 + 3 * 4", 14);
    test_eval("(2 + 3) * 4", 20);
    test_eval("10 - 2 * 3", 4);
    test_eval("(10 - 2) * 3", 24);
    test_eval("100 / (5 * 4)", 5);

    test_eval("1 + 2 + 3 + 4 + 5", 15);
    test_eval("2 * 3 + 4 * 5", 26);
    test_eval("(1 + 2) * (3 + 4)", 21);
    test_eval("((2 + 3) * (4 - 1))", 15);

    test_eval("42", 42);
    test_eval("0 + 0", 0);
    test_eval("1000 * 1000", 1000000);
    test_eval("999 / 3", 333);
    test_eval("(((5)))", 5);

    test_eval("10 - 3 - 2", 5);
    test_eval("100 / 10 / 2", 5);

    printf("All calculator tests passed\n");
    return 0;
}
