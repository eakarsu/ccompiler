int printf(const char *fmt, ...);

/* Stress combination test: structs, arrays, pointers, function pointers,
   enums, macros, control flow in realistic data-processing patterns.
   Implements: a small expression evaluator, a sorting routine,
   a statistics calculator, and a simple state machine. */

/* ========================= MACROS ========================= */
#define MAX_TOKENS  16
#define MAX_STACK   16
#define ARRAY_LEN(a) ((int)(sizeof(a) / sizeof((a)[0])))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define ABS(x)   ((x) < 0 ? -(x) : (x))
#define CLAMP(x,lo,hi) (MIN(MAX((x),(lo)),(hi)))

/* ========================= ENUMS ========================= */
typedef enum {
    TOK_NUM = 0,
    TOK_PLUS = 1,
    TOK_MINUS = 2,
    TOK_MUL = 3,
    TOK_DIV = 4,
    TOK_END = 5
} TokenType;

typedef enum {
    STATE_IDLE = 0,
    STATE_RUNNING = 1,
    STATE_PAUSED = 2,
    STATE_DONE = 3
} MachineState;

typedef enum {
    EVT_START = 0,
    EVT_PAUSE = 1,
    EVT_RESUME = 2,
    EVT_STOP = 3
} Event;

/* ========================= STRUCTS ========================= */
typedef struct {
    TokenType type;
    int value;
} Token;

typedef struct {
    int data[MAX_STACK];
    int top;
} Stack;

typedef struct {
    int items[32];
    int count;
} IntList;

typedef struct {
    int min_val;
    int max_val;
    int sum;
    int count;
} Stats;

typedef struct {
    MachineState state;
    int counter;
    const char *name;
} Machine;

/* ========================= STACK OPS ========================= */
void stack_init(Stack *s) { s->top = 0; }

void stack_push(Stack *s, int v) {
    if (s->top < MAX_STACK) s->data[s->top++] = v;
}

int stack_pop(Stack *s) {
    if (s->top > 0) return s->data[--s->top];
    return 0;
}

int stack_peek(Stack *s) {
    if (s->top > 0) return s->data[s->top - 1];
    return 0;
}

int stack_empty(Stack *s) { return s->top == 0; }

/* ========================= EXPRESSION EVALUATOR ========================= */
/* Evaluate a simple postfix (RPN) expression from a token array */
int eval_rpn(Token *tokens, int n) {
    Stack s;
    stack_init(&s);
    int i;
    for (i = 0; i < n; i++) {
        Token t = tokens[i];
        if (t.type == TOK_NUM) {
            stack_push(&s, t.value);
        } else if (t.type == TOK_PLUS) {
            int b = stack_pop(&s);
            int a = stack_pop(&s);
            stack_push(&s, a + b);
        } else if (t.type == TOK_MINUS) {
            int b = stack_pop(&s);
            int a = stack_pop(&s);
            stack_push(&s, a - b);
        } else if (t.type == TOK_MUL) {
            int b = stack_pop(&s);
            int a = stack_pop(&s);
            stack_push(&s, a * b);
        } else if (t.type == TOK_DIV) {
            int b = stack_pop(&s);
            int a = stack_pop(&s);
            stack_push(&s, (b != 0) ? a / b : 0);
        }
    }
    return stack_pop(&s);
}

/* ========================= SORTING (insertion sort) ========================= */
void insertion_sort(int *arr, int n) {
    int i, j;
    for (i = 1; i < n; i++) {
        int key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

/* Insertion sort descending */
void insertion_sort_desc(int *arr, int n) {
    int i, j;
    for (i = 1; i < n; i++) {
        int key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] < key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

/* ========================= STATISTICS ========================= */
Stats compute_stats(int *arr, int n) {
    Stats st;
    if (n == 0) {
        st.min_val = 0; st.max_val = 0;
        st.sum = 0; st.count = 0;
        return st;
    }
    st.min_val = arr[0];
    st.max_val = arr[0];
    st.sum = 0;
    st.count = n;
    int i;
    for (i = 0; i < n; i++) {
        if (arr[i] < st.min_val) st.min_val = arr[i];
        if (arr[i] > st.max_val) st.max_val = arr[i];
        st.sum += arr[i];
    }
    return st;
}

/* ========================= STATE MACHINE ========================= */
MachineState transition(MachineState cur, Event evt) {
    if (cur == STATE_IDLE    && evt == EVT_START)  return STATE_RUNNING;
    if (cur == STATE_RUNNING && evt == EVT_PAUSE)  return STATE_PAUSED;
    if (cur == STATE_PAUSED  && evt == EVT_RESUME) return STATE_RUNNING;
    if (cur == STATE_RUNNING && evt == EVT_STOP)   return STATE_DONE;
    if (cur == STATE_PAUSED  && evt == EVT_STOP)   return STATE_DONE;
    return cur; /* invalid transition: no change */
}

void run_machine(Machine *m, Event evt) {
    MachineState next = transition(m->state, evt);
    if (next == STATE_RUNNING && m->state != STATE_RUNNING) {
        m->counter++;
    }
    m->state = next;
}

/* ========================= FUNCTION POINTER PIPELINE ========================= */
typedef int (*IntFunc)(int);

int pipeline_apply(int val, IntFunc *fns, int nfns) {
    int i;
    for (i = 0; i < nfns; i++) val = fns[i](val);
    return val;
}

int fn_double(int x)  { return x * 2; }
int fn_inc(int x)     { return x + 1; }
int fn_square(int x)  { return x * x; }
int fn_negate(int x)  { return -x; }
int fn_abs(int x)     { return ABS(x); }
int fn_clamp10(int x) { return CLAMP(x, 0, 10); }

/* ========================= MAIN ========================= */
int main(void) {
    int i;

    /* ===== RPN EVALUATOR TESTS ===== */

    /* "3 4 +" = 7 */
    Token rpn1[3] = {
        {TOK_NUM, 3}, {TOK_NUM, 4}, {TOK_PLUS, 0}
    };
    printf("%d\n", eval_rpn(rpn1, 3));          // EXPECT: 7

    /* "5 2 *" = 10 */
    Token rpn2[3] = {
        {TOK_NUM, 5}, {TOK_NUM, 2}, {TOK_MUL, 0}
    };
    printf("%d\n", eval_rpn(rpn2, 3));          // EXPECT: 10

    /* "10 3 -" = 7 */
    Token rpn3[3] = {
        {TOK_NUM, 10}, {TOK_NUM, 3}, {TOK_MINUS, 0}
    };
    printf("%d\n", eval_rpn(rpn3, 3));          // EXPECT: 7

    /* "20 4 /" = 5 */
    Token rpn4[3] = {
        {TOK_NUM, 20}, {TOK_NUM, 4}, {TOK_DIV, 0}
    };
    printf("%d\n", eval_rpn(rpn4, 3));          // EXPECT: 5

    /* "3 4 + 2 *" = (3+4)*2 = 14 */
    Token rpn5[5] = {
        {TOK_NUM, 3}, {TOK_NUM, 4}, {TOK_PLUS, 0},
        {TOK_NUM, 2}, {TOK_MUL, 0}
    };
    printf("%d\n", eval_rpn(rpn5, 5));          // EXPECT: 14

    /* "5 1 2 + 4 * + 3 -" = 5+((1+2)*4)-3 = 5+12-3 = 14 */
    Token rpn6[9] = {
        {TOK_NUM, 5},  {TOK_NUM, 1}, {TOK_NUM, 2},
        {TOK_PLUS, 0}, {TOK_NUM, 4}, {TOK_MUL, 0},
        {TOK_PLUS, 0}, {TOK_NUM, 3}, {TOK_MINUS, 0}
    };
    printf("%d\n", eval_rpn(rpn6, 9));          // EXPECT: 14

    /* ===== SORTING TESTS ===== */

    int arr1[8] = {5, 3, 8, 1, 9, 2, 7, 4};
    insertion_sort(arr1, 8);
    /* sorted: 1,2,3,4,5,7,8,9 */
    printf("%d\n", arr1[0]);                    // EXPECT: 1
    printf("%d\n", arr1[3]);                    // EXPECT: 4
    printf("%d\n", arr1[7]);                    // EXPECT: 9

    int arr2[5] = {-3, 10, -1, 7, 0};
    insertion_sort(arr2, 5);
    /* sorted: -3,-1,0,7,10 */
    printf("%d\n", arr2[0]);                    // EXPECT: -3
    printf("%d\n", arr2[2]);                    // EXPECT: 0
    printf("%d\n", arr2[4]);                    // EXPECT: 10

    int arr3[6] = {6, 5, 4, 3, 2, 1};
    insertion_sort_desc(arr3, 6);
    /* sorted desc: 6,5,4,3,2,1 */
    printf("%d\n", arr3[0]);                    // EXPECT: 6
    printf("%d\n", arr3[5]);                    // EXPECT: 1

    /* ===== STATISTICS TESTS ===== */

    int data1[6] = {4, 7, 2, 9, 1, 5};
    Stats st1 = compute_stats(data1, 6);
    printf("%d\n", st1.min_val);                // EXPECT: 1
    printf("%d\n", st1.max_val);                // EXPECT: 9
    printf("%d\n", st1.sum);                    // EXPECT: 28
    printf("%d\n", st1.count);                  // EXPECT: 6

    int data2[4] = {-5, 0, 5, 10};
    Stats st2 = compute_stats(data2, 4);
    printf("%d\n", st2.min_val);                // EXPECT: -5
    printf("%d\n", st2.max_val);                // EXPECT: 10
    printf("%d\n", st2.sum);                    // EXPECT: 10

    /* ===== STATE MACHINE TESTS ===== */

    Machine m = {.state = STATE_IDLE, .counter = 0, .name = "test"};
    printf("%d\n", m.state);                    // EXPECT: 0

    run_machine(&m, EVT_START);
    printf("%d\n", m.state);                    // EXPECT: 1
    printf("%d\n", m.counter);                  // EXPECT: 1

    run_machine(&m, EVT_PAUSE);
    printf("%d\n", m.state);                    // EXPECT: 2

    run_machine(&m, EVT_RESUME);
    printf("%d\n", m.state);                    // EXPECT: 1
    printf("%d\n", m.counter);                  // EXPECT: 2

    run_machine(&m, EVT_STOP);
    printf("%d\n", m.state);                    // EXPECT: 3

    /* Invalid transition: DONE + START => no change */
    run_machine(&m, EVT_START);
    printf("%d\n", m.state);                    // EXPECT: 3

    /* ===== PIPELINE TESTS ===== */

    /* double -> inc: 5 -> 10 -> 11 */
    IntFunc p1[2] = {fn_double, fn_inc};
    printf("%d\n", pipeline_apply(5, p1, 2));   // EXPECT: 11

    /* inc -> square -> negate: 3 -> 4 -> 16 -> -16 */
    IntFunc p2[3] = {fn_inc, fn_square, fn_negate};
    printf("%d\n", pipeline_apply(3, p2, 3));   // EXPECT: -16

    /* negate -> abs -> clamp10: -7 -> 7 -> 7 -> 7 */
    IntFunc p3[3] = {fn_negate, fn_abs, fn_clamp10};
    printf("%d\n", pipeline_apply(-7, p3, 3));  // EXPECT: 7

    /* double -> double -> double: 2 -> 4 -> 8 -> 16 */
    IntFunc p4[3] = {fn_double, fn_double, fn_double};
    printf("%d\n", pipeline_apply(2, p4, 3));   // EXPECT: 16

    /* clamp10 on large value: 100 -> 10 */
    IntFunc p5[1] = {fn_clamp10};
    printf("%d\n", pipeline_apply(100, p5, 1)); // EXPECT: 10

    /* ===== MACRO TESTS ===== */

    printf("%d\n", MIN(3, 7));                  // EXPECT: 3
    printf("%d\n", MAX(3, 7));                  // EXPECT: 7
    printf("%d\n", ABS(-42));                   // EXPECT: 42
    printf("%d\n", CLAMP(15, 0, 10));           // EXPECT: 10
    printf("%d\n", CLAMP(-5, 0, 10));           // EXPECT: 0
    printf("%d\n", CLAMP(5, 0, 10));            // EXPECT: 5

    /* ===== COMBINED: sort then stats ===== */

    int comb[8] = {8, 3, 6, 1, 9, 2, 7, 4};
    insertion_sort(comb, 8);
    /* sorted: 1,2,3,4,6,7,8,9 */
    Stats sc = compute_stats(comb, 8);
    printf("%d\n", sc.min_val);                 // EXPECT: 1
    printf("%d\n", sc.max_val);                 // EXPECT: 9
    printf("%d\n", sc.sum);                     // EXPECT: 40

    /* sorted first element */
    printf("%d\n", comb[0]);                    // EXPECT: 1
    /* sorted last element */
    printf("%d\n", comb[7]);                    // EXPECT: 9

    /* ===== ARRAY_LEN macro ===== */
    int testlen[12];
    printf("%d\n", ARRAY_LEN(testlen));         // EXPECT: 12

    /* ===== Stack used independently ===== */
    Stack stk;
    stack_init(&stk);
    for (i = 1; i <= 5; i++) stack_push(&stk, i * 10);
    /* stack: 10,20,30,40,50; top=5 */
    printf("%d\n", stack_peek(&stk));           // EXPECT: 50
    printf("%d\n", stack_pop(&stk));            // EXPECT: 50
    printf("%d\n", stack_pop(&stk));            // EXPECT: 40
    printf("%d\n", stk.top);                    // EXPECT: 3

    int pop_sum = 0;
    while (!stack_empty(&stk)) pop_sum += stack_pop(&stk);
    /* 10+20+30 = 60 */
    printf("%d\n", pop_sum);                    // EXPECT: 60
    printf("%d\n", stack_empty(&stk));          // EXPECT: 1

    return 0;
}
