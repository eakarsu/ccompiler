int printf(const char *fmt, ...);

// Simple markdown-to-text converter
// Handles: headings (#), bold (**), italic (*), inline code (`), lists (- and *)

int str_len(char *s) {
    int n = 0;
    while (s[n] != 0) n = n + 1;
    return n;
}

// Check if line starts with given prefix
int starts_with(char *line, char *prefix) {
    int i = 0;
    while (prefix[i] != 0) {
        if (line[i] != prefix[i]) return 0;
        i = i + 1;
    }
    return 1;
}

// Count leading # characters
int count_hashes(char *line) {
    int n = 0;
    while (line[n] == '#') n = n + 1;
    return n;
}

// Strip inline markdown from text, write to output
// Removes **, *, ` markers
void strip_inline(char *input, char *output) {
    int i = 0;
    int o = 0;

    while (input[i] != 0) {
        if (input[i] == '*' && input[i + 1] == '*') {
            // Bold marker, skip both
            i = i + 2;
        } else if (input[i] == '*') {
            // Italic marker, skip
            i = i + 1;
        } else if (input[i] == '`') {
            // Code marker, skip
            i = i + 1;
        } else {
            output[o] = input[i];
            o = o + 1;
            i = i + 1;
        }
    }
    output[o] = 0;
}

// Process a single line of markdown
// Returns line type: 0=paragraph, 1=heading, 2=list item, 3=empty
int process_line(char *input, char *output) {
    int i = 0;
    char stripped[256];
    int hashes;

    // Skip leading spaces
    while (input[i] == ' ') i = i + 1;

    if (input[i] == 0 || input[i] == '\n') {
        output[0] = 0;
        return 3; // empty
    }

    if (input[i] == '#') {
        hashes = 0;
        while (input[i] == '#') {
            hashes = hashes + 1;
            i = i + 1;
        }
        // Skip space after #
        while (input[i] == ' ') i = i + 1;
        strip_inline(&input[i], output);
        return 1; // heading
    }

    if (input[i] == '-' && input[i + 1] == ' ') {
        i = i + 2;
        strip_inline(&input[i], stripped);
        // Format as "  - text"
        output[0] = ' ';
        output[1] = ' ';
        output[2] = '-';
        output[3] = ' ';
        int j = 0;
        while (stripped[j] != 0) {
            output[4 + j] = stripped[j];
            j = j + 1;
        }
        output[4 + j] = 0;
        return 2; // list
    }

    // Regular paragraph
    strip_inline(&input[i], output);
    return 0;
}

void test_strip_inline() {
    char out[256];

    strip_inline("hello world", out);
    printf("plain: %s\n", out);
    // EXPECT: plain: hello world

    strip_inline("**bold** text", out);
    printf("bold: %s\n", out);
    // EXPECT: bold: bold text

    strip_inline("*italic* text", out);
    printf("italic: %s\n", out);
    // EXPECT: italic: italic text

    strip_inline("`code` here", out);
    printf("code: %s\n", out);
    // EXPECT: code: code here

    strip_inline("**a** and *b* and `c`", out);
    printf("mixed: %s\n", out);
    // EXPECT: mixed: a and b and c

    strip_inline("no **markers", out);
    printf("unmatched: %s\n", out);
    // EXPECT: unmatched: no markers

    strip_inline("", out);
    printf("empty strip:%s\n", out);
    // EXPECT: empty strip:
}

void test_headings() {
    char out[256];
    int type;

    type = process_line("# Title", out);
    printf("h1 type=%d text=%s\n", type, out);
    // EXPECT: h1 type=1 text=Title

    type = process_line("## Subtitle", out);
    printf("h2 type=%d text=%s\n", type, out);
    // EXPECT: h2 type=1 text=Subtitle

    type = process_line("### Deep", out);
    printf("h3 type=%d text=%s\n", type, out);
    // EXPECT: h3 type=1 text=Deep

    type = process_line("# **Bold Title**", out);
    printf("bold h1 type=%d text=%s\n", type, out);
    // EXPECT: bold h1 type=1 text=Bold Title
}

void test_lists() {
    char out[256];
    int type;

    type = process_line("- item one", out);
    printf("list type=%d text=%s\n", type, out);
    // EXPECT: list type=2 text=  - item one

    type = process_line("- **bold** item", out);
    printf("bold list type=%d text=%s\n", type, out);
    // EXPECT: bold list type=2 text=  - bold item

    type = process_line("- `code` item", out);
    printf("code list type=%d text=%s\n", type, out);
    // EXPECT: code list type=2 text=  - code item
}

void test_paragraphs() {
    char out[256];
    int type;

    type = process_line("Just some text.", out);
    printf("para type=%d text=%s\n", type, out);
    // EXPECT: para type=0 text=Just some text.

    type = process_line("Text with **bold** and *italic*.", out);
    printf("fancy type=%d text=%s\n", type, out);
    // EXPECT: fancy type=0 text=Text with bold and italic.

    type = process_line("", out);
    printf("empty type=%d\n", type);
    // EXPECT: empty type=3
}

void test_full_doc() {
    char *lines[8];
    char out[256];
    int types[8];
    int i;

    lines[0] = "# My Doc";
    lines[1] = "This is a **test**.";
    lines[2] = "- item *one*";
    lines[3] = "- item `two`";
    lines[4] = "## Section";
    lines[5] = "More text.";

    printf("--- document ---\n");
    // EXPECT: --- document ---
    for (i = 0; i < 6; i = i + 1) {
        types[i] = process_line(lines[i], out);
        printf("%s\n", out);
    }
    // EXPECT: My Doc
    // EXPECT: This is a test.
    // EXPECT:   - item one
    // EXPECT:   - item two
    // EXPECT: Section
    // EXPECT: More text.

    printf("types:");
    for (i = 0; i < 6; i = i + 1) {
        printf(" %d", types[i]);
    }
    printf("\n");
    // EXPECT: types: 1 0 2 2 1 0

    printf("md converter done\n");
    // EXPECT: md converter done
}

int main() {
    test_strip_inline();
    test_headings();
    test_lists();
    test_paragraphs();
    test_full_doc();
    return 0;
}
