// stress_069_char_torture.c - Character type stress test
// EXPECT: char_add=200
// EXPECT: char_sub=50
// EXPECT: char_mul=100
// EXPECT: char_wrap_up=44
// EXPECT: char_wrap_down=212
// EXPECT: byte_buffer=48 65 6C 6C 6F
// EXPECT: signed_char_neg=-100
// EXPECT: signed_char_pos=100
// EXPECT: unsigned_char_max=255
// EXPECT: unsigned_char_zero=0
// EXPECT: char_switch=digit
// EXPECT: char_compare_lt=1
// EXPECT: char_compare_eq=1
// EXPECT: char_compare_gt=1
// EXPECT: toupper_test=HELLO WORLD
// EXPECT: tolower_test=hello world
// EXPECT: is_alpha=1 1 0 0 1
// EXPECT: is_digit=0 0 1 0 0
// EXPECT: char_as_int_sum=550
// EXPECT: caesar_encrypt=Khoor Zruog
// EXPECT: caesar_decrypt=Hello World
// EXPECT: rot13=Uryyb Jbeyq
// EXPECT: hex_encode=4142434445
// EXPECT: char_freq_a=3
// EXPECT: char_freq_e=2
// EXPECT: atoi_manual=12345
// EXPECT: itoa_manual=6789
// EXPECT: char_array_reverse=dcba
// EXPECT: char_bitfield=01000001
// EXPECT: char_xor_swap=BA
// EXPECT: null_term_len=13
// EXPECT: char_escape_test=9 10 13 0

int printf(const char *fmt, ...);

// --- Char arithmetic ---
void test_char_arithmetic(void) {
    unsigned char a = 100;
    unsigned char b = 100;
    // Addition: promoted to int, result truncated back to unsigned char
    int sum = a + b;
    printf("char_add=%d\n", sum);

    unsigned char c = 150;
    unsigned char d = 100;
    int diff = c - d;
    printf("char_sub=%d\n", diff);

    unsigned char e = 10;
    unsigned char f = 10;
    int prod = e * f;
    printf("char_mul=%d\n", prod);
}

// --- Char wrapping ---
void test_char_wrap(void) {
    unsigned char uc = 255;
    uc = uc + 45; // wraps: (255+45) % 256 = 300 % 256 = 44
    printf("char_wrap_up=%d\n", (int)uc);

    unsigned char uc2 = 0;
    uc2 = uc2 - 44; // wraps: (0-44) % 256 = 212
    printf("char_wrap_down=%d\n", (int)uc2);
}

// --- Char array as byte buffer ---
void test_byte_buffer(void) {
    unsigned char buf[5] = {0x48, 0x65, 0x6C, 0x6C, 0x6F}; // "Hello"
    printf("byte_buffer=");
    int i;
    for (i = 0; i < 5; i++) {
        if (i > 0) printf(" ");
        // Print as hex manually
        unsigned char hi = buf[i] >> 4;
        unsigned char lo = buf[i] & 0x0F;
        printf("%c%c", hi < 10 ? '0' + hi : 'A' + hi - 10,
                       lo < 10 ? '0' + lo : 'A' + lo - 10);
    }
    printf("\n");
}

// --- Signed vs unsigned char ---
void test_signed_unsigned_char(void) {
    signed char sc = -100;
    printf("signed_char_neg=%d\n", (int)sc);

    signed char sc2 = 100;
    printf("signed_char_pos=%d\n", (int)sc2);

    unsigned char uc = 255;
    printf("unsigned_char_max=%d\n", (int)uc);

    unsigned char uc2 = 0;
    printf("unsigned_char_zero=%d\n", (int)uc2);
}

// --- Char in switch ---
void test_char_switch(void) {
    char c = '5';
    switch (c) {
        case 'a': case 'b': case 'c': case 'd': case 'e':
        case 'f': case 'g': case 'h': case 'i': case 'j':
        case 'k': case 'l': case 'm': case 'n': case 'o':
        case 'p': case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x': case 'y':
        case 'z':
            printf("char_switch=lower\n");
            break;
        case 'A': case 'B': case 'C': case 'D': case 'E':
        case 'F': case 'G': case 'H': case 'I': case 'J':
        case 'K': case 'L': case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X': case 'Y':
        case 'Z':
            printf("char_switch=upper\n");
            break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            printf("char_switch=digit\n");
            break;
        default:
            printf("char_switch=other\n");
            break;
    }
}

// --- Char comparison ---
void test_char_compare(void) {
    printf("char_compare_lt=%d\n", 'A' < 'Z');
    printf("char_compare_eq=%d\n", 'M' == 'M');
    printf("char_compare_gt=%d\n", 'z' > 'a');
}

// --- Manual toupper/tolower ---
char my_toupper(char c) {
    if (c >= 'a' && c <= 'z') return c - 32;
    return c;
}

char my_tolower(char c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

void str_toupper(char *dst, const char *src) {
    while (*src) {
        *dst = my_toupper(*src);
        dst++;
        src++;
    }
    *dst = 0;
}

void str_tolower(char *dst, const char *src) {
    while (*src) {
        *dst = my_tolower(*src);
        dst++;
        src++;
    }
    *dst = 0;
}

void test_case_conversion(void) {
    char upper[20];
    char lower[20];
    str_toupper(upper, "hello world");
    str_tolower(lower, "HELLO WORLD");
    printf("toupper_test=%s\n", upper);
    printf("tolower_test=%s\n", lower);
}

// --- Character classification ---
int my_isalpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int my_isdigit(char c) {
    return c >= '0' && c <= '9';
}

void test_classification(void) {
    char test_chars[5] = {'a', 'Z', '5', ' ', 'M'};
    printf("is_alpha=");
    int i;
    for (i = 0; i < 5; i++) {
        if (i > 0) printf(" ");
        printf("%d", my_isalpha(test_chars[i]));
    }
    printf("\n");

    printf("is_digit=");
    for (i = 0; i < 5; i++) {
        if (i > 0) printf(" ");
        printf("%d", my_isdigit(test_chars[i]));
    }
    printf("\n");
}

// --- Char used as small integer ---
void test_char_as_int(void) {
    char values[10];
    int i;
    for (i = 0; i < 10; i++) {
        values[i] = (char)((i + 1) * 10); // 10,20,...,100
    }
    int sum = 0;
    for (i = 0; i < 10; i++) {
        sum += (int)(unsigned char)values[i];
    }
    printf("char_as_int_sum=%d\n", sum); // 10+20+...+100 = 550
}

// --- Caesar cipher ---
void caesar_encrypt(char *dst, const char *src, int shift) {
    while (*src) {
        char c = *src;
        if (c >= 'A' && c <= 'Z') {
            c = 'A' + (c - 'A' + shift) % 26;
        } else if (c >= 'a' && c <= 'z') {
            c = 'a' + (c - 'a' + shift) % 26;
        }
        *dst = c;
        dst++;
        src++;
    }
    *dst = 0;
}

void caesar_decrypt(char *dst, const char *src, int shift) {
    caesar_encrypt(dst, src, 26 - shift);
}

void test_caesar(void) {
    char encrypted[20];
    char decrypted[20];
    caesar_encrypt(encrypted, "Hello World", 3);
    printf("caesar_encrypt=%s\n", encrypted);
    caesar_decrypt(decrypted, encrypted, 3);
    printf("caesar_decrypt=%s\n", decrypted);
}

// --- ROT13 ---
void rot13(char *dst, const char *src) {
    caesar_encrypt(dst, src, 13);
}

void test_rot13(void) {
    char result[20];
    rot13(result, "Hello World");
    printf("rot13=%s\n", result);
}

// --- Hex encode ---
void hex_encode(char *dst, const unsigned char *src, int len) {
    int i;
    for (i = 0; i < len; i++) {
        unsigned char hi = src[i] >> 4;
        unsigned char lo = src[i] & 0x0F;
        dst[i * 2] = hi < 10 ? '0' + hi : 'A' + hi - 10;
        dst[i * 2 + 1] = lo < 10 ? '0' + lo : 'A' + lo - 10;
    }
    dst[len * 2] = 0;
}

void test_hex_encode(void) {
    unsigned char data[5] = {0x41, 0x42, 0x43, 0x44, 0x45}; // "ABCDE"
    char hex[11];
    hex_encode(hex, data, 5);
    printf("hex_encode=%s\n", hex);
}

// --- Character frequency counting ---
// "abcdeafgea" -> a=3 (pos 0,5,9), e=2 (pos 4,8)
void test_char_freq(void) {
    char *text = "abcdeafgea";
    int freq[256];
    int i;
    for (i = 0; i < 256; i++) freq[i] = 0;

    char *p = text;
    while (*p) {
        freq[(unsigned char)*p]++;
        p++;
    }
    printf("char_freq_a=%d\n", freq['a']);
    printf("char_freq_e=%d\n", freq['e']);
}

// --- Manual atoi ---
int my_atoi(const char *s) {
    int result = 0;
    int sign = 1;
    if (*s == '-') { sign = -1; s++; }
    while (*s >= '0' && *s <= '9') {
        result = result * 10 + (*s - '0');
        s++;
    }
    return result * sign;
}

void test_atoi(void) {
    printf("atoi_manual=%d\n", my_atoi("12345"));
}

// --- Manual itoa ---
void my_itoa(int n, char *buf) {
    char tmp[20];
    int i = 0;
    int neg = 0;
    if (n < 0) { neg = 1; n = -n; }
    if (n == 0) { tmp[i++] = '0'; }
    while (n > 0) {
        tmp[i++] = '0' + (n % 10);
        n /= 10;
    }
    int j = 0;
    if (neg) buf[j++] = '-';
    while (i > 0) {
        buf[j++] = tmp[--i];
    }
    buf[j] = 0;
}

void test_itoa(void) {
    char buf[20];
    my_itoa(6789, buf);
    printf("itoa_manual=%s\n", buf);
}

// --- Char array reverse ---
void reverse_str(char *s) {
    int len = 0;
    while (s[len]) len++;
    int i;
    for (i = 0; i < len / 2; i++) {
        char tmp = s[i];
        s[i] = s[len - 1 - i];
        s[len - 1 - i] = tmp;
    }
}

void test_reverse(void) {
    char s[5] = {'a', 'b', 'c', 'd', 0};
    reverse_str(s);
    printf("char_array_reverse=%s\n", s);
}

// --- Print char as binary ---
void print_binary(unsigned char c) {
    int i;
    for (i = 7; i >= 0; i--) {
        printf("%d", (c >> i) & 1);
    }
}

void test_binary(void) {
    printf("char_bitfield=");
    print_binary('A'); // 0x41 = 01000001
    printf("\n");
}

// --- XOR swap ---
void test_xor_swap(void) {
    char a = 'A';
    char b = 'B';
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
    printf("char_xor_swap=%c%c\n", a, b); // B, A
}

// --- Null-terminated string length ---
int my_strlen(const char *s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}

void test_strlen(void) {
    printf("null_term_len=%d\n", my_strlen("Hello, World!"));
}

// --- Escape character handling ---
void test_escape_chars(void) {
    char tab = '\t';
    char newline = '\n';
    char cr = '\r';
    char null = '\0';
    printf("char_escape_test=%d %d %d %d\n",
           (int)tab, (int)newline, (int)cr, (int)null);
}

int main(void) {
    test_char_arithmetic();
    test_char_wrap();
    test_byte_buffer();
    test_signed_unsigned_char();
    test_char_switch();
    test_char_compare();
    test_case_conversion();
    test_classification();
    test_char_as_int();
    test_caesar();
    test_rot13();
    test_hex_encode();

    test_char_freq();
    test_atoi();
    test_itoa();
    test_reverse();
    test_binary();
    test_xor_swap();
    test_strlen();
    test_escape_chars();
    return 0;
}
