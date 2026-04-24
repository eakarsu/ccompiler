int printf(const char *fmt, ...);
// EXPECT: P1=7 P2=2 P3=24 P4=3 P5=1 P6=8 P7=14 P8=14 P9=26 P10=7 P11=8 P12=5 I2P1=3,4,+ I2P1R=7 I2P2=3,4,2,*,+ I2P2R=11 I2P3=3,4,+,2,* I2P3R=14 I2P4=2,3,*,4,+ I2P4R=10 I2P5R=4 I2P6R=24 I2P7R=9 I2P8R=15 OPD1=3 OPR1=2 LEN1=5 VAL1=1 SO1=9 MO1=4 SD1=2 OPD2=5 OPR2=4 LEN2=9 VAL2=1 SD2=3 VAL3=0 VAL4=1 EI1=11 EI2=14 EI3=17 EI4=21 EI5=90 EEQ1=1 EEQ2=0 MOD1=2 MOD2=6 POW1=1024 POW2=81 
/* ====================================================================== */
/* Test 050: Expression Evaluation                                        */
/* Postfix (RPN) evaluation, infix to postfix conversion simulation       */
/* Expressions represented as integer arrays with operator codes          */
/* ====================================================================== */

/* Token types:
   Positive numbers = operands (literal values)
   -1 = + (addition)
   -2 = - (subtraction)
   -3 = * (multiplication)
   -4 = / (division)
   -5 = % (modulo)
   -6 = ( (left paren - for infix)
   -7 = ) (right paren - for infix)
   -8 = ^ (power)
   -9 = unary minus (negate)
   -99 = end of expression
*/

int is_operator(int token) {
    if (token >= -9 && token <= -1) return 1;
    return 0;
}

int is_operand(int token) {
    return (token >= 0) ? 1 : 0;
}

/* Get operator precedence */
int precedence(int op) {
    if (op == -1 || op == -2) return 1;  /* + - */
    if (op == -3 || op == -4 || op == -5) return 2;  /* * / % */
    if (op == -8) return 3;  /* ^ */
    if (op == -9) return 4;  /* unary minus */
    return 0;
}

/* Power function */
int power(int base, int exp) {
    int result;
    int i;
    result = 1;
    i = 0;
    while (i < exp) {
        result = result * base;
        i = i + 1;
    }
    return result;
}

/* Apply binary operator */
int apply_op(int a, int b, int op) {
    if (op == -1) return a + b;
    if (op == -2) return a - b;
    if (op == -3) return a * b;
    if (op == -4) {
        if (b == 0) return 0; /* avoid division by zero */
        return a / b;
    }
    if (op == -5) {
        if (b == 0) return 0;
        return a % b;
    }
    if (op == -8) return power(a, b);
    return 0;
}

/* Evaluate postfix (RPN) expression */
/* expr: array of tokens terminated by -99 */
/* Returns result */
int eval_postfix(int *expr) {
    int stack[50];
    int top;
    int i;
    int a;
    int b;
    int token;

    top = -1;
    i = 0;

    while (expr[i] != -99) {
        token = expr[i];
        if (is_operand(token)) {
            top = top + 1;
            stack[top] = token;
        } else if (token == -9) {
            /* Unary minus */
            a = stack[top];
            stack[top] = -a;
        } else if (is_operator(token)) {
            b = stack[top]; top = top - 1;
            a = stack[top]; top = top - 1;
            top = top + 1;
            stack[top] = apply_op(a, b, token);
        }
        i = i + 1;
    }

    if (top >= 0) return stack[top];
    return 0;
}

/* Infix to postfix conversion */
/* infix: array of tokens terminated by -99 */
/* postfix: output array (terminated by -99) */
/* Returns length of postfix (not counting -99) */
int infix_to_postfix(int *infix, int *postfix) {
    int op_stack[50];
    int op_top;
    int out_idx;
    int i;
    int token;

    op_top = -1;
    out_idx = 0;
    i = 0;

    while (infix[i] != -99) {
        token = infix[i];

        if (is_operand(token)) {
            postfix[out_idx] = token;
            out_idx = out_idx + 1;
        } else if (token == -6) {
            /* Left paren: push */
            op_top = op_top + 1;
            op_stack[op_top] = token;
        } else if (token == -7) {
            /* Right paren: pop until left paren */
            while (op_top >= 0 && op_stack[op_top] != -6) {
                postfix[out_idx] = op_stack[op_top];
                out_idx = out_idx + 1;
                op_top = op_top - 1;
            }
            if (op_top >= 0) op_top = op_top - 1; /* pop left paren */
        } else if (is_operator(token)) {
            while (op_top >= 0 && op_stack[op_top] != -6 &&
                   precedence(op_stack[op_top]) >= precedence(token)) {
                postfix[out_idx] = op_stack[op_top];
                out_idx = out_idx + 1;
                op_top = op_top - 1;
            }
            op_top = op_top + 1;
            op_stack[op_top] = token;
        }
        i = i + 1;
    }

    /* Pop remaining operators */
    while (op_top >= 0) {
        postfix[out_idx] = op_stack[op_top];
        out_idx = out_idx + 1;
        op_top = op_top - 1;
    }

    postfix[out_idx] = -99;
    return out_idx;
}

/* Print expression tokens */
void print_expr(int *expr) {
    int i;
    i = 0;
    while (expr[i] != -99) {
        if (expr[i] >= 0) {
            printf("%d", expr[i]);
        } else if (expr[i] == -1) {
            printf("+");
        } else if (expr[i] == -2) {
            printf("-");
        } else if (expr[i] == -3) {
            printf("*");
        } else if (expr[i] == -4) {
            printf("/");
        } else if (expr[i] == -5) {
            printf("%%");
        } else if (expr[i] == -6) {
            printf("(");
        } else if (expr[i] == -7) {
            printf(")");
        } else if (expr[i] == -8) {
            printf("^");
        } else if (expr[i] == -9) {
            printf("~");
        }
        if (expr[i + 1] != -99) printf(",");
        i = i + 1;
    }
}

/* Count operands in expression */
int count_operands(int *expr) {
    int count;
    int i;
    count = 0;
    i = 0;
    while (expr[i] != -99) {
        if (is_operand(expr[i])) count = count + 1;
        i = i + 1;
    }
    return count;
}

/* Count operators in expression */
int count_operators(int *expr) {
    int count;
    int i;
    count = 0;
    i = 0;
    while (expr[i] != -99) {
        if (is_operator(expr[i])) count = count + 1;
        i = i + 1;
    }
    return count;
}

/* Expression length (number of tokens) */
int expr_length(int *expr) {
    int i;
    i = 0;
    while (expr[i] != -99) {
        i = i + 1;
    }
    return i;
}

/* Validate postfix expression:
   Walk through: operand pushes 1, binary op pops 2 pushes 1
   At end, stack should have exactly 1 */
int validate_postfix(int *expr) {
    int stack_size;
    int i;
    int token;

    stack_size = 0;
    i = 0;
    while (expr[i] != -99) {
        token = expr[i];
        if (is_operand(token)) {
            stack_size = stack_size + 1;
        } else if (token == -9) {
            if (stack_size < 1) return 0;
            /* unary: no change in stack size */
        } else if (is_operator(token)) {
            if (stack_size < 2) return 0;
            stack_size = stack_size - 1;
        }
        i = i + 1;
    }
    return (stack_size == 1) ? 1 : 0;
}

/* Build a simple calculator: evaluate infix expression */
int eval_infix(int *infix) {
    int postfix[100];
    infix_to_postfix(infix, postfix);
    return eval_postfix(postfix);
}

/* Check if two expressions evaluate to same result */
int expr_equal(int *a, int *b) {
    return (eval_postfix(a) == eval_postfix(b)) ? 1 : 0;
}

/* Sum of all operand values in expression */
int sum_operands(int *expr) {
    int sum;
    int i;
    sum = 0;
    i = 0;
    while (expr[i] != -99) {
        if (is_operand(expr[i])) sum = sum + expr[i];
        i = i + 1;
    }
    return sum;
}

/* Max operand value */
int max_operand(int *expr) {
    int maxv;
    int i;
    int first;

    maxv = 0;
    first = 1;
    i = 0;
    while (expr[i] != -99) {
        if (is_operand(expr[i])) {
            if (first || expr[i] > maxv) {
                maxv = expr[i];
                first = 0;
            }
        }
        i = i + 1;
    }
    return maxv;
}

/* Compute the max stack depth needed for postfix evaluation */
int max_stack_depth(int *expr) {
    int depth;
    int max_depth;
    int i;
    int token;

    depth = 0;
    max_depth = 0;
    i = 0;
    while (expr[i] != -99) {
        token = expr[i];
        if (is_operand(token)) {
            depth = depth + 1;
        } else if (token == -9) {
            /* unary: no change */
        } else if (is_operator(token)) {
            depth = depth - 1;
        }
        if (depth > max_depth) max_depth = depth;
        i = i + 1;
    }
    return max_depth;
}

int main(void) {
    int expr[50];
    int postfix[100];
    int plen;

    /* ---- Postfix Evaluation ---- */

    /* 3 4 + = 7 */
    expr[0]=3; expr[1]=4; expr[2]=-1; expr[3]=-99;
    printf("P1=%d ", eval_postfix(expr));

    /* 5 3 - = 2 */
    expr[0]=5; expr[1]=3; expr[2]=-2; expr[3]=-99;
    printf("P2=%d ", eval_postfix(expr));

    /* 4 6 * = 24 */
    expr[0]=4; expr[1]=6; expr[2]=-3; expr[3]=-99;
    printf("P3=%d ", eval_postfix(expr));

    /* 10 3 / = 3 */
    expr[0]=10; expr[1]=3; expr[2]=-4; expr[3]=-99;
    printf("P4=%d ", eval_postfix(expr));

    /* 10 3 % = 1 */
    expr[0]=10; expr[1]=3; expr[2]=-5; expr[3]=-99;
    printf("P5=%d ", eval_postfix(expr));

    /* 2 3 ^ = 8 */
    expr[0]=2; expr[1]=3; expr[2]=-8; expr[3]=-99;
    printf("P6=%d ", eval_postfix(expr));

    /* 3 4 + 2 * = 14 : (3+4)*2 */
    expr[0]=3; expr[1]=4; expr[2]=-1; expr[3]=2; expr[4]=-3; expr[5]=-99;
    printf("P7=%d ", eval_postfix(expr));

    /* 5 1 2 + 4 * + 3 - = 5 + 12 - 3 = 14 */
    expr[0]=5; expr[1]=1; expr[2]=2; expr[3]=-1; expr[4]=4; expr[5]=-3; expr[6]=-1; expr[7]=3; expr[8]=-2; expr[9]=-99;
    printf("P8=%d ", eval_postfix(expr));

    /* 2 3 * 4 5 * + = 6 + 20 = 26 */
    expr[0]=2; expr[1]=3; expr[2]=-3; expr[3]=4; expr[4]=5; expr[5]=-3; expr[6]=-1; expr[7]=-99;
    printf("P9=%d ", eval_postfix(expr));

    /* 7 = 7 (single operand) */
    expr[0]=7; expr[1]=-99;
    printf("P10=%d ", eval_postfix(expr));

    /* 10 2 / 3 + = 5 + 3 = 8 */
    expr[0]=10; expr[1]=2; expr[2]=-4; expr[3]=3; expr[4]=-1; expr[5]=-99;
    printf("P11=%d ", eval_postfix(expr));

    /* 15 7 1 1 + - / 3 * 2 1 1 + + - = 15/(7-(1+1)) * 3 - (2+(1+1)) */
    /* = 15/5 * 3 - 4 = 3*3 - 4 = 9 - 4 = 5 */
    expr[0]=15; expr[1]=7; expr[2]=1; expr[3]=1; expr[4]=-1; expr[5]=-2;
    expr[6]=-4; expr[7]=3; expr[8]=-3; expr[9]=2; expr[10]=1; expr[11]=1;
    expr[12]=-1; expr[13]=-1; expr[14]=-2; expr[15]=-99;
    printf("P12=%d ", eval_postfix(expr));

    /* ---- Infix to Postfix Conversion ---- */

    /* 3 + 4 */
    expr[0]=3; expr[1]=-1; expr[2]=4; expr[3]=-99;
    plen = infix_to_postfix(expr, postfix);
    printf("I2P1="); print_expr(postfix); printf(" ");
    printf("I2P1R=%d ", eval_postfix(postfix));

    /* 3 + 4 * 2 => 3 4 2 * + = 11 */
    expr[0]=3; expr[1]=-1; expr[2]=4; expr[3]=-3; expr[4]=2; expr[5]=-99;
    plen = infix_to_postfix(expr, postfix);
    printf("I2P2="); print_expr(postfix); printf(" ");
    printf("I2P2R=%d ", eval_postfix(postfix));

    /* (3 + 4) * 2 => 3 4 + 2 * = 14 */
    expr[0]=-6; expr[1]=3; expr[2]=-1; expr[3]=4; expr[4]=-7; expr[5]=-3; expr[6]=2; expr[7]=-99;
    plen = infix_to_postfix(expr, postfix);
    printf("I2P3="); print_expr(postfix); printf(" ");
    printf("I2P3R=%d ", eval_postfix(postfix));

    /* 2 * 3 + 4 => 2 3 * 4 + = 10 */
    expr[0]=2; expr[1]=-3; expr[2]=3; expr[3]=-1; expr[4]=4; expr[5]=-99;
    plen = infix_to_postfix(expr, postfix);
    printf("I2P4="); print_expr(postfix); printf(" ");
    printf("I2P4R=%d ", eval_postfix(postfix));

    /* 10 - 2 * 3 => 10 2 3 * - = 4 */
    expr[0]=10; expr[1]=-2; expr[2]=2; expr[3]=-3; expr[4]=3; expr[5]=-99;
    plen = infix_to_postfix(expr, postfix);
    printf("I2P5R=%d ", eval_postfix(postfix));

    /* (10 - 2) * 3 => 10 2 - 3 * = 24 */
    expr[0]=-6; expr[1]=10; expr[2]=-2; expr[3]=2; expr[4]=-7; expr[5]=-3; expr[6]=3; expr[7]=-99;
    plen = infix_to_postfix(expr, postfix);
    printf("I2P6R=%d ", eval_postfix(postfix));

    /* 2 + 3 * 4 - 5 => 2 3 4 * + 5 - = 9 */
    expr[0]=2; expr[1]=-1; expr[2]=3; expr[3]=-3; expr[4]=4; expr[5]=-2; expr[6]=5; expr[7]=-99;
    plen = infix_to_postfix(expr, postfix);
    printf("I2P7R=%d ", eval_postfix(postfix));

    /* (2 + 3) * (4 - 1) => 2 3 + 4 1 - * = 15 */
    expr[0]=-6; expr[1]=2; expr[2]=-1; expr[3]=3; expr[4]=-7; expr[5]=-3;
    expr[6]=-6; expr[7]=4; expr[8]=-2; expr[9]=1; expr[10]=-7; expr[11]=-99;
    plen = infix_to_postfix(expr, postfix);
    printf("I2P8R=%d ", eval_postfix(postfix));

    /* ---- Expression Properties ---- */

    /* 3 4 + 2 * */
    expr[0]=3; expr[1]=4; expr[2]=-1; expr[3]=2; expr[4]=-3; expr[5]=-99;
    printf("OPD1=%d ", count_operands(expr));
    printf("OPR1=%d ", count_operators(expr));
    printf("LEN1=%d ", expr_length(expr));
    printf("VAL1=%d ", validate_postfix(expr));
    printf("SO1=%d ", sum_operands(expr));
    printf("MO1=%d ", max_operand(expr));
    printf("SD1=%d ", max_stack_depth(expr));

    /* 5 1 2 + 4 * + 3 - */
    expr[0]=5; expr[1]=1; expr[2]=2; expr[3]=-1; expr[4]=4; expr[5]=-3;
    expr[6]=-1; expr[7]=3; expr[8]=-2; expr[9]=-99;
    printf("OPD2=%d ", count_operands(expr));
    printf("OPR2=%d ", count_operators(expr));
    printf("LEN2=%d ", expr_length(expr));
    printf("VAL2=%d ", validate_postfix(expr));
    printf("SD2=%d ", max_stack_depth(expr));

    /* Invalid postfix: 3 + (only one operand before op) */
    expr[0]=3; expr[1]=-1; expr[2]=-99;
    printf("VAL3=%d ", validate_postfix(expr));

    /* Valid: single operand */
    expr[0]=42; expr[1]=-99;
    printf("VAL4=%d ", validate_postfix(expr));

    /* eval_infix: 3 + 4 * 2 = 11 */
    expr[0]=3; expr[1]=-1; expr[2]=4; expr[3]=-3; expr[4]=2; expr[5]=-99;
    printf("EI1=%d ", eval_infix(expr));

    /* eval_infix: (3 + 4) * 2 = 14 */
    expr[0]=-6; expr[1]=3; expr[2]=-1; expr[3]=4; expr[4]=-7; expr[5]=-3; expr[6]=2; expr[7]=-99;
    printf("EI2=%d ", eval_infix(expr));

    /* eval_infix: 10 / 2 + 3 * 4 = 5 + 12 = 17 */
    expr[0]=10; expr[1]=-4; expr[2]=2; expr[3]=-1; expr[4]=3; expr[5]=-3; expr[6]=4; expr[7]=-99;
    printf("EI3=%d ", eval_infix(expr));

    /* eval_infix: (1 + 2) * (3 + 4) = 21 */
    expr[0]=-6; expr[1]=1; expr[2]=-1; expr[3]=2; expr[4]=-7; expr[5]=-3;
    expr[6]=-6; expr[7]=3; expr[8]=-1; expr[9]=4; expr[10]=-7; expr[11]=-99;
    printf("EI4=%d ", eval_infix(expr));

    /* eval_infix: 100 - 50 / 5 = 100 - 10 = 90 */
    expr[0]=100; expr[1]=-2; expr[2]=50; expr[3]=-4; expr[4]=5; expr[5]=-99;
    printf("EI5=%d ", eval_infix(expr));

    /* Expression equality check */
    {
        int e1[10];
        int e2[10];

        /* 3 4 + and 4 3 + both = 7 */
        e1[0]=3; e1[1]=4; e1[2]=-1; e1[3]=-99;
        e2[0]=4; e2[1]=3; e2[2]=-1; e2[3]=-99;
        printf("EEQ1=%d ", expr_equal(e1, e2));

        /* 3 4 + and 3 4 * not equal */
        e2[0]=3; e2[1]=4; e2[2]=-3; e2[3]=-99;
        printf("EEQ2=%d ", expr_equal(e1, e2));
    }

    /* Modulo operations */
    expr[0]=17; expr[1]=5; expr[2]=-5; expr[3]=-99;
    printf("MOD1=%d ", eval_postfix(expr));

    expr[0]=20; expr[1]=7; expr[2]=-5; expr[3]=-99;
    printf("MOD2=%d ", eval_postfix(expr));

    /* Power operations */
    expr[0]=2; expr[1]=10; expr[2]=-8; expr[3]=-99;
    printf("POW1=%d ", eval_postfix(expr));

    expr[0]=3; expr[1]=4; expr[2]=-8; expr[3]=-99;
    printf("POW2=%d ", eval_postfix(expr));

    printf("\n");
    return 0;
}
