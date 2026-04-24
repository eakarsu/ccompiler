int printf(const char *fmt, ...);

struct Token {
    int type;
    int value;
};

struct ASTNode {
    int type;
    int value;
    int left;
    int right;
};

struct Compiler {
    struct Token tokens[64];
    int token_count;
    int pos;
    struct ASTNode nodes[64];
    int node_count;
};

int is_digit(char c) {
    return c >= 48 && c <= 57;
}

int is_space(char c) {
    return c == 32;
}

void tokenize(struct Compiler *comp, const char *input) {
    comp->token_count = 0;
    int i = 0;
    while (input[i] != 0) {
        if (is_space(input[i])) {
            i = i + 1;
        } else if (is_digit(input[i])) {
            int val = 0;
            while (input[i] != 0 && is_digit(input[i])) {
                val = val * 10 + (input[i] - 48);
                i = i + 1;
            }
            int idx = comp->token_count;
            comp->tokens[idx].type = 0;
            comp->tokens[idx].value = val;
            comp->token_count = comp->token_count + 1;
        } else if (input[i] == 43) {
            int idx = comp->token_count;
            comp->tokens[idx].type = 1;
            comp->tokens[idx].value = 43;
            comp->token_count = comp->token_count + 1;
            i = i + 1;
        } else if (input[i] == 45) {
            int idx = comp->token_count;
            comp->tokens[idx].type = 2;
            comp->tokens[idx].value = 45;
            comp->token_count = comp->token_count + 1;
            i = i + 1;
        } else if (input[i] == 42) {
            int idx = comp->token_count;
            comp->tokens[idx].type = 3;
            comp->tokens[idx].value = 42;
            comp->token_count = comp->token_count + 1;
            i = i + 1;
        } else if (input[i] == 47) {
            int idx = comp->token_count;
            comp->tokens[idx].type = 4;
            comp->tokens[idx].value = 47;
            comp->token_count = comp->token_count + 1;
            i = i + 1;
        } else if (input[i] == 40) {
            int idx = comp->token_count;
            comp->tokens[idx].type = 5;
            comp->tokens[idx].value = 40;
            comp->token_count = comp->token_count + 1;
            i = i + 1;
        } else if (input[i] == 41) {
            int idx = comp->token_count;
            comp->tokens[idx].type = 6;
            comp->tokens[idx].value = 41;
            comp->token_count = comp->token_count + 1;
            i = i + 1;
        } else {
            i = i + 1;
        }
    }
}

int new_num_node(struct Compiler *comp, int val) {
    int idx = comp->node_count;
    comp->nodes[idx].type = 0;
    comp->nodes[idx].value = val;
    comp->nodes[idx].left = -1;
    comp->nodes[idx].right = -1;
    comp->node_count = comp->node_count + 1;
    return idx;
}

int new_op_node(struct Compiler *comp, int op, int l, int r) {
    int idx = comp->node_count;
    comp->nodes[idx].type = op;
    comp->nodes[idx].value = 0;
    comp->nodes[idx].left = l;
    comp->nodes[idx].right = r;
    comp->node_count = comp->node_count + 1;
    return idx;
}

int parse_primary(struct Compiler *comp);
int parse_term(struct Compiler *comp);
int parse_expr(struct Compiler *comp);

int parse_primary(struct Compiler *comp) {
    if (comp->pos < comp->token_count &&
        comp->tokens[comp->pos].type == 5) {
        comp->pos = comp->pos + 1;
        int node = parse_expr(comp);
        if (comp->pos < comp->token_count &&
            comp->tokens[comp->pos].type == 6) {
            comp->pos = comp->pos + 1;
        }
        return node;
    }
    int val = comp->tokens[comp->pos].value;
    comp->pos = comp->pos + 1;
    return new_num_node(comp, val);
}

int parse_term(struct Compiler *comp) {
    int left = parse_primary(comp);
    while (comp->pos < comp->token_count &&
           (comp->tokens[comp->pos].type == 3 ||
            comp->tokens[comp->pos].type == 4)) {
        int op = comp->tokens[comp->pos].type;
        comp->pos = comp->pos + 1;
        int right = parse_primary(comp);
        left = new_op_node(comp, op, left, right);
    }
    return left;
}

int parse_expr(struct Compiler *comp) {
    int left = parse_term(comp);
    while (comp->pos < comp->token_count &&
           (comp->tokens[comp->pos].type == 1 ||
            comp->tokens[comp->pos].type == 2)) {
        int op = comp->tokens[comp->pos].type;
        comp->pos = comp->pos + 1;
        int right = parse_term(comp);
        left = new_op_node(comp, op, left, right);
    }
    return left;
}

int eval(struct Compiler *comp, int node) {
    if (comp->nodes[node].type == 0) {
        return comp->nodes[node].value;
    }
    int l = eval(comp, comp->nodes[node].left);
    int r = eval(comp, comp->nodes[node].right);
    if (comp->nodes[node].type == 1) return l + r;
    if (comp->nodes[node].type == 2) return l - r;
    if (comp->nodes[node].type == 3) return l * r;
    if (comp->nodes[node].type == 4) return l / r;
    return 0;
}

int compile_and_eval(struct Compiler *comp, const char *expr) {
    comp->token_count = 0;
    comp->node_count = 0;
    comp->pos = 0;
    tokenize(comp, expr);
    int root = parse_expr(comp);
    return eval(comp, root);
}

int main(void) {
    struct Compiler comp;

    int r1 = compile_and_eval(&comp, "3 + 4");
    // EXPECT: 3+4=7
    printf("3+4=%d\n", r1);

    int r2 = compile_and_eval(&comp, "10 - 3");
    // EXPECT: 10-3=7
    printf("10-3=%d\n", r2);

    int r3 = compile_and_eval(&comp, "6 * 7");
    // EXPECT: 6*7=42
    printf("6*7=%d\n", r3);

    int r4 = compile_and_eval(&comp, "20 / 4");
    // EXPECT: 20/4=5
    printf("20/4=%d\n", r4);

    int r5 = compile_and_eval(&comp, "2 + 3 * 4");
    // EXPECT: 2+3*4=14
    printf("2+3*4=%d\n", r5);

    int r6 = compile_and_eval(&comp, "10 - 2 * 3");
    // EXPECT: 10-2*3=4
    printf("10-2*3=%d\n", r6);

    int r7 = compile_and_eval(&comp, "(2 + 3) * 4");
    // EXPECT: (2+3)*4=20
    printf("(2+3)*4=%d\n", r7);

    int r8 = compile_and_eval(&comp, "100 / 10 + 5");
    // EXPECT: 100/10+5=15
    printf("100/10+5=%d\n", r8);

    int r9 = compile_and_eval(&comp, "1 + 2 + 3 + 4");
    // EXPECT: 1+2+3+4=10
    printf("1+2+3+4=%d\n", r9);

    int r10 = compile_and_eval(&comp, "5");
    // EXPECT: single=5
    printf("single=%d\n", r10);

    int r11 = compile_and_eval(&comp, "(10 + 2) / 3");
    // EXPECT: (10+2)/3=4
    printf("(10+2)/3=%d\n", r11);

    tokenize(&comp, "1 + 2 * 3");
    // EXPECT: tokens=5
    printf("tokens=%d\n", comp.token_count);

    // EXPECT: tok0_type=0
    printf("tok0_type=%d\n", comp.tokens[0].type);

    // EXPECT: tok0_val=1
    printf("tok0_val=%d\n", comp.tokens[0].value);

    // EXPECT: tok1_type=1
    printf("tok1_type=%d\n", comp.tokens[1].type);

    int r12 = compile_and_eval(&comp, "2 * 3 + 4 * 5");
    // EXPECT: 2*3+4*5=26
    printf("2*3+4*5=%d\n", r12);

    return 0;
}
