int printf(const char *fmt, ...);

struct Point {
    int x;
    int y;
};

struct Triple {
    int a;
    int b;
    int c;
};

struct Mixed {
    char c1;
    char c2;
    char c3;
    char c4;
    int val;
};

int main(void) {
    struct Point pt;
    pt.x = 0x01020304;
    pt.y = 0x05060708;

    char *bp = (char *)&pt;

    // Little-endian: x = 0x01020304, first byte = 0x04
    // EXPECT: pt_b0 = 4
    printf("pt_b0 = %d\n", (int)bp[0]);

    // EXPECT: pt_b1 = 3
    printf("pt_b1 = %d\n", (int)bp[1]);

    // EXPECT: pt_b2 = 2
    printf("pt_b2 = %d\n", (int)bp[2]);

    // EXPECT: pt_b3 = 1
    printf("pt_b3 = %d\n", (int)bp[3]);

    // y starts at offset 4
    // EXPECT: pt_b4 = 8
    printf("pt_b4 = %d\n", (int)bp[4]);

    // EXPECT: pt_b5 = 7
    printf("pt_b5 = %d\n", (int)bp[5]);

    // EXPECT: pt_b6 = 6
    printf("pt_b6 = %d\n", (int)bp[6]);

    // EXPECT: pt_b7 = 5
    printf("pt_b7 = %d\n", (int)bp[7]);

    // Cast back to struct pointer
    struct Point *pp = (struct Point *)bp;
    // EXPECT: cast_back_x = 16909060
    printf("cast_back_x = %d\n", pp->x);

    // EXPECT: cast_back_y = 84281096
    printf("cast_back_y = %d\n", pp->y);

    // Triple struct
    struct Triple tr;
    tr.a = 10;
    tr.b = 20;
    tr.c = 30;
    char *tp = (char *)&tr;

    // Read each int via char pointer
    int *ip0 = (int *)(tp + 0);
    int *ip1 = (int *)(tp + 4);
    int *ip2 = (int *)(tp + 8);
    // EXPECT: triple_a = 10
    printf("triple_a = %d\n", *ip0);

    // EXPECT: triple_b = 20
    printf("triple_b = %d\n", *ip1);

    // EXPECT: triple_c = 30
    printf("triple_c = %d\n", *ip2);

    // Modify struct through char pointer
    char *mp = (char *)&tr;
    int *mod_b = (int *)(mp + 4);
    *mod_b = 99;
    // EXPECT: modified_b = 99
    printf("modified_b = %d\n", tr.b);

    // Mixed struct: chars + int
    struct Mixed mx;
    mx.c1 = 65;
    mx.c2 = 66;
    mx.c3 = 67;
    mx.c4 = 68;
    mx.val = 12345;

    char *mxp = (char *)&mx;
    // EXPECT: mx_c1 = 65
    printf("mx_c1 = %d\n", (int)mxp[0]);

    // EXPECT: mx_c2 = 66
    printf("mx_c2 = %d\n", (int)mxp[1]);

    // EXPECT: mx_c3 = 67
    printf("mx_c3 = %d\n", (int)mxp[2]);

    // EXPECT: mx_c4 = 68
    printf("mx_c4 = %d\n", (int)mxp[3]);

    // int member at offset 4 (packed after 4 chars)
    int *mxip = (int *)(mxp + 4);
    // EXPECT: mx_val = 12345
    printf("mx_val = %d\n", *mxip);

    // Copy struct via char pointers
    struct Point src;
    src.x = 111;
    src.y = 222;
    struct Point dst;
    dst.x = 0;
    dst.y = 0;

    char *sp = (char *)&src;
    char *dp = (char *)&dst;
    int k;
    for (k = 0; k < 8; k++) {
        dp[k] = sp[k];
    }
    // EXPECT: copied_x = 111
    printf("copied_x = %d\n", dst.x);

    // EXPECT: copied_y = 222
    printf("copied_y = %d\n", dst.y);

    // Compare structs byte by byte
    int equal = 1;
    for (k = 0; k < 8; k++) {
        if (sp[k] != dp[k]) equal = 0;
    }
    // EXPECT: structs_equal = 1
    printf("structs_equal = %d\n", equal);

    return 0;
}
