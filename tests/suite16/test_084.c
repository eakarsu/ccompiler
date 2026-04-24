int printf(const char *fmt, ...);

// Test 084: Command queue with undo/redo stack (LIFO + FIFO)

struct Command {
    int op;      // 1=add, 2=sub, 3=mul
    int operand;
    int prev_val;
};

struct CmdQueue {
    struct Command cmds[8];
    int head;
    int tail;
    int count;
};

struct UndoStack {
    struct Command stack[16];
    int top;
};

void cmdq_init(struct CmdQueue *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

int cmdq_enqueue(struct CmdQueue *q, int op, int operand) {
    if (q->count >= 8) return 0;
    q->cmds[q->tail].op = op;
    q->cmds[q->tail].operand = operand;
    q->cmds[q->tail].prev_val = 0;
    q->tail = (q->tail + 1) % 8;
    q->count = q->count + 1;
    return 1;
}

int cmdq_dequeue(struct CmdQueue *q, struct Command *out) {
    if (q->count <= 0) return 0;
    out->op = q->cmds[q->head].op;
    out->operand = q->cmds[q->head].operand;
    out->prev_val = q->cmds[q->head].prev_val;
    q->head = (q->head + 1) % 8;
    q->count = q->count - 1;
    return 1;
}

void undo_init(struct UndoStack *s) {
    s->top = 0;
}

void undo_push(struct UndoStack *s, struct Command *cmd) {
    if (s->top < 16) {
        s->stack[s->top].op = cmd->op;
        s->stack[s->top].operand = cmd->operand;
        s->stack[s->top].prev_val = cmd->prev_val;
        s->top = s->top + 1;
    }
}

int undo_pop(struct UndoStack *s, struct Command *out) {
    if (s->top <= 0) return 0;
    s->top = s->top - 1;
    out->op = s->stack[s->top].op;
    out->operand = s->stack[s->top].operand;
    out->prev_val = s->stack[s->top].prev_val;
    return 1;
}

int apply_cmd(int current, struct Command *cmd) {
    cmd->prev_val = current;
    if (cmd->op == 1) return current + cmd->operand;
    if (cmd->op == 2) return current - cmd->operand;
    if (cmd->op == 3) return current * cmd->operand;
    return current;
}

int main() {
    struct CmdQueue q;
    struct UndoStack undo;
    struct Command cmd;
    int value;

    cmdq_init(&q);
    undo_init(&undo);
    value = 10;

    // Enqueue commands: +5, -3, *2, +7, -1
    cmdq_enqueue(&q, 1, 5);
    cmdq_enqueue(&q, 2, 3);
    cmdq_enqueue(&q, 3, 2);
    cmdq_enqueue(&q, 1, 7);
    cmdq_enqueue(&q, 2, 1);
    printf("Queued: %d\n", q.count); // EXPECT: Queued: 5

    // Execute all commands one by one
    cmdq_dequeue(&q, &cmd);
    value = apply_cmd(value, &cmd);
    undo_push(&undo, &cmd);
    printf("After op %d(%d): value=%d\n", cmd.op, cmd.operand, value); // EXPECT: After op 1(5): value=15

    cmdq_dequeue(&q, &cmd);
    value = apply_cmd(value, &cmd);
    undo_push(&undo, &cmd);
    printf("After op %d(%d): value=%d\n", cmd.op, cmd.operand, value); // EXPECT: After op 2(3): value=12

    cmdq_dequeue(&q, &cmd);
    value = apply_cmd(value, &cmd);
    undo_push(&undo, &cmd);
    printf("After op %d(%d): value=%d\n", cmd.op, cmd.operand, value); // EXPECT: After op 3(2): value=24

    cmdq_dequeue(&q, &cmd);
    value = apply_cmd(value, &cmd);
    undo_push(&undo, &cmd);
    printf("After op %d(%d): value=%d\n", cmd.op, cmd.operand, value); // EXPECT: After op 1(7): value=31

    cmdq_dequeue(&q, &cmd);
    value = apply_cmd(value, &cmd);
    undo_push(&undo, &cmd);
    printf("After op %d(%d): value=%d\n", cmd.op, cmd.operand, value); // EXPECT: After op 2(1): value=30
    printf("Final value: %d\n", value); // EXPECT: Final value: 30
    printf("Undo stack: %d\n", undo.top); // EXPECT: Undo stack: 5

    // Undo last 2 operations
    undo_pop(&undo, &cmd);
    value = cmd.prev_val;
    printf("Undo: value=%d\n", value); // EXPECT: Undo: value=31

    undo_pop(&undo, &cmd);
    value = cmd.prev_val;
    printf("Undo: value=%d\n", value); // EXPECT: Undo: value=24

    // Enqueue new commands after undo
    cmdq_enqueue(&q, 1, 100);
    cmdq_enqueue(&q, 3, 3);
    while (q.count > 0) {
        cmdq_dequeue(&q, &cmd);
        value = apply_cmd(value, &cmd);
        undo_push(&undo, &cmd);
    }
    // 24+100=124, 124*3=372
    printf("After new cmds: %d\n", value); // EXPECT: After new cmds: 372

    // Undo everything
    while (undo.top > 0) {
        undo_pop(&undo, &cmd);
        value = cmd.prev_val;
    }
    printf("After full undo: %d\n", value); // EXPECT: After full undo: 10

    printf("PASS\n"); // EXPECT: PASS
    return 0;
}
