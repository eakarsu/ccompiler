// EXPECT: === JSON Parser Tests ===
// EXPECT: Parsing: {"name":"Alice","age":30,"score":95}
// EXPECT: Found 3 key-value pairs
// EXPECT: Key[0]: name = Alice (string)
// EXPECT: Key[1]: age = 30 (int)
// EXPECT: Key[2]: score = 95 (int)
// EXPECT: === Lookup Tests ===
// EXPECT: Lookup 'name': Alice
// EXPECT: Lookup 'age': 30
// EXPECT: Lookup 'score': 95
// EXPECT: Lookup 'missing': (not found)
// EXPECT: === Second Object ===
// EXPECT: Parsing: {"city":"Boston","pop":700000,"zip":2101}
// EXPECT: Found 3 key-value pairs
// EXPECT: Lookup 'city': Boston
// EXPECT: Lookup 'pop': 700000
// EXPECT: Lookup 'zip': 2101
// EXPECT: === Edge Cases ===
// EXPECT: Empty object: 0 pairs
// EXPECT: Single pair: key=x val=1
// EXPECT: All JSON parser tests passed!

int printf(const char *fmt, ...);

enum ValueType {
    VAL_STRING,
    VAL_INT
};

struct JsonValue {
    int type;
    char str_val[64];
    int int_val;
};

struct JsonPair {
    char key[64];
    struct JsonValue value;
};

struct JsonObject {
    struct JsonPair pairs[16];
    int count;
};

int str_len(const char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

int str_equal(const char *a, const char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void str_copy(char *dst, const char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int skip_ws(const char *s, int pos) {
    while (s[pos] == ' ' || s[pos] == '\t' || s[pos] == '\n') {
        pos = pos + 1;
    }
    return pos;
}

// Parse a quoted string, return new position, store string in dst
int parse_string(const char *s, int pos, char *dst) {
    int di = 0;
    if (s[pos] != '"') return pos;
    pos = pos + 1;  // skip opening quote
    while (s[pos] != '"' && s[pos] != 0) {
        dst[di] = s[pos];
        di = di + 1;
        pos = pos + 1;
    }
    dst[di] = 0;
    if (s[pos] == '"') pos = pos + 1;  // skip closing quote
    return pos;
}

// Parse an integer, return new position
int parse_int(const char *s, int pos, int *val) {
    int v = 0;
    int neg = 0;
    if (s[pos] == '-') {
        neg = 1;
        pos = pos + 1;
    }
    while (is_digit(s[pos])) {
        v = v * 10 + (s[pos] - '0');
        pos = pos + 1;
    }
    if (neg) v = -v;
    *val = v;
    return pos;
}

// Parse a JSON-like object: {"key":"val","key2":123}
int json_parse(struct JsonObject *obj, const char *s) {
    int pos = 0;
    obj->count = 0;

    pos = skip_ws(s, pos);
    if (s[pos] != '{') return -1;
    pos = pos + 1;

    while (1) {
        pos = skip_ws(s, pos);
        if (s[pos] == '}' || s[pos] == 0) break;

        // Parse key
        if (s[pos] == ',') pos = pos + 1;
        pos = skip_ws(s, pos);

        int idx = obj->count;
        pos = parse_string(s, pos, obj->pairs[idx].key);

        // Expect colon
        pos = skip_ws(s, pos);
        if (s[pos] == ':') pos = pos + 1;
        pos = skip_ws(s, pos);

        // Parse value: string or int
        if (s[pos] == '"') {
            obj->pairs[idx].value.type = VAL_STRING;
            pos = parse_string(s, pos, obj->pairs[idx].value.str_val);
        } else if (is_digit(s[pos]) || s[pos] == '-') {
            obj->pairs[idx].value.type = VAL_INT;
            pos = parse_int(s, pos, &obj->pairs[idx].value.int_val);
        }
        obj->count = obj->count + 1;
    }
    return obj->count;
}

// Lookup a key, return pointer to value or 0
struct JsonValue *json_get(struct JsonObject *obj, const char *key) {
    int i;
    for (i = 0; i < obj->count; i++) {
        if (str_equal(obj->pairs[i].key, key)) {
            return &obj->pairs[i].value;
        }
    }
    return 0;
}

void print_lookup(struct JsonObject *obj, const char *key) {
    struct JsonValue *v = json_get(obj, key);
    if (v == 0) {
        printf("Lookup '%s': (not found)\n", key);
    } else if (v->type == VAL_STRING) {
        printf("Lookup '%s': %s\n", key, v->str_val);
    } else {
        printf("Lookup '%s': %d\n", key, v->int_val);
    }
}

int main(void) {
    struct JsonObject obj;
    int count;
    int i;

    printf("=== JSON Parser Tests ===\n");

    // Test 1: basic object with strings and ints
    const char *json1 = "{\"name\":\"Alice\",\"age\":30,\"score\":95}";
    printf("Parsing: %s\n", json1);
    count = json_parse(&obj, json1);
    printf("Found %d key-value pairs\n", count);
    for (i = 0; i < obj.count; i++) {
        if (obj.pairs[i].value.type == VAL_STRING) {
            printf("Key[%d]: %s = %s (string)\n", i, obj.pairs[i].key, obj.pairs[i].value.str_val);
        } else {
            printf("Key[%d]: %s = %d (int)\n", i, obj.pairs[i].key, obj.pairs[i].value.int_val);
        }
    }

    printf("=== Lookup Tests ===\n");
    print_lookup(&obj, "name");
    print_lookup(&obj, "age");
    print_lookup(&obj, "score");
    print_lookup(&obj, "missing");

    // Test 2: another object
    printf("=== Second Object ===\n");
    const char *json2 = "{\"city\":\"Boston\",\"pop\":700000,\"zip\":2101}";
    printf("Parsing: %s\n", json2);
    count = json_parse(&obj, json2);
    printf("Found %d key-value pairs\n", count);
    print_lookup(&obj, "city");
    print_lookup(&obj, "pop");
    print_lookup(&obj, "zip");

    // Test 3: edge cases
    printf("=== Edge Cases ===\n");
    json_parse(&obj, "{}");
    printf("Empty object: %d pairs\n", obj.count);

    json_parse(&obj, "{\"x\":1}");
    printf("Single pair: key=%s val=%d\n", obj.pairs[0].key, obj.pairs[0].value.int_val);

    printf("All JSON parser tests passed!\n");
    return 0;
}
