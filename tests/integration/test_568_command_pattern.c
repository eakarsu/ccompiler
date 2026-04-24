int printf(const char *fmt, ...);
// EXPECT: Initial state: 0\nExecute 'add(10)': state=10\nExecute 'add(5)': state=15\nExecute 'mul(3)': state=45\nExecute 'add(7)': state=52\nCurrent state: 52\nHistory size: 4\nUndo 'add(7)': state=45\nUndo 'mul(3)': state=15\nAfter 2 undos: 15\nRedo 'mul(3)': state=45\nAfter redo: 45\nExecute 'add(100)': state=145\nNew branch state: 145\nHistory size: 4

#define MAX_HISTORY 20

typedef void (*CommandFn)(int *state, int arg);

struct Command {
    CommandFn execute;
    CommandFn undo;
    int arg;
    char *name;
};

struct CommandHistory {
    struct Command commands[MAX_HISTORY];
    int count;
    int current;
};

void hist_init(struct CommandHistory *h) {
    h->count = 0;
    h->current = -1;
}

void hist_execute(struct CommandHistory *h, int *state, struct Command cmd) {
    h->current++;
    h->commands[h->current] = cmd;
    h->count = h->current + 1;
    cmd.execute(state, cmd.arg);
    printf("Execute '%s(%d)': state=%d\n", cmd.name, cmd.arg, *state);
}

int hist_undo(struct CommandHistory *h, int *state) {
    if (h->current < 0) return -1;
    struct Command cmd = h->commands[h->current];
    cmd.undo(state, cmd.arg);
    printf("Undo '%s(%d)': state=%d\n", cmd.name, cmd.arg, *state);
    h->current--;
    return 0;
}

int hist_redo(struct CommandHistory *h, int *state) {
    if (h->current >= h->count - 1) return -1;
    h->current++;
    struct Command cmd = h->commands[h->current];
    cmd.execute(state, cmd.arg);
    printf("Redo '%s(%d)': state=%d\n", cmd.name, cmd.arg, *state);
    return 0;
}

void do_add(int *state, int arg) { *state = *state + arg; }
void undo_add(int *state, int arg) { *state = *state - arg; }

void do_mul(int *state, int arg) { *state = *state * arg; }
void undo_mul(int *state, int arg) { *state = *state / arg; }

void do_set(int *state, int arg) { *state = arg; }
void undo_set(int *state, int arg) { *state = arg; }

struct Command make_add(int val) {
    struct Command c;
    c.execute = do_add;
    c.undo = undo_add;
    c.arg = val;
    c.name = "add";
    return c;
}

struct Command make_mul(int val) {
    struct Command c;
    c.execute = do_mul;
    c.undo = undo_mul;
    c.arg = val;
    c.name = "mul";
    return c;
}

int main(void) {
    struct CommandHistory hist;
    hist_init(&hist);
    int state = 0;
    printf("Initial state: %d\n", state);

    hist_execute(&hist, &state, make_add(10));
    hist_execute(&hist, &state, make_add(5));
    hist_execute(&hist, &state, make_mul(3));
    hist_execute(&hist, &state, make_add(7));

    printf("Current state: %d\n", state);
    printf("History size: %d\n", hist.count);

    hist_undo(&hist, &state);
    hist_undo(&hist, &state);
    printf("After 2 undos: %d\n", state);

    hist_redo(&hist, &state);
    printf("After redo: %d\n", state);

    hist_execute(&hist, &state, make_add(100));
    printf("New branch state: %d\n", state);
    printf("History size: %d\n", hist.count);

    return 0;
}
