int printf(const char *fmt, ...);

struct ENode {
    int val;
    int is_op;
    int left;
    int right;
};

struct ENode enodes[64];
int enode_count = 0;

int new_num(int val) {
    int idx = enode_count;
    enodes[idx].val = val;
    enodes[idx].is_op = 0;
    enodes[idx].left = -1;
    enodes[idx].right = -1;
    enode_count = enode_count + 1;
    return idx;
}

int new_op(int op, int left, int right) {
    int idx = enode_count;
    enodes[idx].val = op;
    enodes[idx].is_op = 1;
    enodes[idx].left = left;
    enodes[idx].right = right;
    enode_count = enode_count + 1;
    return idx;
}

int evaluate(int root) {
    int lv;
    int rv;
    if (root == -1) return 0;
    if (!enodes[root].is_op) {
        return enodes[root].val;
    }
    lv = evaluate(enodes[root].left);
    rv = evaluate(enodes[root].right);
    if (enodes[root].val == 43) return lv + rv;
    if (enodes[root].val == 45) return lv - rv;
    if (enodes[root].val == 42) return lv * rv;
    if (enodes[root].val == 47) {
        if (rv != 0) return lv / rv;
        return 0;
    }
    return 0;
}

int inorder_buf[64];
int inorder_cnt;

void inorder(int root) {
    if (root == -1) return;
    inorder(enodes[root].left);
    inorder_buf[inorder_cnt] = enodes[root].val;
    inorder_cnt = inorder_cnt + 1;
    inorder(enodes[root].right);
}

int count_nodes(int root) {
    if (root == -1) return 0;
    return 1 + count_nodes(enodes[root].left) + count_nodes(enodes[root].right);
}

int count_leaves(int root) {
    if (root == -1) return 0;
    if (enodes[root].left == -1 && enodes[root].right == -1) {
        return 1;
    }
    return count_leaves(enodes[root].left) + count_leaves(enodes[root].right);
}

int height(int root) {
    int lh;
    int rh;
    if (root == -1) return 0;
    lh = height(enodes[root].left);
    rh = height(enodes[root].right);
    if (lh > rh) return lh + 1;
    return rh + 1;
}

int stack[64];
int sp = 0;

void push(int v) {
    stack[sp] = v;
    sp = sp + 1;
}

int pop(void) {
    sp = sp - 1;
    return stack[sp];
}

int build_from_postfix(int tokens[], int is_ops[], int len) {
    int i;
    int a;
    int b;
    int nd;
    sp = 0;
    for (i = 0; i < len; i = i + 1) {
        if (is_ops[i]) {
            b = pop();
            a = pop();
            nd = new_op(tokens[i], a, b);
            push(nd);
        } else {
            nd = new_num(tokens[i]);
            push(nd);
        }
    }
    return pop();
}

int main(void) {
    int root;
    int i;
    int tokens[16];
    int is_ops[16];
    int root2;

    root = new_op(43,
        new_op(42, new_num(3), new_num(4)),
        new_num(5));

    // EXPECT: eval 3*4+5: 17
    printf("eval 3*4+5: %d\n", evaluate(root));
    // EXPECT: nodes: 5
    printf("nodes: %d\n", count_nodes(root));
    // EXPECT: leaves: 3
    printf("leaves: %d\n", count_leaves(root));
    // EXPECT: height: 3
    printf("height: %d\n", height(root));

    inorder_cnt = 0;
    inorder(root);
    // EXPECT: inorder[0]: 3
    printf("inorder[0]: %d\n", inorder_buf[0]);
    // EXPECT: inorder[1]: 42
    printf("inorder[1]: %d\n", inorder_buf[1]);
    // EXPECT: inorder[2]: 4
    printf("inorder[2]: %d\n", inorder_buf[2]);
    // EXPECT: inorder[3]: 43
    printf("inorder[3]: %d\n", inorder_buf[3]);
    // EXPECT: inorder[4]: 5
    printf("inorder[4]: %d\n", inorder_buf[4]);

    tokens[0] = 2; is_ops[0] = 0;
    tokens[1] = 3; is_ops[1] = 0;
    tokens[2] = 42; is_ops[2] = 1;
    tokens[3] = 4; is_ops[3] = 0;
    tokens[4] = 43; is_ops[4] = 1;
    root2 = build_from_postfix(tokens, is_ops, 5);

    // EXPECT: postfix eval: 10
    printf("postfix eval: %d\n", evaluate(root2));
    // EXPECT: postfix nodes: 5
    printf("postfix nodes: %d\n", count_nodes(root2));

    tokens[0] = 5; is_ops[0] = 0;
    tokens[1] = 3; is_ops[1] = 0;
    tokens[2] = 45; is_ops[2] = 1;
    tokens[3] = 2; is_ops[3] = 0;
    tokens[4] = 42; is_ops[4] = 1;
    root2 = build_from_postfix(tokens, is_ops, 5);

    // EXPECT: sub mul eval: 4
    printf("sub mul eval: %d\n", evaluate(root2));

    tokens[0] = 10; is_ops[0] = 0;
    tokens[1] = 2; is_ops[1] = 0;
    tokens[2] = 47; is_ops[2] = 1;
    tokens[3] = 3; is_ops[3] = 0;
    tokens[4] = 43; is_ops[4] = 1;
    root2 = build_from_postfix(tokens, is_ops, 5);

    // EXPECT: div add eval: 8
    printf("div add eval: %d\n", evaluate(root2));

    tokens[0] = 1; is_ops[0] = 0;
    tokens[1] = 2; is_ops[1] = 0;
    tokens[2] = 43; is_ops[2] = 1;
    tokens[3] = 3; is_ops[3] = 0;
    tokens[4] = 4; is_ops[4] = 0;
    tokens[5] = 42; is_ops[5] = 1;
    tokens[6] = 43; is_ops[6] = 1;
    root2 = build_from_postfix(tokens, is_ops, 7);

    // EXPECT: complex eval: 15
    printf("complex eval: %d\n", evaluate(root2));
    // EXPECT: complex nodes: 7
    printf("complex nodes: %d\n", count_nodes(root2));
    // EXPECT: complex leaves: 4
    printf("complex leaves: %d\n", count_leaves(root2));
    // EXPECT: complex height: 3
    printf("complex height: %d\n", height(root2));

    return 0;
}
