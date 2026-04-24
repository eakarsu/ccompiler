int printf(const char *fmt, ...);

enum NodeKind {
    NODE_NUM,
    NODE_ADD,
    NODE_SUB,
    NODE_MUL,
    NODE_DIV,
    NODE_NEG,
    NODE_VAR
};

struct ASTNode {
    int kind;
    int value;
    int left;
    int right;
    char var_name;
    int is_const;
    int const_val;
};

struct NodePool {
    struct ASTNode nodes[64];
    int count;
};

int new_num(struct NodePool *pool, int val) {
    int idx = pool->count;
    pool->nodes[idx].kind = NODE_NUM;
    pool->nodes[idx].value = val;
    pool->nodes[idx].left = -1;
    pool->nodes[idx].right = -1;
    pool->nodes[idx].var_name = 0;
    pool->nodes[idx].is_const = 1;
    pool->nodes[idx].const_val = val;
    pool->count = pool->count + 1;
    return idx;
}

int new_var(struct NodePool *pool, char name) {
    int idx = pool->count;
    pool->nodes[idx].kind = NODE_VAR;
    pool->nodes[idx].value = 0;
    pool->nodes[idx].left = -1;
    pool->nodes[idx].right = -1;
    pool->nodes[idx].var_name = name;
    pool->nodes[idx].is_const = 0;
    pool->nodes[idx].const_val = 0;
    pool->count = pool->count + 1;
    return idx;
}

int new_binop(struct NodePool *pool, int kind, int left, int right) {
    int idx = pool->count;
    pool->nodes[idx].kind = kind;
    pool->nodes[idx].value = 0;
    pool->nodes[idx].left = left;
    pool->nodes[idx].right = right;
    pool->nodes[idx].var_name = 0;
    pool->nodes[idx].is_const = 0;
    pool->nodes[idx].const_val = 0;
    pool->count = pool->count + 1;
    return idx;
}

int new_neg(struct NodePool *pool, int child) {
    int idx = pool->count;
    pool->nodes[idx].kind = NODE_NEG;
    pool->nodes[idx].value = 0;
    pool->nodes[idx].left = child;
    pool->nodes[idx].right = -1;
    pool->nodes[idx].var_name = 0;
    pool->nodes[idx].is_const = 0;
    pool->nodes[idx].const_val = 0;
    pool->count = pool->count + 1;
    return idx;
}

/* Constant fold: returns 1 if node was folded to a constant */
int const_fold(struct NodePool *pool, int idx) {
    struct ASTNode *node = &pool->nodes[idx];

    if (node->kind == NODE_NUM) {
        node->is_const = 1;
        node->const_val = node->value;
        return 1;
    }

    if (node->kind == NODE_VAR) {
        node->is_const = 0;
        return 0;
    }

    if (node->kind == NODE_NEG) {
        if (const_fold(pool, node->left)) {
            node->is_const = 1;
            node->const_val = -pool->nodes[node->left].const_val;
            node->kind = NODE_NUM;
            node->value = node->const_val;
            return 1;
        }
        return 0;
    }

    /* Binary ops */
    int left_const = const_fold(pool, node->left);
    int right_const = const_fold(pool, node->right);

    if (left_const && right_const) {
        int lv = pool->nodes[node->left].const_val;
        int rv = pool->nodes[node->right].const_val;
        int result = 0;

        switch (node->kind) {
            case NODE_ADD: result = lv + rv; break;
            case NODE_SUB: result = lv - rv; break;
            case NODE_MUL: result = lv * rv; break;
            case NODE_DIV:
                if (rv != 0) result = lv / rv;
                break;
        }

        node->is_const = 1;
        node->const_val = result;
        node->kind = NODE_NUM;
        node->value = result;
        return 1;
    }

    /* Partial folding: x + 0 => x, x * 0 => 0, x * 1 => x */
    if (node->kind == NODE_ADD && right_const && pool->nodes[node->right].const_val == 0) {
        /* fold x+0 to x: mark as same as left */
        node->is_const = pool->nodes[node->left].is_const;
        node->const_val = pool->nodes[node->left].const_val;
        return node->is_const;
    }
    if (node->kind == NODE_MUL && right_const && pool->nodes[node->right].const_val == 0) {
        node->is_const = 1;
        node->const_val = 0;
        node->kind = NODE_NUM;
        node->value = 0;
        return 1;
    }
    if (node->kind == NODE_MUL && right_const && pool->nodes[node->right].const_val == 1) {
        node->is_const = pool->nodes[node->left].is_const;
        node->const_val = pool->nodes[node->left].const_val;
        return node->is_const;
    }

    return 0;
}

char *node_kind_str(int kind) {
    if (kind == NODE_NUM) return "NUM";
    if (kind == NODE_ADD) return "ADD";
    if (kind == NODE_SUB) return "SUB";
    if (kind == NODE_MUL) return "MUL";
    if (kind == NODE_DIV) return "DIV";
    if (kind == NODE_NEG) return "NEG";
    if (kind == NODE_VAR) return "VAR";
    return "???";
}

int main() {
    struct NodePool pool;
    int root;
    int folded;

    /* Test 1: 3 + 4 => 7 */
    pool.count = 0;
    root = new_binop(&pool, NODE_ADD, new_num(&pool, 3), new_num(&pool, 4));
    folded = const_fold(&pool, root);
    // EXPECT: Test 1: 3+4 folded=1 result=7
    printf("Test 1: 3+4 folded=%d result=%d\n", folded, pool.nodes[root].const_val);

    /* Test 2: (2 + 3) * (4 - 1) => 5 * 3 => 15 */
    pool.count = 0;
    root = new_binop(&pool, NODE_MUL,
        new_binop(&pool, NODE_ADD, new_num(&pool, 2), new_num(&pool, 3)),
        new_binop(&pool, NODE_SUB, new_num(&pool, 4), new_num(&pool, 1)));
    folded = const_fold(&pool, root);
    // EXPECT: Test 2: (2+3)*(4-1) folded=1 result=15
    printf("Test 2: (2+3)*(4-1) folded=%d result=%d\n", folded, pool.nodes[root].const_val);

    /* Test 3: -(-5) => 5 */
    pool.count = 0;
    root = new_neg(&pool, new_neg(&pool, new_num(&pool, 5)));
    folded = const_fold(&pool, root);
    // EXPECT: Test 3: -(-5) folded=1 result=5
    printf("Test 3: -(-5) folded=%d result=%d\n", folded, pool.nodes[root].const_val);

    /* Test 4: x + 3 => not fully foldable */
    pool.count = 0;
    root = new_binop(&pool, NODE_ADD, new_var(&pool, 'x'), new_num(&pool, 3));
    folded = const_fold(&pool, root);
    // EXPECT: Test 4: x+3 folded=0 kind=ADD
    printf("Test 4: x+3 folded=%d kind=%s\n", folded, node_kind_str(pool.nodes[root].kind));

    /* Test 5: x * 0 => 0 */
    pool.count = 0;
    root = new_binop(&pool, NODE_MUL, new_var(&pool, 'x'), new_num(&pool, 0));
    folded = const_fold(&pool, root);
    // EXPECT: Test 5: x*0 folded=1 result=0
    printf("Test 5: x*0 folded=%d result=%d\n", folded, pool.nodes[root].const_val);

    /* Test 6: x * 1 => x (not const but simplified) */
    pool.count = 0;
    root = new_binop(&pool, NODE_MUL, new_var(&pool, 'x'), new_num(&pool, 1));
    folded = const_fold(&pool, root);
    // EXPECT: Test 6: x*1 folded=0
    printf("Test 6: x*1 folded=%d\n", folded);

    /* Test 7: 100 / 10 / 2 => 5 */
    pool.count = 0;
    root = new_binop(&pool, NODE_DIV,
        new_binop(&pool, NODE_DIV, new_num(&pool, 100), new_num(&pool, 10)),
        new_num(&pool, 2));
    folded = const_fold(&pool, root);
    // EXPECT: Test 7: 100/10/2 folded=1 result=5
    printf("Test 7: 100/10/2 folded=%d result=%d\n", folded, pool.nodes[root].const_val);

    /* Test 8: (3 + 5) * 2 + (10 - 4) => 22 */
    pool.count = 0;
    root = new_binop(&pool, NODE_ADD,
        new_binop(&pool, NODE_MUL,
            new_binop(&pool, NODE_ADD, new_num(&pool, 3), new_num(&pool, 5)),
            new_num(&pool, 2)),
        new_binop(&pool, NODE_SUB, new_num(&pool, 10), new_num(&pool, 4)));
    folded = const_fold(&pool, root);
    // EXPECT: Test 8: (3+5)*2+(10-4) folded=1 result=22
    printf("Test 8: (3+5)*2+(10-4) folded=%d result=%d\n", folded, pool.nodes[root].const_val);

    /* Test 9: x + 0 => not folded to const but simplified */
    pool.count = 0;
    root = new_binop(&pool, NODE_ADD, new_var(&pool, 'x'), new_num(&pool, 0));
    folded = const_fold(&pool, root);
    // EXPECT: Test 9: x+0 folded=0
    printf("Test 9: x+0 folded=%d\n", folded);

    // EXPECT: All constant folding tests done
    printf("All constant folding tests done\n");
    return 0;
}
