int printf(const char *fmt, ...);
// EXPECT: 30 30 20 1 0 1 6 120 720 5040 1 2 3 4 5 1 0 1 7 2 6 14 5 25 25 -1 -1 1 1 1 2 1 3 3 5 20 14 7 10 9 1 2 3 29 18 1 2 3 1024 243 3 8 3 26 13 6 7 5 45 14

// Stack-based calculator using array as stack
// Operations: push, pop, add, subtract, multiply, divide
// Evaluates complex arithmetic expressions

int stack[100];
int sp;

void stack_init(void) {
    sp = 0;
}

void stack_push(int val) {
    stack[sp] = val;
    sp = sp + 1;
}

int stack_pop(void) {
    sp = sp - 1;
    return stack[sp];
}

int stack_peek(void) {
    return stack[sp - 1];
}

int stack_size(void) {
    return sp;
}

int stack_empty(void) {
    if (sp == 0) return 1;
    return 0;
}

// Evaluate: a op b
int apply_op(int a, int b, int op) {
    if (op == 1) return a + b;
    if (op == 2) return a - b;
    if (op == 3) return a * b;
    if (op == 4) {
        if (b == 0) return 0;
        return a / b;
    }
    return 0;
}

// Compute factorial using stack
int factorial_stack(int n) {
    int result;
    stack_init();
    result = 1;
    while (n > 0) {
        stack_push(n);
        n = n - 1;
    }
    while (stack_empty() == 0) {
        result = result * stack_pop();
    }
    return result;
}

// Reverse an array using stack
void reverse_array(int arr[], int n) {
    int i;
    stack_init();
    for (i = 0; i < n; i = i + 1) {
        stack_push(arr[i]);
    }
    for (i = 0; i < n; i = i + 1) {
        arr[i] = stack_pop();
    }
}

// Check if parentheses are balanced
// We represent '(' as 1 and ')' as 2
int check_balanced(int tokens[], int n) {
    int i;
    stack_init();
    for (i = 0; i < n; i = i + 1) {
        if (tokens[i] == 1) {
            stack_push(1);
        } else if (tokens[i] == 2) {
            if (stack_empty()) return 0;
            stack_pop();
        }
    }
    if (stack_empty()) return 1;
    return 0;
}

// Evaluate postfix expression
// Numbers are positive, operators encoded as negative:
// -1 = +, -2 = -, -3 = *, -4 = /
int eval_postfix(int expr[], int n) {
    int i;
    int a;
    int b;
    int result;
    stack_init();
    for (i = 0; i < n; i = i + 1) {
        if (expr[i] >= 0) {
            stack_push(expr[i]);
        } else {
            b = stack_pop();
            a = stack_pop();
            if (expr[i] == -1) result = a + b;
            else if (expr[i] == -2) result = a - b;
            else if (expr[i] == -3) result = a * b;
            else if (expr[i] == -4) {
                if (b == 0) result = 0;
                else result = a / b;
            } else {
                result = 0;
            }
            stack_push(result);
        }
    }
    return stack_pop();
}

// Next greater element using stack
void next_greater(int arr[], int n, int result[]) {
    int i;
    int top;
    stack_init();
    for (i = 0; i < n; i = i + 1) {
        result[i] = -1;
    }
    for (i = 0; i < n; i = i + 1) {
        while (stack_empty() == 0 && arr[stack_peek()] < arr[i]) {
            top = stack_pop();
            result[top] = arr[i];
        }
        stack_push(i);
    }
}

// Stock span problem
void stock_span(int prices[], int n, int spans[]) {
    int i;
    stack_init();
    for (i = 0; i < n; i = i + 1) {
        while (stack_empty() == 0 && prices[stack_peek()] <= prices[i]) {
            stack_pop();
        }
        if (stack_empty()) {
            spans[i] = i + 1;
        } else {
            spans[i] = i - stack_peek();
        }
        stack_push(i);
    }
}

// Min stack - track minimum using two stacks
int min_stack[100];
int min_sp;

void min_stack_init(void) {
    stack_init();
    min_sp = 0;
}

void min_stack_push(int val) {
    stack_push(val);
    if (min_sp == 0 || val <= min_stack[min_sp - 1]) {
        min_stack[min_sp] = val;
        min_sp = min_sp + 1;
    }
}

int min_stack_pop(void) {
    int val;
    val = stack_pop();
    if (val == min_stack[min_sp - 1]) {
        min_sp = min_sp - 1;
    }
    return val;
}

int min_stack_get_min(void) {
    return min_stack[min_sp - 1];
}

// Evaluate expression: convert simple infix to result
// Simple: just numbers and single ops, left-to-right
int eval_left_to_right(int nums[], int ops[], int n) {
    int result;
    int i;
    result = nums[0];
    for (i = 0; i < n; i = i + 1) {
        result = apply_op(result, nums[i + 1], ops[i]);
    }
    return result;
}

// Largest rectangle in histogram using stack
int largest_rect_histogram(int heights[], int n) {
    int max_area;
    int area;
    int i;
    int top;
    int width;
    max_area = 0;
    stack_init();
    for (i = 0; i <= n; i = i + 1) {
        while (stack_empty() == 0 && (i == n || heights[stack_peek()] > heights[i])) {
            top = stack_pop();
            if (stack_empty()) {
                width = i;
            } else {
                width = i - stack_peek() - 1;
            }
            area = heights[top] * width;
            if (area > max_area) max_area = area;
        }
        if (i < n) stack_push(i);
    }
    return max_area;
}

// Sort a stack using another stack (selection sort style)
void sort_stack(int arr[], int n) {
    int temp_stack[100];
    int temp_sp;
    int tmp;
    int i;
    int j;
    // Simple bubble sort using the array
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

// Decode a sequence using stack
// Given encoded pairs (count, value), decode and sum
int decode_sum(int counts[], int values[], int n) {
    int sum;
    int i;
    int j;
    sum = 0;
    stack_init();
    for (i = 0; i < n; i = i + 1) {
        for (j = 0; j < counts[i]; j = j + 1) {
            stack_push(values[i]);
        }
    }
    while (stack_empty() == 0) {
        sum = sum + stack_pop();
    }
    return sum;
}

// Two-stack queue simulation
int q_in[100];
int q_in_sp;
int q_out[100];
int q_out_sp;

void queue_init(void) {
    q_in_sp = 0;
    q_out_sp = 0;
}

void queue_enqueue(int val) {
    q_in[q_in_sp] = val;
    q_in_sp = q_in_sp + 1;
}

int queue_dequeue(void) {
    int i;
    if (q_out_sp == 0) {
        while (q_in_sp > 0) {
            q_in_sp = q_in_sp - 1;
            q_out[q_out_sp] = q_in[q_in_sp];
            q_out_sp = q_out_sp + 1;
        }
    }
    q_out_sp = q_out_sp - 1;
    return q_out[q_out_sp];
}

// Calculate power using stack
int power_stack(int base, int exp) {
    int result;
    stack_init();
    while (exp > 0) {
        stack_push(base);
        exp = exp - 1;
    }
    result = 1;
    while (stack_empty() == 0) {
        result = result * stack_pop();
    }
    return result;
}

// Convert decimal to binary using stack, return number of 1-bits
int count_ones_binary(int n) {
    int count;
    count = 0;
    stack_init();
    if (n == 0) return 0;
    while (n > 0) {
        stack_push(n % 2);
        n = n / 2;
    }
    while (stack_empty() == 0) {
        count = count + stack_pop();
    }
    return count;
}

// Expression with precedence: evaluate a*b+c*d pattern
int eval_mul_add(int a, int b, int c, int d) {
    return a * b + c * d;
}

// Trapping rain water using stack
int trap_water(int heights[], int n) {
    int water;
    int i;
    int top;
    int dist;
    int bounded;
    int h;
    water = 0;
    stack_init();
    for (i = 0; i < n; i = i + 1) {
        while (stack_empty() == 0 && heights[i] > heights[stack_peek()]) {
            top = stack_pop();
            if (stack_empty()) break;
            dist = i - stack_peek() - 1;
            h = heights[stack_peek()];
            if (heights[i] < h) h = heights[i];
            bounded = h - heights[top];
            water = water + dist * bounded;
        }
        stack_push(i);
    }
    return water;
}

int main(void) {
    int arr[20];
    int result[20];
    int spans[20];
    int i;
    int val;
    int postfix[20];
    int heights[20];
    int nums[10];
    int ops[10];
    int counts[10];
    int values[10];

    // Test 1-5: Basic stack operations
    stack_init();
    stack_push(10);
    stack_push(20);
    stack_push(30);
    printf("%d", stack_peek());    // 30
    printf(" %d", stack_pop());    // 30
    printf(" %d", stack_pop());    // 20
    printf(" %d", stack_size());   // 1
    printf(" %d", stack_empty());  // 0

    // Test 6-10: Factorial using stack
    printf(" %d", factorial_stack(1));   // 1
    printf(" %d", factorial_stack(3));   // 6
    printf(" %d", factorial_stack(5));   // 120
    printf(" %d", factorial_stack(6));   // 720
    printf(" %d", factorial_stack(7));   // 5040

    // Test 11-15: Reverse array using stack
    arr[0] = 5; arr[1] = 4; arr[2] = 3; arr[3] = 2; arr[4] = 1;
    reverse_array(arr, 5);
    printf(" %d", arr[0]);  // 1
    printf(" %d", arr[1]);  // 2
    printf(" %d", arr[2]);  // 3
    printf(" %d", arr[3]);  // 4
    printf(" %d", arr[4]);  // 5

    // Test 16-18: Balanced parentheses
    // (()) -> 1,1,2,2
    arr[0]=1; arr[1]=1; arr[2]=2; arr[3]=2;
    printf(" %d", check_balanced(arr, 4)); // 1
    // (() -> 1,1,2
    arr[0]=1; arr[1]=1; arr[2]=2;
    printf(" %d", check_balanced(arr, 3)); // 0
    // ()() -> 1,2,1,2
    arr[0]=1; arr[1]=2; arr[2]=1; arr[3]=2;
    printf(" %d", check_balanced(arr, 4)); // 1

    // Test 19-22: Postfix evaluation
    // 3 4 + -> 7
    postfix[0]=3; postfix[1]=4; postfix[2]=-1;
    printf(" %d", eval_postfix(postfix, 3)); // 7
    // 5 3 - -> 2
    postfix[0]=5; postfix[1]=3; postfix[2]=-2;
    printf(" %d", eval_postfix(postfix, 3)); // 2
    // 2 3 * -> 6
    postfix[0]=2; postfix[1]=3; postfix[2]=-3;
    printf(" %d", eval_postfix(postfix, 3)); // 6
    // 3 4 + 2 * -> 14
    postfix[0]=3; postfix[1]=4; postfix[2]=-1; postfix[3]=2; postfix[4]=-3;
    printf(" %d", eval_postfix(postfix, 5)); // 14

    // Test 23-27: Next greater element
    arr[0]=4; arr[1]=5; arr[2]=2; arr[3]=25; arr[4]=7;
    next_greater(arr, 5, result);
    printf(" %d", result[0]); // 5
    printf(" %d", result[1]); // 25
    printf(" %d", result[2]); // 25
    printf(" %d", result[3]); // -1
    printf(" %d", result[4]); // -1

    // Test 28-32: Stock span
    arr[0]=100; arr[1]=80; arr[2]=60; arr[3]=70; arr[4]=60;
    arr[5]=75; arr[6]=85;
    stock_span(arr, 7, spans);
    printf(" %d", spans[0]); // 1
    printf(" %d", spans[1]); // 1
    printf(" %d", spans[2]); // 1
    printf(" %d", spans[3]); // 2
    printf(" %d", spans[4]); // 1

    // Test 33-35: Min stack
    min_stack_init();
    min_stack_push(5);
    min_stack_push(3);
    min_stack_push(7);
    printf(" %d", min_stack_get_min()); // 3
    min_stack_pop();
    printf(" %d", min_stack_get_min()); // 3
    min_stack_pop();
    printf(" %d", min_stack_get_min()); // 5

    // Test 36-38: Left to right evaluation
    nums[0]=2; nums[1]=3; nums[2]=4; ops[0]=1; ops[1]=3;
    printf(" %d", eval_left_to_right(nums, ops, 2)); // (2+3)*4 = 20
    nums[0]=10; nums[1]=3; nums[2]=2; ops[0]=2; ops[1]=3;
    printf(" %d", eval_left_to_right(nums, ops, 2)); // (10-3)*2 = 14
    nums[0]=8; nums[1]=2; nums[2]=3; ops[0]=4; ops[1]=1;
    printf(" %d", eval_left_to_right(nums, ops, 2)); // (8/2)+3 = 7

    // Test 39: Largest rectangle in histogram
    heights[0]=2; heights[1]=1; heights[2]=5; heights[3]=6;
    heights[4]=2; heights[5]=3;
    printf(" %d", largest_rect_histogram(heights, 6)); // 10

    // Test 40: Another histogram
    heights[0]=1; heights[1]=2; heights[2]=3; heights[3]=4; heights[4]=5;
    printf(" %d", largest_rect_histogram(heights, 5)); // 9

    // Test 41-43: Sort stack
    arr[0]=5; arr[1]=3; arr[2]=1; arr[3]=4; arr[4]=2;
    sort_stack(arr, 5);
    printf(" %d", arr[0]); // 1
    printf(" %d", arr[1]); // 2
    printf(" %d", arr[2]); // 3

    // Test 44-45: Decode sum
    counts[0]=2; counts[1]=3; counts[2]=1;
    values[0]=5; values[1]=3; values[2]=10;
    printf(" %d", decode_sum(counts, values, 3)); // 2*5 + 3*3 + 1*10 = 29
    counts[0]=4; counts[1]=2;
    values[0]=1; values[1]=7;
    printf(" %d", decode_sum(counts, values, 2)); // 4*1 + 2*7 = 18

    // Test 46-48: Two-stack queue
    queue_init();
    queue_enqueue(1);
    queue_enqueue(2);
    queue_enqueue(3);
    printf(" %d", queue_dequeue()); // 1
    printf(" %d", queue_dequeue()); // 2
    queue_enqueue(4);
    printf(" %d", queue_dequeue()); // 3

    // Test 49-50: Power using stack
    printf(" %d", power_stack(2, 10)); // 1024
    printf(" %d", power_stack(3, 5));  // 243

    // Test 51-53: Count ones in binary
    printf(" %d", count_ones_binary(7));   // 3
    printf(" %d", count_ones_binary(255)); // 8
    printf(" %d", count_ones_binary(100)); // 3

    // Test 54-55: Mul-add expressions
    printf(" %d", eval_mul_add(2, 3, 4, 5)); // 26
    printf(" %d", eval_mul_add(1, 7, 2, 3)); // 13

    // Test 56: Trapping rain water
    heights[0]=0; heights[1]=1; heights[2]=0; heights[3]=2;
    heights[4]=1; heights[5]=0; heights[6]=1; heights[7]=3;
    heights[8]=2; heights[9]=1; heights[10]=2; heights[11]=1;
    printf(" %d", trap_water(heights, 12)); // 6

    // Test 57: More trapping rain water
    heights[0]=3; heights[1]=0; heights[2]=2; heights[3]=0; heights[4]=4;
    printf(" %d", trap_water(heights, 5)); // 7

    // Test 58-60: More postfix
    // 10 2 / -> 5
    postfix[0]=10; postfix[1]=2; postfix[2]=-4;
    printf(" %d", eval_postfix(postfix, 3)); // 5
    // 2 3 + 4 5 + * -> 45
    postfix[0]=2; postfix[1]=3; postfix[2]=-1; postfix[3]=4; postfix[4]=5; postfix[5]=-1; postfix[6]=-3;
    printf(" %d", eval_postfix(postfix, 7)); // 45
    // 5 1 2 + 4 * + 3 - -> 14
    postfix[0]=5; postfix[1]=1; postfix[2]=2; postfix[3]=-1; postfix[4]=4; postfix[5]=-3; postfix[6]=-1; postfix[7]=3; postfix[8]=-2;
    printf(" %d", eval_postfix(postfix, 9)); // 14

    printf("\n");
    return 0;
}
