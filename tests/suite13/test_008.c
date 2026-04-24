int printf(const char *fmt, ...);

// Stack-based parenthesis matching and nesting depth analysis

struct Stack {
    int data[128];
    int top;
};

void stack_init(struct Stack *s) {
    s->top = -1;
}

int stack_empty(struct Stack *s) {
    return s->top == -1;
}

void stack_push(struct Stack *s, int val) {
    s->top = s->top + 1;
    s->data[s->top] = val;
}

int stack_pop(struct Stack *s) {
    int val = s->data[s->top];
    s->top = s->top - 1;
    return val;
}

int stack_peek(struct Stack *s) {
    return s->data[s->top];
}

int stack_size(struct Stack *s) {
    return s->top + 1;
}

// Check if parentheses are balanced
// Supports (, ), [, ], {, }
// Returns 1 if balanced, 0 otherwise
int is_balanced(char *s) {
    struct Stack stk;
    stack_init(&stk);
    int i = 0;
    while (s[i] != 0) {
        char c = s[i];
        if (c == 40 || c == 91 || c == 123) {
            // ( [ {
            stack_push(&stk, c);
        } else if (c == 41) {
            // )
            if (stack_empty(&stk) || stack_peek(&stk) != 40) return 0;
            stack_pop(&stk);
        } else if (c == 93) {
            // ]
            if (stack_empty(&stk) || stack_peek(&stk) != 91) return 0;
            stack_pop(&stk);
        } else if (c == 125) {
            // }
            if (stack_empty(&stk) || stack_peek(&stk) != 123) return 0;
            stack_pop(&stk);
        }
        i = i + 1;
    }
    return stack_empty(&stk);
}

// Compute maximum nesting depth of parentheses (only round parens)
int max_depth(char *s) {
    int depth = 0;
    int max_d = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == 40) {
            depth = depth + 1;
            if (depth > max_d) max_d = depth;
        } else if (s[i] == 41) {
            depth = depth - 1;
        }
        i = i + 1;
    }
    return max_d;
}

// Find the length of the longest valid parentheses substring
// Using stack-based approach
int longest_valid_parens(char *s) {
    struct Stack stk;
    stack_init(&stk);
    stack_push(&stk, -1); // sentinel

    int max_len = 0;
    int i = 0;
    while (s[i] != 0) {
        if (s[i] == 40) {
            // (
            stack_push(&stk, i);
        } else if (s[i] == 41) {
            // )
            stack_pop(&stk);
            if (stack_empty(&stk)) {
                stack_push(&stk, i);
            } else {
                int len = i - stack_peek(&stk);
                if (len > max_len) max_len = len;
            }
        }
        i = i + 1;
    }
    return max_len;
}

// Count the number of bracket reversals needed to make string balanced
// String length must be even, only contains ( and )
// Returns -1 if impossible (odd length)
int min_reversals(char *s) {
    int len = 0;
    while (s[len] != 0) len = len + 1;
    if (len % 2 != 0) return -1;

    struct Stack stk;
    stack_init(&stk);

    int i = 0;
    while (s[i] != 0) {
        if (s[i] == 40) {
            stack_push(&stk, s[i]);
        } else {
            // )
            if (!stack_empty(&stk) && stack_peek(&stk) == 40) {
                stack_pop(&stk);
            } else {
                stack_push(&stk, s[i]);
            }
        }
        i = i + 1;
    }

    // Remaining on stack are unmatched brackets
    int open = 0;
    int close = 0;
    while (!stack_empty(&stk)) {
        int c = stack_pop(&stk);
        if (c == 40) open = open + 1;
        else close = close + 1;
    }

    // Need ceil(open/2) + ceil(close/2) reversals
    return (open + 1) / 2 + (close + 1) / 2;
}

// Score nested parentheses: () = 1, (A) = 2*A, AB = A+B
// e.g. "(())" = 2, "((()))" = 4, "()()" = 2, "(()(()))" = 2*(1+2) = 6
int score_parens(char *s) {
    struct Stack stk;
    stack_init(&stk);
    stack_push(&stk, 0); // current score

    int i = 0;
    while (s[i] != 0) {
        if (s[i] == 40) {
            stack_push(&stk, 0);
        } else {
            // )
            int inner = stack_pop(&stk);
            int val;
            if (inner == 0) {
                val = 1;
            } else {
                val = 2 * inner;
            }
            // Add to parent
            int parent = stack_pop(&stk);
            stack_push(&stk, parent + val);
        }
        i = i + 1;
    }
    return stack_pop(&stk);
}

int main() {
    // Balanced tests
    char s1[8];
    s1[0] = '('; s1[1] = ')'; s1[2] = 0;
    printf("%d\n", is_balanced(s1));
    // EXPECT: 1

    char s2[8];
    s2[0] = '('; s2[1] = '['; s2[2] = ']'; s2[3] = ')'; s2[4] = 0;
    printf("%d\n", is_balanced(s2));
    // EXPECT: 1

    char s3[8];
    s3[0] = '('; s3[1] = '['; s3[2] = ')'; s3[3] = ']'; s3[4] = 0;
    printf("%d\n", is_balanced(s3));
    // EXPECT: 0

    char s4[4];
    s4[0] = '('; s4[1] = '('; s4[2] = 0;
    printf("%d\n", is_balanced(s4));
    // EXPECT: 0

    char s5[12];
    s5[0] = '{'; s5[1] = '['; s5[2] = '('; s5[3] = ')';
    s5[4] = ']'; s5[5] = '}'; s5[6] = '('; s5[7] = ')'; s5[8] = 0;
    printf("%d\n", is_balanced(s5));
    // EXPECT: 1

    // Max depth tests
    char d1[8];
    d1[0] = '('; d1[1] = '('; d1[2] = ')'; d1[3] = ')'; d1[4] = 0;
    printf("%d\n", max_depth(d1));
    // EXPECT: 2

    char d2[12];
    d2[0] = '('; d2[1] = '('; d2[2] = '('; d2[3] = ')';
    d2[4] = ')'; d2[5] = ')'; d2[6] = 0;
    printf("%d\n", max_depth(d2));
    // EXPECT: 3

    char d3[12];
    d3[0] = '('; d3[1] = ')'; d3[2] = '('; d3[3] = '(';
    d3[4] = ')'; d3[5] = ')'; d3[6] = 0;
    printf("%d\n", max_depth(d3));
    // EXPECT: 2

    // Longest valid parens
    // "(()" => 2
    char lv1[8];
    lv1[0] = '('; lv1[1] = '('; lv1[2] = ')'; lv1[3] = 0;
    printf("%d\n", longest_valid_parens(lv1));
    // EXPECT: 2

    // ")()())" => 4
    char lv2[8];
    lv2[0] = ')'; lv2[1] = '('; lv2[2] = ')'; lv2[3] = '(';
    lv2[4] = ')'; lv2[5] = ')'; lv2[6] = 0;
    printf("%d\n", longest_valid_parens(lv2));
    // EXPECT: 4

    // "()()" => 4
    char lv3[8];
    lv3[0] = '('; lv3[1] = ')'; lv3[2] = '('; lv3[3] = ')'; lv3[4] = 0;
    printf("%d\n", longest_valid_parens(lv3));
    // EXPECT: 4

    // Min reversals
    // ")(" => 2 reversals (change both)
    char mr1[4];
    mr1[0] = ')'; mr1[1] = '('; mr1[2] = 0;
    printf("%d\n", min_reversals(mr1));
    // EXPECT: 2

    // ")))(" => unmatched: 3 close, 1 open => ceil(3/2)+ceil(1/2) = 2+1 = 3
    // Wait: after cancellation: )))( -> stack: ) ) ) (, no cancellation
    // Actually: process char by char:
    //   ')': stack empty, push ')'. Stack: [)]
    //   ')': stack top is ')', not '(', push ')'. Stack: [), )]
    //   ')': stack top is ')', push ')'. Stack: [), ), )]
    //   '(': push '('. Stack: [), ), ), (]
    // open=1, close=3
    // ceil(1/2) + ceil(3/2) = 1 + 2 = 3
    char mr2[6];
    mr2[0] = ')'; mr2[1] = ')'; mr2[2] = ')'; mr2[3] = '('; mr2[4] = 0;
    printf("%d\n", min_reversals(mr2));
    // EXPECT: 3

    // "())(" => after cancellation: ) ( => open=1, close=1 => 1+1 = 2
    // Process: '(' push. ')' matches, pop. ')' empty, push ')'. '(' push '('.
    // Stack: [), (]. open=1, close=1. ceil(1/2)+ceil(1/2)=1+1=2
    char mr3[6];
    mr3[0] = '('; mr3[1] = ')'; mr3[2] = ')'; mr3[3] = '('; mr3[4] = 0;
    printf("%d\n", min_reversals(mr3));
    // EXPECT: 2

    // Odd length => -1
    char mr4[4];
    mr4[0] = '('; mr4[1] = ')'; mr4[2] = '('; mr4[3] = 0;
    printf("%d\n", min_reversals(mr4));
    // EXPECT: -1

    // Score parens
    // "()" => 1
    printf("%d\n", score_parens(s1));
    // EXPECT: 1

    // "(())" => 2
    printf("%d\n", score_parens(d1));
    // EXPECT: 2

    // "()()" => 2
    printf("%d\n", score_parens(lv3));
    // EXPECT: 2

    // "(()(()))" => 2*(1 + 2*1) = 2*3 = 6
    char sc1[10];
    sc1[0] = '('; sc1[1] = '('; sc1[2] = ')'; sc1[3] = '(';
    sc1[4] = '('; sc1[5] = ')'; sc1[6] = ')'; sc1[7] = ')'; sc1[8] = 0;
    printf("%d\n", score_parens(sc1));
    // EXPECT: 6

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
