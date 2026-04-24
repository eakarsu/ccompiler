#ifndef CC_H
#define CC_H

/* Master header for mycc compiler */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "arena.h"
#include "vec.h"
#include "hashtab.h"
#include "strintern.h"
#include "diag.h"
#include "token.h"
#include "type.h"
#include "ast.h"
#include "ir.h"
#include "opt.h"
#include "target.h"

/* Compiler context - threaded through all phases */
typedef struct {
    Arena arena;           /* main allocation arena */
    Arena tmp_arena;       /* scratch arena (reset between functions) */
    InternTab strings;     /* string interning */
    DiagCtx diag;          /* diagnostics */
    Target *target;        /* target machine */
    OptLevel opt_level;    /* optimization level */

    /* Source files */
    const char *input_file;
    const char *output_file;

    /* Mode flags */
    bool preprocess_only;  /* -E */
    bool compile_only;     /* -S */
    bool assemble_only;    /* -c */
    bool dump_ast;         /* --dump-ast */
    bool dump_ir;          /* --dump-ir */

    /* Warning flags */
    bool wall;
    bool werror;

    /* Include paths */
    Vec(const char *) include_paths;

    /* Preprocessor defines from command line */
    Vec(const char *) defines;

} CompilerCtx;

void cc_init(CompilerCtx *cc);
void cc_destroy(CompilerCtx *cc);

/* Pipeline stages */
typedef struct TokenList TokenList;
TokenList *cc_lex(CompilerCtx *cc, const char *filename);
TokenList *cc_preprocess(CompilerCtx *cc, TokenList *tokens);
Node      *cc_parse(CompilerCtx *cc, TokenList *tokens);
void       cc_sema(CompilerCtx *cc, Node *program);
IRModule  *cc_irgen(CompilerCtx *cc, Node *program);
void       cc_optimize(CompilerCtx *cc, IRModule *mod);
void       cc_codegen(CompilerCtx *cc, IRModule *mod, FILE *out);

/* Token list for passing between phases */
typedef struct TokenList {
    Token *tokens;
    int count;
    int cap;
} TokenList;

#endif /* CC_H */
