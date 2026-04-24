int printf(const char *fmt, ...);
// EXPECT: Calculator tests (10, 3):\nadd(10,3)=13\nsub(10,3)=7\nmul(10,3)=30\ndiv(10,3)=3\nmod(10,3)=1\nmax(10,3)=10\nmin(10,3)=3\npow(10,3)=1000\nDivision by zero:\ndiv(5,0)=-9999\nmod(5,0)=-9999\nPower tests:\npow(2,0)=1\npow(2,10)=1024\npow(3,5)=243\npow(5,3)=125\nChain calculation:\nstep 1: 2\nstep 2: 6\nstep 3: 21\nstep 4: 88\nstep 5: 445\nUnknown op: 0
int calc(int a, int op, int b) {
    int result;
    switch (op) {
        case 0: result = a + b; break;
        case 1: result = a - b; break;
        case 2: result = a * b; break;
        case 3:
            if (b == 0) result = -9999;
            else result = a / b;
            break;
        case 4:
            if (b == 0) result = -9999;
            else result = a % b;
            break;
        case 5:
            if (a > b) result = a;
            else result = b;
            break;
        case 6:
            if (a < b) result = a;
            else result = b;
            break;
        case 7: {
            int base = 1;
            int exp = b;
            int val = a;
            while (exp > 0) {
                if (exp % 2 == 1) base = base * val;
                val = val * val;
                exp = exp / 2;
            }
            result = base;
            break;
        }
        default:
            result = 0;
            break;
    }
    return result;
}

int main(void) {
    int ops[8];
    ops[0] = 0; ops[1] = 1; ops[2] = 2; ops[3] = 3;
    ops[4] = 4; ops[5] = 5; ops[6] = 6; ops[7] = 7;

    char *names[8];
    names[0] = "add";
    names[1] = "sub";
    names[2] = "mul";
    names[3] = "div";
    names[4] = "mod";
    names[5] = "max";
    names[6] = "min";
    names[7] = "pow";

    int i;
    printf("Calculator tests (10, 3):\n");
    for (i = 0; i < 8; i++) {
        printf("%s(10,3)=%d\n", names[i], calc(10, ops[i], 3));
    }

    printf("Division by zero:\n");
    printf("div(5,0)=%d\n", calc(5, 3, 0));
    printf("mod(5,0)=%d\n", calc(5, 4, 0));

    printf("Power tests:\n");
    printf("pow(2,0)=%d\n", calc(2, 7, 0));
    printf("pow(2,10)=%d\n", calc(2, 7, 10));
    printf("pow(3,5)=%d\n", calc(3, 7, 5));
    printf("pow(5,3)=%d\n", calc(5, 7, 3));

    printf("Chain calculation:\n");
    int result = 1;
    for (i = 1; i <= 5; i++) {
        result = calc(result, 2, i);
        result = calc(result, 0, i);
        printf("step %d: %d\n", i, result);
    }

    printf("Unknown op: %d\n", calc(1, 99, 2));

    return 0;
}
