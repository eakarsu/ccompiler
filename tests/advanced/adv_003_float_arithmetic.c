int printf(const char *fmt, ...);
// EXPECT: === Float Arithmetic Tests ===
// EXPECT: fadd: 1.5+2.5=4.000000
// EXPECT: fsub: 10.7-3.2=7.500000
// EXPECT: fmul: 3.0*4.5=13.500000
// EXPECT: fdiv: 10.0/3.0=3.333333
// EXPECT: fneg: -5.500000
// EXPECT: fneg2: 5.500000
// EXPECT: dadd: 1.5+2.5=4.000000
// EXPECT: dsub: 100.75-50.25=50.500000
// EXPECT: dmul: 2.5*4.0=10.000000
// EXPECT: ddiv: 22.0/7.0=3.142857
// EXPECT: dneg: -3.140000
// EXPECT: fzero_add: 0.0+5.5=5.500000
// EXPECT: fzero_mul: 5.5*0.0=0.000000
// EXPECT: f_chain_add: 1.1+2.2+3.3=6.600000
// EXPECT: f_chain_mul: 2.0*3.0*4.0=24.000000
// EXPECT: f_mixed: 2.5+3.0*4.0=14.500000
// EXPECT: f_parens: (2.5+3.0)*4.0=22.000000
// EXPECT: cast_i2f: 42 as float=42.000000
// EXPECT: cast_f2i: 3.700000 as int=3
// EXPECT: cast_f2i_neg: -3.700000 as int=-3
// EXPECT: cast_i2d: 12345 as double=12345.000000
// EXPECT: cast_d2i: 99.900000 as int=99
// EXPECT: cast_f2d: 1.5f as double=1.500000
// EXPECT: cast_d2f: 1.5d as float=1.500000
// EXPECT: neg_fadd: -1.5+-2.5=-4.000000
// EXPECT: neg_fmul: -3.0*2.0=-6.000000
// EXPECT: neg_fmul2: -3.0*-2.0=6.000000
// EXPECT: f_precision: 0.1+0.2=0.300000
// EXPECT: d_precision: 0.1+0.2=0.300000
// EXPECT: f_large: 1000000.0*1000.0=1000000000.000000
// EXPECT: f_small: 0.001*0.001=0.000001
// EXPECT: multi_cast: (int)(float)42=42
// EXPECT: expr1: (3.14*2.0)+1.0=7.280000
// EXPECT: expr2: 100.0/(3.0+2.0)=20.000000
// EXPECT: expr3: (10.5-0.5)*(2.0+1.0)=30.000000
// EXPECT: int_div_to_float: 7/2=3.000000
// EXPECT: float_div: 7.0/2.0=3.500000
// fadd: 1.5+2.5=4.000000
// fsub: 10.7-3.2=7.500000
// fmul: 3.0*4.5=13.500000
// fdiv: 10.0/3.0=3.333333
// fneg: -5.500000
// fneg2: 5.500000
// dadd: 1.5+2.5=4.000000
// dsub: 100.75-50.25=50.500000
// dmul: 2.5*4.0=10.000000
// ddiv: 22.0/7.0=3.142857
// dneg: -3.140000
// fzero_add: 0.0+5.5=5.500000
// fzero_mul: 5.5*0.0=0.000000
// f_chain_add: 1.1+2.2+3.3=6.600000
// f_chain_mul: 2.0*3.0*4.0=24.000000
// f_mixed: 2.5+3.0*4.0=14.500000
// f_parens: (2.5+3.0)*4.0=22.000000
// cast_i2f: 42 as float=42.000000
// cast_f2i: 3.700000 as int=3
// cast_f2i_neg: -3.700000 as int=-3
// cast_i2d: 12345 as double=12345.000000
// cast_d2i: 99.900000 as int=99
// cast_f2d: 1.5f as double=1.500000
// cast_d2f: 1.5d as float=1.500000
// neg_fadd: -1.5+-2.5=-4.000000
// neg_fmul: -3.0*2.0=-6.000000
// neg_fmul2: -3.0*-2.0=6.000000
// f_precision: 0.1+0.2=0.300000
// d_precision: 0.1+0.2=0.300000
// f_large: 1000000.0*1000.0=1000000000.000000
// f_small: 0.001*0.001=0.000001
// multi_cast: (int)(float)42=42
// expr1: (3.14*2.0)+1.0=7.280000
// expr2: 100.0/(3.0+2.0)=20.000000
// expr3: (10.5-0.5)*(2.0+1.0)=30.000000
// int_div_to_float: 7/2=3.000000
// float_div: 7.0/2.0=3.500000

int main(void) {
    float fa, fb, fr;
    double da, db, dr;
    int ia;

    printf("=== Float Arithmetic Tests ===\n");

    // Basic float operations
    fa = 1.5; fb = 2.5;
    printf("fadd: 1.5+2.5=%f\n", fa + fb);
    fa = 10.7; fb = 3.2;
    printf("fsub: 10.7-3.2=%f\n", fa - fb);
    fa = 3.0; fb = 4.5;
    printf("fmul: 3.0*4.5=%f\n", fa * fb);
    fa = 10.0; fb = 3.0;
    printf("fdiv: 10.0/3.0=%f\n", fa / fb);

    // Float negation
    fa = 5.5;
    printf("fneg: %f\n", -fa);
    fa = -5.5;
    printf("fneg2: %f\n", -fa);

    // Basic double operations
    da = 1.5; db = 2.5;
    printf("dadd: 1.5+2.5=%f\n", da + db);
    da = 100.75; db = 50.25;
    printf("dsub: 100.75-50.25=%f\n", da - db);
    da = 2.5; db = 4.0;
    printf("dmul: 2.5*4.0=%f\n", da * db);
    da = 22.0; db = 7.0;
    printf("ddiv: 22.0/7.0=%f\n", da / db);

    // Double negation
    da = 3.14;
    printf("dneg: %f\n", -da);

    // Zero operations
    fa = 0.0; fb = 5.5;
    printf("fzero_add: 0.0+5.5=%f\n", fa + fb);
    fa = 5.5; fb = 0.0;
    printf("fzero_mul: 5.5*0.0=%f\n", fa * fb);

    // Chained float operations
    fa = 1.1;
    fb = 2.2;
    fr = fa + fb + 3.3;
    printf("f_chain_add: 1.1+2.2+3.3=%f\n", fr);
    fr = 2.0 * 3.0 * 4.0;
    printf("f_chain_mul: 2.0*3.0*4.0=%f\n", fr);

    // Mixed precedence with floats
    fr = 2.5 + 3.0 * 4.0;
    printf("f_mixed: 2.5+3.0*4.0=%f\n", fr);
    fr = (2.5 + 3.0) * 4.0;
    printf("f_parens: (2.5+3.0)*4.0=%f\n", fr);

    // Int to float casting
    ia = 42;
    fa = (float)ia;
    printf("cast_i2f: 42 as float=%f\n", fa);

    // Float to int casting (truncation)
    fa = 3.7;
    ia = (int)fa;
    printf("cast_f2i: %f as int=%d\n", fa, ia);
    fa = -3.7;
    ia = (int)fa;
    printf("cast_f2i_neg: %f as int=%d\n", fa, ia);

    // Int to double
    ia = 12345;
    da = (double)ia;
    printf("cast_i2d: 12345 as double=%f\n", da);

    // Double to int
    da = 99.9;
    ia = (int)da;
    printf("cast_d2i: %f as int=%d\n", da, ia);

    // Float to double
    fa = 1.5;
    da = (double)fa;
    printf("cast_f2d: 1.5f as double=%f\n", da);

    // Double to float
    da = 1.5;
    fa = (float)da;
    printf("cast_d2f: 1.5d as float=%f\n", fa);

    // Negative float arithmetic
    fa = -1.5; fb = -2.5;
    printf("neg_fadd: -1.5+-2.5=%f\n", fa + fb);
    fa = -3.0; fb = 2.0;
    printf("neg_fmul: -3.0*2.0=%f\n", fa * fb);
    fa = -3.0; fb = -2.0;
    printf("neg_fmul2: -3.0*-2.0=%f\n", fa * fb);

    // Precision tests (using %f which gives 6 decimal places)
    fa = 0.1; fb = 0.2;
    fr = fa + fb;
    printf("f_precision: 0.1+0.2=%f\n", fr);
    da = 0.1; db = 0.2;
    dr = da + db;
    printf("d_precision: 0.1+0.2=%f\n", dr);

    // Large and small floats
    da = 1000000.0; db = 1000.0;
    printf("f_large: 1000000.0*1000.0=%f\n", da * db);
    da = 0.001; db = 0.001;
    printf("f_small: 0.001*0.001=%f\n", da * db);

    // Multi-step casting
    ia = 42;
    fa = (float)ia;
    ia = (int)fa;
    printf("multi_cast: (int)(float)42=%d\n", ia);

    // Complex float expressions
    da = 3.14;
    dr = (da * 2.0) + 1.0;
    printf("expr1: (3.14*2.0)+1.0=%f\n", dr);
    dr = 100.0 / (3.0 + 2.0);
    printf("expr2: 100.0/(3.0+2.0)=%f\n", dr);
    dr = (10.5 - 0.5) * (2.0 + 1.0);
    printf("expr3: (10.5-0.5)*(2.0+1.0)=%f\n", dr);

    // Integer division vs float division
    ia = 7;
    da = (double)(ia / 2);
    printf("int_div_to_float: 7/2=%f\n", da);
    da = 7.0 / 2.0;
    printf("float_div: 7.0/2.0=%f\n", da);

    return 0;
}
