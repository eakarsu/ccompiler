int printf(const char *fmt, ...);

typedef int (*CmdFunc)(int, int);

struct Command {
    CmdFunc execute;
    CmdFunc undo_fn;
    int arg1;
    int arg2;
    int result;
};

int cmd_add(int a, int b) { return a + b; }
int cmd_sub(int a, int b) { return a - b; }
int cmd_mul(int a, int b) { return a * b; }
int cmd_div(int a, int b) { return a / b; }

int undo_add(int a, int b) { return a - b; }
int undo_sub(int a, int b) { return a + b; }
int undo_mul(int a, int b) { return a / b; }
int undo_div(int a, int b) { return a * b; }

struct CommandQueue {
    struct Command cmds[16];
    int count;
};

struct UndoStack {
    struct Command history[16];
    int top;
};

void queue_init(struct CommandQueue *q) {
    q->count = 0;
}

void queue_push(struct CommandQueue *q, CmdFunc exec, CmdFunc undo, int a, int b) {
    struct Command *c;
    c = &q->cmds[q->count];
    c->execute = exec;
    c->undo_fn = undo;
    c->arg1 = a;
    c->arg2 = b;
    c->result = 0;
    q->count = q->count + 1;
}

void undo_init(struct UndoStack *s) {
    s->top = 0;
}

void undo_push(struct UndoStack *s, struct Command *cmd) {
    s->history[s->top] = *cmd;
    s->top = s->top + 1;
}

int undo_pop(struct UndoStack *s) {
    struct Command *c;
    if (s->top <= 0) return -1;
    s->top = s->top - 1;
    c = &s->history[s->top];
    return c->undo_fn(c->result, c->arg2);
}

int execute_all(struct CommandQueue *q, struct UndoStack *s) {
    int i;
    int last;
    last = 0;
    for (i = 0; i < q->count; i++) {
        q->cmds[i].result = q->cmds[i].execute(q->cmds[i].arg1, q->cmds[i].arg2);
        undo_push(s, &q->cmds[i]);
        last = q->cmds[i].result;
    }
    return last;
}

int execute_chained(struct CommandQueue *q, struct UndoStack *s, int init) {
    int i;
    int val;
    val = init;
    for (i = 0; i < q->count; i++) {
        q->cmds[i].arg1 = val;
        q->cmds[i].result = q->cmds[i].execute(val, q->cmds[i].arg2);
        undo_push(s, &q->cmds[i]);
        val = q->cmds[i].result;
    }
    return val;
}

int count_commands(struct CommandQueue *q) {
    return q->count;
}

int sum_results(struct CommandQueue *q) {
    int i;
    int sum;
    sum = 0;
    for (i = 0; i < q->count; i++) {
        sum = sum + q->cmds[i].result;
    }
    return sum;
}

int main(void) {
    struct CommandQueue queue;
    struct UndoStack undo;
    int result;
    int undone;

    queue_init(&queue);
    undo_init(&undo);

    queue_push(&queue, cmd_add, undo_add, 10, 5);
    queue_push(&queue, cmd_mul, undo_mul, 3, 4);
    queue_push(&queue, cmd_sub, undo_sub, 20, 7);

    // EXPECT: queued: 3
    printf("queued: %d\n", count_commands(&queue));

    result = execute_all(&queue, &undo);
    // EXPECT: last result: 13
    printf("last result: %d\n", result);

    // EXPECT: cmd[0] result: 15
    printf("cmd[0] result: %d\n", queue.cmds[0].result);
    // EXPECT: cmd[1] result: 12
    printf("cmd[1] result: %d\n", queue.cmds[1].result);
    // EXPECT: cmd[2] result: 13
    printf("cmd[2] result: %d\n", queue.cmds[2].result);

    // EXPECT: sum results: 40
    printf("sum results: %d\n", sum_results(&queue));

    // EXPECT: undo stack size: 3
    printf("undo stack size: %d\n", undo.top);

    undone = undo_pop(&undo);
    // EXPECT: undo1: 20
    printf("undo1: %d\n", undone);

    undone = undo_pop(&undo);
    // EXPECT: undo2: 3
    printf("undo2: %d\n", undone);

    undone = undo_pop(&undo);
    // EXPECT: undo3: 10
    printf("undo3: %d\n", undone);

    queue_init(&queue);
    undo_init(&undo);
    queue_push(&queue, cmd_add, undo_add, 0, 10);
    queue_push(&queue, cmd_mul, undo_mul, 0, 3);
    queue_push(&queue, cmd_add, undo_add, 0, 5);
    queue_push(&queue, cmd_sub, undo_sub, 0, 2);

    result = execute_chained(&queue, &undo, 1);
    // EXPECT: chained result: 36
    printf("chained result: %d\n", result);

    // EXPECT: chain cmd[0]: 11
    printf("chain cmd[0]: %d\n", queue.cmds[0].result);
    // EXPECT: chain cmd[1]: 33
    printf("chain cmd[1]: %d\n", queue.cmds[1].result);
    // EXPECT: chain cmd[2]: 38
    printf("chain cmd[2]: %d\n", queue.cmds[2].result);
    // EXPECT: chain cmd[3]: 36
    printf("chain cmd[3]: %d\n", queue.cmds[3].result);

    undone = undo_pop(&undo);
    // EXPECT: chain undo: 38
    printf("chain undo: %d\n", undone);

    return 0;
}
