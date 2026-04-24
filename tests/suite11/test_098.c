int printf(const char *fmt, ...);

// Test: Large local arrays (500+ elements)

int main() {
    int big[512];
    int i;
    int sum;

    // Fill large array with pattern
    for (i = 0; i < 512; i = i + 1) {
        big[i] = i;
    }

    // Sum first 100 elements: 0+1+...+99 = 4950
    sum = 0;
    for (i = 0; i < 100; i = i + 1) {
        sum = sum + big[i];
    }
    printf("%d\n", sum); // EXPECT: 4950

    // Sum last 10 elements: 502+503+...+511 = 5065
    sum = 0;
    for (i = 502; i < 512; i = i + 1) {
        sum = sum + big[i];
    }
    printf("%d\n", sum); // EXPECT: 5065

    // Verify specific elements
    printf("%d\n", big[0]);   // EXPECT: 0
    printf("%d\n", big[255]); // EXPECT: 255
    printf("%d\n", big[511]); // EXPECT: 511

    // Large char array
    char cbuf[600];
    for (i = 0; i < 600; i = i + 1) {
        cbuf[i] = (char)(i % 26 + 65);
    }
    // Check some values (A=65, B=66, ...)
    printf("%d\n", (int)cbuf[0]);  // EXPECT: 65
    printf("%d\n", (int)cbuf[25]); // EXPECT: 90
    printf("%d\n", (int)cbuf[26]); // EXPECT: 65

    // Sum first 26 chars (A-Z: 65+66+...+90 = 2015)
    sum = 0;
    for (i = 0; i < 26; i = i + 1) {
        sum = sum + (int)cbuf[i];
    }
    printf("%d\n", sum); // EXPECT: 2015

    // Second large array
    int arr2[500];
    for (i = 0; i < 500; i = i + 1) {
        arr2[i] = i * 2;
    }
    printf("%d\n", arr2[0]);   // EXPECT: 0
    printf("%d\n", arr2[249]); // EXPECT: 498
    printf("%d\n", arr2[499]); // EXPECT: 998

    // Sum of arr2[0..9] = 0+2+4+6+8+10+12+14+16+18 = 90
    sum = 0;
    for (i = 0; i < 10; i = i + 1) {
        sum = sum + arr2[i];
    }
    printf("%d\n", sum); // EXPECT: 90

    // Reverse a portion of big array
    int temp;
    for (i = 0; i < 50; i = i + 1) {
        temp = big[i];
        big[i] = big[99 - i];
        big[99 - i] = temp;
    }
    printf("%d\n", big[0]);  // EXPECT: 99
    printf("%d\n", big[99]); // EXPECT: 0
    printf("%d\n", big[50]); // EXPECT: 49

    // Bubble sort small portion of big array (elements 0..9)
    // Currently big[0..9] = 99,98,97,...,90
    int j;
    for (i = 0; i < 10; i = i + 1) {
        for (j = 0; j < 9 - i; j = j + 1) {
            if (big[j] > big[j + 1]) {
                temp = big[j];
                big[j] = big[j + 1];
                big[j + 1] = temp;
            }
        }
    }
    printf("%d\n", big[0]); // EXPECT: 90
    printf("%d\n", big[9]); // EXPECT: 99

    // Use large array as histogram
    int hist[256];
    for (i = 0; i < 256; i = i + 1) {
        hist[i] = 0;
    }

    // Count char frequencies in cbuf[0..99]
    for (i = 0; i < 100; i = i + 1) {
        int ch;
        ch = (int)cbuf[i];
        hist[ch] = hist[ch] + 1;
    }
    // 100/26 = 3 full cycles (78 chars) + 22 remainder
    // A(65) through V(86) appear 4 times, W(87) through Z(90) appear 3 times
    printf("%d\n", hist[65]); // EXPECT: 4
    printf("%d\n", hist[86]); // EXPECT: 4
    printf("%d\n", hist[87]); // EXPECT: 3
    printf("%d\n", hist[90]); // EXPECT: 3

    // Verify total count
    sum = 0;
    for (i = 0; i < 256; i = i + 1) {
        sum = sum + hist[i];
    }
    printf("%d\n", sum); // EXPECT: 100

    // Copy between large arrays
    int dst[512];
    for (i = 0; i < 512; i = i + 1) {
        dst[i] = big[i] + 1;
    }
    printf("%d\n", dst[0]);   // EXPECT: 91
    printf("%d\n", dst[100]); // EXPECT: 101
    printf("%d\n", dst[511]); // EXPECT: 512

    return 0;
}
