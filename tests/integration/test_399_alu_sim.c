int printf(const char *fmt, ...);
// EXPECT: ALU ADD:\n  10+20: result=30 zero=0 neg=0 carry=0\n  0+0: result=0 zero=1 neg=0 carry=0\n  -5+5: result=0 zero=1 neg=0 carry=1\n  200+100: result=300 zero=0 neg=0 carry=1\nALU SUB:\n  20-10: result=10 zero=0 neg=0 carry=0\n  10-10: result=0 zero=1 neg=0 carry=0\n  5-10: result=-5 zero=0 neg=1 carry=0\nALU AND:\n  FF&0F: result=15 zero=0 neg=0 carry=0\n  00&FF: result=0 zero=1 neg=0 carry=0\nALU OR:\n  F0|0F: result=255 zero=0 neg=0 carry=0\nALU XOR:\n  FF^FF: result=0 zero=1 neg=0 carry=0\n  AA^55: result=255 zero=0 neg=0 carry=0\nALU SHIFTS:\n  1<<4: result=16 zero=0 neg=0 carry=0\n  256>>4: result=16 zero=0 neg=0 carry=0\nALU NOT:\n  ~0: result=-1 zero=0 neg=1 carry=0\nALU NEG:\n  -42: result=-42 zero=0 neg=1 carry=0\n  -0: result=0 zero=1 neg=0 carry=0\n(15+25)*2 = 80
// Test: ALU simulation

enum {
    ALU_ADD = 0,
    ALU_SUB = 1,
    ALU_AND = 2,
    ALU_OR  = 3,
    ALU_XOR = 4,
    ALU_NOT = 5,
    ALU_SHL = 6,
    ALU_SHR = 7,
    ALU_CMP = 8,
    ALU_NEG = 9
};

typedef struct {
    int result;
    int zero;
    int negative;
    int carry;
} ALUResult;

ALUResult alu_execute(int op, int a, int b) {
    ALUResult r;
    r.carry = 0;

    if (op == ALU_ADD) {
        r.result = a + b;
        // Detect unsigned carry for 8-bit
        if (((a & 0xFF) + (b & 0xFF)) > 0xFF) r.carry = 1;
    } else if (op == ALU_SUB) {
        r.result = a - b;
    } else if (op == ALU_AND) {
        r.result = a & b;
    } else if (op == ALU_OR) {
        r.result = a | b;
    } else if (op == ALU_XOR) {
        r.result = a ^ b;
    } else if (op == ALU_NOT) {
        r.result = ~a;
    } else if (op == ALU_SHL) {
        r.result = a << b;
    } else if (op == ALU_SHR) {
        r.result = a >> b;
    } else if (op == ALU_CMP) {
        r.result = a - b;
    } else if (op == ALU_NEG) {
        r.result = -a;
    } else {
        r.result = 0;
    }

    r.zero = (r.result == 0);
    r.negative = (r.result < 0);
    return r;
}

void print_result(char *name, ALUResult r) {
    printf("  %s: result=%d zero=%d neg=%d carry=%d\n",
           name, r.result, r.zero, r.negative, r.carry);
}

int main(void) {
    ALUResult r;

    // Addition
    printf("ALU ADD:\n");
    r = alu_execute(ALU_ADD, 10, 20);
    print_result("10+20", r);
    r = alu_execute(ALU_ADD, 0, 0);
    print_result("0+0", r);
    r = alu_execute(ALU_ADD, -5, 5);
    print_result("-5+5", r);
    r = alu_execute(ALU_ADD, 200, 100);
    print_result("200+100", r);

    // Subtraction
    printf("ALU SUB:\n");
    r = alu_execute(ALU_SUB, 20, 10);
    print_result("20-10", r);
    r = alu_execute(ALU_SUB, 10, 10);
    print_result("10-10", r);
    r = alu_execute(ALU_SUB, 5, 10);
    print_result("5-10", r);

    // Bitwise AND
    printf("ALU AND:\n");
    r = alu_execute(ALU_AND, 0xFF, 0x0F);
    print_result("FF&0F", r);
    r = alu_execute(ALU_AND, 0x00, 0xFF);
    print_result("00&FF", r);

    // Bitwise OR
    printf("ALU OR:\n");
    r = alu_execute(ALU_OR, 0xF0, 0x0F);
    print_result("F0|0F", r);

    // XOR
    printf("ALU XOR:\n");
    r = alu_execute(ALU_XOR, 0xFF, 0xFF);
    print_result("FF^FF", r);
    r = alu_execute(ALU_XOR, 0xAA, 0x55);
    print_result("AA^55", r);

    // Shifts
    printf("ALU SHIFTS:\n");
    r = alu_execute(ALU_SHL, 1, 4);
    print_result("1<<4", r);
    r = alu_execute(ALU_SHR, 256, 4);
    print_result("256>>4", r);

    // NOT
    printf("ALU NOT:\n");
    r = alu_execute(ALU_NOT, 0, 0);
    print_result("~0", r);

    // NEG
    printf("ALU NEG:\n");
    r = alu_execute(ALU_NEG, 42, 0);
    print_result("-42", r);
    r = alu_execute(ALU_NEG, 0, 0);
    print_result("-0", r);

    // Chain: compute (a + b) * 2 using ALU ops
    int a = 15;
    int b = 25;
    r = alu_execute(ALU_ADD, a, b);
    r = alu_execute(ALU_SHL, r.result, 1);
    printf("(15+25)*2 = %d\n", r.result);

    return 0;
}
