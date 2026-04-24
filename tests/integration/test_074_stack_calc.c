int printf(const char *fmt, ...);
// EXPECT: === Postfix evaluation ===\n3 4 + = 7\n3 4 2 * + = 11\n5 1 2 + 4 * + 3 - = 14\n=== Infix to postfix ===\n3 + 4 * 2 => postfix count: 5\nPostfix eval: 11\n(3 + 4) * 2 => postfix count: 5\nPostfix eval: 14\n=== Infix eval direct ===\n2 + 3 * 4 = 14\n(2 + 3) * 4 = 20\n10 - 3 + 2 = 9\n6 / 2 + 1 = 4\n10 % 3 = 1\n=== Prefix evaluation ===\n+ 3 4 = 7\n* + 3 4 2 = 14\n=== Balanced parens ===\n(()): balanced=1\n(()(: balanced=0\n()((): balanced=0\n((())): balanced=1\n=== Stack reverse ===\nReversed: 5 4 3 2 1\n=== Next greater element ===\nNGE: 5 5 -1 -1\nNGE2: 3 4 4 -1\n=== Stock span ===\nSpan: 1 1 1 2 1 4 6\n=== Min stack ===\nAfter push 5,3,7,2: min=2\nPop 2: min=3\nPop 7: min=3\n=== Largest rect histogram ===\nLargest rect: 10\n=== Dec to bin ===\n13 in binary:1101\nBack to dec: 13\n42 in binary:101010\nBack to dec: 42\n=== Postfix validation ===\nValid postfix: 1\nInvalid postfix: 0\n=== Multi-base calc ===\n5 + 3 (base10) = 8\n7 * 6 (base10) = 42\n=== Hanoi moves ===\nHanoi(3) = 7\nHanoi(5) = 31\nHanoi(10) = 1023\n=== Stack Fibonacci ===\nfib(10) = 55\nfib(15) = 610\n=== Undo/Redo ===\nInitial: 10\nSet 20: 20\nSet 30: 30\nUndo: 20\nUndo: 10\nRedo: 20\nSet 40: 40\nUndo: 20\n=== Complex expression ===\n3+4*2-1 = 10\n(3+4)*(2-1) = 7\n2*3+4*5 = 26\n(2+3)*(4+5) = 45\nAll stack calc tests passed!
// ============================================================
// test_074_stack_calc.c
// Stack-based calculator, infix to postfix conversion
// (simplified), evaluation, operator precedence simulation.
// ============================================================

// ---- Integer Stack ----
struct IntStack {
    int data[100];
    int top;
};

void int_stack_init(struct IntStack *s) {
    s->top = -1;
}

int int_stack_empty(struct IntStack *s) {
    return (s->top == -1) ? 1 : 0;
}

void int_stack_push(struct IntStack *s, int val) {
    s->top++;
    s->data[s->top] = val;
}

int int_stack_pop(struct IntStack *s) {
    if (s->top == -1) return -999;
    int val = s->data[s->top];
    s->top--;
    return val;
}

int int_stack_peek(struct IntStack *s) {
    if (s->top == -1) return -999;
    return s->data[s->top];
}

int int_stack_size(struct IntStack *s) {
    return s->top + 1;
}

// ---- Character Stack (using int) ----
struct CharStack {
    int data[100];
    int top;
};

void char_stack_init(struct CharStack *s) {
    s->top = -1;
}

int char_stack_empty(struct CharStack *s) {
    return (s->top == -1) ? 1 : 0;
}

void char_stack_push(struct CharStack *s, int val) {
    s->top++;
    s->data[s->top] = val;
}

int char_stack_pop(struct CharStack *s) {
    if (s->top == -1) return 0;
    int val = s->data[s->top];
    s->top--;
    return val;
}

int char_stack_peek(struct CharStack *s) {
    if (s->top == -1) return 0;
    return s->data[s->top];
}

// ---- Token types for expression parsing ----
// We encode expressions as int arrays with a type array
// Type 0 = number, Type 1 = operator ('+', '-', '*', '/', '%')
// Type 2 = left paren, Type 3 = right paren

// ---- Operator precedence ----
int precedence(int op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    if (op == '^') return 3;
    return 0;
}

// ---- Right associative ----
int is_right_assoc(int op) {
    return (op == '^') ? 1 : 0;
}

// ---- Apply binary operator ----
int apply_op(int a, int b, int op) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') {
        if (b == 0) return 0;
        return a / b;
    }
    if (op == '%') {
        if (b == 0) return 0;
        return a % b;
    }
    if (op == '^') {
        // Integer power
        int result = 1;
        int i;
        for (i = 0; i < b; i++) result = result * a;
        return result;
    }
    return 0;
}

// ---- Evaluate simple postfix expression ----
// Expression given as: values array, ops array, where
// format[i] = 0 means value, 1 means operator
int eval_postfix(int tokens[], int types[], int n) {
    struct IntStack s;
    int_stack_init(&s);

    int i;
    for (i = 0; i < n; i++) {
        if (types[i] == 0) {
            // Number
            int_stack_push(&s, tokens[i]);
        } else {
            // Operator
            int b = int_stack_pop(&s);
            int a = int_stack_pop(&s);
            int result = apply_op(a, b, tokens[i]);
            int_stack_push(&s, result);
        }
    }
    return int_stack_pop(&s);
}

// ---- Infix to postfix conversion ----
// tokens: values or operators
// types: 0=number, 1=operator, 2=lparen, 3=rparen
// Writes to out_tokens, out_types, returns count
int infix_to_postfix(int tokens[], int types[], int n,
                     int out_tokens[], int out_types[]) {
    struct CharStack ops;
    char_stack_init(&ops);
    int count = 0;
    int i;

    for (i = 0; i < n; i++) {
        if (types[i] == 0) {
            // Number: output directly
            out_tokens[count] = tokens[i];
            out_types[count] = 0;
            count++;
        } else if (types[i] == 2) {
            // Left paren
            char_stack_push(&ops, '(');
        } else if (types[i] == 3) {
            // Right paren: pop until left paren
            while (!char_stack_empty(&ops) && char_stack_peek(&ops) != '(') {
                int op = char_stack_pop(&ops);
                out_tokens[count] = op;
                out_types[count] = 1;
                count++;
            }
            char_stack_pop(&ops); // pop '('
        } else {
            // Operator
            int op = tokens[i];
            while (!char_stack_empty(&ops) && char_stack_peek(&ops) != '(') {
                int top_op = char_stack_peek(&ops);
                if ((precedence(top_op) > precedence(op)) ||
                    (precedence(top_op) == precedence(op) && !is_right_assoc(op))) {
                    out_tokens[count] = char_stack_pop(&ops);
                    out_types[count] = 1;
                    count++;
                } else {
                    break;
                }
            }
            char_stack_push(&ops, op);
        }
    }
    // Pop remaining operators
    while (!char_stack_empty(&ops)) {
        out_tokens[count] = char_stack_pop(&ops);
        out_types[count] = 1;
        count++;
    }
    return count;
}

// ---- Evaluate infix expression ----
// Using two-stack method (Dijkstra's shunting-yard + immediate eval)
int eval_infix(int tokens[], int types[], int n) {
    int post_tokens[100], post_types[100];
    int pn = infix_to_postfix(tokens, types, n, post_tokens, post_types);
    return eval_postfix(post_tokens, post_types, pn);
}

// ---- Simple expression builder helpers ----
// Build expression: 3 + 4 * 2
// tokens: [3, +, 4, *, 2]
// types:  [0, 1, 0, 1, 0]

// ---- Stack-based parentheses matching ----
int check_balanced(int parens[], int n) {
    struct IntStack s;
    int_stack_init(&s);
    int i;
    for (i = 0; i < n; i++) {
        if (parens[i] == '(') {
            int_stack_push(&s, '(');
        } else if (parens[i] == ')') {
            if (int_stack_empty(&s)) return 0;
            int_stack_pop(&s);
        }
    }
    return int_stack_empty(&s);
}

// ---- Stack-based reversal of an array ----
void stack_reverse(int arr[], int n) {
    struct IntStack s;
    int_stack_init(&s);
    int i;
    for (i = 0; i < n; i++) {
        int_stack_push(&s, arr[i]);
    }
    for (i = 0; i < n; i++) {
        arr[i] = int_stack_pop(&s);
    }
}

// ---- Next greater element using stack ----
// For each element, find the next greater element to the right
void next_greater(int arr[], int n, int result[]) {
    struct IntStack s;
    int_stack_init(&s);
    int i;
    for (i = 0; i < n; i++) result[i] = -1;

    for (i = 0; i < n; i++) {
        while (!int_stack_empty(&s) && arr[int_stack_peek(&s)] < arr[i]) {
            int idx = int_stack_pop(&s);
            result[idx] = arr[i];
        }
        int_stack_push(&s, i);
    }
}

// ---- Stock span problem using stack ----
void stock_span(int prices[], int n, int spans[]) {
    struct IntStack s;
    int_stack_init(&s);
    int i;

    for (i = 0; i < n; i++) {
        while (!int_stack_empty(&s) && prices[int_stack_peek(&s)] <= prices[i]) {
            int_stack_pop(&s);
        }
        spans[i] = int_stack_empty(&s) ? (i + 1) : (i - int_stack_peek(&s));
        int_stack_push(&s, i);
    }
}

// ---- Evaluate prefix expression (reverse Polish) ----
int eval_prefix(int tokens[], int types[], int n) {
    struct IntStack s;
    int_stack_init(&s);
    int i;

    // Process right to left
    for (i = n - 1; i >= 0; i--) {
        if (types[i] == 0) {
            int_stack_push(&s, tokens[i]);
        } else {
            int a = int_stack_pop(&s);
            int b = int_stack_pop(&s);
            int_stack_push(&s, apply_op(a, b, tokens[i]));
        }
    }
    return int_stack_pop(&s);
}

// ---- Min stack: stack that supports O(1) min ----
struct MinStack {
    int data[100];
    int mins[100];
    int top;
};

void min_stack_init(struct MinStack *s) {
    s->top = -1;
}

void min_stack_push(struct MinStack *s, int val) {
    s->top++;
    s->data[s->top] = val;
    if (s->top == 0) {
        s->mins[s->top] = val;
    } else {
        int prev_min = s->mins[s->top - 1];
        s->mins[s->top] = (val < prev_min) ? val : prev_min;
    }
}

int min_stack_pop(struct MinStack *s) {
    int val = s->data[s->top];
    s->top--;
    return val;
}

int min_stack_min(struct MinStack *s) {
    return s->mins[s->top];
}

// ---- Largest rectangle in histogram using stack ----
int largest_rect_histogram(int heights[], int n) {
    struct IntStack s;
    int_stack_init(&s);
    int max_area = 0;
    int i;

    for (i = 0; i <= n; i++) {
        int h = (i == n) ? 0 : heights[i];
        while (!int_stack_empty(&s) && heights[int_stack_peek(&s)] > h) {
            int idx = int_stack_pop(&s);
            int width = int_stack_empty(&s) ? i : (i - int_stack_peek(&s) - 1);
            int area = heights[idx] * width;
            if (area > max_area) max_area = area;
        }
        int_stack_push(&s, i);
    }
    return max_area;
}

// ---- Decode string repetition: "3[2[ab]]" style but with ints ----
// We use: nums[i] with repeat counts and val arrays
// Simplified: stack-based repeat evaluation
// Encode: {3, 5} means repeat 5 three times = 15
int eval_repeat(int counts[], int values[], int n) {
    struct IntStack count_stack;
    struct IntStack value_stack;
    int_stack_init(&count_stack);
    int_stack_init(&value_stack);

    int total = 0;
    int i;
    for (i = 0; i < n; i++) {
        total = total + counts[i] * values[i];
    }
    return total;
}

// ---- Stack-based decimal to binary conversion ----
// Push remainders, pop to get binary digits
// Returns the number of binary digits
int dec_to_bin_stack(int n, int binary_digits[], int max_digits) {
    struct IntStack s;
    int_stack_init(&s);

    if (n == 0) {
        binary_digits[0] = 0;
        return 1;
    }

    int temp = n;
    while (temp > 0) {
        int_stack_push(&s, temp % 2);
        temp = temp / 2;
    }

    int count = 0;
    while (!int_stack_empty(&s) && count < max_digits) {
        binary_digits[count] = int_stack_pop(&s);
        count++;
    }
    return count;
}

// ---- Binary to decimal ----
int bin_to_dec(int binary_digits[], int n) {
    int result = 0;
    int i;
    for (i = 0; i < n; i++) {
        result = result * 2 + binary_digits[i];
    }
    return result;
}

// ---- Stack-based expression validator ----
// Check if expression has valid operator/operand ordering
// Types: 0=number, 1=operator
int validate_postfix_expr(int types[], int n) {
    int operand_count = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (types[i] == 0) {
            operand_count++;
        } else {
            if (operand_count < 2) return 0;
            operand_count--; // Two operands consumed, one result pushed
        }
    }
    return (operand_count == 1) ? 1 : 0;
}

// ---- Multi-base calculator ----
// Evaluate expression in given base, return result
int eval_in_base(int a, int b, int op, int base) {
    int result = apply_op(a, b, op);
    // Ensure result stays within base range (mod base^4)
    int max_val = base * base * base * base;
    if (result < 0) result = result + max_val;
    return result % max_val;
}

// ---- Tower of Hanoi counter ----
// Returns number of moves for n disks
int hanoi_moves(int n) {
    // 2^n - 1
    int result = 1;
    int i;
    for (i = 0; i < n; i++) result = result * 2;
    return result - 1;
}

// ---- Stack-based Fibonacci ----
int fib_stack(int n) {
    if (n <= 1) return n;
    struct IntStack s;
    int_stack_init(&s);
    int_stack_push(&s, 0);
    int_stack_push(&s, 1);
    int i;
    for (i = 2; i <= n; i++) {
        int b = int_stack_pop(&s);
        int a = int_stack_peek(&s);
        int_stack_push(&s, b);
        int_stack_push(&s, a + b);
    }
    return int_stack_peek(&s);
}

// ---- Undo/Redo stack simulation ----
struct UndoRedo {
    int undo_stack[50];
    int undo_top;
    int redo_stack[50];
    int redo_top;
    int current_value;
};

void undo_redo_init(struct UndoRedo *ur, int initial) {
    ur->undo_top = -1;
    ur->redo_top = -1;
    ur->current_value = initial;
}

void undo_redo_set(struct UndoRedo *ur, int new_val) {
    ur->undo_top++;
    ur->undo_stack[ur->undo_top] = ur->current_value;
    ur->current_value = new_val;
    ur->redo_top = -1; // Clear redo stack
}

int undo_redo_undo(struct UndoRedo *ur) {
    if (ur->undo_top == -1) return 0;
    ur->redo_top++;
    ur->redo_stack[ur->redo_top] = ur->current_value;
    ur->current_value = ur->undo_stack[ur->undo_top];
    ur->undo_top--;
    return 1;
}

int undo_redo_redo(struct UndoRedo *ur) {
    if (ur->redo_top == -1) return 0;
    ur->undo_top++;
    ur->undo_stack[ur->undo_top] = ur->current_value;
    ur->current_value = ur->redo_stack[ur->redo_top];
    ur->redo_top--;
    return 1;
}


int main() {
    // === Postfix evaluation ===
    printf("=== Postfix evaluation ===\n");
    {
        // 3 4 +
        int tokens[3];
        int types[3];
        tokens[0] = 3; types[0] = 0;
        tokens[1] = 4; types[1] = 0;
        tokens[2] = '+'; types[2] = 1;
        printf("3 4 + = %d\n", eval_postfix(tokens, types, 3));
    }
    {
        // 3 4 2 * +
        int tokens[5];
        int types[5];
        tokens[0] = 3; types[0] = 0;
        tokens[1] = 4; types[1] = 0;
        tokens[2] = 2; types[2] = 0;
        tokens[3] = '*'; types[3] = 1;
        tokens[4] = '+'; types[4] = 1;
        printf("3 4 2 * + = %d\n", eval_postfix(tokens, types, 5));
    }
    {
        // 5 1 2 + 4 * + 3 -
        int tokens[9];
        int types[9];
        tokens[0] = 5; types[0] = 0;
        tokens[1] = 1; types[1] = 0;
        tokens[2] = 2; types[2] = 0;
        tokens[3] = '+'; types[3] = 1;
        tokens[4] = 4; types[4] = 0;
        tokens[5] = '*'; types[5] = 1;
        tokens[6] = '+'; types[6] = 1;
        tokens[7] = 3; types[7] = 0;
        tokens[8] = '-'; types[8] = 1;
        printf("5 1 2 + 4 * + 3 - = %d\n", eval_postfix(tokens, types, 9));
    }

    // === Infix to postfix ===
    printf("=== Infix to postfix ===\n");
    {
        // 3 + 4 * 2
        int tokens[5], types[5];
        tokens[0] = 3; types[0] = 0;
        tokens[1] = '+'; types[1] = 1;
        tokens[2] = 4; types[2] = 0;
        tokens[3] = '*'; types[3] = 1;
        tokens[4] = 2; types[4] = 0;

        int out_t[20], out_ty[20];
        int cnt = infix_to_postfix(tokens, types, 5, out_t, out_ty);
        printf("3 + 4 * 2 => postfix count: %d\n", cnt);
        printf("Postfix eval: %d\n", eval_postfix(out_t, out_ty, cnt));
    }
    {
        // (3 + 4) * 2
        int tokens[7], types[7];
        tokens[0] = '('; types[0] = 2;
        tokens[1] = 3; types[1] = 0;
        tokens[2] = '+'; types[2] = 1;
        tokens[3] = 4; types[3] = 0;
        tokens[4] = ')'; types[4] = 3;
        tokens[5] = '*'; types[5] = 1;
        tokens[6] = 2; types[6] = 0;

        int out_t[20], out_ty[20];
        int cnt = infix_to_postfix(tokens, types, 7, out_t, out_ty);
        printf("(3 + 4) * 2 => postfix count: %d\n", cnt);
        printf("Postfix eval: %d\n", eval_postfix(out_t, out_ty, cnt));
    }

    // === Infix eval direct ===
    printf("=== Infix eval direct ===\n");
    {
        int t[5], ty[5];
        t[0]=2; ty[0]=0; t[1]='+'; ty[1]=1; t[2]=3; ty[2]=0; t[3]='*'; ty[3]=1; t[4]=4; ty[4]=0;
        printf("2 + 3 * 4 = %d\n", eval_infix(t, ty, 5));
    }
    {
        int t[7], ty[7];
        t[0]='('; ty[0]=2; t[1]=2; ty[1]=0; t[2]='+'; ty[2]=1;
        t[3]=3; ty[3]=0; t[4]=')'; ty[4]=3; t[5]='*'; ty[5]=1; t[6]=4; ty[6]=0;
        printf("(2 + 3) * 4 = %d\n", eval_infix(t, ty, 7));
    }
    {
        int t[5], ty[5];
        t[0]=10; ty[0]=0; t[1]='-'; ty[1]=1; t[2]=3; ty[2]=0; t[3]='+'; ty[3]=1; t[4]=2; ty[4]=0;
        printf("10 - 3 + 2 = %d\n", eval_infix(t, ty, 5));
    }
    {
        int t[5], ty[5];
        t[0]=6; ty[0]=0; t[1]='/'; ty[1]=1; t[2]=2; ty[2]=0; t[3]='+'; ty[3]=1; t[4]=1; ty[4]=0;
        printf("6 / 2 + 1 = %d\n", eval_infix(t, ty, 5));
    }
    {
        int t[3], ty[3];
        t[0]=10; ty[0]=0; t[1]='%'; ty[1]=1; t[2]=3; ty[2]=0;
        printf("10 %c 3 = %d\n", '%', eval_infix(t, ty, 3));
    }

    // === Prefix evaluation ===
    printf("=== Prefix evaluation ===\n");
    {
        // + 3 4
        int t[3], ty[3];
        t[0]='+'; ty[0]=1; t[1]=3; ty[1]=0; t[2]=4; ty[2]=0;
        printf("+ 3 4 = %d\n", eval_prefix(t, ty, 3));
    }
    {
        // * + 3 4 2
        int t[5], ty[5];
        t[0]='*'; ty[0]=1; t[1]='+'; ty[1]=1; t[2]=3; ty[2]=0; t[3]=4; ty[3]=0; t[4]=2; ty[4]=0;
        printf("* + 3 4 2 = %d\n", eval_prefix(t, ty, 5));
    }

    // === Balanced parens ===
    printf("=== Balanced parens ===\n");
    {
        int p1[4]; p1[0]='('; p1[1]='('; p1[2]=')'; p1[3]=')';
        printf("(()): balanced=%d\n", check_balanced(p1, 4));
    }
    {
        int p2[4]; p2[0]='('; p2[1]='('; p2[2]=')'; p2[3]='(';
        printf("(()(: balanced=%d\n", check_balanced(p2, 4));
    }
    {
        int p3[5]; p3[0]='('; p3[1]=')'; p3[2]='('; p3[3]='('; p3[4]=')';
        printf("()((): balanced=%d\n", check_balanced(p3, 5));
    }
    {
        int p4[6]; p4[0]='('; p4[1]='('; p4[2]='('; p4[3]=')'; p4[4]=')'; p4[5]=')';
        printf("((())): balanced=%d\n", check_balanced(p4, 6));
    }

    // === Stack reverse ===
    printf("=== Stack reverse ===\n");
    {
        int arr[5];
        arr[0]=1; arr[1]=2; arr[2]=3; arr[3]=4; arr[4]=5;
        stack_reverse(arr, 5);
        printf("Reversed:");
        int i;
        for (i = 0; i < 5; i++) printf(" %d", arr[i]);
        printf("\n");
    }

    // === Next greater element ===
    printf("=== Next greater element ===\n");
    {
        int arr[4], result[4];
        arr[0]=4; arr[1]=3; arr[2]=5; arr[3]=2;
        next_greater(arr, 4, result);
        printf("NGE:");
        int i;
        for (i = 0; i < 4; i++) printf(" %d", result[i]);
        printf("\n");
    }
    {
        int arr[4], result[4];
        arr[0]=1; arr[1]=3; arr[2]=2; arr[3]=4;
        next_greater(arr, 4, result);
        printf("NGE2:");
        int i;
        for (i = 0; i < 4; i++) printf(" %d", result[i]);
        printf("\n");
    }

    // === Stock span ===
    printf("=== Stock span ===\n");
    {
        int prices[7], spans[7];
        prices[0]=100; prices[1]=80; prices[2]=60; prices[3]=70;
        prices[4]=60; prices[5]=75; prices[6]=85;
        stock_span(prices, 7, spans);
        printf("Span:");
        int i;
        for (i = 0; i < 7; i++) printf(" %d", spans[i]);
        printf("\n");
    }

    // === Min stack ===
    printf("=== Min stack ===\n");
    {
        struct MinStack ms;
        min_stack_init(&ms);
        min_stack_push(&ms, 5);
        min_stack_push(&ms, 3);
        min_stack_push(&ms, 7);
        min_stack_push(&ms, 2);
        printf("After push 5,3,7,2: min=%d\n", min_stack_min(&ms));
        min_stack_pop(&ms);
        printf("Pop 2: min=%d\n", min_stack_min(&ms));
        min_stack_pop(&ms);
        printf("Pop 7: min=%d\n", min_stack_min(&ms));
    }

    // === Largest rect histogram ===
    printf("=== Largest rect histogram ===\n");
    {
        int h[6];
        h[0]=2; h[1]=1; h[2]=5; h[3]=6; h[4]=2; h[5]=3;
        printf("Largest rect: %d\n", largest_rect_histogram(h, 6));
    }

    // === Dec to bin ===
    printf("=== Dec to bin ===\n");
    {
        int digits[32];
        int n = dec_to_bin_stack(13, digits, 32);
        printf("13 in binary:");
        int i;
        for (i = 0; i < n; i++) printf("%d", digits[i]);
        printf("\n");
        printf("Back to dec: %d\n", bin_to_dec(digits, n));
    }
    {
        int digits[32];
        int n = dec_to_bin_stack(42, digits, 32);
        printf("42 in binary:");
        int i;
        for (i = 0; i < n; i++) printf("%d", digits[i]);
        printf("\n");
        printf("Back to dec: %d\n", bin_to_dec(digits, n));
    }

    // === Postfix validation ===
    printf("=== Postfix validation ===\n");
    {
        int ty[5]; ty[0]=0; ty[1]=0; ty[2]=0; ty[3]=1; ty[4]=1;
        printf("Valid postfix: %d\n", validate_postfix_expr(ty, 5));
    }
    {
        int ty[3]; ty[0]=0; ty[1]=1; ty[2]=0;
        printf("Invalid postfix: %d\n", validate_postfix_expr(ty, 3));
    }

    // === Multi-base calc ===
    printf("=== Multi-base calc ===\n");
    printf("5 + 3 (base10) = %d\n", eval_in_base(5, 3, '+', 10));
    printf("7 * 6 (base10) = %d\n", eval_in_base(7, 6, '*', 10));

    // === Hanoi moves ===
    printf("=== Hanoi moves ===\n");
    printf("Hanoi(3) = %d\n", hanoi_moves(3));
    printf("Hanoi(5) = %d\n", hanoi_moves(5));
    printf("Hanoi(10) = %d\n", hanoi_moves(10));

    // === Stack Fibonacci ===
    printf("=== Stack Fibonacci ===\n");
    printf("fib(10) = %d\n", fib_stack(10));
    printf("fib(15) = %d\n", fib_stack(15));

    // === Undo/Redo ===
    printf("=== Undo/Redo ===\n");
    {
        struct UndoRedo ur;
        undo_redo_init(&ur, 10);
        printf("Initial: %d\n", ur.current_value);
        undo_redo_set(&ur, 20);
        printf("Set 20: %d\n", ur.current_value);
        undo_redo_set(&ur, 30);
        printf("Set 30: %d\n", ur.current_value);
        undo_redo_undo(&ur);
        printf("Undo: %d\n", ur.current_value);
        undo_redo_undo(&ur);
        printf("Undo: %d\n", ur.current_value);
        undo_redo_redo(&ur);
        printf("Redo: %d\n", ur.current_value);
        undo_redo_set(&ur, 40);
        printf("Set 40: %d\n", ur.current_value);
        undo_redo_undo(&ur);
        printf("Undo: %d\n", ur.current_value);
    }

    // === Complex expressions ===
    printf("=== Complex expression ===\n");
    {
        // 3+4*2-1
        int t[7], ty[7];
        t[0]=3; ty[0]=0; t[1]='+'; ty[1]=1; t[2]=4; ty[2]=0;
        t[3]='*'; ty[3]=1; t[4]=2; ty[4]=0; t[5]='-'; ty[5]=1; t[6]=1; ty[6]=0;
        printf("3+4*2-1 = %d\n", eval_infix(t, ty, 7));
    }
    {
        // (3+4)*(2-1)
        int t[11], ty[11];
        t[0]='('; ty[0]=2; t[1]=3; ty[1]=0; t[2]='+'; ty[2]=1;
        t[3]=4; ty[3]=0; t[4]=')'; ty[4]=3; t[5]='*'; ty[5]=1;
        t[6]='('; ty[6]=2; t[7]=2; ty[7]=0; t[8]='-'; ty[8]=1;
        t[9]=1; ty[9]=0; t[10]=')'; ty[10]=3;
        printf("(3+4)*(2-1) = %d\n", eval_infix(t, ty, 11));
    }
    {
        // 2*3+4*5
        int t[7], ty[7];
        t[0]=2; ty[0]=0; t[1]='*'; ty[1]=1; t[2]=3; ty[2]=0;
        t[3]='+'; ty[3]=1; t[4]=4; ty[4]=0; t[5]='*'; ty[5]=1; t[6]=5; ty[6]=0;
        printf("2*3+4*5 = %d\n", eval_infix(t, ty, 7));
    }
    {
        // (2+3)*(4+5)
        int t[11], ty[11];
        t[0]='('; ty[0]=2; t[1]=2; ty[1]=0; t[2]='+'; ty[2]=1;
        t[3]=3; ty[3]=0; t[4]=')'; ty[4]=3; t[5]='*'; ty[5]=1;
        t[6]='('; ty[6]=2; t[7]=4; ty[7]=0; t[8]='+'; ty[8]=1;
        t[9]=5; ty[9]=0; t[10]=')'; ty[10]=3;
        printf("(2+3)*(4+5) = %d\n", eval_infix(t, ty, 11));
    }

    printf("All stack calc tests passed!\n");
    return 0;
}
