int printf(const char *fmt, ...);

// Template engine: replace {{variable}} placeholders with values

struct Var {
    char name[32];
    char value[64];
};

struct VarTable {
    struct Var vars[16];
    int count;
};

int str_len(char *s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

void str_cpy(char *dst, char *src) {
    int i = 0;
    while (src[i]) { dst[i] = src[i]; i++; }
    dst[i] = 0;
}

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

void str_ncpy(char *dst, char *src, int n) {
    int i;
    for (i = 0; i < n && src[i]; i++) dst[i] = src[i];
    dst[i] = 0;
}

void set_var(struct VarTable *t, char *name, char *value) {
    // Check if already exists
    int i;
    for (i = 0; i < t->count; i++) {
        if (str_eq(t->vars[i].name, name)) {
            str_cpy(t->vars[i].value, value);
            return;
        }
    }
    str_cpy(t->vars[t->count].name, name);
    str_cpy(t->vars[t->count].value, value);
    t->count++;
}

char *get_var(struct VarTable *t, char *name) {
    int i;
    for (i = 0; i < t->count; i++) {
        if (str_eq(t->vars[i].name, name)) return t->vars[i].value;
    }
    return "";
}

// Expand template string, writing result to output buffer
// Returns length of output
int expand_template(char *tmpl, struct VarTable *vars, char *out, int outsz) {
    int tlen = str_len(tmpl);
    int i = 0;
    int o = 0;

    while (i < tlen && o < outsz - 1) {
        if (i + 1 < tlen && tmpl[i] == '{' && tmpl[i + 1] == '{') {
            // Find closing }}
            i += 2;
            // Skip whitespace
            while (i < tlen && tmpl[i] == ' ') i++;
            int name_start = i;
            while (i < tlen && tmpl[i] != '}' && tmpl[i] != ' ') i++;
            int name_end = i;

            char var_name[32];
            str_ncpy(var_name, tmpl + name_start, name_end - name_start);

            // Skip to }}
            while (i < tlen && tmpl[i] != '}') i++;
            if (i + 1 < tlen && tmpl[i] == '}' && tmpl[i + 1] == '}') i += 2;

            // Substitute
            char *val = get_var(vars, var_name);
            int vlen = str_len(val);
            int j;
            for (j = 0; j < vlen && o < outsz - 1; j++) {
                out[o++] = val[j];
            }
        } else {
            out[o++] = tmpl[i++];
        }
    }
    out[o] = 0;
    return o;
}

// Count number of variables referenced in a template
int count_vars_in_template(char *tmpl) {
    int tlen = str_len(tmpl);
    int count = 0;
    int i = 0;
    while (i < tlen) {
        if (i + 1 < tlen && tmpl[i] == '{' && tmpl[i + 1] == '{') {
            count++;
            i += 2;
            while (i < tlen && !(tmpl[i] == '}' && i + 1 < tlen && tmpl[i + 1] == '}')) i++;
            if (i < tlen) i += 2;
        } else {
            i++;
        }
    }
    return count;
}

// Check if all variables in template are defined
int all_vars_defined(char *tmpl, struct VarTable *vars) {
    int tlen = str_len(tmpl);
    int i = 0;
    while (i < tlen) {
        if (i + 1 < tlen && tmpl[i] == '{' && tmpl[i + 1] == '{') {
            i += 2;
            while (i < tlen && tmpl[i] == ' ') i++;
            int ns = i;
            while (i < tlen && tmpl[i] != '}' && tmpl[i] != ' ') i++;
            char vn[32];
            str_ncpy(vn, tmpl + ns, i - ns);

            // Check if defined
            int found = 0;
            int j;
            for (j = 0; j < vars->count; j++) {
                if (str_eq(vars->vars[j].name, vn)) { found = 1; break; }
            }
            if (!found) return 0;

            while (i < tlen && tmpl[i] != '}') i++;
            if (i + 1 < tlen) i += 2;
        } else {
            i++;
        }
    }
    return 1;
}

int main() {
    struct VarTable vars;
    vars.count = 0;

    set_var(&vars, "name", "Alice");
    set_var(&vars, "greeting", "Hello");
    set_var(&vars, "place", "Wonderland");

    printf("%d\n", vars.count);
    // EXPECT: 3

    // Simple substitution
    char out[256];
    expand_template("{{greeting}}, {{name}}!", &vars, out, 256);
    printf("%s\n", out);
    // EXPECT: Hello, Alice!

    // No variables
    expand_template("Just plain text", &vars, out, 256);
    printf("%s\n", out);
    // EXPECT: Just plain text

    // Multiple uses of same variable
    expand_template("{{name}} is {{name}}", &vars, out, 256);
    printf("%s\n", out);
    // EXPECT: Alice is Alice

    // All three variables
    expand_template("{{greeting}} {{name}} from {{place}}", &vars, out, 256);
    printf("%s\n", out);
    // EXPECT: Hello Alice from Wonderland

    // Undefined variable -> empty
    expand_template("Age:{{age}}", &vars, out, 256);
    printf("%s\n", out);
    // EXPECT: Age:

    // Count variables in template
    int vc = count_vars_in_template("{{a}} and {{b}} and {{c}}");
    printf("%d\n", vc);
    // EXPECT: 3

    vc = count_vars_in_template("no vars here");
    printf("%d\n", vc);
    // EXPECT: 0

    vc = count_vars_in_template("{{x}}");
    printf("%d\n", vc);
    // EXPECT: 1

    // Check if all vars defined
    int ad = all_vars_defined("{{greeting}} {{name}}", &vars);
    printf("%d\n", ad);
    // EXPECT: 1

    ad = all_vars_defined("{{greeting}} {{missing}}", &vars);
    printf("%d\n", ad);
    // EXPECT: 0

    // Update variable
    set_var(&vars, "name", "Bob");
    printf("%d\n", vars.count);
    // EXPECT: 3
    expand_template("Hi {{name}}", &vars, out, 256);
    printf("%s\n", out);
    // EXPECT: Hi Bob

    // Template with adjacent vars
    set_var(&vars, "first", "foo");
    set_var(&vars, "second", "bar");
    expand_template("{{first}}{{second}}", &vars, out, 256);
    printf("%s\n", out);
    // EXPECT: foobar

    // Longer template
    set_var(&vars, "title", "Report");
    set_var(&vars, "date", "2024");
    expand_template("{{title}} for year {{date}} by {{name}}", &vars, out, 256);
    printf("%s\n", out);
    // EXPECT: Report for year 2024 by Bob

    // Output length check
    int olen = expand_template("abc{{name}}def", &vars, out, 256);
    printf("%d\n", olen);
    // EXPECT: 9
    printf("%s\n", out);
    // EXPECT: abcBobdef

    return 0;
}
