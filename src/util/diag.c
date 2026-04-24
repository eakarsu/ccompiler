#include "diag.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void diag_init(DiagCtx *ctx) {
    ctx->errors = 0;
    ctx->warnings = 0;
    ctx->warnings_as_errors = false;
    ctx->suppress_warnings = false;
    ctx->max_errors = 20;
}

static const char *level_str(DiagLevel level) {
    switch (level) {
        case DIAG_NOTE:    return "note";
        case DIAG_WARNING: return "warning";
        case DIAG_ERROR:   return "error";
        case DIAG_FATAL:   return "fatal error";
    }
    return "unknown";
}

static const char *level_color(DiagLevel level) {
    switch (level) {
        case DIAG_NOTE:    return "\033[1;36m";  /* bold cyan */
        case DIAG_WARNING: return "\033[1;35m";  /* bold magenta */
        case DIAG_ERROR:   return "\033[1;31m";  /* bold red */
        case DIAG_FATAL:   return "\033[1;31m";  /* bold red */
    }
    return "";
}

void diag_emit(DiagCtx *ctx, DiagLevel level, SrcLoc loc, const char *fmt, ...) {
    /* Promote warnings to errors if -Werror */
    if (level == DIAG_WARNING && ctx->warnings_as_errors) {
        level = DIAG_ERROR;
    }
    if (level == DIAG_WARNING && ctx->suppress_warnings) {
        return;
    }

    /* Update counters */
    if (level == DIAG_ERROR || level == DIAG_FATAL) {
        ctx->errors++;
    } else if (level == DIAG_WARNING) {
        ctx->warnings++;
    }

    /* Print location */
    FILE *out = stderr;
    bool use_color = true;  /* TODO: detect tty */

    if (loc.filename) {
        if (use_color) fprintf(out, "\033[1m");
        fprintf(out, "%s", loc.filename);
        if (loc.line > 0) {
            fprintf(out, ":%d", loc.line);
            if (loc.col > 0) fprintf(out, ":%d", loc.col);
        }
        fprintf(out, ": ");
        if (use_color) fprintf(out, "\033[0m");
    }

    /* Print severity */
    if (use_color) fprintf(out, "%s", level_color(level));
    fprintf(out, "%s: ", level_str(level));
    if (use_color) fprintf(out, "\033[0m");

    /* Print message */
    if (use_color) fprintf(out, "\033[1m");
    va_list ap;
    va_start(ap, fmt);
    vfprintf(out, fmt, ap);
    va_end(ap);
    if (use_color) fprintf(out, "\033[0m");
    fprintf(out, "\n");

    /* Print source line with caret if available */
    if (loc.line_start && loc.col > 0) {
        const char *p = loc.line_start;
        /* Print the source line */
        while (*p && *p != '\n' && *p != '\r') {
            fputc(*p, out);
            p++;
        }
        fputc('\n', out);

        /* Print caret */
        if (use_color) fprintf(out, "\033[1;32m");
        for (int i = 1; i < loc.col; i++) {
            fputc(loc.line_start[i-1] == '\t' ? '\t' : ' ', out);
        }
        fputc('^', out);
        if (use_color) fprintf(out, "\033[0m");
        fputc('\n', out);
    }

    if (level == DIAG_FATAL) {
        exit(1);
    }
}

bool diag_too_many_errors(DiagCtx *ctx) {
    return ctx->max_errors > 0 && ctx->errors >= ctx->max_errors;
}
