int printf(const char *fmt, ...);

int popcount(int x) {
    int count;
    count = 0;
    while (x) {
        x = x & (x - 1);
        count = count + 1;
    }
    return count;
}

int hamming_distance(int a, int b) {
    return popcount(a ^ b);
}

int hamming_distance_str(char *a, char *b, int len) {
    int dist;
    int i;
    dist = 0;
    for (i = 0; i < len; i = i + 1) {
        if (a[i] != b[i]) {
            dist = dist + 1;
        }
    }
    return dist;
}

int hamming_weight(int x) {
    return popcount(x);
}

int error_detect_parity(int data) {
    int parity;
    int temp;
    parity = 0;
    temp = data;
    while (temp) {
        parity = parity ^ (temp & 1);
        temp = temp >> 1;
    }
    return (data << 1) | parity;
}

int check_parity(int encoded) {
    int temp;
    int parity;
    temp = encoded;
    parity = 0;
    while (temp) {
        parity = parity ^ (temp & 1);
        temp = temp >> 1;
    }
    return parity == 0;
}

int hamming_encode_4bit(int data) {
    int d1;
    int d2;
    int d3;
    int d4;
    int p1;
    int p2;
    int p3;
    int encoded;

    data = data & 15;
    d1 = (data >> 0) & 1;
    d2 = (data >> 1) & 1;
    d3 = (data >> 2) & 1;
    d4 = (data >> 3) & 1;

    p1 = d1 ^ d2 ^ d4;
    p2 = d1 ^ d3 ^ d4;
    p3 = d2 ^ d3 ^ d4;

    encoded = (p1 << 0) | (p2 << 1) | (d1 << 2) | (p3 << 3) | (d2 << 4) | (d3 << 5) | (d4 << 6);
    return encoded;
}

int hamming_decode_7bit(int encoded) {
    int p1;
    int p2;
    int p3;
    int d1;
    int d2;
    int d3;
    int d4;
    int s1;
    int s2;
    int s3;
    int syndrome;

    p1 = (encoded >> 0) & 1;
    p2 = (encoded >> 1) & 1;
    d1 = (encoded >> 2) & 1;
    p3 = (encoded >> 3) & 1;
    d2 = (encoded >> 4) & 1;
    d3 = (encoded >> 5) & 1;
    d4 = (encoded >> 6) & 1;

    s1 = p1 ^ d1 ^ d2 ^ d4;
    s2 = p2 ^ d1 ^ d3 ^ d4;
    s3 = p3 ^ d2 ^ d3 ^ d4;

    syndrome = (s3 << 2) | (s2 << 1) | s1;

    if (syndrome != 0) {
        encoded = encoded ^ (1 << (syndrome - 1));
        d1 = (encoded >> 2) & 1;
        d2 = (encoded >> 4) & 1;
        d3 = (encoded >> 5) & 1;
        d4 = (encoded >> 6) & 1;
    }

    return (d4 << 3) | (d3 << 2) | (d2 << 1) | d1;
}

int min_hamming_in_set(int *values, int n) {
    int min_dist;
    int dist;
    int i;
    int j;
    min_dist = 32;
    for (i = 0; i < n; i = i + 1) {
        for (j = i + 1; j < n; j = j + 1) {
            dist = hamming_distance(values[i], values[j]);
            if (dist < min_dist) {
                min_dist = dist;
            }
        }
    }
    return min_dist;
}

int main() {
    int i;
    int encoded;
    int decoded;
    int corrupted;
    int vals[4];
    char s1[5];
    char s2[5];

    printf("=== Hamming Distance ===\n");
    // EXPECT: === Hamming Distance ===

    printf("hdist(%d,%d) = %d\n", 0, 0, hamming_distance(0, 0));
    // EXPECT: hdist(0,0) = 0
    printf("hdist(%d,%d) = %d\n", 1, 2, hamming_distance(1, 2));
    // EXPECT: hdist(1,2) = 2
    printf("hdist(%d,%d) = %d\n", 7, 0, hamming_distance(7, 0));
    // EXPECT: hdist(7,0) = 3
    printf("hdist(%d,%d) = %d\n", 255, 0, hamming_distance(255, 0));
    // EXPECT: hdist(255,0) = 8
    printf("hdist(%d,%d) = %d\n", 170, 85, hamming_distance(170, 85));
    // EXPECT: hdist(170,85) = 8

    printf("=== Hamming Weight ===\n");
    // EXPECT: === Hamming Weight ===

    printf("hw(%d) = %d\n", 0, hamming_weight(0));
    // EXPECT: hw(0) = 0
    printf("hw(%d) = %d\n", 15, hamming_weight(15));
    // EXPECT: hw(15) = 4
    printf("hw(%d) = %d\n", 255, hamming_weight(255));
    // EXPECT: hw(255) = 8

    printf("=== String Hamming Distance ===\n");
    // EXPECT: === String Hamming Distance ===

    s1[0] = 'k'; s1[1] = 'a'; s1[2] = 'r'; s1[3] = 'l'; s1[4] = 0;
    s2[0] = 'k'; s2[1] = 'e'; s2[2] = 'r'; s2[3] = 'n'; s2[4] = 0;
    printf("str_hdist = %d\n", hamming_distance_str(s1, s2, 4));
    // EXPECT: str_hdist = 2

    printf("=== Parity Encoding ===\n");
    // EXPECT: === Parity Encoding ===

    for (i = 0; i < 8; i = i + 1) {
        encoded = error_detect_parity(i);
        printf("parity_enc(%d) = %d, valid = %d\n", i, encoded, check_parity(encoded));
    }
    // EXPECT: parity_enc(0) = 0, valid = 1
    // EXPECT: parity_enc(1) = 3, valid = 1
    // EXPECT: parity_enc(2) = 5, valid = 1
    // EXPECT: parity_enc(3) = 6, valid = 1
    // EXPECT: parity_enc(4) = 9, valid = 1
    // EXPECT: parity_enc(5) = 10, valid = 1
    // EXPECT: parity_enc(6) = 12, valid = 1
    // EXPECT: parity_enc(7) = 15, valid = 1

    printf("=== Hamming(7,4) Encode/Decode ===\n");
    // EXPECT: === Hamming(7,4) Encode/Decode ===

    for (i = 0; i < 16; i = i + 1) {
        encoded = hamming_encode_4bit(i);
        decoded = hamming_decode_7bit(encoded);
        printf("enc(%d)=%d dec=%d\n", i, encoded, decoded);
    }
    // EXPECT: enc(0)=0 dec=0
    // EXPECT: enc(1)=7 dec=1
    // EXPECT: enc(2)=25 dec=2
    // EXPECT: enc(3)=30 dec=3
    // EXPECT: enc(4)=42 dec=4
    // EXPECT: enc(5)=45 dec=5
    // EXPECT: enc(6)=51 dec=6
    // EXPECT: enc(7)=52 dec=7
    // EXPECT: enc(8)=75 dec=8
    // EXPECT: enc(9)=76 dec=9
    // EXPECT: enc(10)=82 dec=10
    // EXPECT: enc(11)=85 dec=11
    // EXPECT: enc(12)=97 dec=12
    // EXPECT: enc(13)=102 dec=13
    // EXPECT: enc(14)=120 dec=14
    // EXPECT: enc(15)=127 dec=15

    printf("=== Error Correction ===\n");
    // EXPECT: === Error Correction ===

    encoded = hamming_encode_4bit(5);
    corrupted = encoded ^ 4;
    decoded = hamming_decode_7bit(corrupted);
    printf("original=5 encoded=%d corrupted=%d decoded=%d\n", encoded, corrupted, decoded);
    // EXPECT: original=5 encoded=45 corrupted=41 decoded=5

    printf("=== Min Hamming in Set ===\n");
    // EXPECT: === Min Hamming in Set ===

    vals[0] = 0; vals[1] = 15; vals[2] = 51; vals[3] = 85;
    printf("min_hdist = %d\n", min_hamming_in_set(vals, 4));
    // EXPECT: min_hdist = 4

    printf("Done\n");
    // EXPECT: Done

    return 0;
}
