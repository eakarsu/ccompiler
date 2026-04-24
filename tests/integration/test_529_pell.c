int printf(const char *fmt, ...);
// EXPECT: Pell numbers:\nP(0) = 0\nP(1) = 1\nP(2) = 2\nP(3) = 5\nP(4) = 12\nP(5) = 29\nP(6) = 70\nP(7) = 169\nP(8) = 408\nP(9) = 985\nP(10) = 2378\nP(11) = 5741\nP(12) = 13860\nP(13) = 33461\nP(14) = 80782\nP(15) = 195025\nP(16) = 470832\nP(17) = 1136689\nP(18) = 2744210\nP(19) = 6625109\nP(20) = 15994428\nP(21) = 38613965\n\nCompanion Pell numbers:\nQ(0) = 2\nQ(1) = 2\nQ(2) = 6\nQ(3) = 14\nQ(4) = 34\nQ(5) = 82\nQ(6) = 198\nQ(7) = 478\nQ(8) = 1154\nQ(9) = 2786\nQ(10) = 6726\nQ(11) = 16238\nQ(12) = 39202\nQ(13) = 94642\nQ(14) = 228486\n\nPell ratios (x1000):\nP(2)/P(1) = 2000/1000\nP(3)/P(2) = 2500/1000\nP(4)/P(3) = 2400/1000\nP(5)/P(4) = 2416/1000\nP(6)/P(5) = 2413/1000\nP(7)/P(6) = 2414/1000\nP(8)/P(7) = 2414/1000\nP(9)/P(8) = 2414/1000\nP(10)/P(9) = 2414/1000\nP(11)/P(10) = 2414/1000\nP(12)/P(11) = 2414/1000\nP(13)/P(12) = 2414/1000\nP(14)/P(13) = 2414/1000\n\nPell identity P(n+1)*P(n-1) - P(n)^2 = (-1)^n:\nn=1: -1 (expected -1)\nn=2: 1 (expected 1)\nn=3: -1 (expected -1)\nn=4: 1 (expected 1)\nn=5: -1 (expected -1)\nn=6: 1 (expected 1)\nn=7: -1 (expected -1)\nn=8: 1 (expected 1)\nn=9: -1 (expected -1)\nn=10: 1 (expected 1)\nn=11: -1 (expected -1)\n\nPell equation x^2 - 2*y^2:\nx=1, y=1: x^2 - 2y^2 = -1\nx=3, y=2: x^2 - 2y^2 = 1\nx=7, y=5: x^2 - 2y^2 = -1\nx=17, y=12: x^2 - 2y^2 = 1\nx=41, y=29: x^2 - 2y^2 = -1\nx=99, y=70: x^2 - 2y^2 = 1\nx=239, y=169: x^2 - 2y^2 = -1\nx=577, y=408: x^2 - 2y^2 = 1\nx=1393, y=985: x^2 - 2y^2 = -1\n\nPartial sums:\nSum P(0..0) = 0\nSum P(0..1) = 1\nSum P(0..2) = 3\nSum P(0..3) = 8\nSum P(0..4) = 20\nSum P(0..5) = 49\nSum P(0..6) = 119\nSum P(0..7) = 288\nSum P(0..8) = 696\nSum P(0..9) = 1681\nSum P(0..10) = 4059\nSum P(0..11) = 9800
// Test: Pell numbers

int main(void) {
    int pell[25];
    int companion[25];
    int i;

    // Pell numbers: P(0)=0, P(1)=1, P(n) = 2*P(n-1) + P(n-2)
    pell[0] = 0;
    pell[1] = 1;
    for (i = 2; i < 22; i++) {
        pell[i] = 2 * pell[i - 1] + pell[i - 2];
    }

    printf("Pell numbers:\n");
    for (i = 0; i < 22; i++) {
        printf("P(%d) = %d\n", i, pell[i]);
    }

    // Companion Pell numbers (Pell-Lucas): Q(0)=1, Q(1)=1, Q(n) = 2*Q(n-1) + Q(n-2)
    // Actually: Q(0)=2, Q(1)=2
    companion[0] = 2;
    companion[1] = 2;
    for (i = 2; i < 15; i++) {
        companion[i] = 2 * companion[i - 1] + companion[i - 2];
    }

    printf("\nCompanion Pell numbers:\n");
    for (i = 0; i < 15; i++) {
        printf("Q(%d) = %d\n", i, companion[i]);
    }

    // Ratio P(n+1)/P(n) approaches 1 + sqrt(2) ~ 2.414
    printf("\nPell ratios (x1000):\n");
    for (i = 2; i < 15; i++) {
        int ratio = pell[i] * 1000 / pell[i - 1];
        printf("P(%d)/P(%d) = %d/1000\n", i, i - 1, ratio);
    }

    // Pell identity: P(n)^2 - 2*P(n-1)^2... not quite
    // Actually: P(n+1)*P(n-1) - P(n)^2 = (-1)^n
    printf("\nPell identity P(n+1)*P(n-1) - P(n)^2 = (-1)^n:\n");
    for (i = 1; i < 12; i++) {
        int val = pell[i + 1] * pell[i - 1] - pell[i] * pell[i];
        int expected = (i % 2 == 0) ? 1 : -1;
        printf("n=%d: %d (expected %d)\n", i, val, expected);
    }

    // Connection to sqrt(2): P(n)/P(n-1) converges
    // Also: Q(n) = 2*P(n) + Q(n-1)... nah
    // Half-companion: H(n) = P(n) + P(n-1), approximates sqrt(2)*P(n)

    // Pell equation solutions: x^2 - 2*y^2 = +-1
    // (P(n), P(n-1)) give solutions when adjusted
    printf("\nPell equation x^2 - 2*y^2:\n");
    for (i = 1; i < 10; i++) {
        // x = P(i) + P(i-1), y = P(i)
        int x = pell[i] + pell[i - 1];
        int y = pell[i];
        int val = x * x - 2 * y * y;
        printf("x=%d, y=%d: x^2 - 2y^2 = %d\n", x, y, val);
    }

    // Sum of Pell numbers
    printf("\nPartial sums:\n");
    int psum = 0;
    for (i = 0; i < 12; i++) {
        psum = psum + pell[i];
        printf("Sum P(0..%d) = %d\n", i, psum);
    }

    return 0;
}
