#include "cc.h"
#include <ctype.h>
#include <errno.h>
#include <time.h>

/* Preprocessor state */
typedef struct Macro {
    const char *name;
    Token *body;
    int body_len;
    /* Function-like macro */
    const char **params;
    int num_params;
    bool is_func_like;
    bool is_variadic;
} Macro;

typedef struct PPCtx {
    CompilerCtx *cc;
    TokenList *input;
    TokenList *output;
    int pos;
    HashTab macros;
    HashTab pragma_once;  /* files that used #pragma once */
    Vec(const char *) include_stack;  /* for detecting circular includes */
    int if_depth;
    Vec(int) if_stack;  /* 0=inactive, 1=active, 2=else-seen, 3=branch-taken(skip rest) */
} PPCtx;

static void pp_init(PPCtx *pp, CompilerCtx *cc, TokenList *input) {
    pp->cc = cc;
    pp->input = input;
    pp->output = arena_new(&cc->arena, TokenList);
    pp->output->tokens = NULL;
    pp->output->count = 0;
    pp->output->cap = 0;
    pp->pos = 0;
    hashtab_init(&pp->macros, 64);
    hashtab_init(&pp->pragma_once, 16);
    memset(&pp->include_stack, 0, sizeof(pp->include_stack));
    pp->if_depth = 0;
    memset(&pp->if_stack, 0, sizeof(pp->if_stack));
}

static Token *pp_peek(PPCtx *pp) {
    if (pp->pos >= pp->input->count) return &pp->input->tokens[pp->input->count - 1]; /* EOF */
    return &pp->input->tokens[pp->pos];
}

static Token *pp_advance(PPCtx *pp) {
    Token *t = pp_peek(pp);
    if (t->kind != TOK_EOF) pp->pos++;
    return t;
}

static void pp_emit(PPCtx *pp, Token *t) {
    if (pp->output->count >= pp->output->cap) {
        int newcap = pp->output->cap ? pp->output->cap * 2 : 256;
        Token *newtoks = arena_alloc(&pp->cc->arena, sizeof(Token) * newcap);
        if (pp->output->tokens) {
            memcpy(newtoks, pp->output->tokens, sizeof(Token) * pp->output->count);
        }
        pp->output->tokens = newtoks;
        pp->output->cap = newcap;
    }
    pp->output->tokens[pp->output->count++] = *t;
}

static void pp_skip_line(PPCtx *pp) {
    while (pp->pos < pp->input->count) {
        Token *t = &pp->input->tokens[pp->pos];
        if (t->kind == TOK_EOF || t->at_bol) break;
        pp->pos++;
    }
}

static bool pp_is_active(PPCtx *pp) {
    /* Check if current #if nesting is active */
    for (int i = 0; i < pp->if_stack.len; i++) {
        if (pp->if_stack.data[i] == 0 || pp->if_stack.data[i] == 3) return false;
    }
    return true;
}

static void pp_define_object(PPCtx *pp, const char *name, Token *body, int body_len) {
    Macro *m = arena_new(&pp->cc->arena, Macro);
    m->name = name;
    m->body = body;
    m->body_len = body_len;
    m->is_func_like = false;
    m->is_variadic = false;
    m->params = NULL;
    m->num_params = 0;
    hashtab_put(&pp->macros, name, m);
}

static void pp_handle_define(PPCtx *pp) {
    Token *name_tok = pp_advance(pp);
    if (name_tok->kind != TOK_IDENT) {
        diag_error(&pp->cc->diag, name_tok->loc, "expected macro name");
        pp_skip_line(pp);
        return;
    }

    const char *name = name_tok->text;

    /* Check for function-like macro: ( immediately after name (no space) */
    Token *next = pp_peek(pp);
    if (next->kind == TOK_LPAREN && !next->has_space) {
        /* Function-like macro */
        pp_advance(pp); /* consume ( */
        Macro *m = arena_new(&pp->cc->arena, Macro);
        m->name = name;
        m->is_func_like = true;
        m->is_variadic = false;

        Vec(const char *) params = {0};
        while (pp_peek(pp)->kind != TOK_RPAREN && pp_peek(pp)->kind != TOK_EOF) {
            if (params.len > 0) {
                if (pp_peek(pp)->kind != TOK_COMMA) {
                    diag_error(&pp->cc->diag, pp_peek(pp)->loc, "expected ','");
                    break;
                }
                pp_advance(pp);
            }
            if (pp_peek(pp)->kind == TOK_ELLIPSIS) {
                pp_advance(pp);
                m->is_variadic = true;
                break;
            }
            Token *p = pp_advance(pp);
            if (p->kind != TOK_IDENT) {
                diag_error(&pp->cc->diag, p->loc, "expected parameter name");
                break;
            }
            vec_push(&params, p->text);
        }
        if (pp_peek(pp)->kind == TOK_RPAREN) pp_advance(pp);

        m->params = arena_alloc(&pp->cc->arena, sizeof(const char *) * params.len);
        memcpy(m->params, params.data, sizeof(const char *) * params.len);
        m->num_params = params.len;
        vec_free(&params);

        /* Collect body tokens */
        Vec(Token) body = {0};
        while (pp->pos < pp->input->count) {
            Token *t = &pp->input->tokens[pp->pos];
            if (t->kind == TOK_EOF || t->at_bol) break;
            vec_push(&body, *t);
            pp->pos++;
        }
        m->body = arena_alloc(&pp->cc->arena, sizeof(Token) * body.len);
        memcpy(m->body, body.data, sizeof(Token) * body.len);
        m->body_len = body.len;
        vec_free(&body);

        hashtab_put(&pp->macros, name, m);
    } else {
        /* Object-like macro */
        Vec(Token) body = {0};
        while (pp->pos < pp->input->count) {
            Token *t = &pp->input->tokens[pp->pos];
            if (t->kind == TOK_EOF || t->at_bol) break;
            vec_push(&body, *t);
            pp->pos++;
        }
        Token *body_arr = NULL;
        if (body.len > 0) {
            body_arr = arena_alloc(&pp->cc->arena, sizeof(Token) * body.len);
            memcpy(body_arr, body.data, sizeof(Token) * body.len);
        }
        pp_define_object(pp, name, body_arr, body.len);
        vec_free(&body);
    }
}

static void pp_handle_undef(PPCtx *pp) {
    Token *name = pp_advance(pp);
    if (name->kind == TOK_IDENT) {
        hashtab_delete(&pp->macros, name->text);
    }
    pp_skip_line(pp);
}

static long long pp_eval_constant(PPCtx *pp);

static void pp_handle_ifdef(PPCtx *pp, bool is_ifndef) {
    Token *name = pp_advance(pp);
    bool defined = false;
    if (name->kind == TOK_IDENT) {
        defined = hashtab_get(&pp->macros, name->text) != NULL;
    }
    if (is_ifndef) defined = !defined;

    int state = (pp_is_active(pp) && defined) ? 1 : 0;
    vec_push(&pp->if_stack, state);
    pp->if_depth++;
    pp_skip_line(pp);
}

static void pp_handle_if(PPCtx *pp) {
    long long val = 0;
    if (pp_is_active(pp)) {
        val = pp_eval_constant(pp);
    }
    int state = (pp_is_active(pp) && val != 0) ? 1 : 0;
    /* But we need to handle inner nesting when outer is inactive */
    if (!pp_is_active(pp)) state = 0;

    /* Replace the check: we need the if_stack to properly track nesting */
    if (pp->if_stack.len > 0 && pp->if_stack.data[pp->if_stack.len - 1] == 0) {
        state = 0;
    }
    vec_push(&pp->if_stack, state);
    pp->if_depth++;
    pp_skip_line(pp);
}

static void pp_handle_elif(PPCtx *pp) {
    if (pp->if_stack.len == 0) {
        diag_error(&pp->cc->diag, pp_peek(pp)->loc, "#elif without #if");
        pp_skip_line(pp);
        return;
    }

    int *current = &pp->if_stack.data[pp->if_stack.len - 1];
    if (*current == 2) {
        diag_error(&pp->cc->diag, pp_peek(pp)->loc, "#elif after #else");
        pp_skip_line(pp);
        return;
    }

    if (*current == 1 || *current == 3) {
        /* A previous branch was already taken, skip all remaining branches */
        *current = 3;
        pp_skip_line(pp);
    } else {
        /* No branch taken yet (*current == 0), check condition */
        bool parent_active = true;
        for (int i = 0; i < pp->if_stack.len - 1; i++) {
            if (pp->if_stack.data[i] == 0 || pp->if_stack.data[i] == 3) {
                parent_active = false; break;
            }
        }
        if (parent_active) {
            long long val = pp_eval_constant(pp);
            *current = val ? 1 : 0;
        }
        pp_skip_line(pp);
    }
}

static void pp_handle_else(PPCtx *pp) {
    if (pp->if_stack.len == 0) {
        diag_error(&pp->cc->diag, pp_peek(pp)->loc, "#else without #if");
        pp_skip_line(pp);
        return;
    }

    int *current = &pp->if_stack.data[pp->if_stack.len - 1];
    if (*current == 2) {
        diag_error(&pp->cc->diag, pp_peek(pp)->loc, "duplicate #else");
        pp_skip_line(pp);
        return;
    }

    bool parent_active = true;
    for (int i = 0; i < pp->if_stack.len - 1; i++) {
        if (pp->if_stack.data[i] == 0 || pp->if_stack.data[i] == 3) {
            parent_active = false; break;
        }
    }

    /* Activate #else only if no prior branch was taken (state 0) and parent is active */
    /* State 1 = current #if/#elif was true, 3 = a prior branch was taken */
    *current = (parent_active && *current == 0) ? 2 : 0;
    pp_skip_line(pp);
}

static void pp_handle_endif(PPCtx *pp) {
    if (pp->if_stack.len == 0) {
        diag_error(&pp->cc->diag, pp_peek(pp)->loc, "#endif without #if");
    } else {
        pp->if_stack.len--;
        pp->if_depth--;
    }
    pp_skip_line(pp);
}

/* Full constant expression evaluator for #if/#elif with operator precedence.
   Precedence (low to high):
   ternary ?:  ->  ||  ->  &&  ->  |  ->  ^  ->  &
   ->  == !=  ->  < > <= >=  ->  << >>  ->  + -  ->  * / %
   ->  unary (! ~ - +)  ->  primary (int, defined, parens, macro-expanded idents)
*/

static bool pp_eval_at_end(PPCtx *pp) {
    if (pp->pos >= pp->input->count) return true;
    Token *t = &pp->input->tokens[pp->pos];
    return t->kind == TOK_EOF || t->at_bol;
}

/* Forward declarations for recursive descent */
static long long pp_eval_ternary(PPCtx *pp);
static long long pp_eval_or(PPCtx *pp);
static long long pp_eval_and(PPCtx *pp);
static long long pp_eval_bitor(PPCtx *pp);
static long long pp_eval_bitxor(PPCtx *pp);
static long long pp_eval_bitand(PPCtx *pp);
static long long pp_eval_eq(PPCtx *pp);
static long long pp_eval_rel(PPCtx *pp);
static long long pp_eval_shift(PPCtx *pp);
static long long pp_eval_add(PPCtx *pp);
static long long pp_eval_mul(PPCtx *pp);
static long long pp_eval_unary(PPCtx *pp);
static long long pp_eval_primary(PPCtx *pp);

static long long pp_eval_primary(PPCtx *pp) {
    if (pp_eval_at_end(pp)) return 0;
    Token *t = pp_peek(pp);

    if (t->kind == TOK_INT_LIT) {
        pp_advance(pp);
        return (long long)t->int_val;
    }
    if (t->kind == TOK_CHAR_LIT) {
        pp_advance(pp);
        return (long long)t->char_val;
    }
    if (t->kind == TOK_IDENT) {
        /* 'defined' operator */
        if (strcmp(t->text, "defined") == 0) {
            pp_advance(pp);
            bool paren = false;
            if (!pp_eval_at_end(pp) && pp_peek(pp)->kind == TOK_LPAREN) {
                pp_advance(pp);
                paren = true;
            }
            Token *name = pp_advance(pp);
            long long result = hashtab_get(&pp->macros, name->text) ? 1 : 0;
            if (paren && !pp_eval_at_end(pp) && pp_peek(pp)->kind == TOK_RPAREN)
                pp_advance(pp);
            return result;
        }
        /* Try macro expansion: look up identifier in macros */
        Macro *m = hashtab_get(&pp->macros, t->text);
        if (m && !m->is_func_like && m->body_len > 0) {
            pp_advance(pp); /* consume the macro name */
            /* Save current input state */
            TokenList *saved_input = pp->input;
            int saved_pos = pp->pos;
            /* Create a temporary token list from macro body */
            TokenList body_list;
            body_list.tokens = m->body;
            body_list.count = m->body_len;
            body_list.cap = m->body_len;
            /* Evaluate macro body as expression */
            pp->input = &body_list;
            pp->pos = 0;
            long long val = pp_eval_ternary(pp);
            /* Restore input state */
            pp->input = saved_input;
            pp->pos = saved_pos;
            return val;
        }
        /* Unknown identifier in #if -> 0 (per C standard) */
        pp_advance(pp);
        return 0;
    }
    if (t->kind == TOK_LPAREN) {
        pp_advance(pp);
        long long val = pp_eval_ternary(pp);
        if (!pp_eval_at_end(pp) && pp_peek(pp)->kind == TOK_RPAREN)
            pp_advance(pp);
        return val;
    }
    pp_advance(pp);
    return 0;
}

static long long pp_eval_unary(PPCtx *pp) {
    if (pp_eval_at_end(pp)) return 0;
    Token *t = pp_peek(pp);
    if (t->kind == TOK_BANG) {
        pp_advance(pp);
        return !pp_eval_unary(pp);
    }
    if (t->kind == TOK_TILDE) {
        pp_advance(pp);
        return ~pp_eval_unary(pp);
    }
    if (t->kind == TOK_MINUS) {
        pp_advance(pp);
        return -pp_eval_unary(pp);
    }
    if (t->kind == TOK_PLUS) {
        pp_advance(pp);
        return pp_eval_unary(pp);
    }
    return pp_eval_primary(pp);
}

static long long pp_eval_mul(PPCtx *pp) {
    long long left = pp_eval_unary(pp);
    while (!pp_eval_at_end(pp)) {
        Token *t = pp_peek(pp);
        if (t->kind == TOK_STAR) {
            pp_advance(pp); left = left * pp_eval_unary(pp);
        } else if (t->kind == TOK_SLASH) {
            pp_advance(pp);
            long long right = pp_eval_unary(pp);
            left = right ? left / right : 0;
        } else if (t->kind == TOK_PERCENT) {
            pp_advance(pp);
            long long right = pp_eval_unary(pp);
            left = right ? left % right : 0;
        } else break;
    }
    return left;
}

static long long pp_eval_add(PPCtx *pp) {
    long long left = pp_eval_mul(pp);
    while (!pp_eval_at_end(pp)) {
        Token *t = pp_peek(pp);
        if (t->kind == TOK_PLUS) {
            pp_advance(pp); left = left + pp_eval_mul(pp);
        } else if (t->kind == TOK_MINUS) {
            pp_advance(pp); left = left - pp_eval_mul(pp);
        } else break;
    }
    return left;
}

static long long pp_eval_shift(PPCtx *pp) {
    long long left = pp_eval_add(pp);
    while (!pp_eval_at_end(pp)) {
        Token *t = pp_peek(pp);
        if (t->kind == TOK_LSHIFT) {
            pp_advance(pp); left = left << pp_eval_add(pp);
        } else if (t->kind == TOK_RSHIFT) {
            pp_advance(pp); left = left >> pp_eval_add(pp);
        } else break;
    }
    return left;
}

static long long pp_eval_rel(PPCtx *pp) {
    long long left = pp_eval_shift(pp);
    while (!pp_eval_at_end(pp)) {
        Token *t = pp_peek(pp);
        if (t->kind == TOK_LT) {
            pp_advance(pp); left = left < pp_eval_shift(pp);
        } else if (t->kind == TOK_GT) {
            pp_advance(pp); left = left > pp_eval_shift(pp);
        } else if (t->kind == TOK_LE) {
            pp_advance(pp); left = left <= pp_eval_shift(pp);
        } else if (t->kind == TOK_GE) {
            pp_advance(pp); left = left >= pp_eval_shift(pp);
        } else break;
    }
    return left;
}

static long long pp_eval_eq(PPCtx *pp) {
    long long left = pp_eval_rel(pp);
    while (!pp_eval_at_end(pp)) {
        Token *t = pp_peek(pp);
        if (t->kind == TOK_EQ) {
            pp_advance(pp); left = left == pp_eval_rel(pp);
        } else if (t->kind == TOK_NE) {
            pp_advance(pp); left = left != pp_eval_rel(pp);
        } else break;
    }
    return left;
}

static long long pp_eval_bitand(PPCtx *pp) {
    long long left = pp_eval_eq(pp);
    while (!pp_eval_at_end(pp) && pp_peek(pp)->kind == TOK_AMP) {
        pp_advance(pp); left = left & pp_eval_eq(pp);
    }
    return left;
}

static long long pp_eval_bitxor(PPCtx *pp) {
    long long left = pp_eval_bitand(pp);
    while (!pp_eval_at_end(pp) && pp_peek(pp)->kind == TOK_CARET) {
        pp_advance(pp); left = left ^ pp_eval_bitand(pp);
    }
    return left;
}

static long long pp_eval_bitor(PPCtx *pp) {
    long long left = pp_eval_bitxor(pp);
    while (!pp_eval_at_end(pp) && pp_peek(pp)->kind == TOK_PIPE) {
        pp_advance(pp); left = left | pp_eval_bitxor(pp);
    }
    return left;
}

static long long pp_eval_and(PPCtx *pp) {
    long long left = pp_eval_bitor(pp);
    while (!pp_eval_at_end(pp) && pp_peek(pp)->kind == TOK_AND) {
        pp_advance(pp);
        long long right = pp_eval_bitor(pp);
        left = left && right;
    }
    return left;
}

static long long pp_eval_or(PPCtx *pp) {
    long long left = pp_eval_and(pp);
    while (!pp_eval_at_end(pp) && pp_peek(pp)->kind == TOK_OR) {
        pp_advance(pp);
        long long right = pp_eval_and(pp);
        left = left || right;
    }
    return left;
}

static long long pp_eval_ternary(PPCtx *pp) {
    long long cond = pp_eval_or(pp);
    if (!pp_eval_at_end(pp) && pp_peek(pp)->kind == TOK_QUESTION) {
        pp_advance(pp);
        long long then_val = pp_eval_ternary(pp);
        if (!pp_eval_at_end(pp) && pp_peek(pp)->kind == TOK_COLON)
            pp_advance(pp);
        long long else_val = pp_eval_ternary(pp);
        return cond ? then_val : else_val;
    }
    return cond;
}

static long long pp_eval_constant(PPCtx *pp) {
    return pp_eval_ternary(pp);
}

static void pp_handle_include(PPCtx *pp) {
    Token *t = pp_advance(pp);

    const char *filename = NULL;
    bool is_system = false;

    if (t->kind == TOK_STRING_LIT) {
        filename = t->text;
    } else if (t->kind == TOK_LT) {
        /* System include: <...> */
        is_system = true;
        /* Collect tokens until > */
        char buf[4096];
        int len = 0;
        while (pp_peek(pp)->kind != TOK_GT && pp_peek(pp)->kind != TOK_EOF) {
            Token *part = pp_advance(pp);
            int tlen = part->text_len;
            if (len + tlen < (int)sizeof(buf) - 1) {
                memcpy(buf + len, part->text, tlen);
                len += tlen;
            }
        }
        if (pp_peek(pp)->kind == TOK_GT) pp_advance(pp);
        buf[len] = '\0';
        filename = arena_strdup(&pp->cc->arena, buf);
    } else {
        diag_error(&pp->cc->diag, t->loc, "expected filename after #include");
        pp_skip_line(pp);
        return;
    }

    pp_skip_line(pp);

    /* Search for file */
    char path[4096];
    bool found = false;

    if (!is_system) {
        /* Try relative to current file first */
        const char *cur = pp_peek(pp)->loc.filename;
        if (cur) {
            const char *slash = strrchr(cur, '/');
            if (slash) {
                int dirlen = (int)(slash - cur + 1);
                snprintf(path, sizeof(path), "%.*s%s", dirlen, cur, filename);
            } else {
                snprintf(path, sizeof(path), "%s", filename);
            }
            FILE *f = fopen(path, "r");
            if (f) { fclose(f); found = true; }
        }
    }

    if (!found) {
        /* Search include paths */
        for (int i = 0; i < pp->cc->include_paths.len; i++) {
            snprintf(path, sizeof(path), "%s/%s", pp->cc->include_paths.data[i], filename);
            FILE *f = fopen(path, "r");
            if (f) { fclose(f); found = true; break; }
        }
    }

    if (!found) {
        /* Try system paths */
        const char *sys_paths[] = {"/usr/include", "/usr/local/include", NULL};
        for (int i = 0; sys_paths[i]; i++) {
            snprintf(path, sizeof(path), "%s/%s", sys_paths[i], filename);
            FILE *f = fopen(path, "r");
            if (f) { fclose(f); found = true; break; }
        }
    }

    if (!found) {
        diag_error(&pp->cc->diag, t->loc, "cannot find include file '%s'", filename);
        return;
    }

    /* Skip files that used #pragma once */
    if (hashtab_get(&pp->pragma_once, path))
        return;

    /* Lex and preprocess the included file */
    TokenList *inc_tokens = cc_lex(pp->cc, path);
    if (!inc_tokens) return;

    /* Recursively preprocess */
    PPCtx sub;
    pp_init(&sub, pp->cc, inc_tokens);
    /* Copy macros */
    HashIter it = hashtab_iter(&pp->macros);
    const char *key;
    void *val;
    while (hashtab_next(&it, &key, &val)) {
        hashtab_put(&sub.macros, key, val);
    }

    /* Process included tokens */
    while (pp_peek(&sub)->kind != TOK_EOF) {
        /* Simplified: just copy through the recursive call */
        Token *st = pp_advance(&sub);
        pp_emit(pp, st);
    }

    /* Copy back any new macros */
    it = hashtab_iter(&sub.macros);
    while (hashtab_next(&it, &key, &val)) {
        hashtab_put(&pp->macros, key, val);
    }

    hashtab_free(&sub.macros);
    vec_free(&sub.include_stack);
    vec_free(&sub.if_stack);
}

/* Get token text, falling back to token kind string for punctuation */
static const char *pp_token_text(Token *t, int *out_len) {
    if (t->text && t->text_len > 0) {
        *out_len = t->text_len;
        return t->text;
    }
    /* Fallback for punctuation/operators that don't store text */
    const char *s = token_kind_str(t->kind);
    *out_len = (int)strlen(s);
    return s;
}

/* Stringify a sequence of tokens into a string literal token */
static Token pp_stringify(PPCtx *pp, Token *toks, int ntoks) {
    char buf[4096];
    int len = 0;
    for (int i = 0; i < ntoks; i++) {
        if (i > 0 && toks[i].has_space && len < (int)sizeof(buf) - 1)
            buf[len++] = ' ';
        int tl;
        const char *text = pp_token_text(&toks[i], &tl);
        if (len + tl < (int)sizeof(buf) - 1) {
            memcpy(buf + len, text, tl);
            len += tl;
        }
    }
    buf[len] = '\0';
    Token result = {0};
    result.kind = TOK_STRING_LIT;
    result.text = arena_strdup(&pp->cc->arena, buf);
    result.text_len = len;
    return result;
}

/* Concatenate two tokens' text and re-lex as a single token */
static Token pp_paste(PPCtx *pp, Token *left, Token *right) {
    char buf[512];
    int ll, rl;
    const char *lt = pp_token_text(left, &ll);
    const char *rt = pp_token_text(right, &rl);
    if (ll + rl >= (int)sizeof(buf)) {
        /* Too long, just return right */
        return *right;
    }
    memcpy(buf, lt, ll);
    memcpy(buf + ll, rt, rl);
    buf[ll + rl] = '\0';

    /* Create a new token with the concatenated text */
    Token result = *left;
    result.text = arena_strdup(&pp->cc->arena, buf);
    result.text_len = ll + rl;
    /* Try to determine the kind: if it's all digits, make it an int literal,
     * if it's a valid identifier, make it an ident, otherwise keep left's kind */
    bool all_digits = true;
    bool valid_ident = true;
    for (int i = 0; buf[i]; i++) {
        if (!(buf[i] >= '0' && buf[i] <= '9')) all_digits = false;
        if (!(buf[i] == '_' || (buf[i] >= 'a' && buf[i] <= 'z') ||
              (buf[i] >= 'A' && buf[i] <= 'Z') || (buf[i] >= '0' && buf[i] <= '9')))
            valid_ident = false;
    }
    if (all_digits && ll + rl > 0) {
        result.kind = TOK_INT_LIT;
        result.int_val = 0;
        for (int i = 0; buf[i]; i++)
            result.int_val = result.int_val * 10 + (buf[i] - '0');
    } else if (valid_ident && ll + rl > 0) {
        result.kind = TOK_IDENT;
        result.text = intern(&pp->cc->strings, buf);
    }
    return result;
}

static void pp_rescan_emit(PPCtx *pp, Token *toks, int ntoks, int depth);

/* Perform parameter substitution with # and ## handling for function-like macros.
 * Returns a Vec(Token) of the substituted body. */
static void pp_substitute_params(PPCtx *pp, Macro *m, Vec(Token) *args,
                                 int num_args, Vec(Token) *out)
{
    for (int i = 0; i < m->body_len; i++) {
        Token bt = m->body[i];

        /* # operator (stringization) */
        if (bt.kind == TOK_HASH && i + 1 < m->body_len) {
            Token next = m->body[i + 1];
            if (next.kind == TOK_IDENT) {
                for (int p = 0; p < m->num_params; p++) {
                    if (next.text == m->params[p]) {
                        Token str = pp_stringify(pp, args[p].data, args[p].len);
                        str.loc = bt.loc;
                        vec_push(out, str);
                        i++; /* skip the parameter name */
                        goto next_body_token;
                    }
                }
            }
        }

        /* ## operator (token pasting) — supports chained: a ## b ## c */
        if (i + 1 < m->body_len && m->body[i + 1].kind == TOK_HASHHASH) {
            /* Collect left token (possibly substituted) */
            Token left = bt;
            if (bt.kind == TOK_IDENT) {
                for (int p = 0; p < m->num_params; p++) {
                    if (bt.text == m->params[p]) {
                        if (args[p].len > 0)
                            left = args[p].data[args[p].len - 1];
                        else {
                            left.kind = TOK_IDENT;
                            left.text = "";
                            left.text_len = 0;
                        }
                        /* Push all args except the last (which will be pasted) */
                        for (int a = 0; a < args[p].len - 1; a++)
                            vec_push(out, args[p].data[a]);
                        break;
                    }
                }
            }

            /* Loop: keep pasting while next is ## */
            while (i + 1 < m->body_len && m->body[i + 1].kind == TOK_HASHHASH) {
                i += 2; /* skip ## and get right token */
                if (i >= m->body_len) break;
                Token right = m->body[i];
                /* Substitute right if it's a parameter */
                bool right_subst = false;
                if (right.kind == TOK_IDENT) {
                    for (int p = 0; p < m->num_params; p++) {
                        if (right.text == m->params[p]) {
                            if (args[p].len > 0) {
                                Token first_right = args[p].data[0];
                                left = pp_paste(pp, &left, &first_right);
                                /* If more ## follows, only the last arg token matters;
                                 * otherwise push remaining arg tokens after final paste */
                                if (!(i + 1 < m->body_len && m->body[i + 1].kind == TOK_HASHHASH)) {
                                    vec_push(out, left);
                                    for (int a = 1; a < args[p].len; a++)
                                        vec_push(out, args[p].data[a]);
                                    goto next_body_token;
                                }
                                /* More ## follows: take last arg token as new left */
                                if (args[p].len > 1) {
                                    vec_push(out, left);
                                    for (int a = 1; a < args[p].len - 1; a++)
                                        vec_push(out, args[p].data[a]);
                                    left = args[p].data[args[p].len - 1];
                                }
                            } else {
                                /* Empty arg: left stays unchanged */
                            }
                            right_subst = true;
                            break;
                        }
                    }
                }
                if (!right_subst) {
                    left = pp_paste(pp, &left, &right);
                }
            }
            vec_push(out, left);
            continue;
        }

        /* Normal parameter substitution (with argument prescan/pre-expansion) */
        if (bt.kind == TOK_IDENT) {
            bool substituted = false;
            for (int p = 0; p < m->num_params; p++) {
                if (bt.text == m->params[p]) {
                    /* Pre-expand argument tokens (C standard: args are expanded
                     * before substitution, except when used with # or ##) */
                    if (args[p].len > 0) {
                        TokenList tmp_out = {0};
                        TokenList *saved_out = pp->output;
                        pp->output = &tmp_out;
                        pp_rescan_emit(pp, args[p].data, args[p].len, 0);
                        pp->output = saved_out;
                        for (int a = 0; a < tmp_out.count; a++)
                            vec_push(out, tmp_out.tokens[a]);
                    }
                    substituted = true;
                    break;
                }
            }
            /* Handle __VA_ARGS__ for variadic macros */
            if (!substituted && m->is_variadic && strcmp(bt.text, "__VA_ARGS__") == 0) {
                int va_idx = m->num_params;
                if (va_idx < num_args + 1 && args[va_idx].len > 0) {
                    TokenList tmp_out = {0};
                    TokenList *saved_out = pp->output;
                    pp->output = &tmp_out;
                    pp_rescan_emit(pp, args[va_idx].data, args[va_idx].len, 0);
                    pp->output = saved_out;
                    for (int a = 0; a < tmp_out.count; a++)
                        vec_push(out, tmp_out.tokens[a]);
                }
                substituted = true;
            }
            if (!substituted) vec_push(out, bt);
        } else {
            vec_push(out, bt);
        }
        continue;
    next_body_token:;
    }
}

/* Rescan and expand a token sequence, emitting results to pp.
 * This handles both object-like and function-like macros in the token stream. */
static void pp_rescan_emit(PPCtx *pp, Token *toks, int ntoks, int depth) {
    if (depth > 256) {
        for (int i = 0; i < ntoks; i++) pp_emit(pp, &toks[i]);
        return;
    }
    int pos = 0;
    while (pos < ntoks) {
        Token *t = &toks[pos];
        if (t->kind == TOK_IDENT) {
            /* Handle __LINE__/__FILE__ inside macro expansions */
            if (strcmp(t->text, "__LINE__") == 0) {
                Token line_tok = {0};
                line_tok.kind = TOK_INT_LIT;
                char buf[32];
                snprintf(buf, sizeof(buf), "%d", t->loc.line);
                line_tok.text = arena_strdup(&pp->cc->arena, buf);
                line_tok.text_len = (int)strlen(line_tok.text);
                line_tok.int_val = t->loc.line;
                line_tok.loc = t->loc;
                pp_emit(pp, &line_tok);
                pos++;
                continue;
            }
            if (strcmp(t->text, "__FILE__") == 0) {
                const char *fname = t->loc.filename ? t->loc.filename : "<unknown>";
                Token file_tok = {0};
                file_tok.kind = TOK_STRING_LIT;
                file_tok.text = fname;
                file_tok.text_len = (int)strlen(fname);
                file_tok.loc = t->loc;
                pp_emit(pp, &file_tok);
                pos++;
                continue;
            }
            Macro *m = hashtab_get(&pp->macros, t->text);
            if (m && !m->is_func_like) {
                /* Object-like macro: expand body recursively */
                pp_rescan_emit(pp, m->body, m->body_len, depth + 1);
                pos++;
                continue;
            }
            if (m && m->is_func_like && pos + 1 < ntoks && toks[pos + 1].kind == TOK_LPAREN) {
                /* Function-like macro: collect args from the token stream */
                pos += 2; /* skip name and ( */
                Vec(Token) *args = calloc(m->num_params + 1, sizeof(Vec(Token)));
                int arg_idx = 0;
                int paren_depth = 0;
                while (pos < ntoks) {
                    Token *at = &toks[pos];
                    if (at->kind == TOK_LPAREN) { paren_depth++; vec_push(&args[arg_idx], *at); pos++; }
                    else if (at->kind == TOK_RPAREN) {
                        if (paren_depth == 0) { pos++; break; }
                        paren_depth--;
                        vec_push(&args[arg_idx], *at);
                        pos++;
                    }
                    else if (at->kind == TOK_COMMA && paren_depth == 0) {
                        if (m->is_variadic && arg_idx >= m->num_params) {
                            vec_push(&args[arg_idx], *at);
                            pos++;
                        } else {
                            pos++;
                            arg_idx++;
                        }
                    }
                    else { vec_push(&args[arg_idx], *at); pos++; }
                }
                /* Substitute parameters with # and ## handling, then rescan */
                Vec(Token) result = {0};
                pp_substitute_params(pp, m, args, arg_idx, &result);
                pp_rescan_emit(pp, result.data, result.len, depth + 1);
                vec_free(&result);
                for (int i = 0; i <= arg_idx; i++) vec_free(&args[i]);
                free(args);
                continue;
            }
        }
        pp_emit(pp, t);
        pos++;
    }
}

static bool pp_expand_macro(PPCtx *pp, Token *ident) {
    Macro *m = hashtab_get(&pp->macros, ident->text);
    if (!m) return false;

    if (m->is_func_like) {
        /* Save position, advance past the identifier to look for ( */
        int saved_pos = pp->pos;
        pp_advance(pp); /* skip the macro name */
        /* Check for ( */
        if (pp_peek(pp)->kind != TOK_LPAREN) {
            pp->pos = saved_pos; /* restore position if not a function call */
            return false;
        }
        pp_advance(pp); /* consume ( */

        /* Collect arguments */
        Vec(Token) *args = calloc(m->num_params + 1, sizeof(Vec(Token)));
        int arg_idx = 0;
        int paren_depth = 0;

        while (pp_peek(pp)->kind != TOK_EOF) {
            Token *t = pp_peek(pp);
            if (t->kind == TOK_LPAREN) { paren_depth++; vec_push(&args[arg_idx], *t); pp_advance(pp); }
            else if (t->kind == TOK_RPAREN) {
                if (paren_depth == 0) { pp_advance(pp); break; }
                paren_depth--;
                vec_push(&args[arg_idx], *t);
                pp_advance(pp);
            }
            else if (t->kind == TOK_COMMA && paren_depth == 0) {
                if (m->is_variadic && arg_idx >= m->num_params) {
                    vec_push(&args[arg_idx], *t);
                    pp_advance(pp);
                } else {
                    pp_advance(pp);
                    arg_idx++;
                }
            }
            else {
                vec_push(&args[arg_idx], *t);
                pp_advance(pp);
            }
        }

        /* Substitute parameters with # and ## handling */
        Vec(Token) result = {0};
        pp_substitute_params(pp, m, args, arg_idx, &result);
        /* Rescan the substituted tokens for further macro expansion */
        pp_rescan_emit(pp, result.data, result.len, 0);
        vec_free(&result);

        for (int i = 0; i <= arg_idx; i++) vec_free(&args[i]);
        free(args);
        return true;
    }

    /* Object-like macro: advance past the identifier and rescan body */
    pp_advance(pp); /* skip the macro name */
    pp_rescan_emit(pp, m->body, m->body_len, 0);
    return true;
}

static void pp_handle_pragma(PPCtx *pp) {
    Token *t = pp_peek(pp);
    if (t->kind == TOK_IDENT && t->text_len == 4 &&
        memcmp(t->text, "once", 4) == 0) {
        /* #pragma once — mark current file as included */
        if (t->loc.filename)
            hashtab_put(&pp->pragma_once, t->loc.filename, (void*)1);
    }
    pp_skip_line(pp);
}

static void pp_handle_error(PPCtx *pp) {
    Token *t = pp_peek(pp);
    char msg[1024] = {0};
    int len = 0;
    while (pp->pos < pp->input->count) {
        Token *tok = &pp->input->tokens[pp->pos];
        if (tok->kind == TOK_EOF || tok->at_bol) break;
        if (len > 0 && len < (int)sizeof(msg) - 1) msg[len++] = ' ';
        int tl = tok->text_len;
        if (len + tl < (int)sizeof(msg)) {
            memcpy(msg + len, tok->text, tl);
            len += tl;
        }
        pp->pos++;
    }
    msg[len] = '\0';
    diag_error(&pp->cc->diag, t->loc, "#error %s", msg);
}

static void pp_handle_directive(PPCtx *pp) {
    /* Skip the # token */
    Token *hash = pp_advance(pp); /* # */
    (void)hash;

    Token *dir = pp_peek(pp);
    if (dir->at_bol || dir->kind == TOK_EOF) return; /* empty directive */

    /* Handle keywords that are also preprocessor directive names (if, else) */
    if (dir->kind == TOK_KW_IF) {
        pp_advance(pp);
        pp_handle_if(pp);
        return;
    }
    if (dir->kind == TOK_KW_ELSE) {
        pp_advance(pp);
        pp_handle_else(pp);
        return;
    }

    if (dir->kind != TOK_IDENT) {
        /* # followed by number = line directive, skip */
        pp_skip_line(pp);
        return;
    }

    const char *name = dir->text;
    pp_advance(pp); /* consume directive name */

    if (strcmp(name, "define") == 0) {
        if (pp_is_active(pp)) pp_handle_define(pp);
        else pp_skip_line(pp);
    } else if (strcmp(name, "undef") == 0) {
        if (pp_is_active(pp)) pp_handle_undef(pp);
        else pp_skip_line(pp);
    } else if (strcmp(name, "include") == 0) {
        if (pp_is_active(pp)) pp_handle_include(pp);
        else pp_skip_line(pp);
    } else if (strcmp(name, "ifdef") == 0) {
        pp_handle_ifdef(pp, false);
    } else if (strcmp(name, "ifndef") == 0) {
        pp_handle_ifdef(pp, true);
    } else if (strcmp(name, "if") == 0) {
        pp_handle_if(pp);
    } else if (strcmp(name, "elif") == 0) {
        pp_handle_elif(pp);
    } else if (strcmp(name, "else") == 0) {
        pp_handle_else(pp);
    } else if (strcmp(name, "endif") == 0) {
        pp_handle_endif(pp);
    } else if (strcmp(name, "pragma") == 0) {
        if (pp_is_active(pp)) pp_handle_pragma(pp);
        else pp_skip_line(pp);
    } else if (strcmp(name, "error") == 0) {
        if (pp_is_active(pp)) pp_handle_error(pp);
        else pp_skip_line(pp);
    } else if (strcmp(name, "warning") == 0) {
        if (pp_is_active(pp)) {
            Token *wt = pp_peek(pp);
            char msg[1024] = {0};
            int len = 0;
            while (pp->pos < pp->input->count) {
                Token *tok = &pp->input->tokens[pp->pos];
                if (tok->kind == TOK_EOF || tok->at_bol) break;
                if (len > 0 && len < (int)sizeof(msg) - 1) msg[len++] = ' ';
                int tl = tok->text_len;
                if (len + tl < (int)sizeof(msg)) {
                    memcpy(msg + len, tok->text, tl);
                    len += tl;
                }
                pp->pos++;
            }
            msg[len] = '\0';
            diag_warn(&pp->cc->diag, wt->loc, "#warning %s", msg);
        } else {
            pp_skip_line(pp);
        }
    } else if (strcmp(name, "line") == 0) {
        pp_skip_line(pp);
    } else {
        if (pp_is_active(pp)) {
            diag_warn(&pp->cc->diag, dir->loc, "unknown preprocessor directive '%s'", name);
        }
        pp_skip_line(pp);
    }
}

TokenList *cc_preprocess(CompilerCtx *cc, TokenList *tokens) {
    PPCtx pp;
    pp_init(&pp, cc, tokens);

    /* Add command-line defines */
    for (int i = 0; i < cc->defines.len; i++) {
        const char *def = cc->defines.data[i];
        const char *eq = strchr(def, '=');
        if (eq) {
            const char *name = intern_n(&cc->strings, def, eq - def);
            /* Create a single token for the value */
            Token val_tok = {0};
            val_tok.kind = TOK_INT_LIT;
            val_tok.text = eq + 1;
            val_tok.text_len = strlen(eq + 1);
            Token *body = arena_alloc(&cc->arena, sizeof(Token));
            *body = val_tok;
            pp_define_object(&pp, name, body, 1);
        } else {
            const char *name = intern(&cc->strings, def);
            /* Define as 1 */
            Token val_tok = {0};
            val_tok.kind = TOK_INT_LIT;
            val_tok.text = "1";
            val_tok.text_len = 1;
            val_tok.int_val = 1;
            Token *body = arena_alloc(&cc->arena, sizeof(Token));
            *body = val_tok;
            pp_define_object(&pp, name, body, 1);
        }
    }

    /* Add predefined macros */
    {
        const char *predef[] = {
            "__STDC__", "__STDC_VERSION__", "__STDC_HOSTED__",
            NULL
        };
        for (int i = 0; predef[i]; i++) {
            const char *name = intern(&cc->strings, predef[i]);
            Token val_tok = {0};
            val_tok.kind = TOK_INT_LIT;
            val_tok.text = "1";
            val_tok.text_len = 1;
            val_tok.int_val = 1;
            Token *body = arena_alloc(&cc->arena, sizeof(Token));
            *body = val_tok;
            pp_define_object(&pp, name, body, 1);
        }
    }

    /* Process tokens */
    while (pp_peek(&pp)->kind != TOK_EOF) {
        Token *t = pp_peek(&pp);

        /* Directive */
        if (t->at_bol && t->kind == TOK_HASH) {
            pp_handle_directive(&pp);
            continue;
        }

        /* Skip tokens in inactive #if blocks */
        if (!pp_is_active(&pp)) {
            pp_advance(&pp);
            continue;
        }

        /* Try macro expansion */
        if (t->kind == TOK_IDENT) {
            /* Handle built-in macros that depend on source location */
            if (strcmp(t->text, "__LINE__") == 0) {
                pp_advance(&pp);
                Token line_tok = {0};
                line_tok.kind = TOK_INT_LIT;
                char buf[32];
                snprintf(buf, sizeof(buf), "%d", t->loc.line);
                line_tok.text = arena_strdup(&cc->arena, buf);
                line_tok.text_len = (int)strlen(line_tok.text);
                line_tok.int_val = t->loc.line;
                line_tok.loc = t->loc;
                pp_emit(&pp, &line_tok);
                continue;
            }
            if (strcmp(t->text, "__FILE__") == 0) {
                pp_advance(&pp);
                const char *fname = t->loc.filename ? t->loc.filename : "<unknown>";
                Token file_tok = {0};
                file_tok.kind = TOK_STRING_LIT;
                file_tok.text = fname;
                file_tok.text_len = (int)strlen(fname);
                file_tok.loc = t->loc;
                pp_emit(&pp, &file_tok);
                continue;
            }
            if (strcmp(t->text, "__DATE__") == 0) {
                pp_advance(&pp);
                time_t now = time(NULL);
                struct tm *tm = localtime(&now);
                char buf[32];
                strftime(buf, sizeof(buf), "%b %d %Y", tm);
                Token date_tok = {0};
                date_tok.kind = TOK_STRING_LIT;
                date_tok.text = arena_strdup(&cc->arena, buf);
                date_tok.text_len = (int)strlen(date_tok.text);
                date_tok.loc = t->loc;
                pp_emit(&pp, &date_tok);
                continue;
            }
            if (strcmp(t->text, "__TIME__") == 0) {
                pp_advance(&pp);
                time_t now = time(NULL);
                struct tm *tm = localtime(&now);
                char buf[16];
                strftime(buf, sizeof(buf), "%H:%M:%S", tm);
                Token time_tok = {0};
                time_tok.kind = TOK_STRING_LIT;
                time_tok.text = arena_strdup(&cc->arena, buf);
                time_tok.text_len = (int)strlen(time_tok.text);
                time_tok.loc = t->loc;
                pp_emit(&pp, &time_tok);
                continue;
            }

            if (pp_expand_macro(&pp, t)) {
                /* pp_expand_macro already consumed the identifier (and args
                 * for function-like macros), so just continue. */
                continue;
            }
        }

        /* Pass through */
        pp_emit(&pp, pp_advance(&pp));
    }

    /* Emit EOF */
    Token eof = {0};
    eof.kind = TOK_EOF;
    pp_emit(&pp, &eof);

    if (pp.if_stack.len > 0) {
        diag_error(&cc->diag, pp_peek(&pp)->loc, "unterminated #if");
    }

    TokenList *result = pp.output;
    hashtab_free(&pp.macros);
    vec_free(&pp.include_stack);
    vec_free(&pp.if_stack);
    return result;
}
