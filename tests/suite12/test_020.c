int printf(const char *fmt, ...);

/* Test 020: Flood Fill Region
   Flood fill a region of a 8x8 image starting from a seed point.
   Similar to paint bucket tool in image editors.
*/

int img[8][8];

void clear_image(void) {
    int i;
    int j;
    for (i = 0; i < 8; i = i + 1)
        for (j = 0; j < 8; j = j + 1)
            img[i][j] = 0;
}

void draw_rect(int r1, int c1, int r2, int c2, int val) {
    int i;
    int j;
    for (i = r1; i <= r2; i = i + 1) {
        for (j = c1; j <= c2; j = j + 1) {
            img[i][j] = val;
        }
    }
}

void draw_border(int r1, int c1, int r2, int c2, int val) {
    int i;
    for (i = c1; i <= c2; i = i + 1) {
        img[r1][i] = val;
        img[r2][i] = val;
    }
    for (i = r1; i <= r2; i = i + 1) {
        img[i][c1] = val;
        img[i][c2] = val;
    }
}

int flood_fill(int sr, int sc, int old_val, int new_val) {
    int stack_r[64];
    int stack_c[64];
    int sp;
    int cr;
    int cc;
    int nr;
    int nc;
    int d;
    int dr[4];
    int dc[4];
    int filled;

    dr[0] = -1; dr[1] = 1; dr[2] = 0; dr[3] = 0;
    dc[0] = 0;  dc[1] = 0; dc[2] = -1; dc[3] = 1;

    if (old_val == new_val) return 0;
    if (img[sr][sc] != old_val) return 0;

    sp = 0;
    filled = 0;
    stack_r[sp] = sr;
    stack_c[sp] = sc;
    sp = sp + 1;
    img[sr][sc] = new_val;
    filled = filled + 1;

    while (sp > 0) {
        sp = sp - 1;
        cr = stack_r[sp];
        cc = stack_c[sp];

        for (d = 0; d < 4; d = d + 1) {
            nr = cr + dr[d];
            nc = cc + dc[d];
            if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                if (img[nr][nc] == old_val) {
                    img[nr][nc] = new_val;
                    filled = filled + 1;
                    stack_r[sp] = nr;
                    stack_c[sp] = nc;
                    sp = sp + 1;
                }
            }
        }
    }
    return filled;
}

int count_val(int val) {
    int i;
    int j;
    int c;
    c = 0;
    for (i = 0; i < 8; i = i + 1)
        for (j = 0; j < 8; j = j + 1)
            if (img[i][j] == val) c = c + 1;
    return c;
}

void test_simple_fill(void) {
    int filled;
    clear_image();
    filled = flood_fill(3, 3, 0, 5);
    printf("simple_filled=%d\n", filled);  // EXPECT: simple_filled=64
    printf("simple_count5=%d\n", count_val(5));  // EXPECT: simple_count5=64
}

void test_bounded_fill(void) {
    int filled;
    clear_image();
    draw_border(1, 1, 5, 5, 9);
    filled = flood_fill(3, 3, 0, 7);
    printf("bounded_filled=%d\n", filled);  // EXPECT: bounded_filled=9
    printf("bounded_count7=%d\n", count_val(7));  // EXPECT: bounded_count7=9
    printf("border_intact=%d\n", count_val(9));  // EXPECT: border_intact=16
}

void test_fill_outside(void) {
    int filled;
    clear_image();
    draw_border(2, 2, 5, 5, 8);
    filled = flood_fill(0, 0, 0, 3);
    printf("outside_filled=%d\n", filled);  // EXPECT: outside_filled=48
    printf("inside_still_0=%d\n", count_val(0));  // EXPECT: inside_still_0=4
    printf("inside_val=%d\n", img[3][3]);  // EXPECT: inside_val=0
}

void test_no_fill(void) {
    int filled;
    clear_image();
    img[3][3] = 5;
    filled = flood_fill(3, 3, 0, 2);
    printf("no_fill=%d\n", filled);  // EXPECT: no_fill=0
}

void test_two_rooms(void) {
    int f1;
    int f2;
    clear_image();
    draw_border(0, 0, 7, 3, 1);
    draw_border(0, 4, 7, 7, 1);
    f1 = flood_fill(1, 1, 0, 2);
    f2 = flood_fill(1, 5, 0, 3);
    printf("room1=%d\n", f1);  // EXPECT: room1=12
    printf("room2=%d\n", f2);  // EXPECT: room2=12
    printf("walls=%d\n", count_val(1));  // EXPECT: walls=40
}

int main(void) {
    test_simple_fill();
    test_bounded_fill();
    test_fill_outside();
    test_no_fill();
    test_two_rooms();
    printf("test_020 passed\n");  // EXPECT: test_020 passed
    return 0;
}
