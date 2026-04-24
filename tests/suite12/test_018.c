int printf(const char *fmt, ...);

/* Test 018: Connected Component Labeling
   Label connected regions in a binary 8x8 image using
   iterative two-pass algorithm (simplified).
*/

int img[8][8];
int label[8][8];
int next_label;

void clear_all(void) {
    int i;
    int j;
    for (i = 0; i < 8; i = i + 1) {
        for (j = 0; j < 8; j = j + 1) {
            img[i][j] = 0;
            label[i][j] = 0;
        }
    }
    next_label = 1;
}

void init_image(void) {
    clear_all();
    img[0][0] = 1; img[0][1] = 1;
    img[1][0] = 1; img[1][1] = 1;

    img[0][5] = 1; img[0][6] = 1; img[0][7] = 1;
    img[1][5] = 1;

    img[4][2] = 1; img[4][3] = 1;
    img[5][2] = 1; img[5][3] = 1;
    img[6][2] = 1; img[6][3] = 1;

    img[6][6] = 1; img[7][6] = 1; img[7][7] = 1;
}

int min_val(int a, int b) {
    if (a < b) return a;
    return b;
}

void flood_fill_label(int si, int sj, int lbl) {
    int stack_i[64];
    int stack_j[64];
    int sp;
    int ci;
    int cj;
    int ni;
    int nj;
    int d;
    int di[4];
    int dj[4];

    di[0] = -1; di[1] = 1; di[2] = 0; di[3] = 0;
    dj[0] = 0;  dj[1] = 0; dj[2] = -1; dj[3] = 1;

    sp = 0;
    stack_i[sp] = si;
    stack_j[sp] = sj;
    sp = sp + 1;
    label[si][sj] = lbl;

    while (sp > 0) {
        sp = sp - 1;
        ci = stack_i[sp];
        cj = stack_j[sp];

        for (d = 0; d < 4; d = d + 1) {
            ni = ci + di[d];
            nj = cj + dj[d];
            if (ni >= 0 && ni < 8 && nj >= 0 && nj < 8) {
                if (img[ni][nj] == 1 && label[ni][nj] == 0) {
                    label[ni][nj] = lbl;
                    stack_i[sp] = ni;
                    stack_j[sp] = nj;
                    sp = sp + 1;
                }
            }
        }
    }
}

void do_labeling(void) {
    int i;
    int j;
    for (i = 0; i < 8; i = i + 1) {
        for (j = 0; j < 8; j = j + 1) {
            if (img[i][j] == 1 && label[i][j] == 0) {
                flood_fill_label(i, j, next_label);
                next_label = next_label + 1;
            }
        }
    }
}

int count_components(void) {
    return next_label - 1;
}

int component_size(int lbl) {
    int i;
    int j;
    int c;
    c = 0;
    for (i = 0; i < 8; i = i + 1)
        for (j = 0; j < 8; j = j + 1)
            if (label[i][j] == lbl) c = c + 1;
    return c;
}

int largest_component(void) {
    int lbl;
    int maxsz;
    int sz;
    int maxlbl;
    maxsz = 0;
    maxlbl = 0;
    for (lbl = 1; lbl < next_label; lbl = lbl + 1) {
        sz = component_size(lbl);
        if (sz > maxsz) {
            maxsz = sz;
            maxlbl = lbl;
        }
    }
    return maxlbl;
}

int main(void) {
    int nc;
    int lc;

    init_image();
    do_labeling();

    nc = count_components();
    printf("components=%d\n", nc);  // EXPECT: components=4

    printf("size1=%d\n", component_size(1));  // EXPECT: size1=4
    printf("size2=%d\n", component_size(2));  // EXPECT: size2=4
    printf("size3=%d\n", component_size(3));  // EXPECT: size3=6
    printf("size4=%d\n", component_size(4));  // EXPECT: size4=3

    lc = largest_component();
    printf("largest=%d\n", lc);  // EXPECT: largest=3

    printf("label_00=%d\n", label[0][0]);  // EXPECT: label_00=1
    printf("label_05=%d\n", label[0][5]);  // EXPECT: label_05=2
    printf("label_42=%d\n", label[4][2]);  // EXPECT: label_42=3
    printf("label_66=%d\n", label[6][6]);  // EXPECT: label_66=4
    printf("label_33=%d\n", label[3][3]);  // EXPECT: label_33=0

    printf("test_018 passed\n");  // EXPECT: test_018 passed
    return 0;
}
