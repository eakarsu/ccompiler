int printf(const char *fmt, ...);

/* Predefined macros: __LINE__, __func__, __STDC__ */

/* __STDC__ must be 1 for a conforming C implementation */
/* __LINE__ expands to current line number (integer) */
/* __func__ expands to current function name (string) */

/* Helper macros using __LINE__ */
#define LINE_HERE   __LINE__
#define LINE_DIFF(a, b)  ((b) - (a))

/* Helper that captures __LINE__ at call site */
#define GET_LINE()  __LINE__

/* Macro combining __LINE__ with a base */
#define TAG(base)   ((base) + __LINE__)

int get_line_in_func(void) {
    return __LINE__;           /* line 22 */
}

int double_val(int x) {
    return x * 2;
}

int add_vals(int a, int b) {
    return a + b;
}

void print_funcname(void) {
    printf("%s\n", __func__);  /* prints: print_funcname */
}

void helper_a(void) {
    printf("%s\n", __func__);  /* prints: helper_a */
}

void helper_b(void) {
    printf("%s\n", __func__);  /* prints: helper_b */
}

void helper_c(void) {
    printf("%s\n", __func__);  /* prints: helper_c */
}

int line_reporter(void) {
    int l = __LINE__;
    return l;
}

int main(void) {
    /* __STDC__ is 1 for a conforming C compiler */
    printf("%d\n", __STDC__);            // EXPECT: 1

    /* __func__ in main */
    printf("%s\n", __func__);            // EXPECT: main

    /* __func__ in other functions */
    print_funcname();                    // EXPECT: print_funcname
    helper_a();                          // EXPECT: helper_a
    helper_b();                          // EXPECT: helper_b
    helper_c();                          // EXPECT: helper_c

    /* __LINE__: capture two adjacent lines, difference must be 1 */
    int line_a = __LINE__;
    int line_b = __LINE__;
    printf("%d\n", line_b - line_a);     // EXPECT: 1

    /* Capture three lines, differences */
    int l1 = __LINE__;
    int l2 = __LINE__;
    int l3 = __LINE__;
    printf("%d\n", l2 - l1);            // EXPECT: 1
    printf("%d\n", l3 - l2);            // EXPECT: 1
    printf("%d\n", l3 - l1);            // EXPECT: 2

    /* __LINE__ must be positive */
    int cur = __LINE__;
    printf("%d\n", cur > 0 ? 1 : 0);    // EXPECT: 1

    /* Two __LINE__ uses on separate lines differ by 1 */
    int before = __LINE__;
    int after  = __LINE__;
    printf("%d\n", after - before);      // EXPECT: 1

    /* Use LINE_HERE macro twice on consecutive lines */
    int mline1 = LINE_HERE;
    int mline2 = LINE_HERE;
    printf("%d\n", mline2 - mline1);     // EXPECT: 1

    /* GET_LINE() captures the line where it's written */
    int gl1 = GET_LINE();
    int gl2 = GET_LINE();
    printf("%d\n", gl2 - gl1);          // EXPECT: 1

    /* __LINE__ inside an expression: skipping a blank line below changes nothing */
    int la = __LINE__;

    int lb = __LINE__;
    /* lb - la = 2 because one blank line is between them */
    printf("%d\n", lb - la);            // EXPECT: 2

    /* TAG macro: adds __LINE__ to base */
    int base = 1000;
    int tagged_a = TAG(base);
    int tagged_b = TAG(base);
    /* tagged_b - tagged_a = line(tagged_b) - line(tagged_a) = 1 */
    printf("%d\n", tagged_b - tagged_a); // EXPECT: 1

    /* __LINE__ is monotonically increasing */
    int seq1 = __LINE__;
    int seq2 = __LINE__;
    int seq3 = __LINE__;
    int seq4 = __LINE__;
    printf("%d\n", seq2 > seq1 ? 1 : 0); // EXPECT: 1
    printf("%d\n", seq3 > seq2 ? 1 : 0); // EXPECT: 1
    printf("%d\n", seq4 > seq3 ? 1 : 0); // EXPECT: 1

    /* __func__ doesn't change within main */
    printf("%s\n", __func__);            // EXPECT: main
    printf("%s\n", __func__);            // EXPECT: main

    /* Verify line_reporter returns its own line (just verify positive) */
    int lr = line_reporter();
    printf("%d\n", lr > 0 ? 1 : 0);     // EXPECT: 1

    /* Use __func__ in an expression context */
    const char *fn = __func__;
    printf("%s\n", fn);                  // EXPECT: main

    /* get_line_in_func returns a line number (just verify > 0) */
    int gl = get_line_in_func();
    printf("%d\n", gl > 0 ? 1 : 0);     // EXPECT: 1

    /* __STDC__ is still 1 at end of main */
    printf("%d\n", __STDC__);            // EXPECT: 1

    return 0;
}
