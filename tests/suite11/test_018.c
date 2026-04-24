int printf(const char *fmt, ...);

// Sentence parser: tokenizes and analyzes simple sentences
// Identifies words, numbers, punctuation, and sentence structure

enum TokenType {
    TOK_WORD,
    TOK_NUMBER,
    TOK_PUNCT,
    TOK_END
};

struct Token {
    int type;
    char text[32];
    int value;  // for numbers
};

struct Sentence {
    struct Token tokens[20];
    int ntokens;
    int nwords;
    int nnumbers;
    int npunct;
};

int is_letter(char c) {
    if (c >= 'a' && c <= 'z') return 1;
    if (c >= 'A' && c <= 'Z') return 1;
    return 0;
}

int is_digit(char c) {
    if (c >= '0' && c <= '9') return 1;
    return 0;
}

int is_punct(char c) {
    if (c == '.' || c == ',' || c == '!' || c == '?' || c == ';' || c == ':') return 1;
    return 0;
}

void parse_sentence(char *input, struct Sentence *sent) {
    int i = 0;
    int ti;
    int n = 0;

    sent->nwords = 0;
    sent->nnumbers = 0;
    sent->npunct = 0;

    while (input[i] != 0 && n < 20) {
        // Skip spaces
        while (input[i] == ' ') i = i + 1;
        if (input[i] == 0) break;

        if (is_letter(input[i])) {
            ti = 0;
            while (is_letter(input[i]) && ti < 31) {
                sent->tokens[n].text[ti] = input[i];
                ti = ti + 1;
                i = i + 1;
            }
            sent->tokens[n].text[ti] = 0;
            sent->tokens[n].type = TOK_WORD;
            sent->tokens[n].value = 0;
            sent->nwords = sent->nwords + 1;
            n = n + 1;
        } else if (is_digit(input[i])) {
            ti = 0;
            int val = 0;
            while (is_digit(input[i]) && ti < 31) {
                sent->tokens[n].text[ti] = input[i];
                val = val * 10 + (input[i] - '0');
                ti = ti + 1;
                i = i + 1;
            }
            sent->tokens[n].text[ti] = 0;
            sent->tokens[n].type = TOK_NUMBER;
            sent->tokens[n].value = val;
            sent->nnumbers = sent->nnumbers + 1;
            n = n + 1;
        } else if (is_punct(input[i])) {
            sent->tokens[n].text[0] = input[i];
            sent->tokens[n].text[1] = 0;
            sent->tokens[n].type = TOK_PUNCT;
            sent->tokens[n].value = 0;
            sent->npunct = sent->npunct + 1;
            n = n + 1;
            i = i + 1;
        } else {
            i = i + 1; // skip unknown
        }
    }
    sent->ntokens = n;
}

char *type_name(int t) {
    if (t == TOK_WORD) return "WORD";
    if (t == TOK_NUMBER) return "NUM";
    if (t == TOK_PUNCT) return "PUNCT";
    return "END";
}

// Check if sentence ends with period, question mark, etc.
int is_complete(struct Sentence *sent) {
    if (sent->ntokens == 0) return 0;
    int last = sent->ntokens - 1;
    if (sent->tokens[last].type != TOK_PUNCT) return 0;
    char c = sent->tokens[last].text[0];
    if (c == '.' || c == '!' || c == '?') return 1;
    return 0;
}

// Count syllables (simplified: count vowel groups)
int count_vowels(char *word) {
    int i = 0;
    int count = 0;
    int in_vowel = 0;

    while (word[i] != 0) {
        char c = word[i];
        if (c >= 'A' && c <= 'Z') c = c + 32;
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            if (in_vowel == 0) {
                count = count + 1;
                in_vowel = 1;
            }
        } else {
            in_vowel = 0;
        }
        i = i + 1;
    }
    return count;
}

void test_basic_parse() {
    struct Sentence sent;

    parse_sentence("Hello world.", &sent);
    printf("tokens: %d\n", sent.ntokens);
    // EXPECT: tokens: 3
    printf("words: %d nums: %d punct: %d\n", sent.nwords, sent.nnumbers, sent.npunct);
    // EXPECT: words: 2 nums: 0 punct: 1
    printf("t0: %s %s\n", type_name(sent.tokens[0].type), sent.tokens[0].text);
    // EXPECT: t0: WORD Hello
    printf("t1: %s %s\n", type_name(sent.tokens[1].type), sent.tokens[1].text);
    // EXPECT: t1: WORD world
    printf("t2: %s %s\n", type_name(sent.tokens[2].type), sent.tokens[2].text);
    // EXPECT: t2: PUNCT .
    printf("complete: %d\n", is_complete(&sent));
    // EXPECT: complete: 1
}

void test_mixed() {
    struct Sentence sent;

    parse_sentence("I have 42 cats, and 7 dogs!", &sent);
    printf("mixed tokens: %d\n", sent.ntokens);
    // EXPECT: mixed tokens: 9
    printf("mixed words: %d nums: %d punct: %d\n", sent.nwords, sent.nnumbers, sent.npunct);
    // EXPECT: mixed words: 5 nums: 2 punct: 2
    printf("num1: %s val=%d\n", sent.tokens[2].text, sent.tokens[2].value);
    // EXPECT: num1: 42 val=42
    printf("num2: %s val=%d\n", sent.tokens[6].text, sent.tokens[6].value);
    // EXPECT: num2: 7 val=7
    printf("complete: %d\n", is_complete(&sent));
    // EXPECT: complete: 1
}

void test_incomplete() {
    struct Sentence sent;

    parse_sentence("waiting for", &sent);
    printf("incomplete: %d\n", is_complete(&sent));
    // EXPECT: incomplete: 0

    parse_sentence("done;", &sent);
    printf("semicolon: %d\n", is_complete(&sent));
    // EXPECT: semicolon: 0

    parse_sentence("really?", &sent);
    printf("question: %d\n", is_complete(&sent));
    // EXPECT: question: 1
}

void test_vowels() {
    printf("hello vowels: %d\n", count_vowels("hello"));
    // EXPECT: hello vowels: 2
    printf("beautiful vowels: %d\n", count_vowels("beautiful"));
    // EXPECT: beautiful vowels: 3
    printf("rhythm vowels: %d\n", count_vowels("rhythm"));
    // EXPECT: rhythm vowels: 0
    printf("aeiou vowels: %d\n", count_vowels("aeiou"));
    // EXPECT: aeiou vowels: 1
    printf("queue vowels: %d\n", count_vowels("queue"));
    // EXPECT: queue vowels: 1
}

void test_numbers() {
    struct Sentence sent;
    int sum = 0;
    int i;

    parse_sentence("add 10 and 20 and 30.", &sent);
    for (i = 0; i < sent.ntokens; i = i + 1) {
        if (sent.tokens[i].type == TOK_NUMBER) {
            sum = sum + sent.tokens[i].value;
        }
    }
    printf("number sum: %d\n", sum);
    // EXPECT: number sum: 60
}

void test_empty() {
    struct Sentence sent;

    parse_sentence("", &sent);
    printf("empty tokens: %d\n", sent.ntokens);
    // EXPECT: empty tokens: 0
    printf("empty complete: %d\n", is_complete(&sent));
    // EXPECT: empty complete: 0

    parse_sentence("   ", &sent);
    printf("spaces tokens: %d\n", sent.ntokens);
    // EXPECT: spaces tokens: 0

    printf("parser tests done\n");
    // EXPECT: parser tests done
}

int main() {
    test_basic_parse();
    test_mixed();
    test_incomplete();
    test_vowels();
    test_numbers();
    test_empty();
    return 0;
}
