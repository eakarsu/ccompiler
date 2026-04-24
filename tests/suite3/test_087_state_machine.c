int printf(const char *fmt, ...);

/* ---- Simple FSM: token type classifier ----
   States: START, IN_NUM, IN_ALPHA, DONE
   Reads one character at a time, classifies "tokens" in a string.
*/

#define ST_START  0
#define ST_NUM    1
#define ST_ALPHA  2
#define ST_DONE   3

int is_digit(char c) { return c >= '0' && c <= '9'; }
int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
int is_alnum(char c) { return is_digit(c) || is_alpha(c); }

/* Count numeric and alphanumeric tokens in a string (simple lexer FSM) */
void count_tokens(char *s, int *nums, int *words) {
    int state = ST_START;
    int i;
    *nums = 0; *words = 0;
    for (i = 0; s[i] != '\0'; i++) {
        char c = s[i];
        switch (state) {
        case ST_START:
            if (is_digit(c))      state = ST_NUM;
            else if (is_alpha(c)) state = ST_ALPHA;
            break;
        case ST_NUM:
            if (!is_digit(c)) {
                (*nums)++;
                state = ST_START;
                if (is_alpha(c)) state = ST_ALPHA;
            }
            break;
        case ST_ALPHA:
            if (!is_alnum(c)) {
                (*words)++;
                state = ST_START;
                if (is_digit(c)) state = ST_NUM;
            }
            break;
        }
    }
    /* Flush last token */
    if (state == ST_NUM)   (*nums)++;
    if (state == ST_ALPHA) (*words)++;
}

/* ---- State transition table FSM ----
   Pattern matcher: detect substring "abc" in input using DFA.
   States: 0 (none matched), 1 (matched 'a'), 2 (matched 'ab'), 3 (matched 'abc')
*/
int next_state_abc(int state, char c) {
    /* Transition table for pattern "abc" */
    if (state == 0) { if (c == 'a') return 1; return 0; }
    if (state == 1) { if (c == 'b') return 2; if (c == 'a') return 1; return 0; }
    if (state == 2) { if (c == 'c') return 3; if (c == 'a') return 1; return 0; }
    /* state == 3: accepted, keep matching for more */
    if (c == 'a') return 1;
    return 0;
}

int count_occurrences_abc(char *s) {
    int state = 0, count = 0, i;
    for (i = 0; s[i] != '\0'; i++) {
        state = next_state_abc(state, s[i]);
        if (state == 3) count++;
    }
    return count;
}

/* ---- FSM: validate a simple integer literal ----
   Accepts: optional sign, then one or more digits
   States: S0 (start), S1 (sign seen), S2 (digits), S_ERR
*/
#define S0    0
#define S1    1
#define S2    2
#define S_ERR 3

int validate_int(char *s) {
    int state = S0, i;
    for (i = 0; s[i] != '\0'; i++) {
        char c = s[i];
        switch (state) {
        case S0:
            if (c == '+' || c == '-') state = S1;
            else if (is_digit(c)) state = S2;
            else state = S_ERR;
            break;
        case S1:
            if (is_digit(c)) state = S2;
            else state = S_ERR;
            break;
        case S2:
            if (!is_digit(c)) state = S_ERR;
            break;
        case S_ERR:
            break;
        }
    }
    return state == S2;
}

/* ---- FSM: run-length encoder state machine ----
   Outputs encoded form as pairs (count, char) stored in arrays
*/
int rle_encode(char *s, int counts[], char chars[]) {
    int n = 0, i;
    if (s[0] == '\0') return 0;
    chars[0] = s[0];
    counts[0] = 1;
    n = 1;
    for (i = 1; s[i] != '\0'; i++) {
        if (s[i] == chars[n-1]) {
            counts[n-1]++;
        } else {
            chars[n] = s[i];
            counts[n] = 1;
            n++;
        }
    }
    return n;
}

/* ---- Traffic light FSM ----
   States: RED=0, GREEN=1, YELLOW=2
   Transitions: RED->GREEN, GREEN->YELLOW, YELLOW->RED
*/
int traffic_next(int state) {
    return (state + 1) % 3;
}

char *traffic_name(int state) {
    if (state == 0) return "RED";
    if (state == 1) return "GREEN";
    return "YELLOW";
}

int main(void) {
    int nums, words, i;

    /* count_tokens */
    {
        char s[] = "hello 42 world 7 foo";
        count_tokens(s, &nums, &words);
        printf("%d\n", nums);   // EXPECT: 2
        printf("%d\n", words);  // EXPECT: 3
    }
    {
        char s[] = "abc123def456";
        count_tokens(s, &nums, &words);
        printf("%d\n", nums);   // EXPECT: 0
        printf("%d\n", words);  // EXPECT: 1
    }
    {
        char s[] = "  42  ";
        count_tokens(s, &nums, &words);
        printf("%d\n", nums);   // EXPECT: 1
        printf("%d\n", words);  // EXPECT: 0
    }

    /* count_occurrences_abc */
    printf("%d\n", count_occurrences_abc("abcabc"));    // EXPECT: 2
    printf("%d\n", count_occurrences_abc("xabcyabc"));  // EXPECT: 2
    printf("%d\n", count_occurrences_abc("ababc"));     // EXPECT: 1
    printf("%d\n", count_occurrences_abc("xyz"));       // EXPECT: 0
    printf("%d\n", count_occurrences_abc("abcabcabc")); // EXPECT: 3

    /* validate_int */
    printf("%d\n", validate_int("123"));    // EXPECT: 1
    printf("%d\n", validate_int("+456"));   // EXPECT: 1
    printf("%d\n", validate_int("-0"));     // EXPECT: 1
    printf("%d\n", validate_int(""));       // EXPECT: 0
    printf("%d\n", validate_int("12a3"));   // EXPECT: 0
    printf("%d\n", validate_int("+-1"));    // EXPECT: 0
    printf("%d\n", validate_int("abc"));    // EXPECT: 0

    /* rle_encode */
    {
        int counts[20];
        char chars[20];
        int n;

        char s1[] = "aaabbbcc";
        n = rle_encode(s1, counts, chars);
        printf("%d\n", n);  // EXPECT: 3
        for (i = 0; i < n; i++) printf("%d%c\n", counts[i], chars[i]);
        // EXPECT: 3a
        // EXPECT: 3b
        // EXPECT: 2c

        char s2[] = "abcde";
        n = rle_encode(s2, counts, chars);
        printf("%d\n", n);  // EXPECT: 5
        for (i = 0; i < n; i++) printf("%d%c\n", counts[i], chars[i]);
        // EXPECT: 1a
        // EXPECT: 1b
        // EXPECT: 1c
        // EXPECT: 1d
        // EXPECT: 1e

        char s3[] = "xxxxxx";
        n = rle_encode(s3, counts, chars);
        printf("%d\n", n);  // EXPECT: 1
        printf("%d%c\n", counts[0], chars[0]);  // EXPECT: 6x
    }

    /* Traffic light FSM */
    {
        int state = 0; /* RED */
        int step;
        for (step = 0; step < 6; step++) {
            printf("%s\n", traffic_name(state));
            state = traffic_next(state);
        }
        // EXPECT: RED
        // EXPECT: GREEN
        // EXPECT: YELLOW
        // EXPECT: RED
        // EXPECT: GREEN
        // EXPECT: YELLOW
    }

    return 0;
}
