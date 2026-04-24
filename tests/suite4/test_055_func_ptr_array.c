int printf(const char *fmt, ...);

// Arithmetic operations
int op_add(int a, int b) { return a + b; }
int op_sub(int a, int b) { return a - b; }
int op_mul(int a, int b) { return a * b; }
int op_div(int a, int b) { return a / b; }
int op_mod(int a, int b) { return a % b; }

// Unary operations
int u_negate(int x)     { return -x; }
int u_double(int x)     { return x * 2; }
int u_triple(int x)     { return x * 3; }
int u_square(int x)     { return x * x; }
int u_identity(int x)   { return x; }
int u_abs(int x)        { return x < 0 ? -x : x; }
int u_inc(int x)        { return x + 1; }

// Typedef for clarity
typedef int (*BinOp)(int, int);
typedef int (*UnaryOp)(int);

// Apply all operations in an array to a pair of values
void apply_all_bin(BinOp *ops, int count, int a, int b) {
    int i;
    for (i = 0; i < count; i = i + 1) {
        printf("%d\n", ops[i](a, b));
    }
}

// Apply all unary ops and sum the results
int apply_and_sum(UnaryOp *ops, int count, int x) {
    int total = 0;
    int i;
    for (i = 0; i < count; i = i + 1) {
        total = total + ops[i](x);
    }
    return total;
}

// Dispatch table: map an index to an operation
int dispatch_bin(int op_index, int a, int b) {
    BinOp table[5];
    table[0] = op_add;
    table[1] = op_sub;
    table[2] = op_mul;
    table[3] = op_div;
    table[4] = op_mod;
    if (op_index < 0 || op_index > 4) return 0;
    return table[op_index](a, b);
}

// Chain: apply a series of unary operations sequentially
int chain_unary(UnaryOp *ops, int count, int start) {
    int val = start;
    int i;
    for (i = 0; i < count; i = i + 1) {
        val = ops[i](val);
    }
    return val;
}

// Find the operation that gives the max result
int find_max_result(BinOp *ops, int count, int a, int b) {
    int max = ops[0](a, b);
    int i;
    for (i = 1; i < count; i = i + 1) {
        int result = ops[i](a, b);
        if (result > max) max = result;
    }
    return max;
}

// Count how many unary ops produce a positive result
int count_positive(UnaryOp *ops, int count, int x) {
    int c = 0;
    int i;
    for (i = 0; i < count; i = i + 1) {
        if (ops[i](x) > 0) c = c + 1;
    }
    return c;
}

int main(void) {
    // Array of binary function pointers
    BinOp bin_ops[5];
    bin_ops[0] = op_add;
    bin_ops[1] = op_sub;
    bin_ops[2] = op_mul;
    bin_ops[3] = op_div;
    bin_ops[4] = op_mod;

    // Apply all binary ops to (20, 6)
    // add: 26, sub: 14, mul: 120, div: 3, mod: 2
    // EXPECT: 26
    // EXPECT: 14
    // EXPECT: 120
    // EXPECT: 3
    // EXPECT: 2
    apply_all_bin(bin_ops, 5, 20, 6);

    // Dispatch table
    // EXPECT: 15
    printf("%d\n", dispatch_bin(0, 10, 5));
    // EXPECT: 5
    printf("%d\n", dispatch_bin(1, 10, 5));
    // EXPECT: 50
    printf("%d\n", dispatch_bin(2, 10, 5));
    // EXPECT: 2
    printf("%d\n", dispatch_bin(3, 10, 5));
    // EXPECT: 0
    printf("%d\n", dispatch_bin(4, 10, 5));

    // Array of unary function pointers
    UnaryOp un_ops[4];
    un_ops[0] = u_negate;
    un_ops[1] = u_double;
    un_ops[2] = u_triple;
    un_ops[3] = u_square;

    // Apply all to 5: -5, 10, 15, 25; sum = 45
    // EXPECT: 45
    printf("%d\n", apply_and_sum(un_ops, 4, 5));

    // Chain: start=2, double->triple->square = square(triple(double(2)))
    // double(2)=4, triple(4)=12, square(12)=144
    UnaryOp chain[3];
    chain[0] = u_double;
    chain[1] = u_triple;
    chain[2] = u_square;
    // EXPECT: 144
    printf("%d\n", chain_unary(chain, 3, 2));

    // Chain: start=3, negate->abs->inc = inc(abs(negate(3)))
    // negate(3)=-3, abs(-3)=3, inc(3)=4
    UnaryOp chain2[3];
    chain2[0] = u_negate;
    chain2[1] = u_abs;
    chain2[2] = u_inc;
    // EXPECT: 4
    printf("%d\n", chain_unary(chain2, 3, 3));

    // Find max result: ops on (10, 3)
    // add=13, sub=7, mul=30, div=3, mod=1 => max=30
    // EXPECT: 30
    printf("%d\n", find_max_result(bin_ops, 5, 10, 3));

    // Find max result: ops on (2, 5)
    // add=7, sub=-3, mul=10, div=0, mod=2 => max=10
    // EXPECT: 10
    printf("%d\n", find_max_result(bin_ops, 5, 2, 5));

    // Count positive with x = -3
    // negate(-3)=3>0 yes, double(-3)=-6 no, triple(-3)=-9 no, square(-3)=9>0 yes
    // EXPECT: 2
    printf("%d\n", count_positive(un_ops, 4, -3));

    // Iterate through array and print results individually
    int i;
    // results for un_ops on 4: negate=-4, double=8, triple=12, square=16
    // EXPECT: -4
    // EXPECT: 8
    // EXPECT: 12
    // EXPECT: 16
    for (i = 0; i < 4; i = i + 1) {
        printf("%d\n", un_ops[i](4));
    }

    // Swapping function pointers in array
    BinOp tmp = bin_ops[0];
    bin_ops[0] = bin_ops[2];
    bin_ops[2] = tmp;
    // Now bin_ops[0] = mul, bin_ops[2] = add
    // EXPECT: 50
    printf("%d\n", bin_ops[0](10, 5));
    // EXPECT: 15
    printf("%d\n", bin_ops[2](10, 5));

    return 0;
}
