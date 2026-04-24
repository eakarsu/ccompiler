int printf(const char *fmt, ...);
// EXPECT: stack empty: 1\nstack size: 0\npush 10: 1\npush 20: 1\npush 30: 1\npeek: 30\nsize: 3\nempty: 0\npop: 30\npop: 20\npeek: 10\npop: 10\nempty: 1\npop empty: -999\nqueue empty: 1\nqueue size: 0\nenq 100: 1\nenq 200: 1\nenq 300: 1\nq peek: 100\nq size: 3\ndeq: 100\ndeq: 200\nq peek: 300\ndeq: 300\nq empty: 1\ndeq empty: -999\nreversed: 5 4 3 2 1\nbalanced 1: 1\nbalanced 2: 0\nbalanced 3: 1\ns1 peek: 2\ns2 peek: 20\ns1 size: 2\ns2 size: 2\npostfix 3 4 +: 7\npostfix 2 3 * 5 +: 11\npostfix 10 3 -: 7\npq deq: 10\npq deq: 20\npq deq: 5\npq deq: 15\nround robin: 1\nstack 10 size: 10\nstack top: 81\nafter 5 pops size: 5\nafter 5 pops top: 16\nqueue 10 size: 10\nafter 5 deq size: 5\nafter 5 deq front: 6

// ============================================================
// test_013_stack_queue.c
// Stack and Queue implementations using arrays
// ============================================================

// -----------------------------------------------------------
// STACK implementation using array
// -----------------------------------------------------------
int stack_data[50];
int stack_top;

// -----------------------------------------------------------
// Initialize stack
// -----------------------------------------------------------
void stack_init(void) {
    stack_top = -1;
}

// -----------------------------------------------------------
// Check if stack is empty
// -----------------------------------------------------------
int stack_empty(void) {
    if (stack_top == -1) {
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------
// Check if stack is full
// -----------------------------------------------------------
int stack_full(void) {
    if (stack_top == 49) {
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------
// Push onto stack, return 1 success, 0 fail
// -----------------------------------------------------------
int stack_push(int val) {
    if (stack_full()) {
        return 0;
    }
    stack_top = stack_top + 1;
    stack_data[stack_top] = val;
    return 1;
}

// -----------------------------------------------------------
// Pop from stack, return value, -999 if empty
// -----------------------------------------------------------
int stack_pop(void) {
    int val;
    if (stack_empty()) {
        return -999;
    }
    val = stack_data[stack_top];
    stack_top = stack_top - 1;
    return val;
}

// -----------------------------------------------------------
// Peek at top, return value, -999 if empty
// -----------------------------------------------------------
int stack_peek(void) {
    if (stack_empty()) {
        return -999;
    }
    return stack_data[stack_top];
}

// -----------------------------------------------------------
// Stack size
// -----------------------------------------------------------
int stack_size(void) {
    return stack_top + 1;
}

// -----------------------------------------------------------
// Clear stack
// -----------------------------------------------------------
void stack_clear(void) {
    stack_top = -1;
}

// -----------------------------------------------------------
// QUEUE implementation using circular array
// -----------------------------------------------------------
int queue_data[50];
int queue_front;
int queue_rear;
int queue_count;

// -----------------------------------------------------------
// Initialize queue
// -----------------------------------------------------------
void queue_init(void) {
    queue_front = 0;
    queue_rear = -1;
    queue_count = 0;
}

// -----------------------------------------------------------
// Check if queue is empty
// -----------------------------------------------------------
int queue_empty(void) {
    if (queue_count == 0) {
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------
// Check if queue is full
// -----------------------------------------------------------
int queue_full(void) {
    if (queue_count == 50) {
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------
// Enqueue, return 1 success, 0 fail
// -----------------------------------------------------------
int queue_enqueue(int val) {
    if (queue_full()) {
        return 0;
    }
    queue_rear = queue_rear + 1;
    if (queue_rear == 50) {
        queue_rear = 0;
    }
    queue_data[queue_rear] = val;
    queue_count = queue_count + 1;
    return 1;
}

// -----------------------------------------------------------
// Dequeue, return value, -999 if empty
// -----------------------------------------------------------
int queue_dequeue(void) {
    int val;
    if (queue_empty()) {
        return -999;
    }
    val = queue_data[queue_front];
    queue_front = queue_front + 1;
    if (queue_front == 50) {
        queue_front = 0;
    }
    queue_count = queue_count - 1;
    return val;
}

// -----------------------------------------------------------
// Peek at front, return value, -999 if empty
// -----------------------------------------------------------
int queue_peek(void) {
    if (queue_empty()) {
        return -999;
    }
    return queue_data[queue_front];
}

// -----------------------------------------------------------
// Queue size
// -----------------------------------------------------------
int queue_size(void) {
    return queue_count;
}

// -----------------------------------------------------------
// Clear queue
// -----------------------------------------------------------
void queue_clear(void) {
    queue_front = 0;
    queue_rear = -1;
    queue_count = 0;
}

// -----------------------------------------------------------
// SECOND STACK for testing two stacks simultaneously
// -----------------------------------------------------------
int stack2_data[50];
int stack2_top;

void stack2_init(void) {
    stack2_top = -1;
}

int stack2_push(int val) {
    if (stack2_top == 49) {
        return 0;
    }
    stack2_top = stack2_top + 1;
    stack2_data[stack2_top] = val;
    return 1;
}

int stack2_pop(void) {
    int val;
    if (stack2_top == -1) {
        return -999;
    }
    val = stack2_data[stack2_top];
    stack2_top = stack2_top - 1;
    return val;
}

int stack2_peek(void) {
    if (stack2_top == -1) {
        return -999;
    }
    return stack2_data[stack2_top];
}

int stack2_size(void) {
    return stack2_top + 1;
}

// -----------------------------------------------------------
// Use stack to reverse an array of values
// -----------------------------------------------------------
void reverse_with_stack(int arr[20], int n) {
    int i;
    stack_clear();
    for (i = 0; i < n; i = i + 1) {
        stack_push(arr[i]);
    }
    for (i = 0; i < n; i = i + 1) {
        arr[i] = stack_pop();
    }
}

// -----------------------------------------------------------
// Check if sequence of parens is balanced
// Encoding: 1 = open paren, 0 = close paren
// -----------------------------------------------------------
int check_balanced(int seq[20], int n) {
    int i;
    int v;
    stack_clear();
    for (i = 0; i < n; i = i + 1) {
        if (seq[i] == 1) {
            stack_push(1);
        } else {
            if (stack_empty()) {
                return 0;
            }
            v = stack_pop();
        }
    }
    if (stack_empty()) {
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------
// Use queue to simulate round-robin process scheduling
// Returns last remaining process id
// -----------------------------------------------------------
int round_robin(int procs[10], int n) {
    int i;
    int p;
    queue_clear();
    for (i = 0; i < n; i = i + 1) {
        queue_enqueue(procs[i]);
    }
    while (queue_size() > 1) {
        p = queue_dequeue();
        // Process does one unit of work (decrement)
        p = p - 1;
        if (p > 0) {
            queue_enqueue(p);
        }
    }
    return queue_dequeue();
}

// -----------------------------------------------------------
// Priority queue simulation using two queues approach
// (sort values into high/low priority)
// -----------------------------------------------------------
int pq_high[50];
int pq_high_front;
int pq_high_rear;
int pq_high_count;
int pq_low[50];
int pq_low_front;
int pq_low_rear;
int pq_low_count;

void pq_init(void) {
    pq_high_front = 0;
    pq_high_rear = -1;
    pq_high_count = 0;
    pq_low_front = 0;
    pq_low_rear = -1;
    pq_low_count = 0;
}

void pq_enqueue(int val, int priority) {
    if (priority == 1) {
        pq_high_rear = pq_high_rear + 1;
        pq_high[pq_high_rear] = val;
        pq_high_count = pq_high_count + 1;
    } else {
        pq_low_rear = pq_low_rear + 1;
        pq_low[pq_low_rear] = val;
        pq_low_count = pq_low_count + 1;
    }
}

int pq_dequeue(void) {
    int val;
    if (pq_high_count > 0) {
        val = pq_high[pq_high_front];
        pq_high_front = pq_high_front + 1;
        pq_high_count = pq_high_count - 1;
        return val;
    }
    if (pq_low_count > 0) {
        val = pq_low[pq_low_front];
        pq_low_front = pq_low_front + 1;
        pq_low_count = pq_low_count - 1;
        return val;
    }
    return -999;
}

// -----------------------------------------------------------
// Evaluate simple postfix expression using stack
// Operators encoded as: -1 = add, -2 = mul, -3 = sub
// -----------------------------------------------------------
int eval_postfix(int expr[20], int n) {
    int i;
    int a;
    int b;
    int r;
    stack_clear();
    for (i = 0; i < n; i = i + 1) {
        if (expr[i] >= 0) {
            stack_push(expr[i]);
        } else {
            b = stack_pop();
            a = stack_pop();
            if (expr[i] == -1) {
                r = a + b;
            } else if (expr[i] == -2) {
                r = a * b;
            } else {
                r = a - b;
            }
            stack_push(r);
        }
    }
    return stack_pop();
}

// -----------------------------------------------------------
// MAIN
// -----------------------------------------------------------
int main(void) {
    int r;
    int i;
    int arr[20];
    int seq[20];
    int procs[10];
    int expr[20];

    // ---- Stack basic tests ----
    stack_init();
    printf("stack empty: %d\n", stack_empty());
    printf("stack size: %d\n", stack_size());

    r = stack_push(10);
    printf("push 10: %d\n", r);
    r = stack_push(20);
    printf("push 20: %d\n", r);
    r = stack_push(30);
    printf("push 30: %d\n", r);

    printf("peek: %d\n", stack_peek());
    printf("size: %d\n", stack_size());
    printf("empty: %d\n", stack_empty());

    r = stack_pop();
    printf("pop: %d\n", r);
    r = stack_pop();
    printf("pop: %d\n", r);
    printf("peek: %d\n", stack_peek());
    r = stack_pop();
    printf("pop: %d\n", r);
    printf("empty: %d\n", stack_empty());
    r = stack_pop();
    printf("pop empty: %d\n", r);

    // ---- Queue basic tests ----
    queue_init();
    printf("queue empty: %d\n", queue_empty());
    printf("queue size: %d\n", queue_size());

    r = queue_enqueue(100);
    printf("enq 100: %d\n", r);
    r = queue_enqueue(200);
    printf("enq 200: %d\n", r);
    r = queue_enqueue(300);
    printf("enq 300: %d\n", r);

    printf("q peek: %d\n", queue_peek());
    printf("q size: %d\n", queue_size());

    r = queue_dequeue();
    printf("deq: %d\n", r);
    r = queue_dequeue();
    printf("deq: %d\n", r);
    printf("q peek: %d\n", queue_peek());
    r = queue_dequeue();
    printf("deq: %d\n", r);
    printf("q empty: %d\n", queue_empty());
    r = queue_dequeue();
    printf("deq empty: %d\n", r);

    // ---- Stack reverse array test ----
    arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5;
    reverse_with_stack(arr, 5);
    printf("reversed: %d %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3], arr[4]);

    // ---- Balanced parentheses ----
    // (()) -> 1 1 0 0 -> balanced
    seq[0] = 1; seq[1] = 1; seq[2] = 0; seq[3] = 0;
    printf("balanced 1: %d\n", check_balanced(seq, 4));
    // (() -> 1 1 0 -> not balanced
    seq[0] = 1; seq[1] = 1; seq[2] = 0;
    printf("balanced 2: %d\n", check_balanced(seq, 3));
    // ()() -> 1 0 1 0 -> balanced
    seq[0] = 1; seq[1] = 0; seq[2] = 1; seq[3] = 0;
    printf("balanced 3: %d\n", check_balanced(seq, 4));

    // ---- Two stacks test ----
    stack_init();
    stack2_init();
    stack_push(1);
    stack_push(2);
    stack2_push(10);
    stack2_push(20);
    printf("s1 peek: %d\n", stack_peek());
    printf("s2 peek: %d\n", stack2_peek());
    printf("s1 size: %d\n", stack_size());
    printf("s2 size: %d\n", stack2_size());

    // ---- Postfix evaluation ----
    // 3 4 + = 7
    expr[0] = 3; expr[1] = 4; expr[2] = -1;
    printf("postfix 3 4 +: %d\n", eval_postfix(expr, 3));
    // 2 3 * 5 + = 11
    expr[0] = 2; expr[1] = 3; expr[2] = -2; expr[3] = 5; expr[4] = -1;
    printf("postfix 2 3 * 5 +: %d\n", eval_postfix(expr, 5));
    // 10 3 - = 7
    expr[0] = 10; expr[1] = 3; expr[2] = -3;
    printf("postfix 10 3 -: %d\n", eval_postfix(expr, 3));

    // ---- Priority queue ----
    pq_init();
    pq_enqueue(5, 0);
    pq_enqueue(10, 1);
    pq_enqueue(15, 0);
    pq_enqueue(20, 1);
    printf("pq deq: %d\n", pq_dequeue());
    printf("pq deq: %d\n", pq_dequeue());
    printf("pq deq: %d\n", pq_dequeue());
    printf("pq deq: %d\n", pq_dequeue());

    // ---- Round robin ----
    procs[0] = 3; procs[1] = 2; procs[2] = 1;
    printf("round robin: %d\n", round_robin(procs, 3));

    // ---- More stack tests with many pushes ----
    stack_init();
    for (i = 0; i < 10; i = i + 1) {
        stack_push(i * i);
    }
    printf("stack 10 size: %d\n", stack_size());
    printf("stack top: %d\n", stack_peek());
    // Pop 5
    for (i = 0; i < 5; i = i + 1) {
        r = stack_pop();
    }
    printf("after 5 pops size: %d\n", stack_size());
    printf("after 5 pops top: %d\n", stack_peek());

    // ---- More queue tests ----
    queue_init();
    for (i = 0; i < 10; i = i + 1) {
        queue_enqueue(i + 1);
    }
    printf("queue 10 size: %d\n", queue_size());
    // Dequeue first 5
    for (i = 0; i < 5; i = i + 1) {
        r = queue_dequeue();
    }
    printf("after 5 deq size: %d\n", queue_size());
    printf("after 5 deq front: %d\n", queue_peek());

    return 0;
}
