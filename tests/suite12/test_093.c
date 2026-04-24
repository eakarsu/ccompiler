int printf(const char *fmt, ...);

// Switch with 100 cases

int map_value(int x) {
    switch (x) {
        case 0: return 1000;
        case 1: return 999;
        case 2: return 998;
        case 3: return 997;
        case 4: return 996;
        case 5: return 995;
        case 6: return 994;
        case 7: return 993;
        case 8: return 992;
        case 9: return 991;
        case 10: return 990;
        case 11: return 989;
        case 12: return 988;
        case 13: return 987;
        case 14: return 986;
        case 15: return 985;
        case 16: return 984;
        case 17: return 983;
        case 18: return 982;
        case 19: return 981;
        case 20: return 980;
        case 21: return 979;
        case 22: return 978;
        case 23: return 977;
        case 24: return 976;
        case 25: return 975;
        case 26: return 974;
        case 27: return 973;
        case 28: return 972;
        case 29: return 971;
        case 30: return 970;
        case 31: return 969;
        case 32: return 968;
        case 33: return 967;
        case 34: return 966;
        case 35: return 965;
        case 36: return 964;
        case 37: return 963;
        case 38: return 962;
        case 39: return 961;
        case 40: return 960;
        case 41: return 959;
        case 42: return 958;
        case 43: return 957;
        case 44: return 956;
        case 45: return 955;
        case 46: return 954;
        case 47: return 953;
        case 48: return 952;
        case 49: return 951;
        case 50: return 950;
        case 51: return 949;
        case 52: return 948;
        case 53: return 947;
        case 54: return 946;
        case 55: return 945;
        case 56: return 944;
        case 57: return 943;
        case 58: return 942;
        case 59: return 941;
        case 60: return 940;
        case 61: return 939;
        case 62: return 938;
        case 63: return 937;
        case 64: return 936;
        case 65: return 935;
        case 66: return 934;
        case 67: return 933;
        case 68: return 932;
        case 69: return 931;
        case 70: return 930;
        case 71: return 929;
        case 72: return 928;
        case 73: return 927;
        case 74: return 926;
        case 75: return 925;
        case 76: return 924;
        case 77: return 923;
        case 78: return 922;
        case 79: return 921;
        case 80: return 920;
        case 81: return 919;
        case 82: return 918;
        case 83: return 917;
        case 84: return 916;
        case 85: return 915;
        case 86: return 914;
        case 87: return 913;
        case 88: return 912;
        case 89: return 911;
        case 90: return 910;
        case 91: return 909;
        case 92: return 908;
        case 93: return 907;
        case 94: return 906;
        case 95: return 905;
        case 96: return 904;
        case 97: return 903;
        case 98: return 902;
        case 99: return 901;
        default: return -1;
    }
}

int categorize(int x) {
    int result;
    result = 0;
    switch (x % 10) {
        case 0: case 2: case 4: case 6: case 8:
            result = 1;
            break;
        case 1: case 3: case 5: case 7: case 9:
            result = 2;
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

int sum_mapped(int start, int end) {
    int i;
    int sum;
    sum = 0;
    for (i = start; i <= end; i = i + 1) {
        sum = sum + map_value(i);
    }
    return sum;
}

int main() {
    // Test specific cases
    printf("%d\n", map_value(0));   // EXPECT: 1000
    printf("%d\n", map_value(1));   // EXPECT: 999
    printf("%d\n", map_value(42));  // EXPECT: 958
    printf("%d\n", map_value(50));  // EXPECT: 950
    printf("%d\n", map_value(99));  // EXPECT: 901
    printf("%d\n", map_value(100)); // EXPECT: -1

    // Test sum of mapped values for range 0..9
    // 1000+999+998+997+996+995+994+993+992+991 = 9955
    printf("%d\n", sum_mapped(0, 9)); // EXPECT: 9955

    // Test sum of mapped values for range 90..99
    // 910+909+908+907+906+905+904+903+902+901 = 9055
    printf("%d\n", sum_mapped(90, 99)); // EXPECT: 9055

    // Test sum of all 100 mapped values: sum of 901..1000 = 100*1000 - sum(0..99) = 100*1000 - 4950 = 95050
    printf("%d\n", sum_mapped(0, 99)); // EXPECT: 95050

    // Categorize tests
    printf("%d\n", categorize(0));  // EXPECT: 1
    printf("%d\n", categorize(1));  // EXPECT: 2
    printf("%d\n", categorize(14)); // EXPECT: 1
    printf("%d\n", categorize(37)); // EXPECT: 2

    // Verify all even-mapped values
    int i;
    int even_sum;
    even_sum = 0;
    for (i = 0; i < 100; i = i + 2) {
        even_sum = even_sum + map_value(i);
    }
    // Even indices: 0,2,4,...,98 -> values 1000,998,996,...,902
    // 50 terms, first=1000, last=902, sum = 50*(1000+902)/2 = 50*951 = 47550
    printf("%d\n", even_sum); // EXPECT: 47550

    // Verify all odd-mapped values
    int odd_sum;
    odd_sum = 0;
    for (i = 1; i < 100; i = i + 2) {
        odd_sum = odd_sum + map_value(i);
    }
    // 95050 - 47550 = 47500
    printf("%d\n", odd_sum); // EXPECT: 47500

    // Count how many map to values > 950
    int count;
    count = 0;
    for (i = 0; i < 100; i = i + 1) {
        if (map_value(i) > 950) {
            count = count + 1;
        }
    }
    // values > 950: 1000 down to 951, that's cases 0..49, 50 cases
    printf("%d\n", count); // EXPECT: 50

    printf("%s\n", "done"); // EXPECT: done
    return 0;
}
