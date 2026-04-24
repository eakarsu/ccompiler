int printf(const char *fmt, ...);

// Pushdown Automaton (PDA) Simulator
// Simulates a PDA that accepts strings of the form a^n b^n (equal a's and b's)
// Uses a stack to count a's and match with b's

struct Stack {
    char data[50];
    int top;
};

void stack_init(struct Stack *s) {
    s->top = -1;
}

void stack_push(struct Stack *s, char c) {
    s->top = s->top + 1;
    s->data[s->top] = c;
}

char stack_pop(struct Stack *s) {
    if (s->top < 0) return 0;
    char c = s->data[s->top];
    s->top = s->top - 1;
    return c;
}

char stack_peek(struct Stack *s) {
    if (s->top < 0) return 0;
    return s->data[s->top];
}

int stack_empty(struct Stack *s) {
    return s->top < 0;
}

int stack_size(struct Stack *s) {
    return s->top + 1;
}

// PDA states: 0=start/reading a's, 1=reading b's, 2=accept, 3=reject
struct PDA {
    int state;
    struct Stack stack;
};

void pda_init(struct PDA *pda) {
    pda->state = 0;
    stack_init(&pda->stack);
    stack_push(&pda->stack, '$'); // bottom marker
}

void pda_step(struct PDA *pda, char input) {
    if (pda->state == 2 || pda->state == 3) return;

    if (pda->state == 0) {
        if (input == 'a') {
            stack_push(&pda->stack, 'A');
        } else if (input == 'b') {
            // Transition to state 1, try to pop an A
            pda->state = 1;
            if (stack_peek(&pda->stack) == 'A') {
                stack_pop(&pda->stack);
            } else {
                pda->state = 3; // reject: b without matching a
            }
        } else {
            pda->state = 3; // invalid input
        }
    } else if (pda->state == 1) {
        if (input == 'b') {
            if (stack_peek(&pda->stack) == 'A') {
                stack_pop(&pda->stack);
            } else {
                pda->state = 3; // reject: more b's than a's
            }
        } else if (input == 'a') {
            pda->state = 3; // reject: a after b
        } else {
            pda->state = 3;
        }
    }
}

int pda_accept(struct PDA *pda) {
    if (pda->state == 3) return 0;
    // Accept if stack only has bottom marker
    if (stack_peek(&pda->stack) == '$' && stack_size(&pda->stack) == 1) {
        return 1;
    }
    return 0;
}

int run_pda(char *input) {
    struct PDA pda;
    pda_init(&pda);
    int i = 0;
    while (input[i] != 0) {
        pda_step(&pda, input[i]);
        i = i + 1;
    }
    return pda_accept(&pda);
}

int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int main() {
    // Test a^n b^n strings
    // EXPECT: PDA 'ab': accepted=1
    printf("PDA 'ab': accepted=%d\n", run_pda("ab"));
    // EXPECT: PDA 'aabb': accepted=1
    printf("PDA 'aabb': accepted=%d\n", run_pda("aabb"));
    // EXPECT: PDA 'aaabbb': accepted=1
    printf("PDA 'aaabbb': accepted=%d\n", run_pda("aaabbb"));
    // EXPECT: PDA 'aaaabbbb': accepted=1
    printf("PDA 'aaaabbbb': accepted=%d\n", run_pda("aaaabbbb"));

    // Rejections
    // EXPECT: PDA 'aab': accepted=0
    printf("PDA 'aab': accepted=%d\n", run_pda("aab"));
    // EXPECT: PDA 'abb': accepted=0
    printf("PDA 'abb': accepted=%d\n", run_pda("abb"));
    // EXPECT: PDA 'ba': accepted=0
    printf("PDA 'ba': accepted=%d\n", run_pda("ba"));
    // EXPECT: PDA 'abab': accepted=0
    printf("PDA 'abab': accepted=%d\n", run_pda("abab"));
    // EXPECT: PDA 'a': accepted=0
    printf("PDA 'a': accepted=%d\n", run_pda("a"));
    // EXPECT: PDA 'b': accepted=0
    printf("PDA 'b': accepted=%d\n", run_pda("b"));

    // Empty string: should accept (0 a's and 0 b's)
    // EXPECT: PDA '': accepted=1
    printf("PDA '': accepted=%d\n", run_pda(""));

    // Test stack operations directly
    struct Stack s;
    stack_init(&s);
    stack_push(&s, 'X');
    stack_push(&s, 'Y');
    stack_push(&s, 'Z');
    // EXPECT: Stack size: 3
    printf("Stack size: %d\n", stack_size(&s));
    // EXPECT: Stack top: 90
    printf("Stack top: %d\n", (int)stack_peek(&s));
    char p1 = stack_pop(&s);
    char p2 = stack_pop(&s);
    // EXPECT: Popped: 90 89
    printf("Popped: %d %d\n", (int)p1, (int)p2);
    // EXPECT: Stack size after pop: 1
    printf("Stack size after pop: %d\n", stack_size(&s));
    // EXPECT: Stack empty: 0
    printf("Stack empty: %d\n", stack_empty(&s));
    stack_pop(&s);
    // EXPECT: Stack empty after pop all: 1
    printf("Stack empty after pop all: %d\n", stack_empty(&s));

    return 0;
}
