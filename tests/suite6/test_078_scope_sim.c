int printf(const char *fmt, ...);

int MAX_VARS = 128;
int MAX_SCOPES = 16;

int var_names[128];
int var_values[128];
int var_scope[128];
int var_count;

int scope_stack[16];
int scope_depth;

int current_scope_id;

void init(void) {
    var_count = 0;
    scope_depth = 0;
    current_scope_id = 0;
}

void push_scope(void) {
    scope_stack[scope_depth] = current_scope_id;
    scope_depth = scope_depth + 1;
    current_scope_id = current_scope_id + 1;
}

int pop_scope(void) {
    if (scope_depth <= 0) return -1;
    scope_depth = scope_depth - 1;
    int old_scope = scope_stack[scope_depth];
    int removed = 0;
    int i = var_count - 1;
    while (i >= 0 && var_scope[i] == scope_stack[scope_depth] + 1) {
        i = i - 1;
        removed = removed + 1;
    }
    /* Actually, scope IDs don't work like that. Let's use depth-based removal. */
    /* Remove vars whose scope level matches the popped scope depth. */
    int new_count = 0;
    i = 0;
    while (i < var_count) {
        if (var_scope[i] < scope_depth + 1) {
            var_names[new_count] = var_names[i];
            var_values[new_count] = var_values[i];
            var_scope[new_count] = var_scope[i];
            new_count = new_count + 1;
        }
        i = i + 1;
    }
    removed = var_count - new_count;
    var_count = new_count;
    return removed;
}

void define_var(int name, int value) {
    var_names[var_count] = name;
    var_values[var_count] = value;
    var_scope[var_count] = scope_depth;
    var_count = var_count + 1;
}

int lookup_var(int name) {
    int i = var_count - 1;
    while (i >= 0) {
        if (var_names[i] == name) return var_values[i];
        i = i - 1;
    }
    return -999;
}

int var_exists(int name) {
    int i = var_count - 1;
    while (i >= 0) {
        if (var_names[i] == name) return 1;
        i = i - 1;
    }
    return 0;
}

int var_in_current_scope(int name) {
    int i = var_count - 1;
    while (i >= 0) {
        if (var_names[i] == name && var_scope[i] == scope_depth) return 1;
        if (var_scope[i] < scope_depth) break;
        i = i - 1;
    }
    return 0;
}

void set_var(int name, int value) {
    int i = var_count - 1;
    while (i >= 0) {
        if (var_names[i] == name) {
            var_values[i] = value;
            return;
        }
        i = i - 1;
    }
}

int main(void) {
    init();
    push_scope();

    define_var(1, 10);
    // EXPECT: x=10
    printf("x=%d\n", lookup_var(1));

    define_var(2, 20);
    // EXPECT: y=20
    printf("y=%d\n", lookup_var(2));

    // EXPECT: depth=1
    printf("depth=%d\n", scope_depth);

    push_scope();
    define_var(1, 100);
    // EXPECT: x_inner=100
    printf("x_inner=%d\n", lookup_var(1));
    // EXPECT: y_visible=20
    printf("y_visible=%d\n", lookup_var(2));

    // EXPECT: depth=2
    printf("depth=%d\n", scope_depth);
    // EXPECT: vars=3
    printf("vars=%d\n", var_count);

    int removed = pop_scope();
    // EXPECT: removed=1
    printf("removed=%d\n", removed);
    // EXPECT: x_outer=10
    printf("x_outer=%d\n", lookup_var(1));
    // EXPECT: vars_after=2
    printf("vars_after=%d\n", var_count);

    push_scope();
    define_var(3, 30);
    define_var(4, 40);
    define_var(5, 50);
    // EXPECT: z=30
    printf("z=%d\n", lookup_var(3));
    // EXPECT: vars_nested=5
    printf("vars_nested=%d\n", var_count);

    push_scope();
    define_var(3, 300);
    // EXPECT: z_shadow=300
    printf("z_shadow=%d\n", lookup_var(3));
    // EXPECT: depth=3
    printf("depth=%d\n", scope_depth);

    pop_scope();
    // EXPECT: z_restored=30
    printf("z_restored=%d\n", lookup_var(3));

    removed = pop_scope();
    // EXPECT: removed2=3
    printf("removed2=%d\n", removed);

    // EXPECT: z_gone=-999
    printf("z_gone=%d\n", lookup_var(3));

    // EXPECT: x_still=10
    printf("x_still=%d\n", lookup_var(1));

    set_var(1, 999);
    // EXPECT: x_updated=999
    printf("x_updated=%d\n", lookup_var(1));

    // EXPECT: exists_x=1
    printf("exists_x=%d\n", var_exists(1));
    // EXPECT: exists_z=0
    printf("exists_z=%d\n", var_exists(3));

    push_scope();
    define_var(6, 60);
    define_var(7, 70);
    // EXPECT: in_scope=1
    printf("in_scope=%d\n", var_in_current_scope(6));

    pop_scope();
    // EXPECT: final_vars=2
    printf("final_vars=%d\n", var_count);
    // EXPECT: final_depth=1
    printf("final_depth=%d\n", scope_depth);

    return 0;
}
