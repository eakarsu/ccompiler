int printf(const char *fmt, ...);
// EXPECT: A={1,3,5,7,9} B={2,3,5,8,9,10} U={1,2,3,5,7,8,9,10} UN=8 I={3,5,9} IN=3 D1={1,7} D1N=2 D2={2,8,10} D2N=3 SD={1,2,7,8,10} SDN=5 SUB1=0 SUB2=1 SUB3=1 SUB4=1 EQ1=1 EQ2=0 CT1=1 CT2=0 CT3=1 CT4=0 CA=5 CB=6 PA=32 PB=64 SA=25 SB=37 MNA=1 MXA=9 RA=8 MNB=2 MXB=10 RB=8 EM1=0 EM2=1 DJ1=0 DJ2=1 CO={0,2,4,6,8,10} CON=6 INS1={1,3,4,5,7,9} INS2={1,3,4,5,7,9} REM1={1,3,5,7,9} REM2={1,3,5,7,9} PS10=3 PS12=3 MK={1,3,5,7} MKN=4 ID=1 
/* ====================================================================== */
/* Test 048: Set Operations using Sorted Arrays                           */
/* Union, intersection, difference, subset check, symmetric difference    */
/* ====================================================================== */

/* Sort array (insertion sort) */
void sort_array(int *arr, int n) {
    int i;
    int j;
    int key;

    i = 1;
    while (i < n) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
        i = i + 1;
    }
}

/* Remove duplicates from sorted array, return new size */
int remove_duplicates(int *arr, int n) {
    int i;
    int j;

    if (n <= 1) return n;

    j = 0;
    i = 1;
    while (i < n) {
        if (arr[i] != arr[j]) {
            j = j + 1;
            arr[j] = arr[i];
        }
        i = i + 1;
    }
    return j + 1;
}

/* Create a set from unsorted array (sort + remove dups) */
int make_set(int *arr, int n) {
    sort_array(arr, n);
    return remove_duplicates(arr, n);
}

/* Print set */
void print_set(int *arr, int n) {
    int i;
    printf("{");
    i = 0;
    while (i < n) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(",");
        i = i + 1;
    }
    printf("}");
}

/* Union of two sorted sets */
int set_union(int *a, int an, int *b, int bn, int *result) {
    int i;
    int j;
    int k;

    i = 0;
    j = 0;
    k = 0;

    while (i < an && j < bn) {
        if (a[i] < b[j]) {
            result[k] = a[i];
            k = k + 1;
            i = i + 1;
        } else if (a[i] > b[j]) {
            result[k] = b[j];
            k = k + 1;
            j = j + 1;
        } else {
            result[k] = a[i];
            k = k + 1;
            i = i + 1;
            j = j + 1;
        }
    }
    while (i < an) {
        result[k] = a[i];
        k = k + 1;
        i = i + 1;
    }
    while (j < bn) {
        result[k] = b[j];
        k = k + 1;
        j = j + 1;
    }
    return k;
}

/* Intersection of two sorted sets */
int set_intersection(int *a, int an, int *b, int bn, int *result) {
    int i;
    int j;
    int k;

    i = 0;
    j = 0;
    k = 0;

    while (i < an && j < bn) {
        if (a[i] < b[j]) {
            i = i + 1;
        } else if (a[i] > b[j]) {
            j = j + 1;
        } else {
            result[k] = a[i];
            k = k + 1;
            i = i + 1;
            j = j + 1;
        }
    }
    return k;
}

/* Difference: A - B (elements in A but not in B) */
int set_difference(int *a, int an, int *b, int bn, int *result) {
    int i;
    int j;
    int k;

    i = 0;
    j = 0;
    k = 0;

    while (i < an && j < bn) {
        if (a[i] < b[j]) {
            result[k] = a[i];
            k = k + 1;
            i = i + 1;
        } else if (a[i] > b[j]) {
            j = j + 1;
        } else {
            i = i + 1;
            j = j + 1;
        }
    }
    while (i < an) {
        result[k] = a[i];
        k = k + 1;
        i = i + 1;
    }
    return k;
}

/* Symmetric difference: (A - B) union (B - A) */
int set_symmetric_diff(int *a, int an, int *b, int bn, int *result) {
    int i;
    int j;
    int k;

    i = 0;
    j = 0;
    k = 0;

    while (i < an && j < bn) {
        if (a[i] < b[j]) {
            result[k] = a[i];
            k = k + 1;
            i = i + 1;
        } else if (a[i] > b[j]) {
            result[k] = b[j];
            k = k + 1;
            j = j + 1;
        } else {
            i = i + 1;
            j = j + 1;
        }
    }
    while (i < an) {
        result[k] = a[i];
        k = k + 1;
        i = i + 1;
    }
    while (j < bn) {
        result[k] = b[j];
        k = k + 1;
        j = j + 1;
    }
    return k;
}

/* Check if A is subset of B */
int is_subset(int *a, int an, int *b, int bn) {
    int i;
    int j;

    i = 0;
    j = 0;

    while (i < an && j < bn) {
        if (a[i] < b[j]) {
            return 0; /* a[i] not in b */
        } else if (a[i] > b[j]) {
            j = j + 1;
        } else {
            i = i + 1;
            j = j + 1;
        }
    }
    if (i < an) return 0;
    return 1;
}

/* Check if two sets are equal */
int sets_equal(int *a, int an, int *b, int bn) {
    int i;
    if (an != bn) return 0;
    i = 0;
    while (i < an) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return 1;
}

/* Check if element is in set (binary search) */
int set_contains(int *arr, int n, int val) {
    int lo;
    int hi;
    int mid;

    lo = 0;
    hi = n - 1;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        if (arr[mid] == val) return 1;
        if (arr[mid] < val) lo = mid + 1;
        else hi = mid - 1;
    }
    return 0;
}

/* Cardinality (just the size) */
int set_card(int n) {
    return n;
}

/* Power set cardinality = 2^n */
int power_set_card(int n) {
    int result;
    int i;
    result = 1;
    i = 0;
    while (i < n) {
        result = result * 2;
        i = i + 1;
    }
    return result;
}

/* Sum of elements in set */
int set_sum(int *arr, int n) {
    int sum;
    int i;
    sum = 0;
    i = 0;
    while (i < n) {
        sum = sum + arr[i];
        i = i + 1;
    }
    return sum;
}

/* Min element (first in sorted set) */
int set_min(int *arr, int n) {
    if (n == 0) return -1;
    return arr[0];
}

/* Max element (last in sorted set) */
int set_max(int *arr, int n) {
    if (n == 0) return -1;
    return arr[n - 1];
}

/* Range of set (max - min) */
int set_range(int *arr, int n) {
    if (n == 0) return 0;
    return arr[n - 1] - arr[0];
}

/* Check if set is empty */
int set_is_empty(int n) {
    return (n == 0) ? 1 : 0;
}

/* Check if two sets are disjoint (intersection is empty) */
int sets_disjoint(int *a, int an, int *b, int bn) {
    int result[50];
    int rn;
    rn = set_intersection(a, an, b, bn, result);
    return (rn == 0) ? 1 : 0;
}

/* Complement with respect to universal set {0..max} */
int set_complement(int *a, int an, int max_val, int *result) {
    int i;
    int j;
    int k;

    k = 0;
    j = 0;
    i = 0;
    while (i <= max_val) {
        if (j < an && a[j] == i) {
            j = j + 1;
        } else {
            result[k] = i;
            k = k + 1;
        }
        i = i + 1;
    }
    return k;
}

/* Insert element into sorted set, return new size */
int set_insert(int *arr, int n, int val) {
    int i;
    int j;

    /* Check if already present */
    if (set_contains(arr, n, val)) return n;

    /* Find position */
    i = n - 1;
    while (i >= 0 && arr[i] > val) {
        arr[i + 1] = arr[i];
        i = i - 1;
    }
    arr[i + 1] = val;
    return n + 1;
}

/* Remove element from sorted set, return new size */
int set_remove(int *arr, int n, int val) {
    int i;
    int j;
    int found;

    found = -1;
    i = 0;
    while (i < n) {
        if (arr[i] == val) {
            found = i;
            i = n;
        }
        i = i + 1;
    }

    if (found < 0) return n;

    i = found;
    while (i < n - 1) {
        arr[i] = arr[i + 1];
        i = i + 1;
    }
    return n - 1;
}

/* Count pairs with given sum */
int count_pairs_with_sum(int *a, int an, int *b, int bn, int target) {
    int count;
    int i;
    int j;

    count = 0;
    i = 0;
    while (i < an) {
        j = 0;
        while (j < bn) {
            if (a[i] + b[j] == target) count = count + 1;
            j = j + 1;
        }
        i = i + 1;
    }
    return count;
}

int main(void) {
    int a[20];
    int b[20];
    int c[20];
    int result[50];
    int an;
    int bn;
    int cn;
    int rn;

    /* Set A = {1, 3, 5, 7, 9} */
    a[0]=1; a[1]=3; a[2]=5; a[3]=7; a[4]=9;
    an = 5;

    /* Set B = {2, 3, 5, 8, 9, 10} */
    b[0]=2; b[1]=3; b[2]=5; b[3]=8; b[4]=9; b[5]=10;
    bn = 6;

    printf("A="); print_set(a, an); printf(" ");
    printf("B="); print_set(b, bn); printf(" ");

    /* Union */
    rn = set_union(a, an, b, bn, result);
    printf("U="); print_set(result, rn); printf(" ");
    printf("UN=%d ", rn);

    /* Intersection */
    rn = set_intersection(a, an, b, bn, result);
    printf("I="); print_set(result, rn); printf(" ");
    printf("IN=%d ", rn);

    /* Difference A - B */
    rn = set_difference(a, an, b, bn, result);
    printf("D1="); print_set(result, rn); printf(" ");
    printf("D1N=%d ", rn);

    /* Difference B - A */
    rn = set_difference(b, bn, a, an, result);
    printf("D2="); print_set(result, rn); printf(" ");
    printf("D2N=%d ", rn);

    /* Symmetric difference */
    rn = set_symmetric_diff(a, an, b, bn, result);
    printf("SD="); print_set(result, rn); printf(" ");
    printf("SDN=%d ", rn);

    /* Subset checks */
    printf("SUB1=%d ", is_subset(a, an, b, bn));
    printf("SUB2=%d ", is_subset(a, an, a, an));

    /* Create subset C = {3, 5, 9} */
    c[0]=3; c[1]=5; c[2]=9;
    cn = 3;
    printf("SUB3=%d ", is_subset(c, cn, a, an));
    printf("SUB4=%d ", is_subset(c, cn, b, bn));

    /* Equality */
    printf("EQ1=%d ", sets_equal(a, an, a, an));
    printf("EQ2=%d ", sets_equal(a, an, b, bn));

    /* Contains */
    printf("CT1=%d ", set_contains(a, an, 5));
    printf("CT2=%d ", set_contains(a, an, 6));
    printf("CT3=%d ", set_contains(b, bn, 10));
    printf("CT4=%d ", set_contains(b, bn, 1));

    /* Cardinality */
    printf("CA=%d ", set_card(an));
    printf("CB=%d ", set_card(bn));

    /* Power set cardinality */
    printf("PA=%d ", power_set_card(an));
    printf("PB=%d ", power_set_card(bn));

    /* Sum */
    printf("SA=%d ", set_sum(a, an));
    printf("SB=%d ", set_sum(b, bn));

    /* Min, Max, Range */
    printf("MNA=%d ", set_min(a, an));
    printf("MXA=%d ", set_max(a, an));
    printf("RA=%d ", set_range(a, an));
    printf("MNB=%d ", set_min(b, bn));
    printf("MXB=%d ", set_max(b, bn));
    printf("RB=%d ", set_range(b, bn));

    /* Empty check */
    printf("EM1=%d ", set_is_empty(an));
    printf("EM2=%d ", set_is_empty(0));

    /* Disjoint check */
    printf("DJ1=%d ", sets_disjoint(a, an, b, bn));
    c[0]=20; c[1]=30;
    cn = 2;
    printf("DJ2=%d ", sets_disjoint(a, an, c, cn));

    /* Complement wrt {0..10} */
    rn = set_complement(a, an, 10, result);
    printf("CO="); print_set(result, rn); printf(" ");
    printf("CON=%d ", rn);

    /* Insert element */
    a[0]=1; a[1]=3; a[2]=5; a[3]=7; a[4]=9;
    an = 5;
    an = set_insert(a, an, 4);
    printf("INS1="); print_set(a, an); printf(" ");
    an = set_insert(a, an, 5); /* already exists */
    printf("INS2="); print_set(a, an); printf(" ");

    /* Remove element */
    an = set_remove(a, an, 4);
    printf("REM1="); print_set(a, an); printf(" ");
    an = set_remove(a, an, 100); /* not present */
    printf("REM2="); print_set(a, an); printf(" ");

    /* Pairs with sum */
    a[0]=1; a[1]=3; a[2]=5; a[3]=7; a[4]=9;
    an = 5;
    b[0]=2; b[1]=3; b[2]=5; b[3]=8; b[4]=9; b[5]=10;
    bn = 6;
    printf("PS10=%d ", count_pairs_with_sum(a, an, b, bn, 10));
    printf("PS12=%d ", count_pairs_with_sum(a, an, b, bn, 12));

    /* Make set from unsorted with duplicates */
    {
        int d[10];
        int dn;
        d[0]=5; d[1]=3; d[2]=5; d[3]=1; d[4]=3; d[5]=7; d[6]=1;
        dn = make_set(d, 7);
        printf("MK="); print_set(d, dn); printf(" ");
        printf("MKN=%d ", dn);
    }

    /* Verify identity: |A union B| = |A| + |B| - |A intersect B| */
    {
        int u[50];
        int inter[50];
        int un_size;
        int in_size;

        a[0]=1; a[1]=3; a[2]=5; a[3]=7; a[4]=9;
        an = 5;
        b[0]=2; b[1]=3; b[2]=5; b[3]=8; b[4]=9; b[5]=10;
        bn = 6;

        un_size = set_union(a, an, b, bn, u);
        in_size = set_intersection(a, an, b, bn, inter);
        printf("ID=%d ", (un_size == an + bn - in_size) ? 1 : 0);
    }

    printf("\n");
    return 0;
}
