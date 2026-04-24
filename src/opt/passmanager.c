#include "cc.h"

void opt_run_pipeline(Arena *a, IRModule *mod, OptLevel level) {
    if (level == OPT_O0) return;

    for (IRFunc *func = mod->funcs; func; func = func->next) {
        /* Compute CFG info needed by optimizations */
        ir_compute_cfg(func);
        ir_compute_dominators(func);
        ir_compute_dom_frontiers(func);

        /* -O1: basic optimizations */
        if (level >= OPT_O1) {
            opt_mem2reg(a, func);
            opt_dce(func);
            opt_simplifycfg(func);
            opt_instcombine(a, func);
        }

        /* -O2: more aggressive */
        if (level >= OPT_O2) {
            opt_sccp(a, func);
            opt_gvn(a, func);
            ir_compute_loop_info(func);
            opt_licm(a, func);
            opt_dce(func);
            opt_simplifycfg(func);
        }
    }

    /* Module-level: inlining (currently disabled - needs further work) */
    if (level >= OPT_O2) {
        /* opt_inline(a, mod); */
        (void)a; (void)mod;
    }

    /* -O3: most aggressive */
    if (level >= OPT_O3) {
        for (IRFunc *func = mod->funcs; func; func = func->next) {
            opt_instcombine(a, func);
            opt_gvn(a, func);
            opt_dce(func);
        }
    }
}

void cc_optimize(CompilerCtx *cc, IRModule *mod) {
    opt_run_pipeline(&cc->arena, mod, cc->opt_level);
}

void cc_codegen(CompilerCtx *cc, IRModule *mod, FILE *out) {
    codegen(mod, cc->target, out, &cc->arena);
}
