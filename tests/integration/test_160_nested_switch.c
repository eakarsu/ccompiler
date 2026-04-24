int printf(const char *fmt, ...);
// EXPECT: Compute operations:\nabs_diff(10,3)=7\nabs_diff(3,10)=7\nmul(4,5)=20\ndiv(20,4)=5\ndiv(5,0)=-1\nmod(17,5)=2\nmod(10,0)=-1\nunknown(1,2)=0\nKeypad:\n1 2 3 \n4 5 6 \n7 8 9 \nout_of_bounds=-1\nout_of_bounds=0\nNested switch accumulator:\ntotal=34
int compute(int op, int a, int b) {
    int result = 0;
    switch (op) {
        case 1:
            switch (a > b) {
                case 1: result = a - b; break;
                case 0: result = b - a; break;
            }
            break;
        case 2:
            result = a * b;
            break;
        case 3:
            switch (b) {
                case 0: result = -1; break;
                default: result = a / b; break;
            }
            break;
        case 4:
            switch (b) {
                case 0: result = -1; break;
                default: result = a % b; break;
            }
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

int encode(int row, int col) {
    int val = 0;
    switch (row) {
        case 0:
            switch (col) {
                case 0: val = 1; break;
                case 1: val = 2; break;
                case 2: val = 3; break;
                default: val = 0; break;
            }
            break;
        case 1:
            switch (col) {
                case 0: val = 4; break;
                case 1: val = 5; break;
                case 2: val = 6; break;
                default: val = 0; break;
            }
            break;
        case 2:
            switch (col) {
                case 0: val = 7; break;
                case 1: val = 8; break;
                case 2: val = 9; break;
                default: val = 0; break;
            }
            break;
        default:
            val = -1;
            break;
    }
    return val;
}

int main(void) {
    int i;
    int j;
    printf("Compute operations:\n");
    printf("abs_diff(10,3)=%d\n", compute(1, 10, 3));
    printf("abs_diff(3,10)=%d\n", compute(1, 3, 10));
    printf("mul(4,5)=%d\n", compute(2, 4, 5));
    printf("div(20,4)=%d\n", compute(3, 20, 4));
    printf("div(5,0)=%d\n", compute(3, 5, 0));
    printf("mod(17,5)=%d\n", compute(4, 17, 5));
    printf("mod(10,0)=%d\n", compute(4, 10, 0));
    printf("unknown(1,2)=%d\n", compute(5, 1, 2));

    printf("Keypad:\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            printf("%d ", encode(i, j));
        }
        printf("\n");
    }
    printf("out_of_bounds=%d\n", encode(3, 0));
    printf("out_of_bounds=%d\n", encode(0, 5));

    printf("Nested switch accumulator:\n");
    int total = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            total = total + compute(i + 1, j + 1, i + 1);
        }
    }
    printf("total=%d\n", total);

    return 0;
}
