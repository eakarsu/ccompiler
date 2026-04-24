int printf(const char *fmt, ...);
// EXPECT: triple_ptr: x=100\nlist: 10 20 30 40\nlist_insert: 10 20 25 30 40\nptr_arr_sum=165\npa: 100 200 300 400 500\nswap: px=20 py=10\nredirect: p=99\ngrid: 0 1 2 3 10 11 12 13 20 21 22 23\nstruct_diff=3\nchain: outer_id=1 inner_val=999

/* -------------------------------------------------------
 * Stress test: pointer chains, linked list in array,
 * complex pointer patterns targeting ARM64 codegen.
 * Stack-allocated only, no #include, no malloc/free.
 * ------------------------------------------------------- */

/* ----- Linked list node (index-based) ----- */
struct Node {
    int val;
    int next;   /* index into array, -1 = end */
};

/* ----- Structs for chain test (section 9) ----- */
struct Inner {
    int val;
};

struct Outer {
    int id;
    struct Inner *inner;
};

/* ----- Struct for pointer difference test (section 8) ----- */
struct PairS {
    int a;
    int b;
};

/* =========================================================
 * Section 5: swap two int-pointers via double pointer
 * ========================================================= */
void swap(int **a, int **b) {
    int *tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

/* =========================================================
 * Section 6: redirect a pointer-to-pointer to a new target
 * ========================================================= */
void redirect(int **pp, int *new_target) {
    *pp = new_target;
}

/* =========================================================
 * Helper: traverse index-based linked list and print values
 * ========================================================= */
void print_list(struct Node *list, int head) {
    int cur;
    int first;
    cur = head;
    first = 1;
    while (cur != -1) {
        if (first) {
            first = 0;
        } else {
            printf(" ");
        }
        printf("%d", list[cur].val);
        cur = list[cur].next;
    }
}

/* =========================================================
 * main
 * ========================================================= */
int main() {

    /* =====================================================
     * Section 1: Triple pointer
     * ===================================================== */
    {
        int x;
        int *p;
        int **pp;
        int ***ppp;

        x = 42;
        p = &x;
        pp = &p;
        ppp = &pp;

        /* Dereference three levels and write */
        ***ppp = 100;

        printf("triple_ptr: x=%d\n", x);
    }

    /* =====================================================
     * Section 2: Linked list using array (index-based)
     * ===================================================== */
    {
        struct Node list[10];
        int head;
        int i;

        /* Build list: 0 -> 1 -> 2 -> 3 -> -1
         * values:     10   20   30   40           */
        list[0].val = 10;  list[0].next = 1;
        list[1].val = 20;  list[1].next = 2;
        list[2].val = 30;  list[2].next = 3;
        list[3].val = 40;  list[3].next = -1;
        head = 0;

        /* Traverse and print */
        printf("list: ");
        print_list(list, head);
        printf("\n");

        /* Insert node with val=25 between nodes 1 and 2.
         * Use slot 4 in the array.
         * Before: 0->1->2->3->-1
         * After:  0->1->4->2->3->-1               */
        list[4].val = 25;
        list[4].next = list[1].next;   /* 4 -> 2 */
        list[1].next = 4;              /* 1 -> 4 */

        printf("list_insert: ");
        print_list(list, head);
        printf("\n");
    }

    /* =====================================================
     * Section 3: Array of pointers to local ints
     * ===================================================== */
    {
        int a;
        int b;
        int c;
        int d;
        int e;
        int *ptrs[5];
        int sum;
        int i;

        a = 11;
        b = 22;
        c = 33;
        d = 44;
        e = 55;

        ptrs[0] = &a;
        ptrs[1] = &b;
        ptrs[2] = &c;
        ptrs[3] = &d;
        ptrs[4] = &e;

        sum = 0;
        i = 0;
        while (i < 5) {
            sum = sum + *ptrs[i];
            i = i + 1;
        }
        printf("ptr_arr_sum=%d\n", sum);
    }

    /* =====================================================
     * Section 4: Pointer to array
     * ===================================================== */
    {
        int arr[5];
        int (*pa)[5];
        int i;

        arr[0] = 100;
        arr[1] = 200;
        arr[2] = 300;
        arr[3] = 400;
        arr[4] = 500;

        pa = &arr;

        printf("pa:");
        i = 0;
        while (i < 5) {
            printf(" %d", (*pa)[i]);
            i = i + 1;
        }
        printf("\n");
    }

    /* =====================================================
     * Section 5: Swap via double pointer
     * ===================================================== */
    {
        int x;
        int y;
        int *px;
        int *py;

        x = 10;
        y = 20;
        px = &x;
        py = &y;

        swap(&px, &py);

        /* After swap, px points to y (20) and py points to x (10) */
        printf("swap: px=%d py=%d\n", *px, *py);
    }

    /* =====================================================
     * Section 6: Redirect pointer-to-pointer
     * ===================================================== */
    {
        int x;
        int y;
        int *p;

        x = 5;
        y = 99;
        p = &x;

        redirect(&p, &y);

        /* Now p points to y */
        printf("redirect: p=%d\n", *p);
    }

    /* =====================================================
     * Section 7: 2D grid access via pointer arithmetic
     * ===================================================== */
    {
        int grid[3][4];
        int i;
        int j;
        int first;

        /* Fill grid: grid[i][j] = i*10 + j */
        i = 0;
        while (i < 3) {
            j = 0;
            while (j < 4) {
                *(*(grid + i) + j) = i * 10 + j;
                j = j + 1;
            }
            i = i + 1;
        }

        /* Read back and print using pointer arithmetic */
        printf("grid:");
        first = 1;
        i = 0;
        while (i < 3) {
            j = 0;
            while (j < 4) {
                int val;
                val = *(*(grid + i) + j);
                if (first) {
                    printf(" %d", val);
                    first = 0;
                } else {
                    printf(" %d", val);
                }
                j = j + 1;
            }
            i = i + 1;
        }
        printf("\n");
    }

    /* =====================================================
     * Section 8: Pointer difference on struct array
     * ===================================================== */
    {
        struct PairS arr[4];
        int diff;

        arr[0].a = 1;  arr[0].b = 2;
        arr[1].a = 3;  arr[1].b = 4;
        arr[2].a = 5;  arr[2].b = 6;
        arr[3].a = 7;  arr[3].b = 8;

        /* Pointer difference should account for struct stride */
        diff = (int)(&arr[3] - &arr[0]);

        printf("struct_diff=%d\n", diff);
    }

    /* =====================================================
     * Section 9: Chain -- pointer to struct containing
     *            pointer to another struct
     * ===================================================== */
    {
        struct Inner inner;
        struct Outer outer;

        inner.val = 999;

        outer.id = 1;
        outer.inner = &inner;

        printf("chain: outer_id=%d inner_val=%d\n",
               outer.id, outer.inner->val);
    }

    return 0;
}
