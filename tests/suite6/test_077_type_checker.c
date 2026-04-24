int printf(const char *fmt, ...);

int TY_INT = 0;
int TY_CHAR = 1;
int TY_PTR_INT = 2;
int TY_PTR_CHAR = 3;
int TY_VOID = 4;
int TY_PTR_VOID = 5;

char *type_name(int ty) {
    if (ty == 0) return "int";
    if (ty == 1) return "char";
    if (ty == 2) return "int*";
    if (ty == 3) return "char*";
    if (ty == 4) return "void";
    if (ty == 5) return "void*";
    return "unknown";
}

int type_size(int ty) {
    if (ty == TY_INT) return 4;
    if (ty == TY_CHAR) return 1;
    if (ty == TY_PTR_INT) return 8;
    if (ty == TY_PTR_CHAR) return 8;
    if (ty == TY_VOID) return 0;
    if (ty == TY_PTR_VOID) return 8;
    return 0;
}

int is_pointer(int ty) {
    return ty == TY_PTR_INT || ty == TY_PTR_CHAR || ty == TY_PTR_VOID;
}

int is_integer(int ty) {
    return ty == TY_INT || ty == TY_CHAR;
}

int can_assign(int dst, int src) {
    if (dst == src) return 1;
    if (dst == TY_INT && src == TY_CHAR) return 1;
    if (dst == TY_CHAR && src == TY_INT) return 1;
    if (is_pointer(dst) && src == TY_PTR_VOID) return 1;
    if (dst == TY_PTR_VOID && is_pointer(src)) return 1;
    return 0;
}

int can_add(int left, int right) {
    if (is_integer(left) && is_integer(right)) return 1;
    if (is_pointer(left) && is_integer(right)) return 1;
    if (is_integer(left) && is_pointer(right)) return 1;
    return 0;
}

int can_sub(int left, int right) {
    if (is_integer(left) && is_integer(right)) return 1;
    if (is_pointer(left) && is_integer(right)) return 1;
    if (is_pointer(left) && is_pointer(right) && left == right) return 1;
    return 0;
}

int can_compare(int left, int right) {
    if (is_integer(left) && is_integer(right)) return 1;
    if (is_pointer(left) && is_pointer(right)) return 1;
    return 0;
}

int result_type_add(int left, int right) {
    if (is_pointer(left)) return left;
    if (is_pointer(right)) return right;
    if (left == TY_INT || right == TY_INT) return TY_INT;
    return TY_CHAR;
}

int result_type_sub(int left, int right) {
    if (is_pointer(left) && is_pointer(right)) return TY_INT;
    if (is_pointer(left)) return left;
    if (left == TY_INT || right == TY_INT) return TY_INT;
    return TY_CHAR;
}

int deref_type(int ty) {
    if (ty == TY_PTR_INT) return TY_INT;
    if (ty == TY_PTR_CHAR) return TY_CHAR;
    return -1;
}

int addr_of_type(int ty) {
    if (ty == TY_INT) return TY_PTR_INT;
    if (ty == TY_CHAR) return TY_PTR_CHAR;
    return -1;
}

int var_types[26];
int var_count;

void init_vars(void) {
    var_count = 0;
    int i = 0;
    while (i < 26) { var_types[i] = -1; i = i + 1; }
}

int declare_var(int name_idx, int ty) {
    var_types[name_idx] = ty;
    var_count = var_count + 1;
    return 1;
}

int check_assign_var(int name_idx, int src_type) {
    if (var_types[name_idx] < 0) return -1;
    return can_assign(var_types[name_idx], src_type);
}

int main(void) {
    // EXPECT: int_size=4
    printf("int_size=%d\n", type_size(TY_INT));
    // EXPECT: char_size=1
    printf("char_size=%d\n", type_size(TY_CHAR));
    // EXPECT: ptr_size=8
    printf("ptr_size=%d\n", type_size(TY_PTR_INT));

    // EXPECT: int_is_int=1
    printf("int_is_int=%d\n", is_integer(TY_INT));
    // EXPECT: ptr_is_int=0
    printf("ptr_is_int=%d\n", is_integer(TY_PTR_INT));
    // EXPECT: ptr_is_ptr=1
    printf("ptr_is_ptr=%d\n", is_pointer(TY_PTR_INT));

    // EXPECT: assign_int_int=1
    printf("assign_int_int=%d\n", can_assign(TY_INT, TY_INT));
    // EXPECT: assign_int_char=1
    printf("assign_int_char=%d\n", can_assign(TY_INT, TY_CHAR));
    // EXPECT: assign_int_ptr=0
    printf("assign_int_ptr=%d\n", can_assign(TY_INT, TY_PTR_INT));
    // EXPECT: assign_ptr_void=1
    printf("assign_ptr_void=%d\n", can_assign(TY_PTR_INT, TY_PTR_VOID));
    // EXPECT: assign_void_ptr=1
    printf("assign_void_ptr=%d\n", can_assign(TY_PTR_VOID, TY_PTR_CHAR));

    // EXPECT: add_int_int=1
    printf("add_int_int=%d\n", can_add(TY_INT, TY_INT));
    // EXPECT: add_ptr_int=1
    printf("add_ptr_int=%d\n", can_add(TY_PTR_INT, TY_INT));
    // EXPECT: add_ptr_ptr=0
    printf("add_ptr_ptr=%d\n", can_add(TY_PTR_INT, TY_PTR_INT));

    // EXPECT: sub_ptr_ptr=1
    printf("sub_ptr_ptr=%d\n", can_sub(TY_PTR_INT, TY_PTR_INT));
    // EXPECT: sub_ptr_diff=0
    printf("sub_ptr_diff=%d\n", can_sub(TY_PTR_INT, TY_PTR_CHAR));

    // EXPECT: cmp_int_int=1
    printf("cmp_int_int=%d\n", can_compare(TY_INT, TY_INT));
    // EXPECT: cmp_int_ptr=0
    printf("cmp_int_ptr=%d\n", can_compare(TY_INT, TY_PTR_INT));

    // EXPECT: add_res=int*
    printf("add_res=%s\n", type_name(result_type_add(TY_PTR_INT, TY_INT)));

    // EXPECT: deref_int_ptr=int
    printf("deref_int_ptr=%s\n", type_name(deref_type(TY_PTR_INT)));
    // EXPECT: addr_of_char=char*
    printf("addr_of_char=%s\n", type_name(addr_of_type(TY_CHAR)));

    init_vars();
    declare_var(0, TY_INT);
    declare_var(1, TY_PTR_CHAR);
    // EXPECT: check_a_int=1
    printf("check_a_int=%d\n", check_assign_var(0, TY_INT));
    // EXPECT: check_a_ptr=0
    printf("check_a_ptr=%d\n", check_assign_var(0, TY_PTR_INT));
    // EXPECT: check_b_void=1
    printf("check_b_void=%d\n", check_assign_var(1, TY_PTR_VOID));
    // EXPECT: vars=2
    printf("vars=%d\n", var_count);

    // EXPECT: sub_res=int
    printf("sub_res=%s\n", type_name(result_type_sub(TY_PTR_INT, TY_PTR_INT)));

    return 0;
}
