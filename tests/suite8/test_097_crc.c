int printf(const char *fmt, ...);

/* CRC-like calculation: simplified CRC using polynomial division over bits */

int str_len(char *s) {
    int n;
    n = 0;
    while (s[n]) n = n + 1;
    return n;
}

/* Simple 8-bit CRC with a given polynomial */
int crc8(char *data, int len, int poly) {
    int crc;
    int i;
    int j;
    crc = 0;
    for (i = 0; i < len; i = i + 1) {
        crc = crc ^ data[i];
        for (j = 0; j < 8; j = j + 1) {
            if (crc & 128) {
                crc = ((crc << 1) ^ poly) & 255;
            } else {
                crc = (crc << 1) & 255;
            }
        }
    }
    return crc;
}

/* CRC with initial value and final XOR */
int crc8_full(char *data, int len, int poly, int init, int final_xor) {
    int crc;
    int i;
    int j;
    crc = init;
    for (i = 0; i < len; i = i + 1) {
        crc = crc ^ data[i];
        for (j = 0; j < 8; j = j + 1) {
            if (crc & 128) {
                crc = ((crc << 1) ^ poly) & 255;
            } else {
                crc = (crc << 1) & 255;
            }
        }
    }
    return crc ^ final_xor;
}

/* Build a CRC lookup table for speed */
void crc8_build_table(int *table, int poly) {
    int i;
    int j;
    for (i = 0; i < 256; i = i + 1) {
        int crc;
        crc = i;
        for (j = 0; j < 8; j = j + 1) {
            if (crc & 128) {
                crc = ((crc << 1) ^ poly) & 255;
            } else {
                crc = (crc << 1) & 255;
            }
        }
        table[i] = crc;
    }
}

int crc8_table_lookup(char *data, int len, int *table) {
    int crc;
    int i;
    crc = 0;
    for (i = 0; i < len; i = i + 1) {
        int idx;
        idx = (crc ^ data[i]) & 255;
        crc = table[idx];
    }
    return crc;
}

void test_basic_crc(void) {
    int c1;
    int c2;
    int c3;
    c1 = crc8("A", 1, 7);
    c2 = crc8("A", 1, 7);
    c3 = crc8("B", 1, 7);
    // EXPECT: crc A=192
    printf("crc A=%d\n", c1);
    // EXPECT: crc consistent=1
    printf("crc consistent=%d\n", c1 == c2);
    // EXPECT: crc A!=B: 1
    printf("crc A!=B: %d\n", c1 != c3);
}

void test_crc_hello(void) {
    int c1;
    c1 = crc8("hello", 5, 7);
    // EXPECT: crc hello=146
    printf("crc hello=%d\n", c1);
}

void test_crc_polynomial(void) {
    int c1;
    int c2;
    c1 = crc8("test", 4, 7);
    c2 = crc8("test", 4, 49);
    // EXPECT: different poly different crc=1
    printf("different poly different crc=%d\n", c1 != c2);
}

void test_crc_full(void) {
    int c1;
    int c2;
    c1 = crc8_full("hello", 5, 7, 0, 0);
    c2 = crc8("hello", 5, 7);
    // EXPECT: full vs basic same=1
    printf("full vs basic same=%d\n", c1 == c2);
    c1 = crc8_full("hello", 5, 7, 255, 0);
    // EXPECT: init 255 changes crc=1
    printf("init 255 changes crc=%d\n", c1 != c2);
    c1 = crc8_full("hello", 5, 7, 0, 255);
    // EXPECT: final xor changes crc=1
    printf("final xor changes crc=%d\n", c1 != c2);
}

void test_table_crc(void) {
    int table[256];
    int c1;
    int c2;
    crc8_build_table(table, 7);
    c1 = crc8("hello world", 11, 7);
    c2 = crc8_table_lookup("hello world", 11, table);
    // EXPECT: table matches direct=1
    printf("table matches direct=%d\n", c1 == c2);
    c1 = crc8("test data", 9, 7);
    c2 = crc8_table_lookup("test data", 9, table);
    // EXPECT: table matches direct2=1
    printf("table matches direct2=%d\n", c1 == c2);
}

void test_error_detection(void) {
    char data1[6];
    char data2[6];
    int c1;
    int c2;
    data1[0] = 'h'; data1[1] = 'e'; data1[2] = 'l'; data1[3] = 'l'; data1[4] = 'o'; data1[5] = 0;
    data2[0] = 'h'; data2[1] = 'e'; data2[2] = 'l'; data2[3] = 'l'; data2[4] = 'p'; data2[5] = 0;
    c1 = crc8(data1, 5, 7);
    c2 = crc8(data2, 5, 7);
    // EXPECT: single bit diff detected=1
    printf("single bit diff detected=%d\n", c1 != c2);
}

void test_empty(void) {
    int c;
    c = crc8("", 0, 7);
    // EXPECT: empty crc=0
    printf("empty crc=%d\n", c);
}

void test_crc_range(void) {
    int c;
    int in_range;
    c = crc8("anything at all!", 16, 7);
    in_range = (c >= 0 && c <= 255);
    // EXPECT: crc in 0-255: 1
    printf("crc in 0-255: %d\n", in_range);
}

int main(void) {
    test_basic_crc();
    test_crc_hello();
    test_crc_polynomial();
    test_crc_full();
    test_table_crc();
    test_error_detection();
    test_empty();
    test_crc_range();
    // EXPECT: done
    printf("done\n");
    return 0;
}
