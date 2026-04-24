int printf(const char *fmt, ...);
// EXPECT: r1=3\nr2=0\nr3=0\nr4=-4\nr5=4\nr6=7\nr7=10\nr8=2\nr9=4\nr10=0\nr11=90\nr12=86\nr13=81\nr14=40\nr15=8\nr16=22\nr17=62\nr18=58\nr19=-59\nr20=-51\nr21=53\nr22=12\nr23=41\npressure_checksum=-121\ns1=505290270\ns2=1010580540\ns4=-252645136\ns8=252645120\ns16=252641280\nrs1=126322567\nrs2=63161283\nrs4=15790320\nrs8=986895\nrs16=3855\ncs1=-268435456\ncs2=101058054\ncs3=-167377402\ncs4=167377401\ncs5=116848374\nshift_checksum=-647600535\nstep1=305421512\nstep2=305473992\nstep3=305478653\nstep4=305478892\nstep5=1418895724\nstep6=1418895460\nstep7=1418895460\nstep8=1421320292\nstep9=1424168900\nstep10=1729590412\nstep11=1605161196\nstep12=1299682304\nstep13=1300118258\nstep14=-1575953578\nstep15=-159220942\nstep16=159220941\nstep17=159221194\nstep18=204715190\nstep19=1504833448\nstep20=1355574117\nhash_checksum=1034551127\nbm1=286331153\nbm2=2004318071\nbm3=1717986918\nbm4=-1431655766\nbm5=-2004318072\nbm6=-1145324612\nbm7=50529028\nbm8=134088702\nbm9=134150145\nbm10=152181007\nbm11=-1\nbm12=-1\nbm13=50529027\nbm14=555819299\nbm15=-18874659\nbitwise_checksum=1333108887\nda=111\ndb=178\ndc=221\ndd=411\nde=32\ndf=479\ndg=368\ndh=175\ndi=230\ndj=2108\ndk=2266\ndl=-1767\ndm=63769\ndn=63999\ndp=66107\ndq=1610620999\ndr=-1610622626\nds=-1610558857\ndt=1610558856\ndu=1073679823\ndeep_checksum=-1073614506\ngrand_checksum=-1807468680

/* Stress test: register pressure from 20+ chained arithmetic/bitwise/shift ops.
 * Tests interleaved operation chains, heavy intermediate reuse, and
 * bitwise mixed with arithmetic across 15+ simultaneously live variables. */

/* Function 1: 15+ locals alive simultaneously, 4 interleaved chains, 23+ ops */
int test_register_pressure(void) {
    int a = 1, b = 2, c = 3, d = 4, e = 5;
    int f = 6, g = 7, h = 8, i = 9, j = 10;
    int k = 11, l = 12, m = 13, n = 14, o = 15, p = 16;

    /* Chain A: arithmetic and bitwise */
    int r1 = a + b;
    int r2 = r1 ^ c;
    int r3 = r2 << 2;
    int r4 = r3 - d;
    int r5 = r4 & e;

    printf("r1=%d\n", r1);
    printf("r2=%d\n", r2);
    printf("r3=%d\n", r3);
    printf("r4=%d\n", r4);
    printf("r5=%d\n", r5);

    /* Chain B: interleaved with Chain A results */
    int r6 = f | g;
    int r7 = r6 + r1;
    int r8 = r7 ^ h;
    int r9 = r8 << 1;
    int r10 = r9 - r5;

    printf("r6=%d\n", r6);
    printf("r7=%d\n", r7);
    printf("r8=%d\n", r8);
    printf("r9=%d\n", r9);
    printf("r10=%d\n", r10);

    /* Chain C: uses results from both A and B */
    int r11 = i * j;
    int r12 = r11 + r4;
    int r13 = r12 ^ r6;
    int r14 = r13 >> 1;
    int r15 = r14 & k;

    printf("r11=%d\n", r11);
    printf("r12=%d\n", r12);
    printf("r13=%d\n", r13);
    printf("r14=%d\n", r14);
    printf("r15=%d\n", r15);

    /* Chain D: uses results from all previous chains */
    int r16 = l + r7;
    int r17 = r16 | r14;
    int r18 = r17 ^ r5;
    int r19 = ~r18;
    int r20 = r19 + r15;

    printf("r16=%d\n", r16);
    printf("r17=%d\n", r17);
    printf("r18=%d\n", r18);
    printf("r19=%d\n", r19);
    printf("r20=%d\n", r20);

    /* Reuse intermediates from multiple chains (high register pressure) */
    int r21 = r1 + r7 + r14;
    int r22 = r5 ^ r10 ^ r15;
    int r23 = r21 - r22;

    printf("r21=%d\n", r21);
    printf("r22=%d\n", r22);
    printf("r23=%d\n", r23);

    int checksum = r1 ^ r2 ^ r3 ^ r4 ^ r5 ^ r6 ^ r7 ^ r8 ^ r9 ^ r10
                 ^ r11 ^ r12 ^ r13 ^ r14 ^ r15 ^ r16 ^ r17 ^ r18
                 ^ r19 ^ r20 ^ r21 ^ r22 ^ r23;
    printf("pressure_checksum=%d\n", checksum);
    return checksum;
}

/* Function 2: shift varieties with left/right shifts of 1,2,4,8,16 */
int test_shift_varieties(void) {
    int base = 0x0F0F0F0F;

    /* Left shifts */
    int s1 = base << 1;
    int s2 = base << 2;
    int s4 = base << 4;
    int s8 = base << 8;
    int s16 = base << 16;

    printf("s1=%d\n", s1);
    printf("s2=%d\n", s2);
    printf("s4=%d\n", s4);
    printf("s8=%d\n", s8);
    printf("s16=%d\n", s16);

    /* Right shifts */
    int rs1 = base >> 1;
    int rs2 = base >> 2;
    int rs4 = base >> 4;
    int rs8 = base >> 8;
    int rs16 = base >> 16;

    printf("rs1=%d\n", rs1);
    printf("rs2=%d\n", rs2);
    printf("rs4=%d\n", rs4);
    printf("rs8=%d\n", rs8);
    printf("rs16=%d\n", rs16);

    /* Cross-shift operations mixing left and right shifts */
    int cs1 = (base << 4) ^ (base >> 4);
    int cs2 = (s1 & rs1) | (s2 & rs2);
    int cs3 = cs1 + cs2;
    int cs4 = ~cs3;
    int cs5 = cs4 ^ base;

    printf("cs1=%d\n", cs1);
    printf("cs2=%d\n", cs2);
    printf("cs3=%d\n", cs3);
    printf("cs4=%d\n", cs4);
    printf("cs5=%d\n", cs5);

    int checksum = s1 ^ s2 ^ s4 ^ s8 ^ s16
                 ^ rs1 ^ rs2 ^ rs4 ^ rs8 ^ rs16
                 ^ cs1 ^ cs2 ^ cs3 ^ cs4 ^ cs5;
    printf("shift_checksum=%d\n", checksum);
    return checksum;
}

/* Function 3: complex hash-like computation through 20 chained ops */
int test_hash_chain(void) {
    int hv = 0x12345678;
    int k1 = 0xAB, k2 = 0xCD, k3 = 0xEF, k4 = 0x42, k5 = 0x37;

    int step1 = hv ^ (k1 << 4);
    int step2 = step1 + (k2 << 8);
    int step3 = step2 ^ (step2 >> 16);
    int step4 = step3 + k3;
    int step5 = step4 ^ (step4 << 5);
    int step6 = step5 - (k4 << 2);
    int step7 = step6 & 0x7FFFFFFF;
    int step8 = step7 | (k5 << 16);
    int step9 = step8 ^ (step8 >> 8);
    int step10 = step9 + step1;
    int step11 = step10 ^ (step10 << 3);
    int step12 = step11 - step4;
    int step13 = step12 ^ (step12 >> 11);
    int step14 = step13 + step7;
    int step15 = step14 ^ step6;
    int step16 = ~step15;
    int step17 = step16 + (step3 & 0xFF);
    int step18 = step17 ^ (step9 >> 4);
    int step19 = step18 + step13;
    int step20 = step19 ^ step16;

    printf("step1=%d\n", step1);
    printf("step2=%d\n", step2);
    printf("step3=%d\n", step3);
    printf("step4=%d\n", step4);
    printf("step5=%d\n", step5);
    printf("step6=%d\n", step6);
    printf("step7=%d\n", step7);
    printf("step8=%d\n", step8);
    printf("step9=%d\n", step9);
    printf("step10=%d\n", step10);
    printf("step11=%d\n", step11);
    printf("step12=%d\n", step12);
    printf("step13=%d\n", step13);
    printf("step14=%d\n", step14);
    printf("step15=%d\n", step15);
    printf("step16=%d\n", step16);
    printf("step17=%d\n", step17);
    printf("step18=%d\n", step18);
    printf("step19=%d\n", step19);
    printf("step20=%d\n", step20);

    int checksum = step1 ^ step2 ^ step3 ^ step4 ^ step5
                 ^ step6 ^ step7 ^ step8 ^ step9 ^ step10
                 ^ step11 ^ step12 ^ step13 ^ step14 ^ step15
                 ^ step16 ^ step17 ^ step18 ^ step19 ^ step20;
    printf("hash_checksum=%d\n", checksum);
    return checksum;
}

/* Function 4: bitwise operations mixed with arithmetic, unary NOT */
int test_bitwise_mix(void) {
    int ba = 0x55555555;
    int bb = 0x33333333;
    int bc = 0x0F0F0F0F;
    int bd = 0x00FF00FF;
    int be = 0x0000FFFF;

    int bm1 = ba & bb;
    int bm2 = ba | bb;
    int bm3 = ba ^ bb;
    int bm4 = ~ba;
    int bm5 = bm1 + bm2;
    int bm6 = bm3 - bm4;
    int bm7 = (bm5 ^ bm6) & bc;
    int bm8 = (bm7 | bd) << 1;
    int bm9 = bm8 ^ be;
    int bm10 = ~bm9 + bm1;

    printf("bm1=%d\n", bm1);
    printf("bm2=%d\n", bm2);
    printf("bm3=%d\n", bm3);
    printf("bm4=%d\n", bm4);
    printf("bm5=%d\n", bm5);
    printf("bm6=%d\n", bm6);
    printf("bm7=%d\n", bm7);
    printf("bm8=%d\n", bm8);
    printf("bm9=%d\n", bm9);
    printf("bm10=%d\n", bm10);

    /* More bitwise with heavy reuse of prior results */
    int bm11 = (bm1 & bm3) ^ (bm2 | bm4);
    int bm12 = bm11 >> 4;
    int bm13 = bm12 + bm7;
    int bm14 = bm13 ^ (bm5 << 2);
    int bm15 = ~(bm14 & bm8);

    printf("bm11=%d\n", bm11);
    printf("bm12=%d\n", bm12);
    printf("bm13=%d\n", bm13);
    printf("bm14=%d\n", bm14);
    printf("bm15=%d\n", bm15);

    int checksum = bm1 ^ bm2 ^ bm3 ^ bm4 ^ bm5 ^ bm6 ^ bm7
                 ^ bm8 ^ bm9 ^ bm10 ^ bm11 ^ bm12 ^ bm13 ^ bm14 ^ bm15;
    printf("bitwise_checksum=%d\n", checksum);
    return checksum;
}

/* Function 5: deeply interleaved chains where chain A and B constantly
 * feed each other, with rotate-like operations */
int test_deep_interleave(void) {
    int x1 = 100, x2 = 200, x3 = 300, x4 = 400, x5 = 500;
    int y1 = 11, y2 = 22, y3 = 33, y4 = 44, y5 = 55;

    int da = x1 + y1;
    int db = x2 - y2;
    int dc = da ^ db;
    int dd = x3 + da;
    int de = db & y3;
    int df = dc | dd;
    int dg = x4 - de;
    int dh = df ^ dg;
    int di = (da + db + dc + dd) >> 2;
    int dj = (de + df + dg + dh) << 1;
    int dk = di ^ dj;
    int dl = ~dk + x5;
    int dm = dl & 0xFFFF;
    int dn = dm ^ di;
    int dp = dn + dj;
    int dq = (dp >> 3) | (dp << 29);
    int dr = dq ^ dl;
    int ds = dr + dm;
    int dt = ~ds;
    int du = dt ^ dq;

    printf("da=%d\n", da);
    printf("db=%d\n", db);
    printf("dc=%d\n", dc);
    printf("dd=%d\n", dd);
    printf("de=%d\n", de);
    printf("df=%d\n", df);
    printf("dg=%d\n", dg);
    printf("dh=%d\n", dh);
    printf("di=%d\n", di);
    printf("dj=%d\n", dj);
    printf("dk=%d\n", dk);
    printf("dl=%d\n", dl);
    printf("dm=%d\n", dm);
    printf("dn=%d\n", dn);
    printf("dp=%d\n", dp);
    printf("dq=%d\n", dq);
    printf("dr=%d\n", dr);
    printf("ds=%d\n", ds);
    printf("dt=%d\n", dt);
    printf("du=%d\n", du);

    int checksum = da ^ db ^ dc ^ dd ^ de ^ df ^ dg ^ dh ^ di ^ dj
                 ^ dk ^ dl ^ dm ^ dn ^ dp ^ dq ^ dr ^ ds ^ dt ^ du;
    printf("deep_checksum=%d\n", checksum);
    return checksum;
}

int main(void) {
    int c1 = test_register_pressure();
    int c2 = test_shift_varieties();
    int c3 = test_hash_chain();
    int c4 = test_bitwise_mix();
    int c5 = test_deep_interleave();

    int grand = c1 ^ c2 ^ c3 ^ c4 ^ c5;
    printf("grand_checksum=%d\n", grand);

    return 0;
}
