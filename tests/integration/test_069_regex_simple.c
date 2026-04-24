// EXPECT: === Literal Matching ===\nmatch 'abc' vs 'abc': 1\nmatch 'abc' vs 'abd': 0\nmatch 'abc' vs 'ab': 0\nmatch '' vs '': 1\n=== Dot (any char) ===\nmatch 'a.c' vs 'abc': 1\nmatch 'a.c' vs 'aXc': 1\nmatch 'a.c' vs 'ac': 0\nmatch '...' vs 'abc': 1\nmatch '...' vs 'ab': 0\n=== Star (zero or more) ===\nmatch 'a*' vs '': 1\nmatch 'a*' vs 'a': 1\nmatch 'a*' vs 'aaa': 1\nmatch 'a*' vs 'b': 0\nmatch 'a*b' vs 'b': 1\nmatch 'a*b' vs 'aab': 1\nmatch 'a*b' vs 'aac': 0\n=== Plus (one or more) ===\nmatch 'a+' vs '': 0\nmatch 'a+' vs 'a': 1\nmatch 'a+' vs 'aaa': 1\nmatch 'a+b' vs 'b': 0\nmatch 'a+b' vs 'ab': 1\nmatch 'a+b' vs 'aab': 1\n=== Question (optional) ===\nmatch 'a?' vs '': 1\nmatch 'a?' vs 'a': 1\nmatch 'a?b' vs 'b': 1\nmatch 'a?b' vs 'ab': 1\nmatch 'a?b' vs 'aab': 0\n=== Char Class [a-z] ===\nmatch '[abc]' vs 'a': 1\nmatch '[abc]' vs 'b': 1\nmatch '[abc]' vs 'd': 0\nmatch '[a-z]' vs 'g': 1\nmatch '[a-z]' vs 'G': 0\nmatch '[0-9]' vs '5': 1\nmatch '[0-9]' vs 'a': 0\n=== Anchors ^ and $ ===\nsearch 'abc' in 'xyzabc': 1\nanchored '^abc' in 'xyzabc': 0\nanchored '^abc' in 'abcxyz': 1\nanchored 'abc$' in 'xyzabc': 1\nanchored 'abc$' in 'abcxyz': 0\nanchored '^abc$' in 'abc': 1\nanchored '^abc$' in 'abcd': 0\n=== Complex patterns ===\nmatch 'a.*b' vs 'ab': 1\nmatch 'a.*b' vs 'aXYZb': 1\nmatch 'a.*b' vs 'a': 0\nmatch '[abc]+' vs 'abcba': 1\nmatch 'a.b.c' vs 'aXbYc': 1\nmatch 'a?b?c?' vs '': 1\nmatch 'a?b?c?' vs 'abc': 1\nmatch 'a?b?c?' vs 'ac': 1\n=== Search (find pattern in text) ===\nfind 'bc' in 'abcdef': found at 1\nfind 'xyz' in 'abcdef': not found\nfind 'a.c' in 'XaYcZ': found at 1\nfind 'a.c' in 'XabcZ': found at 1\n=== Count matches ===\ncount 'ab' in 'ababab': 3\ncount 'a' in 'banana': 3\nDone.\n
int printf(const char *fmt, ...);

// ---- String utilities ----

int str_len(char *s) {
    int n;
    n = 0;
    while (s[n] != 0) {
        n = n + 1;
    }
    return n;
}

// ---- Pattern parsing structures ----
// Regex token types:
// 0 = literal char
// 1 = dot (any)
// 2 = char class [...]
// 3 = end of pattern

struct RegexToken {
    int type;
    char ch;          // for literal
    char class_chars[64]; // for char class
    int class_len;
    int quantifier;   // 0=none, 1=star, 2=plus, 3=question
};

struct RegexToken tokens[64];
int num_tokens;

// ---- Parse character class ----

int parse_char_class(char *pat, int pos, int plen, char *out, int *out_len) {
    int olen;
    int i;
    char c;
    char start;
    char end;

    olen = 0;
    i = pos;

    while (i < plen && pat[i] != ']') {
        if (i + 2 < plen && pat[i + 1] == '-' && pat[i + 2] != ']') {
            // Range a-z
            start = pat[i];
            end = pat[i + 2];
            c = start;
            while (c <= end) {
                out[olen] = c;
                olen = olen + 1;
                c = c + 1;
            }
            i = i + 3;
        } else {
            out[olen] = pat[i];
            olen = olen + 1;
            i = i + 1;
        }
    }

    *out_len = olen;
    if (i < plen && pat[i] == ']') {
        return i + 1; // skip ']'
    }
    return i;
}

// ---- Parse regex pattern into tokens ----

void parse_pattern(char *pat, int plen) {
    int i;
    int j;
    num_tokens = 0;
    i = 0;

    while (i < plen) {
        if (pat[i] == '.') {
            tokens[num_tokens].type = 1;
            tokens[num_tokens].ch = '.';
            tokens[num_tokens].class_len = 0;
            tokens[num_tokens].quantifier = 0;
            i = i + 1;
        } else if (pat[i] == '[') {
            tokens[num_tokens].type = 2;
            tokens[num_tokens].ch = 0;
            i = parse_char_class(pat, i + 1, plen, tokens[num_tokens].class_chars, &tokens[num_tokens].class_len);
            tokens[num_tokens].quantifier = 0;
        } else if (pat[i] == '^' || pat[i] == '$') {
            // Handle anchors as special literals; will be processed separately
            tokens[num_tokens].type = 0;
            tokens[num_tokens].ch = pat[i];
            tokens[num_tokens].class_len = 0;
            tokens[num_tokens].quantifier = 0;
            i = i + 1;
        } else {
            tokens[num_tokens].type = 0;
            tokens[num_tokens].ch = pat[i];
            tokens[num_tokens].class_len = 0;
            tokens[num_tokens].quantifier = 0;
            i = i + 1;
        }

        // Check for quantifier
        if (i < plen) {
            if (pat[i] == '*') {
                tokens[num_tokens].quantifier = 1;
                i = i + 1;
            } else if (pat[i] == '+') {
                tokens[num_tokens].quantifier = 2;
                i = i + 1;
            } else if (pat[i] == '?') {
                tokens[num_tokens].quantifier = 3;
                i = i + 1;
            }
        }

        num_tokens = num_tokens + 1;
    }
}

// ---- Match a single token against a character ----

int token_matches_char(int tok_idx, char c) {
    int i;
    if (tokens[tok_idx].type == 0) {
        // Literal
        return tokens[tok_idx].ch == c;
    } else if (tokens[tok_idx].type == 1) {
        // Dot - matches anything except null
        return c != 0;
    } else if (tokens[tok_idx].type == 2) {
        // Char class
        for (i = 0; i < tokens[tok_idx].class_len; i = i + 1) {
            if (tokens[tok_idx].class_chars[i] == c) {
                return 1;
            }
        }
        return 0;
    }
    return 0;
}

// ---- Recursive matching engine ----

int match_here(int tok_idx, char *text, int ti, int tlen) {
    int q;

    // Base case: all tokens consumed
    if (tok_idx >= num_tokens) {
        return ti == tlen;
    }

    q = tokens[tok_idx].quantifier;

    if (q == 1) {
        // Star: match zero or more
        // Try matching zero first (non-greedy for simplicity)
        // Actually let's do greedy then backtrack
        {
            int save;
            save = ti;
            // Greedy: consume as many as possible
            while (ti < tlen && token_matches_char(tok_idx, text[ti])) {
                ti = ti + 1;
            }
            // Try from max down to min (zero)
            while (ti >= save) {
                if (match_here(tok_idx + 1, text, ti, tlen)) {
                    return 1;
                }
                ti = ti - 1;
            }
            return 0;
        }
    } else if (q == 2) {
        // Plus: match one or more
        if (ti >= tlen || !token_matches_char(tok_idx, text[ti])) {
            return 0;
        }
        {
            int save;
            ti = ti + 1; // must match at least one
            save = ti;
            while (ti < tlen && token_matches_char(tok_idx, text[ti])) {
                ti = ti + 1;
            }
            while (ti >= save) {
                if (match_here(tok_idx + 1, text, ti, tlen)) {
                    return 1;
                }
                ti = ti - 1;
            }
            return 0;
        }
    } else if (q == 3) {
        // Question: match zero or one
        // Try with one match first
        if (ti < tlen && token_matches_char(tok_idx, text[ti])) {
            if (match_here(tok_idx + 1, text, ti + 1, tlen)) {
                return 1;
            }
        }
        // Try with zero matches
        return match_here(tok_idx + 1, text, ti, tlen);
    } else {
        // No quantifier: must match exactly one
        if (ti >= tlen) {
            return 0;
        }
        if (token_matches_char(tok_idx, text[ti])) {
            return match_here(tok_idx + 1, text, ti + 1, tlen);
        }
        return 0;
    }
}

// ---- Full match (pattern must match entire string) ----

int regex_match(char *pattern, char *text) {
    int plen;
    int tlen;
    plen = str_len(pattern);
    tlen = str_len(text);
    parse_pattern(pattern, plen);
    return match_here(0, text, 0, tlen);
}

// ---- Anchored match with ^ and $ ----

int regex_anchored_match(char *pattern, char *text) {
    int plen;
    int tlen;
    int has_start_anchor;
    int has_end_anchor;
    int pat_start;
    int pat_end;
    char cleaned[128];
    int ci;
    int i;

    plen = str_len(pattern);
    tlen = str_len(text);

    has_start_anchor = 0;
    has_end_anchor = 0;
    pat_start = 0;
    pat_end = plen;

    if (plen > 0 && pattern[0] == '^') {
        has_start_anchor = 1;
        pat_start = 1;
    }
    if (plen > 0 && pattern[plen - 1] == '$') {
        has_end_anchor = 1;
        pat_end = pat_end - 1;
    }

    // Extract inner pattern
    ci = 0;
    for (i = pat_start; i < pat_end; i = i + 1) {
        cleaned[ci] = pattern[i];
        ci = ci + 1;
    }
    cleaned[ci] = 0;

    if (has_start_anchor && has_end_anchor) {
        // Must match entire string
        return regex_match(cleaned, text);
    } else if (has_start_anchor) {
        // Must match from beginning, but can end anywhere
        parse_pattern(cleaned, ci);
        {
            int end;
            for (end = 0; end <= tlen; end = end + 1) {
                if (match_here(0, text, 0, end)) {
                    return 1;
                }
            }
        }
        return 0;
    } else if (has_end_anchor) {
        // Must match at end
        parse_pattern(cleaned, ci);
        {
            int start;
            for (start = 0; start <= tlen; start = start + 1) {
                if (match_here(0, text, start, tlen)) {
                    return 1;
                }
            }
        }
        return 0;
    }

    // No anchors: treat as full match
    return regex_match(cleaned, text);
}

// ---- Search: find pattern anywhere in text ----

int regex_search(char *pattern, char *text) {
    int plen;
    int tlen;
    int i;
    int end;

    plen = str_len(pattern);
    tlen = str_len(text);
    parse_pattern(pattern, plen);

    for (i = 0; i <= tlen; i = i + 1) {
        for (end = i; end <= tlen; end = end + 1) {
            if (match_here(0, text, i, end)) {
                return i;
            }
        }
    }
    return -1;
}

// ---- Search with position return ----

int regex_search_from(char *pattern, char *text, int start) {
    int plen;
    int tlen;
    int i;
    int end;

    plen = str_len(pattern);
    tlen = str_len(text);
    parse_pattern(pattern, plen);

    for (i = start; i <= tlen; i = i + 1) {
        for (end = i; end <= tlen; end = end + 1) {
            if (match_here(0, text, i, end)) {
                return i;
            }
        }
    }
    return -1;
}

// ---- Count non-overlapping matches in text ----

int regex_count(char *pattern, char *text) {
    int count;
    int pos;
    int plen;
    int tlen;
    int end;
    int found;

    plen = str_len(pattern);
    tlen = str_len(text);
    count = 0;
    pos = 0;

    while (pos <= tlen) {
        parse_pattern(pattern, plen);
        found = 0;
        {
            int i;
            for (i = pos; i <= tlen && !found; i = i + 1) {
                for (end = i + 1; end <= tlen && !found; end = end + 1) {
                    if (match_here(0, text, i, end)) {
                        count = count + 1;
                        pos = end;
                        found = 1;
                    }
                }
            }
        }
        if (!found) {
            return count;
        }
    }
    return count;
}

// ---- Helper for checking if char is alphanumeric ----

int is_alpha(char c) {
    if (c >= 'a' && c <= 'z') return 1;
    if (c >= 'A' && c <= 'Z') return 1;
    return 0;
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_alnum(char c) {
    return is_alpha(c) || is_digit(c);
}

// ---- Pattern compilation check ----

int is_valid_pattern(char *pattern) {
    int i;
    int plen;
    int in_class;
    plen = str_len(pattern);
    in_class = 0;

    for (i = 0; i < plen; i = i + 1) {
        if (pattern[i] == '[') {
            if (in_class) return 0;
            in_class = 1;
        } else if (pattern[i] == ']') {
            if (!in_class) return 0;
            in_class = 0;
        } else if (pattern[i] == '*' || pattern[i] == '+' || pattern[i] == '?') {
            // Quantifier must follow something
            if (i == 0) return 0;
            if (pattern[i - 1] == '*' || pattern[i - 1] == '+' || pattern[i - 1] == '?') {
                return 0;
            }
        }
    }
    if (in_class) return 0;
    return 1;
}

// ---- Print helper for char arrays ----

void print_chars(char *s) {
    int i;
    i = 0;
    while (s[i] != 0) {
        printf("%c", s[i]);
        i = i + 1;
    }
}

// ---- Escape helper ----

int needs_escape(char c) {
    if (c == '.' || c == '*' || c == '+' || c == '?' || c == '[' || c == ']' || c == '^' || c == '$') {
        return 1;
    }
    return 0;
}

// ---- Build pattern from literal string (escape special chars) ----

void escape_literal(char *input, char *output) {
    int i;
    int o;
    i = 0;
    o = 0;
    while (input[i] != 0) {
        if (needs_escape(input[i])) {
            output[o] = '\\';
            o = o + 1;
        }
        output[o] = input[i];
        o = o + 1;
        i = i + 1;
    }
    output[o] = 0;
}

// ---- Alternation simulation (try two patterns) ----

int regex_match_either(char *pat1, char *pat2, char *text) {
    if (regex_match(pat1, text)) return 1;
    if (regex_match(pat2, text)) return 1;
    return 0;
}

// ---- Match with length return ----

int regex_match_length(char *pattern, char *text) {
    int plen;
    int tlen;
    int end;

    plen = str_len(pattern);
    tlen = str_len(text);
    parse_pattern(pattern, plen);

    for (end = tlen; end >= 0; end = end - 1) {
        if (match_here(0, text, 0, end)) {
            return end;
        }
    }
    return -1;
}

int main() {
    int r;

    // Literal matching
    printf("=== Literal Matching ===\n");
    printf("match 'abc' vs 'abc': %d\n", regex_match("abc", "abc"));
    printf("match 'abc' vs 'abd': %d\n", regex_match("abc", "abd"));
    printf("match 'abc' vs 'ab': %d\n", regex_match("abc", "ab"));
    printf("match '' vs '': %d\n", regex_match("", ""));

    // Dot
    printf("=== Dot (any char) ===\n");
    printf("match 'a.c' vs 'abc': %d\n", regex_match("a.c", "abc"));
    printf("match 'a.c' vs 'aXc': %d\n", regex_match("a.c", "aXc"));
    printf("match 'a.c' vs 'ac': %d\n", regex_match("a.c", "ac"));
    printf("match '...' vs 'abc': %d\n", regex_match("...", "abc"));
    printf("match '...' vs 'ab': %d\n", regex_match("...", "ab"));

    // Star
    printf("=== Star (zero or more) ===\n");
    printf("match 'a*' vs '': %d\n", regex_match("a*", ""));
    printf("match 'a*' vs 'a': %d\n", regex_match("a*", "a"));
    printf("match 'a*' vs 'aaa': %d\n", regex_match("a*", "aaa"));
    printf("match 'a*' vs 'b': %d\n", regex_match("a*", "b"));
    printf("match 'a*b' vs 'b': %d\n", regex_match("a*b", "b"));
    printf("match 'a*b' vs 'aab': %d\n", regex_match("a*b", "aab"));
    printf("match 'a*b' vs 'aac': %d\n", regex_match("a*b", "aac"));

    // Plus
    printf("=== Plus (one or more) ===\n");
    printf("match 'a+' vs '': %d\n", regex_match("a+", ""));
    printf("match 'a+' vs 'a': %d\n", regex_match("a+", "a"));
    printf("match 'a+' vs 'aaa': %d\n", regex_match("a+", "aaa"));
    printf("match 'a+b' vs 'b': %d\n", regex_match("a+b", "b"));
    printf("match 'a+b' vs 'ab': %d\n", regex_match("a+b", "ab"));
    printf("match 'a+b' vs 'aab': %d\n", regex_match("a+b", "aab"));

    // Question
    printf("=== Question (optional) ===\n");
    printf("match 'a?' vs '': %d\n", regex_match("a?", ""));
    printf("match 'a?' vs 'a': %d\n", regex_match("a?", "a"));
    printf("match 'a?b' vs 'b': %d\n", regex_match("a?b", "b"));
    printf("match 'a?b' vs 'ab': %d\n", regex_match("a?b", "ab"));
    printf("match 'a?b' vs 'aab': %d\n", regex_match("a?b", "aab"));

    // Char class
    printf("=== Char Class [a-z] ===\n");
    printf("match '[abc]' vs 'a': %d\n", regex_match("[abc]", "a"));
    printf("match '[abc]' vs 'b': %d\n", regex_match("[abc]", "b"));
    printf("match '[abc]' vs 'd': %d\n", regex_match("[abc]", "d"));
    printf("match '[a-z]' vs 'g': %d\n", regex_match("[a-z]", "g"));
    printf("match '[a-z]' vs 'G': %d\n", regex_match("[a-z]", "G"));
    printf("match '[0-9]' vs '5': %d\n", regex_match("[0-9]", "5"));
    printf("match '[0-9]' vs 'a': %d\n", regex_match("[0-9]", "a"));

    // Anchors
    printf("=== Anchors ^ and $ ===\n");
    r = regex_search("abc", "xyzabc");
    if (r >= 0) {
        printf("search 'abc' in 'xyzabc': 1\n");
    } else {
        printf("search 'abc' in 'xyzabc': 0\n");
    }
    printf("anchored '^abc' in 'xyzabc': %d\n", regex_anchored_match("^abc", "xyzabc"));
    printf("anchored '^abc' in 'abcxyz': %d\n", regex_anchored_match("^abc", "abcxyz"));
    printf("anchored 'abc$' in 'xyzabc': %d\n", regex_anchored_match("abc$", "xyzabc"));
    printf("anchored 'abc$' in 'abcxyz': %d\n", regex_anchored_match("abc$", "abcxyz"));
    printf("anchored '^abc$' in 'abc': %d\n", regex_anchored_match("^abc$", "abc"));
    printf("anchored '^abc$' in 'abcd': %d\n", regex_anchored_match("^abc$", "abcd"));

    // Complex patterns
    printf("=== Complex patterns ===\n");
    printf("match 'a.*b' vs 'ab': %d\n", regex_match("a.*b", "ab"));
    printf("match 'a.*b' vs 'aXYZb': %d\n", regex_match("a.*b", "aXYZb"));
    printf("match 'a.*b' vs 'a': %d\n", regex_match("a.*b", "a"));
    printf("match '[abc]+' vs 'abcba': %d\n", regex_match("[abc]+", "abcba"));
    printf("match 'a.b.c' vs 'aXbYc': %d\n", regex_match("a.b.c", "aXbYc"));
    printf("match 'a?b?c?' vs '': %d\n", regex_match("a?b?c?", ""));
    printf("match 'a?b?c?' vs 'abc': %d\n", regex_match("a?b?c?", "abc"));
    printf("match 'a?b?c?' vs 'ac': %d\n", regex_match("a?b?c?", "ac"));

    // Search
    printf("=== Search (find pattern in text) ===\n");
    r = regex_search("bc", "abcdef");
    if (r >= 0) {
        printf("find 'bc' in 'abcdef': found at %d\n", r);
    } else {
        printf("find 'bc' in 'abcdef': not found\n");
    }

    r = regex_search("xyz", "abcdef");
    if (r >= 0) {
        printf("find 'xyz' in 'abcdef': found at %d\n", r);
    } else {
        printf("find 'xyz' in 'abcdef': not found\n");
    }

    r = regex_search("a.c", "XaYcZ");
    if (r >= 0) {
        printf("find 'a.c' in 'XaYcZ': found at %d\n", r);
    } else {
        printf("find 'a.c' in 'XaYcZ': not found\n");
    }

    r = regex_search("a.c", "XabcZ");
    if (r >= 0) {
        printf("find 'a.c' in 'XabcZ': found at %d\n", r);
    } else {
        printf("find 'a.c' in 'XabcZ': not found\n");
    }

    // Count
    printf("=== Count matches ===\n");
    printf("count 'ab' in 'ababab': %d\n", regex_count("ab", "ababab"));
    printf("count 'a' in 'banana': %d\n", regex_count("a", "banana"));

    printf("Done.\n");
    return 0;
}
