int printf(const char *fmt, ...);
// EXPECT: Deep if nesting:\n15: >0,<50,odd,<=20\n8: >0,<50,even,<=10\n23: >0,<50,odd,>20\n4: >0,<50,even,<=10\n42: >0,<50,even,>10\nNested loops with conditions:\nsum of products div by 7 (until >50)=343\nComplex index traversal:\npath_sum=130 steps=10\nNested while-for:\n100 factored: remaining=5 steps=3
int main(void) {
    int i;
    int j;

    printf("Deep if nesting:\n");
    int vals[5];
    vals[0] = 15; vals[1] = 8; vals[2] = 23; vals[3] = 4; vals[4] = 42;
    for (i = 0; i < 5; i++) {
        int v = vals[i];
        if (v > 0) {
            if (v < 50) {
                if (v % 2 == 0) {
                    if (v > 10) {
                        printf("%d: >0,<50,even,>10\n", v);
                    } else {
                        printf("%d: >0,<50,even,<=10\n", v);
                    }
                } else {
                    if (v > 20) {
                        printf("%d: >0,<50,odd,>20\n", v);
                    } else {
                        printf("%d: >0,<50,odd,<=20\n", v);
                    }
                }
            } else {
                printf("%d: >=50\n", v);
            }
        } else {
            printf("%d: <=0\n", v);
        }
    }

    printf("Nested loops with conditions:\n");
    int sum = 0;
    for (i = 1; i <= 10; i++) {
        for (j = 1; j <= 10; j++) {
            if (i * j > 50) {
                break;
            }
            if ((i * j) % 7 == 0) {
                sum = sum + i * j;
            }
        }
    }
    printf("sum of products div by 7 (until >50)=%d\n", sum);

    printf("Complex index traversal:\n");
    int grid[6][6];
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            grid[i][j] = i * 6 + j;
        }
    }
    int path_sum = 0;
    int x = 0;
    int y = 0;
    int steps = 0;
    while (x < 5 && y < 5) {
        path_sum = path_sum + grid[x][y];
        if (grid[x + 1][y] > grid[x][y + 1]) {
            y++;
        } else {
            x++;
        }
        steps++;
    }
    while (x < 5) {
        path_sum = path_sum + grid[x][y];
        x++;
        steps++;
    }
    while (y < 5) {
        path_sum = path_sum + grid[x][y];
        y++;
        steps++;
    }
    path_sum = path_sum + grid[5][5];
    printf("path_sum=%d steps=%d\n", path_sum, steps);

    printf("Nested while-for:\n");
    int n = 100;
    int count = 0;
    while (n > 1) {
        int divisor = 2;
        int found = 0;
        for (divisor = 2; divisor * divisor <= n; divisor++) {
            if (n % divisor == 0) {
                n = n / divisor;
                found = 1;
                break;
            }
        }
        if (!found) {
            break;
        }
        count++;
    }
    printf("100 factored: remaining=%d steps=%d\n", n, count);

    return 0;
}
