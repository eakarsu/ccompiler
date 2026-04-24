int printf(const char *fmt, ...);
// EXPECT: add(10,3) = 13\nsub(10,3) = 7\nmul(10,3) = 30\ndiv(10,3) = 3\nmod(10,3) = 1\nops: 27 13 140 2 6 \nfold add = 15\nfold mul = 120\nfold max = 5\nfold min = 1\nall ops(15,4): 19 11 60 3 3 \nchained = 17\nchosen op = 5\ndoubled: 2 4 6 8 10 
// Test: function pointers with arithmetic

typedef int (*binop_t)(int, int);

int add_fn(int a, int b) { return a + b; }
int sub_fn(int a, int b) { return a - b; }
int mul_fn(int a, int b) { return a * b; }
int div_fn(int a, int b) { return b != 0 ? a / b : 0; }
int mod_fn(int a, int b) { return b != 0 ? a % b : 0; }
int max_fn(int a, int b) { return a > b ? a : b; }
int min_fn(int a, int b) { return a < b ? a : b; }

int apply(binop_t f, int a, int b) {
    return f(a, b);
}

int fold(binop_t f, int arr[], int n) {
    int result, i;
    result = arr[0];
    for (i = 1; i < n; i++) {
        result = f(result, arr[i]);
    }
    return result;
}

int main(void) {
    binop_t ops[5];
    int arr[5];
    int i;
    int r;

    /* Direct function pointer calls */
    printf("add(10,3) = %d\n", apply(add_fn, 10, 3));
    printf("sub(10,3) = %d\n", apply(sub_fn, 10, 3));
    printf("mul(10,3) = %d\n", apply(mul_fn, 10, 3));
    printf("div(10,3) = %d\n", apply(div_fn, 10, 3));
    printf("mod(10,3) = %d\n", apply(mod_fn, 10, 3));

    /* Function pointer array */
    ops[0] = add_fn;
    ops[1] = sub_fn;
    ops[2] = mul_fn;
    ops[3] = div_fn;
    ops[4] = mod_fn;

    printf("ops: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", ops[i](20, 7));
    }
    printf("\n");

    /* Fold operations */
    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;

    printf("fold add = %d\n", fold(add_fn, arr, 5));
    printf("fold mul = %d\n", fold(mul_fn, arr, 5));
    printf("fold max = %d\n", fold(max_fn, arr, 5));
    printf("fold min = %d\n", fold(min_fn, arr, 5));

    /* Apply different ops to same data */
    {
        int a, b;
        a = 15;
        b = 4;
        printf("all ops(%d,%d): ", a, b);
        for (i = 0; i < 5; i++) {
            printf("%d ", ops[i](a, b));
        }
        printf("\n");
    }

    /* Chained function pointer application */
    r = apply(add_fn, apply(mul_fn, 3, 4), apply(sub_fn, 10, 5));
    printf("chained = %d\n", r);

    /* Select operation based on condition */
    {
        binop_t chosen;
        int x, y;
        x = 8;
        y = 3;
        chosen = (x > y) ? sub_fn : add_fn;
        printf("chosen op = %d\n", chosen(x, y));
    }

    /* Map-like: apply operation to each element */
    printf("doubled: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", apply(mul_fn, arr[i], 2));
    }
    printf("\n");

    return 0;
}
