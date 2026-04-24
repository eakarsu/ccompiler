int printf(const char *fmt, ...);

/* test_014_switch_cases.c
   Tests: switch fall-through, switch with break, nested switch,
   switch with default, switch on chars, switch over ranges.
*/

/* Simple switch with break */
int day_type(int d) {
    switch (d) {
        case 1: return 1; /* Mon */
        case 2: return 1; /* Tue */
        case 3: return 1; /* Wed */
        case 4: return 1; /* Thu */
        case 5: return 1; /* Fri */
        case 6: return 2; /* Sat */
        case 7: return 2; /* Sun */
        default: return 0;
    }
}

/* Fall-through: multiple cases with same action */
int vowel_or_consonant(int c) {
    switch (c) {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            return 1; /* vowel */
        default:
            return 2; /* consonant or other */
    }
}

/* Fall-through with accumulation */
int fall_accum(int n) {
    int s = 0;
    switch (n) {
        case 4: s += 4;
        case 3: s += 3;
        case 2: s += 2;
        case 1: s += 1;
        case 0: break;
    }
    return s;
}
/* fall_accum(3) = 3+2+1 = 6 */
/* fall_accum(2) = 2+1 = 3 */
/* fall_accum(0) = 0 */

/* Switch with default in middle */
int mid_default(int n) {
    switch (n) {
        case 1: return 10;
        case 2: return 20;
        default: return -1;
        case 3: return 30;
        case 4: return 40;
    }
}
/* default is chosen when no case matches, regardless of position */

/* Nested switch */
int nested_switch(int outer, int inner) {
    int result = 0;
    switch (outer) {
        case 1:
            switch (inner) {
                case 10: result = 110; break;
                case 20: result = 120; break;
                default: result = 100; break;
            }
            break;
        case 2:
            switch (inner) {
                case 10: result = 210; break;
                case 20: result = 220; break;
                default: result = 200; break;
            }
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

/* Switch on char expressions */
int char_switch(int c) {
    switch (c) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return 1; /* digit */
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            return 2; /* upper hex */
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
            return 3; /* lower hex */
        default:
            return 0;
    }
}

/* Switch without default: no match does nothing */
int no_default(int n) {
    int x = 99;
    switch (n) {
        case 1: x = 1; break;
        case 2: x = 2; break;
    }
    return x;
}
/* no_default(5) = 99 (unchanged) */

/* Switch inside loop */
int switch_in_loop(void) {
    int s = 0;
    for (int i = 0; i <= 5; i++) {
        switch (i % 3) {
            case 0: s += 10; break;
            case 1: s += 1;  break;
            case 2: s += 100; break;
        }
    }
    return s;
    /* i=0: 0%3=0 s+=10
       i=1: 1%3=1 s+=1
       i=2: 2%3=2 s+=100
       i=3: 3%3=0 s+=10
       i=4: 4%3=1 s+=1
       i=5: 5%3=2 s+=100
       total = 10+1+100+10+1+100 = 222 */
}

/* Fall-through with break in last case */
int partial_fall(int n) {
    int result = 0;
    switch (n) {
        case 3:
            result += 100;
            /* fall through */
        case 2:
            result += 10;
            /* fall through */
        case 1:
            result += 1;
            break;
        case 0:
            result = 0;
            break;
    }
    return result;
}
/* partial_fall(3) = 100+10+1 = 111 */
/* partial_fall(2) = 10+1 = 11 */
/* partial_fall(1) = 1 */
/* partial_fall(0) = 0 */

int main(void) {
    /* day_type */
    printf("%d\n", day_type(1)); // EXPECT: 1
    printf("%d\n", day_type(5)); // EXPECT: 1
    printf("%d\n", day_type(6)); // EXPECT: 2
    printf("%d\n", day_type(7)); // EXPECT: 2
    printf("%d\n", day_type(8)); // EXPECT: 0

    /* vowel_or_consonant */
    printf("%d\n", vowel_or_consonant('a')); // EXPECT: 1
    printf("%d\n", vowel_or_consonant('e')); // EXPECT: 1
    printf("%d\n", vowel_or_consonant('b')); // EXPECT: 2
    printf("%d\n", vowel_or_consonant('z')); // EXPECT: 2

    /* fall_accum */
    printf("%d\n", fall_accum(0)); // EXPECT: 0
    printf("%d\n", fall_accum(2)); // EXPECT: 3
    printf("%d\n", fall_accum(3)); // EXPECT: 6
    printf("%d\n", fall_accum(4)); // EXPECT: 10

    /* mid_default */
    printf("%d\n", mid_default(1)); // EXPECT: 10
    printf("%d\n", mid_default(3)); // EXPECT: 30
    printf("%d\n", mid_default(4)); // EXPECT: 40
    printf("%d\n", mid_default(9)); // EXPECT: -1

    /* nested_switch */
    printf("%d\n", nested_switch(1, 10));  // EXPECT: 110
    printf("%d\n", nested_switch(1, 20));  // EXPECT: 120
    printf("%d\n", nested_switch(1, 99));  // EXPECT: 100
    printf("%d\n", nested_switch(2, 10));  // EXPECT: 210
    printf("%d\n", nested_switch(2, 20));  // EXPECT: 220
    printf("%d\n", nested_switch(9, 10));  // EXPECT: 0

    /* char_switch */
    printf("%d\n", char_switch('5')); // EXPECT: 1
    printf("%d\n", char_switch('A')); // EXPECT: 2
    printf("%d\n", char_switch('f')); // EXPECT: 3
    printf("%d\n", char_switch('z')); // EXPECT: 0

    /* no_default */
    printf("%d\n", no_default(1)); // EXPECT: 1
    printf("%d\n", no_default(5)); // EXPECT: 99

    /* switch_in_loop */
    printf("%d\n", switch_in_loop()); // EXPECT: 222

    /* partial_fall */
    printf("%d\n", partial_fall(3)); // EXPECT: 111
    printf("%d\n", partial_fall(2)); // EXPECT: 11
    printf("%d\n", partial_fall(1)); // EXPECT: 1
    printf("%d\n", partial_fall(0)); // EXPECT: 0

    return 0;
}
