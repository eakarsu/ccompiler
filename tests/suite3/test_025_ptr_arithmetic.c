int printf(const char *fmt, ...);

/* test_025_ptr_arithmetic.c
   Tests: pointer add/subtract with ints, pointer difference,
          pointer to int vs pointer to char stride behavior,
          walking arrays with pointer math
*/

int sum_range(int *start, int *end) {
    int s = 0;
    int *p;
    for (p = start; p < end; p++) s += *p;
    return s;
}

int count_range(char *start, char *end) {
    int n = 0;
    char *p;
    for (p = start; p < end; p++) n++;
    return n;
}

/* return pointer to first element >= val */
int *lower_bound(int *a, int n, int val) {
    int i;
    for (i = 0; i < n; i++) {
        if (a[i] >= val) return a + i;
    }
    return a + n;
}

int main(void) {
    /* int* arithmetic: each step is sizeof(int) = 4 bytes */
    int iarr[6] = {10, 20, 30, 40, 50, 60};
    int *ip = iarr;

    printf("%d\n", *ip);
    // EXPECT: 10
    ip = ip + 1;
    printf("%d\n", *ip);
    // EXPECT: 20
    ip = ip + 2;
    printf("%d\n", *ip);
    // EXPECT: 40
    ip = ip - 1;
    printf("%d\n", *ip);
    // EXPECT: 30

    /* pointer + n */
    int *ip2 = iarr + 5;
    printf("%d\n", *ip2);
    // EXPECT: 60

    /* pointer difference */
    int *pa = iarr;
    int *pb = iarr + 5;
    int d = (int)(pb - pa);
    printf("%d\n", d);
    // EXPECT: 5

    int *pc = iarr + 3;
    d = (int)(pc - pa);
    printf("%d\n", d);
    // EXPECT: 3

    d = (int)(pa - pc);
    printf("%d\n", d);
    // EXPECT: -3

    /* char* arithmetic: each step is 1 byte */
    char carr[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    char *cp = carr;

    printf("%d\n", (int)*cp);
    // EXPECT: 1
    cp++;
    printf("%d\n", (int)*cp);
    // EXPECT: 2
    cp = cp + 3;
    printf("%d\n", (int)*cp);
    // EXPECT: 5
    cp = cp - 2;
    printf("%d\n", (int)*cp);
    // EXPECT: 3

    /* char pointer difference */
    char *ca = carr;
    char *cb = carr + 7;
    int cd = (int)(cb - ca);
    printf("%d\n", cd);
    // EXPECT: 7

    /* sum_range using pointer range */
    int data[5] = {1, 2, 3, 4, 5};
    printf("%d\n", sum_range(data, data + 5));
    // EXPECT: 15
    printf("%d\n", sum_range(data, data + 3));
    // EXPECT: 6
    printf("%d\n", sum_range(data + 2, data + 5));
    // EXPECT: 12

    /* count_range using char pointers */
    char str[6] = {'a', 'b', 'c', 'd', 'e', '\0'};
    printf("%d\n", count_range(str, str + 5));
    // EXPECT: 5
    printf("%d\n", count_range(str, str + 2));
    // EXPECT: 2

    /* walking with while loop */
    int walk[7] = {3, 1, 4, 1, 5, 9, 2};
    int *wp = walk;
    int *wend = walk + 7;
    int wsum = 0;
    while (wp < wend) {
        wsum += *wp;
        wp++;
    }
    printf("%d\n", wsum);
    // EXPECT: 25

    /* lower_bound */
    int sorted[6] = {2, 4, 6, 8, 10, 12};
    int *lb = lower_bound(sorted, 6, 5);
    printf("%d\n", (int)(lb - sorted));
    // EXPECT: 2
    printf("%d\n", *lb);
    // EXPECT: 6

    lb = lower_bound(sorted, 6, 1);
    printf("%d\n", (int)(lb - sorted));
    // EXPECT: 0
    printf("%d\n", *lb);
    // EXPECT: 2

    lb = lower_bound(sorted, 6, 12);
    printf("%d\n", (int)(lb - sorted));
    // EXPECT: 5
    printf("%d\n", *lb);
    // EXPECT: 12

    lb = lower_bound(sorted, 6, 13);
    printf("%d\n", (int)(lb - sorted));
    // EXPECT: 6

    /* stride: pointer increments by element size */
    int ia[3] = {100, 200, 300};
    int *iptr = ia;
    printf("%d\n", *(iptr + 0));
    // EXPECT: 100
    printf("%d\n", *(iptr + 1));
    // EXPECT: 200
    printf("%d\n", *(iptr + 2));
    // EXPECT: 300

    /* pointer comparison in loop */
    int vals[5] = {5, 4, 3, 2, 1};
    int *vp = vals;
    int *vlast = vals + 4;
    int vprod = 1;
    while (vp <= vlast) {
        vprod *= *vp;
        vp++;
    }
    /* 5*4*3*2*1 = 120 */
    printf("%d\n", vprod);
    // EXPECT: 120

    /* pointer to last element then decrement */
    int rev[4] = {10, 20, 30, 40};
    int *rp = rev + 3;
    printf("%d\n", *rp);
    // EXPECT: 40
    rp--;
    printf("%d\n", *rp);
    // EXPECT: 30
    rp--;
    printf("%d\n", *rp);
    // EXPECT: 20
    rp--;
    printf("%d\n", *rp);
    // EXPECT: 10

    /* combining add and subscript */
    int base[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    int *mid = base + 4;
    printf("%d\n", mid[-1]);
    // EXPECT: 3
    printf("%d\n", mid[0]);
    // EXPECT: 4
    printf("%d\n", mid[1]);
    // EXPECT: 5

    return 0;
}
