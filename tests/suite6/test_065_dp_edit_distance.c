int printf(const char *fmt, ...);

int min2(int a, int b) {
    if (a < b) return a;
    return b;
}

int min3(int a, int b, int c) {
    return min2(min2(a, b), c);
}

int edit_distance(char a[], int alen, char b[], int blen) {
    int dp[30][30];
    int i = 0;
    while (i <= alen) {
        dp[i][0] = i;
        i = i + 1;
    }
    int j = 0;
    while (j <= blen) {
        dp[0][j] = j;
        j = j + 1;
    }
    i = 1;
    while (i <= alen) {
        j = 1;
        while (j <= blen) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + min3(dp[i - 1][j],
                                      dp[i][j - 1],
                                      dp[i - 1][j - 1]);
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return dp[alen][blen];
}

int edit_ops(char a[], int alen, char b[], int blen,
             int *inserts, int *deletes, int *replaces) {
    int dp[30][30];
    int op[30][30];
    int i = 0;
    while (i <= alen) {
        dp[i][0] = i;
        op[i][0] = 2;
        i = i + 1;
    }
    int j = 0;
    while (j <= blen) {
        dp[0][j] = j;
        op[0][j] = 1;
        j = j + 1;
    }
    op[0][0] = 0;

    i = 1;
    while (i <= alen) {
        j = 1;
        while (j <= blen) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
                op[i][j] = 0;
            } else {
                int del = dp[i - 1][j] + 1;
                int ins = dp[i][j - 1] + 1;
                int rep = dp[i - 1][j - 1] + 1;
                if (rep <= del && rep <= ins) {
                    dp[i][j] = rep;
                    op[i][j] = 3;
                } else if (del <= ins) {
                    dp[i][j] = del;
                    op[i][j] = 2;
                } else {
                    dp[i][j] = ins;
                    op[i][j] = 1;
                }
            }
            j = j + 1;
        }
        i = i + 1;
    }

    *inserts = 0;
    *deletes = 0;
    *replaces = 0;
    i = alen;
    j = blen;
    while (i > 0 || j > 0) {
        int o = op[i][j];
        if (o == 0) {
            i = i - 1;
            j = j - 1;
        } else if (o == 3) {
            *replaces = *replaces + 1;
            i = i - 1;
            j = j - 1;
        } else if (o == 2) {
            *deletes = *deletes + 1;
            i = i - 1;
        } else {
            *inserts = *inserts + 1;
            j = j - 1;
        }
    }
    return dp[alen][blen];
}

int hamming(char a[], char b[], int len) {
    int dist = 0;
    int i = 0;
    while (i < len) {
        if (a[i] != b[i]) dist = dist + 1;
        i = i + 1;
    }
    return dist;
}

int is_one_edit(char a[], int alen, char b[], int blen) {
    int d = edit_distance(a, alen, b, blen);
    return d <= 1;
}

int main(void) {
    char a1[10];
    a1[0]='k'; a1[1]='i'; a1[2]='t'; a1[3]='t'; a1[4]='e'; a1[5]='n'; a1[6]=0;
    char b1[10];
    b1[0]='s'; b1[1]='i'; b1[2]='t'; b1[3]='t'; b1[4]='i'; b1[5]='n'; b1[6]='g'; b1[7]=0;
    // EXPECT: ed(kitten,sitting) = 3
    printf("ed(kitten,sitting) = %d\n", edit_distance(a1, 6, b1, 7));

    char a2[10];
    a2[0]='a'; a2[1]='b'; a2[2]='c'; a2[3]=0;
    char b2[10];
    b2[0]='a'; b2[1]='b'; b2[2]='c'; b2[3]=0;
    // EXPECT: ed(abc,abc) = 0
    printf("ed(abc,abc) = %d\n", edit_distance(a2, 3, b2, 3));

    char a3[5];
    a3[0]='a'; a3[1]=0;
    char b3[5];
    b3[0]='b'; b3[1]=0;
    // EXPECT: ed(a,b) = 1
    printf("ed(a,b) = %d\n", edit_distance(a3, 1, b3, 1));

    char empty[2];
    empty[0] = 0;
    // EXPECT: ed(abc,empty) = 3
    printf("ed(abc,empty) = %d\n", edit_distance(a2, 3, empty, 0));
    // EXPECT: ed(empty,abc) = 3
    printf("ed(empty,abc) = %d\n", edit_distance(empty, 0, a2, 3));

    char c1[10];
    c1[0]='s'; c1[1]='u'; c1[2]='n'; c1[3]='d'; c1[4]='a'; c1[5]='y'; c1[6]=0;
    char c2[10];
    c2[0]='s'; c2[1]='a'; c2[2]='t'; c2[3]='u'; c2[4]='r'; c2[5]='d';
    c2[6]='a'; c2[7]='y'; c2[8]=0;
    // EXPECT: ed(sunday,saturday) = 3
    printf("ed(sunday,saturday) = %d\n", edit_distance(c1, 6, c2, 8));

    int ins = 0;
    int del = 0;
    int rep = 0;
    int dist = edit_ops(a1, 6, b1, 7, &ins, &del, &rep);
    // EXPECT: ops dist=3
    printf("ops dist=%d\n", dist);
    // EXPECT: ops total=3
    printf("ops total=%d\n", ins + del + rep);

    dist = edit_ops(a2, 3, b2, 3, &ins, &del, &rep);
    // EXPECT: ops2 dist=0
    printf("ops2 dist=%d\n", dist);
    // EXPECT: ops2 total=0
    printf("ops2 total=%d\n", ins + del + rep);

    char h1[5];
    h1[0]='A'; h1[1]='B'; h1[2]='C'; h1[3]=0;
    char h2[5];
    h2[0]='A'; h2[1]='X'; h2[2]='C'; h2[3]=0;
    // EXPECT: hamming = 1
    printf("hamming = %d\n", hamming(h1, h2, 3));

    // EXPECT: one_edit(ab,abc) = 1
    char oe1[5];
    oe1[0]='a'; oe1[1]='b'; oe1[2]=0;
    char oe2[5];
    oe2[0]='a'; oe2[1]='b'; oe2[2]='c'; oe2[3]=0;
    printf("one_edit(ab,abc) = %d\n", is_one_edit(oe1, 2, oe2, 3));

    // EXPECT: one_edit(abc,xyz) = 0
    char oe3[5];
    oe3[0]='x'; oe3[1]='y'; oe3[2]='z'; oe3[3]=0;
    printf("one_edit(abc,xyz) = %d\n", is_one_edit(a2, 3, oe3, 3));

    // EXPECT: ed(a,empty) = 1
    printf("ed(a,empty) = %d\n", edit_distance(a3, 1, empty, 0));

    char d1[10];
    d1[0]='i'; d1[1]='n'; d1[2]='t'; d1[3]='e';
    d1[4]='n'; d1[5]='t'; d1[6]='i'; d1[7]='o'; d1[8]='n'; d1[9]=0;
    char d2[10];
    d2[0]='e'; d2[1]='x'; d2[2]='e'; d2[3]='c';
    d2[4]='u'; d2[5]='t'; d2[6]='i'; d2[7]='o'; d2[8]='n'; d2[9]=0;
    // EXPECT: ed(intention,execution) = 5
    printf("ed(intention,execution) = %d\n", edit_distance(d1, 9, d2, 9));

    return 0;
}
