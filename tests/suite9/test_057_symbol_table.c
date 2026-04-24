int printf(const char *fmt, ...);

// Symbol table with scope support
// Each scope level has its own hash map layer
// Lookup searches from current scope up to global scope

char sym_names[128][16];
int sym_vals[128];
int sym_scope[128];
int sym_used[128];
int sym_count;
int current_scope;

void st_init(void) {
    int i;
    int j;
    sym_count = 0;
    current_scope = 0;
    for (i = 0; i < 128; i++) {
        sym_vals[i] = 0;
        sym_scope[i] = 0;
        sym_used[i] = 0;
        for (j = 0; j < 16; j++) {
            sym_names[i][j] = 0;
        }
    }
}

int st_strcmp(char *a, char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 1;
        i = i + 1;
    }
    if (a[i] != b[i]) return 1;
    return 0;
}

void st_strcpy(char *dst, char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

void st_enter_scope(void) {
    current_scope = current_scope + 1;
}

void st_leave_scope(void) {
    int i;
    // Remove all symbols at current scope level
    for (i = 0; i < 128; i++) {
        if (sym_used[i] && sym_scope[i] == current_scope) {
            sym_used[i] = 0;
            sym_count = sym_count - 1;
        }
    }
    current_scope = current_scope - 1;
}

void st_define(char *name, int val) {
    int i;
    // Check if already defined in current scope
    for (i = 0; i < 128; i++) {
        if (sym_used[i] && sym_scope[i] == current_scope) {
            if (st_strcmp(sym_names[i], name) == 0) {
                sym_vals[i] = val;
                return;
            }
        }
    }
    // Find free slot
    for (i = 0; i < 128; i++) {
        if (sym_used[i] == 0) {
            st_strcpy(sym_names[i], name);
            sym_vals[i] = val;
            sym_scope[i] = current_scope;
            sym_used[i] = 1;
            sym_count = sym_count + 1;
            return;
        }
    }
}

int st_lookup(char *name, int *found) {
    int scope;
    int i;
    scope = current_scope;
    while (scope >= 0) {
        for (i = 0; i < 128; i++) {
            if (sym_used[i] && sym_scope[i] == scope) {
                if (st_strcmp(sym_names[i], name) == 0) {
                    *found = 1;
                    return sym_vals[i];
                }
            }
        }
        scope = scope - 1;
    }
    *found = 0;
    return 0;
}

int st_lookup_scope(char *name) {
    int i;
    int best_scope;
    best_scope = -1;
    for (i = 0; i < 128; i++) {
        if (sym_used[i]) {
            if (st_strcmp(sym_names[i], name) == 0) {
                if (sym_scope[i] > best_scope) {
                    best_scope = sym_scope[i];
                }
            }
        }
    }
    return best_scope;
}

int st_count_at_scope(int scope) {
    int i;
    int c;
    c = 0;
    for (i = 0; i < 128; i++) {
        if (sym_used[i] && sym_scope[i] == scope) c = c + 1;
    }
    return c;
}

int main(void) {
    int found;
    int val;

    st_init();

    // Global scope (0)
    st_define("x", 10);
    st_define("y", 20);
    st_define("z", 30);
    // EXPECT: scope=0 count=3
    printf("scope=%d count=%d\n", current_scope, sym_count);

    val = st_lookup("x", &found);
    // EXPECT: x=10 found=1
    printf("x=%d found=%d\n", val, found);

    // Enter scope 1
    st_enter_scope();
    st_define("x", 100);  // shadows global x
    st_define("a", 42);
    // EXPECT: scope=1 count=5
    printf("scope=%d count=%d\n", current_scope, sym_count);

    val = st_lookup("x", &found);
    // EXPECT: x in scope1=100
    printf("x in scope1=%d\n", val);

    val = st_lookup("y", &found);
    // EXPECT: y from global=20
    printf("y from global=%d\n", val);

    val = st_lookup("a", &found);
    // EXPECT: a=42 found=1
    printf("a=%d found=%d\n", val, found);

    // Enter scope 2 (nested)
    st_enter_scope();
    st_define("x", 999);  // shadows scope 1's x
    st_define("b", 77);

    val = st_lookup("x", &found);
    // EXPECT: x in scope2=999
    printf("x in scope2=%d\n", val);

    val = st_lookup("a", &found);
    // EXPECT: a from scope1=42
    printf("a from scope1=%d\n", val);

    // EXPECT: x defined at scope=2
    printf("x defined at scope=%d\n", st_lookup_scope("x"));

    // EXPECT: count at scope 0=3
    printf("count at scope 0=%d\n", st_count_at_scope(0));
    // EXPECT: count at scope 1=2
    printf("count at scope 1=%d\n", st_count_at_scope(1));
    // EXPECT: count at scope 2=2
    printf("count at scope 2=%d\n", st_count_at_scope(2));

    // Leave scope 2
    st_leave_scope();
    val = st_lookup("x", &found);
    // EXPECT: x after leave2=100
    printf("x after leave2=%d\n", val);

    val = st_lookup("b", &found);
    // EXPECT: b after leave2 found=0
    printf("b after leave2 found=%d\n", found);

    // Leave scope 1
    st_leave_scope();
    val = st_lookup("x", &found);
    // EXPECT: x after leave1=10
    printf("x after leave1=%d\n", val);

    val = st_lookup("a", &found);
    // EXPECT: a after leave1 found=0
    printf("a after leave1 found=%d\n", found);

    // EXPECT: final count=3
    printf("final count=%d\n", sym_count);
    // EXPECT: final scope=0
    printf("final scope=%d\n", current_scope);

    // EXPECT: done
    printf("done\n");

    return 0;
}
