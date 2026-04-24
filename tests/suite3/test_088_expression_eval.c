int printf(const char *fmt, ...);

/* ---- Postfix (RPN) expression evaluator using a stack ---- */

#define STACK_MAX 32

typedef struct {
    int data[STACK_MAX];
    int top;
} Stack;

void stack_init(Stack *s) { s->top = -1; }
void stack_push(Stack *s, int v) { s->data[++s->top] = v; }
int  stack_pop(Stack *s)  { return s->data[s->top--]; }
int  stack_peek(Stack *s) { return s->data[s->top]; }
int  stack_empty(Stack *s){ return s->top < 0; }

int is_digit(char c) { return c >= '0' && c <= '9'; }

/* Evaluate a postfix expression given as array of tokens (strings).
   Tokens are either integer strings or single-char operators (+,-,*,/).
   Returns the result. */
int eval_postfix(char *tokens[], int n) {
    Stack s;
    stack_init(&s);
    int i, j;
    for (i = 0; i < n; i++) {
        char *tok = tokens[i];
        /* Check if it's a number (possibly negative) */
        int is_num = 1;
        int start = 0;
        if (tok[0] == '-' && tok[1] != '\0') start = 1;
        else if (!is_digit(tok[0])) is_num = 0;
        if (is_num) {
            for (j = start; tok[j] != '\0'; j++) {
                if (!is_digit(tok[j])) { is_num = 0; break; }
            }
        }
        if (is_num) {
            int val = 0, neg = (tok[0] == '-');
            for (j = neg ? 1 : 0; tok[j] != '\0'; j++) val = val * 10 + (tok[j] - '0');
            if (neg) val = -val;
            stack_push(&s, val);
        } else {
            int b = stack_pop(&s);
            int a = stack_pop(&s);
            int r = 0;
            char op = tok[0];
            if (op == '+') r = a + b;
            else if (op == '-') r = a - b;
            else if (op == '*') r = a * b;
            else if (op == '/') r = a / b;
            stack_push(&s, r);
        }
    }
    return stack_pop(&s);
}

/* ---- Operator precedence parser for infix expressions ----
   Supports: integer operands, +, -, *, /, unary minus, parentheses.
   Implemented as recursive descent.
   Input is a char array, index passed by pointer.
*/

char *g_expr;
int   g_pos;

void skip_spaces(void) {
    while (g_expr[g_pos] == ' ') g_pos++;
}

int parse_number(void) {
    skip_spaces();
    int sign = 1;
    if (g_expr[g_pos] == '-') { sign = -1; g_pos++; }
    int v = 0;
    while (is_digit(g_expr[g_pos])) {
        v = v * 10 + (g_expr[g_pos] - '0');
        g_pos++;
    }
    return sign * v;
}

int parse_expr(void);

int parse_primary(void) {
    skip_spaces();
    if (g_expr[g_pos] == '(') {
        g_pos++;
        int v = parse_expr();
        skip_spaces();
        if (g_expr[g_pos] == ')') g_pos++;
        return v;
    }
    return parse_number();
}

int parse_term(void) {
    int left = parse_primary();
    while (1) {
        skip_spaces();
        char op = g_expr[g_pos];
        if (op != '*' && op != '/') break;
        g_pos++;
        int right = parse_primary();
        if (op == '*') left *= right;
        else           left /= right;
    }
    return left;
}

int parse_expr(void) {
    int left = parse_term();
    while (1) {
        skip_spaces();
        char op = g_expr[g_pos];
        if (op != '+' && op != '-') break;
        g_pos++;
        int right = parse_term();
        if (op == '+') left += right;
        else           left -= right;
    }
    return left;
}

int evaluate(char *expr) {
    g_expr = expr;
    g_pos = 0;
    return parse_expr();
}

/* ---- Shunting-yard: infix to postfix conversion ----
   We'll convert a tokenized infix expression to postfix order,
   storing operator characters in an output token list, then evaluate.
   Simplified: only +, -, *, / and integer literals.
*/

/* precedence of operator character */
int prec(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

/* Convert infix integer array + operator char array to result directly
   using Shunting-yard with evaluation.
   tokens: alternating number, op, number, op, ... ending with number.
   n_nums: count of numbers, n_ops = n_nums - 1
*/
int shunting_eval(int nums[], char ops[], int n_nums) {
    /* operator stack */
    char op_stack[32];
    int op_top = -1;
    /* value stack */
    int val_stack[32];
    int val_top = -1;
    int ni = 0, oi = 0;

    /* push first number */
    val_stack[++val_top] = nums[ni++];

    while (ni < n_nums) {
        char op = ops[oi++];
        /* pop operators with higher/equal precedence */
        while (op_top >= 0 && prec(op_stack[op_top]) >= prec(op)) {
            int b = val_stack[val_top--];
            int a = val_stack[val_top--];
            char pop = op_stack[op_top--];
            int r;
            if (pop == '+') r = a + b;
            else if (pop == '-') r = a - b;
            else if (pop == '*') r = a * b;
            else r = a / b;
            val_stack[++val_top] = r;
        }
        op_stack[++op_top] = op;
        val_stack[++val_top] = nums[ni++];
    }
    /* drain operator stack */
    while (op_top >= 0) {
        int b = val_stack[val_top--];
        int a = val_stack[val_top--];
        char pop = op_stack[op_top--];
        int r;
        if (pop == '+') r = a + b;
        else if (pop == '-') r = a - b;
        else if (pop == '*') r = a * b;
        else r = a / b;
        val_stack[++val_top] = r;
    }
    return val_stack[0];
}

int main(void) {
    /* Postfix evaluation */
    {
        /* "3 4 +" = 7 */
        char *t1[] = {"3", "4", "+"};
        printf("%d\n", eval_postfix(t1, 3));  // EXPECT: 7

        /* "5 1 2 + 4 * + 3 -" = 14 */
        char *t2[] = {"5", "1", "2", "+", "4", "*", "+", "3", "-"};
        printf("%d\n", eval_postfix(t2, 9));  // EXPECT: 14

        /* "2 3 * 4 5 * +" = 26 */
        char *t3[] = {"2", "3", "*", "4", "5", "*", "+"};
        printf("%d\n", eval_postfix(t3, 7));  // EXPECT: 26

        /* "10 2 /" = 5 */
        char *t4[] = {"10", "2", "/"};
        printf("%d\n", eval_postfix(t4, 3));  // EXPECT: 5

        /* "15 7 1 1 + - / 3 * 2 1 1 + + -" = 5 */
        char *t5[] = {"15","7","1","1","+","-","/","3","*","2","1","1","+","+"," -"};
        /* Let's use a simpler one: "6 2 / 5 +" = 8 */
        char *t6[] = {"6", "2", "/", "5", "+"};
        printf("%d\n", eval_postfix(t6, 5));  // EXPECT: 8
    }

    /* Infix evaluator (recursive descent) */
    {
        char e1[] = "3 + 4";
        printf("%d\n", evaluate(e1));          // EXPECT: 7

        char e2[] = "2 + 3 * 4";
        printf("%d\n", evaluate(e2));          // EXPECT: 14

        char e3[] = "(2 + 3) * 4";
        printf("%d\n", evaluate(e3));          // EXPECT: 20

        char e4[] = "10 / 2 + 3";
        printf("%d\n", evaluate(e4));          // EXPECT: 8

        char e5[] = "100 - 4 * 5 + 3";
        printf("%d\n", evaluate(e5));          // EXPECT: 83

        char e6[] = "(1 + 2) * (3 + 4)";
        printf("%d\n", evaluate(e6));          // EXPECT: 21

        char e7[] = "2 * 3 + 4 * 5";
        printf("%d\n", evaluate(e7));          // EXPECT: 26

        char e8[] = "((3 + 5) * 2) / 4";
        printf("%d\n", evaluate(e8));          // EXPECT: 4
    }

    /* Shunting-yard evaluator */
    {
        /* 3 + 4 * 2 = 11 */
        int nums1[] = {3, 4, 2};
        char ops1[] = {'+', '*'};
        printf("%d\n", shunting_eval(nums1, ops1, 3));  // EXPECT: 11

        /* 10 - 3 + 2 = 9 */
        int nums2[] = {10, 3, 2};
        char ops2[] = {'-', '+'};
        printf("%d\n", shunting_eval(nums2, ops2, 3));  // EXPECT: 9

        /* 2 + 3 * 4 - 1 = 13 */
        int nums3[] = {2, 3, 4, 1};
        char ops3[] = {'+', '*', '-'};
        printf("%d\n", shunting_eval(nums3, ops3, 4));  // EXPECT: 13

        /* 6 * 2 + 4 / 2 = 14 */
        int nums4[] = {6, 2, 4, 2};
        char ops4[] = {'*', '+', '/'};
        printf("%d\n", shunting_eval(nums4, ops4, 4));  // EXPECT: 14
    }

    return 0;
}
