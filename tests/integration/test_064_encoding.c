// EXPECT: Base64 encode test:\nInput bytes: 72 101 108 108 111\nEncoded: SGVsbG8\nDecoded: 72 101 108 108 111\nBase64 roundtrip OK\nRLE encode of 'aaabbbccdddddd':\n3a3b2c6d\nRLE decode:\naaabbbccdddddd\nRLE roundtrip OK\nHuffman frequencies:\na:5 b:3 c:2 d:1 e:1\nHuffman tree built, root weight: 12\nHuffman codes:\na: 0\nb: 10\nc: 110\nd: 1110\ne: 1111\nLZ77 compression simulation:\nLiteral: h\nLiteral: e\nLiteral: l\nLiteral: l\nLiteral: o\nLiteral: _\nMatch(6,5)\nLZ77 output length: 7\nXOR encode/decode test:\nOriginal: 65 66 67 68 69\nKey: 42 43 44 45 46\nEncoded: 107 105 111 105 107\nDecoded: 65 66 67 68 69\nXOR roundtrip OK\nSimple checksum: 335\nByte packing test:\nPacked 1234 -> hi=4 lo=210\nUnpacked -> 1234\nNibble encode of 171: ab\nNibble decode of ab: 171\nDone.\n
int printf(const char *fmt, ...);

// ---- Base64-like encoding (using int lookup) ----

int b64_enc_table[64];
int b64_dec_table[128];

void b64_init() {
    int i;
    int c;
    // A-Z = 0-25
    for (i = 0; i < 26; i = i + 1) {
        b64_enc_table[i] = 'A' + i;
    }
    // a-z = 26-51
    for (i = 0; i < 26; i = i + 1) {
        b64_enc_table[26 + i] = 'a' + i;
    }
    // 0-9 = 52-61
    for (i = 0; i < 10; i = i + 1) {
        b64_enc_table[52 + i] = '0' + i;
    }
    b64_enc_table[62] = '+';
    b64_enc_table[63] = '/';

    // Build decode table
    for (i = 0; i < 128; i = i + 1) {
        b64_dec_table[i] = -1;
    }
    for (i = 0; i < 64; i = i + 1) {
        b64_dec_table[b64_enc_table[i]] = i;
    }
}

int b64_encoded[256];
int b64_encoded_len;

void b64_encode(int *data, int len) {
    int i;
    int val;
    int bits;
    int idx;

    b64_encoded_len = 0;
    val = 0;
    bits = 0;

    for (i = 0; i < len; i = i + 1) {
        val = (val << 8) | (data[i] & 255);
        bits = bits + 8;
        while (bits >= 6) {
            bits = bits - 6;
            idx = (val >> bits) & 63;
            b64_encoded[b64_encoded_len] = b64_enc_table[idx];
            b64_encoded_len = b64_encoded_len + 1;
        }
    }
    if (bits > 0) {
        idx = (val << (6 - bits)) & 63;
        b64_encoded[b64_encoded_len] = b64_enc_table[idx];
        b64_encoded_len = b64_encoded_len + 1;
    }
}

int b64_decoded[256];
int b64_decoded_len;

void b64_decode(int *encoded, int enc_len) {
    int i;
    int val;
    int bits;
    int ch;

    b64_decoded_len = 0;
    val = 0;
    bits = 0;

    for (i = 0; i < enc_len; i = i + 1) {
        ch = b64_dec_table[encoded[i]];
        if (ch >= 0) {
            val = (val << 6) | ch;
            bits = bits + 6;
            if (bits >= 8) {
                bits = bits - 8;
                b64_decoded[b64_decoded_len] = (val >> bits) & 255;
                b64_decoded_len = b64_decoded_len + 1;
            }
        }
    }
}

// ---- Run-Length Encoding ----

char rle_input[256];
char rle_encoded[512];
int rle_enc_len;

void rle_encode(char *input, int len) {
    int i;
    int count;
    char current;
    rle_enc_len = 0;

    i = 0;
    while (i < len) {
        current = input[i];
        count = 1;
        while (i + count < len && input[i + count] == current) {
            count = count + 1;
        }
        // Store count as digit(s) then char
        if (count >= 10) {
            rle_encoded[rle_enc_len] = '0' + count / 10;
            rle_enc_len = rle_enc_len + 1;
            rle_encoded[rle_enc_len] = '0' + count % 10;
            rle_enc_len = rle_enc_len + 1;
        } else {
            rle_encoded[rle_enc_len] = '0' + count;
            rle_enc_len = rle_enc_len + 1;
        }
        rle_encoded[rle_enc_len] = current;
        rle_enc_len = rle_enc_len + 1;
        i = i + count;
    }
    rle_encoded[rle_enc_len] = 0;
}

char rle_decoded[512];
int rle_dec_len;

void rle_decode(char *encoded, int enc_len) {
    int i;
    int count;
    int j;
    char ch;

    rle_dec_len = 0;
    i = 0;
    while (i < enc_len) {
        count = 0;
        while (i < enc_len && encoded[i] >= '0' && encoded[i] <= '9') {
            count = count * 10 + (encoded[i] - '0');
            i = i + 1;
        }
        if (i < enc_len) {
            ch = encoded[i];
            i = i + 1;
            for (j = 0; j < count; j = j + 1) {
                rle_decoded[rle_dec_len] = ch;
                rle_dec_len = rle_dec_len + 1;
            }
        }
    }
    rle_decoded[rle_dec_len] = 0;
}

// ---- Huffman Tree Building ----

struct HuffNode {
    int freq;
    int ch;       // -1 for internal
    int left;     // index or -1
    int right;    // index or -1
    int parent;   // for building
};

struct HuffNode huff_nodes[64];
int huff_node_count;
int huff_root;

int huff_new_node(int freq, int ch, int left, int right) {
    int idx;
    idx = huff_node_count;
    huff_nodes[idx].freq = freq;
    huff_nodes[idx].ch = ch;
    huff_nodes[idx].left = left;
    huff_nodes[idx].right = right;
    huff_nodes[idx].parent = -1;
    huff_node_count = huff_node_count + 1;
    return idx;
}

void huff_build(int *freqs, int *chars, int n) {
    int active[64];
    int num_active;
    int i;
    int min1;
    int min2;
    int min1_idx;
    int min2_idx;
    int new_idx;
    int j;

    huff_node_count = 0;
    num_active = 0;

    // Create leaf nodes
    for (i = 0; i < n; i = i + 1) {
        active[num_active] = huff_new_node(freqs[i], chars[i], -1, -1);
        num_active = num_active + 1;
    }

    while (num_active > 1) {
        // Find two minimum frequency nodes
        min1 = 999999;
        min2 = 999999;
        min1_idx = -1;
        min2_idx = -1;

        for (i = 0; i < num_active; i = i + 1) {
            if (huff_nodes[active[i]].freq < min1) {
                min2 = min1;
                min2_idx = min1_idx;
                min1 = huff_nodes[active[i]].freq;
                min1_idx = i;
            } else if (huff_nodes[active[i]].freq < min2) {
                min2 = huff_nodes[active[i]].freq;
                min2_idx = i;
            }
        }

        // Create internal node
        new_idx = huff_new_node(min1 + min2, -1, active[min1_idx], active[min2_idx]);
        huff_nodes[active[min1_idx]].parent = new_idx;
        huff_nodes[active[min2_idx]].parent = new_idx;

        // Remove the two and add new
        // Remove higher index first
        if (min1_idx > min2_idx) {
            active[min1_idx] = active[num_active - 1];
            num_active = num_active - 1;
            active[min2_idx] = active[num_active - 1];
            num_active = num_active - 1;
        } else {
            active[min2_idx] = active[num_active - 1];
            num_active = num_active - 1;
            active[min1_idx] = active[num_active - 1];
            num_active = num_active - 1;
        }

        active[num_active] = new_idx;
        num_active = num_active + 1;
    }

    huff_root = active[0];
}

// ---- Print Huffman codes ----

int huff_code_buf[32];
int huff_code_len;

void huff_print_codes(int node, int depth) {
    if (huff_nodes[node].ch >= 0) {
        // Leaf
        printf("%c: ", huff_nodes[node].ch);
        {
            int k;
            for (k = 0; k < depth; k = k + 1) {
                printf("%d", huff_code_buf[k]);
            }
        }
        printf("\n");
        return;
    }
    if (huff_nodes[node].left >= 0) {
        huff_code_buf[depth] = 0;
        huff_print_codes(huff_nodes[node].left, depth + 1);
    }
    if (huff_nodes[node].right >= 0) {
        huff_code_buf[depth] = 1;
        huff_print_codes(huff_nodes[node].right, depth + 1);
    }
}

// ---- LZ77-like compression simulation ----

struct LZ77Token {
    int type;    // 0 = literal, 1 = match
    int ch;      // for literal
    int offset;  // for match
    int length;  // for match
};

struct LZ77Token lz77_output[256];
int lz77_output_len;

void lz77_compress(char *input, int len) {
    int i;
    int j;
    int best_off;
    int best_len;
    int cur_len;
    int window_start;

    lz77_output_len = 0;
    i = 0;

    while (i < len) {
        best_off = 0;
        best_len = 0;

        // Search window (up to 256 back)
        window_start = i - 256;
        if (window_start < 0) {
            window_start = 0;
        }

        for (j = window_start; j < i; j = j + 1) {
            cur_len = 0;
            while (i + cur_len < len && input[j + cur_len] == input[i + cur_len] && cur_len < 255) {
                cur_len = cur_len + 1;
            }
            if (cur_len > best_len) {
                best_len = cur_len;
                best_off = i - j;
            }
        }

        if (best_len >= 2) {
            lz77_output[lz77_output_len].type = 1;
            lz77_output[lz77_output_len].offset = best_off;
            lz77_output[lz77_output_len].length = best_len;
            lz77_output[lz77_output_len].ch = 0;
            lz77_output_len = lz77_output_len + 1;
            i = i + best_len;
        } else {
            lz77_output[lz77_output_len].type = 0;
            lz77_output[lz77_output_len].ch = input[i];
            lz77_output[lz77_output_len].offset = 0;
            lz77_output[lz77_output_len].length = 0;
            lz77_output_len = lz77_output_len + 1;
            i = i + 1;
        }
    }
}

// ---- XOR one-time pad ----

int xor_encoded[256];
int xor_decoded[256];

void xor_encode(int *data, int *key, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        xor_encoded[i] = data[i] ^ key[i];
    }
}

void xor_decode(int *encoded, int *key, int len) {
    int i;
    for (i = 0; i < len; i = i + 1) {
        xor_decoded[i] = encoded[i] ^ key[i];
    }
}

// ---- Simple checksum ----

int simple_checksum(int *data, int len) {
    int i;
    int sum;
    sum = 0;
    for (i = 0; i < len; i = i + 1) {
        sum = sum + data[i];
    }
    return sum;
}

// ---- Byte packing ----

int pack_bytes(int hi, int lo) {
    return (hi << 8) | (lo & 255);
}

int unpack_hi(int packed) {
    return (packed >> 8) & 255;
}

int unpack_lo(int packed) {
    return packed & 255;
}

// ---- Nibble encoding ----

char nibble_to_hex(int n) {
    if (n < 10) {
        return '0' + n;
    }
    return 'a' + (n - 10);
}

int hex_to_nibble(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    return -1;
}

void encode_nibbles(int byte_val, char *out) {
    out[0] = nibble_to_hex((byte_val >> 4) & 15);
    out[1] = nibble_to_hex(byte_val & 15);
    out[2] = 0;
}

int decode_nibbles(char *hex) {
    int hi;
    int lo;
    hi = hex_to_nibble(hex[0]);
    lo = hex_to_nibble(hex[1]);
    return (hi << 4) | lo;
}

// ---- Frequency analysis helper ----

int freq_count[128];

void count_frequencies(char *data, int len) {
    int i;
    for (i = 0; i < 128; i = i + 1) {
        freq_count[i] = 0;
    }
    for (i = 0; i < len; i = i + 1) {
        if (data[i] >= 0) {
            freq_count[(int)data[i]] = freq_count[(int)data[i]] + 1;
        }
    }
}

int find_most_frequent(int from, int to) {
    int i;
    int max_freq;
    int max_ch;
    max_freq = 0;
    max_ch = from;
    for (i = from; i < to; i = i + 1) {
        if (freq_count[i] > max_freq) {
            max_freq = freq_count[i];
            max_ch = i;
        }
    }
    return max_ch;
}

// ---- Bit manipulation helpers ----

int get_bit(int val, int pos) {
    return (val >> pos) & 1;
}

int set_bit(int val, int pos) {
    return val | (1 << pos);
}

int clear_bit(int val, int pos) {
    return val & ~(1 << pos);
}

int count_bits(int val) {
    int cnt;
    cnt = 0;
    while (val > 0) {
        cnt = cnt + (val & 1);
        val = val >> 1;
    }
    return cnt;
}

int main() {
    int data[16];
    int key[16];
    int i;
    int ok;
    int packed;
    int hi;
    int lo;
    char hex_buf[4];
    int freqs[5];
    int chars[5];
    char rle_in[32];
    char lz_in[32];

    // Base64 test
    b64_init();
    data[0] = 72; data[1] = 101; data[2] = 108; data[3] = 108; data[4] = 111; // "Hello"
    printf("Base64 encode test:\n");
    printf("Input bytes: %d %d %d %d %d\n", data[0], data[1], data[2], data[3], data[4]);
    b64_encode(data, 5);
    printf("Encoded: ");
    for (i = 0; i < b64_encoded_len; i = i + 1) {
        printf("%c", b64_encoded[i]);
    }
    printf("\n");

    b64_decode(b64_encoded, b64_encoded_len);
    printf("Decoded: ");
    for (i = 0; i < b64_decoded_len; i = i + 1) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", b64_decoded[i]);
    }
    printf("\n");

    ok = 1;
    for (i = 0; i < 5; i = i + 1) {
        if (b64_decoded[i] != data[i]) {
            ok = 0;
        }
    }
    if (ok) {
        printf("Base64 roundtrip OK\n");
    } else {
        printf("Base64 roundtrip FAIL\n");
    }

    // RLE test
    rle_in[0]='a'; rle_in[1]='a'; rle_in[2]='a';
    rle_in[3]='b'; rle_in[4]='b'; rle_in[5]='b';
    rle_in[6]='c'; rle_in[7]='c';
    rle_in[8]='d'; rle_in[9]='d'; rle_in[10]='d'; rle_in[11]='d'; rle_in[12]='d'; rle_in[13]='d';
    rle_in[14]=0;

    printf("RLE encode of 'aaabbbccdddddd':\n");
    rle_encode(rle_in, 14);
    for (i = 0; i < rle_enc_len; i = i + 1) {
        printf("%c", rle_encoded[i]);
    }
    printf("\n");

    printf("RLE decode:\n");
    rle_decode(rle_encoded, rle_enc_len);
    for (i = 0; i < rle_dec_len; i = i + 1) {
        printf("%c", rle_decoded[i]);
    }
    printf("\n");

    ok = 1;
    if (rle_dec_len != 14) {
        ok = 0;
    } else {
        for (i = 0; i < 14; i = i + 1) {
            if (rle_decoded[i] != rle_in[i]) {
                ok = 0;
            }
        }
    }
    if (ok) {
        printf("RLE roundtrip OK\n");
    } else {
        printf("RLE roundtrip FAIL\n");
    }

    // Huffman
    freqs[0] = 5; chars[0] = 'a';
    freqs[1] = 3; chars[1] = 'b';
    freqs[2] = 2; chars[2] = 'c';
    freqs[3] = 1; chars[3] = 'd';
    freqs[4] = 1; chars[4] = 'e';

    printf("Huffman frequencies:\n");
    printf("a:%d b:%d c:%d d:%d e:%d\n", freqs[0], freqs[1], freqs[2], freqs[3], freqs[4]);

    huff_build(freqs, chars, 5);
    printf("Huffman tree built, root weight: %d\n", huff_nodes[huff_root].freq);

    printf("Huffman codes:\n");
    huff_print_codes(huff_root, 0);

    // LZ77
    lz_in[0]='h'; lz_in[1]='e'; lz_in[2]='l'; lz_in[3]='l'; lz_in[4]='o';
    lz_in[5]='_'; lz_in[6]='h'; lz_in[7]='e'; lz_in[8]='l'; lz_in[9]='l'; lz_in[10]='o';
    lz_in[11]=0;

    printf("LZ77 compression simulation:\n");
    lz77_compress(lz_in, 11);
    for (i = 0; i < lz77_output_len; i = i + 1) {
        if (lz77_output[i].type == 0) {
            printf("Literal: %c\n", lz77_output[i].ch);
        } else {
            printf("Match(%d,%d)\n", lz77_output[i].offset, lz77_output[i].length);
        }
    }
    printf("LZ77 output length: %d\n", lz77_output_len);

    // XOR encode/decode
    data[0] = 65; data[1] = 66; data[2] = 67; data[3] = 68; data[4] = 69;
    key[0] = 42; key[1] = 43; key[2] = 44; key[3] = 45; key[4] = 46;

    printf("XOR encode/decode test:\n");
    printf("Original: %d %d %d %d %d\n", data[0], data[1], data[2], data[3], data[4]);
    printf("Key: %d %d %d %d %d\n", key[0], key[1], key[2], key[3], key[4]);

    xor_encode(data, key, 5);
    printf("Encoded: %d %d %d %d %d\n", xor_encoded[0], xor_encoded[1], xor_encoded[2], xor_encoded[3], xor_encoded[4]);

    xor_decode(xor_encoded, key, 5);
    printf("Decoded: %d %d %d %d %d\n", xor_decoded[0], xor_decoded[1], xor_decoded[2], xor_decoded[3], xor_decoded[4]);

    ok = 1;
    for (i = 0; i < 5; i = i + 1) {
        if (xor_decoded[i] != data[i]) {
            ok = 0;
        }
    }
    if (ok) {
        printf("XOR roundtrip OK\n");
    } else {
        printf("XOR roundtrip FAIL\n");
    }

    // Checksum
    printf("Simple checksum: %d\n", simple_checksum(data, 5));

    // Byte packing
    packed = pack_bytes(4, 210);
    hi = unpack_hi(packed);
    lo = unpack_lo(packed);
    printf("Byte packing test:\n");
    printf("Packed 1234 -> hi=%d lo=%d\n", hi, lo);
    printf("Unpacked -> %d\n", packed);

    // Nibble encode
    encode_nibbles(171, hex_buf);
    printf("Nibble encode of 171: %c%c\n", hex_buf[0], hex_buf[1]);
    printf("Nibble decode of %c%c: %d\n", hex_buf[0], hex_buf[1], decode_nibbles(hex_buf));

    printf("Done.\n");
    return 0;
}
