// stress_099_everything_stress.c - Mega stress: every major C feature
// A mini compiler pipeline simulation using all C features:
// control flow, types, structs/unions/enums, pointers, arrays,
// function pointers, recursion, bitwise ops, compound literals,
// goto, break, continue, do-while, switch, ternary.
// EXPECT: lex_count=6
// EXPECT: tokens=NUM:42 PLUS NUM:8 STAR NUM:5 EOF
// EXPECT: parse_tree=ADD(42,MUL(8,5))
// EXPECT: eval=82
// EXPECT: opt=82
// EXPECT: codegen=1
// EXPECT: hash_set=1 1 1 0
// EXPECT: bitops=170 85 255 0
// EXPECT: shifts=1360 21
// EXPECT: goto_test=55
// EXPECT: dowhile=10
// EXPECT: continue_sum=25
// EXPECT: nested_break=12
// EXPECT: ternary_chain=3
// EXPECT: comma_expr=30
// EXPECT: compound_assign=42
// EXPECT: enum_dispatch=300
// EXPECT: union_reinterpret=1
// EXPECT: recursion_fib=55
// EXPECT: funcptr_table=111
// EXPECT: array_2d_sum=225
// EXPECT: struct_chain=6
// EXPECT: all_pass=1

int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

// ===== ENUMS =====
enum TokenKind { TOK_NUM, TOK_PLUS, TOK_MINUS, TOK_STAR, TOK_SLASH, TOK_EOF };
enum NodeKind { ND_NUM, ND_ADD, ND_SUB, ND_MUL, ND_DIV };

// ===== STRUCTS =====
struct Token {
    enum TokenKind kind;
    int value;
};

struct ASTNode {
    enum NodeKind kind;
    int value;
    struct ASTNode *left;
    struct ASTNode *right;
};

// ===== UNION =====
union ValOrPtr {
    int ival;
    long lval;
};

// ===== TYPEDEF =====
typedef struct ASTNode Node;
typedef int (*BinOp)(int, int);

// ===== FUNCTION POINTERS =====
int op_add(int a, int b) { return a + b; }
int op_sub(int a, int b) { return a - b; }
int op_mul(int a, int b) { return a * b; }
int op_div(int a, int b) { return b != 0 ? a / b : 0; }

// ===== LEXER =====
// Lex "42+8*5" into tokens
int lex(char *input, struct Token *tokens) {
    int count = 0;
    int i = 0;
    while (input[i] != 0) {
        if (input[i] >= '0' && input[i] <= '9') {
            int val = 0;
            while (input[i] >= '0' && input[i] <= '9') {
                val = val * 10 + (input[i] - '0');
                i++;
            }
            tokens[count].kind = TOK_NUM;
            tokens[count].value = val;
            count++;
        } else if (input[i] == '+') {
            tokens[count].kind = TOK_PLUS;
            tokens[count].value = 0;
            count++;
            i++;
        } else if (input[i] == '-') {
            tokens[count].kind = TOK_MINUS;
            tokens[count].value = 0;
            count++;
            i++;
        } else if (input[i] == '*') {
            tokens[count].kind = TOK_STAR;
            tokens[count].value = 0;
            count++;
            i++;
        } else if (input[i] == '/') {
            tokens[count].kind = TOK_SLASH;
            tokens[count].value = 0;
            count++;
            i++;
        } else {
            i++;
        }
    }
    tokens[count].kind = TOK_EOF;
    tokens[count].value = 0;
    count++;
    return count;
}

void print_token(struct Token *t) {
    switch (t->kind) {
        case TOK_NUM:   printf("NUM:%d", t->value); break;
        case TOK_PLUS:  printf("PLUS"); break;
        case TOK_MINUS: printf("MINUS"); break;
        case TOK_STAR:  printf("STAR"); break;
        case TOK_SLASH: printf("SLASH"); break;
        case TOK_EOF:   printf("EOF"); break;
    }
}

// ===== PARSER (recursive descent) =====
Node *make_node(enum NodeKind kind, int val, Node *l, Node *r) {
    Node *n = (Node *)malloc(sizeof(Node));
    n->kind = kind;
    n->value = val;
    n->left = l;
    n->right = r;
    return n;
}

int parse_pos;
struct Token *parse_tokens;

Node *parse_expr(void);
Node *parse_term(void);
Node *parse_primary(void);

Node *parse_primary(void) {
    if (parse_tokens[parse_pos].kind == TOK_NUM) {
        int val = parse_tokens[parse_pos].value;
        parse_pos++;
        return make_node(ND_NUM, val, 0, 0);
    }
    return make_node(ND_NUM, 0, 0, 0);
}

Node *parse_term(void) {
    Node *left = parse_primary();
    while (parse_tokens[parse_pos].kind == TOK_STAR ||
           parse_tokens[parse_pos].kind == TOK_SLASH) {
        enum NodeKind nk = (parse_tokens[parse_pos].kind == TOK_STAR) ? ND_MUL : ND_DIV;
        parse_pos++;
        Node *right = parse_primary();
        left = make_node(nk, 0, left, right);
    }
    return left;
}

Node *parse_expr(void) {
    Node *left = parse_term();
    while (parse_tokens[parse_pos].kind == TOK_PLUS ||
           parse_tokens[parse_pos].kind == TOK_MINUS) {
        enum NodeKind nk = (parse_tokens[parse_pos].kind == TOK_PLUS) ? ND_ADD : ND_SUB;
        parse_pos++;
        Node *right = parse_term();
        left = make_node(nk, 0, left, right);
    }
    return left;
}

// ===== EVALUATOR =====
int eval_node(Node *n) {
    if (n->kind == ND_NUM) return n->value;
    int l = eval_node(n->left);
    int r = eval_node(n->right);
    switch (n->kind) {
        case ND_ADD: return l + r;
        case ND_SUB: return l - r;
        case ND_MUL: return l * r;
        case ND_DIV: return r != 0 ? l / r : 0;
        default: return 0;
    }
}

// ===== OPTIMIZER: constant folding =====
Node *optimize(Node *n) {
    if (n->kind == ND_NUM) return n;
    n->left = optimize(n->left);
    n->right = optimize(n->right);
    if (n->left->kind == ND_NUM && n->right->kind == ND_NUM) {
        int val = eval_node(n);
        n->kind = ND_NUM;
        n->value = val;
        // Don't free children for simplicity
        n->left = 0;
        n->right = 0;
    }
    return n;
}

// ===== CODEGEN: count instructions =====
int codegen_count(Node *n) {
    if (n->kind == ND_NUM) return 1;  // load immediate
    return codegen_count(n->left) + codegen_count(n->right) + 1;
}

// Print AST
void print_ast(Node *n) {
    if (n->kind == ND_NUM) {
        printf("%d", n->value);
        return;
    }
    char *ops[5];
    ops[0] = "NUM"; ops[1] = "ADD"; ops[2] = "SUB"; ops[3] = "MUL"; ops[4] = "DIV";
    printf("%s(", ops[n->kind]);
    print_ast(n->left);
    printf(",");
    print_ast(n->right);
    printf(")");
}

void free_tree(Node *n) {
    if (n == 0) return;
    free_tree(n->left);
    free_tree(n->right);
    free(n);
}

// ===== HASH TABLE (simple) =====
int hash_contains(int *table, int size, int key) {
    int idx = ((unsigned int)key) % (unsigned int)size;
    int i;
    for (i = 0; i < size; i++) {
        int probe = (idx + i) % size;
        if (table[probe] == key) return 1;
        if (table[probe] == -1) return 0;
    }
    return 0;
}

void hash_insert(int *table, int size, int key) {
    int idx = ((unsigned int)key) % (unsigned int)size;
    int i;
    for (i = 0; i < size; i++) {
        int probe = (idx + i) % size;
        if (table[probe] == -1) {
            table[probe] = key;
            return;
        }
    }
}

// ===== BITWISE OPS =====
int test_bitops(void) {
    unsigned char a = 0xAA;  // 10101010 = 170
    unsigned char b = 0x55;  // 01010101 = 85
    unsigned char c = a | b; // 11111111 = 255
    unsigned char d = a & b; // 00000000 = 0
    printf("bitops=%d %d %d %d\n", (int)a, (int)b, (int)c, (int)d);
    return (a == 170 && b == 85 && c == 255 && d == 0);
}

// ===== GOTO =====
int test_goto(void) {
    int sum = 0;
    int i = 1;
loop:
    sum = sum + i;
    i++;
    if (i <= 10) goto loop;
    return sum;  // 55
}

// ===== DO-WHILE =====
int test_dowhile(void) {
    int count = 0;
    do {
        count++;
    } while (count < 10);
    return count;
}

// ===== CONTINUE =====
int test_continue(void) {
    int sum = 0;
    int i;
    for (i = 0; i < 10; i++) {
        if (i % 2 == 0) continue;  // skip evens
        sum = sum + i;  // 1+3+5+7+9 = 25
    }
    return sum;
}

// ===== NESTED BREAK =====
int test_nested_break(void) {
    int count = 0;
    int i;
    int j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            if (j >= 3) break;
            count++;
        }
        if (i >= 3) break;
    }
    return count;  // 4 iterations of i * 3 iterations of j = 12
}

// ===== TERNARY CHAIN =====
int test_ternary(int x) {
    return (x < 0) ? 1 : (x < 10) ? 2 : (x < 100) ? 3 : 4;
}

// ===== FIBONACCI (recursion) =====
int fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

// ===== FUNCTION POINTER TABLE =====
int test_funcptr_table(void) {
    BinOp ops[4];
    ops[0] = op_add;
    ops[1] = op_sub;
    ops[2] = op_mul;
    ops[3] = op_div;

    int result = 0;
    result = result + ops[0](10, 20);  // 30
    result = result + ops[1](100, 50); // 50
    result = result + ops[2](3, 10);   // 30
    result = result + ops[3](100, 100); // 1
    // 30 + 50 + 30 + 1 = 111
    // Hmm let me adjust: 10+20=30, 100-50=50, 3*10=30, 100/100=1
    // total = 111? Let me just return it
    return result;
}

// ===== 2D ARRAY =====
int test_2d_array(void) {
    int grid[5][5];
    int i;
    int j;
    int sum = 0;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            grid[i][j] = (i + 1) * (j + 1);
        }
    }
    // Sum all: sum of (i+1)*(j+1) for i,j in 0..4
    // = (sum 1..5) * (sum 1..5) = 15 * 15 = 225
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            sum = sum + grid[i][j];
        }
    }
    return sum;
}

// ===== LINKED STRUCT CHAIN =====
struct Link {
    int val;
    struct Link *next;
};

int test_struct_chain(void) {
    struct Link a;
    struct Link b;
    struct Link c;
    a.val = 1; a.next = &b;
    b.val = 2; b.next = &c;
    c.val = 3; c.next = 0;

    int sum = 0;
    struct Link *p = &a;
    while (p != 0) {
        sum = sum + p->val;
        p = p->next;
    }
    return sum;  // 6
}

int main(void) {
    int pass = 1;

    // ===== LEXER TEST =====
    char input[20];
    input[0]='4'; input[1]='2'; input[2]='+'; input[3]='8';
    input[4]='*'; input[5]='5'; input[6]=0;

    struct Token tokens[20];
    int count = lex(input, tokens);
    printf("lex_count=%d\n", count);
    if (count != 6) pass = 0;

    printf("tokens=");
    int t;
    for (t = 0; t < count; t++) {
        if (t > 0) printf(" ");
        print_token(&tokens[t]);
    }
    printf("\n");

    // ===== PARSER TEST =====
    parse_pos = 0;
    parse_tokens = tokens;
    Node *tree = parse_expr();

    // 42+8*5 should parse as ADD(42, MUL(8, 5))
    printf("parse_tree=");
    print_ast(tree);
    printf("\n");

    // ===== EVAL TEST =====
    int result = eval_node(tree);
    printf("eval=%d\n", result);
    if (result != 82) pass = 0;  // 42 + 8*5 = 82

    // ===== OPTIMIZER TEST =====
    Node *opt_tree = optimize(tree);
    printf("opt=%d\n", eval_node(opt_tree));
    if (eval_node(opt_tree) != 82) pass = 0;

    // ===== CODEGEN TEST =====
    // After optimization (constant folding), tree is a single number node
    int instr = codegen_count(opt_tree);
    printf("codegen=%d\n", instr);
    if (instr != 1) pass = 0;

    free_tree(opt_tree);

    // ===== HASH SET TEST =====
    int htable[16];
    int h;
    for (h = 0; h < 16; h++) htable[h] = -1;
    hash_insert(htable, 16, 42);
    hash_insert(htable, 16, 99);
    hash_insert(htable, 16, 7);
    printf("hash_set=%d %d %d %d\n",
        hash_contains(htable, 16, 42),
        hash_contains(htable, 16, 99),
        hash_contains(htable, 16, 7),
        hash_contains(htable, 16, 100));
    if (!hash_contains(htable, 16, 42)) pass = 0;
    if (hash_contains(htable, 16, 100)) pass = 0;

    // ===== BITOPS =====
    if (!test_bitops()) pass = 0;

    // ===== SHIFTS =====
    int sv = 170;
    int sl = sv << 3;   // 170 * 8 = 1360
    int sr = sv >> 3;   // 170 / 8 = 21
    printf("shifts=%d %d\n", sl, sr);
    if (sl != 1360 || sr != 21) pass = 0;

    // ===== GOTO =====
    printf("goto_test=%d\n", test_goto());
    if (test_goto() != 55) pass = 0;

    // ===== DO-WHILE =====
    printf("dowhile=%d\n", test_dowhile());
    if (test_dowhile() != 10) pass = 0;

    // ===== CONTINUE =====
    printf("continue_sum=%d\n", test_continue());
    if (test_continue() != 25) pass = 0;

    // ===== NESTED BREAK =====
    printf("nested_break=%d\n", test_nested_break());
    if (test_nested_break() != 12) pass = 0;

    // ===== TERNARY =====
    printf("ternary_chain=%d\n", test_ternary(50));
    if (test_ternary(50) != 3) pass = 0;

    // ===== COMMA EXPRESSION =====
    int cv = (1, 2, 3, 10);
    cv = cv * 3;
    printf("comma_expr=%d\n", cv);
    if (cv != 30) pass = 0;

    // ===== COMPOUND ASSIGNMENT =====
    int ca = 10;
    ca += 5;    // 15
    ca *= 2;    // 30
    ca -= 3;    // 27
    ca |= 0xF;  // 31
    ca &= 0x2A; // 42 (0x2A = 42, 31 & 42 = 10... hmm)
    // 31 = 0b00011111, 42 = 0b00101010, AND = 0b00001010 = 10
    // Let me fix: make it come out to 42
    // Start fresh
    ca = 10;
    ca += 20;   // 30
    ca *= 2;    // 60
    ca -= 18;   // 42
    printf("compound_assign=%d\n", ca);
    if (ca != 42) pass = 0;

    // ===== ENUM DISPATCH =====
    enum { CMD_A = 100, CMD_B = 200, CMD_C = 300 };
    int ecmd = CMD_C;
    int edisp = 0;
    switch (ecmd) {
        case CMD_A: edisp = 100; break;
        case CMD_B: edisp = 200; break;
        case CMD_C: edisp = 300; break;
    }
    printf("enum_dispatch=%d\n", edisp);
    if (edisp != 300) pass = 0;

    // ===== UNION REINTERPRET =====
    union ValOrPtr uv;
    uv.lval = 0;
    uv.ival = 1;
    printf("union_reinterpret=%d\n", uv.ival);
    if (uv.ival != 1) pass = 0;

    // ===== FIBONACCI =====
    printf("recursion_fib=%d\n", fib(10));
    if (fib(10) != 55) pass = 0;

    // ===== FUNCPTR TABLE =====
    printf("funcptr_table=%d\n", test_funcptr_table());
    // Check actual value
    if (test_funcptr_table() != 111) pass = 0;  // 30+50+30+1=111

    // ===== 2D ARRAY =====
    printf("array_2d_sum=%d\n", test_2d_array());
    if (test_2d_array() != 225) pass = 0;

    // ===== STRUCT CHAIN =====
    printf("struct_chain=%d\n", test_struct_chain());
    if (test_struct_chain() != 6) pass = 0;

    printf("all_pass=%d\n", pass);
    return 0;
}
