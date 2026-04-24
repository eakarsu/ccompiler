int printf(const char *fmt, ...);
// EXPECT: Tribonacci numbers:\nT(0) = 0\nT(1) = 0\nT(2) = 1\nT(3) = 1\nT(4) = 2\nT(5) = 4\nT(6) = 7\nT(7) = 13\nT(8) = 24\nT(9) = 44\nT(10) = 81\nT(11) = 149\nT(12) = 274\nT(13) = 504\nT(14) = 927\nT(15) = 1705\nT(16) = 3136\nT(17) = 5768\nT(18) = 10609\nT(19) = 19513\nT(20) = 35890\nT(21) = 66012\nT(22) = 121415\nT(23) = 223317\nT(24) = 410744\n\nTribonacci ratios (x1000):\nT(4)/T(3) = 2000/1000\nT(5)/T(4) = 2000/1000\nT(6)/T(5) = 1750/1000\nT(7)/T(6) = 1857/1000\nT(8)/T(7) = 1846/1000\nT(9)/T(8) = 1833/1000\nT(10)/T(9) = 1840/1000\nT(11)/T(10) = 1839/1000\nT(12)/T(11) = 1838/1000\nT(13)/T(12) = 1839/1000\nT(14)/T(13) = 1839/1000\nT(15)/T(14) = 1839/1000\nT(16)/T(15) = 1839/1000\nT(17)/T(16) = 1839/1000\nT(18)/T(17) = 1839/1000\nT(19)/T(18) = 1839/1000\n\nTetranacci numbers:\nTe(0) = 0\nTe(1) = 0\nTe(2) = 0\nTe(3) = 1\nTe(4) = 1\nTe(5) = 2\nTe(6) = 4\nTe(7) = 8\nTe(8) = 15\nTe(9) = 29\nTe(10) = 56\nTe(11) = 108\nTe(12) = 208\nTe(13) = 401\nTe(14) = 773\nTe(15) = 1490\nTe(16) = 2872\nTe(17) = 5536\nTe(18) = 10671\nTe(19) = 20569\n\nPartial sums of Tribonacci:\nSum T(0..0) = 0\nSum T(0..1) = 0\nSum T(0..2) = 1\nSum T(0..3) = 2\nSum T(0..4) = 4\nSum T(0..5) = 8\nSum T(0..6) = 15\nSum T(0..7) = 28\nSum T(0..8) = 52\nSum T(0..9) = 96\nSum T(0..10) = 177\nSum T(0..11) = 326\nSum T(0..12) = 600\nSum T(0..13) = 1104\nSum T(0..14) = 2031\n\nComparison at same index:\nn    Fib        Trib       Tetra     \n0    0          0          0         \n1    1          0          0         \n2    1          1          0         \n3    2          1          1         \n4    3          2          1         \n5    5          4          2         \n6    8          7          4         \n7    13         13         8         \n8    21         24         15        \n9    34         44         29        \n10   55         81         56        \n11   89         149        108       \n12   144        274        208       \n13   233        504        401       \n14   377        927        773       \n\nTribonacci-like sequence mod 3:\n0 0 1 1 2 1 1 1 0 2 0 2 1 0 0 1 1 2 1 1 
// Test: Tribonacci numbers

int main(void) {
    int trib[30];
    int i;

    // Tribonacci: T(0)=0, T(1)=0, T(2)=1, T(n)=T(n-1)+T(n-2)+T(n-3)
    trib[0] = 0;
    trib[1] = 0;
    trib[2] = 1;
    for (i = 3; i < 25; i++) {
        trib[i] = trib[i - 1] + trib[i - 2] + trib[i - 3];
    }

    printf("Tribonacci numbers:\n");
    for (i = 0; i < 25; i++) {
        printf("T(%d) = %d\n", i, trib[i]);
    }

    // Ratio approaches tribonacci constant ~ 1.839
    printf("\nTribonacci ratios (x1000):\n");
    for (i = 4; i < 20; i++) {
        if (trib[i - 1] > 0) {
            int ratio = trib[i] * 1000 / trib[i - 1];
            printf("T(%d)/T(%d) = %d/1000\n", i, i - 1, ratio);
        }
    }

    // Tetranacci: T(n) = T(n-1)+T(n-2)+T(n-3)+T(n-4)
    int tetra[25];
    tetra[0] = 0;
    tetra[1] = 0;
    tetra[2] = 0;
    tetra[3] = 1;
    for (i = 4; i < 20; i++) {
        tetra[i] = tetra[i - 1] + tetra[i - 2] + tetra[i - 3] + tetra[i - 4];
    }
    printf("\nTetranacci numbers:\n");
    for (i = 0; i < 20; i++) {
        printf("Te(%d) = %d\n", i, tetra[i]);
    }

    // Sum of first n tribonacci numbers
    printf("\nPartial sums of Tribonacci:\n");
    int tsum = 0;
    for (i = 0; i < 15; i++) {
        tsum = tsum + trib[i];
        printf("Sum T(0..%d) = %d\n", i, tsum);
    }

    // Compare Fibonacci, Tribonacci, Tetranacci at same index
    int fib[25];
    fib[0] = 0;
    fib[1] = 1;
    for (i = 2; i < 20; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    printf("\nComparison at same index:\n");
    printf("%-4s %-10s %-10s %-10s\n", "n", "Fib", "Trib", "Tetra");
    for (i = 0; i < 15; i++) {
        printf("%-4d %-10d %-10d %-10d\n", i, fib[i], trib[i], tetra[i]);
    }

    // Tribonacci word: substitute 1->12, 2->13, 3->1
    printf("\nTribonacci-like sequence mod 3:\n");
    for (i = 0; i < 20; i++) {
        printf("%d ", trib[i] % 3);
    }
    printf("\n");

    return 0;
}
