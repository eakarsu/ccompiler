int printf(const char *fmt, ...);
// EXPECT: basic_deref=10
// ptr_assign=20
// ptr_arith_0=1
// ptr_arith_1=2
// ptr_arith_2=3
// ptr_arith_3=4
// ptr_arith_4=5
// ptr_diff=5
// swap: a=20 b=10
// pp_val=42
// pp_modify=99
// arr_ptr_0=10
// arr_ptr_1=20
// arr_ptr_2=30
// arr_ptr_3=40
// arr_ptr_4=50
// ptr_cmp: less=1 equal=0 greater=0
// ptr_cmp2: less=0 equal=1 greater=0
// walk_sum=150
// reverse: 50 40 30 20 10
// chain_val=77
// deep_pp=88
// func_ptr_sum=30
// func_ptr_prod=200
// ptr_inc_0=100
// ptr_inc_1=200
// ptr_inc_2=300
// offset_read_0=5
// offset_read_1=15
// offset_read_2=25
// offset_read_3=35
// offset_read_4=45
// fill_via_ptr: 0 2 4 6 8 10 12 14 16 18
// max_via_ptr=18
// min_via_ptr=0
// copy_arr: 0 2 4 6 8 10 12 14 16 18
// find_ptr_idx=3
// bubble_via_ptr: 1 2 3 4 5 6 7 8 9 10
// sum_via_pp=55
// matrix_00=1
// matrix_01=2
// matrix_10=3
// matrix_11=4
// tri_ptr_val=123
// assign_chain=50
// null_check=safe
// ptr_array_0=10
// ptr_array_1=20
// ptr_array_2=30
// local_addr_sum=60
// struct_ptr_x=5
// struct_ptr_y=10
// arr_of_ptr_sum=60
// cascade_val=256
// xor_swap: a=88 b=44
// ptr_walk_back_4=5
// ptr_walk_back_3=4
// ptr_walk_back_2=3
// ptr_walk_back_1=2
// ptr_walk_back_0=1
// rotate_left: 2 3 4 5 1
// partial_sum_0=1
// partial_sum_1=3
// partial_sum_2=6
// partial_sum_3=10
// partial_sum_4=15
// ptr_flag=1
// done

// =============================================================================
// TEST 004: Pointers
// =============================================================================

// --- Swap two integers via pointers ---
void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// --- Sum array via pointer walking ---
int sum_array_ptr(int *arr, int n) {
    int s = 0;
    int *p = arr;
    int i = 0;
    while (i < n) {
        s = s + *p;
        p = p + 1;
        i = i + 1;
    }
    return s;
}

// --- Find element, return index or -1 ---
int find_element(int *arr, int n, int val) {
    int i = 0;
    while (i < n) {
        if (*(arr + i) == val) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

// --- Copy array via pointers ---
void copy_array(int *dst, int *src, int n) {
    int i = 0;
    while (i < n) {
        *(dst + i) = *(src + i);
        i = i + 1;
    }
}

// --- Reverse array in-place via pointers ---
void reverse_array(int *arr, int n) {
    int *lo = arr;
    int *hi = arr + n - 1;
    while (lo < hi) {
        int tmp = *lo;
        *lo = *hi;
        *hi = tmp;
        lo = lo + 1;
        hi = hi - 1;
    }
}

// --- Fill array with even numbers ---
void fill_evens(int *arr, int n) {
    int i = 0;
    while (i < n) {
        *(arr + i) = i * 2;
        i = i + 1;
    }
}

// --- Find max via pointer ---
int find_max_ptr(int *arr, int n) {
    int mx = *arr;
    int i = 1;
    while (i < n) {
        if (*(arr + i) > mx) {
            mx = *(arr + i);
        }
        i = i + 1;
    }
    return mx;
}

// --- Find min via pointer ---
int find_min_ptr(int *arr, int n) {
    int mn = *arr;
    int i = 1;
    while (i < n) {
        if (*(arr + i) < mn) {
            mn = *(arr + i);
        }
        i = i + 1;
    }
    return mn;
}

// --- Bubble sort via pointers ---
void bubble_sort_ptr(int *arr, int n) {
    int i = 0;
    while (i < n - 1) {
        int j = 0;
        while (j < n - 1 - i) {
            if (*(arr + j) > *(arr + j + 1)) {
                swap(arr + j, arr + j + 1);
            }
            j = j + 1;
        }
        i = i + 1;
    }
}

// --- Sum via pointer-to-pointer (array of pointers) ---
int sum_via_pp(int **ptrs, int n) {
    int s = 0;
    int i = 0;
    while (i < n) {
        s = s + *ptrs[i];
        i = i + 1;
    }
    return s;
}

// --- Simple add/multiply for function pointer tests ---
int add_fn(int a, int b) {
    return a + b;
}

int mul_fn(int a, int b) {
    return a * b;
}

int apply_fn(int (*f)(int, int), int x, int y) {
    return f(x, y);
}

// --- Struct for pointer test ---
struct Point {
    int x;
    int y;
};

// --- Rotate left by one via pointers ---
void rotate_left_one(int *arr, int n) {
    int first = *arr;
    int i = 0;
    while (i < n - 1) {
        *(arr + i) = *(arr + i + 1);
        i = i + 1;
    }
    *(arr + n - 1) = first;
}

// --- Prefix sum via pointers ---
void prefix_sum(int *arr, int *out, int n) {
    *out = *arr;
    int i = 1;
    while (i < n) {
        *(out + i) = *(out + i - 1) + *(arr + i);
        i = i + 1;
    }
}

// --- XOR swap via pointers ---
void xor_swap(int *a, int *b) {
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

// --- Increment all elements ---
void increment_all(int *arr, int n, int val) {
    int i = 0;
    while (i < n) {
        *(arr + i) = *(arr + i) + val;
        i = i + 1;
    }
}

// --- Zero out array ---
void zero_array(int *arr, int n) {
    int i = 0;
    while (i < n) {
        *(arr + i) = 0;
        i = i + 1;
    }
}

// --- Count occurrences ---
int count_occurrences(int *arr, int n, int val) {
    int count = 0;
    int i = 0;
    while (i < n) {
        if (*(arr + i) == val) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

// --- Set value at pointer offset ---
void set_at_offset(int *base, int offset, int val) {
    *(base + offset) = val;
}

// --- Get value at pointer offset ---
int get_at_offset(int *base, int offset) {
    return *(base + offset);
}

// --- Compute dot product ---
int dot_product(int *a, int *b, int n) {
    int result = 0;
    int i = 0;
    while (i < n) {
        result = result + (*(a + i)) * (*(b + i));
        i = i + 1;
    }
    return result;
}

// --- Check if array is sorted ---
int is_sorted(int *arr, int n) {
    int i = 0;
    while (i < n - 1) {
        if (*(arr + i) > *(arr + i + 1)) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

// --- Partition around pivot ---
int partition_arr(int *arr, int lo, int hi) {
    int pivot = *(arr + hi);
    int i = lo;
    int j = lo;
    while (j < hi) {
        if (*(arr + j) < pivot) {
            swap(arr + i, arr + j);
            i = i + 1;
        }
        j = j + 1;
    }
    swap(arr + i, arr + hi);
    return i;
}

// --- Accumulate: running sum in place ---
void accumulate(int *arr, int n) {
    int i = 1;
    while (i < n) {
        *(arr + i) = *(arr + i) + *(arr + i - 1);
        i = i + 1;
    }
}

// --- Scale array elements ---
void scale_array(int *arr, int n, int factor) {
    int i = 0;
    while (i < n) {
        *(arr + i) = *(arr + i) * factor;
        i = i + 1;
    }
}

// --- Negate array elements ---
void negate_array(int *arr, int n) {
    int i = 0;
    while (i < n) {
        *(arr + i) = -(*(arr + i));
        i = i + 1;
    }
}

// --- Inner product of two arrays ---
int inner_product(int *a, int *b, int n) {
    int s = 0;
    int *pa_ip = a;
    int *pb_ip = b;
    int i = 0;
    while (i < n) {
        s = s + (*pa_ip) * (*pb_ip);
        pa_ip = pa_ip + 1;
        pb_ip = pb_ip + 1;
        i = i + 1;
    }
    return s;
}

int main() {
    // --- Basic dereference ---
    int x = 10;
    int *px = &x;
    printf("basic_deref=%d\n", *px);

    // --- Pointer assignment ---
    *px = 20;
    printf("ptr_assign=%d\n", x);

    // --- Pointer arithmetic on array ---
    int arr5[5];
    arr5[0] = 1; arr5[1] = 2; arr5[2] = 3; arr5[3] = 4; arr5[4] = 5;
    int *pa = arr5;
    printf("ptr_arith_0=%d\n", *pa);
    printf("ptr_arith_1=%d\n", *(pa + 1));
    printf("ptr_arith_2=%d\n", *(pa + 2));
    printf("ptr_arith_3=%d\n", *(pa + 3));
    printf("ptr_arith_4=%d\n", *(pa + 4));

    // --- Pointer difference ---
    int *p_end = pa + 5;
    int diff = p_end - pa;
    printf("ptr_diff=%d\n", diff);

    // --- Swap via pointers ---
    int sa = 10;
    int sb = 20;
    swap(&sa, &sb);
    printf("swap: a=%d b=%d\n", sa, sb);

    // --- Pointer to pointer ---
    int val42 = 42;
    int *pv = &val42;
    int **ppv = &pv;
    printf("pp_val=%d\n", **ppv);

    **ppv = 99;
    printf("pp_modify=%d\n", val42);

    // --- Array-pointer equivalence ---
    int arr_ap[5];
    arr_ap[0] = 10; arr_ap[1] = 20; arr_ap[2] = 30; arr_ap[3] = 40; arr_ap[4] = 50;
    int *ap = arr_ap;
    printf("arr_ptr_0=%d\n", ap[0]);
    printf("arr_ptr_1=%d\n", ap[1]);
    printf("arr_ptr_2=%d\n", ap[2]);
    printf("arr_ptr_3=%d\n", ap[3]);
    printf("arr_ptr_4=%d\n", ap[4]);

    // --- Pointer comparison ---
    int *cmp_lo = arr_ap;
    int *cmp_hi = arr_ap + 3;
    int cmp_less = (cmp_lo < cmp_hi);
    int cmp_eq = (cmp_lo == cmp_hi);
    int cmp_gt = (cmp_lo > cmp_hi);
    printf("ptr_cmp: less=%d equal=%d greater=%d\n", cmp_less, cmp_eq, cmp_gt);

    int *cmp_same = arr_ap + 3;
    int cmp_less2 = (cmp_hi < cmp_same);
    int cmp_eq2 = (cmp_hi == cmp_same);
    int cmp_gt2 = (cmp_hi > cmp_same);
    printf("ptr_cmp2: less=%d equal=%d greater=%d\n", cmp_less2, cmp_eq2, cmp_gt2);

    // --- Walk and sum: {10,20,30,40,50} => 150 ---
    int ws = sum_array_ptr(arr_ap, 5);
    printf("walk_sum=%d\n", ws);

    // --- Reverse: {10,20,30,40,50} => {50,40,30,20,10} ---
    reverse_array(arr_ap, 5);
    printf("reverse: %d %d %d %d %d\n", arr_ap[0], arr_ap[1], arr_ap[2], arr_ap[3], arr_ap[4]);

    // --- Pointer chain ---
    int chain_val = 77;
    int *cp1 = &chain_val;
    int **cp2 = &cp1;
    printf("chain_val=%d\n", **cp2);

    // --- Deep pointer-to-pointer ---
    int deep = 88;
    int *dp1 = &deep;
    int **dp2 = &dp1;
    printf("deep_pp=%d\n", **dp2);

    // --- Function pointers ---
    int fp_sum = apply_fn(add_fn, 10, 20);
    printf("func_ptr_sum=%d\n", fp_sum);
    int fp_prod = apply_fn(mul_fn, 10, 20);
    printf("func_ptr_prod=%d\n", fp_prod);

    // --- Pointer increment walk ---
    int arr_inc[3];
    arr_inc[0] = 100; arr_inc[1] = 200; arr_inc[2] = 300;
    int *pi = arr_inc;
    printf("ptr_inc_0=%d\n", *pi);
    pi = pi + 1;
    printf("ptr_inc_1=%d\n", *pi);
    pi = pi + 1;
    printf("ptr_inc_2=%d\n", *pi);

    // --- Offset read ---
    int arr_off[5];
    arr_off[0] = 5; arr_off[1] = 15; arr_off[2] = 25; arr_off[3] = 35; arr_off[4] = 45;
    printf("offset_read_0=%d\n", get_at_offset(arr_off, 0));
    printf("offset_read_1=%d\n", get_at_offset(arr_off, 1));
    printf("offset_read_2=%d\n", get_at_offset(arr_off, 2));
    printf("offset_read_3=%d\n", get_at_offset(arr_off, 3));
    printf("offset_read_4=%d\n", get_at_offset(arr_off, 4));

    // --- Fill via pointer: {0,2,4,6,8,10,12,14,16,18} ---
    int arr_fill[10];
    fill_evens(arr_fill, 10);
    printf("fill_via_ptr: %d %d %d %d %d %d %d %d %d %d\n",
        arr_fill[0], arr_fill[1], arr_fill[2], arr_fill[3], arr_fill[4],
        arr_fill[5], arr_fill[6], arr_fill[7], arr_fill[8], arr_fill[9]);

    // --- Max=18, Min=0 ---
    printf("max_via_ptr=%d\n", find_max_ptr(arr_fill, 10));
    printf("min_via_ptr=%d\n", find_min_ptr(arr_fill, 10));

    // --- Copy array ---
    int arr_copy[10];
    copy_array(arr_copy, arr_fill, 10);
    printf("copy_arr: %d %d %d %d %d %d %d %d %d %d\n",
        arr_copy[0], arr_copy[1], arr_copy[2], arr_copy[3], arr_copy[4],
        arr_copy[5], arr_copy[6], arr_copy[7], arr_copy[8], arr_copy[9]);

    // --- Find 6 in {0,2,4,6,8,...} => index 3 ---
    int fidx = find_element(arr_fill, 10, 6);
    printf("find_ptr_idx=%d\n", fidx);

    // --- Bubble sort ---
    int arr_bub[10];
    arr_bub[0] = 5; arr_bub[1] = 3; arr_bub[2] = 8; arr_bub[3] = 1;
    arr_bub[4] = 9; arr_bub[5] = 2; arr_bub[6] = 7; arr_bub[7] = 4;
    arr_bub[8] = 10; arr_bub[9] = 6;
    bubble_sort_ptr(arr_bub, 10);
    printf("bubble_via_ptr: %d %d %d %d %d %d %d %d %d %d\n",
        arr_bub[0], arr_bub[1], arr_bub[2], arr_bub[3], arr_bub[4],
        arr_bub[5], arr_bub[6], arr_bub[7], arr_bub[8], arr_bub[9]);

    // --- Sum via pointer-to-pointer: 1+2+...+10 = 55 ---
    int v1 = 1; int v2 = 2; int v3 = 3; int v4 = 4; int v5 = 5;
    int v6 = 6; int v7 = 7; int v8 = 8; int v9 = 9; int v10 = 10;
    int *ptrs[10];
    ptrs[0] = &v1; ptrs[1] = &v2; ptrs[2] = &v3; ptrs[3] = &v4; ptrs[4] = &v5;
    ptrs[5] = &v6; ptrs[6] = &v7; ptrs[7] = &v8; ptrs[8] = &v9; ptrs[9] = &v10;
    printf("sum_via_pp=%d\n", sum_via_pp(ptrs, 10));

    // --- 2D matrix via flat pointer ---
    int matrix[2][2];
    matrix[0][0] = 1; matrix[0][1] = 2;
    matrix[1][0] = 3; matrix[1][1] = 4;
    int *mp = &matrix[0][0];
    printf("matrix_00=%d\n", *mp);
    printf("matrix_01=%d\n", *(mp + 1));
    printf("matrix_10=%d\n", *(mp + 2));
    printf("matrix_11=%d\n", *(mp + 3));

    // --- Triple pointer ---
    int tri = 123;
    int *tp1 = &tri;
    int **tp2 = &tp1;
    int ***tp3 = &tp2;
    printf("tri_ptr_val=%d\n", ***tp3);

    // --- Assignment chain ---
    int ac1 = 50;
    int *acp = &ac1;
    int ac2 = *acp;
    printf("assign_chain=%d\n", ac2);

    // --- Null pointer check ---
    int *np = 0;
    if (np == 0) {
        printf("null_check=safe\n");
    }

    // --- Array of pointers ---
    int pa1 = 10; int pa2 = 20; int pa3 = 30;
    int *ptr_arr[3];
    ptr_arr[0] = &pa1; ptr_arr[1] = &pa2; ptr_arr[2] = &pa3;
    printf("ptr_array_0=%d\n", *ptr_arr[0]);
    printf("ptr_array_1=%d\n", *ptr_arr[1]);
    printf("ptr_array_2=%d\n", *ptr_arr[2]);

    // --- Sum of local addresses ---
    int la1 = 10; int la2 = 20; int la3 = 30;
    int *lap[3];
    lap[0] = &la1; lap[1] = &la2; lap[2] = &la3;
    int la_sum = 0;
    int lai = 0;
    while (lai < 3) {
        la_sum = la_sum + *lap[lai];
        lai = lai + 1;
    }
    printf("local_addr_sum=%d\n", la_sum);

    // --- Struct via pointer ---
    struct Point pt;
    pt.x = 5;
    pt.y = 10;
    struct Point *ppt = &pt;
    printf("struct_ptr_x=%d\n", ppt->x);
    printf("struct_ptr_y=%d\n", ppt->y);

    // --- Array of pointers sum: 5+10+15+10+15+5 = 60 ---
    int aop_vals[6];
    aop_vals[0] = 5; aop_vals[1] = 10; aop_vals[2] = 15;
    aop_vals[3] = 10; aop_vals[4] = 15; aop_vals[5] = 5;
    int *aop_ptrs[6];
    int aopi = 0;
    while (aopi < 6) {
        aop_ptrs[aopi] = &aop_vals[aopi];
        aopi = aopi + 1;
    }
    int aop_sum = 0;
    aopi = 0;
    while (aopi < 6) {
        aop_sum = aop_sum + *aop_ptrs[aopi];
        aopi = aopi + 1;
    }
    printf("arr_of_ptr_sum=%d\n", aop_sum);

    // --- Cascade: 1 * 2^8 = 256 ---
    int cascade = 1;
    int *cas_p = &cascade;
    int ci = 0;
    while (ci < 8) {
        *cas_p = *cas_p * 2;
        ci = ci + 1;
    }
    printf("cascade_val=%d\n", cascade);

    // --- XOR swap: 44,88 => 88,44 ---
    int xa = 44;
    int xb = 88;
    xor_swap(&xa, &xb);
    printf("xor_swap: a=%d b=%d\n", xa, xb);

    // --- Walk pointer backwards ---
    int wb_arr[5];
    wb_arr[0] = 1; wb_arr[1] = 2; wb_arr[2] = 3; wb_arr[3] = 4; wb_arr[4] = 5;
    int *wb_p = wb_arr + 4;
    printf("ptr_walk_back_4=%d\n", *wb_p);
    wb_p = wb_p - 1;
    printf("ptr_walk_back_3=%d\n", *wb_p);
    wb_p = wb_p - 1;
    printf("ptr_walk_back_2=%d\n", *wb_p);
    wb_p = wb_p - 1;
    printf("ptr_walk_back_1=%d\n", *wb_p);
    wb_p = wb_p - 1;
    printf("ptr_walk_back_0=%d\n", *wb_p);

    // --- Rotate left: {1,2,3,4,5} => {2,3,4,5,1} ---
    int rot_arr[5];
    rot_arr[0] = 1; rot_arr[1] = 2; rot_arr[2] = 3; rot_arr[3] = 4; rot_arr[4] = 5;
    rotate_left_one(rot_arr, 5);
    printf("rotate_left: %d %d %d %d %d\n",
        rot_arr[0], rot_arr[1], rot_arr[2], rot_arr[3], rot_arr[4]);

    // --- Prefix sum: {1,2,3,4,5} => {1,3,6,10,15} ---
    int ps_in[5];
    ps_in[0] = 1; ps_in[1] = 2; ps_in[2] = 3; ps_in[3] = 4; ps_in[4] = 5;
    int ps_out[5];
    prefix_sum(ps_in, ps_out, 5);
    printf("partial_sum_0=%d\n", ps_out[0]);
    printf("partial_sum_1=%d\n", ps_out[1]);
    printf("partial_sum_2=%d\n", ps_out[2]);
    printf("partial_sum_3=%d\n", ps_out[3]);
    printf("partial_sum_4=%d\n", ps_out[4]);

    // --- Pointer boolean flag ---
    int flag_val = 1;
    int *flag_p = &flag_val;
    if (*flag_p) {
        printf("ptr_flag=%d\n", *flag_p);
    }

    printf("done\n");
    return 0;
}
