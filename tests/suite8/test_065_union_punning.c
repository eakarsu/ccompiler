int printf(const char *fmt, ...);

union IntBytes {
    int value;
    char bytes[4];
};

union WordParts {
    int word;
    char parts[4];
};

union Converter {
    int as_int;
    char as_chars[4];
};

struct PackedFlags {
    int flags;
};

int get_byte(int value, int which) {
    union IntBytes ib;
    ib.value = value;
    return ib.bytes[which] & 255;
}

int set_byte(int value, int which, int new_byte) {
    union IntBytes ib;
    ib.value = value;
    ib.bytes[which] = (char)new_byte;
    return ib.value;
}

int has_flag(struct PackedFlags *pf, int bit) {
    return (pf->flags >> bit) & 1;
}

void set_flag(struct PackedFlags *pf, int bit) {
    pf->flags = pf->flags | (1 << bit);
}

void clear_flag(struct PackedFlags *pf, int bit) {
    pf->flags = pf->flags & ~(1 << bit);
}

int count_flags(struct PackedFlags *pf) {
    int count = 0;
    int val = pf->flags;
    while (val) {
        count = count + (val & 1);
        val = val >> 1;
    }
    return count;
}

void swap_bytes(union Converter *c, int i, int j) {
    char tmp = c->as_chars[i];
    c->as_chars[i] = c->as_chars[j];
    c->as_chars[j] = tmp;
}

int build_from_bytes(int b0, int b1, int b2, int b3) {
    union IntBytes ib;
    ib.bytes[0] = (char)b0;
    ib.bytes[1] = (char)b1;
    ib.bytes[2] = (char)b2;
    ib.bytes[3] = (char)b3;
    return ib.value;
}

int extract_nibble(int value, int which) {
    int shift = which * 4;
    return (value >> shift) & 15;
}

int main() {
    union IntBytes ib;
    ib.value = 0;

    ib.bytes[0] = 1;
    ib.bytes[1] = 0;
    ib.bytes[2] = 0;
    ib.bytes[3] = 0;
    // EXPECT: After setting byte 0 to 1: 1
    printf("After setting byte 0 to 1: %d\n", ib.value);

    ib.value = 0;
    ib.bytes[1] = 1;
    // EXPECT: After setting byte 1 to 1: 256
    printf("After setting byte 1 to 1: %d\n", ib.value);

    ib.value = 0;
    ib.bytes[2] = 1;
    // EXPECT: After setting byte 2 to 1: 65536
    printf("After setting byte 2 to 1: %d\n", ib.value);

    int b0 = get_byte(258, 0);
    int b1 = get_byte(258, 1);
    // EXPECT: 258 byte0=2, byte1=1
    printf("258 byte0=%d, byte1=%d\n", b0, b1);

    int modified = set_byte(255, 1, 1);
    // EXPECT: Modified: 511
    printf("Modified: %d\n", modified);

    struct PackedFlags pf;
    pf.flags = 0;
    set_flag(&pf, 0);
    set_flag(&pf, 2);
    set_flag(&pf, 4);
    // EXPECT: Flags value: 21
    printf("Flags value: %d\n", pf.flags);
    // EXPECT: Flag 0: 1
    printf("Flag 0: %d\n", has_flag(&pf, 0));
    // EXPECT: Flag 1: 0
    printf("Flag 1: %d\n", has_flag(&pf, 1));
    // EXPECT: Flag 2: 1
    printf("Flag 2: %d\n", has_flag(&pf, 2));
    // EXPECT: Flag count: 3
    printf("Flag count: %d\n", count_flags(&pf));

    clear_flag(&pf, 2);
    // EXPECT: After clear bit2: 17
    printf("After clear bit2: %d\n", pf.flags);
    // EXPECT: Flag count after: 2
    printf("Flag count after: %d\n", count_flags(&pf));

    int built = build_from_bytes(1, 2, 0, 0);
    // EXPECT: Built from bytes: 513
    printf("Built from bytes: %d\n", built);

    // EXPECT: Nibble 0 of 171: 11
    printf("Nibble 0 of 171: %d\n", extract_nibble(171, 0));
    // EXPECT: Nibble 1 of 171: 10
    printf("Nibble 1 of 171: %d\n", extract_nibble(171, 1));

    union Converter conv;
    conv.as_int = 258;
    swap_bytes(&conv, 0, 1);
    // EXPECT: After swap bytes 0,1 of 258: 513
    printf("After swap bytes 0,1 of 258: %d\n", conv.as_int);

    return 0;
}
