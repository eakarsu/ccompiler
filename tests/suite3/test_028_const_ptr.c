int printf(const char *fmt, ...);

/* test_028_const_ptr.c
   Tests: const int* (pointer to const), int *const (const pointer),
          const int *const (both const), passing const pointers to functions,
          array decay to const pointer, read-only access patterns
*/

/* accepts pointer to const: cannot modify *p */
int sum_const(const int *p, int n) {
    int s = 0;
    int i;
    for (i = 0; i < n; i++) s += p[i];
    return s;
}

int max_const(const int *p, int n) {
    int m = p[0];
    int i;
    for (i = 1; i < n; i++) {
        if (p[i] > m) m = p[i];
    }
    return m;
}

/* returns pointer to const: caller cannot modify data through it */
const int *get_const_ptr(const int *arr, int idx) {
    return arr + idx;
}

/* const char* parameter: read-only string access */
int count_vowels(const char *s) {
    int n = 0;
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        char c = s[i];
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') n++;
    }
    return n;
}

int my_strlen_const(const char *s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

/* function with const int *const parameter */
int first_elem(const int * const p) {
    return p[0];
}

int main(void) {
    /* const int*: pointer can move, data cannot be written through it */
    int arr[5] = {10, 20, 30, 40, 50};
    const int *cp = arr;

    printf("%d\n", *cp);
    // EXPECT: 10
    printf("%d\n", cp[2]);
    // EXPECT: 30

    /* advance the pointer (pointer itself is not const) */
    cp++;
    printf("%d\n", *cp);
    // EXPECT: 20
    cp = arr + 4;
    printf("%d\n", *cp);
    // EXPECT: 50

    /* reset and pass to functions */
    cp = arr;
    printf("%d\n", sum_const(cp, 5));
    // EXPECT: 150
    printf("%d\n", max_const(cp, 5));
    // EXPECT: 50

    /* int *const: pointer is fixed, data can change */
    int val = 5;
    int * const fixedp = &val;
    printf("%d\n", *fixedp);
    // EXPECT: 5
    *fixedp = 99;
    printf("%d\n", val);
    // EXPECT: 99
    printf("%d\n", *fixedp);
    // EXPECT: 99

    /* const int *const: neither pointer nor data can change */
    int immutable = 42;
    const int * const ccp = &immutable;
    printf("%d\n", *ccp);
    // EXPECT: 42

    /* get_const_ptr: read through returned const pointer */
    int data[4] = {3, 6, 9, 12};
    const int *rp = get_const_ptr(data, 2);
    printf("%d\n", *rp);
    // EXPECT: 9
    rp = get_const_ptr(data, 0);
    printf("%d\n", *rp);
    // EXPECT: 3

    /* array decays to const pointer in function call */
    int ro[3] = {7, 14, 21};
    printf("%d\n", sum_const(ro, 3));
    // EXPECT: 42

    /* count_vowels on string literals */
    printf("%d\n", count_vowels("hello world"));
    // EXPECT: 3
    printf("%d\n", count_vowels("aeiou"));
    // EXPECT: 5
    printf("%d\n", count_vowels("rhythm"));
    // EXPECT: 0
    printf("%d\n", count_vowels("abcdefghijklmno"));
    // EXPECT: 4

    /* const char* for string literals */
    const char *msg = "constant";
    printf("%d\n", my_strlen_const(msg));
    // EXPECT: 8
    printf("%c\n", msg[0]);
    // EXPECT: c
    printf("%c\n", msg[7]);
    // EXPECT: t

    /* reassign const char* to different literal */
    msg = "hello";
    printf("%s\n", msg);
    // EXPECT: hello

    /* first_elem with const int *const param */
    int fe[3] = {55, 66, 77};
    printf("%d\n", first_elem(fe));
    // EXPECT: 55

    /* const pointer to array element mid-array */
    int mid[6] = {1, 2, 3, 4, 5, 6};
    const int *midp = mid + 2;
    printf("%d\n", *midp);
    // EXPECT: 3
    printf("%d\n", *(midp + 1));
    // EXPECT: 4
    printf("%d\n", midp[2]);
    // EXPECT: 5

    /* const pointer in loop (pointer advances, not const pointer itself) */
    const int *it = data;
    const int *it_end = data + 4;
    int it_sum = 0;
    while (it < it_end) {
        it_sum += *it;
        it++;
    }
    /* 3+6+9+12 = 30 */
    printf("%d\n", it_sum);
    // EXPECT: 30

    /* pointer comparison with const */
    const int *pa = data;
    const int *pb = data + 3;
    if (pa < pb) printf("pa<pb\n");
    // EXPECT: pa<pb
    int diff = (int)(pb - pa);
    printf("%d\n", diff);
    // EXPECT: 3

    /* const string array traversal */
    const char *strs[3] = {"abc", "de", "fghi"};
    int total_len = 0;
    int i;
    for (i = 0; i < 3; i++) total_len += my_strlen_const(strs[i]);
    /* 3 + 2 + 4 = 9 */
    printf("%d\n", total_len);
    // EXPECT: 9

    return 0;
}
