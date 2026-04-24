int printf(const char *fmt, ...);

/* Checksum computation: additive, XOR, Fletcher-like, and Adler-like */

int str_len(char *s) {
    int n;
    n = 0;
    while (s[n]) n = n + 1;
    return n;
}

int additive_checksum(char *data, int len) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < len; i = i + 1) {
        sum = sum + data[i];
    }
    return sum & 255;
}

int xor_checksum(char *data, int len) {
    int result;
    int i;
    result = 0;
    for (i = 0; i < len; i = i + 1) {
        result = result ^ data[i];
    }
    return result;
}

/* Fletcher-16-like checksum */
int fletcher_checksum(char *data, int len) {
    int sum1;
    int sum2;
    int i;
    sum1 = 0;
    sum2 = 0;
    for (i = 0; i < len; i = i + 1) {
        sum1 = (sum1 + data[i]) % 255;
        sum2 = (sum2 + sum1) % 255;
    }
    return (sum2 << 8) | sum1;
}

/* Adler-32-like checksum (simplified with smaller mod) */
int adler_checksum(int *data, int len) {
    int a;
    int b;
    int i;
    a = 1;
    b = 0;
    for (i = 0; i < len; i = i + 1) {
        a = (a + data[i]) % 65521;
        b = (b + a) % 65521;
    }
    return (b << 16) | a;
}

/* Internet-like checksum (16-bit ones complement sum) */
int ones_complement_sum(int *words, int count) {
    int sum;
    int i;
    sum = 0;
    for (i = 0; i < count; i = i + 1) {
        sum = sum + words[i];
        while (sum > 65535) {
            sum = (sum & 65535) + (sum >> 16);
        }
    }
    return (~sum) & 65535;
}

void test_additive(void) {
    int c1;
    int c2;
    int c3;
    c1 = additive_checksum("hello", 5);
    c2 = additive_checksum("hello", 5);
    c3 = additive_checksum("world", 5);
    // EXPECT: add hello=20
    printf("add hello=%d\n", c1);
    // EXPECT: add consistent=1
    printf("add consistent=%d\n", c1 == c2);
    // EXPECT: add different=1
    printf("add different=%d\n", c1 != c3);
}

void test_xor(void) {
    int c1;
    int c2;
    c1 = xor_checksum("AB", 2);
    c2 = xor_checksum("BA", 2);
    /* XOR is order-independent for these two */
    // EXPECT: xor AB=3
    printf("xor AB=%d\n", c1);
    // EXPECT: xor commutative=1
    printf("xor commutative=%d\n", c1 == c2);
    /* XOR of single byte */
    c1 = xor_checksum("A", 1);
    // EXPECT: xor A=65
    printf("xor A=%d\n", c1);
}

void test_fletcher(void) {
    int f1;
    int f2;
    f1 = fletcher_checksum("abcde", 5);
    f2 = fletcher_checksum("abcde", 5);
    // EXPECT: fletcher consistent=1
    printf("fletcher consistent=%d\n", f1 == f2);
    f2 = fletcher_checksum("edcba", 5);
    // EXPECT: fletcher order matters=1
    printf("fletcher order matters=%d\n", f1 != f2);
    // EXPECT: fletcher abcde=51440
    printf("fletcher abcde=%d\n", f1);
}

void test_adler(void) {
    int data[5];
    int a1;
    data[0] = 72;
    data[1] = 101;
    data[2] = 108;
    data[3] = 108;
    data[4] = 111;
    a1 = adler_checksum(data, 5);
    // EXPECT: adler Hello=93061621
    printf("adler Hello=%d\n", a1);
}

void test_ones_complement(void) {
    int words[3];
    int cksum;
    words[0] = 256;
    words[1] = 512;
    words[2] = 1024;
    cksum = ones_complement_sum(words, 3);
    // EXPECT: ones comp=63743
    printf("ones comp=%d\n", cksum);
}

void test_empty_data(void) {
    int a;
    int x;
    a = additive_checksum("", 0);
    x = xor_checksum("", 0);
    // EXPECT: empty add=0
    printf("empty add=%d\n", a);
    // EXPECT: empty xor=0
    printf("empty xor=%d\n", x);
}

void test_single_byte(void) {
    int a;
    int x;
    a = additive_checksum("Z", 1);
    x = xor_checksum("Z", 1);
    // EXPECT: single add=90
    printf("single add=%d\n", a);
    // EXPECT: single xor=90
    printf("single xor=%d\n", x);
}

void test_checksum_detects_change(void) {
    int c1;
    int c2;
    char data1[6];
    char data2[6];
    int i;
    data1[0] = 'h'; data1[1] = 'e'; data1[2] = 'l'; data1[3] = 'l'; data1[4] = 'o'; data1[5] = 0;
    data2[0] = 'h'; data2[1] = 'e'; data2[2] = 'l'; data2[3] = 'l'; data2[4] = 'p'; data2[5] = 0;
    c1 = fletcher_checksum(data1, 5);
    c2 = fletcher_checksum(data2, 5);
    // EXPECT: change detected=1
    printf("change detected=%d\n", c1 != c2);
}

int main(void) {
    test_additive();
    test_xor();
    test_fletcher();
    test_adler();
    test_ones_complement();
    test_empty_data();
    test_single_byte();
    test_checksum_detects_change();
    // EXPECT: done
    printf("done\n");
    return 0;
}
