int printf(const char *fmt, ...);
// EXPECT: === Comparison and Logic Tests ===
// EXPECT: eq_true: (5==5)=1
// EXPECT: eq_false: (5==6)=0
// EXPECT: ne_true: (5!=6)=1
// EXPECT: ne_false: (5!=5)=0
// EXPECT: lt_true: (3<5)=1
// EXPECT: lt_false: (5<3)=0
// EXPECT: lt_eq: (5<5)=0
// EXPECT: gt_true: (5>3)=1
// EXPECT: gt_false: (3>5)=0
// EXPECT: gt_eq: (5>5)=0
// EXPECT: le_true: (3<=5)=1
// EXPECT: le_eq: (5<=5)=1
// EXPECT: le_false: (6<=5)=0
// EXPECT: ge_true: (5>=3)=1
// EXPECT: ge_eq: (5>=5)=1
// EXPECT: ge_false: (3>=5)=0
// EXPECT: neg_lt: (-5<0)=1
// EXPECT: neg_gt: (0>-5)=1
// EXPECT: neg_cmp: (-3<-1)=1
// EXPECT: and_tt: (1&&1)=1
// EXPECT: and_tf: (1&&0)=0
// EXPECT: and_ft: (0&&1)=0
// EXPECT: and_ff: (0&&0)=0
// EXPECT: or_tt: (1||1)=1
// EXPECT: or_tf: (1||0)=1
// EXPECT: or_ft: (0||1)=1
// EXPECT: or_ff: (0||0)=0
// EXPECT: not_0: !0=1
// EXPECT: not_1: !1=0
// EXPECT: not_42: !42=0
// EXPECT: not_neg: !(-1)=0
// EXPECT: sc_and: short-circuit AND: a=1
// EXPECT: sc_or: short-circuit OR: a=1
// EXPECT: sc_and2: no side effect: b=0
// EXPECT: sc_or2: no side effect: b=0
// EXPECT: nested1: ((3>1)&&(5<10))=1
// EXPECT: nested2: ((3>1)&&(5>10))=0
// EXPECT: nested3: ((3>5)||(5<10))=1
// EXPECT: nested4: (!(3>5)&&(5<10))=1
// EXPECT: chain_and: (1&&2&&3)=1
// EXPECT: chain_or: (0||0||1)=1
// EXPECT: chain_mixed: (1||0)&&(0||1)=1
// EXPECT: complex1: (3>1&&5>2)||(10<5)=1
// EXPECT: complex2: !(3>5||2>8)=1
// EXPECT: ternary1: (5>3)?10:20=10
// EXPECT: ternary2: (5<3)?10:20=20
// EXPECT: ternary_nest: 100
// EXPECT: truth_vals: 42 is true=1
// EXPECT: truth_vals2: -1 is true=1
// EXPECT: cmp_chain: (1<2)+(2<3)+(3<4)=3
// EXPECT: eq_expr: (2+3==5)=1
// EXPECT: ne_expr: (2+3!=6)=1
// eq_true: (5==5)=1
// eq_false: (5==6)=0
// ne_true: (5!=6)=1
// ne_false: (5!=5)=0
// lt_true: (3<5)=1
// lt_false: (5<3)=0
// lt_eq: (5<5)=0
// gt_true: (5>3)=1
// gt_false: (3>5)=0
// gt_eq: (5>5)=0
// le_true: (3<=5)=1
// le_eq: (5<=5)=1
// le_false: (6<=5)=0
// ge_true: (5>=3)=1
// ge_eq: (5>=5)=1
// ge_false: (3>=5)=0
// neg_lt: (-5<0)=1
// neg_gt: (0>-5)=1
// neg_cmp: (-3<-1)=1
// and_tt: (1&&1)=1
// and_tf: (1&&0)=0
// and_ft: (0&&1)=0
// and_ff: (0&&0)=0
// or_tt: (1||1)=1
// or_tf: (1||0)=1
// or_ft: (0||1)=1
// or_ff: (0||0)=0
// not_0: !0=1
// not_1: !1=0
// not_42: !42=0
// not_neg: !(-1)=0
// sc_and: short-circuit AND: a=1
// sc_or: short-circuit OR: a=1
// sc_and2: no side effect: b=0
// sc_or2: no side effect: b=0
// nested1: ((3>1)&&(5<10))=1
// nested2: ((3>1)&&(5>10))=0
// nested3: ((3>5)||(5<10))=1
// nested4: (!(3>5)&&(5<10))=1
// chain_and: (1&&2&&3)=1
// chain_or: (0||0||1)=1
// chain_mixed: (1||0)&&(0||1)=1
// complex1: (3>1&&5>2)||(10<5)=1
// complex2: !(3>5||2>8)=1
// ternary1: (5>3)?10:20=10
// ternary2: (5<3)?10:20=20
// ternary_nest: 100
// truth_vals: 42 is true=1
// truth_vals2: -1 is true=1
// cmp_chain: (1<2)+(2<3)+(3<4)=3
// eq_expr: (2+3==5)=1
// ne_expr: (2+3!=6)=1

int main(void) {
    int a, b, r;

    printf("=== Comparison and Logic Tests ===\n");

    // Equality
    printf("eq_true: (5==5)=%d\n", 5 == 5);
    printf("eq_false: (5==6)=%d\n", 5 == 6);

    // Not equal
    printf("ne_true: (5!=6)=%d\n", 5 != 6);
    printf("ne_false: (5!=5)=%d\n", 5 != 5);

    // Less than
    printf("lt_true: (3<5)=%d\n", 3 < 5);
    printf("lt_false: (5<3)=%d\n", 5 < 3);
    printf("lt_eq: (5<5)=%d\n", 5 < 5);

    // Greater than
    printf("gt_true: (5>3)=%d\n", 5 > 3);
    printf("gt_false: (3>5)=%d\n", 3 > 5);
    printf("gt_eq: (5>5)=%d\n", 5 > 5);

    // Less than or equal
    printf("le_true: (3<=5)=%d\n", 3 <= 5);
    printf("le_eq: (5<=5)=%d\n", 5 <= 5);
    printf("le_false: (6<=5)=%d\n", 6 <= 5);

    // Greater than or equal
    printf("ge_true: (5>=3)=%d\n", 5 >= 3);
    printf("ge_eq: (5>=5)=%d\n", 5 >= 5);
    printf("ge_false: (3>=5)=%d\n", 3 >= 5);

    // Negative number comparisons
    printf("neg_lt: (-5<0)=%d\n", -5 < 0);
    printf("neg_gt: (0>-5)=%d\n", 0 > -5);
    printf("neg_cmp: (-3<-1)=%d\n", -3 < -1);

    // Logical AND truth table
    printf("and_tt: (1&&1)=%d\n", 1 && 1);
    printf("and_tf: (1&&0)=%d\n", 1 && 0);
    printf("and_ft: (0&&1)=%d\n", 0 && 1);
    printf("and_ff: (0&&0)=%d\n", 0 && 0);

    // Logical OR truth table
    printf("or_tt: (1||1)=%d\n", 1 || 1);
    printf("or_tf: (1||0)=%d\n", 1 || 0);
    printf("or_ft: (0||1)=%d\n", 0 || 1);
    printf("or_ff: (0||0)=%d\n", 0 || 0);

    // Logical NOT
    printf("not_0: !0=%d\n", !0);
    printf("not_1: !1=%d\n", !1);
    printf("not_42: !42=%d\n", !42);
    printf("not_neg: !(-1)=%d\n", !(-1));

    // Short-circuit evaluation for AND
    a = 1;
    if (1 && a) {
        printf("sc_and: short-circuit AND: a=%d\n", a);
    }

    // Short-circuit: when LHS of && is false, RHS not evaluated
    a = 1; b = 0;
    if (0 && (b = 1)) {
        a = 99;
    }
    printf("sc_or: short-circuit OR: a=%d\n", a);
    printf("sc_and2: no side effect: b=%d\n", b);

    // Short-circuit: when LHS of || is true, RHS not evaluated
    b = 0;
    if (1 || (b = 1)) {
        // b should still be 0
    }
    printf("sc_or2: no side effect: b=%d\n", b);

    // Nested conditions
    printf("nested1: ((3>1)&&(5<10))=%d\n", (3 > 1) && (5 < 10));
    printf("nested2: ((3>1)&&(5>10))=%d\n", (3 > 1) && (5 > 10));
    printf("nested3: ((3>5)||(5<10))=%d\n", (3 > 5) || (5 < 10));
    printf("nested4: (!(3>5)&&(5<10))=%d\n", !(3 > 5) && (5 < 10));

    // Chained logical operators
    a = 1; b = 2; r = 3;
    printf("chain_and: (1&&2&&3)=%d\n", a && b && r);
    printf("chain_or: (0||0||1)=%d\n", 0 || 0 || 1);
    printf("chain_mixed: (1||0)&&(0||1)=%d\n", (1 || 0) && (0 || 1));

    // Complex combined
    printf("complex1: (3>1&&5>2)||(10<5)=%d\n", (3 > 1 && 5 > 2) || (10 < 5));
    printf("complex2: !(3>5||2>8)=%d\n", !(3 > 5 || 2 > 8));

    // Ternary operator
    r = (5 > 3) ? 10 : 20;
    printf("ternary1: (5>3)?10:20=%d\n", r);
    r = (5 < 3) ? 10 : 20;
    printf("ternary2: (5<3)?10:20=%d\n", r);

    // Nested ternary
    a = 5;
    r = (a > 10) ? 200 : (a > 3) ? 100 : 50;
    printf("ternary_nest: %d\n", r);

    // Truthiness of non-boolean values
    printf("truth_vals: 42 is true=%d\n", 42 ? 1 : 0);
    printf("truth_vals2: -1 is true=%d\n", -1 ? 1 : 0);

    // Comparison results as integers
    r = (1 < 2) + (2 < 3) + (3 < 4);
    printf("cmp_chain: (1<2)+(2<3)+(3<4)=%d\n", r);

    // Comparisons with expressions
    printf("eq_expr: (2+3==5)=%d\n", 2 + 3 == 5);
    printf("ne_expr: (2+3!=6)=%d\n", 2 + 3 != 6);

    return 0;
}
