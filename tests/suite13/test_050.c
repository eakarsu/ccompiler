int printf(const char *fmt, ...);

// Binary representation printer and converter

// Write binary representation of val (nbits bits) into buf
// buf must have room for nbits+1 chars
void to_binary(int val, int nbits, char *buf) {
    int i = 0;
    while (i < nbits) {
        int bit = (val >> (nbits - 1 - i)) & 1;
        if (bit) {
            buf[i] = '1';
        } else {
            buf[i] = '0';
        }
        i = i + 1;
    }
    buf[nbits] = 0;
}

// Convert binary string to integer
int from_binary(char *buf) {
    int result = 0;
    int i = 0;
    while (buf[i] != 0) {
        result = result << 1;
        if (buf[i] == '1') {
            result = result | 1;
        }
        i = i + 1;
    }
    return result;
}

// Write octal representation into buf (max 11 digits for 32-bit)
void to_octal(int val, char *buf) {
    if (val == 0) {
        buf[0] = '0';
        buf[1] = 0;
        return;
    }
    char tmp[12];
    int i = 0;
    int v = val;
    while (v > 0 && i < 11) {
        int digit = v & 7;
        tmp[i] = '0' + digit;
        v = (v >> 3) & 0x1FFFFFFF;
        i = i + 1;
    }
    // Reverse into buf
    int j = 0;
    while (j < i) {
        buf[j] = tmp[i - 1 - j];
        j = j + 1;
    }
    buf[i] = 0;
}

// Write hexadecimal representation into buf
void to_hex(int val, char *buf) {
    if (val == 0) {
        buf[0] = '0';
        buf[1] = 0;
        return;
    }
    char hex_chars[17];
    hex_chars[0] = '0'; hex_chars[1] = '1'; hex_chars[2] = '2'; hex_chars[3] = '3';
    hex_chars[4] = '4'; hex_chars[5] = '5'; hex_chars[6] = '6'; hex_chars[7] = '7';
    hex_chars[8] = '8'; hex_chars[9] = '9'; hex_chars[10] = 'a'; hex_chars[11] = 'b';
    hex_chars[12] = 'c'; hex_chars[13] = 'd'; hex_chars[14] = 'e'; hex_chars[15] = 'f';
    hex_chars[16] = 0;

    char tmp[9];
    int i = 0;
    int v = val;
    while (v > 0 && i < 8) {
        int digit = v & 0xF;
        tmp[i] = hex_chars[digit];
        v = (v >> 4) & 0x0FFFFFFF;
        i = i + 1;
    }
    int j = 0;
    while (j < i) {
        buf[j] = tmp[i - 1 - j];
        j = j + 1;
    }
    buf[i] = 0;
}

// Count number of '1' characters in a binary string
int count_ones_str(char *buf) {
    int count = 0;
    int i = 0;
    while (buf[i] != 0) {
        if (buf[i] == '1') {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

// Check if binary string is a palindrome
int is_binary_palindrome(char *buf) {
    int len = 0;
    while (buf[len] != 0) {
        len = len + 1;
    }
    int i = 0;
    while (i < len / 2) {
        if (buf[i] != buf[len - 1 - i]) {
            return 0;
        }
        i = i + 1;
    }
    return 1;
}

// Compare two binary strings
int binary_strcmp(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) {
            return a[i] - b[i];
        }
        i = i + 1;
    }
    if (a[i] == 0 && b[i] == 0) return 0;
    if (a[i] == 0) return -1;
    return 1;
}

// Binary addition of two binary strings (fixed width)
int binary_add_str(char *a, char *b, int width) {
    int va = from_binary(a);
    int vb = from_binary(b);
    int sum = va + vb;
    int mask = (1 << width) - 1;
    return sum & mask;
}

int main() {
    char buf[33];

    // to_binary tests
    to_binary(0, 4, buf);
    printf("b0=%s\n", buf);
    // EXPECT: b0=0000
    to_binary(1, 4, buf);
    printf("b1=%s\n", buf);
    // EXPECT: b1=0001
    to_binary(5, 4, buf);
    printf("b5=%s\n", buf);
    // EXPECT: b5=0101
    to_binary(15, 4, buf);
    printf("b15=%s\n", buf);
    // EXPECT: b15=1111
    to_binary(255, 8, buf);
    printf("b255=%s\n", buf);
    // EXPECT: b255=11111111
    to_binary(170, 8, buf);
    printf("b170=%s\n", buf);
    // EXPECT: b170=10101010
    to_binary(1, 8, buf);
    printf("b1x8=%s\n", buf);
    // EXPECT: b1x8=00000001

    // from_binary tests
    printf("fb0=%d\n", from_binary("0000"));
    // EXPECT: fb0=0
    printf("fb1=%d\n", from_binary("0001"));
    // EXPECT: fb1=1
    printf("fb5=%d\n", from_binary("0101"));
    // EXPECT: fb5=5
    printf("fb15=%d\n", from_binary("1111"));
    // EXPECT: fb15=15
    printf("fb170=%d\n", from_binary("10101010"));
    // EXPECT: fb170=170
    printf("fb255=%d\n", from_binary("11111111"));
    // EXPECT: fb255=255

    // roundtrip: to_binary then from_binary
    to_binary(42, 8, buf);
    printf("rt42=%d\n", from_binary(buf));
    // EXPECT: rt42=42
    to_binary(100, 8, buf);
    printf("rt100=%d\n", from_binary(buf));
    // EXPECT: rt100=100

    // to_octal tests
    to_octal(0, buf);
    printf("o0=%s\n", buf);
    // EXPECT: o0=0
    to_octal(7, buf);
    printf("o7=%s\n", buf);
    // EXPECT: o7=7
    to_octal(8, buf);
    printf("o8=%s\n", buf);
    // EXPECT: o8=10
    to_octal(64, buf);
    printf("o64=%s\n", buf);
    // EXPECT: o64=100
    to_octal(255, buf);
    printf("o255=%s\n", buf);
    // EXPECT: o255=377
    to_octal(511, buf);
    printf("o511=%s\n", buf);
    // EXPECT: o511=777

    // to_hex tests
    to_hex(0, buf);
    printf("h0=%s\n", buf);
    // EXPECT: h0=0
    to_hex(15, buf);
    printf("h15=%s\n", buf);
    // EXPECT: h15=f
    to_hex(16, buf);
    printf("h16=%s\n", buf);
    // EXPECT: h16=10
    to_hex(255, buf);
    printf("h255=%s\n", buf);
    // EXPECT: h255=ff
    to_hex(256, buf);
    printf("h256=%s\n", buf);
    // EXPECT: h256=100
    to_hex(48879, buf);
    printf("hbeef=%s\n", buf);
    // EXPECT: hbeef=beef

    // count_ones_str
    to_binary(7, 4, buf);
    printf("co7=%d\n", count_ones_str(buf));
    // EXPECT: co7=3
    to_binary(255, 8, buf);
    printf("co255=%d\n", count_ones_str(buf));
    // EXPECT: co255=8
    to_binary(0, 8, buf);
    printf("co0=%d\n", count_ones_str(buf));
    // EXPECT: co0=0

    // is_binary_palindrome
    to_binary(5, 3, buf);   // 101
    printf("pal5=%d\n", is_binary_palindrome(buf));
    // EXPECT: pal5=1
    to_binary(9, 4, buf);   // 1001
    printf("pal9=%d\n", is_binary_palindrome(buf));
    // EXPECT: pal9=1
    to_binary(6, 4, buf);   // 0110
    printf("pal6=%d\n", is_binary_palindrome(buf));
    // EXPECT: pal6=1
    to_binary(11, 4, buf);  // 1011
    printf("pal11=%d\n", is_binary_palindrome(buf));
    // EXPECT: pal11=0

    // binary_strcmp
    printf("cmp1=%d\n", binary_strcmp("1010", "1010"));
    // EXPECT: cmp1=0
    printf("cmp2=%d\n", binary_strcmp("1010", "1011") < 0);
    // EXPECT: cmp2=1
    printf("cmp3=%d\n", binary_strcmp("1011", "1010") > 0);
    // EXPECT: cmp3=1

    // binary_add_str
    // 0011 + 0001 = 0100 = 4
    printf("add1=%d\n", binary_add_str("0011", "0001", 4));
    // EXPECT: add1=4
    // 1111 + 0001 = 10000 masked to 4 bits = 0000 = 0
    printf("add2=%d\n", binary_add_str("1111", "0001", 4));
    // EXPECT: add2=0
    // 0110 + 0011 = 1001 = 9
    printf("add3=%d\n", binary_add_str("0110", "0011", 4));
    // EXPECT: add3=9

    printf("done\n");
    // EXPECT: done
    return 0;
}
