#include "cc.h"
#include <errno.h>
#include <unistd.h>

static void usage(const char *prog) {
    fprintf(stderr,
        "mycc - A C11 compiler with SSA-based optimizations\n"
        "Targets: ARM64 (Apple Silicon), x86-64\n"
        "\n"
        "Usage: %s [options] <input.c>\n"
        "\n"
        "Compilation modes:\n"
        "  -E              Preprocess only (output to stdout)\n"
        "  -S              Compile to assembly (.s)\n"
        "  -c              Compile and assemble to object (.o)\n"
        "  (default)       Compile, assemble, and link executable\n"
        "\n"
        "Output:\n"
        "  -o <file>       Write output to <file>\n"
        "\n"
        "Optimization:\n"
        "  -O0             No optimization (default)\n"
        "  -O1             Basic: mem2reg, DCE, simplifycfg, instcombine\n"
        "  -O2             Full: + SCCP, GVN, LICM\n"
        "  -O3             Aggressive: + inlining, loop unrolling\n"
        "\n"
        "Preprocessor:\n"
        "  -I<dir>         Add include search path\n"
        "  -D<name>[=val]  Define preprocessor macro\n"
        "\n"
        "Warnings:\n"
        "  -Wall           Enable all warnings\n"
        "  -Werror         Treat warnings as errors\n"
        "\n"
        "Debug:\n"
        "  --dump-ast      Print AST and exit\n"
        "  --dump-ir       Print SSA IR after optimization and exit\n"
        "  -v              Verbose output\n"
        "\n"
        "Target:\n"
        "  -target <arch>  Target architecture (x86-64, arm64)\n"
        "\n"
        "  -h, --help      Show this help\n",
        prog);
}

static char *read_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "error: cannot open '%s': %s\n", path, strerror(errno));
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buf = malloc(size + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }

    size_t nread = fread(buf, 1, size, f);
    buf[nread] = '\0';
    fclose(f);
    return buf;
}

void cc_init(CompilerCtx *cc) {
    arena_init(&cc->arena, 0);
    arena_init(&cc->tmp_arena, 0);
    intern_init(&cc->strings, &cc->arena);
    diag_init(&cc->diag);
    type_init(&cc->arena);
    cc->target = NULL;
    cc->opt_level = OPT_O0;
    cc->input_file = NULL;
    cc->output_file = NULL;
    cc->preprocess_only = false;
    cc->compile_only = false;
    cc->assemble_only = false;
    cc->dump_ast = false;
    cc->dump_ir = false;
    cc->wall = false;
    cc->werror = false;
    memset(&cc->include_paths, 0, sizeof(cc->include_paths));
    memset(&cc->defines, 0, sizeof(cc->defines));
}

void cc_destroy(CompilerCtx *cc) {
    vec_free(&cc->include_paths);
    vec_free(&cc->defines);
    intern_free(&cc->strings);
    arena_destroy(&cc->tmp_arena);
    arena_destroy(&cc->arena);
}

static int parse_args(CompilerCtx *cc, int argc, char **argv, const char *argv0) {
    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];

        if (arg[0] != '-') {
            if (cc->input_file) {
                fprintf(stderr, "error: multiple input files\n");
                return 1;
            }
            cc->input_file = arg;
            continue;
        }

        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            usage(argv[0]);
            exit(0);
        } else if (strcmp(arg, "-E") == 0) {
            cc->preprocess_only = true;
        } else if (strcmp(arg, "-S") == 0) {
            cc->compile_only = true;
        } else if (strcmp(arg, "-c") == 0) {
            cc->assemble_only = true;
        } else if (strcmp(arg, "-o") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "error: -o requires an argument\n");
                return 1;
            }
            cc->output_file = argv[++i];
        } else if (strncmp(arg, "-O", 2) == 0) {
            int level = arg[2] - '0';
            if (level < 0 || level > 3) {
                fprintf(stderr, "error: invalid optimization level: %s\n", arg);
                return 1;
            }
            cc->opt_level = (OptLevel)level;
        } else if (strncmp(arg, "-I", 2) == 0) {
            const char *path = arg[2] ? arg + 2 : argv[++i];
            vec_push(&cc->include_paths, path);
        } else if (strncmp(arg, "-D", 2) == 0) {
            const char *def = arg[2] ? arg + 2 : argv[++i];
            vec_push(&cc->defines, def);
        } else if (strcmp(arg, "-Wall") == 0) {
            cc->wall = true;
        } else if (strcmp(arg, "-Werror") == 0) {
            cc->werror = true;
            cc->diag.warnings_as_errors = true;
        } else if (strcmp(arg, "-target") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "error: -target requires an argument\n");
                return 1;
            }
            i++; /* target string parsed later */
        } else if (strcmp(arg, "--dump-ast") == 0) {
            cc->dump_ast = true;
        } else if (strcmp(arg, "--dump-ir") == 0) {
            cc->dump_ir = true;
        } else if (strcmp(arg, "-v") == 0) {
            /* verbose mode - currently ignored */
        } else {
            fprintf(stderr, "error: unknown option: %s\n", arg);
            return 1;
        }
    }

    if (!cc->input_file) {
        usage(argv0);
        return 1;
    }

    return 0;
}

static char *get_output_name(CompilerCtx *cc) {
    if (cc->output_file) return strdup(cc->output_file);

    /* Derive from input filename */
    const char *base = strrchr(cc->input_file, '/');
    base = base ? base + 1 : cc->input_file;

    size_t len = strlen(base);
    const char *dot = strrchr(base, '.');
    if (dot) len = dot - base;

    char *out;
    if (cc->compile_only) {
        out = malloc(len + 3);
        memcpy(out, base, len);
        strcpy(out + len, ".s");
    } else if (cc->assemble_only) {
        out = malloc(len + 3);
        memcpy(out, base, len);
        strcpy(out + len, ".o");
    } else {
        out = strdup("a.out");
    }
    return out;
}

int main(int argc, char **argv) {
    CompilerCtx cc;
    cc_init(&cc);

    if (parse_args(&cc, argc, argv, argv[0]) != 0) {
        cc_destroy(&cc);
        return 1;
    }

    /* Set up target */
    cc.target = target_host(&cc.arena);

    /* Read input file */
    char *source = read_file(cc.input_file);
    if (!source) {
        cc_destroy(&cc);
        return 1;
    }

    /* Phase 1: Lex */
    TokenList *tokens = cc_lex(&cc, cc.input_file);
    if (cc.diag.errors > 0) goto fail;

    /* Phase 2: Preprocess */
    tokens = cc_preprocess(&cc, tokens);
    if (cc.diag.errors > 0) goto fail;

    if (cc.preprocess_only) {
        /* -E: print preprocessed tokens */
        for (int i = 0; i < tokens->count; i++) {
            Token *t = &tokens->tokens[i];
            if (t->kind == TOK_EOF) break;
            if (t->has_space) printf(" ");
            if (t->at_bol) printf("\n");
            printf("%.*s", t->text_len, t->text);
        }
        printf("\n");
        free(source);
        cc_destroy(&cc);
        return 0;
    }

    /* Phase 3: Parse */
    Node *program = cc_parse(&cc, tokens);
    if (cc.diag.errors > 0) goto fail;

    /* Phase 4: Semantic Analysis */
    cc_sema(&cc, program);
    if (cc.diag.errors > 0) goto fail;

    if (cc.dump_ast) {
        ast_dump(program, 0);
        free(source);
        cc_destroy(&cc);
        return 0;
    }

    /* Phase 5: IR generation */
    IRModule *mod = cc_irgen(&cc, program);
    if (cc.diag.errors > 0) goto fail;

    /* Phase 6: Optimize */
    cc_optimize(&cc, mod);

    if (cc.dump_ir) {
        ir_print_module(mod, stdout);
        free(source);
        cc_destroy(&cc);
        return 0;
    }

    /* Phase 7: Code generation */
    char *outname = get_output_name(&cc);

    if (cc.compile_only) {
        /* -S: emit assembly */
        FILE *out = fopen(outname, "w");
        if (!out) {
            fprintf(stderr, "error: cannot open '%s': %s\n", outname, strerror(errno));
            free(outname);
            goto fail;
        }
        cc_codegen(&cc, mod, out);
        fclose(out);
    } else {
        /* Emit assembly to temp file, then assemble/link */
        char tmpasm[] = "/tmp/mycc_XXXXXX.s";
        int fd = mkstemps(tmpasm, 2);
        if (fd < 0) {
            fprintf(stderr, "error: cannot create temp file\n");
            free(outname);
            goto fail;
        }
        FILE *out = fdopen(fd, "w");
        cc_codegen(&cc, mod, out);
        fclose(out);

        if (cc.assemble_only) {
            /* -c: assemble to object */
            char cmd[4096];
            snprintf(cmd, sizeof(cmd), "cc -c -o '%s' '%s'", outname, tmpasm);
            if (system(cmd) != 0) {
                fprintf(stderr, "error: assembler failed\n");
                unlink(tmpasm);
                free(outname);
                goto fail;
            }
        } else {
            /* Full compile: assemble + link */
            char cmd[4096];
            snprintf(cmd, sizeof(cmd), "cc -o '%s' '%s'", outname, tmpasm);
            if (system(cmd) != 0) {
                fprintf(stderr, "error: linker failed\n");
                unlink(tmpasm);
                free(outname);
                goto fail;
            }
        }
        unlink(tmpasm);
    }

    free(outname);
    free(source);
    cc_destroy(&cc);
    return 0;

fail:
    free(source);
    cc_destroy(&cc);
    return 1;
}
