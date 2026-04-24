int printf(const char *fmt, ...);
// EXPECT: classify: -100:-2 -5:-1 -1:-1 0:0 1:1 5:1 10:1 11:2 100:2 \nfizzbuzz: 1 2 F 4 B F 7 8 F B 11 F 13 14 FB 16 17 F 19 B \nmed(1,2,3) = 2\nmed(3,1,2) = 2\nmed(2,3,1) = 2\nmed(5,5,5) = 5\nmed(1,1,2) = 1\nmed(9,3,7) = 7\nbounded(5,3,0,10) = 8\nbounded(8,5,0,10) = 10\nbounded(-3,-8,0,10) = 0\nrange = 6\n95=A  87=B+ 72=C- 63=D  45=F  91=A- 
// Test: nested ternary expression trees

int classify(int n) {
    /* Returns: 0=zero, 1=small pos, 2=large pos, -1=small neg, -2=large neg */
    return n == 0 ? 0 :
           n > 0 ? (n <= 10 ? 1 : 2) :
                   (n >= -10 ? -1 : -2);
}

int fizzbuzz_val(int n) {
    /* Returns: 3 for fizz, 5 for buzz, 15 for fizzbuzz, n otherwise */
    return (n % 15 == 0) ? 15 :
           (n % 3 == 0) ? 3 :
           (n % 5 == 0) ? 5 : n;
}

int median3(int a, int b, int c) {
    return a > b ?
           (b > c ? b : (a > c ? c : a)) :
           (a > c ? a : (b > c ? c : b));
}

int bounded_add(int a, int b, int lo, int hi) {
    int sum;
    sum = a + b;
    return sum < lo ? lo : (sum > hi ? hi : sum);
}

int main(void) {
    int i;

    /* Classification test */
    printf("classify: ");
    {
        int vals[9];
        vals[0] = -100; vals[1] = -5; vals[2] = -1;
        vals[3] = 0;
        vals[4] = 1; vals[5] = 5; vals[6] = 10;
        vals[7] = 11; vals[8] = 100;
        for (i = 0; i < 9; i++) {
            printf("%d:%d ", vals[i], classify(vals[i]));
        }
    }
    printf("\n");

    /* FizzBuzz via ternary */
    printf("fizzbuzz: ");
    for (i = 1; i <= 20; i++) {
        int v;
        v = fizzbuzz_val(i);
        if (v == 15) printf("FB ");
        else if (v == 3) printf("F ");
        else if (v == 5) printf("B ");
        else printf("%d ", v);
    }
    printf("\n");

    /* Median of three */
    printf("med(1,2,3) = %d\n", median3(1, 2, 3));
    printf("med(3,1,2) = %d\n", median3(3, 1, 2));
    printf("med(2,3,1) = %d\n", median3(2, 3, 1));
    printf("med(5,5,5) = %d\n", median3(5, 5, 5));
    printf("med(1,1,2) = %d\n", median3(1, 1, 2));
    printf("med(9,3,7) = %d\n", median3(9, 3, 7));

    /* Bounded addition */
    printf("bounded(5,3,0,10) = %d\n", bounded_add(5, 3, 0, 10));
    printf("bounded(8,5,0,10) = %d\n", bounded_add(8, 5, 0, 10));
    printf("bounded(-3,-8,0,10) = %d\n", bounded_add(-3, -8, 0, 10));

    /* Deeply nested: absolute difference of min and max */
    {
        int a, b, c;
        a = 7; b = 3; c = 9;
        printf("range = %d\n",
            (a > b ? (a > c ? a : c) : (b > c ? b : c)) -
            (a < b ? (a < c ? a : c) : (b < c ? b : c)));
    }

    /* Letter grade with + and - */
    for (i = 0; i < 6; i++) {
        int scores[6];
        char grade;
        int mod;
        scores[0] = 95; scores[1] = 87; scores[2] = 72;
        scores[3] = 63; scores[4] = 45; scores[5] = 91;
        grade = scores[i] >= 90 ? 'A' :
                scores[i] >= 80 ? 'B' :
                scores[i] >= 70 ? 'C' :
                scores[i] >= 60 ? 'D' : 'F';
        mod = scores[i] % 10;
        printf("%d=%c%c ", scores[i], grade,
               grade == 'F' ? ' ' :
               mod >= 7 ? '+' :
               mod < 3 ? '-' : ' ');
    }
    printf("\n");

    return 0;
}
