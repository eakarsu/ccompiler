int printf(const char *fmt, ...);

// Symbol table with scope management
// Implements a simple scoped symbol table using a stack of scopes

enum SymKind {
    SYM_VAR,
    SYM_FUNC,
    SYM_CONST
};

struct Symbol {
    char name[16];
    int kind;
    int value;
    int scope_level;
    int active;     // 1 if in scope, 0 if popped
};

struct SymTable {
    struct Symbol syms[64];
    int count;
    int current_scope;
};

void symtable_init(struct SymTable *st) {
    st->count = 0;
    st->current_scope = 0;
}

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0 && i < 15) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

void enter_scope(struct SymTable *st) {
    st->current_scope = st->current_scope + 1;
}

void leave_scope(struct SymTable *st) {
    // Deactivate all symbols at current scope level
    int i;
    for (i = 0; i < st->count; i++) {
        if (st->syms[i].scope_level == st->current_scope && st->syms[i].active) {
            st->syms[i].active = 0;
        }
    }
    st->current_scope = st->current_scope - 1;
}

int add_symbol(struct SymTable *st, char *name, int kind, int value) {
    // Check for duplicate in current scope
    int i;
    for (i = st->count - 1; i >= 0; i--) {
        if (st->syms[i].active && st->syms[i].scope_level == st->current_scope
            && str_eq(st->syms[i].name, name)) {
            return -1; // duplicate
        }
    }
    int idx = st->count;
    str_copy(st->syms[idx].name, name);
    st->syms[idx].kind = kind;
    st->syms[idx].value = value;
    st->syms[idx].scope_level = st->current_scope;
    st->syms[idx].active = 1;
    st->count = st->count + 1;
    return idx;
}

int lookup(struct SymTable *st, char *name) {
    // Search from newest to oldest for innermost scope
    int i;
    for (i = st->count - 1; i >= 0; i--) {
        if (st->syms[i].active && str_eq(st->syms[i].name, name)) {
            return i;
        }
    }
    return -1;
}

int get_value(struct SymTable *st, char *name) {
    int idx = lookup(st, name);
    if (idx < 0) return -9999;
    return st->syms[idx].value;
}

void set_value(struct SymTable *st, char *name, int val) {
    int idx = lookup(st, name);
    if (idx >= 0) {
        st->syms[idx].value = val;
    }
}

char *kind_str(int kind) {
    if (kind == SYM_VAR) return "VAR";
    if (kind == SYM_FUNC) return "FUNC";
    if (kind == SYM_CONST) return "CONST";
    return "???";
}

int count_active(struct SymTable *st) {
    int c = 0;
    int i;
    for (i = 0; i < st->count; i++) {
        if (st->syms[i].active) c = c + 1;
    }
    return c;
}

int main() {
    struct SymTable st;
    symtable_init(&st);

    printf("=== SymTable Test 1: Global scope ===\n");
    // EXPECT: === SymTable Test 1: Global scope ===
    add_symbol(&st, "x", SYM_VAR, 10);
    add_symbol(&st, "y", SYM_VAR, 20);
    add_symbol(&st, "MAX", SYM_CONST, 100);

    printf("x = %d\n", get_value(&st, "x"));
    // EXPECT: x = 10
    printf("y = %d\n", get_value(&st, "y"));
    // EXPECT: y = 20
    printf("MAX = %d\n", get_value(&st, "MAX"));
    // EXPECT: MAX = 100
    printf("active: %d\n", count_active(&st));
    // EXPECT: active: 3
    printf("scope: %d\n", st.current_scope);
    // EXPECT: scope: 0

    printf("=== SymTable Test 2: Enter inner scope ===\n");
    // EXPECT: === SymTable Test 2: Enter inner scope ===
    enter_scope(&st);
    add_symbol(&st, "x", SYM_VAR, 99);  // shadows outer x
    add_symbol(&st, "z", SYM_VAR, 42);

    printf("x = %d\n", get_value(&st, "x"));
    // EXPECT: x = 99
    printf("y = %d\n", get_value(&st, "y"));
    // EXPECT: y = 20
    printf("z = %d\n", get_value(&st, "z"));
    // EXPECT: z = 42
    printf("active: %d\n", count_active(&st));
    // EXPECT: active: 5
    printf("scope: %d\n", st.current_scope);
    // EXPECT: scope: 1

    printf("=== SymTable Test 3: Nested scope ===\n");
    // EXPECT: === SymTable Test 3: Nested scope ===
    enter_scope(&st);
    add_symbol(&st, "x", SYM_VAR, 777);  // shadows again
    add_symbol(&st, "w", SYM_VAR, 55);

    printf("x = %d\n", get_value(&st, "x"));
    // EXPECT: x = 777
    printf("w = %d\n", get_value(&st, "w"));
    // EXPECT: w = 55
    printf("active: %d\n", count_active(&st));
    // EXPECT: active: 7
    printf("scope: %d\n", st.current_scope);
    // EXPECT: scope: 2

    printf("=== SymTable Test 4: Leave innermost ===\n");
    // EXPECT: === SymTable Test 4: Leave innermost ===
    leave_scope(&st);
    printf("x = %d\n", get_value(&st, "x"));
    // EXPECT: x = 99
    printf("w = %d\n", get_value(&st, "w"));
    // EXPECT: w = -9999
    printf("active: %d\n", count_active(&st));
    // EXPECT: active: 5
    printf("scope: %d\n", st.current_scope);
    // EXPECT: scope: 1

    printf("=== SymTable Test 5: Leave to global ===\n");
    // EXPECT: === SymTable Test 5: Leave to global ===
    leave_scope(&st);
    printf("x = %d\n", get_value(&st, "x"));
    // EXPECT: x = 10
    printf("z = %d\n", get_value(&st, "z"));
    // EXPECT: z = -9999
    printf("active: %d\n", count_active(&st));
    // EXPECT: active: 3
    printf("scope: %d\n", st.current_scope);
    // EXPECT: scope: 0

    printf("=== SymTable Test 6: Mutation ===\n");
    // EXPECT: === SymTable Test 6: Mutation ===
    set_value(&st, "x", 500);
    printf("x = %d\n", get_value(&st, "x"));
    // EXPECT: x = 500

    printf("=== SymTable Test 7: Duplicate check ===\n");
    // EXPECT: === SymTable Test 7: Duplicate check ===
    int dup = add_symbol(&st, "x", SYM_VAR, 1);
    printf("dup result: %d\n", dup);
    // EXPECT: dup result: -1

    printf("=== SymTable Test 8: Function symbol ===\n");
    // EXPECT: === SymTable Test 8: Function symbol ===
    add_symbol(&st, "main", SYM_FUNC, 0);
    int fi = lookup(&st, "main");
    printf("main kind: %s\n", kind_str(st.syms[fi].kind));
    // EXPECT: main kind: FUNC

    printf("total entries: %d\n", st.count);
    // EXPECT: total entries: 8

    printf("done\n");
    // EXPECT: done
    return 0;
}
