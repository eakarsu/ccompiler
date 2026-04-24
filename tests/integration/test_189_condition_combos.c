int printf(const char *fmt, ...);
// EXPECT: Between tests:\n0 1 2 3 4 5 6 7 8 9 10 \nQuadrant classification:\n(0,0)->0\n(3,4)->1\n(-2,5)->2\n(-3,-1)->3\n(7,-2)->4\n(0,5)->5\n(4,0)->6\n(-1,0)->6\n(0,-3)->5\n(1,1)->1\nLeap years: 1900:0 2000:1 2024:1 2023:0 1600:1 1700:0 2100:0 2400:1 \nComplex boolean table:\na=0 b=0: and=0 or=0 nand=1 nor=1 xor=0\na=0 b=1: and=0 or=1 nand=1 nor=0 xor=1\na=1 b=0: and=0 or=1 nand=1 nor=0 xor=1\na=1 b=1: and=1 or=1 nand=0 nor=0 xor=0\nCondition cascade:\nFB 1 2 F 4 B F 7 8 F B 11 F 13 14 FB 16 17 F 19 \nMulti-range: 5 6 7 8 9 15 16 17 18 19 25 26 27 28 29 
int between(int x, int lo, int hi) {
    return x >= lo && x <= hi;
}

int classify_point(int x, int y) {
    if (x == 0 && y == 0) return 0;
    if (x > 0 && y > 0) return 1;
    if (x < 0 && y > 0) return 2;
    if (x < 0 && y < 0) return 3;
    if (x > 0 && y < 0) return 4;
    if (x == 0) return 5;
    return 6;
}

int leap_year(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int main(void) {
    int i;
    int j;

    printf("Between tests:\n");
    for (i = -2; i <= 12; i++) {
        if (between(i, 0, 10)) {
            printf("%d ", i);
        }
    }
    printf("\n");

    printf("Quadrant classification:\n");
    int points[10][2];
    points[0][0] = 0; points[0][1] = 0;
    points[1][0] = 3; points[1][1] = 4;
    points[2][0] = -2; points[2][1] = 5;
    points[3][0] = -3; points[3][1] = -1;
    points[4][0] = 7; points[4][1] = -2;
    points[5][0] = 0; points[5][1] = 5;
    points[6][0] = 4; points[6][1] = 0;
    points[7][0] = -1; points[7][1] = 0;
    points[8][0] = 0; points[8][1] = -3;
    points[9][0] = 1; points[9][1] = 1;
    for (i = 0; i < 10; i++) {
        printf("(%d,%d)->%d\n", points[i][0], points[i][1],
               classify_point(points[i][0], points[i][1]));
    }

    printf("Leap years: ");
    int years[8];
    years[0] = 1900; years[1] = 2000; years[2] = 2024;
    years[3] = 2023; years[4] = 1600; years[5] = 1700;
    years[6] = 2100; years[7] = 2400;
    for (i = 0; i < 8; i++) {
        printf("%d:%d ", years[i], leap_year(years[i]));
    }
    printf("\n");

    printf("Complex boolean table:\n");
    for (i = 0; i < 4; i++) {
        int a = i / 2;
        int b = i % 2;
        printf("a=%d b=%d: and=%d or=%d nand=%d nor=%d xor=%d\n",
               a, b, a && b, a || b, !(a && b), !(a || b), (a || b) && !(a && b));
    }

    printf("Condition cascade:\n");
    for (i = 0; i < 20; i++) {
        int fizz = (i % 3 == 0);
        int buzz = (i % 5 == 0);
        if (fizz && buzz) {
            printf("FB ");
        } else if (fizz) {
            printf("F ");
        } else if (buzz) {
            printf("B ");
        } else {
            printf("%d ", i);
        }
    }
    printf("\n");

    printf("Multi-range: ");
    for (i = 0; i < 30; i++) {
        if ((i >= 5 && i < 10) || (i >= 15 && i < 20) || (i >= 25 && i < 30)) {
            printf("%d ", i);
        }
    }
    printf("\n");

    return 0;
}
