int printf(const char *fmt, ...);

int my_strlen(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return (a[i] == b[i]) ? 1 : 0;
}

struct CharBuf {
    char data[256];
    int len;
};

void buf_init(struct CharBuf *b) {
    b->len = 0;
    b->data[0] = 0;
}

void buf_append_char(struct CharBuf *b, int ch) {
    b->data[b->len] = ch;
    b->len = b->len + 1;
    b->data[b->len] = 0;
}

void buf_append_str(struct CharBuf *b, char *s) {
    int i = 0;
    while (s[i] != 0) {
        b->data[b->len] = s[i];
        b->len = b->len + 1;
        i = i + 1;
    }
    b->data[b->len] = 0;
}

void buf_insert(struct CharBuf *b, int pos, int ch) {
    int i = b->len;
    while (i > pos) {
        b->data[i] = b->data[i - 1];
        i = i - 1;
    }
    b->data[pos] = ch;
    b->len = b->len + 1;
    b->data[b->len] = 0;
}

void buf_delete(struct CharBuf *b, int pos) {
    int i = pos;
    while (i < b->len - 1) {
        b->data[i] = b->data[i + 1];
        i = i + 1;
    }
    b->len = b->len - 1;
    b->data[b->len] = 0;
}

void buf_replace(struct CharBuf *b, int pos, int ch) {
    if (pos >= 0 && pos < b->len) {
        b->data[pos] = ch;
    }
}

void buf_clear(struct CharBuf *b) {
    b->len = 0;
    b->data[0] = 0;
}

int buf_char_at(struct CharBuf *b, int pos) {
    return b->data[pos];
}

void buf_append_int(struct CharBuf *b, int val) {
    char tmp[16];
    int i = 0;
    int negative = 0;

    if (val == 0) {
        buf_append_char(b, '0');
        return;
    }
    if (val < 0) {
        negative = 1;
        val = -val;
    }
    while (val > 0) {
        tmp[i] = '0' + (val % 10);
        val = val / 10;
        i = i + 1;
    }
    if (negative) {
        buf_append_char(b, '-');
    }
    i = i - 1;
    while (i >= 0) {
        buf_append_char(b, tmp[i]);
        i = i - 1;
    }
}

void buf_insert_str(struct CharBuf *b, int pos, char *s) {
    int slen = my_strlen(s);
    int i = b->len;
    while (i >= pos) {
        b->data[i + slen] = b->data[i];
        i = i - 1;
    }
    i = 0;
    while (s[i] != 0) {
        b->data[pos + i] = s[i];
        i = i + 1;
    }
    b->len = b->len + slen;
}

int main(void) {
    struct CharBuf buf;
    buf_init(&buf);

    // EXPECT: init_len=0
    printf("init_len=%d\n", buf.len);

    buf_append_char(&buf, 'H');
    buf_append_char(&buf, 'i');
    // EXPECT: hi=Hi
    printf("hi=%s\n", buf.data);

    // EXPECT: hi_len=2
    printf("hi_len=%d\n", buf.len);

    buf_append_str(&buf, " there");
    // EXPECT: hithere=Hi there
    printf("hithere=%s\n", buf.data);

    // EXPECT: ht_len=8
    printf("ht_len=%d\n", buf.len);

    buf_insert(&buf, 2, '!');
    // EXPECT: insert=Hi! there
    printf("insert=%s\n", buf.data);

    // EXPECT: ins_len=9
    printf("ins_len=%d\n", buf.len);

    buf_delete(&buf, 2);
    // EXPECT: delete=Hi there
    printf("delete=%s\n", buf.data);

    // EXPECT: del_len=8
    printf("del_len=%d\n", buf.len);

    buf_replace(&buf, 0, 'h');
    // EXPECT: replace=hi there
    printf("replace=%s\n", buf.data);

    // EXPECT: char_0=104
    printf("char_0=%d\n", buf_char_at(&buf, 0));

    buf_clear(&buf);
    // EXPECT: clear_len=0
    printf("clear_len=%d\n", buf.len);

    buf_append_str(&buf, "val=");
    buf_append_int(&buf, 42);
    // EXPECT: fmt1=val=42
    printf("fmt1=%s\n", buf.data);

    buf_clear(&buf);
    buf_append_str(&buf, "x=");
    buf_append_int(&buf, -7);
    buf_append_str(&buf, " y=");
    buf_append_int(&buf, 100);
    // EXPECT: fmt2=x=-7 y=100
    printf("fmt2=%s\n", buf.data);

    // EXPECT: fmt2_len=10
    printf("fmt2_len=%d\n", buf.len);

    buf_clear(&buf);
    buf_append_str(&buf, "abcdef");
    buf_insert_str(&buf, 3, "XY");
    // EXPECT: ins_str=abcXYdef
    printf("ins_str=%s\n", buf.data);

    // EXPECT: ins_str_len=8
    printf("ins_str_len=%d\n", buf.len);

    buf_clear(&buf);
    buf_append_int(&buf, 0);
    // EXPECT: zero=0
    printf("zero=%s\n", buf.data);

    buf_clear(&buf);
    int i = 0;
    while (i < 5) {
        buf_append_int(&buf, i);
        if (i < 4) buf_append_char(&buf, ',');
        i = i + 1;
    }
    // EXPECT: list=0,1,2,3,4
    printf("list=%s\n", buf.data);

    // EXPECT: list_len=9
    printf("list_len=%d\n", buf.len);

    return 0;
}
