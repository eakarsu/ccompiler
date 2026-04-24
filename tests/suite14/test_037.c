int printf(const char *fmt, ...);

/* Morse code encoder/decoder */
/* Morse stored as int: 0=dot, 1=dash, length stored separately */
/* We encode A-Z (0-25) and 0-9 (26-35) */

struct morse_entry {
    int pattern;   /* bits: 0=dot, 1=dash, LSB first */
    int length;    /* number of dots/dashes */
};

struct morse_entry morse_table[36];

void init_morse(void) {
    /* A: .- */  morse_table[0].pattern = 2; morse_table[0].length = 2;    /* 0b10 = dash,dot => .- */
    /* B: -... */ morse_table[1].pattern = 1; morse_table[1].length = 4;   /* 0b0001 */
    /* C: -.-. */ morse_table[2].pattern = 5; morse_table[2].length = 4;   /* 0b0101 */
    /* D: -.. */  morse_table[3].pattern = 1; morse_table[3].length = 3;   /* 0b001 */
    /* E: . */    morse_table[4].pattern = 0; morse_table[4].length = 1;   /* 0b0 */
    /* F: ..-. */ morse_table[5].pattern = 4; morse_table[5].length = 4;   /* 0b0100 */
    /* G: --. */  morse_table[6].pattern = 3; morse_table[6].length = 3;   /* 0b011 */
    /* H: .... */ morse_table[7].pattern = 0; morse_table[7].length = 4;   /* 0b0000 */
    /* I: .. */   morse_table[8].pattern = 0; morse_table[8].length = 2;   /* 0b00 */
    /* J: .--- */ morse_table[9].pattern = 14; morse_table[9].length = 4;  /* 0b1110 */
    /* K: -.- */  morse_table[10].pattern = 5; morse_table[10].length = 3; /* 0b101 */
    /* L: .-.. */ morse_table[11].pattern = 2; morse_table[11].length = 4; /* 0b0010 */
    /* M: -- */   morse_table[12].pattern = 3; morse_table[12].length = 2; /* 0b11 */
    /* N: -. */   morse_table[13].pattern = 1; morse_table[13].length = 2; /* 0b01 */
    /* O: --- */  morse_table[14].pattern = 7; morse_table[14].length = 3; /* 0b111 */
    /* P: .--. */ morse_table[15].pattern = 6; morse_table[15].length = 4; /* 0b0110 */
    /* Q: --.- */ morse_table[16].pattern = 11; morse_table[16].length = 4;/* 0b1011 */
    /* R: .-. */  morse_table[17].pattern = 2; morse_table[17].length = 3; /* 0b010 */
    /* S: ... */  morse_table[18].pattern = 0; morse_table[18].length = 3; /* 0b000 */
    /* T: - */    morse_table[19].pattern = 1; morse_table[19].length = 1; /* 0b1 */
    /* U: ..- */  morse_table[20].pattern = 4; morse_table[20].length = 3; /* 0b100 */
    /* V: ...- */ morse_table[21].pattern = 8; morse_table[21].length = 4; /* 0b1000 */
    /* W: .-- */  morse_table[22].pattern = 6; morse_table[22].length = 3; /* 0b110 */
    /* X: -..- */ morse_table[23].pattern = 9; morse_table[23].length = 4; /* 0b1001 */
    /* Y: -.-- */ morse_table[24].pattern = 13; morse_table[24].length = 4;/* 0b1101 */
    /* Z: --.. */ morse_table[25].pattern = 3; morse_table[25].length = 4; /* 0b0011 */
    /* 0: ----- */ morse_table[26].pattern = 31; morse_table[26].length = 5;/* 0b11111 */
    /* 1: .---- */ morse_table[27].pattern = 30; morse_table[27].length = 5;/* 0b11110 */
    /* 2: ..--- */ morse_table[28].pattern = 28; morse_table[28].length = 5;/* 0b11100 */
    /* 3: ...-- */ morse_table[29].pattern = 24; morse_table[29].length = 5;/* 0b11000 */
    /* 4: ....- */ morse_table[30].pattern = 16; morse_table[30].length = 5;/* 0b10000 */
    /* 5: ..... */ morse_table[31].pattern = 0; morse_table[31].length = 5; /* 0b00000 */
    /* 6: -.... */ morse_table[32].pattern = 1; morse_table[32].length = 5; /* 0b00001 */
    /* 7: --... */ morse_table[33].pattern = 3; morse_table[33].length = 5; /* 0b00011 */
    /* 8: ---.. */ morse_table[34].pattern = 7; morse_table[34].length = 5; /* 0b00111 */
    /* 9: ----. */ morse_table[35].pattern = 15; morse_table[35].length = 5;/* 0b01111 */
}

int char_to_index(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a';
    if (c >= '0' && c <= '9') return c - '0' + 26;
    return -1;
}

/* Print morse for a single character */
void print_morse_char(int idx) {
    if (idx < 0 || idx >= 36) return;
    int pat = morse_table[idx].pattern;
    int len = morse_table[idx].length;
    int i;
    for (i = 0; i < len; i++) {
        if (pat & (1 << i)) {
            printf("-");
        } else {
            printf(".");
        }
    }
}

/* Encode a string to morse dot/dash representation */
void encode_morse(char *text, int len) {
    int i;
    for (i = 0; i < len; i++) {
        if (text[i] == ' ') {
            printf(" / ");
        } else {
            int idx = char_to_index(text[i]);
            if (idx >= 0) {
                print_morse_char(idx);
                if (i + 1 < len && text[i+1] != ' ') printf(" ");
            }
        }
    }
}

/* Decode morse: given pattern and length, find character index */
int decode_morse_char(int pattern, int length) {
    int i;
    for (i = 0; i < 36; i++) {
        if (morse_table[i].pattern == pattern && morse_table[i].length == length) {
            return i;
        }
    }
    return -1;
}

int index_to_char(int idx) {
    if (idx >= 0 && idx < 26) return 'A' + idx;
    if (idx >= 26 && idx < 36) return '0' + (idx - 26);
    return '?';
}

int total_dots_dashes(int idx) {
    return morse_table[idx].length;
}

int count_dashes(int idx) {
    int pat = morse_table[idx].pattern;
    int len = morse_table[idx].length;
    int count = 0;
    int i;
    for (i = 0; i < len; i++) {
        if (pat & (1 << i)) count = count + 1;
    }
    return count;
}

int main(void) {
    init_morse();

    /* Print morse for SOS */
    printf("SOS: ");
    print_morse_char(18); /* S = ... */
    printf(" ");
    print_morse_char(14); /* O = --- */
    printf(" ");
    print_morse_char(18); /* S = ... */
    printf("\n");
    // EXPECT: SOS: ... --- ...

    /* Print morse for HI */
    printf("HI: ");
    print_morse_char(7); /* H = .... */
    printf(" ");
    print_morse_char(8); /* I = .. */
    printf("\n");
    // EXPECT: HI: .... ..

    /* Decode tests */
    int d1 = decode_morse_char(0, 3);  /* ... = S */
    printf("decode ...: %c\n", index_to_char(d1));
    // EXPECT: decode ...: S

    int d2 = decode_morse_char(7, 3);  /* --- = O */
    printf("decode ---: %c\n", index_to_char(d2));
    // EXPECT: decode ---: O

    int d3 = decode_morse_char(2, 2);  /* .- = A */
    printf("decode .-: %c\n", index_to_char(d3));
    // EXPECT: decode .-: A

    int d4 = decode_morse_char(1, 1);  /* - = T */
    printf("decode -: %c\n", index_to_char(d4));
    // EXPECT: decode -: T

    int d5 = decode_morse_char(0, 1);  /* . = E */
    printf("decode .: %c\n", index_to_char(d5));
    // EXPECT: decode .: E

    /* Roundtrip: encode then decode each letter */
    int all_ok = 1;
    int i;
    for (i = 0; i < 36; i++) {
        int decoded = decode_morse_char(morse_table[i].pattern, morse_table[i].length);
        if (decoded != i) all_ok = 0;
    }
    printf("roundtrip all: %d\n", all_ok);
    // EXPECT: roundtrip all: 1

    /* Count total symbols for E (shortest = 1) and 0 (longest = 5) */
    printf("E length: %d\n", total_dots_dashes(4));
    // EXPECT: E length: 1
    printf("T length: %d\n", total_dots_dashes(19));
    // EXPECT: T length: 1
    printf("0 length: %d\n", total_dots_dashes(26));
    // EXPECT: 0 length: 5

    /* Count dashes in M (--) */
    printf("M dashes: %d\n", count_dashes(12));
    // EXPECT: M dashes: 2

    /* Count dashes in S (...) = 0 */
    printf("S dashes: %d\n", count_dashes(18));
    // EXPECT: S dashes: 0

    /* Count dashes in 0 (-----) = 5 */
    printf("0 dashes: %d\n", count_dashes(26));
    // EXPECT: 0 dashes: 5

    /* char_to_index tests */
    printf("idx A: %d\n", char_to_index('A'));
    // EXPECT: idx A: 0
    printf("idx Z: %d\n", char_to_index('Z'));
    // EXPECT: idx Z: 25
    printf("idx 0: %d\n", char_to_index('0'));
    // EXPECT: idx 0: 26
    printf("idx 9: %d\n", char_to_index('9'));
    // EXPECT: idx 9: 35
    printf("idx ?: %d\n", char_to_index('?'));
    // EXPECT: idx ?: -1

    /* Average morse length */
    int total_len = 0;
    for (i = 0; i < 26; i++) {
        total_len = total_len + morse_table[i].length;
    }
    printf("avg letter len (x26): %d\n", total_len);
    // EXPECT: avg letter len (x26): 82

    printf("done\n");
    // EXPECT: done
    return 0;
}
