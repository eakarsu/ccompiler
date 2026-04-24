// stress_092_mega_switch.c - Mega switch torture test
// Exercises: 100-case switch, nested switch, switch in loops,
// fallthrough patterns, switch with computed values.
// EXPECT: mega100(0)=1
// EXPECT: mega100(1)=4
// EXPECT: mega100(10)=31
// EXPECT: mega100(25)=76
// EXPECT: mega100(50)=151
// EXPECT: mega100(75)=226
// EXPECT: mega100(99)=298
// EXPECT: mega100(-1)=-1
// EXPECT: mega100(100)=-1
// EXPECT: mega100(42)=127
// EXPECT: nested(0,0)=0
// EXPECT: nested(0,4)=4
// EXPECT: nested(5,2)=52
// EXPECT: nested(9,3)=93
// EXPECT: nested(15,1)=151
// EXPECT: nested(19,4)=194
// EXPECT: sum100=14950
// EXPECT: classify=15000
// EXPECT: all_pass=1

int printf(const char *fmt, ...);

// 100-case switch: each case returns case_num * 3 + 1
int mega100(int x) {
    switch (x) {
        case 0: return 0 * 3 + 1;
        case 1: return 1 * 3 + 1;
        case 2: return 2 * 3 + 1;
        case 3: return 3 * 3 + 1;
        case 4: return 4 * 3 + 1;
        case 5: return 5 * 3 + 1;
        case 6: return 6 * 3 + 1;
        case 7: return 7 * 3 + 1;
        case 8: return 8 * 3 + 1;
        case 9: return 9 * 3 + 1;
        case 10: return 10 * 3 + 1;
        case 11: return 11 * 3 + 1;
        case 12: return 12 * 3 + 1;
        case 13: return 13 * 3 + 1;
        case 14: return 14 * 3 + 1;
        case 15: return 15 * 3 + 1;
        case 16: return 16 * 3 + 1;
        case 17: return 17 * 3 + 1;
        case 18: return 18 * 3 + 1;
        case 19: return 19 * 3 + 1;
        case 20: return 20 * 3 + 1;
        case 21: return 21 * 3 + 1;
        case 22: return 22 * 3 + 1;
        case 23: return 23 * 3 + 1;
        case 24: return 24 * 3 + 1;
        case 25: return 25 * 3 + 1;
        case 26: return 26 * 3 + 1;
        case 27: return 27 * 3 + 1;
        case 28: return 28 * 3 + 1;
        case 29: return 29 * 3 + 1;
        case 30: return 30 * 3 + 1;
        case 31: return 31 * 3 + 1;
        case 32: return 32 * 3 + 1;
        case 33: return 33 * 3 + 1;
        case 34: return 34 * 3 + 1;
        case 35: return 35 * 3 + 1;
        case 36: return 36 * 3 + 1;
        case 37: return 37 * 3 + 1;
        case 38: return 38 * 3 + 1;
        case 39: return 39 * 3 + 1;
        case 40: return 40 * 3 + 1;
        case 41: return 41 * 3 + 1;
        case 42: return 42 * 3 + 1;
        case 43: return 43 * 3 + 1;
        case 44: return 44 * 3 + 1;
        case 45: return 45 * 3 + 1;
        case 46: return 46 * 3 + 1;
        case 47: return 47 * 3 + 1;
        case 48: return 48 * 3 + 1;
        case 49: return 49 * 3 + 1;
        case 50: return 50 * 3 + 1;
        case 51: return 51 * 3 + 1;
        case 52: return 52 * 3 + 1;
        case 53: return 53 * 3 + 1;
        case 54: return 54 * 3 + 1;
        case 55: return 55 * 3 + 1;
        case 56: return 56 * 3 + 1;
        case 57: return 57 * 3 + 1;
        case 58: return 58 * 3 + 1;
        case 59: return 59 * 3 + 1;
        case 60: return 60 * 3 + 1;
        case 61: return 61 * 3 + 1;
        case 62: return 62 * 3 + 1;
        case 63: return 63 * 3 + 1;
        case 64: return 64 * 3 + 1;
        case 65: return 65 * 3 + 1;
        case 66: return 66 * 3 + 1;
        case 67: return 67 * 3 + 1;
        case 68: return 68 * 3 + 1;
        case 69: return 69 * 3 + 1;
        case 70: return 70 * 3 + 1;
        case 71: return 71 * 3 + 1;
        case 72: return 72 * 3 + 1;
        case 73: return 73 * 3 + 1;
        case 74: return 74 * 3 + 1;
        case 75: return 75 * 3 + 1;
        case 76: return 76 * 3 + 1;
        case 77: return 77 * 3 + 1;
        case 78: return 78 * 3 + 1;
        case 79: return 79 * 3 + 1;
        case 80: return 80 * 3 + 1;
        case 81: return 81 * 3 + 1;
        case 82: return 82 * 3 + 1;
        case 83: return 83 * 3 + 1;
        case 84: return 84 * 3 + 1;
        case 85: return 85 * 3 + 1;
        case 86: return 86 * 3 + 1;
        case 87: return 87 * 3 + 1;
        case 88: return 88 * 3 + 1;
        case 89: return 89 * 3 + 1;
        case 90: return 90 * 3 + 1;
        case 91: return 91 * 3 + 1;
        case 92: return 92 * 3 + 1;
        case 93: return 93 * 3 + 1;
        case 94: return 94 * 3 + 1;
        case 95: return 95 * 3 + 1;
        case 96: return 96 * 3 + 1;
        case 97: return 97 * 3 + 1;
        case 98: return 98 * 3 + 1;
        case 99: return 99 * 3 + 1;
        default: return -1;
    }
}

// Nested switch: 20 outer * 5 inner cases
int nested_switch(int outer, int inner) {
    int result = -1;
    switch (outer) {
        case 0: switch(inner) { case 0: result=0; break; case 1: result=1; break; case 2: result=2; break; case 3: result=3; break; case 4: result=4; break; } break;
        case 1: switch(inner) { case 0: result=10; break; case 1: result=11; break; case 2: result=12; break; case 3: result=13; break; case 4: result=14; break; } break;
        case 2: switch(inner) { case 0: result=20; break; case 1: result=21; break; case 2: result=22; break; case 3: result=23; break; case 4: result=24; break; } break;
        case 3: switch(inner) { case 0: result=30; break; case 1: result=31; break; case 2: result=32; break; case 3: result=33; break; case 4: result=34; break; } break;
        case 4: switch(inner) { case 0: result=40; break; case 1: result=41; break; case 2: result=42; break; case 3: result=43; break; case 4: result=44; break; } break;
        case 5: switch(inner) { case 0: result=50; break; case 1: result=51; break; case 2: result=52; break; case 3: result=53; break; case 4: result=54; break; } break;
        case 6: switch(inner) { case 0: result=60; break; case 1: result=61; break; case 2: result=62; break; case 3: result=63; break; case 4: result=64; break; } break;
        case 7: switch(inner) { case 0: result=70; break; case 1: result=71; break; case 2: result=72; break; case 3: result=73; break; case 4: result=74; break; } break;
        case 8: switch(inner) { case 0: result=80; break; case 1: result=81; break; case 2: result=82; break; case 3: result=83; break; case 4: result=84; break; } break;
        case 9: switch(inner) { case 0: result=90; break; case 1: result=91; break; case 2: result=92; break; case 3: result=93; break; case 4: result=94; break; } break;
        case 10: switch(inner) { case 0: result=100; break; case 1: result=101; break; case 2: result=102; break; case 3: result=103; break; case 4: result=104; break; } break;
        case 11: switch(inner) { case 0: result=110; break; case 1: result=111; break; case 2: result=112; break; case 3: result=113; break; case 4: result=114; break; } break;
        case 12: switch(inner) { case 0: result=120; break; case 1: result=121; break; case 2: result=122; break; case 3: result=123; break; case 4: result=124; break; } break;
        case 13: switch(inner) { case 0: result=130; break; case 1: result=131; break; case 2: result=132; break; case 3: result=133; break; case 4: result=134; break; } break;
        case 14: switch(inner) { case 0: result=140; break; case 1: result=141; break; case 2: result=142; break; case 3: result=143; break; case 4: result=144; break; } break;
        case 15: switch(inner) { case 0: result=150; break; case 1: result=151; break; case 2: result=152; break; case 3: result=153; break; case 4: result=154; break; } break;
        case 16: switch(inner) { case 0: result=160; break; case 1: result=161; break; case 2: result=162; break; case 3: result=163; break; case 4: result=164; break; } break;
        case 17: switch(inner) { case 0: result=170; break; case 1: result=171; break; case 2: result=172; break; case 3: result=173; break; case 4: result=174; break; } break;
        case 18: switch(inner) { case 0: result=180; break; case 1: result=181; break; case 2: result=182; break; case 3: result=183; break; case 4: result=184; break; } break;
        case 19: switch(inner) { case 0: result=190; break; case 1: result=191; break; case 2: result=192; break; case 3: result=193; break; case 4: result=194; break; } break;
    }
    return result;
}

// Sum all case returns from 0..99 using the switch
int sum_all_cases(void) {
    int sum = 0;
    int i;
    for (i = 0; i < 100; i++) {
        sum = sum + i;  // sum of 0..99 = 4950
    }
    return sum;
}

// Classify numbers using switch ranges (using individual cases)
int classify(int x) {
    switch (x % 10) {
        case 0: case 2: case 4: case 6: case 8:
            return 1;  // even digit
        case 1: case 3: case 5: case 7: case 9:
            return 2;  // odd digit
        default:
            return 0;
    }
}

int main(void) {
    int pass = 1;

    // Test mega100 with 10 different inputs
    printf("mega100(0)=%d\n", mega100(0));
    if (mega100(0) != 1) pass = 0;
    printf("mega100(1)=%d\n", mega100(1));
    if (mega100(1) != 4) pass = 0;
    printf("mega100(10)=%d\n", mega100(10));
    if (mega100(10) != 31) pass = 0;
    printf("mega100(25)=%d\n", mega100(25));
    if (mega100(25) != 76) pass = 0;
    printf("mega100(50)=%d\n", mega100(50));
    if (mega100(50) != 151) pass = 0;
    printf("mega100(75)=%d\n", mega100(75));
    if (mega100(75) != 226) pass = 0;
    printf("mega100(99)=%d\n", mega100(99));
    if (mega100(99) != 298) pass = 0;
    printf("mega100(-1)=%d\n", mega100(-1));
    if (mega100(-1) != -1) pass = 0;
    printf("mega100(100)=%d\n", mega100(100));
    if (mega100(100) != -1) pass = 0;
    printf("mega100(42)=%d\n", mega100(42));
    if (mega100(42) != 127) pass = 0;

    // Test nested switch
    printf("nested(0,0)=%d\n", nested_switch(0, 0));
    if (nested_switch(0, 0) != 0) pass = 0;
    printf("nested(0,4)=%d\n", nested_switch(0, 4));
    if (nested_switch(0, 4) != 4) pass = 0;
    printf("nested(5,2)=%d\n", nested_switch(5, 2));
    if (nested_switch(5, 2) != 52) pass = 0;
    printf("nested(9,3)=%d\n", nested_switch(9, 3));
    if (nested_switch(9, 3) != 93) pass = 0;
    printf("nested(15,1)=%d\n", nested_switch(15, 1));
    if (nested_switch(15, 1) != 151) pass = 0;
    printf("nested(19,4)=%d\n", nested_switch(19, 4));
    if (nested_switch(19, 4) != 194) pass = 0;

    // Sum all case values: sum(0..99)*3 + 100 = 4950*3 + 100 = 14950
    // Actually sum_all_cases returns sum of 0..99 = 4950
    // But let's compute via switch: sum of (i*3+1) for i=0..99 = 3*(0+1+..+99)+100 = 3*4950+100 = 14950
    int s = 0;
    int i;
    for (i = 0; i < 100; i++) {
        s = s + mega100(i);
    }
    printf("sum100=%d\n", s);
    if (s != 14950) pass = 0;

    // Classify test
    int csum = 0;
    for (i = 0; i < 10000; i++) {
        csum = csum + classify(i);
    }
    // 5000 even-digit numbers -> 5000*1 = 5000, 5000 odd-digit -> 5000*2 = 10000
    // Wait, i%10: 0,2,4,6,8 are even (5 values), 1,3,5,7,9 are odd (5 values)
    // For i=0..9999, each digit 0-9 appears 1000 times
    // sum = 1000*5*1 + 1000*5*2 = 5000 + 10000 = 15000
    // Hmm let me just compute
    printf("classify=%d\n", csum);
    if (csum != 15000) pass = 0;

    printf("all_pass=%d\n", pass);
    return 0;
}
