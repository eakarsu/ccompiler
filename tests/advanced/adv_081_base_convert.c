// EXPECT: === Base Conversion Tests ===
// EXPECT: dec2bin(0) = 0
// EXPECT: dec2bin(1) = 1
// EXPECT: dec2bin(5) = 101
// EXPECT: dec2bin(42) = 101010
// EXPECT: dec2bin(255) = 11111111
// EXPECT: dec2bin(1024) = 10000000000
// EXPECT: dec2hex(0) = 0
// EXPECT: dec2hex(10) = A
// EXPECT: dec2hex(255) = FF
// EXPECT: dec2hex(4096) = 1000
// EXPECT: dec2hex(48879) = BEEF
// EXPECT: dec2hex(65535) = FFFF
// EXPECT: bin2dec(0) = 0
// EXPECT: bin2dec(1) = 1
// EXPECT: bin2dec(101) = 5
// EXPECT: bin2dec(101010) = 42
// EXPECT: bin2dec(11111111) = 255
// EXPECT: hex2dec(0) = 0
// EXPECT: hex2dec(A) = 10
// EXPECT: hex2dec(FF) = 255
// EXPECT: hex2dec(BEEF) = 48879
// EXPECT: hex2dec(1000) = 4096
// EXPECT: dec2oct(0) = 0
// EXPECT: dec2oct(8) = 10
// EXPECT: dec2oct(64) = 100
// EXPECT: dec2oct(255) = 377
// EXPECT: dec2oct(511) = 777
// EXPECT: oct2dec(0) = 0
// EXPECT: oct2dec(10) = 8
// EXPECT: oct2dec(100) = 64
// EXPECT: oct2dec(377) = 255
// EXPECT: oct2dec(777) = 511
// EXPECT: All base conversion tests passed!

int printf(const char *fmt, ...);

void dec2bin(int n, char *buf) {
    if (n == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    char tmp[33];
    int i = 0;
    int val = n;
    while (val > 0) {
        tmp[i] = (val % 2) + '0';
        val = val / 2;
        i = i + 1;
    }
    int j;
    for (j = 0; j < i; j++) {
        buf[j] = tmp[i - 1 - j];
    }
    buf[i] = '\0';
}

void dec2hex(int n, char *buf) {
    if (n == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    char hexchars[17];
    hexchars[0] = '0'; hexchars[1] = '1'; hexchars[2] = '2'; hexchars[3] = '3';
    hexchars[4] = '4'; hexchars[5] = '5'; hexchars[6] = '6'; hexchars[7] = '7';
    hexchars[8] = '8'; hexchars[9] = '9'; hexchars[10] = 'A'; hexchars[11] = 'B';
    hexchars[12] = 'C'; hexchars[13] = 'D'; hexchars[14] = 'E'; hexchars[15] = 'F';
    hexchars[16] = '\0';
    char tmp[20];
    int i = 0;
    int val = n;
    while (val > 0) {
        tmp[i] = hexchars[val % 16];
        val = val / 16;
        i = i + 1;
    }
    int j;
    for (j = 0; j < i; j++) {
        buf[j] = tmp[i - 1 - j];
    }
    buf[i] = '\0';
}

int bin2dec(char *bin) {
    int result = 0;
    int i = 0;
    while (bin[i] != '\0') {
        result = result * 2 + (bin[i] - '0');
        i = i + 1;
    }
    return result;
}

int hex2dec(char *hex) {
    int result = 0;
    int i = 0;
    while (hex[i] != '\0') {
        char c = hex[i];
        int val;
        if (c >= '0' && c <= '9') {
            val = c - '0';
        } else if (c >= 'A' && c <= 'F') {
            val = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'f') {
            val = c - 'a' + 10;
        } else {
            val = 0;
        }
        result = result * 16 + val;
        i = i + 1;
    }
    return result;
}

void dec2oct(int n, char *buf) {
    if (n == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    char tmp[20];
    int i = 0;
    int val = n;
    while (val > 0) {
        tmp[i] = (val % 8) + '0';
        val = val / 8;
        i = i + 1;
    }
    int j;
    for (j = 0; j < i; j++) {
        buf[j] = tmp[i - 1 - j];
    }
    buf[i] = '\0';
}

int oct2dec(char *oct) {
    int result = 0;
    int i = 0;
    while (oct[i] != '\0') {
        result = result * 8 + (oct[i] - '0');
        i = i + 1;
    }
    return result;
}

int main() {
    char buf[64];

    printf("=== Base Conversion Tests ===\n");

    dec2bin(0, buf);    printf("dec2bin(0) = %s\n", buf);
    dec2bin(1, buf);    printf("dec2bin(1) = %s\n", buf);
    dec2bin(5, buf);    printf("dec2bin(5) = %s\n", buf);
    dec2bin(42, buf);   printf("dec2bin(42) = %s\n", buf);
    dec2bin(255, buf);  printf("dec2bin(255) = %s\n", buf);
    dec2bin(1024, buf); printf("dec2bin(1024) = %s\n", buf);

    dec2hex(0, buf);     printf("dec2hex(0) = %s\n", buf);
    dec2hex(10, buf);    printf("dec2hex(10) = %s\n", buf);
    dec2hex(255, buf);   printf("dec2hex(255) = %s\n", buf);
    dec2hex(4096, buf);  printf("dec2hex(4096) = %s\n", buf);
    dec2hex(48879, buf); printf("dec2hex(48879) = %s\n", buf);
    dec2hex(65535, buf); printf("dec2hex(65535) = %s\n", buf);

    printf("bin2dec(0) = %d\n", bin2dec("0"));
    printf("bin2dec(1) = %d\n", bin2dec("1"));
    printf("bin2dec(101) = %d\n", bin2dec("101"));
    printf("bin2dec(101010) = %d\n", bin2dec("101010"));
    printf("bin2dec(11111111) = %d\n", bin2dec("11111111"));

    printf("hex2dec(0) = %d\n", hex2dec("0"));
    printf("hex2dec(A) = %d\n", hex2dec("A"));
    printf("hex2dec(FF) = %d\n", hex2dec("FF"));
    printf("hex2dec(BEEF) = %d\n", hex2dec("BEEF"));
    printf("hex2dec(1000) = %d\n", hex2dec("1000"));

    dec2oct(0, buf);   printf("dec2oct(0) = %s\n", buf);
    dec2oct(8, buf);   printf("dec2oct(8) = %s\n", buf);
    dec2oct(64, buf);  printf("dec2oct(64) = %s\n", buf);
    dec2oct(255, buf); printf("dec2oct(255) = %s\n", buf);
    dec2oct(511, buf); printf("dec2oct(511) = %s\n", buf);

    printf("oct2dec(0) = %d\n", oct2dec("0"));
    printf("oct2dec(10) = %d\n", oct2dec("10"));
    printf("oct2dec(100) = %d\n", oct2dec("100"));
    printf("oct2dec(377) = %d\n", oct2dec("377"));
    printf("oct2dec(777) = %d\n", oct2dec("777"));

    printf("All base conversion tests passed!\n");
    return 0;
}
