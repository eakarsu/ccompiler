int printf(const char *fmt, ...);
// EXPECT: After insert: "Hello World" (len=11, cursor=11)\nAfter insert at 5: "Hello Beautiful World" (len=21, cursor=15)\nAfter prepend: "Oh, Hello Beautiful World" (len=25, cursor=4)\nAfter append: "Oh, Hello Beautiful World!" (len=26, cursor=26)\nAfter delete comma: "Oh Hello Beautiful World!" (len=25, cursor=2)\nText: Oh Hello Beautiful World!\nLength: 25
// Text editor buffer (gap buffer with array)

struct GapBuffer {
    char buf[64];
    int gap_start;
    int gap_end;
    int capacity;
};

void gb_init(struct GapBuffer *g) {
    int i;
    for (i = 0; i < 64; i++) g->buf[i] = 0;
    g->gap_start = 0;
    g->gap_end = 64;
    g->capacity = 64;
}

int gb_length(struct GapBuffer *g) {
    return g->capacity - (g->gap_end - g->gap_start);
}

void gb_move_cursor(struct GapBuffer *g, int pos) {
    while (g->gap_start < pos) {
        g->buf[g->gap_start] = g->buf[g->gap_end];
        g->gap_start++;
        g->gap_end++;
    }
    while (g->gap_start > pos) {
        g->gap_start--;
        g->gap_end--;
        g->buf[g->gap_end] = g->buf[g->gap_start];
    }
}

void gb_insert_char(struct GapBuffer *g, char c) {
    if (g->gap_start >= g->gap_end) return;
    g->buf[g->gap_start] = c;
    g->gap_start++;
}

void gb_insert_str(struct GapBuffer *g, char *s) {
    int i = 0;
    while (s[i] != 0) {
        gb_insert_char(g, s[i]);
        i++;
    }
}

void gb_delete_before(struct GapBuffer *g) {
    if (g->gap_start > 0) {
        g->gap_start--;
    }
}

void gb_delete_after(struct GapBuffer *g) {
    if (g->gap_end < g->capacity) {
        g->gap_end++;
    }
}

char gb_char_at(struct GapBuffer *g, int pos) {
    if (pos < g->gap_start) return g->buf[pos];
    return g->buf[g->gap_end + (pos - g->gap_start)];
}

void gb_print(struct GapBuffer *g) {
    int i;
    int len = gb_length(g);
    printf("\"");
    for (i = 0; i < len; i++) {
        printf("%c", gb_char_at(g, i));
    }
    printf("\" (len=%d, cursor=%d)\n", len, g->gap_start);
}

void gb_get_text(struct GapBuffer *g, char *out) {
    int i;
    int len = gb_length(g);
    for (i = 0; i < len; i++) {
        out[i] = gb_char_at(g, i);
    }
    out[len] = 0;
}

int main(void) {
    struct GapBuffer g;
    char text[64];

    gb_init(&g);

    gb_insert_str(&g, "Hello World");
    printf("After insert: ");
    gb_print(&g);

    gb_move_cursor(&g, 5);
    gb_insert_str(&g, " Beautiful");
    printf("After insert at 5: ");
    gb_print(&g);

    gb_move_cursor(&g, 0);
    gb_insert_str(&g, "Oh, ");
    printf("After prepend: ");
    gb_print(&g);

    gb_move_cursor(&g, gb_length(&g));
    gb_insert_char(&g, '!');
    printf("After append: ");
    gb_print(&g);

    gb_move_cursor(&g, 3);
    gb_delete_before(&g);
    printf("After delete comma: ");
    gb_print(&g);

    gb_get_text(&g, text);
    printf("Text: %s\n", text);
    printf("Length: %d\n", gb_length(&g));

    return 0;
}
