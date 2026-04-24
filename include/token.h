#ifndef TOKEN_H
#define TOKEN_H

#include "diag.h"

/* C11 token kinds */
typedef enum {
    /* Literals */
    TOK_EOF = 0,
    TOK_INT_LIT,        /* integer literal */
    TOK_FLOAT_LIT,      /* floating point literal */
    TOK_CHAR_LIT,       /* character literal */
    TOK_STRING_LIT,     /* string literal */
    TOK_IDENT,          /* identifier */

    /* Punctuation */
    TOK_LPAREN,         /* ( */
    TOK_RPAREN,         /* ) */
    TOK_LBRACE,         /* { */
    TOK_RBRACE,         /* } */
    TOK_LBRACKET,       /* [ */
    TOK_RBRACKET,       /* ] */
    TOK_SEMI,           /* ; */
    TOK_COMMA,          /* , */
    TOK_DOT,            /* . */
    TOK_ARROW,          /* -> */
    TOK_ELLIPSIS,       /* ... */
    TOK_COLON,          /* : */
    TOK_QUESTION,       /* ? */
    TOK_TILDE,          /* ~ */

    /* Operators */
    TOK_PLUS,           /* + */
    TOK_MINUS,          /* - */
    TOK_STAR,           /* * */
    TOK_SLASH,          /* / */
    TOK_PERCENT,        /* % */
    TOK_AMP,            /* & */
    TOK_PIPE,           /* | */
    TOK_CARET,          /* ^ */
    TOK_LSHIFT,         /* << */
    TOK_RSHIFT,         /* >> */
    TOK_BANG,           /* ! */

    /* Comparison */
    TOK_EQ,             /* == */
    TOK_NE,             /* != */
    TOK_LT,             /* < */
    TOK_GT,             /* > */
    TOK_LE,             /* <= */
    TOK_GE,             /* >= */

    /* Logical */
    TOK_AND,            /* && */
    TOK_OR,             /* || */

    /* Assignment */
    TOK_ASSIGN,         /* = */
    TOK_PLUS_ASSIGN,    /* += */
    TOK_MINUS_ASSIGN,   /* -= */
    TOK_STAR_ASSIGN,    /* *= */
    TOK_SLASH_ASSIGN,   /* /= */
    TOK_PERCENT_ASSIGN, /* %= */
    TOK_AMP_ASSIGN,     /* &= */
    TOK_PIPE_ASSIGN,    /* |= */
    TOK_CARET_ASSIGN,   /* ^= */
    TOK_LSHIFT_ASSIGN,  /* <<= */
    TOK_RSHIFT_ASSIGN,  /* >>= */

    /* Increment/Decrement */
    TOK_INC,            /* ++ */
    TOK_DEC,            /* -- */

    /* Preprocessor (used internally) */
    TOK_HASH,           /* # */
    TOK_HASHHASH,       /* ## */

    /* C11 Keywords */
    TOK_KW_AUTO,
    TOK_KW_BREAK,
    TOK_KW_CASE,
    TOK_KW_CHAR,
    TOK_KW_CONST,
    TOK_KW_CONTINUE,
    TOK_KW_DEFAULT,
    TOK_KW_DO,
    TOK_KW_DOUBLE,
    TOK_KW_ELSE,
    TOK_KW_ENUM,
    TOK_KW_EXTERN,
    TOK_KW_FLOAT,
    TOK_KW_FOR,
    TOK_KW_GOTO,
    TOK_KW_IF,
    TOK_KW_INLINE,
    TOK_KW_INT,
    TOK_KW_LONG,
    TOK_KW_REGISTER,
    TOK_KW_RESTRICT,
    TOK_KW_RETURN,
    TOK_KW_SHORT,
    TOK_KW_SIGNED,
    TOK_KW_SIZEOF,
    TOK_KW_STATIC,
    TOK_KW_STRUCT,
    TOK_KW_SWITCH,
    TOK_KW_TYPEDEF,
    TOK_KW_UNION,
    TOK_KW_UNSIGNED,
    TOK_KW_VOID,
    TOK_KW_VOLATILE,
    TOK_KW_WHILE,
    TOK_KW__ALIGNAS,
    TOK_KW__ALIGNOF,
    TOK_KW__ATOMIC,
    TOK_KW__BOOL,
    TOK_KW__COMPLEX,
    TOK_KW__GENERIC,
    TOK_KW__IMAGINARY,
    TOK_KW__NORETURN,
    TOK_KW__STATIC_ASSERT,
    TOK_KW__THREAD_LOCAL,

    TOK_COUNT
} TokenKind;

/* Integer literal suffix info */
typedef enum {
    INT_SUFFIX_NONE = 0,
    INT_SUFFIX_U    = 1,
    INT_SUFFIX_L    = 2,
    INT_SUFFIX_UL   = 3,
    INT_SUFFIX_LL   = 4,
    INT_SUFFIX_ULL  = 5,
} IntSuffix;

typedef struct {
    TokenKind kind;
    SrcLoc loc;
    const char *text;    /* interned string for identifiers/strings */
    int text_len;
    union {
        unsigned long long int_val;
        double float_val;
        int char_val;
    };
    IntSuffix int_suffix;
    bool at_bol;          /* at beginning of line (for preprocessor) */
    bool has_space;       /* preceded by whitespace */
} Token;

const char *token_kind_str(TokenKind kind);

#endif /* TOKEN_H */
