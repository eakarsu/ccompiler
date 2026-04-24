int printf(const char *fmt, ...);

// Command line argument parser simulation
// Supports: --flag, --key=value, -f (short), positional args

struct Option {
    char long_name[32];
    char short_name;
    int has_value;
    char value[64];
    int present;
};

struct ArgResult {
    struct Option options[8];
    int opt_count;
    char positional[8][64];
    int pos_count;
    int error;
    char error_msg[64];
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

void add_option(struct ArgResult *r, char *lname, char sname, int has_val) {
    int idx = r->opt_count;
    str_cpy(r->options[idx].long_name, lname);
    r->options[idx].short_name = sname;
    r->options[idx].has_value = has_val;
    r->options[idx].value[0] = 0;
    r->options[idx].present = 0;
    r->opt_count++;
}

int find_long_opt(struct ArgResult *r, char *name) {
    int i;
    for (i = 0; i < r->opt_count; i++) {
        if (str_eq(r->options[i].long_name, name)) return i;
    }
    return -1;
}

int find_short_opt(struct ArgResult *r, char c) {
    int i;
    for (i = 0; i < r->opt_count; i++) {
        if (r->options[i].short_name == c) return i;
    }
    return -1;
}

void parse_args(struct ArgResult *r, char *argv[], int argc) {
    int i;
    r->pos_count = 0;
    r->error = 0;
    r->error_msg[0] = 0;

    for (i = 0; i < argc; i++) {
        char *arg = argv[i];
        int alen = str_len(arg);

        if (alen > 2 && arg[0] == '-' && arg[1] == '-') {
            // Long option
            // Check for = sign
            int eq_pos = -1;
            int j;
            for (j = 2; j < alen; j++) {
                if (arg[j] == '=') { eq_pos = j; break; }
            }
            char name_buf[32];
            if (eq_pos >= 0) {
                str_ncpy(name_buf, arg + 2, eq_pos - 2);
            } else {
                str_cpy(name_buf, arg + 2);
            }

            int oi = find_long_opt(r, name_buf);
            if (oi < 0) {
                r->error = 1;
                str_cpy(r->error_msg, "unknown option");
                return;
            }
            r->options[oi].present = 1;
            if (eq_pos >= 0) {
                str_cpy(r->options[oi].value, arg + eq_pos + 1);
            } else if (r->options[oi].has_value && i + 1 < argc) {
                i++;
                str_cpy(r->options[oi].value, argv[i]);
            }
        } else if (alen == 2 && arg[0] == '-' && arg[1] != '-') {
            // Short option
            int oi = find_short_opt(r, arg[1]);
            if (oi < 0) {
                r->error = 1;
                str_cpy(r->error_msg, "unknown short option");
                return;
            }
            r->options[oi].present = 1;
            if (r->options[oi].has_value && i + 1 < argc) {
                i++;
                str_cpy(r->options[oi].value, argv[i]);
            }
        } else {
            // Positional
            str_cpy(r->positional[r->pos_count], arg);
            r->pos_count++;
        }
    }
}

int is_present(struct ArgResult *r, char *name) {
    int i = find_long_opt(r, name);
    if (i >= 0) return r->options[i].present;
    return 0;
}

char *get_opt_val(struct ArgResult *r, char *name) {
    int i = find_long_opt(r, name);
    if (i >= 0 && r->options[i].present) return r->options[i].value;
    return "";
}

int main() {
    struct ArgResult res;
    res.opt_count = 0;

    // Define options
    add_option(&res, "verbose", 'v', 0);
    add_option(&res, "output", 'o', 1);
    add_option(&res, "help", 'h', 0);
    add_option(&res, "count", 'c', 1);

    printf("%d\n", res.opt_count);
    // EXPECT: 4

    // Parse: mycc --verbose --output=result.s -c 5 input.c
    char *argv1[] = {"--verbose", "--output=result.s", "-c", "5", "input.c"};
    parse_args(&res, argv1, 5);

    printf("%d\n", res.error);
    // EXPECT: 0
    printf("%d\n", is_present(&res, "verbose"));
    // EXPECT: 1
    printf("%s\n", get_opt_val(&res, "output"));
    // EXPECT: result.s
    printf("%s\n", get_opt_val(&res, "count"));
    // EXPECT: 5
    printf("%d\n", is_present(&res, "help"));
    // EXPECT: 0
    printf("%d\n", res.pos_count);
    // EXPECT: 1
    printf("%s\n", res.positional[0]);
    // EXPECT: input.c

    // Parse with short options
    struct ArgResult res2;
    res2.opt_count = 0;
    add_option(&res2, "verbose", 'v', 0);
    add_option(&res2, "output", 'o', 1);
    add_option(&res2, "help", 'h', 0);
    add_option(&res2, "count", 'c', 1);

    char *argv2[] = {"-v", "-o", "out.s", "-h", "a.c", "b.c"};
    parse_args(&res2, argv2, 6);

    printf("%d\n", res2.error);
    // EXPECT: 0
    printf("%d\n", is_present(&res2, "verbose"));
    // EXPECT: 1
    printf("%d\n", is_present(&res2, "help"));
    // EXPECT: 1
    printf("%s\n", get_opt_val(&res2, "output"));
    // EXPECT: out.s
    printf("%d\n", res2.pos_count);
    // EXPECT: 2
    printf("%s\n", res2.positional[0]);
    // EXPECT: a.c
    printf("%s\n", res2.positional[1]);
    // EXPECT: b.c

    // Unknown option error
    struct ArgResult res3;
    res3.opt_count = 0;
    add_option(&res3, "verbose", 'v', 0);

    char *argv3[] = {"--unknown"};
    parse_args(&res3, argv3, 1);
    printf("%d\n", res3.error);
    // EXPECT: 1
    printf("%s\n", res3.error_msg);
    // EXPECT: unknown option

    return 0;
}
