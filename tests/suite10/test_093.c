int printf(const char *fmt, ...);

// Massive switch with 50+ cases

int classify(int x) {
    switch (x) {
        case 0: return 0;
        case 1: return 1;
        case 2: return 4;
        case 3: return 9;
        case 4: return 16;
        case 5: return 25;
        case 6: return 36;
        case 7: return 49;
        case 8: return 64;
        case 9: return 81;
        case 10: return 100;
        case 11: return 121;
        case 12: return 144;
        case 13: return 169;
        case 14: return 196;
        case 15: return 225;
        case 16: return 256;
        case 17: return 289;
        case 18: return 324;
        case 19: return 361;
        case 20: return 400;
        case 21: return 441;
        case 22: return 484;
        case 23: return 529;
        case 24: return 576;
        case 25: return 625;
        case 26: return 676;
        case 27: return 729;
        case 28: return 784;
        case 29: return 841;
        case 30: return 900;
        case 31: return 961;
        case 32: return 1024;
        case 33: return 1089;
        case 34: return 1156;
        case 35: return 1225;
        case 36: return 1296;
        case 37: return 1369;
        case 38: return 1444;
        case 39: return 1521;
        case 40: return 1600;
        case 41: return 1681;
        case 42: return 1764;
        case 43: return 1849;
        case 44: return 1936;
        case 45: return 2025;
        case 46: return 2116;
        case 47: return 2209;
        case 48: return 2304;
        case 49: return 2401;
        case 50: return 2500;
        default: return -1;
    }
}

int day_category(int day) {
    switch (day) {
        case 1: case 2: case 3: case 4: case 5:
        case 6: case 7: case 8: case 9: case 10:
            return 1;
        case 11: case 12: case 13: case 14: case 15:
        case 16: case 17: case 18: case 19: case 20:
            return 2;
        case 21: case 22: case 23: case 24: case 25:
        case 26: case 27: case 28: case 29: case 30:
        case 31:
            return 3;
        default:
            return 0;
    }
}

int char_type(int ch) {
    switch (ch) {
        case 48: case 49: case 50: case 51: case 52:
        case 53: case 54: case 55: case 56: case 57:
            return 1;
        case 65: case 66: case 67: case 68: case 69:
        case 70: case 71: case 72: case 73: case 74:
        case 75: case 76: case 77: case 78: case 79:
        case 80: case 81: case 82: case 83: case 84:
        case 85: case 86: case 87: case 88: case 89: case 90:
            return 2;
        case 97: case 98: case 99: case 100: case 101:
        case 102: case 103: case 104: case 105: case 106:
        case 107: case 108: case 109: case 110: case 111:
        case 112: case 113: case 114: case 115: case 116:
        case 117: case 118: case 119: case 120: case 121: case 122:
            return 3;
        case 32: case 9: case 10:
            return 4;
        default:
            return 0;
    }
}

int main() {
    printf("sq 0: %d\n", classify(0));
    // EXPECT: sq 0: 0
    printf("sq 7: %d\n", classify(7));
    // EXPECT: sq 7: 49
    printf("sq 15: %d\n", classify(15));
    // EXPECT: sq 15: 225
    printf("sq 25: %d\n", classify(25));
    // EXPECT: sq 25: 625
    printf("sq 50: %d\n", classify(50));
    // EXPECT: sq 50: 2500
    printf("sq 99: %d\n", classify(99));
    // EXPECT: sq 99: -1

    int sum = 0;
    int idx = 0;
    while (idx <= 50) {
        sum = sum + classify(idx);
        idx = idx + 1;
    }
    printf("sum of squares 0-50: %d\n", sum);
    // EXPECT: sum of squares 0-50: 42925

    printf("day 5: %d\n", day_category(5));
    // EXPECT: day 5: 1
    printf("day 15: %d\n", day_category(15));
    // EXPECT: day 15: 2
    printf("day 25: %d\n", day_category(25));
    // EXPECT: day 25: 3
    printf("day 50: %d\n", day_category(50));
    // EXPECT: day 50: 0

    printf("digit: %d\n", char_type(48));
    // EXPECT: digit: 1
    printf("upper: %d\n", char_type(65));
    // EXPECT: upper: 2
    printf("lower: %d\n", char_type(97));
    // EXPECT: lower: 3
    printf("space: %d\n", char_type(32));
    // EXPECT: space: 4
    printf("other: %d\n", char_type(33));
    // EXPECT: other: 0

    return 0;
}
