int printf(const char *fmt, ...);

// Command Pattern: undo/redo with command history

enum CmdType { CMD_ADD, CMD_SUB, CMD_MUL, CMD_SET };

struct Command {
    enum CmdType type;
    int operand;
    int prev_value;
    int active;
};

struct Calculator {
    int value;
    struct Command history[16];
    int hist_count;
    int undo_count;
};

void calc_init(struct Calculator *c) {
    c->value = 0;
    c->hist_count = 0;
    c->undo_count = 0;
    int i;
    for (i = 0; i < 16; i = i + 1) {
        c->history[i].active = 0;
    }
}

void calc_execute(struct Calculator *c, enum CmdType type, int operand) {
    struct Command cmd;
    cmd.type = type;
    cmd.operand = operand;
    cmd.prev_value = c->value;
    cmd.active = 1;

    if (type == CMD_ADD) {
        c->value = c->value + operand;
    } else if (type == CMD_SUB) {
        c->value = c->value - operand;
    } else if (type == CMD_MUL) {
        c->value = c->value * operand;
    } else if (type == CMD_SET) {
        c->value = operand;
    }

    c->history[c->hist_count] = cmd;
    c->hist_count = c->hist_count + 1;
    c->undo_count = 0;
}

int calc_undo(struct Calculator *c) {
    if (c->hist_count <= 0) return 0;
    c->hist_count = c->hist_count - 1;
    c->undo_count = c->undo_count + 1;
    struct Command *cmd;
    cmd = &c->history[c->hist_count];
    c->value = cmd->prev_value;
    return 1;
}

int calc_redo(struct Calculator *c) {
    if (c->undo_count <= 0) return 0;
    int idx;
    idx = c->hist_count;
    struct Command *cmd;
    cmd = &c->history[idx];

    if (cmd->type == CMD_ADD) {
        c->value = c->value + cmd->operand;
    } else if (cmd->type == CMD_SUB) {
        c->value = c->value - cmd->operand;
    } else if (cmd->type == CMD_MUL) {
        c->value = c->value * cmd->operand;
    } else if (cmd->type == CMD_SET) {
        c->value = cmd->operand;
    }

    c->hist_count = c->hist_count + 1;
    c->undo_count = c->undo_count - 1;
    return 1;
}

char *cmd_name(enum CmdType t) {
    if (t == CMD_ADD) return "ADD";
    if (t == CMD_SUB) return "SUB";
    if (t == CMD_MUL) return "MUL";
    return "SET";
}

int main() {
    struct Calculator calc;
    calc_init(&calc);

    printf("Initial: %d\n", calc.value); // EXPECT: Initial: 0

    calc_execute(&calc, CMD_SET, 10);
    printf("Set 10: %d\n", calc.value); // EXPECT: Set 10: 10

    calc_execute(&calc, CMD_ADD, 5);
    printf("Add 5: %d\n", calc.value); // EXPECT: Add 5: 15

    calc_execute(&calc, CMD_MUL, 3);
    printf("Mul 3: %d\n", calc.value); // EXPECT: Mul 3: 45

    calc_execute(&calc, CMD_SUB, 20);
    printf("Sub 20: %d\n", calc.value); // EXPECT: Sub 20: 25

    printf("History: %d commands\n", calc.hist_count); // EXPECT: History: 4 commands

    int r;
    r = calc_undo(&calc);
    printf("Undo: %d, val: %d\n", r, calc.value); // EXPECT: Undo: 1, val: 45

    r = calc_undo(&calc);
    printf("Undo: %d, val: %d\n", r, calc.value); // EXPECT: Undo: 1, val: 15

    r = calc_redo(&calc);
    printf("Redo: %d, val: %d\n", r, calc.value); // EXPECT: Redo: 1, val: 45

    r = calc_redo(&calc);
    printf("Redo: %d, val: %d\n", r, calc.value); // EXPECT: Redo: 1, val: 25

    r = calc_redo(&calc);
    printf("Redo (none): %d, val: %d\n", r, calc.value); // EXPECT: Redo (none): 0, val: 25

    r = calc_undo(&calc);
    r = calc_undo(&calc);
    r = calc_undo(&calc);
    printf("After 3 undos: %d\n", calc.value); // EXPECT: After 3 undos: 10

    calc_execute(&calc, CMD_ADD, 100);
    printf("New branch: %d\n", calc.value); // EXPECT: New branch: 110

    r = calc_redo(&calc);
    printf("Redo after new: %d\n", r); // EXPECT: Redo after new: 0

    printf("Final: %d\n", calc.value); // EXPECT: Final: 110
    printf("Done\n"); // EXPECT: Done
    return 0;
}
