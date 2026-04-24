int printf(const char *fmt, ...);

// Test: Cast chains (int to char to int) and various cast patterns

int main() {
    int i;
    char c;
    int result;

    // Basic int to char truncation
    i = 300;
    c = (char)i;
    result = (int)c;
    printf("%d\n", result); // EXPECT: 44

    // Chain: int -> char -> int
    i = 1000;
    result = (int)(char)i;
    printf("%d\n", result); // EXPECT: -24

    // Negative int to char to int
    i = -50;
    c = (char)i;
    result = (int)c;
    printf("%d\n", result); // EXPECT: -50

    // Double cast preserving value in range
    i = 65;
    result = (int)(char)(int)i;
    printf("%d\n", result); // EXPECT: 65

    // Cast chain with arithmetic
    i = 256 + 10;
    c = (char)i;
    result = (int)c + 100;
    printf("%d\n", result); // EXPECT: 110

    // Sign extension through char
    i = 200;
    c = (char)i;
    result = (int)c;
    printf("%d\n", result); // EXPECT: -56

    // Cast in conditional
    i = 256;
    if ((char)i == 0) {
        printf("%d\n", 1); // EXPECT: 1
    }

    // Multiple casts in expression
    int a;
    int b;
    a = 300;
    b = 400;
    result = (int)(char)a + (int)(char)b;
    // (char)300 = 44, (char)400 = -112 (400 & 0xFF = 144, signed = -112)
    printf("%d\n", result); // EXPECT: -68

    // Cast chain with multiplication
    i = 50;
    result = (int)(char)(i * 6);
    // 50*6 = 300, (char)300 = 44
    printf("%d\n", result); // EXPECT: 44

    // Preserving sign through casts
    i = -1;
    c = (char)i;
    result = (int)c;
    printf("%d\n", result); // EXPECT: -1

    // Cast to char masks to 8 bits
    i = 0;
    while (i < 5) {
        char ch;
        ch = (char)(i * 60);
        printf("%d\n", (int)ch);
        i = i + 1;
    }
    // i=0: 0, i=1: 60, i=2: 120, i=3: (char)180=-76, i=4: (char)240=-16
    // EXPECT: 0
    // EXPECT: 60
    // EXPECT: 120
    // EXPECT: -76
    // EXPECT: -16

    // Cast in function arguments
    i = 321;
    printf("%d\n", (int)(char)i); // EXPECT: 65

    // Cast result used as index
    int arr[4];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    i = 258;
    result = arr[(int)(char)i];
    // (char)258 = 2
    printf("%d\n", result); // EXPECT: 30

    // Cast to char comparison
    i = 256;
    if ((int)(char)i == 0) {
        printf("%d\n", 1); // EXPECT: 1
    }

    // Cast chain in ternary
    i = 128;
    result = ((int)(char)i < 0) ? -1 : 1;
    // (char)128 = -128, so < 0
    printf("%d\n", result); // EXPECT: -1

    // Sequential casts accumulating
    int sum;
    sum = 0;
    for (i = 0; i < 8; i = i + 1) {
        sum = sum + (int)(char)(i * 40);
    }
    // 0, 40, 80, 120, (char)160=-96, (char)200=-56, (char)240=-16, (char)280=24
    // sum = 0+40+80+120+(-96)+(-56)+(-16)+24 = 96
    printf("%d\n", sum); // EXPECT: 96

    // Char pointer cast read
    int val;
    val = 0x41424344;
    char *cp;
    cp = (char *)&val;
    printf("%d\n", (int)cp[0]); // EXPECT: 68

    // Double truncation
    i = 100000;
    result = (int)(char)(int)(char)i;
    // (char)100000: 100000 & 0xFF = 160, signed = -96
    // (int)-96 = -96, (char)-96 = -96, (int)-96 = -96
    printf("%d\n", result); // EXPECT: -96

    return 0;
}
