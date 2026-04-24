#ifndef OPT_H
#define OPT_H

#include <stdbool.h>

typedef struct IRFunc IRFunc;
typedef struct IRModule IRModule;
typedef struct Arena Arena;

typedef enum {
    OPT_O0 = 0,
    OPT_O1 = 1,
    OPT_O2 = 2,
    OPT_O3 = 3,
} OptLevel;

/* Individual optimization passes */
bool opt_mem2reg(Arena *a, IRFunc *func);
bool opt_dce(IRFunc *func);
bool opt_sccp(Arena *a, IRFunc *func);
bool opt_gvn(Arena *a, IRFunc *func);
bool opt_inline(Arena *a, IRModule *mod);
bool opt_licm(Arena *a, IRFunc *func);
bool opt_simplifycfg(IRFunc *func);
bool opt_instcombine(Arena *a, IRFunc *func);

/* Pass manager */
void opt_run_pipeline(Arena *a, IRModule *mod, OptLevel level);

#endif /* OPT_H */
