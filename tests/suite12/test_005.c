int printf(const char *fmt, ...);

// Type inference engine
// Infers types of expressions in a simple expression language

int TY_INT = 0;
int TY_BOOL = 1;
int TY_STR = 2;
int TY_ERR = 3;
int TY_VOID = 4;

int OP_ADD = 0;
int OP_SUB = 1;
int OP_MUL = 2;
int OP_DIV = 3;
int OP_EQ = 4;
int OP_LT = 5;
int OP_AND = 6;
int OP_OR = 7;
int OP_NOT = 8;
int OP_CONCAT = 9;

char *type_name(int ty) {
    if (ty == 0) return "int";
    if (ty == 1) return "bool";
    if (ty == 2) return "str";
    if (ty == 3) return "error";
    if (ty == 4) return "void";
    return "unknown";
}

int infer_binop(int op, int left_ty, int right_ty) {
    if (op == OP_ADD || op == OP_SUB || op == OP_MUL || op == OP_DIV) {
        if (left_ty == TY_INT && right_ty == TY_INT) return TY_INT;
        return TY_ERR;
    }
    if (op == OP_EQ) {
        if (left_ty == right_ty) return TY_BOOL;
        return TY_ERR;
    }
    if (op == OP_LT) {
        if (left_ty == TY_INT && right_ty == TY_INT) return TY_BOOL;
        return TY_ERR;
    }
    if (op == OP_AND || op == OP_OR) {
        if (left_ty == TY_BOOL && right_ty == TY_BOOL) return TY_BOOL;
        return TY_ERR;
    }
    if (op == OP_CONCAT) {
        if (left_ty == TY_STR && right_ty == TY_STR) return TY_STR;
        return TY_ERR;
    }
    return TY_ERR;
}

int infer_unary(int op, int operand_ty) {
    if (op == OP_NOT) {
        if (operand_ty == TY_BOOL) return TY_BOOL;
        return TY_ERR;
    }
    if (op == OP_SUB) {
        if (operand_ty == TY_INT) return TY_INT;
        return TY_ERR;
    }
    return TY_ERR;
}

// Type environment for variables
struct TypeBinding {
    char name[16];
    int type;
    int active;
};

struct TypeBinding env[32];
int env_count;

void init_env() {
    env_count = 0;
    int i = 0;
    while (i < 32) { env[i].active = 0; i = i + 1; }
}

int str_eq(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void bind_type(char *name, int ty) {
    int i = 0;
    while (i < env_count) {
        if (env[i].active && str_eq(env[i].name, name)) {
            env[i].type = ty;
            return;
        }
        i = i + 1;
    }
    int idx = env_count;
    int j = 0;
    while (name[j] != 0 && j < 15) {
        env[idx].name[j] = name[j];
        j = j + 1;
    }
    env[idx].name[j] = 0;
    env[idx].type = ty;
    env[idx].active = 1;
    env_count = env_count + 1;
}

int lookup_type(char *name) {
    int i = env_count - 1;
    while (i >= 0) {
        if (env[i].active && str_eq(env[i].name, name)) {
            return env[i].type;
        }
        i = i - 1;
    }
    return TY_ERR;
}

int can_assign(int target_ty, int value_ty) {
    if (target_ty == value_ty) return 1;
    return 0;
}

int is_numeric(int ty) {
    return ty == TY_INT;
}

int is_comparable(int ty) {
    return ty == TY_INT || ty == TY_BOOL || ty == TY_STR;
}

int check_if_cond(int cond_ty) {
    return cond_ty == TY_BOOL;
}

int infer_ternary(int cond_ty, int then_ty, int else_ty) {
    if (cond_ty != TY_BOOL) return TY_ERR;
    if (then_ty != else_ty) return TY_ERR;
    return then_ty;
}

int main() {
    init_env();

    int r;
    r = infer_binop(OP_ADD, TY_INT, TY_INT);
    printf("int + int = %s\n", type_name(r));
    // EXPECT: int + int = int

    r = infer_binop(OP_ADD, TY_INT, TY_BOOL);
    printf("int + bool = %s\n", type_name(r));
    // EXPECT: int + bool = error

    r = infer_binop(OP_MUL, TY_INT, TY_INT);
    printf("int * int = %s\n", type_name(r));
    // EXPECT: int * int = int

    r = infer_binop(OP_EQ, TY_INT, TY_INT);
    printf("int == int = %s\n", type_name(r));
    // EXPECT: int == int = bool

    r = infer_binop(OP_EQ, TY_STR, TY_STR);
    printf("str == str = %s\n", type_name(r));
    // EXPECT: str == str = bool

    r = infer_binop(OP_EQ, TY_INT, TY_BOOL);
    printf("int == bool = %s\n", type_name(r));
    // EXPECT: int == bool = error

    r = infer_binop(OP_LT, TY_INT, TY_INT);
    printf("int < int = %s\n", type_name(r));
    // EXPECT: int < int = bool

    r = infer_binop(OP_AND, TY_BOOL, TY_BOOL);
    printf("bool && bool = %s\n", type_name(r));
    // EXPECT: bool && bool = bool

    r = infer_binop(OP_OR, TY_INT, TY_INT);
    printf("int || int = %s\n", type_name(r));
    // EXPECT: int || int = error

    r = infer_binop(OP_CONCAT, TY_STR, TY_STR);
    printf("str ++ str = %s\n", type_name(r));
    // EXPECT: str ++ str = str

    r = infer_unary(OP_NOT, TY_BOOL);
    printf("!bool = %s\n", type_name(r));
    // EXPECT: !bool = bool

    r = infer_unary(OP_NOT, TY_INT);
    printf("!int = %s\n", type_name(r));
    // EXPECT: !int = error

    r = infer_unary(OP_SUB, TY_INT);
    printf("-int = %s\n", type_name(r));
    // EXPECT: -int = int

    bind_type("x", TY_INT);
    bind_type("flag", TY_BOOL);
    bind_type("msg", TY_STR);

    printf("x: %s\n", type_name(lookup_type("x")));
    // EXPECT: x: int
    printf("flag: %s\n", type_name(lookup_type("flag")));
    // EXPECT: flag: bool
    printf("msg: %s\n", type_name(lookup_type("msg")));
    // EXPECT: msg: str
    printf("undef: %s\n", type_name(lookup_type("undef")));
    // EXPECT: undef: error

    printf("assign int=int: %d\n", can_assign(TY_INT, TY_INT));
    // EXPECT: assign int=int: 1
    printf("assign int=bool: %d\n", can_assign(TY_INT, TY_BOOL));
    // EXPECT: assign int=bool: 0

    r = infer_ternary(TY_BOOL, TY_INT, TY_INT);
    printf("bool?int:int = %s\n", type_name(r));
    // EXPECT: bool?int:int = int

    r = infer_ternary(TY_INT, TY_INT, TY_INT);
    printf("int?int:int = %s\n", type_name(r));
    // EXPECT: int?int:int = error

    r = infer_ternary(TY_BOOL, TY_INT, TY_STR);
    printf("bool?int:str = %s\n", type_name(r));
    // EXPECT: bool?int:str = error

    printf("type inference done\n");
    // EXPECT: type inference done

    return 0;
}
