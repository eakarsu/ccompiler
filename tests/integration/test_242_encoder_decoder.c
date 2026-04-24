int printf(const char *fmt, ...);
// EXPECT: === caesar cipher ===\nencode(Hello World,3)=Khoor Zruog\ndecode=Hello World\nroundtrip=1\nencode(ABCXYZ,1)=BCDYZA\ndecode=ABCXYZ\nrot13=nggnpx ng qnja\nrot13x2=attack at dawn\n=== RLE ===\nrle(aabbbcccc)=2a3b4c\ndecode=aabbbcccc\nmatch=1\nrle(aaabbbccc)=3a3b3c\nrle(abcdef)=abcdef\nrle(aaaaaa)=6a\ndecode=aaaaaa
// Test 242: Encoder/decoder using function pointers

int caesar_encode_char(int c, int shift) {
    if (c >= 'A' && c <= 'Z') {
        int offset = c - 'A';
        offset = offset + shift;
        while (offset < 0) offset = offset + 26;
        offset = offset - (offset / 26) * 26;
        return 'A' + offset;
    }
    if (c >= 'a' && c <= 'z') {
        int offset = c - 'a';
        offset = offset + shift;
        while (offset < 0) offset = offset + 26;
        offset = offset - (offset / 26) * 26;
        return 'a' + offset;
    }
    return c;
}

void caesar_encode(char *input, char *output, int shift) {
    int i = 0;
    while (input[i] != 0) {
        output[i] = (char)caesar_encode_char(input[i], shift);
        i = i + 1;
    }
    output[i] = 0;
}

void caesar_decode(char *input, char *output, int shift) {
    caesar_encode(input, output, -shift);
}

void xor_cipher(char *input, char *output, int key) {
    int i = 0;
    while (input[i] != 0) {
        int encoded = input[i] ^ key;
        if (encoded < 32) encoded = encoded + 32;
        if (encoded > 126) encoded = 126;
        output[i] = (char)encoded;
        i = i + 1;
    }
    output[i] = 0;
}

int run_length_encode(char *input, char *output) {
    int i = 0;
    int j = 0;
    while (input[i] != 0) {
        char c = input[i];
        int count = 0;
        while (input[i] == c) {
            count = count + 1;
            i = i + 1;
        }
        if (count > 1) {
            if (count >= 10) {
                output[j] = (char)('0' + count / 10);
                j = j + 1;
            }
            output[j] = (char)('0' + count - (count / 10) * 10);
            j = j + 1;
        }
        output[j] = c;
        j = j + 1;
    }
    output[j] = 0;
    return j;
}

void run_length_decode(char *input, char *output) {
    int i = 0;
    int j = 0;
    while (input[i] != 0) {
        int count = 0;
        while (input[i] >= '0' && input[i] <= '9') {
            count = count * 10 + (input[i] - '0');
            i = i + 1;
        }
        if (count == 0) count = 1;
        while (count > 0) {
            output[j] = input[i];
            j = j + 1;
            count = count - 1;
        }
        i = i + 1;
    }
    output[j] = 0;
}

int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

int main(void) {
    char buf1[100];
    char buf2[100];

    printf("=== caesar cipher ===\n");
    caesar_encode("Hello World", buf1, 3);
    printf("encode(Hello World,3)=%s\n", buf1);
    caesar_decode(buf1, buf2, 3);
    printf("decode=%s\n", buf2);
    printf("roundtrip=%d\n", str_equal(buf2, "Hello World"));

    caesar_encode("ABCXYZ", buf1, 1);
    printf("encode(ABCXYZ,1)=%s\n", buf1);
    caesar_decode(buf1, buf2, 1);
    printf("decode=%s\n", buf2);

    caesar_encode("attack at dawn", buf1, 13);
    printf("rot13=%s\n", buf1);
    caesar_encode(buf1, buf2, 13);
    printf("rot13x2=%s\n", buf2);

    printf("=== RLE ===\n");
    run_length_encode("aabbbcccc", buf1);
    printf("rle(aabbbcccc)=%s\n", buf1);
    run_length_decode(buf1, buf2);
    printf("decode=%s\n", buf2);
    printf("match=%d\n", str_equal(buf2, "aabbbcccc"));

    run_length_encode("aaabbbccc", buf1);
    printf("rle(aaabbbccc)=%s\n", buf1);

    run_length_encode("abcdef", buf1);
    printf("rle(abcdef)=%s\n", buf1);

    run_length_encode("aaaaaa", buf1);
    printf("rle(aaaaaa)=%s\n", buf1);
    run_length_decode(buf1, buf2);
    printf("decode=%s\n", buf2);

    return 0;
}
