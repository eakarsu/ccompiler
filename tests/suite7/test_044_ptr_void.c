int printf(const char *fmt, ...);

struct Pair {
    int first;
    int second;
};

int get_int(void *p) {
    int *ip = (int *)p;
    return *ip;
}

char get_char(void *p) {
    char *cp = (char *)p;
    return *cp;
}

void set_int(void *p, int val) {
    int *ip = (int *)p;
    *ip = val;
}

int main(void) {
    int x = 42;
    void *vp = &x;
    int *ip = (int *)vp;
    // EXPECT: int_via_void = 42
    printf("int_via_void = %d\n", *ip);

    char c = 65;
    vp = &c;
    char *cp = (char *)vp;
    // EXPECT: char_via_void = 65
    printf("char_via_void = %d\n", (int)*cp);

    struct Pair pr;
    pr.first = 100;
    pr.second = 200;
    vp = &pr;
    struct Pair *pp = (struct Pair *)vp;
    // EXPECT: pair_first = 100
    printf("pair_first = %d\n", pp->first);

    // EXPECT: pair_second = 200
    printf("pair_second = %d\n", pp->second);

    // Use void* in function calls
    int val = 77;
    // EXPECT: func_get_int = 77
    printf("func_get_int = %d\n", get_int(&val));

    char ch = 88;
    // EXPECT: func_get_char = 88
    printf("func_get_char = %d\n", (int)get_char(&ch));

    int target = 0;
    set_int(&target, 555);
    // EXPECT: func_set_int = 555
    printf("func_set_int = %d\n", target);

    // Array of void pointers
    int a = 10;
    int b = 20;
    int c2 = 30;
    void *ptrs[3];
    ptrs[0] = &a;
    ptrs[1] = &b;
    ptrs[2] = &c2;

    int sum = 0;
    int i;
    for (i = 0; i < 3; i++) {
        sum = sum + *(int *)ptrs[i];
    }
    // EXPECT: void_arr_sum = 60
    printf("void_arr_sum = %d\n", sum);

    // Mixed types in void* array
    int num = 999;
    char letter = 65;
    struct Pair sp;
    sp.first = 11;
    sp.second = 22;

    void *mixed[3];
    mixed[0] = &num;
    mixed[1] = &letter;
    mixed[2] = &sp;

    // EXPECT: mixed_int = 999
    printf("mixed_int = %d\n", *(int *)mixed[0]);

    // EXPECT: mixed_char = 65
    printf("mixed_char = %d\n", (int)*(char *)mixed[1]);

    // EXPECT: mixed_struct_f = 11
    printf("mixed_struct_f = %d\n", ((struct Pair *)mixed[2])->first);

    // EXPECT: mixed_struct_s = 22
    printf("mixed_struct_s = %d\n", ((struct Pair *)mixed[2])->second);

    // Swap via void pointers
    int s1 = 111;
    int s2 = 222;
    void *vp1 = &s1;
    void *vp2 = &s2;
    int tmp = *(int *)vp1;
    *(int *)vp1 = *(int *)vp2;
    *(int *)vp2 = tmp;
    // EXPECT: swapped_s1 = 222
    printf("swapped_s1 = %d\n", s1);

    // EXPECT: swapped_s2 = 111
    printf("swapped_s2 = %d\n", s2);

    // Chain of void* casts
    int chain_val = 7777;
    void *v1 = &chain_val;
    void *v2 = v1;
    void *v3 = v2;
    int *final_ip = (int *)v3;
    // EXPECT: chain_val = 7777
    printf("chain_val = %d\n", *final_ip);

    // Null void pointer check
    void *np = 0;
    int is_null = (np == 0);
    // EXPECT: is_null = 1
    printf("is_null = %d\n", is_null);

    return 0;
}
