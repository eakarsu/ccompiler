int printf(const char *fmt, ...);
// EXPECT: === Compiler Phase Simulation ===\nExpr: "3 + 4 * 2"\n  Tokens(6): 3 + 4 * 2 EOF\n  AST nodes: 5\n  Result: 14\nExpr: "10 - 3 + 5"\n  Tokens(6): 10 - 3 + 5 EOF\n  AST nodes: 5\n  Result: 12\nExpr: "7 * 8"\n  Tokens(4): 7 * 8 EOF\n  AST nodes: 3\n  Result: 56\nExpr: "100 - 20 - 5"\n  Tokens(6): 100 - 20 - 5 EOF\n  AST nodes: 5\n  Result: 75

enum TokenType { TOK_NUM, TOK_PLUS, TOK_MINUS, TOK_STAR, TOK_LPAREN, TOK_RPAREN, TOK_EOF };

struct Token {
    enum TokenType type;
    int value;
};

struct Lexer {
    char input[64];
    int pos;
    int length;
    int token_count;
};

struct ASTNode {
    int op;
    int value;
    int left;
    int right;
    int is_leaf;
};

struct Compiler {
    struct Token tokens[20];
    int num_tokens;
    struct ASTNode nodes[20];
    int num_nodes;
    int instructions[20];
    int num_instr;
    int parse_pos;
};

void init_lexer(struct Lexer *l, const char *input) {
    int i = 0;
    while (input[i] && i < 63) {
        l->input[i] = input[i];
        i++;
    }
    l->input[i] = '\0';
    l->length = i;
    l->pos = 0;
    l->token_count = 0;
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

void tokenize(struct Lexer *l, struct Compiler *comp) {
    comp->num_tokens = 0;
    while (l->pos < l->length) {
        char c = l->input[l->pos];
        if (c == ' ') { l->pos++; continue; }
        if (is_digit(c)) {
            int val = 0;
            while (l->pos < l->length && is_digit(l->input[l->pos])) {
                val = val * 10 + (l->input[l->pos] - '0');
                l->pos++;
            }
            comp->tokens[comp->num_tokens].type = TOK_NUM;
            comp->tokens[comp->num_tokens].value = val;
            comp->num_tokens++;
        } else {
            if (c == '+') comp->tokens[comp->num_tokens].type = TOK_PLUS;
            else if (c == '-') comp->tokens[comp->num_tokens].type = TOK_MINUS;
            else if (c == '*') comp->tokens[comp->num_tokens].type = TOK_STAR;
            else if (c == '(') comp->tokens[comp->num_tokens].type = TOK_LPAREN;
            else if (c == ')') comp->tokens[comp->num_tokens].type = TOK_RPAREN;
            comp->tokens[comp->num_tokens].value = 0;
            comp->num_tokens++;
            l->pos++;
        }
    }
    comp->tokens[comp->num_tokens].type = TOK_EOF;
    comp->tokens[comp->num_tokens].value = 0;
    comp->num_tokens++;
}

const char *tok_name(enum TokenType t) {
    if (t == TOK_NUM) return "NUM";
    if (t == TOK_PLUS) return "+";
    if (t == TOK_MINUS) return "-";
    if (t == TOK_STAR) return "*";
    if (t == TOK_LPAREN) return "(";
    if (t == TOK_RPAREN) return ")";
    return "EOF";
}

int make_leaf(struct Compiler *comp, int value) {
    int idx = comp->num_nodes;
    comp->nodes[idx].is_leaf = 1;
    comp->nodes[idx].value = value;
    comp->nodes[idx].op = 0;
    comp->nodes[idx].left = -1;
    comp->nodes[idx].right = -1;
    comp->num_nodes++;
    return idx;
}

int make_node(struct Compiler *comp, int op, int left, int right) {
    int idx = comp->num_nodes;
    comp->nodes[idx].is_leaf = 0;
    comp->nodes[idx].op = op;
    comp->nodes[idx].left = left;
    comp->nodes[idx].right = right;
    comp->nodes[idx].value = 0;
    comp->num_nodes++;
    return idx;
}

int eval_ast(struct Compiler *comp, int node) {
    if (comp->nodes[node].is_leaf) return comp->nodes[node].value;
    int l = eval_ast(comp, comp->nodes[node].left);
    int r = eval_ast(comp, comp->nodes[node].right);
    if (comp->nodes[node].op == TOK_PLUS) return l + r;
    if (comp->nodes[node].op == TOK_MINUS) return l - r;
    if (comp->nodes[node].op == TOK_STAR) return l * r;
    return 0;
}

int main(void) {
    printf("=== Compiler Phase Simulation ===\n");

    char *exprs[4];
    exprs[0] = "3 + 4 * 2";
    exprs[1] = "10 - 3 + 5";
    exprs[2] = "7 * 8";
    exprs[3] = "100 - 20 - 5";

    int e;
    for (e = 0; e < 4; e++) {
        struct Lexer lex;
        struct Compiler comp;
        comp.num_nodes = 0;
        comp.num_instr = 0;

        init_lexer(&lex, exprs[e]);
        tokenize(&lex, &comp);

        printf("Expr: \"%s\"\n", exprs[e]);
        printf("  Tokens(%d):", comp.num_tokens);
        int i;
        for (i = 0; i < comp.num_tokens; i++) {
            if (comp.tokens[i].type == TOK_NUM)
                printf(" %d", comp.tokens[i].value);
            else
                printf(" %s", tok_name(comp.tokens[i].type));
        }
        printf("\n");

        int root;
        if (comp.num_tokens == 2) {
            root = make_leaf(&comp, comp.tokens[0].value);
        } else {
            int left = make_leaf(&comp, comp.tokens[0].value);
            int idx = 1;
            while (idx < comp.num_tokens - 1) {
                int op = comp.tokens[idx].type;
                idx++;
                int right = make_leaf(&comp, comp.tokens[idx].value);
                idx++;

                if (op == TOK_STAR) {
                    left = make_node(&comp, op, left, right);
                } else {
                    left = make_node(&comp, op, left, right);
                }
            }
            root = left;
        }

        int result = eval_ast(&comp, root);
        printf("  AST nodes: %d\n", comp.num_nodes);
        printf("  Result: %d\n", result);
    }

    return 0;
}
