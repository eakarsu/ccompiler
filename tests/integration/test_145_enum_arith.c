int printf(const char *fmt, ...);
// EXPECT: colors: R=0 G=1 B=2 Y=3 C=4 M=5 W=6\ndays: M=1 T=2 W=3 T=4 F=5 S=6 S=7\nRED+GREEN = 1\nBLUE*2 = 4\nWHITE-RED = 6\nMON+4 = FRI? 1\nSAT-MON = 5\nweekend: 1:0 2:0 3:0 4:0 5:0 6:1 7:1 \nday types: 0 0 2 0 0 1 1 \nweekdays = 5\nprimary: 0 1 2 \nscores[RED]=10 scores[BLUE]=30 scores[WHITE]=70\nis warm: 1\nis cool: 0\ncolor idx: 3\nsum of color^2 = 91
// Test: enum values in arithmetic expressions

enum color { RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE };
enum day { MON = 1, TUE, WED, THU, FRI, SAT, SUN };

int is_weekend(int d) {
    return d == SAT || d == SUN;
}

int is_primary(int c) {
    return c == RED || c == GREEN || c == BLUE;
}

int day_type(int d) {
    /* 0=weekday, 1=weekend, 2=wednesday(hump day) */
    if (d == WED) return 2;
    if (is_weekend(d)) return 1;
    return 0;
}

int main(void) {
    int i;
    int count;

    /* Enum values */
    printf("colors: R=%d G=%d B=%d Y=%d C=%d M=%d W=%d\n",
           RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE);

    printf("days: M=%d T=%d W=%d T=%d F=%d S=%d S=%d\n",
           MON, TUE, WED, THU, FRI, SAT, SUN);

    /* Arithmetic with enum values */
    printf("RED+GREEN = %d\n", RED + GREEN);
    printf("BLUE*2 = %d\n", BLUE * 2);
    printf("WHITE-RED = %d\n", WHITE - RED);

    /* Day arithmetic */
    printf("MON+4 = FRI? %d\n", (MON + 4) == FRI);
    printf("SAT-MON = %d\n", SAT - MON);

    /* Enum in conditions */
    printf("weekend: ");
    for (i = MON; i <= SUN; i++) {
        printf("%d:%d ", i, is_weekend(i));
    }
    printf("\n");

    /* Day type classification */
    printf("day types: ");
    for (i = MON; i <= SUN; i++) {
        printf("%d ", day_type(i));
    }
    printf("\n");

    /* Count weekdays */
    count = 0;
    for (i = MON; i <= SUN; i++) {
        if (!is_weekend(i)) count++;
    }
    printf("weekdays = %d\n", count);

    /* Primary colors */
    printf("primary: ");
    for (i = RED; i <= WHITE; i++) {
        if (is_primary(i)) {
            printf("%d ", i);
        }
    }
    printf("\n");

    /* Enum in array index */
    {
        int scores[7];
        for (i = 0; i < 7; i++) {
            scores[i] = (i + 1) * 10;
        }
        printf("scores[RED]=%d scores[BLUE]=%d scores[WHITE]=%d\n",
               scores[RED], scores[BLUE], scores[WHITE]);
    }

    /* Enum comparison chain */
    {
        int c;
        c = YELLOW;
        printf("is warm: %d\n", c == RED || c == YELLOW);
        printf("is cool: %d\n", c == BLUE || c == CYAN || c == GREEN);
        printf("color idx: %d\n", c);
    }

    /* Iterate and compute */
    {
        int sum;
        sum = 0;
        for (i = RED; i <= WHITE; i++) {
            sum += i * i;
        }
        printf("sum of color^2 = %d\n", sum);
    }

    return 0;
}
