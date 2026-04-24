int printf(const char *fmt, ...);

enum SymKind {
    SYM_VAR,
    SYM_FUNC,
    SYM_CONST,
    SYM_PARAM
};

struct Symbol {
    char name[16];
    int kind;
    int value;
    int scope_level;
    int is_defined;
};

struct SymbolTable {
    struct Symbol syms[64];
    int count;
    int current_scope;
};

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
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

char *kind_name(int kind) {
    if (kind == SYM_VAR) return "VAR";
    if (kind == SYM_FUNC) return "FUNC";
    if (kind == SYM_CONST) return "CONST";
    if (kind == SYM_PARAM) return "PARAM";
    return "UNKNOWN";
}

void table_init(struct SymbolTable *t) {
    t->count = 0;
    t->current_scope = 0;
}

int table_add(struct SymbolTable *t, char *name, int kind, int value) {
    /* Check for duplicate in current scope */
    int i;
    for (i = t->count - 1; i >= 0; i--) {
        if (t->syms[i].scope_level == t->current_scope && str_eq(t->syms[i].name, name)) {
            return -1; /* duplicate */
        }
    }
    struct Symbol *s = &t->syms[t->count];
    str_copy(s->name, name);
    s->kind = kind;
    s->value = value;
    s->scope_level = t->current_scope;
    s->is_defined = 1;
    t->count = t->count + 1;
    return t->count - 1;
}

int table_lookup(struct SymbolTable *t, char *name) {
    /* Search from most recent to oldest (inner scope first) */
    int i;
    for (i = t->count - 1; i >= 0; i--) {
        if (str_eq(t->syms[i].name, name)) {
            return i;
        }
    }
    return -1;
}

int table_lookup_scope(struct SymbolTable *t, char *name, int scope) {
    int i;
    for (i = t->count - 1; i >= 0; i--) {
        if (t->syms[i].scope_level == scope && str_eq(t->syms[i].name, name)) {
            return i;
        }
    }
    return -1;
}

void enter_scope(struct SymbolTable *t) {
    t->current_scope = t->current_scope + 1;
}

void leave_scope(struct SymbolTable *t) {
    /* Remove symbols from current scope */
    while (t->count > 0 && t->syms[t->count - 1].scope_level == t->current_scope) {
        t->count = t->count - 1;
    }
    t->current_scope = t->current_scope - 1;
}

int main() {
    struct SymbolTable tab;
    table_init(&tab);

    /* Global scope: add some globals */
    table_add(&tab, "x", SYM_VAR, 0);
    table_add(&tab, "y", SYM_VAR, 0);
    table_add(&tab, "PI", SYM_CONST, 314);
    table_add(&tab, "main", SYM_FUNC, 100);

    // EXPECT: Global scope symbols: 4
    printf("Global scope symbols: %d\n", tab.count);

    int idx = table_lookup(&tab, "PI");
    // EXPECT: Lookup PI: found kind=CONST value=314
    printf("Lookup PI: found kind=%s value=%d\n", kind_name(tab.syms[idx].kind), tab.syms[idx].value);

    idx = table_lookup(&tab, "main");
    // EXPECT: Lookup main: found kind=FUNC value=100
    printf("Lookup main: found kind=%s value=%d\n", kind_name(tab.syms[idx].kind), tab.syms[idx].value);

    /* Enter function scope */
    enter_scope(&tab);
    table_add(&tab, "a", SYM_PARAM, 0);
    table_add(&tab, "b", SYM_PARAM, 1);
    table_add(&tab, "x", SYM_VAR, 0);  /* shadows global x */

    // EXPECT: After entering scope 1: 7 symbols
    printf("After entering scope 1: %d symbols\n", tab.count);

    /* x should resolve to local (scope 1), not global */
    idx = table_lookup(&tab, "x");
    // EXPECT: Lookup x: scope=1 kind=VAR
    printf("Lookup x: scope=%d kind=%s\n", tab.syms[idx].scope_level, kind_name(tab.syms[idx].kind));

    /* y should resolve to global */
    idx = table_lookup(&tab, "y");
    // EXPECT: Lookup y: scope=0 kind=VAR
    printf("Lookup y: scope=%d kind=%s\n", tab.syms[idx].scope_level, kind_name(tab.syms[idx].kind));

    /* Enter nested block scope */
    enter_scope(&tab);
    table_add(&tab, "tmp", SYM_VAR, 0);
    table_add(&tab, "x", SYM_VAR, 0);  /* shadows both previous x's */

    idx = table_lookup(&tab, "x");
    // EXPECT: Lookup x (nested): scope=2 kind=VAR
    printf("Lookup x (nested): scope=%d kind=%s\n", tab.syms[idx].scope_level, kind_name(tab.syms[idx].kind));

    // EXPECT: Total symbols before leave: 9
    printf("Total symbols before leave: %d\n", tab.count);

    /* Leave nested scope */
    leave_scope(&tab);
    // EXPECT: After leaving scope 2: 7 symbols
    printf("After leaving scope 2: %d symbols\n", tab.count);

    /* x should now resolve to scope 1 again */
    idx = table_lookup(&tab, "x");
    // EXPECT: Lookup x after leave: scope=1
    printf("Lookup x after leave: scope=%d\n", tab.syms[idx].scope_level);

    /* tmp should not be found */
    idx = table_lookup(&tab, "tmp");
    // EXPECT: Lookup tmp after leave: -1
    printf("Lookup tmp after leave: %d\n", idx);

    /* Test duplicate detection */
    int dup = table_add(&tab, "a", SYM_VAR, 99);
    // EXPECT: Duplicate a in scope 1: -1
    printf("Duplicate a in scope 1: %d\n", dup);

    /* Leave function scope */
    leave_scope(&tab);
    // EXPECT: After leaving scope 1: 4 symbols
    printf("After leaving scope 1: %d symbols\n", tab.count);

    /* x should resolve to global again */
    idx = table_lookup(&tab, "x");
    // EXPECT: Lookup x (global again): scope=0
    printf("Lookup x (global again): scope=%d\n", tab.syms[idx].scope_level);

    /* Lookup nonexistent symbol */
    idx = table_lookup(&tab, "z");
    // EXPECT: Lookup z: -1
    printf("Lookup z: %d\n", idx);

    // EXPECT: All symbol resolver tests done
    printf("All symbol resolver tests done\n");
    return 0;
}
