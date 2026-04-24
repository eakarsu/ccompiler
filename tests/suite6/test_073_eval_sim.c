int printf(const char *fmt, ...);

int vars[26];

void init_vars(void) {
    int i = 0;
    while (i < 26) {
        vars[i] = 0;
        i = i + 1;
    }
}

int var_index(char c) {
    return c - 97;
}

char prog_buf[512];
int prog_pos;
int output_vals[64];
int output_count;

int is_digit(char c) {
    return c >= 48 && c <= 57;
}

int is_lower(char c) {
    return c >= 97 && c <= 122;
}

void skip_spaces(void) {
    while (prog_buf[prog_pos] == 32) prog_pos = prog_pos + 1;
}

int parse_number(void) {
    int n = 0;
    while (is_digit(prog_buf[prog_pos])) {
        n = n * 10 + (prog_buf[prog_pos] - 48);
        prog_pos = prog_pos + 1;
    }
    return n;
}

int eval_expr(void);

int eval_primary(void) {
    skip_spaces();
    char c = prog_buf[prog_pos];
    if (c == 40) {
        prog_pos = prog_pos + 1;
        int val = eval_expr();
        skip_spaces();
        prog_pos = prog_pos + 1;
        return val;
    }
    if (is_digit(c)) {
        return parse_number();
    }
    if (is_lower(c)) {
        int idx = var_index(c);
        prog_pos = prog_pos + 1;
        return vars[idx];
    }
    return 0;
}

int eval_term(void) {
    int left = eval_primary();
    while (1) {
        skip_spaces();
        char c = prog_buf[prog_pos];
        if (c == 42) {
            prog_pos = prog_pos + 1;
            int right = eval_primary();
            left = left * right;
        } else if (c == 47) {
            prog_pos = prog_pos + 1;
            int right = eval_primary();
            left = left / right;
        } else {
            break;
        }
    }
    return left;
}

int eval_expr(void) {
    int left = eval_term();
    while (1) {
        skip_spaces();
        char c = prog_buf[prog_pos];
        if (c == 43) {
            prog_pos = prog_pos + 1;
            int right = eval_term();
            left = left + right;
        } else if (c == 45) {
            prog_pos = prog_pos + 1;
            int right = eval_term();
            left = left - right;
        } else {
            break;
        }
    }
    return left;
}

void copy_str(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

void exec_statement(char *stmt) {
    copy_str(prog_buf, stmt);
    prog_pos = 0;
    skip_spaces();

    if (prog_buf[prog_pos] == 112 && prog_buf[prog_pos + 1] == 32) {
        prog_pos = prog_pos + 2;
        int val = eval_expr();
        output_vals[output_count] = val;
        output_count = output_count + 1;
        printf("out=%d\n", val);
        return;
    }

    if (is_lower(prog_buf[prog_pos]) && prog_buf[prog_pos + 1] == 61) {
        int idx = var_index(prog_buf[prog_pos]);
        prog_pos = prog_pos + 2;
        int val = eval_expr();
        vars[idx] = val;
        return;
    }

    int val = eval_expr();
    output_vals[output_count] = val;
    output_count = output_count + 1;
    printf("out=%d\n", val);
}

int main(void) {
    init_vars();
    output_count = 0;

    exec_statement("a=5");
    exec_statement("b=3");
    // EXPECT: out=8
    exec_statement("p a+b");

    // EXPECT: out=15
    exec_statement("p a*b");

    exec_statement("c=a+b");
    // EXPECT: out=8
    exec_statement("p c");

    // EXPECT: out=2
    exec_statement("p a-b");

    exec_statement("x=10");
    exec_statement("y=20");
    // EXPECT: out=30
    exec_statement("p x+y");

    // EXPECT: out=200
    exec_statement("p x*y");

    exec_statement("z=x+y+a");
    // EXPECT: out=35
    exec_statement("p z");

    exec_statement("a=100");
    // EXPECT: out=100
    exec_statement("p a");

    // EXPECT: out=120
    exec_statement("p a+y");

    exec_statement("m=7");
    exec_statement("n=3");
    // EXPECT: out=2
    exec_statement("p m/n");

    // EXPECT: out=21
    exec_statement("p m*n");

    exec_statement("r=m+n*2");
    // EXPECT: out=13
    exec_statement("p r");

    exec_statement("s=(m+n)*2");
    // EXPECT: out=20
    exec_statement("p s");

    // EXPECT: out=1
    exec_statement("p m-n-n");

    // EXPECT: total_outputs=14
    printf("total_outputs=%d\n", output_count);

    return 0;
}
