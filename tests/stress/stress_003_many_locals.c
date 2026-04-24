// stress_003_many_locals.c - Functions with 50+ local variables, register pressure
// EXPECT: sum50=1275
// EXPECT: mixed=17250
// EXPECT: fibchain=75025
// EXPECT: ptrs=2470
// EXPECT: multiarr=330
int printf(const char *fmt, ...);

struct small { int x; int y; };

int compute_50_locals(void) {
    int v0 = 1, v1 = 2, v2 = 3, v3 = 4, v4 = 5;
    int v5 = 6, v6 = 7, v7 = 8, v8 = 9, v9 = 10;
    int v10 = 11, v11 = 12, v12 = 13, v13 = 14, v14 = 15;
    int v15 = 16, v16 = 17, v17 = 18, v18 = 19, v19 = 20;
    int v20 = 21, v21 = 22, v22 = 23, v23 = 24, v24 = 25;
    int v25 = 26, v26 = 27, v27 = 28, v28 = 29, v29 = 30;
    int v30 = 31, v31 = 32, v32 = 33, v33 = 34, v34 = 35;
    int v35 = 36, v36 = 37, v37 = 38, v38 = 39, v39 = 40;
    int v40 = 41, v41 = 42, v42 = 43, v43 = 44, v44 = 45;
    int v45 = 46, v46 = 47, v47 = 48, v48 = 49, v49 = 50;

    // Sum all: 1+2+...+50 = 1275
    int sum = v0+v1+v2+v3+v4+v5+v6+v7+v8+v9
            + v10+v11+v12+v13+v14+v15+v16+v17+v18+v19
            + v20+v21+v22+v23+v24+v25+v26+v27+v28+v29
            + v30+v31+v32+v33+v34+v35+v36+v37+v38+v39
            + v40+v41+v42+v43+v44+v45+v46+v47+v48+v49;
    return sum;
}

int mixed_types(void) {
    char c0 = 'A', c1 = 'B', c2 = 'C', c3 = 'D', c4 = 'E';
    char c5 = 'F', c6 = 'G', c7 = 'H', c8 = 'I', c9 = 'J';
    int i0 = 100, i1 = 200, i2 = 300, i3 = 400, i4 = 500;
    long l0 = 1000, l1 = 2000, l2 = 3000, l3 = 4000, l4 = 5000;
    struct small s0, s1, s2, s3, s4;
    s0.x = 1; s0.y = 2;
    s1.x = 3; s1.y = 4;
    s2.x = 5; s2.y = 6;
    s3.x = 7; s3.y = 8;
    s4.x = 9; s4.y = 10;

    int char_sum = c0+c1+c2+c3+c4+c5+c6+c7+c8+c9;
    // A=65..J=74, sum = 65+66+67+68+69+70+71+72+73+74 = 695
    int int_sum = i0+i1+i2+i3+i4; // 1500
    long long_sum = l0+l1+l2+l3+l4; // 15000
    int struct_sum = s0.x+s0.y+s1.x+s1.y+s2.x+s2.y+s3.x+s3.y+s4.x+s4.y;
    // 1+2+3+4+5+6+7+8+9+10 = 55

    return char_sum + int_sum + (int)long_sum + struct_sum;
    // 695 + 1500 + 15000 + 55 = 17250
}

int cross_variable_deps(void) {
    int a0=1,a1=0,a2=0,a3=0,a4=0,a5=0,a6=0,a7=0,a8=0,a9=0;
    int a10=0,a11=0,a12=0,a13=0,a14=0,a15=0,a16=0,a17=0,a18=0,a19=0;
    int a20=0,a21=0,a22=0,a23=0,a24=0;

    // Fibonacci-like chain using 25 variables
    a1=1;
    a2=a0+a1; a3=a1+a2; a4=a2+a3; a5=a3+a4;
    a6=a4+a5; a7=a5+a6; a8=a6+a7; a9=a7+a8;
    a10=a8+a9; a11=a9+a10; a12=a10+a11; a13=a11+a12;
    a14=a12+a13; a15=a13+a14; a16=a14+a15; a17=a15+a16;
    a18=a16+a17; a19=a17+a18; a20=a18+a19; a21=a19+a20;
    a22=a20+a21; a23=a21+a22; a24=a22+a23;

    // a24 = fib(24) = 46368
    return a24;
}

int pointer_locals(void) {
    int arr[20];
    int i;
    for (i = 0; i < 20; i++) {
        arr[i] = i * i;
    }
    int *p0 = &arr[0], *p1 = &arr[1], *p2 = &arr[2], *p3 = &arr[3], *p4 = &arr[4];
    int *p5 = &arr[5], *p6 = &arr[6], *p7 = &arr[7], *p8 = &arr[8], *p9 = &arr[9];
    int *p10 = &arr[10], *p11 = &arr[11], *p12 = &arr[12], *p13 = &arr[13], *p14 = &arr[14];
    int *p15 = &arr[15], *p16 = &arr[16], *p17 = &arr[17], *p18 = &arr[18], *p19 = &arr[19];

    int sum = *p0 + *p1 + *p2 + *p3 + *p4 + *p5 + *p6 + *p7 + *p8 + *p9
            + *p10 + *p11 + *p12 + *p13 + *p14 + *p15 + *p16 + *p17 + *p18 + *p19;
    // sum of i^2 for i=0..19 = 19*20*39/6 = 2470
    return sum;
}

int multi_array_locals(void) {
    int a[10], b[10], c[10];
    int i;
    for (i = 0; i < 10; i++) {
        a[i] = i + 1;
        b[i] = (i + 1) * 2;
        c[i] = (i + 1) * 3;
    }
    int sum = 0;
    for (i = 0; i < 10; i++) {
        sum += a[i] + b[i] + c[i];
    }
    // sum = sum of (i+1)*6 for i=0..9 = 6*(1+2+...+10) = 6*55 = 330
    return sum;
}

int main(void) {
    printf("sum50=%d\n", compute_50_locals());
    printf("mixed=%d\n", mixed_types());
    printf("fibchain=%d\n", cross_variable_deps());
    printf("ptrs=%d\n", pointer_locals());
    printf("multiarr=%d\n", multi_array_locals());
    return 0;
}
