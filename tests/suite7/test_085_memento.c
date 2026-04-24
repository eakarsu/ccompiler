int printf(const char *fmt, ...);

struct EditorState {
    int cursor_x;
    int cursor_y;
    int selection_start;
    int selection_end;
    int modified;
};

struct Memento {
    struct EditorState saved;
    int valid;
};

struct History {
    struct Memento snapshots[16];
    int count;
    int current;
};

void editor_init(struct EditorState *e) {
    e->cursor_x = 0;
    e->cursor_y = 0;
    e->selection_start = 0;
    e->selection_end = 0;
    e->modified = 0;
}

void history_init(struct History *h) {
    int i;
    h->count = 0;
    h->current = -1;
    for (i = 0; i < 16; i++) {
        h->snapshots[i].valid = 0;
    }
}

void save_state(struct History *h, struct EditorState *e) {
    h->current = h->current + 1;
    h->snapshots[h->current].saved = *e;
    h->snapshots[h->current].valid = 1;
    h->count = h->current + 1;
}

int undo(struct History *h, struct EditorState *e) {
    if (h->current < 0) return 0;
    *e = h->snapshots[h->current].saved;
    h->current = h->current - 1;
    return 1;
}

int redo(struct History *h, struct EditorState *e) {
    if (h->current + 1 >= h->count) return 0;
    h->current = h->current + 1;
    *e = h->snapshots[h->current].saved;
    return 1;
}

void move_cursor(struct EditorState *e, int dx, int dy) {
    e->cursor_x = e->cursor_x + dx;
    e->cursor_y = e->cursor_y + dy;
    e->modified = 1;
}

void set_selection(struct EditorState *e, int start, int end) {
    e->selection_start = start;
    e->selection_end = end;
    e->modified = 1;
}

void clear_selection(struct EditorState *e) {
    e->selection_start = 0;
    e->selection_end = 0;
}

int has_selection(struct EditorState *e) {
    return e->selection_start != e->selection_end;
}

int selection_length(struct EditorState *e) {
    int len;
    len = e->selection_end - e->selection_start;
    if (len < 0) len = -len;
    return len;
}

int history_size(struct History *h) {
    return h->count;
}

int can_undo(struct History *h) {
    return h->current >= 0;
}

int can_redo(struct History *h) {
    return h->current + 1 < h->count;
}

int main(void) {
    struct EditorState editor;
    struct History hist;
    int ok;

    editor_init(&editor);
    history_init(&hist);

    // EXPECT: initial: x=0 y=0
    printf("initial: x=%d y=%d\n", editor.cursor_x, editor.cursor_y);

    save_state(&hist, &editor);
    move_cursor(&editor, 5, 3);
    // EXPECT: moved: x=5 y=3
    printf("moved: x=%d y=%d\n", editor.cursor_x, editor.cursor_y);

    save_state(&hist, &editor);
    set_selection(&editor, 10, 25);
    // EXPECT: selection: 10-25
    printf("selection: %d-%d\n", editor.selection_start, editor.selection_end);
    // EXPECT: sel length: 15
    printf("sel length: %d\n", selection_length(&editor));

    save_state(&hist, &editor);
    move_cursor(&editor, 3, 2);
    // EXPECT: moved again: x=8 y=5
    printf("moved again: x=%d y=%d\n", editor.cursor_x, editor.cursor_y);

    save_state(&hist, &editor);
    clear_selection(&editor);
    // EXPECT: has selection: 0
    printf("has selection: %d\n", has_selection(&editor));

    // EXPECT: history size: 4
    printf("history size: %d\n", history_size(&hist));
    // EXPECT: can undo: 1
    printf("can undo: %d\n", can_undo(&hist));

    ok = undo(&hist, &editor);
    // EXPECT: undo1 ok: 1
    printf("undo1 ok: %d\n", ok);
    // EXPECT: undo1: x=8 y=5
    printf("undo1: x=%d y=%d\n", editor.cursor_x, editor.cursor_y);

    ok = undo(&hist, &editor);
    // EXPECT: undo2 ok: 1
    printf("undo2 ok: %d\n", ok);
    // EXPECT: undo2: x=5 y=3 sel=10-25
    printf("undo2: x=%d y=%d sel=%d-%d\n", editor.cursor_x, editor.cursor_y, editor.selection_start, editor.selection_end);

    ok = undo(&hist, &editor);
    // EXPECT: undo3 ok: 1
    printf("undo3 ok: %d\n", ok);
    // EXPECT: undo3: x=5 y=3
    printf("undo3: x=%d y=%d\n", editor.cursor_x, editor.cursor_y);

    ok = undo(&hist, &editor);
    // EXPECT: undo4 ok: 1
    printf("undo4 ok: %d\n", ok);
    // EXPECT: undo4: x=0 y=0
    printf("undo4: x=%d y=%d\n", editor.cursor_x, editor.cursor_y);

    ok = undo(&hist, &editor);
    // EXPECT: undo5 (empty): 0
    printf("undo5 (empty): %d\n", ok);

    ok = redo(&hist, &editor);
    // EXPECT: redo1 ok: 1
    printf("redo1 ok: %d\n", ok);
    // EXPECT: redo1: x=0 y=0
    printf("redo1: x=%d y=%d\n", editor.cursor_x, editor.cursor_y);

    ok = redo(&hist, &editor);
    // EXPECT: redo2 ok: 1
    printf("redo2 ok: %d\n", ok);
    // EXPECT: redo2: x=5 y=3
    printf("redo2: x=%d y=%d\n", editor.cursor_x, editor.cursor_y);

    return 0;
}
