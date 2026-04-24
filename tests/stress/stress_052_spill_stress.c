// stress_052_spill_stress.c - Register spill stress test
// Forces 20+ live variables simultaneously, heavy spilling required.
// EXPECT: chain20=210
// EXPECT: cross_dep=980
// EXPECT: pyramid=8178
// EXPECT: interleave=4200
// EXPECT: mega_live=12865
// EXPECT: accumulate=4830
// EXPECT: live_across_call=1560
// EXPECT: spill_total=32823

int printf(const char *fmt, ...);

// 20 live variables, all contributing to result
int chain20(void) {
    int a1 = 1, a2 = 2, a3 = 3, a4 = 4, a5 = 5;
    int a6 = 6, a7 = 7, a8 = 8, a9 = 9, a10 = 10;
    int a11 = 11, a12 = 12, a13 = 13, a14 = 14, a15 = 15;
    int a16 = 16, a17 = 17, a18 = 18, a19 = 19, a20 = 20;
    // Use all of them in one expression
    int sum = a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9 + a10
            + a11 + a12 + a13 + a14 + a15 + a16 + a17 + a18 + a19 + a20;
    return sum;
}

// Cross-dependencies: each var depends on two previous
int cross_dep(void) {
    int v1 = 1, v2 = 2, v3 = 3, v4 = 4, v5 = 5;
    int v6 = 6, v7 = 7, v8 = 8, v9 = 9, v10 = 10;
    int v11 = 11, v12 = 12, v13 = 13, v14 = 14, v15 = 15;
    int v16 = 16, v17 = 17, v18 = 18, v19 = 19, v20 = 20;

    // Cross multiply pairs
    int r1 = v1 * v20;  // 20
    int r2 = v2 * v19;  // 38
    int r3 = v3 * v18;  // 54
    int r4 = v4 * v17;  // 68
    int r5 = v5 * v16;  // 80
    int r6 = v6 * v15;  // 90
    int r7 = v7 * v14;  // 98
    int r8 = v8 * v13;  // 104
    int r9 = v9 * v12;  // 108
    int r10 = v10 * v11; // 110

    // All r values live at once
    return r1 + r2 + r3 + r4 + r5 + r6 + r7 + r8 + r9 + r10
         + v1 + v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9 + v10
         + v11 + v12 + v13 + v14 + v15 + v16 + v17 + v18 + v19 + v20;
}

// Pyramid of accumulations - 20 vars, each adds all previous
int pyramid(void) {
    int p1 = 1;
    int p2 = p1 + 2;
    int p3 = p1 + p2 + 3;
    int p4 = p1 + p2 + p3 + 4;
    int p5 = p1 + p2 + p3 + p4 + 5;
    int p6 = p1 + p2 + p3 + p4 + p5 + 6;
    int p7 = p1 + p2 + p3 + p4 + p5 + p6 + 7;
    int p8 = p1 + p2 + p3 + p4 + p5 + p6 + p7 + 8;
    int p9 = p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + 9;
    int p10 = p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + 10;
    int p11 = p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + 11;
    int p12 = p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11 + 12;
    // Now use all to force them live
    return p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10 + p11 + p12;
}

// Interleaved reads: read vars in non-sequential order to stress allocator
int interleave(void) {
    int a = 10, b = 20, c = 30, d = 40, e = 50;
    int f = 60, g = 70, h = 80, i = 90, j = 100;
    int k = 110, l = 120, m = 130, n = 140, o = 150;
    int p = 160, q = 170, r = 180, s = 190, t = 200;

    // Access in jumbled order to create interference
    int x1 = a + t + c + r + e + p + g + n + i + l;
    int x2 = b + s + d + q + f + o + h + m + j + k;
    int x3 = t + a + s + b + r + c + q + d + p + e;
    int x4 = o + f + n + g + m + h + l + i + k + j;

    return x1 + x2 + x3 + x4;
}

// Mega live: 30 variables, complex expression tree
int mega_live(void) {
    int m1=1, m2=2, m3=3, m4=4, m5=5, m6=6, m7=7, m8=8, m9=9, m10=10;
    int m11=11, m12=12, m13=13, m14=14, m15=15;
    int m16=16, m17=17, m18=18, m19=19, m20=20;
    int m21=21, m22=22, m23=23, m24=24, m25=25;
    int m26=26, m27=27, m28=28, m29=29, m30=30;

    // Build a tree of operations
    int t1 = m1 * m30 + m2 * m29;  // 30 + 58 = 88
    int t2 = m3 * m28 + m4 * m27;  // 84 + 108 = 192
    int t3 = m5 * m26 + m6 * m25;  // 130 + 150 = 280
    int t4 = m7 * m24 + m8 * m23;  // 168 + 184 = 352
    int t5 = m9 * m22 + m10 * m21; // 198 + 210 = 408
    int t6 = m11 * m20 + m12 * m19; // 220 + 228 = 448
    int t7 = m13 * m18 + m14 * m17; // 234 + 238 = 472
    int t8 = m15 * m16;             // 240

    // Second level
    int u1 = t1 + t2 + t3 + t4;  // 912
    int u2 = t5 + t6 + t7 + t8;  // 1568

    // Still need originals live for this
    int u3 = m1+m2+m3+m4+m5+m6+m7+m8+m9+m10
           + m11+m12+m13+m14+m15+m16+m17+m18+m19+m20
           + m21+m22+m23+m24+m25+m26+m27+m28+m29+m30; // 465

    return (u1 + u2) * 5 + u3;
    // (912 + 1568) * 5 + 465 = 2480 * 5 + 465 = 12400 + 465 = 12865
    // Let me recompute...
    // Actually let me just compute it carefully
}

// Accumulate in loop with many live vars
int accumulate(void) {
    int a=0, b=0, c=0, d=0, e=0, f=0, g=0, h=0, i_val=0, j=0;
    int k=0, l=0, m=0, n=0, o=0, p=0, q=0, r=0, s=0, t=0;
    int iter;
    for (iter = 1; iter <= 20; iter++) {
        a += iter;
        b += iter * 2;
        c += iter * 3;
        d += iter;
        e += iter;
        f += iter;
        g += iter;
        h += iter;
        i_val += iter;
        j += iter;
        k += iter;
        l += iter;
        m += iter;
        n += iter;
        o += iter;
        p += iter;
        q += iter;
        r += iter;
        s += iter;
        t += iter;
    }
    // a = 210, b = 420, c = 630, rest = 210 each (17 vars)
    return a + b + c + d + e + f + g + h + i_val + j
         + k + l + m + n + o + p + q + r + s + t;
}

// Variables live across function call (forces spill around call)
int helper_add(int x, int y) {
    return x + y;
}

int live_across_call(void) {
    int a=10, b=20, c=30, d=40, e=50, f=60, g=70, h=80, i_v=90, j=100;
    int k=110, l=120;

    // Call uses a,b but c-l must survive
    int r1 = helper_add(a, b);
    // Call uses c,d but rest must survive
    int r2 = helper_add(c, d);
    int r3 = helper_add(e, f);
    int r4 = helper_add(g, h);
    int r5 = helper_add(i_v, j);
    int r6 = helper_add(k, l);

    // Now use everything: originals + results
    return a + b + c + d + e + f + g + h + i_v + j + k + l
         + r1 + r2 + r3 + r4 + r5 + r6;
}

int main(void) {
    int c20 = chain20();
    printf("chain20=%d\n", c20);

    int cd = cross_dep();
    printf("cross_dep=%d\n", cd);

    int py = pyramid();
    printf("pyramid=%d\n", py);

    int il = interleave();
    printf("interleave=%d\n", il);

    int ml = mega_live();
    printf("mega_live=%d\n", ml);

    int ac = accumulate();
    printf("accumulate=%d\n", ac);

    int lac = live_across_call();
    printf("live_across_call=%d\n", lac);

    int total = c20 + cd + py + il + ml + ac + lac;
    printf("spill_total=%d\n", total);

    return 0;
}
