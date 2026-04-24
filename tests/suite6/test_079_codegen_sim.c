int printf(const char *fmt, ...);

int NODE_NUM = 0;
int NODE_ADD = 1;
int NODE_SUB = 2;
int NODE_MUL = 3;

int INST_PUSH = 0;
int INST_ADD = 1;
int INST_SUB = 2;
int INST_MUL = 3;
int INST_PRINT = 4;

int node_type[64];
int node_value[64];
int node_left[64];
int node_right[64];
int node_count;

int inst_ops[128];
int inst_args[128];
int inst_count;

int stack[64];
int sp;

int new_num(int val) {
    int id = node_count;
    node_type[id] = NODE_NUM;
    node_value[id] = val;
    node_left[id] = -1;
    node_right[id] = -1;
    node_count = node_count + 1;
    return id;
}

int new_binop(int op, int left, int right) {
    int id = node_count;
    node_type[id] = op;
    node_value[id] = 0;
    node_left[id] = left;
    node_right[id] = right;
    node_count = node_count + 1;
    return id;
}

void emit(int op, int arg) {
    inst_ops[inst_count] = op;
    inst_args[inst_count] = arg;
    inst_count = inst_count + 1;
}

void codegen(int id) {
    if (node_type[id] == NODE_NUM) {
        emit(INST_PUSH, node_value[id]);
        return;
    }
    codegen(node_left[id]);
    codegen(node_right[id]);
    if (node_type[id] == NODE_ADD) {
        emit(INST_ADD, 0);
    } else if (node_type[id] == NODE_SUB) {
        emit(INST_SUB, 0);
    } else if (node_type[id] == NODE_MUL) {
        emit(INST_MUL, 0);
    }
}

void push_val(int v) {
    stack[sp] = v;
    sp = sp + 1;
}

int pop_val(void) {
    sp = sp - 1;
    return stack[sp];
}

int execute(void) {
    sp = 0;
    int i = 0;
    while (i < inst_count) {
        int op = inst_ops[i];
        int arg = inst_args[i];
        if (op == INST_PUSH) {
            push_val(arg);
        } else if (op == INST_ADD) {
            int b = pop_val();
            int a = pop_val();
            push_val(a + b);
        } else if (op == INST_SUB) {
            int b = pop_val();
            int a = pop_val();
            push_val(a - b);
        } else if (op == INST_MUL) {
            int b = pop_val();
            int a = pop_val();
            push_val(a * b);
        } else if (op == INST_PRINT) {
            printf("top=%d\n", stack[sp - 1]);
        }
        i = i + 1;
    }
    if (sp > 0) return stack[sp - 1];
    return 0;
}

char *inst_name(int op) {
    if (op == 0) return "push";
    if (op == 1) return "add";
    if (op == 2) return "sub";
    if (op == 3) return "mul";
    if (op == 4) return "print";
    return "???";
}

void print_instructions(void) {
    int i = 0;
    while (i < inst_count) {
        if (inst_ops[i] == INST_PUSH) {
            printf("  %s %d\n", inst_name(inst_ops[i]), inst_args[i]);
        } else {
            printf("  %s\n", inst_name(inst_ops[i]));
        }
        i = i + 1;
    }
}

int tree_size(int id) {
    if (id < 0) return 0;
    return 1 + tree_size(node_left[id]) + tree_size(node_right[id]);
}

void reset(void) {
    node_count = 0;
    inst_count = 0;
    sp = 0;
}

int main(void) {
    int root;
    int result;

    /* 3 + 4 */
    reset();
    root = new_binop(NODE_ADD, new_num(3), new_num(4));
    codegen(root);
    result = execute();
    // EXPECT: 3+4=7
    printf("3+4=%d\n", result);
    // EXPECT: insts=3
    printf("insts=%d\n", inst_count);

    /* 2 * 6 */
    reset();
    root = new_binop(NODE_MUL, new_num(2), new_num(6));
    codegen(root);
    result = execute();
    // EXPECT: 2*6=12
    printf("2*6=%d\n", result);

    /* 10 - 3 */
    reset();
    root = new_binop(NODE_SUB, new_num(10), new_num(3));
    codegen(root);
    result = execute();
    // EXPECT: 10-3=7
    printf("10-3=%d\n", result);

    /* (2 + 3) * 4 */
    reset();
    int add_node = new_binop(NODE_ADD, new_num(2), new_num(3));
    root = new_binop(NODE_MUL, add_node, new_num(4));
    codegen(root);
    result = execute();
    // EXPECT: (2+3)*4=20
    printf("(2+3)*4=%d\n", result);
    // EXPECT: insts=5
    printf("insts=%d\n", inst_count);
    // EXPECT: tree=5
    printf("tree=%d\n", tree_size(root));

    /* 1 + 2 + 3 */
    reset();
    int left = new_binop(NODE_ADD, new_num(1), new_num(2));
    root = new_binop(NODE_ADD, left, new_num(3));
    codegen(root);
    result = execute();
    // EXPECT: 1+2+3=6
    printf("1+2+3=%d\n", result);

    /* (3 + 5) * (2 + 4) */
    reset();
    int a = new_binop(NODE_ADD, new_num(3), new_num(5));
    int b = new_binop(NODE_ADD, new_num(2), new_num(4));
    root = new_binop(NODE_MUL, a, b);
    codegen(root);
    result = execute();
    // EXPECT: (3+5)*(2+4)=48
    printf("(3+5)*(2+4)=%d\n", result);
    // EXPECT: insts=7
    printf("insts=%d\n", inst_count);

    /* Print generated code for 5 * 3 */
    reset();
    root = new_binop(NODE_MUL, new_num(5), new_num(3));
    codegen(root);
    // EXPECT:   push 5
    // EXPECT:   push 3
    // EXPECT:   mul
    print_instructions();
    result = execute();
    // EXPECT: 5*3=15
    printf("5*3=%d\n", result);

    /* single number */
    reset();
    root = new_num(42);
    codegen(root);
    result = execute();
    // EXPECT: single=42
    printf("single=%d\n", result);
    // EXPECT: insts=1
    printf("insts=%d\n", inst_count);

    /* (10 - 2) * (3 + 1) */
    reset();
    a = new_binop(NODE_SUB, new_num(10), new_num(2));
    b = new_binop(NODE_ADD, new_num(3), new_num(1));
    root = new_binop(NODE_MUL, a, b);
    codegen(root);
    result = execute();
    // EXPECT: (10-2)*(3+1)=32
    printf("(10-2)*(3+1)=%d\n", result);

    return 0;
}
