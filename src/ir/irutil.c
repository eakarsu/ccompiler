/*
 * irutil.c - CFG and dominator utilities for mycc
 *
 * Implements:
 * - CFG construction (predecessor/successor edges)
 * - Dominator tree computation (iterative algorithm)
 * - Dominance frontier computation
 * - Natural loop detection and loop depth computation
 */

#include "cc.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* ================================================================
 * CFG Construction
 * ================================================================
 *
 * Walk all blocks, inspect their terminators, and fill in
 * predecessor/successor edge lists.
 */

static void add_edge(IRBlock *from, IRBlock *to)
{
    /* Add to successor list of 'from' (avoid duplicates) */
    for (int i = 0; i < from->succs.len; i++) {
        if (from->succs.data[i] == to)
            goto add_pred;
    }
    vec_push(&from->succs, to);

add_pred:
    /* Add to predecessor list of 'to' (avoid duplicates) */
    for (int i = 0; i < to->preds.len; i++) {
        if (to->preds.data[i] == from)
            return;
    }
    vec_push(&to->preds, from);
}

void ir_compute_cfg(IRFunc *func)
{
    if (!func) return;

    /* Clear existing edges */
    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        vec_clear(&bb->preds);
        vec_clear(&bb->succs);
    }

    /* Walk all blocks and add edges based on terminators */
    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        IRInst *term = bb->last;
        if (!term) continue;

        switch (term->op) {
        case IR_BR:
            if (term->br_target)
                add_edge(bb, term->br_target);
            break;

        case IR_CONDBR:
            if (term->br_true)
                add_edge(bb, term->br_true);
            if (term->br_false)
                add_edge(bb, term->br_false);
            break;

        case IR_RET:
        case IR_RETV:
            /* No successors */
            break;

        default:
            /* Non-terminator at end of block - should not happen in valid IR */
            break;
        }
    }
}

/* ================================================================
 * Dominator Computation
 * ================================================================
 *
 * Uses the iterative algorithm from Cooper, Harvey, Kennedy:
 * "A Simple, Fast Dominance Algorithm" (2001).
 *
 * This is simple and efficient for the typical CFG sizes we see
 * in a C compiler. The algorithm iterates to a fixed point by
 * computing:
 *     idom(b) = intersection of idom of all predecessors of b
 *
 * Where intersection uses the intersect function on the
 * reverse postorder numbering.
 */

/* Reverse postorder numbering via DFS */
typedef struct {
    IRBlock **rpo;     /* blocks in reverse postorder */
    int *rpo_num;      /* block id -> RPO number */
    int count;
    int max_id;
} RPOCtx;

static void rpo_dfs(IRBlock *bb, bool *visited, IRBlock **postorder, int *idx)
{
    if (!bb || visited[bb->id]) return;
    visited[bb->id] = true;

    for (int i = 0; i < bb->succs.len; i++)
        rpo_dfs(bb->succs.data[i], visited, postorder, idx);

    postorder[(*idx)++] = bb;
}

static void compute_rpo(IRFunc *func, RPOCtx *rpo)
{
    int n = func->block_count;
    int max_id = 0;
    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        if (bb->id >= max_id) max_id = bb->id + 1;
    }
    rpo->max_id = max_id;

    bool *visited = calloc(max_id, sizeof(bool));
    IRBlock **postorder = calloc(n, sizeof(IRBlock *));
    int idx = 0;

    rpo_dfs(func->entry, visited, postorder, &idx);

    /* Reverse the postorder to get RPO */
    rpo->rpo = calloc(idx, sizeof(IRBlock *));
    rpo->count = idx;
    rpo->rpo_num = calloc(max_id, sizeof(int));

    /* Initialize all rpo_num to -1 (unreachable) */
    for (int i = 0; i < max_id; i++)
        rpo->rpo_num[i] = -1;

    for (int i = 0; i < idx; i++) {
        rpo->rpo[i] = postorder[idx - 1 - i];
        rpo->rpo_num[rpo->rpo[i]->id] = i;
    }

    free(visited);
    free(postorder);
}

static void free_rpo(RPOCtx *rpo)
{
    free(rpo->rpo);
    free(rpo->rpo_num);
}

/* Intersect two dominators using RPO numbers (Cooper et al.) */
static IRBlock *intersect(IRBlock **idoms, int *rpo_num, IRBlock *b1, IRBlock *b2)
{
    IRBlock *finger1 = b1;
    IRBlock *finger2 = b2;

    while (finger1 != finger2) {
        while (rpo_num[finger1->id] > rpo_num[finger2->id])
            finger1 = idoms[finger1->id];
        while (rpo_num[finger2->id] > rpo_num[finger1->id])
            finger2 = idoms[finger2->id];
    }
    return finger1;
}

void ir_compute_dominators(IRFunc *func)
{
    if (!func || !func->entry) return;

    /* First compute CFG if not already done */
    ir_compute_cfg(func);

    RPOCtx rpo;
    compute_rpo(func, &rpo);

    if (rpo.count == 0) {
        free_rpo(&rpo);
        return;
    }

    /* Allocate idom array indexed by block id */
    IRBlock **idoms = calloc(rpo.max_id, sizeof(IRBlock *));

    /* Initialize: entry's idom is itself */
    idoms[func->entry->id] = func->entry;

    /* Iterate until convergence */
    bool changed = true;
    while (changed) {
        changed = false;

        /* Process all blocks in RPO except the entry */
        for (int i = 0; i < rpo.count; i++) {
            IRBlock *bb = rpo.rpo[i];
            if (bb == func->entry) continue;

            /* Find the first processed predecessor */
            IRBlock *new_idom = NULL;
            for (int p = 0; p < bb->preds.len; p++) {
                IRBlock *pred = bb->preds.data[p];
                if (idoms[pred->id] != NULL) {
                    new_idom = pred;
                    break;
                }
            }

            if (!new_idom) continue; /* unreachable block */

            /* Intersect with remaining processed predecessors */
            for (int p = 0; p < bb->preds.len; p++) {
                IRBlock *pred = bb->preds.data[p];
                if (pred == new_idom) continue;
                if (idoms[pred->id] != NULL) {
                    new_idom = intersect(idoms, rpo.rpo_num, pred, new_idom);
                }
            }

            if (idoms[bb->id] != new_idom) {
                idoms[bb->id] = new_idom;
                changed = true;
            }
        }
    }

    /* Store results in the block structures and build dominator tree children */
    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        bb->idom = idoms[bb->id];
        vec_clear(&bb->dom_children);
    }

    /* Entry has no idom (it dominates itself) */
    func->entry->idom = NULL;

    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        if (bb->idom && bb->idom != bb) {
            vec_push(&bb->idom->dom_children, bb);
        }
    }

    /* Compute DFS order in dominator tree for quick dominance checks */
    int order = 0;
    /* Iterative DFS using a stack */
    typedef struct { IRBlock *bb; int child_idx; } DFSFrame;
    int stack_cap = func->block_count + 1;
    DFSFrame *stack = calloc(stack_cap, sizeof(DFSFrame));
    int sp = 0;

    func->entry->dom_order = order++;
    stack[sp++] = (DFSFrame){ func->entry, 0 };

    while (sp > 0) {
        DFSFrame *top = &stack[sp - 1];
        if (top->child_idx < top->bb->dom_children.len) {
            IRBlock *child = top->bb->dom_children.data[top->child_idx++];
            child->dom_order = order++;
            if (sp < stack_cap)
                stack[sp++] = (DFSFrame){ child, 0 };
        } else {
            sp--;
        }
    }

    free(stack);
    free(idoms);
    free_rpo(&rpo);
}

/* ================================================================
 * Dominance Frontier Computation
 * ================================================================
 *
 * Uses the algorithm from Cytron et al.:
 *   For each block B:
 *     For each predecessor P of B:
 *       runner = P
 *       while runner != idom(B):
 *         DF(runner) += B
 *         runner = idom(runner)
 */

void ir_compute_dom_frontiers(IRFunc *func)
{
    if (!func || !func->entry) return;

    /* Clear existing frontiers */
    for (IRBlock *bb = func->blocks; bb; bb = bb->next)
        vec_clear(&bb->dom_frontier);

    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        /* Only consider blocks with multiple predecessors or
         * blocks where a predecessor is not the immediate dominator.
         * Actually, the standard algorithm works on all join points. */
        if (bb->preds.len < 2) continue;

        for (int p = 0; p < bb->preds.len; p++) {
            IRBlock *runner = bb->preds.data[p];

            while (runner && runner != bb->idom) {
                /* Add bb to DF(runner) if not already there */
                bool found = false;
                for (int i = 0; i < runner->dom_frontier.len; i++) {
                    if (runner->dom_frontier.data[i] == bb) {
                        found = true;
                        break;
                    }
                }
                if (!found)
                    vec_push(&runner->dom_frontier, bb);

                runner = runner->idom;
            }
        }
    }
}

/* ================================================================
 * Loop Detection and Loop Depth Computation
 * ================================================================
 *
 * Identify natural loops by finding back edges (edges from a block
 * to one of its dominators). For each back edge B -> H:
 *   - H is the loop header
 *   - The natural loop body is the set of blocks that can reach B
 *     without going through H
 *
 * Loop depth is the nesting level (how many loops contain this block).
 */

/* Check if 'a' dominates 'b' using the dominator tree.
 * Walk up from 'b' to the entry, checking if 'a' is encountered. */
bool ir_dominates(IRBlock *a, IRBlock *b)
{
    if (!a || !b) return false;
    if (a == b) return true;

    /* Walk up the dominator tree from b */
    IRBlock *runner = b->idom;
    while (runner && runner != a) {
        if (runner->idom == runner) break; /* reached entry without finding a */
        runner = runner->idom;
    }
    return runner == a;
}

/* Collect the natural loop body for back edge tail -> header.
 * Uses reverse reachability from tail, stopping at header. */
static void collect_loop_body(IRBlock *header, IRBlock *tail,
                              IRBlock **body, int *body_count,
                              int max_blocks, bool *in_loop)
{
    if (in_loop[header->id]) {
        /* header already in loop */
    } else {
        in_loop[header->id] = true;
        body[(*body_count)++] = header;
    }

    if (header == tail) return;

    /* BFS/DFS backward from tail to header */
    /* Use a simple stack-based approach */
    int stack_cap = max_blocks;
    IRBlock **stack = calloc(stack_cap, sizeof(IRBlock *));
    int sp = 0;

    if (!in_loop[tail->id]) {
        in_loop[tail->id] = true;
        body[(*body_count)++] = tail;
        stack[sp++] = tail;
    }

    while (sp > 0) {
        IRBlock *bb = stack[--sp];
        for (int i = 0; i < bb->preds.len; i++) {
            IRBlock *pred = bb->preds.data[i];
            if (!in_loop[pred->id]) {
                in_loop[pred->id] = true;
                if (*body_count < max_blocks)
                    body[(*body_count)++] = pred;
                if (sp < stack_cap)
                    stack[sp++] = pred;
            }
        }
    }

    free(stack);
}

void ir_compute_loop_info(IRFunc *func)
{
    if (!func || !func->entry) return;

    /* Reset loop depth */
    for (IRBlock *bb = func->blocks; bb; bb = bb->next)
        bb->loop_depth = 0;

    /* Find maximum block id */
    int max_id = 0;
    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        if (bb->id >= max_id) max_id = bb->id + 1;
    }

    /* Find all back edges: an edge B -> H is a back edge if H dominates B */
    typedef struct { IRBlock *header; IRBlock *tail; } BackEdge;
    int num_back_edges = 0;
    int back_edge_cap = 16;
    BackEdge *back_edges = calloc(back_edge_cap, sizeof(BackEdge));

    for (IRBlock *bb = func->blocks; bb; bb = bb->next) {
        for (int i = 0; i < bb->succs.len; i++) {
            IRBlock *succ = bb->succs.data[i];
            if (ir_dominates(succ, bb)) {
                /* Back edge: bb -> succ */
                if (num_back_edges >= back_edge_cap) {
                    back_edge_cap *= 2;
                    back_edges = realloc(back_edges, back_edge_cap * sizeof(BackEdge));
                }
                back_edges[num_back_edges].header = succ;
                back_edges[num_back_edges].tail = bb;
                num_back_edges++;
            }
        }
    }

    /* For each back edge, collect the loop body and increment loop depth */
    IRBlock **body = calloc(func->block_count + max_id, sizeof(IRBlock *));
    bool *in_loop = calloc(max_id, sizeof(bool));

    for (int e = 0; e < num_back_edges; e++) {
        int body_count = 0;
        memset(in_loop, 0, max_id * sizeof(bool));

        collect_loop_body(back_edges[e].header, back_edges[e].tail,
                          body, &body_count, func->block_count + max_id, in_loop);

        /* Increment loop depth for all blocks in this loop */
        for (int i = 0; i < body_count; i++) {
            body[i]->loop_depth++;
        }
    }

    free(body);
    free(in_loop);
    free(back_edges);
}
