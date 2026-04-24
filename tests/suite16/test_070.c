int printf(const char *fmt, ...);

// Command-Line Argument Parser (simulated)
// Parses flags (-v, --verbose), options (--name=value, -n value), positional args
// Operates as a state machine over a flat character array with null separators

enum arg_type {
    ARG_FLAG_SHORT,
    ARG_FLAG_LONG,
    ARG_OPTION_SHORT,
    ARG_OPTION_LONG,
    ARG_POSITIONAL,
    ARG_END_OF_OPTIONS,
    ARG_ERROR
};

struct parsed_arg {
    int type;
    char name[16];
    char value[32];
    int name_len;
    int val_len;
};

struct arg_parser {
    struct parsed_arg args[16];
    int count;
    int flags_done;
};

void parser_init(struct arg_parser *p) {
    p->count = 0;
    p->flags_done = 0;
    int i;
    int j;
    for (i = 0; i < 16; i++) {
        p->args[i].type = 0;
        p->args[i].name_len = 0;
        p->args[i].val_len = 0;
        for (j = 0; j < 16; j++) p->args[i].name[j] = 0;
        for (j = 0; j < 32; j++) p->args[i].value[j] = 0;
    }
}

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

void copy_str(char *dst, char *src, int len, int maxlen) {
    int i;
    for (i = 0; i < len && i < maxlen; i++) {
        dst[i] = src[i];
    }
}

// Check if a short flag expects a value (simulated known flags)
// Flags that take values: -n, -o, -p
int short_takes_value(char c) {
    return c == 'n' || c == 'o' || c == 'p';
}

void parse_one_arg(struct arg_parser *p, char *arg, char *next, int *skip) {
    int idx = p->count;
    if (idx >= 16) return;
    int len = str_len(arg);
    *skip = 0;

    if (p->flags_done) {
        p->args[idx].type = ARG_POSITIONAL;
        copy_str(p->args[idx].value, arg, len, 31);
        p->args[idx].val_len = len;
        p->count = p->count + 1;
        return;
    }

    // "--" end of options
    if (len == 2 && arg[0] == 45 && arg[1] == 45) {
        p->args[idx].type = ARG_END_OF_OPTIONS;
        p->flags_done = 1;
        p->count = p->count + 1;
        return;
    }

    // Long option: --xxx or --xxx=yyy
    if (len > 2 && arg[0] == 45 && arg[1] == 45) {
        // Find '=' if present
        int eq_pos = -1;
        int i;
        for (i = 2; i < len; i++) {
            if (arg[i] == 61) { eq_pos = i; break; }
        }
        if (eq_pos >= 0) {
            p->args[idx].type = ARG_OPTION_LONG;
            copy_str(p->args[idx].name, arg + 2, eq_pos - 2, 15);
            p->args[idx].name_len = eq_pos - 2;
            copy_str(p->args[idx].value, arg + eq_pos + 1, len - eq_pos - 1, 31);
            p->args[idx].val_len = len - eq_pos - 1;
        } else {
            p->args[idx].type = ARG_FLAG_LONG;
            copy_str(p->args[idx].name, arg + 2, len - 2, 15);
            p->args[idx].name_len = len - 2;
        }
        p->count = p->count + 1;
        return;
    }

    // Short option: -x or -x value
    if (len >= 2 && arg[0] == 45) {
        char flag_char = arg[1];
        if (short_takes_value(flag_char) && len == 2 && next != 0) {
            p->args[idx].type = ARG_OPTION_SHORT;
            p->args[idx].name[0] = flag_char;
            p->args[idx].name_len = 1;
            int nlen = str_len(next);
            copy_str(p->args[idx].value, next, nlen, 31);
            p->args[idx].val_len = nlen;
            *skip = 1;
        } else if (short_takes_value(flag_char) && len > 2) {
            // -nVALUE form
            p->args[idx].type = ARG_OPTION_SHORT;
            p->args[idx].name[0] = flag_char;
            p->args[idx].name_len = 1;
            copy_str(p->args[idx].value, arg + 2, len - 2, 31);
            p->args[idx].val_len = len - 2;
        } else {
            // Could be grouped flags: -abc = -a -b -c
            int i;
            for (i = 1; i < len; i++) {
                int ci = p->count;
                if (ci >= 16) break;
                p->args[ci].type = ARG_FLAG_SHORT;
                p->args[ci].name[0] = arg[i];
                p->args[ci].name_len = 1;
                p->count = p->count + 1;
            }
            return;
        }
        p->count = p->count + 1;
        return;
    }

    // Positional argument
    p->args[idx].type = ARG_POSITIONAL;
    copy_str(p->args[idx].value, arg, len, 31);
    p->args[idx].val_len = len;
    p->count = p->count + 1;
}

char *type_name(int t) {
    if (t == ARG_FLAG_SHORT) return "FLAG_S";
    if (t == ARG_FLAG_LONG) return "FLAG_L";
    if (t == ARG_OPTION_SHORT) return "OPT_S";
    if (t == ARG_OPTION_LONG) return "OPT_L";
    if (t == ARG_POSITIONAL) return "POS";
    if (t == ARG_END_OF_OPTIONS) return "END";
    return "ERR";
}

int main() {
    struct arg_parser p;

    printf("=== Arg Parser ===\n"); // EXPECT: === Arg Parser ===

    // Test 1: Simple flags and positional
    // args: -v --verbose file.txt
    char a0[4]; a0[0]='-'; a0[1]='v'; a0[2]=0;
    char a1[16]; a1[0]='-'; a1[1]='-'; a1[2]='v'; a1[3]='e'; a1[4]='r';
    a1[5]='b'; a1[6]='o'; a1[7]='s'; a1[8]='e'; a1[9]=0;
    char a2[16]; a2[0]='f'; a2[1]='i'; a2[2]='l'; a2[3]='e'; a2[4]='.';
    a2[5]='t'; a2[6]='x'; a2[7]='t'; a2[8]=0;

    parser_init(&p);
    int skip;
    parse_one_arg(&p, a0, a1, &skip);
    parse_one_arg(&p, a1, a2, &skip);
    parse_one_arg(&p, a2, 0, &skip);
    printf("Count: %d\n", p.count);                       // EXPECT: Count: 3
    printf("A0: %s %s\n", type_name(p.args[0].type), p.args[0].name); // EXPECT: A0: FLAG_S v
    printf("A1: %s %s\n", type_name(p.args[1].type), p.args[1].name); // EXPECT: A1: FLAG_L verbose
    printf("A2: %s %s\n", type_name(p.args[2].type), p.args[2].value); // EXPECT: A2: POS file.txt

    // Test 2: Option with value
    // args: -n hello --out=result
    char b0[4]; b0[0]='-'; b0[1]='n'; b0[2]=0;
    char b1[8]; b1[0]='h'; b1[1]='e'; b1[2]='l'; b1[3]='l'; b1[4]='o'; b1[5]=0;
    char b2[16]; b2[0]='-'; b2[1]='-'; b2[2]='o'; b2[3]='u'; b2[4]='t';
    b2[5]='='; b2[6]='r'; b2[7]='e'; b2[8]='s'; b2[9]='u'; b2[10]='l'; b2[11]='t'; b2[12]=0;

    parser_init(&p);
    skip = 0;
    parse_one_arg(&p, b0, b1, &skip);
    printf("Skip: %d\n", skip);                           // EXPECT: Skip: 1
    printf("A0: %s %s %s\n", type_name(p.args[0].type), p.args[0].name, p.args[0].value); // EXPECT: A0: OPT_S n hello
    parse_one_arg(&p, b2, 0, &skip);
    printf("A1: %s %s %s\n", type_name(p.args[1].type), p.args[1].name, p.args[1].value); // EXPECT: A1: OPT_L out result

    // Test 3: Grouped short flags
    // args: -abc
    char c0[8]; c0[0]='-'; c0[1]='a'; c0[2]='b'; c0[3]='c'; c0[4]=0;
    parser_init(&p);
    parse_one_arg(&p, c0, 0, &skip);
    printf("Count: %d\n", p.count);                       // EXPECT: Count: 3
    printf("G0: %s %s\n", type_name(p.args[0].type), p.args[0].name); // EXPECT: G0: FLAG_S a
    printf("G1: %s %s\n", type_name(p.args[1].type), p.args[1].name); // EXPECT: G1: FLAG_S b
    printf("G2: %s %s\n", type_name(p.args[2].type), p.args[2].name); // EXPECT: G2: FLAG_S c

    // Test 4: End of options with "--"
    // args: -- -notflag
    char d0[4]; d0[0]='-'; d0[1]='-'; d0[2]=0;
    char d1[16]; d1[0]='-'; d1[1]='n'; d1[2]='o'; d1[3]='t';
    d1[4]='f'; d1[5]='l'; d1[6]='a'; d1[7]='g'; d1[8]=0;
    parser_init(&p);
    parse_one_arg(&p, d0, d1, &skip);
    parse_one_arg(&p, d1, 0, &skip);
    printf("Count: %d\n", p.count);                       // EXPECT: Count: 2
    printf("D0: %s\n", type_name(p.args[0].type));        // EXPECT: D0: END
    printf("D1: %s %s\n", type_name(p.args[1].type), p.args[1].value); // EXPECT: D1: POS -notflag

    // Test 5: Attached short option: -nworld
    char e0[8]; e0[0]='-'; e0[1]='n'; e0[2]='w'; e0[3]='o'; e0[4]='r';
    e0[5]='l'; e0[6]='d'; e0[7]=0;
    parser_init(&p);
    parse_one_arg(&p, e0, 0, &skip);
    printf("Count: %d\n", p.count);                       // EXPECT: Count: 1
    printf("E0: %s %s %s\n", type_name(p.args[0].type), p.args[0].name, p.args[0].value); // EXPECT: E0: OPT_S n world

    printf("Done\n"); // EXPECT: Done
    return 0;
}
