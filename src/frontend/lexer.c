/*
 * lexer.c - Complete C11 lexer implementation
 *
 * Reads a source file and produces a flat array of tokens.
 * Handles all C11 tokens: keywords, identifiers, integer/float/char/string
 * literals, all operators and punctuation, C/C++ comments.
 * Uses keyword hash lookup for efficient keyword recognition.
 */

#include "cc.h"
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>

/* --------------------------------------------------------------------------
 * Token kind -> string mapping
 * -------------------------------------------------------------------------- */

const char *token_kind_str(TokenKind kind)
{
    static const char *names[] = {
        [TOK_EOF]             = "EOF",
        [TOK_INT_LIT]        = "int_lit",
        [TOK_FLOAT_LIT]      = "float_lit",
        [TOK_CHAR_LIT]       = "char_lit",
        [TOK_STRING_LIT]     = "string_lit",
        [TOK_IDENT]          = "identifier",

        [TOK_LPAREN]         = "(",
        [TOK_RPAREN]         = ")",
        [TOK_LBRACE]         = "{",
        [TOK_RBRACE]         = "}",
        [TOK_LBRACKET]       = "[",
        [TOK_RBRACKET]       = "]",
        [TOK_SEMI]           = ";",
        [TOK_COMMA]          = ",",
        [TOK_DOT]            = ".",
        [TOK_ARROW]          = "->",
        [TOK_ELLIPSIS]       = "...",
        [TOK_COLON]          = ":",
        [TOK_QUESTION]       = "?",
        [TOK_TILDE]          = "~",

        [TOK_PLUS]           = "+",
        [TOK_MINUS]          = "-",
        [TOK_STAR]           = "*",
        [TOK_SLASH]          = "/",
        [TOK_PERCENT]        = "%",
        [TOK_AMP]            = "&",
        [TOK_PIPE]           = "|",
        [TOK_CARET]          = "^",
        [TOK_LSHIFT]         = "<<",
        [TOK_RSHIFT]         = ">>",
        [TOK_BANG]           = "!",

        [TOK_EQ]             = "==",
        [TOK_NE]             = "!=",
        [TOK_LT]             = "<",
        [TOK_GT]             = ">",
        [TOK_LE]             = "<=",
        [TOK_GE]             = ">=",

        [TOK_AND]            = "&&",
        [TOK_OR]             = "||",

        [TOK_ASSIGN]         = "=",
        [TOK_PLUS_ASSIGN]    = "+=",
        [TOK_MINUS_ASSIGN]   = "-=",
        [TOK_STAR_ASSIGN]    = "*=",
        [TOK_SLASH_ASSIGN]   = "/=",
        [TOK_PERCENT_ASSIGN] = "%=",
        [TOK_AMP_ASSIGN]     = "&=",
        [TOK_PIPE_ASSIGN]    = "|=",
        [TOK_CARET_ASSIGN]   = "^=",
        [TOK_LSHIFT_ASSIGN]  = "<<=",
        [TOK_RSHIFT_ASSIGN]  = ">>=",

        [TOK_INC]            = "++",
        [TOK_DEC]            = "--",

        [TOK_HASH]           = "#",
        [TOK_HASHHASH]       = "##",

        [TOK_KW_AUTO]            = "auto",
        [TOK_KW_BREAK]           = "break",
        [TOK_KW_CASE]            = "case",
        [TOK_KW_CHAR]            = "char",
        [TOK_KW_CONST]           = "const",
        [TOK_KW_CONTINUE]        = "continue",
        [TOK_KW_DEFAULT]         = "default",
        [TOK_KW_DO]              = "do",
        [TOK_KW_DOUBLE]          = "double",
        [TOK_KW_ELSE]            = "else",
        [TOK_KW_ENUM]            = "enum",
        [TOK_KW_EXTERN]          = "extern",
        [TOK_KW_FLOAT]           = "float",
        [TOK_KW_FOR]             = "for",
        [TOK_KW_GOTO]            = "goto",
        [TOK_KW_IF]              = "if",
        [TOK_KW_INLINE]          = "inline",
        [TOK_KW_INT]             = "int",
        [TOK_KW_LONG]            = "long",
        [TOK_KW_REGISTER]        = "register",
        [TOK_KW_RESTRICT]        = "restrict",
        [TOK_KW_RETURN]          = "return",
        [TOK_KW_SHORT]           = "short",
        [TOK_KW_SIGNED]          = "signed",
        [TOK_KW_SIZEOF]          = "sizeof",
        [TOK_KW_STATIC]          = "static",
        [TOK_KW_STRUCT]          = "struct",
        [TOK_KW_SWITCH]          = "switch",
        [TOK_KW_TYPEDEF]         = "typedef",
        [TOK_KW_UNION]           = "union",
        [TOK_KW_UNSIGNED]        = "unsigned",
        [TOK_KW_VOID]            = "void",
        [TOK_KW_VOLATILE]        = "volatile",
        [TOK_KW_WHILE]           = "while",
        [TOK_KW__ALIGNAS]        = "_Alignas",
        [TOK_KW__ALIGNOF]        = "_Alignof",
        [TOK_KW__ATOMIC]         = "_Atomic",
        [TOK_KW__BOOL]           = "_Bool",
        [TOK_KW__COMPLEX]        = "_Complex",
        [TOK_KW__GENERIC]        = "_Generic",
        [TOK_KW__IMAGINARY]      = "_Imaginary",
        [TOK_KW__NORETURN]       = "_Noreturn",
        [TOK_KW__STATIC_ASSERT]  = "_Static_assert",
        [TOK_KW__THREAD_LOCAL]   = "_Thread_local",
    };

    if (kind >= 0 && kind < TOK_COUNT)
        return names[kind];
    return "<unknown>";
}

/* --------------------------------------------------------------------------
 * Keyword hash table
 * -------------------------------------------------------------------------- */

typedef struct {
    const char *name;
    TokenKind kind;
} Keyword;

static const Keyword keywords[] = {
    { "auto",            TOK_KW_AUTO },
    { "break",           TOK_KW_BREAK },
    { "case",            TOK_KW_CASE },
    { "char",            TOK_KW_CHAR },
    { "const",           TOK_KW_CONST },
    { "continue",        TOK_KW_CONTINUE },
    { "default",         TOK_KW_DEFAULT },
    { "do",              TOK_KW_DO },
    { "double",          TOK_KW_DOUBLE },
    { "else",            TOK_KW_ELSE },
    { "enum",            TOK_KW_ENUM },
    { "extern",          TOK_KW_EXTERN },
    { "float",           TOK_KW_FLOAT },
    { "for",             TOK_KW_FOR },
    { "goto",            TOK_KW_GOTO },
    { "if",              TOK_KW_IF },
    { "inline",          TOK_KW_INLINE },
    { "int",             TOK_KW_INT },
    { "long",            TOK_KW_LONG },
    { "register",        TOK_KW_REGISTER },
    { "restrict",        TOK_KW_RESTRICT },
    { "return",          TOK_KW_RETURN },
    { "short",           TOK_KW_SHORT },
    { "signed",          TOK_KW_SIGNED },
    { "sizeof",          TOK_KW_SIZEOF },
    { "static",          TOK_KW_STATIC },
    { "struct",          TOK_KW_STRUCT },
    { "switch",          TOK_KW_SWITCH },
    { "typedef",         TOK_KW_TYPEDEF },
    { "union",           TOK_KW_UNION },
    { "unsigned",        TOK_KW_UNSIGNED },
    { "void",            TOK_KW_VOID },
    { "volatile",        TOK_KW_VOLATILE },
    { "while",           TOK_KW_WHILE },
    { "_Alignas",        TOK_KW__ALIGNAS },
    { "_Alignof",        TOK_KW__ALIGNOF },
    { "_Atomic",         TOK_KW__ATOMIC },
    { "_Bool",           TOK_KW__BOOL },
    { "_Complex",        TOK_KW__COMPLEX },
    { "_Generic",        TOK_KW__GENERIC },
    { "_Imaginary",      TOK_KW__IMAGINARY },
    { "_Noreturn",       TOK_KW__NORETURN },
    { "_Static_assert",  TOK_KW__STATIC_ASSERT },
    { "_Thread_local",   TOK_KW__THREAD_LOCAL },
};

#define NUM_KEYWORDS (sizeof(keywords) / sizeof(keywords[0]))

/* Simple hash table for keyword lookup. Power-of-two size for mask. */
#define KW_TAB_SIZE 128  /* must be power of two, > 2 * NUM_KEYWORDS */

typedef struct {
    const char *name;
    int len;
    TokenKind kind;
} KWEntry;

static KWEntry kw_table[KW_TAB_SIZE];
static bool    kw_table_init = false;

static uint32_t kw_hash(const char *s, int len)
{
    /* FNV-1a */
    uint32_t h = 2166136261u;
    for (int i = 0; i < len; i++) {
        h ^= (unsigned char)s[i];
        h *= 16777619u;
    }
    return h;
}

static void init_kw_table(void)
{
    if (kw_table_init) return;
    memset(kw_table, 0, sizeof(kw_table));
    for (int i = 0; i < (int)NUM_KEYWORDS; i++) {
        const char *name = keywords[i].name;
        int len = (int)strlen(name);
        uint32_t h = kw_hash(name, len) & (KW_TAB_SIZE - 1);
        /* Linear probe */
        while (kw_table[h].name != NULL) {
            h = (h + 1) & (KW_TAB_SIZE - 1);
        }
        kw_table[h].name = name;
        kw_table[h].len  = len;
        kw_table[h].kind = keywords[i].kind;
    }
    kw_table_init = true;
}

/* Look up an identifier in the keyword table. Returns TOK_IDENT if not a keyword. */
static TokenKind lookup_keyword(const char *s, int len)
{
    uint32_t h = kw_hash(s, len) & (KW_TAB_SIZE - 1);
    for (;;) {
        if (kw_table[h].name == NULL)
            return TOK_IDENT;
        if (kw_table[h].len == len && memcmp(kw_table[h].name, s, len) == 0)
            return kw_table[h].kind;
        h = (h + 1) & (KW_TAB_SIZE - 1);
    }
}

/* --------------------------------------------------------------------------
 * Lexer state
 * -------------------------------------------------------------------------- */

typedef struct {
    CompilerCtx *cc;
    const char *src;       /* start of source buffer */
    const char *p;         /* current read position */
    const char *end;       /* one past last byte */
    const char *filename;
    int line;
    const char *line_start; /* pointer to start of current line */
    bool at_bol;            /* next token is at beginning of line */
    bool has_space;         /* next token preceded by whitespace */

    /* Output token list */
    TokenList *tl;
} Lexer;

static SrcLoc lexer_loc(Lexer *L)
{
    return (SrcLoc){
        .filename   = L->filename,
        .line       = L->line,
        .col        = (int)(L->p - L->line_start) + 1,
        .line_start = L->line_start,
    };
}

static void emit_token(Lexer *L, Token tok)
{
    TokenList *tl = L->tl;
    if (tl->count >= tl->cap) {
        int newcap = tl->cap ? tl->cap * 2 : 256;
        Token *newtoks = realloc(tl->tokens, sizeof(Token) * newcap);
        if (!newtoks) {
            fprintf(stderr, "fatal: out of memory\n");
            exit(1);
        }
        tl->tokens = newtoks;
        tl->cap = newcap;
    }
    tl->tokens[tl->count++] = tok;
}

/* --------------------------------------------------------------------------
 * Character helpers
 * -------------------------------------------------------------------------- */

static inline bool is_ident_start(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static inline bool is_ident_char(char c)
{
    return is_ident_start(c) || (c >= '0' && c <= '9');
}

static inline bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

static inline bool is_hex_digit(char c)
{
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

static inline bool is_octal_digit(char c)
{
    return c >= '0' && c <= '7';
}

static inline bool is_binary_digit(char c)
{
    return c == '0' || c == '1';
}

static inline int hex_val(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

/* --------------------------------------------------------------------------
 * Whitespace and comment skipping
 * -------------------------------------------------------------------------- */

static void skip_whitespace_and_comments(Lexer *L)
{
    for (;;) {
        /* Skip whitespace (not newlines - we track those for at_bol) */
        while (L->p < L->end && (*L->p == ' ' || *L->p == '\t' ||
               *L->p == '\r' || *L->p == '\f' || *L->p == '\v')) {
            L->has_space = true;
            L->p++;
        }

        /* Newline */
        if (L->p < L->end && *L->p == '\n') {
            L->p++;
            L->line++;
            L->line_start = L->p;
            L->at_bol = true;
            L->has_space = true;
            continue;
        }

        /* Line comment */
        if (L->p + 1 < L->end && L->p[0] == '/' && L->p[1] == '/') {
            L->has_space = true;
            L->p += 2;
            while (L->p < L->end && *L->p != '\n')
                L->p++;
            /* newline will be consumed on next iteration */
            continue;
        }

        /* Block comment */
        if (L->p + 1 < L->end && L->p[0] == '/' && L->p[1] == '*') {
            SrcLoc comment_loc = lexer_loc(L);
            L->has_space = true;
            L->p += 2;
            while (L->p + 1 < L->end) {
                if (L->p[0] == '*' && L->p[1] == '/') {
                    L->p += 2;
                    goto next_ws;
                }
                if (*L->p == '\n') {
                    L->line++;
                    L->p++;
                    L->line_start = L->p;
                    L->at_bol = true;
                } else {
                    L->p++;
                }
            }
            /* Unterminated block comment */
            diag_error(&L->cc->diag, comment_loc, "unterminated block comment");
            L->p = L->end;
            return;
        next_ws:
            continue;
        }

        /* No more whitespace or comments */
        break;
    }
}

/* --------------------------------------------------------------------------
 * Escape sequence parsing (shared by char and string literals)
 * -------------------------------------------------------------------------- */

/* Parse an escape sequence starting after the backslash. Returns the character
 * value and advances L->p past the escape sequence. */
static int parse_escape(Lexer *L)
{
    if (L->p >= L->end) {
        diag_error(&L->cc->diag, lexer_loc(L),
                   "unexpected end of file in escape sequence");
        return 0;
    }

    char c = *L->p++;
    switch (c) {
    case 'a':  return '\a';
    case 'b':  return '\b';
    case 'f':  return '\f';
    case 'n':  return '\n';
    case 'r':  return '\r';
    case 't':  return '\t';
    case 'v':  return '\v';
    case '\\': return '\\';
    case '\'': return '\'';
    case '\"': return '\"';
    case '?':  return '\?';
    case '0': case '1': case '2': case '3':
    case '4': case '5': case '6': case '7': {
        /* Octal: up to 3 digits */
        int val = c - '0';
        for (int i = 0; i < 2 && L->p < L->end && is_octal_digit(*L->p); i++)
            val = val * 8 + (*L->p++ - '0');
        if (val > 255) {
            diag_error(&L->cc->diag, lexer_loc(L),
                       "octal escape value \\%o out of range", val);
        }
        return val;
    }
    case 'x': {
        /* Hex: arbitrary number of digits */
        if (L->p >= L->end || !is_hex_digit(*L->p)) {
            diag_error(&L->cc->diag, lexer_loc(L),
                       "\\x used with no following hex digits");
            return 0;
        }
        unsigned long val = 0;
        while (L->p < L->end && is_hex_digit(*L->p)) {
            val = val * 16 + hex_val(*L->p);
            L->p++;
            if (val > 0xFF) {
                /* Keep going to consume digits, but warn */
            }
        }
        if (val > 0xFF) {
            diag_error(&L->cc->diag, lexer_loc(L),
                       "hex escape value out of range");
            val &= 0xFF;
        }
        return (int)val;
    }
    default:
        diag_error(&L->cc->diag, lexer_loc(L),
                   "unknown escape sequence '\\%c'", c);
        return c;
    }
}

/* --------------------------------------------------------------------------
 * Number literal parsing
 * -------------------------------------------------------------------------- */

/* Parse integer suffix (u, l, ll, ul, ull, lu, llu, etc.) */
static IntSuffix parse_int_suffix(Lexer *L)
{
    bool u = false;
    int lcount = 0;

    while (L->p < L->end) {
        if ((*L->p == 'u' || *L->p == 'U') && !u) {
            u = true;
            L->p++;
        } else if ((*L->p == 'l' || *L->p == 'L') && lcount < 2) {
            lcount++;
            L->p++;
        } else {
            break;
        }
    }

    if (u && lcount == 2) return INT_SUFFIX_ULL;
    if (u && lcount == 1) return INT_SUFFIX_UL;
    if (u && lcount == 0) return INT_SUFFIX_U;
    if (!u && lcount == 2) return INT_SUFFIX_LL;
    if (!u && lcount == 1) return INT_SUFFIX_L;
    return INT_SUFFIX_NONE;
}

/* Check if the next char makes this a float (. or e/E or p/P for hex float) */
static bool is_float_ahead(Lexer *L, bool is_hex)
{
    if (L->p >= L->end) return false;
    if (*L->p == '.') return true;
    if (!is_hex && (*L->p == 'e' || *L->p == 'E')) return true;
    if (is_hex  && (*L->p == 'p' || *L->p == 'P')) return true;
    return false;
}

/* Parse a floating point suffix (f, F, l, L) */
static void parse_float_suffix(Lexer *L)
{
    if (L->p < L->end && (*L->p == 'f' || *L->p == 'F' ||
                           *L->p == 'l' || *L->p == 'L')) {
        L->p++;
    }
}

static void lex_number(Lexer *L, Token *tok)
{
    const char *start = L->p;
    tok->kind = TOK_INT_LIT;
    tok->int_suffix = INT_SUFFIX_NONE;

    /* Determine base */
    bool is_hex = false;
    bool is_oct = false;
    bool is_bin = false;

    if (*L->p == '0' && L->p + 1 < L->end) {
        char next = L->p[1];
        if (next == 'x' || next == 'X') {
            is_hex = true;
            L->p += 2; /* skip "0x" */
        } else if (next == 'b' || next == 'B') {
            is_bin = true;
            L->p += 2; /* skip "0b" */
        } else if (is_octal_digit(next)) {
            is_oct = true;
            /* don't skip the leading 0 - we'll parse from here */
        }
    }

    if (is_hex) {
        /* Hex integer or hex float */
        const char *digits_start = L->p;
        while (L->p < L->end && is_hex_digit(*L->p))
            L->p++;

        if (L->p == digits_start && !(*L->p == '.' || *L->p == 'p' || *L->p == 'P')) {
            diag_error(&L->cc->diag, tok->loc,
                       "no digits in hexadecimal integer literal");
            tok->int_val = 0;
            return;
        }

        /* Check for hex float */
        if (is_float_ahead(L, true)) {
            /* Hex float: consume fraction part */
            if (L->p < L->end && *L->p == '.') {
                L->p++;
                while (L->p < L->end && is_hex_digit(*L->p))
                    L->p++;
            }
            /* p exponent is mandatory for hex floats */
            if (L->p < L->end && (*L->p == 'p' || *L->p == 'P')) {
                L->p++;
                if (L->p < L->end && (*L->p == '+' || *L->p == '-'))
                    L->p++;
                if (L->p >= L->end || !is_digit(*L->p)) {
                    diag_error(&L->cc->diag, tok->loc,
                               "expected exponent digits in hex float literal");
                }
                while (L->p < L->end && is_digit(*L->p))
                    L->p++;
            } else {
                diag_error(&L->cc->diag, tok->loc,
                           "hex float literal requires 'p' exponent");
            }
            tok->kind = TOK_FLOAT_LIT;
            /* Parse the entire token as a double using strtod which handles hex floats */
            char *endptr;
            errno = 0;
            tok->float_val = strtod(start, &endptr);
            if (errno == ERANGE) {
                diag_error(&L->cc->diag, tok->loc,
                           "hex float literal out of range");
            }
            parse_float_suffix(L);
            tok->text = intern_n(&L->cc->strings, start, (int)(L->p - start));
            tok->text_len = (int)(L->p - start);
            return;
        }

        /* Hex integer */
        char *endptr;
        errno = 0;
        tok->int_val = strtoull(start, &endptr, 16);
        if (errno == ERANGE) {
            diag_error(&L->cc->diag, tok->loc,
                       "hex integer literal out of range");
        }
        L->p = endptr;
        tok->int_suffix = parse_int_suffix(L);
        tok->text = intern_n(&L->cc->strings, start, (int)(L->p - start));
        tok->text_len = (int)(L->p - start);
        return;
    }

    if (is_bin) {
        /* Binary integer */
        const char *digits_start = L->p;
        while (L->p < L->end && is_binary_digit(*L->p))
            L->p++;
        if (L->p == digits_start) {
            diag_error(&L->cc->diag, tok->loc,
                       "no digits in binary integer literal");
            tok->int_val = 0;
            return;
        }
        /* Parse manually since strtoll may not handle 0b portably */
        unsigned long long val = 0;
        for (const char *d = digits_start; d < L->p; d++) {
            unsigned long long prev = val;
            val = val * 2 + (*d - '0');
            if (val < prev) {
                diag_error(&L->cc->diag, tok->loc,
                           "binary integer literal out of range");
                break;
            }
        }
        tok->int_val = val;
        tok->int_suffix = parse_int_suffix(L);
        tok->text = intern_n(&L->cc->strings, start, (int)(L->p - start));
        tok->text_len = (int)(L->p - start);
        return;
    }

    /* Decimal or octal integer, or decimal float */
    /* Consume decimal digits first (octal digits are a subset) */
    while (L->p < L->end && is_digit(*L->p))
        L->p++;

    /* Check for float */
    if (is_float_ahead(L, false)) {
        /* Decimal float */
        if (L->p < L->end && *L->p == '.') {
            L->p++;
            while (L->p < L->end && is_digit(*L->p))
                L->p++;
        }
        if (L->p < L->end && (*L->p == 'e' || *L->p == 'E')) {
            L->p++;
            if (L->p < L->end && (*L->p == '+' || *L->p == '-'))
                L->p++;
            if (L->p >= L->end || !is_digit(*L->p)) {
                diag_error(&L->cc->diag, tok->loc,
                           "expected exponent digits in float literal");
            }
            while (L->p < L->end && is_digit(*L->p))
                L->p++;
        }
        tok->kind = TOK_FLOAT_LIT;
        char *endptr;
        errno = 0;
        tok->float_val = strtod(start, &endptr);
        if (errno == ERANGE) {
            diag_error(&L->cc->diag, tok->loc,
                       "float literal out of range");
        }
        parse_float_suffix(L);
        tok->text = intern_n(&L->cc->strings, start, (int)(L->p - start));
        tok->text_len = (int)(L->p - start);
        return;
    }

    /* Integer: octal if started with 0, else decimal */
    if (is_oct) {
        /* Validate octal digits */
        for (const char *d = start + 1; d < L->p; d++) {
            if (*d >= '8' && *d <= '9') {
                diag_error(&L->cc->diag, tok->loc,
                           "invalid digit '%c' in octal literal", *d);
                break;
            }
        }
        char *endptr;
        errno = 0;
        tok->int_val = strtoull(start, &endptr, 8);
        if (errno == ERANGE) {
            diag_error(&L->cc->diag, tok->loc,
                       "octal integer literal out of range");
        }
        L->p = endptr;
    } else {
        /* Decimal */
        char *endptr;
        errno = 0;
        tok->int_val = strtoull(start, &endptr, 10);
        if (errno == ERANGE) {
            diag_error(&L->cc->diag, tok->loc,
                       "decimal integer literal out of range");
        }
        L->p = endptr;
    }

    tok->int_suffix = parse_int_suffix(L);

    /* Store source text for preprocessor stringization/token-pasting */
    int text_len = (int)(L->p - start);
    tok->text = intern_n(&L->cc->strings, start, text_len);
    tok->text_len = text_len;
}

/* --------------------------------------------------------------------------
 * Character literal
 * -------------------------------------------------------------------------- */

static void lex_char_literal(Lexer *L, Token *tok)
{
    assert(*L->p == '\'');
    L->p++; /* skip opening quote */

    tok->kind = TOK_CHAR_LIT;

    if (L->p >= L->end || *L->p == '\n') {
        diag_error(&L->cc->diag, tok->loc, "unterminated character literal");
        tok->char_val = 0;
        return;
    }

    if (*L->p == '\'') {
        diag_error(&L->cc->diag, tok->loc, "empty character literal");
        L->p++; /* skip closing quote */
        tok->char_val = 0;
        return;
    }

    if (*L->p == '\\') {
        L->p++;
        tok->char_val = parse_escape(L);
    } else {
        tok->char_val = (unsigned char)*L->p++;
    }

    /* Warn about multi-character constants but accept them */
    if (L->p < L->end && *L->p != '\'') {
        /* Multi-character literal - implementation-defined in C */
        int val = tok->char_val;
        while (L->p < L->end && *L->p != '\'' && *L->p != '\n') {
            if (*L->p == '\\') {
                L->p++;
                val = (val << 8) | (parse_escape(L) & 0xFF);
            } else {
                val = (val << 8) | ((unsigned char)*L->p++);
            }
        }
        tok->char_val = val;
    }

    if (L->p < L->end && *L->p == '\'') {
        L->p++; /* skip closing quote */
    } else {
        diag_error(&L->cc->diag, tok->loc, "unterminated character literal");
    }
}

/* --------------------------------------------------------------------------
 * String literal
 * -------------------------------------------------------------------------- */

static void lex_string_literal(Lexer *L, Token *tok)
{
    assert(*L->p == '"');
    L->p++; /* skip opening quote */

    tok->kind = TOK_STRING_LIT;

    /* Build string contents into a temp buffer */
    /* Use the arena for the final string; use a local growable buffer for building */
    int cap = 64;
    int len = 0;
    char *buf = malloc(cap);
    if (!buf) {
        fprintf(stderr, "fatal: out of memory\n");
        exit(1);
    }

    while (L->p < L->end && *L->p != '"') {
        if (*L->p == '\n') {
            diag_error(&L->cc->diag, tok->loc, "unterminated string literal");
            break;
        }

        int ch;
        if (*L->p == '\\') {
            L->p++;
            ch = parse_escape(L);
        } else {
            ch = (unsigned char)*L->p++;
        }

        /* Grow buffer if needed */
        if (len + 1 >= cap) {
            cap *= 2;
            buf = realloc(buf, cap);
            if (!buf) {
                fprintf(stderr, "fatal: out of memory\n");
                exit(1);
            }
        }
        buf[len++] = (char)ch;
    }

    if (L->p < L->end && *L->p == '"') {
        L->p++; /* skip closing quote */
    } else {
        diag_error(&L->cc->diag, tok->loc, "unterminated string literal");
    }

    /* Null-terminate and copy to arena */
    buf[len] = '\0';
    tok->text = arena_strndup(&L->cc->arena, buf, len);
    tok->text_len = len;

    free(buf);
}

/* --------------------------------------------------------------------------
 * Identifier / keyword
 * -------------------------------------------------------------------------- */

static void lex_ident(Lexer *L, Token *tok)
{
    const char *start = L->p;
    while (L->p < L->end && is_ident_char(*L->p))
        L->p++;

    int len = (int)(L->p - start);

    /* Check keyword table first */
    TokenKind kw = lookup_keyword(start, len);
    if (kw != TOK_IDENT) {
        tok->kind = kw;
        tok->text = intern_n(&L->cc->strings, start, len);
        tok->text_len = len;
        return;
    }

    /* Regular identifier - intern the string */
    tok->kind = TOK_IDENT;
    tok->text = intern_n(&L->cc->strings, start, len);
    tok->text_len = len;
}

/* --------------------------------------------------------------------------
 * Main lexer loop
 * -------------------------------------------------------------------------- */

static Token next_token(Lexer *L)
{
    skip_whitespace_and_comments(L);

    Token tok;
    memset(&tok, 0, sizeof(tok));
    tok.at_bol = L->at_bol;
    tok.has_space = L->has_space;
    tok.loc = lexer_loc(L);

    /* Reset flags for the next token */
    L->at_bol = false;
    L->has_space = false;

    /* End of file */
    if (L->p >= L->end) {
        tok.kind = TOK_EOF;
        return tok;
    }

    char c = *L->p;

    /* Identifiers and keywords */
    if (is_ident_start(c)) {
        lex_ident(L, &tok);
        return tok;
    }

    /* Number literals (including those starting with '.') */
    if (is_digit(c)) {
        lex_number(L, &tok);
        return tok;
    }

    /* Dot: could be '.', '...', or float starting with '.' */
    if (c == '.') {
        if (L->p + 1 < L->end && is_digit(L->p[1])) {
            /* Float starting with decimal point, e.g. .5 */
            lex_number(L, &tok);
            return tok;
        }
        if (L->p + 2 < L->end && L->p[1] == '.' && L->p[2] == '.') {
            tok.kind = TOK_ELLIPSIS;
            L->p += 3;
            return tok;
        }
        tok.kind = TOK_DOT;
        L->p++;
        return tok;
    }

    /* Character literal */
    if (c == '\'') {
        lex_char_literal(L, &tok);
        return tok;
    }

    /* String literal */
    if (c == '"') {
        lex_string_literal(L, &tok);
        return tok;
    }

    /* Punctuation and operators */
    switch (c) {
    case '(':
        tok.kind = TOK_LPAREN;
        L->p++;
        return tok;
    case ')':
        tok.kind = TOK_RPAREN;
        L->p++;
        return tok;
    case '{':
        tok.kind = TOK_LBRACE;
        L->p++;
        return tok;
    case '}':
        tok.kind = TOK_RBRACE;
        L->p++;
        return tok;
    case '[':
        tok.kind = TOK_LBRACKET;
        L->p++;
        return tok;
    case ']':
        tok.kind = TOK_RBRACKET;
        L->p++;
        return tok;
    case ';':
        tok.kind = TOK_SEMI;
        L->p++;
        return tok;
    case ',':
        tok.kind = TOK_COMMA;
        L->p++;
        return tok;
    case ':':
        tok.kind = TOK_COLON;
        L->p++;
        return tok;
    case '?':
        tok.kind = TOK_QUESTION;
        L->p++;
        return tok;
    case '~':
        tok.kind = TOK_TILDE;
        L->p++;
        return tok;

    case '+':
        L->p++;
        if (L->p < L->end) {
            if (*L->p == '+') { tok.kind = TOK_INC; L->p++; return tok; }
            if (*L->p == '=') { tok.kind = TOK_PLUS_ASSIGN; L->p++; return tok; }
        }
        tok.kind = TOK_PLUS;
        return tok;

    case '-':
        L->p++;
        if (L->p < L->end) {
            if (*L->p == '-') { tok.kind = TOK_DEC; L->p++; return tok; }
            if (*L->p == '=') { tok.kind = TOK_MINUS_ASSIGN; L->p++; return tok; }
            if (*L->p == '>') { tok.kind = TOK_ARROW; L->p++; return tok; }
        }
        tok.kind = TOK_MINUS;
        return tok;

    case '*':
        L->p++;
        if (L->p < L->end && *L->p == '=') {
            tok.kind = TOK_STAR_ASSIGN;
            L->p++;
            return tok;
        }
        tok.kind = TOK_STAR;
        return tok;

    case '/':
        /* Comments already handled by skip_whitespace_and_comments */
        L->p++;
        if (L->p < L->end && *L->p == '=') {
            tok.kind = TOK_SLASH_ASSIGN;
            L->p++;
            return tok;
        }
        tok.kind = TOK_SLASH;
        return tok;

    case '%':
        L->p++;
        if (L->p < L->end && *L->p == '=') {
            tok.kind = TOK_PERCENT_ASSIGN;
            L->p++;
            return tok;
        }
        tok.kind = TOK_PERCENT;
        return tok;

    case '&':
        L->p++;
        if (L->p < L->end) {
            if (*L->p == '&') { tok.kind = TOK_AND; L->p++; return tok; }
            if (*L->p == '=') { tok.kind = TOK_AMP_ASSIGN; L->p++; return tok; }
        }
        tok.kind = TOK_AMP;
        return tok;

    case '|':
        L->p++;
        if (L->p < L->end) {
            if (*L->p == '|') { tok.kind = TOK_OR; L->p++; return tok; }
            if (*L->p == '=') { tok.kind = TOK_PIPE_ASSIGN; L->p++; return tok; }
        }
        tok.kind = TOK_PIPE;
        return tok;

    case '^':
        L->p++;
        if (L->p < L->end && *L->p == '=') {
            tok.kind = TOK_CARET_ASSIGN;
            L->p++;
            return tok;
        }
        tok.kind = TOK_CARET;
        return tok;

    case '<':
        L->p++;
        if (L->p < L->end) {
            if (*L->p == '<') {
                L->p++;
                if (L->p < L->end && *L->p == '=') {
                    tok.kind = TOK_LSHIFT_ASSIGN;
                    L->p++;
                    return tok;
                }
                tok.kind = TOK_LSHIFT;
                return tok;
            }
            if (*L->p == '=') { tok.kind = TOK_LE; L->p++; return tok; }
        }
        tok.kind = TOK_LT;
        return tok;

    case '>':
        L->p++;
        if (L->p < L->end) {
            if (*L->p == '>') {
                L->p++;
                if (L->p < L->end && *L->p == '=') {
                    tok.kind = TOK_RSHIFT_ASSIGN;
                    L->p++;
                    return tok;
                }
                tok.kind = TOK_RSHIFT;
                return tok;
            }
            if (*L->p == '=') { tok.kind = TOK_GE; L->p++; return tok; }
        }
        tok.kind = TOK_GT;
        return tok;

    case '!':
        L->p++;
        if (L->p < L->end && *L->p == '=') {
            tok.kind = TOK_NE;
            L->p++;
            return tok;
        }
        tok.kind = TOK_BANG;
        return tok;

    case '=':
        L->p++;
        if (L->p < L->end && *L->p == '=') {
            tok.kind = TOK_EQ;
            L->p++;
            return tok;
        }
        tok.kind = TOK_ASSIGN;
        return tok;

    case '#':
        L->p++;
        if (L->p < L->end && *L->p == '#') {
            tok.kind = TOK_HASHHASH;
            L->p++;
            return tok;
        }
        tok.kind = TOK_HASH;
        return tok;

    default:
        /* Unknown character */
        diag_error(&L->cc->diag, tok.loc,
                   "unexpected character '%c' (0x%02x)", c, (unsigned char)c);
        L->p++;
        /* Return EOF-like token to avoid infinite loops; caller will see the error */
        tok.kind = TOK_EOF;
        return tok;
    }
}

/* --------------------------------------------------------------------------
 * Read entire file into memory
 * -------------------------------------------------------------------------- */

static char *read_file(CompilerCtx *cc, const char *filename, long *out_size)
{
    FILE *f = fopen(filename, "rb");
    if (!f) {
        diag_fatal(&cc->diag, SRCLOC_NONE, "cannot open file '%s': %s",
                   filename, strerror(errno));
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (size < 0) {
        diag_fatal(&cc->diag, SRCLOC_NONE, "cannot determine size of '%s'",
                   filename);
        fclose(f);
        return NULL;
    }

    /* Allocate from arena with an extra byte for null terminator */
    char *buf = arena_alloc(&cc->arena, size + 1);
    if (!buf) {
        diag_fatal(&cc->diag, SRCLOC_NONE, "out of memory reading '%s'",
                   filename);
        fclose(f);
        return NULL;
    }

    size_t nread = fread(buf, 1, size, f);
    fclose(f);

    if ((long)nread != size) {
        diag_fatal(&cc->diag, SRCLOC_NONE, "short read on '%s'", filename);
        return NULL;
    }

    buf[size] = '\0';
    *out_size = size;
    return buf;
}

/* --------------------------------------------------------------------------
 * Translation phase 2: line splicing (remove backslash-newline)
 * -------------------------------------------------------------------------- */
static long splice_lines(char *buf, long size)
{
    char *r = buf, *w = buf, *end = buf + size;
    while (r < end) {
        if (*r == '\\' && r + 1 < end && (r[1] == '\n' || (r[1] == '\r' && r + 2 < end && r[2] == '\n'))) {
            /* Skip backslash and newline */
            r++; /* skip backslash */
            if (*r == '\r') r++; /* skip \r if \r\n */
            r++; /* skip \n */
        } else {
            *w++ = *r++;
        }
    }
    *w = '\0';
    return (long)(w - buf);
}

/* --------------------------------------------------------------------------
 * Public API: cc_lex
 * -------------------------------------------------------------------------- */

TokenList *cc_lex(CompilerCtx *cc, const char *filename)
{
    /* Initialize keyword hash table (once) */
    init_kw_table();

    /* Read the source file */
    long file_size = 0;
    char *src = read_file(cc, filename, &file_size);
    if (!src) {
        return NULL;
    }

    /* Translation phase 2: splice lines (remove backslash-newline) */
    file_size = splice_lines(src, file_size);

    /* Allocate the token list */
    TokenList *tl = arena_new(&cc->arena, TokenList);
    tl->tokens = NULL;
    tl->count = 0;
    tl->cap = 0;

    /* Initialize lexer state */
    Lexer L;
    memset(&L, 0, sizeof(L));
    L.cc         = cc;
    L.src        = src;
    L.p          = src;
    L.end        = src + file_size;
    L.filename   = intern(&cc->strings, filename);
    L.line       = 1;
    L.line_start = src;
    L.at_bol     = true;
    L.has_space   = false;
    L.tl         = tl;

    /* Lex all tokens */
    for (;;) {
        Token tok = next_token(&L);
        emit_token(&L, tok);
        if (tok.kind == TOK_EOF)
            break;

        /* Bail early if we have accumulated too many errors */
        if (diag_too_many_errors(&cc->diag))
            break;
    }

    return tl;
}
