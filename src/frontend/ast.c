#include "cc.h"

Node *new_node(Arena *a, NodeKind kind, SrcLoc loc) {
    Node *n = arena_new(a, Node);
    n->kind = kind;
    n->loc = loc;
    return n;
}

Node *new_binary(Arena *a, NodeKind kind, Node *lhs, Node *rhs, SrcLoc loc) {
    Node *n = new_node(a, kind, loc);
    n->lhs = lhs;
    n->rhs = rhs;
    return n;
}

Node *new_unary(Arena *a, NodeKind kind, Node *expr, SrcLoc loc) {
    Node *n = new_node(a, kind, loc);
    n->lhs = expr;
    return n;
}

Node *new_num(Arena *a, long long val, SrcLoc loc) {
    Node *n = new_node(a, ND_NUM, loc);
    n->ival = val;
    n->ty = ty_int;
    return n;
}

Node *new_var(Arena *a, Obj *var, SrcLoc loc) {
    Node *n = new_node(a, ND_VAR, loc);
    n->var = var;
    n->ty = var->ty;
    return n;
}

Node *new_cast(Arena *a, Node *expr, Type *ty) {
    Node *n = new_node(a, ND_CAST, expr->loc);
    n->lhs = expr;
    n->ty = ty;
    return n;
}

static const char *node_kind_str(NodeKind kind) {
    switch (kind) {
    case ND_NUM: return "NUM";
    case ND_FNUM: return "FNUM";
    case ND_VAR: return "VAR";
    case ND_STRING: return "STRING";
    case ND_MEMBER: return "MEMBER";
    case ND_DEREF: return "DEREF";
    case ND_ADDR: return "ADDR";
    case ND_NEG: return "NEG";
    case ND_NOT: return "NOT";
    case ND_BITNOT: return "BITNOT";
    case ND_ADD: return "ADD";
    case ND_SUB: return "SUB";
    case ND_MUL: return "MUL";
    case ND_DIV: return "DIV";
    case ND_MOD: return "MOD";
    case ND_BITAND: return "BITAND";
    case ND_BITOR: return "BITOR";
    case ND_BITXOR: return "BITXOR";
    case ND_SHL: return "SHL";
    case ND_SHR: return "SHR";
    case ND_EQ: return "EQ";
    case ND_NE: return "NE";
    case ND_LT: return "LT";
    case ND_LE: return "LE";
    case ND_LOGAND: return "LOGAND";
    case ND_LOGOR: return "LOGOR";
    case ND_ASSIGN: return "ASSIGN";
    case ND_TERNARY: return "TERNARY";
    case ND_COMMA: return "COMMA";
    case ND_CAST: return "CAST";
    case ND_SIZEOF: return "SIZEOF";
    case ND_ALIGNOF: return "ALIGNOF";
    case ND_CALL: return "CALL";
    case ND_PRE_INC: return "PRE_INC";
    case ND_PRE_DEC: return "PRE_DEC";
    case ND_POST_INC: return "POST_INC";
    case ND_POST_DEC: return "POST_DEC";
    case ND_COMPOUND_ASSIGN: return "COMPOUND_ASSIGN";
    case ND_STMT_EXPR: return "STMT_EXPR";
    case ND_INIT_LIST: return "INIT_LIST";
    case ND_BLOCK: return "BLOCK";
    case ND_IF: return "IF";
    case ND_FOR: return "FOR";
    case ND_SWITCH: return "SWITCH";
    case ND_CASE: return "CASE";
    case ND_DEFAULT: return "DEFAULT";
    case ND_RETURN: return "RETURN";
    case ND_BREAK: return "BREAK";
    case ND_CONTINUE: return "CONTINUE";
    case ND_GOTO: return "GOTO";
    case ND_LABEL: return "LABEL";
    case ND_EXPR_STMT: return "EXPR_STMT";
    case ND_NULL_STMT: return "NULL_STMT";
    case ND_FUNCDEF: return "FUNCDEF";
    case ND_DECLARATION: return "DECLARATION";
    case ND_TYPEDEF: return "TYPEDEF";
    case ND_PROGRAM: return "PROGRAM";
    }
    return "???";
}

static void print_indent(int indent) {
    for (int i = 0; i < indent; i++) fprintf(stderr, "  ");
}

void ast_dump(Node *node, int indent) {
    if (!node) return;

    print_indent(indent);
    fprintf(stderr, "%s", node_kind_str(node->kind));

    switch (node->kind) {
    case ND_NUM:
        fprintf(stderr, " %lld", node->ival);
        break;
    case ND_FNUM:
        fprintf(stderr, " %g", node->fval);
        break;
    case ND_VAR:
        if (node->var) fprintf(stderr, " '%s'", node->var->name);
        break;
    case ND_STRING:
        if (node->str_val) fprintf(stderr, " \"%s\"", node->str_val);
        break;
    case ND_FUNCDEF:
        if (node->var) fprintf(stderr, " '%s'", node->var->name);
        break;
    case ND_DECLARATION:
        if (node->var) fprintf(stderr, " '%s'", node->var->name);
        break;
    case ND_CALL:
        if (node->func_name) fprintf(stderr, " '%s'", node->func_name);
        break;
    case ND_GOTO:
    case ND_LABEL:
        if (node->label) fprintf(stderr, " '%s'", node->label);
        break;
    case ND_CASE:
        fprintf(stderr, " %lld", node->case_val);
        break;
    default:
        break;
    }
    fprintf(stderr, "\n");

    if (node->lhs) ast_dump(node->lhs, indent + 1);
    if (node->rhs) ast_dump(node->rhs, indent + 1);
    if (node->cond) ast_dump(node->cond, indent + 1);
    if (node->then) ast_dump(node->then, indent + 1);
    if (node->els) ast_dump(node->els, indent + 1);
    if (node->init) ast_dump(node->init, indent + 1);
    if (node->inc) ast_dump(node->inc, indent + 1);

    for (Node *arg = node->args; arg; arg = arg->next)
        ast_dump(arg, indent + 1);

    for (Node *s = node->body; s; s = s->next)
        ast_dump(s, indent + 1);
}
