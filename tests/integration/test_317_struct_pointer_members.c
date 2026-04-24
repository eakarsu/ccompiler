int printf(const char *fmt, ...);
// EXPECT: length: 5\nchar[0]: h\nchar[4]: o\ncount 'l': 2\nequal: 1\nequal after change: 0\nref get: 42\nvalue after update: 99\nref get: 99\ninvalid ref: -1
// Test: struct with pointer members

struct StringView {
    char *data;
    int len;
};

struct IntRef {
    int *ptr;
    int valid;
};

int sv_length(struct StringView *sv) {
    return sv->len;
}

char sv_char_at(struct StringView *sv, int idx) {
    if (idx >= 0 && idx < sv->len) {
        return sv->data[idx];
    }
    return 0;
}

int sv_equals(struct StringView *a, struct StringView *b) {
    if (a->len != b->len) return 0;
    int i;
    for (i = 0; i < a->len; i++) {
        if (a->data[i] != b->data[i]) return 0;
    }
    return 1;
}

int sv_count_char(struct StringView *sv, char c) {
    int count = 0;
    int i;
    for (i = 0; i < sv->len; i++) {
        if (sv->data[i] == c) count = count + 1;
    }
    return count;
}

void intref_set(struct IntRef *ref, int *target) {
    ref->ptr = target;
    ref->valid = 1;
}

int intref_get(struct IntRef *ref) {
    if (ref->valid && ref->ptr) {
        return *(ref->ptr);
    }
    return -1;
}

void intref_update(struct IntRef *ref, int val) {
    if (ref->valid && ref->ptr) {
        *(ref->ptr) = val;
    }
}

int main(void) {
    char buf1[6];
    buf1[0] = 'h'; buf1[1] = 'e'; buf1[2] = 'l';
    buf1[3] = 'l'; buf1[4] = 'o'; buf1[5] = 0;

    struct StringView sv1;
    sv1.data = buf1;
    sv1.len = 5;

    printf("length: %d\n", sv_length(&sv1));
    printf("char[0]: %c\n", sv_char_at(&sv1, 0));
    printf("char[4]: %c\n", sv_char_at(&sv1, 4));
    printf("count 'l': %d\n", sv_count_char(&sv1, 'l'));

    char buf2[6];
    buf2[0] = 'h'; buf2[1] = 'e'; buf2[2] = 'l';
    buf2[3] = 'l'; buf2[4] = 'o'; buf2[5] = 0;

    struct StringView sv2;
    sv2.data = buf2;
    sv2.len = 5;
    printf("equal: %d\n", sv_equals(&sv1, &sv2));

    buf2[0] = 'w';
    printf("equal after change: %d\n", sv_equals(&sv1, &sv2));

    int value = 42;
    struct IntRef ref;
    intref_set(&ref, &value);
    printf("ref get: %d\n", intref_get(&ref));

    intref_update(&ref, 99);
    printf("value after update: %d\n", value);
    printf("ref get: %d\n", intref_get(&ref));

    struct IntRef invalid;
    invalid.valid = 0;
    invalid.ptr = 0;
    printf("invalid ref: %d\n", intref_get(&invalid));

    return 0;
}
