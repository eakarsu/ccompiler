int printf(const char *fmt, ...);
// EXPECT: apply add(3,4)=7\napply sub(10,3)=7\napply mul(5,6)=30\napply div(20,4)=5\ntwice add(5,3)=11\ntwice mul(2,3)=18\nsum=24\nproduct=756\nmax=9\nmin=1\nadded 5: 15 25 35 45\nmul by 2: 20 40 60 80\nvia typedef=300\nvia typedef=20000
// Test 207: Function pointers basics

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int div_fn(int a, int b) { if (b == 0) return 0; return a / b; }

int apply(int (*fn)(int, int), int a, int b) {
    return fn(a, b);
}

int apply_twice(int (*fn)(int, int), int a, int b) {
    int first = fn(a, b);
    return fn(first, b);
}

typedef int (*binop)(int, int);

int fold_array(int *arr, int n, binop fn, int init) {
    int result = init;
    int i = 0;
    while (i < n) {
        result = fn(result, arr[i]);
        i = i + 1;
    }
    return result;
}

int max_fn(int a, int b) { if (a > b) return a; return b; }
int min_fn(int a, int b) { if (a < b) return a; return b; }

void apply_to_array(int *arr, int n, int (*fn)(int, int), int operand) {
    int i = 0;
    while (i < n) {
        arr[i] = fn(arr[i], operand);
        i = i + 1;
    }
}

int identity(int a, int b) { return a; }

int main(void) {
    printf("apply add(3,4)=%d\n", apply(add, 3, 4));
    printf("apply sub(10,3)=%d\n", apply(sub, 10, 3));
    printf("apply mul(5,6)=%d\n", apply(mul, 5, 6));
    printf("apply div(20,4)=%d\n", apply(div_fn, 20, 4));

    printf("twice add(5,3)=%d\n", apply_twice(add, 5, 3));
    printf("twice mul(2,3)=%d\n", apply_twice(mul, 2, 3));

    int arr[5];
    arr[0] = 3; arr[1] = 7; arr[2] = 1; arr[3] = 9; arr[4] = 4;

    printf("sum=%d\n", fold_array(arr, 5, add, 0));
    printf("product=%d\n", fold_array(arr, 5, mul, 1));
    printf("max=%d\n", fold_array(arr, 5, max_fn, arr[0]));
    printf("min=%d\n", fold_array(arr, 5, min_fn, arr[0]));

    int arr2[4];
    arr2[0] = 10; arr2[1] = 20; arr2[2] = 30; arr2[3] = 40;
    apply_to_array(arr2, 4, add, 5);
    printf("added 5:");
    int i = 0;
    while (i < 4) { printf(" %d", arr2[i]); i = i + 1; }
    printf("\n");

    arr2[0] = 10; arr2[1] = 20; arr2[2] = 30; arr2[3] = 40;
    apply_to_array(arr2, 4, mul, 2);
    printf("mul by 2:");
    i = 0;
    while (i < 4) { printf(" %d", arr2[i]); i = i + 1; }
    printf("\n");

    binop op = add;
    printf("via typedef=%d\n", op(100, 200));
    op = mul;
    printf("via typedef=%d\n", op(100, 200));

    return 0;
}
