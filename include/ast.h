#ifndef AST_H
#define AST_H

#include "token.h"
#include "type.h"

typedef struct Node Node;
typedef struct Arena Arena;

typedef enum {
    /* Expressions */
    ND_NUM,           /* integer literal */
    ND_FNUM,          /* float literal */
    ND_VAR,           /* variable reference */
    ND_STRING,        /* string literal */
    ND_MEMBER,        /* struct.member or struct->member */
    ND_DEREF,         /* *ptr */
    ND_ADDR,          /* &val */
    ND_NEG,           /* -val */
    ND_NOT,           /* !val */
    ND_BITNOT,        /* ~val */
    ND_ADD,           /* + */
    ND_SUB,           /* - */
    ND_MUL,           /* * */
    ND_DIV,           /* / */
    ND_MOD,           /* % */
    ND_BITAND,        /* & */
    ND_BITOR,         /* | */
    ND_BITXOR,        /* ^ */
    ND_SHL,           /* << */
    ND_SHR,           /* >> */
    ND_EQ,            /* == */
    ND_NE,            /* != */
    ND_LT,            /* < */
    ND_LE,            /* <= */
    ND_LOGAND,        /* && */
    ND_LOGOR,         /* || */
    ND_ASSIGN,        /* = */
    ND_TERNARY,       /* ?: */
    ND_COMMA,         /* , */
    ND_CAST,          /* (type)expr */
    ND_SIZEOF,        /* sizeof */
    ND_ALIGNOF,       /* _Alignof */
    ND_CALL,          /* function call */
    ND_PRE_INC,       /* ++x */
    ND_PRE_DEC,       /* --x */
    ND_POST_INC,      /* x++ */
    ND_POST_DEC,      /* x-- */
    ND_COMPOUND_ASSIGN, /* +=, -=, etc */
    ND_STMT_EXPR,     /* ({ ... }) GNU extension */
    ND_INIT_LIST,     /* initializer list {1, 2, 3} */
    ND_GENERIC,       /* _Generic(expr, type: expr, ...) */

    /* Statements */
    ND_BLOCK,         /* { ... } */
    ND_IF,            /* if */
    ND_FOR,           /* for (also while/do-while) */
    ND_SWITCH,        /* switch */
    ND_CASE,          /* case */
    ND_DEFAULT,       /* default */
    ND_RETURN,        /* return */
    ND_BREAK,         /* break */
    ND_CONTINUE,      /* continue */
    ND_GOTO,          /* goto */
    ND_LABEL,         /* label: */
    ND_EXPR_STMT,     /* expression statement */
    ND_NULL_STMT,     /* ; */

    /* Declarations */
    ND_FUNCDEF,       /* function definition */
    ND_DECLARATION,   /* variable declaration */
    ND_TYPEDEF,       /* typedef */

    /* Top level */
    ND_PROGRAM,       /* translation unit */
} NodeKind;

/* Symbol / Variable */
typedef struct Obj {
    struct Obj *next;
    const char *name;
    Type *ty;
    SrcLoc loc;

    /* Local variable */
    int offset;         /* stack offset for locals */

    /* Global variable / function */
    bool is_global;
    bool is_static;
    bool is_extern;
    bool is_definition;
    bool is_tentative;

    /* String literal */
    const char *string_val;
    int string_len;

    /* Enum constant */
    bool is_enum_const;
    long long enum_val;

    /* For local variable init */
    Node *init_expr;

    /* Function body */
    Node *body;
    struct Obj *locals;
    int stack_size;

    /* IR generation (set during irgen phase) */
    void *ir_alloca;    /* IRInst* for the variable's alloca */
} Obj;

/* Designator chain for nested designated initializers (.a.b, [0].x, etc.) */
typedef struct Designator {
    const char *name;       /* field name (for .field), NULL for [index] */
    int index;              /* array index (for [N]), -1 if field designator */
    struct Designator *next;
} Designator;

struct Node {
    NodeKind kind;
    Type *ty;
    SrcLoc loc;

    /* Binary / unary operands */
    Node *lhs;
    Node *rhs;

    /* Block / program: list of statements */
    Node *body;
    Node *next;       /* next in linked list */

    /* Variable reference */
    Obj *var;

    /* Literals */
    long long ival;
    double fval;
    const char *str_val;
    int str_len;

    /* If / for / while */
    Node *cond;
    Node *then;
    Node *els;
    Node *init;
    Node *inc;

    /* Switch/case */
    long long case_val;
    Node *case_next;
    Node *default_case;

    /* Function call */
    const char *func_name;
    Node *args;

    /* Goto / label */
    const char *label;
    Node *label_next;

    /* Member access */
    Member *member;

    /* Compound assignment: the underlying binary op kind */
    NodeKind compound_op;

    /* Ternary */
    /* cond, then, els reused */

    /* Cast */
    /* lhs reused, ty is target type */

    /* Designated initializer info (single-level, backwards compat) */
    const char *designator_name;  /* ".field" designator (for struct/union) */
    int designator_index;         /* "[N]" designator (for arrays), -1 if not set */

    /* Chained designator list for nested designators like .o.x or .o.in.a */
    struct Designator *desig_chain;

    /* do-while flag */
    bool is_dowhile;
};

/* AST construction helpers */
Node *new_node(Arena *a, NodeKind kind, SrcLoc loc);
Node *new_binary(Arena *a, NodeKind kind, Node *lhs, Node *rhs, SrcLoc loc);
Node *new_unary(Arena *a, NodeKind kind, Node *expr, SrcLoc loc);
Node *new_num(Arena *a, long long val, SrcLoc loc);
Node *new_var(Arena *a, Obj *var, SrcLoc loc);
Node *new_cast(Arena *a, Node *expr, Type *ty);

/* AST printer */
void ast_dump(Node *node, int indent);

#endif /* AST_H */
