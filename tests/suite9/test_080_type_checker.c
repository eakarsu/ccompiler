int printf(const char *fmt, ...);

enum TypeKind {
    TY_INT,
    TY_CHAR,
    TY_PTR,
    TY_ARRAY,
    TY_FUNC,
    TY_VOID,
    TY_ERROR
};

struct Type {
    int kind;
    int base;         /* index into type pool for ptr/array/func-return */
    int array_size;
    int param_count;
    int params[4];    /* param type indices for functions */
};

struct TypePool {
    struct Type types[32];
    int count;
};

int make_type(struct TypePool *p, int kind) {
    int idx = p->count;
    p->types[idx].kind = kind;
    p->types[idx].base = -1;
    p->types[idx].array_size = 0;
    p->types[idx].param_count = 0;
    int i;
    for (i = 0; i < 4; i++) p->types[idx].params[i] = -1;
    p->count = p->count + 1;
    return idx;
}

int make_ptr(struct TypePool *p, int base) {
    int idx = make_type(p, TY_PTR);
    p->types[idx].base = base;
    return idx;
}

int make_array(struct TypePool *p, int base, int size) {
    int idx = make_type(p, TY_ARRAY);
    p->types[idx].base = base;
    p->types[idx].array_size = size;
    return idx;
}

int make_func(struct TypePool *p, int ret_type, int param_count, int *param_types) {
    int idx = make_type(p, TY_FUNC);
    p->types[idx].base = ret_type;
    p->types[idx].param_count = param_count;
    int i;
    for (i = 0; i < param_count && i < 4; i++) {
        p->types[idx].params[i] = param_types[i];
    }
    return idx;
}

char *type_kind_str(int kind) {
    if (kind == TY_INT) return "int";
    if (kind == TY_CHAR) return "char";
    if (kind == TY_PTR) return "ptr";
    if (kind == TY_ARRAY) return "array";
    if (kind == TY_FUNC) return "func";
    if (kind == TY_VOID) return "void";
    if (kind == TY_ERROR) return "error";
    return "unknown";
}

/* Check if two types are compatible */
int types_compatible(struct TypePool *p, int a, int b) {
    if (a == b) return 1;
    if (a < 0 || b < 0) return 0;

    struct Type *ta = &p->types[a];
    struct Type *tb = &p->types[b];

    if (ta->kind == TY_ERROR || tb->kind == TY_ERROR) return 0;

    /* int and char are compatible (implicit conversion) */
    if ((ta->kind == TY_INT && tb->kind == TY_CHAR) ||
        (ta->kind == TY_CHAR && tb->kind == TY_INT)) {
        return 1;
    }

    if (ta->kind != tb->kind) return 0;

    /* Both pointers: check base types */
    if (ta->kind == TY_PTR) {
        return types_compatible(p, ta->base, tb->base);
    }

    /* Both arrays: check base and size */
    if (ta->kind == TY_ARRAY) {
        if (ta->array_size != tb->array_size) return 0;
        return types_compatible(p, ta->base, tb->base);
    }

    return 1;
}

/* Type-check binary addition */
int check_add(struct TypePool *p, int left, int right) {
    struct Type *lt = &p->types[left];
    struct Type *rt = &p->types[right];

    /* int + int => int */
    if ((lt->kind == TY_INT || lt->kind == TY_CHAR) &&
        (rt->kind == TY_INT || rt->kind == TY_CHAR)) {
        return left;
    }

    /* ptr + int => ptr */
    if (lt->kind == TY_PTR && (rt->kind == TY_INT || rt->kind == TY_CHAR)) {
        return left;
    }

    /* int + ptr => ptr */
    if ((lt->kind == TY_INT || lt->kind == TY_CHAR) && rt->kind == TY_PTR) {
        return right;
    }

    /* array + int => pointer to element (array decays) */
    if (lt->kind == TY_ARRAY && (rt->kind == TY_INT || rt->kind == TY_CHAR)) {
        return make_ptr(p, lt->base);
    }

    return make_type(p, TY_ERROR);
}

/* Type-check function call */
int check_call(struct TypePool *p, int func_type, int argc, int *arg_types) {
    struct Type *ft = &p->types[func_type];

    if (ft->kind != TY_FUNC) {
        printf("  Error: not a function type\n");
        return make_type(p, TY_ERROR);
    }

    if (argc != ft->param_count) {
        printf("  Error: expected %d args, got %d\n", ft->param_count, argc);
        return make_type(p, TY_ERROR);
    }

    int i;
    for (i = 0; i < argc; i++) {
        if (!types_compatible(p, arg_types[i], ft->params[i])) {
            printf("  Error: arg %d type mismatch\n", i);
            return make_type(p, TY_ERROR);
        }
    }

    return ft->base;  /* return type */
}

/* Check dereference */
int check_deref(struct TypePool *p, int operand_type) {
    struct Type *t = &p->types[operand_type];
    if (t->kind == TY_PTR) {
        return t->base;
    }
    if (t->kind == TY_ARRAY) {
        return t->base;
    }
    return make_type(p, TY_ERROR);
}

/* Check address-of */
int check_addr_of(struct TypePool *p, int operand_type) {
    return make_ptr(p, operand_type);
}

int main() {
    struct TypePool pool;
    pool.count = 0;

    /* Create basic types */
    int ty_int = make_type(&pool, TY_INT);
    int ty_char = make_type(&pool, TY_CHAR);
    int ty_void = make_type(&pool, TY_VOID);

    // EXPECT: Basic types: int=0 char=1 void=2
    printf("Basic types: int=%d char=%d void=%d\n", ty_int, ty_char, ty_void);

    /* Test 1: int + int */
    int result = check_add(&pool, ty_int, ty_int);
    // EXPECT: Test 1: int+int => int
    printf("Test 1: int+int => %s\n", type_kind_str(pool.types[result].kind));

    /* Test 2: int + char => int */
    result = check_add(&pool, ty_int, ty_char);
    // EXPECT: Test 2: int+char => int
    printf("Test 2: int+char => %s\n", type_kind_str(pool.types[result].kind));

    /* Test 3: ptr(int) + int => ptr(int) */
    int ty_ptr_int = make_ptr(&pool, ty_int);
    result = check_add(&pool, ty_ptr_int, ty_int);
    // EXPECT: Test 3: ptr(int)+int => ptr
    printf("Test 3: ptr(int)+int => %s\n", type_kind_str(pool.types[result].kind));

    /* Test 4: Deref ptr(int) => int */
    result = check_deref(&pool, ty_ptr_int);
    // EXPECT: Test 4: *ptr(int) => int
    printf("Test 4: *ptr(int) => %s\n", type_kind_str(pool.types[result].kind));

    /* Test 5: Deref int => error */
    result = check_deref(&pool, ty_int);
    // EXPECT: Test 5: *int => error
    printf("Test 5: *int => %s\n", type_kind_str(pool.types[result].kind));

    /* Test 6: addr-of int => ptr(int) */
    result = check_addr_of(&pool, ty_int);
    // EXPECT: Test 6: &int => ptr
    printf("Test 6: &int => %s\n", type_kind_str(pool.types[result].kind));

    /* Test 7: Type compatibility */
    int compat;
    compat = types_compatible(&pool, ty_int, ty_int);
    // EXPECT: Test 7a: int~int => 1
    printf("Test 7a: int~int => %d\n", compat);
    compat = types_compatible(&pool, ty_int, ty_char);
    // EXPECT: Test 7b: int~char => 1
    printf("Test 7b: int~char => %d\n", compat);
    int ty_ptr_char = make_ptr(&pool, ty_char);
    compat = types_compatible(&pool, ty_ptr_int, ty_ptr_char);
    // EXPECT: Test 7c: ptr(int)~ptr(char) => 1
    printf("Test 7c: ptr(int)~ptr(char) => %d\n", compat);

    /* Test 8: Function type checking */
    int params[2];
    params[0] = ty_int;
    params[1] = ty_int;
    int ty_func = make_func(&pool, ty_int, 2, params);
    // EXPECT: Test 8: func returns int, params=2
    printf("Test 8: func returns %s, params=%d\n",
        type_kind_str(pool.types[pool.types[ty_func].base].kind),
        pool.types[ty_func].param_count);

    /* Test 9: Correct call */
    int args[2];
    args[0] = ty_int;
    args[1] = ty_int;
    result = check_call(&pool, ty_func, 2, args);
    // EXPECT: Test 9: call(int,int) => int
    printf("Test 9: call(int,int) => %s\n", type_kind_str(pool.types[result].kind));

    /* Test 10: Wrong arg count */
    args[0] = ty_int;
    // EXPECT:   Error: expected 2 args, got 1
    result = check_call(&pool, ty_func, 1, args);
    // EXPECT: Test 10: call(int) => error
    printf("Test 10: call(int) => %s\n", type_kind_str(pool.types[result].kind));

    /* Test 11: Array type */
    int ty_arr = make_array(&pool, ty_int, 10);
    // EXPECT: Test 11: array of int, size=10
    printf("Test 11: array of %s, size=%d\n",
        type_kind_str(pool.types[pool.types[ty_arr].base].kind),
        pool.types[ty_arr].array_size);

    /* Test 12: Array + int => ptr */
    result = check_add(&pool, ty_arr, ty_int);
    // EXPECT: Test 12: array+int => ptr
    printf("Test 12: array+int => %s\n", type_kind_str(pool.types[result].kind));

    /* Test 13: Deref array => element type */
    result = check_deref(&pool, ty_arr);
    // EXPECT: Test 13: *array => int
    printf("Test 13: *array => %s\n", type_kind_str(pool.types[result].kind));

    // EXPECT: All type checker tests done
    printf("All type checker tests done\n");
    return 0;
}
