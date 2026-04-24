// stress_097_expression_tree.c - Expression tree torture test
// Exercises: build expression tree from postfix, evaluate recursively,
// count nodes, compute depth, multiple trees tested.
// EXPECT: eval1=14
// EXPECT: nodes1=5
// EXPECT: depth1=3
// EXPECT: eval2=44
// EXPECT: nodes2=9
// EXPECT: depth2=4
// EXPECT: eval3=-1
// EXPECT: nodes3=3
// EXPECT: eval4=120
// EXPECT: nodes4=11
// EXPECT: depth4=4
// EXPECT: eval5=23
// EXPECT: leaves5=4
// EXPECT: all_pass=1

int printf(const char *fmt, ...);
void *malloc(unsigned long);
void free(void *);

// Expression tree node
struct Expr {
    int kind;      // 0=number, 1=add, 2=sub, 3=mul, 4=div
    int value;     // only for kind==0
    struct Expr *left;
    struct Expr *right;
};

struct Expr *make_num(int val) {
    struct Expr *e = (struct Expr *)malloc(sizeof(struct Expr));
    e->kind = 0;
    e->value = val;
    e->left = 0;
    e->right = 0;
    return e;
}

struct Expr *make_op(int kind, struct Expr *left, struct Expr *right) {
    struct Expr *e = (struct Expr *)malloc(sizeof(struct Expr));
    e->kind = kind;
    e->value = 0;
    e->left = left;
    e->right = right;
    return e;
}

// Evaluate expression tree
int eval(struct Expr *e) {
    if (e->kind == 0) return e->value;
    int l = eval(e->left);
    int r = eval(e->right);
    if (e->kind == 1) return l + r;
    if (e->kind == 2) return l - r;
    if (e->kind == 3) return l * r;
    if (e->kind == 4) {
        if (r == 0) return 0;
        return l / r;
    }
    return 0;
}

// Count nodes
int count_nodes(struct Expr *e) {
    if (e == 0) return 0;
    return 1 + count_nodes(e->left) + count_nodes(e->right);
}

// Compute depth
int max(int a, int b) { return a > b ? a : b; }

int depth(struct Expr *e) {
    if (e == 0) return 0;
    if (e->left == 0 && e->right == 0) return 1;
    return 1 + max(depth(e->left), depth(e->right));
}

// Count leaves
int count_leaves(struct Expr *e) {
    if (e == 0) return 0;
    if (e->left == 0 && e->right == 0) return 1;
    return count_leaves(e->left) + count_leaves(e->right);
}

// Build expression from postfix using stack
// Tokens: positive numbers are literals, -1=add, -2=sub, -3=mul, -4=div
struct Expr *build_from_postfix(int *tokens, int n) {
    struct Expr *stack[64];
    int sp = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (tokens[i] >= 0) {
            stack[sp] = make_num(tokens[i]);
            sp++;
        } else {
            int kind;
            if (tokens[i] == -1) kind = 1;       // add
            else if (tokens[i] == -2) kind = 2;   // sub
            else if (tokens[i] == -3) kind = 3;   // mul
            else kind = 4;                         // div
            struct Expr *right = stack[sp - 1]; sp--;
            struct Expr *left = stack[sp - 1]; sp--;
            stack[sp] = make_op(kind, left, right);
            sp++;
        }
    }
    return stack[0];
}

// Free tree
void free_tree(struct Expr *e) {
    if (e == 0) return;
    free_tree(e->left);
    free_tree(e->right);
    free(e);
}

int main(void) {
    int pass = 1;

    // Tree 1: (3 + 4) * 2 = 14
    // Postfix: 3 4 + 2 *
    // Manual build
    struct Expr *t1 = make_op(3,
        make_op(1, make_num(3), make_num(4)),
        make_num(2));
    printf("eval1=%d\n", eval(t1));
    if (eval(t1) != 14) pass = 0;
    printf("nodes1=%d\n", count_nodes(t1));
    if (count_nodes(t1) != 5) pass = 0;  // 3 nums + 2 ops
    printf("depth1=%d\n", depth(t1));
    if (depth(t1) != 3) pass = 0;
    free_tree(t1);

    // Tree 2: ((5 + 3) * 2 + (10 - 6)) * 2 = (16 + 4) * 2 = 40... wait
    // Postfix: 5 3 + 2 * 10 6 - + 2 *
    // (5+3)*2 = 16, 10-6=4, 16+4=20, 20*2=40
    // Let me use postfix builder
    int tok2[9];
    tok2[0] = 5; tok2[1] = 3; tok2[2] = -1;  // 5 3 +
    tok2[3] = 2; tok2[4] = -3;                // * 2
    tok2[5] = 10; tok2[6] = 6; tok2[7] = -2;  // 10 6 -
    tok2[8] = -1;                              // +
    // Wait, that's (5+3)*2 + (10-6) = 16+4 = 20, only 9 tokens
    // Let me add final *2: need 11 tokens
    int tok2b[11];
    tok2b[0] = 5; tok2b[1] = 3; tok2b[2] = -1;  // 5+3=8
    tok2b[3] = 2; tok2b[4] = -3;                  // 8*2=16
    tok2b[5] = 10; tok2b[6] = 6; tok2b[7] = -2;  // 10-6=4
    tok2b[8] = -1;                                 // 16+4=20
    tok2b[9] = 2; tok2b[10] = -3;                 // 20*2=40
    // Actually let me just build: 5 3 + 8 * -> 64, that's simpler
    // Let me reconsider. I want to test the postfix builder with good values.
    // (2 + 3) * (8 + 1) - 1 = 5*9-1 = 44
    // Postfix: 2 3 + 8 1 + * 1 -
    int post2[9];
    post2[0] = 2; post2[1] = 3; post2[2] = -1;  // 2+3=5
    post2[3] = 8; post2[4] = 1; post2[5] = -1;  // 8+1=9
    post2[6] = -3;  // 5*9=45
    post2[7] = 1; post2[8] = -2;  // 45-1=44
    struct Expr *t2 = build_from_postfix(post2, 9);
    printf("eval2=%d\n", eval(t2));
    if (eval(t2) != 44) pass = 0;
    printf("nodes2=%d\n", count_nodes(t2));
    if (count_nodes(t2) != 9) pass = 0;
    printf("depth2=%d\n", depth(t2));
    if (depth(t2) != 4) pass = 0;
    free_tree(t2);

    // Tree 3: 3 - 4 = -1
    // Postfix: 3 4 -
    int post3[3];
    post3[0] = 3; post3[1] = 4; post3[2] = -2;
    struct Expr *t3 = build_from_postfix(post3, 3);
    printf("eval3=%d\n", eval(t3));
    if (eval(t3) != -1) pass = 0;
    printf("nodes3=%d\n", count_nodes(t3));
    if (count_nodes(t3) != 3) pass = 0;
    free_tree(t3);

    // Tree 4: (1+2)*(3+4)*(5-1) = 3*7*4 = 84... let me try
    // Actually: ((2*3) + (4*5)) * (10/2) = (6+20)*5 = 130
    // Postfix: 2 3 * 4 5 * + 10 2 / *
    // = 6 + 20 = 26, 26 * 5 = 130
    // Hmm let me pick something cleaner
    // (2+3) * (4+2) * (6-2) = 5*6*4 = 120
    // Postfix: 2 3 + 4 2 + * 6 2 - *
    int post4[11];
    post4[0] = 2; post4[1] = 3; post4[2] = -1;  // 2+3=5
    post4[3] = 4; post4[4] = 2; post4[5] = -1;  // 4+2=6
    post4[6] = -3;  // 5*6=30
    post4[7] = 6; post4[8] = 2; post4[9] = -2;  // 6-2=4
    post4[10] = -3;  // 30*4=120
    struct Expr *t4 = build_from_postfix(post4, 11);
    printf("eval4=%d\n", eval(t4));
    if (eval(t4) != 120) pass = 0;
    printf("nodes4=%d\n", count_nodes(t4));
    if (count_nodes(t4) != 11) pass = 0;  // 6 nums + 5 ops
    printf("depth4=%d\n", depth(t4));
    if (depth(t4) != 4) pass = 0;
    free_tree(t4);

    // Tree 5: (10 + 5) + (3 + 5) = 23
    // Postfix: 10 5 + 3 5 + +
    int post5[7];
    post5[0] = 10; post5[1] = 5; post5[2] = -1;  // 15
    post5[3] = 3; post5[4] = 5; post5[5] = -1;   // 8
    post5[6] = -1;                                  // 23
    struct Expr *t5 = build_from_postfix(post5, 7);
    printf("eval5=%d\n", eval(t5));
    if (eval(t5) != 23) pass = 0;
    printf("leaves5=%d\n", count_leaves(t5));
    if (count_leaves(t5) != 4) pass = 0;
    free_tree(t5);

    printf("all_pass=%d\n", pass);
    return 0;
}
