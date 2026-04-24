int printf(const char *fmt, ...);

int NODE_NUM = 0;
int NODE_ADD = 1;
int NODE_SUB = 2;
int NODE_MUL = 3;
int NODE_DIV = 4;

int node_type[128];
int node_value[128];
int node_left[128];
int node_right[128];
int node_count;

int new_num_node(int val) {
    int id = node_count;
    node_type[id] = NODE_NUM;
    node_value[id] = val;
    node_left[id] = -1;
    node_right[id] = -1;
    node_count = node_count + 1;
    return id;
}

int new_binop_node(int op, int left, int right) {
    int id = node_count;
    node_type[id] = op;
    node_value[id] = 0;
    node_left[id] = left;
    node_right[id] = right;
    node_count = node_count + 1;
    return id;
}

char input_buf[256];
int pos;

int peek(void) {
    while (input_buf[pos] == 32) pos = pos + 1;
    return input_buf[pos];
}

int read_number(void) {
    int num = 0;
    while (input_buf[pos] >= 48 && input_buf[pos] <= 57) {
        num = num * 10 + (input_buf[pos] - 48);
        pos = pos + 1;
    }
    return num;
}

int parse_expr(void);

int parse_primary(void) {
    int c = peek();
    if (c == 40) {
        pos = pos + 1;
        int result = parse_expr();
        peek();
        pos = pos + 1;
        return result;
    }
    int num = read_number();
    return new_num_node(num);
}

int parse_term(void) {
    int left = parse_primary();
    while (1) {
        int c = peek();
        if (c == 42) {
            pos = pos + 1;
            int right = parse_primary();
            left = new_binop_node(NODE_MUL, left, right);
        } else if (c == 47) {
            pos = pos + 1;
            int right = parse_primary();
            left = new_binop_node(NODE_DIV, left, right);
        } else {
            break;
        }
    }
    return left;
}

int parse_expr(void) {
    int left = parse_term();
    while (1) {
        int c = peek();
        if (c == 43) {
            pos = pos + 1;
            int right = parse_term();
            left = new_binop_node(NODE_ADD, left, right);
        } else if (c == 45) {
            pos = pos + 1;
            int right = parse_term();
            left = new_binop_node(NODE_SUB, left, right);
        } else {
            break;
        }
    }
    return left;
}

int eval_node(int id) {
    if (node_type[id] == NODE_NUM) return node_value[id];
    int l = eval_node(node_left[id]);
    int r = eval_node(node_right[id]);
    if (node_type[id] == NODE_ADD) return l + r;
    if (node_type[id] == NODE_SUB) return l - r;
    if (node_type[id] == NODE_MUL) return l * r;
    if (node_type[id] == NODE_DIV) return l / r;
    return 0;
}

int tree_depth(int id) {
    if (id < 0) return 0;
    if (node_type[id] == NODE_NUM) return 1;
    int ld = tree_depth(node_left[id]);
    int rd = tree_depth(node_right[id]);
    if (ld > rd) return ld + 1;
    return rd + 1;
}

int count_nodes(int id) {
    if (id < 0) return 0;
    return 1 + count_nodes(node_left[id]) + count_nodes(node_right[id]);
}

void copy_str(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int parse_and_eval(char *expr) {
    copy_str(input_buf, expr);
    pos = 0;
    node_count = 0;
    int root = parse_expr();
    return eval_node(root);
}

int main(void) {
    int result;

    result = parse_and_eval("3 + 4");
    // EXPECT: 3+4=7
    printf("3+4=%d\n", result);

    result = parse_and_eval("10 - 3");
    // EXPECT: 10-3=7
    printf("10-3=%d\n", result);

    result = parse_and_eval("2 * 6");
    // EXPECT: 2*6=12
    printf("2*6=%d\n", result);

    result = parse_and_eval("20 / 4");
    // EXPECT: 20/4=5
    printf("20/4=%d\n", result);

    result = parse_and_eval("2 + 3 * 4");
    // EXPECT: 2+3*4=14
    printf("2+3*4=%d\n", result);

    result = parse_and_eval("(2 + 3) * 4");
    // EXPECT: (2+3)*4=20
    printf("(2+3)*4=%d\n", result);

    result = parse_and_eval("100 - 50 / 5");
    // EXPECT: 100-50/5=90
    printf("100-50/5=%d\n", result);

    result = parse_and_eval("(100 - 50) / 5");
    // EXPECT: (100-50)/5=10
    printf("(100-50)/5=%d\n", result);

    copy_str(input_buf, "1 + 2 + 3");
    pos = 0;
    node_count = 0;
    int root = parse_expr();
    // EXPECT: nodes=5
    printf("nodes=%d\n", count_nodes(root));
    // EXPECT: depth=3
    printf("depth=%d\n", tree_depth(root));
    // EXPECT: eval=6
    printf("eval=%d\n", eval_node(root));

    result = parse_and_eval("(1 + 2) * (3 + 4)");
    // EXPECT: (1+2)*(3+4)=21
    printf("(1+2)*(3+4)=%d\n", result);

    result = parse_and_eval("8 / 2 * 3");
    // EXPECT: 8/2*3=12
    printf("8/2*3=%d\n", result);

    result = parse_and_eval("42");
    // EXPECT: single=42
    printf("single=%d\n", result);

    return 0;
}
