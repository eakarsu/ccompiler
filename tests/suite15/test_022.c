int printf(const char *fmt, ...);

// Recursive descent parser for arithmetic expressions
// Grammar: expr -> term (('+' | '-') term)*
//          term -> factor (('*' | '/') factor)*
//          factor -> NUM | '(' expr ')'

struct Parser {
    char input[128];
    int pos;
    int len;
    int error;
};

void parser_init(struct Parser *p, char *src) {
    int i = 0;
    while (src[i] != 0) {
        p->input[i] = src[i];
        i = i + 1;
    }
    p->input[i] = 0;
    p->len = i;
    p->pos = 0;
    p->error = 0;
}

void skip_spaces(struct Parser *p) {
    while (p->pos < p->len && p->input[p->pos] == ' ') {
        p->pos = p->pos + 1;
    }
}

char peek(struct Parser *p) {
    skip_spaces(p);
    if (p->pos >= p->len) return 0;
    return p->input[p->pos];
}

char advance(struct Parser *p) {
    skip_spaces(p);
    if (p->pos >= p->len) return 0;
    char c = p->input[p->pos];
    p->pos = p->pos + 1;
    return c;
}

int parse_expr(struct Parser *p);

int parse_number(struct Parser *p) {
    skip_spaces(p);
    int num = 0;
    int found = 0;
    int neg = 0;

    if (p->pos < p->len && p->input[p->pos] == '-') {
        // Check if it's unary minus (at start or after operator/paren)
        neg = 1;
        p->pos = p->pos + 1;
    }

    while (p->pos < p->len && p->input[p->pos] >= '0' && p->input[p->pos] <= '9') {
        num = num * 10 + (p->input[p->pos] - '0');
        p->pos = p->pos + 1;
        found = 1;
    }
    if (!found) {
        p->error = 1;
        return 0;
    }
    if (neg) num = -num;
    return num;
}

int parse_factor(struct Parser *p) {
    char c = peek(p);
    if (c == '(') {
        advance(p); // consume '('
        int val = parse_expr(p);
        if (peek(p) == ')') {
            advance(p); // consume ')'
        } else {
            p->error = 1;
        }
        return val;
    }
    return parse_number(p);
}

int parse_term(struct Parser *p) {
    int left = parse_factor(p);
    while (1) {
        char c = peek(p);
        if (c == '*') {
            advance(p);
            int right = parse_factor(p);
            left = left * right;
        } else if (c == '/') {
            advance(p);
            int right = parse_factor(p);
            if (right != 0) {
                left = left / right;
            } else {
                p->error = 1;
                return 0;
            }
        } else {
            break;
        }
    }
    return left;
}

int parse_expr(struct Parser *p) {
    int left = parse_term(p);
    while (1) {
        char c = peek(p);
        if (c == '+') {
            advance(p);
            int right = parse_term(p);
            left = left + right;
        } else if (c == '-') {
            advance(p);
            int right = parse_term(p);
            left = left - right;
        } else {
            break;
        }
    }
    return left;
}

int eval(char *src) {
    struct Parser p;
    parser_init(&p, src);
    int result = parse_expr(&p);
    if (p.error) return -9999;
    return result;
}

int check_error(char *src) {
    struct Parser p;
    parser_init(&p, src);
    parse_expr(&p);
    return p.error;
}

int main() {
    printf("=== Parser Test 1: Simple ops ===\n");
    // EXPECT: === Parser Test 1: Simple ops ===
    printf("2+3 = %d\n", eval("2+3"));
    // EXPECT: 2+3 = 5
    printf("10-4 = %d\n", eval("10-4"));
    // EXPECT: 10-4 = 6
    printf("6*7 = %d\n", eval("6*7"));
    // EXPECT: 6*7 = 42
    printf("20/4 = %d\n", eval("20/4"));
    // EXPECT: 20/4 = 5

    printf("=== Parser Test 2: Precedence ===\n");
    // EXPECT: === Parser Test 2: Precedence ===
    printf("2+3*4 = %d\n", eval("2+3*4"));
    // EXPECT: 2+3*4 = 14
    printf("10-2*3 = %d\n", eval("10-2*3"));
    // EXPECT: 10-2*3 = 4
    printf("2*3+4*5 = %d\n", eval("2*3+4*5"));
    // EXPECT: 2*3+4*5 = 26
    printf("100/10/2 = %d\n", eval("100/10/2"));
    // EXPECT: 100/10/2 = 5

    printf("=== Parser Test 3: Parentheses ===\n");
    // EXPECT: === Parser Test 3: Parentheses ===
    printf("(2+3)*4 = %d\n", eval("(2+3)*4"));
    // EXPECT: (2+3)*4 = 20
    printf("(10-2)*(3+1) = %d\n", eval("(10-2)*(3+1)"));
    // EXPECT: (10-2)*(3+1) = 32
    printf("((2+3)) = %d\n", eval("((2+3))"));
    // EXPECT: ((2+3)) = 5
    printf("(1+2)*(3+4)*(1+1) = %d\n", eval("(1+2)*(3+4)*(1+1)"));
    // EXPECT: (1+2)*(3+4)*(1+1) = 42

    printf("=== Parser Test 4: Larger expressions ===\n");
    // EXPECT: === Parser Test 4: Larger expressions ===
    printf("1+2+3+4+5 = %d\n", eval("1+2+3+4+5"));
    // EXPECT: 1+2+3+4+5 = 15
    printf("10*10+1 = %d\n", eval("10*10+1"));
    // EXPECT: 10*10+1 = 101
    printf("(5+5)*(5+5) = %d\n", eval("(5+5)*(5+5)"));
    // EXPECT: (5+5)*(5+5) = 100

    printf("=== Parser Test 5: Spaces ===\n");
    // EXPECT: === Parser Test 5: Spaces ===
    printf("3 + 4 = %d\n", eval("3 + 4"));
    // EXPECT: 3 + 4 = 7
    printf(" 2 * (3 + 5) = %d\n", eval(" 2 * (3 + 5)"));
    // EXPECT:  2 * (3 + 5) = 16

    printf("=== Parser Test 6: Complex nested ===\n");
    // EXPECT: === Parser Test 6: Complex nested ===
    printf("(2+3)*(4+5)-(1+1) = %d\n", eval("(2+3)*(4+5)-(1+1)"));
    // EXPECT: (2+3)*(4+5)-(1+1) = 43
    printf("50/(2+3) = %d\n", eval("50/(2+3)"));
    // EXPECT: 50/(2+3) = 10

    printf("done\n");
    // EXPECT: done
    return 0;
}
