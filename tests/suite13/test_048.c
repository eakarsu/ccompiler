int printf(const char *fmt, ...);

// Parity computation and Hamming distance

// Compute parity (0 if even number of 1-bits, 1 if odd)
int parity(int x) {
    int p = 0;
    int val = x;
    while (val != 0) {
        p = p ^ (val & 1);
        val = (val >> 1) & 0x7FFFFFFF;
    }
    return p;
}

// Hamming distance: number of bit positions where two values differ
int hamming_distance(int a, int b) {
    int diff = a ^ b;
    int count = 0;
    while (diff != 0) {
        count = count + (diff & 1);
        diff = (diff >> 1) & 0x7FFFFFFF;
    }
    return count;
}

// Hamming weight (popcount)
int hamming_weight(int x) {
    return hamming_distance(x, 0);
}

// Simple Hamming(7,4) encoder
// Encodes 4 data bits into 7 bits with 3 parity bits
// Bit layout: p1 p2 d1 p3 d2 d3 d4
// p1 covers bits 1,3,5,7 (positions with bit0 set in 1-indexed)
// p2 covers bits 2,3,6,7
// p3 covers bits 4,5,6,7
int hamming_encode(int data4) {
    int d1 = (data4 >> 0) & 1;
    int d2 = (data4 >> 1) & 1;
    int d3 = (data4 >> 2) & 1;
    int d4 = (data4 >> 3) & 1;

    // Position assignment (1-indexed): p1=1, p2=2, d1=3, p3=4, d2=5, d3=6, d4=7
    int p1 = d1 ^ d2 ^ d4;   // covers positions 1,3,5,7: p1,d1,d2,d4
    int p2 = d1 ^ d3 ^ d4;   // covers positions 2,3,6,7: p2,d1,d3,d4
    int p3 = d2 ^ d3 ^ d4;   // covers positions 4,5,6,7: p3,d2,d3,d4

    // Encoded: bit0=p1, bit1=p2, bit2=d1, bit3=p3, bit4=d2, bit5=d3, bit6=d4
    int encoded = 0;
    encoded = encoded | (p1 << 0);
    encoded = encoded | (p2 << 1);
    encoded = encoded | (d1 << 2);
    encoded = encoded | (p3 << 3);
    encoded = encoded | (d2 << 4);
    encoded = encoded | (d3 << 5);
    encoded = encoded | (d4 << 6);
    return encoded;
}

// Hamming(7,4) syndrome computation
int hamming_syndrome(int code7) {
    int b1 = (code7 >> 0) & 1;
    int b2 = (code7 >> 1) & 1;
    int b3 = (code7 >> 2) & 1;
    int b4 = (code7 >> 3) & 1;
    int b5 = (code7 >> 4) & 1;
    int b6 = (code7 >> 5) & 1;
    int b7 = (code7 >> 6) & 1;

    int s1 = b1 ^ b3 ^ b5 ^ b7;
    int s2 = b2 ^ b3 ^ b6 ^ b7;
    int s3 = b4 ^ b5 ^ b6 ^ b7;

    return (s3 << 2) | (s2 << 1) | s1;
}

// Decode Hamming(7,4) with single-error correction
int hamming_decode(int code7) {
    int syndrome = hamming_syndrome(code7);
    if (syndrome != 0) {
        // Flip the bit at position (syndrome - 1) (0-indexed)
        code7 = code7 ^ (1 << (syndrome - 1));
    }
    // Extract data bits: d1=bit2, d2=bit4, d3=bit5, d4=bit6
    int d1 = (code7 >> 2) & 1;
    int d2 = (code7 >> 4) & 1;
    int d3 = (code7 >> 5) & 1;
    int d4 = (code7 >> 6) & 1;
    return (d4 << 3) | (d3 << 2) | (d2 << 1) | d1;
}

// Compute minimum Hamming distance of a set of codewords
int min_hamming_dist(int *codes, int n) {
    int min_dist = 100;
    int i = 0;
    while (i < n) {
        int j = i + 1;
        while (j < n) {
            int d = hamming_distance(codes[i], codes[j]);
            if (d < min_dist) {
                min_dist = d;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return min_dist;
}

// Check if a value has even parity (useful for error detection)
int even_parity_check(int x) {
    return parity(x) == 0;
}

int main() {
    // Parity tests
    printf("par(0)=%d\n", parity(0));
    // EXPECT: par(0)=0
    printf("par(1)=%d\n", parity(1));
    // EXPECT: par(1)=1
    printf("par(3)=%d\n", parity(3));
    // EXPECT: par(3)=0
    printf("par(7)=%d\n", parity(7));
    // EXPECT: par(7)=1
    printf("par(15)=%d\n", parity(15));
    // EXPECT: par(15)=0
    printf("par(255)=%d\n", parity(255));
    // EXPECT: par(255)=0
    printf("par(170)=%d\n", parity(170));
    // EXPECT: par(170)=0
    printf("par(5)=%d\n", parity(5));
    // EXPECT: par(5)=0
    printf("par(6)=%d\n", parity(6));
    // EXPECT: par(6)=0
    printf("par(10)=%d\n", parity(10));
    // EXPECT: par(10)=0

    // Hamming distance
    printf("hd(0,0)=%d\n", hamming_distance(0, 0));
    // EXPECT: hd(0,0)=0
    printf("hd(0,1)=%d\n", hamming_distance(0, 1));
    // EXPECT: hd(0,1)=1
    printf("hd(0,7)=%d\n", hamming_distance(0, 7));
    // EXPECT: hd(0,7)=3
    printf("hd(0,15)=%d\n", hamming_distance(0, 15));
    // EXPECT: hd(0,15)=4
    printf("hd(15,15)=%d\n", hamming_distance(15, 15));
    // EXPECT: hd(15,15)=0
    printf("hd(170,85)=%d\n", hamming_distance(170, 85));
    // EXPECT: hd(170,85)=8

    // Hamming weight
    printf("hw(0)=%d\n", hamming_weight(0));
    // EXPECT: hw(0)=0
    printf("hw(255)=%d\n", hamming_weight(255));
    // EXPECT: hw(255)=8

    // Hamming(7,4) encode/decode roundtrip
    int i = 0;
    int all_ok = 1;
    while (i < 16) {
        int encoded = hamming_encode(i);
        int decoded = hamming_decode(encoded);
        if (decoded != i) {
            all_ok = 0;
        }
        i = i + 1;
    }
    printf("h74_rt=%d\n", all_ok);
    // EXPECT: h74_rt=1

    // Specific encode values
    // data=0 (0000): d1=0,d2=0,d3=0,d4=0 -> p1=0,p2=0,p3=0 -> 0000000 = 0
    printf("enc0=%d\n", hamming_encode(0));
    // EXPECT: enc0=0
    // data=1 (0001): d1=1,d2=0,d3=0,d4=0 -> p1=1,p2=1,p3=0 -> 0000111 = 7
    printf("enc1=%d\n", hamming_encode(1));
    // EXPECT: enc1=7
    // data=15 (1111): d1=1,d2=1,d3=1,d4=1 -> p1=1^1^1=1,p2=1^1^1=1,p3=1^1^1=1
    // -> 1111111 = 127
    printf("enc15=%d\n", hamming_encode(15));
    // EXPECT: enc15=127

    // Single-error correction
    int code = hamming_encode(5);  // 5 = 0101
    // Flip bit 0
    int corrupted = code ^ 1;
    printf("fix0=%d\n", hamming_decode(corrupted));
    // EXPECT: fix0=5
    // Flip bit 4
    corrupted = code ^ 16;
    printf("fix4=%d\n", hamming_decode(corrupted));
    // EXPECT: fix4=5
    // Flip bit 6
    corrupted = code ^ 64;
    printf("fix6=%d\n", hamming_decode(corrupted));
    // EXPECT: fix6=5

    // Syndrome of uncorrupted code is 0
    printf("syn_ok=%d\n", hamming_syndrome(hamming_encode(9)));
    // EXPECT: syn_ok=0

    // Min Hamming distance of all 16 Hamming(7,4) codewords
    int codewords[16];
    i = 0;
    while (i < 16) {
        codewords[i] = hamming_encode(i);
        i = i + 1;
    }
    printf("min_dist=%d\n", min_hamming_dist(codewords, 16));
    // EXPECT: min_dist=3

    // Even parity check
    printf("ep(0)=%d\n", even_parity_check(0));
    // EXPECT: ep(0)=1
    printf("ep(3)=%d\n", even_parity_check(3));
    // EXPECT: ep(3)=1
    printf("ep(1)=%d\n", even_parity_check(1));
    // EXPECT: ep(1)=0

    printf("done\n");
    // EXPECT: done
    return 0;
}
