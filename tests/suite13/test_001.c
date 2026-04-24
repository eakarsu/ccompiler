int printf(const char *fmt, ...);

// Stack implementation with push/pop/peek using arrays

struct Stack {
    int data[64];
    int top;
};

void stack_init(struct Stack *s) {
    s->top = -1;
}

int stack_empty(struct Stack *s) {
    return s->top == -1;
}

int stack_size(struct Stack *s) {
    return s->top + 1;
}

int stack_push(struct Stack *s, int val) {
    if (s->top >= 63) return -1;
    s->top = s->top + 1;
    s->data[s->top] = val;
    return 0;
}

int stack_pop(struct Stack *s) {
    if (s->top < 0) return -999;
    int val = s->data[s->top];
    s->top = s->top - 1;
    return val;
}

int stack_peek(struct Stack *s) {
    if (s->top < 0) return -999;
    return s->data[s->top];
}

// Reverse an array using a stack
void reverse_array(int *arr, int n) {
    struct Stack s;
    stack_init(&s);
    int i = 0;
    while (i < n) {
        stack_push(&s, arr[i]);
        i = i + 1;
    }
    i = 0;
    while (i < n) {
        arr[i] = stack_pop(&s);
        i = i + 1;
    }
}

// Check if a sequence is a valid pop sequence given push order 1..n
int is_valid_pop_sequence(int *seq, int n) {
    struct Stack s;
    stack_init(&s);
    int next_push = 1;
    int i = 0;
    while (i < n) {
        while (next_push <= n && (stack_empty(&s) || stack_peek(&s) != seq[i])) {
            stack_push(&s, next_push);
            next_push = next_push + 1;
        }
        if (stack_peek(&s) != seq[i]) return 0;
        stack_pop(&s);
        i = i + 1;
    }
    return 1;
}

// Sort a stack using another stack (ascending from top)
void sort_stack(struct Stack *s) {
    struct Stack tmp;
    stack_init(&tmp);
    while (!stack_empty(s)) {
        int val = stack_pop(s);
        while (!stack_empty(&tmp) && stack_peek(&tmp) > val) {
            stack_push(s, stack_pop(&tmp));
        }
        stack_push(&tmp, val);
    }
    // Move back so smallest is at top
    while (!stack_empty(&tmp)) {
        stack_push(s, stack_pop(&tmp));
    }
}

int main() {
    struct Stack s;
    stack_init(&s);

    // Basic push/pop/peek
    printf("%d\n", stack_empty(&s));
    // EXPECT: 1
    printf("%d\n", stack_size(&s));
    // EXPECT: 0

    stack_push(&s, 10);
    stack_push(&s, 20);
    stack_push(&s, 30);

    printf("%d\n", stack_peek(&s));
    // EXPECT: 30
    printf("%d\n", stack_size(&s));
    // EXPECT: 3
    printf("%d\n", stack_empty(&s));
    // EXPECT: 0

    printf("%d\n", stack_pop(&s));
    // EXPECT: 30
    printf("%d\n", stack_pop(&s));
    // EXPECT: 20
    printf("%d\n", stack_peek(&s));
    // EXPECT: 10
    printf("%d\n", stack_pop(&s));
    // EXPECT: 10
    printf("%d\n", stack_empty(&s));
    // EXPECT: 1

    // Pop from empty
    printf("%d\n", stack_pop(&s));
    // EXPECT: -999

    // Reverse array test
    int arr[6];
    arr[0] = 1; arr[1] = 2; arr[2] = 3;
    arr[3] = 4; arr[4] = 5; arr[5] = 6;
    reverse_array(arr, 6);
    printf("%d %d %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
    // EXPECT: 6 5 4 3 2 1

    // Valid pop sequence: push 1,2,3,4,5 and pop in order 4,5,3,2,1
    int seq1[5];
    seq1[0] = 4; seq1[1] = 5; seq1[2] = 3; seq1[3] = 2; seq1[4] = 1;
    printf("%d\n", is_valid_pop_sequence(seq1, 5));
    // EXPECT: 1

    // Invalid pop sequence: 4,3,5,1,2 is not possible
    int seq2[5];
    seq2[0] = 4; seq2[1] = 3; seq2[2] = 5; seq2[3] = 1; seq2[4] = 2;
    printf("%d\n", is_valid_pop_sequence(seq2, 5));
    // EXPECT: 0

    // Sort stack test
    struct Stack s2;
    stack_init(&s2);
    stack_push(&s2, 5);
    stack_push(&s2, 1);
    stack_push(&s2, 4);
    stack_push(&s2, 2);
    stack_push(&s2, 3);

    sort_stack(&s2);

    // After sorting, smallest on top
    printf("%d\n", stack_pop(&s2));
    // EXPECT: 1
    printf("%d\n", stack_pop(&s2));
    // EXPECT: 2
    printf("%d\n", stack_pop(&s2));
    // EXPECT: 3
    printf("%d\n", stack_pop(&s2));
    // EXPECT: 4
    printf("%d\n", stack_pop(&s2));
    // EXPECT: 5

    // Push many, then pop all
    struct Stack s3;
    stack_init(&s3);
    int j = 0;
    int sum = 0;
    while (j < 20) {
        stack_push(&s3, j * j);
        j = j + 1;
    }
    while (!stack_empty(&s3)) {
        sum = sum + stack_pop(&s3);
    }
    // sum = 0+1+4+9+16+25+36+49+64+81+100+121+144+169+196+225+256+289+324+361 = 2470
    printf("%d\n", sum);
    // EXPECT: 2470

    printf("%s\n", "DONE");
    // EXPECT: DONE

    return 0;
}
