int printf(const char *fmt, ...);
// EXPECT: nested:123\nfall:1234\nfall:34\nc0 c5 c10 c15 c20 c25 c29 \nchar:A\nchar:z\nchar:digit\nchar:other\ncolor:red\ncolor:green\ncolor:blue\ncolor:yellow\nmid:one\nmid:two\nmid:default\nmid:three\ngroup:A\ngroup:A\ngroup:A\ngroup:B\ngroup:B\ngroup:C\ngroup:other\nshadow:outer1inner2\nL0 L1 L2 L0 L1 \nexpr:two\nexpr:A\nexpr:seven\nexpr:other\nonly:default\nonly:default\nblock:10\nblock:20\ndone

enum Color { RED = 0, GREEN = 1, BLUE = 2, YELLOW = 3 };

/*
 * Test 1: 3-level nested switch statements.
 * The compiler had a bug where an inner switch clobbered the outer
 * switch context, so we verify all three levels dispatch correctly.
 */
void test_nested_3level(void) {
    int outer = 1;
    int mid = 2;
    int inner = 3;
    int r1 = -1, r2 = -1, r3 = -1;

    switch (outer) {
        case 0:
            r1 = 0;
            switch (mid) {
                case 0:
                    r2 = 0;
                    break;
                default:
                    r2 = 9;
                    break;
            }
            break;
        case 1:
            r1 = 1;
            switch (mid) {
                case 1:
                    r2 = 1;
                    break;
                case 2:
                    r2 = 2;
                    switch (inner) {
                        case 1:
                            r3 = 1;
                            break;
                        case 2:
                            r3 = 2;
                            break;
                        case 3:
                            r3 = 3;
                            break;
                        default:
                            r3 = 9;
                            break;
                    }
                    break;
                default:
                    r2 = 9;
                    break;
            }
            break;
        case 2:
            r1 = 2;
            break;
        default:
            r1 = 9;
            break;
    }
    printf("nested:%d%d%d\n", r1, r2, r3);
}

/*
 * Test 2: Fallthrough — no break between cases.
 * val=1 should fall through case 1 → 2 → 3 → default.
 * val=3 should fall through case 3 → default.
 */
void test_fallthrough(int val) {
    printf("fall:");
    switch (val) {
        case 1:
            printf("1");
        case 2:
            printf("2");
        case 3:
            printf("3");
        default:
            printf("4");
    }
    printf("\n");
}

/*
 * Test 3: 30+ cases — potential jump table optimization.
 * Returns the case number that was hit; we call with selected values.
 */
void test_many_cases(int val) {
    int result = -1;
    switch (val) {
        case 0:  result = 0;  break;
        case 1:  result = 1;  break;
        case 2:  result = 2;  break;
        case 3:  result = 3;  break;
        case 4:  result = 4;  break;
        case 5:  result = 5;  break;
        case 6:  result = 6;  break;
        case 7:  result = 7;  break;
        case 8:  result = 8;  break;
        case 9:  result = 9;  break;
        case 10: result = 10; break;
        case 11: result = 11; break;
        case 12: result = 12; break;
        case 13: result = 13; break;
        case 14: result = 14; break;
        case 15: result = 15; break;
        case 16: result = 16; break;
        case 17: result = 17; break;
        case 18: result = 18; break;
        case 19: result = 19; break;
        case 20: result = 20; break;
        case 21: result = 21; break;
        case 22: result = 22; break;
        case 23: result = 23; break;
        case 24: result = 24; break;
        case 25: result = 25; break;
        case 26: result = 26; break;
        case 27: result = 27; break;
        case 28: result = 28; break;
        case 29: result = 29; break;
        case 30: result = 30; break;
        default: result = -1; break;
    }
    printf("c%d ", result);
}

/*
 * Test 4: Switch on char value.
 */
void test_switch_char(char c) {
    switch (c) {
        case 'A':
            printf("char:A\n");
            break;
        case 'z':
            printf("char:z\n");
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            printf("char:digit\n");
            break;
        default:
            printf("char:other\n");
            break;
    }
}

/*
 * Test 5: Switch on enum value.
 */
void test_switch_enum(enum Color c) {
    switch (c) {
        case RED:
            printf("color:red\n");
            break;
        case GREEN:
            printf("color:green\n");
            break;
        case BLUE:
            printf("color:blue\n");
            break;
        case YELLOW:
            printf("color:yellow\n");
            break;
    }
}

/*
 * Test 6: Default case in the middle of switch, not at end.
 */
void test_default_middle(int val) {
    switch (val) {
        case 1:
            printf("mid:one\n");
            break;
        default:
            printf("mid:default\n");
            break;
        case 2:
            printf("mid:two\n");
            break;
        case 3:
            printf("mid:three\n");
            break;
    }
}

/*
 * Test 7: Empty cases that fall through to a shared handler.
 * Cases 1, 2, 3 → group A; cases 4, 5 → group B; case 6 → group C.
 */
void test_empty_fallthrough(int val) {
    switch (val) {
        case 1:
        case 2:
        case 3:
            printf("group:A\n");
            break;
        case 4:
        case 5:
            printf("group:B\n");
            break;
        case 6:
            printf("group:C\n");
            break;
        default:
            printf("group:other\n");
            break;
    }
}

/*
 * Test 8: Nested switch where inner and outer both switch on a
 * variable with the same name ("x"), testing that the inner switch
 * does not clobber the outer switch context.
 */
void test_shadow(void) {
    int x = 1;
    int r1 = -1, r2 = -1;
    switch (x) {
        case 0:
            r1 = 0;
            break;
        case 1:
            r1 = 1;
            {
                int x = 2;
                switch (x) {
                    case 0:
                        r2 = 0;
                        break;
                    case 1:
                        r2 = 1;
                        break;
                    case 2:
                        r2 = 2;
                        break;
                    default:
                        r2 = 9;
                        break;
                }
            }
            break;
        case 2:
            r1 = 2;
            break;
        default:
            r1 = 9;
            break;
    }
    printf("shadow:outer%dinner%d\n", r1, r2);
}

/*
 * Test 9: Switch inside a loop.
 * break should exit the switch, NOT the loop.
 * Loop runs i = 0..4, switch on i%3.
 */
void test_switch_in_loop(void) {
    int i;
    for (i = 0; i < 5; i++) {
        switch (i % 3) {
            case 0:
                printf("L0 ");
                break;
            case 1:
                printf("L1 ");
                break;
            case 2:
                printf("L2 ");
                break;
        }
    }
    printf("\n");
}

/*
 * Test 10: Case values that are constant expressions.
 */
void test_case_expr(int val) {
    switch (val) {
        case 1 + 1:
            printf("expr:two\n");
            break;
        case 'A':
            printf("expr:A\n");
            break;
        case 3 + 4:
            printf("expr:seven\n");
            break;
        default:
            printf("expr:other\n");
            break;
    }
}

/*
 * Test 11: Switch with only a default case.
 */
void test_only_default(int val) {
    switch (val) {
        default:
            printf("only:default\n");
            break;
    }
}

/*
 * Test 12: Switch with block scope declaring local variables.
 */
void test_block_scope(int val) {
    switch (val) {
        case 1: {
            int x = 10;
            printf("block:%d\n", x);
            break;
        }
        case 2: {
            int x = 20;
            printf("block:%d\n", x);
            break;
        }
        default: {
            int x = 0;
            printf("block:%d\n", x);
            break;
        }
    }
}

int main(void) {
    test_nested_3level();
    test_fallthrough(1);
    test_fallthrough(3);
    test_many_cases(0);  test_many_cases(5);  test_many_cases(10);
    test_many_cases(15); test_many_cases(20); test_many_cases(25);
    test_many_cases(29); printf("\n");
    test_switch_char('A');
    test_switch_char('z');
    test_switch_char('5');
    test_switch_char('?');
    test_switch_enum(RED);
    test_switch_enum(GREEN);
    test_switch_enum(BLUE);
    test_switch_enum(YELLOW);
    test_default_middle(1);
    test_default_middle(2);
    test_default_middle(99);
    test_default_middle(3);
    test_empty_fallthrough(1);
    test_empty_fallthrough(2);
    test_empty_fallthrough(3);
    test_empty_fallthrough(4);
    test_empty_fallthrough(5);
    test_empty_fallthrough(6);
    test_empty_fallthrough(7);
    test_shadow();
    test_switch_in_loop();
    test_case_expr(2);
    test_case_expr(65);
    test_case_expr(7);
    test_case_expr(0);
    test_only_default(42);
    test_only_default(0);
    test_block_scope(1);
    test_block_scope(2);
    printf("done\n");
    return 0;
}
