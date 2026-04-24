int printf(const char *fmt, ...);

// Constant folding optimizer
// Walks an expression tree and folds constant sub-expressions

int ND_NUM = 1;
int ND_ADD = 2;
int ND_SUB = 3;
int ND_MUL = 4;
int ND_DIV = 5;
int ND_NEG = 6;
int ND_VAR = 7;

struct Node {
    int kind;
    int value;
    int left;
    int right;
    int folded;
};

struct Node nodes[256];
int node_count;

int alloc_node(int kind, int value, int left, int right) {
    int idx = node_count;
    nodes[idx].kind = kind;
    nodes[idx].value = value;
    nodes[idx].left = left;
    nodes[idx].right = right;
    nodes[idx].folded = 0;
    node_count = node_count + 1;
    return idx;
}

int mk_num(int val) {
    return alloc_node(ND_NUM, val, -1, -1);
}

int mk_var(int id) {
    return alloc_node(ND_VAR, id, -1, -1);
}

int mk_bin(int kind, int l, int r) {
    return alloc_node(kind, 0, l, r);
}

int mk_neg(int child) {
    return alloc_node(ND_NEG, 0, child, -1);
}

int is_const(int idx) {
    if (idx < 0) return 0;
    return nodes[idx].kind == ND_NUM;
}

int fold_count;

int constant_fold(int idx) {
    if (idx < 0) return idx;

    struct Node n = nodes[idx];

    if (n.kind == ND_NUM || n.kind == ND_VAR) return idx;

    int new_left = constant_fold(n.left);
    int new_right = constant_fold(n.right);

    nodes[idx].left = new_left;
    nodes[idx].right = new_right;

    if (n.kind == ND_NEG) {
        if (is_const(new_left)) {
            nodes[idx].kind = ND_NUM;
            nodes[idx].value = -nodes[new_left].value;
            nodes[idx].left = -1;
            nodes[idx].folded = 1;
            fold_count = fold_count + 1;
            return idx;
        }
        return idx;
    }

    if (!is_const(new_left) || !is_const(new_right)) return idx;

    int lv = nodes[new_left].value;
    int rv = nodes[new_right].value;
    int result = 0;

    if (n.kind == ND_ADD) result = lv + rv;
    else if (n.kind == ND_SUB) result = lv - rv;
    else if (n.kind == ND_MUL) result = lv * rv;
    else if (n.kind == ND_DIV) {
        if (rv == 0) return idx;
        result = lv / rv;
    }
    else return idx;

    nodes[idx].kind = ND_NUM;
    nodes[idx].value = result;
    nodes[idx].left = -1;
    nodes[idx].right = -1;
    nodes[idx].folded = 1;
    fold_count = fold_count + 1;
    return idx;
}

// Strength reduction: multiply by power of 2 -> shift
int strength_reduce(int idx) {
    if (idx < 0) return idx;
    struct Node n = nodes[idx];

    nodes[idx].left = strength_reduce(n.left);
    nodes[idx].right = strength_reduce(n.right);

    if (n.kind == ND_MUL) {
        if (is_const(nodes[idx].right)) {
            int rv = nodes[nodes[idx].right].value;
            if (rv == 0) {
                nodes[idx].kind = ND_NUM;
                nodes[idx].value = 0;
                nodes[idx].left = -1;
                nodes[idx].right = -1;
                fold_count = fold_count + 1;
                return idx;
            }
            if (rv == 1) {
                return nodes[idx].left;
            }
        }
        if (is_const(nodes[idx].left)) {
            int lv = nodes[nodes[idx].left].value;
            if (lv == 0) {
                nodes[idx].kind = ND_NUM;
                nodes[idx].value = 0;
                nodes[idx].left = -1;
                nodes[idx].right = -1;
                fold_count = fold_count + 1;
                return idx;
            }
            if (lv == 1) {
                return nodes[idx].right;
            }
        }
    }

    if (n.kind == ND_ADD) {
        if (is_const(nodes[idx].right) && nodes[nodes[idx].right].value == 0)
            return nodes[idx].left;
        if (is_const(nodes[idx].left) && nodes[nodes[idx].left].value == 0)
            return nodes[idx].right;
    }

    if (n.kind == ND_SUB) {
        if (is_const(nodes[idx].right) && nodes[nodes[idx].right].value == 0)
            return nodes[idx].left;
    }

    return idx;
}

char *kind_str(int k) {
    if (k == ND_NUM) return "NUM";
    if (k == ND_ADD) return "ADD";
    if (k == ND_SUB) return "SUB";
    if (k == ND_MUL) return "MUL";
    if (k == ND_DIV) return "DIV";
    if (k == ND_NEG) return "NEG";
    if (k == ND_VAR) return "VAR";
    return "?";
}

int main() {
    node_count = 0;
    fold_count = 0;

    // Fold: 3 + 4
    int e1 = mk_bin(ND_ADD, mk_num(3), mk_num(4));
    constant_fold(e1);
    printf("3+4 folded: %s val=%d\n", kind_str(nodes[e1].kind), nodes[e1].value);
    // EXPECT: 3+4 folded: NUM val=7

    // Fold: (2 * 5) - 3
    int e2 = mk_bin(ND_SUB, mk_bin(ND_MUL, mk_num(2), mk_num(5)), mk_num(3));
    constant_fold(e2);
    printf("2*5-3 folded: %s val=%d\n", kind_str(nodes[e2].kind), nodes[e2].value);
    // EXPECT: 2*5-3 folded: NUM val=7

    // Partial fold: x + (3 * 4)
    int e3 = mk_bin(ND_ADD, mk_var(0), mk_bin(ND_MUL, mk_num(3), mk_num(4)));
    constant_fold(e3);
    printf("x+(3*4) kind: %s\n", kind_str(nodes[e3].kind));
    // EXPECT: x+(3*4) kind: ADD
    int rchild = nodes[e3].right;
    printf("right folded: %s val=%d\n", kind_str(nodes[rchild].kind), nodes[rchild].value);
    // EXPECT: right folded: NUM val=12

    // Fold negation: -(5)
    int e4 = mk_neg(mk_num(5));
    constant_fold(e4);
    printf("-(5) folded: %s val=%d\n", kind_str(nodes[e4].kind), nodes[e4].value);
    // EXPECT: -(5) folded: NUM val=-5

    // Fold: 100 / (2 * 5)
    int e5 = mk_bin(ND_DIV, mk_num(100), mk_bin(ND_MUL, mk_num(2), mk_num(5)));
    constant_fold(e5);
    printf("100/(2*5) folded: %s val=%d\n", kind_str(nodes[e5].kind), nodes[e5].value);
    // EXPECT: 100/(2*5) folded: NUM val=10

    printf("fold count: %d\n", fold_count);
    // EXPECT: fold count: 7

    // Strength reduction tests
    fold_count = 0;
    int e6 = mk_bin(ND_MUL, mk_var(1), mk_num(0));
    int r6 = strength_reduce(e6);
    printf("x*0: %s val=%d\n", kind_str(nodes[r6].kind), nodes[r6].value);
    // EXPECT: x*0: NUM val=0

    int e7 = mk_bin(ND_MUL, mk_var(1), mk_num(1));
    int r7 = strength_reduce(e7);
    printf("x*1: %s\n", kind_str(nodes[r7].kind));
    // EXPECT: x*1: VAR

    int e8 = mk_bin(ND_ADD, mk_var(2), mk_num(0));
    int r8 = strength_reduce(e8);
    printf("x+0: %s\n", kind_str(nodes[r8].kind));
    // EXPECT: x+0: VAR

    int e9 = mk_bin(ND_SUB, mk_var(3), mk_num(0));
    int r9 = strength_reduce(e9);
    printf("x-0: %s\n", kind_str(nodes[r9].kind));
    // EXPECT: x-0: VAR

    printf("const fold done\n");
    // EXPECT: const fold done

    return 0;
}
