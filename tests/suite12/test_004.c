int printf(const char *fmt, ...);

// Symbol table with scopes
// Implements a scoped symbol table using a stack of scope frames

int MAX_SYMBOLS = 64;
int MAX_SCOPES = 8;

struct Symbol {
    char name[16];
    int value;
    int type;
    int scope_level;
    int active;
};

struct Symbol symtab[64];
int sym_count;
int scope_stack[8];
int scope_top;
int current_scope;

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

void init_symtab() {
    sym_count = 0;
    scope_top = 0;
    current_scope = 0;
    scope_stack[0] = 0;
    int i = 0;
    while (i < 64) {
        symtab[i].active = 0;
        i = i + 1;
    }
}

void enter_scope() {
    current_scope = current_scope + 1;
    scope_top = scope_top + 1;
    scope_stack[scope_top] = sym_count;
}

void leave_scope() {
    int restore_to = scope_stack[scope_top];
    int i = restore_to;
    while (i < sym_count) {
        symtab[i].active = 0;
        i = i + 1;
    }
    sym_count = restore_to;
    scope_top = scope_top - 1;
    current_scope = current_scope - 1;
}

int add_symbol(char *name, int value, int type) {
    int i = sym_count - 1;
    while (i >= scope_stack[scope_top]) {
        if (symtab[i].active && str_eq(symtab[i].name, name)) {
            return -1;
        }
        i = i - 1;
    }
    int idx = sym_count;
    str_copy(symtab[idx].name, name);
    symtab[idx].value = value;
    symtab[idx].type = type;
    symtab[idx].scope_level = current_scope;
    symtab[idx].active = 1;
    sym_count = sym_count + 1;
    return idx;
}

int lookup(char *name) {
    int i = sym_count - 1;
    while (i >= 0) {
        if (symtab[i].active && str_eq(symtab[i].name, name)) {
            return i;
        }
        i = i - 1;
    }
    return -1;
}

int get_value(char *name) {
    int idx = lookup(name);
    if (idx < 0) return -999;
    return symtab[idx].value;
}

void set_value(char *name, int val) {
    int idx = lookup(name);
    if (idx >= 0) {
        symtab[idx].value = val;
    }
}

char *type_name(int t) {
    if (t == 0) return "int";
    if (t == 1) return "char";
    if (t == 2) return "ptr";
    return "unknown";
}

int count_active() {
    int c = 0;
    int i = 0;
    while (i < sym_count) {
        if (symtab[i].active) c = c + 1;
        i = i + 1;
    }
    return c;
}

int main() {
    init_symtab();

    add_symbol("x", 10, 0);
    add_symbol("y", 20, 0);
    add_symbol("msg", 0, 1);

    printf("x = %d\n", get_value("x"));
    // EXPECT: x = 10
    printf("y = %d\n", get_value("y"));
    // EXPECT: y = 20
    printf("scope: %d\n", current_scope);
    // EXPECT: scope: 0
    printf("active: %d\n", count_active());
    // EXPECT: active: 3

    enter_scope();
    printf("scope: %d\n", current_scope);
    // EXPECT: scope: 1

    add_symbol("x", 100, 0);
    add_symbol("z", 30, 2);

    printf("inner x = %d\n", get_value("x"));
    // EXPECT: inner x = 100
    printf("y from outer = %d\n", get_value("y"));
    // EXPECT: y from outer = 20
    printf("z = %d\n", get_value("z"));
    // EXPECT: z = 30
    printf("active: %d\n", count_active());
    // EXPECT: active: 5

    enter_scope();
    add_symbol("x", 999, 0);
    add_symbol("w", 50, 0);
    printf("deep x = %d\n", get_value("x"));
    // EXPECT: deep x = 999
    printf("scope: %d\n", current_scope);
    // EXPECT: scope: 2
    printf("active: %d\n", count_active());
    // EXPECT: active: 7

    leave_scope();
    printf("after leave x = %d\n", get_value("x"));
    // EXPECT: after leave x = 100
    printf("w visible: %d\n", lookup("w") >= 0);
    // EXPECT: w visible: 0
    printf("active: %d\n", count_active());
    // EXPECT: active: 5

    leave_scope();
    printf("outer x = %d\n", get_value("x"));
    // EXPECT: outer x = 10
    printf("z visible: %d\n", lookup("z") >= 0);
    // EXPECT: z visible: 0
    printf("scope: %d\n", current_scope);
    // EXPECT: scope: 0
    printf("active: %d\n", count_active());
    // EXPECT: active: 3

    set_value("x", 42);
    printf("updated x = %d\n", get_value("x"));
    // EXPECT: updated x = 42

    int dup = add_symbol("x", 0, 0);
    printf("dup result: %d\n", dup);
    // EXPECT: dup result: -1

    int idx = lookup("x");
    printf("x type: %s\n", type_name(symtab[idx].type));
    // EXPECT: x type: int
    printf("x scope_level: %d\n", symtab[idx].scope_level);
    // EXPECT: x scope_level: 0

    printf("symtab done\n");
    // EXPECT: symtab done

    return 0;
}
