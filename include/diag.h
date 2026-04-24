#ifndef DIAG_H
#define DIAG_H

#include <stdarg.h>
#include <stdbool.h>

/* Source location tracking */
typedef struct {
    const char *filename;
    int line;
    int col;
    const char *line_start;  /* pointer into source buffer for caret display */
} SrcLoc;

#define SRCLOC_NONE ((SrcLoc){NULL, 0, 0, NULL})

/* Diagnostic severity */
typedef enum {
    DIAG_NOTE,
    DIAG_WARNING,
    DIAG_ERROR,
    DIAG_FATAL,
} DiagLevel;

/* Diagnostic context (tracks error/warning counts) */
typedef struct {
    int errors;
    int warnings;
    bool warnings_as_errors;
    bool suppress_warnings;
    int max_errors;  /* 0 = unlimited */
} DiagCtx;

void diag_init(DiagCtx *ctx);

/* Report a diagnostic with printf-style formatting */
void diag_emit(DiagCtx *ctx, DiagLevel level, SrcLoc loc, const char *fmt, ...)
    __attribute__((format(printf, 4, 5)));

/* Convenience macros */
#define diag_error(ctx, loc, ...) diag_emit(ctx, DIAG_ERROR, loc, __VA_ARGS__)
#define diag_warn(ctx, loc, ...)  diag_emit(ctx, DIAG_WARNING, loc, __VA_ARGS__)
#define diag_note(ctx, loc, ...)  diag_emit(ctx, DIAG_NOTE, loc, __VA_ARGS__)
#define diag_fatal(ctx, loc, ...) do { diag_emit(ctx, DIAG_FATAL, loc, __VA_ARGS__); } while(0)

/* Check if we should stop compilation */
bool diag_too_many_errors(DiagCtx *ctx);

#endif /* DIAG_H */
