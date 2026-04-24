int printf(const char *fmt, ...);

// Markdown-like formatter: detect headers (#), bold (**text**), lists (- item)
// and output structured information about the parsed elements

struct MDElement {
    int type;       // 0=text, 1=header, 2=bold, 3=list_item
    int level;      // header level (1-3) or list depth
    char text[64];
    int text_len;
};

struct MDDoc {
    struct MDElement elems[32];
    int count;
    int header_count;
    int bold_count;
    int list_count;
};

int str_len(char *s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

void str_ncpy(char *dst, char *src, int n) {
    int i;
    for (i = 0; i < n && i < 63; i++) dst[i] = src[i];
    dst[i] = 0;
}

void init_doc(struct MDDoc *doc) {
    doc->count = 0;
    doc->header_count = 0;
    doc->bold_count = 0;
    doc->list_count = 0;
}

void add_elem(struct MDDoc *doc, int type, int level, char *text, int tlen) {
    int idx = doc->count;
    doc->elems[idx].type = type;
    doc->elems[idx].level = level;
    str_ncpy(doc->elems[idx].text, text, tlen);
    doc->elems[idx].text_len = tlen;
    doc->count++;
    if (type == 1) doc->header_count++;
    if (type == 2) doc->bold_count++;
    if (type == 3) doc->list_count++;
}

int count_leading_char(char *s, char c) {
    int n = 0;
    while (s[n] == c) n++;
    return n;
}

void parse_line(char *line, struct MDDoc *doc) {
    int len = str_len(line);
    if (len == 0) return;

    int pos = 0;

    // Check for header: # ## ###
    if (line[0] == '#') {
        int level = count_leading_char(line, '#');
        if (level > 3) level = 3;
        pos = level;
        while (line[pos] == ' ') pos++;
        add_elem(doc, 1, level, line + pos, len - pos);
        return;
    }

    // Check for list item: - text or * text
    if ((line[0] == '-' || line[0] == '*') && line[1] == ' ') {
        pos = 2;
        add_elem(doc, 3, 1, line + pos, len - pos);
        return;
    }

    // Check for indented list: spaces then - or *
    int spaces = 0;
    while (line[spaces] == ' ') spaces++;
    if (spaces > 0 && (line[spaces] == '-' || line[spaces] == '*') && line[spaces + 1] == ' ') {
        int depth = spaces / 2 + 1;
        pos = spaces + 2;
        add_elem(doc, 3, depth, line + pos, len - pos);
        return;
    }

    // Scan for **bold** markers within text
    int i = 0;
    int seg_start = 0;
    while (i < len) {
        if (i + 1 < len && line[i] == '*' && line[i + 1] == '*') {
            // Output text before bold
            if (i > seg_start) {
                add_elem(doc, 0, 0, line + seg_start, i - seg_start);
            }
            i += 2;
            int bold_start = i;
            while (i + 1 < len && !(line[i] == '*' && line[i + 1] == '*')) i++;
            add_elem(doc, 2, 0, line + bold_start, i - bold_start);
            if (i + 1 < len) i += 2;
            seg_start = i;
        } else {
            i++;
        }
    }
    if (seg_start < len) {
        add_elem(doc, 0, 0, line + seg_start, len - seg_start);
    }
}

char *type_name(int t) {
    if (t == 0) return "text";
    if (t == 1) return "header";
    if (t == 2) return "bold";
    if (t == 3) return "list";
    return "unknown";
}

int main() {
    struct MDDoc doc;
    init_doc(&doc);

    parse_line("# Welcome", &doc);
    parse_line("## Introduction", &doc);
    parse_line("### Details", &doc);

    printf("%d\n", doc.count);
    // EXPECT: 3
    printf("%d\n", doc.header_count);
    // EXPECT: 3

    printf("%s\n", type_name(doc.elems[0].type));
    // EXPECT: header
    printf("%d\n", doc.elems[0].level);
    // EXPECT: 1
    printf("%s\n", doc.elems[0].text);
    // EXPECT: Welcome

    printf("%d\n", doc.elems[1].level);
    // EXPECT: 2
    printf("%s\n", doc.elems[1].text);
    // EXPECT: Introduction

    printf("%d\n", doc.elems[2].level);
    // EXPECT: 3
    printf("%s\n", doc.elems[2].text);
    // EXPECT: Details

    // List items
    parse_line("- first item", &doc);
    parse_line("- second item", &doc);
    parse_line("  - nested item", &doc);

    printf("%d\n", doc.list_count);
    // EXPECT: 3
    printf("%s\n", doc.elems[3].text);
    // EXPECT: first item
    printf("%d\n", doc.elems[3].level);
    // EXPECT: 1
    printf("%s\n", doc.elems[4].text);
    // EXPECT: second item
    printf("%s\n", doc.elems[5].text);
    // EXPECT: nested item
    printf("%d\n", doc.elems[5].level);
    // EXPECT: 2

    // Bold text
    parse_line("Look:**important**+more", &doc);

    printf("%d\n", doc.bold_count);
    // EXPECT: 1

    // The line gets split: "Look:" (text), "important" (bold), "+more" (text)
    printf("%s\n", type_name(doc.elems[6].type));
    // EXPECT: text
    printf("%s\n", doc.elems[6].text);
    // EXPECT: Look:
    printf("%s\n", type_name(doc.elems[7].type));
    // EXPECT: bold
    printf("%s\n", doc.elems[7].text);
    // EXPECT: important
    printf("%s\n", type_name(doc.elems[8].type));
    // EXPECT: text
    printf("%s\n", doc.elems[8].text);
    // EXPECT: +more

    printf("%d\n", doc.count);
    // EXPECT: 9

    // Multiple bold in one line
    struct MDDoc doc2;
    init_doc(&doc2);
    parse_line("**one**+and+**two**", &doc2);
    printf("%d\n", doc2.bold_count);
    // EXPECT: 2
    printf("%s\n", doc2.elems[0].text);
    // EXPECT: one
    printf("%s\n", doc2.elems[1].text);
    // EXPECT: +and+
    printf("%s\n", doc2.elems[2].text);
    // EXPECT: two

    // Plain text
    struct MDDoc doc3;
    init_doc(&doc3);
    parse_line("Just plain text here", &doc3);
    printf("%d\n", doc3.count);
    // EXPECT: 1
    printf("%s\n", doc3.elems[0].text);
    // EXPECT: Just plain text here

    return 0;
}
