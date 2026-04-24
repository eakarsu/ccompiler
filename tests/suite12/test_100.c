int printf(const char *fmt, ...);

// Ultimate integration test: all features combined

enum Op { OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD, OP_NEG, OP_NOP };

struct Value {
    int data;
    int valid;
};

struct Instruction {
    enum Op op;
    int arg1;
    int arg2;
};

struct Program {
    struct Instruction code[16];
    int length;
};

typedef int (*BinaryOp)(int, int);

int op_add(int a, int b) { return a + b; }
int op_sub(int a, int b) { return a - b; }
int op_mul(int a, int b) { return a * b; }
int op_div(int a, int b) { return a / b; }
int op_mod(int a, int b) { return a % b; }

int execute_op(enum Op op, int a, int b) {
    switch (op) {
        case OP_ADD: return a + b;
        case OP_SUB: return a - b;
        case OP_MUL: return a * b;
        case OP_DIV: return a / b;
        case OP_MOD: return a % b;
        case OP_NEG: return -a;
        case OP_NOP: return a;
        default: return 0;
    }
}

int run_program(struct Program *prog, int init) {
    int acc;
    int i;
    acc = init;
    for (i = 0; i < prog->length; i = i + 1) {
        struct Instruction *inst;
        inst = &prog->code[i];
        if (inst->op == OP_NEG) {
            acc = -acc;
        } else if (inst->op == OP_NOP) {
            /* do nothing */
        } else {
            acc = execute_op(inst->op, acc, inst->arg2);
        }
    }
    return acc;
}

// Linked list via array
struct ListNode {
    int value;
    int next;  // index, -1 = end
};

int list_sum(struct ListNode *nodes, int head) {
    int sum;
    int cur;
    sum = 0;
    cur = head;
    while (cur >= 0) {
        sum = sum + nodes[cur].value;
        cur = nodes[cur].next;
    }
    return sum;
}

int list_length(struct ListNode *nodes, int head) {
    int count;
    int cur;
    count = 0;
    cur = head;
    while (cur >= 0) {
        count = count + 1;
        cur = nodes[cur].next;
    }
    return count;
}

// Binary search
int binary_search(int *arr, int len, int target) {
    int lo;
    int hi;
    int mid;
    lo = 0;
    hi = len - 1;
    while (lo <= hi) {
        mid = (lo + hi) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

// Recursive GCD
int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

// Bubble sort
void sort(int *arr, int n) {
    int i;
    int j;
    int tmp;
    for (i = 0; i < n - 1; i = i + 1) {
        for (j = 0; j < n - 1 - i; j = j + 1) {
            if (arr[j] > arr[j + 1]) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

// Function pointer dispatch table
int dispatch(BinaryOp *table, int idx, int a, int b) {
    return table[idx](a, b);
}

// Struct with nested arrays
struct Matrix2x2 {
    int m[2][2];
};

void mat_mul(struct Matrix2x2 *a, struct Matrix2x2 *b, struct Matrix2x2 *out) {
    out->m[0][0] = a->m[0][0]*b->m[0][0] + a->m[0][1]*b->m[1][0];
    out->m[0][1] = a->m[0][0]*b->m[0][1] + a->m[0][1]*b->m[1][1];
    out->m[1][0] = a->m[1][0]*b->m[0][0] + a->m[1][1]*b->m[1][0];
    out->m[1][1] = a->m[1][0]*b->m[0][1] + a->m[1][1]*b->m[1][1];
}

int mat_trace(struct Matrix2x2 *m) {
    return m->m[0][0] + m->m[1][1];
}

int main() {
    // === Program execution ===
    struct Program prog;
    prog.code[0].op = OP_ADD; prog.code[0].arg2 = 10;
    prog.code[1].op = OP_MUL; prog.code[1].arg2 = 3;
    prog.code[2].op = OP_SUB; prog.code[2].arg2 = 5;
    prog.code[3].op = OP_NEG; prog.code[3].arg2 = 0;
    prog.code[4].op = OP_ADD; prog.code[4].arg2 = 100;
    prog.length = 5;

    // init=5: +10=15, *3=45, -5=40, neg=-40, +100=60
    printf("%d\n", run_program(&prog, 5)); // EXPECT: 60

    // === Switch dispatch ===
    printf("%d\n", execute_op(OP_ADD, 10, 20)); // EXPECT: 30
    printf("%d\n", execute_op(OP_MUL, 7, 8));   // EXPECT: 56
    printf("%d\n", execute_op(OP_MOD, 17, 5));   // EXPECT: 2
    printf("%d\n", execute_op(OP_NEG, 42, 0));   // EXPECT: -42

    // === Linked list ===
    struct ListNode nodes[6];
    nodes[0].value = 10; nodes[0].next = 1;
    nodes[1].value = 20; nodes[1].next = 2;
    nodes[2].value = 30; nodes[2].next = 3;
    nodes[3].value = 40; nodes[3].next = 4;
    nodes[4].value = 50; nodes[4].next = -1;
    nodes[5].value = 99; nodes[5].next = -1;

    printf("%d\n", list_sum(nodes, 0));    // EXPECT: 150
    printf("%d\n", list_length(nodes, 0)); // EXPECT: 5
    printf("%d\n", list_sum(nodes, 2));    // EXPECT: 120
    printf("%d\n", list_length(nodes, 2)); // EXPECT: 3

    // === Binary search ===
    int sorted[10];
    int idx;
    sorted[0] = 2; sorted[1] = 5; sorted[2] = 8; sorted[3] = 12; sorted[4] = 15;
    sorted[5] = 19; sorted[6] = 23; sorted[7] = 30; sorted[8] = 45; sorted[9] = 67;

    printf("%d\n", binary_search(sorted, 10, 8));  // EXPECT: 2
    printf("%d\n", binary_search(sorted, 10, 67)); // EXPECT: 9
    printf("%d\n", binary_search(sorted, 10, 2));  // EXPECT: 0
    printf("%d\n", binary_search(sorted, 10, 99)); // EXPECT: -1

    // === GCD ===
    printf("%d\n", gcd(48, 18)); // EXPECT: 6
    printf("%d\n", gcd(100, 75)); // EXPECT: 25
    printf("%d\n", gcd(17, 13)); // EXPECT: 1

    // === Sort ===
    int data[8];
    data[0] = 38; data[1] = 27; data[2] = 43; data[3] = 3;
    data[4] = 9;  data[5] = 82; data[6] = 10; data[7] = 1;
    sort(data, 8);
    printf("%d\n", data[0]); // EXPECT: 1
    printf("%d\n", data[1]); // EXPECT: 3
    printf("%d\n", data[6]); // EXPECT: 43
    printf("%d\n", data[7]); // EXPECT: 82

    // === Function pointer table ===
    BinaryOp ops[5];
    ops[0] = op_add;
    ops[1] = op_sub;
    ops[2] = op_mul;
    ops[3] = op_div;
    ops[4] = op_mod;

    printf("%d\n", dispatch(ops, 0, 10, 5)); // EXPECT: 15
    printf("%d\n", dispatch(ops, 1, 10, 5)); // EXPECT: 5
    printf("%d\n", dispatch(ops, 2, 10, 5)); // EXPECT: 50
    printf("%d\n", dispatch(ops, 3, 10, 5)); // EXPECT: 2
    printf("%d\n", dispatch(ops, 4, 10, 5)); // EXPECT: 0

    // === Matrix multiplication ===
    struct Matrix2x2 a;
    struct Matrix2x2 b;
    struct Matrix2x2 c;
    a.m[0][0] = 1; a.m[0][1] = 2;
    a.m[1][0] = 3; a.m[1][1] = 4;
    b.m[0][0] = 5; b.m[0][1] = 6;
    b.m[1][0] = 7; b.m[1][1] = 8;
    mat_mul(&a, &b, &c);

    // c = [[1*5+2*7, 1*6+2*8], [3*5+4*7, 3*6+4*8]] = [[19,22],[43,50]]
    printf("%d\n", c.m[0][0]); // EXPECT: 19
    printf("%d\n", c.m[0][1]); // EXPECT: 22
    printf("%d\n", c.m[1][0]); // EXPECT: 43
    printf("%d\n", c.m[1][1]); // EXPECT: 50
    printf("%d\n", mat_trace(&c)); // EXPECT: 69

    // === Enum arithmetic ===
    printf("%d\n", OP_ADD); // EXPECT: 0
    printf("%d\n", OP_NOP); // EXPECT: 6

    printf("%s\n", "all done"); // EXPECT: all done
    return 0;
}
