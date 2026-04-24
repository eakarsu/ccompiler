int printf(const char *fmt, ...);

int main(void) {
    /* Comma operator in expression: evaluates left, discards, yields right */
    int a = (1, 2, 3);
    printf("%d\n", a);              // EXPECT: 3
    /* comma expr: 1 evaluated (discarded), 2 evaluated (discarded), 3 is result */

    int b = (10 + 5, 7 * 8);
    printf("%d\n", b);              // EXPECT: 56
    /* 10+5=15 (discarded), 7*8=56 is result */

    /* Comma with side effects */
    int x = 0, y = 0;
    int c = (x = 5, y = 10, x + y);
    printf("%d\n", c);              // EXPECT: 15
    printf("%d\n", x);              // EXPECT: 5
    printf("%d\n", y);              // EXPECT: 10

    /* Comma in for loop: multiple init expressions */
    int sum = 0;
    int i, j;
    for (i = 0, j = 10; i < 5; i++, j--) {
        sum += i + j;
    }
    /* i=0,j=10: 0+10=10
       i=1,j=9:  1+9=10
       i=2,j=8:  2+8=10
       i=3,j=7:  3+7=10
       i=4,j=6:  4+6=10
       sum = 50 */
    printf("%d\n", sum);            // EXPECT: 50
    printf("%d\n", i);              // EXPECT: 5
    printf("%d\n", j);              // EXPECT: 5

    /* Comma in for loop: increment two vars */
    int p = 0, q = 0;
    for (int k = 0; k < 4; k++, p += 2, q += 3) {
        /* loop body empty */
    }
    /* Each iteration: p += 2, q += 3
       k=0->1: p=2, q=3
       k=1->2: p=4, q=6
       k=2->3: p=6, q=9
       k=3->4: p=8, q=12
       loop ends when k=4 */
    printf("%d\n", p);              // EXPECT: 8
    printf("%d\n", q);              // EXPECT: 12

    /* Comma as sequence point: left fully evaluated before right */
    int m = 0;
    int n = 0;
    int r = (m++, n = m * 2);
    /* m++ post-increments: m was 0, then becomes 1. n = 1 * 2 = 2 */
    printf("%d\n", r);              // EXPECT: 2
    printf("%d\n", m);              // EXPECT: 1
    printf("%d\n", n);              // EXPECT: 2

    /* Comma operator in while loop update */
    int s = 0, t = 0;
    int loop_count = 0;
    while (loop_count < 3) {
        s += loop_count, t += loop_count * loop_count;
        loop_count++;
    }
    /* loop_count=0: s+=0, t+=0 -> s=0, t=0
       loop_count=1: s+=1, t+=1 -> s=1, t=1
       loop_count=2: s+=2, t+=4 -> s=3, t=5 */
    printf("%d\n", s);              // EXPECT: 3
    printf("%d\n", t);              // EXPECT: 5

    /* Comma in assignment: assigned value is from right of comma */
    int v1, v2;
    int assigned = (v1 = 7, v2 = v1 * 3, v2 - v1);
    /* v1=7, v2=21, result=21-7=14 */
    printf("%d\n", assigned);       // EXPECT: 14
    printf("%d\n", v1);             // EXPECT: 7
    printf("%d\n", v2);             // EXPECT: 21

    /* Comma with function calls */
    int arr[5];
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    arr[3] = 4;
    arr[4] = 5;

    /* Comma to compute index and fetch in one expr */
    int idx = 0;
    int elem = (idx = 3, arr[idx]);
    printf("%d\n", elem);           // EXPECT: 4
    printf("%d\n", idx);            // EXPECT: 3

    /* Nested comma: (a, (b, c)) */
    int na = 0, nb = 0, nc = 0;
    int nested = (na = 1, (nb = 2, nc = na + nb));
    printf("%d\n", nested);         // EXPECT: 3
    printf("%d\n", na);             // EXPECT: 1
    printf("%d\n", nb);             // EXPECT: 2
    printf("%d\n", nc);             // EXPECT: 3

    /* Comma in for: multiple decrements */
    int countdown = 10;
    int steps = 0;
    for (; countdown > 0; countdown -= 2, steps++) {
        /* empty */
    }
    /* countdown: 10,8,6,4,2 -> steps=5, then countdown=0 */
    printf("%d\n", countdown);      // EXPECT: 0
    printf("%d\n", steps);          // EXPECT: 5

    /* Variable declarations with comma: NOT comma operator, just multi-decl */
    int d1 = 10, d2 = 20, d3 = 30;
    printf("%d\n", d1 + d2 + d3);   // EXPECT: 60

    /* Comma in printf argument list (not comma operator - just multiple args) */
    int pa = 5, pb = 10;
    printf("%d %d\n", pa, pb);      // EXPECT: 5 10

    /* Comma operator produces value but not lvalue */
    int lv1 = 3, lv2 = 7;
    int result = (lv1 * lv1, lv2 * lv2);
    printf("%d\n", result);         // EXPECT: 49
    /* 9 discarded, 49 is result */

    /* Comma in conditional context */
    int flag = 1;
    int outcome = flag ? (printf("yes\n"), 100) : (printf("no\n"), 200);
    /* flag=1 so first branch: prints "yes", yields 100 */
    // EXPECT: yes
    printf("%d\n", outcome);        // EXPECT: 100

    return 0;
}
