int printf(const char *fmt, ...);
// EXPECT: ex1:toks=4,val=46,nums=2,op=1|ex2:toks=8,val=27,nums=3,op=2,paren=2,bal=1|ex3:toks=6,val=40,sumnum=123|ex4:toks=2,val=7|ex5:toks=12,val=15,depth=1,parcnt=4|ex6:toks=4,val=0|ex7:toks=2,val=9|ex8:toks=8,val=17,maxnum=10|ex9:toks=6,val=5,depth=2|ex10:toks=8,val=10|types:NUM,+,NUM,EOF|pos:0,2,4,5,7,9,10,11|errpos=1,errcnt=1|consecop=1|noconsec=0|unbal=0|vals:12,0,34,0|big=999|nest=9,depth=2|div=5|sub=33|mulop=26,nops=3|onlyparen=2,bal=1|zero=0|neg=-42|summary:total=10,nums=5,op=4,val=5
/* Simple lexer simulation: tokenize integer expressions */

/* Token types */
enum TokenType {
    TOK_NUM,
    TOK_PLUS,
    TOK_MINUS,
    TOK_MUL,
    TOK_DIV,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_EOF,
    TOK_ERR
};

/* Token structure */
struct Token {
    int type;
    int value; /* numeric value if TOK_NUM */
    int pos;   /* position in input */
};

/* Globals */
int input[256];
int input_len;
int pos;
struct Token tokens[128];
int token_count;

void set_input(int *src, int len) {
    int i;
    i = 0;
    while (i < len) {
        input[i] = src[i];
        i = i + 1;
    }
    input_len = len;
    pos = 0;
    token_count = 0;
}

int is_digit(int c) {
    return (c >= 48 && c <= 57);
}

int is_space(int c) {
    return (c == 32);
}

int digit_val(int c) {
    return c - 48;
}

int peek(void) {
    if (pos >= input_len) return -1;
    return input[pos];
}

int advance(void) {
    int c;
    c = input[pos];
    pos = pos + 1;
    return c;
}

void skip_spaces(void) {
    while (pos < input_len && is_space(input[pos])) {
        pos = pos + 1;
    }
}

int read_number(void) {
    int val;
    val = 0;
    while (pos < input_len && is_digit(input[pos])) {
        val = val * 10 + digit_val(input[pos]);
        pos = pos + 1;
    }
    return val;
}

void add_token(int type, int value, int p) {
    tokens[token_count].type = type;
    tokens[token_count].value = value;
    tokens[token_count].pos = p;
    token_count = token_count + 1;
}

int tokenize(void) {
    int c;
    int start;
    int val;
    token_count = 0;
    pos = 0;
    while (pos < input_len) {
        skip_spaces();
        if (pos >= input_len) break;
        start = pos;
        c = input[pos];
        if (is_digit(c)) {
            val = read_number();
            add_token(TOK_NUM, val, start);
        } else if (c == 43) { /* '+' */
            advance();
            add_token(TOK_PLUS, 0, start);
        } else if (c == 45) { /* '-' */
            advance();
            add_token(TOK_MINUS, 0, start);
        } else if (c == 42) { /* '*' */
            advance();
            add_token(TOK_MUL, 0, start);
        } else if (c == 47) { /* '/' */
            advance();
            add_token(TOK_DIV, 0, start);
        } else if (c == 40) { /* '(' */
            advance();
            add_token(TOK_LPAREN, 0, start);
        } else if (c == 41) { /* ')' */
            advance();
            add_token(TOK_RPAREN, 0, start);
        } else {
            advance();
            add_token(TOK_ERR, c, start);
        }
    }
    add_token(TOK_EOF, 0, pos);
    return token_count;
}

int count_type(int type) {
    int i;
    int cnt;
    cnt = 0;
    i = 0;
    while (i < token_count) {
        if (tokens[i].type == type) {
            cnt = cnt + 1;
        }
        i = i + 1;
    }
    return cnt;
}

void print_token_type(int type) {
    if (type == TOK_NUM) { printf("%c%c%c", 'N', 'U', 'M'); }
    else if (type == TOK_PLUS) { printf("%c", '+'); }
    else if (type == TOK_MINUS) { printf("%c", '-'); }
    else if (type == TOK_MUL) { printf("%c", '*'); }
    else if (type == TOK_DIV) { printf("%c", '/'); }
    else if (type == TOK_LPAREN) { printf("%c", '('); }
    else if (type == TOK_RPAREN) { printf("%c", ')'); }
    else if (type == TOK_EOF) { printf("%c%c%c", 'E', 'O', 'F'); }
    else { printf("%c%c%c", 'E', 'R', 'R'); }
}

/* Simple expression evaluator using tokens */
int eval_pos;

int eval_expr(void);
int eval_term(void);
int eval_factor(void);

int eval_factor(void) {
    int val;
    int t;
    t = tokens[eval_pos].type;
    if (t == TOK_NUM) {
        val = tokens[eval_pos].value;
        eval_pos = eval_pos + 1;
        return val;
    }
    if (t == TOK_LPAREN) {
        eval_pos = eval_pos + 1;
        val = eval_expr();
        if (tokens[eval_pos].type == TOK_RPAREN) {
            eval_pos = eval_pos + 1;
        }
        return val;
    }
    if (t == TOK_MINUS) {
        eval_pos = eval_pos + 1;
        return 0 - eval_factor();
    }
    return 0;
}

int eval_term(void) {
    int val;
    int t;
    val = eval_factor();
    while (1) {
        t = tokens[eval_pos].type;
        if (t == TOK_MUL) {
            eval_pos = eval_pos + 1;
            val = val * eval_factor();
        } else if (t == TOK_DIV) {
            eval_pos = eval_pos + 1;
            val = val / eval_factor();
        } else {
            break;
        }
    }
    return val;
}

int eval_expr(void) {
    int val;
    int t;
    val = eval_term();
    while (1) {
        t = tokens[eval_pos].type;
        if (t == TOK_PLUS) {
            eval_pos = eval_pos + 1;
            val = val + eval_term();
        } else if (t == TOK_MINUS) {
            eval_pos = eval_pos + 1;
            val = val - eval_term();
        } else {
            break;
        }
    }
    return val;
}

int evaluate(void) {
    eval_pos = 0;
    return eval_expr();
}

/* Classify tokens: count operators, numbers, parens */
int count_operators(void) {
    int i;
    int cnt;
    cnt = 0;
    i = 0;
    while (i < token_count) {
        if (tokens[i].type == TOK_PLUS || tokens[i].type == TOK_MINUS ||
            tokens[i].type == TOK_MUL || tokens[i].type == TOK_DIV) {
            cnt = cnt + 1;
        }
        i = i + 1;
    }
    return cnt;
}

int count_parens(void) {
    int i;
    int cnt;
    cnt = 0;
    i = 0;
    while (i < token_count) {
        if (tokens[i].type == TOK_LPAREN || tokens[i].type == TOK_RPAREN) {
            cnt = cnt + 1;
        }
        i = i + 1;
    }
    return cnt;
}

int check_balanced_parens(void) {
    int i;
    int depth;
    depth = 0;
    i = 0;
    while (i < token_count) {
        if (tokens[i].type == TOK_LPAREN) {
            depth = depth + 1;
        } else if (tokens[i].type == TOK_RPAREN) {
            depth = depth - 1;
            if (depth < 0) return 0;
        }
        i = i + 1;
    }
    return (depth == 0);
}

int max_paren_depth(void) {
    int i;
    int depth;
    int max_d;
    depth = 0;
    max_d = 0;
    i = 0;
    while (i < token_count) {
        if (tokens[i].type == TOK_LPAREN) {
            depth = depth + 1;
            if (depth > max_d) max_d = depth;
        } else if (tokens[i].type == TOK_RPAREN) {
            depth = depth - 1;
        }
        i = i + 1;
    }
    return max_d;
}

/* Sum of all numeric token values */
int sum_num_values(void) {
    int i;
    int s;
    s = 0;
    i = 0;
    while (i < token_count) {
        if (tokens[i].type == TOK_NUM) {
            s = s + tokens[i].value;
        }
        i = i + 1;
    }
    return s;
}

/* Max numeric value among tokens */
int max_num_value(void) {
    int i;
    int m;
    m = -1;
    i = 0;
    while (i < token_count) {
        if (tokens[i].type == TOK_NUM) {
            if (tokens[i].value > m) {
                m = tokens[i].value;
            }
        }
        i = i + 1;
    }
    return m;
}

/* Find position of first error token, or -1 */
int first_error_pos(void) {
    int i;
    i = 0;
    while (i < token_count) {
        if (tokens[i].type == TOK_ERR) {
            return tokens[i].pos;
        }
        i = i + 1;
    }
    return -1;
}

/* Check if expression has consecutive operators (syntax error) */
int has_consecutive_ops(void) {
    int i;
    int prev_op;
    prev_op = 0;
    i = 0;
    while (i < token_count) {
        if (tokens[i].type == TOK_PLUS || tokens[i].type == TOK_MUL ||
            tokens[i].type == TOK_DIV) {
            if (prev_op) return 1;
            prev_op = 1;
        } else {
            prev_op = 0;
        }
        i = i + 1;
    }
    return 0;
}

void print_str(int *s, int len) {
    int i;
    i = 0;
    while (i < len) {
        printf("%c", s[i]);
        i = i + 1;
    }
}

int main(void) {
    int tc;
    int val;
    int e1[7];
    int e2[13];
    int e3[11];
    int e4[3];
    int e5[19];
    int e6[5];
    int e7[1];
    int e8[15];
    int e9[9];
    int e10[7];
    int i;

    /* Expression 1: "12+34" -> chars: 49,50,43,51,52 => "12+34" */
    e1[0] = 49; e1[1] = 50; e1[2] = 43; e1[3] = 51; e1[4] = 52;
    set_input(e1, 5);
    tc = tokenize();
    printf("%c%c%c%c", 'e', 'x', '1', ':');
    printf("%c%c%c%c%c", 't', 'o', 'k', 's', '=');
    printf("%d", tc);
    val = evaluate();
    printf("%c%c%c%c", ',', 'v', 'a', 'l');
    printf("%c", '=');
    printf("%d", val);
    printf("%c%c%c%c%c", ',', 'n', 'u', 'm', 's');
    printf("%c", '=');
    printf("%d", count_type(TOK_NUM));
    printf("%c%c%c", ',', 'o', 'p');
    printf("%c", '=');
    printf("%d", count_operators());

    /* Expression 2: "3 * (4 + 5)" -> 51,32,42,32,40,52,32,43,32,53,41 */
    e2[0] = 51; e2[1] = 32; e2[2] = 42; e2[3] = 32; e2[4] = 40;
    e2[5] = 52; e2[6] = 32; e2[7] = 43; e2[8] = 32; e2[9] = 53;
    e2[10] = 41;
    set_input(e2, 11);
    tc = tokenize();
    printf("%c", '|');
    printf("%c%c%c%c", 'e', 'x', '2', ':');
    printf("%c%c%c%c%c", 't', 'o', 'k', 's', '=');
    printf("%d", tc);
    val = evaluate();
    printf("%c%c%c%c%c", ',', 'v', 'a', 'l', '=');
    printf("%d", val);
    printf("%c%c%c%c%c%c", ',', 'n', 'u', 'm', 's', '=');
    printf("%d", count_type(TOK_NUM));
    printf("%c%c%c%c", ',', 'o', 'p', '=');
    printf("%d", count_operators());
    printf("%c%c%c%c%c%c%c", ',', 'p', 'a', 'r', 'e', 'n', '=');
    printf("%d", count_parens());
    printf("%c%c%c%c", ',', 'b', 'a', 'l');
    printf("%c", '=');
    printf("%d", check_balanced_parens());

    /* Expression 3: "100-20*3" -> 49,48,48,45,50,48,42,51 */
    e3[0] = 49; e3[1] = 48; e3[2] = 48; e3[3] = 45; e3[4] = 50;
    e3[5] = 48; e3[6] = 42; e3[7] = 51;
    set_input(e3, 8);
    tc = tokenize();
    printf("%c", '|');
    printf("%c%c%c%c", 'e', 'x', '3', ':');
    printf("%c%c%c%c%c", 't', 'o', 'k', 's', '=');
    printf("%d", tc);
    val = evaluate();
    printf("%c%c%c%c%c", ',', 'v', 'a', 'l', '=');
    printf("%d", val);
    printf("%c%c%c%c%c%c%c%c", ',', 's', 'u', 'm', 'n', 'u', 'm', '=');
    printf("%d", sum_num_values());

    /* Expression 4: "7" -> 55 */
    e4[0] = 55;
    set_input(e4, 1);
    tc = tokenize();
    printf("%c", '|');
    printf("%c%c%c%c", 'e', 'x', '4', ':');
    printf("%c%c%c%c%c", 't', 'o', 'k', 's', '=');
    printf("%d", tc);
    val = evaluate();
    printf("%c%c%c%c%c", ',', 'v', 'a', 'l', '=');
    printf("%d", val);

    /* Expression 5: "(2+3)*(4-1)" -> 40,50,43,51,41,42,40,52,45,49,41 */
    e5[0] = 40; e5[1] = 50; e5[2] = 43; e5[3] = 51; e5[4] = 41;
    e5[5] = 42; e5[6] = 40; e5[7] = 52; e5[8] = 45; e5[9] = 49;
    e5[10] = 41;
    set_input(e5, 11);
    tc = tokenize();
    printf("%c", '|');
    printf("%c%c%c%c", 'e', 'x', '5', ':');
    printf("%c%c%c%c%c", 't', 'o', 'k', 's', '=');
    printf("%d", tc);
    val = evaluate();
    printf("%c%c%c%c%c", ',', 'v', 'a', 'l', '=');
    printf("%d", val);
    printf("%c%c%c%c%c%c", ',', 'd', 'e', 'p', 't', 'h');
    printf("%c", '=');
    printf("%d", max_paren_depth());
    printf("%c%c%c%c%c%c%c%c", ',', 'p', 'a', 'r', 'c', 'n', 't', '=');
    printf("%d", count_parens());

    /* Expression 6: "0+0" -> 48,43,48 */
    e6[0] = 48; e6[1] = 43; e6[2] = 48;
    set_input(e6, 3);
    tc = tokenize();
    printf("%c", '|');
    printf("%c%c%c%c", 'e', 'x', '6', ':');
    printf("%c%c%c%c%c", 't', 'o', 'k', 's', '=');
    printf("%d", tc);
    val = evaluate();
    printf("%c%c%c%c%c", ',', 'v', 'a', 'l', '=');
    printf("%d", val);

    /* Expression 7: just a number "9" -> 57 */
    e7[0] = 57;
    set_input(e7, 1);
    tc = tokenize();
    printf("%c", '|');
    printf("%c%c%c%c", 'e', 'x', '7', ':');
    printf("%c%c%c%c%c", 't', 'o', 'k', 's', '=');
    printf("%d", tc);
    val = evaluate();
    printf("%c%c%c%c%c", ',', 'v', 'a', 'l', '=');
    printf("%d", val);

    /* Expression 8: "10/2+3*4" -> 49,48,47,50,43,51,42,52 */
    e8[0] = 49; e8[1] = 48; e8[2] = 47; e8[3] = 50; e8[4] = 43;
    e8[5] = 51; e8[6] = 42; e8[7] = 52;
    set_input(e8, 8);
    tc = tokenize();
    printf("%c", '|');
    printf("%c%c%c%c", 'e', 'x', '8', ':');
    printf("%c%c%c%c%c", 't', 'o', 'k', 's', '=');
    printf("%d", tc);
    val = evaluate();
    printf("%c%c%c%c%c", ',', 'v', 'a', 'l', '=');
    printf("%d", val);
    printf("%c%c%c%c%c%c%c", ',', 'm', 'a', 'x', 'n', 'u', 'm');
    printf("%c", '=');
    printf("%d", max_num_value());

    /* Expression 9: "((5))" -> 40,40,53,41,41 */
    e9[0] = 40; e9[1] = 40; e9[2] = 53; e9[3] = 41; e9[4] = 41;
    set_input(e9, 5);
    tc = tokenize();
    printf("%c", '|');
    printf("%c%c%c%c", 'e', 'x', '9', ':');
    printf("%c%c%c%c%c", 't', 'o', 'k', 's', '=');
    printf("%d", tc);
    val = evaluate();
    printf("%c%c%c%c%c", ',', 'v', 'a', 'l', '=');
    printf("%d", val);
    printf("%c%c%c%c%c%c", ',', 'd', 'e', 'p', 't', 'h');
    printf("%c", '=');
    printf("%d", max_paren_depth());

    /* Expression 10: "1+2+3+4+5" -> 49,43,50,43,51,43,52,43,53 */
    e10[0] = 49; e10[1] = 43; e10[2] = 50; e10[3] = 43; e10[4] = 51;
    e10[5] = 43; e10[6] = 52;
    set_input(e10, 7);
    tc = tokenize();
    printf("%c", '|');
    printf("%c%c%c%c%c", 'e', 'x', '1', '0', ':');
    printf("%c%c%c%c%c", 't', 'o', 'k', 's', '=');
    printf("%d", tc);
    val = evaluate();
    printf("%c%c%c%c%c", ',', 'v', 'a', 'l', '=');
    printf("%d", val);

    /* Token types of expression 1 */
    e1[0] = 49; e1[1] = 50; e1[2] = 43; e1[3] = 51; e1[4] = 52;
    set_input(e1, 5);
    tokenize();
    printf("%c", '|');
    printf("%c%c%c%c%c", 't', 'y', 'p', 'e', 's');
    printf("%c", ':');
    i = 0;
    while (i < token_count) {
        if (i > 0) printf("%c", ',');
        print_token_type(tokens[i].type);
        i = i + 1;
    }

    /* Token positions of expression 2 */
    e2[0] = 51; e2[1] = 32; e2[2] = 42; e2[3] = 32; e2[4] = 40;
    e2[5] = 52; e2[6] = 32; e2[7] = 43; e2[8] = 32; e2[9] = 53;
    e2[10] = 41;
    set_input(e2, 11);
    tokenize();
    printf("%c", '|');
    printf("%c%c%c", 'p', 'o', 's');
    printf("%c", ':');
    i = 0;
    while (i < token_count) {
        if (i > 0) printf("%c", ',');
        printf("%d", tokens[i].pos);
        i = i + 1;
    }

    /* Test error token with '@' = 64 in "1@2" */
    {
        int err_in[3];
        err_in[0] = 49; err_in[1] = 64; err_in[2] = 50;
        set_input(err_in, 3);
        tokenize();
        printf("%c", '|');
        printf("%c%c%c%c%c%c", 'e', 'r', 'r', 'p', 'o', 's');
        printf("%c", '=');
        printf("%d", first_error_pos());
        printf("%c%c%c%c%c%c%c%c", ',', 'e', 'r', 'r', 'c', 'n', 't', '=');
        printf("%d", count_type(TOK_ERR));
    }

    /* Consecutive ops check on "1++2" -> 49,43,43,50 */
    {
        int cc_in[4];
        cc_in[0] = 49; cc_in[1] = 43; cc_in[2] = 43; cc_in[3] = 50;
        set_input(cc_in, 4);
        tokenize();
        printf("%c", '|');
        printf("%c%c%c%c%c%c%c%c", 'c', 'o', 'n', 's', 'e', 'c', 'o', 'p');
        printf("%c", '=');
        printf("%d", has_consecutive_ops());
    }

    /* No consecutive ops in "1+2" */
    {
        int nc_in[3];
        nc_in[0] = 49; nc_in[1] = 43; nc_in[2] = 50;
        set_input(nc_in, 3);
        tokenize();
        printf("%c", '|');
        printf("%c%c%c%c%c%c%c%c%c", 'n', 'o', 'c', 'o', 'n', 's', 'e', 'c', '=');
        printf("%d", has_consecutive_ops());
    }

    /* Unbalanced parens "(1+2" -> 40,49,43,50 */
    {
        int ub_in[4];
        ub_in[0] = 40; ub_in[1] = 49; ub_in[2] = 43; ub_in[3] = 50;
        set_input(ub_in, 4);
        tokenize();
        printf("%c", '|');
        printf("%c%c%c%c%c", 'u', 'n', 'b', 'a', 'l');
        printf("%c", '=');
        printf("%d", check_balanced_parens());
    }

    /* All token values for "12+34" */
    e1[0] = 49; e1[1] = 50; e1[2] = 43; e1[3] = 51; e1[4] = 52;
    set_input(e1, 5);
    tokenize();
    printf("%c", '|');
    printf("%c%c%c%c", 'v', 'a', 'l', 's');
    printf("%c", ':');
    i = 0;
    while (i < token_count) {
        if (i > 0) printf("%c", ',');
        printf("%d", tokens[i].value);
        i = i + 1;
    }

    /* Large number: "999" -> 57,57,57 */
    {
        int big[3];
        big[0] = 57; big[1] = 57; big[2] = 57;
        set_input(big, 3);
        tokenize();
        printf("%c", '|');
        printf("%c%c%c", 'b', 'i', 'g');
        printf("%c", '=');
        printf("%d", evaluate());
    }

    /* Nested: "((1+2)*3)" -> 40,40,49,43,50,41,42,51,41 */
    {
        int nest[9];
        nest[0] = 40; nest[1] = 40; nest[2] = 49; nest[3] = 43; nest[4] = 50;
        nest[5] = 41; nest[6] = 42; nest[7] = 51; nest[8] = 41;
        set_input(nest, 9);
        tokenize();
        val = evaluate();
        printf("%c", '|');
        printf("%c%c%c%c", 'n', 'e', 's', 't');
        printf("%c", '=');
        printf("%d", val);
        printf("%c%c%c%c%c%c", ',', 'd', 'e', 'p', 't', 'h');
        printf("%c", '=');
        printf("%d", max_paren_depth());
    }

    /* Division: "20/4" -> 50,48,47,52 */
    {
        int dv[4];
        dv[0] = 50; dv[1] = 48; dv[2] = 47; dv[3] = 52;
        set_input(dv, 4);
        tokenize();
        val = evaluate();
        printf("%c", '|');
        printf("%c%c%c", 'd', 'i', 'v');
        printf("%c", '=');
        printf("%d", val);
    }

    /* Subtraction: "50-17" -> 53,48,45,49,55 */
    {
        int sb[5];
        sb[0] = 53; sb[1] = 48; sb[2] = 45; sb[3] = 49; sb[4] = 55;
        set_input(sb, 5);
        tokenize();
        val = evaluate();
        printf("%c", '|');
        printf("%c%c%c", 's', 'u', 'b');
        printf("%c", '=');
        printf("%d", val);
    }

    /* Multiple operations: "2*3+4*5" -> 50,42,51,43,52,42,53 */
    {
        int mo[7];
        mo[0] = 50; mo[1] = 42; mo[2] = 51; mo[3] = 43; mo[4] = 52;
        mo[5] = 42; mo[6] = 53;
        set_input(mo, 7);
        tokenize();
        val = evaluate();
        printf("%c", '|');
        printf("%c%c%c%c%c", 'm', 'u', 'l', 'o', 'p');
        printf("%c", '=');
        printf("%d", val);
        printf("%c%c%c%c%c%c", ',', 'n', 'o', 'p', 's', '=');
        printf("%d", count_operators());
    }

    /* Only parens: "()" -> just parens */
    {
        int op[2];
        op[0] = 40; op[1] = 41;
        set_input(op, 2);
        tokenize();
        printf("%c", '|');
        printf("%c%c%c%c%c%c%c%c%c%c", 'o', 'n', 'l', 'y', 'p', 'a', 'r', 'e', 'n', '=');
        printf("%d", count_parens());
        printf("%c%c%c%c%c", ',', 'b', 'a', 'l', '=');
        printf("%d", check_balanced_parens());
    }

    /* Zero: "0" -> 48 */
    {
        int zz[1];
        zz[0] = 48;
        set_input(zz, 1);
        tokenize();
        val = evaluate();
        printf("%c", '|');
        printf("%c%c%c%c", 'z', 'e', 'r', 'o');
        printf("%c", '=');
        printf("%d", val);
    }

    /* Negative via subtraction: "0-42" -> 48,45,52,50 */
    {
        int ng[4];
        ng[0] = 48; ng[1] = 45; ng[2] = 52; ng[3] = 50;
        set_input(ng, 4);
        tokenize();
        val = evaluate();
        printf("%c", '|');
        printf("%c%c%c", 'n', 'e', 'g');
        printf("%c", '=');
        printf("%d", val);
    }

    /* Token count summary for "1+2*3-4/2" => 49,43,50,42,51,45,52,47,50 */
    {
        int sum_in[9];
        sum_in[0] = 49; sum_in[1] = 43; sum_in[2] = 50; sum_in[3] = 42; sum_in[4] = 51;
        sum_in[5] = 45; sum_in[6] = 52; sum_in[7] = 47; sum_in[8] = 50;
        set_input(sum_in, 9);
        tc = tokenize();
        printf("%c", '|');
        printf("%c%c%c%c%c%c%c", 's', 'u', 'm', 'm', 'a', 'r', 'y');
        printf("%c%c%c%c%c%c", ':','t','o','t','a','l');
        printf("%c", '=');
        printf("%d", tc);
        printf("%c%c%c%c%c%c", ',', 'n', 'u', 'm', 's', '=');
        printf("%d", count_type(TOK_NUM));
        printf("%c%c%c%c", ',', 'o', 'p', '=');
        printf("%d", count_operators());
        printf("%c%c%c%c%c", ',', 'v', 'a', 'l', '=');
        val = evaluate();
        printf("%d", val);
    }

    printf("%c", '\n');

    return 0;
}
