int printf(const char *fmt, ...);

struct Entry {
    char key[20];
    char value[20];
};

struct Config {
    struct Entry entries[32];
    int count;
};

void copy_str(char *dst, const char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_eq(const char *a, const char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int str_len(const char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

int is_digit(char c) {
    return c >= 48 && c <= 57;
}

int atoi_simple(const char *s) {
    int result = 0;
    int neg = 0;
    int i = 0;
    if (s[0] == 45) {
        neg = 1;
        i = 1;
    }
    while (s[i] != 0 && is_digit(s[i])) {
        result = result * 10 + (s[i] - 48);
        i = i + 1;
    }
    if (neg) return 0 - result;
    return result;
}

int parse_line(const char *line, char *key, char *val) {
    int i = 0;
    int ki = 0;
    while (line[i] != 0 && line[i] != 61) {
        key[ki] = line[i];
        ki = ki + 1;
        i = i + 1;
    }
    key[ki] = 0;
    if (line[i] != 61) return 0;
    i = i + 1;
    int vi = 0;
    while (line[i] != 0 && line[i] != 10) {
        val[vi] = line[i];
        vi = vi + 1;
        i = i + 1;
    }
    val[vi] = 0;
    return 1;
}

void config_init(struct Config *cfg) {
    cfg->count = 0;
}

int config_add(struct Config *cfg, const char *key, const char *val) {
    if (cfg->count >= 32) return 0;
    int idx = cfg->count;
    copy_str(cfg->entries[idx].key, key);
    copy_str(cfg->entries[idx].value, val);
    cfg->count = cfg->count + 1;
    return 1;
}

int config_parse_line(struct Config *cfg, const char *line) {
    char key[20];
    char val[20];
    int ok = parse_line(line, key, val);
    if (ok) {
        return config_add(cfg, key, val);
    }
    return 0;
}

const char *config_get(struct Config *cfg, const char *key) {
    int i = 0;
    while (i < cfg->count) {
        if (str_eq(cfg->entries[i].key, key)) {
            return cfg->entries[i].value;
        }
        i = i + 1;
    }
    return "";
}

int config_get_int(struct Config *cfg, const char *key) {
    const char *v = config_get(cfg, key);
    return atoi_simple(v);
}

int config_has(struct Config *cfg, const char *key) {
    int i = 0;
    while (i < cfg->count) {
        if (str_eq(cfg->entries[i].key, key)) return 1;
        i = i + 1;
    }
    return 0;
}

int config_remove(struct Config *cfg, const char *key) {
    int i = 0;
    while (i < cfg->count) {
        if (str_eq(cfg->entries[i].key, key)) {
            int j = i;
            while (j < cfg->count - 1) {
                cfg->entries[j] = cfg->entries[j + 1];
                j = j + 1;
            }
            cfg->count = cfg->count - 1;
            return 1;
        }
        i = i + 1;
    }
    return 0;
}

int main(void) {
    struct Config cfg;
    config_init(&cfg);

    config_parse_line(&cfg, "host=localhost");
    config_parse_line(&cfg, "port=8080");
    config_parse_line(&cfg, "debug=1");
    config_parse_line(&cfg, "name=myapp");
    config_parse_line(&cfg, "timeout=30");

    // EXPECT: count=5
    printf("count=%d\n", cfg.count);

    // EXPECT: host=localhost
    printf("host=%s\n", config_get(&cfg, "host"));

    // EXPECT: port=8080
    printf("port=%s\n", config_get(&cfg, "port"));

    // EXPECT: port_int=8080
    printf("port_int=%d\n", config_get_int(&cfg, "port"));

    // EXPECT: debug=1
    printf("debug=%s\n", config_get(&cfg, "debug"));

    // EXPECT: name=myapp
    printf("name=%s\n", config_get(&cfg, "name"));

    // EXPECT: has_host=1
    printf("has_host=%d\n", config_has(&cfg, "host"));

    // EXPECT: has_foo=0
    printf("has_foo=%d\n", config_has(&cfg, "foo"));

    int r = config_remove(&cfg, "debug");
    // EXPECT: removed=1
    printf("removed=%d\n", r);

    // EXPECT: count_after=4
    printf("count_after=%d\n", cfg.count);

    // EXPECT: has_debug=0
    printf("has_debug=%d\n", config_has(&cfg, "debug"));

    // EXPECT: timeout_int=30
    printf("timeout_int=%d\n", config_get_int(&cfg, "timeout"));

    config_parse_line(&cfg, "max=-5");
    // EXPECT: max_int=-5
    printf("max_int=%d\n", config_get_int(&cfg, "max"));

    // EXPECT: count_final=5
    printf("count_final=%d\n", cfg.count);

    int bad = config_parse_line(&cfg, "noequals");
    // EXPECT: bad_parse=0
    printf("bad_parse=%d\n", bad);

    return 0;
}
