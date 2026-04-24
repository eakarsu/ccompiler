int printf(const char *fmt, ...);

int str_len(char *s) {
    int len = 0;
    while (s[len] != 0) {
        len = len + 1;
    }
    return len;
}

void str_copy(char *dst, char *src) {
    int i = 0;
    while (src[i] != 0) {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = 0;
}

int str_equal(char *a, char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i]) return 0;
        i = i + 1;
    }
    return a[i] == b[i];
}

void caesar_encrypt(char *src, char *dst, int shift) {
    int i = 0;
    while (src[i] != 0) {
        char c = src[i];
        if (c >= 'a' && c <= 'z') {
            int val = c - 'a';
            val = (val + shift) % 26;
            if (val < 0) val = val + 26;
            dst[i] = 'a' + val;
        } else if (c >= 'A' && c <= 'Z') {
            int val = c - 'A';
            val = (val + shift) % 26;
            if (val < 0) val = val + 26;
            dst[i] = 'A' + val;
        } else {
            dst[i] = c;
        }
        i = i + 1;
    }
    dst[i] = 0;
}

void caesar_decrypt(char *src, char *dst, int shift) {
    caesar_encrypt(src, dst, 26 - (shift % 26));
}

int crack_caesar(char *ciphertext) {
    int freq[26];
    int i;
    int len = str_len(ciphertext);
    int best_shift = 0;
    int best_score = -1;

    char common[6];
    common[0] = 'e';
    common[1] = 't';
    common[2] = 'a';
    common[3] = 'o';
    common[4] = 'i';
    common[5] = 0;

    int shift = 0;
    while (shift < 26) {
        i = 0;
        while (i < 26) {
            freq[i] = 0;
            i = i + 1;
        }
        i = 0;
        while (i < len) {
            char c = ciphertext[i];
            if (c >= 'a' && c <= 'z') {
                int idx = (c - 'a' - shift + 26) % 26;
                freq[idx] = freq[idx] + 1;
            } else if (c >= 'A' && c <= 'Z') {
                int idx = (c - 'A' - shift + 26) % 26;
                freq[idx] = freq[idx] + 1;
            }
            i = i + 1;
        }
        int score = freq['e' - 'a'] + freq['t' - 'a'] + freq['a' - 'a'] + freq['o' - 'a'] + freq['i' - 'a'];
        if (score > best_score) {
            best_score = score;
            best_shift = shift;
        }
        shift = shift + 1;
    }
    return best_shift;
}

void rot13(char *src, char *dst) {
    caesar_encrypt(src, dst, 13);
}

int main() {
    char buf[100];
    char buf2[100];

    caesar_encrypt("hello", buf, 3);
    printf("encrypt hello shift 3: %s\n", buf);
    // EXPECT: encrypt hello shift 3: khoor

    caesar_decrypt(buf, buf2, 3);
    printf("decrypt back: %s\n", buf2);
    // EXPECT: decrypt back: hello

    caesar_encrypt("abc", buf, 1);
    printf("encrypt abc shift 1: %s\n", buf);
    // EXPECT: encrypt abc shift 1: bcd

    caesar_encrypt("xyz", buf, 3);
    printf("encrypt xyz shift 3: %s\n", buf);
    // EXPECT: encrypt xyz shift 3: abc

    caesar_encrypt("Hello World", buf, 5);
    printf("encrypt Hello World shift 5: %s\n", buf);
    // EXPECT: encrypt Hello World shift 5: Mjqqt Btwqi

    caesar_decrypt(buf, buf2, 5);
    printf("decrypt back: %s\n", buf2);
    // EXPECT: decrypt back: Hello World

    caesar_encrypt("abcdefghijklmnopqrstuvwxyz", buf, 13);
    printf("rot13 alphabet: %s\n", buf);
    // EXPECT: rot13 alphabet: nopqrstuvwxyzabcdefghijklm

    rot13("hello", buf);
    printf("rot13 hello: %s\n", buf);
    // EXPECT: rot13 hello: uryyb

    rot13(buf, buf2);
    printf("rot13 twice: %s\n", buf2);
    // EXPECT: rot13 twice: hello

    caesar_encrypt("attack at dawn", buf, 7);
    printf("encrypted: %s\n", buf);
    // EXPECT: encrypted: haahjr ha khdu

    caesar_decrypt(buf, buf2, 7);
    printf("decrypted: %s\n", buf2);
    // EXPECT: decrypted: attack at dawn

    int round_trip_ok = str_equal("attack at dawn", buf2);
    printf("round trip: %d\n", round_trip_ok);
    // EXPECT: round trip: 1

    caesar_encrypt("the quick brown fox", buf, 10);
    int guessed = crack_caesar(buf);
    printf("cracked shift: %d\n", guessed);
    // EXPECT: cracked shift: 10

    caesar_encrypt("a", buf, 25);
    printf("encrypt a shift 25: %s\n", buf);
    // EXPECT: encrypt a shift 25: z

    caesar_encrypt("z", buf, 1);
    printf("encrypt z shift 1: %s\n", buf);
    // EXPECT: encrypt z shift 1: a

    printf("done\n");
    // EXPECT: done

    return 0;
}
