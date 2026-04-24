int printf(const char *fmt, ...);

struct Option {
    char short_name;
    char long_name[32];
    int has_value;
    int present;
    char value[64];
};

struct ArgParser {
    struct Option options[16];
    int num_options;
    char positional[10][64];
    int num_positional;
    int error;
};

void str_copy(char *dst, const char *src) {
    int i;
    i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_eq(const char *a, const char *b) {
    int i;
    i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int str_len(const char *s) {
    int i;
    i = 0;
    while (s[i] != 0) i = i + 1;
    return i;
}

void init_parser(struct ArgParser *p) {
    p->num_options = 0;
    p->num_positional = 0;
    p->error = 0;
}

void add_option(struct ArgParser *p, char sname, const char *lname, int has_val) {
    int idx;
    idx = p->num_options;
    p->options[idx].short_name = sname;
    str_copy(p->options[idx].long_name, lname);
    p->options[idx].has_value = has_val;
    p->options[idx].present = 0;
    p->options[idx].value[0] = 0;
    p->num_options = p->num_options + 1;
}

int find_short(struct ArgParser *p, char c) {
    int i;
    for (i = 0; i < p->num_options; i = i + 1) {
        if (p->options[i].short_name == c) return i;
    }
    return -1;
}

int find_long(struct ArgParser *p, const char *name) {
    int i;
    for (i = 0; i < p->num_options; i = i + 1) {
        if (str_eq(p->options[i].long_name, name)) return i;
    }
    return -1;
}

void parse_args(struct ArgParser *p, char args[][64], int argc) {
    int i;
    int opt_idx;
    i = 0;
    while (i < argc) {
        if (args[i][0] == '-' && args[i][1] == '-') {
            opt_idx = find_long(p, args[i] + 2);
            if (opt_idx < 0) {
                p->error = 1;
                i = i + 1;
                continue;
            }
            p->options[opt_idx].present = 1;
            if (p->options[opt_idx].has_value && i + 1 < argc) {
                i = i + 1;
                str_copy(p->options[opt_idx].value, args[i]);
            }
        } else if (args[i][0] == '-' && args[i][1] != 0) {
            opt_idx = find_short(p, args[i][1]);
            if (opt_idx < 0) {
                p->error = 1;
                i = i + 1;
                continue;
            }
            p->options[opt_idx].present = 1;
            if (p->options[opt_idx].has_value && i + 1 < argc) {
                i = i + 1;
                str_copy(p->options[opt_idx].value, args[i]);
            }
        } else {
            str_copy(p->positional[p->num_positional], args[i]);
            p->num_positional = p->num_positional + 1;
        }
        i = i + 1;
    }
}

int is_present(struct ArgParser *p, const char *lname) {
    int idx;
    idx = find_long(p, lname);
    if (idx < 0) return 0;
    return p->options[idx].present;
}

const char *get_value(struct ArgParser *p, const char *lname) {
    int idx;
    idx = find_long(p, lname);
    if (idx < 0) return "";
    if (!p->options[idx].present) return "";
    return p->options[idx].value;
}

void print_help(struct ArgParser *p) {
    int i;
    printf("Options:\n");
    for (i = 0; i < p->num_options; i = i + 1) {
        if (p->options[i].has_value) {
            printf("  -%d --%s <value>\n", p->options[i].short_name, p->options[i].long_name);
        } else {
            printf("  -%d --%s\n", p->options[i].short_name, p->options[i].long_name);
        }
    }
}

int main() {
    struct ArgParser parser;
    char args[10][64];
    int i;

    // EXPECT: === Argument Parser Test ===
    printf("=== Argument Parser Test ===\n");

    init_parser(&parser);
    add_option(&parser, 'v', "verbose", 0);
    add_option(&parser, 'o', "output", 1);
    add_option(&parser, 'h', "help", 0);
    add_option(&parser, 'n', "count", 1);
    add_option(&parser, 'q', "quiet", 0);

    str_copy(args[0], "--verbose");
    str_copy(args[1], "-o");
    str_copy(args[2], "result.txt");
    str_copy(args[3], "--count");
    str_copy(args[4], "42");
    str_copy(args[5], "input1.c");
    str_copy(args[6], "input2.c");

    parse_args(&parser, args, 7);

    // EXPECT: verbose: 1
    printf("verbose: %d\n", is_present(&parser, "verbose"));
    // EXPECT: quiet: 0
    printf("quiet: %d\n", is_present(&parser, "quiet"));
    // EXPECT: help: 0
    printf("help: %d\n", is_present(&parser, "help"));
    // EXPECT: output: result.txt
    printf("output: %s\n", get_value(&parser, "output"));
    // EXPECT: count: 42
    printf("count: %s\n", get_value(&parser, "count"));
    // EXPECT: Positional args: 2
    printf("Positional args: %d\n", parser.num_positional);
    // EXPECT:   [0] input1.c
    // EXPECT:   [1] input2.c
    for (i = 0; i < parser.num_positional; i = i + 1) {
        printf("  [%d] %s\n", i, parser.positional[i]);
    }
    // EXPECT: Parse error: 0
    printf("Parse error: %d\n", parser.error);

    init_parser(&parser);
    add_option(&parser, 'v', "verbose", 0);
    add_option(&parser, 'o', "output", 1);
    str_copy(args[0], "-v");
    str_copy(args[1], "--unknown");
    str_copy(args[2], "file.c");
    parse_args(&parser, args, 3);

    // EXPECT: Second parse - verbose: 1
    printf("Second parse - verbose: %d\n", is_present(&parser, "verbose"));
    // EXPECT: Second parse - error: 1
    printf("Second parse - error: %d\n", parser.error);
    // EXPECT: Second parse - positional: 1
    printf("Second parse - positional: %d\n", parser.num_positional);

    // EXPECT: === Done ===
    printf("=== Done ===\n");
    return 0;
}
